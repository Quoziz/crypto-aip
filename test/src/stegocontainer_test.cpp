#include <gtest/gtest.h>

#define private   public
#define protected public
#include <crypto/stegocontainer.h>

TEST(StegoContainerTest, ReadWriteInteger)
{
  int8_t buffer[64];
  WriteonlyStegoContainer writer(buffer, sizeof(buffer));

  writer.write<int>(42);

  ReadonlyStegoContainer container(buffer, sizeof(buffer));

  int result = container.read<int>();
  EXPECT_EQ(result, 42);
}

TEST(StegoContainerTest, ReadWriteArray)
{
  int8_t buffer[65];
  WriteonlyStegoContainer writer(buffer, sizeof(buffer));

  int data[] = { 1, 2, 3, 4 };
  writer.write(data, sizeof(data) / sizeof(data[0]));

  ReadonlyStegoContainer container(buffer, sizeof(buffer));

  int result[4];
  container.read(result, sizeof(result) / sizeof(result[0]));
  for (int i = 0; i < 4; ++i)
  {
    EXPECT_EQ(result[i], data[i]);
  }
}

TEST(WriteonlyStegoContainerTest, OverflowWriteInteger)
{
  // Специально создаем маленький буфер для вызова ошибки
  int8_t buffer[4];
  WriteonlyStegoContainer container(buffer, sizeof(buffer));

  // Попытка записать слишком большое число в буфер
  // Эта строка вызовет std::out_of_range exception
  EXPECT_THROW(container.write<int>(12345678), std::out_of_range);
}

TEST(WriteonlyStegoContainerTest, OverflowWriteArray)
{
  // Специально создаем маленький буфер для вызова ошибки
  int8_t buffer[8];
  ReadonlyStegoContainer container(buffer, sizeof(buffer));

  int data[] = { 1, 2, 3, 4 };
  EXPECT_THROW(container.read(data, sizeof(data) / sizeof(data[0])), std::out_of_range);
}