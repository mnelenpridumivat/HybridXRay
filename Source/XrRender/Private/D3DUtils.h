//----------------------------------------------------
// file: D3DUtils.h
//----------------------------------------------------

#ifndef D3DUtilsH
#define D3DUtilsH
#include "DrawUtils.h"
//----------------------------------------------------
#ifdef REDITOR
class CEditableObject;
#	define DU_DRAW_DIP	EDevice->DIP
#	define DU_DRAW_DP	EDevice->DP
#else
#	define DU_DRAW_DIP	RCache.dbg_DIP
#	define DU_DRAW_DP	RCache.dbg_DP
#endif

struct SPrimitiveBuffer{
    ref_geom				pGeom;
    u32						v_cnt;
    u32						i_cnt;
    D3DPRIMITIVETYPE 		p_type;
    u32						p_cnt;
	typedef fastdelegate::FastDelegate0<> TOnRender;
    TOnRender				OnRender;
    void 			RenderDIP();
    void 			RenderDP();
public:
                            SPrimitiveBuffer():OnRender(0),pGeom(0){;}
    void					CreateFromData(D3DPRIMITIVETYPE _pt, u32 _p_cnt, u32 FVF, LPVOID vertices, u32 _v_cnt, u16* indices=0, u32 _i_cnt=0);
    void					Destroy();
    void					Render(){OnRender();}
};

//----------------------------------------------------
// Utilities
//----------------------------------------------------
class ECORE_API CDrawUtilities: public CDUInterface
{
	SPrimitiveBuffer		m_SolidCone;
	SPrimitiveBuffer		m_WireCone;
    SPrimitiveBuffer		m_SolidSphere;
    SPrimitiveBuffer		m_WireSphere;
    SPrimitiveBuffer		m_SolidSpherePart;
    SPrimitiveBuffer		m_WireSpherePart;
    SPrimitiveBuffer		m_SolidCylinder;
    SPrimitiveBuffer		m_WireCylinder;
    SPrimitiveBuffer		m_SolidBox;
    SPrimitiveBuffer		m_WireBox;
	CGameFont* 				m_Font;
#ifdef REDITOR
    CEditableObject* m_axis_object;
#endif
};
//----------------------------------------------------
#endif /*_INCDEF_D3DUtils_H_*/

