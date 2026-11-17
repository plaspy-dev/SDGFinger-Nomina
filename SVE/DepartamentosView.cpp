// DepartamentosView.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "DepartamentosView.h"
# include "DlgUsuarios.h"


// CDepartamentosView

IMPLEMENT_DYNCREATE(CDepartamentosView, CFormView)

CDepartamentosView::CDepartamentosView()
	: CFormView(CDepartamentosView::IDD)
{
	m_dwCurrentID=0;
}

CDepartamentosView::~CDepartamentosView()
{
}

void CDepartamentosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LDEPARTAMENTOS, m_LCDepartamentos);
	DDX_Control(pDX, IDC_EDEPARTAMENTO, m_EDepartamento);
	DDX_Control(pDX, IDC_EDESCRIPCION, m_EDescripcion);
	DDX_Control(pDX, IDC_EVALOR, m_EValor);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
}

BEGIN_MESSAGE_MAP(CDepartamentosView, CFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CDepartamentosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CDepartamentosView::OnBnClickedBcancelar)
	ON_EN_CHANGE(IDC_EDEPARTAMENTO, &CDepartamentosView::OnEnChangeEdepartamento)
	ON_NOTIFY(NM_DBLCLK, IDC_LDEPARTAMENTOS, &CDepartamentosView::OnNMDblclkLdepartamentos)
	ON_NOTIFY(NM_RCLICK, IDC_LDEPARTAMENTOS, &CDepartamentosView::OnNMRclickLdepartamentos)
	ON_MESSAGE(WM_MYSQL, OnMySQL)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LDEPARTAMENTOS, &CDepartamentosView::OnLvnKeydownLdepartamentos)
	ON_COMMAND(2000,OnEditar)
	ON_COMMAND(2001,OnEliminar)
	ON_COMMAND(2002,OnVerUsuarios)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDepartamentosView diagnostics

#ifdef _DEBUG
void CDepartamentosView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDepartamentosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDepartamentosView message handlers

void CDepartamentosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Departamentos");
	// este es el nReservado
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);

	m_LCDepartamentos.AddColumn("Departamento");
	m_LCDepartamentos.AddColumn("Descripción");
	m_LCDepartamentos.AddColumn("Valor Hora", CMyListCtrl::tipocol_double);
	m_LCDepartamentos.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_LCDepartamentos.SetOrdenable(true);
	m_LCDepartamentos.AutoSizeColumns();

	m_LCDepartamentos.InsertItem(0, "(Ninguno)");

	m_pConector->Query("SELECT id,nombre,descripcion,szReservado FROM shared WHERE nReservado=6 AND (szReservado IS NULL OR szReservado!=-1)", query_departamentos, this);
	m_BAceptar.EnableWindow(false);
}

LRESULT CDepartamentosView::OnMySQL(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess= (MySQLMessage *)lParam;
	switch(wParam) {
		case QUERY_RESULT:
			if(mess->GetID()==query_departamentos) {
				int pos= m_LCDepartamentos.InsertItem(0,mess->GetAt(1));
				m_LCDepartamentos.SetItemText(pos, 1, mess->GetAt(2));
				m_LCDepartamentos.SetItemText(pos, 2, CEditFloat::GetFormatNumber(mess->GetAt(3)));
				m_LCDepartamentos.SetItemData(pos, atoi(mess->GetAt(0)));
			} else if(mess->GetID()==query_insert) {
				for( int i=0; i<m_LCDepartamentos.GetItemCount(); i++) {
					if(m_LCDepartamentos.GetItemData(i)==-1) {
						m_LCDepartamentos.SetItemData(i, atol(mess->GetAt(0)));
						break;
					}
				}
			}
			break;
		case THREAD_END:
			if(mess->GetID()==query_departamentos) {
				m_LCDepartamentos.AutoSizeColumns();
			}
			break;
	}
	return 0;
}

void CDepartamentosView::OnBnClickedBaceptar()
{
	CStringArray campos;
	CStringArray valores;

	CString temp;

	m_EDepartamento.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_EDepartamento, "Por favor ingrese un nombre", 2, "departamentos");
		return;
	}
	valores.Add(temp);
	m_EDescripcion.GetWindowText(temp);
	valores.Add(temp);
	valores.Add(m_EValor.GetNumero());
	valores.Add("6");

	campos.Add("nombre");
	campos.Add("descripcion");
	campos.Add("szReservado");
	campos.Add("nReservado");

	int pos=0;

	if(m_dwCurrentID) {
		temp.Format("id=%d", m_dwCurrentID);
		m_pConector->Query(CQuery::CreateUpdate("shared", campos, valores, temp), 0, this);
		for( int i=0; i<m_LCDepartamentos.GetItemCount(); i++) {
			if( m_LCDepartamentos.GetItemData(i)==m_dwCurrentID) {
				pos=i;
				break;
			}
		}
		CStringArray valores1, campos1;
		campos1.Add("valorhora");
		valores1.Add(valores[2]);
		temp.Format("departamento=%d", m_dwCurrentID);
		m_pConector->Query(CQuery::CreateUpdate("usuario", campos1, valores1, temp), 0, this);
	} else {
		m_pConector->Query(CQuery::CreateInsert("shared", campos, valores), query_insert,this);
		pos= m_LCDepartamentos.InsertItem(m_LCDepartamentos.GetItemCount(), "");
		m_LCDepartamentos.SetItemData(pos, -1);
	}
	m_LCDepartamentos.SetItemText(pos, 0, valores[0]);
	m_LCDepartamentos.SetItemText(pos, 1, valores[1]);
	m_LCDepartamentos.SetItemText(pos, 2, CEditFloat::GetFormatNumber(valores[2]));

	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_UPDATEHORARIOS, 1);

	OnBnClickedBcancelar();
}

