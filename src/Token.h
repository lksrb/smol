#pragma once

#include <string>

namespace smol 
{
	enum class TokenType
	{
		Illegal = 0,
		Eof,

		// Identifies + literals
		Identifier,
		Int,

		// Operators
		Assign,
		Plus,
		Minus,
		Asterisk,
		Slash,
		Bang,

		EQ,
		NotEQ,

		Lt, // <
		Gt, // >

		// Delimiter
		Comma,
		Semicolon,

		LParen,
		RParen,
		LBrace,
		RBrace,

		// Keywords
		Function,
		Let,
		True,
		False,
		If,
		Else,
		Return
	};

	struct Token
	{
		TokenType Type;
		std::string Literal;
	};
}