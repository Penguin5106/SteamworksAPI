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

	HSteamListenSocket ListenSocket;
};

