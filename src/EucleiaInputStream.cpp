//
//  EucleiaInputStream.cpp
//  Eucleia
//
//  Created by Edward on 18/01/2024.
//

#include "EucleiaInputStream.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "EucleiaUtility.hpp"
#include "RecognizedInputs.hpp"

static long getFileSize(FILE *fp);
static const char *getFileContents(const char *fpath);


InputStream::InputStream(const std::string & fpath) : _recognizedInputs()
{
	const char *fileBuffer = getFileContents(fpath.c_str());
	if (!fileBuffer)
	{
		printWarpError("Failed to extract an input stream from %s.", fpath.c_str());
	}
	
	_current.ptr = (char *)fileBuffer;
}


void InputStream::destroy()
{
	if (_basePtr)
	{
		free(_basePtr);
		
		_basePtr = _current.ptr = nullptr;
	}
}


InputStream::~InputStream()
{
	destroy();
}


char InputStream::peek() const
{
	return (_current.ptr ? *_current.ptr : '\0');
}


char InputStream::next()
{
	char c = peek();
	
	consume();
	
	return c;
}


#pragma mark -

bool InputStream::isComment() const
{
	return (peek() == '/');
}
			
bool InputStream::isOperator() const
{
	return _recognizedInputs.isOperator(peek());
}


bool InputStream::isPunctuation() const
{
	return _recognizedInputs.isPunctuation(peek());
}


bool InputStream::isDigit() const
{
	return isdigit(peek());
}


bool InputStream::isWhiteSpace() const
{
	return (isspace(peek()));
}


bool InputStream::isID() const
{
	char c = peek();
	
	return (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}


#pragma mark -

void InputStream::consume()
{
	if (isEof()) 
	{
		return;
	}
	else if (isNewLine())
	{
		_current.line++;
		_current.col = 1;
	}
	else
	{
		_current.col++;
	}
	
	_current.ptr++;
}


void InputStream::reportError(const char *format, ...)
{
	const size_t kBufferSize = 500;
	char errorBuffer[kBufferSize];
	
	va_list args;
	va_start(args, format);
	
	long nchars = vsnprintf(errorBuffer, kBufferSize, format, args);
	
	// Add NULL terminator:
	auto endOffset = (nchars < (kBufferSize - 1)) ? nchars : (kBufferSize - 1);
	
	errorBuffer[endOffset] = '\0';
	
	va_end(args);
	
	fprintf(stderr, "Error on line %d, col %d: '%s'\n", _current.line, _current.col, errorBuffer);
}



#pragma mark -

static long getFileSize(FILE *fp)
{
	if (!fp) return 0;
	
	fseek(fp, 0, SEEK_END);	// Jump to end.

	const long nBytes = ftell(fp);

	rewind(fp);	// Jump to start.
	
	return nBytes;
}


static const char *getFileContents(const char *fpath)
{
	if (!fpath) return NULL;
	
	FILE *fp = fopen(fpath, "r");
	if (!fp) return NULL;
	
	const long bufferSize = getFileSize(fp);
	if (bufferSize <= 0)
	{
		fclose(fp);
		return NULL;
	}
	
	// NB: add 1 for '\0' character.
	char *fileBuffer = (char *)malloc(sizeof(char) * (bufferSize + 1));
	if (!fileBuffer)
	{
		fclose(fp);
		return NULL;
	}
	
	// Read from file.
	fileBuffer[bufferSize] = '\0';
	fread((void *)fileBuffer, sizeof(char), bufferSize, fp);
	
	// Cleanup.
	fclose(fp);

	return (const char *)fileBuffer;
}
