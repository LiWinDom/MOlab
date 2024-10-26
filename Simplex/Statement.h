#pragma once

#include <vector>

#include "Enums.h"

namespace Simplex {
  struct Statement final {
   public:
    Statement() {}
    Statement(const std::vector<double>& function, const std::vector<std::vector<double>>& a, const std::vector<double>& b);

    std::vector<double> functionVector;
    LimitTo functionLimit = LimitTo::Min;

    std::vector<std::vector<double>> aMatrix;
    std::vector<double> bVector;
    std::vector<EquationSign> bSign;
  };
}
