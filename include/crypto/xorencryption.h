#pragma once

#include <boost/asio/streambuf.hpp>
#include <functional>
#include <random>
#include <string>

namespace Encryption
{

  class XOREncryption
  {
   private:
    std::function<int(void)> generator;       // Генератор случайных чисел
    std::normal_distribution<double> normal;  // Нормальное распределение
    std::mt19937 engine;                      // Генератор случайных чисел Mersenne Twister

    /**
     * @brief Создает ключ для шифрования на основе заданной строки.
     * @param key Ключ для шифрования
     * @return Значение ключа, готовое для использования в операции шифрования
     */
    int makeKey(const std::string &key);

   public:
    XOREncryption();

    /**
     * @brief Зашифровывает или расшифровывает текст.
     * Преобразует зашифрованный текст в расшифрованный и наоборот.
     * @param text Текст для шифрования или расшифрования
     * @param key Ключ шифрования
     * @return Зашифрованный или расшифрованный текст
     */
    std::string encryptDecrypt(std::string text, std::string key);
  };

}  // namespace Encryption
