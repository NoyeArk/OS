#ifndef Compiler_H
#define Compiler_H

/*********************************************************************
 * \file   compiler.h
 * \brief  ±‡“Î∆˜ µœ÷
 *
 * \author horiki
 * \date   January 2024
 *********************************************************************/

#include "lex.h"
#include "my_parser.h"


class Compiler
{
private:
	std::vector<int> lineNumberVector;
	std::vector<std::string> tokenVector;

private:
	std::string removeDivXing(const std::string& input);
	int removeComments(std::string& input);
	void lexing(std::vector<std::string>&, std::vector<int>&);

public:
	void GCC(const std::string& code);
};

static Compiler compiler;

#endif
