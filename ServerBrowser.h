#pragma once

#include "steam_api.h"
#include "isteammatchmaking.h"
#include "isteamnetworkingutils.h"
#include "stdafx.h"
#include <vector>

class GameServer
{
public:
	GameServer(gameserveritem_t* GameServerItem);

private:
	uint32 IPAddress;
	int32 ConnectionPort;
	int Ping;
	int NumPlayers;
	int MaxPlayers;
	int ServerVersion;
	char ServerName[64];
	CSteamID SteamID;
};

class ServerBrowser : public ISteamMatchmakingServerListResponse
{
public:
	
	void RefreshInternetServers();
	void RefreshLanServers();

	// callbacks
	void ServerResponded(HServerListRequest hReq, int iServer);
	void ServerFailedToRespond(HServerListRequest hReq, int iServer);
	void RefreshComplete(HServerListRequest hReq, EMatchMakingServerResponse response);

	std::vector<GameServer> AvailableServers;

private:

	bool requestInProgress = false;
	HServerListRequest currentServerListRequest;

};

