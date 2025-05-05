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
#include <string>

namespace NodeFactory
{

namespace Loops
{
AnyNode createWhileNode(AnyNode condition, AnyNode body);

AnyNode createDoWhileNode(AnyNode condition, AnyNode body);

AnyNode createForLoopNode(AnyNode start, AnyNode condition, AnyNode update, AnyNode body);
} // namespace Loops


namespace Instances
{
AnyNode createBoolNode(bool state);

AnyNode createIntNode(long value);

AnyNode createFloatNode(double value);

AnyNode createStringNode(std::string value);
} // namespace Instances


namespace Utilities
{
/* Jump to last set-point and return nullptr */
AnyNode createBreakNode();

} // namespace Utilities


namespace Operators
{

/* Unary not operator (e.g. !true --> false) */
AnyNode createNotNode(AnyNode body);


} // namespace Operators

} // namespace NodeFactory