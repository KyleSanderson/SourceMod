/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod
 * Copyright (C) 2004-2007 AlliedModders LLC.  All rights reserved.
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

#ifndef _INCLUDE_SOURCEMOD_CTIMERSYS_H_
#define _INCLUDE_SOURCEMOD_CTIMERSYS_H_

#include "ShareSys.h"
#include <ITimerSystem.h>
#include <sh_stack.h>
#include <sh_list.h>
#include "sourcemm_api.h"

using namespace SourceHook;
using namespace SourceMod;

typedef List<ITimer *> TimerList;
typedef List<ITimer *>::iterator TimerIter;

struct TickInfo
{
	bool ticking;				/* true=game is ticking, false=we're ticking */
	unsigned int tickcount;		/* number of simulated ticks we've done */
	float ticktime;				/* tick time we're maintaining */
};

class SourceMod::ITimer
{
public:
	void Initialize(ITimedEvent *pCallbacks, float fInterval, float fToExec, void *pData, int flags);
	ITimedEvent *m_Listener;
	void *m_pData;
	float m_Interval;
	float m_ToExec;
	int m_Flags;
	bool m_InExec;
	bool m_KillMe;
};

class TimerSystem : 
	public ITimerSystem,
	public SMGlobalClass
{
public:
	~TimerSystem();
public: //SMGlobalClass
	void OnSourceModAllInitialized();
	void OnSourceModLevelChange(const char *mapName);
public: //ITimerSystem
	ITimer *CreateTimer(ITimedEvent *pCallbacks, float fInterval, void *pData, int flags);
	void KillTimer(ITimer *pTimer);
	void FireTimerOnce(ITimer *pTimer, bool delayExec=false);
public:
	void RunFrame();
	void MapChange(bool real_mapchange);
private:
	List<ITimer *> m_SingleTimers;
	List<ITimer *> m_LoopTimers;
	CStack<ITimer *> m_FreeTimers;
	float m_LastExecTime;
};

time_t GetAdjustedTime(time_t *buf = NULL);

extern TimerSystem g_Timers;
extern TickInfo g_SimTicks;

#endif //_INCLUDE_SOURCEMOD_CTIMERSYS_H_
