///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   RegistryKey.cpp  $
// Version        : $Revision:   1.6  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Dec 24 1998 06:59:48  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/RegistryKey.cpv  $
// 
//    Rev 1.6   Dec 24 1998 06:59:48   len
// Fixed a bug in handling of UNICODE strings (REG_SZ) where we were
// passing the length of the string rather than the number of bytes, which
// was wrong...
// 
//    Rev 1.5   Oct 25 1998 11:24:38   len
// Tidy up after running Lint...
// 
//    Rev 1.4   Oct 21 1998 20:08:52   len
// Bug fixes as reported by Steve Greenland - steve.greenland@aspentech.com
// 
//    Rev 1.3   Aug 27 1998 07:46:56   len
// Reference counted HKEY and loads of other things.
// 
//    Rev 1.2   Jun 06 1998 07:42:34   Len
// Made delete key reliable. Added DeleteKeyAndSubKeys.
// Tidied up.
// 
//    Rev 1.1   May 25 1998 11:02:40   Len
// Bug fixes.
// 
//    Rev 1.0   May 18 1998 07:49:24   Len
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

///////////////////////////////////////////////////////////////////////////////
// Include files
///////////////////////////////////////////////////////////////////////////////

#include "RegistryKey.hpp"

#include <tchar.h>
#include <stdio.h>
#include <malloc.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

CRegistryKey::CRegistryKey(
   HKEY hKey) 
   :  m_pKey(NewCountedKey(hKey, true))
{
}

CRegistryKey::CRegistryKey(
   LPTSTR pRemoteMachine,
   HKEY hKey) 
   :  m_pKey(0)
{
   HKEY theKey = hKey;

   LONG result = RegConnectRegistry(pRemoteMachine, hKey, &theKey);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::CRegistryKey() - RegConnectRegistry"), result);
   }

   m_pKey = NewCountedKey(theKey, true);
}


CRegistryKey::CRegistryKey(
   HKEY hKey, 
   LPCTSTR pSubKey, 
   REGSAM samDesired /* = KEY_ALL_ACCESS */,
   LPTSTR pRemoteMachine /* = 0 */)
   :  m_pKey(0)
{
   HKEY theKey = hKey;

   // if we're passed a remote machine name...
   // do a connect registry first

   if (pRemoteMachine)
   {
      LONG result = RegConnectRegistry(pRemoteMachine, hKey, &theKey);

      if (ERROR_SUCCESS != result)
      {
         throw Exception(_T("CRegistryKey::CRegistryKey() - RegConnectRegistry"), result);
      }
   }

   HKEY newKey;

   LONG result = RegOpenKeyEx(theKey, pSubKey, 0, samDesired, &newKey);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::CRegistryKey(HKEY hKey ...)"), result);
   }

   m_pKey = NewCountedKey(newKey, true);
}

CRegistryKey::CRegistryKey(const CRegistryKey &rhs)
   :  m_pKey(rhs.m_pKey->AddRef())
{

}

CRegistryKey::~CRegistryKey()
{
   m_pKey = m_pKey->Release();
}

///////////////////////////////////////////////////////////////////////////////
// Assignment
///////////////////////////////////////////////////////////////////////////////

CRegistryKey &CRegistryKey::operator=(const CRegistryKey &rhs)
{
   if (this != &rhs)
   {
      CCountedRegKey *pNewKey = rhs.m_pKey->AddRef();
      
      m_pKey->Release();

      m_pKey = pNewKey;
   }

   return *this;
}

// NOTE: We OWN This hKey from this point on...

