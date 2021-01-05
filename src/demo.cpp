#include <data_description.hpp>
#include <generate.hpp>
#include <generators/cpp.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace dds = dpsg::data_description;

constexpr dds::struct_t test("test", dds::value<float>);

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

    std::ofstream out(output_path);
    dpsg::generate(test, dpsg::generators::cpp17, out);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}