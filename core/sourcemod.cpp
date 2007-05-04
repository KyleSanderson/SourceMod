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

#include <stdio.h>
#include "sourcemod.h"
#include "sourcemm_api.h"
#include "systems/LibrarySys.h"
#include "vm/sp_vm_engine.h"
#include <sh_string.h>
#include "PluginSys.h"
#include "ShareSys.h"
#include "CoreConfig.h"
#include "Logger.h"
#include "ExtensionSys.h"
#include "AdminCache.h"
#include "sm_stringutil.h"
#include "PlayerManager.h"
#include "Translator.h"
#include "ForwardSys.h"
#include "TimerSys.h"

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, false, bool, const char *, const char *, const char *, const char *, bool, bool);
SH_DECL_HOOK0_void(IServerGameDLL, LevelShutdown, SH_NOATTRIB, false);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, false, bool);

SourcePawnEngine g_SourcePawn;
SourceModBase g_SourceMod;

ILibrary *g_pJIT = NULL;
SourceHook::String g_BaseDir;
ISourcePawnEngine *g_pSourcePawn = &g_SourcePawn;
IVirtualMachine *g_pVM;
IdentityToken_t *g_pCoreIdent = NULL;
float g_LastTime = 0.0f;
float g_LastAuthCheck = 0.0f;
IForward *g_pOnGameFrame = NULL;
IForward *g_pOnMapEnd = NULL;
bool g_Loaded = false;
int g_StillFrames = 0;
float g_StillTime = 0.0f;

typedef int (*GIVEENGINEPOINTER)(ISourcePawnEngine *);
typedef unsigned int (*GETEXPORTCOUNT)();
typedef IVirtualMachine *(*GETEXPORT)(unsigned int);
typedef void (*NOTIFYSHUTDOWN)();

void ShutdownJIT()
{
	NOTIFYSHUTDOWN notify = (NOTIFYSHUTDOWN)g_pJIT->GetSymbolAddress("NotifyShutdown");
	if (notify)
	{
		notify();
	}

	g_pJIT->CloseLibrary();
}

SourceModBase::SourceModBase()
{
	m_IsMapLoading = false;
	m_ExecPluginReload = false;
	m_GotBasePath = false;
}

ConfigResult SourceModBase::OnSourceModConfigChanged(const char *key, 
									  const char *value, 
									  ConfigSource source, 
									  char *error, 
									  size_t maxlength)
{
	if (strcasecmp(key, "BasePath") == 0)
	{
		if (source == ConfigSource_Console)
		{
			UTIL_Format(error, maxlength, "Cannot be set at runtime");
			return ConfigResult_Reject;
		}

		if (!m_GotBasePath)
		{
			g_LibSys.PathFormat(m_SMBaseDir, sizeof(m_SMBaseDir), "%s/%s", g_BaseDir.c_str(), value);
			g_LibSys.PathFormat(m_SMRelDir, sizeof(m_SMRelDir), value);

			m_GotBasePath = true;
		}

		return ConfigResult_Accept;
	}

	return ConfigResult_Ignore;
}