CRegistryKey &CRegistryKey::operator=(HKEY hKey)
{
   CCountedRegKey *pNewKey = NewCountedKey(hKey, true);
   
   m_pKey->Release();

   m_pKey = pNewKey;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Static helper function...
///////////////////////////////////////////////////////////////////////////////

CCountedRegKey *CRegistryKey::NewCountedKey(
   HKEY hKey, 
   bool bCloseKeyOnFailure /* = false */)
{
   CCountedRegKey *pCCountedRegKey = 0;

   try
   {
      pCCountedRegKey = new CCountedRegKey(hKey);
   }
   catch (...) //xalloc &e)
   {
      pCCountedRegKey = 0;
   }

   if (!pCCountedRegKey)
   {
      if (bCloseKeyOnFailure)
      {
         RegCloseKey(hKey);
      }

      throw Exception(_T("CRegistryKey::NewCCountedRegKey()"), 
         ERROR_NOT_ENOUGH_MEMORY);
   }

   return pCCountedRegKey;
}

///////////////////////////////////////////////////////////////////////////////
// Registry API wrappers...
///////////////////////////////////////////////////////////////////////////////

CRegistryKey CRegistryKey::OpenKey(
   LPCTSTR pSubKey, 
   REGSAM samDesired /* = KEY_ALL_ACCESS */) const
{
   return CRegistryKey(*this, pSubKey, samDesired);
}

void CRegistryKey::DeleteKey(
   LPCTSTR pKeyName) const 
{
   // Behaviour of RegDeleteKey differs on Win95 and NT
   // On 95 RegDeleteKey will delete keys with subkeys
   // on NT it wont.
   // To add some consistency to the world DeleteKey 
   // will always fail to delete a key with sub keys and
   // DeleteKeyAndSubKeys will always work...


   // scope the key...
   {
      CRegistryKey deadKey = OpenKey(pKeyName); 

      if (deadKey.BeginSubkeyIteration() != deadKey.EndSubkeyIteration())
      {
         throw Exception(_T("CRegistryKey::DeleteKey()"), ERROR_ACCESS_DENIED);
      }
   }

   LONG result = RegDeleteKey(m_pKey->GetCounted(), pKeyName);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::DeleteKey()"), result);
   }
}

void CRegistryKey::DeleteKeyAndSubkeys(
   LPCTSTR pKeyName) const 
{
   // Win95 doesn't need this as it deletes subkeys by default
   // NT wont delete a key with subkeys...

   // Scope the deadKey...
   {
      CRegistryKey deadKey = OpenKey(pKeyName);

		// This wont work, we are frigging with the sub keys of the key we're
		// iterating...

//      for (SubkeyIterator it = deadKey.BeginSubkeyIteration();
//         it != deadKey.EndSubkeyIteration();
//        ++it)
//      {
//         deadKey.DeleteKeyAndSubkeys(it.GetName());
//      }

      for (SubkeyIterator it = deadKey.BeginSubkeyIteration();
         it != deadKey.EndSubkeyIteration();
         it = deadKey.BeginSubkeyIteration())
      {
         deadKey.DeleteKeyAndSubkeys(it.GetName());
      }


   }

   DeleteKey(pKeyName);
}

CRegistryKey CRegistryKey::CreateKey(
   LPCTSTR pSubKey, 
   LPTSTR pClass /* = _T("") */, 
   DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */, 
   REGSAM samDesired /* = KEY_ALL_ACCESS */,
   LPSECURITY_ATTRIBUTES pSecurityAttributes /* = NULL */) const
{
   DWORD disposition;

   CRegistryKey key = CreateOrOpenKey(
                        pSubKey,
                        &disposition,
                        pClass,
                        dwOptions,
                        samDesired,
                        pSecurityAttributes);
   
   if (disposition != REG_CREATED_NEW_KEY)
   {
      RegCloseKey(key);

      throw Exception(_T("CRegistryKey::CreateKey()"), ERROR_ALREADY_EXISTS);
   }


   return key;
}

CRegistryKey CRegistryKey::CreateOrOpenKey(
   LPCTSTR pSubKey, 
   DWORD *pDisposition /* = NULL */,
   LPTSTR pClass /* = _T("") */, 
   DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */, 
   REGSAM samDesired /* = KEY_ALL_ACCESS */,
   LPSECURITY_ATTRIBUTES pSecurityAttributes /* = NULL */) const
{
   HKEY hKey;

   DWORD disposition;

   if (!pDisposition)
   {
      pDisposition = &disposition;
   }

   LONG result = RegCreateKeyEx(m_pKey->GetCounted(), pSubKey, 0, pClass, dwOptions,
                           samDesired, pSecurityAttributes, &hKey,
                           pDisposition);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::CreateOrOpenKey()"), result);
   }

   return CRegistryKey(hKey);
}

bool CRegistryKey::HasSubkey(
   LPCTSTR pSubKey,    
   REGSAM samDesired /* = KEY_ALL_ACCESS */) const
{
   bool hasKey = false;

   HKEY hKey;

   LONG result = RegOpenKeyEx(m_pKey->GetCounted(), pSubKey, 0, samDesired, &hKey);

   if (ERROR_SUCCESS == result)
   {
      hasKey = true;

      RegCloseKey(hKey);
   }
   else // should check for errors that mean no we dont have it, or we 
      // have it but not with the right access and throw if a real error
   {
      hasKey = false;
   }

   return hasKey;
}

