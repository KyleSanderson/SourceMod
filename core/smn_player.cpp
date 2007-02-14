/**
 * ===============================================================
 * SourceMod (C)2004-2007 AlliedModders LLC.  All rights reserved.
 * ===============================================================
 *
 * This file is not open source and may not be copied without explicit
 * written permission of AlliedModders LLC.  This file may not be redistributed 
 * in whole or significant part.
 * For information, see LICENSE.txt or http://www.sourcemod.net/license.php
 *
 * Version: $Id$
 */

#include "CPlayerManager.h"
#include "AdminCache.h"
#include "sm_stringutil.h"

static cell_t sm_GetClientCount(IPluginContext *pCtx, const cell_t *params)
{
	if (params[1])
	{
		return g_Players.NumPlayers();
	}

	int maxplayers = g_Players.MaxClients();
	int count = 0;
	for (int i=1; i<=maxplayers; ++i)
	{
		CPlayer *pPlayer = g_Players.GetPlayerByIndex(i);
		if ((pPlayer->IsConnected()) && !(pPlayer->IsInGame()))
		{
			count++;
		}
	}

	return (g_Players.NumPlayers() + count);
}

static cell_t sm_GetMaxClients(IPluginContext *pCtx, const cell_t *params)
{
	return g_Players.MaxClients();
}

static cell_t sm_GetClientName(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(index);
	if (!pPlayer->IsConnected())
	{
		return pCtx->ThrowNativeError("Client %d is not connected", index);
	}

	pCtx->StringToLocalUTF8(params[2], static_cast<size_t>(params[3]), pPlayer->GetName(), NULL);
	return 1;
}

static cell_t sm_GetClientIP(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(index);
	if (!pPlayer->IsConnected())
	{
		return pCtx->ThrowNativeError("Client %d is not connected", index);
	}

	char buf[64], *ptr;
	strcpy(buf, pPlayer->GetIPAddress());

	if (params[4] && (ptr = strchr(buf, ':')))
	{
		*ptr = '\0';
	}

	pCtx->StringToLocal(params[2], static_cast<size_t>(params[3]), buf);
	return 1;
}

static cell_t sm_GetClientAuthStr(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(index);
	if (!pPlayer->IsConnected())
	{
		return pCtx->ThrowNativeError("Client %d is not connected", index);
	}

	if (!pPlayer->IsAuthorized())
	{
		return 0;
	}

	pCtx->StringToLocal(params[2], static_cast<size_t>(params[3]), pPlayer->GetAuthString());

	return 1;
}

static cell_t sm_IsPlayerConnected(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	return (g_Players.GetPlayerByIndex(index)->IsConnected()) ? 1 : 0;
}

static cell_t sm_IsPlayerIngame(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	return (g_Players.GetPlayerByIndex(index)->IsInGame()) ? 1 : 0;
}

static cell_t sm_IsPlayerAuthorized(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	return (g_Players.GetPlayerByIndex(index)->IsAuthorized()) ? 1 : 0;
}

static cell_t sm_IsPlayerFakeClient(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(index);
	if (!pPlayer->IsConnected())
	{
		return pCtx->ThrowNativeError("Client %d is not connected", index);
	}

	return (pPlayer->IsFakeClient()) ? 1 : 0;
}

static cell_t sm_PrintToServer(IPluginContext *pCtx, const cell_t *params)
{
	char buffer[1024];
	char *fmt;
	int arg = 2;

	pCtx->LocalToString(params[1], &fmt);
	size_t res = atcprintf(buffer, sizeof(buffer)-2, fmt, pCtx, params, &arg);

	buffer[res++] = '\n';
	buffer[res] = '\0';

	META_CONPRINT(buffer);

	return 1;
}

static cell_t sm_GetClientInfo(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	const char *val = engine->GetClientConVarValue(client, key);
	if (!val)
	{
		return false;
	}

	pContext->StringToLocalUTF8(params[3], params[4], val, NULL);
	return 1;
}

