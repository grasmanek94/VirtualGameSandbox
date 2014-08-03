#ifndef __WIN32_EXCEPTION__
#define __WIN32_EXCEPTION__
///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   Win32Exception.hpp  $
// Version        : $Revision:   1.1  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Oct 25 1998 20:43:44  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/Win32Exception.hpv  $
// 
//    Rev 1.1   Oct 25 1998 20:43:44   len
// GetMessage() should be virtual. Added optional HWND parameter to MessageBox.
// 
//    Rev 1.0   Jun 06 1998 07:48:12   Len
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

#pragma warning(disable: 4201)

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in this file...
///////////////////////////////////////////////////////////////////////////////

class CWin32Exception;

///////////////////////////////////////////////////////////////////////////////
// CWin32Exception
///////////////////////////////////////////////////////////////////////////////

class CWin32Exception
{
   public : 

      CWin32Exception(
         const LPCTSTR pWhere, 
         DWORD error);

      DWORD GetError() const;

      LPCTSTR GetWhere() const;

      virtual LPCTSTR GetMessage() const; 

      void MessageBox(HWND hWnd = NULL) const; 

   private :
      
      LPCTSTR m_pWhere;
      DWORD m_error;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // __WIN32_EXCEPTION__

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
