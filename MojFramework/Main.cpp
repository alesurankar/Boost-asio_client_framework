/****************************************************************************************************** 
This Framework is Cloned and modified from PlanetChili - chili_framework <http://www.planetchili.net> 
*******************************************************************************************************/	
#include "MainWindow.h"
#include "App.h"
#include "MyException.h"

#include "TCP_Client.h"
#include <memory>
#include <fstream>
#include <thread>
#include <atomic>
//#include <iostream>


//void CreateConsole()
//{
//	AllocConsole();
//	FILE* fp;
//	freopen_s(&fp, "CONOUT$", "w", stdout);
//	std::cout.clear();
//}


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	//CreateConsole();

	std::ifstream file("C:/Projects/Python_API_setup_for_NinjaStrike/username.txt");
	std::string username;
	if (file.is_open())
	{
		std::getline(file, username);
	}

	MainWindow wnd(hInst, pArgs);
	std::atomic<bool> running{ true };
	std::shared_ptr<MessageHandler> msgHandler = std::make_shared<MessageHandler>();

	App theApp(wnd, running, msgHandler);

	boost::asio::io_context io;
	auto client = std::make_shared<ChatClient>(io, "127.0.0.1", 1234, username, msgHandler);
	boost::asio::post(io, [client]()
		{
			client->Start();
		});
	std::thread networking([&]()
		{
			io.run();
		});


	while (wnd.ProcessMessage() && running)
	{
		theApp.Go();
	}

	running = false;
	io.stop();
	if (networking.joinable())
	{
		networking.join();
	}
	
	return 0;
}