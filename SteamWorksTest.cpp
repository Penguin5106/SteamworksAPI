#include <iostream>
#include "steam_api.h"
#include "isteamnetworking.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"
#include "steamnetworkingtypes.h"
#include <steam_gameserver.h>

#include "GameClient.h"
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

	while (mode == "c")
	{
		GameClient client = GameClient();

		std::string selection = "a";
		while (selection == "a")
		{
			client.findServer();

			std::cout << "enter a to refresh, x to exit,  or server index to join server" << std::endl;
			std::cin >> selection;

			if (selection == "a")
				continue;
			if (selection == "x")
			{
				mode == "x";

				break;
			}

			try { 
				int num = std::stoi(selection);
				
				client.StartServerConnection(client.browser.AvailableServers[num].GetIP(), client.browser.AvailableServers[num].GetPort());

			}
			catch (const std::invalid_argument&) {
				std::cout << "Invalid input: The string is not a valid number." << std::endl;
			}
			catch (const std::out_of_range&) {
				std::cout << "Number out of range." << std::endl;
			}
		}

		
	}
	return;
	
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
