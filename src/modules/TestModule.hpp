/**
 * @file TestModule.hpp
 * @author Edward Palmer
 * @brief Provides test functions.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TestModule_hpp
#define TestModule_hpp

#include "EucleiaModules.hpp"

class TestModule : public ModuleNode
{
  protected:
    void defineFunctions() override;
};

#endif
