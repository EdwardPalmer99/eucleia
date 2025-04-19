/**
 * @file BreakNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BreakNode.hpp"
#include "FileParser.hpp"
#include "JumpPoints.hpp"

BaseObject *BreakNode::evaluate(Scope &scope)
{
    jumpToBreakJumpPoint();
    return nullptr;
}


BreakNode *BreakNode::parse(FileParser &parser)
{
    parser._skipFunctor("break");
    return new BreakNode();
}
