<?php
/**
 * =============================================================================
 * SourcePawn GeSHi Syntax File
 * Copyright (C) 2010 AlliedModders LLC
 * INC parser originally by Zach "theY4Kman" Kanzler,
 *     ported to perl and enhanced by Nicholas "psychonic" Hastings
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
 */

$language_data = array(
	'LANG_NAME' => 'SourcePawn',
	'COMMENT_SINGLE' => array(1 => '//'),
	'COMMENT_MULTI' => array("/*" => "*/"),
	'CASE_KEYWORDS' => GESHI_CAPS_NO_CHANGE,
	'QUOTEMARKS' => array('"','\''),
	'ESCAPE_CHAR' => '\\',
	'ESCAPE_REGEXP' => array(
		1 => "#\\\\x[\da-fA-F]{1,2}#",
		2 => "#\\\\b[01]{1,8}#",
		3 => "#%[%sdif%NLbxXtTc]#",
		),
	'SYMBOLS' => array(
		0 => array(';'),
		// Assignment operators
		1 => array('=', '+=', '-=', '/=', '*=', '&=', '|=', '~=', '^='),
		// Comparison and logical operators
		2 => array('==', '!=', '&&', '||', '>', '<', '<=', '>='),
		// Other operators
		3 => array('+', '-', '*', '/', '|', '&', '~', '++', '--', '^', '%%', '!'),
		),
	'KEYWORDS' => array(
		// Reserved words
		1 => array(
			'for', 'if', 'else', 'do', 'while', 'switch', 'case', 'return',
			'break', 'continue', 'new', 'decl', 'public', 'stock', 'const',
			'enum', 'forward', 'static', 'funcenum', 'functag', 'native',
			'sizeof', 'true', 'false', 'default',
			),
		// Tags
		2 => array(
			'Action', 'bool', 'Float', 'Plugin', 'String', 'any',
			'AdminFlag','OverrideType','OverrideRule','ImmunityType','GroupId','AdminId','AdmAccessMode','AdminCachePart','CookieAccess','CookieMenu','CookieMenuAction','NetFlow','ConVarBounds','QueryCookie','ReplySource','ConVarQueryResult','ConVarQueryFinished','Function','Action','Identity','PluginStatus','PluginInfo','DBResult','DBBindType','DBPriority','PropType','PropFieldType','MoveType','RenderMode','RenderFx','EventHookMode','EventHook','FileType','FileTimeMode','PathType','ParamType','ExecType','DialogType','Handle','KvDataTypes','NominateResult','MapChange','MenuStyle','MenuAction','MenuSource','RegexError','SDKCallType','SDKLibrary','SDKFuncConfSource','SDKType','SDKPassMethod','RayType','TraceEntityFilter','ListenOverride','SortOrder','SortType','SortFunc2D','APLRes','FeatureType','FeatureStatus','SMCResult','SMCError','TFClassType','TFTeam','TFCond','TFResourceType','Timer','TopMenuAction','TopMenuObjectType','TopMenuPosition','TopMenuObject','UserMsg',
			),
		// Natives
		3 => array(
			'DumpAdminCache','AddCommandOverride','GetCommandOverride','UnsetCommandOverride','CreateAdmGroup','FindAdmGroup','SetAdmGroupAddFlag','GetAdmGroupAddFlag','GetAdmGroupAddFlags','SetAdmGroupImmunity','GetAdmGroupImmunity','SetAdmGroupImmuneFrom','GetAdmGroupImmuneCount','GetAdmGroupImmuneFrom','AddAdmGroupCmdOverride','GetAdmGroupCmdOverride','RegisterAuthIdentType','CreateAdmin','GetAdminUsername','BindAdminIdentity','SetAdminFlag','GetAdminFlag','GetAdminFlags','AdminInheritGroup','GetAdminGroupCount','GetAdminGroup','SetAdminPassword','GetAdminPassword','FindAdminByIdentity','RemoveAdmin','FlagBitsToBitArray','FlagBitArrayToBits','FlagArrayToBits','FlagBitsToArray','FindFlagByName','FindFlagByChar','ReadFlagString','CanAdminTarget','CreateAuthMethod','SetAdmGroupImmunityLevel','GetAdmGroupImmunityLevel','SetAdminImmunityLevel','GetAdminImmunityLevel','FlagToBit','BitToFlag','GetAdminTopMenu','AddTargetsToMenu','AddTargetsToMenu2','RedisplayAdminMenu','CreateArray','ClearArray','CloneArray','ResizeArray','GetArraySize','PushArrayCell','PushArrayString','PushArrayArray','GetArrayCell','GetArrayString','GetArrayArray','SetArrayCell','SetArrayString','SetArrayArray','ShiftArrayUp','RemoveFromArray','SwapArrayItems','FindStringInArray','FindValueInArray','ByteCountToCells','CreateStack','PushStackCell','PushStackString','PushStackArray','PopStackCell','PopStackString','PopStackArray','IsStackEmpty','PopStack','CreateTrie','SetTrieValue','SetTrieArray','SetTrieString','GetTrieValue','GetTrieArray','GetTrieString','RemoveFromTrie','ClearTrie','GetTrieSize','BanClient','BanIdentity','RemoveBan','BfWriteBool','BfWriteByte','BfWriteChar','BfWriteShort','BfWriteWord','BfWriteNum','BfWriteFloat','BfWriteString','BfWriteEntity','BfWriteAngle','BfWriteCoord','BfWriteVecCoord','BfWriteVecNormal','BfWriteAngles','BfReadBool','BfReadByte','BfReadChar','BfReadShort','BfReadWord','BfReadNum','BfReadFloat','BfReadString','BfReadEntity','BfReadAngle','BfReadCoord','BfReadVecCoord','BfReadVecNormal','BfReadAngles','BfGetNumBytesLeft','RegClientCookie','FindClientCookie','SetClientCookie','GetClientCookie','AreClientCookiesCached','SetCookiePrefabMenu','SetCookieMenuItem','ShowCookieMenu','GetCookieIterator','ReadCookieIterator','GetCookieAccess','GetClientCookieTime','GetMaxClients','GetClientCount','GetClientName','GetClientIP','GetClientAuthString','GetClientUserId','IsClientConnected','IsClientInGame','IsClientInKickQueue','IsClientAuthorized','IsFakeClient','IsClientObserver','IsPlayerAlive','GetClientInfo','GetClientTeam','SetUserAdmin','GetUserAdmin','AddUserFlags','RemoveUserFlags','SetUserFlagBits','GetUserFlagBits','CanUserTarget','RunAdminCacheChecks','NotifyPostAdminCheck','CreateFakeClient','SetFakeClientConVar','GetClientHealth','GetClientModel','GetClientWeapon','GetClientMaxs','GetClientMins','GetClientAbsAngles','GetClientAbsOrigin','GetClientArmor','GetClientDeaths','GetClientFrags','GetClientDataRate','IsClientTimingOut','GetClientTime','GetClientLatency','GetClientAvgLatency','GetClientAvgLoss','GetClientAvgChoke','GetClientAvgData','GetClientAvgPackets','GetClientOfUserId','KickClient','KickClientEx','ChangeClientTeam','GetClientSerial','GetClientFromSerial','IsPlayerInGame','ProcessTargetString','ReplyToTargetError','ServerCommand','InsertServerCommand','ServerExecute','ClientCommand','FakeClientCommand','FakeClientCommandEx','PrintToServer','PrintToConsole','ReplyToCommand','GetCmdReplySource','SetCmdReplySource','IsChatTrigger','ShowActivity2','ShowActivity','ShowActivityEx','FormatActivitySource','RegServerCmd','RegConsoleCmd','RegAdminCmd','GetCmdArgs','GetCmdArg','GetCmdArgString','CreateConVar','FindConVar','HookConVarChange','UnhookConVarChange','GetConVarBool','SetConVarBool','GetConVarInt','SetConVarInt','GetConVarFloat','SetConVarFloat','GetConVarString','SetConVarString','ResetConVar','GetConVarFlags','SetConVarFlags','GetConVarBounds','SetConVarBounds','GetConVarName','QueryClientConVar','GetCommandIterator','ReadCommandIterator','CheckCommandAccess','GetCommandFlags','SetCommandFlags','FindFirstConCommand','FindNextConCommand','SendConVarValue','AddServerTag','RemoveServerTag','AddCommandListener','RemoveCommandListener','IsValidConVarChar','VerifyCoreVersion','MarkNativeAsOptional','CS_RespawnPlayer','CS_SwitchTeam','CreateDataPack','WritePackCell','WritePackFloat','WritePackString','ReadPackCell','ReadPackFloat','ReadPackString','ResetPack','GetPackPosition','SetPackPosition','IsPackReadable','SQL_Connect','SQL_ConnectCustom','SQL_ConnectEx','SQL_CheckConfig','SQL_GetDriver','SQL_ReadDriver','SQL_GetDriverIdent','SQL_GetDriverProduct','SQL_GetAffectedRows','SQL_GetInsertId','SQL_GetError','SQL_EscapeString','SQL_FastQuery','SQL_Query','SQL_PrepareQuery','SQL_FetchMoreResults','SQL_HasResultSet','SQL_GetRowCount','SQL_GetFieldCount','SQL_FieldNumToName','SQL_FieldNameToNum','SQL_FetchRow','SQL_MoreRows','SQL_Rewind','SQL_FetchString','SQL_FetchFloat','SQL_FetchInt','SQL_IsFieldNull','SQL_FetchSize','SQL_BindParamInt','SQL_BindParamFloat','SQL_BindParamString','SQL_Execute','SQL_LockDatabase','SQL_UnlockDatabase','SQL_IsSameConnection','SQL_TConnect','SQL_TQuery','SQL_DefConnect','SQLite_UseDatabase','SQL_QuoteString','GetMaxEntities','GetEntityCount','IsValidEntity','IsValidEdict','IsEntNetworkable','CreateEdict','RemoveEdict','GetEdictFlags','SetEdictFlags','GetEdictClassname','GetEntityNetClass','ChangeEdictState','GetEntData','SetEntData','GetEntDataFloat','SetEntDataFloat','GetEntDataEnt','SetEntDataEnt','GetEntDataEnt2','SetEntDataEnt2','GetEntDataVector','SetEntDataVector','GetEntDataString','SetEntDataString','FindSendPropOffs','FindSendPropInfo','FindDataMapOffs','GetEntProp','SetEntProp','GetEntPropFloat','SetEntPropFloat','GetEntPropEnt','SetEntPropEnt','GetEntPropVector','SetEntPropVector','GetEntPropString','SetEntPropString','GetEntSendPropOffs','GetEntDataArray','SetEntDataArray','GetEntityFlags','GetEntityMoveType','SetEntityMoveType','GetEntityRenderMode','SetEntityRenderMode','GetEntityRenderFx','SetEntityRenderFx','SetEntityRenderColor','GetEntityGravity','SetEntityGravity','SetEntityHealth','GetClientButtons','HookEvent','HookEventEx','UnhookEvent','CreateEvent','FireEvent','CancelCreatedEvent','GetEventBool','SetEventBool','GetEventInt','SetEventInt','GetEventFloat','SetEventFloat','GetEventString','SetEventString','GetEventName','SetEventBroadcast','BuildPath','OpenDirectory','ReadDirEntry','OpenFile','DeleteFile','ReadFileLine','ReadFile','ReadFileString','WriteFile','WriteFileString','WriteFileLine','IsEndOfFile','FileSeek','FilePosition','FileExists','RenameFile','DirExists','FileSize','FlushFile','RemoveDir','CreateDirectory','GetFileTime','LogToOpenFile','LogToOpenFileEx','ReadFileCell','WriteFileCell','float','FloatMul','FloatDiv','FloatAdd','FloatSub','FloatFraction','RoundToZero','RoundToCeil','RoundToFloor','RoundToNearest','FloatCompare','SquareRoot','Pow','Exponential','Logarithm','Sine','Cosine','Tangent','FloatAbs','ArcTangent','ArcCosine','ArcSine','ArcTangent2','operator-','GetURandomInt','GetURandomFloat','SetURandomSeed','RoundFloat','operator--','operator-','operator-','operator-','DegToRad','RadToDeg','SetURandomSeedSimple','GetFunctionByName','CreateGlobalForward','CreateForward','GetForwardFunctionCount','AddToForward','RemoveFromForward','RemoveAllFromForward','Call_StartForward','Call_StartFunction','Call_PushCell','Call_PushCellRef','Call_PushFloat','Call_PushFloatRef','Call_PushArray','Call_PushArrayEx','Call_PushString','Call_PushStringEx','Call_Finish','Call_Cancel','CreateNative','ThrowNativeError','GetNativeStringLength','GetNativeString','SetNativeString','GetNativeCell','GetNativeCellRef','SetNativeCellRef','parameter','GetNativeArray','SetNativeArray','FormatNativeString','GeoipCode2','GeoipCode3','GeoipCountry','LogToGame','SetRandomSeed','GetRandomFloat','GetRandomInt','IsMapValid','IsDedicatedServer','GetEngineTime','GetGameTime','GetGameDescription','GetGameFolderName','GetCurrentMap','PrecacheModel','PrecacheSentenceFile','PrecacheDecal','PrecacheGeneric','IsModelPrecached','IsDecalPrecached','IsGenericPrecached','PrecacheSound','IsSoundPrecached','CreateDialog','GuessSDKVersion','PrintToChat','PrintCenterText','PrintHintText','ShowVGUIPanel','CreateHudSynchronizer','SetHudTextParams','SetHudTextParamsEx','ShowSyncHudText','ClearSyncHud','ShowHudText','EntIndexToEntRef','EntRefToEntIndex','MakeCompatEntRef','PrintToChatAll','PrintCenterTextAll','PrintHintTextToAll','ShowMOTDPanel','DisplayAskConnectBox','CloseHandle','CloneHandle','IsValidHandle','FormatUserLogText','FindPluginByFile','SearchForClients','FindTarget','LoadMaps','CreateKeyValues','KvSetString','KvSetNum','KvSetUInt64','KvSetFloat','KvSetColor','KvSetVector','KvGetString','KvGetNum','KvGetFloat','KvGetColor','KvGetUInt64','KvGetVector','KvJumpToKey','KvJumpToKeySymbol','KvGotoFirstSubKey','KvGotoNextKey','KvSavePosition','KvDeleteKey','KvDeleteThis','KvGoBack','KvRewind','KvGetSectionName','KvSetSectionName','KvGetDataType','KeyValuesToFile','FileToKeyValues','KvSetEscapeSequences','KvNodesInStack','KvCopySubkeys','KvFindKeyById','KvGetNameSymbol','KvGetSectionSymbol','LoadTranslations','SetGlobalTransTarget','GetClientLanguage','GetServerLanguage','GetLanguageCount','GetLanguageInfo','LogMessage','LogMessageEx','LogToFile','LogToFileEx','LogAction','LogError','AddGameLogHook','RemoveGameLogHook','NominateMap','GetExcludeMapList','CanMapChooserStartVote','InitiateMapChooserVote','HasEndOfMapVoteFinished','EndOfMapVoteEnabled','CreateMenu','DisplayMenu','DisplayMenuAtItem','AddMenuItem','InsertMenuItem','RemoveMenuItem','RemoveAllMenuItems','GetMenuItem','GetMenuSelectionPosition','GetMenuItemCount','SetMenuPagination','GetMenuPagination','GetMenuStyle','SetMenuTitle','GetMenuTitle','CreatePanelFromMenu','GetMenuExitButton','SetMenuExitButton','GetMenuExitBackButton','SetMenuExitBackButton','CancelMenu','GetMenuOptionFlags','SetMenuOptionFlags','IsVoteInProgress','CancelVote','VoteMenu','SetVoteResultCallback','CheckVoteDelay','IsClientInVotePool','RedrawClientVoteMenu','GetMenuStyleHandle','CreatePanel','CreateMenuEx','GetClientMenu','CancelClientMenu','GetMaxPageItems','GetPanelStyle','SetPanelTitle','DrawPanelItem','DrawPanelText','CanPanelDrawFlags','SetPanelKeys','SendPanelToClient','GetPanelTextRemaining','GetPanelCurrentKey','SetPanelCurrentKey','RedrawMenuItem','InternalShowMenu','VoteMenuToAll','GetMenuVoteInfo','IsNewVoteAllowed','SetNextMap','GetNextMap','ForceChangeLevel','GetMapHistorySize','GetMapHistory','CreateProfiler','StartProfiling','StopProfiling','GetProfilerTime','CompileRegex','MatchRegex','GetRegexSubString','SimpleRegexMatch','StartPrepSDKCall','PrepSDKCall_SetVirtual','PrepSDKCall_SetSignature','PrepSDKCall_SetFromConf','PrepSDKCall_SetReturnInfo','PrepSDKCall_AddParameter','EndPrepSDKCall','SDKCall','SetClientViewEntity','SetLightStyle','GetClientEyePosition','AcceptEntityInput','SetVariantBool','SetVariantString','SetVariantInt','SetVariantFloat','SetVariantVector3D','SetVariantPosVector3D','SetVariantColor','SetVariantEntity','HookEntityOutput','UnhookEntityOutput','HookSingleEntityOutput','UnhookSingleEntityOutput','RemovePlayerItem','GivePlayerItem','GetPlayerWeaponSlot','IgniteEntity','ExtinguishEntity','TeleportEntity','ForcePlayerSuicide','SlapPlayer','FindEntityByClassname','GetClientEyeAngles','CreateEntityByName','DispatchSpawn','DispatchKeyValue','DispatchKeyValueFloat','DispatchKeyValueVector','GetClientAimTarget','GetTeamCount','GetTeamName','GetTeamScore','SetTeamScore','GetTeamClientCount','SetEntityModel','GetPlayerDecalFile','GetServerNetStats','EquipPlayerWeapon','ActivateEntity','SetClientInfo','PrefetchSound','GetSoundDuration','EmitAmbientSound','FadeClientVolume','StopSound','EmitSound','EmitSentence','AddAmbientSoundHook','AddNormalSoundHook','RemoveAmbientSoundHook','RemoveNormalSoundHook','EmitSoundToClient','EmitSoundToAll','ATTN_TO_SNDLEVEL','FindTeamByName','FindStringTable','GetNumStringTables','GetStringTableNumStrings','GetStringTableMaxStrings','GetStringTableName','FindStringIndex','ReadStringTable','GetStringTableDataLength','GetStringTableData','SetStringTableData','AddToStringTable','LockStringTables','AddFileToDownloadsTable','AddTempEntHook','RemoveTempEntHook','TE_Start','TE_IsValidProp','TE_WriteNum','TE_ReadNum','TE_WriteFloat','TE_ReadFloat','TE_WriteVector','TE_ReadVector','TE_WriteAngles','TE_WriteFloatArray','TE_Send','TE_WriteEncodedEnt','TE_SendToAll','TE_SendToClient','TE_SetupSparks','TE_SetupSmoke','TE_SetupDust','TE_SetupMuzzleFlash','TE_SetupMetalSparks','TE_SetupEnergySplash','TE_SetupArmorRicochet','TE_SetupGlowSprite','TE_SetupExplosion','TE_SetupBloodSprite','TE_SetupBeamRingPoint','TE_SetupBeamPoints','TE_SetupBeamLaser','TE_SetupBeamRing','TE_SetupBeamFollow','TR_GetPointContents','TR_GetPointContentsEnt','TR_TraceRay','TR_TraceHull','TR_TraceRayFilter','TR_TraceHullFilter','TR_TraceRayEx','TR_TraceHullEx','TR_TraceRayFilterEx','TR_TraceHullFilterEx','TR_GetFraction','TR_GetEndPosition','TR_GetEntityIndex','TR_DidHit','TR_GetHitGroup','TR_GetPlaneNormal','TR_PointOutsideWorld','SetClientListeningFlags','GetClientListeningFlags','SetClientListening','GetClientListening','SetListenOverride','GetListenOverride','IsClientMuted','SortIntegers','SortFloats','SortStrings','SortCustom1D','SortCustom2D','SortADTArray','SortADTArrayCustom','GetMyHandle','GetPluginIterator','MorePlugins','ReadPlugin','GetPluginStatus','GetPluginFilename','IsPluginDebugging','GetPluginInfo','FindPluginByNumber','SetFailState','ThrowError','GetTime','FormatTime','LoadGameConfigFile','GameConfGetOffset','GameConfGetKeyValue','GetSysTickCount','AutoExecConfig','RegPluginLibrary','LibraryExists','GetExtensionFileStatus','ReadMapList','SetMapListCompatBind','GetFeatureStatus','RequireFeature','CanTestFeatures','strlen','StrContains','strcmp','strncmp','strcopy','Format','FormatEx','VFormat','StringToInt','StringToIntEx','IntToString','StringToFloat','StringToFloatEx','FloatToString','BreakString','TrimString','SplitString','ReplaceString','ReplaceStringEx','GetCharBytes','IsCharAlpha','IsCharNumeric','IsCharSpace','IsCharMB','IsCharUpper','IsCharLower','StripQuotes','StrCompare','StrEqual','StrCopy','StrBreak','CharToUpper','CharToLower','FindCharInString','StrCat','ExplodeString','ImplodeStrings','SMC_CreateParser','SMC_ParseFile','SMC_GetErrorString','SMC_SetParseStart','SMC_SetParseEnd','SMC_SetReaders','SMC_SetRawLine','TF2_IgnitePlayer','TF2_RespawnPlayer','TF2_RegeneratePlayer','TF2_AddCondition','TF2_RemoveCondition','TF2_SetPlayerPowerPlay','TF2_DisguisePlayer','TF2_RemovePlayerDisguise','TF2_StunPlayer','TF2_GetResourceEntity','TF2_GetClass','TF2_GetPlayerClass','TF2_SetPlayerClass','TF2_GetPlayerResourceData','TF2_SetPlayerResourceData','TF2_RemoveWeaponSlot','TF2_RemoveAllWeapons','TF2_GetPlayerConditionFlags','CreateTimer','KillTimer','TriggerTimer','GetTickedTime','GetMapTimeLeft','GetMapTimeLimit','ExtendMapTimeLimit','GetTickInterval','IsServerProcessing','CreateDataTimer','CreateTopMenu','LoadTopMenuConfig','AddToTopMenu','GetTopMenuInfoString','GetTopMenuObjName','RemoveFromTopMenu','DisplayTopMenu','FindTopMenuCategory','GetUserMessageId','GetUserMessageName','StartMessage','StartMessageEx','EndMessage','HookUserMessage','UnhookUserMessage','StartMessageAll','StartMessageOne','GetVectorLength','GetVectorDistance','GetVectorDotProduct','GetVectorCrossProduct','NormalizeVector','GetAngleVectors','GetVectorAngles','GetVectorVectors','AddVectors','SubtractVectors','ScaleVector','NegateVector','MakeVectorFromPoints',
			),
		// Forwards
		4 => array(
			'OnRebuildAdminCache','OnAdminMenuCreated','OnAdminMenuReady','OnBanClient','OnBanIdentity','OnRemoveBan','OnClientCookiesCached','OnClientConnect','OnClientConnected','OnClientPutInServer','OnClientDisconnect','OnClientDisconnect_Post','OnClientCommand','OnClientSettingsChanged','OnClientAuthorized','OnClientPreAdminCheck','OnClientPostAdminFilter','OnClientPostAdminCheck','OnLogAction','OnNominationRemoved','OnPlayerRunCmd','OnPluginStart','AskPluginLoad','AskPluginLoad2','OnPluginEnd','OnPluginPauseChange','OnGameFrame','OnMapStart','OnMapEnd','OnConfigsExecuted','OnAutoConfigsBuffered','OnServerCfg','OnAllPluginsLoaded','OnLibraryAdded','OnLibraryRemoved','OnClientFloodCheck','OnClientFloodResult','TF2_CalcIsAttackCritical','OnMapTimeLeftChanged',
			),
		// Defines
		5 => array(
			'MaxClients',
			'AdminFlags_TOTAL','ADMFLAG_RESERVATION','ADMFLAG_GENERIC','ADMFLAG_KICK','ADMFLAG_BAN','ADMFLAG_UNBAN','ADMFLAG_SLAY','ADMFLAG_CHANGEMAP','ADMFLAG_CONVARS','ADMFLAG_CONFIG','ADMFLAG_CHAT','ADMFLAG_VOTE','ADMFLAG_PASSWORD','ADMFLAG_RCON','ADMFLAG_CHEATS','ADMFLAG_ROOT','ADMFLAG_CUSTOM1','ADMFLAG_CUSTOM2','ADMFLAG_CUSTOM3','ADMFLAG_CUSTOM4','ADMFLAG_CUSTOM5','ADMFLAG_CUSTOM6','AUTHMETHOD_STEAM','AUTHMETHOD_IP','AUTHMETHOD_NAME','Admin_Reservation','Admin_Generic','Admin_Kick','Admin_Ban','Admin_Unban','Admin_Slay','Admin_Changemap','Admin_Convars','Admin_Config','Admin_Chat','Admin_Vote','Admin_Password','Admin_RCON','Admin_Cheats','Admin_Root','Admin_Custom1','Admin_Custom2','Admin_Custom3','Admin_Custom4','Admin_Custom5','Admin_Custom6','Override_Command','Override_CommandGroup','Command_Deny','Command_Allow','Immunity_Default','Immunity_Global','INVALID_GROUP_ID','INVALID_ADMIN_ID','Access_Real','Access_Effective','AdminCache_Overrides','AdminCache_Groups','AdminCache_Admins','TEMP_REQUIRE_EXTENSIONS','REQUIRE_EXTENSIONS','ADMINMENU_PLAYERCOMMANDS','ADMINMENU_SERVERCOMMANDS','ADMINMENU_VOTINGCOMMANDS','BANFLAG_AUTO','BANFLAG_IP','BANFLAG_AUTHID','BANFLAG_NOKICK','CookieAccess_Public','CookieAccess_Protected','CookieAccess_Private','CookieMenu_YesNo','CookieMenu_YesNo_Int','CookieMenu_OnOff','CookieMenu_OnOff_Int','CookieMenuAction_DisplayOption','CookieMenuAction_SelectOption','MAXPLAYERS','MAX_NAME_LENGTH','NetFlow_Outgoing','NetFlow_Incoming','NetFlow_Both','MAX_TARGET_LENGTH','COMMAND_FILTER_ALIVE','COMMAND_FILTER_DEAD','COMMAND_FILTER_CONNECTED','COMMAND_FILTER_NO_IMMUNITY','COMMAND_FILTER_NO_MULTI','COMMAND_FILTER_NO_BOTS','COMMAND_TARGET_NONE','COMMAND_TARGET_NOT_ALIVE','COMMAND_TARGET_NOT_DEAD','COMMAND_TARGET_NOT_IN_GAME','COMMAND_TARGET_IMMUNE','COMMAND_TARGET_EMPTY_FILTER','COMMAND_TARGET_NOT_HUMAN','COMMAND_TARGET_AMBIGUOUS','INVALID_FCVAR_FLAGS','FCVAR_NONE','FCVAR_UNREGISTERED','FCVAR_LAUNCHER','FCVAR_GAMEDLL','FCVAR_CLIENTDLL','FCVAR_MATERIAL_SYSTEM','FCVAR_PROTECTED','FCVAR_SPONLY','FCVAR_ARCHIVE','FCVAR_NOTIFY','FCVAR_USERINFO','FCVAR_PRINTABLEONLY','FCVAR_UNLOGGED','FCVAR_NEVER_AS_STRING','FCVAR_REPLICATED','FCVAR_CHEAT','FCVAR_STUDIORENDER','FCVAR_DEMO','FCVAR_DONTRECORD','FCVAR_PLUGIN','FCVAR_DATACACHE','FCVAR_TOOLSYSTEM','FCVAR_FILESYSTEM','FCVAR_NOT_CONNECTED','FCVAR_SOUNDSYSTEM','FCVAR_ARCHIVE_XBOX','FCVAR_INPUTSYSTEM','FCVAR_NETWORKSYSTEM','FCVAR_VPHYSICS','FEATURECAP_COMMANDLISTENER','ConVarBound_Upper','ConVarBound_Lower','QUERYCOOKIE_FAILED','SM_REPLY_TO_CONSOLE','SM_REPLY_TO_CHAT','ConVarQuery_Okay','ConVarQuery_NotFound','ConVarQuery_NotValid','ConVarQuery_Protected','SOURCEMOD_PLUGINAPI_VERSION','AUTOLOAD_EXTENSIONS','REQUIRE_EXTENSIONS','REQUIRE_PLUGIN','INVALID_FUNCTION','Plugin_Continue','Plugin_Changed','Plugin_Handled','Plugin_Stop','Identity_Core','Identity_Extension','Identity_Plugin','Plugin_Running','Plugin_Paused','Plugin_Error','Plugin_Loaded','Plugin_Failed','Plugin_Created','Plugin_Uncompiled','Plugin_BadLoad','PlInfo_Name','PlInfo_Author','PlInfo_Description','PlInfo_Version','PlInfo_URL','CS_TEAM_NONE','CS_TEAM_SPECTATOR','CS_TEAM_T','CS_TEAM_CT','CS_SLOT_PRIMARY','CS_SLOT_SECONDARY','CS_SLOT_GRENADE','CS_SLOT_C4','DBVal_Error','DBVal_TypeMismatch','DBVal_Null','DBVal_Data','DBBind_Int','DBBind_Float','DBBind_String','DBPrio_High','DBPrio_Normal','DBPrio_Low','FL_EDICT_CHANGED','FL_EDICT_FREE','FL_EDICT_FULL','FL_EDICT_FULLCHECK','FL_EDICT_ALWAYS','FL_EDICT_DONTSEND','FL_EDICT_PVSCHECK','FL_EDICT_PENDING_DORMANT_CHECK','FL_EDICT_DIRTY_PVS_INFORMATION','FL_FULL_EDICT_CHANGED','Prop_Send','Prop_Data','PropField_Unsupported','PropField_Integer','PropField_Float','PropField_Entity','PropField_Vector','PropField_String','PropField_String_T','IN_ATTACK','IN_JUMP','IN_DUCK','IN_FORWARD','IN_BACK','IN_USE','IN_CANCEL','IN_LEFT','IN_RIGHT','IN_MOVELEFT','IN_MOVERIGHT','IN_ATTACK2','IN_RUN','IN_RELOAD','IN_ALT1','IN_ALT2','IN_SCORE','IN_SPEED','IN_WALK','IN_ZOOM','IN_WEAPON1','IN_WEAPON2','IN_BULLRUSH','IN_GRENADE1','IN_GRENADE2','FL_ONGROUND','FL_DUCKING','FL_WATERJUMP','FL_ONTRAIN','FL_INRAIN','FL_FROZEN','FL_ATCONTROLS','FL_CLIENT','FL_FAKECLIENT','PLAYER_FLAG_BITS','FL_INWATER','FL_FLY','FL_SWIM','FL_CONVEYOR','FL_NPC','FL_GODMODE','FL_NOTARGET','FL_AIMTARGET','FL_PARTIALGROUND','FL_STATICPROP','FL_GRAPHED','FL_GRENADE','FL_STEPMOVEMENT','FL_DONTTOUCH','FL_BASEVELOCITY','FL_WORLDBRUSH','FL_OBJECT','FL_KILLME','FL_ONFIRE','FL_DISSOLVING','FL_TRANSRAGDOLL','FL_UNBLOCKABLE_BY_PLAYER','MOVETYPE_NONE','MOVETYPE_ISOMETRIC','MOVETYPE_WALK','MOVETYPE_STEP','MOVETYPE_FLY','MOVETYPE_FLYGRAVITY','MOVETYPE_VPHYSICS','MOVETYPE_PUSH','MOVETYPE_NOCLIP','MOVETYPE_LADDER','MOVETYPE_OBSERVER','MOVETYPE_CUSTOM','RENDER_NORMAL','RENDER_TRANSCOLOR','RENDER_TRANSTEXTURE','RENDER_GLOW','RENDER_TRANSALPHA','RENDER_TRANSADD','RENDER_ENVIRONMENTAL','RENDER_TRANSADDFRAMEBLEND','RENDER_TRANSALPHAADD','RENDER_WORLDGLOW','RENDER_NONE','RENDERFX_NONE','RENDERFX_PULSE_SLOW','RENDERFX_PULSE_FAST','RENDERFX_PULSE_SLOW_WIDE','RENDERFX_PULSE_FAST_WIDE','RENDERFX_FADE_SLOW','RENDERFX_FADE_FAST','RENDERFX_SOLID_SLOW','RENDERFX_SOLID_FAST','RENDERFX_STROBE_SLOW','RENDERFX_STROBE_FAST','RENDERFX_STROBE_FASTER','RENDERFX_FLICKER_SLOW','RENDERFX_FLICKER_FAST','RENDERFX_NO_DISSIPATION','RENDERFX_DISTORT','RENDERFX_HOLOGRAM','RENDERFX_EXPLODE','RENDERFX_GLOWSHELL','RENDERFX_CLAMP_MIN_SCALE','RENDERFX_ENV_RAIN','RENDERFX_ENV_SNOW','RENDERFX_SPOTLIGHT','RENDERFX_RAGDOLL','RENDERFX_PULSE_FAST_WIDER','RENDERFX_MAX','EventHookMode_Pre','EventHookMode_Post','EventHookMode_PostNoCopy','PLATFORM_MAX_PATH','SEEK_SET','SEEK_CUR','SEEK_END','FPERM_U_READ','FPERM_U_WRITE','FPERM_U_EXEC','FPERM_G_READ','FPERM_G_WRITE','FPERM_G_EXEC','FPERM_O_READ','FPERM_O_WRITE','FPERM_O_EXEC','FileType_Unknown','FileType_Directory','FileType_File','FileTime_LastAccess','FileTime_Created','FileTime_LastChange','Path_SM','FLOAT_PI','SP_PARAMFLAG_BYREF','SM_PARAM_COPYBACK','SM_PARAM_STRING_UTF8','SM_PARAM_STRING_COPY','SM_PARAM_STRING_BINARY','SP_ERROR_NONE','SP_ERROR_FILE_FORMAT','SP_ERROR_DECOMPRESSOR','SP_ERROR_HEAPLOW','SP_ERROR_PARAM','SP_ERROR_INVALID_ADDRESS','SP_ERROR_NOT_FOUND','SP_ERROR_INDEX','SP_ERROR_STACKLOW','SP_ERROR_NOTDEBUGGING','SP_ERROR_INVALID_INSTRUCTION','SP_ERROR_MEMACCESS','SP_ERROR_STACKMIN','SP_ERROR_HEAPMIN','SP_ERROR_DIVIDE_BY_ZERO','SP_ERROR_ARRAY_BOUNDS','SP_ERROR_INSTRUCTION_PARAM','SP_ERROR_STACKLEAK','SP_ERROR_HEAPLEAK','SP_ERROR_ARRAY_TOO_BIG','SP_ERROR_TRACKER_BOUNDS','SP_ERROR_INVALID_NATIVE','SP_ERROR_PARAMS_MAX','SP_ERROR_NATIVE','SP_ERROR_NOT_RUNNABLE','SP_ERROR_ABORTED','Param_Any','Param_Cell','Param_Float','Param_String','Param_Array','Param_VarArgs','Param_CellByRef','Param_FloatByRef','ET_Ignore','ET_Single','ET_Event','ET_Hook','SOURCE_SDK_UNKNOWN','SOURCE_SDK_ORIGINAL','SOURCE_SDK_DARKMESSIAH','SOURCE_SDK_EPISODE1','SOURCE_SDK_EPISODE2','SOURCE_SDK_EPISODE2VALVE','SOURCE_SDK_LEFT4DEAD','SOURCE_SDK_LEFT4DEAD2','MOTDPANEL_TYPE_TEXT','MOTDPANEL_TYPE_INDEX','MOTDPANEL_TYPE_URL','MOTDPANEL_TYPE_FILE','INVALID_ENT_REFERENCE','DialogType_Msg','DialogType_Menu','DialogType_Text','DialogType_Entry','DialogType_AskConnect','INVALID_HANDLE','KvData_None','KvData_String','KvData_Int','KvData_Float','KvData_Ptr','KvData_WString','KvData_Color','KvData_UInt64','KvData_NUMTYPES','LANG_SERVER','Nominate_Added','Nominate_Replaced','Nominate_AlreadyInVote','Nominate_InvalidMap','Nominate_VoteFull','MapChange_Instant','MapChange_RoundEnd','MapChange_MapEnd','MENU_ACTIONS_DEFAULT','MENU_ACTIONS_ALL','MENU_NO_PAGINATION','MENU_TIME_FOREVER','ITEMDRAW_DEFAULT','ITEMDRAW_DISABLED','ITEMDRAW_RAWLINE','ITEMDRAW_NOTEXT','ITEMDRAW_SPACER','ITEMDRAW_IGNORE','ITEMDRAW_CONTROL','MENUFLAG_BUTTON_EXIT','MENUFLAG_BUTTON_EXITBACK','MENUFLAG_NO_SOUND','VOTEINFO_CLIENT_INDEX','VOTEINFO_CLIENT_ITEM','VOTEINFO_ITEM_INDEX','VOTEINFO_ITEM_VOTES','VOTEFLAG_NO_REVOTES','MenuStyle_Default','MenuStyle_Valve','MenuStyle_Radio','MenuAction_Start','MenuAction_Display','MenuAction_Select','MenuAction_Cancel','MenuAction_End','MenuAction_VoteEnd','MenuAction_VoteStart','MenuAction_VoteCancel','MenuAction_DrawItem','MenuAction_DisplayItem','MenuCancel_Disconnected','MenuCancel_Interrupted','MenuCancel_Exit','MenuCancel_NoDisplay','MenuCancel_Timeout','MenuCancel_ExitBack','VoteCancel_Generic','VoteCancel_NoVotes','MenuEnd_Selected','MenuEnd_VotingDone','MenuEnd_VotingCancelled','MenuEnd_Cancelled','MenuEnd_Exit','MenuEnd_ExitBack','MenuSource_None','MenuSource_External','MenuSource_Normal','MenuSource_RawPanel','PCRE_CASELESS','PCRE_MULTILINE','PCRE_DOTALL','PCRE_EXTENDED','PCRE_UNGREEDY','PCRE_UTF8','PCRE_NO_UTF8_CHECK','REGEX_ERROR_NONE','REGEX_ERROR_NOMATCH','REGEX_ERROR_NULL','REGEX_ERROR_BADOPTION','REGEX_ERROR_BADMAGIC','REGEX_ERROR_UNKNOWN_OPCODE','REGEX_ERROR_NOMEMORY','REGEX_ERROR_NOSUBSTRING','REGEX_ERROR_MATCHLIMIT','REGEX_ERROR_CALLOUT','REGEX_ERROR_BADUTF8','REGEX_ERROR_BADUTF8_OFFSET','REGEX_ERROR_PARTIAL','REGEX_ERROR_BADPARTIAL','REGEX_ERROR_INTERNAL','REGEX_ERROR_BADCOUNT','REGEX_ERROR_DFA_UITEM','REGEX_ERROR_DFA_UCOND','REGEX_ERROR_DFA_UMLIMIT','REGEX_ERROR_DFA_WSSIZE','REGEX_ERROR_DFA_RECURSE','REGEX_ERROR_RECURSIONLIMIT','REGEX_ERROR_NULLWSLIMIT','REGEX_ERROR_BADNEWLINE','VDECODE_FLAG_ALLOWNULL','VDECODE_FLAG_ALLOWNOTINGAME','VDECODE_FLAG_ALLOWWORLD','VDECODE_FLAG_BYREF','VENCODE_FLAG_COPYBACK','SDKCall_Static','SDKCall_Entity','SDKCall_Player','SDKCall_GameRules','SDKCall_EntityList','SDKLibrary_Server','SDKLibrary_Engine','SDKConf_Virtual','SDKConf_Signature','SDKType_CBaseEntity','SDKType_CBasePlayer','SDKType_Vector','SDKType_QAngle','SDKType_PlainOldData','SDKType_Float','SDKType_Edict','SDKType_String','SDKType_Bool','SDKPass_Pointer','SDKPass_Plain','SDKPass_ByValue','SDKPass_ByRef','MAX_LIGHTSTYLES','SOUND_FROM_PLAYER','SOUND_FROM_LOCAL_PLAYER','SOUND_FROM_WORLD','SNDVOL_NORMAL','SNDPITCH_NORMAL','SNDPITCH_LOW','SNDPITCH_HIGH','SNDATTN_NONE','SNDATTN_NORMAL','SNDATTN_STATIC','SNDATTN_RICOCHET','SNDATTN_IDLE','SNDCHAN_REPLACE','SNDCHAN_AUTO','SNDCHAN_WEAPON','SNDCHAN_VOICE','SNDCHAN_ITEM','SNDCHAN_BODY','SNDCHAN_STREAM','SNDCHAN_STATIC','SNDCHAN_VOICE_BASE','SNDCHAN_USER_BASE','SND_NOFLAGS','SND_CHANGEVOL','SND_CHANGEPITCH','SND_STOP','SND_SPAWNING','SND_DELAY','SND_STOPLOOPING','SND_SPEAKER','SND_SHOULDPAUSE','SNDLEVEL_NONE','SNDLEVEL_RUSTLE','SNDLEVEL_WHISPER','SNDLEVEL_LIBRARY','SNDLEVEL_FRIDGE','SNDLEVEL_HOME','SNDLEVEL_CONVO','SNDLEVEL_DRYER','SNDLEVEL_DISHWASHER','SNDLEVEL_CAR','SNDLEVEL_NORMAL','SNDLEVEL_TRAFFIC','SNDLEVEL_MINIBIKE','SNDLEVEL_SCREAMING','SNDLEVEL_TRAIN','SNDLEVEL_HELICOPTER','SNDLEVEL_SNOWMOBILE','SNDLEVEL_AIRCRAFT','SNDLEVEL_RAIDSIREN','SNDLEVEL_GUNFIRE','SNDLEVEL_ROCKET','INVALID_STRING_TABLE','INVALID_STRING_INDEX','TE_EXPLFLAG_NONE','TE_EXPLFLAG_NOADDITIVE','TE_EXPLFLAG_NODLIGHTS','TE_EXPLFLAG_NOSOUND','TE_EXPLFLAG_NOPARTICLES','TE_EXPLFLAG_DRAWALPHA','TE_EXPLFLAG_ROTATE','TE_EXPLFLAG_NOFIREBALL','TE_EXPLFLAG_NOFIREBALLSMOKE','FBEAM_STARTENTITY','FBEAM_ENDENTITY','FBEAM_FADEIN','FBEAM_FADEOUT','FBEAM_SINENOISE','FBEAM_SOLID','FBEAM_SHADEIN','FBEAM_SHADEOUT','FBEAM_ONLYNOISEONCE','FBEAM_NOTILE','FBEAM_USE_HITBOXES','FBEAM_STARTVISIBLE','FBEAM_ENDVISIBLE','FBEAM_ISACTIVE','FBEAM_FOREVER','FBEAM_HALOBEAM','CONTENTS_EMPTY','CONTENTS_SOLID','CONTENTS_WINDOW','CONTENTS_AUX','CONTENTS_GRATE','CONTENTS_SLIME','CONTENTS_WATER','CONTENTS_MIST','CONTENTS_OPAQUE','LAST_VISIBLE_CONTENTS','ALL_VISIBLE_CONTENTS','CONTENTS_TESTFOGVOLUME','CONTENTS_UNUSED5','CONTENTS_UNUSED6','CONTENTS_TEAM1','CONTENTS_TEAM2','CONTENTS_IGNORE_NODRAW_OPAQUE','CONTENTS_MOVEABLE','CONTENTS_AREAPORTAL','CONTENTS_PLAYERCLIP','CONTENTS_MONSTERCLIP','CONTENTS_CURRENT_0','CONTENTS_CURRENT_90','CONTENTS_CURRENT_180','CONTENTS_CURRENT_270','CONTENTS_CURRENT_UP','CONTENTS_CURRENT_DOWN','CONTENTS_ORIGIN','CONTENTS_MONSTER','CONTENTS_DEBRIS','CONTENTS_DETAIL','CONTENTS_TRANSLUCENT','CONTENTS_LADDER','CONTENTS_HITBOX','MASK_ALL','MASK_SOLID','MASK_PLAYERSOLID','MASK_NPCSOLID','MASK_WATER','MASK_OPAQUE','MASK_OPAQUE_AND_NPCS','MASK_VISIBLE','MASK_VISIBLE_AND_NPCS','MASK_SHOT','MASK_SHOT_HULL','MASK_SHOT_PORTAL','MASK_SOLID_BRUSHONLY','MASK_PLAYERSOLID_BRUSHONLY','MASK_NPCSOLID_BRUSHONLY','MASK_NPCWORLDSTATIC','MASK_SPLITAREAPORTAL','RayType_EndPoint','RayType_Infinite','VOICE_NORMAL','VOICE_MUTED','VOICE_SPEAKALL','VOICE_LISTENALL','VOICE_TEAM','VOICE_LISTENTEAM','Listen_Default','Listen_No','Listen_Yes','Sort_Ascending','Sort_Descending','Sort_Random','Sort_Integer','Sort_Float','Sort_String','MAPLIST_FLAG_MAPSFOLDER','MAPLIST_FLAG_CLEARARRAY','MAPLIST_FLAG_NO_DEFAULT','APLRes_Success','APLRes_Failure','APLRes_SilentFailure','FeatureType_Native','FeatureType_Capability','FeatureStatus_Available','FeatureStatus_Unavailable','FeatureStatus_Unknown','SMCParse_Continue','SMCParse_Halt','SMCParse_HaltFail','SMCError_Okay','SMCError_StreamOpen','SMCError_StreamError','SMCError_Custom','SMCError_InvalidSection1','SMCError_InvalidSection2','SMCError_InvalidSection3','SMCError_InvalidSection4','SMCError_InvalidSection5','SMCError_InvalidTokens','SMCError_TokenOverflow','SMCError_InvalidProperty1','TF_STUNFLAG_SLOWDOWN','TF_STUNFLAG_BONKSTUCK','TF_STUNFLAG_LIMITMOVEMENT','TF_STUNFLAG_CHEERSOUND','TF_STUNFLAG_NOSOUNDOREFFECT','TF_STUNFLAG_THIRDPERSON','TF_STUNFLAG_GHOSTEFFECT','TF_STUNFLAGS_LOSERSTATE','TF_STUNFLAGS_GHOSTSCARE','TF_STUNFLAGS_SMALLBONK','TF_STUNFLAGS_NORMALBONK','TF_STUNFLAGS_BIGBONK','TFClass_Unknown','TFClass_Scout','TFClass_Sniper','TFClass_Soldier','TFClass_DemoMan','TFClass_Medic','TFClass_Heavy','TFClass_Pyro','TFClass_Spy','TFClass_Engineer','TFTeam_Unassigned','TFTeam_Spectator','TFTeam_Red','TFTeam_Blue','TFCond_Slowed','TFCond_Zoomed','TFCond_Disguising','TFCond_Disguised','TFCond_Cloaked','TFCond_Ubercharged','TFCond_TeleportedGlow','TFCond_Taunting','TFCond_UberchargeFading','TFCond_Unknown1','TFCond_Teleporting','TFCond_Kritzkrieged','TFCond_Unknown2','TFCond_DeadRingered','TFCond_Bonked','TFCond_Dazed','TFCond_Buffed','TFCond_Charging','TFCond_DemoBuff','TFCond_Healing','TFCond_OnFire','TFCond_Overhealed','TFCond_Jarated','TF_CONDFLAG_NONE','TF_CONDFLAG_SLOWED','TF_CONDFLAG_ZOOMED','TF_CONDFLAG_DISGUISING','TF_CONDFLAG_DISGUISED','TF_CONDFLAG_CLOAKED','TF_CONDFLAG_UBERCHARGED','TF_CONDFLAG_TELEPORTGLOW','TF_CONDFLAG_TAUNTING','TF_CONDFLAG_UBERCHARGEFADE','TF_CONDFLAG_TELEPORTING','TF_CONDFLAG_KRITZKRIEGED','TF_CONDFLAG_DEADRINGERED','TF_CONDFLAG_BONKED','TF_CONDFLAG_DAZED','TF_CONDFLAG_BUFFED','TF_CONDFLAG_CHARGING','TF_CONDFLAG_DEMOBUFF','TF_CONDFLAG_HEALING','TF_CONDFLAG_ONFIRE','TF_CONDFLAG_OVERHEALED','TF_CONDFLAG_JARATED','TF_DEATHFLAG_KILLERDOMINATION','TF_DEATHFLAG_ASSISTERDOMINATION','TF_DEATHFLAG_KILLERREVENGE','TF_DEATHFLAG_ASSISTERREVENGE','TF_DEATHFLAG_FIRSTBLOOD','TF_DEATHFLAG_DEADRINGER','TFResource_Ping','TFResource_Score','TFResource_Deaths','TFResource_TotalScore','TFResource_Captures','TFResource_Defenses','TFResource_Dominations','TFResource_Revenge','TFResource_BuildingsDestroyed','TFResource_Headshots','TFResource_Backstabs','TFResource_HealPoints','TFResource_Invulns','TFResource_Teleports','TFResource_ResupplyPoints','TFResource_KillAssists','TFResource_MaxHealth','TFResource_PlayerClass','TIMER_REPEAT','TIMER_FLAG_NO_MAPCHANGE','TIMER_HNDL_CLOSE','TIMER_DATA_HNDL_CLOSE','TopMenuAction_DisplayOption','TopMenuAction_DisplayTitle','TopMenuAction_SelectOption','TopMenuAction_DrawOption','TopMenuAction_RemoveObject','TopMenuObject_Category','TopMenuObject_Item','TopMenuPosition_Start','TopMenuPosition_LastRoot','TopMenuPosition_LastCategory','INVALID_TOPMENUOBJECT','USERMSG_RELIABLE','USERMSG_INITMSG','USERMSG_BLOCKHOOKS','INVALID_MESSAGE_ID','SOURCEMOD_V_MAJOR','SOURCEMOD_V_MINOR','SOURCEMOD_V_RELEASE','SOURCEMOD_VERSION',
			),
		),
	'NUMBERS' => array(
		GESHI_NUMBER_INT_BASIC | GESHI_NUMBER_FLT_NONSCI | GESHI_NUMBER_BIN_PREFIX_0B | GESHI_NUMBER_HEX_PREFIX
		),
	'TAB_WIDTH' => 4,
	'CASE_SENSITIVE' => array(
		1 => true,
		2 => true,
		3 => true,
		4 => true,
		5 => true
		),
	'REGEXPS' => array(
		0 => array(
			GESHI_SEARCH => '(#include\s+)(&lt;\w+&gt;)',
			GESHI_REPLACE => '\\2',
			GESHI_BEFORE => '\\1',
			),
		1 => array(
			GESHI_SEARCH => '(#\w+)(\s+)',
			GESHI_REPLACE => '\\1',
			GESHI_AFTER => '\\2'		
			),
		),
	'STYLES' => array(
        'KEYWORDS' => array(
            1 => 'color: #0000EE; font-weight: bold;',
            2 => 'color: #218087; font-weight: bold;',
            3 => 'color: #000040;',
            4 => 'color: #000040;',
            5 => 'color: #8000FF;',
            ),
        'COMMENTS' => array(
            1 => 'color: #006600; font-style: italic;',
            'MULTI' => 'color: #006600; font-style: italic;',
            ),
        'ESCAPE_CHAR' => array(
            0 => 'color: #ff00ff;',
			1 => 'color: #ff00ff;',
			2 => 'color: #ff00ff;',
			3 => 'color: #ff00ff;',
            ),
        'SYMBOLS' => array(
            0 => 'color: #1B5B00; font-weight: bold;',
            1 => 'color: #1B5B00;',
            2 => 'color: #1B5B00;',
            3 => 'color: #1B5B00;',
            ),
        'STRINGS' => array(
            0 => 'color: #B90000;',
            ),
        'BRACKETS' => array(
            0 => 'color: #1B5B00; font-weight: bold;',
            ),
        'NUMBERS' => array(
            0 => 'color: #AE5700;',
            ),
		'REGEXPS' => array(
			0 => 'color: #B90000;',
			1 => 'color: #0000aa;'
			)
        ),
);

?>
