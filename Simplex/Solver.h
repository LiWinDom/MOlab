#pragma once

#include "Enums.h"
#include "Statement.h"

namespace Simplex {
  Result getSolution(const Statement &statement, double &answer);
}
