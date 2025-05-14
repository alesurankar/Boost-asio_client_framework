/****************************************************************************************************** 
This Framework is Cloned and modified from PlanetChili - chili_framework <http://www.planetchili.net> 
*******************************************************************************************************/	
#include "MainWindow.h"
#include "App.h"
#include "MyException.h"

#include "TCP_Client.h"
#include <fstream>
#include <thread>
#include <atomic>
#include <chrono>


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	MainWindow wnd(hInst, pArgs);
	std::atomic<bool> running{ true };
	std::shared_ptr<MessageHandler> msgHandler = std::make_shared<MessageHandler>();

	App theApp(wnd, running, msgHandler);

	boost::asio::io_context io;
	auto client = std::make_shared<ChatClient>(io, "127.0.0.1", 1234, "username", msgHandler);
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