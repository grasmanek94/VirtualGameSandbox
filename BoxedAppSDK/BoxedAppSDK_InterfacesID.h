

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon May 11 16:35:48 2015
 */
/* Compiler settings for i:\build_area\boxedapp_src\src\BoxedApp\src\BoxedAppSDK\BoxedAppSDK_Interfaces.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_BoxedAppSDK_Interfaces,0x4CF2AA85,0x1267,0x404c,0xAA,0x11,0x6A,0x7A,0x4E,0x24,0xBF,0xC1);


MIDL_DEFINE_GUID(IID, IID_IBxFile,0xD6BECAD0,0x5D56,0x4b51,0x82,0x78,0xD6,0x90,0x47,0x4D,0x69,0xFF);


MIDL_DEFINE_GUID(IID, IID_IBxFileHandle,0x236F1640,0xA96D,0x499c,0xAB,0x32,0x66,0xA7,0x02,0x4F,0x19,0x7E);


MIDL_DEFINE_GUID(IID, IID_IBxFileHandle2,0xA13C593D,0x835F,0x429d,0x97,0x19,0xE9,0x40,0x09,0x74,0x21,0xFD);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



