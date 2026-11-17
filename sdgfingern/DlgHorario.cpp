// DlgHorario.cpp : implementation file
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "DlgHorario.h"

// CDlgHorario dialog

IMPLEMENT_DYNAMIC(CDlgHorario, CDialog)

CDlgHorario::CDlgHorario(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHorario::IDD, pParent)
{
	m_pActual=NULL;
	m_bEditable=true;
}

CDlgHorario::~CDlgHorario()
{
}

void CDlgHorario::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDIARIO, m_RDiario);
	DDX_Control(pDX, IDC_RDIARIOHOR, m_RDiarioHorario);
	DDX_Control(pDX, IDC_DDIARIOENTRADA, m_DTDiarioEntrada);
	DDX_Control(pDX, IDC_DDIARIOSALIDA, m_DTDiarioSalida);
	DDX_Control(pDX, IDC_DDIARIOTURNO, m_DTDiarioHoras);
	DDX_Control(pDX, IDC_CLUNES, m_CLunes);
	DDX_Control(pDX, IDC_CMARTES, m_CMartes);
	DDX_Control(pDX, IDC_CMIERCOLES, m_CMiercoles);
	DDX_Control(pDX, IDC_CJUEVES, m_CJueves);
	DDX_Control(pDX, IDC_CVIERNES, m_CViernes);
	DDX_Control(pDX, IDC_CSABADO, m_CSabado);
	DDX_Control(pDX, IDC_CDOMINGO, m_CDomingo);
	DDX_Control(pDX, IDC_RSEMANAL, m_RSemana);
	DDX_Control(pDX, IDC_DSEMANAHORA, m_DTSemanaHoras);
	DDX_Control(pDX, IDC_CALMUERZO, m_CAlmuerzo);
	DDX_Control(pDX, IDC_RALMUERZOHORARIO, m_RAlmuerzoHorario);
	DDX_Control(pDX, IDC_DALMUERZOSALIDA, m_DTAlmuerzoSalida);
	DDX_Control(pDX, IDC_DALMUERZOENTRADA, m_DTAlmuerzoEntrada);
	DDX_Control(pDX, IDC_DALMUERZOTURNO, m_DTAlmuerzoHoras);
	DDX_Control(pDX, IDC_CSABADOS, m_CSabados);
	DDX_Control(pDX, IDC_RSABADOSHORARIO, m_RSabadosHorario);
	DDX_Control(pDX, IDC_DSABADOSENTRADA, m_DTSabadosEntrada);
	DDX_Control(pDX, IDC_DSABADOSSALIDA, m_DTSabadosSalida);
	DDX_Control(pDX, IDC_DSABADOSTURNO, m_DTSabadosHoras);
	DDX_Control(pDX, IDC_CNOHABILES, m_CNoHabil);
	DDX_Control(pDX, IDC_RNOHABILESHORARIO, m_RNoHabilHorario);
	DDX_Control(pDX, IDC_DNOHABILESENTRADA, m_DTNoHabilEntrada);
	DDX_Control(pDX, IDC_DNOHABILESSALIDA, m_DTNoHabilSalida);
	DDX_Control(pDX, IDC_DNOHABILESTURNO, m_DTNoHabilHoras);
	DDX_Control(pDX, IDC_SFIN, m_SFin);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDOK, m_BAceptar);
}


