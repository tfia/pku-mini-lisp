#include "eval_env.h"
#include "error.h"

extern std::unordered_map<std::string, ValuePtr> BuiltinSymbols;

EvalEnv::EvalEnv()
{
    for(auto & it : BuiltinSymbols)
        SymbolTable.insert_or_assign(it.first, it.second);
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr)
{
    std::vector<ValuePtr> result;
    auto v = expr->toVector();
    for(auto & i : v) if(!i->isNil()) result.push_back(this->eval(i));
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args)
{
    if(proc->isBuiltinProc())
        return std::dynamic_pointer_cast<BuiltinProcValue>(proc)->getFunc()(args);
    else throw LispError("Unimplemented");
}

ValuePtr EvalEnv::eval(ValuePtr expr)
{
    using namespace std::literals;
    if(expr->isSelfEvaluating()) return expr;
    else if(expr->isSymbol())
    {
        auto name = expr->asSymbol();
        if(SymbolTable.find(*name) != SymbolTable.end())
        {
            auto value = SymbolTable.find(*name)->second;
            return value;
        }
        else throw LispError("Variable " + *name + " not defined.");
    }
    else if(expr->isNil()) throw LispError("Evaluating nil is prohibited.");
    else if(expr->isPair())
    {
        std::vector<ValuePtr> v = expr->toVector();
        if(v[0]->asSymbol() == "define"s)
        {
            if(auto name = v[1]->asSymbol())
            {
                if(BuiltinSymbols.find(*name) != BuiltinSymbols.end())
                    throw LispError("Cannot override builtin symbols.");
                SymbolTable.insert_or_assign(name.value(), eval(v[2]));
                return std::make_shared<NilValue>();
            }
            else throw LispError("Malformed define.");
        }
        else
        {
            ValuePtr proc = this->eval(v[0]);
            std::vector<ValuePtr> args = evalList(std::dynamic_pointer_cast<PairValue>(expr)->getR());
            return this->apply(proc, args);
        }
    }
    throw LispError("Unimplemented.");
}