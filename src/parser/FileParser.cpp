//
//  FileParser.cpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#include "FileParser.hpp"
#include "EucleiaModules.hpp"
#include "Exceptions.hpp"
#include "Grammar.hpp"
#include "Logger.hpp"
#include "ObjectTypes.hpp"
#include "ParserData.hpp"
#include "TestModule.hpp"
#include <assert.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <stdlib.h>


FileParser::FileParser(const std::string &fpath)
    : _tokens(Tokenizer::build(fpath)),
      _fileInfo(fpath),
      _skipFunctor(_tokens) /* TODO: - need some method for init */
{
}


#pragma mark -

FileNode *FileParser::buildAST()
{
    std::vector<BaseNode *> nodes;

    while (!_tokens.empty())
    {
        auto node = parseExpression();

        nodes.push_back(node);

        skipSemicolonLineEndingIfRequired(*node);
    }

    nodes.shrink_to_fit();

    return new FileNode(nodes);
}


#pragma mark - *** Simple Types ***


BaseNode *FileParser::parseBrackets()
{
    _skipFunctor("(");

    BaseNode *expression = parseExpression();

    _skipFunctor(")");

    return expression;
}


BaseNode *FileParser::parseVariableName()
{
    return new LookupVariableNode(_tokens.dequeue());
}


/// Variable definition:
/// int/float/string/bool/array [VARIABLE_NAME]
BaseNode *FileParser::parseVariableDefinition()
{
    Token typeToken = _tokens.dequeue();
    assert(typeToken.type() == Token::Keyword);

    ObjectType typeOfObject = objectTypeForName(typeToken);

    if (_tokens.front() == "&") // Is reference.
    {
        return parseReference(typeOfObject);
    }

    Token nameToken = _tokens.dequeue();
    assert(nameToken.type() == Token::Variable);

    return new AddVariableNode(nameToken, typeOfObject);
}

/**
 * Bind a reference to an already declared variable in this scope or a parent
 * scope. We do not allow any unbound references and once bound, references
 * cannot be unbound. By default, we pass by value to functions, but the use
 * of a reference as in other languages such as C++ avoids copying.
 *
 * Reference definition:
 * VARIABLE_TO_BIND_TO_TYPE & REFERENCE_NAME = VARIABLE_TO_BIND_TO;
 */
BaseNode *FileParser::parseReference(ObjectType boundVariableType)
{
    _skipFunctor("&");

    Token referenceNameToken = _tokens.dequeue();
    assert(referenceNameToken.type() == Token::Variable);

    _skipFunctor("=");

    Token boundVariableNameToken = _tokens.dequeue();
    assert(boundVariableNameToken.type() == Token::Variable);

    return new AddReferenceVariableNode(referenceNameToken, boundVariableNameToken, boundVariableType);
}


#pragma mark - *** Functions ***

/// func test(a, b, c)
/// {
///		[BODY]
/// }
FunctionNode *FileParser::parseFunctionDefinition()
{
    _skipFunctor("func");

    auto funcName = new LookupVariableNode(_tokens.dequeue());
    auto funcArgs = parseDelimited("(", ")", ",", std::bind(&FileParser::parseVariableDefinition, this)); // Func variables.
    auto funcBody = ProgramNode::parse(*this, true);

    return new FunctionNode(funcName, funcArgs, funcBody);
}


///
/// Example: test(a, b, c);
///
FunctionCallNode *FileParser::parseFunctionCall(BaseNode *lastExpression)
{
    auto functionName = std::move(lastExpression);
    auto functionArgs = parseDelimited("(", ")", ",", std::bind(&FileParser::parseExpression, this));

    return new FunctionCallNode(functionName, functionArgs);
}


#pragma mark - *** Struct ***

/**
 * Case 1: definition of a new structure:
 *
 * struct SomeStruct
 * {
 *      int i;
 *      float f;
 *      string s;
 * };
 *
 * Case 2: instance of structure.
 *
 * struct SomeStruct aStruct;
 */