BEGIN_MESSAGE_MAP(CDlgHorario, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedBaceptar)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedBcancelar)
	ON_BN_CLICKED(IDC_RDIARIOHOR, OnBnClickedRdiariohor)
	ON_BN_CLICKED(IDC_RDIARIOTURNO, OnBnClickedRdiariohor)
	ON_BN_CLICKED(IDC_RDIARIO, OnBnClickedRdiario)
	ON_BN_CLICKED(IDC_RSEMANAL, OnBnClickedRsemanal)
	ON_BN_CLICKED(IDC_RALMUERZOHORARIO, OnBnClickedRalmuerzohorario)
	ON_BN_CLICKED(IDC_RALMUERZOTURNO, OnBnClickedRalmuerzohorario)
	ON_BN_CLICKED(IDC_RSABADOSHORARIO, OnBnClickedRsabadoshorario)
	ON_BN_CLICKED(IDC_RSABADOSTURNO, OnBnClickedRsabadoshorario)
	ON_BN_CLICKED(IDC_RNOHABILESHORARIO, OnBnClickedRnohabileshorario)
	ON_BN_CLICKED(IDC_RNOHABILESTURNO, OnBnClickedRnohabileshorario)
	ON_BN_CLICKED(IDC_CALMUERZO, OnBnClickedCalmuerzo)
	ON_BN_CLICKED(IDC_CSABADOS, OnBnClickedCsabados)
	ON_BN_CLICKED(IDC_CNOHABILES, OnBnClickedCnohabiles)
	ON_BN_CLICKED(IDC_CSABADO, &CDlgHorario::OnBnClickedCsabado)
	ON_BN_CLICKED(IDC_CDOMINGO, &CDlgHorario::OnBnClickedCdomingo)
END_MESSAGE_MAP()


// CDlgHorario message handlers

BOOL CDlgHorario::OnInitDialog()
{
	CDialog::OnInitDialog();

	COleDateTime otro;

	CString formatofecha;
	formatofecha="hh:mm tt";
	m_DTDiarioEntrada.SetFormat(formatofecha);
	m_DTDiarioSalida.SetFormat(formatofecha);
	m_DTAlmuerzoEntrada.SetFormat(formatofecha);
	m_DTAlmuerzoSalida.SetFormat(formatofecha);
	m_DTSabadosEntrada.SetFormat(formatofecha);
	m_DTSabadosSalida.SetFormat(formatofecha);
	m_DTNoHabilEntrada.SetFormat(formatofecha);
	m_DTNoHabilSalida.SetFormat(formatofecha);

	formatofecha="mm:ss";

	m_DTDiarioHoras.SetFormat(formatofecha);
	m_DTAlmuerzoHoras.SetFormat(formatofecha);
	m_DTSabadosHoras.SetFormat(formatofecha);
	m_DTSemanaHoras.SetFormat(formatofecha);
	m_DTNoHabilHoras.SetFormat(formatofecha);

	m_CAlmuerzo.SetNextSectionCtrl(m_CSabados);
	m_CSabados.SetNextSectionCtrl(m_CNoHabil);
	m_CNoHabil.SetNextSectionCtrl(m_SFin);

	m_RDiarioHorario.SetCheck(1);
	m_RAlmuerzoHorario.SetCheck(1);
	m_RSabadosHorario.SetCheck(1);
	m_RNoHabilHorario.SetCheck(1);

	CTime tiempo(2000,1,1,8,0,0);
	CTime tiempos(2000,1,1,18,0,0);

	m_DTDiarioEntrada.SetTime(&tiempo);
	m_DTDiarioSalida.SetTime(&tiempos);
	m_DTSabadosEntrada.SetTime(&tiempo);
	m_DTSabadosSalida.SetTime(&tiempos);
	m_DTNoHabilEntrada.SetTime(&tiempo);
	m_DTNoHabilSalida.SetTime(&tiempos);

	tiempo= CTime(2000,1,1,12,0,0);
	m_DTAlmuerzoSalida.SetTime(&tiempo);
	tiempo= CTime(2000,1,1,13,0,0);
	m_DTAlmuerzoEntrada.SetTime(&tiempo);

	tiempo= CTime(2000,1,1,0,1,0);
	m_DTDiarioHoras.SetTime(&tiempo);
	m_DTAlmuerzoHoras.SetTime(&tiempo);
	m_DTSabadosHoras.SetTime(&tiempo);
	m_DTNoHabilHoras.SetTime(&tiempo);
	tiempo= CTime(2000,1,1,0,48,0);
	m_DTSemanaHoras.SetTime(&tiempo);

	m_CLunes.SetCheck(1);
	m_CMartes.SetCheck(1);
	m_CMiercoles.SetCheck(1);
	m_CJueves.SetCheck(1);
	m_CViernes.SetCheck(1);

	m_RDiario.SetCheck(1);
	m_CAlmuerzo.SetCheck(0);
	m_CSabados.SetCheck(0);
	m_CNoHabil.SetCheck(0);

	UpdateSemana(false);

	m_CAlmuerzo.UpdateControlledDlgItems();
	m_CSabados.UpdateControlledDlgItems();
	m_CNoHabil.UpdateControlledDlgItems();

	m_BAceptar.EnableWindow(m_bEditable);

	OnEditar();

	return TRUE;
}

