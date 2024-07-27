#include "../src/cube.hpp"
#include "../src/path.hpp"
#include "../src/scene.hpp"
#include <fstream>
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

  ln::Scene s{};
  double n = 3;

  for(double x = -n; x <= n; x++) {

    for(double y = -n; y <= n; y++) {
      for(double z = 0.8; z < 0.9; z += 0.2) {
        auto p = z * 0.25 + 0.2;

        auto fx = x;
        auto fy = y;
        auto fz = z * 3 + 1;
        ln::cube c{{fx - p, fy - p, 0}, {fx + p, fy + p, fz}};

        if(x == 2 && y == 1) {
          continue;
        }
        auto cs = std::make_shared<ln::cube>(c);
        s.Add(cs);
      }
    }
  }

  // define camera parameters
  ln::Vec3 eye{1.75, 1.25, 6}; // camera position
  ln::Vec3 center{0, 0, 0}; // camera looks at
  ln::Vec3 up{0, 0, 1}; // up direction

  // define rendering parameters
  auto width = 1024.0; // rendered width
  auto height = 1024.0; // rendered height
  auto fovy = 100.0; // vertical field of view, degrees
  auto znear = 0.1; // near z plane
  auto zfar = 100.0; // far z plane
  auto step = 0.01; // how
  std::cout << s.Size();
  auto ps = s.Render(eye, center, up, width, height, fovy, znear, zfar, step);
  std::ofstream svgFile("scrapers.svg");
  svgFile << ps.toSVG(width, height);
  svgFile.close();
  return 0;
}
