#pragma once

#include <string>
#include <vector>
#include <functional>

#include "Common.h"
#include "Token.h"

namespace smol 
{
	struct Ast_Node
	{
		virtual std::string TokenLiteral() = 0;
	};

	struct Ast_Statement : public Ast_Node
	{
		virtual std::string TokenLiteral() = 0;
	};

	struct Ast_Expression : public Ast_Node
	{
		virtual std::string TokenLiteral() = 0;
	};

	struct Ast_Identifier : public Ast_Expression
	{
		smol::Token Token;
		std::string Value;

		std::string TokenLiteral() override { return Token.Literal; };
	};

	// Root note of every AST
	struct Ast_Program : public Ast_Node
	{
		std::vector<Ast_Statement*> Statements;

		std::string TokenLiteral() override;
	};

	struct Ast_LetStatement : public Ast_Statement
	{
		Ast_Expression* Value;
		Ast_Identifier Name;
		smol::Token Token; // Let token

		std::string TokenLiteral() override { return Token.Literal; };
	};

	struct Ast_ReturnStatement : public Ast_Statement
	{
		Ast_Expression* ReturnValue;
		smol::Token Token; // return token

		std::string TokenLiteral() override { return Token.Literal; };
	};

	struct Ast_ExpressionStatement : public Ast_Statement
	{
		smol::Token Token; // First token of the expression
		Ast_Expression* Expression;

		std::string TokenLiteral() override { return Token.Literal; };
	};

	using Ast_PrefixParseFn = std::function<Ast_Expression*()>;
	using Ast_InfixParseFn = std::function<Ast_Expression*(Ast_Expression*)>;

}