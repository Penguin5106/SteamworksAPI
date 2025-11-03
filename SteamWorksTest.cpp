#include <iostream>
#include "steam_api.h"
#include "isteamnetworking.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"
#include "steamnetworkingtypes.h"
#include <steam_gameserver.h>
#include "ServerBrowser.h"
#include "PlayerHostedGameServer.h"

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



void findServer(ServerBrowser Browser)
{



}

void GameLoop()
{
	std::string mode;
	std::cout << "server or client? s/c" << std::endl;
	std::cin >> mode;
	if (mode != "s" && mode != "c")
		return;

	if (mode == "s")
	{
		PlayerHostedGameServer PHGameServer = PlayerHostedGameServer();

		while (mode == "s")
		{
			for (int i = 0; i < 9999999; ++i)
			{
				SteamGameServer_RunCallbacks();

				PHGameServer.sendUpdatedServerDetailsToSteam();

				SteamAPI_RunCallbacks();
			}
			std::cout << "enter s to refresh" << std::endl;
			std::cin >> mode;
		}
		
		return;
	}

	
	if (mode == "c")
	{
		while (mode == "c")
		{
			ServerBrowser browser = ServerBrowser();

			for (int i = 0; i < 9999999; ++i)
			{
				browser.RefreshLanServers();

				SteamAPI_RunCallbacks();
			}
			std::cout << "enter c to refresh" << std::endl;
			std::cin >> mode;
		}
		return;
	}
	//client can find internet servers but no lan servers, may be due to VM settings
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
