// CopyPathContextMenu.cpp : Implementation of CCopyPathContextMenu
#include "stdafx.h"
#include "CopyPathExt.h"
#include "CopyPathContextMenu.h"
#include <stdio.h>

#ifdef _UNICODE
#define CF_TEXT_FORMAT		CF_UNICODETEXT
#define ALLOC_GLOBAL_STRING(hGlobal, strText) HGLOBAL hGlobal = ::GlobalAlloc(GMEM_SHARE, ((wcslen(strText) + 1) * 2));
#else
#define CF_TEXT_FORMAT		CF_TEXT
#define ALLOC_GLOBAL_STRING(hGlobal, strText) HGLOBAL hGlobal = ::GlobalAlloc(GMEM_SHARE, strlen(strText) + 1);
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyPathContextMenu

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 1.20.2000
//* Function name	: CCopyPathContextMenu::QueryContextMenu
//* Description	    : Called when explorer wants to add your item to the context
//*					  menu.
//***************************************
//
STDMETHODIMP CCopyPathContextMenu::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	_TCHAR		strMenuText[30];

	// Adds our item as ID: (idCmdFirst + ID_COPY_PATH) = idCmdFirst.
	_stprintf(strMenuText, _T("Copy &Path%s to Clipboard"), (m_listFileNames.size() > 1 ? _T("s") : _T("")));
	::InsertMenu(hmenu, indexMenu++, MF_STRING | MF_BYPOSITION,
        idCmdFirst + ID_COPY_PATH, strMenuText);
	return ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, (USHORT)(ID_COPY_PATH + 1)));
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 5.27.99
//* Function name	: CCopyPathContextMenu::Initialize
//* Description	    : 
//***************************************
//
STDMETHODIMP CCopyPathContextMenu::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID)
{
	HRESULT				hres = E_FAIL;
	FORMATETC			fmte = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM			medium;
	int					nFileCount = 0, i;
	_TCHAR				strFilePath[MAX_PATH];

	// No data object
	if (lpdobj == NULL)
		return E_FAIL;

	// Use the given IDataObject to get a list of filenames (CF_HDROP).
	hres = lpdobj->GetData(&fmte, &medium);

	if (FAILED(hres))
		return E_FAIL;

	// Make sure HDROP contains at least one file.
	if ((nFileCount = DragQueryFile((HDROP)medium.hGlobal, (UINT)(-1), NULL, 0)) >= 1)
	{
		// Loop through all the files that were selected.
		for (i = 0; i < nFileCount; i++) {
			DragQueryFile((HDROP)medium.hGlobal, i, strFilePath, MAX_PATH);
			// If the file name is a directory, make sure it has a backslash at the end.
			if (::GetFileAttributes(strFilePath) & FILE_ATTRIBUTE_DIRECTORY) {
				int nLen = _tcslen(strFilePath);
				if (strFilePath[nLen-1] != _T('\\')) {
					_tcscat(strFilePath, _T("\\"));
				}
			}
			// Add the file name to the end of the list.
			m_listFileNames.push_back(strFilePath);
		}
		hres = S_OK;
	}
	else
		hres = E_FAIL;

	// Release the data.
	ReleaseStgMedium (&medium);

	return hres;
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 1.20.2000
//* Function name	: CCopyPathContextMenu::InvokeCommand
//* Description	    : 
//***************************************
//
STDMETHODIMP CCopyPathContextMenu::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
{
	_TCHAR*		pStrClipboardText = NULL, strTempFileNameBuff[MAX_PATH + 50];
	_TCHAR		*pCurrent = NULL, *pLast = NULL;
	BOOLEAN		bMakeCStyleString = ((GetKeyState(VK_CONTROL) & 0x8000) != 0);
	BOOLEAN		bMakeShortPath = ((GetKeyState(VK_SHIFT) & 0x8000) != 0);
	int			nFileCount = 0, i;

    switch (LOWORD(lpici->lpVerb)) {		
	case ID_COPY_PATH:

		nFileCount = m_listFileNames.size();
		if (nFileCount == 0)
			return S_OK;
		
		// The '+ 50' is so that we provide ample room for double backslashes.
		pStrClipboardText = new _TCHAR[nFileCount * (MAX_PATH+50)];
		pStrClipboardText[0] = _T('\0');

		// Loop through all the files.
		for (i = 0; i < nFileCount; i++) {
			// Copy the file name into a temporary buffer.  If the ALT key is down,
			// convert the long file name to a short one.
			if (bMakeShortPath) {
				::GetShortPathName(m_listFileNames.front().data(), strTempFileNameBuff, MAX_PATH + 50);
			}
			else {
				_tcscpy(strTempFileNameBuff, m_listFileNames.front().data());
			}
			
			pLast = strTempFileNameBuff;
			// If the control key is pressed, change the path so that all the
			// backslashes are converted to double backslashes.  This is useful
			// when pasting a path into C/C++ code, as a single backslash denotes
			// an escape sequence and a double backslash denotes a literal backslash.				
			if (bMakeCStyleString) {				
				while ((pCurrent = _tcschr(pLast, _T('\\'))) != NULL) {
					_tcsncat(pStrClipboardText, pLast, pCurrent - pLast + 1);
					pLast = pCurrent + 1;
					_tcscat(pStrClipboardText, _T("\\"));
				}
				_tcscat(pStrClipboardText, pLast);
			}
			else {
				_tcscat(pStrClipboardText, pLast);
			}
			// If this isn't the last file, add a line break before we add the next file name.
			if (i != (nFileCount - 1))
				_tcscat(pStrClipboardText, _T("\r\n"));

			m_listFileNames.pop_front();
		}		

		// Open and empty the clipboard, allocate global memory, copy the file
		// names into the memory, and then set the clipboard.
		if (::OpenClipboard(NULL)) {
			::EmptyClipboard();
			ALLOC_GLOBAL_STRING(hText, pStrClipboardText);
			LPTSTR pText = (LPTSTR) ::GlobalLock(hText);
			_tcscpy(pText, pStrClipboardText);
			::SetClipboardData(CF_TEXT_FORMAT, hText);
			::GlobalUnlock(hText);
			::CloseClipboard();
		}

		delete[] pStrClipboardText;
		break;
    }
    return S_OK;
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 1.21.2000
//* Function name	: CCopyPathContextMenu::GetCommandString
//* Description	    : Called to do one of three things: Get a help string, make sure the item
//*					  exists, and to get a language-independent string.
//***************************************
//
STDMETHODIMP CCopyPathContextMenu::GetCommandString(UINT idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	HRESULT				hr = E_INVALIDARG; 
	int					nFileCount = 0;
	_TCHAR				strFileCount[20], strHelpTextBuff[100];
	BOOLEAN				bMakeCStyleString = ((GetKeyState(VK_CONTROL) & 0x8000) != 0);
	BOOLEAN				bMakeShortPath = ((GetKeyState(VK_SHIFT) & 0x8000) != 0);
	USES_CONVERSION;

	switch (idCmd) {
	case ID_COPY_PATH:
		switch (uType) {
		// Requesting a help string for the status bar.
		case GCS_HELPTEXTA:
		case GCS_HELPTEXTW:
			nFileCount = m_listFileNames.size();
			// Help text looks like "Copies the [full / short] path[s] to the clipboard. [n Objects Selected]"
			_stprintf(strFileCount, _T("%d Object%s Selected"), nFileCount, (nFileCount > 1 ? _T("s") : _T("")));			
			_stprintf(strHelpTextBuff, _T("Copies the %s file path%s%s to the clipboard. [%s]"),
				(bMakeShortPath ? _T("short") : _T("full")),
				(nFileCount > 1 ? _T("s") : _T("")),
				(bMakeCStyleString ? _T(" (C-Style)") : _T("")), strFileCount);

			if ((uType & GCS_HELPTEXTW) == GCS_HELPTEXTW) {
				wcsncpy((LPWSTR)pszName, T2W(strHelpTextBuff), cchMax);
				((LPWSTR)pszName)[cchMax - 1] = OLESTR('\0');
			}
			else {
				strncpy((LPSTR)pszName, T2A(strHelpTextBuff), cchMax);
				((LPSTR)pszName)[cchMax - 1] = '\0';
			}

			hr = S_OK;
			break;
			
		case GCS_VERB:
			hr = S_OK;
			break;

		case GCS_VALIDATE:
			hr = S_OK;
			break;
		}
		
	}
	return hr;
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 5.27.99
//* Function name	: CCopyPathContextMenu::HandleMenuMsg
//* Description	    : 
//***************************************
//
STDMETHODIMP CCopyPathContextMenu::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ATLTRACE(_T("CCopyPathContextMenu ---> IContextMenu2 interface method HandleMenuMsg() called!\n"));
	return S_OK;
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 5.27.99
//* Function name	: CCopyPathContextMenu::HandleMenuMsg2
//* Description	    : 
//***************************************
//
STDMETHODIMP CCopyPathContextMenu::HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
{
	ATLTRACE(_T("CCopyPathContextMenu ---> IContextMenu3 interface method HandleMenuMsg2() called!\n"));
	return S_OK;
}
