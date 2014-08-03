///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   Win32Exception.cpp  $
// Version        : $Revision:   1.2  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Oct 25 1998 20:44:58  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/Win32Exception.cpv  $
// 
//    Rev 1.2   Oct 25 1998 20:44:58   len
// Added optional HWND parameter to MessageBox().
// 
//    Rev 1.1   Aug 27 1998 07:48:44   len
// 
//    Rev 1.0   Jun 06 1998 07:48:06   Len
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

#include "Win32Exception.hpp"

#include "Utils.hpp"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// CWin32Exception
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

CWin32Exception::CWin32Exception(
   const LPCTSTR pWhere, 
   DWORD error)
   :  m_pWhere(pWhere), 
      m_error(error)
{
//   DEBUG_ONLY(MessageBox());
}

DWORD CWin32Exception::GetError() const 
{ 
   return m_error; 
}

LPCTSTR CWin32Exception::GetWhere() const 
{ 
   // Wrap the message with the file and line?
   return m_pWhere; 
}

LPCTSTR CWin32Exception::GetMessage() const 
{ 
   return GetLastErrorMessage(m_error); 
}

void CWin32Exception::MessageBox(HWND hWnd /* = NULL */) const 
{ 
   ::MessageBox(hWnd, GetMessage(), m_pWhere, MB_ICONSTOP); 
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file...
///////////////////////////////////////////////////////////////////////////////
