#pragma once

#include "steam_api.h"
#include "isteamnetworking.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"
#include "steamnetworkingtypes.h"
#include <steam_gameserver.h>

class PlayerHostedGameServer
{
public:

	PlayerHostedGameServer();
	~PlayerHostedGameServer();

	void sendUpdatedServerDetailsToSteam();

private:

	// Tells us when we have successfully connected to Steam
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnSteamServersConnected, SteamServersConnected_t);

	// Tells us when there was a failure to connect to Steam
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnSteamServersConnectFailure, SteamServerConnectFailure_t);

	// Tells us when we have been logged out of Steam
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnSteamServersDisconnected, SteamServersDisconnected_t);

	// Tells us that Steam has set our security policy (VAC on or off)
	STEAM_GAMESERVER_CALLBACK(PlayerHostedGameServer, OnPolicyResponse, GSPolicyResponse_t);

	HSteamListenSocket ListenSocket;
};

