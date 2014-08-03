#ifndef __REFERENCE_COUNTED__
#define __REFERENCE_COUNTED__
///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   ReferenceCounted.hpp  $
// Version        : $Revision:   1.0  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Aug 27 1998 07:50:20  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/C++Tools/PVCS/ReferenceCounted.hpv  $
// 
//    Rev 1.0   Aug 27 1998 07:50:20   len
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
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// Templates defined in this file...
///////////////////////////////////////////////////////////////////////////////

template <class T, class B> class TReferenceCounted;

///////////////////////////////////////////////////////////////////////////////
// TReferenceCounted
///////////////////////////////////////////////////////////////////////////////

template <class T, class B> class TReferenceCounted
{
   public :

      // Construction and destruction

      TReferenceCounted(T counted);

      // Access functions 

      const T &GetCounted() const;

      operator T &() const;

      operator T *() const;

      // Manage the reference count

      B *AddRef();
      B *Release();

      // Copy a pointer to a TReferenceCounted...

      static B *Copy(B *pRefCounted);

   protected :

      virtual void PreRelease() = 0;

      T m_counted;
      
      // We can't be deleted except by ourself...

      virtual ~TReferenceCounted();

   private :

      LONG m_count;

      // Restrict copying, don't implement

      TReferenceCounted(const TReferenceCounted &rhs);
      TReferenceCounted &operator=(const TReferenceCounted &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
TReferenceCounted<T,B>::TReferenceCounted(T counted) 
   : m_counted(counted), m_count(1) 
{ 

}

template <class T, class B>
TReferenceCounted<T,B>::~TReferenceCounted() 
{ 
   
}

///////////////////////////////////////////////////////////////////////////////
// Access the underlying object
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
const T &TReferenceCounted<T,B>::GetCounted() const
{
   return m_counted;
}

template <class T, class B>
TReferenceCounted<T,B>::operator T &() const
{
   return m_counted;
}

template <class T, class B>
TReferenceCounted<T,B>::operator T *() const
{
   return &m_counted;
}

///////////////////////////////////////////////////////////////////////////////
// Manage the reference count
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
B *TReferenceCounted<T,B>::AddRef()
{
   InterlockedIncrement(&m_count);

   return (B*)this;
}

template <class T, class B>
B *TReferenceCounted<T,B>::Release()
{
   if (0 == InterlockedDecrement(&m_count))
   {
      try
      {
         PreRelease();
      }
      catch (...)
      {
         // Release() mustn't throw...
      }

      delete this;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Copy a reference counted pointer
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
B *TReferenceCounted<T,B>::Copy(B *pRefCounted)
{
   return (pRefCounted ? pRefCounted->AddRef() : 0);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // __REFERENCE_COUNTED__

///////////////////////////////////////////////////////////////////////////////
// End of file...
///////////////////////////////////////////////////////////////////////////////
