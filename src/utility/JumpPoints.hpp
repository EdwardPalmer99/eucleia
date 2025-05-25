/**
 * @file JumpPoints.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AnyObject.hpp"
#include <csetjmp>


struct GlobalEnvRec
{
    jmp_buf *breakJumpPoint;
    jmp_buf *returnJumpPoint;

    AnyObject returnValue;
};

extern GlobalEnvRec gEnvironmentContext; // TODO: - remove once return done as well.

void pushBreakJumpPoint(jmp_buf *jumpPoint);
void popBreakJumpPoint();
void jumpToBreakJumpPoint();
