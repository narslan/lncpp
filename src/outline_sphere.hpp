#pragma once
#include "sphere.hpp"
namespace ln {

  struct outlineSphere : sphere
  {
    outlineSphere(vector eye, vector up, vector center, double radius);
    ~outlineSphere() = default;

    vector eye_;
    vector up_;
    Paths paths() override;
  };
} // namespace ln