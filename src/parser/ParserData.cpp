/**
 * @file ParserData.cpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ParserData.hpp"

void ParserData::clearImports()
{
    _mutex.lock();

    _importedModuleNames.clear();
    _importedFileNames.clear();

    _mutex.unlock();
}

void ParserData::addImport(std::string importName, Type importType)
{
    _mutex.lock();

    if (importType == Module)
        _importedModuleNames.insert(importName);
    else
        _importedFileNames.insert(importName);

    _mutex.unlock();
}


bool ParserData::isImported(std::string importName, Type importType) const
{
    bool result{false};

    _mutex.lock();

    if (importType == Module)
        result = _importedModuleNames.count(importName);
    else
        result = _importedFileNames.count(importName);

    _mutex.unlock();

    return result;
}