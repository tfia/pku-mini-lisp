#include "builtins.h"
#include <iostream>
#include <cmath>

// ---------- Core library ----------

ValuePtr print(const std::vector<ValuePtr> & params)
{
    for(const auto & i : params)
        std::cout << i->toString() << std::endl;
    return std::make_shared<NilValue>();
}

// ---------- Arithmetic library ----------

ValuePtr add(const std::vector<ValuePtr> & params)
{
    double result = 0;
    for(const auto & i : params)
    {
        if(!i->isNumeric()) throw LispError("Cannot add a non-numeric value.");
        result += i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr sub(const std::vector<ValuePtr> & params)
{
    if(params.size() == 1) return std::make_shared<NumericValue>(-(params[0]->asNumeric().value()));
    if(params.size() == 2)
    {
        double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
        double result = x - y;
        return std::make_shared<NumericValue>(result);
    }
    throw SyntaxError("Needed 1 or 2 params.");
}

ValuePtr tim(const std::vector<ValuePtr> & params)
{
    double result = 1;
    for(const auto & i : params)
    {
        if(!i->isNumeric()) throw LispError("Cannot add a non-numeric value.");
        result *= i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr dvd(const std::vector<ValuePtr> & params)
{
    if(params.size() == 1)
    {
        double result = 1 / (params[0]->asNumeric().value());
        return std::make_shared<NumericValue>(result);
    }
    if(params.size() == 2)
    {
        double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
        double result = x / y;
        return std::make_shared<NumericValue>(result);
    }
    throw SyntaxError("Needed 1 or 2 params.");
}

ValuePtr abst(const std::vector<ValuePtr> & params)
{
    if(params.size() != 1) throw SyntaxError("Needed only 1 param.");
    return std::make_shared<NumericValue>(fabs(params[0]->asNumeric().value()));
}

ValuePtr expt(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr quotient(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    double result = x / y;
    if(result < 0) result = ceil(result);
    else result = floor(result);
    return std::make_shared<NumericValue>(result);
}

ValuePtr rmd(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    int result = (int)x % (int)y;
    return std::make_shared<NumericValue>(result);
}

ValuePtr modu(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    int c = floor(x / y);
    double result = x - c * y;
    return std::make_shared<NumericValue>(result);
}

// ---------- exported symbol table ----------

std::unordered_map<std::string, ValuePtr> BuiltinSymbols = 
{
    {"print", std::make_shared<BuiltinProcValue>(&print)},
    {"+", std::make_shared<BuiltinProcValue>(&add)},
    {"-", std::make_shared<BuiltinProcValue>(&sub)},
    {"*", std::make_shared<BuiltinProcValue>(&tim)},
    {"/", std::make_shared<BuiltinProcValue>(&dvd)},
    {"abs", std::make_shared<BuiltinProcValue>(&abst)},
    {"expt", std::make_shared<BuiltinProcValue>(&expt)},
    {"quotient", std::make_shared<BuiltinProcValue>(&quotient)},
    {"remainder", std::make_shared<BuiltinProcValue>(&rmd)},
    {"modulo", std::make_shared<BuiltinProcValue>(&modu)},
};