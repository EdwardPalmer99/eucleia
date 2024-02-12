//
//  EucleiaScope.hpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#ifndef EucleiaScope_hpp
#define EucleiaScope_hpp

#include <string>
#include <memory>
#include <map>

struct BaseObject;


class Scope
{
public:
	Scope(const Scope & _parent);
	Scope(const Scope * _parent = nullptr);
	~Scope() = default;
		
	/// Returns true if object is in current scope.
	bool hasObject(const std::string & name) const;

	/// Get an object from the scope.
	std::shared_ptr<BaseObject> getObject(const std::string & name) const;
	
	/// Get an object from the scope and cast to a subclass.
	template <typename T>
	std::shared_ptr<T> getObject(const std::string & name) const
	{
		auto ownedObject = getObject(name);
		
		return std::static_pointer_cast<T>(ownedObject);
	}
	
	/// Get an object and cast to the desired type. Returns null if not found or
	/// cast fails.
	template <typename T>
	std::shared_ptr<T> getOptionalObject(const std::string & name) const
	{
		if (!hasObject(name)) return nullptr;

		return std::dynamic_pointer_cast<T>(getObject(name));
	}
	
	/// Create a new object in the current scope.
	void defineObject(const std::string & name, std::shared_ptr<BaseObject> object);
	
	/// Update the value of an existing object in the current scope.
	void updateObject(const std::string & name, std::shared_ptr<BaseObject> object);
	
	/// Returns true if object is in this scope AND object was created in this
	/// scope (as opposed to being created in a parent scope). We cannot have
	/// multiple objects with the same name created in the same scope but we
	/// can have variable shadowing (shadowing variable defined in outer scope).
	bool objectCreatedInScope(const std::string & name) const;
		
protected:
	void removeObject(const std::string & name);
	
	void checkForVariableNameClash(const std::string & name) const;
	
private:
	std::map<std::string, std::shared_ptr<BaseObject>> objects;

	/// Stores whether a variable was defined in an outer scope. We allow shadow
	/// variables which is when a variable in the current scope hides the definition
	/// of a variable defined in an outer scope.
	std::map<std::string, Scope *> objectCreationScope;
	
	const Scope * parent{nullptr};
};

#endif /* EucleiaScope_hpp */
