#include "stdafx.h"
#pragma hdrstop

#include "r_backend_hemi.h"

R_hemi::R_hemi()
{
	unmap();
}

void	R_hemi::unmap		()
{
	c_pos_faces = 0;
	c_neg_faces = 0;
	c_material	= 0;
}

void	R_hemi::set_pos_faces		(float posx, float posy, float posz)
{

}
void	R_hemi::set_neg_faces		(float negx, float negy, float negz)
{

}

void	R_hemi::set_material		(float x, float y, float z, float w)
{

}

