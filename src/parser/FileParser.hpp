//
//  FileParser.hpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#pragma once
#include "EucleiaModules.hpp"
#include "FileInfoRec.hpp"
#include "Nodes.hpp" /* TODO: - move into source file if possible */
#include "ParserFunctors.hpp"
#include "Tokenizer.hpp"
#include <unordered_set>

/* Parser for a single file */
class FileParser
{
public:
    [[nodiscard]] static inline FileNode *parse(std::string filePath)
    {
        return FileParser(filePath).buildAST();
    }

protected:
    friend class AddArrayNode;
    friend class AddBoolNode;
    friend class AddFloatNode;
    friend class AddIntNode;
    friend class AddStringNode;
    friend class LookupVariableNode;
    friend class ProgramNode;
    friend class WhileNode;
    friend class DoWhileNode;
    friend class ForLoopNode;
    friend class IfNode;
    friend class BreakNode;
    friend class ReturnNode;
    friend class NotNode;
    friend class FileNode;
    friend class ModuleNode;
    friend class PrefixIncrementNode;


    /* Default constructor */
    FileParser() = delete;
    FileParser(const std::string &fpath);

    FileNode *buildAST();

    BaseNode *parseBrackets();

    BaseNode *parseVariableDefinition();
    BaseNode *parseVariableName();

    BaseNode *parseReference(ObjectType boundObjectType);

    FunctionNode *parseFunctionDefinition();

    BaseNode *parseStruct();
    BaseNode *parseStructAccessor(BaseNode *lastExpression);

    BaseNode *parseClass();

    PrefixDecrementNode *parsePrefixDecrement();
    NegationNode *parseNegation();

    ArrayAccessNode *parseArrayAccessor(BaseNode *lastExpression);

    bool isKeyword(const std::string &keyword);
    bool isPunctuation(const std::string &punctuation);
    bool isOperator(const std::string &operatorName);

    bool isDataTypeKeyword();

    void unexpectedToken();

    FunctionCallNode *parseFunctionCall(BaseNode *lastExpression);

    void skipSemicolonLineEndingIfRequired(const BaseNode &node);

    [[nodiscard]] inline Tokens &tokens()
    {
        return _tokens;
    }

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

    BaseNode *maybeBinary(BaseNode *leftExpression,
                          int leftPrecedence);

    Tokens _tokens;
    FileComponentsRec _fileInfo;

    /* Functor for skipping a token */
    SkipFunctor _skipFunctor;
};
