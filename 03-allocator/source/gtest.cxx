#include <gtest/gtest.h>

namespace Test {

TEST(AllocatorTests, DraftForTest) {
  volatile bool isEqual = true;
  ASSERT_TRUE(isEqual);
}

}  // namespace Test
