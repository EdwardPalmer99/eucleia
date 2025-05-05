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
    : BaseParser(Tokenizer::build(fpath)),
      _loopParser(*this), /* TODO: - initialize in a method */
      _controlFlowParser(*this),
      _blockParser(*this),
      _unaryParser(*this),
      _dataTypeParser(*this),
      _fileInfo(fpath)
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


BaseNode *FileParser::parseImport()
{
    skip("import");

    auto token = _tokens.front();

    if (token.type() == Token::String)
        return parseFileImport();
    else if (equals(Token::Operator, "<"))
        return parseLibraryImport();
    else
    {
        unexpectedToken();
        exit(0);
    }
}

/// import "path_to_some_file"
///
/// Imports a file and its functions into this scope.
FileNode *FileParser::parseFileImport()
{
    // File name token:
    auto token = _tokens.dequeue();
    assert(token.type() == Token::String);

    // Check: has file already been imported somewhere? If it has then we don't
    // want to import it a second time! (i.e. A imports B, C and B imports C. In
    // this case, PARSE A set[A]--> PARSE B set[A,B]--> PARSE C set[A,B,C].
    if (ParserData::instance().isImported(token, ParserDataImpl::File))
    {
        return new FileNode(); // Return "empty file".
    }

    ParserData::instance().addImport(token, ParserDataImpl::File);

    // Build the file path:
    std::string filePath = _fileInfo.dirPath + token;
    Logger::debug("importing file: " + filePath);

    auto ast = FileParser(filePath).buildAST(); // NB: don't use static method as this will clear loaded modules/files.
    if (!ast)
    {
        ThrowException("failed to import file with path " + filePath);
    }

    return ast;
}

/// import <io> or import <math>
///
/// This is for importing functions from a stdlib as opposed to user-defined functions
/// into this scope.

ModuleNode *FileParser::parseLibraryImport()
{
    skip("<");

    auto token = _tokens.dequeue();
    assert(token.type() == Token::Variable);

    skip(">");

    if (ParserData::instance().isImported(token, ParserDataImpl::Module))
    {
        return new ModuleNode(); // Return "empty module".
    }

    ParserData::instance().addImport(token, ParserDataImpl::Module);

    Logger::debug("importing library: " + token);

    return EucleiaModuleLoader::getModuleInstance(token);
}


#pragma mark - *** Simple Types ***


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
    skip("&");

    Token referenceNameToken = _tokens.dequeue();
    assert(referenceNameToken.type() == Token::Variable);

    skip("=");

    Token boundVariableNameToken = _tokens.dequeue();
    assert(boundVariableNameToken.type() == Token::Variable);

    return new AddReferenceVariableNode(referenceNameToken, boundVariableNameToken, boundVariableType);
}


/// [VARIABLE_NAME]
BaseNode *FileParser::parseVariableName()
{
    Token token = _tokens.dequeue();
    assert(token.type() == Token::Variable);

    return new LookupVariableNode(token);
}


#pragma mark - *** Functions ***

/// func test(a, b, c)
/// {
///		[BODY]
/// }
FunctionNode *FileParser::parseFunctionDefinition()
{
    skip("func");

    auto funcName = parseVariableName();
    auto funcArgs = parseDelimited("(", ")", ",", std::bind(&FileParser::parseVariableDefinition, this)); // Func variables.
    auto funcBody = _blockParser.parseBlock();

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
    skip("struct");

    auto structTypeName = _tokens.dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Keyword, "extends"))
    {
        std::string structParentTypeName = "";

        if (equals(Token::Keyword, "extends"))
        {
            skip("extends");

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
        if (equals(Token::Operator, "&"))
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
    skip("class");

    auto classTypeName = _tokens.dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Keyword, "extends"))
    {
        // *** Inheritance: class SomeClass(ParentClass) ***
        std::string classParentTypeName = "";

        if (equals(Token::Keyword, "extends"))
        {
            skip("extends");

            classParentTypeName = _tokens.dequeue();
        }

        // NB: have to be a bit careful with parseBlock. If there is only one node, it will return that!
        ProgramNode *classBody = static_cast<ProgramNode *>(_blockParser.parseBlock(false));

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
        if (equals(Token::Operator, "&"))
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

    skip(".");

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

    skip("[");

    auto arrayIndex = static_cast<AddIntNode *>(parseExpression());

    skip("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}


/// [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"].
AddArrayNode *FileParser::parseArray()
{
    auto programNodes = parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, this));

    auto nodesVector = programNodes->releaseNodes();

    delete programNodes;

    return new AddArrayNode(nodesVector);
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

    return equals(Token::Punctuation, "(") ? parseFunctionCall(expr) : expr;
}