BaseNode *FileParser::parseStruct()
{
    _skipFunctor("struct");

    auto structTypeName = _tokens.dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (isPunctuation("{") || isKeyword("extends"))
    {
        std::string structParentTypeName = "";

        if (isKeyword("extends"))
        {
            _skipFunctor("extends");

            structParentTypeName = _tokens.dequeue();
        }

        auto structMemberVars = parseDelimited("{", "}", ";", std::bind(&FileParser::parseVariableDefinition, this));

        std::vector<BaseNode *> nodes = structMemberVars->releaseNodes();

        delete structMemberVars;

        std::vector<AddVariableNode *> variableDefs;
        variableDefs.reserve(nodes.size());

        for (BaseNode *node : nodes)
        {
            variableDefs.push_back(reinterpret_cast<AddVariableNode *>(node));
        }

        return new StructDefinitionObject(structTypeName, structParentTypeName, variableDefs);
    }
    else
    {
        // Case: "struct STRUCT_TYPE_NAME & STRUCT_REF_INSTANCE_NAME = STRUCT_VARIABLE_NAME_TO_BIND"
        if (isOperator("&"))
        {
            return parseReference(ObjectType::Struct);
        }

        auto structInstanceName = _tokens.dequeue();

        return new StructObject(structTypeName, structInstanceName);
    }
}

#pragma mark - *** Class ***

/**
 * Case 1: definition of a new class:
 *
 * class SomeClass
 * {
 *      int i;
 *
 *      func someFunc(int a)
        {
            i = i + 1;
            print("Hello");
            return i;
        }
 * };
 *
 *
 * Case 2: instance of the class:
 *
 * class SomeClass aClass;
 */
BaseNode *FileParser::parseClass()
{
    _skipFunctor("class");

    auto classTypeName = _tokens.dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (isPunctuation("{") || isKeyword("extends"))
    {
        // *** Inheritance: class SomeClass(ParentClass) ***
        std::string classParentTypeName = "";

        if (isKeyword("extends"))
        {
            _skipFunctor("extends");

            classParentTypeName = _tokens.dequeue();
        }

        // NB: have to be a bit careful with parseProgram. If there is only
        // one node, it will return that!
        ProgramNode *classBody = static_cast<ProgramNode *>(ProgramNode::parse(*this, false));

        std::vector<BaseNode *> nodes = classBody->releaseNodes();

        delete classBody;

        // Split-up into class variables and class methods:
        std::vector<AddVariableNode *> classVariables;
        std::vector<FunctionNode *> classMethods;

        for (BaseNode *node : nodes)
        {
            if (node->isNodeType<AddVariableNode>())
                classVariables.push_back(reinterpret_cast<AddVariableNode *>(node));
            else if (node->isNodeType<FunctionNode>())
                classMethods.push_back(reinterpret_cast<FunctionNode *>(node));
            else
                ThrowException("unexpected node type for class definition " + classTypeName);
        }

        return new ClassDefinitionObject(classTypeName, classParentTypeName, classVariables, classMethods);
    }
    else
    {
        // Case: "class CLASS_INSTANCE_NAME & CLASS_REF_NAME = CLASS_VARIABLE_NAME_TO_BIND"
        if (isOperator("&"))
        {
            return parseReference(ObjectType::Class);
        }

        auto classInstanceName = _tokens.dequeue();

        return new ClassObject(classTypeName, classInstanceName);
    }

    return nullptr;
}


/**
 * Example:
 *
 * aStruct.i --> returns int object stored in struct instance.
 */
BaseNode *FileParser::parseStructAccessor(BaseNode *lastExpression)
{
    auto instanceName = lastExpression->castNode<LookupVariableNode>().name;
    delete lastExpression; // NB: ugly.

    _skipFunctor(".");

    BaseNode *expression = maybeFunctionCall(std::bind(&FileParser::parseVariableName, this));

    if (expression->isNodeType<FunctionCallNode>()) // Method.
    {
        // NB: do NOT delete expression as owned by ClassMethodCallNode.
        return new ClassMethodCallNode(instanceName, reinterpret_cast<FunctionNode *>(expression));
    }
    else if (expression->isNodeType<LookupVariableNode>()) // Member variable.
    {
        std::string memberVariableName = expression->castNode<LookupVariableNode>().name;
        delete expression; // No longer required. Ugly.

        return new StructAccessNode(instanceName, memberVariableName);
    }
    else
    {
        ThrowException("unexpected node type while accessing struct/class member");
    }
}


#pragma mark - *** Arrays ***

