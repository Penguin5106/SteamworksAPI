#include <iostream>
#include "steam_api.h"
#include "isteamnetworking.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"
#include "steamnetworkingtypes.h"
#include <steam_gameserver.h>
#include "ServerBrowser.h"

#include <string>


HSteamListenSocket ListenSocket;

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

void startServer()
{
	EServerMode serverMode = eServerModeAuthenticationAndSecure;
	uint32 unIP = 0x00000000;
	const char* version = "2000000000009";

	SteamErrMsg *errMsg = { 0 };

	if (SteamGameServer_InitEx(unIP, 27015, 27016, serverMode, version, errMsg) != k_ESteamAPIInitResult_OK)
	{
		std::cout << "Game Server failed to initialise - " << errMsg << std::endl;
	}

	if (SteamGameServer())
	{
		// i dont expect anyone to mod my game so this will stay an empty string
		SteamGameServer()->SetModDir("");

		// sets unique identifying details
		SteamGameServer()->SetProduct("480");
		SteamGameServer()->SetGameDescription("SteamWorks Networking Test");

		// anonymous is used here since i dont need a persistent server account
		SteamGameServer()->LogOnAnonymous();

		// when using authentication this sets the server open to requests
		SteamGameServer()->SetAdvertiseServerActive(true);

	}
	else 
	{
		std::cout << "steam game server invalid" << std::endl;
	}



	ListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
}

void stopServer()
{
	// should send shutdown message to all clients here

	SteamGameServerNetworkingSockets()->CloseListenSocket(ListenSocket);

	SteamGameServer()->LogOff();

	SteamGameServer_Shutdown();
}

void findServer(ServerBrowser Browser)
{



}

void GameLoop()
{
	bool running = true;

	while (running)
	{
		std::string x;
		std::cin >> x;

		running = false;

		SteamAPI_RunCallbacks();
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

	SteamNetworkingUtils()->InitRelayNetworkAccess();

	// ------------------------------------ setup complete ----------------------------------------------- //

	GameLoop();

	SteamAPI_Shutdown();
	
	return 0;
}
