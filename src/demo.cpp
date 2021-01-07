#include <data_description.hpp>
#include <generators/cpp.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace dds = dpsg::data_description;

constexpr dds::struct_t test("test",
                             dds::value<unsigned int>,
                             dds::variable_t<const float>{"my_const_float"},
                             dds::variable_t<const volatile std::size_t>{
                                 "qualified"},
                             dds::variable_t<char&>{"ref"},
                             dds::typedef_t<float>{"my_type"});

constexpr dds::struct_t test2{"test2", dds::variable_t<const char*>{"string"}};

constexpr auto tpl = std::tuple{test, test2};

int main(int argc, char** argv) {
  namespace fs = std::filesystem;

  try {
    const auto output_path = [&]() -> fs::path {
      if (argc > 1) {
        return argv[1];
      }
      else {
        return fs::current_path() / "output";
      }
    }();

    if (!fs::exists(output_path)) {
      std::cout << "Creating output directory " << output_path << "\n";
      fs::create_directory(output_path);
    }
    else {
      std::cout << "Output directory already exists\n";
    }

    dpsg::generators::cpp17(tpl, std::cout);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}