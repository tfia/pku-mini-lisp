#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <memory>
#include <vector>
#include <optional>

class Value : public std::enable_shared_from_this<Value>
{
public:
    virtual std::string toString() = 0;
    bool isBoolean();
    bool isNumeric();
    bool isString();
    bool isNil();
    bool isSymbol();
    bool isPair();
    bool isSelfEvaluating();
    std::vector<std::shared_ptr<Value>> toVector();
    std::optional<std::string> asSymbol();
    virtual std::shared_ptr<Value> toValuePtr();
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value
{
    bool data;
public:
    BooleanValue(bool data) : data(data) {}
    virtual std::string toString();
    virtual ValuePtr toValuePtr() override;
};

class NumericValue : public Value
{
    double data;
public:
    NumericValue(double data) : data(data) {}
    virtual std::string toString();
    virtual ValuePtr toValuePtr() override;
};

class StringValue : public Value
{
    std::string data;
public:
    StringValue(std::string data) : data(data) {}
    virtual std::string toString();
    virtual ValuePtr toValuePtr() override;
};

class NilValue : public Value
{
public:
    NilValue() {}
    virtual std::string toString();
    virtual ValuePtr toValuePtr() override;
};

class SymbolValue : public Value
{
    std::string data;
public:
    SymbolValue(std::string data) : data(data) {}
    virtual std::string toString();
    virtual ValuePtr toValuePtr() override;
};

class PairValue : public Value
{
    ValuePtr dataL, dataR;
public:
    PairValue(ValuePtr dataL, ValuePtr dataR) : dataL(dataL), dataR(dataR) {}
    virtual std::string toString();
    virtual ValuePtr toValuePtr() override;
    ValuePtr getL();
    ValuePtr getR();
};

#endif