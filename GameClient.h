#pragma once

#include "ServerBrowser.h"
#include "steamclientpublic.h"
#include "isteammatchmaking.h"


class GameClient
{
public:

	ServerBrowser browser;

	class MatchmakingPing : public ISteamMatchmakingPingResponse
	{
	private:

		HServerQuery serverQuery;
		GameClient *gameClient;


	public:
		MatchmakingPing()
		{
			serverQuery = HSERVERQUERY_INVALID;
			gameClient = NULL;
		}

		void RetrieveSteamIDFromGameServer(GameClient* gameClient, uint32 serverIP, uint16 serverPort)
		{
			this->gameClient = gameClient;
			serverQuery = SteamMatchmakingServers()->PingServer(serverIP, serverPort, this);
		}

		void CancelPing()
		{
			serverQuery = HSERVERQUERY_INVALID;
		}

		virtual void ServerResponded(gameserveritem_t& server)
		{
			if (serverQuery != HSERVERQUERY_INVALID && server.m_steamID.IsValid())
			{
				gameClient->ConnectToServer(server.m_steamID);
			}

			serverQuery = HSERVERQUERY_INVALID;
		}

		virtual void ServerFailedToRespond()
		{
			serverQuery = HSERVERQUERY_INVALID;
		}
	};
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

