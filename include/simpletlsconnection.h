#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"

/**
 * @brief Установление и управление TLS-соединением с сервером.
 */
class SimpleTLSConnection
{
 protected:
  std::string host;  // Хост сервера
  std::string port;  // Порт сервера

  net::io_context ioCtx;                        // Контекст ввода-вывода
  ssl::context sslCtx;                          // Контекст SSL
  tcp::endpoint endpoint;                       // Сетевая конечная точка
  beast::flat_buffer dataBuffer;                // Буфер для данных
  beast::ssl_stream<beast::tcp_stream> stream;  // SSL-поток

 public:
  /**
   * @brief Конструктор класса SimpleTLSConnection.
   * @param host Хост сервера
   * @param port Порт сервера
   * @throws std::exception в случае ошибки инициализации
   */
  SimpleTLSConnection(std::string host, std::string port) noexcept(false);

  /**
   * @brief Деструктор класса SimpleTLSConnection.
   */
  ~SimpleTLSConnection();

  /**
   * @brief Отправляет HTTP-запрос и возвращает HTTP-ответ.
   * @tparam BodyType Тип тела HTTP-запроса и ответа
   * @param request HTTP-запрос
   * @return Указатель на HTTP-ответ
   * @throws std::exception в случае ошибки отправки запроса или получения ответа
   */
  template<typename BodyType>
  std::shared_ptr<http::response<BodyType>> sendRequest(const http::request<BodyType>& request) noexcept(false)
  {
    beast::error_code errorCode{};
    http::write(this->stream, request, errorCode);
    if (errorCode.failed())
    {
      throw beast::system_error{ errorCode, "making request" };
    }

    auto response = std::make_shared<http::response<BodyType>>();
    http::read(this->stream, this->dataBuffer, *response, errorCode);
    if (errorCode.failed())
    {
      throw beast::system_error{ errorCode, "getting response" };
    }

    return response;
  }
  /**
   * @brief Отправляет HTTP GET-запрос и возвращает HTTP-ответ.
   * @tparam BodyType Тип тела HTTP-запроса и ответа
   * @param target Цель запроса (URI)
   * @return Указатель на HTTP-ответ
   * @throws std::exception в случае ошибки отправки запроса или получения ответа
   */
  template<typename BodyType>
  std::shared_ptr<http::response<BodyType>> sendGet(const std::string& target) noexcept(false)
  {
    http::request<BodyType> request{ http::verb::get, target, 11 };
    request.set(http::field::host, this->host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    return this->sendRequest(request);
  }

  /**
   * @brief Загружает файл и сохраняет его в буфере.
   * @param request HTTP-запрос для загрузки файла
   * @param buf Буфер для сохранения данных файла
   * @return Код состояния HTTP-ответа
   * @throws std::exception в случае ошибки отправки запроса или загрузки файла
   */
  int loadFile(const http::request<http::dynamic_body>& request, boost::asio::streambuf& buf) noexcept(false)
  {
    beast::error_code errorCode{};
    http::write(this->stream, request, errorCode);
    if (errorCode.failed())
    {
      throw beast::system_error{ errorCode, "making load file request" };
    }

    http::response<http::dynamic_body> response;
    http::read(this->stream, this->dataBuffer, response, errorCode);
    if (errorCode.failed())
    {
      throw beast::system_error{ errorCode, "loading file" };
    }

    buf.commit(boost::asio::buffer_copy(buf.prepare(beast::buffer_bytes(response.body().cdata())), response.body().cdata()));
    return response.result_int();
  }

  /**
   * @brief Загружает файл по URL и сохраняет его в буфере.
   * @param target URL цели загрузки файла
   * @param buf Буфер для сохранения данных файла
   * @return Код состояния HTTP-ответа
   * @throws std::exception в случае ошибки отправки запроса или загрузки файла
   */
  int loadFile(const std::string& target, boost::asio::streambuf& buf) noexcept(false)
  {
    http::request<http::dynamic_body> request{ http::verb::get, target, 11 };
    request.set(http::field::host, this->host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    return this->loadFile(request, buf);
  }
};

#pragma GCC diagnostic pop
