#include "../src/outline_sphere.hpp"
#include "../src/scene.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <random>

double dice()
{
  std::random_device r;
  // Choose a random mean between 0.0 and 1.0 .
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> unif(0.0, 1.0);

  return unif(r);
};

std::vector<double> normalize(const std::vector<double>& values, double a, double b)
{
  std::vector<double> result(values.size(), 0.0);
  auto lo = values.front();
  auto hi = values.front();
  for(auto el : values) {
    lo = std::min(lo, el);
    hi = std::max(hi, el);
  }

  for(int i = 0; i < values.size(); i++) {
    auto p = (values[i] - lo) / (hi - lo);
    result[i] = a + p * (b - a);
  }
  return result;
}
std::vector<double> lowPass(const std::vector<double> values, double alpha)
{
  std::vector<double> result(values.size(), 0.0);
  double y;

  for(int i = 0; i < values.size(); i++) {
    y -= alpha * (y - values[i]);
    result[i] = y;
  }
  return result;
}

std::vector<double> lowPassNoise(int n, double alpha, int iterations)
{

  std::vector<double> result(n, 0.0);
  for(int i = 0; i < n; i++) {
    result[i] = dice();
  }

  for(int i = 0; i < iterations; i++) {
    result = lowPass(result, alpha);
  }
  result = normalize(result, -1, 1);

  return result;
}

int main(int argc, char const* argv[])
{

  auto eye = ln::Vec3{8, 8, 8};
  auto center = ln::Vec3{0, 0, 0};
  auto up = ln::Vec3{0, 0, 1};

  ln::Scene sc{};

  for(int a = 0; a < 5; a++) {
    auto n = 20;
    auto xs = lowPassNoise(n, 0.3, 4);
    auto ys = lowPassNoise(n, 0.3, 4);
    auto zs = lowPassNoise(n, 0.3, 4);
    auto ss = lowPassNoise(n, 0.3, 4);
    auto position = ln::Vec3{0, 0, 0};
    for(int i = 0; i < n; i++) {
      // auto sp = ln::outlineSphere{eye, up, position, 0.1};
      auto spp = std::make_shared<ln::outlineSphere>(ln::outlineSphere{eye, up, position, 0.1});
      sc.Add(spp);
      auto s = (ss[i] + 1.0) / (2 * 0.1) + 0.01;
      auto v = ln::Vec3{xs[i], ys[i], zs[i]}.normalize() * s;
      position = position + v;
    }
  }
  auto width = 380.0 * 5;
  auto height = 315.0 * 5;
  auto fovy = 50.0;
  auto ps = sc.Render(eye, center, up, width, height, fovy, 0.1, 100, 0.01);
  //std::cout << ln::BNode::howMany() << std::endl;
  std::ofstream svgFile("beads.svg");
  svgFile << ps.toSVG(width, height);
  svgFile.close();
  return 0;
}

// int main(int argc, char const* argv[])
// {

//   auto eye = ln::Vec3{8, 8, 8};
//   auto center = ln::Vec3{0, 0, 0};
//   auto up = ln::Vec3{0, 0, 1};
//   ln::Scene sc{};
//   auto position = ln::Vec3{0, 0, 0};
//   auto spp = std::make_shared<ln::outlineSphere>(ln::outlineSphere{eye, up, position, 0.1});
//   sc.Add(spp);

//   auto width = 380.0 * 5;
//   auto height = 315.0 * 5;
//   auto fovy = 50.0;
//   auto ps = sc.render(eye, center, up, width, height, fovy, 0.1, 100, 0.01);
//   std::ofstream svgFile("beads.svg");
//   svgFile << ps.toSVG(width, height);
//   svgFile.close();
//   return 0;
// }