bool SourceModBase::InitializeSourceMod(char *error, size_t err_max, bool late)
{
	const char *gamepath = g_SMAPI->GetBaseDir();

	/* Store full path to game */
	g_BaseDir.assign(gamepath);

	/* Store name of game directory by itself */
	size_t len = strlen(gamepath);
	for (size_t i = len - 1; i >= 0; i--)
	{
		if (gamepath[i] == PLATFORM_SEP_CHAR)
		{
			strncopy(m_ModDir, &gamepath[++i], sizeof(m_ModDir));
			break;
		}
	}

	/* Initialize CoreConfig so we can get SourceMod base path properly - this basically parses core.cfg */
	g_CoreConfig.Initialize();

	/* This shouldn't happen, but can't hurt to be safe */
	if (!g_LibSys.PathExists(m_SMBaseDir) || !m_GotBasePath)
	{
		g_LibSys.PathFormat(m_SMBaseDir, sizeof(m_SMBaseDir), "%s/addons/sourcemod", g_BaseDir.c_str());
		g_LibSys.PathFormat(m_SMRelDir, sizeof(m_SMRelDir), "addons/sourcemod");
		m_GotBasePath = true;
	}

	/* Attempt to load the JIT! */
	char file[PLATFORM_MAX_PATH];
	char myerror[255];
	g_SMAPI->PathFormat(file, sizeof(file), "%s/bin/sourcepawn.jit.x86.%s",
		GetSourceModPath(),
		PLATFORM_LIB_EXT
		);

	g_pJIT = g_LibSys.OpenLibrary(file, myerror, sizeof(myerror));
	if (!g_pJIT)
	{
		if (error && err_max)
		{
			snprintf(error, err_max, "%s (failed to load bin/sourcepawn.jit.x86.%s)", 
				myerror,
				PLATFORM_LIB_EXT);
		}
		return false;
	}

	int err;
	GIVEENGINEPOINTER jit_init = (GIVEENGINEPOINTER)g_pJIT->GetSymbolAddress("GiveEnginePointer");
	if (!jit_init)
	{
		ShutdownJIT();
		if (error && err_max)
		{
			snprintf(error, err_max, "Failed to find GiveEnginePointer in JIT!");
		}
		return false;
	}

	if ((err=jit_init(g_pSourcePawn)) != 0)
	{
		ShutdownJIT();
		if (error && err_max)
		{
			snprintf(error, err_max, "GiveEnginePointer returned %d in the JIT", err);
		}
		return false;
	}

	GETEXPORTCOUNT jit_getnum = (GETEXPORTCOUNT)g_pJIT->GetSymbolAddress("GetExportCount");
	GETEXPORT jit_get = (GETEXPORT)g_pJIT->GetSymbolAddress("GetExport");
	if (!jit_get)
	{
		ShutdownJIT();
		if (error && err_max)
		{
			snprintf(error, err_max, "JIT is missing a necessary export!");
		}
		return false;
	}

	unsigned int num = jit_getnum();
	if (!num || ((g_pVM=jit_get(0)) == NULL))
	{
		ShutdownJIT();
		if (error && err_max)
		{
			snprintf(error, err_max, "JIT did not export any virtual machines!");
		}
		return false;
	}

	unsigned int api = g_pVM->GetAPIVersion();
	if (api != SOURCEPAWN_VM_API_VERSION)
	{
		ShutdownJIT();
		if (error && err_max)
		{
			snprintf(error, err_max, "JIT is not a compatible version");
		}
		return false;
	}

	/* Hook this now so we can detect startup without calling StartSourceMod() */
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, gamedll, this, &SourceModBase::LevelInit, false);

	/* Only load if we're not late */
	if (!late)
	{
		StartSourceMod(false);
	}

	return true;
}

void SourceModBase::StartSourceMod(bool late)
{
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, gamedll, this, &SourceModBase::LevelShutdown, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, gamedll, this, &SourceModBase::GameFrame, false);

	enginePatch = SH_GET_CALLCLASS(engine);
	gamedllPatch = SH_GET_CALLCLASS(gamedll);

	/* Notify! */
	SMGlobalClass *pBase = SMGlobalClass::head;
	while (pBase)
	{
		pBase->OnSourceModStartup(false);
		pBase = pBase->m_pGlobalClassNext;
	}

	/* Make the global core identity */
	g_pCoreIdent = g_ShareSys.CreateCoreIdentity();

	/* Notify! */
	pBase = SMGlobalClass::head;
	while (pBase)
	{
		pBase->OnSourceModAllInitialized();
		pBase = pBase->m_pGlobalClassNext;
	}

	/* Add us now... */
	g_ShareSys.AddInterface(NULL, this);

	/* We're loaded! */
	g_Loaded = true;
}

bool SourceModBase::LevelInit(char const *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background)
{
	/* If we're not loaded... */
	if (!g_Loaded)
	{
		/* Do all global initialization now */
		StartSourceMod(true);
	}

	m_IsMapLoading = true;
	m_ExecPluginReload = true;
	g_LastTime = 0.0f;
	g_LastAuthCheck = 0.0f;
	g_SimTicks.ticking = true;
	g_SimTicks.tickcount = 0;
	g_StillTime = 0.0f;
	g_StillFrames = 0;

	/* Notify! */
	SMGlobalClass *pBase = SMGlobalClass::head;
	while (pBase)
	{
		pBase->OnSourceModLevelChange(pMapName);
		pBase = pBase->m_pGlobalClassNext;
	}

	DoGlobalPluginLoads();

	m_IsMapLoading = false;

	/* Notify! */
	pBase = SMGlobalClass::head;
	while (pBase)
	{
		pBase->OnSourceModPluginsLoaded();
		pBase = pBase->m_pGlobalClassNext;
	}

	if (!g_pOnGameFrame)
	{
		g_pOnGameFrame = g_Forwards.CreateForward("OnGameFrame", ET_Ignore, 0, NULL);
	}

	if (!g_pOnMapEnd)
	{
		g_pOnMapEnd = g_Forwards.CreateForward("OnMapEnd", ET_Ignore, 0, NULL);
	}

	RETURN_META_VALUE(MRES_IGNORED, true);
}

