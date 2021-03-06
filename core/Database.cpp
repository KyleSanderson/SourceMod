/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod
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

#include "Database.h"
#include "sourcemod.h"
#include "sm_stringutil.h"
#include "Logger.h"
#include <stdlib.h>
#include <IThreader.h>
#include "logic_bridge.h"

#define DBPARSE_LEVEL_NONE		0
#define DBPARSE_LEVEL_MAIN		1
#define DBPARSE_LEVEL_DATABASE	2

DBManager g_DBMan;
static bool s_OneTimeThreaderErrorMsg = false;

DBManager::DBManager() 
: m_ParseLevel(0), m_ParseState(0), m_pDefault(NULL)
{
}

void DBManager::OnSourceModAllInitialized()
{
	HandleAccess sec;

	handlesys->InitAccessDefaults(NULL, &sec);
	sec.access[HandleAccess_Delete] |= HANDLE_RESTRICT_IDENTITY;
	sec.access[HandleAccess_Clone] |= HANDLE_RESTRICT_IDENTITY;
	
	m_DriverType = handlesys->CreateType("IDriver", this, 0, NULL, &sec, g_pCoreIdent, NULL);
	m_DatabaseType = handlesys->CreateType("IDatabase", this, 0, NULL, NULL, g_pCoreIdent, NULL);

	sharesys->AddInterface(NULL, this);

	g_SourceMod.BuildPath(Path_SM, m_Filename, sizeof(m_Filename), "configs/databases.cfg");

	m_pConfigLock = g_pThreader->MakeMutex();
	m_pThinkLock = g_pThreader->MakeMutex();
	m_pQueueLock = g_pThreader->MakeMutex();

	scripts->AddPluginsListener(this);
}

void DBManager::OnSourceModLevelChange(const char *mapName)
{
	SMCError err;
	SMCStates states = {0, 0};

	/* We lock and don't give up the lock until we're done.
	 * This way the thread's search won't be searching through a
	 * potentially empty/corrupt list, which would be very bad.
	 */
	m_pConfigLock->Lock();
	if ((err = textparsers->ParseFile_SMC(m_Filename, this, &states)) != SMCError_Okay)
	{
		g_Logger.LogError("[SM] Detected parse error(s) in file \"%s\"", m_Filename);
		if (err != SMCError_Custom)
		{
			const char *txt = textparsers->GetSMCErrorString(err);
			g_Logger.LogError("[SM] Line %d: %s", states.line, txt);
		}
	}
	m_pConfigLock->Unlock();
}

void DBManager::OnSourceModShutdown()
{
	KillWorkerThread();
	scripts->RemovePluginsListener(this);
	m_pConfigLock->DestroyThis();
	m_pThinkLock->DestroyThis();
	m_pQueueLock->DestroyThis();
	handlesys->RemoveType(m_DatabaseType, g_pCoreIdent);
	handlesys->RemoveType(m_DriverType, g_pCoreIdent);
	ClearConfigs();
}

unsigned int DBManager::GetInterfaceVersion()
{
	return SMINTERFACE_DBI_VERSION;
}

const char *DBManager::GetInterfaceName()
{
	return SMINTERFACE_DBI_NAME;
}

void DBManager::OnHandleDestroy(HandleType_t type, void *object)
{
	if (type == m_DriverType)
	{
		/* Ignore */
		return;
	}

	if (handlesys->TypeCheck(type, m_DatabaseType))
	{
		IDatabase *pdb = (IDatabase *)object;
		pdb->Close();
	}
}

void DBManager::ReadSMC_ParseStart()
{
	ClearConfigs();
	m_ParseLevel = 0;
	m_ParseState = DBPARSE_LEVEL_NONE;
	m_DefDriver.clear();
}

void DBManager::ClearConfigs()
{
	List<ConfDbInfo *>::iterator iter;
	for (iter=m_confs.begin(); iter!=m_confs.end(); iter++)
		delete (*iter);
	m_confs.clear();
}

