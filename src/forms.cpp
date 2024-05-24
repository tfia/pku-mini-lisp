#include "forms.h"
#include <iostream>
#include <string>

using namespace std::literals;

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);

ValuePtr defineForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    if(auto name = args[1]->asSymbol())
    {
        // std::cout << args[2]->toString() << std::endl;
        env.defineBinding(name.value(), args[2]);
        return std::make_shared<NilValue>();
    }
    else if(args[1]->isPair())
    {
        auto name = std::dynamic_pointer_cast<PairValue>(args[1])->getL()->asSymbol();
        auto params = std::dynamic_pointer_cast<PairValue>(args[1])->getR();
        std::vector<ValuePtr> vin = 
        {
            std::make_shared<SymbolValue>("lambda"),
            params,
        };
        for(int i = 2; i <= (int)args.size() - 1; i++) vin.push_back(args[i]);
        auto val = lambdaForm(vin, env); // Construct a LambdaValue
        env.defineBinding(name.value(), val);
        return std::make_shared<NilValue>();
    }
    throw LispError("defineForm: Unimplemented.");
}

ValuePtr quoteForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    return args[1];
    throw LispError("quoteForm: Unimplemented.");
}

ValuePtr ifForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    auto tp = env.eval(args[1]);
    if(tp->isNil()) return env.eval(args[2]); // special case of '()
    if(tp->isBoolean() && std::dynamic_pointer_cast<BooleanValue>(tp)->toBool() == false)
    {
        return env.eval(args[3]);
    }
    else return env.eval(args[2]);
}

ValuePtr andForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    if(args.size() == 2 && args[1]->isNil()) return std::make_shared<BooleanValue>(true);
    for(int i = 1; i <= args.size() - 2; i++)
    {
        auto it = args[i];
        auto tp = env.eval(it);
        if(tp->isBoolean()
            && std::dynamic_pointer_cast<BooleanValue>(tp)->toBool() == false)
            return std::make_shared<BooleanValue>(false);
    }
    return env.eval(args[args.size() - 2]);
}

ValuePtr orForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    if(args.size() == 2 && args[1]->isNil()) return std::make_shared<BooleanValue>(false);
    for(int i = 1; i <= args.size() - 2; i++)
    {
        auto it = args[i];
        auto tp = env.eval(it);
        if(!tp->isBoolean()) return tp;
        else if(std::dynamic_pointer_cast<BooleanValue>(tp)->toBool() == true) return tp;
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    auto tp = args[1];
    std::vector<ValuePtr> body;
    std::vector<ValuePtr> tv = tp->toVector();
    std::vector<std::string> params;
    for(auto & it : tv) params.push_back(it->toString());
    for(int i = 2; i <= (int)args.size() - 1; i++) body.push_back(args[i]);
    return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}

ValuePtr condForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    ValuePtr result;
    for(int i = 1; i <= args.size() - 2; i++)
    {
        auto it = args[i]->toVector();
        if(it[0]->isSymbol() && it[0]->toString() == "else")
        {
            if(i != args.size() - 2)
                throw LispError("condForm: `else` is not the last condition.");
            for(int j = 1; j <= it.size() - 2; j++)
            {
                result = env.eval(it[j]);
            }
            return result;
        }
        auto tp = env.eval(it[0]);
        if(tp->isBoolean() && std::dynamic_pointer_cast<BooleanValue>(tp)->toBool() == false)
        {
            continue;
        }
        else
        {  
            if(it.size() == 2) return tp;
            for(int j = 1; j <= it.size() - 2; j++) result = env.eval(it[j]);
        }
            
        return result;
    }
    throw LispError("condForm: All conditions are false.");
}

ValuePtr beginForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    ValuePtr result;
    for(int i = 1; i <= args.size() - 2; i++)
    {
        result = env.eval(args[i]);
    }
    return result;
}

ValuePtr letForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    auto params = args[1]->toVector();
    std::vector<std::string> lparams;
    std::vector<ValuePtr> largs;
    for(int i = 0; i <= params.size() - 2; i++)
    {
        auto tp = std::dynamic_pointer_cast<PairValue>(params[i]);
        lparams.push_back(tp->getL()->toString());
        largs.push_back(std::dynamic_pointer_cast<PairValue>(tp->getR())->getL());
    }
    std::vector<ValuePtr> lbody;
    for(int i = 2; i <= args.size() - 2; i++)
    {
        lbody.push_back(args[i]);
    }
    lparams.push_back("()");
    lbody.push_back(std::make_shared<NilValue>());
    auto l = std::make_shared<LambdaValue>(lparams, lbody, env.shared_from_this());
    return env.apply(l, largs);
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    auto v = args[1]->toVector();
    std::vector<ValuePtr> result;
    for(auto & i : v)
    {
        if(i->isNil()) continue;
        std::vector<ValuePtr> forquote{std::make_shared<NilValue>(), i};
        if(!i->isPair()) result.push_back(quoteForm(forquote, env));
        else
        {
            if(std::dynamic_pointer_cast<PairValue>(i)->getL()->toString() == "unquote")
            {
                auto tp = std::dynamic_pointer_cast<PairValue>(i)->getR();
                result.push_back(env.eval(std::dynamic_pointer_cast<PairValue>(tp)->getL()));
            }
            else result.push_back(quoteForm(forquote, env));
        }
    }
    return vectorToList(result);
}

// ---------- export special forms table to global ----------

std::unordered_map<std::string, SpecialFormType *> SpecialForms = 
{
    {"define", &defineForm},
    {"quote", &quoteForm},
    {"if", &ifForm},
    {"and", &andForm},
    {"or", &orForm},
    {"lambda", &lambdaForm},
    {"cond", &condForm},
    {"begin", &beginForm},
    {"let", &letForm},
    {"quasiquote", &quasiquoteForm},
};