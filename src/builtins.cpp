#include "builtins.h"
#include <iostream>
#include <cmath>

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);

// ---------- Core Library ----------

ValuePtr applyProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("applyProc: Needed 2 params.");
    if(!params[0]->isBuiltinProc())
        throw SyntaxError("applyProc: Param #1 should be Proc.");
    std::vector<ValuePtr> vin = params[1]->toVector();
    vin.erase(vin.end() - 1); // the last element is NilValue. delete it
    return env.apply(params[0], vin);
}

ValuePtr printProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    for(const auto & i : params)
        std::cout << i->toString() << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr displayProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    for(const auto & i : params)
    {
        if(i->isString())
            std::cout << std::dynamic_pointer_cast<StringValue>(i)->getString();
        else std::cout << i->toString();
    }
    return std::make_shared<NilValue>();
}

ValuePtr displaylnProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    for(const auto & i : params)
    {
        if(i->isString())
            std::cout << std::dynamic_pointer_cast<StringValue>(i)->getString();
        else std::cout << i->toString();
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr exitProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() == 0) exit(0);
    else if(params.size() == 1 && params[0]->isNumeric()) exit(params[0]->asNumeric().value());
    throw SyntaxError("exitProc: Needed 0 or 1 params.");
}

ValuePtr newlineProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr evalProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("evalProc: Needed only 1 param.");
    return env.eval(params[0]);
}

ValuePtr errorProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() == 0) throw LispError(0);
    else if(params.size() == 1 && params[0]->isNumeric()) 
        throw LispError(std::dynamic_pointer_cast<NumericValue>(params[0])->toString());
    throw SyntaxError("exitProc: Needed 0 or 1 params.");
}

// ---------- Type Identification Library ----------

ValuePtr atomProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("atomProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    bool result = (tp->isBoolean() || tp->isNumeric() || tp->isString()
        || tp->isSymbol() || tp->isNil());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr booleanProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("booleanProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    bool result = tp->isBoolean();
    return std::make_shared<BooleanValue>(result);
}

ValuePtr integerProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("integerProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(!tp->isNumeric()) return std::make_shared<BooleanValue>(false);
    double t = std::dynamic_pointer_cast<NumericValue>(tp)->toDouble();
    bool result = ((int)t == t);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr listProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("listProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(tp->isNil()) return std::make_shared<BooleanValue>(true);
    else if(!tp->isPair()) return std::make_shared<BooleanValue>(false);
    auto R = std::dynamic_pointer_cast<PairValue>(tp)->getR();
    if(R->isNil()) return std::make_shared<BooleanValue>(true);
    else if(!R->isPair()) return std::make_shared<BooleanValue>(false);
    return std::make_shared<BooleanValue>(true);
}

ValuePtr numberProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("integerProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(!tp->isNumeric()) return std::make_shared<BooleanValue>(false);
    return std::make_shared<BooleanValue>(true);
}

ValuePtr nullProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("nullProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(tp->isNil()) return std::make_shared<BooleanValue>(true);
    return std::make_shared<BooleanValue>(false);
}

ValuePtr pairProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("pairProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(tp->isPair()) return std::make_shared<BooleanValue>(true);
    return std::make_shared<BooleanValue>(false);
}

ValuePtr procedureProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("procedureProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(tp->isBuiltinProc() || tp->isLambda()) return std::make_shared<BooleanValue>(true);
    return std::make_shared<BooleanValue>(false);
}

ValuePtr stringProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("stringProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(tp->isString()) return std::make_shared<BooleanValue>(true);
    return std::make_shared<BooleanValue>(false);
}

ValuePtr symbolProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("symbolProc: Needed only 1 param.");
    ValuePtr tp = params[0];
    if(tp->isSymbol()) return std::make_shared<BooleanValue>(true);
    return std::make_shared<BooleanValue>(false);
}

// ---------- Pair and List Manipulation Library ----------

