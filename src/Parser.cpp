#include "Parser.h"

#include <format>

#define BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace smol
{

	namespace Utils
	{
		static const char* TokenTypeToString(TokenType type)
		{
			switch (type)
			{
			case TokenType::Illegal: return "Illegal";
			case TokenType::Eof: return "EOF";
			case TokenType::Identifier: return "Identifier";
			case TokenType::Int: return "Int";
			case TokenType::Assign: return "Assign";
			case TokenType::Plus: return "Plus";
			case TokenType::Minus: return "Minus";
			case TokenType::Asterisk: return "Asterisk";
			case TokenType::Slash: return "Slash";
			case TokenType::Bang: return "Bang";
			case TokenType::EQ: return "EQ";
			case TokenType::NotEQ: return "NotEQ";
			case TokenType::Lt: return "Lt";
			case TokenType::Gt: return "Gt";
			case TokenType::Comma: return "Comma";
			case TokenType::Semicolon: return "Semicolon";
			case TokenType::LParen: return "LParen";
			case TokenType::RParen: return "RParen";
			case TokenType::LBrace: return "LBrace";
			case TokenType::RBrace: return "RBrace";
			case TokenType::Function: return "Function";
			case TokenType::Let: return "Let";
			case TokenType::True:return "True";
			case TokenType::False:return "False";
			case TokenType::If: return "If";
			case TokenType::Else:return "Else";
			case TokenType::Return: return "Return";
			}

			ASSERT2(false);
			return "Unknown";
		}
	}

	Parser::Parser(Lexer* lexer)
		: m_Lexer(lexer)
	{
		// Read two tokens for m_CurrentToken and m_PeekToken
		NextToken();
		NextToken();

		m_PrefixParseFns[TokenType::Identifier] = BIND_FN(Parser::ParseIdentifier);
	}

	Parser::~Parser()
	{
	}

	Ast_Program* Parser::ParseProgram()
	{
		Ast_Program* program = new Ast_Program();

		while (m_CurrentToken.Type != TokenType::Eof)
		{
			Ast_Statement* statement = ParseStatement();

			if (statement != nullptr)
			{
				program->Statements.push_back(statement);
			}

			// Skip semicolon
			NextToken();
		}

		return program;
	}

	Ast_Statement* Parser::ParseStatement()
	{
		switch (m_CurrentToken.Type)
		{
		case TokenType::Let: return ParseLetStatement();
		case TokenType::Return: return ParseReturnStatement();
		}

		return ParseExpressionStatement();
	}

	Ast_LetStatement* Parser::ParseLetStatement()
	{
		Ast_LetStatement* letStatement = new Ast_LetStatement;
		letStatement->Token = m_CurrentToken;

		if (!ExpectPeek(TokenType::Identifier))
		{
			delete letStatement;
			return nullptr;
		}

		letStatement->Name.Token = m_CurrentToken;
		letStatement->Name.Value = m_CurrentToken.Literal;

		if (!ExpectPeek(TokenType::Assign))
		{
			return nullptr;
		}

		// TODO: Expression
		while (m_CurrentToken.Type != TokenType::Semicolon)
		{
			NextToken();
		}

		return letStatement;
	}

	Ast_ReturnStatement* Parser::ParseReturnStatement()
	{
		Ast_ReturnStatement* returnStatement = new Ast_ReturnStatement;
		returnStatement->Token = m_CurrentToken;

		// TODO: Expression
		while (m_CurrentToken.Type != TokenType::Semicolon)
		{
			NextToken();
		}

		return returnStatement;
	}

	Ast_ExpressionStatement* Parser::ParseExpressionStatement()
	{
		Ast_ExpressionStatement* expressionStatement = new Ast_ExpressionStatement;
		expressionStatement->Token = m_CurrentToken;

		expressionStatement->Expression = ParseExpression(ParseOrder::Lowest);

		if (m_PeekToken.Type == TokenType::Semicolon)
		{
			NextToken();
		}

		return expressionStatement;
	}

	Ast_Expression* Parser::ParseIdentifier()
	{
		Ast_Identifier* identifier = new Ast_Identifier;
		identifier->Token = m_CurrentToken;
		identifier->Value = m_CurrentToken.Literal;
		return identifier;
	}

	void Parser::NextToken()
	{
		m_CurrentToken = m_PeekToken;
		m_PeekToken = m_Lexer->NextToken();
	}

	bool Parser::ExpectPeek(TokenType type) {
		if (m_PeekToken.Type == type) {
			NextToken();
			return true;
		}
		else {
			PeekError(type);
			return false;
		}
	}

	Ast_Expression* Parser::ParseExpression(ParseOrder order)
	{
		auto prefix = m_PrefixParseFns.find(m_CurrentToken.Type);

		if (prefix == m_PrefixParseFns.end())
			return nullptr;

		return prefix->second();
	}

	void Parser::PeekError(TokenType type)
	{
		const char* expectedType = Utils::TokenTypeToString(type);
		const char* actualType = Utils::TokenTypeToString(m_PeekToken.Type);

		std::string msg = std::format("Expected next token to be {} got {} instead!", expectedType, actualType);

		m_Errors.push_back(msg);
	}

}