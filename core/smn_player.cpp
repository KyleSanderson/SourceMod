/**
 * vim: set ts=4 :
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

#include "PlayerManager.h"
#include "AdminCache.h"
#include "sm_stringutil.h"
#include "HalfLife2.h"
#include "sourcemod.h"
#include <inetchannelinfo.h>

ConVar sm_show_activity("sm_show_activity", "13", FCVAR_SPONLY|FCVAR_PROTECTED, "Activity display setting (see sourcemod.cfg)");

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

static cell_t sm_IsClientConnected(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	return (g_Players.GetPlayerByIndex(index)->IsConnected()) ? 1 : 0;
}

static cell_t sm_IsClientInGame(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	return (g_Players.GetPlayerByIndex(index)->IsInGame()) ? 1 : 0;
}

static cell_t sm_IsClientAuthorized(IPluginContext *pCtx, const cell_t *params)
{
	int index = params[1];
	if ((index < 1) || (index > g_Players.GetMaxClients()))
	{
		return pCtx->ThrowNativeError("Invalid client index %d", index);
	}

	return (g_Players.GetPlayerByIndex(index)->IsAuthorized()) ? 1 : 0;
}

static cell_t sm_IsClientFakeClient(IPluginContext *pCtx, const cell_t *params)
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

static cell_t CanUserTarget(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	int target = params[2];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsConnected()) {
		return pContext->ThrowNativeError("Player %d is not connected", client);
	}

	CPlayer *pTarget = g_Players.GetPlayerByIndex(target);
	if (!pTarget)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", target);
	} else if (!pTarget->IsConnected()) {
		return pContext->ThrowNativeError("Player %d is not connected", target);
	}

	return g_Admins.CanAdminTarget(pPlayer->GetAdminId(), pTarget->GetAdminId()) ? 1 : 0;
}

static cell_t GetClientTeam(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	return pInfo->GetTeamIndex();
}

static cell_t GetFragCount(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	return pInfo->GetFragCount();
}

static cell_t GetDeathCount(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	return pInfo->GetDeathCount();
}

static cell_t GetArmorValue(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	return pInfo->GetArmorValue();
}

static cell_t GetAbsOrigin(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	cell_t *pVec;
	pContext->LocalToPhysAddr(params[2], &pVec);

	Vector vec = pInfo->GetAbsOrigin();
	pVec[0] = sp_ftoc(vec.x);
	pVec[1] = sp_ftoc(vec.y);
	pVec[2] = sp_ftoc(vec.z);

	return 1;
}

static cell_t GetAbsAngles(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	cell_t *pAng;
	pContext->LocalToPhysAddr(params[2], &pAng);

	QAngle ang = pInfo->GetAbsAngles();
	pAng[0] = sp_ftoc(ang.x);
	pAng[1] = sp_ftoc(ang.y);
	pAng[2] = sp_ftoc(ang.z);

	return 1;
}

static cell_t GetPlayerMins(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	cell_t *pVec;
	pContext->LocalToPhysAddr(params[2], &pVec);

	Vector vec = pInfo->GetPlayerMins();
	pVec[0] = sp_ftoc(vec.x);
	pVec[1] = sp_ftoc(vec.y);
	pVec[2] = sp_ftoc(vec.z);

	return 1;
}

static cell_t GetPlayerMaxs(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	cell_t *pVec;
	pContext->LocalToPhysAddr(params[2], &pVec);

	Vector vec = pInfo->GetPlayerMaxs();
	pVec[0] = sp_ftoc(vec.x);
	pVec[1] = sp_ftoc(vec.y);
	pVec[2] = sp_ftoc(vec.z);

	return 1;
}

static cell_t GetWeaponName(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	const char *weapon = pInfo->GetWeaponName();
	pContext->StringToLocalUTF8(params[2], static_cast<size_t>(params[3]), weapon ? weapon : "", NULL);

	return 1;
}

static cell_t GetModelName(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	const char *model = pInfo->GetModelName();
	pContext->StringToLocalUTF8(params[2], static_cast<size_t>(params[3]), model ? model : "", NULL);

	return 1;
}

static cell_t GetHealth(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	}

	IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
	if (!pInfo)
	{
		return pContext->ThrowNativeError("IPlayerInfo not supported by game");
	}

	return pInfo->GetHealth();
}

static cell_t GetTimeConnected(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetTimeConnected());
}

static cell_t GetDataRate(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return pInfo->GetDataRate();
}

static cell_t IsTimingOut(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return pInfo->IsTimingOut() ? 1 : 0;
}

static cell_t GetLatency(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetLatency(params[2]));
}

static cell_t GetAvgLatency(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetAvgLatency(params[2]));
}

static cell_t GetAvgLoss(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetAvgLoss(params[2]));
}

static cell_t GetAvgChoke(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetAvgChoke(params[2]));
}

static cell_t GetAvgData(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetAvgData(params[2]));
}

static cell_t GetAvgPackets(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];

	CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
	if (!pPlayer)
	{
		return pContext->ThrowNativeError("Player %d is not a valid client", client);
	} else if (!pPlayer->IsInGame()) {
		return pContext->ThrowNativeError("Player %d is not in game", client);
	} else if (pPlayer->IsFakeClient()) {
		return pContext->ThrowNativeError("Player %d is a bot", client);
	}

	INetChannelInfo *pInfo = engine->GetPlayerNetInfo(client);

	return sp_ftoc(pInfo->GetAvgPackets(params[2]));
}

static cell_t GetClientOfUserId(IPluginContext *pContext, const cell_t *params)
{
	return g_Players.GetClientOfUserId(params[1]);
}

static cell_t ShowActivity(IPluginContext *pContext, const cell_t *params)
{
	int value = sm_show_activity.GetInt();

	if (!value)
	{
		return 0;
	}

	int client = params[1];

	const char *name = "Console";
	const char *sign = "ADMIN";
	if (client != 0)
	{
		CPlayer *pPlayer = g_Players.GetPlayerByIndex(client);
		if (!pPlayer || !pPlayer->IsConnected())
		{
			return pContext->ThrowNativeError("Client index %d is invalid", client);
		}
		name = pPlayer->GetName();
		AdminId id = pPlayer->GetAdminId();
		if (id == INVALID_ADMIN_ID
			|| !g_Admins.GetAdminFlag(id, Admin_Generic, Access_Effective))
		{
			sign = "PLAYER";
		}
	}

	char message[255];
	char buffer[255];
	int maxClients = g_Players.GetMaxClients();
	for (int i=1; i<=maxClients; i++)
	{
		CPlayer *pPlayer = g_Players.GetPlayerByIndex(i);
		if (!pPlayer->IsInGame() || pPlayer->IsFakeClient())
		{
			continue;
		}
		AdminId id = pPlayer->GetAdminId();
		g_SourceMod.SetGlobalTarget(i);
		if (id == INVALID_ADMIN_ID
			|| !g_Admins.GetAdminFlag(id, Admin_Generic, Access_Effective))
		{
			/* Treat this as a normal user */
			if ((value & 1) || (value & 2))
			{
				const char *newsign = sign;
				if (value & 2)
				{
					newsign = name;
				}
				g_SourceMod.FormatString(buffer, sizeof(buffer), pContext, params, 2);
				UTIL_Format(message, sizeof(message), "[SM] %s: %s", newsign, buffer);
				g_HL2.TextMsg(i, HUD_PRINTTALK, message);
			}
		} else {
			/* Treat this as an admin user */
			bool is_root = g_Admins.GetAdminFlag(id, Admin_Root, Access_Effective);
			if ((value & 4) 
				|| (value & 8)
				|| ((value & 16) && is_root))
			{
				const char *newsign = sign;
				if ((value & 8) || ((value & 16) && is_root))
				{
					newsign = name;
				}
				g_SourceMod.FormatString(buffer, sizeof(buffer), pContext, params, 2);
				UTIL_Format(message, sizeof(message), "[SM] %s: %s", newsign, buffer);
				g_HL2.TextMsg(i, HUD_PRINTTALK, message);
			}
		}
	}

	return 1;
}

