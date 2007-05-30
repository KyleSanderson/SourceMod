/**
 * vim: set ts=4 :
 * ===============================================================
 * SourceMod, Copyright (C) 2004-2007 AlliedModders LLC. 
 * All rights reserved.
 * ===============================================================
 *
 *  This file is part of the SourceMod/SourcePawn SDK.  This file may only be 
 * used or modified under the Terms and Conditions of its License Agreement, 
 * which is found in public/licenses/LICENSE.txt.  As of this notice, derivative 
 * works must be licensed under the GNU General Public License (version 2 or 
 * greater).  A copy of the GPL is included under public/licenses/GPL.txt.
 * 
 * To view the latest information, see: http://www.sourcemod.net/license.php
 *
 * Version: $Id$
 */

#ifndef _INCLUDE_SOURCEMOD_INTERFACE_DBDRIVER_H_
#define _INCLUDE_SOURCEMOD_INTERFACE_DBDRIVER_H_

#include <IShareSys.h>
#include <IHandleSys.h>
#include <string.h>

#define SMINTERFACE_DBI_NAME		"IDBI"
#define SMINTERFACE_DBI_VERSION		1

namespace SourceMod
{
	/**
	 * @brief Describes a database field value.
	 */
	enum DBResult
	{
		DBVal_Error = 0,		/**< Column number/field is invalid */
		DBVal_TypeMismatch = 1,	/**< You cannot retrieve this data with this type */
		DBVal_Null = 2,			/**< Field has no data (NULL) */
		DBVal_Data = 3,			/**< Field has data */
	};

	/**
	 * @brief Describes a primitive database type.
	 */
	enum DBType
	{
		DBType_Unknown = 0,		/**< Type could not be inferred */
		DBType_String,			/**< NULL-terminated string (variable length) */
		DBType_Blob,			/**< Raw binary data (variable length) */
		DBType_Integer,			/**< 4-byte signed integer */
		DBType_Float,			/**< 4-byte floating point  */
	};

	/**
	 * @brief Represents a one database result row.
	 *
	 * Note that type mismatches will only occur when type safety is being
	 * enforced.  So far this is only the case for prepared statements in 
	 * MySQL and SQLite.
	 *
	 * Also, it is worth noting that retrieving as raw data will never cause a
	 * type mismatch.
	 */
	class IResultRow
	{
	public:
		/**
		 * @brief Retrieves a database field result as a string.
		 *
		 * For NULL values, the resulting string pointer will be non-NULL but 
		 * empty.  The pointer returned will become invalid after  advancing to
		 * the next row.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @param pString		Pointer to store a pointer to the string.
		 * @param length		Optional pointer to store the string length.
		 * @return				A DBResult return code.
		 */
		virtual DBResult GetString(unsigned int columnId, const char **pString, size_t *length) =0;

		/**
		 * @brief Retrieves a database field result as a string, using a 
		 * user-supplied buffer.  If the field is NULL, an empty string
		 * will be copied.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @param buffer		Buffer to store string in.
		 * @param maxlength		Maximum length of the buffer.
		 * @param written		Optional pointer to store the number of bytes 
		 *						written, excluding the null terminator.
		 * @return				A DBResult return code.
		 */
		virtual DBResult CopyString(unsigned int columnId, 
									char *buffer, 
									size_t maxlength, 
									size_t *written) =0;
		
		/**
		 * @brief Retrieves a database field result as a float.
		 *
		 * For NULL entries, the returned float value will be 0.0.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @param pFloat		Pointer to a floating point number to set.
		 * @return				A DBResult return code.
		 */
		virtual DBResult GetFloat(unsigned int columnId, float *pFloat) =0;

		/**
		 * @brief Retrieves a database field result as an integer.
		 *
		 * For NULL entries, the returned integer value will be 0.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @param pInt			Pointer to an integer number to set.
		 * @return				A DBResult return code.
		 */
		virtual DBResult GetInt(unsigned int columnId, int *pInt) =0;

		/**
		 * @brief Returns whether or not a field is NULL.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @return				True if field is NULL, false otherwise.
		 */
		virtual bool IsNull(unsigned int columnId) =0;

