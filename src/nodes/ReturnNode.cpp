/**
 * @file ReturnNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ReturnNode.hpp"
#include "FileParser.hpp"
#include "JumpPoints.hpp"
#include <cassert>

BaseObject *ReturnNode::evaluate(Scope &scope)
{
    gEnvironmentContext.returnValue = nullptr;

    if (returnNode != nullptr) // i.e. return true;
    {
        // Evaluate in function scope (need all local variables, etc.).
        BaseObject *tmpResult = returnNode->evaluate(scope);
        if (tmpResult != nullptr)
        {
            // TODO: - instead, remove this from the scope and copy into parent scope instead of cloning.

            // Get the scope in which the function was called. We copy the object to this scope.
            // This is because the function scope will destroy the return object as soon as its
            // destructor is called and we need this object to persist until parent scope destructor called.
            assert(scope.parentScope() != nullptr);

            BaseObject *result = scope.parentScope()->cloneObject(tmpResult);
            gEnvironmentContext.returnValue = result;
        }
    }

    longjmp(*gEnvironmentContext.returnJumpPoint, 1);
    return nullptr;
}


ReturnNode *ReturnNode::parse(FileParser &parser)
{
    parser._skipFunctor("return");

    BaseNode *returnedExpression{nullptr};

    if (!parser.isPunctuation(";"))
    {
        returnedExpression = parser.parseExpression();
    }

    return new ReturnNode(returnedExpression);
}