CRegistryKey CRegistryKey::ConnectRegistry(LPTSTR pMachineName) const
{
   HKEY hKey;

   LONG result = RegConnectRegistry(pMachineName, m_pKey->GetCounted(), &hKey);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::ConnectRegistry()"), result);
   }

   return CRegistryKey(hKey);
}

void CRegistryKey::FlushKey() const
{
   LONG result = RegFlushKey(m_pKey->GetCounted());

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::FlushKey()"), result);
   }
}

CRegistryKey::operator HKEY() const 
{ 
   return m_pKey->GetCounted(); 
}

void CRegistryKey::DeleteValue(LPCTSTR pValueName) const
{
   LONG result = RegDeleteValue(m_pKey->GetCounted(), pValueName);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::DeleteValue()"), result);
   }
}

// Subkey iteration

CRegistryKey::SubkeyIterator CRegistryKey::BeginSubkeyIteration() const
{
   return SubkeyIterator(m_pKey);
}

CRegistryKey::SubkeyIterator CRegistryKey::EndSubkeyIteration() const
{
   return SubkeyIterator(0);
}
///////////////////////////////////////////////////////////////////////////////
// Value iteration
///////////////////////////////////////////////////////////////////////////////

CRegistryKey::ValueIterator CRegistryKey::BeginValueIteration() const
{
   return ValueIterator(m_pKey);
}

CRegistryKey::ValueIterator CRegistryKey::EndValueIteration() const
{
   return ValueIterator(0);
}

///////////////////////////////////////////////////////////////////////////////
// Query values
///////////////////////////////////////////////////////////////////////////////

bool CRegistryKey::QueryValue(LPCTSTR pValueName, LPBYTE *ppBytes) const
{
   DWORD dwType;
   LPBYTE pBuffer = 0;
   DWORD bufSize = 0;

   bool ok = false;

   LONG result = RegQueryValueEx(
      m_pKey->GetCounted(), 
      pValueName, 
      0,
      &dwType,
      pBuffer,
      &bufSize);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::QueryValue()"), result);
   }

   if (dwType == REG_BINARY || dwType == REG_NONE)
   {
      // bufSize should now tell us how much space we need...

		// TODO Smart pointer
		
      pBuffer = (LPBYTE)malloc(bufSize);

      result = RegQueryValueEx(
            m_pKey->GetCounted(), 
            pValueName, 
            0,
            &dwType,
            pBuffer,
            &bufSize);

      if (ERROR_SUCCESS == result)
      {
         ok = true;
         *ppBytes = pBuffer;
      }
      else
      {
         free(pBuffer); //crap

         throw Exception(_T("CRegistryKey::QueryValue()"), result);
      }
   }

   return ok;
}

bool CRegistryKey::QueryValue(LPCTSTR pValueName, LPCTSTR *ppString) const
{
   DWORD dwType;
   LPBYTE pBuffer = 0;
   DWORD bufSize = 0;

   bool ok = false;

   LONG result = RegQueryValueEx(
      m_pKey->GetCounted(), 
      pValueName, 
      0,
      &dwType,
      pBuffer,
      &bufSize);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::QueryValue()"), result);
   }

   if (dwType == REG_EXPAND_SZ || dwType == REG_SZ)
   {
      // bufSize should now tell us how much space we need...

      pBuffer = (LPBYTE)malloc(bufSize);

      result = RegQueryValueEx(
            m_pKey->GetCounted(), 
            pValueName, 
            0,
            &dwType,
            pBuffer,
            &bufSize);

      if (ERROR_SUCCESS == result)
      {
         ok = true;
         *ppString = (LPCTSTR)pBuffer;
      }
      else
      {
         free(pBuffer); //crap

         throw Exception(_T("CRegistryKey::QueryValue()"), result);
      }
   }

   return ok;
}

bool CRegistryKey::QueryValue(
   LPCTSTR pValueName, 
   DWORD &dwValue) const
{
   DWORD dwType;
   LPBYTE pBuffer = 0;
   DWORD bufSize = 0;

   bool ok = false;

   LONG result = RegQueryValueEx(
      m_pKey->GetCounted(), 
      pValueName, 
      0,
      &dwType,
      pBuffer,
      &bufSize);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::QueryValue()"), result);
   }

   if (dwType == REG_DWORD || 
       dwType == REG_DWORD_LITTLE_ENDIAN || 
       dwType == REG_DWORD_BIG_ENDIAN)
   {
      // bufSize should be sizeof(DWORD)
      
      if (bufSize != sizeof(DWORD))
      {
         throw Exception(_T("CRegistryKey::QueryValue()"), result);
      }

      result = RegQueryValueEx(
            m_pKey->GetCounted(), 
            pValueName, 
            0,
            &dwType,
            (LPBYTE)&dwValue,
            &bufSize);

      if (ERROR_SUCCESS != result)
      {
          throw Exception(_T("CRegistryKey::QueryValue()"), result);
      }
   }

   return ok;
}



