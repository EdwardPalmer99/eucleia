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
      _subParsers(*this),
      _fileInfo(fpath)
{
    // TODO: - any subparsers need to be initialized here after registration
}


#pragma mark -

FileNode *FileParser::buildAST()
{
    std::vector<BaseNode *> nodes;

    while (!tokens().empty())
    {
        auto node = parseExpression();

        nodes.push_back(node);

        skipSemicolonLineEndingIfRequired(*node);
    }

    nodes.shrink_to_fit();

    return new FileNode(nodes);
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

    auto structTypeName = tokens().dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Keyword, "extends"))
    {
        std::string structParentTypeName = "";

        if (equals(Token::Keyword, "extends"))
        {
            skip("extends");

            structParentTypeName = tokens().dequeue();
        }

        auto structMemberVars = parseDelimited("{", "}", ";", std::bind(&VariableSubParser::parseVariableDefinition, _subParsers.variable));

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
            return _subParsers.variable.parseReference(ObjectType::Struct);
        }

        auto structInstanceName = tokens().dequeue();

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

    auto classTypeName = tokens().dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Keyword, "extends"))
    {
        // *** Inheritance: class SomeClass(ParentClass) ***
        std::string classParentTypeName = "";

        if (equals(Token::Keyword, "extends"))
        {
            skip("extends");

            classParentTypeName = tokens().dequeue();
        }

        // NB: have to be a bit careful with parseBlock. If there is only one node, it will return that!
        ProgramNode *classBody = static_cast<ProgramNode *>(_subParsers.block.parseBlock(false));

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
            return _subParsers.variable.parseReference(ObjectType::Class);
        }

        auto classInstanceName = tokens().dequeue();

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

    BaseNode *expression = maybeFunctionCall(std::bind(&VariableSubParser::parseVariableName, &_subParsers.variable));

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


#pragma mark - *** Assignment/Binary ***

BaseNode *FileParser::maybeBinary(BaseNode *leftExpression, int leftPrecedence)
{
    // Special case: tokens empty. Cannot be binary expression.
    if (tokens().empty())
    {
        return leftExpression;
    }

    // Take a peek look at the next token. Is it an operator token. If it is then
    // we need to create a binary node.
    const Token &next = tokens().front();

    if (next.type() == Token::Operator)
    {
        // Yes it is an operator. Does it have a higher precedence than the
        // left-hand expression?
        const int nextPrecedence = getPrecedence();
        if (nextPrecedence > leftPrecedence)
        {
            tokens().pop(); // Move along one.

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

    return equals(Token::Punctuation, "(") ? _subParsers.function.parseFunctionCall(expr) : expr;
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

    if (!tokens().empty())
    {
        if (equals(Token::Punctuation, "("))
            return _subParsers.function.parseFunctionCall(expr);
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
        return _subParsers.dataType.parseArray();
    else if (equals(Token::Punctuation, "{"))
        return _subParsers.block.parseBlock();
    else if (equals(Token::Keyword, "true") || equals(Token::Keyword, "false"))
        return _subParsers.dataType.parseBool();
    else if (equals(Token::Keyword, "while"))
        return _subParsers.loop.parseWhile();
    else if (equals(Token::Keyword, "do"))
        return _subParsers.loop.parseDoWhile();
    else if (equals(Token::Keyword, "for"))
        return _subParsers.loop.parseFor();
    else if (equals(Token::Keyword, "if"))
        return _subParsers.controlFlow.parseIf();
    else if (equals(Token::Keyword, "import"))
        return _subParsers.import.parseImport();
    else if (equals(Token::Keyword, "func")) // Functions should be defined as in C --> will need void type
        return _subParsers.function.parseFunctionDefinition();
    else if (equals(Token::Keyword, "struct"))
        return parseStruct();
    else if (equals(Token::Keyword, "class"))
        return parseClass();
    else if (isDataTypeKeyword())
        return _subParsers.variable.parseVariableDefinition();
    else if (equals(Token::Keyword, "break"))
        return _subParsers.controlFlow.parseBreak();
    else if (equals(Token::Keyword, "return"))
        return _subParsers.controlFlow.parseReturn();

    // TODO: - split-up into separate method for unary operators.
    // Parse unary operators.
    else if (equals(Token::Operator, "!"))
        return _subParsers.unary.parseNot();
    // Parse prefix increment operator i.e.
    else if (equals(Token::Operator, "++"))
        return _subParsers.unary.parsePrefixIncrement();
    else if (equals(Token::Operator, "--"))
        return _subParsers.unary.parsePrefixDecrement();
    else if (equals(Token::Operator, "-"))
        return _subParsers.unary.parseNegation();

    const Token &token = tokens().front();

    switch (token.type())
    {
        case Token::Variable:
            return _subParsers.variable.parseVariableName();
        case Token::String:
            return _subParsers.dataType.parseString();
        case Token::Int:
            return _subParsers.dataType.parseInt();
        case Token::Float:
            return _subParsers.dataType.parseFloat();
        default:
            ThrowException("unexpected token: " + token);
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
    const Token &token = tokens().front();

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
    return (Grammar::instance().isDataType(tokens().front()));
}


BaseNode *FileParser::parseBrackets()
{
    skip("(");

    BaseNode *expression = parseExpression();

    skip(")");

    return expression;
}