		/**
		 * @brief Returns the size of a field (text/raw/blob) in bytes.
		 * For strings, this returned size will not include the null
		 * terminator.
		 * 
		 * When used on fields that are not of variable length,
		 * the size returned will be the number of bytes required
		 * to store the internal data.  Note that the data size 
		 * will correspond to the ACTUAL data type, not the 
		 * COLUMN type.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @return				Number of bytes required to store
		 *						the data, or 0 on failure.
		 */
		virtual size_t GetDataSize(unsigned int columnId) =0;

		/**
		 * @brief Retrieves field data as a raw bitstream.  The pointer returned
		 * will become invalid after advancing to the next row.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @param pData			Pointer to store the raw bit stream.  If the 
		 *						data is NULL, a NULL pointer will be returned.
		 * @param length		Pointer to store the data length.
		 * @return				A DBResult return code.
		 */
		virtual DBResult GetBlob(unsigned int columnId, const void **pData, size_t *length) =0;

		/**
		 * @brief Copies field data as a raw bitstream.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @param buffer		Pointer to copy the data to.  If the data is
		 *						NULL, no data will be copied.
		 * @param maxlength		Maximum length of the buffer.
		 * @param written		Optional pointer to store the number of bytes 
		 *						written.
		 * @return				A DBResult return code.
		 */
		virtual DBResult CopyBlob(unsigned int columnId, void *buffer, size_t maxlength, size_t *written) =0;
	};

	/**
	 * @brief Describes a set of database results.
	 */
	class IResultSet
	{
	public:
		/**
		 * @brief Returns the number of rows in the set.
		 *
		 * @return				Number of rows in the set.
		 */
		virtual unsigned int GetRowCount() =0;

		/**
		 * @brief Returns the number of fields in the set.
		 *
		 * @return				Number of fields in the set.
		 */
		virtual unsigned int GetFieldCount() =0;

		/**
		 * @brief Converts a column number to a column name.
		 *
		 * @param columnId		Column to use, starting from 0.
		 * @return				Pointer to column name, or NULL if not found.
		 */
		virtual const char *FieldNumToName(unsigned int columnId) =0;

		/**
		 * @brief Converts a column name to a column id.
		 *
		 * @param name			Column name (case sensitive).
		 * @param columnId		Pointer to store the column id.  If the
		 *						name is not found, the value will be
		 *						undefined.
		 * @return				True on success, false if not found.
		 */
		virtual bool FieldNameToNum(const char *name, unsigned int *columnId) =0;

		/**
		 * @brief Returns if there is still data in the result set.
		 *
		 * @return				False if there is more data to be read, 
		 *						true, otherwise.
		 */
		virtual bool MoreRows() =0;

		/**
		 * @brief Returns a pointer to the current row and advances
		 * the internal row pointer/counter to the next row available.
		 *
		 * @return				IResultRow pointer to the current row,
		 *						or NULL if there is no more data.
		 */
		virtual IResultRow *FetchRow() =0;

		/**
		 * @brief Returns a pointer to the current row.
		 *
		 * @return				IResultRow pointer to the current row,
		 *						or NULL if the current row is invalid.
		 */
		virtual IResultRow *CurrentRow() =0;

		/**
		 * @brief Rewinds back to the beginning of the row iteration.
		 * 
		 * @return				True on success, false otherwise.
		 */
		virtual bool Rewind() =0;

		/**
		 * @brief Returns a field's type as it should be interpreted
		 * by the user.
		 *
		 * @param field			Field number (starting from 0).
		 * @return				A DBType value.
		 */
		virtual DBType GetFieldType(unsigned int field) =0;

		/**
		 * @brief Returns a field's type as it will be interpreted
		 * by the GetDataSize() function.  For example, MySQL
		 * for non-prepared queries will store all results as
		 * strings internally.
		 *
		 * @param field			Field number (starting from 0).
		 * @return				A DBType value.
		 */
		virtual DBType GetFieldDataType(unsigned int field) =0;
	};

	class IDBDriver;

	class IQuery
	{
	public:
		/**
		 * @brief Returns a pointer to the current result set, if any.  
		 *
		 * @return				An IResultSet pointer on success,
		 *						NULL if no result set exists.
		 */
		virtual IResultSet *GetResultSet() =0;

