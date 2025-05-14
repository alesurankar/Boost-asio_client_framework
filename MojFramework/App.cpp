#include "MainWindow.h"
#include "App.h"
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


void App::Go()
{
    //float dt = ftOUT.Mark();
    //float dtMs = dt * 1000.0f;
    //std::cout << "Frame Time: " << dtMs << " ms" << std::endl;

    gfx.BeginFrame();
    DisplayOutput();
    nextFrame.store(true, std::memory_order_release);
    gfx.EndFrame();
}


void App::InputLoop()
{

    while (running)
    {
        //float dt = ftIN.Mark();
        //float dtMs = dt * 1000.0f;
        //std::cout << "Input Frame Time: " << dtMs << " ms" << std::endl;

        if (nextFrame.load(std::memory_order_acquire))
        {
            PlayerInput();
            nextFrame.store(false, std::memory_order_release);
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
    if (msgHandler->GetFirstMessage())
    {
        input = "FIRST_MESSAGE";
        msgHandler->FirstMessageSend();
    }
    if (!input.empty())
    {
        msgHandler->AppToMSG(input);
    }
}

void App::UpdateEnemy()
{
    if (xEnemy < 100)
    {
        moveLeft = false;
    }
    if (xEnemy > 700)
    {
        moveLeft = true;
    }
    if (yEnemy < 100)
    {
        moveUP = false;
    }
    if (yEnemy > 500)
    {
        moveUP = true;
    }
    if (moveUP)
    {
        yEnemy--;
    }
    else if (!moveUP)
    {
        yEnemy++;
    }
    if (moveLeft)
    {
        xEnemy--;
    }
    else if (!moveLeft)
    {
        xEnemy++;
    }
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




void App::DisplayOutput()
{
    UpdateEnemy();
    UpdateCharacter();
}