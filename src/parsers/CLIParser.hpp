/**
 * @file CLIParser.hpp
 * @author Edward Palmer
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Exceptions.hpp"
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>

class CLIParser
{
public:
    CLIParser() = delete;
    CLIParser(std::string programName);

    // Add a required positional argument.
    void addPositionalArg(std::string argName);

    // Add a new flag (on/off).
    void addFlagArg(std::string flagName, std::optional<std::string> description = std::nullopt);

    // Parse CLI arguments from main().
    void parseArgs(int argc, const char *argv[]);

    // Returns true if flag/positional argument is set.
    bool isSet(const std::string &name) const;

    // Returns reference to positional argument's value.
    const std::string &operator[](const std::string &name) const;

    // Prints available options.
    void printOptions(std::ostream &out = std::cout) const;

protected:
    // Sets a flag.
    void setFlag(const std::string &name, bool state = true);

    // Sets the next positional argument.
    void setNextPositionalArg(const std::string &value);

    // Returns true if this is a CLI flag.
    bool isFlag(const std::string &name) const;

    // Returns true if this is a positional argument.
    bool isPositionalArg(const std::string &name) const;

    // Checks flag name has '--' dashes.
    bool hasDashPrefix(const std::string &name) const;

private:
    const std::string programName;

    // Map of positional arguments.
    std::map<std::string, std::optional<std::string>> cliPositionalArgs;

    struct FlagRec
    {
        bool _state{false};
        std::optional<std::string> _description{std::nullopt};

        FlagRec() = default;

        FlagRec(bool state, std::optional<std::string> description = std::nullopt)
            : _state(state), _description(std::move(description)) {}
    };

    // Map of cli flags. Default to off (false).
    std::unordered_map<std::string, FlagRec> cliFlags;
};
