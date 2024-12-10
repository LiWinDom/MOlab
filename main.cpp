#include <iostream>

#include "Simplex.h"

int main() {
  Simplex::Statement statementA, statementB;

  statementA.functionVector = {1, 1, 1, 1, 1};
  statementA.functionLimit = Simplex::LimitTo::Min;
  statementA.aMatrix = {
      {6, 17, 16, 18, 15},
      {18, 8, 16, 8, 8},
      {6, 13, 28, 4, 3},
      {15, 14, 12, 18, 19},
  };
  statementA.bVector = {1, 1, 1, 1};
  statementA.bSign = {
      Simplex::EquationSign::MoreOrEqual,
      Simplex::EquationSign::MoreOrEqual,
      Simplex::EquationSign::MoreOrEqual,
      Simplex::EquationSign::MoreOrEqual,
  };

  statementB.functionVector = {1, 1, 1, 1};
  statementB.functionLimit = Simplex::LimitTo::Max;
  statementB.aMatrix = {
      {6, 18, 6, 15},
      {17, 8, 13, 14},
      {16, 16, 28, 12},
      {18, 8, 4, 18},
      {15, 8, 3, 19},
  };
  statementB.bVector = {1, 1, 1, 1, 1};
  statementB.bSign = {
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
      Simplex::EquationSign::LessOrEqual,
  };

  double answerA, answerB;
  auto resultA = Simplex::getSolution(statementA, answerA);
  auto resultB = Simplex::getSolution(statementB, answerB);

  switch (resultA) {
    case Simplex::Result::NoneSolutions:
      std::cout << "None solutions found" << std::endl;
      break;
    case Simplex::Result::OneSolution:
      std::cout << "One solution found: " << answerA << ' ' << answerB << std::endl;
      break;
    case Simplex::Result::InfiniteSolutions:
      std::cout << "Infinite solutions found" << std::endl;
      break;
  }

  return 0;
}
