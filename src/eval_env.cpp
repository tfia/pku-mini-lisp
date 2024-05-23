#include "eval_env.h"
#include "error.h"
#include "forms.h"
#include "builtins.h"
#include <iostream>

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);

EvalEnv::EvalEnv(std::shared_ptr<EvalEnv> p)
{
    for(auto & it : BuiltinSymbols)
        SymbolTable.insert_or_assign(it.first, it.second);
    parent = p;
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal()
{
    EvalEnv res(nullptr);
    return std::make_shared<EvalEnv>(res);
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr)
{
    std::vector<ValuePtr> result;
    auto v = expr->toVector();
    for(auto & i : v) if(!i->isNil()) result.push_back(this->eval(i));
    return result;
}

ValuePtr EvalEnv::lookupBinding(std::string & name)
{
    if(SymbolTable.find(name) != SymbolTable.end())
        return SymbolTable[name];
    else if(parent != nullptr)
        return parent->lookupBinding(name);
    return nullptr;
}

void EvalEnv::defineBinding(std::string & name, ValuePtr val)
{
    SymbolTable.insert_or_assign(name, eval(val));
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args)
{
    if(proc->isBuiltinProc())
        return std::dynamic_pointer_cast<BuiltinProcValue>(proc)->getFunc()(args, *this);
    if(proc->isLambda())
        return std::dynamic_pointer_cast<LambdaValue>(proc)->apply(args);
    throw LispError("EvalEnv::apply: Unimplemented.");
}

ValuePtr EvalEnv::eval(ValuePtr expr)
{
    using namespace std::literals;
    // std::cout << expr->isPair() << std::endl;
    if(expr->isSelfEvaluating()) return expr;
    else if(expr->isSymbol())
    {
        auto name = expr->asSymbol();
        if(lookupBinding(*name) != nullptr)
        {
            auto value = lookupBinding(*name);
            return value;
        }
        else throw LispError("Variable `" + *name + "` not defined.");
    }
    else if(expr->isNil()) throw LispError("Evaluating nil is prohibited.");
    else if(expr->isPair())
    {
        std::vector<ValuePtr> v = expr->toVector();
        auto tp = std::dynamic_pointer_cast<PairValue>(expr);
        // std::cout << v[0]->toString() << std::endl;
        
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
        if(tp->isPair())
        {
            ValuePtr proc = this->eval(v[0]);
            std::vector<ValuePtr> args = evalList(std::dynamic_pointer_cast<PairValue>(expr)->getR());
            return this->apply(proc, args);
        }
        
    }
    throw LispError("EvalEnv::eval: Unimplemented." + expr->toString());
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(std::vector<std::string> & params, std::vector<ValuePtr> & args)
{
    EvalEnv child(shared_from_this());
    for(int i = 0; i <= (int)params.size() - 2; i++)
    {
        if(!args[i]->isNil()) child.defineBinding(params[i], args[i]);
        else child.SymbolTable.insert_or_assign(params[i], args[i]);
    }
        
    return std::make_shared<EvalEnv>(child);
}