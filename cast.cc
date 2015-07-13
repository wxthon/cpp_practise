#include <iostream>
#include <string>

#include <gtest/gtest.h>

class A {
public:
  void print() { ASSERT_EQ(desc_, "print"); }

  void set_desc (const std::string &desc) { desc_ = desc; }

private:
  std::string desc_{ "print" };
};

class B : public A {
public:
  void dump() { ASSERT_EQ(data_, "dump"); }
private:
  std::string data_{ "dump" };
};

TEST(cast, static_cast_test) {
  A a;
  B *b = static_cast<B *>(&a);
  //b->dump(); // runtime error
  b->print();
}

TEST(cast, dynamic_cast_test) {
  //A a; // compile error
  B a; // ok
  B *b = dynamic_cast<B *>(&a);
  b->dump();
  b->print();
}

TEST(cast, reinterpret_cast_test) {
  A a;
  long addr = reinterpret_cast<long>(&a);
  ASSERT_EQ((long)&a, addr);
}

TEST(cast, const_cast_test) {
  const A a;
  //a.set_desc("const"); // compile error
  A *pa = const_cast<A *>(&a);
  pa->set_desc("const");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
