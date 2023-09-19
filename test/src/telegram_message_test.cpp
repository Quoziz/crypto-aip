#include <gtest/gtest.h>

#include <boost/beast/core/buffers_to_string.hpp>

#include "telegram/message.h"  // Include the header file containing your classes
// Test cases for the Message class
TEST(MessageTest, DefaultConstructor)
{
  Telegram::Message message;
  EXPECT_EQ("", message.senderId());
  EXPECT_EQ("", message.text());
}

TEST(MessageTest, ConstructorWithValues)
{
  Telegram::Message message;
  message.senderId() = "123";
  message.text() = "Hello, World!";
  EXPECT_EQ("123", message.senderId());
  EXPECT_EQ("Hello, World!", message.text());
}

// Test cases for the PhotoMessage class
TEST(PhotoMessageTest, DefaultConstructor)
{
  Telegram::PhotoMessage photoMessage;
  EXPECT_EQ("", photoMessage.senderId());
  EXPECT_EQ("", photoMessage.text());
}

TEST(PhotoMessageTest, ConstructorWithValues)
{
  Telegram::PhotoMessage photoMessage;
  photoMessage.senderId() = "456";
  photoMessage.text() = "Test Photo";
  EXPECT_EQ("456", photoMessage.senderId());
  EXPECT_EQ("Test Photo", photoMessage.text());
}

TEST(PhotoMessageTest, EncodeToMultipartFormData)
{
  Telegram::PhotoMessage photoMessage;
  photoMessage.senderId() = "789";
  photoMessage.text() = "Test Photo";

  auto multipartBuffer = photoMessage.encodeToMultipartFormData("boundary123");

  // Verify that the multipartBuffer contains the expected data
  std::string expectedData =
      "--boundary123\r\nContent-Disposition: form-data; "
      "name=\"chat_id\"\r\n\r\n789\r\n--boundary123\r\nContent-Disposition: form-data; name=\"caption\"\r\n\r\nTest "
      "Photo\r\n--boundary123\r\nContent-Disposition: form-data; name=\"document\"; "
      "filename=\"document.bmp\"\r\n\r\n\r\n--boundary123--\r\n";

  // print the actual data
  std::string actualData = boost::beast::buffers_to_string(multipartBuffer->data());

  EXPECT_EQ(expectedData, actualData);
}
