

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __BoxedAppSDK_Interfaces_h__
#define __BoxedAppSDK_Interfaces_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBxFile_FWD_DEFINED__
#define __IBxFile_FWD_DEFINED__
typedef interface IBxFile IBxFile;
#endif 	/* __IBxFile_FWD_DEFINED__ */


#ifndef __IBxFileHandle_FWD_DEFINED__
#define __IBxFileHandle_FWD_DEFINED__
typedef interface IBxFileHandle IBxFileHandle;
#endif 	/* __IBxFileHandle_FWD_DEFINED__ */


#ifndef __IBxFileHandle2_FWD_DEFINED__
#define __IBxFileHandle2_FWD_DEFINED__
typedef interface IBxFileHandle2 IBxFileHandle2;
#endif 	/* __IBxFileHandle2_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __BoxedAppSDK_Interfaces_LIBRARY_DEFINED__
#define __BoxedAppSDK_Interfaces_LIBRARY_DEFINED__

/* library BoxedAppSDK_Interfaces */
/* [helpstring][version][uuid] */ 




typedef struct _SBxFileProp
    {
    DWORD m_dwSizeOfStruct;
    LPOLESTR m_szName;
    LARGE_INTEGER m_nSize;
    DWORD m_dwAttributes;
    FILETIME m_CreationTime;
    FILETIME m_LastWriteTime;
    FILETIME m_LastAccessTime;
    FILETIME m_ChangeTime;
    } 	SBxFileProp;

typedef struct _SBxStreamProp
    {
    DWORD m_dwSizeOfStruct;
    LPOLESTR m_szName;
    LARGE_INTEGER m_nSize;
    } 	SBxStreamProp;


EXTERN_C const IID LIBID_BoxedAppSDK_Interfaces;

#ifndef __IBxFile_INTERFACE_DEFINED__
#define __IBxFile_INTERFACE_DEFINED__

/* interface IBxFile */
/* [object][uuid] */ 


