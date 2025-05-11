/**
 * @file NodeType.hpp
 * @author Edward Palmer
 * @date 2025-05-08
 *
 * @copyright Copyright (c) 2025
 *
 */


#pragma once

enum class NodeType
{
    Unknown = (-1),
    Bool,
    Int,
    String,
    Float,
    If,
    ForLoop,
    Break,
    Return,
    Not,
    While,
    DoWhile,
    Block,
    Assign,
    Array,
    File
};
