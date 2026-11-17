// DlgConfigurar.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "DlgConfigurar.h"
#include <math.h>

// CDlgConfigurar dialog

IMPLEMENT_DYNAMIC(CDlgConfigurar, CDialog)

CDlgConfigurar::CDlgConfigurar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfigurar::IDD, pParent)
{

}

CDlgConfigurar::~CDlgConfigurar()
{
}

void CDlgConfigurar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DTOLERANCIA, m_DTTolerancia);
	DDX_Control(pDX, IDC_BALERTA2, m_BAlerta);
	DDX_Control(pDX, IDC_BEXTRAS, m_BExtra);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_CLARGAS, m_CFechas);
	DDX_Control(pDX, IDC_CDIAS, m_CDias);
	DDX_Control(pDX, IDC_CCALCULAR, m_BCalcular);
	DDX_Control(pDX, IDC_CHECK2, m_BDiferencia);
}


BEGIN_MESSAGE_MAP(CDlgConfigurar, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgConfigurar::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgConfigurar::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgConfigurar message handlers

BOOL CDlgConfigurar::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BAlerta.Color				= AfxGetApp()->GetProfileInt("informes", "alerta", RGB(255,0,0));
	m_BAlerta.DefaultColor			= RGB(255,0,0);
	m_BAlerta.TrackSelection		= TRUE;
	m_BAlerta.CustomText			= _T("Mas colores...");
	m_BAlerta.DefaultText			= _T("Automatico");
	m_BAlerta.SetColor(AfxGetApp()->GetProfileInt("informes", "alerta", RGB(255,0,0)));
	

	m_BExtra.Color				= AfxGetApp()->GetProfileInt("informes", "extra", RGB(0,255,0));
	m_BExtra.DefaultColor			= RGB(0,255,0);
	m_BExtra.TrackSelection		= TRUE;
	m_BExtra.CustomText			= _T("Mas colores...");
	m_BExtra.DefaultText			= _T("Automatico");
	m_BExtra.SetColor(AfxGetApp()->GetProfileInt("informes", "extra", RGB(0,255,0)));

	CString formatofecha="mm:ss";
	m_DTTolerancia.SetFormat(formatofecha);
	int segundos= AfxGetApp()->GetProfileInt("informes", "tolerancia", 15*60);

	CTime tiempo(2000,1,1,0,(int)floor((float)segundos/60),segundos%60,0);
	m_DTTolerancia.SetTime(&tiempo);

	m_CDias.SetCheck(AfxGetApp()->GetProfileInt("informes","todoslosdias",1));
	m_CFechas.SetCheck(AfxGetApp()->GetProfileInt("informes", "fechaslargas",1));

	m_BCalcular.SetCheck(AfxGetApp()->GetProfileInt("informes", "calcular", 0));
	m_BDiferencia.SetCheck(AfxGetApp()->GetProfileInt("informes", "diferencia", 0));

	return TRUE;
}

void CDlgConfigurar::OnBnClickedOk()
{
	AfxGetApp()->WriteProfileInt("informes", "alerta", m_BAlerta.GetColor());
	AfxGetApp()->WriteProfileInt("informes", "extra", m_BExtra.GetColor());
	CTime tiempo;
	m_DTTolerancia.GetTime(tiempo);
	AfxGetApp()->WriteProfileInt("informes", "tolerancia",tiempo.GetMinute()*60+tiempo.GetSecond());

	AfxGetApp()->WriteProfileInt("informes","todoslosdias",m_CDias.GetCheck());
	AfxGetApp()->WriteProfileInt("informes", "fechaslargas",m_CFechas.GetCheck());

	AfxGetApp()->WriteProfileInt("informes", "calcular", m_BCalcular.GetCheck());
	AfxGetApp()->WriteProfileInt("informes", "diferencia", m_BDiferencia.GetCheck());

	OnOK();
}

void CDlgConfigurar::OnBnClickedCancel()
{
	OnCancel();
}
