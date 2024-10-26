#include <iostream>

#include "Simplex.h"

int main() {
  Simplex::Statement statement, statement2;

  // Variant 6
  statement.functionVector = {2, 5, 3};
  statement.functionLimit = Simplex::LimitTo::Max;
  statement.aMatrix = {
      {2, 1, 2},
      {1, 2, 0},
      {0, 0.5, 1},
  };
  statement.bVector = {6, 6, 2};
  statement.bSign = {
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
  };

  // Dvoystvennaya
  statement2.functionVector = {6, 6, 2};
  statement2.functionLimit = Simplex::LimitTo::Min;
  statement2.aMatrix = {
      {2, 1, 0},
      {1, 2, 0.5},
      {2, 0, 1},
  };
  statement2.bVector = {2, 5, 3};
  statement2.bSign = {
      Simplex::EquationSign::MoreOrEqual,
      Simplex::EquationSign::MoreOrEqual,
      Simplex::EquationSign::MoreOrEqual,
  };

  double answer, answer2;
  auto result = Simplex::getSolution(statement, answer);
  auto result2 = Simplex::getSolution(statement2, answer2);

  switch (result) {
    case Simplex::Result::NoneSolutions:
      std::cout << "None solutions found" << std::endl;
      break;
    case Simplex::Result::OneSolution:
      std::cout << "One solution found: " << answer << " " << answer2 << std::endl;
      break;
    case Simplex::Result::InfiniteSolutions:
      std::cout << "Infinite solutions found" << std::endl;
      break;
  }

  return 0;
}
