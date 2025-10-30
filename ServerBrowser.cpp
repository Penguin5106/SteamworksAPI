#include "ServerBrowser.h"

GameServer::GameServer(gameserveritem_t* GameServerItem)
{
	IPAddress = GameServerItem->m_NetAdr.GetIP();
	ConnectionPort = GameServerItem->m_NetAdr.GetConnectionPort();
	Ping = GameServerItem->m_nPing;
	NumPlayers = GameServerItem->m_nPlayers;
	MaxPlayers = GameServerItem->m_nMaxPlayers;
	ServerVersion = GameServerItem->m_nServerVersion;
	strncpy_safe(ServerName, GameServerItem->GetName(), 64);
	SteamID = GameServerItem->m_steamID;
}


void ServerBrowser::RefreshInternetServers()
{
	// ignores new requests if previous one is still in progress
	if (requestInProgress)
	{
		return;
	}

	// clears data for previous request
	if (currentServerListRequest)
	{
		SteamMatchmakingServers()->ReleaseRequest(currentServerListRequest);

		currentServerListRequest = NULL;
	}

	requestInProgress = true;

	MatchMakingKeyValuePair_t pFilters[1];
	MatchMakingKeyValuePair_t* pFilter = pFilters;

	strncpy_safe(pFilters[0].m_szKey, "secure", sizeof(pFilters[0].m_szKey));
	strncpy_safe(pFilters[0].m_szValue, "1", sizeof(pFilters[0].m_szValue));

	currentServerListRequest = SteamMatchmakingServers()->RequestInternetServerList(SteamUtils()->GetAppID(), &pFilter, 1, this);
}

void ServerBrowser::RefreshLanServers()
{
	// ignores new requests if previous one is still in progress
	if (requestInProgress)
	{
		return;
	}

	// clears data for previous request
	if (currentServerListRequest)
	{
		SteamMatchmakingServers()->ReleaseRequest(currentServerListRequest);

		currentServerListRequest = NULL;
	}

	requestInProgress = true;

	currentServerListRequest = SteamMatchmakingServers()->RequestLANServerList(SteamUtils()->GetAppID(), this);
}

void ServerBrowser::ServerResponded(HServerListRequest hReq, int iServer)
{
	gameserveritem_t* pServer = SteamMatchmakingServers()->GetServerDetails(hReq, iServer);
	if (pServer)
	{
		// Filter out servers that don't match our appid here (might get these in LAN calls since we can't put more filters on it)
		if (pServer->m_nAppID == SteamUtils()->GetAppID())
		{
			AvailableServers.push_back(GameServer(pServer));
			
		}
	}
}

void ServerBrowser::ServerFailedToRespond(HServerListRequest hReq, int iServer)
{
	return;
}

void ServerBrowser::RefreshComplete(HServerListRequest hReq, EMatchMakingServerResponse response)
{
	requestInProgress = false;
}