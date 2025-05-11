/**
 * @file ParserData.hpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SingletonT.hpp"
#include "Token.hpp"
#include <mutex>
#include <string>
#include <unordered_set>

/* Stores info required by all program-parsers such as imported files */
class ParserDataImpl
{
public:
    using StringSet = std::unordered_set<std::string>;

    enum Type
    {
        Module = 0, /* import <math> */
        File = 1    /* import "some-user-file" */
    };

    /* Clear all imports */
    void clearImports();

    /* Records file as being imported (thread-safe) */
    void addImport(std::string importName, Type importType);

    /* Returns true if file is already imported (thread-safe) */
    bool isImported(std::string importName, Type importType) const;

protected:
    friend class SingletonT<ParserDataImpl>;

    /* Prevent direct initialization */
    ParserDataImpl() = default;

    StringSet _importedModuleNames;
    StringSet _importedFileNames;

    using LockGuard = std::lock_guard<std::mutex>;

    mutable std::mutex _mutex;
};


using ParserData = SingletonT<ParserDataImpl>;