ValuePtr vectorToList(const std::vector<ValuePtr> & v)
{
    ValuePtr result = std::make_shared<PairValue>(std::make_shared<NilValue>(), std::make_shared<NilValue>());
    ValuePtr* cur = &result;
    bool isFirst = true;
    for(auto & i : v)
    {
        if(isFirst)
        {
            std::dynamic_pointer_cast<PairValue>(result)->setL(i);
            isFirst = false;
            continue;
        }
        auto tmp = std::make_shared<PairValue>(i, std::make_shared<NilValue>());
        std::dynamic_pointer_cast<PairValue>(*cur)->setR(tmp);
        cur = (std::dynamic_pointer_cast<PairValue>(*cur)->getRp());
    }
    return result;
}

ValuePtr appendProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() == 0) return std::make_shared<NilValue>();
    std::vector<ValuePtr> v;
    for(auto & i : params)
    {
        auto t = i->toVector();
        for(auto & j : t)
        {
            if(j->isNil()) continue;
            v.push_back(j);
        }
    }
    return vectorToList(v);
}

ValuePtr carProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("carProc: Needed only 1 param.");
    return std::dynamic_pointer_cast<PairValue>(params[0])->getL();
}

ValuePtr cdrProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("cdrProc: Needed only 1 param.");
    return std::dynamic_pointer_cast<PairValue>(params[0])->getR();
}

ValuePtr consProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("consProc: Needed 2 params.");
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr lengthProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("lengthProc: Needed only 1 param.");
    return std::make_shared<NumericValue>(params[0]->toVector().size() - 1);
}

ValuePtr listConstructProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() == 0) return std::make_shared<NilValue>();
    return vectorToList(params);
}

ValuePtr mapProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    std::vector<ValuePtr> result;
    if(params.size() != 2) throw SyntaxError("mapProc: Needed 2 params.");
    if(!params[0]->isBuiltinProc() && !params[0]->isLambda())
        throw SyntaxError("mapProc: Param #1 should be Proc.");
    std::vector<ValuePtr> vin = params[1]->toVector();
    vin.erase(vin.end() - 1); // the last element is NilValue. delete it
    for(auto & i : vin)
    {
        auto t = i->toVector();
        result.push_back(env.apply(params[0], t));
    }
    return vectorToList(result);
}

ValuePtr filterProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    std::vector<ValuePtr> result;
    if(params.size() != 2) throw SyntaxError("filterProc: Needed 2 params.");
    if(!params[0]->isBuiltinProc())
        throw SyntaxError("filterProc: Param #1 should be Proc.");
    std::vector<ValuePtr> vin = params[1]->toVector();
    vin.erase(vin.end() - 1); // the last element is NilValue. delete it
    for(auto & i : vin)
    {
        auto t = i->toVector();
        auto j = env.apply(params[0], t);
        if(!j->isBoolean()) result.push_back(i);
        else if(std::dynamic_pointer_cast<BooleanValue>(j)->toBool()) result.push_back(i);
    }
    return vectorToList(result);
}

ValuePtr reduceProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("reduceProc: Needed 2 params.");
    if(!params[0]->isBuiltinProc() && !params[0]->isLambda())
        throw SyntaxError("reduceProc: Param #1 should be Proc.");
    if(params[1]->isNil()) throw SyntaxError("reduceProc: Param #2 should not be Nil.");
    if(params[1]->toVector().size() - 1 == 1)
    {
        return std::dynamic_pointer_cast<PairValue>(params[1])->getL();
    }
    else
    {
        std::vector<ValuePtr> vin;
        vin.push_back(params[0]);
        vin.push_back(std::dynamic_pointer_cast<PairValue>(params[1])->getR());
        auto t = reduceProc(vin, env);
        return env.apply
        (
            params[0],
            std::make_shared<PairValue>(std::dynamic_pointer_cast<PairValue>(params[1])->getL(), t)->toVector()
        );
    }
    throw LispError("reduceProc: unknown error.");
}

// ---------- Arithmetic Library ----------

