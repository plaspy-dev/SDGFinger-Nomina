// sdgfingern.h: archivo de encabezado principal del archivo DLL de sdgfingern
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif
# define WM_UPDATEHORARIOS	WM_USER+5102
# define WM_INSERTODS		WM_USER+5103

# include <ConectorODBC/ConectorODBC.h>
# include <IOFile/IOFile.h>

#include "resource.h"		// Símbolos principales


// CsdgfingernApp
// Consultar sdgfingern.cpp para realizar la implementación de esta clase
//

class CsdgfingernApp : public CWinApp
{
public:
	CsdgfingernApp();

// Reemplazos
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