static cell_t sm_PrintToConsole(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(index);
	if (!pPlayer->IsInGame())
	{
		return pCtx->ThrowNativeError("Client %d is not in game", index);
	}

	char buffer[1024];
	char *fmt;
	int arg = 3;

	pCtx->LocalToString(params[2], &fmt);
	size_t res = atcprintf(buffer, sizeof(buffer)-2, fmt, pCtx, params, &arg);

	buffer[res++] = '\n';
	buffer[res] = '\0';

	engine->ClientPrintf(pPlayer->GetEdict(), buffer);

	return 1;
}

static cell_t SetUserAdmin(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}
	if (!g_Admins.IsValidAdmin(params[2]))
	{
		return pContext->ThrowNativeError("AdminId %x is not valid", params[2]);
	}

	pPlayer->SetAdminId(params[2], params[3] ? true : false);

	return 1;
}

static cell_t GetUserAdmin(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}

	return pPlayer->GetAdminId();
}

static cell_t AddUserFlags(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}

	AdminId id;
	if ((id=pPlayer->GetAdminId()) == INVALID_ADMIN_ID)
	{
		id = g_Admins.CreateAdmin(NULL);
		pPlayer->SetAdminId(id, true);
	}

	cell_t *addr;
	for (int i=2; i<=params[0]; i++)
	{
		pContext->LocalToPhysAddr(params[i], &addr);
		g_Admins.SetAdminFlag(id, (AdminFlag)*addr, true);
	}

	return 1;
}

static cell_t RemoveUserFlags(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}
	
	AdminId id;
	if ((id=pPlayer->GetAdminId()) == INVALID_ADMIN_ID)
	{
		return 0;
	}

	cell_t *addr;
	for (int i=2; i<=params[0]; i++)
	{
		pContext->LocalToPhysAddr(params[i], &addr);
		g_Admins.SetAdminFlag(id, (AdminFlag)*addr, false);
	}

	return 1;
}

static cell_t SetUserFlagBits(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}

	AdminId id;
	if ((id=pPlayer->GetAdminId()) == INVALID_ADMIN_ID)
	{
		id = g_Admins.CreateAdmin(NULL);
		pPlayer->SetAdminId(id, true);
	}

	g_Admins.SetAdminFlags(id, Access_Effective, params[2]);

	return 1;
}

static cell_t GetUserFlagBits(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}

	AdminId id;
	if ((id=pPlayer->GetAdminId()) == INVALID_ADMIN_ID)
	{
		return 0;
	}

	return g_Admins.GetAdminFlags(id, Access_Effective);
}

static cell_t GetClientUserId(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Invalid client index %d", client);
	}
	if (!pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client %d is not connected", client);
	}

	return engine->GetPlayerUserId(pPlayer->GetEdict());
}

REGISTER_NATIVES(playernatives)
{
	{"GetMaxClients",			sm_GetMaxClients},
	{"GetClientCount",			sm_GetClientCount},
	{"GetClientName",			sm_GetClientName},
	{"GetClientIP",				sm_GetClientIP},
	{"GetClientAuthString",		sm_GetClientAuthStr},
	{"IsClientConnected",		sm_IsPlayerConnected},
	{"IsPlayerInGame",			sm_IsPlayerIngame},
	{"IsClientAuthorized",		sm_IsPlayerAuthorized},
	{"IsFakeClient",			sm_IsPlayerFakeClient},
	{"PrintToServer",			sm_PrintToServer},
	{"PrintToConsole",			sm_PrintToConsole},
	{"GetClientInfo",			sm_GetClientInfo},
	{"SetUserAdmin",			SetUserAdmin},
	{"GetUserAdmin",			GetUserAdmin},
	{"AddUserFlags",			AddUserFlags},
	{"RemoveUserFlags",			RemoveUserFlags},
	{"SetUserFlagBits",			SetUserFlagBits},
	{"GetUserFlagBits",			GetUserFlagBits},
	{"GetClientUserId",			GetClientUserId},
	{NULL,						NULL}
};

