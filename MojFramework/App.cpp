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
    input.clear();
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
    if (pos == std::string::npos)
    {
        outX = 0;
        outY = 0;
        return;
    }

    pos += prefix_len;
    const char* data = message.c_str() + pos;

    // Parse x
    outX = 0;
    while (*data >= '0' && *data <= '9')
    {
        outX = outX * 10 + (*data - '0');
        ++data;
    }

    if (*data != ',')
    {
        outX = 0;
        outY = 0;
        return;
    }

    ++data;

    // Parse y
    outY = 0;
    while (*data >= '0' && *data <= '9')
    {
        outY = outY * 10 + (*data - '0');
        ++data;
    }
}

void App::UnpackMessage()
{
    const std::string& response = msgHandler->MSGToApp();
    //std::cout << "void App::UnpackMessage(): " << response << "\n";
    if (!response.empty())
    {
        size_t newlinePos = response.find('|');
        //std::cout << "void App::UnpackMessage(): " << response << "\n";

        if (newlinePos != std::string::npos)
        {
            std::string playerPart = response.substr(0, newlinePos);
            std::string enemyPart = response.substr(newlinePos + 1);

            ParsePosition(playerPart, Constants::player_prefix, x, y);
            ParsePosition(enemyPart, Constants::enemy_prefix, xEnemy, yEnemy);
        }

        //std::cout << "void App::UnpackMessage(): x = " << x << "\n";
        //std::cout << "void App::UnpackMessage(): y = " << y << "\n";
        //std::cout << "void App::UnpackMessage(): xEnemy = " << xEnemy << "\n";
        //std::cout << "void App::UnpackMessage(): yEnemy = " << yEnemy << "\n";
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