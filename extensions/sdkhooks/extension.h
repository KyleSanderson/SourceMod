#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "smsdk_ext.h"
#include <IBinTools.h>
#include <convar.h>

#include <iplayerinfo.h>
#include <shareddefs.h>

#include "takedamageinfohack.h"

#ifndef METAMOD_PLAPI_VERSION
#define GetCGlobals pGlobals
#define GetEngineFactory engineFactory
#define GetServerFactory serverFactory
#endif

#if SOURCE_ENGINE >= SE_CSS && SOURCE_ENGINE != SE_LEFT4DEAD
#define GETMAXHEALTH_IS_VIRTUAL
#endif
#if SOURCE_ENGINE != SE_ORANGEBOXVALVE && SOURCE_ENGINE != SE_CSS && SOURCE_ENGINE != SE_LEFT4DEAD2 && SOURCE_ENGINE != SE_CSGO
#define GAMEDESC_CAN_CHANGE
#endif


/**
 * Globals
 */

struct HookTypeData
{
	const char *name;
	const char *dtReq;
	bool supported;
};

enum SDKHookType
{
	SDKHook_EndTouch,
	SDKHook_FireBulletsPost,
	SDKHook_OnTakeDamage,
	SDKHook_OnTakeDamagePost,
	SDKHook_PreThink,
	SDKHook_PostThink,
	SDKHook_SetTransmit,
	SDKHook_Spawn,
	SDKHook_StartTouch,
	SDKHook_Think,
	SDKHook_Touch,
	SDKHook_TraceAttack,
	SDKHook_TraceAttackPost,
	SDKHook_WeaponCanSwitchTo,
	SDKHook_WeaponCanUse,
	SDKHook_WeaponDrop,
	SDKHook_WeaponEquip,
	SDKHook_WeaponSwitch,
	SDKHook_ShouldCollide,
	SDKHook_PreThinkPost,
	SDKHook_PostThinkPost,
	SDKHook_ThinkPost,
	SDKHook_EndTouchPost,
	SDKHook_GroundEntChangedPost,
	SDKHook_SpawnPost,
	SDKHook_StartTouchPost,
	SDKHook_TouchPost,
	SDKHook_VPhysicsUpdate,
	SDKHook_VPhysicsUpdatePost,
	SDKHook_WeaponCanSwitchToPost,
	SDKHook_WeaponCanUsePost,
	SDKHook_WeaponDropPost,
	SDKHook_WeaponEquipPost,
	SDKHook_WeaponSwitchPost,
	SDKHook_Use,
	SDKHook_UsePost,
	SDKHook_Reload,
	SDKHook_ReloadPost,
	SDKHook_GetMaxHealth,
	SDKHook_MAXHOOKS
};

enum HookReturn
{
	HookRet_Successful,
	HookRet_InvalidEntity,
	HookRet_InvalidHookType,
	HookRet_NotSupported,
	HookRet_BadEntForHookType,
};

#if SOURCE_ENGINE >= SE_CSS
typedef void *(*ReticulateSplines)();
#endif

/**
 * Classes
 */
class IPhysicsObject;
typedef CBaseEntity CBaseCombatWeapon;

class HookList
{
public:
	int entity;
	IPluginFunction *callback;
};

class IEntityListener
{
public:
	virtual void OnEntityCreated( CBaseEntity *pEntity ) {};
	virtual void OnEntitySpawned( CBaseEntity *pEntity ) {};
	virtual void OnEntityDeleted( CBaseEntity *pEntity ) {};
};

