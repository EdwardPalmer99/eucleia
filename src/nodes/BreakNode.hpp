/**
 * @file BreakNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include <csetjmp>

class BreakNode : public BaseNode
{
public:
    // Jump to last set point and return nullptr.
    BaseObject *evaluate(Scope &scope) override;
};