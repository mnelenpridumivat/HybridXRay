////////////////////////////////////////////////////////////////////////////
//	Module 		: inventory_item.cpp
//	Created 	: 24.03.2003
//  Modified 	: 29.01.2004
//	Author		: Victor Reutsky, Yuri Dobronravin
//	Description : Inventory item
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "inventory_item.h"
#include "inventory_item_impl.h"
#include "inventory.h"
#include "Physics.h"
#include "xrserver_objects_alife.h"
#include "xrserver_objects_alife_items.h"
#include "entity_alive.h"
#include "Level.h"
#include "game_cl_base.h"
#include "Actor.h"
#include "string_table.h"
#include	"../XrRender/Public/KinematicsAnimated.h"
#include	"../XrRender/Public/Kinematics.h"
#include "ai_object_location.h"
#include "../XrEngine/object_broker.h"
#include "../XrEngine/igame_persistent.h"

#ifdef DEBUG
#	include "debug_renderer.h"
#endif

#define ITEM_REMOVE_TIME		30000
struct net_update_IItem {	u32					dwTimeStamp;
SPHNetState			State;};
struct net_updateData{



	xr_deque<net_update_IItem>	NET_IItem;
	/// spline coeff /////////////////////
	float			SCoeff[3][4];

#ifdef DEBUG
	DEF_VECTOR		(VIS_POSITION, Fvector);
	VIS_POSITION	LastVisPos;
#endif

	Fvector			IStartPos;
	Fquaternion		IStartRot;

	Fvector			IRecPos;
	Fquaternion		IRecRot;

	Fvector			IEndPos;
	Fquaternion		IEndRot;	

	SPHNetState		LastState;
	SPHNetState		RecalculatedState;

#ifdef DEBUG
	SPHNetState		CheckState;
#endif
	SPHNetState		PredictedState;

	u32				m_dwIStartTime;
	u32				m_dwIEndTime;
	u32				m_dwILastUpdateTime;
};

net_updateData* CInventoryItem::NetSync()			
{
	if(!m_net_updateData) 
		m_net_updateData = xr_new<net_updateData>();
	return m_net_updateData;
}

CInventoryItem::CInventoryItem() 
{
	m_net_updateData	= NULL;
	m_slot				= NO_ACTIVE_SLOT;
	m_flags.set			(Fbelt,FALSE);
	m_flags.set			(Fruck,TRUE);
	m_flags.set			(FRuckDefault,TRUE);
	m_pCurrentInventory	= NULL;

	SetDropManual		(FALSE);

	m_flags.set			(FCanTake,TRUE);
	m_flags.set			(FCanTrade,TRUE);
	m_flags.set			(FUsingCondition,FALSE);
	m_fCondition		= 1.0f;

	m_name = m_nameShort = NULL;

	m_eItemPlace		= eItemPlaceUndefined;
	m_Description		= "";
}

CInventoryItem::~CInventoryItem() 
{
	delete_data			(m_net_updateData);

	bool B_GOOD			= (	!m_pCurrentInventory || 
							(std::find(	m_pCurrentInventory->m_all.begin(),m_pCurrentInventory->m_all.end(), this)==m_pCurrentInventory->m_all.end()) );
	if(!B_GOOD)
	{
		CObject* p	= object().H_Parent();
		Msg("inventory ptr is [%s]",m_pCurrentInventory?"not-null":"null");
		if(p)
			Msg("parent name is [%s]",p->cName().c_str());

			Msg("! ERROR item_id[%d] H_Parent=[%s][%d] [%d]",
				object().ID(),
				p ? p->cName().c_str() : "none",
				p ? p->ID() : -1,
				Device->dwFrame);
	}
}

void CInventoryItem::Load(LPCSTR section) 
{
	CHitImmunity::LoadImmunities	(pSettings->r_string(section,"immunities_sect"),pSettings);

	ISpatial*			self				=	smart_cast<ISpatial*> (this);
	if (self)			self->spatial.type	|=	STYPE_VISIBLEFORAI;	

	m_name				= CStringTable().translate( pSettings->r_string(section, "inv_name") );
	m_nameShort			= CStringTable().translate( pSettings->r_string(section, "inv_name_short"));

//.	NameComplex			();
	m_weight			= pSettings->r_float(section, "inv_weight");
	R_ASSERT			(m_weight>=0.f);

	m_cost				= pSettings->r_u32(section, "cost");

	m_slot				= READ_IF_EXISTS(pSettings,r_u32,section,"slot", NO_ACTIVE_SLOT);


	// Description
	if ( pSettings->line_exist(section, "description") )
		m_Description = CStringTable().translate( pSettings->r_string(section, "description") );

	m_flags.set(Fbelt,			READ_IF_EXISTS(pSettings, r_bool, section, "belt",				FALSE));
	m_flags.set(FRuckDefault,	READ_IF_EXISTS(pSettings, r_bool, section, "default_to_ruck",	TRUE));

	m_flags.set(FCanTake,		READ_IF_EXISTS(pSettings, r_bool, section, "can_take",			TRUE));
	m_flags.set(FCanTrade,		READ_IF_EXISTS(pSettings, r_bool, section, "can_trade",			TRUE));
	m_flags.set(FIsQuestItem,	READ_IF_EXISTS(pSettings, r_bool, section, "quest_item",		FALSE));



	//����� �������� ������� � ������
	m_dwItemRemoveTime			= READ_IF_EXISTS(pSettings, r_u32, section,"item_remove_time",			ITEM_REMOVE_TIME);

	m_flags.set					(FAllowSprint,READ_IF_EXISTS	(pSettings, r_bool, section,"sprint_allowed",			TRUE));
	m_fControlInertionFactor	= READ_IF_EXISTS(pSettings, r_float,section,"control_inertion_factor",	1.0f);
	m_icon_name					= READ_IF_EXISTS(pSettings, r_string,section,"icon_name",				NULL);

}


