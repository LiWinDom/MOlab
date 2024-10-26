#pragma once

namespace Simplex {
  enum struct LimitTo {
    Min,
    Max,
  };

  enum struct EquationSign {
    LessOrEqual,
    MoreOrEqual,
  };

  enum struct Result {
    NoneSolutions,
    OneSolution,
    InfiniteSolutions,
  };
}
