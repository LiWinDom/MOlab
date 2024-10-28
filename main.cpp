#include <iostream>

#include "Simplex.h"

int main() {
  Simplex::Statement statement;

  statement.functionVector = {2, 6, 7};
  statement.functionLimit = Simplex::LimitTo::Max;
  statement.aMatrix = {
      {3, 1, 1},
      {1, 2, 0},
      {0, 0.5, 2},
  };
  statement.bVector = {3, 8, 1};
  statement.bSign = {
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
  };
  statement.fractional = false;

  double answer;
  auto result = Simplex::getSolution(statement, answer);

  switch (result) {
    case Simplex::Result::NoneSolutions:
      std::cout << "None solutions found" << std::endl;
      break;
    case Simplex::Result::OneSolution:
      std::cout << "One solution found: " << answer << std::endl;
      break;
    case Simplex::Result::InfiniteSolutions:
      std::cout << "Infinite solutions found" << std::endl;
      break;
  }

  return 0;
}