// QueryValue for other types
// What about multiple values?

CRegistryKey::Value CRegistryKey::QueryValue(LPCTSTR pValueName /* = 0 */) const
{
   DWORD dwType;
   LPBYTE pBuffer = 0;
   DWORD bufSize = 0;

   LONG result = RegQueryValueEx(
      m_pKey->GetCounted(), 
      pValueName, 
      0,
      &dwType,
      pBuffer,
      &bufSize);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::QueryValue()"), result);
   }

   // bufSize should now tell us how much space we need...

   pBuffer = new unsigned char[bufSize];

   result = RegQueryValueEx(
         m_pKey->GetCounted(), 
         pValueName, 
         0,
         &dwType,
         pBuffer,
         &bufSize);

   if (ERROR_SUCCESS != result)
   {
      delete[] pBuffer; //crap

      throw Exception(_T("CRegistryKey::QueryValue()"), result);
   }

   return Value(pValueName, pBuffer, bufSize, dwType);
}






void CRegistryKey::SetValue(
    LPCTSTR pValueName, 
    LPBYTE pBytes, 
    DWORD cbBytes) const
{
   LONG result = RegSetValueEx(
      m_pKey->GetCounted(),
      pValueName,
      0,
      REG_BINARY,
      pBytes,
      cbBytes);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::SetValue()"), result);
   }
}

void CRegistryKey::SetValue(
   LPCTSTR pValueName, 
   LPCTSTR pValue, 
   DWORD dwType /*= REG_SZ*/) const
{
   if (dwType != REG_SZ &&
       //dwType != REG_LINK && 
       dwType != REG_EXPAND_SZ)
       // Handle MULTI_SZ and do the strlen ourselves?
   {
      throw Exception(_T("CRegistryKey::SetValue()"), ERROR_INVALID_FLAGS);
   }

   LONG result = RegSetValueEx(
      m_pKey->GetCounted(),
      pValueName,
      0,
      dwType,
      (LPBYTE)pValue,
      (_tcslen(pValue) + 1) * sizeof(TCHAR));      // length of string plus null in bytes

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::SetValue()"), result);
   }
}

void CRegistryKey::SetValue(
   LPCTSTR pValueName, 
   DWORD value, 
   DWORD dwType /*= REG_DWORD*/) const
{
   if (dwType != REG_DWORD &&
       dwType != REG_DWORD_LITTLE_ENDIAN && 
       dwType != REG_DWORD_BIG_ENDIAN)
   {
      throw Exception(_T("CRegistryKey::SetValue()"), ERROR_INVALID_FLAGS);
   }

   LONG result = RegSetValueEx(
      m_pKey->GetCounted(),
      pValueName,
      0,
      dwType,
      (LPBYTE)&value,
      sizeof(DWORD));

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::SetValue()"), result);
   }
}

void CRegistryKey::SetValue(
   const Value &value) const
{
   // Should the value store the name too?
   LONG result = RegSetValueEx(
      m_pKey->GetCounted(),
      value.m_pName,
      0,
      value.m_dwType,
      value.m_pBuffer,
      value.m_bufSize);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::SetValue()"), result);
   }
}

void CRegistryKey::LoadKey(LPCTSTR pSubkeyName, LPCTSTR pFile) const
{
   LONG result = RegLoadKey(m_pKey->GetCounted(), pSubkeyName, pFile);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::LoadKey()"), result);
   }
}

void CRegistryKey::UnLoadKey(LPCTSTR pSubkeyName) const
{
   LONG result = RegUnLoadKey(m_pKey->GetCounted(), pSubkeyName);

   if (ERROR_SUCCESS == result)
   {
      throw Exception(_T("CRegistryKey::UnLoadKey()"), result);
   }
}

void CRegistryKey::SaveKey(
   LPCTSTR pFile, 
   LPSECURITY_ATTRIBUTES pSecurityAttributes /* = NULL */) const
{
   LONG result = RegSaveKey(m_pKey->GetCounted(), pFile, pSecurityAttributes);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::SaveKey()"), result);
   }
}

void CRegistryKey::RestoreKey(LPCTSTR pFile, DWORD flags /* = 0 */) const
{
   LONG result = RegRestoreKey(m_pKey->GetCounted(), pFile, flags);

   if (ERROR_SUCCESS == result)
   {
      throw Exception(_T("CRegistryKey::RestoreKey()"), result);
   }
}

