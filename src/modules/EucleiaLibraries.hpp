//
//  EucleiaLibraries.hpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#ifndef EucleiaLibraries_hpp
#define EucleiaLibraries_hpp

#include "EucleiaScope.hpp"
#include "EucleiaObject.hpp"
#include "EucleiaNode.hpp"
#include <map>
#include <string>
#include <memory>

class LibraryNode : public BaseNode
{
public:
	using Function = std::function<std::shared_ptr<BaseObject>(ProgramNode & callArgs, Scope & scope)>;
	
	LibraryNode() = default;
	~LibraryNode() = default;
	
	inline NodeType type() const override { return NodeType::Library; }
	
	/// Add all library functions to the scope this is evaluated in.
	std::shared_ptr<BaseObject> evaluate(Scope & scope) final;

protected:
	void defineFunction(const std::string & name, Function function);
	
	std::vector<std::shared_ptr<BaseObject>> evaluateArgs(ProgramNode & args, Scope & scope) const;
	
	virtual void defineFunctions() { }
	
private:
	std::map<std::string, Function> _functionsMap;
};


/// Library functions for maths.
class MathLibraryNode : public LibraryNode
{
public:
	MathLibraryNode() = default;
	~MathLibraryNode() = default;
		
protected:
	void defineFunctions() override;
};


/// Library functions for input/output.
class IOLibraryNode : public LibraryNode
{
public:
	IOLibraryNode() = default;
	~IOLibraryNode() = default;
	
protected:
	void defineFunctions() override;
};

/// Singleton store of libraries.
class EucleiaLibraries
{
public:
	static EucleiaLibraries & getInstance()
	{
		if (!_instance)
		{
			_instance = std::make_unique<EucleiaLibraries>();
			_instance->buildDefaultLibraries();
		}
		
		return *_instance;
	}
	
	static std::shared_ptr<LibraryNode> getLibraryInstance(const std::string & name)
	{
		return getInstance().getLibrary(name);
	}
		
	static std::unique_ptr<EucleiaLibraries> _instance;

protected:
	void buildDefaultLibraries();
		
	std::shared_ptr<LibraryNode> getLibrary(const std::string & name) const;

private:
	std::map<std::string, std::shared_ptr<LibraryNode>> _libraries;
};


#endif /* EucleiaLibraries_hpp */
