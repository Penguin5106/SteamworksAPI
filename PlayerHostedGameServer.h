#pragma once

#include "steam_api.h"
#include "isteamnetworking.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"
#include "steamnetworkingtypes.h"
#include <string>
#include <steam_gameserver.h>

#define MAX_PLAYERS_PER_SERVER 2

struct ClientConnectionData
{
	bool isActive;
	CSteamID steamID;
	HSteamNetConnection connectionHandle;

	ClientConnectionData()
	{
		isActive = false;
		connectionHandle = 0;
	}
};

class PlayerHostedGameServer
{
public:

	PlayerHostedGameServer();
	~PlayerHostedGameServer();

	void sendUpdatedServerDetailsToSteam();
	void ReceiveNetworkData();

private:

	// Tells us when we have successfully connected to Steam
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnSteamServersConnected, SteamServersConnected_t);

	// Tells us when there was a failure to connect to Steam
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnSteamServersConnectFailure, SteamServerConnectFailure_t);

	// Tells us when we have been logged out of Steam
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnSteamServersDisconnected, SteamServersDisconnected_t);

	// Tells us that Steam has set our security policy (VAC on or off)
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnPolicyResponse, GSPolicyResponse_t);

	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	HSteamListenSocket ListenSocket;

	// data for all active clients
	ClientConnectionData ClientData[MAX_PLAYERS_PER_SERVER];
	//data for connecting players
	ClientConnectionData PendingClientData[MAX_PLAYERS_PER_SERVER];

	HSteamNetPollGroup pollGroup;

	std::string serverName = "APITest";
};