void CRegistryKey::ReplaceKey(
   LPCTSTR pNewFile, 
   LPCTSTR pOldFile, 
   LPCTSTR pSubkeyName /* = 0 */) const
{
   LONG result = RegReplaceKey(m_pKey->GetCounted(), pSubkeyName, pNewFile, pOldFile);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::ReplaceKey()"), result);
   }
}

PSECURITY_DESCRIPTOR CRegistryKey::GetKeySecurity(
   SECURITY_INFORMATION securityInformation) const
{
   TExpandableBuffer<unsigned char> descriptor;

   DWORD dwSize = 0;
   
   LONG result = RegGetKeySecurity(m_pKey->GetCounted(), 
      securityInformation, 
      0,
      &dwSize);

   if (ERROR_INSUFFICIENT_BUFFER == result)
   {
      descriptor.Resize(dwSize);

      result = RegGetKeySecurity(m_pKey->GetCounted(), 
            securityInformation, 
            (PSECURITY_DESCRIPTOR)descriptor.GetBuffer(),
            &dwSize);
   }

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::GetKeySecurity()"), result);
   }

   return descriptor.ReleaseBuffer();
}


void CRegistryKey::SetKeySecurity(
   SECURITY_INFORMATION securityInformation,
   PSECURITY_DESCRIPTOR pSecurityDescriptor) const
{
   LONG result = RegSetKeySecurity(m_pKey->GetCounted(), 
      securityInformation, 
      pSecurityDescriptor);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::SetKeySecurity()"), result);
   }
}

void CRegistryKey::NotifyChangeKeyValue(
   HANDLE hEvent, 
   bool bSubKeys /* = false */,
   DWORD dwNotifyFilter /* = REG_NOTIFY_CHANGE_LAST_SET */) const
{
   LONG result = RegNotifyChangeKeyValue(m_pKey->GetCounted(),
      bSubKeys,
      dwNotifyFilter,
      hEvent,
      true);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::RegNotifyChangeKeyValue()"), result);
   }
}

void CRegistryKey::NotifyChangeKeyValue(
   bool bSubKeys /* = false */,
   DWORD dwNotifyFilter /* = REG_NOTIFY_CHANGE_LAST_SET */) const
{
   LONG result = RegNotifyChangeKeyValue(m_pKey->GetCounted(),
      bSubKeys,
      dwNotifyFilter,
      0,
      false);

   if (ERROR_SUCCESS != result)
   {
      throw Exception(_T("CRegistryKey::RegNotifyChangeKeyValue()"), result);
   }
}


///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::SubkeyIterator
///////////////////////////////////////////////////////////////////////////////

CRegistryKey::SubkeyIteratorImpl::SubkeyIteratorImpl(CCountedRegKey *pKey) 
   :  CRegKeyIterator(pKey),
      m_Name(0),
      m_Class(0)
{
   if (pKey)
   {
      DWORD dwNumSubKeys = 0;
      DWORD dwMaxNameLen = 0;
      DWORD dwMaxClassLen = 0;

      LONG result = RegQueryInfoKey(
         pKey->GetCounted(),
         NULL,    // Not interested in this key's class 
         NULL,    // ditto
         NULL,    // Reserved
         &dwNumSubKeys,    
         &dwMaxNameLen,    
         &dwMaxClassLen,    
         NULL,    // Not interested in number of values 
         NULL,    // Not interested in max length of value name
         NULL,    // Not interested in max length of value buffer
         NULL,    // Not interested in length of security descriptor
         NULL);   // Not interested in last write time

      if (ERROR_SUCCESS != result)
      {
         throw Exception(_T("CRegistryKey::SubkeyIterator::SubkeyIterator()"),
            result);
      }

      if (0 != dwNumSubKeys)
      {
         // Allow for NULL character...

         m_Name.Resize(dwMaxNameLen + 1);
         m_Class.Resize(dwMaxClassLen + 1);
      }
   }
}

bool CRegistryKey::SubkeyIteratorImpl::operator==(const SubkeyIteratorImpl &rhs) const
{
   return CRegKeyIterator::operator==(rhs);
}

LPCTSTR CRegistryKey::SubkeyIteratorImpl::GetName() const
{
   return m_Name;
}

LPCTSTR CRegistryKey::SubkeyIteratorImpl::GetClass() const
{
   return m_Class;
}