void  CInventoryItem::ChangeCondition(float fDeltaCondition)
{
	m_fCondition += fDeltaCondition;
	clamp(m_fCondition, 0.f, 1.f);
}


void	CInventoryItem::Hit					(SHit* pHDS)
{
	if( !m_flags.test(FUsingCondition) ) return;

	float hit_power = pHDS->damage();
	hit_power *= m_HitTypeK[pHDS->hit_type];

	ChangeCondition(-hit_power);
}

const char* CInventoryItem::Name() 
{
	return *m_name;
}

const char* CInventoryItem::NameShort() 
{
	return *m_nameShort;
}

bool CInventoryItem::Useful() const
{
	return CanTake();
}

bool CInventoryItem::Activate() 
{
	return false;
}

void CInventoryItem::Deactivate() 
{
}

void CInventoryItem::OnH_B_Independent(bool just_before_destroy)
{
	UpdateXForm();
	m_eItemPlace = eItemPlaceUndefined ;
}

void CInventoryItem::OnH_A_Independent()
{
	m_dwItemIndependencyTime	= Level().timeServer();
	m_eItemPlace				= eItemPlaceUndefined;	
	inherited::OnH_A_Independent();
}

void CInventoryItem::OnH_B_Chield()
{
}

void CInventoryItem::OnH_A_Chield()
{
	inherited::OnH_A_Chield		();
}
#ifdef DEBUG
extern	Flags32	dbg_net_Draw_Flags;
#endif

void CInventoryItem::UpdateCL()
{
#ifdef DEBUG
	if(bDebug){
		if (dbg_net_Draw_Flags.test(1<<4) )
		{
			Device->seqRenderDebug.Remove(this);
			Device->seqRenderDebug.Add(this);
		}else
		{
			Device->seqRenderDebug.Remove(this);
		}
	}

#endif

}

void CInventoryItem::OnEvent (NET_Packet& P, u16 type)
{
	switch (type)
	{
	case GE_ADDON_ATTACH:
		{
			u32 ItemID;
			P.r_u32			(ItemID);
			CInventoryItem*	 ItemToAttach	= smart_cast<CInventoryItem*>(Level().Objects.net_Find(ItemID));
			if (!ItemToAttach) break;
			Attach(ItemToAttach,true);
			CActor* pActor = smart_cast<CActor*>(object().H_Parent());
			if (pActor && pActor->inventory().ActiveItem() == this)
			{
				pActor->inventory().SetPrevActiveSlot(pActor->inventory().GetActiveSlot());
				pActor->inventory().Activate(NO_ACTIVE_SLOT);
				
			}
		}break;
	case GE_ADDON_DETACH:
		{
			string64			i_name;
			P.r_stringZ			(i_name);
			Detach(i_name, true);
			CActor* pActor = smart_cast<CActor*>(object().H_Parent());
			if (pActor && pActor->inventory().ActiveItem() == this)
			{
				pActor->inventory().SetPrevActiveSlot(pActor->inventory().GetActiveSlot());
				pActor->inventory().Activate(NO_ACTIVE_SLOT);
			};
		}break;	
	case GE_CHANGE_POS:
		{
			Fvector p; 
			P.r_vec3(p);
			CPHSynchronize* pSyncObj = NULL;
			pSyncObj = object().PHGetSyncItem(0);
			if (!pSyncObj) return;
			SPHNetState state;
			pSyncObj->get_State(state);
			state.position = p;
			state.previous_position = p;
			pSyncObj->set_State(state);

		}break;
	}
}