ConfDbInfo s_CurInfo;
SMCResult DBManager::ReadSMC_NewSection(const SMCStates *states, const char *name)
{
	if (m_ParseLevel)
	{
		m_ParseLevel++;
		return SMCResult_Continue;
	}

	if (m_ParseState == DBPARSE_LEVEL_NONE)
	{
		if (strcmp(name, "Databases") == 0)
		{
			m_ParseState = DBPARSE_LEVEL_MAIN;
		} else {
			m_ParseLevel++;
		}
	} else if (m_ParseState == DBPARSE_LEVEL_MAIN) {
		s_CurInfo = ConfDbInfo();
		s_CurInfo.name = name;
		m_ParseState = DBPARSE_LEVEL_DATABASE;
	} else if (m_ParseState == DBPARSE_LEVEL_DATABASE) {
		m_ParseLevel++;
	}

	return SMCResult_Continue;
}

SMCResult DBManager::ReadSMC_KeyValue(const SMCStates *states, const char *key, const char *value)
{
	if (m_ParseLevel)
	{
		return SMCResult_Continue;
	}

	if (m_ParseState == DBPARSE_LEVEL_MAIN)
	{
		if (strcmp(key, "driver_default") == 0)
		{
			m_DefDriver.assign(value);
		}
	} else if (m_ParseState == DBPARSE_LEVEL_DATABASE) {
		if (strcmp(key, "driver") == 0)
		{
			if (strcmp(value, "default") != 0)
			{
				s_CurInfo.driver = value;
			}
		} else if (strcmp(key, "database") == 0) {
			s_CurInfo.database = value;
		} else if (strcmp(key, "host") == 0) {
			s_CurInfo.host = value;
		} else if (strcmp(key, "user") == 0) {
			s_CurInfo.user = value;
		} else if (strcmp(key, "pass") == 0) {
			s_CurInfo.pass = value;
		} else if (strcmp(key, "timeout") == 0) {
			s_CurInfo.info.maxTimeout = atoi(value);
		} else if (strcmp(key, "port") == 0) {
			s_CurInfo.info.port = atoi(value);
		}
	}

	return SMCResult_Continue;
}

SMCResult DBManager::ReadSMC_LeavingSection(const SMCStates *states)
{
	if (m_ParseLevel)
	{
		m_ParseLevel--;
		return SMCResult_Continue;
	}

	if (m_ParseState == DBPARSE_LEVEL_DATABASE)
	{
		ConfDbInfo *cdb = new ConfDbInfo();

		cdb->name = s_CurInfo.name;
		cdb->driver = s_CurInfo.driver;
		cdb->host = s_CurInfo.host;
		cdb->user = s_CurInfo.user;
		cdb->pass = s_CurInfo.pass;
		cdb->database = s_CurInfo.database;
		cdb->realDriver = s_CurInfo.realDriver;
		cdb->info.maxTimeout = s_CurInfo.info.maxTimeout;
		cdb->info.port = s_CurInfo.info.port;

		cdb->info.driver = cdb->driver.c_str();
		cdb->info.database = cdb->database.c_str();
		cdb->info.host = cdb->host.c_str();
		cdb->info.user = cdb->user.c_str();
		cdb->info.pass = cdb->pass.c_str();
		
		/* Save it.. */
		m_confs.push_back(cdb);

		/* Go up one level */
		m_ParseState = DBPARSE_LEVEL_MAIN;
	} else if (m_ParseState == DBPARSE_LEVEL_MAIN) {
		m_ParseState = DBPARSE_LEVEL_NONE;
		return SMCResult_Halt;
	}

	return SMCResult_Continue;
}

void DBManager::ReadSMC_ParseEnd(bool halted, bool failed)
{
}

