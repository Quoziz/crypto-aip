#include <gtest/gtest.h>

#define private public
#include <crypto/xorencryption.h>  // Подключаем заголовочный файл вашего класса

using namespace Encryption;

// Тестирование метода makeKey
TEST(XOREncryptionTest, MakeKeyTest)
{
  XOREncryption encryption;
  const std::string& key = "mysecretkey";

  int result = encryption.makeKey(key);

  // Проверяем, что результат создания ключа не равен нулю
  EXPECT_NE(result, 0);
}

// Тестирование метода encryptDecrypt
TEST(XOREncryptionTest, EncryptDecryptTest)
{
  XOREncryption encryption;
  std::string key = "mysecretkey";

  std::string originalText = "Hello, World!";

  std::string encryptedText = encryption.encryptDecrypt(originalText, key);

  // Проверяем, что зашифрованный текст не равен исходному
  EXPECT_NE(encryptedText, originalText);

  // Расшифровываем зашифрованный текст с тем же ключом
  std::string decryptedText = encryption.encryptDecrypt(encryptedText, key);

  // Проверяем, что расшифрованный текст равен исходному
  EXPECT_EQ(decryptedText, originalText);
}