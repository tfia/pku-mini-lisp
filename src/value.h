#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <memory>

class Value
{
public:
    virtual std::string toString() = 0;
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value
{
    bool data;
public:
    BooleanValue(bool data) : data(data) {}
    virtual std::string toString();
};

class NumericValue : public Value
{
    double data;
public:
    NumericValue(double data) : data(data) {}
    virtual std::string toString();
};

class StringValue : public Value
{
    std::string data;
public:
    StringValue(std::string data) : data(data) {}
    virtual std::string toString();
};

class NilValue : public Value
{
public:
    NilValue() {}
    virtual std::string toString();
};

class SymbolValue : public Value
{
    std::string data;
public:
    SymbolValue(std::string data) : data(data) {}
    virtual std::string toString();
};

class PairValue : public Value
{
    ValuePtr dataL, dataR;
public:
    PairValue(ValuePtr dataL, ValuePtr dataR) : dataL(dataL), dataR(dataR) {}
    virtual std::string toString();
};

#endif