#include "stdafx.h"
#pragma hdrstop

#include "PhysicsShellHolderEditorBase.h"
#	include "GameMtlLib.h"
//CObjectList	 Objects;
static void SetBoneMaterials( IKinematics &K )
{
	const u16 count =  K.LL_BoneCount();
    for(u16 i = 0; i < count; ++i)
    {
		CBoneData& bd =  K.LL_GetData( i );
        if (*(bd.game_mtl_name))
	        bd.game_mtl_idx = GameMaterialLibrary->GetMaterialIdx( bd.game_mtl_name.c_str() );
        else
        	bd.game_mtl_idx = 0 ;
    }
}


void  CPhysicsShellHolderEditorBase::CreatePhysicsShell( Fmatrix*	obj_xform )
{

}

void  CPhysicsShellHolderEditorBase::DeletePhysicsShell	()
{
}

void  CPhysicsShellHolderEditorBase::UpdateObjectXform(Fmatrix &obj_xform)
{
}
 void			CPhysicsShellHolderEditorBase::ApplyDragForce		( const Fvector &force )
 {
 }
