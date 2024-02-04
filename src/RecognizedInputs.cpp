//
//  RecognizedInputs.cpp
//  Eucleia
//
//  Created by Edward on 03/02/2024.
//

#include "RecognizedInputs.hpp"

RecognizedInputs::RecognizedInputs()
{
	_punctuation = { ',', ';', '(', ')', '{', '}', '[', ']', '.' };
	
	_operators	= { '+', '-', '*', '/', '%', '&', '|', '<', '>', '=', '!' };
		
	// TODO: - remove --> not required.
	_extraOperators = {
		"-",	//
		"++",	// prefix and postfix!
		"--",
		"!",
		"+=",
		"-="
	};
	
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
		"return"
	};
	
	_dataTypes = {
		"int",
		"float",
		"bool",
		"array",
		"string"
	};
}
