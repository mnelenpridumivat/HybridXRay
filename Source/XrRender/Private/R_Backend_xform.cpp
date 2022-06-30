#include "stdafx.h"
#pragma hdrstop

#include "r_backend_xform.h"

void	R_xforms::set_W			(const Fmatrix& m)
{

}
void	R_xforms::set_V			(const Fmatrix& m)
{

}
void	R_xforms::set_P			(const Fmatrix& m)
{
	
}

void	R_xforms::apply_invw()
{

}

void	R_xforms::unmap			()
{
	c_w			= NULL;
	c_invw		= NULL;
	c_v			= NULL;
	c_p			= NULL;
	c_wv		= NULL;
	c_vp		= NULL;
	c_wvp		= NULL;
}
R_xforms::R_xforms				()
{
	unmap			();
	m_w.identity	();
	m_invw.identity	();
	m_v.identity	();
	m_p.identity	();
	m_wv.identity	();
	m_vp.identity	();
	m_wvp.identity	();
	m_bInvWValid = true;
}