//������� ������������ ���� ����������� � ������ ����� ���� 
//� ��������� � ��������� ��������������� ������ � ������������
//�������, ������� ������� ������ ���� ��������������
bool CInventoryItem::Detach(const char* item_section_name, bool b_spawn_item) 
{
	if (OnClient()) return true;
	if(b_spawn_item)
	{
		CSE_Abstract*		D	= F_entity_Create(item_section_name);
		R_ASSERT		   (D);
		CSE_ALifeDynamicObject	*l_tpALifeDynamicObject = 
								smart_cast<CSE_ALifeDynamicObject*>(D);
		R_ASSERT			(l_tpALifeDynamicObject);
		
		l_tpALifeDynamicObject->m_tNodeID = (g_dedicated_server)?u32(-1):object().ai_location().level_vertex_id();
			
		// Fill
		D->s_name			=	item_section_name;
		D->set_name_replace	("");
		D->m_gameType.m_GameType.flags=	u16(GameID());
		D->s_RP				=	0xff;
		D->ID				=	0xffff;
		if (GameID() == GAME_SINGLE)
		{
			D->ID_Parent		=	u16(object().H_Parent()->ID());
		}
		else	// i'm not sure this is right
		{		// but it is simpliest way to avoid exception in MP BuyWnd... [Satan]
			if (object().H_Parent())
				D->ID_Parent	=	u16(object().H_Parent()->ID());
			else
				D->ID_Parent	= NULL;
		}
		D->ID_Phantom		=	0xffff;
		D->o_Position		=	object().Position();
		D->s_flags.assign	(M_SPAWN_OBJECT_LOCAL);
		D->RespawnTime		=	0;
		// Send
		NET_Packet			P;
		D->Spawn_Write		(P,TRUE);
		Level().Send		(P,net_flags(TRUE));
		// Destroy
		F_entity_Destroy	(D);
	}
	return true;
}

/////////// network ///////////////////////////////
BOOL CInventoryItem::net_Spawn			(CSE_Abstract* DC)
{
	m_flags.set						(FInInterpolation, FALSE);
	m_flags.set						(FInInterpolate,	FALSE);
//	m_bInInterpolation				= false;
//	m_bInterpolate					= false;

	m_flags.set						(Fuseful_for_NPC, TRUE);
	CSE_Abstract					*e	= (CSE_Abstract*)(DC);
	CSE_ALifeObject					*alife_object = smart_cast<CSE_ALifeObject*>(e);
	if (alife_object)	{
		m_flags.set(Fuseful_for_NPC, alife_object->m_flags.test(CSE_ALifeObject::flUsefulForAI));
	}

	CSE_ALifeInventoryItem			*pSE_InventoryItem = smart_cast<CSE_ALifeInventoryItem*>(e);
	if (!pSE_InventoryItem)			return TRUE;

	//!!!
	m_fCondition = pSE_InventoryItem->m_fCondition;
	if (GameID() != GAME_SINGLE)
		object().processing_activate();

	m_dwItemIndependencyTime		= 0;

	return							TRUE;
}

void CInventoryItem::net_Destroy		()
{
	//��������� �������� �� ������������
//.	m_pCurrentInventory = NULL;
}

void CInventoryItem::save(NET_Packet &packet)
{
	packet.w_u8				((u8)m_eItemPlace);
	packet.w_float			(m_fCondition);

	if (object().H_Parent()) {
		packet.w_u8			(0);
		return;
	}

	u8 _num_items			= (u8)object().PHGetSyncItemsNumber(); 
	packet.w_u8				(_num_items);
	object().PHSaveState	(packet);
}

typedef CSE_ALifeInventoryItem::mask_num_items	mask_num_items;

void CInventoryItem::net_Import			(NET_Packet& P) 
{	
	u8							NumItems = 0;
	NumItems					= P.r_u8();
	if (!NumItems)
		return;

	net_update_IItem			N;
	N.State.force.set			(0.f,0.f,0.f);
	N.State.torque.set			(0.f,0.f,0.f);
	
	P.r_vec3					(N.State.position);

	N.State.quaternion.x		= P.r_float_q8(0.f,1.f);
	N.State.quaternion.y		= P.r_float_q8(0.f,1.f);
	N.State.quaternion.z		= P.r_float_q8(0.f,1.f);
	N.State.quaternion.w		= P.r_float_q8(0.f,1.f);

	mask_num_items				num_items;
	num_items.common			= NumItems;
	NumItems					= num_items.num_items;

	N.State.enabled				= num_items.mask & CSE_ALifeInventoryItem::inventory_item_state_enabled;
	if (!(num_items.mask & CSE_ALifeInventoryItem::inventory_item_angular_null)) {
		N.State.angular_vel.x	= P.r_float_q8(0.f,10.f*PI_MUL_2);
		N.State.angular_vel.y	= P.r_float_q8(0.f,10.f*PI_MUL_2);
		N.State.angular_vel.z	= P.r_float_q8(0.f,10.f*PI_MUL_2);
	}
	else
		N.State.angular_vel.set	(0.f,0.f,0.f);

	if (!(num_items.mask & CSE_ALifeInventoryItem::inventory_item_linear_null)) {
		N.State.linear_vel.x	= P.r_float_q8(-32.f,32.f);
		N.State.linear_vel.y	= P.r_float_q8(-32.f,32.f);
		N.State.linear_vel.z	= P.r_float_q8(-32.f,32.f);
	}
	else
		N.State.linear_vel.set	(0.f,0.f,0.f);
	////////////////////////////////////////////

	N.State.previous_position	= N.State.position;
	N.State.previous_quaternion	= N.State.quaternion;

	net_updateData				*p = NetSync();

	//if (!p->NET_IItem.empty() && (p->NET_IItem.back().dwTimeStamp>=N.dwTimeStamp))
	//	return;
	if (!p->NET_IItem.empty())
	{
		m_flags.set				(FInInterpolate, TRUE);
	}

	Level().AddObject_To_Objects4CrPr		(m_object);
	object().CrPr_SetActivated				(false);
	object().CrPr_SetActivationStep			(0);

	p->NET_IItem.push_back					(N);
	while (p->NET_IItem.size() > 2)
	{
		p->NET_IItem.pop_front				();
	};
};

