#include "../src/cube.hpp"
#include "../src/matrix.hpp"
#include "../src/path.hpp"
#include "../src/scene.hpp"

#include <catch2/catch_all.hpp>

#include <cmath>
#include <iostream>
#include <memory>

TEST_CASE("vector constructor tests", "[vector]")
{
  ln::vector v{};
  ln::vector v2{2.0, 3.0, 4.0};
  ln::vector v3{1.0, 1.0, 1.0};
  auto v4 = v2.normalize();
  auto v5 = ln::randomUnitVector();

  REQUIRE(v.length() == 0.0);
  REQUIRE(v2.lengthSquared() == 29.0);
  REQUIRE(v3.length() == std::sqrt(3.0));
  REQUIRE(v4.length() == std::sqrt(1.0));
  REQUIRE(v5.length() == std::sqrt(1.0));
}

TEST_CASE("vectoral operations on vectors", "[vector]")
{

  ln::vector v1{1.0, 1.0, 1.0};
  ln::vector v2{1.0, 1.0, 1.0};
  auto v3 = v1 + v2;
  auto v4 = v1 - v2;
  auto v5 = v1 * v2;
  auto v6 = v1 / v2;

  REQUIRE(v3.lengthSquared() == 12.0);
  REQUIRE(v4.lengthSquared() == 0.0);
  REQUIRE(v5.lengthSquared() == 3.0);
  REQUIRE(v6.lengthSquared() == 3.0);
}

TEST_CASE("scalar operations on vectors", "[vector]")
{

  const double scalar = 1;
  ln::vector v1{1.0, 1.0, 1.0};

  auto v3 = v1 + 1;
  auto v4 = v1 - 1;
  auto v5 = v1 * 1;
  auto v6 = v1 / 1;

  REQUIRE(v3.lengthSquared() == 12.0);
  REQUIRE(v4.lengthSquared() == 0.0);
  REQUIRE(v5.lengthSquared() == 3.0);
  REQUIRE(v6.lengthSquared() == 3.0);
}

TEST_CASE("vectoral and scalar min-max operations on vectors", "[vector]")
{

  ln::vector v1{1.0, 1.0, 1.0};
  ln::vector v2{2.0, 2.0, 2.0};
  auto v3 = v1.min(v2);
  auto v4 = v1.max(v2);
  ln::vector v5{10.0, 9.0, 8.0};
  auto s5 = v5.minComponent();

  REQUIRE(v3.lengthSquared() == 3.0);
  REQUIRE(v4.lengthSquared() == 12.0);
  REQUIRE(s5 == 8.0);
}

TEST_CASE("complex operations on vectors", "[vector]")
{

  //find minAxis
  ln::vector v1{1.0, 2.0, 3.0};
  ln::vector v2{3.0, 2.0, 3.0};
  ln::vector v3{3.0, 2.0, 1.0};
  auto v4 = v1.minAxis() + v2.minAxis() + v3.minAxis();

  REQUIRE(v4.lengthSquared() == 3.0);
  auto s5 = v2.distanceSquared(v1);
  REQUIRE(s5 == 4.0);

  // dot operation.
  auto s6 = v2.dot(v1);
  REQUIRE(s6 == 16.0);

  //check divide by zero exceptions.
  ln::vector v0{0.0, 2.0, 3.0};
  REQUIRE_THROWS_WITH(v1 / v0, "divide by zero");
  REQUIRE_THROWS_WITH(v1 / 0.0, "divide by zero");

  //comparison test.
  REQUIRE((v1 == v2) == false);
  REQUIRE((v1 == v1) == true);
}

TEST_CASE("print operations on paths", "[path]")
{
  ln::vector v1{1.1, 2.1, 3.0};
  ln::vector v2{3.01, 2.19, 3.0};
  ln::vector v3{5.0, 2.0, 3.0};
  ln::vector v4{6.0, 2.0, 3.0};
  ln::vector v5{7.0, 2.0, 3.0};
  ln::Path p{v1, v2, v3, v4, v5};

  //bounding box
  auto b1 = p.boundingBox();

  //svgpath
  std::cout << "------ svgpath ----- \n";
  std::cout << p.toSVG() << '\n';

  //transform ...
  auto m1 = ln::random_matrix();
  auto p1 = p.transform(m1);
  std::cout << "------ transform ----- \n";
  std::cout << p1.toSVG() << '\n';
}

TEST_CASE("paths test with a cube", "[paths]")
{

  ln::vector v1{-400.0, -400.0, -400.0};
  ln::vector v2{400.0, 400.0, 400.0};
  ln::cube c{v1, v2};
  ln::Paths p = c.paths();
  std::cout << p.toSVG(600, 600);
  // std::ofstream svgFile("cube.svg");
  // svgFile << p.toSVG(600, 600);
  // svgFile.close();
}

TEST_CASE("print matrix", "[matrix]")
{
  ln::matrix m{0};
  std::cout << m;
  auto r = ln::random_matrix();
  std::cout << r;
  // identity
  std::cout << "------ translate ----- \n";
  auto i = ln::identity();
  std::cout << i;
  // translate
  std::cout << "------ translate ----- \n";
  ln::vector v1{4.0, 2.0, 3.0};
  auto mt1 = ln::translatev(v1);
  std::cout << mt1;
  // scale
  std::cout << "------ scale ----- \n";
  auto mt2 = ln::scalev(v1);
  std::cout << mt2;
  const double PI = atan(1) * 4;

  // rotate
  std::cout << "------ rotate ----- \n";
  auto mt3 = ln::rotatev(v1, std::numbers::pi / 2);
  std::cout << mt3;
}

TEST_CASE("matrix operations", "[matrix]")
{
  // identity
  std::cout << "------ identity ----- \n";
  auto i = ln::identity();
  std::cout << i;

  std::cout << "------ rotate ----- \n";
  ln::vector v1{1.0, 1.0, 1.0};
  auto mt3 = ln::rotatev(v1, 2 * std::numbers::pi);
  std::cout << mt3;
  std::cout << "------ lookat ----- \n";
  ln::vector eye{0.5, 0.5, 0.9};
  ln::vector center{0.1, 0.1, 0.2};
  ln::vector up{0.3, 0.1, 0.05};
  auto mt4 = ln::lookAt(eye, center, up);
  std::cout << mt4;

  std::cout << "------ transpose ----- \n";
  auto mt5 = mt4.transpose();
  std::cout << mt5;
}

TEST_CASE("add shape", "[scene]")
{
  ln::EmptyShape e{};
  ln::Scene s{};
  auto sp = std::make_shared<ln::EmptyShape>(e);
  s.Add(sp);
  REQUIRE(s.Size() == 1);
}

// std::ofstream svgFile("cube.svg");
// svgFile << p.toSVG(600, 600);
// svgFile.close();