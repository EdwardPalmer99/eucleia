//
//  FileParser.hpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#pragma once
#include "AddVariableNode.hpp"
#include "AnyNode.hpp"
#include "BaseParser.hpp"
#include "BinaryNode.hpp"
#include "BlockSubParser.hpp"
#include "ClassDefinitionObject.hpp"
#include "ClassObject.hpp"
#include "ControlFlowSubParser.hpp"
#include "DataTypeSubParser.hpp"
#include "FunctionCallNode.hpp"
#include "FunctionNode.hpp"
#include "FunctionSubParser.hpp"
#include "ImportSubParser.hpp"
#include "LookupVariableNode.hpp"
#include "LoopSubParser.hpp"
#include "ProgramNode.hpp"
#include "StructDefinitionObject.hpp"
#include "StructObject.hpp"
#include "SubParsers.hpp"
#include "Tokenizer.hpp"
#include "UnaryOperatorSubParser.hpp"
#include <unordered_set>


/* Parser for a single file */
class FileParser : public BaseParser // TODO: - build a thread-pool for FileParsing
{
public:
    /* Constructors */
    FileParser() = delete;
    FileParser(const std::string &fpath);

    /* Top-level parse */
    static AnyNode::Ptr parseMainFile(const std::string entryPointPath_);

    /* Returns reference to subparsers for file */
    [[nodiscard]] SubParsers &subparsers() { return _subParsers; }

    /* Returns tokenized file */
    [[nodiscard]] Tokens &tokens() final { return _tokens; }

    /* Returns parent directory path for file */
    [[nodiscard]] const std::string &parentDirPath() const { return _parentDirPath; }

    /* Construct AST for file */
    AnyNode::Ptr buildAST();

    /* Parse generic expression */
    BaseNode::Ptr parseExpression();

    /* Parse non-binary expression */
    BaseNode::Ptr parseAtomically();

    /* Parse an expression in brackets, e.g. (a) */
    BaseNode::Ptr parseBrackets();

    BaseNode::Ptr maybeFunctionCall(ParseMethod expression);

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

protected:
    BaseNode::Ptr parseExpressionHelper();

    BaseNode::Ptr parseAtomicallyExpression();

    int getPrecedence(void);

    BaseNode::Ptr maybeArrayAccess(ParseMethod expression);

    BaseNode::Ptr maybeFunctionCallOrArrayAccess(ParseMethod expression);

    BaseNode::Ptr maybeBinary(BaseNode::Ptr leftExpression, int leftPrecedence);

    /* Convert a path 'a/b/c/fileName.ek' --> 'a/b/c/' */
    std::string buildParentDirPath(const std::string &filePath_) const;

private:
    const std::string _parentDirPath;
    Tokens _tokens;
    SubParsers _subParsers;
};
