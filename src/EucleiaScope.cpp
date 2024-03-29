//
//  EucleiaScope.cpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#include "EucleiaScope.hpp"
#include "EucleiaUtility.hpp"
#include "EucleiaObject.hpp"

Scope::Scope(const Scope & _parent) : Scope(&_parent) {}

Scope::Scope(const Scope * _parent) : parent{_parent}
{
	// If there is a parent, shallow-copy all values. If we modify the value in
	// this scope, it will modify these variables defined in an outer scope.
	if (parent)
	{
		objects = parent->objects;
		objectCreationScope = parent->objectCreationScope;
	}
}


bool Scope::hasObject(const std::string & name) const
{
	return (objects.find(name) != objects.end());
}


bool Scope::objectCreatedInScope(const std::string & name) const
{
	return hasObject(name) && (objectCreationScope.at(name) == this);
}


std::shared_ptr<BaseObject> Scope::getObject(const std::string & name) const
{
	auto it = objects.find(name);
	if (it == objects.end())
	{
		printWarpError("undefined variable '%s'.\n", name.c_str());
	}
	
	return (it->second);
}


void Scope::defineObject(const std::string & name, std::shared_ptr<BaseObject> object)
{
	// 1. Check for name clashes. This is where we have two variables with
	// the same name defined in the SAME scope.
	checkForVariableNameClash(name);
	
	// 2. Remove any existing objects with same name. If there are any these will
	// have been defined in an outer scope. This is variable shadowing.
	removeObject(name);
	
	// 3. Set object creation scope.
	objectCreationScope[name] = this;
	objects[name] = std::move(object);
}


void Scope::updateObject(const std::string & name, std::shared_ptr<BaseObject> object)
{
	assert(hasObject(name));
	
	// Perform basic type-checking.
	auto & existingObject = objects[name];
	
	// NB: None type means it has not yet been initialized.
	if (existingObject->type() != object->type())
	{
		printWarpError("Setting object of type %s with incompatible type %s",
					   existingObject->description().c_str(), object->description().c_str());
	}
	
	// If the creationScope != this then the variable was defined in a parent
	// scope and we are using it. We need to jump up through the scopes and update
	// all scopes using this variable with its new value.
	const Scope * creationScope = objectCreationScope[name];
	
	if (creationScope != this)
	{
		const_cast<Scope *>(parent)->updateObject(name, object);	// Update object in parent scope as well.
	}

	objects[name] = std::move(object);
}


void Scope::checkForVariableNameClash(const std::string & name) const
{
	if (objectCreatedInScope(name))
	{
		printWarpError("Multiple variables defined in scope with the same name '%s'.", name.c_str());
	}
}


void Scope::removeObject(const std::string & name)
{
	objects.erase(name);
	objectCreationScope.erase(name);
}
