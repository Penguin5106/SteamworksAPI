#pragma once

#include "ServerBrowser.h"
#include "steamclientpublic.h"
#include "MatchmakingPing.h"

class GameClient
{
public:

	ServerBrowser browser;
	MatchmakingPing matchmakingPing;

	uint32 serverAddress;
	uint16 serverPort;
	CSteamID steamLobbyID;

	CSteamID gameServerID;
	HSteamNetConnection serverNetConnection;

	GameClient();

	void findServer();

	void StartServerConnection(uint32 serverAddress, int32 serverPort);
	void ConnectToServer(CSteamID steamID);



};

