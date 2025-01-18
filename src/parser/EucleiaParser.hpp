//
//  EucleiaParser.hpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#pragma once
#include "EucleiaModules.hpp"
#include "Tokenizer.hpp"
#include "FileInfoRec.hpp"
#include "Nodes.hpp"
#include <unordered_set>


class Parser
{
public:
    static FileNode *buildAST(const std::string &fpath);

protected:
    Parser(const std::string &fpath);

    FileNode *buildAST();

    AddBoolNode *parseBool();
    AddStringNode *parseString();
    AddIntNode *parseInt();
    AddFloatNode *parseFloat();

    BaseNode *parseBrackets();

    BaseNode *parseProgram();
    AddArrayNode *parseArray();

    BaseNode *parseVariableDefinition();
    BaseNode *parseVariableName();

    BaseNode *parseReference(ObjectType boundObjectType);

    DoWhileNode *parseDoWhile();
    WhileNode *parseWhile();
    ForLoopNode *parseFor();

    IfNode *parseIf();
    FunctionNode *parseFunctionDefinition();

    BaseNode *parseStruct();
    BaseNode *parseStructAccessor(BaseNode *lastExpression);

    BaseNode *parseClass();

    BreakNode *parseBreak();
    ReturnNode *parseReturn();

    NotNode *parseNot();
    PrefixIncrementNode *parsePrefixIncrement();
    PrefixDecrementNode *parsePrefixDecrement();
    NegationNode *parseNegation();

    ModuleNode *parseLibraryImport();
    FileNode *parseFileImport();
    BaseNode *parseImport();

    ArrayAccessNode *parseArrayAccessor(BaseNode *lastExpression);

    bool isKeyword(const std::string &keyword);
    bool isPunctuation(const std::string &punctuation);
    bool isOperator(const std::string &operatorName);

    bool isDataTypeKeyword();

    void skipKeyword(const std::string &keyword);
    void skipPunctuation(const std::string &punctuation);
    void skipOperator(const std::string &operatorName);

    const Token &peekToken() const { return tokenizer.peek(); }

    Token nextToken() { return tokenizer.next(); }
    void skipToken() { (void)tokenizer.next(); }

    void unexpectedToken();

    FunctionCallNode *parseFunctionCall(BaseNode *lastExpression);

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

private:
    using ParseMethod = std::function<BaseNode *(void)>;

    BaseNode *parseExpression();
    BaseNode *parseExpressionHelper();
    BaseNode *parseAtomically();
    BaseNode *parseAtomicallyExpression();

    int getPrecedence(void);

    BaseNode *maybeArrayAccess(ParseMethod expression);

    BaseNode *maybeFunctionCall(ParseMethod expression);

    BaseNode *maybeFunctionCallOrArrayAccess(ParseMethod expression);

    ProgramNode *parseDelimited(std::string start,
                                std::string stop,
                                std::string separator,
                                ParseMethod expression);

    ProgramNode *parseProgramLines();

    BaseNode *maybeBinary(BaseNode *leftExpression,
                          int leftPrecedence);

    Tokenizer tokenizer;

    FileComponentsRec fileInfo;
};
