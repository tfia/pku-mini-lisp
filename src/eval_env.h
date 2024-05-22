#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include "value.h"
#include "builtins.h"
#include <unordered_map>
#include <algorithm>
#include <iterator>

class EvalEnv
{
    std::unordered_map<std::string, ValuePtr> SymbolTable;
public:
    EvalEnv();
    std::vector<ValuePtr> evalList(ValuePtr expr);
    void addVar(std::string & name, ValuePtr val);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    ValuePtr eval(ValuePtr expr);
};

#endif