//
//  EucleiaModuleLoader.hpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#ifndef EucleiaModules_hpp
#define EucleiaModules_hpp

#include "EucleiaNode.hpp"
#include "EucleiaObject.hpp"
#include "EucleiaScope.hpp"
#include <map>
#include <memory>
#include <string>

class ModuleNode : public BaseNode
{
  public:
    using Function = std::function<std::shared_ptr<BaseObject>(ProgramNode &callArgs, Scope &scope)>;

    ModuleNode() = default;
    ~ModuleNode() = default;

    inline NodeType type() const override
    {
        return NodeType::Library;
    }

    /// Add all library functions to the scope this is evaluated in.
    std::shared_ptr<BaseObject> evaluate(Scope &scope) final;

  protected:
    void defineFunction(const std::string &name, Function function);

    std::vector<std::shared_ptr<BaseObject>> evaluateArgs(ProgramNode &args, Scope &scope) const;

    virtual void defineFunctions()
    {
    }

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
class EucleiaModuleLoader
{
  public:
    static EucleiaModuleLoader &getInstance()
    {
        if (!_instance)
        {
            _instance = std::make_unique<EucleiaModuleLoader>();
            _instance->buildDefaultLibraries();
        }

        return *_instance;
    }

    static std::shared_ptr<ModuleNode> getModuleInstance(const std::string &name)
    {
        return getInstance().getModule(name);
    }

    static std::unique_ptr<EucleiaModuleLoader> _instance;

  protected:
    void buildDefaultLibraries();

    std::shared_ptr<ModuleNode> getModule(const std::string &name) const;

  private:
    std::map<std::string, std::shared_ptr<ModuleNode>> _modules;
};


#endif /* EucleiaModules_hpp */