		/**
		 * @brief Advances to the next result set if one exists.  This
		 * is for checking for MORE result sets, and should not be used
		 * on the first result set.
		 *
		 * Multiple results only happen in certain cases, such as CALLing
		 * stored procedure that have a SELECTs, where MySQL will return 
		 * both the CALL status and one or more SELECT result sets.  If
		 * you do not process these results, they will be automatically
		 * processed for you.  However, the behaviour of creating a new
		 * query from the same connection while results are left 
		 * unprocessed is undefined, and may result in a dropped 
		 * connection.  Therefore, process all extra results or destroy the
		 * IQuery pointer before starting a new query.
		 *
		 * Again, this only happens in very specific cases, so there is
		 * no need to call this for normal queries.
		 *
		 * After calling this function, GetResultSet() must be called
		 * again to return the result set.  The previous result set
		 * is automatically destroyed and will be unusable.
		 *
		 * @return				True if another result set is
		 *						available, false otherwise.
		 */
		virtual bool FetchMoreResults() =0;

		/**
		 * @brief Frees resources created by this query.
		 */
		virtual void Destroy() =0;
	};

	class IPreparedQuery : public IQuery
	{
	public:
		/** 
		 * @brief Binds an integer parameter.
		 *
		 * @param param			Parameter index, starting from 0.
		 * @param num			Number to bind as a value.
		 * @param signd			True to write as signed, false to write as 
		 *						unsigned.
		 * @return				True on success, false otherwise.
		 */
		virtual bool BindParamInt(unsigned int param, int num, bool signd=true) =0;

		/**
		 * @brief Binds a float parameter.
		 *
		 * @param param			Parameter index, starting from 0.
		 * @param f				Float to bind as a value.
		 * @return				True on success, false otherwise.
		 */
		virtual bool BindParamFloat(unsigned int param, float f) =0;

		/**
		 * @brief Binds an SQL NULL type as a parameter.
		 *
		 * @param param			Parameter index, starting from 0.
		 * @return				True on success, false otherwise.
		 */
		virtual bool BindParamNull(unsigned int param) =0;

		/**
		 * @brief Binds a string as a parameter.
		 *
		 * @param param			Parameter index, starting from 0.
		 * @param text			Pointer to a null-terminated string.
		 * @param copy			If true, the pointer is assumed to be
		 *						volatile and a temporary copy may be
		 *						made for safety.
		 * @return				True on success, false otherwise.
		 */
		virtual bool BindParamString(unsigned int param, const char *text, bool copy) =0;

		/**
		 * @brief Binds a blob of raw data as a parameter.
		 *
		 * @param param			Parameter index, starting from 0.
		 * @param data			Pointer to a blob of memory.
		 * @param length		Number of bytes to copy.
		 * @param copy			If true, the pointer is assumed to be
		 *						volatile and a temporary copy may be
		 *						made for safety.
		 * @return				True on success, false otherwise.
		 */
		virtual bool BindParamBlob(unsigned int param, 
									const void *data, 
									size_t length, 
									bool copy) =0;

		/**
		 * @brief Executes the query with the currently bound parameters.

		 * @return				True on success, false otherwise.
		 */
		virtual bool Execute() =0;

		/**
		 * @brief Returns the last error message from this statement.
		 *
		 * @param errCode		Optional pointer to store the driver-specific
		 *						error code.
		 * @return				Error message string.
		 */
		virtual const char *GetError(int *errCode=NULL) =0;

		/**
		 * @brief Number of rows affected by the last execute.
		 *
		 * @return				Number of rows affected by the last execute.
		 */
		virtual unsigned int GetAffectedRows() =0;

		/**
		 * @brief Retrieves the last insert ID on this database connection.
		 *
		 * @return				Row insertion ID of the last execute, if any.
		 */
		virtual unsigned int GetInsertID() =0;
	};

	/**
	 * @brief Encapsulates a database connection.
	 */
	class IDatabase
	{
	public:
		/**
		 * @brief Disconnects the database and frees its associated memory.
		 * Note that the actual object will not be freed until all open 
		 * references have been closed.
		 *
		 * It is guaranteed that an IDatabase pointer won't be destroyed until
		 * all open IQuery or IPreparedQuery pointers are closed.
		 *
		 * @param				Only pass true if being called from an 
		 *						IHandleTypeDispatch destructor.
		 * @return				True if object was destroyed, false if 
		 *						references are remaining.
		 */
		virtual bool Close(bool fromHndlSys=false) =0;

