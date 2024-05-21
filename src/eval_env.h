#include "value.h"
#include <unordered_map>

class EvalEnv
{
    std::unordered_map<std::string, ValuePtr> SymbolTable;
public:
    ValuePtr eval(ValuePtr expr);
};