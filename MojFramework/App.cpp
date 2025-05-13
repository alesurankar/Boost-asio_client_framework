#include "MainWindow.h"
#include "App.h"

App::App(MainWindow& wnd, std::atomic<bool>& runFlag, std::shared_ptr<MessageHandler> msgHandler_in)
	:
	wnd(wnd),
	gfx(wnd),
    msgHandler(msgHandler_in),
    running(runFlag),
    nextFrame(true),
    firstMessage(true)
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

void App::Go()
{
    if (firstMessage == false)
    {
        gfx.BeginFrame();
        if (!nextFrame.load(std::memory_order_acquire))
        {
            DisplayOutput();
            nextFrame.store(true, std::memory_order_release);
        }
        gfx.EndFrame();
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void App::InputLoop()
{
    while (running)
    {
        if (nextFrame.load(std::memory_order_acquire))
        {
            PlayerInput();
            nextFrame.store(false, std::memory_order_release);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    } 
}



void App::PlayerInput()
{
    if (wnd.kbd.KeyIsPressed('W'))
    {
        msgHandler->AppToMSG("move_up");
    }
    if (wnd.kbd.KeyIsPressed('S'))
    {
        msgHandler->AppToMSG("move_down");
    }
    if (wnd.kbd.KeyIsPressed('A'))
    {
        msgHandler->AppToMSG("move_left");
    }
    if (wnd.kbd.KeyIsPressed('D'))
    {
        msgHandler->AppToMSG("move_right");
    }
    if (firstMessage.load())
    {
        msgHandler->AppToMSG("start_pos");
        firstMessage.store(false);
    }
}

void App::DisplayOutput()
{
    auto optPos = msgHandler->MSGToApp();
    if (optPos)
    {
        std::pair<int, int> pos = *optPos;
        int x = pos.first;
        int y = pos.second;
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
}