#include "../src/cube.hpp"
#include "../src/path.hpp"
#include "../src/scene.hpp"
#include <fstream>

int main(int argc, char const* argv[])
{
  ln::cube c{{-1, -1, -1}, {1, 1, 1}};

  auto cs = std::make_shared<ln::cube>(c);
  ln::Scene s{};
  s.Add(cs);
  // define camera parameters
  ln::Vec3 eye{4, 3, 2}; // camera position
  ln::Vec3 center{0, 0, 0}; // camera looks at
  ln::Vec3 up{0, 0, 1}; // up direction

  // define rendering parameters
  auto width = 1024.0; // rendered width
  auto height = 1024.0; // rendered height
  auto fovy = 50.0; // vertical field of view, degrees
  auto znear = 0.1; // near z plane
  auto zfar = 10.0; // far z plane
  auto step = 0.01; // how

  auto ps = s.Render(eye, center, up, width, height, fovy, znear, zfar, step);
  std::ofstream svgFile("cube.svg");
  svgFile << ps.toSVG(1024.0, 1024.0);
  svgFile.close();
  return 0;
}
