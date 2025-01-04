//
//  EucleiaParser.cpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#include "EucleiaParser.hpp"
#include "EucleiaModules.hpp"
#include "EucleiaUtility.hpp"
#include "ObjectTypes.hpp"
#include "TestModule.hpp"
#include <assert.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <stdlib.h>

std::unordered_set<std::string> importedModuleNames;
std::unordered_set<std::string> importedFileNames;


Parser::Parser(const std::string &fpath)
    : tokenizer(Tokenizer::loadFromFile(fpath)),
      fileInfo(fpath)
{
    // Add this file info so we don't accidentally import twice.
    importedFileNames.insert(fileInfo.nameWithExt);
}


#pragma mark -

FileNode *Parser::buildAST(const std::string &fpath)
{
    // Static method. Called by user. Clear imports.
    importedModuleNames.clear();
    importedFileNames.clear();

    Parser parser(fpath);
    return parser.buildAST();
}


FileNode *Parser::buildAST()
{
    std::vector<BaseNode *> nodes;

    while (!tokenizer.empty() && peekToken().type != Token::None)
    {
        auto node = parseExpression();

        nodes.push_back(node);

        skipSemicolonLineEndingIfRequired(*node);
    }

    nodes.shrink_to_fit();

    return new FileNode(nodes);
}