void CInventoryItem::net_Export			(NET_Packet& P) 
{	
	if (object().H_Parent() || IsGameTypeSingle()) 
	{
		P.w_u8				(0);
		return;
	}
	CPHSynchronize* pSyncObj				= NULL;
	SPHNetState								State;
	pSyncObj = object().PHGetSyncItem		(0);

	if (pSyncObj && !object().H_Parent()) 
		pSyncObj->get_State					(State);
	else 	
		State.position.set					(object().Position());


	mask_num_items			num_items;
	num_items.mask			= 0;
	u16						temp = bone_count_to_synchronize();

	R_ASSERT				(temp < (u16(1) << 5));
	num_items.num_items		= u8(temp);

	if (State.enabled)									num_items.mask |= CSE_ALifeInventoryItem::inventory_item_state_enabled;
	if (fis_zero(State.angular_vel.square_magnitude()))	num_items.mask |= CSE_ALifeInventoryItem::inventory_item_angular_null;
	if (fis_zero(State.linear_vel.square_magnitude()))	num_items.mask |= CSE_ALifeInventoryItem::inventory_item_linear_null;

	P.w_u8					(num_items.common);

	P.w_vec3				(State.position);

	float					magnitude = _sqrt(State.quaternion.magnitude());
	if (fis_zero(magnitude)) {
		magnitude			= 1;
		State.quaternion.x	= 0.f;
		State.quaternion.y	= 0.f;
		State.quaternion.z	= 1.f;
		State.quaternion.w	= 0.f;
	}
	else {
		float				invert_magnitude = 1.f/magnitude;
		
		State.quaternion.x	*= invert_magnitude;
		State.quaternion.y	*= invert_magnitude;
		State.quaternion.z	*= invert_magnitude;
		State.quaternion.w	*= invert_magnitude;

		clamp				(State.quaternion.x,0.f,1.f);
		clamp				(State.quaternion.y,0.f,1.f);
		clamp				(State.quaternion.z,0.f,1.f);
		clamp				(State.quaternion.w,0.f,1.f);
	}

	P.w_float_q8			(State.quaternion.x,0.f,1.f);
	P.w_float_q8			(State.quaternion.y,0.f,1.f);
	P.w_float_q8			(State.quaternion.z,0.f,1.f);
	P.w_float_q8			(State.quaternion.w,0.f,1.f);

	if (!(num_items.mask & CSE_ALifeInventoryItem::inventory_item_angular_null)) {
		clamp				(State.angular_vel.x,0.f,10.f*PI_MUL_2);
		clamp				(State.angular_vel.y,0.f,10.f*PI_MUL_2);
		clamp				(State.angular_vel.z,0.f,10.f*PI_MUL_2);

		P.w_float_q8		(State.angular_vel.x,0.f,10.f*PI_MUL_2);
		P.w_float_q8		(State.angular_vel.y,0.f,10.f*PI_MUL_2);
		P.w_float_q8		(State.angular_vel.z,0.f,10.f*PI_MUL_2);
	}

	if (!(num_items.mask & CSE_ALifeInventoryItem::inventory_item_linear_null)) {
		clamp				(State.linear_vel.x,-32.f,32.f);
		clamp				(State.linear_vel.y,-32.f,32.f);
		clamp				(State.linear_vel.z,-32.f,32.f);

		P.w_float_q8		(State.linear_vel.x,-32.f,32.f);
		P.w_float_q8		(State.linear_vel.y,-32.f,32.f);
		P.w_float_q8		(State.linear_vel.z,-32.f,32.f);
	}
};

void CInventoryItem::load(IReader &packet)
{
	m_eItemPlace			= (EItemPlace)packet.r_u8();
	m_fCondition			= packet.r_float();

	u8						tmp = packet.r_u8();
	if (!tmp)
		return;
	
	if (!object().PPhysicsShell()) {
		object().setup_physic_shell	();
		object().PPhysicsShell()->Disable();
	}
	
	object().PHLoadState(packet);
	object().PPhysicsShell()->Disable();
}

