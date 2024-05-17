#ifndef PARSER_H
#define PARSER_H
#include <deque>
#include "token.h"
#include "value.h"

class Parser
{
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr> && tokensIn);
    ValuePtr parse();
    ValuePtr parseTails();
};

#endif