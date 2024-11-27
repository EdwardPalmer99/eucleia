//
//  EucleiaGrammar.cpp
//  Eucleia
//
//  Created by Edward on 03/02/2024.
//

#include "EucleiaGrammar.hpp"

EucleiaGrammar::EucleiaGrammar()
{
    _punctuation = {',', ';', '(', ')', '{', '}', '[', ']', '.'};

    _operators = {'+', '-', '*', '/', '%', '&', '|', '<', '>', '=', '!'};

    _keywords = {
        "if",
        "else",
        "true",
        "false",
        "func",
        "while",
        "do",
        "for",
        "int",
        "float",
        "bool",
        "array",
        "string",
        "break",
        "return",
        "import",
        "struct"};

    _dataTypes = {
        "int",
        "float",
        "bool",
        "array",
        "string"};
}