BaseNode *Parser::parseImport()
{
    skipKeyword("import");

    auto token = peekToken();

    if (token.type == Token::String)
        return parseFileImport();
    else if (isOperator("<"))
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
FileNode *Parser::parseFileImport()
{
    // File name token:
    auto token = nextToken();
    assert(token.type == Token::String);

    // Check: has file already been imported somewhere? If it has then we don't
    // want to import it a second time! (i.e. A imports B, C and B imports C. In
    // this case, PARSE A set[A]--> PARSE B set[A,B]--> PARSE C set[A,B,C].
    if (importedFileNames.count(token.value))
    {
        return new FileNode(); // Return "empty file".
    }

    // Build the file path:
    std::string filePath = fileInfo.dirPath + token.value;

    auto ast = Parser(filePath).buildAST(); // NB: don't use static method as this will clear loaded modules/files.
    if (!ast)
    {
        EucleiaError("failed to import file with path '%s'.", filePath.c_str());
    }

    return ast;
}

/// import <io> or import <math>
///
/// This is for importing functions from a stdlib as opposed to user-defined functions
/// into this scope.

ModuleNode *Parser::parseLibraryImport()
{
    skipOperator("<");

    auto token = nextToken();
    assert(token.type == Token::Variable);

    skipOperator(">");

    auto libraryName = token.value;

    if (importedModuleNames.count(token.value))
    {
        return new ModuleNode(); // Return "empty module".
    }

    importedModuleNames.insert(token.value);

    return EucleiaModuleLoader::getModuleInstance(libraryName);
}


#pragma mark - *** Simple Types ***

/// parseProgram
///
/// Parses a sequence of expression. If there is only one expression within the
/// brackets then we just return that.
/// TODO: - add handling for no expressions within the brackets.
BaseNode *Parser::parseProgram()
{
    ProgramNode *program = parseProgramLines();

    if (program->programNodes.size() == 1) // Return single node (more efficient).
    {
        BaseNode *first = program->releaseNodes()[0];
        delete program;

        return first;
    }

    return program;
}


AddIntNode *Parser::parseInt()
{
    Token token = nextToken();

    long intValue = strtold(token.value.c_str(), NULL);

    return new AddIntNode(intValue);
}


AddFloatNode *Parser::parseFloat()
{
    Token token = nextToken();

    double floatValue = strtof(token.value.c_str(), NULL);

    return new AddFloatNode(floatValue);
}


AddBoolNode *Parser::parseBool()
{
    Token token = nextToken();

    bool state = (token.value == "true");

    return new AddBoolNode(state);
}


AddStringNode *Parser::parseString()
{
    Token token = nextToken();

    return new AddStringNode(token.value);
}


BaseNode *Parser::parseBrackets()
{
    skipPunctuation("(");

    BaseNode *expression = parseExpression();

    skipPunctuation(")");

    return expression;
}


/// Variable definition:
/// int/float/string/bool/array [VARIABLE_NAME]
BaseNode *Parser::parseVariableDefinition()
{
    Token typeToken = nextToken();
    assert(typeToken.type == Token::Keyword);

    ObjectType typeOfObject = objectTypeForName(typeToken.value);

    if (peekToken().value == "&") // Is reference.
    {
        return parseReference(typeOfObject);
    }

    Token nameToken = nextToken();
    assert(nameToken.type == Token::Variable);

    return new AddVariableNode(nameToken.value, typeOfObject);
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
BaseNode *Parser::parseReference(ObjectType boundVariableType)
{
    skipOperator("&");

    Token referenceNameToken = nextToken();
    assert(referenceNameToken.type == Token::Variable);

    skipOperator("=");

    Token boundVariableNameToken = nextToken();
    assert(boundVariableNameToken.type == Token::Variable);

    return new AddReferenceVariableNode(referenceNameToken.value, boundVariableNameToken.value, boundVariableType);
}


/// [VARIABLE_NAME]
BaseNode *Parser::parseVariableName()
{
    Token token = nextToken();
    assert(token.type == Token::Variable);

    return new LookupVariableNode(token.value);
}


#pragma mark - *** Loops ***

/// do
/// {
/// 	[code]
/// }
/// while ([condition is true]);
DoWhileNode *Parser::parseDoWhile()
{
    skipKeyword("do");
    BaseNode *body = parseProgram();
    skipKeyword("while");
    BaseNode *condition = parseBrackets();

    return new DoWhileNode(condition, body);
}


/// while ([condition is true])
/// {
/// 	[code]
/// }
WhileNode *Parser::parseWhile()
{
    skipKeyword("while");

    BaseNode *condition = parseBrackets();
    BaseNode *body = parseProgram();

    return new WhileNode(condition, body);
}


/// for ([start]; [condition]; [update])
/// {
/// 	[code]
/// }
ForLoopNode *Parser::parseFor()
{
    skipKeyword("for");

    ProgramNode *brackets = parseDelimited("(", ")", ";", std::bind(&Parser::parseExpression, this));

    std::vector<BaseNode *> forLoopArgs = brackets->releaseNodes();

    delete brackets;

    if (forLoopArgs.size() != 3)
    {
        EucleiaError("expected 3 arguments for for-loop but got %ld.", brackets->programNodes.size());
    }

    auto start = forLoopArgs[0];
    auto condition = forLoopArgs[1];
    auto update = forLoopArgs[2];
    auto body = parseProgram();

    return new ForLoopNode(start, condition, update, body);
}


#pragma mark - *** Control Flow ***

IfNode *Parser::parseIf()
{
    // For now, only permit a single if statement.
    skipKeyword("if");

    auto condition = parseBrackets();
    auto thenDo = parseProgram();


    BaseNode *elseDo{nullptr}; // Optional.
    if (peekToken().value == "else")
    {
        skipKeyword("else");

        // Option 1: else if (condition) { [statement]; }
        if (peekToken().value == "if")
            elseDo = parseIf();
        // Option 2: else { [statement]; }
        else
            elseDo = parseProgram();
    }

    return new IfNode(condition, thenDo, elseDo);
}


BreakNode *Parser::parseBreak()
{
    skipKeyword("break");

    return new BreakNode();
}


ReturnNode *Parser::parseReturn()
{
    skipKeyword("return");

    BaseNode *returnedExpression{nullptr};

    if (!isPunctuation(";"))
    {
        returnedExpression = parseExpression();
    }

    return new ReturnNode(returnedExpression);
}


#pragma mark - *** Functions ***

/// func test(a, b, c)
/// {
///		[BODY]
/// }
FunctionNode *Parser::parseFunctionDefinition()
{
    skipKeyword("func");

    auto funcName = parseVariableName();
    auto funcArgs = parseDelimited("(", ")", ",", std::bind(&Parser::parseVariableDefinition, this)); // Func variables.
    auto funcBody = parseProgram();

    return new FunctionNode(funcName, funcArgs, funcBody);
}


///
/// Example: test(a, b, c);
///
FunctionCallNode *Parser::parseFunctionCall(BaseNode *lastExpression)
{
    auto functionName = std::move(lastExpression);
    auto functionArgs = parseDelimited("(", ")", ",", std::bind(&Parser::parseExpression, this));

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
BaseNode *Parser::parseStruct()
{
    skipKeyword("struct");

    auto structTypeName = nextToken().value;

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (isPunctuation("{") || isKeyword("extends"))
    {
        std::string structParentTypeName = "";

        if (isKeyword("extends"))
        {
            skipKeyword("extends");

            structParentTypeName = nextToken().value;
        }

        auto structMemberVars = parseDelimited("{", "}", ";", std::bind(&Parser::parseVariableDefinition, this));

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

        auto structInstanceName = nextToken().value;

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
BaseNode *Parser::parseClass()
{
    skipKeyword("class");

    auto classTypeName = nextToken().value;

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (isPunctuation("{") || isKeyword("extends"))
    {
        // *** Inheritance: class SomeClass(ParentClass) ***
        std::string classParentTypeName = "";

        if (isKeyword("extends"))
        {
            skipKeyword("extends");

            classParentTypeName = nextToken().value;
        }

        // NB: have to be a bit careful with parseProgram. If there is only
        // one node, it will return that!
        ProgramNode *classBody = parseProgramLines();

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
                EucleiaError("unexpected node type for class definition %s\n", classTypeName.c_str());
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

        auto classInstanceName = nextToken().value;

        return new ClassObject(classTypeName, classInstanceName);
    }

    return nullptr;
}


/**
 * Example:
 *
 * aStruct.i --> returns int object stored in struct instance.
 */
BaseNode *Parser::parseStructAccessor(BaseNode *lastExpression)
{
    auto instanceName = lastExpression->castNode<LookupVariableNode>().name;
    delete lastExpression; // NB: ugly.

    skipPunctuation(".");

    BaseNode *expression = maybeFunctionCall(std::bind(&Parser::parseVariableName, this));

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
        EucleiaError("unexpected node type while accessing struct/class member.");
    }
}


#pragma mark - *** Arrays ***

///
/// Expression: array_variable[0].
///
ArrayAccessNode *Parser::parseArrayAccessor(BaseNode *lastExpression)
{
    auto arrayName = static_cast<LookupVariableNode *>(lastExpression);

    skipPunctuation("[");

    auto arrayIndex = static_cast<AddIntNode *>(parseExpression());

    skipPunctuation("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}


/// [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"].
AddArrayNode *Parser::parseArray()
{
    auto programNodes = parseDelimited("[", "]", ",", std::bind(&Parser::parseExpression, this));

    auto nodesVector = programNodes->releaseNodes();

    delete programNodes;

    return new AddArrayNode(nodesVector);
}


#pragma mark - *** Assignment/Binary ***

BaseNode *Parser::maybeBinary(BaseNode *leftExpression, int leftPrecedence)
{
    // Special case: tokens empty. Cannot be binary expression.
    if (tokenizer.empty())
    {
        return leftExpression;
    }

    // Take a peek look at the next token. Is it an operator token. If it is then
    // we need to create a binary node.
    Token &next = peekToken();

    if (next.type == Token::Operator)
    {
        // Yes it is an operator. Does it have a higher precedence than the
        // left-hand expression?
        const int nextPrecedence = getPrecedence();
        if (nextPrecedence > leftPrecedence)
        {
            skipToken(); // Move along one.

            auto rightExpression = maybeBinary(parseAtomically(), nextPrecedence);

            // Create binary or assign node.
            BaseNode *node{nullptr};

            bool isAssignNode = (next.value == "=");

            if (isAssignNode)
                node = new AssignNode(leftExpression, rightExpression);
            else
                node = new BinaryNode(leftExpression, rightExpression, next.value);

            // Wrap binary node by calling ourselves should the next operator
            // be of a greater precedence.
            return maybeBinary(std::move(node), leftPrecedence);
        }
    }

    // Default.
    return leftExpression;
}


#pragma mark - *** Maybe ***

BaseNode *Parser::maybeFunctionCall(ParseMethod expression)
{
    auto expr = expression(); // Possible function name.

    return isPunctuation("(") ? parseFunctionCall(expr) : expr;
}


BaseNode *Parser::maybeArrayAccess(ParseMethod expression)
{
    auto expr = expression(); // Possible array variable name.

    return isPunctuation("[") ? parseArrayAccessor(expr) : expr;
}


BaseNode *Parser::maybeFunctionCallOrArrayAccess(ParseMethod expression)
{
    auto expr = expression();
    assert(expr != nullptr);

    if (!tokenizer.empty())
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

BaseNode *Parser::parseExpression()
{
    return maybeFunctionCallOrArrayAccess(std::bind(&Parser::parseExpressionHelper, this));
}


BaseNode *Parser::parseExpressionHelper()
{
    return maybeBinary(parseAtomically(), 0);
}


BaseNode *Parser::parseAtomically()
{
    // 1. Call parseAtomicallyExpression() -> This may be the function name.
    // 2. Do we have a next token which is "("? --> YES --> FUNCTION CALL!
    return maybeFunctionCallOrArrayAccess(std::bind(&Parser::parseAtomicallyExpression, this));
}


BaseNode *Parser::parseAtomicallyExpression()
{
    // TODO: - breakup
    if (isPunctuation("("))
        return parseBrackets();
    else if (isPunctuation("["))
        return parseArray();
    else if (isPunctuation("{"))
        return parseProgram();
    else if (isKeyword("true") || isKeyword("false"))
        return parseBool();
    else if (isKeyword("while"))
        return parseWhile();
    else if (isKeyword("do"))
        return parseDoWhile();
    else if (isKeyword("for"))
        return parseFor();
    else if (isKeyword("if"))
        return parseIf();
    else if (isKeyword("import"))
        return parseImport();
    else if (isKeyword("func")) // Functions should be defined as in C --> will need void type
        return parseFunctionDefinition();
    else if (isKeyword("struct"))
        return parseStruct();
    else if (isKeyword("class"))
        return parseClass();
    else if (isDataTypeKeyword())
        return parseVariableDefinition();
    else if (isKeyword("break"))
        return parseBreak();
    else if (isKeyword("return"))
        return parseReturn();

    // TODO: - split-up into separate method for unary operators.
    // Parse unary operators.
    else if (isOperator("!"))
        return parseNot();
    // Parse prefix increment operator i.e.
    else if (isOperator("++"))
        return parsePrefixIncrement();
    else if (isOperator("--"))
        return parsePrefixDecrement();
    else if (isOperator("-"))
        return parseNegation();

    Token &token = peekToken();

    switch (token.type)
    {
        case Token::Variable:
            return parseVariableName();
        case Token::String:
            return parseString();
        case Token::Int:
            return parseInt();
        case Token::Float:
            return parseFloat();
        default:
            unexpectedToken();
            exit(EXIT_FAILURE);
    }
}


NotNode *Parser::parseNot()
{
    skipOperator("!");

    return new NotNode(parseAtomically());
}


PrefixIncrementNode *Parser::parsePrefixIncrement()
{
    skipOperator("++");

    return new PrefixIncrementNode(parseAtomically());
}


PrefixDecrementNode *Parser::parsePrefixDecrement()
{
    skipOperator("--");

    return new PrefixDecrementNode(parseAtomically());
}


NegationNode *Parser::parseNegation()
{
    skipOperator("-");

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
ProgramNode *Parser::parseDelimited(std::string start,
                                    std::string stop,
                                    std::string separator,
                                    ParseMethod parseMethod)
{
    skipPunctuation(start); // Skip the punctuation at the start.

    std::vector<BaseNode *> parsedNodes;

    // Iterate while we still have tokens and haven't reached stop token.
    bool firstCall = true;

    while (!tokenizer.empty() && !isPunctuation(stop))
    {
        // Skip separator on each subsequent call (i.e. a, b)
        if (firstCall)
        {
            firstCall = false;
        }
        else
        {
            skipPunctuation(separator);
        }

        if (isPunctuation(stop))
        {
            break;
        }

        // Parse the token.
        parsedNodes.push_back(parseMethod());
    }

    skipPunctuation(stop); // Skip the punctuation at the end.

    // Resize to fit.
    parsedNodes.shrink_to_fit();

    return new ProgramNode(parsedNodes);
}


void Parser::skipSemicolonLineEndingIfRequired(const BaseNode &node)
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
        skipPunctuation(";");
}


ProgramNode *Parser::parseProgramLines()
{
    skipPunctuation("{");

    std::vector<BaseNode *> parsedNodes;

    while (!tokenizer.empty() && !isPunctuation("}"))
    {
        auto expression = parseExpression();

        parsedNodes.push_back(expression);

        skipSemicolonLineEndingIfRequired(*expression);
    }

    skipPunctuation("}");

    return new ProgramNode(std::move(parsedNodes));
}


int Parser::getPrecedence()
{
    Token &token = peekToken();

    if (token.type != Token::Operator)
    {
        return 0;
    }

    if (token.value == "=")
        return 1;
    else if (token.value == "||")
        return 2;
    else if (token.value == "&&")
        return 3;
    else if ((token.value == "<") ||
             (token.value == ">") ||
             (token.value == "<=") ||
             (token.value == ">=") ||
             (token.value == "==") ||
             (token.value == "!="))
        return 7;
    else if ((token.value == "+") ||
             (token.value == "-"))
        return 10;
    else if ((token.value == "*") ||
             (token.value == "/") ||
             (token.value == "%"))
        return 20;
    else
        return 0;
}


#pragma mark - *** is... ***

bool Parser::isKeyword(const std::string &keyword)
{
    Token &token = peekToken();

    return (token.type == Token::Keyword && token.value == keyword);
}


bool Parser::isDataTypeKeyword()
{
    return (tokenizer.isDataTypeToken());
}


bool Parser::isPunctuation(const std::string &punctuation)
{
    Token &token = peekToken();

    return (token.type == Token::Punctuation && token.value == punctuation);
}


bool Parser::isOperator(const std::string &operatorName)
{
    Token &token = peekToken();

    return (token.type == Token::Operator && token.value == operatorName);
}


#pragma mark - *** skip... ***

void Parser::skipKeyword(const std::string &name)
{
    if (!isKeyword(name))
    {
        EucleiaError("expected keyword '%s' while parsing %s.", name.c_str(), fileInfo.nameWithExt.c_str());
    }

    skipToken();
}


void Parser::skipPunctuation(const std::string &name)
{
    if (!isPunctuation(name))
    {
        EucleiaError("expected punctuation '%s' while parsing %s.", name.c_str(), fileInfo.nameWithExt.c_str());
    }

    skipToken();
}


void Parser::skipOperator(const std::string &name)
{
    if (!isOperator(name))
    {
        EucleiaError("expected operator '%s' while parsing %s.", name.c_str(), fileInfo.nameWithExt.c_str());
    }

    skipToken();
}


#pragma mark -

void Parser::unexpectedToken()
{
    Token &token = peekToken();

    EucleiaError("Unexpected token of type '%s' and value '%s'.", token.description().c_str(), token.value.c_str());
}
