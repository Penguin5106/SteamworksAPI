#include <iostream>
#include "steam_api.h"
#include "isteamnetworking.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"

#include <string>


extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText)
{
	std::cout << pchDebugText << std::endl;
	if (nSeverity >= 1)
	{
		// place to set a breakpoint for catching API errors
		int x = 3;
		x = x;
	}
}


void GameLoop()
{
	bool running = true;

	while (running)
	{
		std::string x;
		std::cin >> x;

		running = false;
	}
}

// main function is reserved for setup and shutdown procedures
int main()
{
	//checks steam app id to see if the app is valid and the steam client is running

	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
	{
		std::cout << "App ID Invalid" << std::endl;

		return EXIT_FAILURE;
	}
	
	SteamErrMsg errMsg = { 0 };
	if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
	{
		std::cout << "SteamInit failed\n";
		std::cout << errMsg << "\n";
		std::cout << "steam must be running to play this game" << std::endl;

		return EXIT_FAILURE;
	}

	SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);



	// ------------------------------------ setup complete ----------------------------------------------- //

	GameLoop();

	SteamAPI_Shutdown();
	
	return 0;
}