bool DBManager::Connect(const char *name, IDBDriver **pdr, IDatabase **pdb, bool persistent, char *error, size_t maxlength)
{
	ConfDbInfo *pInfo = GetDatabaseConf(name);

	if (!pInfo)
	{
		if (pdr)
		{
			*pdr = NULL;
		}
		*pdb = NULL;
		UTIL_Format(error, maxlength, "Configuration \"%s\" not found", name);
		return false;
	}

	const char *dname = pInfo->info.driver;
	if (!pInfo->realDriver)
	{
		/* Try to assign a real driver pointer */
		if (pInfo->info.driver[0] == '\0')
		{
			if (!m_pDefault && m_DefDriver.size() > 0)
			{
				m_pDefault = FindOrLoadDriver(m_DefDriver.c_str());
			}
			dname = m_DefDriver.size() ? m_DefDriver.c_str() : "default";
			pInfo->realDriver = m_pDefault;
		} else {
			pInfo->realDriver = FindOrLoadDriver(pInfo->info.driver);
		}
	}

	if (pInfo->realDriver)
	{
		if (pdr)
		{
			*pdr = pInfo->realDriver;
		}
		*pdb = pInfo->realDriver->Connect(&pInfo->info, persistent, error, maxlength);
		return (*pdb != NULL);
	}

	if (pdr)
	{
		*pdr = NULL;
	}
	*pdb = NULL;

	UTIL_Format(error, maxlength, "Driver \"%s\" not found", dname);

	return false;
}

void DBManager::AddDriver(IDBDriver *pDriver)
{
	 /* Let's kill the worker.  Join the thread and let the queries flush.
	  * This is kind of stupid but we just want to unload safely.
	  * Rather than recreate the worker, we'll wait until someone throws 
	  * another query through.
	  */
	KillWorkerThread();

	 m_drivers.push_back(pDriver);
}

void DBManager::RemoveDriver(IDBDriver *pDriver)
{
	/* Again, we're forced to kill the worker.  How rude!
	 * Doing this flushes the queue, and thus we don't need to 
	 * clean anything else.  
	 */
	KillWorkerThread();

	for (size_t i=0; i<m_drivers.size(); i++)
	{
		if (m_drivers[i] == pDriver)
		{
			m_drivers.erase(m_drivers.iterAt(i));
			break;
		}
	}

	/* Make sure NOTHING references this! */
	List<ConfDbInfo *>::iterator iter;
	for (iter=m_confs.begin(); iter!=m_confs.end(); iter++)
	{
		ConfDbInfo &db = *(*iter);
		if (db.realDriver == pDriver)
		{
			db.realDriver = NULL;
		}
	}

	/* Now that the driver is gone, we have to test the think queue.
	 * Whatever happens therein is up to the db op!
	 */
	Queue<IDBThreadOperation *>::iterator qiter = m_ThinkQueue.begin();
	Queue<IDBThreadOperation *> templist;
	while (qiter != m_ThinkQueue.end())
	{
		IDBThreadOperation *op = (*qiter);
		if (op->GetDriver() == pDriver)
		{
			templist.push(op);
			qiter = m_ThinkQueue.erase(qiter);
		} else {
			qiter++;
		}
	}

	for (qiter = templist.begin();
		 qiter != templist.end();
		 qiter++)
	{
		IDBThreadOperation *op = (*qiter);
		op->CancelThinkPart();
		op->Destroy();
	}
}

IDBDriver *DBManager::GetDefaultDriver()
{
	if (!m_pDefault && m_DefDriver.size() > 0)
	{
		m_pDefault = FindOrLoadDriver(m_DefDriver.c_str());
	}

	return m_pDefault;
}

Handle_t DBManager::CreateHandle(DBHandleType dtype, void *ptr, IdentityToken_t *pToken)
{
	HandleType_t type = 0;

	if (dtype == DBHandle_Driver)
	{
		type = m_DriverType;
	} else if (dtype == DBHandle_Database) {
		type = m_DatabaseType;
	} else {
		return BAD_HANDLE;
	}

	return handlesys->CreateHandle(type, ptr, pToken, g_pCoreIdent, NULL);
}

HandleError DBManager::ReadHandle(Handle_t hndl, DBHandleType dtype, void **ptr)
{
	HandleType_t type = 0;

	if (dtype == DBHandle_Driver)
	{
		type = m_DriverType;
	} else if (dtype == DBHandle_Database) {
		type = m_DatabaseType;
	} else {
		return HandleError_Type;
	}

	HandleSecurity sec(NULL, g_pCoreIdent);

	return handlesys->ReadHandle(hndl, type, &sec, ptr);
}