void CDlgHorario::OnEditar() {

	if(!m_pActual)
		return;

	COleDateTime *desde=NULL, *hasta=NULL;
	m_CNoHabil.SetCheck(0);
	m_CAlmuerzo.SetCheck(0);
	m_CSabados.SetCheck(0);
	CButton *radio=NULL;

	for( int i=0; i<m_pActual->GetHorarios(); i++) {

		int index=m_pActual->GetTipo(i);
		desde= m_pActual->GetDesde(i);
		hasta= m_pActual->GetHasta(i);

		switch(index)
		{
		case CHorarios::dia_todasemana:
			m_RSemana.SetCheck(1);
			m_RDiario.SetCheck(0);
			ASSERT(hasta==NULL);
			m_DTSemanaHoras.SetTime(*desde);
			UpdateSemana(true);
			break;
		case CHorarios::dia_almuerzo:
			m_CAlmuerzo.SetCheck(1);
			m_RAlmuerzoHorario.SetCheck(hasta!=NULL);
			radio=(CButton *) GetDlgItem(IDC_RALMUERZOTURNO);
			if(radio)
				radio->SetCheck(hasta==NULL);
			if( hasta) {
				m_DTAlmuerzoSalida.SetTime(*desde);
				m_DTAlmuerzoEntrada.SetTime(*hasta);
			} else {
				m_DTAlmuerzoHoras.SetTime(*desde);
			}
			break;
		case CHorarios::dia_festivo:
			m_CNoHabil.SetCheck(1);
			m_RNoHabilHorario.SetCheck(hasta!=NULL);
			radio=(CButton *) GetDlgItem(IDC_RNOHABILESTURNO);
			if(radio)
				radio->SetCheck(hasta==NULL);
			if (hasta)
			{
				m_DTNoHabilEntrada.SetTime(*desde);
				m_DTNoHabilSalida.SetTime(*hasta);
			} 
			else
			{
				m_DTNoHabilHoras.SetTime(*desde);
			}
			break;
		case CHorarios::dia_sabado:
			m_CSabados.SetCheck(1);
			m_RSabadosHorario.SetCheck(hasta!=NULL);
			radio=(CButton *) GetDlgItem(IDC_RSABADOSTURNO);
			if(radio)
				radio->SetCheck(hasta==NULL);
			if(hasta) {
				m_DTSabadosEntrada.SetTime(*desde);
				m_DTSabadosSalida.SetTime(*hasta);
			} else {
				m_DTSabadosHoras.SetTime(*desde);
			}
			break;
		default: // todos los dias
			m_RSemana.SetCheck(0);
			m_RDiario.SetCheck(1);
			m_RDiarioHorario.SetCheck(hasta!=NULL);
			radio=(CButton *) GetDlgItem(IDC_RDIARIOTURNO);
			if(radio)
				radio->SetCheck(hasta==NULL);
			if(hasta) {
				m_DTDiarioEntrada.SetTime(*desde);
				m_DTDiarioSalida.SetTime(*hasta);
			} else {
				m_DTDiarioHoras.SetTime(*desde);
			}
			m_CLunes.SetCheck(index&1);
			m_CMartes.SetCheck(index&2);
			m_CMiercoles.SetCheck(index&4);
			m_CJueves.SetCheck(index&8);
			m_CViernes.SetCheck(index&16);
			m_CSabado.SetCheck(index&32);
			m_CDomingo.SetCheck(index&64);
			UpdateSemana(false);
			break;
		}
	}
	m_CSabados.UpdateControlledDlgItems();
	m_CNoHabil.UpdateControlledDlgItems();
	m_CAlmuerzo.UpdateControlledDlgItems();
	OnBnClickedCnohabiles();
	OnBnClickedCsabados();
	OnBnClickedCalmuerzo();
}

