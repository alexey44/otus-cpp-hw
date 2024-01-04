
// #include <cstdlib>
#include <iostream>

// int main(int argc, char const *argv[])
int main() {
  try {
    std::cout << "[main] Hello world 3" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
