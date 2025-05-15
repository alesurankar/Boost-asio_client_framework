#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <optional>
#include <utility>

class MessageHandler
{
public:
    MessageHandler();
    void AppToMSG(const std::string& message);
    std::string MSGToClient();
    void ClientToMSG(const std::string& response); 
    std::string MSGToApp();
    //void ClientToMSG(int x, int y);
    //std::optional<std::pair<int, int>> MSGToApp();
    bool GetFirstMessage();
    void FirstMessageSend();
private:
    std::mutex IN_mtx;
    std::mutex OUT_mtx;
    std::string msg = "";
    std::string response = "";
    std::string validResponse;
    std::queue<std::string> app_messages;
    //std::queue<std::string> app_responses;
    //std::queue<std::pair<int, int>> app_position;
    std::optional<std::pair<int, int>> lastPos = std::nullopt;
    std::atomic<bool> firstMessage;
};