/**
 * @file FileNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FileNode.hpp"
#include "EucleiaModules.hpp"
#include "FileParser.hpp"
#include "Logger.hpp"
#include "ParserData.hpp"


BaseObject *FileNode::evaluate(Scope &globalScope)
{
    for (const auto &node : programNodes)
    {
        (void)node->evaluate(globalScope);
    }

    return nullptr;
}


FileNode *FileNode::parseUserImport(FileParser &parser)
{
    // File name token:
    auto token = parser.tokens().dequeue();
    assert(token.type() == Token::String);

    // Check: has file already been imported somewhere? If it has then we don't
    // want to import it a second time! (i.e. A imports B, C and B imports C. In
    // this case, PARSE A set[A]--> PARSE B set[A,B]--> PARSE C set[A,B,C].
    if (ParserData::instance().isImported(token, ParserData::File))
    {
        return new FileNode(); // Return "empty file".
    }

    ParserData::instance().addImport(token, ParserData::File);

    // Build the file path:
    std::string filePath = parser._fileInfo.dirPath + token;
    Logger::debug("importing file: " + filePath);

    auto ast = FileParser::parse(filePath);
    if (!ast)
    {
        ThrowException("failed to import file with path " + filePath);
    }

    return ast;
}


BaseNode *FileNode::parse(FileParser &parser)
{
    parser._skipFunctor("import");

    auto token = parser.tokens().front();

    if (token.type() == Token::String)
        return FileNode::parseUserImport(parser);
    else if (parser.isOperator("<"))
        return ModuleNode::parse(parser);

    ThrowException("unexpected token while parsing import: '" + token + "'");
}
