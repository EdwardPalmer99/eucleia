/**
 * @file ParserData.hpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "FileNode.hpp"
#include "Token.hpp"
#include <mutex>
#include <string>
#include <unordered_set>

/* Stores info required by all program-parsers such as imported files */
class ParserData
{
public:
    using StringSet = std::unordered_set<std::string>;

    /* Singleton (thread-safe) */
    static ParserData &instance()
    {
        static ParserData info;
        return info;
    }

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
    /* Prevent direct initialization */
    ParserData() = default;

    /* Delete implicit copy constructors */
    ParserData &operator=(const ParserData &) = delete;
    ParserData(const ParserData &) = delete;

    StringSet _importedModuleNames;
    StringSet _importedFileNames;

    mutable std::mutex _mutex;
};