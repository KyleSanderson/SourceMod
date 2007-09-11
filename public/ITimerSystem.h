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

#ifndef _INCLUDE_SOURCEMOD_TIMER_SYSTEM_H_
#define _INCLUDE_SOURCEMOD_TIMER_SYSTEM_H_

/**
 * @file ITimerSystem.h
 * @brief Contains functions for creating and managing timers.
 */


#include <IShareSys.h>
#include <IForwardSys.h>

#define SMINTERFACE_TIMERSYS_NAME		"ITimerSys"
#define SMINTERFACE_TIMERSYS_VERSION	1

namespace SourceMod
{
	class ITimer;

	typedef float (*SM_TIMELEFT_FUNCTION)();

	/**
	 * @brief Event callbacks for when a timer is executed.
	 */
	class ITimedEvent
	{
	public:
		/**
		 * @brief Called when a timer is executed.
		 *
		 * @param pTimer		Pointer to the timer instance.
		 * @param pData			Private pointer passed from host.
		 * @return				Pl_Stop to stop timer, Pl_Continue to continue.
		 */
		virtual ResultType OnTimer(ITimer *pTimer, void *pData) =0;

		/**
		 * @brief Called when the timer has been killed.
		 *
		 * @param pTimer		Pointer to the timer instance.
		 * @param pData			Private data pointer passed from host.
		 */
		virtual void OnTimerEnd(ITimer *pTimer, void *pData) =0;
	};

	#define TIMER_FLAG_REPEAT			(1<<0)		/**< Timer will repeat until stopped */
	#define TIMER_FLAG_NO_MAPCHANGE		(1<<1)		/**< Timer will not carry over mapchanges */
	#define TIMER_FLAG_BEFORE_MAP_END	(1<<2)		/**< Timer will fire <interval> seconds before map end.
														 This flag is cannot be used with REPEAT, 
														 and TIMER_FLAG_NO_MAPCHANGE is implied.
														 */

	class ITimerSystem : public SMInterface
	{
	public:
		const char *GetInterfaceName()
		{
			return SMINTERFACE_TIMERSYS_NAME;
		}
		unsigned int GetInterfaceVersion()
		{
			return SMINTERFACE_TIMERSYS_VERSION;
		}
	public:
		/**
		 * @brief Creates a timed event.
		 *
		 * @param pCallbacks		Pointer to ITimedEvent callbacks.
		 * @param fInterval			Interval, in seconds, of the timed event to occur.
		 *							The smallest allowed interval is 0.1 seconds.
		 * @param pData				Private data to pass on to the timer.
		 * @param flags				Extra flags to pass on to the timer.
		 * @return					An ITimer pointer on success, NULL on 
		 *							failure.
		 */
		virtual ITimer *CreateTimer(ITimedEvent *pCallbacks, 
										 float fInterval, 
										 void *pData,
										 int flags) =0;

		/**
		 * @brief Kills a timer.
		 *
		 * @param pTimer			Pointer to the ITimer structure.
		 * @return
		 */
		virtual void KillTimer(ITimer *pTimer) =0;

		/**
		 * @brief Arbitrarily fires a timer.  If the timer is not a repeating 
		 * timer, this will also kill the timer.
		 *
		 * @param pTimer			Pointer to the ITimer structure.
		 * @param delayExec			If true, and the timer is repeating, the
		 *							next execution will be delayed by its 
		 *							interval.
		 * @return
		 */
		virtual void FireTimerOnce(ITimer *pTimer, bool delayExec=false) =0;

		/**
		 * @brief Sets the function which is used to find the time left in the map.
		 *
		 * @param fn				Function that returns the time left in seconds.
		 * @return					The previous SM_TIMELEFT_FUNCTION pointer.
		 */
		virtual SM_TIMELEFT_FUNCTION SetTimeLeftFunction(SM_TIMELEFT_FUNCTION fn) =0;
	};
}

#endif //_INCLUDE_SOURCEMOD_TIMER_SYSTEM_H_

