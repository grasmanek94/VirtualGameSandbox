#ifndef __REG_KEY_ITERATOR__
#define __REG_KEY_ITERATOR__
///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   RegKeyIterator.hpp  $
// Version        : $Revision:   1.1  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Oct 25 1998 11:27:06  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/RegKeyIterator.hpv  $
// 
//    Rev 1.1   Oct 25 1998 11:27:06   len
// Tidy up after running Lint.
// 
//    Rev 1.0   Aug 27 1998 07:48:22   len
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

#include "CountedRegKey.hpp"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// Templates defined in this file...
///////////////////////////////////////////////////////////////////////////////

template <class B> class TRegKeyIterator;

///////////////////////////////////////////////////////////////////////////////
// Classes defined in this file...
///////////////////////////////////////////////////////////////////////////////

class CRegKeyIterator;

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class CRegistryKey;
class CCountedRegKey;

///////////////////////////////////////////////////////////////////////////////
// CRegKeyIterator
///////////////////////////////////////////////////////////////////////////////

class CRegKeyIterator
{
   public :

      virtual ~CRegKeyIterator();
   
   protected :

      CRegKeyIterator(CCountedRegKey *pKey);

      CRegKeyIterator(const CRegKeyIterator &rhs);

      CRegKeyIterator &operator=(const CRegKeyIterator &rhs);

      bool operator==(const CRegKeyIterator &rhs) const;

      CCountedRegKey *m_pKey;

      DWORD m_index;
};


///////////////////////////////////////////////////////////////////////////////
// TRegKeyIterator
///////////////////////////////////////////////////////////////////////////////

template <class B> 
class TRegKeyIterator : public B
{
   public :

      friend class CRegistryKey;

      TRegKeyIterator<B> &operator++();
      TRegKeyIterator<B> operator++(int);

      bool operator!=(const TRegKeyIterator<B> &rhs) const;
      bool operator==(const TRegKeyIterator<B> &rhs) const;

   private :

      TRegKeyIterator(CCountedRegKey *pKey);

      void Advance();
};

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

template <class B> 
TRegKeyIterator<B>::TRegKeyIterator(CCountedRegKey *pKey) 
   :  B(pKey)
{
   Advance();
}

///////////////////////////////////////////////////////////////////////////////
// Iteration
///////////////////////////////////////////////////////////////////////////////

template <class B> 
TRegKeyIterator<B> &TRegKeyIterator<B>::operator++()
{
   Advance();

   return *this;
}

template <class B> 
TRegKeyIterator<B> TRegKeyIterator<B>::operator++(int)
{
   TRegKeyIterator<B> oldValue = *this;

   Advance();

   return oldValue;
}

///////////////////////////////////////////////////////////////////////////////
// Equality
///////////////////////////////////////////////////////////////////////////////

template <class B> 
bool TRegKeyIterator<B>::operator!=(
   const TRegKeyIterator<B> &rhs) const
{
   // defer to operator==
   return !(*this == rhs);
}

template <class B> 
bool TRegKeyIterator<B>::operator==(
   const TRegKeyIterator<B> &rhs) const
{
   return B::operator==(rhs);
}

///////////////////////////////////////////////////////////////////////////////
// Iteration helper function
///////////////////////////////////////////////////////////////////////////////

template <class B> 
void TRegKeyIterator<B>::Advance()
{
   if (m_pKey)
   {
      if (!B::GetItem())
      {
         // End of the line

         m_pKey = m_pKey->Release();

         m_index = 0;
      }
      else
      {
         m_index++;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// End of TRegKeyIterator
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // __REG_KEY_ITERATOR__

///////////////////////////////////////////////////////////////////////////////
// End of file...
///////////////////////////////////////////////////////////////////////////////
