/**
 * @file BaseNode.cpp
 * @author Edward Palmer
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BaseNode.hpp"
#include "Logger.hpp"
#include <algorithm>


BaseNodeSharedPtrVector toSharedNodePtrVector(BaseNodePtrVector &nodes)
{
    _trace;

    BaseNodeSharedPtrVector sharedNodes;
    sharedNodes.reserve(nodes.size());

    std::for_each(nodes.begin(), nodes.end(), [&sharedNodes](auto *node)
    {
        sharedNodes.emplace_back(node);
    });

    return sharedNodes;
}
