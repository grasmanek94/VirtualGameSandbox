///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   RegKeyIterator.cpp  $
// Version        : $Revision:   1.1  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Dec 24 1998 07:05:14  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/RegKeyIterator.cpv  $
// 
//    Rev 1.1   Dec 24 1998 07:05:14   len
// Rurn off warnings.
// 
//    Rev 1.0   Aug 27 1998 07:48:12   len
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

#include "RegKeyIterator.hpp"

#pragma warning(disable: 4514)   // unreferenced inline function removed
#pragma warning(disable: 4710)   // function not expanded

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// CRegKeyIterator
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

CRegKeyIterator::CRegKeyIterator(CCountedRegKey *pKey) 
   :  m_pKey(CCountedRegKey::Copy(pKey)), 
      m_index(0) 
{
}

CRegKeyIterator::CRegKeyIterator(const CRegKeyIterator &rhs) 
   :  m_pKey(CCountedRegKey::Copy(rhs.m_pKey)), 
      m_index(rhs.m_index)
{
}

CRegKeyIterator::~CRegKeyIterator()
{
   if (m_pKey)
   {
      m_pKey->Release();
      m_pKey = 0;
   }
}
   

CRegKeyIterator &CRegKeyIterator::operator=(const CRegKeyIterator &rhs)
{
   if (this != &rhs)
   {
      CCountedRegKey *pNewKey = CCountedRegKey::Copy(rhs.m_pKey);
      m_pKey->Release();
      m_pKey = pNewKey;

      m_index = rhs.m_index;
   }

   return *this;
}

bool CRegKeyIterator::operator==(const CRegKeyIterator &rhs) const
{
   return (m_pKey == rhs.m_pKey && m_index == rhs.m_index);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file...
///////////////////////////////////////////////////////////////////////////////
