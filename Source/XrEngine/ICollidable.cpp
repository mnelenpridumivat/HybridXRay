#include "stdafx.h"
#include "icollidable.h"
#include "xr_collide_form.h"

ICollidable::ICollidable()		
{
	collidable.model					=	NULL;		
};
ICollidable::~ICollidable()		
{
	xr_delete		( collidable.model );	
};
