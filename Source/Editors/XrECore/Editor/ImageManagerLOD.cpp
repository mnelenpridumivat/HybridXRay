#include "stdafx.h"
#pragma hdrstop

#include "ImageManager.h"
#include "editobject.h"
#include "editmesh.h"
#include "ui_main.h"
#include "xrHemisphere.h"
#include "xrImage_Resampler.h"
#include "..\Engine\Image.h"
/*
IC void SetCamera(float angle, const Fvector& C, float height, float radius, float dist)
{
    Fvector 	D;
    Fvector 	hpb;
    Fmatrix 	P;

	hpb.set		(angle,0,0);
	D.setHP		(hpb.x,hpb.y);
    D.mul		(-dist);
    D.add		(C);

    float ta	= height/dist;
    float asp 	= height/radius;
	float fp	= dist+4.f*radius;
    float np	= dist-4.f*radius; clamp(np,0.1f,fp);
    EDevice->m_Camera.Set		(hpb,D);
    P.build_projection_HAT	(ta,asp,np,fp);
    RCache.set_xform_project(P);
}

IC void CopyLODImage(U32Vec& src, U32Vec& dest, u32 src_w, u32 src_h, int id, int pitch)
{
	for (u32 y=0; y<src_h; y++)
    	CopyMemory(dest.begin()+y*pitch+id*src_w,src.begin()+y*src_w,src_w*sizeof(u32));
}

void CImageManager::CreateLODTexture(const Fbox& bb, U32Vec& tgt_data, u32 tgt_w, u32 tgt_h, int samples)
{
	U32Vec pixels;

    Fvector C;
    Fvector S;
    bb.getradius				(S);
    float R 					= _max(S.x,S.z);
    bb.getcenter				(C);

    Fmatrix save_projection		= EDevice->mProjection;
    Fvector save_pos 			= EDevice->m_Camera.GetPosition();
    Fvector save_hpb 			= EDevice->m_Camera.GetHPB();
    float save_far		 		= EDevice->m_Camera._Zfar();
	ECameraStyle save_style 	= EDevice->m_Camera.GetStyle();

    float D		= 500.f;
    u32 pitch 					= tgt_w*samples;

    tgt_data.resize				(pitch*tgt_h);
	EDevice->m_Camera.SetStyle	(csPlaneMove);
    EDevice->m_Camera.SetDepth	(D*2,true);

    // save render params
    Flags32 old_flag			= psDeviceFlags;
	u32 old_dwFillMode			= EDevice->dwFillMode;
    u32 old_dwShadeMode			= EDevice->dwShadeMode;
    // set render params

    u32 cc						= 	EPrefs->scene_clear_color;
    EPrefs.scene_clear_color 	= 	0x0000000;
    psDeviceFlags.zero			();
	psDeviceFlags.set			(rsFilterLinear,TRUE);
	EDevice->dwFillMode			= D3DFILL_SOLID;
    EDevice->dwShadeMode			= D3DSHADE_GOURAUD;

    SetCamera(0,C,S.y,R,D);

    for (int frame=0; frame<samples; frame++){
    	float angle 			= frame*(PI_MUL_2/samples);
	    SetCamera				(angle,C,S.y,R,D);
	    EDevice->MakeScreenshot	(pixels,tgt_w,tgt_h);
        // copy LOD to final
		for (u32 y=0; y<tgt_h; y++)
    		CopyMemory			(tgt_data.begin()+y*pitch+frame*tgt_w,pixels.begin()+y*tgt_w,tgt_w*sizeof(u32));
    }

    ApplyBorders				(tgt_data,pitch,tgt_h);

    // flip data
	for (u32 y=0; y<tgt_h/2; y++){
		u32 y2 = tgt_h-y-1;
		for (u32 x=0; x<pitch; x++){
        	std::swap	(tgt_data[y*pitch+x],tgt_data[y2*pitch+x]);	
		}
	}
        
    // restore render params
	EDevice->dwFillMode			= old_dwFillMode;
    EDevice->dwShadeMode			= old_dwShadeMode;
    psDeviceFlags 				= old_flag;
    EPrefs.scene_clear_color 	= cc;

	EDevice->m_Camera.SetStyle	(save_style);
    RCache.set_xform_project	(save_projection);
    EDevice->m_Camera.Set			(save_hpb,save_pos);
    EDevice->m_Camera.Set			(save_hpb,save_pos);
    EDevice->m_Camera.SetDepth	(save_far,false);
}
*/

