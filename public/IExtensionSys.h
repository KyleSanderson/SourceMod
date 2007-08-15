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

#ifndef _INCLUDE_SOURCEMOD_MODULE_INTERFACE_H_
#define _INCLUDE_SOURCEMOD_MODULE_INTERFACE_H_

#include <IShareSys.h>
#include <ILibrarySys.h>

/**
 * @file IExtensionSys.h
 * @brief Defines the interface for loading/unloading/managing extensions.
 */

namespace SourceMod
{
	class IExtensionInterface;
	typedef void *		ITERATOR;		/**< Generic pointer for dependency iterators */

	/** 
	 * @brief Encapsulates an IExtensionInterface and its dependencies.
	 */
	class IExtension
	{
	public:
		/**
		 * @brief Returns whether or not the extension is properly loaded.
		 */
		virtual bool IsLoaded() =0;

		/**
		 * @brief Returns the extension's API interface
		 *
		 * @return			An IExtensionInterface pointer.
		 */
		virtual IExtensionInterface *GetAPI() =0;

		/**
		 * @brief Returns the filename of the extension, relative to the
		 * extension folder.
		 *
		 * @return			A string containing the extension file name.
		 */
		virtual const char *GetFilename() =0;

		/**
		 * @brief Returns the extension's identity token.
		 *
		 * @return			An IdentityToken_t pointer.
		 */
		virtual IdentityToken_t *GetIdentity() =0;

		/**
		 * @brief Retrieves the extension dependency list for this extension.
		 *
		 * @param pOwner		Optional pointer to store the first interface's owner.
		 * @param pInterface	Optional pointer to store the first interface.
		 * @return				An ITERATOR pointer for the results, or NULL if no results at all.
		 */
		virtual ITERATOR *FindFirstDependency(IExtension **pOwner, SMInterface **pInterface) =0;

		/**
		 * @brief Finds the next dependency in the dependency list.
		 *
		 * @param iter			Pointer to iterator from FindFirstDependency.
		 * @param pOwner		Optional pointer to store the interface's owner.
		 * @param pInterface	Optional pointer to store the interface.
		 * @return				True if there are more results after this, false otherwise.
		 */
		virtual bool FindNextDependency(ITERATOR *iter, IExtension **pOwner, SMInterface **pInterface) =0;

		/**
		 * @brief Frees an ITERATOR handle from FindFirstDependency.
		 *
		 * @param iter			Pointer to iterator to free.
		 */
		virtual void FreeDependencyIterator(ITERATOR *iter) =0;

		/**
		 * @brief Queries the extension to see its run state.
		 *
		 * @param error			Error buffer (may be NULL).
		 * @param maxlength		Maximum length of buffer.
		 * @return				True if extension is okay, false if not okay.
		 */
		virtual bool IsRunning(char *error, size_t maxlength) =0;
	};

	/**
	 * @brief Version code of the IExtensionInterface API itself.
	 */
	#define SMINTERFACE_EXTENSIONAPI_VERSION	1

	/**
	 * @brief The interface an extension must expose.
	 */
	class IExtensionInterface
	{
	public:
		/** Returns the interface API version */
		virtual unsigned int GetExtensionVersion()
		{
			return SMINTERFACE_EXTENSIONAPI_VERSION;
		}
	public:
		/**
		 * @brief Called when the extension is loaded.
		 * 
		 * @param me		Pointer back to extension.
		 * @param sys		Pointer to interface sharing system of SourceMod.
		 * @param error		Error buffer to print back to, if any.
		 * @param maxlength	Maximum size of error buffer.
		 * @param late		If this extension was loaded "late" (i.e. manually).
		 * @return			True if load should continue, false otherwise.
		 */
		virtual bool OnExtensionLoad(IExtension *me,
								  IShareSys *sys, 
								  char *error, 
								  size_t maxlength, 
								  bool late) =0;

		/**
		 * @brief Called when the extension is about to be unloaded.
		 */
		virtual void OnExtensionUnload() =0;

		/**
		 * @brief Called when all extensions are loaded (loading cycle is done).
		 * If loaded late, this will be called right after OnExtensionLoad().
		 */
		virtual void OnExtensionsAllLoaded() =0;

		/**
		 * @brief Called when your pause state is about to change.
		 * 
		 * @param pause		True if pausing, false if unpausing.
		 */
		virtual void OnExtensionPauseChange(bool pause) =0;

		/**
		 * @brief Asks the extension whether it's safe to remove an external interface it's using.
		 * If it's not safe, return false, and the extension will be unloaded afterwards.
		 * NOTE: It is important to also hook NotifyInterfaceDrop() in order to clean up resources.
		 *
		 * @param pInterface		Pointer to interface being dropped.
		 * @return					True to continue, false to unload this extension afterwards.
		 */
		virtual bool QueryInterfaceDrop(SMInterface *pInterface)
		{
			return false;
		}

		/**
		 * @brief Notifies the extension that an external interface it uses is being removed.
		 *
		 * @param pInterface		Pointer to interface being dropped.
		 */
		virtual void NotifyInterfaceDrop(SMInterface *pInterface)
		{
		}

		/**
		 * @brief Return false to tell Core that your extension should be considered unusable.
		 *
		 * @param error				Error buffer.
		 * @param maxlength			Size of error buffer.
		 * @return					True on success, false otherwise.
		 */
		virtual bool QueryRunning(char *error, size_t maxlength)
		{
			return true;
		}
	public:
		virtual bool IsMetamodExtension() =0;
		virtual const char *GetExtensionName() =0;
		virtual const char *GetExtensionURL() =0;
		virtual const char *GetExtensionTag() =0;
		virtual const char *GetExtensionAuthor() =0;
		virtual const char *GetExtensionVerString() =0;
		virtual const char *GetExtensionDescription() =0;
		virtual const char *GetExtensionDateString() =0;
	};

	#define SMINTERFACE_EXTENSIONMANAGER_NAME			"IExtensionManager"
	#define SMINTERFACE_EXTENSIONMANAGER_VERSION		1

	/**
	 * @brief Not currently used.
	 */
	enum ExtensionLifetime
	{
		ExtLifetime_Forever,			//Extension will never be unloaded automatically
		ExtLifetime_Map,				//Extension will be unloaded at the end of the map
	};

	/**
	 * @brief Manages the loading/unloading of extensions.
	 */
	class IExtensionManager : public SMInterface
	{
	public:
		virtual const char *GetInterfaceName()
		{
			return SMINTERFACE_EXTENSIONMANAGER_NAME;
		}
		virtual unsigned int GetInterfaceVersion()
		{
			return SMINTERFACE_EXTENSIONMANAGER_VERSION;
		}
	public:
		/**
		 * @brief Loads a extension into the extension system.
		 *
		 * @param path		Path to extension file, relative to the extensions folder.
		 * @param lifetime	Lifetime of the extension (currently ignored).
		 * @param error		Error buffer.
		 * @param maxlength	Maximum error buffer length.
		 * @return			New IExtension on success, NULL on failure.
		 */
		virtual IExtension *LoadExtension(const char *path, 
									ExtensionLifetime lifetime, 
									char *error,
									size_t maxlength) =0;

		/**
		 * @brief Attempts to unload a module.
		 *
		 * @param pExt		IExtension pointer.
		 * @return			True if successful, false otherwise.
		 */
		virtual bool UnloadExtension(IExtension *pExt) =0;
	};
}

#endif //_INCLUDE_SOURCEMOD_MODULE_INTERFACE_H_
