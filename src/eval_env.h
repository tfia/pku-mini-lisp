#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include "value.h"
#include <unordered_map>
#include <algorithm>
#include <iterator>

class EvalEnv : public std::enable_shared_from_this<EvalEnv>
{
    EvalEnv(std::shared_ptr<EvalEnv> p);
    std::unordered_map<std::string, ValuePtr> SymbolTable;
    std::shared_ptr<EvalEnv> parent;
public:
    static std::shared_ptr<EvalEnv> createGlobal();
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr lookupBinding(std::string & name);
    void defineBinding(std::string & name, ValuePtr val);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    ValuePtr eval(ValuePtr expr);
    std::shared_ptr<EvalEnv> createChild(std::vector<std::string> & params, std::vector<ValuePtr> & args);
};

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);

// ---------- moved here from value.h ----------

class BuiltinProcValue : public Value
{
    BuiltinFuncType* func;
public:
    BuiltinProcValue(BuiltinFuncType* func) : func(func) {}
    virtual std::string toString() override;
    BuiltinFuncType* getFunc();
}; // definition in value.cpp

class LambdaValue : public Value
{
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> localEnv;
public:
    LambdaValue
    (std::vector<std::string> params, std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> localEnv) : 
        params(params), body(body), localEnv(localEnv) {}
    std::string toString() override;
    ValuePtr apply(std::vector<ValuePtr> & args);
}; // definition in value.cpp

#endif