class SDKHooks :
	public SDKExtension,
	public IConCommandBaseAccessor,
	public IPluginsListener,
	public IFeatureProvider,
	public IEntityListener,
	public IClientListener
{
public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
	
	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload();

	/**
	 * @brief This is called once all known extensions have been loaded.
	 * Note: It is is a good idea to add natives here, if any are provided.
	 */
	virtual void SDK_OnAllLoaded();

	/**
	 * @brief Called when the pause state is changed.
	 */
	//virtual void SDK_OnPauseChange(bool paused);

	/**
	 * @brief this is called when Core wants to know if your extension is working.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @return			True if working, false otherwise.
	 */
	//virtual bool QueryRunning(char *error, size_t maxlength);

	/** Returns version string */
	virtual const char *GetExtensionVerString();

	/** Returns date string */
	virtual const char *GetExtensionDateString();

public:
#if defined SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlength, bool late);

	/**
	 * @brief Called when Metamod is detaching, after the extension version is called.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodUnload(char *error, size_t maxlength);

	/**
	 * @brief Called when Metamod's pause state is changing.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param paused		Pause state being set.
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlength);
#endif

public:  // IPluginsListener
	virtual void OnPluginLoaded(IPlugin *plugin);
	virtual void OnPluginUnloaded(IPlugin *plugin);

public:  // IConCommandBaseAccessor
	virtual bool RegisterConCommandBase(ConCommandBase *pVar);

public:  // IFeatureProvider
	virtual FeatureStatus GetFeatureStatus(FeatureType type, const char *name);

public:  // IEntityListener
	virtual void OnEntityCreated(CBaseEntity *pEntity);
	virtual void OnEntityDeleted(CBaseEntity *pEntity);

public:  // IClientListener
	virtual void OnClientPutInServer(int client);

public:
	/**
	 * Functions
	 */
	cell_t Call(int entity, SDKHookType type, int other=-2);
	cell_t Call(CBaseEntity *pEnt, SDKHookType type, int other=-2);
	cell_t Call(CBaseEntity *pEnt, SDKHookType type, CBaseEntity *pOther);
	void SetupHooks();

	HookReturn Hook(int entity, SDKHookType type, IPluginFunction *callback);
	void Unhook(SDKHookType type, int index);

	/**
	 * IServerGameDLL & IVEngineServer Hook Handlers
	 */
#ifdef GAMEDESC_CAN_CHANGE
	const char *Hook_GetGameDescription();
#endif
	const char *Hook_GetMapEntitiesString();
	bool Hook_LevelInit(char const *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background);

	/**
	 * CBaseEntity Hook Handlers
	 */
	void Hook_EndTouch(CBaseEntity *pOther);
	void Hook_EndTouchPost(CBaseEntity *pOther);
	void Hook_FireBulletsPost(const FireBulletsInfo_t &info);
#ifdef GETMAXHEALTH_IS_VIRTUAL
	int Hook_GetMaxHealth();
#endif
	void Hook_GroundEntChangedPost();
	int Hook_OnTakeDamage(CTakeDamageInfoHack &info);
	int Hook_OnTakeDamagePost(CTakeDamageInfoHack &info);
	void Hook_PreThink();
	void Hook_PreThinkPost();
	void Hook_PostThink();
	void Hook_PostThinkPost();
	bool Hook_Reload();
	bool Hook_ReloadPost();
	void Hook_SetTransmit(CCheckTransmitInfo *pInfo, bool bAlways);
	bool Hook_ShouldCollide(int collisonGroup, int contentsMask);
	void Hook_Spawn();
	void Hook_SpawnPost();
	void Hook_StartTouch(CBaseEntity *pOther);
	void Hook_StartTouchPost(CBaseEntity *pOther);
	void Hook_Think();
	void Hook_ThinkPost();
	void Hook_Touch(CBaseEntity *pOther);
	void Hook_TouchPost(CBaseEntity *pOther);
#if SOURCE_ENGINE == SE_ORANGEBOXVALVE || SOURCE_ENGINE == SE_CSS
	void Hook_TraceAttack(CTakeDamageInfoHack &info, const Vector &vecDir, trace_t *ptr, void *pUnknownJK);
	void Hook_TraceAttackPost(CTakeDamageInfoHack &info, const Vector &vecDir, trace_t *ptr, void *pUnknownJK);
#else
	void Hook_TraceAttack(CTakeDamageInfoHack &info, const Vector &vecDir, trace_t *ptr);
	void Hook_TraceAttackPost(CTakeDamageInfoHack &info, const Vector &vecDir, trace_t *ptr);
#endif
	void Hook_UpdateOnRemove();
	void Hook_Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void Hook_UsePost(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void Hook_VPhysicsUpdate(IPhysicsObject *pPhysics);
	void Hook_VPhysicsUpdatePost(IPhysicsObject *pPhysics);
	bool Hook_WeaponCanSwitchTo(CBaseCombatWeapon *pWeapon);
	bool Hook_WeaponCanSwitchToPost(CBaseCombatWeapon *pWeapon);
	bool Hook_WeaponCanUse(CBaseCombatWeapon *pWeapon);
	bool Hook_WeaponCanUsePost(CBaseCombatWeapon *pWeapon);
	void Hook_WeaponDrop(CBaseCombatWeapon *pWeapon, const Vector *pvecTarget, const Vector *pVelocity);
	void Hook_WeaponDropPost(CBaseCombatWeapon *pWeapon, const Vector *pvecTarget, const Vector *pVelocity);
	void Hook_WeaponEquip(CBaseCombatWeapon *pWeapon);
	void Hook_WeaponEquipPost(CBaseCombatWeapon *pWeapon);
	bool Hook_WeaponSwitch(CBaseCombatWeapon *pWeapon, int viewmodelindex);
	bool Hook_WeaponSwitchPost(CBaseCombatWeapon *pWeapon, int viewmodelindex);
	
private:
	void RemoveEntityHooks(CBaseEntity *pEnt);
};

extern CGlobalVars *gpGlobals;
extern CUtlVector<HookList> g_HookList[SDKHook_MAXHOOKS];

extern ICvar *icvar;
#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
