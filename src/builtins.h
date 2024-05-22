#include <memory>
#include <vector>
#include <unordered_map>
#include "value.h"
#include "error.h"

// ---------- Core Library ----------

BuiltinFuncType printProc;

BuiltinFuncType displayProc;

BuiltinFuncType exitProc;

BuiltinFuncType newlineProc;

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