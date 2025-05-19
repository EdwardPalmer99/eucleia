/**
 * @file JumpPoints.cpp
 * @author Edward Palmer
 * @date 2024-11-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "JumpPoints.hpp"
#include "AnyObject.hpp"
#include "Exceptions.hpp"
#include <stack>


std::stack<jmp_buf *> gBreakJumpPointStack;

void pushBreakJumpPoint(jmp_buf *jumpPoint)
{
    gBreakJumpPointStack.push(jumpPoint);
}

void popBreakJumpPoint()
{
    if (gBreakJumpPointStack.empty())
    {
        ThrowException("break jump stack is empty!");
    }

    gBreakJumpPointStack.pop();
}

void jumpToBreakJumpPoint()
{
    if (gBreakJumpPointStack.empty())
    {
        ThrowException("break jump stack is empty!");
    }

    jmp_buf *top = gBreakJumpPointStack.top();
    longjmp(*top, true);
}

GlobalEnvRec gEnvironmentContext;