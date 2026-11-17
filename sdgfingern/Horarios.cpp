#include "StdAfx.h"
#include "Horarios.h"
# include <Componentes/Componentes.h>

CHorarios::CHorarios(void)
{
	m_nTipo=0;
	m_bHasta=false;
	m_pDesde= COleDateTime::GetCurrentTime();
	m_pHasta= COleDateTime::GetCurrentTime();
}

CHorarios::~CHorarios(void)
{
}

CHorarios::CHorarios(int tipo, CTime *desde, CTime *hasta) {
	m_pDesde= COleDateTime::GetCurrentTime();
	m_pHasta= COleDateTime::GetCurrentTime();
	SetTipo(tipo, desde, hasta);
}

CHorarios::CHorarios(int tipo, CString desde, CString hasta) {
	m_pDesde= COleDateTime::GetCurrentTime();
	m_pHasta= COleDateTime::GetCurrentTime();
	SetTipo(tipo, desde, hasta);
}

bool CHorarios::operator ==(CHorarios &horario) {

	if( &horario==this)
		return true;
	if( horario.m_nTipo!=m_nTipo)
		return false;
	if(m_pDesde!=horario.m_pDesde)
		return false;
	if( m_bHasta!= horario.m_bHasta)
		return false;
	if( m_bHasta) {
		if( m_pHasta!=horario.m_pHasta)
			return false;
	}
	return true;
}

void CHorarios::SetTipo(int tipo, CString desde, CString hasta) {

	COleDateTime tdesde, thasta;
	tdesde.ParseDateTime(desde);
	thasta.ParseDateTime(hasta);

	m_nTipo=tipo;
	m_bHasta= !hasta.IsEmpty();

	if( m_bHasta) {
		m_pDesde.SetDateTime(m_pDesde.GetYear(), m_pDesde.GetMonth(), m_pDesde.GetDay(),tdesde.GetHour(), tdesde.GetMinute(), 0);
		m_pHasta.SetDateTime(m_pHasta.GetYear(), m_pHasta.GetMonth(), m_pHasta.GetDay(),thasta.GetHour(), thasta.GetMinute(), 0);
	} else {
		m_pDesde.SetDateTime(m_pDesde.GetYear(), m_pDesde.GetMonth(), m_pDesde.GetDay(),0, tdesde.GetMinute(), tdesde.GetSecond());
	}
}

void CHorarios::SetTipo(int tipo, CTime *desde, CTime *hasta) {

	m_nTipo=tipo;
	m_bHasta= hasta!=NULL;
	ASSERT(desde);
	if(m_bHasta) {
		m_pDesde.SetDateTime(m_pDesde.GetYear(), m_pDesde.GetMonth(), m_pDesde.GetDay(),desde->GetHour(), desde->GetMinute(), 0);
		m_pHasta.SetDateTime(m_pHasta.GetYear(), m_pHasta.GetMonth(), m_pHasta.GetDay(),hasta->GetHour(), hasta->GetMinute(), 0);
	} else {
		m_pDesde.SetDateTime(m_pDesde.GetYear(), m_pDesde.GetMonth(), m_pDesde.GetDay(),0, desde->GetMinute(), desde->GetSecond());
	}
}

int CHorarios::GetTipo() {

	return m_nTipo;
}

COleDateTime *CHorarios::GetDesde() {
	
	return &m_pDesde;
}

COleDateTime *CHorarios::GetHasta() {

	return m_bHasta ? &m_pHasta : NULL;
}

int CHorarios::GetTipo(CString &desde, CString &hasta) {

	CString temp;
	if(m_bHasta) {

		desde = CMyTime(m_pDesde).GetFechaHora(false);
		hasta= CMyTime(m_pHasta).GetFechaHora(false);
	} else {
		desde = CMyTime(m_pDesde).GetFechaHora(false);
		hasta="";
	}
	return m_nTipo;
}
