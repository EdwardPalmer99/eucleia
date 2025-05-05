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
    /* Constructors */
    FileParser() = delete;
    FileParser(const std::string &fpath);

    static FileNode *parse(std::string filePath_);

    /* Returns reference to subparsers for file */
    [[nodiscard]] SubParsers &subParsers() { return _subParsers; }

    /* Returns tokenized file */
    [[nodiscard]] Tokens &tokens() final { return _tokens; }

    /* Returns parent directory path for file */
    [[nodiscard]] const std::string &parentDirPath() const { return _parentDirPath; }

    /* Construct AST for file */
    FileNode *buildAST();

    /* Parse generic expression */
    BaseNode *parseExpression();

    /* Parse non-binary expression */
    BaseNode *parseAtomically();

    /* Parse an expression in brackets, e.g. (a) */
    BaseNode *parseBrackets();

    BaseNode *maybeFunctionCall(ParseMethod expression);

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

protected:
    BaseNode *parseExpressionHelper();

    BaseNode *parseAtomicallyExpression();

    int getPrecedence(void);

    BaseNode *maybeArrayAccess(ParseMethod expression);

    BaseNode *maybeFunctionCallOrArrayAccess(ParseMethod expression);

    BaseNode *maybeBinary(BaseNode *leftExpression, int leftPrecedence);

    /* Convert a path 'a/b/c/fileName.ek' --> 'a/b/c/' */
    std::string buildParentDirPath(const std::string &filePath_) const;

private:
    const std::string _parentDirPath;
    Tokens _tokens;
    SubParsers _subParsers;
};
