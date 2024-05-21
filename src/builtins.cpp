#include "builtins.h"
#include <iostream>

ValuePtr add(const std::vector<ValuePtr> & params)
{
    int result = 0;
    for(const auto & i : params)
    {
        if(!i->isNumeric()) throw LispError("Cannot add a non-numeric value.");
        result += i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr> & params)
{
    if(params.size() != 1) throw LispError("Print expect only 1 param.");
    std::cout << params[0]->toString() << std::endl;
    return std::make_shared<NilValue>();
}

std::unordered_map<std::string, ValuePtr> BuiltinSymbols = 
{
    {"+", std::make_shared<BuiltinProcValue>(&add)},
    {"print", std::make_shared<BuiltinProcValue>(&print)}
};