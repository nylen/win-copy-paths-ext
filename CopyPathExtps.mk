
CopyPathExtps.dll: dlldata.obj CopyPathExt_p.obj CopyPathExt_i.obj
	link /dll /out:CopyPathExtps.dll /def:CopyPathExtps.def /entry:DllMain dlldata.obj CopyPathExt_p.obj CopyPathExt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CopyPathExtps.dll
	@del CopyPathExtps.lib
	@del CopyPathExtps.exp
	@del dlldata.obj
	@del CopyPathExt_p.obj
	@del CopyPathExt_i.obj