void CDlgHorario::OnBnClickedBaceptar()
{
	CString nombre;

	int tipo=0;
	if( m_RDiario.GetCheck()) {
		if( m_CLunes.GetCheck())
			tipo|=1;
		if(m_CMartes.GetCheck())
			tipo|=2;
		if(m_CMiercoles.GetCheck())
			tipo|=4;
		if(m_CJueves.GetCheck())
			tipo|=8;
		if(m_CViernes.GetCheck())
			tipo|=16;
		if(m_CSabado.GetCheck())
			tipo|=32;
		if(m_CDomingo.GetCheck())
			tipo|=64;
		if( tipo==0) {
			SetErrorEx(&m_CLunes, "Por lo menos seleccione un dia",2,"horarios");
			return;
		}
	}

	m_pActual= NULL;
	m_pActual= new CUsuarioHorario();

	m_pActual->CleanAll();
	m_pActual->SetNombre("(otro)");

	CTime desde, hasta;

	if( m_RDiario.GetCheck()) {
		ASSERT(tipo);
		if( m_RDiarioHorario.GetCheck()) {
			m_DTDiarioEntrada.GetTime(desde);
			m_DTDiarioSalida.GetTime(hasta);
		} else {
			m_DTDiarioHoras.GetTime(desde);
		}
		m_pActual->AddTipo(tipo, &desde,m_RDiarioHorario.GetCheck() ? &hasta : NULL);
	} else if( m_RSemana.GetCheck()) {
		m_DTSemanaHoras.GetTime(desde);
		m_pActual->AddTipo(256, &desde);
	}
	if( m_CAlmuerzo.GetCheck()) {
		if( m_RAlmuerzoHorario.GetCheck()) {
			m_DTAlmuerzoEntrada.GetTime(hasta);
			m_DTAlmuerzoSalida.GetTime(desde);
		} else {
			m_DTAlmuerzoHoras.GetTime(desde);
		}
		m_pActual->AddTipo(128, &desde, m_RAlmuerzoHorario.GetCheck() ? &hasta :NULL);
	}
	if( m_CSabados.GetCheck()) {
		if( m_RSabadosHorario.GetCheck()) {
			m_DTSabadosEntrada.GetTime(desde);
			m_DTSabadosSalida.GetTime(hasta);
		} else {
			m_DTSabadosHoras.GetTime(desde);
		}
		m_pActual->AddTipo(32, &desde, m_RSabadosHorario.GetCheck() ? &hasta :NULL);
	}
	if( m_CNoHabil.GetCheck()) {
		if( m_RNoHabilHorario.GetCheck()) {
			m_DTNoHabilEntrada.GetTime(desde);
			m_DTNoHabilSalida.GetTime(hasta);
		} else {
			m_DTNoHabilHoras.GetTime(desde);
		}
		m_pActual->AddTipo(64, &desde, m_RNoHabilHorario.GetCheck() ? &hasta :NULL);
	}
	CDialog::OnOK();
}

void CDlgHorario::OnBnClickedBcancelar()
{
	m_pActual= NULL;
	CDialog::OnCancel();
}

void CDlgHorario::OnBnClickedRdiariohor()
{
	m_DTDiarioEntrada.EnableWindow(m_RDiarioHorario.GetCheck());
	m_DTDiarioSalida.EnableWindow(m_RDiarioHorario.GetCheck());
	m_DTDiarioHoras.EnableWindow(!m_RDiarioHorario.GetCheck());
}

