#ifndef __REGISTRY_KEY__
#define __REGISTRY_KEY__
///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   RegistryKey.hpp  $
// Version        : $Revision:   1.4  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Oct 25 1998 11:25:50  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/RegistryKey.hpv  $
// 
//    Rev 1.4   Oct 25 1998 11:25:50   len
// Tidy up after running Lint.
// 
//    Rev 1.3   Aug 27 1998 07:47:46   len
// Reference counted HKEY and loads of other stuff.
// 
//    Rev 1.2   Jun 06 1998 07:43:12   Len
// Lots of stuff.
// 
//    Rev 1.1   May 25 1998 11:01:58   Len
// Added some functionality to the registry exception class to aid debugging.
// 
//    Rev 1.0   May 18 1998 07:49:26   Len
// Initial revision.
// 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1998 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "AS IS," without a warranty of any kind. ALL
// EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING 
// ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
// OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. JETBYTE LIMITED AND ITS LICENSORS 
// SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF 
// USING, MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO 
// EVENT WILL JETBYTE LIMITED BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, 
// OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE 
// DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING 
// OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF JETBYTE LIMITED 
// HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
// This software is not designed or intended for use in on-line control of
// aircraft, air traffic, aircraft navigation or aircraft communications; or in
// the design, construction, operation or maintenance of any nuclear
// facility. Licensee represents and warrants that it will not use or
// redistribute the Software for such purposes.
//
///////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4201)   // nameless struct/union

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#pragma warning(default: 4201)

#ifndef __EXPANDABLE_BUFFER__
#include "ExpandableBuffer.hpp"
#endif

#ifndef __REFERENCE_COUNTED__
#include "ReferenceCounted.hpp"
#endif

#ifndef __REG_KEY_ITERATOR__
#include "RegKeyIterator.hpp"
#endif

#ifndef __WIN32_EXCEPTION__
#include "Win32Exception.hpp"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef TExpandableBuffer<TCHAR> TCHAR_Buffer;
typedef TExpandableBuffer<BYTE> BYTE_Buffer;

