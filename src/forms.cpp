#include "forms.h"
#include <iostream>
#include <string>

using namespace std::literals;

ValuePtr defineForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    if(auto name = args[1]->asSymbol())
    {
        // std::cout << args[2]->toString() << std::endl;
        env.addVar(name.value(), args[2]);
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
        auto val = lambdaForm(vin, env);
        env.addVar(name.value(), val);
        return std::make_shared<NilValue>();
    }
    throw LispError("Unimplemented");
}

ValuePtr quoteForm(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    return args[1];
    throw LispError("Unimplemented");
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
    return std::make_shared<LambdaValue>(params, body);
}

std::unordered_map<std::string, SpecialFormType *> SpecialForms = 
{
    {"define", &defineForm},
    {"quote", &quoteForm},
    {"if", &ifForm},
    {"and", &andForm},
    {"or", &orForm},
    {"lambda", &lambdaForm},
};