EXTERN_C const IID IID_IBxFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D6BECAD0-5D56-4b51-8278-D690474D69FF")
    IBxFile : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE createFile( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ ULONG DesiredAccess,
            /* [in] */ ULONG Attributes,
            /* [in] */ ULONG ShareAccess,
            /* [in] */ ULONG CreateOptions,
            /* [out] */ IBxFileHandle **ppHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE openFile( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ ULONG DesiredAccess,
            /* [in] */ ULONG ShareAccess,
            /* [in] */ ULONG OpenOptions,
            /* [out] */ IBxFileHandle **ppHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE replaceFile( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ ULONG DesiredAccess,
            /* [in] */ ULONG Attributes,
            /* [in] */ ULONG ShareAccess,
            /* [in] */ ULONG CreateOptions,
            /* [out] */ IBxFileHandle **ppHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Attributes( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ DWORD *pdwAttributes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Attributes( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ DWORD dwAttributes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Size( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ PLARGE_INTEGER pnSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_CreationTime( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_LastWriteTime( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_LastAccessTime( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ChangeTime( 
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBxFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBxFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBxFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBxFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *createFile )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ ULONG DesiredAccess,
            /* [in] */ ULONG Attributes,
            /* [in] */ ULONG ShareAccess,
            /* [in] */ ULONG CreateOptions,
            /* [out] */ IBxFileHandle **ppHandle);
        
        HRESULT ( STDMETHODCALLTYPE *openFile )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ ULONG DesiredAccess,
            /* [in] */ ULONG ShareAccess,
            /* [in] */ ULONG OpenOptions,
            /* [out] */ IBxFileHandle **ppHandle);
        
        HRESULT ( STDMETHODCALLTYPE *replaceFile )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ ULONG DesiredAccess,
            /* [in] */ ULONG Attributes,
            /* [in] */ ULONG ShareAccess,
            /* [in] */ ULONG CreateOptions,
            /* [out] */ IBxFileHandle **ppHandle);
        
        HRESULT ( STDMETHODCALLTYPE *get_Attributes )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ DWORD *pdwAttributes);
        
        HRESULT ( STDMETHODCALLTYPE *put_Attributes )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [in] */ DWORD dwAttributes);
        
        HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ PLARGE_INTEGER pnSize);
        
        HRESULT ( STDMETHODCALLTYPE *get_CreationTime )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *get_LastWriteTime )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *get_LastAccessTime )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *get_ChangeTime )( 
            IBxFile * This,
            /* [string][unique][in] */ LPCWSTR szRelativePath,
            /* [out] */ FILETIME *ptime);
        
        END_INTERFACE
    } IBxFileVtbl;

    interface IBxFile
    {
        CONST_VTBL struct IBxFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBxFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBxFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBxFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBxFile_createFile(This,szRelativePath,DesiredAccess,Attributes,ShareAccess,CreateOptions,ppHandle)	\
    (This)->lpVtbl -> createFile(This,szRelativePath,DesiredAccess,Attributes,ShareAccess,CreateOptions,ppHandle)

#define IBxFile_openFile(This,szRelativePath,DesiredAccess,ShareAccess,OpenOptions,ppHandle)	\
    (This)->lpVtbl -> openFile(This,szRelativePath,DesiredAccess,ShareAccess,OpenOptions,ppHandle)

#define IBxFile_replaceFile(This,szRelativePath,DesiredAccess,Attributes,ShareAccess,CreateOptions,ppHandle)	\
    (This)->lpVtbl -> replaceFile(This,szRelativePath,DesiredAccess,Attributes,ShareAccess,CreateOptions,ppHandle)

#define IBxFile_get_Attributes(This,szRelativePath,pdwAttributes)	\
    (This)->lpVtbl -> get_Attributes(This,szRelativePath,pdwAttributes)

#define IBxFile_put_Attributes(This,szRelativePath,dwAttributes)	\
    (This)->lpVtbl -> put_Attributes(This,szRelativePath,dwAttributes)

#define IBxFile_get_Size(This,szRelativePath,pnSize)	\
    (This)->lpVtbl -> get_Size(This,szRelativePath,pnSize)

#define IBxFile_get_CreationTime(This,szRelativePath,ptime)	\
    (This)->lpVtbl -> get_CreationTime(This,szRelativePath,ptime)

#define IBxFile_get_LastWriteTime(This,szRelativePath,ptime)	\
    (This)->lpVtbl -> get_LastWriteTime(This,szRelativePath,ptime)

#define IBxFile_get_LastAccessTime(This,szRelativePath,ptime)	\
    (This)->lpVtbl -> get_LastAccessTime(This,szRelativePath,ptime)

#define IBxFile_get_ChangeTime(This,szRelativePath,ptime)	\
    (This)->lpVtbl -> get_ChangeTime(This,szRelativePath,ptime)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IBxFile_createFile_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [in] */ ULONG DesiredAccess,
    /* [in] */ ULONG Attributes,
    /* [in] */ ULONG ShareAccess,
    /* [in] */ ULONG CreateOptions,
    /* [out] */ IBxFileHandle **ppHandle);


void __RPC_STUB IBxFile_createFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_openFile_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [in] */ ULONG DesiredAccess,
    /* [in] */ ULONG ShareAccess,
    /* [in] */ ULONG OpenOptions,
    /* [out] */ IBxFileHandle **ppHandle);


void __RPC_STUB IBxFile_openFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_replaceFile_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [in] */ ULONG DesiredAccess,
    /* [in] */ ULONG Attributes,
    /* [in] */ ULONG ShareAccess,
    /* [in] */ ULONG CreateOptions,
    /* [out] */ IBxFileHandle **ppHandle);


void __RPC_STUB IBxFile_replaceFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_get_Attributes_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [out] */ DWORD *pdwAttributes);


void __RPC_STUB IBxFile_get_Attributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_put_Attributes_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [in] */ DWORD dwAttributes);


