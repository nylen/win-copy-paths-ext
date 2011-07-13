/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed May 26 17:03:27 1999
 */
/* Compiler settings for D:\Development\Projects\Utilities\CopyPathExt\CopyPathExt.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

const IID LIBID_COPYPATHEXTLib = {0x7E419111,0x13AA,0x11D3,{0xA8,0x31,0x00,0x10,0x4B,0x9E,0x30,0xB5}};


const CLSID CLSID_CopyPathContextMenu = {0x7E41911F,0x13AA,0x11D3,{0xA8,0x31,0x00,0x10,0x4B,0x9E,0x30,0xB5}};


#ifdef __cplusplus
}
#endif

