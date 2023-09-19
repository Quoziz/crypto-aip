#pragma once

#include <boost/asio/streambuf.hpp>
#include <boost/beast/core/multi_buffer.hpp>
#include <boost/beast/core/ostream.hpp>
#include <memory>
#include <ostream>
#include <string>

namespace Telegram
{

  class Message
  {
   protected:
    std::string msgSenderId;  // Идентификатор отправителя сообщения
    std::string msgText;      // Текст сообщения

   public:
    Message()
    {
    }

    /**
     * @brief Получает идентификатор отправителя сообщения.
     * @return Идентификатор отправителя
     */
    std::string& senderId()
    {
      return this->msgSenderId;
    }

    /**
     * @brief Получает текст сообщения.
     * @return Текст сообщения
     */
    std::string& text()
    {
      return this->msgText;
    }

    /**
     * @brief Получает константный идентификатор отправителя сообщения.
     * @return Константный идентификатор отправителя
     */
    const std::string& senderId() const
    {
      return this->msgSenderId;
    }

    /**
     * @brief Получает константный текст сообщения.
     * @return Константный текст сообщения
     */
    const std::string& text() const
    {
      return this->msgText;
    }
  };

  class PhotoMessage : public Message
  {
   protected:
    boost::asio::streambuf msgBinaryData;  // Бинарные данные сообщения

   public:
    PhotoMessage() : Message()
    {
    }

    /**
     * @brief Кодирует сообщение в многокомпонентную форму с учетом переданной границы.
     * @param boundary Граница многокомпонентной формы
     * @return Указатель на многокомпонентный буфер, содержащий закодированное сообщение
     */
    std::shared_ptr<boost::beast::multi_buffer> encodeToMultipartFormData(const std::string& boundary)
    {
      std::string innerBoundary = "--" + boundary;
      auto bufPtr = std::make_shared<boost::beast::multi_buffer>();
      auto os = boost::beast::ostream(*bufPtr.get());

      os << innerBoundary << "\r\n"
         << "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n"
         << this->msgSenderId << "\r\n"
         << innerBoundary << "\r\n"
         << "Content-Disposition: form-data; name=\"caption\"\r\n\r\n"
         << this->msgText << "\r\n"
         << innerBoundary << "\r\n"
         << "Content-Disposition: form-data; name=\"document\"; filename=\"document.bmp\"\r\n\r\n";
      os.write(static_cast<const char*>(this->msgBinaryData.data().data()), this->msgBinaryData.size());
      os << "\r\n" << innerBoundary << "--\r\n";

      os.flush();
      return bufPtr;
    }

    /**
     * @brief Получает бинарные данные сообщения.
     * @return Бинарные данные сообщения
     */
    boost::asio::streambuf& binaryData()
    {
      return this->msgBinaryData;
    }

    /**
     * @brief Получает константные бинарные данные сообщения.
     * @return Константные бинарные данные сообщения
     */
    const boost::asio::streambuf& binaryData() const
    {
      return this->msgBinaryData;
    }
  };

}  // namespace Telegram
