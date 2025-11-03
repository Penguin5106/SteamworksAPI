#include "PlayerHostedGameServer.h"
#include <iostream>

PlayerHostedGameServer::PlayerHostedGameServer()
{
	EServerMode serverMode = eServerModeAuthenticationAndSecure;
	uint32 unIP = 0x00000000;
	const char* version = "2000000000009";

	SteamErrMsg* errMsg = { 0 };

	if (SteamGameServer_InitEx(unIP, 27015, 27016, serverMode, version, errMsg) != k_ESteamAPIInitResult_OK)
	{
		std::cout << "Game Server failed to initialise - " << errMsg << std::endl;
	}

	if (SteamGameServer())
	{
		// i dont expect anyone to mod my game so this will stay an empty string
		SteamGameServer()->SetModDir("2000000000009");

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

PlayerHostedGameServer::~PlayerHostedGameServer()
{
	// should send shutdown message to all clients here

	SteamGameServerNetworkingSockets()->CloseListenSocket(ListenSocket);

	SteamGameServer()->LogOff();

	SteamGameServer_Shutdown();
}

void PlayerHostedGameServer::sendUpdatedServerDetailsToSteam()
{
	SteamGameServer()->SetMaxPlayerCount(2);
	SteamGameServer()->SetPasswordProtected(false);
	SteamGameServer()->SetServerName("APITest");
	SteamGameServer()->SetMapName("map");
}