#include "../src/cube.hpp"
#include "../src/path.hpp"
#include "../src/scene.hpp"
#include <fstream>

double dice()
{
  std::random_device r;
  // Choose a random mean between 0.0 and 1.0 .
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> unif(0.0, 1);

  return unif(r);
};

std::shared_ptr<ln::cube> makeCube(double x, double y, double z)
{
  auto size = 0.5;
  auto v = ln::Vec3{x, y, z};
  ln::cube c{v - size, v + size};
  return std::make_shared<ln::cube>(c);
}

int main(int argc, char const* argv[])
{
  ln::Scene s{};

  for(double x = -2; x <= 2; x++) {
    for(double y = -2; y <= 2; y++) {
      for(double z = 0.1; z <= 0.9; z += 0.2) {
        s.Add(makeCube(x, y, z));
      }
    }
  }

  // define camera parameters
  ln::Vec3 eye{6, 5, 3}; // camera position
  ln::Vec3 center{0, 0, 0}; // camera looks at
  ln::Vec3 up{0, 0, 1}; // up direction

  // define rendering parameters
  constexpr auto width = 1920.0; // rendered width
  constexpr auto height = 1920.0; // rendered height
  auto fovy = 30.0; // vertical field of view, degrees
  auto znear = 0.1; // near z plane
  auto zfar = 100.0; // far z plane
  auto step = 0.01; // how

  auto ps = s.Render(eye, center, up, width, height, fovy, znear, zfar, step);
  std::ofstream svgFile("cube.svg");
  svgFile << ps.toSVG(1920.0, 1920.0);
  svgFile.close();
  return 0;
}
