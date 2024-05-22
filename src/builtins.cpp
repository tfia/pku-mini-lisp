#include "builtins.h"
#include <iostream>
#include <cmath>

// ---------- Core Library ----------

ValuePtr printProc(const std::vector<ValuePtr> & params)
{
    for(const auto & i : params)
        std::cout << i->toString() << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr displayProc(const std::vector<ValuePtr> & params)
{
    for(const auto & i : params)
    {
        if(i->isString())
            std::cout << std::dynamic_pointer_cast<StringValue>(i)->getString() << std::endl;
        else std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr exitProc(const std::vector<ValuePtr> & params)
{
    if(params.size() == 0) exit(0);
    else if(params.size() == 1 && params[0]->isNumeric()) exit(params[0]->asNumeric().value());
    throw SyntaxError("Needed 0 or 1 params.");
}

ValuePtr newlineProc(const std::vector<ValuePtr> & params)
{
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

// ---------- Arithmetic Library ----------

ValuePtr addProc(const std::vector<ValuePtr> & params)
{
    double result = 0;
    for(const auto & i : params)
    {
        if(!i->isNumeric()) throw LispError("Cannot add a non-numeric value.");
        result += i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr subProc(const std::vector<ValuePtr> & params)
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

ValuePtr timeProc(const std::vector<ValuePtr> & params)
{
    double result = 1;
    for(const auto & i : params)
    {
        if(!i->isNumeric()) throw LispError("Cannot add a non-numeric value.");
        result *= i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divProc(const std::vector<ValuePtr> & params)
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

ValuePtr absProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 1) throw SyntaxError("Needed only 1 param.");
    return std::make_shared<NumericValue>(fabs(params[0]->asNumeric().value()));
}

ValuePtr exptProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr quotientProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    double result = x / y;
    if(result < 0) result = ceil(result);
    else result = floor(result);
    return std::make_shared<NumericValue>(result);
}

ValuePtr remainderProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    int result = (int)x % (int)y;
    return std::make_shared<NumericValue>(result);
}

ValuePtr moduloProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    int c = floor(x / y);
    double result = x - c * y;
    return std::make_shared<NumericValue>(result);
}

// ---------- Comparison Library ----------

ValuePtr eqMarkProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x == y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr lMarkProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x < y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr gMarkProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x > y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr leqMarkProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x <= y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr geqMarkProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 2) throw SyntaxError("Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x >= y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr evenProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 1) throw SyntaxError("Needed only 1 param.");
    double x = params[0]->asNumeric().value();
    if((int)x != x) throw LispError("Param must be int.");
    bool result = ((int)x % 2 == 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr oddProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 1) throw SyntaxError("Needed only 1 param.");
    double x = params[0]->asNumeric().value();
    if((int)x != x) throw LispError("Param must be int.");
    bool result = ((int)x % 2 != 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr zeroProc(const std::vector<ValuePtr> & params)
{
    if(params.size() != 1) throw SyntaxError("Needed only 1 param.");
    double x = params[0]->asNumeric().value();
    bool result = (x == 0);
    return std::make_shared<BooleanValue>(result);
}

// ---------- export builtin-symbol table to global ----------

const std::unordered_map<std::string, ValuePtr> BuiltinSymbols = 
{
    // ---------- Core Library ----------
    {"print", std::make_shared<BuiltinProcValue>(&printProc)},
    {"display", std::make_shared<BuiltinProcValue>(&displayProc)},
    {"exit", std::make_shared<BuiltinProcValue>(&exitProc)},
    {"newline", std::make_shared<BuiltinProcValue>(&newlineProc)},
    // ---------- Arithmetic Library ----------
    {"+", std::make_shared<BuiltinProcValue>(&addProc)},
    {"-", std::make_shared<BuiltinProcValue>(&subProc)},
    {"*", std::make_shared<BuiltinProcValue>(&timeProc)},
    {"/", std::make_shared<BuiltinProcValue>(&divProc)},
    {"abs", std::make_shared<BuiltinProcValue>(&absProc)},
    {"expt", std::make_shared<BuiltinProcValue>(&exptProc)},
    {"quotient", std::make_shared<BuiltinProcValue>(&quotientProc)},
    {"remainder", std::make_shared<BuiltinProcValue>(&remainderProc)},
    {"modulo", std::make_shared<BuiltinProcValue>(&moduloProc)},
    // ---------- Comparison Library ----------
    {"=", std::make_shared<BuiltinProcValue>(&eqMarkProc)},
    {">", std::make_shared<BuiltinProcValue>(&gMarkProc)},
    {"<", std::make_shared<BuiltinProcValue>(&lMarkProc)},
    {">=", std::make_shared<BuiltinProcValue>(&geqMarkProc)},
    {"<=", std::make_shared<BuiltinProcValue>(&leqMarkProc)},
    {"even?", std::make_shared<BuiltinProcValue>(&evenProc)},
    {"odd?", std::make_shared<BuiltinProcValue>(&oddProc)},
    {"zero?", std::make_shared<BuiltinProcValue>(&zeroProc)},
};