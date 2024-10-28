#include <iostream>

#include "Simplex.h"

int main() {
  Simplex::Statement statement;

  // Variant 6
  statement.functionVector = {2, 5, 3};
  statement.functionLimit = Simplex::LimitTo::Max;
  statement.aMatrix = {
      {2, 1, 2},
      {1, 2, 0},
      {0, 0.5, 1},
      {0, -1, 0},
  };
  statement.bVector = {6, 6, 2, -3};
  statement.bSign = {
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
  };

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
