#include "request_reply_interface.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MockRequestReplyInterface : public RequestReplyInterface 
{
 public:
  MOCK_METHOD0(Init,
      bool());
  MOCK_METHOD2(Connect,
      bool(const char *server, unsigned short port));
  MOCK_METHOD1(Send,
      bool(unsigned int number));
  MOCK_METHOD0(GetReply,
      bool());
};

using ::testing::AtLeast;

TEST(InterfaceTest, Foo)
{
	MockRequestReplyInterface interface;

	EXPECT_CALL(interface, GetReply()).Times(AtLeast(1));
}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
