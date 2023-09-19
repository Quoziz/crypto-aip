#pragma once

#include <boost/beast/http.hpp>
#include <boost/json/parse.hpp>
#include <memory>
#include <queue>
#include <string>

#include "../simpletlsconnection.h"
#include "message.h"

#ifndef CUSTOM_HOST_PORT
#define HOST "api.telegram.org"
#define PORT "443"
#endif

// Использование: /encrypt [ключ] текст или /decrypt [ключ] с отправкой BMP-изображения в качестве документа
#define INVALID_SYNTAX_MESSAGE                                                                        \
  "Использование%3A%20%2Fencrypt%20%5Bключ%5D%20текст%20или%20%2Fdecrypt%20" \
  "%5Bключ%5D%20с%20отправкой%20BMP-изображения%20в%20качестве%20документа"

namespace Telegram
{
  namespace beast = boost::beast;

  class TgBot
  {
   private:
    std::string token;                                   // Токен бота
    SimpleTLSConnection conn;                            // Простое TLS-соединение
    std::queue<std::shared_ptr<PhotoMessage>> messages;  // Очередь фотосообщений

    /**
     * @brief Загружает фотографию по ее идентификатору в буфер.
     * @param fileId Идентификатор файла из Telegram
     * @param buffer Буфер для хранения данных фотографии
     * @throws std::runtime_error в случае ошибки
     */
    void loadPhoto(const std::string &fileId, boost::asio::streambuf &buffer) noexcept(false);

    /**
     * @brief Подтверждает получение обновлений с указанным идентификатором.
     * @param lastUpdateId Идентификатор последнего обновления
     */
    void confirmUpdates(int64_t lastUpdateId);

   public:
    TgBot(std::string token) noexcept(false);

    /**
     * @brief Загружает обновления от Telegram.
     * @throws std::runtime_error в случае ошибки
     */
    void loadUpdates() noexcept(false);

    /**
     * @brief Отправляет сообщение.
     * @param msg Указатель на сообщение
     * @throws std::runtime_error в случае ошибки
     */
    void sendMessage(std::shared_ptr<Message> msg) noexcept(false);

    /**
     * @brief Отправляет фотосообщение.
     * @param msg Указатель на фотосообщение
     * @throws std::runtime_error в случае ошибки
     */
    void sendPhotoMessage(std::shared_ptr<PhotoMessage> msg) noexcept(false);

    /**
     * @brief Отправляет сообщение с сообщением об ошибке недопустимого синтаксиса.
     * @param reciever Получатель сообщения об ошибке
     */
    void sendInvalidSyntaxErrorMessage(std::string reciever);

    /**
     * @brief Получает следующее фотосообщение из очереди.
     * @return Указатель на следующее фотосообщение
     * @throws std::runtime_error, если очередь пуста
     */
    std::shared_ptr<PhotoMessage> getNextMessage() noexcept(false)
    {
      if (this->hasNextMessage())
      {
        auto msg = std::shared_ptr<PhotoMessage>(this->messages.front());
        this->messages.pop();

        return msg;
      }
      else
      {
        throw std::runtime_error("getting message from empty queue");
      }
    }

    /**
     * @brief Проверяет, есть ли следующее фотосообщение в очереди.
     * @return true, если есть следующее фотосообщение, иначе false
     */
    bool hasNextMessage()
    {
      return !this->messages.empty();
    }
  };

}  // namespace Telegram
