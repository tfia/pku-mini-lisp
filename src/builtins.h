#include <memory>
#include <vector>
#include <unordered_map>
#include "value.h"
#include "error.h"

ValuePtr print(const std::vector<ValuePtr> & params);

ValuePtr add(const std::vector<ValuePtr> & params);

ValuePtr sub(const std::vector<ValuePtr> & params);
