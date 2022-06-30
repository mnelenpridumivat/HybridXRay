//----------------------------------------------------
// file: PSLibrary.cpp
//----------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "PSLibrary.h"
#include "ParticleEffect.h"

#ifdef REDITOR
#	include "Editor/ParticleEffectActions.h"
#include "Editor/ui_main.h"
#endif

#define _game_data_			"$game_data$"

bool ped_sort_pred	(const PS::CPEDef* a, 	const PS::CPEDef* b)	{	return xr_strcmp(a->Name(),b->Name())<0;}

bool ped_find_pred	(const PS::CPEDef* a, 	LPCSTR b)				{	return xr_strcmp(a->Name(),b)<0;}
//----------------------------------------------------
void CPSLibrary::OnCreate()
{
}
 
void CPSLibrary::OnDestroy()
{

}
//----------------------------------------------------
PS::PEDIt CPSLibrary::FindPEDIt(LPCSTR Name)
{
	if (!Name) return m_PEDs.end();
#ifdef REDITOR
	for (PS::PEDIt it=m_PEDs.begin(); it!=m_PEDs.end(); it++)
    	if (0==xr_strcmp((*it)->Name(),Name)) return it;
	return m_PEDs.end();
#else
	PS::PEDIt I = std::lower_bound(m_PEDs.begin(),m_PEDs.end(),Name,ped_find_pred);
	if (I==m_PEDs.end() || (0!=xr_strcmp((*I)->m_Name,Name)))	return m_PEDs.end();
	else														return I;
#endif
}

PS::PGDIt CPSLibrary::FindPGDIt(LPCSTR Name)
{
	if (!Name) return m_PGDs.end();
#ifdef REDITOR
	return m_PGDs.end();
#else
	PS::PGDIt I = std::lower_bound(m_PGDs.begin(),m_PGDs.end(),Name,pgd_find_pred);
	if (I==m_PGDs.end() || (0!=xr_strcmp((*I)->m_Name,Name)))	return m_PGDs.end();
	else														return I;
#endif
}

void CPSLibrary::RenamePED(PS::CPEDef* src, LPCSTR new_name)
{
}

void CPSLibrary::Remove(const char* nm)
{
}
//----------------------------------------------------
bool CPSLibrary::Load2()
{
	return true;
}


bool CPSLibrary::Load(const char* nm)
{
    return false;
}
//----------------------------------------------------
void CPSLibrary::Reload()
{
}
//----------------------------------------------------
