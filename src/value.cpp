#include <iostream>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <ctype.h>
#include "value.h"

std::string BooleanValue::toString()
{
    if(data == 0) return "#f";
    else if(data == 1) return "#t";
}

std::string NumericValue::toString()
{
    if((int)data == data) return std::to_string((int)data);
    return std::to_string(data);
}

std::string StringValue::toString()
{
    std::stringstream t;
    t << std::quoted(data);
    return t.str();
}

std::string NilValue::toString()
{
    return "()";
}

std::string SymbolValue::toString()
{
    return data;
}

namespace Tmp
{
    bool isBegin = 1;
}

std::string PairValue::toString()
{
    std::string res = "";
    if(Tmp::isBegin == 1)
    {
        res += "(";
        Tmp::isBegin = 0;
    } 
    res += (dataL->toString());
    if(typeid(*dataR) == typeid(NilValue))
    {
        res += ")";
        Tmp::isBegin = 1;
        return res;
    }
    if(typeid(*dataR) == typeid(PairValue))
        res += (" " + dataR->toString());
    else res += (" . " + dataR->toString() + ")");
    Tmp::isBegin = 1;
    return res;
}