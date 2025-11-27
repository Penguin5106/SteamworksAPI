#pragma once

#include "ServerBrowser.h"
#include "steamclientpublic.h"
#include "isteammatchmaking.h"
#include <iostream>


class GameClient
{
public:

	ServerBrowser browser;

	class MatchmakingPing : public ISteamMatchmakingPingResponse
	{

	public:
		MatchmakingPing()
		{
			serverQuery = HSERVERQUERY_INVALID;
			gameClient = NULL;
		}

		void RetrieveSteamIDFromGameServer(GameClient* gameClient, uint32 serverIP, uint16 serverPort)
		{
			this->gameClient = gameClient;

			IP = serverIP;
			Port = serverPort;

			std::cout << "attemting to ping server at IP: " << IP << " Port: " << Port << std::endl;

			serverQuery = SteamMatchmakingServers()->PingServer(IP, Port, this);
		}

		void CancelPing()
		{
			serverQuery = HSERVERQUERY_INVALID;
		}

		virtual void ServerResponded(gameserveritem_t &server)
		{
			std::cout << "Server has Responded" << std::endl;

			if (serverQuery != HSERVERQUERY_INVALID && server.m_steamID.IsValid())
			{
				gameClient->ConnectToServer(server.m_steamID);
			}

			serverQuery = HSERVERQUERY_INVALID;
		}

		virtual void ServerFailedToRespond()
		{
			std::cout << "server failed to respond" << std::endl;
			serverQuery = HSERVERQUERY_INVALID;

			RetrieveSteamIDFromGameServer(gameClient, IP, Port);
		}

	private:
		uint32 IP;
		uint16 Port;
		HServerQuery serverQuery;
		GameClient* gameClient;

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


private:
	STEAM_CALLBACK(GameClient, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
};

