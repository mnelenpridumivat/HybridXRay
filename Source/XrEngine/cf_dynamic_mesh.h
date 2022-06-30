#pragma once

#include "xr_collide_form.h"


class ENGINE_API	CCF_DynamicMesh:
public CCF_Skeleton
{
	typedef	 CCF_Skeleton inherited;
public:
						CCF_DynamicMesh	( CObject* _owner ) :CCF_Skeleton(_owner){};
};