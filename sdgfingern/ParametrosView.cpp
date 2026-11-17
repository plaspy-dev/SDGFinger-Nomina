// ParametrosView.cpp : implementation file
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "ParametrosView.h"


// CParametrosView

IMPLEMENT_DYNCREATE(CParametrosView, CFormView)

CParametrosView::CParametrosView()
	: CFormView(CParametrosView::IDD)
{
}

CParametrosView::~CParametrosView()
{
}

void CParametrosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DDESDE, m_DTDesde);
	DDX_Control(pDX, IDC_DHASTA, m_DTHasta);
	DDX_Control(pDX, IDC_EONOCTURNO, m_EONocturna);
	DDX_Control(pDX, IDC_EEDIURNA, m_EEDiurna);
	DDX_Control(pDX, IDC_EENOCTURNA, m_EENocturna);
	DDX_Control(pDX, IDC_EODOMINCAL, m_EODominical);
	DDX_Control(pDX, IDC_EEDDOMINICAL, m_EEDDominical);
	DDX_Control(pDX, IDC_EENDOMINICAL, m_EENDominical);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_ERECARGONOCTURNO, m_ERecargoNocturno);
	DDX_Control(pDX, IDC_ESMMLV, m_ESMMLV);
	DDX_Control(pDX, IDC_EAUXILIOTRANSPORTE, m_EAuxilioTransporte);
}

BEGIN_MESSAGE_MAP(CParametrosView, CFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CParametrosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CParametrosView::OnBnClickedBcancelar)
END_MESSAGE_MAP()


// CParametrosView diagnostics

#ifdef _DEBUG
void CParametrosView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CParametrosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CParametrosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Parámetros");

	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);
	
	m_DTDesde.SetFormat("hh:mm tt");
	m_DTHasta.SetFormat("hh:mm tt");

	OnBnClickedBcancelar();
}

void CParametrosView::OnBnClickedBaceptar()
{
	COleDateTime tiempo;

	m_DTDesde.GetTime(tiempo);
	VERIFY(AfxGetApp()->WriteProfileBinary("configuracion", "desde", (LPBYTE)&tiempo, sizeof(tiempo)));
	m_DTHasta.GetTime(tiempo);
	VERIFY(AfxGetApp()->WriteProfileBinary("configuracion", "hasta", (LPBYTE)&tiempo, sizeof(tiempo)));

	AfxGetApp()->WriteProfileString("configuracion", "nocturna", m_EONocturna.Number);
	AfxGetApp()->WriteProfileString("configuracion", "dominical", m_EODominical.Number);
	AfxGetApp()->WriteProfileString("configuracion", "diurna", m_EEDiurna.Number);
	AfxGetApp()->WriteProfileString("configuracion", "enocturna", m_EENocturna.Number);
	AfxGetApp()->WriteProfileString("configuracion", "edomical", m_EEDDominical.Number);
	AfxGetApp()->WriteProfileString("configuracion", "ndominical", m_EENDominical.Number);
	AfxGetApp()->WriteProfileString("configuracion", "recargon", m_ERecargoNocturno.Number);

	AfxGetApp()->WriteProfileString("configuracion", "smmlv", m_ESMMLV.Number);
	AfxGetApp()->WriteProfileString("configuracion", "auxiliotransporte", m_EAuxilioTransporte.Number);


	((CMDIFrameWndXP *)AfxGetMainWnd())->SendMessageBroadCast(WM_UPDATEHORARIOS, 0, 3);
	OnBnClickedBcancelar();
	SetError(NULL, "Configuracion Guardada.");
}

void CParametrosView::OnBnClickedBcancelar()
{
	COleDateTime *tiempo;
	UINT tamano;

	if(!AfxGetApp()->GetProfileBinary("configuracion", "desde", (LPBYTE *)&tiempo, &tamano)) {
		tiempo= new COleDateTime();
		tiempo->SetTime(6, 0, 0);
	}
	
	m_DTDesde.SetTime(*tiempo);
	delete tiempo;
	if(!AfxGetApp()->GetProfileBinary("configuracion", "hasta", (LPBYTE *)&tiempo, &tamano)) {
		tiempo= new COleDateTime();
		tiempo->SetTime(22, 0, 0);
	}
	m_DTHasta.SetTime(*tiempo);
	delete tiempo;

	m_EONocturna.Number= AfxGetApp()->GetProfileString("configuracion", "nocturna", "1.35");
	m_EODominical.Number= AfxGetApp()->GetProfileString("configuracion", "dominical", "1.75");
	m_EEDiurna.Number= AfxGetApp()->GetProfileString("configuracion", "diurna", "1.25");
	m_EENocturna.Number= AfxGetApp()->GetProfileString("configuracion", "enocturna", "1.75");
	m_EEDDominical.Number= AfxGetApp()->GetProfileString("configuracion", "edomical", "2");
	m_EENDominical.Number= AfxGetApp()->GetProfileString("configuracion", "ndominical", "2.5");
	m_ERecargoNocturno.Number= AfxGetApp()->GetProfileString("configuracion", "recargon", "0.35");

	m_ESMMLV.Number= AfxGetApp()->GetProfileString("configuracion", "smmlv", "515000");
	m_EAuxilioTransporte.Number= AfxGetApp()->GetProfileString("configuracion", "auxiliotransporte", "61500");
}
