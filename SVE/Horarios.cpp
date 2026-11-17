#include "StdAfx.h"
#include "Horarios.h"

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

	m_nTipo=tipo;
	m_bHasta= !hasta.IsEmpty();
	int hor=0;
	int min=0;
	if( m_bHasta) {
		hor= atoi(desde.Left(2));
		min= atoi(desde.Mid(3,2));
		m_pDesde.SetDateTime(m_pDesde.GetYear(), m_pDesde.GetMonth(), m_pDesde.GetDay(),hor, min, 0);
		hor= atoi(hasta.Left(2));
		min= atoi(hasta.Mid(3,2));
		m_pHasta.SetDateTime(m_pHasta.GetYear(), m_pHasta.GetMonth(), m_pHasta.GetDay(),hor, min, 0);
	} else {
		hor= atoi(desde.Mid(3,2));
		min= atoi(desde.Right(2));
		m_pDesde.SetDateTime(m_pDesde.GetYear(), m_pDesde.GetMonth(), m_pDesde.GetDay(),0, hor, min);
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
		if(m_pDesde.GetHour()<10)
			temp="0";
		temp+="%d:";
		if( m_pDesde.GetMinute()<10)
			temp+="0";
		temp+="%d:";
		temp+="00";
		desde.Format(temp, m_pDesde.GetHour(), m_pDesde.GetMinute());
		temp="";
		if(m_pHasta.GetHour()<10)
			temp="0";
		temp+="%d:";
		if(m_pHasta.GetMinute()<10)
			temp+="0";
		temp+="%d:";
		temp+="00";
		hasta.Format(temp, m_pHasta.GetHour(), m_pHasta.GetMinute());
	} else {
		temp+="00:";
		if(m_pDesde.GetMinute()<10)
			temp+="0";
		temp+="%d:";
		if( m_pDesde.GetSecond()<10)
			temp+="0";
		temp+="%d";
		desde.Format(temp, m_pDesde.GetMinute(), m_pDesde.GetSecond());
		hasta="";
	}
	return m_nTipo;
}
