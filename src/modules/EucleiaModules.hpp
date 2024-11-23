//
//  EucleiaModuleLoader.hpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#ifndef EucleiaModules_hpp
#define EucleiaModules_hpp

#include "BaseNode.hpp"
#include "Objects.hpp"
#include "Scope.hpp"
#include <map>
#include <memory>
#include <string>

class ModuleNode : public BaseNode
{
public:
    using Function = std::function<BaseObject *(ProgramNode &callArgs, Scope &scope)>;

    ModuleNode() = default;
    ~ModuleNode() = default;

    /// Add all library functions to the scope this is evaluated in.
    BaseObject *evaluate(Scope &scope) final;

protected:
    void defineFunction(const std::string &name, Function function);

    std::vector<BaseObject *> evaluateArgs(ProgramNode &args, Scope &scope) const;

    virtual void defineFunctions() {}

private:
    std::map<std::string, Function> _functionsMap;
};


/// Library functions for maths.
class MathModuleNode : public ModuleNode
{
public:
    MathModuleNode() = default;
    ~MathModuleNode() = default;

protected:
    void defineFunctions() override;
};


/// Library functions for input/output.
class IOModuleNode : public ModuleNode
{
public:
    IOModuleNode() = default;
    ~IOModuleNode() = default;

protected:
    void defineFunctions() override;
};

/// Singleton store of libraries.
class EucleiaModuleLoader // TODO: - massive overkill. Can just add to the parser.
{
public:
    static EucleiaModuleLoader &getInstance()
    {
        if (!_instance)
        {
            _instance = std::make_unique<EucleiaModuleLoader>();
        }

        return *_instance;
    }

    static ModuleNode *getModuleInstance(const std::string &name)
    {
        return getInstance().getModule(name);
    }

    static std::unique_ptr<EucleiaModuleLoader> _instance;

protected:
    ModuleNode *getModule(const std::string &name) const;
};


#endif /* EucleiaModules_hpp */
