#pragma once
#include "MessageHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <string>

class App
{
public:
	App(class MainWindow& wnd, std::atomic<bool>& runFlag, std::shared_ptr<MessageHandler> msgHandler_in);
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App();
	void Go();
private:
	void InputLoop();
	void PlayerInput();
	void DisplayOutput();
	/********************************/
	/*  Moje Funkcije               */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  Moji Parametri              */
	std::shared_ptr<MessageHandler> msgHandler;
	std::thread InputThread;
	std::atomic<bool>& running;
	std::atomic<bool> nextFrame;
	std::atomic<bool> firstMessage;
	/********************************/
};