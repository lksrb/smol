#pragma once

#include <map>

#include "Token.h"

namespace smol
{
	class Lexer
	{
	public:
		Lexer(std::string_view input);
		~Lexer();

		Token NextToken();
	private:
		void ReadChar();
		char PeekChar();
		std::string_view ReadIdentifier();
		std::string_view ReadNumber();
		TokenType LookupIdentifier(std::string_view identifier);
		void SkipWhitespace();
	private:
		std::string_view m_Input;
		size_t m_Position;		// current position in input (points to current char)
		size_t m_ReadPosition;	// current reading position in input (after current char)
		char m_CurrentCh;				// current char under examination

		const std::map<std::string_view, TokenType> m_Keywords = {
			{ "fn", TokenType::Function },
			{ "let", TokenType::Let },
			{ "true", TokenType::True },
			{ "false", TokenType::False },
			{ "if", TokenType::If },
			{ "else", TokenType::Else },
			{ "return", TokenType::Return },
		};
	};
}