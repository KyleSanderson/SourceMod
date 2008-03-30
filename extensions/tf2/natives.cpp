/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Counter-Strike:Source Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "RegNatives.h"

#define REGISTER_NATIVE_ADDR(name, code) \
	void *addr; \
	if (!g_pGameConf->GetMemSig(name, &addr)) \
	{ \
		return pContext->ThrowNativeError("Failed to locate function"); \
	} \
	code; \
	g_RegNatives.Register(pWrapper);

inline CBaseEntity *GetCBaseEntity(int num, bool onlyPlayers)
{
	edict_t *pEdict = engine->PEntityOfEntIndex(num);
	if (!pEdict || pEdict->IsFree())
	{
		return NULL;
	}

	if (num > 0 && num < playerhelpers->GetMaxClients())
	{
		IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(pEdict);
		if (!pPlayer || !pPlayer->IsConnected())
		{
			return NULL;
		}
	}
	else if (onlyPlayers)
	{
		return NULL;
	}

	IServerUnknown *pUnk;
	if ((pUnk=pEdict->GetUnknown()) == NULL)
	{
		return NULL;
	}

	return pUnk->GetBaseEntity();
}


// native TF2_Burn(client)
cell_t TF2_Burn(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTFPlayerShared::Burn(CTFPlayer*)
	if (!pWrapper)
	{
		REGISTER_NATIVE_ADDR("Burn", 
			PassInfo pass[1]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(CBaseEntity *); \
			pass[0].type = PassType_Basic; \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, NULL, pass, 1))
	}

	CBaseEntity *pEntity;
	if (!(pEntity=GetCBaseEntity(params[1], true)))
	{
		return pContext->ThrowNativeError("Client index %d is not valid", params[1]);
	}

	void *obj = (void *)((uint8_t *)pEntity + playerSharedOffset->GetOffset());

	unsigned char vstk[sizeof(void *) + sizeof(CBaseEntity *)];
	unsigned char *vptr = vstk;

	*(void **)vptr = obj;
	vptr += sizeof(void *);
	*(CBaseEntity **)vptr = pEntity;

	pWrapper->Execute(vstk, NULL);

	return 1;
}

// native TF2_Invuln(client, bool:something, bool:anothersomething)
cell_t TF2_Invuln(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	//CTFPlayerShared::SetInvulnerable(bool, bool)
	if (!pWrapper)
	{
		REGISTER_NATIVE_ADDR("Invuln", 
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(bool); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(bool); \
			pass[1].type = PassType_Basic; \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, NULL, pass, 2))
	}

	CBaseEntity *pEntity;
	if (!(pEntity=GetCBaseEntity(params[1], true)))
	{
		return pContext->ThrowNativeError("Client index %d is not valid", params[1]);
	}

	void *obj = (void *)((uint8_t *)pEntity + playerSharedOffset->GetOffset());

	unsigned char vstk[sizeof(void *) + 2*sizeof(bool)];
	unsigned char *vptr = vstk;


	*(void **)vptr = obj;
	vptr += sizeof(bool);
	*(bool *)vptr = !!params[2];
	vptr += sizeof(bool);
	*(bool *)vptr = !!params[3];

	pWrapper->Execute(vstk, NULL);

	return 1;
}


sp_nativeinfo_t g_TFNatives[] = 
{
	{"TF2_Burn",						TF2_Burn},
	{"TF2_Invuln",					TF2_Invuln},
	{NULL,							NULL}
};