///////////////////////////////////////////////////////////////////////////////
// Classes defined in this file...
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey;

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey 
{
   private :

      // Implementation classes for our iterators
      
      class SubkeyIteratorImpl;
      class ValueIteratorImpl;
   
   public :

      class Exception;
      class Value;

      // Easier to manage typedefs for our iterators

      typedef  TRegKeyIterator<SubkeyIteratorImpl> SubkeyIterator;
      typedef  TRegKeyIterator<ValueIteratorImpl> ValueIterator;

      // Construction and destruction

      explicit CRegistryKey(
         HKEY hKey);

      CRegistryKey(
         LPTSTR pRemoteMachine,
         HKEY hKey);

      CRegistryKey(HKEY hKey, 
         LPCTSTR pSubKey, 
         REGSAM samDesired = KEY_ALL_ACCESS,
         LPTSTR pRemoteMachine = 0);

      CRegistryKey(const CRegistryKey &rhs);

      ~CRegistryKey();

      // Assignment

      CRegistryKey &operator=(const CRegistryKey &rhs);
      CRegistryKey &operator=(HKEY hKey);

      // Registry API

      CRegistryKey OpenKey(LPCTSTR pSubKey, 
         REGSAM samDesired = KEY_ALL_ACCESS) const;

      CRegistryKey CreateKey(
         LPCTSTR pSubKey, 
         LPTSTR pClass = _T(""), 
         DWORD dwOptions = REG_OPTION_NON_VOLATILE, 
         REGSAM samDesired = KEY_ALL_ACCESS,
         LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL) const;

      CRegistryKey CreateOrOpenKey(
         LPCTSTR pSubKey, 
         DWORD *pDisposition = NULL,
         LPTSTR pClass = _T(""), 
         DWORD dwOptions = REG_OPTION_NON_VOLATILE, 
         REGSAM samDesired = KEY_ALL_ACCESS,
         LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL) const;

      void DeleteKey(LPCTSTR pKeyName) const;

      void DeleteKeyAndSubkeys(LPCTSTR pKeyName) const;

      bool HasSubkey(
         LPCTSTR pSubkey,    
         REGSAM samDesired = KEY_ALL_ACCESS) const;

      CRegistryKey ConnectRegistry(LPTSTR pMachineName) const;

      void FlushKey() const;

      operator HKEY() const;
      
      // Subkeys

      SubkeyIterator BeginSubkeyIteration() const;
      SubkeyIterator EndSubkeyIteration() const;

      // Values...

      void DeleteValue(
         LPCTSTR pValueName) const;

      Value QueryValue(
         LPCTSTR pValueName = 0) const;
      
      bool QueryValue(
         LPCTSTR pValueName, 
         LPBYTE *ppBytes) const;

      bool QueryValue(
         LPCTSTR pValueName, 
         LPCTSTR *ppString) const;

      bool QueryValue(
         LPCTSTR pValueName, 
         DWORD &dwValue) const;

      void SetValue(
         LPCTSTR pValueName, 
         LPBYTE pBytes, 
         DWORD cbBytes) const;

      void SetValue(
         LPCTSTR pValueName, 
         LPCTSTR pValue, 
         DWORD dwType = REG_SZ) const;

      void SetValue(
         LPCTSTR pValueName, 
         DWORD value, 
         DWORD dwType = REG_DWORD) const;

      void SetValue(
         const Value &value) const;

      ValueIterator BeginValueIteration() const;
      ValueIterator EndValueIteration() const;
      
      void LoadKey(
         LPCTSTR pSubkeyName, 
         LPCTSTR pFile) const;

      void UnLoadKey(
         LPCTSTR pSubkeyName) const;

      void SaveKey(
         LPCTSTR pFile, 
         LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL) const;

      void RestoreKey(
         LPCTSTR pFile, 
         DWORD flags = 0) const;

      void ReplaceKey(
         LPCTSTR pNewFile, 
         LPCTSTR pOldFile, 
         LPCTSTR pSubkeyName = 0) const;

      PSECURITY_DESCRIPTOR GetKeySecurity(
         SECURITY_INFORMATION securityInformation) const;

      void SetKeySecurity(
         SECURITY_INFORMATION securityInformation,
         PSECURITY_DESCRIPTOR pSecurityDescriptor) const;
   
      void NotifyChangeKeyValue(
         HANDLE hEvent, 
         bool bSubKeys = false,
         DWORD dwNotifyFilter = REG_NOTIFY_CHANGE_LAST_SET) const;

      void NotifyChangeKeyValue(
         bool bSubKeys = false,
         DWORD dwNotifyFilter = REG_NOTIFY_CHANGE_LAST_SET) const;

   private :

      static CCountedRegKey *NewCountedKey(
			HKEY hKey, 
			bool bCloseKeyOnFailure = false);

      CCountedRegKey *m_pKey;
};

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::Exception
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey::Exception : public CWin32Exception
{
   public : 

      Exception(
         const LPCTSTR pWhere, 
         LONG error);
};

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::Value
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey::Value
{
   public :

      class Exception;

      friend CRegistryKey;
		friend CRegistryKey::ValueIteratorImpl;

      Value(const Value &rhs);
      ~Value();

      Value &operator=(const Value &rhs);

      operator LPBYTE() const;

      operator LPCTSTR() const;

      operator DWORD() const;

      // other operators

		LPCTSTR Name() const;

      LPCTSTR AsString() const;

      Value(
			LPCTSTR pName, 
			const LPBYTE pBuffer, 
			DWORD bufSize, 
			DWORD dwType = REG_BINARY);

      Value(
			LPCTSTR pName, 
			LPCTSTR pString, 
			DWORD dwType = REG_SZ);

      Value(
			LPCTSTR pName, 
			DWORD dwValue, 
			DWORD dwType = REG_DWORD);

   private :

		static LPTSTR AsString(
			DWORD dwType, 
			const LPBYTE pBuffer, 
			DWORD bufSize);

		static LPTSTR bytesAsString(
			const LPBYTE pBuffer, 
			DWORD bufSize);


      LPBYTE duplicateBuffer() const;

      LPBYTE duplicateBuffer(
			LPBYTE pBuffer, 
			DWORD bufSize) const;

      LPTSTR m_pName;
      DWORD m_dwType;
      LPBYTE m_pBuffer;
      DWORD m_bufSize;
      mutable LPTSTR m_pStringRep;
};

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::Value::Exception
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey::Value::Exception : public CRegistryKey::Exception
{
   public : 

      Exception();
};

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::SubkeyIteratorImpl
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey::SubkeyIteratorImpl : public CRegKeyIterator
{
   public :

      CRegistryKey OpenKey(REGSAM samDesired = KEY_ALL_ACCESS) const;

      LPCTSTR GetName() const;

      LPCTSTR GetClass() const;

   protected :

      SubkeyIteratorImpl(CCountedRegKey *pKey);
   
      bool GetItem();

      bool operator==(const SubkeyIteratorImpl &rhs) const;

   private :


      TCHAR_Buffer m_Name;
      TCHAR_Buffer m_Class;
};

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::ValueIteratorImpl
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey::ValueIteratorImpl : public CRegKeyIterator
{
   public :

      LPCTSTR GetName() const;

      LPCTSTR AsString() const;

		operator Value() const;


   protected :

      ValueIteratorImpl(CCountedRegKey *pKey);

      bool GetItem();

      bool operator==(const ValueIteratorImpl &rhs) const;

   private :

      TCHAR_Buffer m_Name;
      BYTE_Buffer m_Buffer;
		DWORD m_dwType;
		DWORD m_dwBufUsed;
		mutable LPTSTR m_pStringRep;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // __REGISTRY_KEY__

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