ValuePtr addProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    double result = 0;
    for(const auto & i : params)
    {
        if(i->isNil()) continue;
        if(!i->isNumeric()) throw LispError("addProc: Cannot add a non-numeric value.");
        result += i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr subProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() == 1) return std::make_shared<NumericValue>(-(params[0]->asNumeric().value()));
    if(params.size() == 2)
    {
        double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
        double result = x - y;
        return std::make_shared<NumericValue>(result);
    }
    throw SyntaxError("subProc: Needed 1 or 2 params.");
}

ValuePtr timeProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    double result = 1;
    for(const auto & i : params)
    {
        if(!i->isNumeric()) throw LispError("timeProc: Cannot add a non-numeric value.");
        result *= i->asNumeric().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() == 1)
    {
        double result = 1 / (params[0]->asNumeric().value());
        return std::make_shared<NumericValue>(result);
    }
    if(params.size() == 2)
    {
        double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
        double result = x / y;
        return std::make_shared<NumericValue>(result);
    }
    throw SyntaxError("divProc: Needed 1 or 2 params.");
}

ValuePtr absProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("absProc: Needed only 1 param.");
    return std::make_shared<NumericValue>(fabs(params[0]->asNumeric().value()));
}

ValuePtr exptProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("exptProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr quotientProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("quotientProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    double result = x / y;
    if(result < 0) result = ceil(result);
    else result = floor(result);
    return std::make_shared<NumericValue>(result);
}

ValuePtr remainderProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("remainderProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    int result = (int)x % (int)y;
    return std::make_shared<NumericValue>(result);
}

ValuePtr moduloProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("moduloProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    int c = floor(x / y);
    double result = x - c * y;
    return std::make_shared<NumericValue>(result);
}

// ---------- Comparison Library ----------