CRegistryKey CRegistryKey::SubkeyIteratorImpl::OpenKey(
   REGSAM samDesired /*= KEY_ALL_ACCESS*/) const
{
   return CRegistryKey(m_pKey->GetCounted(), m_Name, samDesired);
}

bool CRegistryKey::SubkeyIteratorImpl::GetItem()
{
   FILETIME m_lastWriteTime;

   bool ok = true;
   bool done = false;

   // Could initialise with a call to GetKeyInfo to get longest key name etc

   while (!done && ok)
   {

      DWORD nameLen = m_Name.GetSize();
      DWORD classLen = m_Class.GetSize();

      LONG result = RegEnumKeyEx(
                     m_pKey->GetCounted(), 
                     m_index,
                     m_Name, 
                     &nameLen,
                     NULL,
                     m_Class,
                     &classLen,
                     &m_lastWriteTime);

      if (ERROR_NO_MORE_ITEMS == result)
      {
         ok = false;
      }
      else if (ERROR_MORE_DATA == result)
      {
         // Size has changed since we started 

         DWORD dwNumSubKeys = 0;
         DWORD dwMaxNameLen = 0;
         DWORD dwMaxClassLen = 0;

         result = RegQueryInfoKey(
            m_pKey->GetCounted(),
            NULL,    // Not interested in this key's class 
            NULL,    // ditto
            NULL,    // Reserved
            &dwNumSubKeys,    
            &dwMaxNameLen,    
            &dwMaxClassLen,    
            NULL,    // Not interested in number of values 
            NULL,    // Not interested in max length of value name
            NULL,    // Not interested in max length of value buffer
            NULL,    // Not interested in length of security descriptor
            NULL);   // Not interested in last write time

         if (ERROR_SUCCESS != result)
         {
            throw Exception(_T("CRegistryKey::SubkeyIterator::GetSubkeyInfo()"),
               result);
         }

         if (0 != dwNumSubKeys)
         {
            // Allow for NULL character...

            m_Name.Resize(dwMaxNameLen + 1);
            m_Class.Resize(dwMaxClassLen + 1);
         }
         else
         {
            done = true;
         }
      }
      else if (ERROR_SUCCESS != result)
      {
         throw Exception(_T("CRegistryKey::SubkeyIterator::GetSubkeyInfo()"),
            result);
      }
      else
      {
         done = true;
      }
   }
   return ok;
}
///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::ValueIteratorImpl
///////////////////////////////////////////////////////////////////////////////

CRegistryKey::ValueIteratorImpl::ValueIteratorImpl(CCountedRegKey *pKey) 
   :  CRegKeyIterator(pKey),
      m_Name(0),
      m_Buffer(0),
		m_dwType(REG_NONE),
		m_dwBufUsed(0),
		m_pStringRep(0)
{
   if (m_pKey)
   {
      DWORD dwNumValues = 0;
      DWORD dwMaxValueNameLen = 0;
      DWORD dwMaxValueLen = 0;

      LONG result = RegQueryInfoKey(
         m_pKey->GetCounted(),
         NULL,    // Not interested in this key's class 
         NULL,    // ditto
         NULL,    // Reserved
         NULL,    // Not interested in number of sub keys
         NULL,    // Not interested in max sub key name length
         NULL,    // Not interested in max sub key class length
         &dwNumValues,
         &dwMaxValueNameLen,
         &dwMaxValueLen,
         NULL,    // Not interested in length of security descriptor
         NULL);   // Not interested in last write time

      if (ERROR_SUCCESS != result)
      {
         throw Exception(_T("CRegistryKey::ValueIterator::ValueIterator()"),
            result);
      }

      if (0 != dwNumValues)
      {
         // Allow for NULL character...

         m_Name.Resize(dwMaxValueNameLen + 1);
         m_Buffer.Resize(dwMaxValueLen + 1);
			m_dwBufUsed = 0;
			m_dwType = REG_NONE;
      }
   }
}

bool CRegistryKey::ValueIteratorImpl::operator==(
      const ValueIteratorImpl &rhs) const
{
   return CRegKeyIterator::operator==(rhs);
}

LPCTSTR CRegistryKey::ValueIteratorImpl::GetName() const
{
   return m_Name;
}

LPCTSTR CRegistryKey::ValueIteratorImpl::AsString() const
{
	if (!m_pStringRep)
	{
		m_pStringRep = Value::AsString(m_dwType, m_Buffer, m_dwBufUsed);
	}

	return m_pStringRep;
}

CRegistryKey::ValueIteratorImpl::operator CRegistryKey::Value() const
{
	return Value(m_Name, m_Buffer, m_dwBufUsed, m_dwType);
}

