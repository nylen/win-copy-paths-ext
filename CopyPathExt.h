/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed May 26 17:03:27 1999
 */
/* Compiler settings for D:\Development\Projects\Utilities\CopyPathExt\CopyPathExt.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __CopyPathExt_h__
#define __CopyPathExt_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __CopyPathContextMenu_FWD_DEFINED__
#define __CopyPathContextMenu_FWD_DEFINED__

#ifdef __cplusplus
typedef class CopyPathContextMenu CopyPathContextMenu;
#else
typedef struct CopyPathContextMenu CopyPathContextMenu;
#endif /* __cplusplus */

#endif 	/* __CopyPathContextMenu_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __COPYPATHEXTLib_LIBRARY_DEFINED__
#define __COPYPATHEXTLib_LIBRARY_DEFINED__

/* library COPYPATHEXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COPYPATHEXTLib;

EXTERN_C const CLSID CLSID_CopyPathContextMenu;

#ifdef __cplusplus

class DECLSPEC_UUID("7E41911F-13AA-11D3-A831-00104B9E30B5")
CopyPathContextMenu;
#endif
#endif /* __COPYPATHEXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
