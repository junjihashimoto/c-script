
#define CATCH_CONFIG_MAIN // main is defined by catch
#include <catch.hpp>
 
int f()
{
  return 2;
}
 
int g()
{
  return 0;
}
 
TEST_CASE("Test", "[sample]")
{
  CHECK( f() == 2 );
  REQUIRE( g() <= 0 );
}

