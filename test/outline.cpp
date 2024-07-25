#include "../src/outline_sphere.hpp"
#include "../src/path.hpp"
#include "../src/scene.hpp"
#include <fstream>
#include <iostream>
#include <random>

double dice()
{
  std::random_device r;
  // Choose a random mean between 0.0 and 1.0 .
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> unif(0.0, 1.0);

  return unif(r);
};

int main(int argc, char const* argv[])
{
  auto eye = ln::vector{8, 8, 8};
  auto center = ln::vector{0, 0, 0};
  auto up = ln::vector{0, 0, 1};
  ln::Scene sc{};

  double n = 6.0;

  for(double x = -n; x <= n; x++) {

    for(double y = -n; y <= n; y++) {
      auto z = dice() * 3;

      ln::vector v{x, y, z};
      auto sp = ln::outlineSphere{eye, up, v, 0.45};
      auto spp = std::make_shared<ln::outlineSphere>(sp);
      sc.Add(spp);
      /* code */
    }
  }

  // define rendering parameters
  auto width = 1920.0; // rendered width
  auto height = 1200.0; // rendered height
  auto fovy = 50.0; // vertical field of view, degrees
  auto znear = 0.1; // near z plane
  auto zfar = 100.0; // far z plane
  auto step = 0.01; // how

  auto ps = sc.Render(eye, center, up, width, height, fovy, znear, zfar, step);

  std::ofstream svgFile("outline.svg");
  svgFile << ps.toSVG(width, height);
  svgFile.close();
  return 0;
}