void StartTickSimulation()
{
	g_SimTicks.ticking = false;
	g_SimTicks.tickcount = 0;
	g_SimTicks.ticktime = gpGlobals->curtime;
}

void StopTickSimulation()
{
	g_SimTicks.ticking = true;
	g_Timers.MapChange();
	g_StillFrames = 0;
	g_LastTime = gpGlobals->curtime;
}

void SimulateTick()
{
	g_SimTicks.tickcount++;
	g_SimTicks.ticktime = g_StillTime + (g_SimTicks.tickcount * gpGlobals->interval_per_tick);
}

void SourceModBase::GameFrame(bool simulating)
{
	/**
	 * Note: This is all hardcoded rather than delegated to save
	 * precious CPU cycles.
	 */
	float curtime = gpGlobals->curtime;
	int framecount = gpGlobals->framecount;

	/* Verify that we're still ticking */
	if (g_SimTicks.ticking)
	{
		if (g_StillFrames == 0)
		{
			g_StillFrames = framecount;
			g_StillTime = curtime;
		} else {
			/* Try to detect when we've stopped ticking.
			 * We do this once 10 frames pass and there have been no ticks.
			 */
			if (g_StillTime == curtime)
			{
				if (framecount - g_StillFrames >= 5)
				{
					StartTickSimulation();
					return;
				}
			} else {
				/* We're definitely ticking we get here, 
				 * but update everything as a precaution */
				g_StillFrames = framecount;
				g_StillTime = curtime;
			}
		}
	} else {
		/* We need to make sure we should still be simulating. */
		if (g_StillTime != curtime)
		{
			/* Wow, we're ticking again! It's time to revert. */
			StopTickSimulation();
			return;
		}
		/* Nope, not ticking.  Simulate! */
		SimulateTick();
		curtime = g_SimTicks.ticktime;
	}

	if (curtime - g_LastTime >= 0.1f)
	{
		if (m_CheckingAuth
			&& (gpGlobals->curtime - g_LastAuthCheck > 0.7f))
		{
			g_LastAuthCheck = gpGlobals->curtime;
			g_Players.RunAuthChecks();
		}

		g_Timers.RunFrame();
		g_LastTime = curtime;
	}

	if (g_pOnGameFrame && g_pOnGameFrame->GetFunctionCount())
	{
		g_pOnGameFrame->Execute(NULL);
	}
}

void SourceModBase::LevelShutdown()
{
	if (g_pOnMapEnd)
	{
		g_pOnMapEnd->Execute(NULL);
	}

	if (m_ExecPluginReload)
	{
		g_PluginSys.ReloadOrUnloadPlugins();
		m_ExecPluginReload = false;
	}
}

bool SourceModBase::IsMapLoading() const
{
	return m_IsMapLoading;
}

void SourceModBase::DoGlobalPluginLoads()
{
	char config_path[PLATFORM_MAX_PATH];
	char plugins_path[PLATFORM_MAX_PATH];

	BuildPath(Path_SM, config_path, 
		sizeof(config_path),
		"configs/plugin_settings.cfg");
	BuildPath(Path_SM, plugins_path,
		sizeof(plugins_path),
		"plugins");

	/* Load any auto extensions */
	g_Extensions.TryAutoload();

	/* Run the first pass */
	g_PluginSys.LoadAll_FirstPass(config_path, plugins_path);

	/* Mark any extensions as loaded */
	g_Extensions.MarkAllLoaded();

	/* No modules yet, it's safe to call this from here */
	g_PluginSys.LoadAll_SecondPass();

	/* Re-mark any extensions as loaded */
	g_Extensions.MarkAllLoaded();
}

size_t SourceModBase::BuildPath(PathType type, char *buffer, size_t maxlength, char *format, ...)
{
	char _buffer[PLATFORM_MAX_PATH];
	va_list ap;

	va_start(ap, format);
	vsnprintf(_buffer, PLATFORM_MAX_PATH, format, ap);
	va_end(ap);

	const char *base = NULL;
	if (type == Path_Game)
	{
		base = GetGamePath();
	} else if (type == Path_SM) {
		base = GetSourceModPath();
	} else if (type == Path_SM_Rel) {
		base = m_SMRelDir;
	}

	if (base)
	{
		return g_LibSys.PathFormat(buffer, maxlength, "%s/%s", base, _buffer);
	} else {
		return g_LibSys.PathFormat(buffer, maxlength, "%s", _buffer);
	}
}

