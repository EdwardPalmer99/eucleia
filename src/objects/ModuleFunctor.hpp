/**
 * @file ModuleFunctor.hpp
 * @author Edward Palmer
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */


#pragma once
#include "BaseNode.hpp"
#include "Scope.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

class ModuleFunctor
{
public:
    using Name = std::string;
    using Function = std::function<std::shared_ptr<class AnyObject>(BaseNodePtrVector &, Scope &)>;
    using Definition = std::pair<Name, Function>;
    using Definitions = std::vector<Definition>;

    ModuleFunctor(ModuleFunctor::Function function);

    [[nodiscard]] std::shared_ptr<class AnyObject> operator()(BaseNodePtrVector &args, Scope &scope);

private:
    Function _function{nullptr};
};
