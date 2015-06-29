#include <exception>
#include <iostream>


class ErrorException : std::exception {
public:
  const char *what() const noexcept { return "ErrorException"; }
};

class OpenError : public ErrorException {
};

void func() {
  throw OpenError();
}

int main() {
  try {
    func();
  } catch(ErrorException &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
