#include "MainWindow.h"
#include "App.h"
#include "Constants.h"
#include <iostream>


App::App(MainWindow& wnd, std::atomic<bool>& runFlag, std::shared_ptr<MessageHandler> msgHandler_in)
	:
	wnd(wnd),
	gfx(wnd),
    msgHandler(msgHandler_in),
    running(runFlag),
    nextFrame(true)
{
    InputThread = std::thread(&App::InputLoop, this);
}

App::~App()
{
    if (InputThread.joinable())
    {
        InputThread.join();
    }
}

void App::InputLoop()
{
    while (running)
    {
        if (nextFrame)
        {
            //float dt = ft.Mark();
            //float dtMs = dt * 1000.0f;
            //std::cout << "Input Frame Time: " << dtMs << " ms" << std::endl;

            PlayerInput();
            nextFrame = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

void App::PlayerInput()
{
    std::string input = "";
    if (wnd.kbd.KeyIsPressed('W'))
    {
        input += 'W';
    }
    if (wnd.kbd.KeyIsPressed('S'))
    {
        input += 'S';
    }
    if (wnd.kbd.KeyIsPressed('A'))
    {
        input += 'A';
    }
    if (wnd.kbd.KeyIsPressed('D'))
    {
        input += 'D';
    }
    //if (msgHandler->GetFirstMessage())
    //{
    //    input = "FIRST_MESSAGE";
    //    msgHandler->FirstMessageSend();
    //}
    if (!input.empty())
    {
        msgHandler->AppToMSG(input);
    }
}

///////////////////////////////////////////////////////////////////////////////////

void App::Go()
{
    if (!nextFrame)
    {
        //float dt = ft.Mark();
        //float dtMs = dt * 1000.0f;
        //std::cout << "Frame Time: " << dtMs << " ms" << std::endl;
        
        gfx.BeginFrame();
        UnpackMessage();
        DisplayOutput();
        gfx.EndFrame();
        nextFrame = true;
    }
}

void App::ParsePosition(const std::string& message, const char* prefix, int& outX, int& outY)
{
    size_t prefix_len = std::strlen(prefix);
    size_t pos = message.find(prefix);
    if (pos != std::string::npos)
    {
        pos += prefix_len;

        size_t commaPos = message.find(',', pos);
        if (commaPos != std::string::npos)
        {
            outX = std::stoi(message.substr(pos, commaPos - pos));
            outY = std::stoi(message.substr(commaPos + 1));
        }
        else
        {
            outX = 0;
            outY = 0;
        }
    }
    else
    {
        outX = 0;
        outY = 0;
    }
}



void App::UnpackMessage()
{
    std::string response = msgHandler->MSGToApp();
    if (!response.empty())
    {
        size_t newlinePos = response.find('\n');
        //std::cout << "void App::UnpackMessage(): " << response << "\n";

        //if (!response.empty() && response.back() == '\n') 
        //{
        //    response.pop_back();
        //}

        if (newlinePos != std::string::npos)
        {
            std::string playerPart = response.substr(0, newlinePos);
            std::string enemyPart = response.substr(newlinePos + 1);


            ParsePosition(playerPart, Constants::player_prefix, x, y);
            ParsePosition(enemyPart, Constants::enemy_prefix, xEnemy, yEnemy);
        }
        else
        {
            // fallback if no newline, try parsing only player
            ParsePosition(response, Constants::player_prefix, x, y);
        }
        std::cout << "void App::UnpackMessage(): x = " << x << "\n";
        std::cout << "void App::UnpackMessage(): y = " << y << "\n";
        std::cout << "void App::UnpackMessage(): xEnemy = " << xEnemy << "\n";
        std::cout << "void App::UnpackMessage(): yEnemy = " << yEnemy << "\n";
    }
}

void App::DisplayOutput()
{
    UpdateEnemy();
    UpdateCharacter();
}

void App::UpdateEnemy()
{
    int width = 10;
    int height = 10;
    for (int i = xEnemy; i < xEnemy + width; i++)
    {
        for (int j = yEnemy; j < yEnemy + height; j++)
        {
            gfx.PutPixel(i, j, Colors::Red);
        }
    }   
}

void App::UpdateCharacter()
{
    int width = 20;
    int height = 20;
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            gfx.PutPixel(i, j, Colors::Green);
        }
    }
}