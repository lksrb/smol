#include "Ast.h"

namespace smol
{
	std::string Ast_Program::TokenLiteral()
	{
		if (Statements.size() > 0)
		{
			// Returns first statement 
			return Statements[0]->TokenLiteral();
		}
		else {
			return "";
		}
	}
}