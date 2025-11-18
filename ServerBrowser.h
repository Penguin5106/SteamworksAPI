#pragma once

#include "steam_api.h"
#include "isteammatchmaking.h"
#include "isteamnetworkingutils.h"
#include "stdafx.h"
#include <string>
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

public:

	uint32 GetIP()
	{
		return IPAddress;
	}

	int32 GetPort()
	{
		return ConnectionPort;
	}

	std::string GetName()
	{
		return ServerName;
	}
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
	bool requestInProgress = false;

private:

	HServerListRequest currentServerListRequest;

};

