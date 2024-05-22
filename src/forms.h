#ifndef FORMS_H
#define FORMS_H

#include "value.h"
#include "eval_env.h"

using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);

SpecialFormType defineForm;

SpecialFormType quoteForm;

SpecialFormType ifForm;

SpecialFormType andForm;

SpecialFormType orForm;

SpecialFormType lambdaForm;

extern std::unordered_map<std::string, SpecialFormType *> SpecialForms;

#endif