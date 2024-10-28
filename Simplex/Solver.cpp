#include "Solver.h"

#include <cmath>

#define DEBUG_PRINT 1
#ifndef DEBUG_PRINT
  #define DEBUG_PRINT 0
#endif

#pragma region
#if defined(DEBUG_PRINT) && DEBUG_PRINT
#include <iomanip>
#include <iostream>
#include <vector>

std::vector<std::vector<std::string>> variablesTable(2);

static size_t step = 0;
void fillVariablesTable(size_t rows, size_t columns) {
  variablesTable[0].resize(rows);
  variablesTable[0][rows - 1] = "F";
  variablesTable[1].resize(columns);
  variablesTable[1][0] = "Sio";

  size_t indexer = 1;
  for (indexer; indexer < columns; ++indexer) {
    variablesTable[1][indexer] = "x" + std::to_string(indexer);
  }
  for (auto i = 0; i < rows - 1; ++i, ++indexer) {
    variablesTable[0][i] = "x" + std::to_string(indexer);
  }

  step = 0;
}

void matrixSwap(size_t row, size_t column) {
  std::swap(variablesTable[0][row], variablesTable[1][column]);
}

inline void printDivider(size_t columsNum) {
  std::cout << "+";
  for (auto i = 0; i <= columsNum; ++i) {
    std::cout << "-------+";
  }
  std::cout << std::endl;
}

void printMatrix(const std::vector<std::vector<double>> &matrix) {
  std::cout << "Step " << step++ << std::endl;

  printDivider(matrix[0].size());
  std::cout << "|";
  std::cout << std::right << std::setfill(' ') << std::setw(7) << " " << '|';
  for (auto elem : variablesTable[1]) {
    std::cout << std::right << std::setfill(' ') << std::setw(7) << elem << '|';
  }
  std::cout << std::endl;
  printDivider(matrix[0].size());

  for (auto i = 0; i < matrix.size(); ++i) {
    auto row = matrix[i];
    std::cout << "|" << std::right << std::setfill(' ') << std::setw(7) << variablesTable[0][i] << '|';
    for (auto elem : row) {
      std::cout << std::right << std::setfill(' ') << std::setw(7) << std::setprecision(4) << elem << '|';
    }
    std::cout << std::endl;
    printDivider(row.size());
  }
  std::cout << std::endl;
}

#else
#define fillVariablesTable(...) 42
#define matrixSwap(...) 42
#define printMatrix(...) 42
#endif
#pragma endregion

using namespace Simplex;

std::vector<std::vector<double>> createMatrix(Statement statement) {
  // Function need to be limited to min
  if (statement.functionLimit == LimitTo::Max) {
    for (auto &item : statement.functionVector) {
      item = -item;
    }
  }
  for (auto i = 0; i < statement.aMatrix.size(); ++i) {
    if (statement.bSign[i] == EquationSign::MoreOrEqual) {
      for (auto &item : statement.aMatrix[i]) {
        item = -item;
      }
      statement.bVector[i] = -statement.bVector[i];
    }
  }

  std::vector<std::vector<double>> matrix(statement.aMatrix.size() + 1, std::vector<double>(statement.aMatrix[0].size() + 1));
  // Some black magik
  for (auto i = 0; i < statement.aMatrix.size(); ++i) {
    matrix[i][0] = statement.bVector[i];
    for (auto j = 0; j < statement.aMatrix[i].size(); ++j) {
      matrix[i][j + 1] = statement.aMatrix[i][j];
    }
  }
  matrix[matrix.size() - 1][0] = 0;
  for (auto j = 0; j < statement.functionVector.size(); ++j) {
    matrix[matrix.size() - 1][j + 1] = -statement.functionVector[j];
  }

  fillVariablesTable(matrix.size(), matrix[0].size());
  return matrix;
}

std::vector<std::vector<double>> recalcMatrix(const std::vector<std::vector<double>> &matrix, size_t jMain) {
  // Searching main element
  double minRelation = MAXFLOAT;
  size_t iMain = -1;
  for (auto i = 0; i < matrix.size() - 1; ++i) {
    if (matrix[i][jMain] == 0) {
      continue;
    }
    auto relation = matrix[i][0] / matrix[i][jMain];
    if (relation > 0 && relation < minRelation) {
      minRelation = relation;
      iMain = i;
    }
  }

  matrixSwap(iMain, jMain);

  // Recalculating matrix
  auto newMatrix = matrix;
  for (auto i = 0; i < matrix.size(); ++i) {
    for (auto j = 0; j < matrix[i].size(); ++j) {
      if (i == iMain && j == jMain) {
        // Recalculating main element
        newMatrix[i][j] = 1 / matrix[iMain][jMain];
      }
      else if (i == iMain) {
        // Recalculating main line
        newMatrix[i][j] = matrix[i][j] / matrix[iMain][jMain];
      }
      else if (j == jMain) {
        // Recalculating main column
        newMatrix[i][j] = -(matrix[i][j] / matrix[iMain][jMain]);
      }
      else {
        // Anything else
        newMatrix[i][j] = matrix[i][j] - matrix[i][jMain] * matrix[iMain][j] / matrix[iMain][jMain];
      }
    }
  }

  printMatrix(newMatrix);
  return newMatrix;
}

