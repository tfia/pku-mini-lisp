#include "eval_env.h"
#include "error.h"
#include "forms.h"

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

void EvalEnv::addVar(std::string & name, ValuePtr val)
{
    SymbolTable.insert_or_assign(name, eval(val));
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
        else throw LispError("Variable `" + *name + "` not defined.");
    }
    else if(expr->isNil()) throw LispError("Evaluating nil is prohibited.");
    else if(expr->isPair())
    {
        std::vector<ValuePtr> v = expr->toVector();
        // if(v[0]->asSymbol() == "define"s)
        // {
        //     if(auto name = v[1]->asSymbol())
        //     {
        //         SymbolTable.insert_or_assign(name.value(), eval(v[2]));
        //         return std::make_shared<NilValue>();
        //     }
        //     else throw LispError("Malformed define.");
        // }
        auto tp = std::dynamic_pointer_cast<PairValue>(expr);
        if(auto name = tp->getL()->asSymbol())
        {
            if(SpecialForms.find(name.value()) != SpecialForms.end())
            {
                return SpecialForms[name.value()](v, *this);
            }
            else
            {
                ValuePtr proc = this->eval(v[0]);
                std::vector<ValuePtr> args = evalList(std::dynamic_pointer_cast<PairValue>(expr)->getR());
                return this->apply(proc, args);
            }
        }
        
    }
    throw LispError("Unimplemented.");
}