///////////////////////////////////////////////
void CInventoryItem::PH_B_CrPr		()
{
	net_updateData* p		= NetSync();
	//just set last update data for now
	if (object().CrPr_IsActivated()) return;
	if (object().CrPr_GetActivationStep() > ph_world->m_steps_num) return;
	object().CrPr_SetActivated(true);

	///////////////////////////////////////////////
	CPHSynchronize* pSyncObj				= NULL;
	pSyncObj = object().PHGetSyncItem		(0);
	if (!pSyncObj)							return;
	///////////////////////////////////////////////
	pSyncObj->get_State						(p->LastState);
	///////////////////////////////////////////////
	net_update_IItem N_I	= p->NET_IItem.back();

	pSyncObj->set_State						(N_I.State);

	object().PHUnFreeze						();
	///////////////////////////////////////////////
	if (Level().InterpolationDisabled())
	{
		m_flags.set			(FInInterpolation, FALSE);
//		m_bInInterpolation = false;
	};
	///////////////////////////////////////////////
};	

void CInventoryItem::PH_I_CrPr		()		// actions & operations between two phisic prediction steps
{
	net_updateData* p					= NetSync();
	//store recalculated data, then we able to restore it after small future prediction
	if (!object().CrPr_IsActivated())	return;
	////////////////////////////////////
	CPHSynchronize* pSyncObj			= NULL;
	pSyncObj = object().PHGetSyncItem	(0);
	if (!pSyncObj)						return;
	////////////////////////////////////
	pSyncObj->get_State					(p->RecalculatedState);
	///////////////////////////////////////////////
	Fmatrix xformX;
	pSyncObj->cv2obj_Xfrom(p->RecalculatedState.quaternion, p->RecalculatedState.position, xformX);

	VERIFY2								(_valid(xformX),*object().cName());
	pSyncObj->cv2obj_Xfrom				(p->RecalculatedState.quaternion, p->RecalculatedState.position, xformX);

	p->IRecRot.set(xformX);
	p->IRecPos.set(xformX.c);
	VERIFY2								(_valid(p->IRecPos),*object().cName());
}; 

#ifdef DEBUG
void CInventoryItem::PH_Ch_CrPr			()
{
	net_updateData* p					= NetSync();
	//restore recalculated data and get data for interpolation	
	if (!object().CrPr_IsActivated())	return;
	////////////////////////////////////
	CPHSynchronize* pSyncObj			= NULL;
	pSyncObj = object().PHGetSyncItem	(0);
	if (!pSyncObj)						return;
	////////////////////////////////////
	pSyncObj->get_State					(p->CheckState);

	if (!object().H_Parent() && object().getVisible())
	{
		if (p->CheckState.enabled == false && p->RecalculatedState.enabled == true)
		{
			///////////////////////////////////////////////////////////////////
			pSyncObj->set_State			(p->LastState);
			pSyncObj->set_State			(p->RecalculatedState);//, N_A.State.enabled);

			object().PHUnFreeze			();
			///////////////////////////////////////////////////////////////////
			ph_world->Step				();
			///////////////////////////////////////////////////////////////////
			PH_Ch_CrPr					();
			////////////////////////////////////
		};
	};	
};
#endif

void CInventoryItem::PH_A_CrPr		()
{
	net_updateData* p					= NetSync();
	//restore recalculated data and get data for interpolation	
	if (!object().CrPr_IsActivated())	return;
	////////////////////////////////////
	CPHSynchronize* pSyncObj			= NULL;
	pSyncObj = object().PHGetSyncItem	(0);
	if (!pSyncObj)						return;
	////////////////////////////////////
	pSyncObj->get_State					(p->PredictedState);
	////////////////////////////////////
	pSyncObj->set_State					(p->RecalculatedState);
	////////////////////////////////////

	if (!m_flags.test(FInInterpolate)) return;
	////////////////////////////////////
	Fmatrix xformX;
	pSyncObj->cv2obj_Xfrom(p->PredictedState.quaternion, p->PredictedState.position, xformX);

	VERIFY2								(_valid(xformX),*object().cName());
	pSyncObj->cv2obj_Xfrom				(p->PredictedState.quaternion, p->PredictedState.position, xformX);
	
	p->IEndRot.set						(xformX);
	p->IEndPos.set						(xformX.c);
	VERIFY2								(_valid(p->IEndPos),*object().cName());
	/////////////////////////////////////////////////////////////////////////
	CalculateInterpolationParams		();
	///////////////////////////////////////////////////
};

extern	float		g_cl_lvInterp;

