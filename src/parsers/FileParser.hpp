//
//  FileParser.hpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#pragma once
#include "BaseParser.hpp"
#include "BlockSubParser.hpp"
#include "ControlFlowSubParser.hpp"
#include "DataTypeSubParser.hpp"
#include "EucleiaModules.hpp"
#include "FileInfoRec.hpp"
#include "FunctionSubParser.hpp"
#include "ImportSubParser.hpp"
#include "LoopSubParser.hpp"
#include "Nodes.hpp"
#include "Tokenizer.hpp"
#include "UnaryOperatorSubParser.hpp"
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

    /* TODO: - make these methods public */

    FileNode *buildAST();

    BaseNode *parseVariableDefinition();
    BaseNode *parseVariableName();

    BaseNode *parseReference(ObjectType boundObjectType);

    /* Parse an expression in brackets, e.g. (a) */
    BaseNode *parseBrackets();

    BaseNode *parseStruct();
    BaseNode *parseStructAccessor(BaseNode *lastExpression);

    BaseNode *parseClass();

    ArrayAccessNode *parseArrayAccessor(BaseNode *lastExpression);

    bool isDataTypeKeyword();

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

private:
    friend class SubParser;
    friend class LoopSubParser;
    friend class ControlFlowSubParser;
    friend class BlockSubParser;
    friend class UnaryOperatorSubParser;
    friend class DataTypeSubParser;
    friend class FunctionSubParser;
    friend class ImportSubParser;

    /* Subparsers */
    LoopSubParser _loopParser;
    ControlFlowSubParser _controlFlowParser;
    BlockSubParser _blockParser;
    UnaryOperatorSubParser _unaryParser;
    DataTypeSubParser _dataTypeParser;
    FunctionSubParser _functionParser;
    ImportSubParser _importParser;

    BaseNode *parseExpression();
    BaseNode *parseExpressionHelper();
    BaseNode *parseAtomically();
    BaseNode *parseAtomicallyExpression();

    int getPrecedence(void);

    BaseNode *maybeArrayAccess(ParseMethod expression);

    BaseNode *maybeFunctionCall(ParseMethod expression);

    BaseNode *maybeFunctionCallOrArrayAccess(ParseMethod expression);

    BaseNode *maybeBinary(BaseNode *leftExpression,
                          int leftPrecedence);

    FileComponentsRec _fileInfo;

    /* Stores tokens for file */
    Tokens _tokens;
};
