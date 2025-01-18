/**
 * @file CLIParser.cpp
 * @author Edward Palmer
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "CLIParser.hpp"
#include "Exceptions.hpp"
#include <stdexcept>

CLIParser::CLIParser(std::string programName_)
    : programName(std::move(programName_))
{
}

void CLIParser::addPositionalArg(std::string name)
{
    if (isPositionalArg(name))
        ThrowException(name + " is already defined");

    cliPositionalArgs[name] = std::nullopt;
}

void CLIParser::addFlagArg(std::string name, std::optional<std::string> description)
{
    if (!hasDashPrefix(name))
        ThrowException("incorrect format for flag argument");
    if (isFlag(name))
        ThrowException("argument " + name + " is already defined");

    cliFlags[name] = {.state = false, .description = std::move(description)};
}

const std::string &CLIParser::operator[](const std::string &name) const
{
    if (!isPositionalArg(name))
        ThrowException("unknown argument: " + name);

    auto &optional = cliPositionalArgs.at(name);
    if (!optional)
        ThrowException("positional argument " + name + " is null");

    return *optional;
}

bool CLIParser::isSet(const std::string &name) const
{
    if (isFlag(name))
        return (cliFlags.at(name).state);
    else if (isPositionalArg(name))
        return (cliPositionalArgs.at(name) != std::nullopt);
    else
        ThrowException("unknown argument: " + name);
}

void CLIParser::setFlag(const std::string &name, bool state)
{
    if (!isFlag(name))
        ThrowException("unrecognized flag: " + name);

    cliFlags.at(name).state = state;
}

void CLIParser::setNextPositionalArg(const std::string &value)
{
    for (auto &iter : cliPositionalArgs) // Set in order provided.
    {
        if (iter.second == std::nullopt)
        {
            iter.second = std::make_optional<std::string>(value);
            return;
        }
    }

    // Uh-oh!
    ThrowException("too many positional arguments supplied");
}

void CLIParser::parseArgs(int argc, const char *argv[])
{
    for (int iarg = 1; iarg < argc; ++iarg)
    {
        std::string value{argv[iarg]};

        if (hasDashPrefix(value))
            setFlag(value);
        else
            setNextPositionalArg(value);
    }
}

void CLIParser::printOptions(std::ostream &out) const
{
    out << "USAGE: " << programName;

    for (auto &[name, _] : cliPositionalArgs)
    {
        out << " [" << name << "]";
    }

    out << " [options]" << std::endl;

    const size_t kBufferSize = 250;
    char lineBuffer[kBufferSize];

    out << "OPTIONS:" << std::endl;
    for (auto &[name, flagInfo] : cliFlags)
    {
        if (flagInfo.description)
            snprintf(lineBuffer, kBufferSize, "\t%-20s %s", name.c_str(), (*flagInfo.description).c_str());
        else
            snprintf(lineBuffer, kBufferSize, "\t%-20s", name.c_str());

        out << std::string(lineBuffer) << std::endl;
    }
}

bool CLIParser::hasDashPrefix(const std::string &name) const
{
    return (name.compare(0, 2, "--", 0, 2) == 0);
}

bool CLIParser::isFlag(const std::string &name) const
{
    return cliFlags.count(name);
}

bool CLIParser::isPositionalArg(const std::string &name) const
{
    return cliPositionalArgs.count(name);
}