void __RPC_STUB IBxFile_put_Attributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_get_Size_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [out] */ PLARGE_INTEGER pnSize);


void __RPC_STUB IBxFile_get_Size_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_get_CreationTime_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFile_get_CreationTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_get_LastWriteTime_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFile_get_LastWriteTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_get_LastAccessTime_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFile_get_LastAccessTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFile_get_ChangeTime_Proxy( 
    IBxFile * This,
    /* [string][unique][in] */ LPCWSTR szRelativePath,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFile_get_ChangeTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBxFile_INTERFACE_DEFINED__ */


#ifndef __IBxFileHandle_INTERFACE_DEFINED__
#define __IBxFileHandle_INTERFACE_DEFINED__

/* interface IBxFileHandle */
/* [object][uuid] */ 


EXTERN_C const IID IID_IBxFileHandle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("236F1640-A96D-499c-AB32-66A7024F197E")
    IBxFileHandle : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE get_Stream( 
            /* [out] */ LPSTREAM *ppStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Attributes( 
            /* [out] */ DWORD *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Attributes( 
            /* [in] */ DWORD newValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_DeleteOnClose( 
            /* [out] */ BOOL *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_DeleteOnClose( 
            /* [in] */ BOOL newValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_CreationTime( 
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_CreationTime( 
            /* [in] */ FILETIME time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_LastWriteTime( 
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_LastWriteTime( 
            /* [in] */ FILETIME time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_LastAccessTime( 
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_LastAccessTime( 
            /* [in] */ FILETIME time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ChangeTime( 
            /* [out] */ FILETIME *ptime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_ChangeTime( 
            /* [in] */ FILETIME time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE queryStreamInfo( 
            /* [in] */ ULONG nRequested,
            /* [length_is][size_is][out] */ SBxStreamProp *pProp,
            /* [out] */ ULONG *pnFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_NextFile( 
            /* [in] */ BOOL bRestart,
            /* [out][in] */ SBxFileProp *pProp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rename( 
            /* [string][unique][in] */ LPCWSTR szNewRelativePath,
            /* [in] */ BOOL bReplaceIfExists) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBxFileHandleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBxFileHandle * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBxFileHandle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBxFileHandle * This);
        
        HRESULT ( STDMETHODCALLTYPE *get_Stream )( 
            IBxFileHandle * This,
            /* [out] */ LPSTREAM *ppStream);
        
        HRESULT ( STDMETHODCALLTYPE *get_Attributes )( 
            IBxFileHandle * This,
            /* [out] */ DWORD *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *put_Attributes )( 
            IBxFileHandle * This,
            /* [in] */ DWORD newValue);
        
        HRESULT ( STDMETHODCALLTYPE *get_DeleteOnClose )( 
            IBxFileHandle * This,
            /* [out] */ BOOL *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *put_DeleteOnClose )( 
            IBxFileHandle * This,
            /* [in] */ BOOL newValue);
        
        HRESULT ( STDMETHODCALLTYPE *get_CreationTime )( 
            IBxFileHandle * This,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *put_CreationTime )( 
            IBxFileHandle * This,
            /* [in] */ FILETIME time);
        
        HRESULT ( STDMETHODCALLTYPE *get_LastWriteTime )( 
            IBxFileHandle * This,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *put_LastWriteTime )( 
            IBxFileHandle * This,
            /* [in] */ FILETIME time);
        
        HRESULT ( STDMETHODCALLTYPE *get_LastAccessTime )( 
            IBxFileHandle * This,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *put_LastAccessTime )( 
            IBxFileHandle * This,
            /* [in] */ FILETIME time);
        
        HRESULT ( STDMETHODCALLTYPE *get_ChangeTime )( 
            IBxFileHandle * This,
            /* [out] */ FILETIME *ptime);
        
        HRESULT ( STDMETHODCALLTYPE *put_ChangeTime )( 
            IBxFileHandle * This,
            /* [in] */ FILETIME time);
        
        HRESULT ( STDMETHODCALLTYPE *queryStreamInfo )( 
            IBxFileHandle * This,
            /* [in] */ ULONG nRequested,
            /* [length_is][size_is][out] */ SBxStreamProp *pProp,
            /* [out] */ ULONG *pnFetched);
        
        HRESULT ( STDMETHODCALLTYPE *get_NextFile )( 
            IBxFileHandle * This,
            /* [in] */ BOOL bRestart,
            /* [out][in] */ SBxFileProp *pProp);
        
        HRESULT ( STDMETHODCALLTYPE *rename )( 
            IBxFileHandle * This,
            /* [string][unique][in] */ LPCWSTR szNewRelativePath,
            /* [in] */ BOOL bReplaceIfExists);
        
        END_INTERFACE
    } IBxFileHandleVtbl;

    interface IBxFileHandle
    {
        CONST_VTBL struct IBxFileHandleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBxFileHandle_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBxFileHandle_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBxFileHandle_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBxFileHandle_get_Stream(This,ppStream)	\
    (This)->lpVtbl -> get_Stream(This,ppStream)

#define IBxFileHandle_get_Attributes(This,pValue)	\
    (This)->lpVtbl -> get_Attributes(This,pValue)

#define IBxFileHandle_put_Attributes(This,newValue)	\
    (This)->lpVtbl -> put_Attributes(This,newValue)

#define IBxFileHandle_get_DeleteOnClose(This,pValue)	\
    (This)->lpVtbl -> get_DeleteOnClose(This,pValue)

#define IBxFileHandle_put_DeleteOnClose(This,newValue)	\
    (This)->lpVtbl -> put_DeleteOnClose(This,newValue)

#define IBxFileHandle_get_CreationTime(This,ptime)	\
    (This)->lpVtbl -> get_CreationTime(This,ptime)

#define IBxFileHandle_put_CreationTime(This,time)	\
    (This)->lpVtbl -> put_CreationTime(This,time)

#define IBxFileHandle_get_LastWriteTime(This,ptime)	\
    (This)->lpVtbl -> get_LastWriteTime(This,ptime)

#define IBxFileHandle_put_LastWriteTime(This,time)	\
    (This)->lpVtbl -> put_LastWriteTime(This,time)

#define IBxFileHandle_get_LastAccessTime(This,ptime)	\
    (This)->lpVtbl -> get_LastAccessTime(This,ptime)

#define IBxFileHandle_put_LastAccessTime(This,time)	\
    (This)->lpVtbl -> put_LastAccessTime(This,time)

#define IBxFileHandle_get_ChangeTime(This,ptime)	\
    (This)->lpVtbl -> get_ChangeTime(This,ptime)

#define IBxFileHandle_put_ChangeTime(This,time)	\
    (This)->lpVtbl -> put_ChangeTime(This,time)

#define IBxFileHandle_queryStreamInfo(This,nRequested,pProp,pnFetched)	\
    (This)->lpVtbl -> queryStreamInfo(This,nRequested,pProp,pnFetched)

#define IBxFileHandle_get_NextFile(This,bRestart,pProp)	\
    (This)->lpVtbl -> get_NextFile(This,bRestart,pProp)

#define IBxFileHandle_rename(This,szNewRelativePath,bReplaceIfExists)	\
    (This)->lpVtbl -> rename(This,szNewRelativePath,bReplaceIfExists)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IBxFileHandle_get_Stream_Proxy( 
    IBxFileHandle * This,
    /* [out] */ LPSTREAM *ppStream);


void __RPC_STUB IBxFileHandle_get_Stream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_Attributes_Proxy( 
    IBxFileHandle * This,
    /* [out] */ DWORD *pValue);


void __RPC_STUB IBxFileHandle_get_Attributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_put_Attributes_Proxy( 
    IBxFileHandle * This,
    /* [in] */ DWORD newValue);


void __RPC_STUB IBxFileHandle_put_Attributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_DeleteOnClose_Proxy( 
    IBxFileHandle * This,
    /* [out] */ BOOL *pValue);


void __RPC_STUB IBxFileHandle_get_DeleteOnClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_put_DeleteOnClose_Proxy( 
    IBxFileHandle * This,
    /* [in] */ BOOL newValue);


void __RPC_STUB IBxFileHandle_put_DeleteOnClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_CreationTime_Proxy( 
    IBxFileHandle * This,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFileHandle_get_CreationTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_put_CreationTime_Proxy( 
    IBxFileHandle * This,
    /* [in] */ FILETIME time);


void __RPC_STUB IBxFileHandle_put_CreationTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_LastWriteTime_Proxy( 
    IBxFileHandle * This,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFileHandle_get_LastWriteTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_put_LastWriteTime_Proxy( 
    IBxFileHandle * This,
    /* [in] */ FILETIME time);


void __RPC_STUB IBxFileHandle_put_LastWriteTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_LastAccessTime_Proxy( 
    IBxFileHandle * This,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFileHandle_get_LastAccessTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_put_LastAccessTime_Proxy( 
    IBxFileHandle * This,
    /* [in] */ FILETIME time);


void __RPC_STUB IBxFileHandle_put_LastAccessTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_ChangeTime_Proxy( 
    IBxFileHandle * This,
    /* [out] */ FILETIME *ptime);


void __RPC_STUB IBxFileHandle_get_ChangeTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_put_ChangeTime_Proxy( 
    IBxFileHandle * This,
    /* [in] */ FILETIME time);


void __RPC_STUB IBxFileHandle_put_ChangeTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_queryStreamInfo_Proxy( 
    IBxFileHandle * This,
    /* [in] */ ULONG nRequested,
    /* [length_is][size_is][out] */ SBxStreamProp *pProp,
    /* [out] */ ULONG *pnFetched);


void __RPC_STUB IBxFileHandle_queryStreamInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_get_NextFile_Proxy( 
    IBxFileHandle * This,
    /* [in] */ BOOL bRestart,
    /* [out][in] */ SBxFileProp *pProp);


void __RPC_STUB IBxFileHandle_get_NextFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle_rename_Proxy( 
    IBxFileHandle * This,
    /* [string][unique][in] */ LPCWSTR szNewRelativePath,
    /* [in] */ BOOL bReplaceIfExists);


void __RPC_STUB IBxFileHandle_rename_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBxFileHandle_INTERFACE_DEFINED__ */


#ifndef __IBxFileHandle2_INTERFACE_DEFINED__
#define __IBxFileHandle2_INTERFACE_DEFINED__

/* interface IBxFileHandle2 */
/* [object][uuid] */ 


EXTERN_C const IID IID_IBxFileHandle2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A13C593D-835F-429d-9719-E940097421FD")
    IBxFileHandle2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onNewHandle( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onCloseHandle( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBxFileHandle2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBxFileHandle2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBxFileHandle2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBxFileHandle2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *onNewHandle )( 
            IBxFileHandle2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *onCloseHandle )( 
            IBxFileHandle2 * This);
        
        END_INTERFACE
    } IBxFileHandle2Vtbl;

    interface IBxFileHandle2
    {
        CONST_VTBL struct IBxFileHandle2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBxFileHandle2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBxFileHandle2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBxFileHandle2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBxFileHandle2_onNewHandle(This)	\
    (This)->lpVtbl -> onNewHandle(This)

#define IBxFileHandle2_onCloseHandle(This)	\
    (This)->lpVtbl -> onCloseHandle(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IBxFileHandle2_onNewHandle_Proxy( 
    IBxFileHandle2 * This);


void __RPC_STUB IBxFileHandle2_onNewHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBxFileHandle2_onCloseHandle_Proxy( 
    IBxFileHandle2 * This);


void __RPC_STUB IBxFileHandle2_onCloseHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBxFileHandle2_INTERFACE_DEFINED__ */

#endif /* __BoxedAppSDK_Interfaces_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