void SourceModBase::CloseSourceMod()
{
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, gamedll, this, &SourceModBase::LevelInit, false);

	if (g_Loaded)
	{
		if (g_pOnGameFrame)
		{
			g_Forwards.ReleaseForward(g_pOnGameFrame);
		}

		if (g_pOnMapEnd)
		{
			g_Forwards.ReleaseForward(g_pOnMapEnd);
		}
	
		/* Notify! */
		SMGlobalClass *pBase = SMGlobalClass::head;
		while (pBase)
		{
			pBase->OnSourceModShutdown();
			pBase = pBase->m_pGlobalClassNext;
		}
	
		/* Delete all data packs */
		CStack<CDataPack *>::iterator iter;
		CDataPack *pd;
		for (iter=m_freepacks.begin(); iter!=m_freepacks.end(); iter++)
		{
			pd = (*iter);
			delete pd;
		}
		m_freepacks.popall();
	
		/* Notify! */
		pBase = SMGlobalClass::head;
		while (pBase)
		{
			pBase->OnSourceModAllShutdown();
			pBase = pBase->m_pGlobalClassNext;
		}
	
		if (enginePatch)
		{
			SH_RELEASE_CALLCLASS(enginePatch);
			enginePatch = NULL;
		}
	
		if (gamedllPatch)
		{
			SH_RELEASE_CALLCLASS(gamedllPatch);
			gamedllPatch = NULL;
		}

		SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, gamedll, this, &SourceModBase::LevelShutdown, false);
		SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, gamedll, this, &SourceModBase::GameFrame, false);
	}

	/* Rest In Peace */
	ShutdownJIT();
}

void SourceModBase::LogMessage(IExtension *pExt, const char *format, ...)
{
	IExtensionInterface *pAPI = pExt->GetAPI();
	const char *tag = pAPI->GetExtensionTag();
	char buffer[2048];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);

	if (tag)
	{
		g_Logger.LogMessage("[%s] %s", tag, buffer);
	} else {
		g_Logger.LogMessage("%s", buffer);
	}
}

void SourceModBase::LogError(IExtension *pExt, const char *format, ...)
{
	IExtensionInterface *pAPI = pExt->GetAPI();
	const char *tag = pAPI->GetExtensionTag();
	char buffer[2048];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);

	if (tag)
	{
		g_Logger.LogError("[%s] %s", tag, buffer);
	} else {
		g_Logger.LogError("%s", buffer);
	}
}

size_t SourceModBase::FormatString(char *buffer, size_t maxlength, IPluginContext *pContext, const cell_t *params, unsigned int param)
{
	char *fmt;

	pContext->LocalToString(params[param], &fmt);

	int lparam = ++param;

	return atcprintf(buffer, maxlength, fmt, pContext, params, &lparam);
}

const char *SourceModBase::GetSourceModPath() const
{
	return m_SMBaseDir;
}

const char *SourceModBase::GetGamePath() const
{
	return g_BaseDir.c_str();
}

void SourceModBase::SetGlobalTarget(unsigned int index)
{
	m_target = index;
}

void SourceModBase::SetAuthChecking(bool set)
{
	m_CheckingAuth = set;
}

unsigned int SourceModBase::GetGlobalTarget() const
{
	return m_target;
}

IDataPack *SourceModBase::CreateDataPack()
{
	CDataPack *pack;
	if (m_freepacks.empty())
	{
		pack = new CDataPack;
	} else {
		pack = m_freepacks.front();
		m_freepacks.pop();
		pack->Initialize();
	}
	return pack;
}

void SourceModBase::FreeDataPack(IDataPack *pack)
{
	m_freepacks.push(static_cast<CDataPack *>(pack));
}

Handle_t SourceModBase::GetDataPackHandleType(bool readonly)
{
	//:TODO:
	return 0;
}

const char *SourceModBase::GetGameFolderName() const
{
	return m_ModDir;
}

ISourcePawnEngine *SourceModBase::GetScriptingEngine()
{
	return g_pSourcePawn;
}

IVirtualMachine *SourceModBase::GetScriptingVM()
{
	return g_pVM;
}

SMGlobalClass *SMGlobalClass::head = NULL;

SMGlobalClass::SMGlobalClass()
{
	m_pGlobalClassNext = SMGlobalClass::head;
	SMGlobalClass::head = this;
}
