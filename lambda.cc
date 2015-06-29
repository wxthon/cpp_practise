/*************************/
/*     C++11 Test        */
/*************************/
//     lambda             /
///////////////////////////

#include <iostream>
#include <exception>

#include <gtest/gtest.h>


using namespace std;

TEST(lambda, simple) {

  int i = 10;
  auto func = [&i](int num) {
    i = 20;
  };
  func(i);

  ASSERT_EQ(i, 20);
}

TEST(lambda, simple_no_params) {

  int i = 10;
  auto func = [&i]() {
    i = 20;
  };
  func();

  ASSERT_EQ(i, 20);
}

TEST(lambda, simple_no_params_capture) {
  auto func = []() {
  };
  func();
}

TEST(lambda, call_immediately) {
  int i = 10;
  [&i]() { i = 20; }();
  ASSERT_EQ(i, 20);
}

class Simple {
public:
  Simple(int num): num_(num) {}
  void set_num(int num) { num_ = num; }
  int num() const { return num_; }
private:
  int num_;
};

TEST(lambda, simple_class) {
  Simple obj(10);
  auto func = [&obj](int num) {
    obj.set_num(num);
  };
  func(20);

  ASSERT_EQ(obj.num(), 20);
}

TEST(lambda, simple_class_mutable) {
  Simple obj(10);
  auto func = [obj](int num) mutable {
    obj.set_num(num);
    ASSERT_EQ(obj.num(), 20);
  };
  func(20);

  ASSERT_EQ(obj.num(), 10);
}

TEST(lambda, simple_class_return) {
  Simple obj(10);
  auto func = [&obj](int num) -> bool {
    obj.set_num(num);
    return true;
  };
  ASSERT_EQ(func(20), true);
}

TEST(lambda, simple_class_return_type) {
  Simple obj(10);
  auto func = [&obj](int num) -> float {
    obj.set_num(num);
    return 1.0;
  };
  decltype(func(20)) ret = func(20);
  ASSERT_EQ(ret, 1.0);
}

class LambdaException : public std::exception {
public:
  LambdaException(const char *msg):
    msg_(msg) {}
  const char *what() const noexcept { return msg_.c_str(); }

private:
  string msg_;
};

TEST(lambda, simple_class_with_exception) {
  auto func = [](int num) {//throw() {
    if (num == 20) {
      throw LambdaException ("lambda exception test");
    }
  };
  try {
    func(20);
  } catch (LambdaException &e) {
    //std::cout << "LambdaException: " << e.what() << std::endl;
  } catch(std::exception &e) {
    //std::cout << "std::exception: " << e.what() << std::endl;
  }
}

TEST(lambda, simple_with_attributes) {
  // use c++14
#if 0
  auto func = []() [[deprecated("test")]] {
  //auto func = []() [[noreturen]] {
  };
  func();
#endif
}

TEST(lambda, complex) {
  // generic lambda, operator() is a template with two parameters
  auto glambda = [](auto a, auto&& b) { return a < b; };
  ASSERT_EQ(glambda(3, 3.14), true);

  // generic lambda, operator() is a template with one parameter
  auto vglambda = [](auto printer) {
    return [=](auto&&... ts) { // generic lambda, ts is a parameter pack
      printer(std::forward<decltype(ts)>(ts)...);
      return [=]{ printer(ts...); }; // nullary lambda (takes no parameters)
    };
  };

  auto p = vglambda( [](auto v1, auto v2, auto v3) {
      //std::cout << v1 << v2 << v3;
      ASSERT_EQ(v1, 1);
      ASSERT_EQ(v2, 'a');
      ASSERT_EQ(v3, 3.14);
    } );
  auto q = p(1, 'a', 3.14); // outputs 1a3.14
  q();                      // outputs 1a3.14
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
