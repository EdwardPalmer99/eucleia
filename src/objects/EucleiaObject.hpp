//
//  EucleiaObject.hpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#ifndef EucleiaObject_hpp
#define EucleiaObject_hpp

#include "ArrayObject.hpp"
#include "BaseObject.hpp"
#include "EucleiaUtility.hpp"
#include "FloatObject.hpp"
#include "FunctionObject.hpp"
#include "IntObject.hpp"
#include "StringObject.hpp"
#include <assert.h>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

class ProgramNode;
class Scope;


/// Library function allows us to define lambdas which wrap around existing stdlib
/// functions. These can then be added to a global scope after seeing "import <...>"
/// with angled-brackets.
struct LibraryFunctionObject : public BaseObject
{
public:
    using LibraryFunction = std::function<BaseObject *(ProgramNode &, Scope &)>;

    LibraryFunctionObject(LibraryFunction function_) : evaluate(std::move(function_)) {}

    std::string typeName() const override { return "LibraryFunctionObject"; }

    LibraryFunctionObject *clone() const override
    {
        return new LibraryFunctionObject(evaluate);
    }

    LibraryFunction evaluate;
};


inline std::ostream &operator<<(std::ostream &out, const BaseObject &object)
{
    if (object.isObjectType<IntObject>())
        return (out << object.castObject<IntObject>().value);
    else if (object.isObjectType<FloatObject>())
        return (out << object.castObject<FloatObject>().value);
    else if (object.isObjectType<StringObject>())
        return (out << object.castObject<StringObject>().value);
    else if (object.isObjectType<ArrayObject>())
    {
        auto &arrayObject = object.castObject<ArrayObject>();

        out << "[";
        for (int i = 0; i < arrayObject.values.size(); i++)
        {
            out << *arrayObject[i] << ", ";
        }
        out << "]";
        return out;
    }
    else
        return out; // Don't print anything.
}

#endif /* EucleiaObject_hpp */