void CDlgHorario::OnBnClickedRdiario()
{
	UpdateSemana(false);
}

void CDlgHorario::OnBnClickedRsemanal()
{
	UpdateSemana(true);
}

void CDlgHorario::UpdateSemana(bool semana) {

	if(semana)
		m_RDiario.SetCheck(!m_RSemana.GetCheck());
	else m_RSemana.SetCheck(!m_RDiario.GetCheck());
	m_DTSemanaHoras.EnableWindow(m_RSemana.GetCheck());
	m_CLunes.EnableWindow(m_RDiario.GetCheck());
	m_CMartes.EnableWindow(m_RDiario.GetCheck());
	m_CMiercoles.EnableWindow(m_RDiario.GetCheck());
	m_CJueves.EnableWindow(m_RDiario.GetCheck());
	m_CViernes.EnableWindow(m_RDiario.GetCheck());
	m_CSabado.EnableWindow(m_RDiario.GetCheck());
	m_CDomingo.EnableWindow(m_RDiario.GetCheck());
	m_DTDiarioEntrada.EnableWindow(m_RDiario.GetCheck());
	m_DTDiarioSalida.EnableWindow(m_RDiario.GetCheck());
	m_DTDiarioHoras.EnableWindow(m_RDiario.GetCheck());
	m_RDiarioHorario.EnableWindow(m_RDiario.GetCheck());
	CWnd *handle= GetDlgItem(IDC_RDIARIOTURNO);
	if(handle)
		handle->EnableWindow(m_RDiario.GetCheck());
	if( m_RDiario.GetCheck())
		OnBnClickedRdiariohor();
}

void CDlgHorario::OnBnClickedRalmuerzohorario()
{
	m_DTAlmuerzoEntrada.EnableWindow(m_RAlmuerzoHorario.GetCheck());
	m_DTAlmuerzoSalida.EnableWindow(m_RAlmuerzoHorario.GetCheck());
	m_DTAlmuerzoHoras.EnableWindow(!m_RAlmuerzoHorario.GetCheck());
}

void CDlgHorario::OnBnClickedRsabadoshorario()
{
	m_DTSabadosEntrada.EnableWindow(m_RSabadosHorario.GetCheck());
	m_DTSabadosSalida.EnableWindow(m_RSabadosHorario.GetCheck());
	m_DTSabadosHoras.EnableWindow(!m_RSabadosHorario.GetCheck());
}

void CDlgHorario::OnBnClickedRnohabileshorario()
{
	m_DTNoHabilEntrada.EnableWindow(m_RNoHabilHorario.GetCheck());
	m_DTNoHabilSalida.EnableWindow(m_RNoHabilHorario.GetCheck());
	m_DTNoHabilHoras.EnableWindow(!m_RNoHabilHorario.GetCheck());
}

void CDlgHorario::OnBnClickedCalmuerzo()
{
	if( m_CAlmuerzo.GetCheck()) {
		OnBnClickedRalmuerzohorario();
	}
}

void CDlgHorario::OnBnClickedCsabados()
{
	if( m_CSabados.GetCheck()) {
		m_CSabado.SetCheck(false);
		OnBnClickedRsabadoshorario();
	}
}

void CDlgHorario::OnBnClickedCnohabiles()
{
	if( m_CNoHabil.GetCheck()) {
		m_CDomingo.SetCheck(false);
		OnBnClickedRnohabileshorario();
	}
}


void CDlgHorario::OnBnClickedCsabado()
{
	if(m_CSabado.GetCheck()) {
		m_CSabados.SetCheck(false);
		m_CSabados.UpdateControlledDlgItems();
	}
}

void CDlgHorario::OnBnClickedCdomingo()
{
	if(m_CDomingo.GetCheck()) {
		m_CNoHabil.SetCheck(false);	
		m_CNoHabil.UpdateControlledDlgItems();
	}
}
