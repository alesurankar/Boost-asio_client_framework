#pragma once
#include "MessageHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include <string>
#include <thread>    

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
	void UpdateEnemy();
	void UpdateCharacter();
	void DisplayOutput();
	void ParsePosition(const std::string& message, const char* prefix, int& outX, int& outY);
	void UnpackMessage();
	/********************************/
	/*  Moje Funkcije               */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  Moji Parametri              */
	FrameTimer ft;
	std::shared_ptr<MessageHandler> msgHandler;
	std::thread InputThread;
	std::atomic<bool>& running;
	std::atomic<bool> nextFrame;
	int x = 0;
	int y = 0;
	int xEnemy = 0;
	int yEnemy = 0;
	/********************************/
};