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
#include "SubParsers.hpp"
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

    [[nodiscard]] SubParsers &subParsers() { return _subParsers; }

protected:
    /* Default constructor */
    FileParser() = delete;
    FileParser(const std::string &fpath);

    /* TODO: - make these methods public */

    FileNode *buildAST();

    /* Parse an expression in brackets, e.g. (a) */
    BaseNode *parseBrackets();


    bool isDataTypeKeyword();

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

    [[nodiscard]] Tokens &tokens() final { return _tokens; }

private:
    friend class SubParser; /* TODO: - remove once we've finished extracting stuff from class */
    friend class LoopSubParser;
    friend class ControlFlowSubParser;
    friend class BlockSubParser;
    friend class UnaryOperatorSubParser;
    friend class DataTypeSubParser;
    friend class FunctionSubParser;
    friend class ImportSubParser;
    friend class ClassSubParser;

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

    /* TODO: - extract-out */
    FileComponentsRec _fileInfo;

    /* File tokens */
    Tokens _tokens;

    SubParsers _subParsers;
};
