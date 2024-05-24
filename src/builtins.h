#include <memory>
#include <vector>
#include <unordered_map>
#include "eval_env.h"
#include "error.h"

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);

// ---------- Core Library ----------

BuiltinFuncType applyProc;

BuiltinFuncType printProc;

BuiltinFuncType displayProc;

BuiltinFuncType displaylnProc;

BuiltinFuncType exitProc;

BuiltinFuncType newlineProc;

BuiltinFuncType evalProc;

// ---------- Type Identify Library ----------

BuiltinFuncType atomProc;

BuiltinFuncType booleanProc;

BuiltinFuncType integerProc;

BuiltinFuncType listProc;

BuiltinFuncType nullProc;

BuiltinFuncType pairProc;

BuiltinFuncType procedureProc;

BuiltinFuncType stringProc;

BuiltinFuncType symbolProc;

// ---------- Arithmetic Library ----------

BuiltinFuncType addProc;

BuiltinFuncType subProc;

BuiltinFuncType timeProc;

BuiltinFuncType divProc;

BuiltinFuncType absProc;

BuiltinFuncType exptProc;

BuiltinFuncType quotientProc;

BuiltinFuncType remainderProc;

BuiltinFuncType moduloProc;

// ---------- Compare Library ----------

BuiltinFuncType eqMarkProc;

BuiltinFuncType lMarkProc;

BuiltinFuncType gMarkProc;

BuiltinFuncType leqMarkProc;

BuiltinFuncType geqMarkProc;

BuiltinFuncType evenProc;

BuiltinFuncType oddProc;

BuiltinFuncType zeroProc;

extern const std::unordered_map<std::string, ValuePtr> BuiltinSymbols;