BaseNode *FileParser::maybeArrayAccess(ParseMethod expression)
{
    auto expr = expression(); // Possible array variable name.

    return equals(Token::Punctuation, "[") ? parseArrayAccessor(expr) : expr;
}


BaseNode *FileParser::maybeFunctionCallOrArrayAccess(ParseMethod expression)
{
    auto expr = expression();
    assert(expr != nullptr);

    if (!_tokens.empty())
    {
        if (equals(Token::Punctuation, "("))
            return parseFunctionCall(expr);
        else if (equals(Token::Punctuation, "["))
            return parseArrayAccessor(expr);
        else if (equals(Token::Punctuation, "."))
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
    if (equals(Token::Punctuation, "("))
        return parseBrackets();
    else if (equals(Token::Punctuation, "["))
        return parseArray();
    else if (equals(Token::Punctuation, "{"))
        return _blockParser.parseBlock();
    else if (equals(Token::Keyword, "true") || equals(Token::Keyword, "false"))
        return _dataTypeParser.parseBool();
    else if (equals(Token::Keyword, "while"))
        return _loopParser.parseWhile();
    else if (equals(Token::Keyword, "do"))
        return _loopParser.parseDoWhile();
    else if (equals(Token::Keyword, "for"))
        return _loopParser.parseFor();
    else if (equals(Token::Keyword, "if"))
        return _controlFlowParser.parseIf();
    else if (equals(Token::Keyword, "import"))
        return parseImport();
    else if (equals(Token::Keyword, "func")) // Functions should be defined as in C --> will need void type
        return parseFunctionDefinition();
    else if (equals(Token::Keyword, "struct"))
        return parseStruct();
    else if (equals(Token::Keyword, "class"))
        return parseClass();
    else if (isDataTypeKeyword())
        return parseVariableDefinition();
    else if (equals(Token::Keyword, "break"))
        return _controlFlowParser.parseBreak();
    else if (equals(Token::Keyword, "return"))
        return _controlFlowParser.parseReturn();

    // TODO: - split-up into separate method for unary operators.
    // Parse unary operators.
    else if (equals(Token::Operator, "!"))
        return _unaryParser.parseNot();
    // Parse prefix increment operator i.e.
    else if (equals(Token::Operator, "++"))
        return _unaryParser.parsePrefixIncrement();
    else if (equals(Token::Operator, "--"))
        return _unaryParser.parsePrefixDecrement();
    else if (equals(Token::Operator, "-"))
        return _unaryParser.parseNegation();

    const Token &token = _tokens.front();

    switch (token.type())
    {
        case Token::Variable:
            return parseVariableName();
        case Token::String:
            return _dataTypeParser.parseString();
        case Token::Int:
            return _dataTypeParser.parseInt();
        case Token::Float:
            return _dataTypeParser.parseFloat();
        default:
            unexpectedToken();
            exit(EXIT_FAILURE);
    }
}


void FileParser::skipSemicolonLineEndingIfRequired(const BaseNode &node)
{
    bool doSkipPunctuation = (node.isNodeType<ModuleNode>() || // Bit ugly.
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

    if (!doSkipPunctuation)
        skip(";");
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


bool FileParser::isDataTypeKeyword()
{
    return (Grammar::instance().isDataType(_tokens.front()));
}


void FileParser::unexpectedToken()
{
    const Token &token = _tokens.front();

    ThrowException("unexpected token: " + token);
}


BaseNode *FileParser::parseBrackets()
{
    skip("(");

    BaseNode *expression = parseExpression();

    skip(")");

    return expression;
}
