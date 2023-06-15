#include <iostream>

#include "Common.h"
#include "Lexer.h"
#include "Ast.h"
#include <vector>
#include "Parser.h"

void CheckForErrors(const smol::Parser& parser)
{
	const auto& errors = parser.GetErrors();
	if (errors.size() != 0)
	{
		for (auto& error : errors)
		{
			std::cerr << "Parser error: " << error << "\n";
		}
	}
}

int main(int argc, const char* argv[])
{
	const std::string inputTest = R"(
			return 5;
			return 10;
			return 9935542;
		)";

	/**
	 * let ten = 10;

		let add = fn(x, y) {
			x + y;
		};

		let result = add(five, ten);

		!-/*5;
		5 < 10 > 5;

		if(5 < 10)
		{
			return true;
		} else {
			return false;
		}

		10 == 10;
		10 != 9;
	 */

	 // Tokenize the input
	smol::Lexer lexer(inputTest);
	if (true) {
		std::vector<smol::Token> tokens;
		smol::Token currentToken = lexer.NextToken();
		while (currentToken.Type != smol::TokenType::Eof)
		{
			tokens.push_back(currentToken);

			ASSERT(currentToken.Type != smol::TokenType::Illegal, "Illegal type in parser!");

			currentToken = lexer.NextToken();
		}
		tokens.push_back(currentToken);
		ASSERT2(false);
	}

	smol::Parser parser(&lexer);

	// Parsing
	smol::Ast_Program* program = parser.ParseProgram(); // Leak for now
	ASSERT(program != nullptr, "ParseProgram() returned nullptr");
	CheckForErrors(parser);

	ASSERT(program->Statements.size() == 3, "Must have 3 statements!");

	// Pause
	ASSERT2(false);
}