#include "eval_env.h"
#include "error.h"

ValuePtr EvalEnv::eval(ValuePtr expr)
{
    using namespace std::literals;
    if(expr->isSelfEvaluating()) return expr;
    else if(expr->isNil()) throw LispError("Evaluating nil is prohibited.");
    std::vector<ValuePtr> v = expr->toVector();
    if(v[0]->asSymbol() == "define"s)
    {
        if(auto name = v[1]->asSymbol())
        {
            SymbolTable.insert_or_assign(name.value(), eval(v[2]));
            return std::make_shared<NilValue>();
        }
        else throw LispError("Malformed define.");
    }
    if(auto name = expr->asSymbol())
    {
        if(SymbolTable.find(*name) != SymbolTable.end())
        {
            auto value = SymbolTable.find(*name)->second;
            return value;
        }
        else throw LispError("Variable " + *name + " not defined.");
    }
    throw LispError("Unimplemented.");
}