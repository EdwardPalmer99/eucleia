//
//  FileParser.hpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#pragma once
#include "BaseParser.hpp"
#include "EucleiaModules.hpp"
#include "FileInfoRec.hpp"
#include "Nodes.hpp"
#include "Tokenizer.hpp"
#include <unordered_set>

/* Parser for a single file */
class FileParser : public BaseParser
{
public:
    [[nodiscard]] static inline FileNode *parse(std::string filePath)
    {
        return FileParser(filePath).buildAST();
    }

protected:
    /* Default constructor */
    FileParser() = delete;
    FileParser(const std::string &fpath);

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

    bool isDataTypeKeyword();

    void unexpectedToken();

    FunctionCallNode *parseFunctionCall(BaseNode *lastExpression);

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

private:
    BaseNode *parseExpression();
    BaseNode *parseExpressionHelper();
    BaseNode *parseAtomically();
    BaseNode *parseAtomicallyExpression();

    int getPrecedence(void);

    BaseNode *maybeArrayAccess(ParseMethod expression);

    BaseNode *maybeFunctionCall(ParseMethod expression);

    BaseNode *maybeFunctionCallOrArrayAccess(ParseMethod expression);

    ProgramNode *parseProgramLines();

    BaseNode *maybeBinary(BaseNode *leftExpression,
                          int leftPrecedence);

    FileComponentsRec _fileInfo;
};
