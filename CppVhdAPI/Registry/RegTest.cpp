#pragma warning(disable: 4201)   // nameless struct/union

#include <windows.h>
#include <tchar.h>

#pragma warning(default: 4201)

#include "RegistryKey.hpp"

#include <iostream>

using namespace JetByteTools;

int main()
{
   try 
   {
      //CRegistryKey key = CRegistryKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE"));
      CRegistryKey key = CRegistryKey(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION"));

      for (CRegistryKey::SubkeyIterator it = key.BeginSubkeyIteration();
           it != key.EndSubkeyIteration();
           ++it)
      {
         std::cout << it.GetName() << std::endl;

         CRegistryKey thisKey = it.OpenKey();

         for (CRegistryKey::ValueIterator val = thisKey.BeginValueIteration();
              val != thisKey.EndValueIteration();
              ++val)
         {
            std::cout << "   " << val.GetName() << " - " << val.AsString() << std::endl;
         }
      }

		CRegistryKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE")).CreateKey(_T("JetByteTest")).SetValue(_T(""), _T("This is a Test"));

		CRegistryKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE")).DeleteKey(_T("JetByteTest"));
   }
   catch (CRegistryKey::Exception &e)
   {
      e.MessageBox();
   }

   return 1;
}
