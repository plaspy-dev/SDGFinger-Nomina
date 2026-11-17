// Template.h: archivo de encabezado principal para la aplicación Template
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales
# include <IOFile/IOFile.h>
# include <JPrint/JPrint.h>

class CTemplateApp : public CWinAppXP
{
public:
	CTemplateApp();

// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateSave(CCmdUI *pCmdUI);
};

extern CTemplateApp theApp;