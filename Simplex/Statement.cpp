#include "Statement.h"

using namespace Simplex;

Statement::Statement(const std::vector<double> &function, const std::vector<std::vector<double>> &a, const std::vector<double> &b)
                     : functionVector(function), aMatrix(a), bVector(b) {
  if (functionVector.size() != aMatrix[0].size()) {
    throw std::invalid_argument("Sizes of a and function vector must be equal");
  }
  if (aMatrix.size() != bVector.size()) {
    throw std::invalid_argument("Sizes of a and b must be equal");
  }
  bSign = std::vector<EquationSign>(bVector.size(), EquationSign::LessOrEqual);
}
