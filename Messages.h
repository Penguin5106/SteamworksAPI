#pragma once


#define LittleDWord( val )	( val )
#define LittleQWord( val )	( val )

#include "stdafx.h"
#include <steamtypes.h>

enum EMessage
{
	// Server messages
	k_EMsgServerBegin = 0,
	k_EMsgServerSendInfo = k_EMsgServerBegin + 1,
	k_EMsgServerFailAuthentication = k_EMsgServerBegin + 2,
	k_EMsgServerPassAuthentication = k_EMsgServerBegin + 3,
	k_EMsgServerUpdateWorld = k_EMsgServerBegin + 4,
	k_EMsgServerExiting = k_EMsgServerBegin + 5,
	k_EMsgServerPingResponse = k_EMsgServerBegin + 6,

	// Client messages
	k_EMsgClientBegin = 500,
	k_EMsgClientBeginAuthentication = k_EMsgClientBegin + 2,
	k_EMsgClientSendLocalUpdate = k_EMsgClientBegin + 3,

	// P2P authentication messages
	k_EMsgP2PBegin = 600,
	k_EMsgP2PSendingTicket = k_EMsgP2PBegin + 1,

	// force 32-bit size enum so the wire protocol doesn't get outgrown later
	k_EForceDWORD = 0x7fffffff,
};

enum EDisconnectReason
{
	k_EDRClientDisconnect = k_ESteamNetConnectionEnd_App_Min + 1,
	k_EDRServerClosed = k_ESteamNetConnectionEnd_App_Min + 2,
	k_EDRServerReject = k_ESteamNetConnectionEnd_App_Min + 3,
	k_EDRServerFull = k_ESteamNetConnectionEnd_App_Min + 4,
	k_EDRClientKicked = k_ESteamNetConnectionEnd_App_Min + 5
};

// Msg from the server to the client which is sent right after communications are established
// and tells the client what SteamID the game server is using as well as whether the server is secure
struct MsgServerSendInfo_t
{
	MsgServerSendInfo_t() : m_dwMessageType(LittleDWord(k_EMsgServerSendInfo)) {}
	uint32 GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetSteamIDServer(uint64 SteamID) { m_ulSteamIDServer = LittleQWord(SteamID); }
	uint64 GetSteamIDServer() { return LittleQWord(m_ulSteamIDServer); }

	void SetSecure(bool bSecure) { m_bIsVACSecure = bSecure; }
	bool GetSecure() { return m_bIsVACSecure; }

	void SetServerName(const char* pchName) { strncpy_safe(m_rgchServerName, pchName, sizeof(m_rgchServerName)); }
	const char* GetServerName() { return m_rgchServerName; }

private:
	const uint32 m_dwMessageType;
	uint64 m_ulSteamIDServer;
	bool m_bIsVACSecure;
	char m_rgchServerName[128];
};

struct MsgClientBeginAuthentication_t
{
	MsgClientBeginAuthentication_t() : m_dwMessageType(LittleDWord(k_EMsgClientBeginAuthentication)) {}
	uint32 GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetSteamID(uint64 ulSteamID) { m_ulSteamID = LittleQWord(ulSteamID); }
	uint64 GetSteamID() { return LittleQWord(m_ulSteamID); }

private:
	const uint32 m_dwMessageType;

	uint64 m_ulSteamID;
};