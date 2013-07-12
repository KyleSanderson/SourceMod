/**
* vim: set ts=4 :
* =============================================================================
* SourceMod SDKTools Extension
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

#ifndef _INCLUDE_SOURCEMOD_VSOUND_H_
#define _INCLUDE_SOURCEMOD_VSOUND_H_

#include <sh_list.h>
#include "extension.h"
#include "CellRecipientFilter.h"

#define NORMAL_SOUND_HOOK		0
#define AMBIENT_SOUND_HOOK		1

typedef SourceHook::List<IPluginFunction *>::iterator SoundHookIter;

class SoundHooks : public IPluginsListener
{
public: //IPluginsListener
	void OnPluginUnloaded(IPlugin *plugin);
public:
	void Initialize();
	void Shutdown();
	void AddHook(int type, IPluginFunction *pFunc);
	bool RemoveHook(int type, IPluginFunction *pFunc);

#if SOURCE_ENGINE == SE_DOTA
	void OnEmitAmbientSound(CEntityIndex index, const Vector &pos, const char *samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch, float delay);
#else
	void OnEmitAmbientSound(int entindex, const Vector &pos, const char *samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch, float delay);
#endif

#if SOURCE_ENGINE >= SE_PORTAL2
	int OnEmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *, unsigned int, const char *pSample, float flVolume, 
		soundlevel_t iSoundlevel, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, 
		const Vector *pDirection, CUtlVector<Vector> *pUtlVecOrigins, bool bUpdatePositions, 
		float soundtime, int speakerentity);
	int OnEmitSound2(IRecipientFilter &filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, 
		float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, 
		const Vector *pDirection, CUtlVector<Vector> *pUtlVecOrigins, bool bUpdatePositions, 
		float soundtime, int speakerentity);
#else
#if SOURCE_ENGINE == SE_ORANGEBOXVALVE || SOURCE_ENGINE == SE_CSS
	
	void OnEmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, float flVolume, 
		soundlevel_t iSoundlevel, int iFlags, int iPitch, int iSpecialDSP, const Vector *pOrigin, 
		const Vector *pDirection, CUtlVector<Vector> *pUtlVecOrigins, bool bUpdatePositions, 
		float soundtime, int speakerentity);
	void OnEmitSound2(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, float flVolume, 
		float flAttenuation, int iFlags, int iPitch, int iSpecialDSP, const Vector *pOrigin, 
		const Vector *pDirection, CUtlVector<Vector> *pUtlVecOrigins, bool bUpdatePositions, 
		float soundtime, int speakerentity);
#else
	void OnEmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, float flVolume, 
		soundlevel_t iSoundlevel, int iFlags, int iPitch, const Vector *pOrigin, 
		const Vector *pDirection, CUtlVector<Vector> *pUtlVecOrigins, bool bUpdatePositions, 
		float soundtime, int speakerentity);
	void OnEmitSound2(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, float flVolume, 
		float flAttenuation, int iFlags, int iPitch, const Vector *pOrigin, 
		const Vector *pDirection, CUtlVector<Vector> *pUtlVecOrigins, bool bUpdatePositions, 
		float soundtime, int speakerentity);
#endif // SOURCE_ENGINE == SE_ORANGEBOXVALVE || SOURCE_ENGINE == SE_CSS
#endif // SOURCE_ENGINE >= SE_PORTAL2
private:
	size_t _FillInPlayers(int *pl_array, IRecipientFilter *pFilter);
	void _IncRefCounter(int type);
	void _DecRefCounter(int type);
private:
	SourceHook::List<IPluginFunction *> m_AmbientFuncs;
	SourceHook::List<IPluginFunction *> m_NormalFuncs;
	size_t m_NormalCount;
	size_t m_AmbientCount;
};

extern SoundHooks s_SoundHooks;

#endif //_INCLUDE_SOURCEMOD_VSOUND_H_