DEFINE_VECTOR(Fvector4,Fvector4Vec,Fvector4It);
BOOL GetPointColor(SPickQuery::SResult* R, u32& alpha, u32& color)
{
    CSurface* surf			= R->e_mesh->GetSurfaceByFaceID(R->tag); VERIFY(surf);
    Shader_xrLC* c_sh		= EDevice->ShaderXRLC.Get(surf->_ShaderXRLCName());
    if (!c_sh->flags.bRendering) return FALSE;
    const Fvector2*			cuv[3];
    R->e_mesh->GetFaceTC	(R->tag,cuv);

    // barycentric coords
    // note: W,U,V order
    Fvector B;
    B.set	(1.0f - R->u - R->v,R->u,R->v);

    // calc UV
    Fvector2	uv;
    uv.x = cuv[0]->x*B.x + cuv[1]->x*B.y + cuv[2]->x*B.z;
    uv.y = cuv[0]->y*B.x + cuv[1]->y*B.y + cuv[2]->y*B.z;

    int U = iFloor(uv.x*float(surf->m_ImageData->w) + .5f);
    int V = iFloor(uv.y*float(surf->m_ImageData->h)+ .5f);
    U %= surf->m_ImageData->w;	if (U<0) U+=surf->m_ImageData->w;
    V %= surf->m_ImageData->h;	if (V<0) V+=surf->m_ImageData->h;

/*    
//	float filter_core[3][3]={{0.0125,0.0125,0.0125},{0.0125,0.9,0.0125},{0.0125,0.0125,0.0125}};
//	float filter_core[3][3]={{0.0625,0.0625,0.0625},{0.0625,0.5,0.0625},{0.0625,0.0625,0.0625}};
//	float filter_core[3][3]={{0,0,0},{0,1,0},{0,0,0}};
    Fvector4 C={0,0,0,0};
    u32 cnt=0;
    for (int y=-1; y<=1; y++){
        int v 			= V+y;
        if (v<0||v>=(int)surf->m_ImageData->h) continue;
	    for (int x=-1; x<=1; x++){
	        int u 		= U+x;
	        if (u<0||u>=(int)surf->m_ImageData->w) continue;
        	Fvector4 	c;
            color		= surf->m_ImageData->layers.back()[v*surf->m_ImageData->w+u];
            float f 	= filter_core[x+1][y+1];
		    c.set		(f*color_get_R(color),f*color_get_G(color),f*color_get_B(color),f*color_get_A(color));
            C.add		(c);
            cnt++;
        }
    }
//	if (0!=cnt)	C.div(cnt);
    color	= color_rgba(C.x,C.y,C.z,C.w);
*/
	color = surf->m_ImageData->layers.back()[V*surf->m_ImageData->w+U];
    alpha = color_get_A(color);
    return TRUE;
}

struct SBuildLight{
    Flight					light;
    float					energy;
};
DEFINE_VECTOR				(SBuildLight,BLVec,BLIt);
ICF static void  simple_hemi_callback(float x, float y, float z, float E, LPVOID P)
{
    BLVec* dst 					= (BLVec*)P;
    SBuildLight 				T;
    T.energy     				= E;
    T.light.direction.set		(x,y,z);
    dst->push_back  			(T);
}

const u32 lod_ss_quality  = 8;

