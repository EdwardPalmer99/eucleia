/**
 * @file NodeFactory.hpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "ExpressionScope.hpp"
#include "JumpPoints.hpp"

namespace NodeFactory
{

namespace Loops
{
AnyNode createWhileNode(AnyNode condition_, AnyNode body_);

AnyNode createDoWhileNode(AnyNode condition, AnyNode body);

AnyNode createForLoopNode(AnyNode start, AnyNode condition, AnyNode update, AnyNode body);
} // namespace Loops


} // namespace NodeFactory