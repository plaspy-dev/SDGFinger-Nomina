// SVE.h: archivo de encabezado principal para la aplicación SVE
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Símbolos principales

# include <iofile/iofile.h>
# include <jfinger/jfinger.h>


// mensajes
# define WM_INSERTODS			2000		
// # define WM_QUERYUSUARIO		2001
# define WM_NEWXML				2005
# define WM_UPDATEHORARIOS		2006


// id
# define GRUPO_ADMINISTRADOR		1
# define GRUPO_USUARIOS				2
# define GRUPO_USUARIOSAVANZADOS	4

// CSVEApp:
// Consulte la sección SVE.cpp para obtener información sobre la implementación de esta clase
//

class CSVEApp : public CWinAppEx
{
public:
	CSVEApp();
// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CSVEApp theApp;
