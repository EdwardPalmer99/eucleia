/**
 * @file Grammar.cpp
 * @author Edward Palmer
 * @date 2025-01-10
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Grammar.hpp"

GrammarImpl::GrammarImpl()
    : punctuation{',', ';', '(', ')', '{', '}', '[', ']', '.'},
      operators{'+', '-', '*', '/', '%', '&', '|', '<', '>', '=', '!'},
      keywords{"if", "else", "true", "false", "func", "while", "do", "for", "int",
               "float", "bool", "array", "string", "break", "return", "import",
               "struct", "class", "extends"},
      dataTypes{"int", "float", "bool", "array", "string"}
{
}