ValuePtr eqMarkProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("eqMarkProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x == y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr lMarkProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("lMarkProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x < y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr gMarkProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("gMarkProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x > y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr leqMarkProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("leqMarkProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x <= y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr geqMarkProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("geqMarkProc: Needed 2 params.");
    double x = params[0]->asNumeric().value(), y = params[1]->asNumeric().value();
    bool result = (x >= y);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr evenProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("evenProc: Needed only 1 param.");
    double x = params[0]->asNumeric().value();
    if((int)x != x) throw LispError("evenProc: Param must be int.");
    bool result = ((int)x % 2 == 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr oddProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("oddProc: Needed only 1 param.");
    double x = params[0]->asNumeric().value();
    if((int)x != x) throw LispError("oddProc: Param must be int.");
    bool result = ((int)x % 2 != 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr zeroProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("zeroProc: Needed only 1 param.");
    double x = params[0]->asNumeric().value();
    bool result = (x == 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr eqProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("eqProc: Needed 2 params.");
    if(typeid(params[0]) != (typeid(params[1])))
        return std::make_shared<BooleanValue>(false);
    if(params[0]->isSymbol() || params[0]->isNumeric() || params[0]->isNil() || params[0]->isBoolean())
        return std::make_shared<BooleanValue>(params[0]->toString() == params[1]->toString());
    bool result = (params[0] == params[1]);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr equalProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 2) throw SyntaxError("equalProc: Needed 2 params.");
    if(typeid(params[0]) != (typeid(params[1])))
        return std::make_shared<BooleanValue>(false);
    return std::make_shared<BooleanValue>(params[0]->toString() == params[1]->toString());
}

ValuePtr notProc(const std::vector<ValuePtr> & params, EvalEnv & env)
{
    if(params.size() != 1) throw SyntaxError("notProc: Needed only 1 param.");
    if(!params[0]->isBoolean()) return std::make_shared<BooleanValue>(false);
    else if(!std::dynamic_pointer_cast<BooleanValue>(params[0])->toBool())
        return std::make_shared<BooleanValue>(true);
    return std::make_shared<BooleanValue>(false);
}

// ---------- export builtin-symbol table to global ----------

const std::unordered_map<std::string, ValuePtr> BuiltinSymbols = 
{
    // ---------- Core Library ----------
    {"apply", std::make_shared<BuiltinProcValue>(&applyProc)},
    {"print", std::make_shared<BuiltinProcValue>(&printProc)},
    {"display", std::make_shared<BuiltinProcValue>(&displayProc)},
    {"displayln", std::make_shared<BuiltinProcValue>(&displaylnProc)},
    {"exit", std::make_shared<BuiltinProcValue>(&exitProc)},
    {"newline", std::make_shared<BuiltinProcValue>(&newlineProc)},
    {"eval", std::make_shared<BuiltinProcValue>(&evalProc)},
    {"error", std::make_shared<BuiltinProcValue>(&errorProc)},
    // ---------- Type Identification Library ----------
    {"atom?", std::make_shared<BuiltinProcValue>(&atomProc)},
    {"boolean?", std::make_shared<BuiltinProcValue>(&booleanProc)},
    {"integer?", std::make_shared<BuiltinProcValue>(&integerProc)},
    {"list?", std::make_shared<BuiltinProcValue>(&listProc)},
    {"number?", std::make_shared<BuiltinProcValue>(&numberProc)},
    {"null?", std::make_shared<BuiltinProcValue>(&nullProc)},
    {"pair?", std::make_shared<BuiltinProcValue>(&pairProc)},
    {"procedure?", std::make_shared<BuiltinProcValue>(&procedureProc)},
    {"string?", std::make_shared<BuiltinProcValue>(&stringProc)},
    {"symbol?", std::make_shared<BuiltinProcValue>(&symbolProc)},
    // ---------- Pair and List Manipulation Library ----------
    {"append", std::make_shared<BuiltinProcValue>(&appendProc)},
    {"car", std::make_shared<BuiltinProcValue>(&carProc)},
    {"cdr", std::make_shared<BuiltinProcValue>(&cdrProc)},
    {"cons", std::make_shared<BuiltinProcValue>(&consProc)},
    {"length", std::make_shared<BuiltinProcValue>(&lengthProc)},
    {"list", std::make_shared<BuiltinProcValue>(&listConstructProc)},
    {"map", std::make_shared<BuiltinProcValue>(&mapProc)},
    {"filter", std::make_shared<BuiltinProcValue>(&filterProc)},
    {"reduce", std::make_shared<BuiltinProcValue>(&reduceProc)},
    // ---------- Arithmetic Library ----------
    {"+", std::make_shared<BuiltinProcValue>(&addProc)},
    {"-", std::make_shared<BuiltinProcValue>(&subProc)},
    {"*", std::make_shared<BuiltinProcValue>(&timeProc)},
    {"/", std::make_shared<BuiltinProcValue>(&divProc)},
    {"abs", std::make_shared<BuiltinProcValue>(&absProc)},
    {"expt", std::make_shared<BuiltinProcValue>(&exptProc)},
    {"quotient", std::make_shared<BuiltinProcValue>(&quotientProc)},
    {"remainder", std::make_shared<BuiltinProcValue>(&remainderProc)},
    {"modulo", std::make_shared<BuiltinProcValue>(&moduloProc)},
    // ---------- Comparison Library ----------
    {"=", std::make_shared<BuiltinProcValue>(&eqMarkProc)},
    {">", std::make_shared<BuiltinProcValue>(&gMarkProc)},
    {"<", std::make_shared<BuiltinProcValue>(&lMarkProc)},
    {">=", std::make_shared<BuiltinProcValue>(&geqMarkProc)},
    {"<=", std::make_shared<BuiltinProcValue>(&leqMarkProc)},
    {"even?", std::make_shared<BuiltinProcValue>(&evenProc)},
    {"odd?", std::make_shared<BuiltinProcValue>(&oddProc)},
    {"zero?", std::make_shared<BuiltinProcValue>(&zeroProc)},
    {"eq?", std::make_shared<BuiltinProcValue>(&eqProc)},
    {"equal?", std::make_shared<BuiltinProcValue>(&equalProc)},
    {"not", std::make_shared<BuiltinProcValue>(&notProc)},
};