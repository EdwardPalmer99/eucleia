#include "EucleiaFileReader.hpp"

namespace eucleia
{

std::string EucleiaFileReader::readFile(const std::string &fpath)
{
    if (fpath.empty())
        return std::string();

    FILE *fp = fopen(fpath.c_str(), "r");
    if (!fp)
        return std::string(); // TODO: - warning or try / catch. throws.

    const long bufferSize = fileSize(fp);
    if (bufferSize <= 0)
    {
        fclose(fp);
        return std::string();
    }

    // NB: add 1 for '\0' character.
    char *fileBuffer = (char *)malloc(sizeof(char) * (bufferSize + 1));
    if (!fileBuffer)
    {
        fclose(fp);
        return std::string();
    }

    // Read from file.
    fileBuffer[bufferSize] = '\0';
    fread((void *)fileBuffer, sizeof(char), bufferSize, fp);

    // Cleanup.
    fclose(fp);

    // Now put into a string.
    std::string output(fileBuffer);

    // Free memory since string makes a copy.
    free(fileBuffer);

    return output;
}


long EucleiaFileReader::fileSize(FILE *fp) const
{
    if (!fp)
        return 0;

    fseek(fp, 0, SEEK_END); // Jump to end.

    const long nBytes = ftell(fp);

    rewind(fp); // Jump to start.

    return nBytes;
}

} // namespace eucleia