/**
 * @file SubParsers.hpp
 * @author Edward Palmer
 * @date 2025-05-04
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BlockSubParser.hpp"
#include "ControlFlowSubParser.hpp"
#include "DataTypeSubParser.hpp"
#include "FunctionSubParser.hpp"
#include "ImportSubParser.hpp"
#include "LoopSubParser.hpp"
#include "SubParser.hpp"
#include "UnaryOperatorSubParser.hpp"


struct SubParsers
{
    SubParsers() = delete;
    SubParsers(FileParser &parser_);

    LoopSubParser loop;
    ControlFlowSubParser controlFlow;
    BlockSubParser block;
    UnaryOperatorSubParser unary;
    DataTypeSubParser dataType;
    FunctionSubParser function;
    ImportSubParser import;
};