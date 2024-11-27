/**
 * @file JumpPoints.cpp
 * @author Edward Palmer
 * @date 2024-11-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "JumpPoints.hpp"
#include "EucleiaUtility.hpp"
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
        EucleiaError("%s", "break jump stack is empty!\n");
    }

    gBreakJumpPointStack.pop();
}

void jumpToBreakJumpPoint()
{
    if (gBreakJumpPointStack.empty())
    {
        EucleiaError("%s", "break jump stack is empty!\n");
    }

    jmp_buf *top = gBreakJumpPointStack.top();
    longjmp(*top, true);
}

GlobalEnvRec gEnvironmentContext;