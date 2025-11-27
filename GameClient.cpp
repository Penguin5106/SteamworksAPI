#include "GameClient.h"
#include "Messages.h"
#include <iostream>
GameClient::GameClient()
{
	browser = ServerBrowser();
	matchmakingPing = MatchmakingPing();
}

void GameClient::StartServerConnection(uint32 serverAddress, int32 serverPort)
{
	std::cout << "checking server details" << std::endl;

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

	if (!identity.IsInvalid())
		std::cout << "attempting to connect to server" << std::endl;

	serverNetConnection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
}

void GameClient::findServer()
{
	std::string searchMode;
	std::cout << "would you like to search the internet (i) or the local network (l)? ";
	std::cin >> searchMode;

	while (browser.AvailableServers.size() == 0)
	{
		if (searchMode == "i")
			browser.RefreshInternetServers();
		else if (searchMode == "l")
			browser.RefreshLanServers();
		else
			break;

		SteamAPI_RunCallbacks();
	}

	while (browser.requestInProgress)
	{
		SteamAPI_RunCallbacks();
	}
}

void GameClient::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	HSteamNetConnection connection = pCallback->m_hConn;
	SteamNetConnectionInfo_t info = pCallback->m_info;
	ESteamNetworkingConnectionState oldState = pCallback->m_eOldState;

	std::cout << oldState << "\n" << info.m_eState << std::endl;

	if ((oldState == k_ESteamNetworkingConnectionState_Connecting || oldState == k_ESteamNetworkingConnectionState_Connected) && info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
	{
		switch (info.m_eEndReason)
		{
		case k_EDRServerReject:
			std::cout << "recieved server rejection" << std::endl;
			break;
		case k_EDRServerFull:
			std::cout << "server full, failed to connect" << std::endl;
			break;
		}
	}

	else if ((oldState == k_ESteamNetworkingConnectionState_Connecting || oldState == k_ESteamNetworkingConnectionState_Connected) && info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
	{
		std::cout << "problem detected locally, failed to make P2P connection" << std::endl;

		SteamNetworkingSockets()->CloseConnection(connection, info.m_eEndReason, nullptr, false);
	}

	else if (info.m_eState == k_ESteamNetworkingConnectionState_Connected)
	{
		std::cout << "client connected" << std::endl;
	}
}