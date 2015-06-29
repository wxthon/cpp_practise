#include <type_traits>
#include <algorithm>
#include <iostream>

#include <gtest/gtest.h>
#include <boost/type_traits.hpp>

namespace detail{
  template <bool b>
  struct copier {
    template<typename I1, typename I2>
    static I2 do_copy(I1 first, I1 last, I2 out);
  };

  template <bool b>
  template<typename I1, typename I2>
  I2 copier<b>::do_copy(I1 first, I1 last, I2 out) {
    //std::cout << "copying normal ...\n";
    while(first != last) {
      *out = *first;
      ++out;
      ++first;
    }
    return out;
  }

  template <>
  struct copier<true> {
    template<typename I1, typename I2>
    static I2* do_copy(I1* first, I1* last, I2* out) {
      //std::cout << "copying true ...\n";
      memcpy(out, first, (last-first)*sizeof(I2));
      return out+(last-first);
    }
  };
}

template<typename I1, typename I2>
inline I2 copy(I1 first, I1 last, I2 out)
{
  //std::cout << "copying ...\n";
  typedef typename
    boost::remove_cv<typename std::iterator_traits<I1>::value_type>::type v1_t;
  typedef typename
    boost::remove_cv<typename std::iterator_traits<I2>::value_type>::type v2_t;
  enum{ can_opt =
        boost::is_same<v1_t, v2_t>::value
        && boost::is_pointer<I1>::value
        && boost::is_pointer<I2>::value
        && boost::has_trivial_assign<v1_t>::value /* && false */
  };
  return detail::copier<can_opt>::do_copy(first, last, out);
}

TEST(traits, simple) {
  std::array<int, 3> a1{1, 2, 3};
  std::array<int, 3> a2;
  copy(a1.begin(), a1.end(), a2.begin());
  //std::for_each(a2.begin(), a2.end(), [](int num) { std::cout << num << std::endl; });
}

//// test enable_if /////
class A {};
enum E {};

template<typename T>
void test_fun(T t, typename std::enable_if<std::is_integral<T>::value >::type* = 0)
{
}

TEST(traits, manual) {
  test_fun(3);
  //test_fun(1.0); // compile error
}

TEST(traits, methods) {
  // is_void
  ASSERT_EQ(std::is_void<void>::value, true);
  ASSERT_EQ(std::is_void<bool>::value, false);
  // is_null_pointer
  ASSERT_EQ(std::is_null_pointer<decltype(nullptr)>::value, true);
  ASSERT_EQ(std::is_null_pointer<void *>::value, false);
  ASSERT_EQ(std::is_null_pointer<bool>::value, false);
  // is_integral
  ASSERT_EQ(std::is_integral<int>::value, true);
  ASSERT_EQ(std::is_integral<float>::value, false);
  // is_floating_point
  ASSERT_EQ(std::is_floating_point<int>::value, false);
  ASSERT_EQ(std::is_floating_point<float>::value, true);
  // is_array
  ASSERT_EQ(std::is_array<int>::value, false);
  ASSERT_EQ(std::is_array<int[3]>::value, true);
  // is_enum
  ASSERT_EQ(std::is_enum<A>::value, false);
  ASSERT_EQ(std::is_enum<E>::value, true);
  ASSERT_EQ(std::is_enum<int>::value, false);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