void CInventoryItem::CalculateInterpolationParams()
{
	net_updateData* p = NetSync();
	p->IStartPos.set(object().Position());
	p->IStartRot.set(object().XFORM());

	Fvector P0, P1, P2, P3;

	CPHSynchronize* pSyncObj = NULL;
	pSyncObj = object().PHGetSyncItem(0);
	
	Fmatrix xformX0, xformX1;	

	if (m_flags.test(FInInterpolation))
	{
		u32 CurTime = Level().timeServer();
		float factor	= float(CurTime - p->m_dwIStartTime)/(p->m_dwIEndTime - p->m_dwIStartTime);
		if (factor > 1.0f) factor = 1.0f;

		float c = factor;
		for (u32 k=0; k<3; k++)
		{
			P0[k] = c*(c*(c*p->SCoeff[k][0]+p->SCoeff[k][1])+p->SCoeff[k][2])+p->SCoeff[k][3];
			P1[k] = (c*c*p->SCoeff[k][0]*3+c*p->SCoeff[k][1]*2+p->SCoeff[k][2])/3; // �������� �� ������� � 3 ���� ��������� �������� ��� ������� ������������� !!!!
		};
		P0.set(p->IStartPos);
		P1.add(p->IStartPos);
	}	
	else
	{
		P0 = p->IStartPos;

		if (p->LastState.linear_vel.x == 0 && 
			p->LastState.linear_vel.y == 0 && 
			p->LastState.linear_vel.z == 0)
		{
			pSyncObj->cv2obj_Xfrom(p->RecalculatedState.previous_quaternion, p->RecalculatedState.previous_position, xformX0);
			pSyncObj->cv2obj_Xfrom(p->RecalculatedState.quaternion, p->RecalculatedState.position, xformX1);
		}
		else
		{
			pSyncObj->cv2obj_Xfrom(p->LastState.previous_quaternion, p->LastState.previous_position, xformX0);
			pSyncObj->cv2obj_Xfrom(p->LastState.quaternion, p->LastState.position, xformX1);
		};

		P1.sub(xformX1.c, xformX0.c);
		P1.add(p->IStartPos);
	}

	P2.sub(p->PredictedState.position, p->PredictedState.linear_vel);
	pSyncObj->cv2obj_Xfrom(p->PredictedState.quaternion, P2, xformX0);
	P2.set(xformX0.c);

	pSyncObj->cv2obj_Xfrom(p->PredictedState.quaternion, p->PredictedState.position, xformX1);
	P3.set(xformX1.c);
	/////////////////////////////////////////////////////////////////////////////
	Fvector TotalPath;
	TotalPath.sub(P3, P0);
	float TotalLen = TotalPath.magnitude();
	
	SPHNetState	State0 = (p->NET_IItem.back()).State;
	SPHNetState	State1 = p->PredictedState;

	float lV0 = State0.linear_vel.magnitude();
	float lV1 = State1.linear_vel.magnitude();

	u32		ConstTime = u32((fixed_step - ph_world->m_frame_time)*1000)+ Level().GetInterpolationSteps()*u32(fixed_step*1000);
	
	p->m_dwIStartTime = p->m_dwILastUpdateTime;

	if (( lV0 + lV1) > 0.000001 && g_cl_lvInterp == 0)
	{
		u32		CulcTime = iCeil(TotalLen*2000/( lV0 + lV1));
		p->m_dwIEndTime = p->m_dwIStartTime + _min(CulcTime, ConstTime);
	}
	else
		p->m_dwIEndTime = p->m_dwIStartTime + ConstTime;
	/////////////////////////////////////////////////////////////////////////////
	Fvector V0, V1;
	V0.sub(P1, P0);
	V1.sub(P3, P2);
	lV0 = V0.magnitude();
	lV1 = V1.magnitude();

	if (TotalLen != 0)
	{
		if (V0.x != 0 || V0.y != 0 || V0.z != 0)
		{
			if (lV0 > TotalLen/3)
			{
				V0.normalize();
				V0.mul(TotalLen/3);
				P1.add(V0, P0);
			}
		}
		
		if (V1.x != 0 || V1.y != 0 || V1.z != 0)
		{
			if (lV1 > TotalLen/3)
			{
				V1.normalize();
				V1.mul(TotalLen/3);
				P2.sub(P3, V1);
			};
		}
	};
	/////////////////////////////////////////////////////////////////////////////
	for( u32 i =0; i<3; i++)
	{
		p->SCoeff[i][0] = P3[i]	- 3*P2[i] + 3*P1[i] - P0[i];
		p->SCoeff[i][1] = 3*P2[i]	- 6*P1[i] + 3*P0[i];
		p->SCoeff[i][2] = 3*P1[i]	- 3*P0[i];
		p->SCoeff[i][3] = P0[i];
	};
	/////////////////////////////////////////////////////////////////////////////
	m_flags.set	(FInInterpolation, TRUE);

	if (object().m_pPhysicsShell) object().m_pPhysicsShell->NetInterpolationModeON();

};

