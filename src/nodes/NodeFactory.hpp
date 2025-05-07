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
#include "BaseNode.hpp"
#include "IntObject.hpp"


namespace NodeFactory
{

AnyNode *createBoolNode(bool state);

} // namespace NodeFactory