#include "parser.h"
#include "error.h"

// TokenPtr doesn't support copy construct
Parser::Parser(std::deque<TokenPtr> && tokensIn)
{
    for(auto & it : tokensIn) tokens.push_back(std::move(it));
}

ValuePtr Parser::parse()
{
    auto token = std::move(tokens.front());
    tokens.pop_front();
    // ---------- literals ----------
    if(token->getType() == TokenType::NUMERIC_LITERAL)
    {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    if(token->getType() == TokenType::BOOLEAN_LITERAL)
    {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    if(token->getType() == TokenType::STRING_LITERAL)
    {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    if(token->getType() == TokenType::IDENTIFIER)
    {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    // ---------- lists ----------
    if(token->getType() == TokenType::LEFT_PAREN)
    {
        return this->parseTails();
    }
    // ---------- quotes ----------
    if(token->getType() == TokenType::QUOTE)
    {
        return std::make_shared<PairValue>
        (
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>
            (
                this->parse(),
                std::make_shared<NilValue>()
            )
        );
    }
    if(token->getType() == TokenType::QUASIQUOTE)
    {
        return std::make_shared<PairValue>
        (
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>
            (
                this->parse(),
                std::make_shared<NilValue>()
            )
        );
    }
    if(token->getType() == TokenType::UNQUOTE)
    {
        return std::make_shared<PairValue>
        (
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>
            (
                this->parse(),
                std::make_shared<NilValue>()
            )
        );
    }
    throw SyntaxError("Unimplemented");
    return nullptr;
}

ValuePtr Parser::parseTails()
{
    if(tokens.empty()) throw SyntaxError("Expected more tokens.");
    auto next = std::move(tokens.front());
    tokens.pop_front();
    if(next->getType() == TokenType::RIGHT_PAREN) return std::make_shared<NilValue>();
    else tokens.push_front(std::move(next));
    auto car = this->parse();
    next = std::move(tokens.front());
    if(next->getType() == TokenType::DOT)
    {
        tokens.pop_front();
        if(tokens.empty()) throw SyntaxError("Expected one more token after DOT.");
        auto cdr = this->parse();
        if(tokens.empty()) throw SyntaxError("Expected RIGHT_PAREN.");
        next = std::move(tokens.front());
        if(next->getType() != TokenType::RIGHT_PAREN)
            throw SyntaxError("Expected only one token after DOT");
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    }
    else
    {
        tokens.pop_front(); // tokens.front() was moved. must move it back
        tokens.push_front(std::move(next));
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
    
}