void CInventoryItem::make_Interpolation	()
{
	net_updateData* p		= NetSync();
	p->m_dwILastUpdateTime = Level().timeServer();
	
	if(!object().H_Parent() && object().getVisible() && object().m_pPhysicsShell && m_flags.test(FInInterpolation) ) 
	{

		u32 CurTime = Level().timeServer();
		if (CurTime >= p->m_dwIEndTime) 
		{
			m_flags.set(FInInterpolation, FALSE);

			object().m_pPhysicsShell->NetInterpolationModeOFF();

			CPHSynchronize* pSyncObj		= NULL;
			pSyncObj						= object().PHGetSyncItem(0);
			pSyncObj->set_State				(p->PredictedState);
			Fmatrix xformI;
			pSyncObj->cv2obj_Xfrom			(p->PredictedState.quaternion, p->PredictedState.position, xformI);
			VERIFY2							(_valid(object().renderable.xform),*object().cName());
			object().XFORM().set			(xformI);
			VERIFY2								(_valid(object().renderable.xform),*object().cName());
		}
		else 
		{
			VERIFY			(CurTime <= p->m_dwIEndTime);
			float factor	= float(CurTime - p->m_dwIStartTime)/(p->m_dwIEndTime - p->m_dwIStartTime);
			if (factor > 1) factor = 1.0f;
			else if (factor < 0) factor = 0;

			Fvector IPos;
			Fquaternion IRot;

			float c = factor;
			for (u32 k=0; k<3; k++)
			{
				IPos[k] = c*(c*(c*p->SCoeff[k][0]+p->SCoeff[k][1])+p->SCoeff[k][2])+p->SCoeff[k][3];
			};

			VERIFY2								(_valid(IPos),*object().cName());
			VERIFY			(factor>=0.f && factor<=1.f);
			IRot.slerp(p->IStartRot, p->IEndRot, factor);
			VERIFY2								(_valid(IRot),*object().cName());
			object().XFORM().rotation(IRot);
			VERIFY2								(_valid(object().renderable.xform),*object().cName());
			object().Position().set(IPos);
			VERIFY2								(_valid(object().renderable.xform),*object().cName());
		};
	}
	else
	{
		m_flags.set(FInInterpolation,FALSE);
	};

#ifdef DEBUG
	Fvector iPos = object().Position();

	if (!object().H_Parent() && object().getVisible()) 
	{
		if(m_net_updateData)
			m_net_updateData->LastVisPos.push_back(iPos);
	};
#endif
}


void CInventoryItem::reload		(LPCSTR section)
{
	inherited::reload		(section);
	m_holder_range_modifier	= READ_IF_EXISTS(pSettings,r_float,section,"holder_range_modifier",1.f);
	m_holder_fov_modifier	= READ_IF_EXISTS(pSettings,r_float,section,"holder_fov_modifier",1.f);
}

void CInventoryItem::reinit		()
{
	m_pCurrentInventory	= NULL;
	m_eItemPlace	= eItemPlaceUndefined;
}

bool CInventoryItem::can_kill			() const
{
	return				(false);
}

CInventoryItem *CInventoryItem::can_kill	(CInventory *inventory) const
{
	return				(0);
}

const CInventoryItem *CInventoryItem::can_kill			(const xr_vector<const CGameObject*> &items) const
{
	return				(0);
}

CInventoryItem *CInventoryItem::can_make_killing	(const CInventory *inventory) const
{
	return				(0);
}

bool CInventoryItem::ready_to_kill		() const
{
	return				(false);
}

void CInventoryItem::activate_physic_shell()
{
	CEntityAlive*	E		= smart_cast<CEntityAlive*>(object().H_Parent());
	if (!E) {
		on_activate_physic_shell();
		return;
	};

	UpdateXForm();

	object().CPhysicsShellHolder::activate_physic_shell();
}

void CInventoryItem::UpdateXForm	()
{
	if (0==object().H_Parent())	return;

	// Get access to entity and its visual
	CEntityAlive*	E		= smart_cast<CEntityAlive*>(object().H_Parent());
	if (!E) return;
	
	if (E->cast_base_monster()) return;

	const CInventoryOwner	*parent = smart_cast<const CInventoryOwner*>(E);
	if (parent && parent->use_simplified_visual())
		return;

	if (parent->attached(this))
		return;

	R_ASSERT		(E);
	IKinematics*	V		= CastToIKinematics	(E->Visual());
	VERIFY			(V);

	// Get matrices
	int				boneL,boneR,boneR2;
	E->g_WeaponBones(boneL,boneR,boneR2);
	//	if ((HandDependence() == hd1Hand) || (STATE == eReload) || (!E->g_Alive()))
	//		boneL = boneR2;
#pragma todo("TO ALL: serious performance problem")
	V->CalculateBones	();
	const Fmatrix& mL			= V->LL_GetTransform(u16(boneL));
	const Fmatrix& mR			= V->LL_GetTransform(u16(boneR));
	// Calculate
	Fmatrix			mRes;
	Fvector			R,D,N;
	D.sub			(mL.c,mR.c);	D.normalize_safe();

	if(fis_zero(D.magnitude()))
	{
		mRes.set(E->XFORM());
		mRes.c.set(mR.c);
	}
	else
	{		
		D.normalize();
		R.crossproduct	(mR.j,D);

		N.crossproduct	(D,R);
		N.normalize();

		mRes.set		(R,N,D,mR.c);
		mRes.mulA_43	(E->XFORM());
	}

	//	UpdatePosition	(mRes);
	object().Position().set(mRes.c);
}



#ifdef DEBUG

