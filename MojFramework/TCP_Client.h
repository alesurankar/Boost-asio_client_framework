#pragma once
#define _WIN32_WINNT 0x0601
#include "MessageHandler.h"
#include "FrameTimer.h"
#include <boost/asio.hpp>
#include <string>
#include <atomic>
#include <mutex>
#include <memory>

class ChatClient : public std::enable_shared_from_this<ChatClient>
{
public:
    ChatClient(boost::asio::io_context& io_in, const std::string& host_in, unsigned short port_in, const std::string& username_in, std::shared_ptr<MessageHandler> msgHandler_in);
    void Start();
private:
    void SendUsername();
    void ConfirmUsername();
    void ReceiveMessages();
    void CheckAndSend();
    void Shutdown();
private:
    FrameTimer ft;
    boost::asio::ip::tcp::socket socket;
    std::string username;
    std::string host;
    unsigned short port;
    std::shared_ptr<MessageHandler> msgHandler;
    boost::asio::steady_timer timer;
    std::string msg;
    boost::asio::streambuf input_buffer;
};