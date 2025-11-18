#include "GameClient.h"

GameClient::GameClient()
{
	browser = ServerBrowser();
	matchmakingPing = MatchmakingPing();
}

void GameClient::StartServerConnection(uint32 serverAddress, int32 serverPort)
{

	if (steamLobbyID.IsValid())
	{
		SteamMatchmaking()->LeaveLobby(steamLobbyID);
	}

	this->serverAddress = serverAddress;
	this->serverPort = (uint16)serverPort;

	matchmakingPing.RetrieveSteamIDFromGameServer(this, serverAddress, serverPort);
}

void GameClient::ConnectToServer(CSteamID steamID)
{
	if (steamLobbyID.IsValid())
	{
		SteamMatchmaking()->LeaveLobby(steamLobbyID);
	}

	gameServerID = steamID;

	SteamNetworkingIdentity identity;
	identity.SetSteamID(steamID);

	serverNetConnection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
}

void GameClient::findServer()
{

	while (browser.AvailableServers.size() == 0)
	{
		browser.RefreshInternetServers();

		SteamAPI_RunCallbacks();
	}

	while (browser.requestInProgress)
	{
		SteamAPI_RunCallbacks();
	}
}