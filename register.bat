cd "%~dp0"

regsvr32 /u C:\Windows\system32\CopyPathExt.dll

copy bin\ReleaseUMinDependency\CopyPathExt.dll C:\Windows\system32 ^
 && regsvr32 C:\Windows\system32\CopyPathExt.dll ^
 || pause
