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

#include "sm_globals.h"
#include "sourcemm_api.h"
#include "HandleSys.h"
#include "CConVarManager.h"
#include "CConCmdManager.h"
#include "PluginSys.h"

static cell_t sm_CreateConVar(IPluginContext *pContext, const cell_t *params)
{
	char *name, *defaultVal, *helpText;

	pContext->LocalToString(params[1], &name);

	// While the engine seems to accept a blank convar name, it causes a crash upon server quit
	if (name == NULL || strcmp(name, "") == 0)
	{
		return pContext->ThrowNativeError("Null or blank convar name is not allowed");
	}

	pContext->LocalToString(params[2], &defaultVal);
	pContext->LocalToString(params[3], &helpText);

	bool hasMin = params[5] ? true : false;
	bool hasMax = params[7] ? true : false;
	float min = sp_ctof(params[6]);
	float max = sp_ctof(params[8]);

	return g_ConVarManager.CreateConVar(pContext, name, defaultVal, helpText, params[4], hasMin, min, hasMax, max);
}

static cell_t sm_FindConVar(IPluginContext *pContext, const cell_t *params)
{
	char *name;

	pContext->LocalToString(params[1], &name);

	// While the engine seems to accept a blank convar name, it causes a crash upon server quit
	if (name == NULL || strcmp(name, "") == 0)
	{
		return BAD_HANDLE;
	}

	return g_ConVarManager.FindConVar(name);
}

static cell_t sm_HookConVarChange(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	g_ConVarManager.HookConVarChange(pContext, cvar, static_cast<funcid_t>(params[2]));

	return 1;
}

static cell_t sm_UnhookConVarChange(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	g_ConVarManager.UnhookConVarChange(pContext, cvar, static_cast<funcid_t>(params[2]));

	return 1;
}

static cell_t sm_GetConVarBool(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	return cvar->GetBool();
}

static cell_t sm_GetConVarInt(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	return cvar->GetInt();
}

/* This handles both SetConVarBool() and SetConVarInt() */
static cell_t sm_SetConVarNum(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	cvar->SetValue(params[2]);

	return 1;
}

static cell_t sm_GetConVarFloat(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}
	
	float value = cvar->GetFloat();

	return sp_ftoc(value);
}

static cell_t sm_SetConVarFloat(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	float value = sp_ctof(params[2]);
	cvar->SetValue(value);

	return 1;
}

static cell_t sm_GetConVarString(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	pContext->StringToLocalUTF8(params[2], params[3], cvar->GetString(), NULL);

	return 1;
}

static cell_t sm_SetConVarString(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	char *value;
	pContext->LocalToString(params[2], &value);

	cvar->SetValue(value);

	return 1;
}

static cell_t sm_GetConVarFlags(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	return cvar->GetFlags();
}

static cell_t sm_SetConVarFlags(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	cvar->SetFlags(params[2]);

	return 1;
}

static cell_t sm_GetConVarMin(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	cell_t *addr;
	bool hasMin;
	float min;

	pContext->LocalToPhysAddr(params[2], &addr);

	hasMin = cvar->GetMin(min);
	*addr = sp_ftoc(min);

	return hasMin;
}

static cell_t sm_GetConVarMax(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	cell_t *addr;
	bool hasMax;
	float max;

	pContext->LocalToPhysAddr(params[2], &addr);

	hasMax = cvar->GetMax(max);
	*addr = sp_ftoc(max);

	return hasMax;
}

static cell_t sm_GetConVarName(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	pContext->StringToLocalUTF8(params[2], params[3], cvar->GetName(), NULL);

	return 1;
}

static cell_t sm_ResetConVar(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	ConVar *cvar;

	if ((err=g_HandleSys.ReadHandle(hndl, g_ConVarManager.GetHandleType(), NULL, (void **)&cvar))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid ConVar Handle %x (error %d)", hndl, err);
	}

	cvar->Revert();

	return 1;
}

static cell_t sm_RegServerCmd(IPluginContext *pContext, const cell_t *params)
{
	char *name,*help;
	IPluginFunction *pFunction;

	pContext->LocalToString(params[1], &name);
	pContext->LocalToString(params[3], &help);
	pFunction = pContext->GetFunctionById(params[2]);

	if (!pFunction)
	{
		return pContext->ThrowNativeError("Invalid function id (%X)", params[2]);
	}

	g_ConCmds.AddServerCommand(pFunction, name, help, params[4]);

	return 1;
}

static cell_t sm_RegConsoleCmd(IPluginContext *pContext, const cell_t *params)
{
	char *name,*help;
	IPluginFunction *pFunction;

	pContext->LocalToString(params[1], &name);
	pContext->LocalToString(params[3], &help);
	pFunction = pContext->GetFunctionById(params[2]);

	if (!pFunction)
	{
		return pContext->ThrowNativeError("Invalid function id (%X)", params[2]);
	}

	g_ConCmds.AddConsoleCommand(pFunction, name, help, params[4]);

	return 1;
}

static cell_t sm_RegAdminCmd(IPluginContext *pContext, const cell_t *params)
{
	char *name,*help;
	const char *group;
	IPluginFunction *pFunction;
	FlagBits flags = params[3];
	int cmdflags = params[6];

	pContext->LocalToString(params[1], &name);
	pContext->LocalToString(params[4], &help);
	pContext->LocalToString(params[5], (char **)&group);
	pFunction = pContext->GetFunctionById(params[2]);

	if (group[0] == '\0')
	{
		CPlugin *pPlugin = g_PluginSys.GetPluginByCtx(pContext->GetContext());
		group = pPlugin->GetFilename();
	}

	if (!pFunction)
	{
		return pContext->ThrowNativeError("Invalid function id (%X)", params[2]);
	}

	g_ConCmds.AddAdminCommand(pFunction, name, group, flags, help, cmdflags);

	return 1;
}

REGISTER_NATIVES(convarNatives)
{
	{"CreateConVar",		sm_CreateConVar},
	{"FindConVar",			sm_FindConVar},
	{"HookConVarChange",	sm_HookConVarChange},
	{"UnhookConVarChange",	sm_UnhookConVarChange},
	{"GetConVarBool",		sm_GetConVarBool},
	{"SetConVarBool",		sm_SetConVarNum},
	{"GetConVarInt",		sm_GetConVarInt},
	{"SetConVarInt",		sm_SetConVarNum},
	{"GetConVarFloat",		sm_GetConVarFloat},
	{"SetConVarFloat",		sm_SetConVarFloat},
	{"GetConVarString",		sm_GetConVarString},
	{"SetConVarString",		sm_SetConVarString},
	{"GetConVarFlags",		sm_GetConVarFlags},
	{"SetConVarFlags",		sm_SetConVarFlags},
	{"GetConVarName",		sm_GetConVarName},
	{"GetConVarMin",		sm_GetConVarMin},
	{"GetConVarMax",		sm_GetConVarMax},
	{"ResetConVar",			sm_ResetConVar},
	{"RegServerCmd",		sm_RegServerCmd},
	{"RegConsoleCmd",		sm_RegConsoleCmd},
	{NULL,					NULL}
};
