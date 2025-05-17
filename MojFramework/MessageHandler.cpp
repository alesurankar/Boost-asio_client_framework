#include "MessageHandler.h"
#include <iostream>


MessageHandler::MessageHandler()
{}


void MessageHandler::AppToMSG(const std::string& message)
{
    std::lock_guard<std::mutex> lock(IN_mtx);
    app_messages.push(message);
    //std::cout << "void MessageHandler::AppToMSG(const std::string& message): app_messages.push(message): " << message << "\n";
}

std::string MessageHandler::MSGToClient()
{
    {
        std::lock_guard<std::mutex> lock(IN_mtx);
        if (!app_messages.empty())
        {
            msg = app_messages.front() + "\n";
            app_messages.pop();
        }
        else
        {
            msg.clear();
        }
    }
    //std::cout << "std::string MessageHandler::MSGToClient(): app_messages.size(): " << app_messages.size() << "\n";
    //std::cout << "std::string MessageHandler::MSGToClient(): msg: " << msg << "\n";
    return msg;
}

///////////////////////////////////////////////////////////////////////////

void MessageHandler::ClientToMSG(const std::string& response)
{
    std::lock_guard<std::mutex> lock(OUT_mtx);
    validResponse = response;
    //std::cout << "void MessageHandler::ClientToMSG(const std::string& response): " << validResponse << "-waiting to be read from App\n";
}

std::string MessageHandler::MSGToApp()
{
    std::lock_guard<std::mutex> lock(OUT_mtx);
    const std::string messageToApp = validResponse;
    //std::cout << "std::string MessageHandler::MSGToApp(): " << messageToApp << "-was read\n";
    return messageToApp;
}