bool CRegistryKey::ValueIteratorImpl::GetItem()
{
   bool ok = true;
   bool done = false;

	// String representation cache is no longer valid

	delete[] m_pStringRep;
	m_pStringRep = 0;

   while (!done && ok)
   {

      DWORD nameLen = m_Name.GetSize();
      m_dwBufUsed = m_Buffer.GetSize();

      LONG result = RegEnumValue(
                     m_pKey->GetCounted(), 
                     m_index,
                     m_Name, 
                     &nameLen,
                     NULL,
                     &m_dwType,
                     m_Buffer,
                     &m_dwBufUsed);

      if (ERROR_NO_MORE_ITEMS == result)
      {
         ok = false;
      }
      else if (ERROR_MORE_DATA == result)
      {
         DWORD dwNumValues = 0;
         DWORD dwMaxValueNameLen = 0;
         DWORD dwMaxValueLen = 0;

         result = RegQueryInfoKey(
            m_pKey->GetCounted(),
            NULL,    // Not interested in this key's class 
            NULL,    // ditto
            NULL,    // Reserved
            NULL,    // Not interested in number of sub keys
            NULL,    // Not interested in max sub key name length
            NULL,    // Not interested in max sub key class length
            &dwNumValues,
            &dwMaxValueNameLen,
            &dwMaxValueLen,
            NULL,    // Not interested in length of security descriptor
            NULL);   // Not interested in last write time

         if (ERROR_SUCCESS != result)
         {
            throw Exception(_T("CRegistryKey::ValueIterator::GetValueInfo()"),
               result);
         }

         if (0 != dwNumValues)
         {
            // Allow for NULL character...

            m_Name.Resize(dwMaxValueNameLen + 1);
            m_Buffer.Resize(dwMaxValueLen + 1);
         }
         else
         {
            done = true;
         }
      }
      else if (ERROR_SUCCESS != result)
      {
         throw Exception(_T("CRegistryKey::ValueIterator::GetValueInfo()"),
            result);
      }
      else
      {
         done = true;
      }
   }
   return ok;
}

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::Exception
///////////////////////////////////////////////////////////////////////////////

CRegistryKey::Exception::Exception(
   const LPCTSTR pWhere, 
   LONG error)
   :  CWin32Exception(pWhere, (DWORD)error)
{
}

///////////////////////////////////////////////////////////////////////////////
// CRegistryKey::Value
///////////////////////////////////////////////////////////////////////////////

CRegistryKey::Value::Value(
   LPCTSTR pName, 
   const LPBYTE pBuffer, 
   DWORD bufSize, 
   DWORD dwType /*= REG_BINARY*/)
   :  m_pName((LPTSTR)_tcsdup(pName)),
      m_dwType(dwType), 
      m_pBuffer((LPBYTE)duplicateBuffer(pBuffer, bufSize)), 
      m_bufSize(bufSize),
      m_pStringRep(0)
{
}

CRegistryKey::Value::Value(
   LPCTSTR pName, 
   LPCTSTR pString, 
   DWORD dwType /*= REG_SZ */)
   :  m_pName((LPTSTR)_tcsdup(pName)),
      m_dwType(dwType),
      m_pBuffer((LPBYTE)_tcsdup(pString)),
      m_bufSize((_tcslen(pString) + 1) * sizeof(TCHAR)),
      m_pStringRep(0)
{
   if (m_dwType != REG_EXPAND_SZ && 
       m_dwType != REG_SZ)
   {
      throw Exception();
   }
}

CRegistryKey::Value::Value(
   LPCTSTR pName, 
   DWORD dwValue, 
   DWORD dwType /*= REG_DWORD*/)
   :  m_pName((LPTSTR)_tcsdup(pName)),
      m_dwType(dwType),
      m_pBuffer(new unsigned char[sizeof(DWORD)]),
      m_bufSize(sizeof(DWORD)),
      m_pStringRep(0)
{
   if (dwType != REG_DWORD &&
       dwType != REG_DWORD_LITTLE_ENDIAN && 
       dwType != REG_DWORD_BIG_ENDIAN)
   {
      throw Exception();
   }

   memcpy(m_pBuffer, &dwValue, sizeof(DWORD));
}

CRegistryKey::Value::Value(const Value &rhs)
   :  m_pName(_tcsdup(rhs.m_pName)), 
      m_dwType(rhs.m_dwType),
      m_pBuffer(rhs.duplicateBuffer()),
      m_bufSize(rhs.m_bufSize),
      m_pStringRep(0)
{
   memcpy(m_pBuffer, rhs.m_pBuffer, m_bufSize);
}