void CInventoryItem::OnRenderDebug()
{
	if (bDebug && object().Visual())
	{
		if (!(dbg_net_Draw_Flags.is_any((1<<4)))) return;

		Fvector bc,bd; 
		object().Visual()->getVisData().box.get_CD	(bc,bd);
		Fmatrix	M = object().XFORM();
		M.c.add (bc);
		Level().debug_renderer().draw_obb			(M,bd,color_rgba(0,0,255,255));
/*
		u32 Color;
		if (processing_enabled())
		{
			if (m_bInInterpolation)
				Color = color_rgba(0,255,255, 255);
			else
				Color = color_rgba(0,255,0, 255);
		}
		else
		{
			if (m_bInInterpolation)
				Color = color_rgba(255,0,255, 255);
			else
				Color = color_rgba(255, 0, 0, 255);
		};

//		Level().debug_renderer().draw_obb			(M,bd,Color);
		float size = 0.01f;
		if (!H_Parent())
		{
			Level().debug_renderer().draw_aabb			(Position(), size, size, size, color_rgba(0, 255, 0, 255));

			Fvector Pos1, Pos2;
			VIS_POSITION_it It = LastVisPos.begin();
			Pos1 = *It;
			for (; It != LastVisPos.end(); It++)
			{
				Pos2 = *It;
				Level().debug_renderer().draw_line(Fidentity, Pos1, Pos2, color_rgba(255, 255, 0, 255));
				Pos1 = Pos2;
			};

		}
		//---------------------------------------------------------
		if (OnClient() && !H_Parent() && m_bInInterpolation)
		{

			Fmatrix xformI;

			xformI.rotation(IRecRot);
			xformI.c.set(IRecPos);
			Level().debug_renderer().draw_aabb			(IRecPos, size, size, size, color_rgba(255, 0, 255, 255));

			xformI.rotation(IEndRot);
			xformI.c.set(IEndPos);
			Level().debug_renderer().draw_obb			(xformI,bd,color_rgba(0, 255, 0, 255));

			///////////////////////////////////////////////////////////////////////////
			Fvector point0 = IStartPos, point1;			
			
			float c = 0;
			for (float i=0.1f; i<1.1f; i+= 0.1f)
			{
				c = i;// * 0.1f;
				for (u32 k=0; k<3; k++)
				{
					point1[k] = c*(c*(c*SCoeff[k][0]+SCoeff[k][1])+SCoeff[k][2])+SCoeff[k][3];
				};
				Level().debug_renderer().draw_line(Fidentity, point0, point1, color_rgba(0, 0, 255, 255));
				point0.set(point1);
			};
		};
		*/
	};
}
#endif

DLL_Pure *CInventoryItem::_construct	()
{
	m_object	= smart_cast<CPhysicsShellHolder*>(this);
	VERIFY		(m_object);
	return		(inherited::_construct());
}

void CInventoryItem::modify_holder_params	(float &range, float &fov) const
{
	range		*= m_holder_range_modifier;
	fov			*= m_holder_fov_modifier;
}

bool CInventoryItem::NeedToDestroyObject()	const
{
	return (TimePassedAfterIndependant() > m_dwItemRemoveTime);
}

ALife::_TIME_ID	 CInventoryItem::TimePassedAfterIndependant()	const
{
	if(!object().H_Parent() && m_dwItemIndependencyTime != 0)
		return Level().timeServer() - m_dwItemIndependencyTime;
	else
		return 0;
}

bool	CInventoryItem::CanTrade() const 
{
	bool res = true;
#pragma todo("Dima to Andy : why CInventoryItem::CanTrade can be called for the item, which doesn't have owner?")
	if(m_pCurrentInventory)
		res = inventory_owner().AllowItemToTrade(this,m_eItemPlace);

	return (res && m_flags.test(FCanTrade) && !IsQuestItem());
}

float CInventoryItem::GetKillMsgXPos		() const 
{
	return READ_IF_EXISTS(pSettings,r_float,m_object->cNameSect(),"kill_msg_x", 0.0f);
}

float CInventoryItem::GetKillMsgYPos		() const 
{
	return READ_IF_EXISTS(pSettings,r_float,m_object->cNameSect(),"kill_msg_y", 0.0f);
}

float CInventoryItem::GetKillMsgWidth		() const 
{
	return READ_IF_EXISTS(pSettings,r_float,m_object->cNameSect(),"kill_msg_width", 0.0f);
}

float CInventoryItem::GetKillMsgHeight	() const 
{
	return READ_IF_EXISTS(pSettings,r_float,m_object->cNameSect(),"kill_msg_height", 0.0f);
}

int  CInventoryItem::GetGridWidth			() const 
{
	return pSettings->r_u32(m_object->cNameSect(), "inv_grid_width");
}

int  CInventoryItem::GetGridHeight			() const 
{
	return pSettings->r_u32(m_object->cNameSect(), "inv_grid_height");
}
int  CInventoryItem::GetXPos				() const 
{
	return pSettings->r_u32(m_object->cNameSect(), "inv_grid_x");
}
int  CInventoryItem::GetYPos				() const 
{
	return pSettings->r_u32(m_object->cNameSect(), "inv_grid_y");
}

bool CInventoryItem::IsNecessaryItem(CInventoryItem* item)		
{
	return IsNecessaryItem(item->object().cNameSect());
};

BOOL CInventoryItem::IsInvalid() const
{
	return object().getDestroy() || GetDropManual();
}

u16 CInventoryItem::bone_count_to_synchronize	() const
{
	return 0;
}