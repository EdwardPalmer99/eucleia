//
//  EucleiaObject.cpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#include "EucleiaObject.hpp"

FloatObject IntObject::castToFloat() const
{
    return FloatObject((double)value);
}
