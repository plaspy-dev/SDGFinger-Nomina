#include "StdAfx.h"
#include "UsuarioHorario.h"
# include "sdgfingern.h"

CUsuarioHorario::CUsuarioHorario(CString id, CString valor, CString documento, CString departamento)
{
	m_szID= id;
	m_dValor= atof(valor);
	m_szDocumento= documento;
	m_szDepartamento= departamento;
}

CUsuarioHorario::~CUsuarioHorario(void)
{
	CleanAll();
}

double CUsuarioHorario::GetSalario() const {

	return m_dValor;
}

double CUsuarioHorario::GetValorHora() const {

	return m_dValor/240;
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
	m_szDocumento="";
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

void CUsuarioHorario::SetDocumento(const CString &documento) {

	m_szDocumento= documento;
}

CString CUsuarioHorario::GetDocumento() const {

	return m_szDocumento;
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

	CQuery query("horario");
	
	query["nombre"]=m_szNombre;

	if( m_szID.IsEmpty())
		return query.GetInsert();

	return query.GetUpdate("id=" + m_szID);
}

int CUsuarioHorario::GetQuerys(CStringArray &querys,CString szid) {

	querys.RemoveAll();
	querys.Add("DELETE FROM horarios WHERE usuario='" + szid + "'");

	CQuery query("horarios");

	query["usuario"]=szid;

	CHorarios *hor;
	CString desde, hasta;
	CString temp;

	for( int i=0; i<m_pHorarios.GetCount(); i++) {
		hor= (CHorarios *)m_pHorarios[i];
		temp.Format("%d", hor->GetTipo(desde, hasta));
		query["dia"]= temp;
		query["entrada"]= desde;
		query["salida"]= hasta;
		querys.Add(query.GetInsert());
	}
	return querys.GetCount();
}

int CUsuarioHorario::GetQuerys(CStringArray &querys) {
	
	querys.RemoveAll();
	querys.Add("DELETE FROM horariost WHERE turno=" + m_szID);

	CQuery query("horariost");

	query["turno"]= m_szID;

	CHorarios *hor;
	CString desde, hasta;
	CString temp;

	for( int i=0; i<m_pHorarios.GetCount(); i++) {
		hor= (CHorarios *)m_pHorarios[i];
		temp.Format("%d", hor->GetTipo(desde, hasta));
		query["dia"]= temp;
		query["entrada"]= desde;
		query["salida"]= hasta;
		querys.Add(query.GetInsert());
	}
	return querys.GetCount();
}

CString CUsuarioHorario::GetDepartamento() const {

	return m_szDepartamento;
}

void CUsuarioHorario::SetDepartamento(const CString &departamento) {

	m_szDepartamento= departamento;
}