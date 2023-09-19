#pragma once

#include <boost/asio/streambuf.hpp>
#include <iostream>

/**
 * @brief Базовый класс StegoContainer (Контейнер стеганографии)
 */
class StegoContainer
{
 protected:
  int8_t *buf;  // Указатель на буфер данных
  size_t pos;   // Текущая позиция в буфере
  size_t size;  // Размер буфера

 public:
  /**
   * @brief Конструктор класса StegoContainer
   * @param data Указатель на массив данных
   * @param length Длина массива данных
   */
  StegoContainer(int8_t *data, size_t length) : buf(data), pos(0), size(length)
  {
  }
};

/**
 * @brief Контейнер стеганографии для записи
 */
class WriteonlyStegoContainer : public StegoContainer
{
 public:
  /**
   * @brief Конструктор класса WriteonlyStegoContainer
   * @param data Указатель на массив данных
   * @param length Длина массива данных
   */
  WriteonlyStegoContainer(int8_t *data, size_t length) : StegoContainer(data, length)
  {
  }

  /**
   * @brief Записывает объект типа @ref T в контейнер; @ref T - только целочисленный тип
   * @param what Объект для записи
   * @throws std::out_of_range в случае переполнения контейнера
   */
  template<typename T>
  void write(T what)
  {
    static_assert(std::numeric_limits<T>::is_integer, "only integer types are allowed");
    if (this->pos + sizeof(T) * 4 >= this->size)
      throw std::out_of_range("stego container overflow");

    for (uint16_t i = this->pos, reps = 0; reps < sizeof(T); i += 4, ++reps)
    {
      int8_t ch = static_cast<int8_t>(what);
      this->buf[i] = (this->buf[i] & 0b11111100) | (ch & 0b11);
      this->buf[i + 1] = (this->buf[i + 1] & 0b11111100) | ((ch >> 2) & 0b11);
      this->buf[i + 2] = (this->buf[i + 2] & 0b11111100) | ((ch >> 4) & 0b11);
      this->buf[i + 3] = (this->buf[i + 3] & 0b11111100) | ((ch >> 6) & 0b11);
      what >>= 8;
    }

    this->pos += sizeof(T) * 4;
  }

  /**
   * @brief Записывает массив объектов типа @ref T в контейнер; @ref T - только целочисленный тип
   * @param what Массив объектов для записи
   * @param length Размер массива @ref what
   * @throws std::out_of_range в случае переполнения контейнера
   */
  template<typename T>
  void write(T *what, size_t length)
  {
    for (size_t i = 0; i < length; ++i)
      this->write(what[i]);
  }
};

/**
 * @brief Контейнер стеганографии для чтения
 */
class ReadonlyStegoContainer : public StegoContainer
{
 public:
  /**
   * @brief Конструктор класса ReadonlyStegoContainer
   * @param data Указатель на массив данных
   * @param length Длина массива данных
   */
  ReadonlyStegoContainer(int8_t *data, size_t length) : StegoContainer(data, length)
  {
  }

  /**
   * @brief Читает объект типа @ref T из контейнера; @ref T - только целочисленный тип
   * @return Объект из контейнера
   * @throws std::out_of_range в случае переполнения контейнера
   */
  template<typename T>
  T read()
  {
    static_assert(std::numeric_limits<T>::is_integer, "only integer types are allowed");
    if (this->pos + sizeof(T) * 4 >= this->size)
      throw std::out_of_range("stego container overflow");

    T result{ 0 };
    for (uint16_t i = this->pos, reps = 0; reps < sizeof(T); i += 4, ++reps)
    {
      result |= ((buf[i] & 0b11) << 0);
      result |= ((buf[i + 1] & 0b11) << 2);
      result |= ((buf[i + 2] & 0b11) << 4);
      result |= ((buf[i + 3] & 0b11) << 6);
    }

    this->pos += sizeof(T) * 4;

    return result;
  }

  /**
   * @brief Читает массив объектов типа @ref T из контейнера; @ref T - только целочисленный тип
   * @param where Массив-получатель
   * @param length Размер массива-получателя
   * @throws std::out_of_range в случае переполнения контейнера
   */
  template<typename T>
  void read(T *where, size_t length)
  {
    for (size_t i = 0; i < length; ++i)
      where[i] = this->read<T>();
  }
};