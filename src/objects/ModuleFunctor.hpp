/**
 * @file ModuleFunctor.hpp
 * @author Edward Palmer
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */


#pragma once
#include "Scope.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>


class ModuleFunctor
{
public:
    using Name = std::string;
    using Function = std::function<class AnyObject(std::vector<std::shared_ptr<class BaseNode>> &, Scope &)>;
    using Definition = std::pair<Name, Function>;
    using Definitions = std::vector<Definition>;

    ModuleFunctor(ModuleFunctor::Function function);

    [[nodiscard]] class AnyObject operator()(std::vector<std::shared_ptr<class BaseNode>> &args, Scope &scope);

private:
    Function _function{nullptr};
};
