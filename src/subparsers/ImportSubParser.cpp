/**
 * @file ImportSubParser.cpp
 * @author Edward Palmer
 * @date 2025-05-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ImportSubParser.hpp"
#include "BaseNode.hpp"
#include "EucleiaModules.hpp"
#include "FileNode.hpp"
#include "FileParser.hpp"
#include "Logger.hpp"
#include "ParserData.hpp"
#include "Token.hpp"


BaseNode *ImportSubParser::parseImport()
{
    skip("import");

    auto token = tokens().front();

    if (token.type() == Token::String)
        return parseFileImport();
    else if (equals(Token::Operator, "<"))
        return parseLibraryImport();

    ThrowException("unexpected token: " + token);
}


FileNode *ImportSubParser::parseFileImport()
{
    // File name token:
    auto token = tokens().dequeue();
    assert(token.type() == Token::String);

    // Check: has file already been imported somewhere? If it has then we don't
    // want to import it a second time! (i.e. A imports B, C and B imports C. In
    // this case, PARSE A set[A]--> PARSE B set[A,B]--> PARSE C set[A,B,C].
    if (ParserData::instance().isImported(token, ParserDataImpl::File))
    {
        return new FileNode(); // Return "empty file".
    }

    ParserData::instance().addImport(token, ParserDataImpl::File);

    // Build the file path:
    std::string filePath = parent().parentDirPath() + token;
    log().debug("importing file: " + filePath);

    auto ast = FileParser(filePath).buildAST(); // NB: don't use static method as this will clear loaded modules/files.
    if (!ast)
    {
        ThrowException("failed to import file with path " + filePath);
    }

    return ast;
}


ModuleNode *ImportSubParser::parseLibraryImport()
{
    skip("<");

    auto token = tokens().dequeue();
    assert(token.type() == Token::Variable);

    skip(">");

    if (ParserData::instance().isImported(token, ParserDataImpl::Module))
    {
        return new ModuleNode(); // Return "empty module".
    }

    ParserData::instance().addImport(token, ParserDataImpl::Module);

    log().debug("importing library: " + token);

    return EucleiaModuleLoader::getModuleInstance(token);
}
