// DlgUsuarios.cpp : implementation file
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "DlgUsuarios.h"


// CDlgUsuarios dialog

IMPLEMENT_DYNAMIC(CDlgUsuarios, CDialog)

CDlgUsuarios::CDlgUsuarios(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUsuarios::IDD, pParent)
{
	m_pConector=NULL;
	m_szCampo=" horario ";
}

CDlgUsuarios::~CDlgUsuarios()
{
}

void CDlgUsuarios::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LUSUARIOS, m_LCUsuarios);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
}


BEGIN_MESSAGE_MAP(CDlgUsuarios, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgUsuarios::OnBnClickedOk)
	ON_COMMAND(2010, &CDlgUsuarios::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LUSUARIOS, &CDlgUsuarios::OnNMDblclkLusuarios)
	ON_NOTIFY(NM_RCLICK, IDC_LUSUARIOS, &CDlgUsuarios::OnNMRclickLusuarios)
END_MESSAGE_MAP()


// CDlgUsuarios message handlers

void CDlgUsuarios::OnBnClickedOk()
{
	int index= m_LCUsuarios.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1) {
		OnCancel();
		return;
	}
	m_szGrupo= m_LCUsuarios.GetItemText(index, 0);
	OnOK();
}

BOOL CDlgUsuarios::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT(m_pConector);

	if(m_szGrupo.IsEmpty())
		m_szGrupo=" IS NULL";
	else m_szGrupo= "=" + m_szGrupo;

	m_LCUsuarios.InsertColumn(0, "Documento", LVCFMT_RIGHT);
	m_LCUsuarios.InsertColumn(1, "Nombre");

	m_LCUsuarios.SetType(0, CMyListCtrl::tipocol_numero);
	m_LCUsuarios.SetMessageLista("No hay usuarios");
	m_LCUsuarios.SetOrdenable();
	m_LCUsuarios.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CStringArray valores;
	int pos=-1;
	if(m_pConector->Find("SELECT Userid,Name FROM Userinfo WHERE " + m_szCampo + m_szGrupo + " ORDER BY Name", this, &valores)>0) {
		for( int i=0; i<valores.GetCount(); i++) {
			if(i%2) {
				ASSERT(pos!=-1);
				m_LCUsuarios.SetItemText(pos, 1, valores[i]);
			} else {
				pos= m_LCUsuarios.InsertItem(m_LCUsuarios.GetItemCount(), valores[i]);
			}
		}
	}
	if(m_LCUsuarios.GetItemCount()) 
		m_LCUsuarios.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_LCUsuarios.AutoSizeColumns();
	return TRUE;
}

void CDlgUsuarios::OnNMDblclkLusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	OnBnClickedOk();
	*pResult = 0;
}

void CDlgUsuarios::OnNMRclickLusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, 2010, "&Ver Usuario");
	menu.SetDefaultItem(2010);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}