HandleError DBManager::ReleaseHandle(Handle_t hndl, DBHandleType type, IdentityToken_t *token)
{
	HandleSecurity sec(token, g_pCoreIdent);
	return handlesys->FreeHandle(hndl, &sec);
}

unsigned int DBManager::GetDriverCount()
{
	return (unsigned int)m_drivers.size();
}

IDBDriver *DBManager::GetDriver(unsigned int index)
{
	if (index >= m_drivers.size())
	{
		return NULL;
	}

	return m_drivers[index];
}

const DatabaseInfo *DBManager::FindDatabaseConf(const char *name)
{
	ConfDbInfo *info = GetDatabaseConf(name);
	if (!info)
	{
		return NULL;
	}

	return &info->info;
}

ConfDbInfo *DBManager::GetDatabaseConf(const char *name)
{
	List<ConfDbInfo *>::iterator iter;

	for (iter=m_confs.begin(); iter!=m_confs.end(); iter++)
	{
		ConfDbInfo &conf = *(*iter);
		if (conf.name == name)
		{
			return &conf;
		}
	}

	return NULL;
}

IDBDriver *DBManager::FindOrLoadDriver(const char *name)
{
	size_t last_size = m_drivers.size();
	for (size_t i=0; i<last_size; i++)
	{
		if (strcmp(m_drivers[i]->GetIdentifier(), name) == 0)
		{
			return m_drivers[i];
		}
	}

	char filename[PLATFORM_MAX_PATH];
	UTIL_Format(filename, sizeof(filename), "dbi.%s.ext", name);

	IExtension *pExt = extsys->LoadAutoExtension(filename);
	if (!pExt || !pExt->IsLoaded() || m_drivers.size() <= last_size)
	{
		return NULL;
	}

	/* last_size is now gauranteed to be a valid index.
	 * The identifier must match the name.
	 */
	if (strcmp(m_drivers[last_size]->GetIdentifier(), name) == 0)
	{
		return m_drivers[last_size];
	}

	return NULL;
}

void DBManager::KillWorkerThread()
{
	if (m_pWorker)
	{
		m_pWorker->Stop(false);
		g_pThreader->DestroyWorker(m_pWorker);
		m_pWorker = NULL;
		s_OneTimeThreaderErrorMsg = false;
	}
}

static IdentityToken_t *s_pAddBlock = NULL;

bool DBManager::AddToThreadQueue(IDBThreadOperation *op, PrioQueueLevel prio)
{
	if (s_pAddBlock && op->GetOwner() == s_pAddBlock)
	{
		return false;
	}

	if (!m_pWorker)
	{
		m_pWorker = g_pThreader->MakeWorker(this, true);
		if (!m_pWorker)
		{
			if (!s_OneTimeThreaderErrorMsg)
			{
				g_Logger.LogError("[SM] Unable to create db threader (error unknown)");
				s_OneTimeThreaderErrorMsg = true;
			}
			return false;
		}
		if (!m_pWorker->Start())
		{
			if (!s_OneTimeThreaderErrorMsg)
			{
				g_Logger.LogError("[SM] Unable to start db threader (error unknown)");
				s_OneTimeThreaderErrorMsg = true;
			}
			g_pThreader->DestroyWorker(m_pWorker);
			m_pWorker = NULL;
			return false;
		}
	}

	/* Add to the queue */
	{
		m_pQueueLock->Lock();
		Queue<IDBThreadOperation *> &queue = m_OpQueue.GetQueue(prio);
		queue.push(op);
		m_pQueueLock->Unlock();
	}

	/* Make the thread */
	m_pWorker->MakeThread(this);

	return true;
}

void DBManager::OnWorkerStart(IThreadWorker *pWorker)
{
	m_drSafety.clear();
	for (size_t i=0; i<m_drivers.size(); i++)
	{
		if (m_drivers[i]->IsThreadSafe())
		{
			m_drSafety.push_back(m_drivers[i]->InitializeThreadSafety());
		} else {
			m_drSafety.push_back(false);
		}
	}
}

