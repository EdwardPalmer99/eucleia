/**
 * @file FileUtils.cpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "FileUtils.hpp"
#include "Exceptions.hpp"
#include "Stringify.hpp"
#include <cstdio>
#include <cstdlib>

namespace eucleia
{

static long numBytesInFile(FILE *fp);


char *loadFileContents(const char *path)
{
    if (!path)
    {
        ThrowException("path cannot be NULL");
    }

    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        ThrowException(eucleia::stringify("failed to open file %s", path));
    }

    const long kBufferSize = numBytesInFile(fp);
    if (kBufferSize <= 0)
    {
        fclose(fp);
        ThrowException("file is empty");
    }

    // NB: +1 for '\0' character.
    char *buffer = (char *)malloc(sizeof(char) * (kBufferSize + 1));
    if (!buffer)
    {
        fclose(fp);
        ThrowException("failed to malloc file buffer");
    }

    // Read from file.
    buffer[kBufferSize] = '\0';
    (void)fread((void *)buffer, sizeof(char), kBufferSize, fp);

    // Close file pointer.
    fclose(fp);

    return buffer;
}


static long numBytesInFile(FILE *fp)
{
    if (!fp)
        return 0;

    fseek(fp, 0, SEEK_END); // Jump to end.

    const long nBytes = ftell(fp);

    rewind(fp); // Jump to start.

    return nBytes;
}

} // namespace eucleia