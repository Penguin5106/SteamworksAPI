#include "PlayerHostedGameServer.h"
#include "Messages.h"
#include <iostream>

PlayerHostedGameServer::PlayerHostedGameServer()
{
	EServerMode serverMode = eServerModeAuthenticationAndSecure;
	uint32 unIP = 0x00000000;
	const char* version = "2000000000009";

	SteamErrMsg* errMsg = { 0 };

	if (SteamGameServer_InitEx(unIP, 27015, 27016, serverMode, version, errMsg) != k_ESteamAPIInitResult_OK)
	{
		std::cout << "Game Server failed to initialise - " << errMsg << std::endl;
	}

	if (SteamGameServer())
	{
		// i dont expect anyone to mod my game so this will stay an empty string
		SteamGameServer()->SetModDir("2000000000009");

		// sets unique identifying details
		SteamGameServer()->SetProduct("480");
		SteamGameServer()->SetGameDescription("SteamWorks Networking Test");

		// anonymous is used here since i dont need a persistent server account
		SteamGameServer()->LogOnAnonymous();

		// when using authentication this sets the server open to requests
		SteamGameServer()->SetAdvertiseServerActive(true);

	}
	else
	{
		std::cout << "steam game server invalid" << std::endl;
	}



	ListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);

	pollGroup = SteamNetworkingSockets()->CreatePollGroup();
}

PlayerHostedGameServer::~PlayerHostedGameServer()
{
	// should send shutdown message to all clients here

	SteamGameServerNetworkingSockets()->CloseListenSocket(ListenSocket);

	SteamGameServer()->LogOff();

	SteamGameServer_Shutdown();
}


void PlayerHostedGameServer::OnSteamServersConnected(SteamServersConnected_t* pLogonSuccess)
{
	std:: cout << "SpaceWarServer connected to Steam successfully" << std::endl;

	// Tell Steam about our server details
	sendUpdatedServerDetailsToSteam();
}

void PlayerHostedGameServer::OnPolicyResponse(GSPolicyResponse_t* pPolicyResponse)
{
	std::cout << pPolicyResponse->m_bSecure << std::endl;
}

void PlayerHostedGameServer::OnSteamServersDisconnected(SteamServersDisconnected_t* pLoggedOff)
{
	std::cout << "SpaceWarServer disconnected from Steam" << std::endl;
}

void PlayerHostedGameServer::OnSteamServersConnectFailure(SteamServerConnectFailure_t* pConnectFailure)
{
	std::cout << "SpaceWarServer failed to connect to Steam" << std::endl;
}

void PlayerHostedGameServer::sendUpdatedServerDetailsToSteam()
{
	SteamGameServer()->SetMaxPlayerCount(2);
	SteamGameServer()->SetPasswordProtected(false);
	SteamGameServer()->SetServerName(serverName.c_str());
	SteamGameServer()->SetMapName("map");
}

void PlayerHostedGameServer::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	std::cout << "connection status changed message recieved" << std::endl;

	//connection handle
	HSteamNetConnection connection = pCallback->m_hConn;
	//connection info
	SteamNetConnectionInfo_t info = pCallback->m_info;
	//previous state
	ESteamNetworkingConnectionState oldState = pCallback->m_eOldState;

	if (info.m_hListenSocket && oldState == k_ESteamNetworkingConnectionState_None && info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
	{
		for (int i = 0; i < MAX_PLAYERS_PER_SERVER; i++)
		{
			if (!ClientData[i].isActive && !PendingClientData[i].connectionHandle)
			{
				EResult result = SteamNetworkingSockets()->AcceptConnection(connection);

				if (result != k_EResultOK)
				{
					std::cout << "Accept Connection did not return ok, instead it returned: " << result << std::endl;

					SteamGameServerNetworkingSockets()->CloseConnection(connection, k_ESteamNetConnectionEnd_AppException_Generic, "failed to accept connection", false);
					
					return;
				}

				PendingClientData[i].connectionHandle = connection;

				SteamGameServerNetworkingSockets()->SetConnectionPollGroup(connection, pollGroup);

				MsgServerSendInfo_t msg;
				msg.SetSteamIDServer(SteamGameServer()->GetSteamID().ConvertToUint64());
				msg.SetServerName(serverName.c_str());

				SteamGameServerNetworkingSockets()->SendMessageToConnection(connection, &msg, sizeof(MsgServerSendInfo_t), k_nSteamNetworkingSend_Reliable, nullptr);
				
				std::cout << "accepting connection" << std::endl;

				return;
			}
		}

		std::cout << "rejecting connection, server is full" << std::endl;

		SteamGameServerNetworkingSockets()->CloseConnection(connection, k_ESteamNetConnectionEnd_AppException_Generic, "Server Full", false);
	}

}

void PlayerHostedGameServer::ReceiveNetworkData()
{
	SteamNetworkingMessage_t* messages[64];
	int numMessages = SteamGameServerNetworkingSockets()->ReceiveMessagesOnPollGroup(pollGroup, messages, 64);

	for (int messageIndex = 0; messageIndex < numMessages; messageIndex++)
	{
		std::cout << "processing message" << std::endl;

		SteamNetworkingMessage_t* mssge = messages[messageIndex];
		CSteamID steamIDRemote = mssge->m_identityPeer.GetSteamID();
		HSteamNetConnection connection = mssge->m_conn;

		if (mssge->GetSize() < sizeof(uint32))
		{
			std::cout << "message too short, skipping message" << std::endl;
			mssge->Release();
			mssge = nullptr;
			continue;
		}

		EMessage eMssge = (EMessage)LittleDWord(*(uint32*)mssge->GetData());

		switch (eMssge)
		{
		case k_EMsgClientBeginAuthentication:
		{
			if (mssge->GetSize() != sizeof(MsgClientBeginAuthentication_t))
			{

				std::cout << "mesage size does not match specified type, skipping message" << std::endl;

				mssge->Release();
				mssge = nullptr;
				continue;
			}

			std::cout << "begin auth message recieved" << std::endl;
		}
		
		case k_EMsgP2PBegin:
		{
			std::cout << "P2P Begin message recieved" << std::endl;
		}

		case k_EMsgP2PSendingTicket:
		{
			std::cout << "P2P Sending Ticket recieved" << std::endl;
		}
		}
	}
}