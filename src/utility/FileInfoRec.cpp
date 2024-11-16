/**
 * @file FileComponentsRec.cpp
 * @author Edward Palmer
 * @date 2024-11-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FileInfoRec.hpp"
#include <cstring>

FileComponentsRec::FileComponentsRec(const std::string &path)
{
    char dirPathBuffer[path.size() + 1];
    char nameWithExtBuffer[path.size() + 1];

    dirPathBuffer[0] = nameWithExtBuffer[0] = '\0';

    const char *pathDataBase = path.data();

    for (long i = (long)path.size() - 1; i >= 0; --i)
    {
        char c = path[i];

        if (c == '/') // Hit our first '/' (successfully stripped file name and extension).
        {
            strcpy(nameWithExtBuffer, pathDataBase + (i + 1));
            strcpy(dirPathBuffer, pathDataBase);
            dirPathBuffer[i + 1] = '\0';

            break;
        }
    }

    // If this fails then we will just end-up with empty strings.
    dirPath = std::string(dirPathBuffer);
    nameWithExt = std::string(nameWithExtBuffer);
}