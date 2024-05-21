#include <iostream>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <ctype.h>
#include "value.h"
#include "error.h"

// ---------- type identifiers ----------
bool Value::isBoolean() {return typeid(*this) == typeid(BooleanValue);}
bool Value::isNumeric() {return typeid(*this) == typeid(NumericValue);}
bool Value::isString() {return typeid(*this) == typeid(StringValue);}
bool Value::isNil() {return typeid(*this) == typeid(NilValue);}
bool Value::isSymbol() {return typeid(*this) == typeid(SymbolValue);}
bool Value::isPair() {return typeid(*this) == typeid(PairValue);}
bool Value::isBuiltinProc() {return typeid(*this) == typeid(BuiltinProcValue);}
bool Value::isSelfEvaluating()
{
    return (Value::isBoolean() || Value::isNumeric()
        || Value::isString() || Value::isBuiltinProc());
}

// ---------- convert Value to std::vector ----------
std::vector<ValuePtr> Value::toVector()
{
    std::vector<ValuePtr> res;
    if(!this->isPair())
    {
        res.push_back(this->toValuePtr());
        return res;
    }
    auto tp = this->toValuePtr();
    while(1)
    {
        res.push_back(std::dynamic_pointer_cast<PairValue>(tp)->getL());
        if(!(std::dynamic_pointer_cast<PairValue>(tp)->getR())->isPair())
            break;
        tp = std::dynamic_pointer_cast<PairValue>(tp)->getR();
    }
    res.push_back(std::dynamic_pointer_cast<PairValue>(tp)->getR());
    return res;
}

// ---------- evaluate a Value as a symbol ----------
std::optional<std::string> Value::asSymbol()
{
    if(this->isSymbol()) return this->toString();
    return std::nullopt;
}

// ---------- evaluate a Value as a numeric ----------
std::optional<double> Value::asNumeric()
{
    if(this->isNumeric()) return dynamic_cast<NumericValue*>(this)->toDouble();
    return std::nullopt;
}

// ---------- convert Value to std::string ----------
std::string BooleanValue::toString()
{
    if(data == 0) return "#f";
    else if(data == 1) return "#t";
    throw SyntaxError("Expected a boolean value.");
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

std::string BuiltinProcValue::toString()
{
    return "#<procedure>";
}

// ---------- get the data of Value ----------
double NumericValue::toDouble() {return data;}

// ---------- get shared_ptr of dataL & dataR ----------
ValuePtr PairValue::getL() {return dataL;}

ValuePtr PairValue::getR() {return dataR;}

// ---------- get function pointer of BuiltinProcValue ----------
BuiltinFuncType* BuiltinProcValue::getFunc() {return func;}

// ---------- convert this pointer to ValuePtr ----------
ValuePtr Value::toValuePtr() {return shared_from_this();}

ValuePtr BooleanValue::toValuePtr() {return shared_from_this();}

ValuePtr NumericValue::toValuePtr() {return shared_from_this();}

ValuePtr StringValue::toValuePtr() {return shared_from_this();}

ValuePtr NilValue::toValuePtr() {return shared_from_this();}

ValuePtr SymbolValue::toValuePtr() {return shared_from_this();}

ValuePtr PairValue::toValuePtr() {return shared_from_this();}