//
//  EucleiaObject.cpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#include "EucleiaObject.hpp"
#include "EucleiaUtility.hpp"
#include <assert.h>

std::string BaseObject::description() const
{
	switch (type())
	{
		case ObjectType::None:
			return "None";
		case ObjectType::Bool:
			return "Bool";
		case ObjectType::Int:
			return "Int";
		case ObjectType::Float:
			return "Float";
		case ObjectType::String:
			return "String";
		case ObjectType::Array:
			return "Array";
		case ObjectType::Function:
			return "Function";
		default:
			return "Unknown";
	}
}



double BaseObject::floatCast() const
{
	switch (type())
	{
		case ObjectType::Int:
			return (double)castToInt(*this).value;
		case ObjectType::Float:
			return castToFloat(*this).value;
		case ObjectType::Bool:
			return (double)castToBool(*this).value;
		default:
			printWarpError("%s", "invalid object type.\n");
	}
}


long BaseObject::intCast() const
{
	switch (type())
	{
		case ObjectType::Int:
			return castToInt(*this).value;
		case ObjectType::Float:
			return (long)castToFloat(*this).value;
		case ObjectType::Bool:
			return (long)castToBool(*this).value;
		default:
			printWarpError("%s", "invalid object type.\n");
	}
}


bool BaseObject::boolCast() const
{
	return castToBool(*this).value;
}


long IntObject::positiveValue() const
{
	if (value <= 0)
	{
		printWarpError("%s", "int value is zero or negative.\n");
	}
	
	return value;
}


long IntObject::nonzeroValue() const
{
	if (value == 0)
	{
		printWarpError("%s", "int value is zero.\n");
	}
	
	return value;
}


long IntObject::positiveOrZeroValue() const
{
	if (value < 0)
	{
		printWarpError("%s", "int value is negative.\n");
	}
	
	return value;
}


FunctionObject::FunctionObject(std::shared_ptr<FunctionNode> function) :
	functionValue{std::move(function)}
{
	
}