		/**
		 * @brief Error code and string returned by the last operation on this
		 * connection.
		 *
		 * @param errorCode		Optional pointer to retrieve an error code.
		 * @return				Error string pointer (empty if none).
		 */
		virtual const char *GetError(int *errorCode=NULL) =0;

		/**
		 * @brief Prepares and executes a query in one step, and discards
		 * any return data.
		 *
		 * @param query			Query string.
		 * @return				True on success, false otherwise.
		 */
		virtual bool DoSimpleQuery(const char *query) =0;

		/**
		 * @brief Prepares and executes a query in one step, and returns
		 * the resultant data set.
		 *
		 * Note: If a query contains more than one result set, each
		 * result set must be processed before a new query is started.
		 *
		 * @param query			Query string.
		 * @return				IQuery pointer on success, NULL otherwise.
		 */
		virtual IQuery *DoQuery(const char *query) =0;

		/** 
		 * @brief Prepares a query statement for multiple executions and/or
		 * binding marked parameters (? in MySQL/sqLite, $n in PostgreSQL).
		 *
		 * @param query			Query string.
		 * @param error			Error buffer.
		 * @param maxlength		Maximum length of the error buffer.
		 * @param errCode		Optional pointer to store a driver-specific error code.
		 * @return				IPreparedQuery pointer on success, NULL
		 *						otherwise.
		 */
		virtual IPreparedQuery *PrepareQuery(const char *query, char *error, size_t maxlength, int *errCode=NULL) =0;

		/**
		 * Quotes a string for insertion into a query.
		 *
		 * @param str			Source string.
		 * @param buffer		Buffer to store new string (should not overlap source string).
		 * @param maxlen		Maximum length of the output buffer.
		 * @param newSize		Pointer to store the output size.
		 * @return				True on success, false if the output buffer is not big enough.
		 *						If not big enough, the required buffer size is passed through
		 *						newSize.
		 */
		virtual bool QuoteString(const char *str, char buffer[], size_t maxlen, size_t *newSize) =0;

		/**
		 * @brief Number of rows affected by the last execute.
		 *
		 * @return				Number of rows affected by the last execute.
		 */
		virtual unsigned int GetAffectedRows() =0;

		/**
		 * @brief Retrieves the last insert ID on this database connection.
		 *
		 * @return				Row insertion ID of the last execute, if any.
		 */
		virtual unsigned int GetInsertID() =0;

		/**
		 * @brief Returns an IDatabase Handle to this IDatabase.
		 *
		 * @return				Handle_t of type IDatabase.
		 */
		virtual Handle_t GetHandle() =0;
	};

	/** 
	 * @brief Describes database connection info.
	 */
	struct DatabaseInfo
	{
		DatabaseInfo()
		{
			dbiVersion = SMINTERFACE_DBI_VERSION;
			port = 0;
			maxTimeout = 0;
		}
		unsigned int dbiVersion;		/**< DBI Version for backwards compatibility */
		const char *host;				/**< Host string */
		const char *database;			/**< Database name string */
		const char *user;				/**< User to authenticate as */
		const char *pass;				/**< Password to authenticate with */
		const char *driver;				/**< Driver to use */
		unsigned int port;				/**< Port to use, 0=default */
		unsigned int maxTimeout;		/**< Maximum timeout, 0=default */
	};

	/**
	 * @brief Describes an SQL driver.
	 */
	class IDBDriver
	{
	public:
		virtual unsigned int GetDBIVersion()
		{
			return SMINTERFACE_DBI_VERSION;
		}
	public:
		/**
		 * @brief Initiates a database connection.
		 * 
		 * @param info			Database connection info pointer.
		 * @param persistent	If true, a previous persistent connection will
		 *						be re-used if possible.
		 * @param error			Buffer to store error message.
		 * @param maxlength		Maximum size of the error buffer.
		 * @return				A new IDatabase pointer, or NULL on failure.
		 */
		virtual IDatabase *Connect(const DatabaseInfo *info, bool persistent, char *error, size_t maxlength) =0;

		/**
		 * @brief Returns a case insensitive database identifier string.
		 *
		 * @return				String containing an identifier.
		 */
		virtual const char *GetIdentifier() =0;

