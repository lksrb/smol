#include "Lexer.h"

namespace smol
{
	namespace Utils
	{
		static bool IsLetter(char ch)
		{
			return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
		}

		static bool IsDigit(char ch)
		{
			return '0' <= ch && ch <= '9';
		}
	}

	Lexer::Lexer(std::string_view input)
		: m_Input(input), m_Position(0), m_ReadPosition(0), m_CurrentCh(0)
	{
		ReadChar();
	}

	Lexer::~Lexer()
	{
	}

	Token Lexer::NextToken()
	{
		Token token;

		SkipWhitespace();

		token.Literal = m_CurrentCh;

		switch (m_CurrentCh)
		{
		case '=':
			if (PeekChar() == '=')
			{
				char previous = m_CurrentCh;
				ReadChar();
				token.Literal = { previous, m_CurrentCh };
				token.Type = TokenType::EQ;
			}
			else {
				token.Type = TokenType::Assign;
			}

			break;
		case ';': token.Type = TokenType::Semicolon; break;
		case '(': token.Type = TokenType::LParen;	 break;
		case ')': token.Type = TokenType::RParen;	 break;
		case ',': token.Type = TokenType::Comma;	 break;
		case '+': token.Type = TokenType::Plus;		 break;
		case '-': token.Type = TokenType::Minus;	 break;
		case '*': token.Type = TokenType::Asterisk;	 break;
		case '/': token.Type = TokenType::Slash;	 break;
		case '!':
			if (PeekChar() == '=')
			{
				char ch = m_CurrentCh;
				ReadChar();
				token.Literal = ch + m_CurrentCh;
				token.Type = TokenType::NotEQ;
			}
			else {
				token.Type = TokenType::Bang;
			}
			break;
		case '<': token.Type = TokenType::Lt;		break;
		case '>': token.Type = TokenType::Gt;		break;
		case '{': token.Type = TokenType::LBrace;	break;
		case '}': token.Type = TokenType::RBrace;	break;
		case 0:	  token.Type = TokenType::Eof;		break;
		default:
			if (Utils::IsLetter(m_CurrentCh))
			{
				token.Literal = ReadIdentifier();
				token.Type = LookupIdentifier(token.Literal);
				return token;
			}
			else if (Utils::IsDigit(m_CurrentCh))
			{
				token.Literal = ReadNumber();
				token.Type = TokenType::Int;
				return token;
			}
			else {
				token.Type = TokenType::Illegal;
			}
			break;
		}

		ReadChar();
		return token;
	}

	void Lexer::ReadChar()
	{
		m_CurrentCh = m_ReadPosition < m_Input.size() ? m_Input[m_ReadPosition] : 0;
		m_Position = m_ReadPosition;
		m_ReadPosition++;
	}

	char Lexer::PeekChar()
	{
		return m_ReadPosition != m_Input.size() ? m_Input[m_ReadPosition] : 0;
	}

	std::string_view Lexer::ReadIdentifier()
	{
		size_t position = m_Position;

		while (Utils::IsLetter(m_CurrentCh))
		{
			ReadChar();
		}

		return m_Input.substr(position, m_Position - position);
	}

	std::string_view Lexer::ReadNumber()
	{
		size_t position = m_Position;

		while (Utils::IsDigit(m_CurrentCh))
		{
			ReadChar();
		}

		return m_Input.substr(position, m_Position - position);
	}

	TokenType Lexer::LookupIdentifier(std::string_view identifier)
	{
		auto it = m_Keywords.find(identifier);

		if (it != m_Keywords.end())
		{
			return it->second;
		}

		return TokenType::Identifier;
	}

	void Lexer::SkipWhitespace()
	{
		while (m_CurrentCh == ' ' || m_CurrentCh == '\t' || m_CurrentCh == '\n' || m_CurrentCh == '\r')
		{
			ReadChar();
		}
	}
}