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

#ifndef _INCLUDE_MENUSTYLE_VALVE_H
#define _INCLUDE_MENUSTYLE_VALVE_H

#include "sm_globals.h"
#include "MenuManager.h"
#include "MenuStyle_Base.h"
#include "sourcemm_api.h"
#include "KeyValues.h"
#include "sm_fastlink.h"

using namespace SourceMod;

class CValveMenuPlayer : public CBaseMenuPlayer
{
public:
	CValveMenuPlayer() : curPrioLevel(1)
	{
	}
	int curPrioLevel;
};

class CValveMenu;
class CValveMenuDisplay;

class ValveMenuStyle : 
	public SMGlobalClass,
	public BaseMenuStyle
{
public:
	ValveMenuStyle();
	bool OnClientCommand(int client);
public: //BaseMenuStyle
	CBaseMenuPlayer *GetMenuPlayer(int client);
	void SendDisplay(int client, IMenuDisplay *display);
	bool DoClientMenu(int client, CBaseMenu *menu, IMenuHandler *mh, unsigned int time);
	bool DoClientMenu(int client, IMenuDisplay *menu, IMenuHandler *mh, unsigned int time);
public: //SMGlobalClass
	void OnSourceModAllInitialized();
	void OnSourceModShutdown();
	void OnSourceModVSPReceived(IServerPluginCallbacks *iface);
public: //IMenuStyle
	const char *GetStyleName();
	IMenuDisplay *CreateDisplay();
	IBaseMenu *CreateMenu();
	unsigned int GetMaxPageItems();
private:
	void HookCreateMessage(edict_t *pEdict, DIALOG_TYPE type, KeyValues *kv, IServerPluginCallbacks *plugin);
private:
	CValveMenuPlayer *m_players;
};

class CValveMenu;

class CValveMenuDisplay : public IMenuDisplay
{
public:
	CValveMenuDisplay();
	CValveMenuDisplay(CValveMenu *pMenu);
	~CValveMenuDisplay();
public:
	IMenuStyle *GetParentStyle();
	void Reset();
	void DrawTitle(const char *text, bool onlyIfEmpty=false);
	unsigned int DrawItem(const ItemDrawInfo &item);
	bool DrawRawLine(const char *rawline);
	bool SendDisplay(int client, IMenuHandler *handler, unsigned int time);
	bool SetExtOption(MenuOption option, const void *valuePtr);
	bool CanDrawItem(unsigned int drawFlags);
	void SendRawDisplay(int client, int priority, unsigned int time);
private:
	KeyValues *m_pKv;
	unsigned int m_NextPos;
	bool m_TitleDrawn;
};

class CValveMenu : public CBaseMenu
{
	friend class CValveMenuDisplay;
public:
	CValveMenu();
public:
	bool SetExtOption(MenuOption option, const void *valuePtr);
	IMenuDisplay *CreateDisplay();
	bool GetExitButton();
	bool SetExitButton(bool set);
	bool SetPagination(unsigned int itemsPerPage);
	bool Display(int client, IMenuHandler *handler, unsigned int time);
	void Cancel_Finally();
private:
	Color m_IntroColor;
	char m_IntroMsg[128];
};

extern ValveMenuStyle g_ValveMenuStyle;

#endif //_INCLUDE_MENUSTYLE_VALVE_H
