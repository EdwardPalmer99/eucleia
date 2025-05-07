/**
 * @file NodeFactory.hpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"

namespace NodeFactory
{

AnyNode *createBoolNode(bool state);

AnyNode *createIntNode(long value);


} // namespace NodeFactory