CRegistryKey::Value::~Value()
{
   delete[] m_pName;
   delete[] m_pBuffer;
   delete[] m_pStringRep;
}

CRegistryKey::Value &CRegistryKey::Value::operator=(const Value &rhs)
{
   if (this != &rhs)
   {
      LPBYTE pNewBuffer = rhs.duplicateBuffer();
      LPBYTE pOldBuffer = m_pBuffer;
	  
      LPTSTR pNewName = _tcsdup(rhs.m_pName);
      LPTSTR pOldName = m_pName;

      m_dwType = rhs.m_dwType;
      m_bufSize = rhs.m_bufSize;

      m_pBuffer = pNewBuffer;
      m_pName = pNewName;

      delete[] pOldBuffer;
      delete[] pOldName;

      delete[] m_pStringRep;
      m_pStringRep = 0;
   }

   return *this;
}

CRegistryKey::Value::operator LPBYTE() const
{
   if (m_dwType != REG_BINARY && 
       m_dwType != REG_NONE)
   {
      throw Exception();
   }

   return m_pBuffer;
}

CRegistryKey::Value::operator LPCTSTR() const
{
   if (m_dwType != REG_EXPAND_SZ && 
       m_dwType != REG_SZ)
   {
      throw Exception();
   }

   return (LPCTSTR)m_pBuffer;
}

CRegistryKey::Value::operator DWORD() const
{
   if (m_dwType != REG_DWORD &&
       m_dwType != REG_DWORD_LITTLE_ENDIAN && 
       m_dwType != REG_DWORD_BIG_ENDIAN)
   {
      throw Exception();
   }

   return *(DWORD*)m_pBuffer;
}

LPCTSTR CRegistryKey::Value::Name() const
{
	return m_pName;
}

LPCTSTR CRegistryKey::Value::AsString() const
{
	if (!m_pStringRep)
	{
		m_pStringRep = AsString(m_dwType, m_pBuffer, m_bufSize);
	}

	return m_pStringRep;
}

LPTSTR CRegistryKey::Value::AsString(
	DWORD dwType, 
	const LPBYTE pBuffer, 
	DWORD bufSize)
{
	LPTSTR pStringRep;

   // TODO Convert all reps to a string rep

   if (dwType == REG_EXPAND_SZ || 
       dwType == REG_SZ)
   {
      // Already a string!

      pStringRep = _tcsdup((LPTSTR)pBuffer);
   }
	else if (dwType == REG_MULTI_SZ)
	{
		// multiple strings...

		// Should strip the \0's out...
		pStringRep = _tcsdup((LPTSTR)pBuffer);
	}
	else if (dwType == REG_BINARY || 
			   dwType == REG_NONE)
   {
		pStringRep = bytesAsString(pBuffer, bufSize);
   }
   else if (dwType == REG_DWORD ||
            dwType == REG_DWORD_LITTLE_ENDIAN || 
            dwType == REG_DWORD_BIG_ENDIAN)
   {
      pStringRep = _tcsdup(_T("A number"));
   }
	else
	{
		// Anything else...
      pStringRep = bytesAsString(pBuffer, bufSize);
	}

   return pStringRep;
}

LPTSTR CRegistryKey::Value::bytesAsString(
	const LPBYTE pBuffer, 
	DWORD bufSize)
{
	LPTSTR pStringRep = new TCHAR[(bufSize * 2) + 1];
	
	LPTSTR pHere = pStringRep;
	LPBYTE pBuf = pBuffer;

	for (DWORD i = 0; i < bufSize; i++)
	{
		_stprintf(pHere, _T("%2.2x"), *pBuf);

		pHere += 2;
		pBuf++;
	}

	return pStringRep;
}

LPBYTE CRegistryKey::Value::duplicateBuffer() const
{
	return duplicateBuffer(m_pBuffer, m_bufSize);
}

LPBYTE CRegistryKey::Value::duplicateBuffer(LPBYTE pBuffer, DWORD bufSize) const
{
	LPBYTE pNewBuffer = new unsigned char[bufSize];
	memcpy(pNewBuffer, pBuffer, bufSize);
	
	return pNewBuffer;
}


CRegistryKey::Value::Exception::Exception()
   :  CRegistryKey::Exception(_T("CRegistryKey::Value::Exception"),
         ERROR_INVALID_PARAMETER)
{
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file...
///////////////////////////////////////////////////////////////////////////////
