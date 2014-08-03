#ifndef __REGISTRY_LIST__
#define __REGISTRY_LIST__
///////////////////////////////////////////////////////////////////////////////
//
// File           : $Workfile:   RegistryList.hpp  $
// Version        : $Revision:   1.3  $
// Function       : 
//
// Author         : $Author:   len  $
// Date           : $Date:   Dec 24 1998 07:04:28  $
//
// Notes          : 
//
// Modifications  :
//
// $Log:   G:/Documents/JetByte/Source/JetByteTools/Win32Tools/PVCS/RegistryList.hpv  $
// 
//    Rev 1.3   Dec 24 1998 07:04:28   len
// Turn off warning 4710
// 
//    Rev 1.2   Oct 25 1998 11:27:18   len
// Tidy up after running Lint.
// 
//    Rev 1.1   Jun 06 1998 07:44:00   Len
// Use DeleteKeyAndSubKeys so that we're sure to remove all trace of the item.
// 
//    Rev 1.0   May 25 1998 10:57:42   Len
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

#include "RegistryKey.hpp"

#pragma warning(disable: 4710)   // function not expanded

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// Templates defined in this file...
///////////////////////////////////////////////////////////////////////////////

template <class T>
class TRegistryList;

///////////////////////////////////////////////////////////////////////////////
// CRegistryList
///////////////////////////////////////////////////////////////////////////////

template <class T>
class TRegistryList 
{
   public :
      
      class iterator : public CRegistryKey::SubkeyIterator
      {
         public :

            friend TRegistryList<T>;

            operator T() const
            { 
               return m_list.ReadItem(*this); 
            }
       
         private :

            iterator(const TRegistryList<T> &list, const CRegistryKey::SubkeyIterator &it)
               :  TRegKeyIterator<class CRegistryKey::SubkeyIteratorImpl>(it), 
                  //CRegistryKey::SubkeyIterator(it),
                  m_list(list) 
            {
            }

            const TRegistryList<T> &m_list;

            // No assignment due to const member... 
            // Do not implement

            iterator operator=(const iterator &rhs);
      };

      friend iterator;

      TRegistryList(CRegistryKey key);

      virtual ~TRegistryList();

      bool HasItem(T item);

      void Add(T item);

      void Update(T item);

      void AddOrUpdate(T item);

      void Remove(T item);


      iterator Begin() const;

      iterator End() const;

   private :

      virtual LPCTSTR GetItemName(T &item) = 0;

      virtual void AddItem(CRegistryKey &key, T &item);
      
      virtual void UpdateItem(CRegistryKey &key, T &item);

      virtual void RemoveItem(CRegistryKey &key, T &item);

      virtual T ReadItem(const iterator &it) const;

      CRegistryKey m_hKey;
};

template <class T>
TRegistryList<T>::TRegistryList(CRegistryKey key)
   :  m_hKey(key)
{

}

template <class T>
TRegistryList<T>::~TRegistryList()
{
}

template <class T>
bool TRegistryList<T>::HasItem(T item)
{
   return m_hKey.HasSubkey(GetItemName(item));
}

template <class T>
void TRegistryList<T>::Add(T item)
{
   LPCTSTR pName = GetItemName(item);

   CRegistryKey itemKey = m_hKey.CreateKey(pName);

   AddItem(itemKey, item);
}

template <class T>
void TRegistryList<T>::Update(T item)
{
   LPCTSTR pName = GetItemName(item);

   CRegistryKey itemKey = m_hKey.OpenKey(pName);

   UpdateItem(itemKey, item);
}

template <class T>
void TRegistryList<T>::AddOrUpdate(T item)
{
   if (HasItem(item))
   {
      UpdateItem(item);
   }
   else
   {
      AddItem(item);
   }
}

template <class T>
void TRegistryList<T>::Remove(T item)
{
   LPCTSTR pName = GetItemName(item);
      
   CRegistryKey itemKey = m_hKey.OpenKey(pName);

   RemoveItem(itemKey, item);

   m_hKey.DeleteKeyAndSubkeys(pName);
}

template <class T>
TRegistryList<T>::iterator TRegistryList<T>::Begin() const
{
   return iterator(*this, m_hKey.BeginSubkeyIteration());
}

template <class T>
TRegistryList<T>::iterator TRegistryList<T>::End() const
{
   return iterator(*this, m_hKey.EndSubkeyIteration());
}

///////////////////////////////////////////////////////////////////////////////
// Default implementation...
///////////////////////////////////////////////////////////////////////////////
/*
template <class T>
LPCTSTR TRegistryList<T>::GetItemName(T &item)
{
   return (LPCTSTR)item;
}
*/
template <class T>
void TRegistryList<T>::AddItem(CRegistryKey &/*key*/, T &/*item*/)
{

}


template <class T>
void TRegistryList<T>::UpdateItem(CRegistryKey &/*key*/, T &/*item*/)
{

}

template <class T>
void TRegistryList<T>::RemoveItem(CRegistryKey &/*key*/, T &/*item*/)
{

}

template <class T>
T TRegistryList<T>::ReadItem(const iterator &it) const
{
   return it.GetName();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // __REGISTRY_LIST__

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
