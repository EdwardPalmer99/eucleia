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

// TODO: - need a global parser but something inbetween which holds all parsed Tokens and other state info
// that can then be accessed by other parser static methods

/* Parser for a single file */
class FileParser
{
public:
    // TODO: - should be a FileNode parse method
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
    friend class PrefixDecrementNode;
    friend class NegationNode;
    friend class FunctionNode;
    friend class ArrayAccessNode;
    friend class FunctionCallNode;

    /* Default constructor */
    FileParser() = delete;
    FileParser(const std::string &fpath);

    FileNode *buildAST();

    BaseNode *parseBrackets();

    BaseNode *parseVariableDefinition();
    BaseNode *parseVariableName();

    BaseNode *parseReference(ObjectType boundObjectType);

    BaseNode *parseStruct();
    BaseNode *parseStructAccessor(BaseNode *lastExpression);

    BaseNode *parseClass();

    bool isKeyword(const std::string &keyword);
    bool isPunctuation(const std::string &punctuation);
    bool isOperator(const std::string &operatorName);

    bool isDataTypeKeyword();

    void unexpectedToken();

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
