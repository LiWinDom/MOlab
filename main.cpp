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

  // Bruteforce
  double ans = 0;
  double x1, x2, x3;
  for (auto i = 0; i < 10; ++i) {
    for (auto j = 0; j < 10; ++j) {
      for (auto k = 0; k < 10; ++k) {
        for (auto l = 0; l < statement.bVector.size(); ++l) {
          if (i * statement.aMatrix[l][0] + j * statement.aMatrix[l][1] + k * statement.aMatrix[l][2] > statement.bVector[l]) {
            goto next;
          }
        }
        // Every statement is correct
        if (i * statement.functionVector[0] + j * statement.functionVector[1] + k * statement.functionVector[2] > ans) {
          ans = i * statement.functionVector[0] + j * statement.functionVector[1] + k * statement.functionVector[2];
          x1 = i;
          x2 = j;
          x3 = k;
        }
        next:
      }
    }
  }
  std::cout << std::endl << "Answer with bruteforce: " << ans << std::endl;
  std::cout << "x1: " << x1 << ", x2: " << x2 << ", x3: " << x3;

  return 0;
}
