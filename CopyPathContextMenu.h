// CopyPathContextMenu.h : Declaration of the CCopyPathContextMenu

#ifndef __COPYPATHCONTEXTMENU_H_
#define __COPYPATHCONTEXTMENU_H_

#pragma warning(disable:4786)

#include "resource.h"       // main symbols
#include <shlguid.h>
#include <comdef.h>
#include <shlobj.h>
#include <list>
#include <string>
#include <algorithm>
#include "alphanum.hpp"

#define ID_COPY_PATH	0

/////////////////////////////////////////////////////////////////////////////
// CCopyPathContextMenu
class ATL_NO_VTABLE CCopyPathContextMenu : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCopyPathContextMenu, &CLSID_CopyPathContextMenu>,
	public IContextMenu3,
	public IShellExtInit
{
public:
	CCopyPathContextMenu()
	{
	}

	// IContextMenu interface
    STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici);
    STDMETHOD(GetCommandString)(UINT idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax);   
    
	// IContextMenu2 interface
	STDMETHOD(HandleMenuMsg)(UINT uMsg, WPARAM wParam, LPARAM lParam);    
    
	// IContextMenu3 interface
	STDMETHOD(HandleMenuMsg2)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
    
	// IShellExtInit interface
	STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID);

DECLARE_REGISTRY_RESOURCEID(IDR_COPYPATHCONTEXTMENU)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCopyPathContextMenu)
	COM_INTERFACE_ENTRY(IContextMenu3)
	COM_INTERFACE_ENTRY(IContextMenu2)
	COM_INTERFACE_ENTRY(IContextMenu)
	COM_INTERFACE_ENTRY(IShellExtInit)
END_COM_MAP()

private:
	typedef std::basic_string<_TCHAR>		string;	

	struct alphanum_less_str : public std::binary_function<const string&, const string&, bool>
	{
		bool operator()(const string& left, const string& right) const
		{
			// http://www.codeguru.com/forum/showthread.php?t=231155
			CT2CA lc(left.c_str()), rc(right.c_str());
			std::string ls(lc), rs(rc);

			// http://stackoverflow.com/questions/313970/stl-string-to-lower-case
			std::transform(ls.begin(), ls.end(), ls.begin(), ::tolower);
			std::transform(rs.begin(), rs.end(), rs.begin(), ::tolower);

			return doj::alphanum_comp(ls, rs) < 0;
		}
	};

	std::list<string>						m_listFileNames;

	BOOLEAN		m_MakeCStyleString;
	BOOLEAN		m_MakeQuotedPath;
};

#endif //__COPYPATHCONTEXTMENU_H_
