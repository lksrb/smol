#pragma once

#include "Ast.h"
#include "Lexer.h"

#include <map>

namespace smol 
{
	enum class ParseOrder 
	{
		Lowest = 1,
		Equals,
		LessGreater,
		Sum,
		Product,
		Prefix,
		Call
	};

	class Parser 
	{
	public:
		Parser(Lexer* lexer);
		~Parser();

		Ast_Program* ParseProgram();

		const std::vector<std::string> GetErrors() const { return m_Errors; }
	private:
		Ast_Statement* ParseStatement();
		Ast_LetStatement* ParseLetStatement();
		Ast_ReturnStatement* ParseReturnStatement();
		Ast_ExpressionStatement* ParseExpressionStatement();
		Ast_Expression* ParseIdentifier();
		void NextToken();

		void PeekError(TokenType type);
		bool ExpectPeek(TokenType type);

		Ast_Expression* ParseExpression(ParseOrder order);

		std::map<TokenType, Ast_PrefixParseFn> m_PrefixParseFns;
		std::map<TokenType, Ast_InfixParseFn> m_InfixParseFns;

		std::vector<std::string> m_Errors;
		Lexer* m_Lexer;
		Token m_CurrentToken;
		Token m_PeekToken;
	};

}
