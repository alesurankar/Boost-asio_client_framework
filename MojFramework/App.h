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
	//void UpdateEnemy();
	void UpdateCharacter();
	void DisplayOutput();
	void UnpackMessage();
	/********************************/
	/*  Moje Funkcije               */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  Moji Parametri              */
	FrameTimer ftIN;
	FrameTimer ftOUT;
	std::shared_ptr<MessageHandler> msgHandler;
	std::thread InputThread;
	std::atomic<bool>& running;
	std::atomic<bool> nextFrame;
	int x = 0;
	int y = 0;
	int xEnemy = 200;
	int yEnemy = 200;
	bool moveUP = true;
	bool moveLeft = true;
	/********************************/
};