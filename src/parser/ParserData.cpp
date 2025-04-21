/**
 * @file ParserData.cpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ParserData.hpp"

void ParserDataImpl::clearImports()
{
    LockGuard lock(_mutex);

    _importedModuleNames.clear();
    _importedFileNames.clear();
}

void ParserDataImpl::addImport(std::string importName, Type importType)
{
    LockGuard lock(_mutex);

    if (importType == Module)
        _importedModuleNames.insert(importName);
    else
        _importedFileNames.insert(importName);
}


bool ParserDataImpl::isImported(std::string importName, Type importType) const
{
    LockGuard lock(_mutex);

    bool result{false};

    if (importType == Module)
        result = _importedModuleNames.count(importName);
    else
        result = _importedFileNames.count(importName);

    return result;
}