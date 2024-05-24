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

SpecialFormType condForm;

SpecialFormType beginForm;

SpecialFormType letForm;

SpecialFormType quasiquoteForm;

ValuePtr vectorToList(const std::vector<ValuePtr> & v);

extern std::unordered_map<std::string, SpecialFormType *> SpecialForms;

#endif