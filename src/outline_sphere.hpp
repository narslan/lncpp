#pragma once
#include "sphere.hpp"
namespace ln {

  struct outlineSphere : sphere
  {
    outlineSphere(Vec3 eye, Vec3 up, Vec3 center, double radius);
    ~outlineSphere() = default;

    Vec3 eye_;
    Vec3 up_;
    Paths paths() override;
  };
} // namespace ln