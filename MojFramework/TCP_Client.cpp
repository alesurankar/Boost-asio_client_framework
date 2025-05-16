#include "TCP_Client.h"
#include <chrono>
#include <thread>
#include <iostream>

using namespace boost;

ChatClient::ChatClient(asio::io_context& io_in, const std::string& host_in, unsigned short port_in, const std::string& username_in, std::shared_ptr<MessageHandler> msgHandler_in)
    :
    socket(io_in),
    username(username_in),
    host(host_in),
    port(port_in),
    msgHandler(msgHandler_in),
    timer(io_in)
{}


void ChatClient::Start()
{
    //std::cout << "NetworkingThread::ChatClient::Start\n\n";
    auto self = shared_from_this();
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(host), port);

    socket.async_connect(endpoint, [this, self](boost::system::error_code ec)
        {
            if (!ec)
            {
                boost::asio::ip::tcp::no_delay option(true);
                socket.set_option(option);
                SendUsername();
                ConfirmUsername();
                boost::asio::post(socket.get_executor(), [this, self]()
                    {
                        ReceiveMessages();
                        CheckAndSend();
                    });
            }
            else
            {
                //std::cerr << "Connect failed: " << ec.message() << "\n";
            }
        });
}

void ChatClient::SendUsername()
{
    //std::cout << "ChatClient::SendUsername(): "<< username << "\n";
    asio::write(socket, asio::buffer(username + "\n"));
}

void ChatClient::ConfirmUsername()
{
    //std::cout << "void ChatClient::ConfirmUsername():\n";
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, '\n');

    std::istream is(&buf);
    std::string reply;
    std::getline(is, reply);
    std::cout << "Server reply: " << reply << "\n";
}

void ChatClient::ReceiveMessages()
{
    //float dt = ft.Mark();
    //float dtMs = dt * 1000.0f;
    //std::cout << "void App::UpdateLoop(): Frame Time: " << dtMs << " ms\n";
    auto self = shared_from_this();
    //timer.expires_after(std::chrono::milliseconds(8));
    //timer.async_wait([this, self](boost::system::error_code ec)
    //    {
            boost::asio::async_read_until(socket, input_buffer, '\n',
                [this, self](boost::system::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        std::istream is(&input_buffer);
                        std::string msg;
                        std::getline(is, msg);

                        msgHandler->ClientToMSG(msg);
                        //std::cout << "Step 12, ChatClient::ReadMessage::Received: " << msg << "\n";
                        ReceiveMessages();
                    }
                    else
                    {
                        Shutdown();
                    }
                });
    //    });
    //std::cout << "Step 12--------------\n";
}


void ChatClient::CheckAndSend()
{
    //std::cout << "ChatClient::CheckAndSendMessage: " << ", Step 3. Client(TCP)\n";
    auto self = shared_from_this();
    msg = msgHandler->MSGToClient();
    if (!msg.empty())
    {
        if (!socket.is_open())
        {
            return;
        }

        boost::asio::async_write(socket, boost::asio::buffer(msg),
            [this, self](const boost::system::error_code& ec, std::size_t)
            {
                if (!ec)
                {
                    //std::cout << "Step 4: NetworkingThread::ChatClient::CheckAndSendMessage: " << msg;
                }
                else
                {
                    //std::cerr << "Send error: " << ec.message() << "\n";
                    Shutdown();
                    return;
                }
                boost::asio::post(socket.get_executor(), [this, self]() 
                    {
                        CheckAndSend();
                    });
                //std::cout << "Step4--------------\n";
            });
    }
    else
    {
        timer.expires_after(std::chrono::milliseconds(2));
        timer.async_wait([this, self](boost::system::error_code ec)
            {
                if (!ec)
                {
                    CheckAndSend();
                }
            });
    }
}


void ChatClient::Shutdown()
{
    socket.cancel();
    socket.shutdown(asio::ip::tcp::socket::shutdown_both); 
    socket.close();
}
