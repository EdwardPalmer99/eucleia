/**
 * @file FileComponentsRec
 * @author Edward Palmer
 * @date 2024-11-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>

/**
 * Example:
 * path: 'include/examples/PrintFib.ek
 *
 * nameWithExt: 'PrintFib.ek'
 * dirPath: 'include/examples/'
 */
struct FileComponentsRec
{
    explicit FileComponentsRec(const std::string &path);

    std::string nameWithExt;
    std::string dirPath;
};