void CreateLODSamples(const Fbox& bbox, U32Vec& tgt_data, u32 tgt_w, u32 tgt_h)
{
	U32Vec 	s_pixels,d_pixels;
    Fmatrix save_projection		= EDevice->mProject;
    Fmatrix save_view			= EDevice->mView;

    // save render params
    Flags32 old_flag			= psDeviceFlags;
	u32 old_dwFillMode			= EDevice->dwFillMode;
    u32 old_dwShadeMode			= EDevice->dwShadeMode;
    // set render params

    u32 cc						= 	EPrefs->scene_clear_color;
    EPrefs->scene_clear_color 	= 	0x0000000;
    psDeviceFlags.zero			();
	psDeviceFlags.set			(rsFilterLinear,TRUE);
	EDevice->dwFillMode			= D3DFILL_SOLID;
    EDevice->dwShadeMode			= D3DSHADE_GOURAUD;

    Fvector vP,vD,vN,vR;
    Fmatrix mV,mP;
    
    Fvector S;
    bbox.getradius				(S);
    float R 					= 2.f*_max(S.x,S.z);

    u32 pitch 					= tgt_w*LOD_SAMPLE_COUNT;
    tgt_data.resize				(pitch*tgt_h);
    for (int frame=0; frame<LOD_SAMPLE_COUNT; frame++){
        float angle 			= frame*(PI_MUL_2/LOD_SAMPLE_COUNT);

		Fbox bb 				= bbox;
        // build camera matrix
        bb.getcenter			( vP );
        vN.set					( 0.f,1.f,0.f );
        vD.setHP				( angle, 0 );
        vR.crossproduct			( vN,vD );
        mV.build_camera_dir		(vP,vD,vN);
        bb.xform				(mV);
        // build project matrix
        mP.build_projection_ortho(R,bb.max.y-bb.min.y,bb.min.z,bb.max.z);
	    RCache.set_xform_project(mP);
	    RCache.set_xform_view	(mV);
        EDevice->mFullTransform.mul(mP,mV);
        EDevice->MakeScreenshot	(s_pixels,tgt_w*lod_ss_quality,tgt_h*lod_ss_quality);
        d_pixels.resize 		(tgt_w*tgt_h);
		imf_Process				(d_pixels.data(),tgt_w,tgt_h,s_pixels.data(),tgt_w*lod_ss_quality,tgt_h*lod_ss_quality,imf_box);
        // copy LOD to final
		for (u32 y=0; y<tgt_h; y++)
    		CopyMemory			(tgt_data.data()+y*pitch+frame*tgt_w,d_pixels.data()+y*tgt_w,tgt_w*sizeof(u32));
	}

    // flip data
	for (u32 y=0; y<tgt_h/2; y++){
		u32 y2 = tgt_h-y-1;
		for (u32 x=0; x<pitch; x++){
        	std::swap	(tgt_data[y*pitch+x],tgt_data[y2*pitch+x]);	
		}
	}

    // restore render params
	EDevice->dwFillMode			= old_dwFillMode;
    EDevice->dwShadeMode			= old_dwShadeMode;
    psDeviceFlags 				= old_flag;
    EPrefs->scene_clear_color 	= cc;

    RCache.set_xform_view		(save_view);
    RCache.set_xform_project	(save_projection);
}

void CImageManager::CreateLODTexture(CEditableObject* OBJECT, U32Vec& lod_pixels, U32Vec& nm_pixels, u32 tgt_w, u32 tgt_h, int _samples, int quality)
{
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void CImageManager::CreateLODTexture(CEditableObject* OBJECT, LPCSTR tex_name, u32 tgt_w, u32 tgt_h, int samples, int age, int quality)
{
    U32Vec 						lod_pixels, nm_pixels;

	CreateLODTexture			(OBJECT,lod_pixels,nm_pixels,tgt_w,tgt_h,samples,quality);

    string_path					out_name,src_name;
    CImage* I 					= xr_new<CImage>();
    // save lod
    strcpy						(src_name,tex_name);

    strcpy						(src_name, EFS.ChangeFileExt(src_name,".thm").c_str());
    FS.file_delete				(_textures_,src_name);

    strcpy						(src_name, EFS.ChangeFileExt(src_name,".tga").c_str());
    FS.update_path				(out_name,_textures_,src_name);
    I->Create					(tgt_w*samples,tgt_h,lod_pixels.data());
//	I->Vflip					();
    I->SaveTGA					(out_name);
    FS.set_file_age				(out_name,age);
    SynchronizeTexture			(src_name,age);

    // save normal map
    strconcat					(sizeof(src_name),src_name, tex_name, "_nm");
    strcpy						(src_name,EFS.ChangeFileExt(src_name,".thm").c_str());
    FS.file_delete				(_textures_,src_name);

    strcpy						(src_name, EFS.ChangeFileExt(src_name,".tga").c_str());
    FS.update_path				(out_name,_textures_,src_name);
    I->Create					(tgt_w*samples,tgt_h,nm_pixels.data());
//	I->Vflip					();
    I->SaveTGA					(out_name);
    FS.set_file_age				(out_name,age);
    SynchronizeTexture			(src_name,age);
    
    xr_delete					(I);
}