void CDepartamentosView::OnBnClickedBcancelar()
{	
	m_dwCurrentID=0;
	m_EDepartamento.SetWindowText("");
	m_EDescripcion.SetWindowText("");
	m_EValor.SetNumero("");
	GetDocument()->SetTitle("Departamentos");
	GetDocument()->SetTitle("Departamentos");
	SetError(NULL, "Listo");
}

void CDepartamentosView::OnEnChangeEdepartamento()
{
	m_BAceptar.EnableWindow(m_EDepartamento.GetWindowTextLength()>0);
}

void CDepartamentosView::OnNMDblclkLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	OnEditar();
	*pResult = 0;
}

void CDepartamentosView::OnNMRclickLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult)
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;
	menu.CreatePopupMenu();

	if(m_LCDepartamentos.GetItemData(pNMListView->iItem)) {

		menu.AppendMenu(MF_STRING, 2000, "&Editar");
		menu.AppendMenu(MF_STRING, 2001, "E&liminar");
		menu.AppendMenu(MF_SEPARATOR);
	}
	menu.AppendMenu(MF_STRING, 2002, "&Ver Usuarios");

	menu.SetDefaultItem(m_LCDepartamentos.GetItemData(pNMListView->iItem) ? 2000 : 2002);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CDepartamentosView::OnEditar() {

	int index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	if(m_LCDepartamentos.GetItemData(index)==0) {
		OnVerUsuarios();
		return;
	}
	m_EDepartamento.SetWindowText(m_LCDepartamentos.GetItemText(index, 0));
	m_EDescripcion.SetWindowText(m_LCDepartamentos.GetItemText(index, 1));
	m_EValor.SetFormatNumero(m_LCDepartamentos.GetItemText(index, 2));
	m_dwCurrentID= m_LCDepartamentos.GetItemData(index);
	GetDocument()->SetTitle("Departamento - " + m_LCDepartamentos.GetItemText(index, 0));
	GetDocument()->SetTitle("Departamento - " + m_LCDepartamentos.GetItemText(index, 0));
}

void CDepartamentosView::OnEliminar() {
	int index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1 || m_LCDepartamentos.GetItemData(index)==0)
		return;

	index= m_LCDepartamentos.GetSelectedCount();
	CString temp;
	temp.Format("Esta seguro de eliminar %s elemento%s seleccionado%s", index>1 ? "los" : "el", index>1 ? "s" : "", index>1 ? "s" : "");
	if(MessageBox(temp, NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {
		CStringArray valores;
		CStringArray campos;
		campos.Add("szReservado");
		valores.Add("-1");
		CString temp;

		while((index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED))!=-1) {
			
			if(m_LCDepartamentos.GetItemData(index)==0)
				continue;
			temp.Format("id=%d", m_LCDepartamentos.GetItemData(index));
			m_pConector->Query(CQuery::CreateUpdate("shared", campos, valores, temp), 0, this);
			if(m_LCDepartamentos.GetItemData(index)==m_dwCurrentID) {
				m_dwCurrentID=0;
				GetDocument()->SetTitle("Departamentos");
				GetDocument()->SetTitle("Departamentos");
			}
			m_LCDepartamentos.DeleteItem(index);
		}
	}
}

void CDepartamentosView::OnVerUsuarios() {
	int index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	CDlgUsuarios dlg;
	dlg.m_pConector= m_pConector;
	dlg.m_szCampo=" departamento ";
	index=m_LCDepartamentos.GetItemData(index);
	if(index) 
		dlg.m_szGrupo.Format("%d", index);
	if(dlg.DoModal()==IDOK) {
		AfxGetMainWnd()->SendMessage(WM_INSERTODS, 0, (LPARAM) atol(dlg.m_szGrupo));
	}
}

void CDepartamentosView::OnLvnKeydownLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if( pLVKeyDow->wVKey==VK_DELETE)
		OnEliminar();
	*pResult = 0;
}

void CDepartamentosView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(IsWindow(m_LCDepartamentos)) {
		CRect rect;
		m_LCDepartamentos.GetClientRect(rect);
		rect.bottom= cy-20;
		m_LCDepartamentos.MoveWindow(rect);
	}
}