Result findSolution(std::vector<std::vector<double>> &matrix) {
  start:
  // Searching negative element in first column
  for (auto i = 0; i < matrix.size() - 1; ++i) {
    if (matrix[i][0] < 0) {
      // Searching negative element in this line
      for (auto j = 1; j < matrix[i].size(); ++j) {
        if (matrix[i][j] < 0) {
          // Element found => recalculating matrix and doing everything again
          matrix = recalcMatrix(matrix, j);
          goto start;
        }
      }
      // Didn't find any negative elements in this line => solution doesn't exist
      return Result::NoneSolutions;
    }
  }
  // Didn't find any negative elements in first column => solution found
  return Result::OneSolution;
}

Result optimizeMatrix(std::vector<std::vector<double>> &matrix) {
  start:
  // Searching positive element in last line
  size_t i = matrix.size() - 1;
  for (auto j = 1; j < matrix[i].size(); ++j) {
    if (matrix[i][j] > 0) {
      // Trying to find any positive elements in this column
      for (auto k = 0; k < matrix.size() - 1; ++k) {
        if (matrix[k][j] > 0) {
          // Element found => recalculating matrix
          auto newMatrix = recalcMatrix(matrix, j);
          // Checking
          for (auto k = 0; k < newMatrix.size() - 1; ++k) {
            if (newMatrix[k][0] < 0) {
              // Optimization broke something => trying next column
              #if defined(DEBUG_PRINT) && DEBUG_PRINT
                std::cout << "Incorrect solution - rollback to previous" << std::endl;
              #endif
              goto next;
            }
          }
          // Nothing broke => optimizing further
          matrix = newMatrix;
          goto start;
        }
      }
      // Didn't find any positive elements in this column => infinite solutions exists
      return Result::InfiniteSolutions;
    }
    next:
  }
  // Didn't find any negative elements in first column => solution found
  return Result::OneSolution;
}

double defractionizeMatrix(Statement statement, const std::vector<std::vector<double>> &matrix) {
  // Checking if solution is valid
  for (auto i = 0; i < matrix.size() - 1; ++i) {
    double intPart;
    if (std::modf(matrix[i][0], &intPart) != 0) {
      // Nu eto uzhe perebor...
      #if defined(DEBUG_PRINT) && DEBUG_PRINT
        std::cout << "Spliting x" << i + 1 << std::endl;
      #endif

      double answer1, answer2;

      std::vector<double> vec(statement.functionVector.size(), 0);
      vec[i] = 1;
      statement.aMatrix.push_back(vec);
      statement.bVector.push_back(intPart);
      statement.bSign.push_back(EquationSign::LessOrEqual);

      #if defined(DEBUG_PRINT) && DEBUG_PRINT
        std::cout << "===== x" << i + 1 << " <= " << intPart << " =====" << std::endl;
      #endif
      auto result1 = getSolution(statement, answer1);

      statement.bVector[statement.bVector.size() - 1] = intPart + 1;
      statement.bSign[statement.bSign.size() - 1] = EquationSign::MoreOrEqual;

      #if defined(DEBUG_PRINT) && DEBUG_PRINT
        std::cout << "===== x" << i + 1 << " >= " << intPart + 1 << " =====" << std::endl;
      #endif
      auto result2 = getSolution(statement, answer2);

      if (result1 != Simplex::Result::OneSolution) {
        return answer2;
      }
      if (result2 != Simplex::Result::OneSolution) {
        return answer1;
      }

      if (statement.functionLimit == LimitTo::Min) {
        return std::min(answer1, answer2);
      }
      else {
        return std::max(answer1, answer2);
      }
    }
  }

  // Everything is ok
  double answer = matrix[matrix.size() - 1][0];
  if (statement.functionLimit == LimitTo::Max) {
    answer = -answer;
  }
  return answer;
}

Result Simplex::getSolution(const Statement &statement, double &answer) {
  auto matrix = createMatrix(statement);
  printMatrix(matrix);

  #if defined(DEBUG_PRINT) && DEBUG_PRINT
    #include <iostream>
    std::cout << "Trying to find solution..." << std::endl;
  #endif

  auto result = findSolution(matrix);
  if (result != Result::OneSolution) {
    return result;
  }

  #if defined(DEBUG_PRINT) && DEBUG_PRINT
    std::cout << "Solution found, optimizing it..." << std::endl;
  #endif

  result = optimizeMatrix(matrix);
  if (result != Result::OneSolution) {
    return result;
  }

  if (statement.fractional) {
    answer = matrix[matrix.size() - 1][0];
    if (statement.functionLimit == LimitTo::Max) {
      answer = -answer;
    }
  }
  else {
    answer = defractionizeMatrix(statement, matrix);
  }
  return result;
}