void DBManager::OnWorkerStop(IThreadWorker *pWorker)
{
	for (size_t i=0; i<m_drivers.size(); i++)
	{
		if (m_drSafety[i])
		{
			m_drivers[i]->ShutdownThreadSafety();
		}
	}
	m_drSafety.clear();
}

void DBManager::RunThread(IThreadHandle *pThread)
{
	IDBThreadOperation *op = NULL;

	/* Get something from the queue */
	{
		m_pQueueLock->Lock();
		Queue<IDBThreadOperation *> &queue = m_OpQueue.GetLikelyQueue();
		if (!queue.empty())
		{
			op = queue.first();
			queue.pop();
		}
		m_pQueueLock->Unlock();
	}

	/* whoa.  hi.  did we get something? we should have. */
	if (!op)
	{
		/* wtf? */
		return;
	}

	op->RunThreadPart();

	m_pThinkLock->Lock();
	m_ThinkQueue.push(op);
	m_pThinkLock->Unlock();
}

void DBManager::RunFrame()
{
	/* Don't bother if we're empty */
	if (!m_ThinkQueue.size())
	{
		return;
	}

	/* Dump one thing per-frame so the server stays sane. */
	m_pThinkLock->Lock();
	IDBThreadOperation *op = m_ThinkQueue.first();
	m_ThinkQueue.pop();
	m_pThinkLock->Unlock();
	op->RunThinkPart();
	op->Destroy();
}

void DBManager::OnTerminate(IThreadHandle *pThread, bool cancel)
{
	/* Do nothing */
}

void DBManager::OnSourceModIdentityDropped(IdentityToken_t *pToken)
{
	s_pAddBlock = pToken;

	/* Kill the thread so we can flush everything into the think queue... */
	KillWorkerThread();

	/* Run all of the think operations.
 	 * Unlike the driver unloading example, we'll let these calls go through, 
	 * since a plugin unloading is far more normal.
	 */
	Queue<IDBThreadOperation *>::iterator iter = m_ThinkQueue.begin();
	Queue<IDBThreadOperation *> templist;
	while (iter != m_ThinkQueue.end())
	{
		IDBThreadOperation *op = (*iter);
		if (op->GetOwner() == pToken)
		{
			templist.push(op);
			iter = m_ThinkQueue.erase(iter);
		} else {
			iter++;
		}
	}

	for (iter = templist.begin();
		iter != templist.end();
		iter++)
	{
		IDBThreadOperation *op = (*iter);
		op->RunThinkPart();
		op->Destroy();
	}

	s_pAddBlock = NULL;
}

void DBManager::OnPluginUnloaded(IPlugin *plugin)
{
	/* Kill the thread so we can flush everything into the think queue... */
	KillWorkerThread();

	/* Mark the plugin as being unloaded so future database calls will ignore threading... */
	plugin->SetProperty("DisallowDBThreads", NULL);

	/* Run all of the think operations.
	 * Unlike the driver unloading example, we'll let these calls go through, 
	 * since a plugin unloading is far more normal.
	 */
	Queue<IDBThreadOperation *>::iterator iter = m_ThinkQueue.begin();
	Queue<IDBThreadOperation *> templist;
	while (iter != m_ThinkQueue.end())
	{
		IDBThreadOperation *op = (*iter);
		if (op->GetOwner() == plugin->GetIdentity())
		{
			templist.push(op);
			iter = m_ThinkQueue.erase(iter);
		} else {
			iter++;
		}
	}

	for (iter = templist.begin();
		 iter != templist.end();
		 iter++)
	{
		IDBThreadOperation *op = (*iter);
		op->RunThinkPart();
		op->Destroy();
	}
}

void DBManager::LockConfig()
{
	m_pConfigLock->Lock();
}

void DBManager::UnlockConfig()
{
	m_pConfigLock->Unlock();
}

const char *DBManager::GetDefaultDriverName()
{
	return m_DefDriver.c_str();
}

void DBManager::AddDependency(IExtension *myself, IDBDriver *driver)
{
	extsys->AddRawDependency(myself, driver->GetIdentity(), driver);
}