		/**
		 * @brief Returns a case sensitive implementation name.
		 *
		 * @return				String containing an implementation name.
		 */
		virtual const char *GetProductName() =0;

		/**
		 * @brief Retrieves a Handle_t handle of the IDBDriver type.
		 *
		 * @return				A Handle_t handle.
		 */
		virtual Handle_t GetHandle() =0;

		/**
		 * @brief Returns the driver's controlling identity.
		 *
		 * @return				An IdentityToken_t identity.
		 */
		virtual IdentityToken_t *GetIdentity() =0;
	};

	/**
	 * @brief Database-related Handle types.
	 */
	enum DBHandleType
	{
		DBHandle_Driver = 0,		/**< Driver Handle */
		DBHandle_Database = 1,		/**< Database Handle */
	};

	/**
	 * @brief Describes the DBI manager.
	 */
	class IDBManager : public SMInterface
	{
	public:
		virtual const char *GetInterfaceName() =0;
		virtual unsigned int GetInterfaceVersion() =0;
	public:
		/**
		 * @brief Adds a driver to the DBI system.
		 *
		 * @param pDriver		Database driver.
		 */
		virtual void AddDriver(IDBDriver *pDriver) =0;

		/**
		 * @brief Removes a driver from the DBI system.
		 *
		 * @param pDriver		Database driver.
		 */
		virtual void RemoveDriver(IDBDriver *pDriver) =0;

		/**
		 * @brief Searches for database info by name.
		 *
		 * @param name			Named database info.
		 * @return				DatabaseInfo pointer.
		 */
		virtual const DatabaseInfo *FindDatabaseConf(const char *name) =0;

		/**
		 * @brief Tries to connect to a named database.  
		 *
		 * @param name			Named database info.
		 * @param pdr			Pointer to store the IDBDriver pointer in.
		 *						If driver is not found, NULL will be stored.
		 * @param pdb			Pointer to store the IDatabase pointer in.
		 *						If connection fails, NULL will be stored.
		 * @param persistent	If true, the dbmanager will attempt to PConnect
		 *						instead of connect.
		 * @param error			Error buffer to store a driver's error message.
		 * @param maxlength		Maximum length of the error buffer.
		 * @return				True on success, false otherwise.
		 */
		virtual bool Connect(const char *name, 
							IDBDriver **pdr, 
							IDatabase **pdb, 
							bool persistent, 
							char *error, 
							size_t maxlength) =0;

		/**
		 * @brief Returns the number of drivers loaded.
		 *
		 * @return				Number of drivers loaded.
		 */
		virtual unsigned int GetDriverCount() =0;

		/**
		 * @brief Returns a driver by index.
		 *
		 * @param index			Driver index, starting from 0.
		 * @return				IDBDriver pointer for the given index.
		 */
		virtual IDBDriver *GetDriver(unsigned int index) =0;

		/**
		 * @brief Creates a Handle_t of the IDBDriver type.
		 *
		 * @param type			A DBHandleType value.
		 * @param ptr			A pointer corrresponding to a DBHandleType object.
		 * @param token			Identity pointer of the owning identity.
		 * @return				A new Handle_t handle, or 0 on failure.
		 */
		virtual Handle_t CreateHandle(DBHandleType type, void *ptr, IdentityToken_t *pToken) =0;

		/**
		 * @brief Reads an IDBDriver pointer from an IDBDriver handle.
		 *
		 * @param hndl			Handle_t handle to read.
		 * @param type			A DBHandleType value.
		 * @param ptr			Pointer to store the object pointer.
		 * @return				HandleError value.
		 */
		virtual HandleError ReadHandle(Handle_t hndl, DBHandleType type, void **ptr) =0;

		/**
		 * @brief Releases an IDBDriver handle.
		 *
		 * @param hndl			Handle_t handle to release.
		 * @param type			A DBHandleType value.
		 * @param token			Identity pointer of the owning identity.
		 * @return				HandleError value.
		 */
		virtual HandleError ReleaseHandle(Handle_t hndl, DBHandleType type, IdentityToken_t *token) =0;
	};
}

#endif //_INCLUDE_SOURCEMOD_INTERFACE_DBDRIVER_H_
