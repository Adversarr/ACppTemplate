#include <doctest/doctest.h>
#include <acpptemplate/version.h>
#include <ACppTemplate.h>

#include <string>

TEST_CASE("Greeter") {
  ACppTemplate::hello_world();
}