REGISTER_NATIVES(playernatives)
{
	{"AddUserFlags",			AddUserFlags},
	{"CanUserTarget",			CanUserTarget},
	{"GetClientAuthString",		sm_GetClientAuthStr},
	{"GetClientCount",			sm_GetClientCount},
	{"GetClientInfo",			sm_GetClientInfo},
	{"GetClientIP",				sm_GetClientIP},
	{"GetClientName",			sm_GetClientName},
	{"GetClientTeam",			GetClientTeam},
	{"GetClientUserId",			GetClientUserId},
	{"GetMaxClients",			sm_GetMaxClients},
	{"GetUserAdmin",			GetUserAdmin},
	{"GetUserFlagBits",			GetUserFlagBits},
	{"IsClientAuthorized",		sm_IsClientAuthorized},
	{"IsClientConnected",		sm_IsClientConnected},
	{"IsFakeClient",			sm_IsClientFakeClient},
	{"IsPlayerInGame",			sm_IsClientInGame},			/* Backwards compat shim */
	{"IsClientInGame",			sm_IsClientInGame},
	{"RemoveUserFlags",			RemoveUserFlags},
	{"SetUserAdmin",			SetUserAdmin},
	{"SetUserFlagBits",			SetUserFlagBits},
	{"GetClientDeaths",			GetDeathCount},
	{"GetClientFrags",			GetFragCount},
	{"GetClientArmor",			GetArmorValue},
	{"GetClientAbsOrigin",		GetAbsOrigin},
	{"GetClientAbsAngles",		GetAbsAngles},
	{"GetClientMins",			GetPlayerMins},
	{"GetClientMaxs",			GetPlayerMaxs},
	{"GetClientWeapon",			GetWeaponName},
	{"GetClientModel",			GetModelName},
	{"GetClientHealth",			GetHealth},
	{"GetClientTime",			GetTimeConnected},
	{"GetClientDataRate",		GetDataRate},
	{"IsClentTimingOut",		IsTimingOut},
	{"GetClientLatency",		GetLatency},
	{"GetClientAvgLatency",		GetAvgLatency},
	{"GetClientAvgLoss",		GetAvgLoss},
	{"GetClientAvgChoke",		GetAvgChoke},
	{"GetClientAvgData",		GetAvgData},
	{"GetClientAvgPackets",		GetAvgPackets},
	{"GetClientOfUserId",		GetClientOfUserId},
	{"ShowActivity",			ShowActivity},
	{NULL,						NULL}
};
