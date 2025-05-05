/**
 * @file LoopSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseParser.hpp"
#include "DoWhileNode.hpp"
#include "ForLoopNode.hpp"
#include "SubParser.hpp"
#include "Tokenizer.hpp"
#include "WhileNode.hpp"


class LoopSubParser : public SubParser
{
public:
    explicit LoopSubParser(FileParser &parser) : SubParser(parser) {}

    enum Type
    {
        DoWhile,
        While,
        For
    };

    /**
     * @brief Parses a loop construct based on the specified type.
     *
     * @param type The type of loop to parse (DoWhile, While, or For).
     * @return AnyNode The parsed representation of the loop.
     */
    AnyNode parse(int type, AnyNodeOptional lastExpr) override;

protected:
    /**
     * @brief Parses a do-while loop.
     *
     * Example:
     * do {
     *     [code]
     * } while ([condition]);
     */
    AnyNode parseDoWhile();

    /**
     * @brief Parses a while loop.
     *
     * Example:
     * while ([condition]) {
     *     [code]
     * }
     */
    AnyNode parseWhile();

    /**
     * @brief Parses a for loop.
     *
     * Example:
     * for ([initialization]; [condition]; [increment]) {
     *     [code]
     * }
     */
    AnyNode parseFor();
};
