// DlgOpciones.cpp : implementation file
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "DlgOpciones.h"


// CDlgOpciones dialog

IMPLEMENT_DYNAMIC(CDlgOpciones, CDialog)

CDlgOpciones::CDlgOpciones(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOpciones::IDD, pParent)
{

}

CDlgOpciones::~CDlgOpciones()
{
}

void CDlgOpciones::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CPROTECTOR, m_BProtector);
	DDX_Control(pDX, IDC_CDISPLAY, m_BDisplay);
	DDX_Control(pDX, IDC_EDIRECCION, m_EDireccion);
	DDX_Control(pDX, IDC_BCERRAR, m_BCerrar);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_CCLIENTE, m_BCliente);
}


BEGIN_MESSAGE_MAP(CDlgOpciones, CDialog)
	ON_BN_CLICKED(IDC_BCERRAR, &CDlgOpciones::OnBnClickedBcerrar)
	ON_BN_CLICKED(IDOK, &CDlgOpciones::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CDISPLAY, &CDlgOpciones::OnBnClickedCdisplay)
	ON_BN_CLICKED(IDC_CCLIENTE, &CDlgOpciones::OnBnClickedCcliente)
END_MESSAGE_MAP()


// CDlgOpciones message handlers

void CDlgOpciones::OnBnClickedBcerrar()
{
	AfxGetApp()->WriteProfileInt("", "autostart", 0);
	MessageBox("Debe Reiniciar " + CApplicacion::GetAppName() + " Para efectuar los cambios.", NULL, MB_ICONINFORMATION);
	OnCancel();
}

void CDlgOpciones::OnBnClickedOk()
{
	AfxGetApp()->WriteProfileInt("opciones", "protector", m_BProtector.GetCheck());
	AfxGetApp()->WriteProfileInt("opciones", "display", m_BDisplay.GetCheck());
	CString temp;
	m_EDireccion.GetWindowText(temp);
	AfxGetApp()->WriteProfileInt("opciones", "direccion", atoi(temp));
	AfxGetApp()->WriteProfileInt("cliente", "reemplazar", m_BCliente.GetCheck());

	if(AfxGetApp()->GetProfileInt("cliente", "reemplazar", 0)) 
		CScreenSaverDE::EnableScreenSaver(AfxGetApp()->GetProfileInt("opciones", "protector", 1)==0);
	OnOK();
}

void CDlgOpciones::OnBnClickedCdisplay()
{
	m_EDireccion.EnableWindow(m_BDisplay.GetCheck());
}

BOOL CDlgOpciones::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BCliente.SetCheck(AfxGetApp()->GetProfileInt("cliente", "reemplazar", 0));
	m_BProtector.SetCheck(AfxGetApp()->GetProfileInt("opciones", "protector", 1));
	m_BDisplay.SetCheck(AfxGetApp()->GetProfileInt("opciones", "display", 0));
	m_BCerrar.EnableWindow(AfxGetApp()->GetProfileInt("", "autostart", 0));
	CString temp;
	temp.Format("%d", AfxGetApp()->GetProfileInt("opciones", "direccion", 888));
	m_EDireccion.SetWindowText(temp);

	m_BCliente.SetNextSectionCtrl(m_BAceptar);
	m_BCliente.UpdateControlledDlgItems();

	OnBnClickedCdisplay();
	OnBnClickedCcliente();

	return TRUE;
}

void CDlgOpciones::OnBnClickedCcliente()
{

}