///
/// Expression: array_variable[0].
///
ArrayAccessNode *FileParser::parseArrayAccessor(BaseNode *lastExpression)
{
    auto arrayName = static_cast<LookupVariableNode *>(lastExpression);

    _skipFunctor("[");

    auto arrayIndex = static_cast<AddIntNode *>(parseExpression());

    _skipFunctor("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}


#pragma mark - *** Assignment/Binary ***

BaseNode *FileParser::maybeBinary(BaseNode *leftExpression, int leftPrecedence)
{
    // Special case: tokens empty. Cannot be binary expression.
    if (_tokens.empty())
    {
        return leftExpression;
    }

    // Take a peek look at the next token. Is it an operator token. If it is then
    // we need to create a binary node.
    const Token &next = _tokens.front();

    if (next.type() == Token::Operator)
    {
        // Yes it is an operator. Does it have a higher precedence than the
        // left-hand expression?
        const int nextPrecedence = getPrecedence();
        if (nextPrecedence > leftPrecedence)
        {
            _tokens.pop(); // Move along one.

            auto rightExpression = maybeBinary(parseAtomically(), nextPrecedence);

            // Create binary or assign node.
            BaseNode *node{nullptr};

            bool isAssignNode = (next == "=");

            if (isAssignNode)
                node = new AssignNode(leftExpression, rightExpression);
            else
                node = new BinaryNode(leftExpression, rightExpression, next);

            // Wrap binary node by calling ourselves should the next operator
            // be of a greater precedence.
            return maybeBinary(std::move(node), leftPrecedence);
        }
    }

    // Default.
    return leftExpression;
}


#pragma mark - *** Maybe ***

BaseNode *FileParser::maybeFunctionCall(ParseMethod expression)
{
    auto expr = expression(); // Possible function name.

    return isPunctuation("(") ? parseFunctionCall(expr) : expr;
}


BaseNode *FileParser::maybeArrayAccess(ParseMethod expression)
{
    auto expr = expression(); // Possible array variable name.

    return isPunctuation("[") ? parseArrayAccessor(expr) : expr;
}


BaseNode *FileParser::maybeFunctionCallOrArrayAccess(ParseMethod expression)
{
    auto expr = expression();
    assert(expr != nullptr);

    if (!_tokens.empty())
    {
        if (isPunctuation("("))
            return parseFunctionCall(expr);
        else if (isPunctuation("["))
            return parseArrayAccessor(expr);
        else if (isPunctuation("."))
            return parseStructAccessor(expr);
    }

    return expr;
}


#pragma mark - *** Utility ***

BaseNode *FileParser::parseExpression()
{
    return maybeFunctionCallOrArrayAccess(std::bind(&FileParser::parseExpressionHelper, this));
}


BaseNode *FileParser::parseExpressionHelper()
{
    return maybeBinary(parseAtomically(), 0);
}


BaseNode *FileParser::parseAtomically()
{
    // 1. Call parseAtomicallyExpression() -> This may be the function name.
    // 2. Do we have a next token which is "("? --> YES --> FUNCTION CALL!
    return maybeFunctionCallOrArrayAccess(std::bind(&FileParser::parseAtomicallyExpression, this));
}


BaseNode *FileParser::parseAtomicallyExpression()
{
    // TODO: - breakup
    if (isPunctuation("("))
        return parseBrackets();
    else if (isPunctuation("["))
        return AddArrayNode::parse(*this);
    else if (isPunctuation("{"))
        return ProgramNode::parse(*this, true);
    else if (isKeyword("true") || isKeyword("false"))
        return AddBoolNode::parse(*this);
    else if (isKeyword("while"))
        return WhileNode::parse(*this);
    else if (isKeyword("do"))
        return DoWhileNode::parse(*this);
    else if (isKeyword("for"))
        return ForLoopNode::parse(*this);
    else if (isKeyword("if"))
        return IfNode::parse(*this);
    else if (isKeyword("import"))
        return FileNode::parse(*this);
    else if (isKeyword("func")) // Functions should be defined as in C --> will need void type
        return parseFunctionDefinition();
    else if (isKeyword("struct"))
        return parseStruct();
    else if (isKeyword("class"))
        return parseClass();
    else if (isDataTypeKeyword())
        return parseVariableDefinition();
    else if (isKeyword("break"))
        return BreakNode::parse(*this);
    else if (isKeyword("return"))
        return ReturnNode::parse(*this);

    // TODO: - split-up into separate method for unary operators.
    // Parse unary operators.
    else if (isOperator("!"))
        return NotNode::parse(*this);
    // Parse prefix increment operator i.e.
    else if (isOperator("++"))
        return PrefixIncrementNode::parse(*this);
    else if (isOperator("--"))
        return PrefixDecrementNode::parse(*this);
    else if (isOperator("-"))
        return parseNegation();

    const Token &token = _tokens.front();

    switch (token.type())
    {
        case Token::Variable:
            return LookupVariableNode::parse(*this);
        case Token::String:
            return AddStringNode::parse(*this);
        case Token::Int:
            return AddIntNode::parse(*this);
        case Token::Float:
            return AddFloatNode::parse(*this);
        default:
            unexpectedToken();
            exit(EXIT_FAILURE);
    }
}


NegationNode *FileParser::parseNegation()
{
    _skipFunctor("-");

    return new NegationNode(parseAtomically());
}


/// parseDelimited
///
/// Parse everything between start using the provided parser and ignoring the separators.
///
/// **Example:**
///
/// function aFunction(a, b, c) --> start = (, stop = ), separator = ,
///
ProgramNode *FileParser::parseDelimited(std::string start,
                                        std::string stop,
                                        std::string separator,
                                        ParseMethod parseMethod)
{
    _skipFunctor(start); // Skip the punctuation at the start.

    std::vector<BaseNode *> parsedNodes;

    // Iterate while we still have tokens and haven't reached stop token.
    bool firstCall = true;

    while (!_tokens.empty() && !isPunctuation(stop))
    {
        // Skip separator on each subsequent call (i.e. a, b)
        if (firstCall)
        {
            firstCall = false;
        }
        else
        {
            _skipFunctor(separator);
        }

        if (isPunctuation(stop))
        {
            break;
        }

        // Parse the token.
        parsedNodes.push_back(parseMethod());
    }

    _skipFunctor(stop); // Skip the punctuation at the end.

    // Resize to fit.
    parsedNodes.shrink_to_fit();

    return new ProgramNode(parsedNodes);
}


void FileParser::skipSemicolonLineEndingIfRequired(const BaseNode &node)
{
    bool do_skipper = (node.isNodeType<ModuleNode>() || // Bit ugly.
                       node.isNodeType<MathModuleNode>() ||
                       node.isNodeType<IOModuleNode>() ||
                       node.isNodeType<ArrayModuleNode>() ||
                       node.isNodeType<TestModule>() ||
                       node.isNodeType<FileNode>() ||
                       node.isNodeType<ProgramNode>() ||
                       node.isNodeType<IfNode>() ||
                       node.isNodeType<WhileNode>() ||
                       node.isNodeType<DoWhileNode>() ||
                       node.isNodeType<ForLoopNode>() ||
                       node.isNodeType<FunctionNode>());

    if (!do_skipper)
        _skipFunctor(";");
}


int FileParser::getPrecedence()
{
    const Token &token = _tokens.front();

    if (token.type() != Token::Operator)
    {
        return 0;
    }

    if (token == "=")
        return 1;
    else if (token == "||")
        return 2;
    else if (token == "&&")
        return 3;
    else if ((token == "<") ||
             (token == ">") ||
             (token == "<=") ||
             (token == ">=") ||
             (token == "==") ||
             (token == "!="))
        return 7;
    else if ((token == "+") ||
             (token == "-"))
        return 10;
    else if ((token == "*") ||
             (token == "/") ||
             (token == "%"))
        return 20;
    else
        return 0;
}


#pragma mark - *** is... ***

bool FileParser::isKeyword(const std::string &keyword)
{
    const Token &token = _tokens.front();

    return (token.type() == Token::Keyword && token == keyword);
}


bool FileParser::isDataTypeKeyword()
{
    return (Grammar::instance().isDataType(_tokens.front()));
}


bool FileParser::isPunctuation(const std::string &punctuation)
{
    const Token &token = _tokens.front();

    return (token.type() == Token::Punctuation && token == punctuation);
}


bool FileParser::isOperator(const std::string &operatorName)
{
    const Token &token = _tokens.front();

    return (token.type() == Token::Operator && token == operatorName);
}


#pragma mark -

void FileParser::unexpectedToken()
{
    const Token &token = _tokens.front();

    ThrowException("unexpected token: " + token);
}
