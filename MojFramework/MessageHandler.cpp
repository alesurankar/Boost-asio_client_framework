#include "MessageHandler.h"
#include <iostream>


MessageHandler::MessageHandler()
    :
    firstMessage(true)
{}


void MessageHandler::AppToMSG(const std::string& message)
{
    std::lock_guard<std::mutex> lock(IN_mtx);
    app_messages.push(message);
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
            msg = "";
        }
    }
    return msg;
}

///////////////////////////////////////////////////////////////////////////

void MessageHandler::ClientToMSG(const std::string& response)
{
    std::lock_guard<std::mutex> lock(OUT_mtx);
    validResponse = response;
    //std::cout << "void MessageHandler::ClientToMSG(const std::string& response): " << validResponse << "-waiting to be read from App\n";
    //app_responses.push(response);
}

std::string MessageHandler::MSGToApp()
{
    //{
    //    std::lock_guard<std::mutex> lock(OUT_mtx);
    //    if (!app_responses.empty())
    //    {
    //        response = app_responses.front() + "\n";
    //        app_responses.pop();
    //    }
    //    else
    //    {
    //        response = "";
    //    }
    //}
    //return response;
    std::lock_guard<std::mutex> lock(OUT_mtx);
    std::string messageToApp = validResponse;
    //validResponse = "";
    //validResponse = "";
    //std::cout << "std::string MessageHandler::MSGToApp(): " << messageToApp << "-was read\n";
    return messageToApp;
}


//void MessageHandler::ClientToMSG(int x, int y) //13. MSGClient(middleman)
//{
//    //std::cout << "Step 13, MessageHandler::ClientToMSG: x = " << x << ", y = " << y << "\n";
//    std::lock_guard<std::mutex> lock(pos_mtx);
//    app_position.push(std::make_pair(x, y));
//    //std::cout << "Step13--------------\n";
//}


//std::optional<std::pair<int, int>> MessageHandler::MSGToApp()
//{
//    std::lock_guard<std::mutex> lock(pos_mtx); 
//    if (!app_position.empty())
//    {
//        //std::cout << "Step 14, MessageHandler:MSGToApp:\n";
//        std::pair<int, int> pos = app_position.front();
//        lastPos = pos;
//        app_position.pop();
//        int x = pos.first;
//        int y = pos.second;
//        //std::cout << "Popped position and extracted value: " << "x = " << x << ", y = " << y << "\n";
//        //std::cout << "Step14--------------\n";
//        return pos;
//    }
//    else
//    {
//        //std::cout << "[Output] No data to display.\n";
//        //return std::nullopt;
//        return lastPos;
//    }
//}

bool MessageHandler::GetFirstMessage()
{
    return firstMessage.load();
}

void MessageHandler::FirstMessageSend()
{
    firstMessage.store(false);
}
