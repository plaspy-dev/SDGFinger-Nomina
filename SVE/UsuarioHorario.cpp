#include "StdAfx.h"
#include "UsuarioHorario.h"
# include "sve.h"

CUsuarioHorario::CUsuarioHorario(CString id, CString valor)
{
	m_szID= id;
	m_dValor= atof(valor);
}

CUsuarioHorario::~CUsuarioHorario(void)
{
	CleanAll();
}

double CUsuarioHorario::GetValor() const {

	return m_dValor;
}

bool CUsuarioHorario::operator==(CUsuarioHorario &horario) {

	if(&horario==this)
		return true;
	if( horario.GetHorarios()!=GetHorarios())
		return false;
	CHorarios *hor, *segundo;
	bool found= false;
	for( int i=0; i<m_pHorarios.GetCount(); i++) {
		found=false;
		hor= (CHorarios *)m_pHorarios[i];
		for( int j=0; j<horario.m_pHorarios.GetCount(); j++) {
			segundo=(CHorarios *)horario.m_pHorarios[j];
			if((*hor)==(*segundo)) {
				found=true;
				break;
			}
		}
		if(!found)
			return false;
	}
	return true;
}

void CUsuarioHorario::Set(CString id, CString nombre) {
	m_szID=id;
	m_szNombre=nombre;
}

void CUsuarioHorario::SetNombre(CString nombre) {

	m_szNombre= nombre;
}

void CUsuarioHorario::CleanAll() {

	m_szNombre.Empty();
	CHorarios *hor;
	for( int i=0; i<m_pHorarios.GetCount(); i++) {
		hor= (CHorarios *)m_pHorarios[i];
		delete hor;
	}
	m_pHorarios.RemoveAll();
}

void CUsuarioHorario::SetId(CString id) {
	m_szID=id;
}

void CUsuarioHorario::AddTipo(int tipo, CString desde, CString hasta) {

	m_pHorarios.Add(new CHorarios(tipo, desde, hasta));
}

void CUsuarioHorario::AddTipo(int tipo, CTime *desde, CTime *hasta) {

	m_pHorarios.Add(new CHorarios(tipo, desde, hasta));
}

CString CUsuarioHorario::GetNombre() {

	return m_szNombre;
}

CString CUsuarioHorario::GetID() {

	return m_szID;
}

int CUsuarioHorario::GetHorarios() {
	return m_pHorarios.GetCount();
}

COleDateTime *CUsuarioHorario::GetDesde(int index) {

	CHorarios *hor=(CHorarios *)m_pHorarios[index];
	if(hor) 
		return hor->GetDesde();
	return NULL;
}

COleDateTime *CUsuarioHorario::GetHasta(int index) {

	CHorarios *hor=(CHorarios *)m_pHorarios[index];
	if(hor) 
		return hor->GetHasta();
	return NULL;
}

int CUsuarioHorario::GetTipo(int index) {

	CHorarios *hor=(CHorarios *)m_pHorarios[index];
	if(hor) 
		return hor->GetTipo();
	return 0;
}

CString CUsuarioHorario::GetQuery() {

	CString query;

	CStringArray valores;
	CStringArray campos;

	campos.Add("nombre");
	campos.Add("nReservado");

	valores.Add(m_szNombre);
	valores.Add("5");

	if( m_szID.IsEmpty())
		query= CQuery::CreateInsert("shared", campos, valores);
	else query= CQuery::CreateUpdate("shared", campos, valores, "id=" + m_szID);
	return query;
}

int CUsuarioHorario::GetQuerys(CStringArray &querys,CString szid) {

	querys.RemoveAll();
	querys.Add("DELETE FROM horarios WHERE usuario=" + szid);

	CStringArray valores;
	CStringArray campos;

	campos.Add("usuario");
	campos.Add("dia");
	campos.Add("entrada");
	campos.Add("salida");

	valores.Add(szid);
	valores.Add("");
	valores.Add("");
	valores.Add("");

	CHorarios *hor;
	CString desde, hasta;
	CString temp;

	for( int i=0; i<m_pHorarios.GetCount(); i++) {
		hor= (CHorarios *)m_pHorarios[i];
		temp.Format("%d", hor->GetTipo(desde, hasta));
		valores.SetAt(1,temp);
		valores.SetAt(2, desde);
		valores.SetAt(3, hasta);
		querys.Add(CQuery::CreateInsert("horarios", campos, valores));
	}
	return querys.GetCount();
}

int CUsuarioHorario::GetQuerys(CStringArray &querys) {
	
	querys.RemoveAll();
	querys.Add("DELETE FROM horariost WHERE turno=" + m_szID);

	CStringArray valores;
	CStringArray campos;

	campos.Add("turno");
	campos.Add("dia");
	campos.Add("entrada");
	campos.Add("salida");
	
	valores.Add(m_szID);
	valores.Add("");
	valores.Add("");
	valores.Add("");

	CHorarios *hor;
	CString desde, hasta;
	CString temp;

	for( int i=0; i<m_pHorarios.GetCount(); i++) {
		hor= (CHorarios *)m_pHorarios[i];
		temp.Format("%d", hor->GetTipo(desde, hasta));
		valores.SetAt(1,temp);
		valores.SetAt(2, desde);
		valores.SetAt(3, hasta);
		querys.Add(CQuery::CreateInsert("horariost", campos, valores));
	}
	return querys.GetCount();
}