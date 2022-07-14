#include "stdafx.h"
#pragma hdrstop

#include "ParticleEffect.h"

#ifndef REDITOR
#include <xmmintrin.h>
#include "../../xrCPU_Pipe/ttapi.h"
#endif

using namespace PS;

const u32	PS::uDT_STEP 	= 33;
const float	PS::fDT_STEP 	= float(uDT_STEP)/1000.f;

static void ApplyTexgen( const Fmatrix &mVP )
{
}

void PS::OnEffectParticleBirth(void* owner, u32 , PAPI::Particle& m, u32 )
{
}
void PS::OnEffectParticleDead(void* , u32 , PAPI::Particle& , u32 )
{
//	CPEDef* PE = static_cast<CPEDef*>(owner);
}
//------------------------------------------------------------------------------
// class CParticleEffect
//------------------------------------------------------------------------------
CParticleEffect::CParticleEffect()
{
	m_RT_Flags.zero			();
	m_Def					= 0;
	m_fElapsedLimit			= 0.f;
	m_MemDT					= 0;
	m_InitialPosition.set	(0,0,0);
	m_DestroyCallback		= 0;
	m_CollisionCallback		= 0;
	m_XFORM.identity		();
}
CParticleEffect::~CParticleEffect()
{
	// Log					("--- destroy PE");
	OnDeviceDestroy			();
}

void CParticleEffect::Play()
{
	m_RT_Flags.set		(flRT_DefferedStop,FALSE);
	m_RT_Flags.set		(flRT_Playing,TRUE);
}
void CParticleEffect::Stop(BOOL bDefferedStop)
{
}
void CParticleEffect::RefreshShader()
{
	OnDeviceDestroy();
	OnDeviceCreate();
}

void CParticleEffect::UpdateParent(const Fmatrix& m, const Fvector& velocity, BOOL bXFORM)
{
}

void CParticleEffect::OnFrame(u32 frame_dt)
{
}

BOOL CParticleEffect::Compile(CPEDef* def)
{
	return TRUE;
}

u32 CParticleEffect::ParticlesCount()
{
	return 0;
}

//------------------------------------------------------------------------------
// Render
//------------------------------------------------------------------------------
void CParticleEffect::Copy(dxRender_Visual* )
{
	FATAL	("Can't duplicate particle system - NOT IMPLEMENTED");
}

void CParticleEffect::OnDeviceCreate()
{
	if (m_Def){
		if (m_Def->m_Flags.is(CPEDef::dfSprite)){
			geom.create			(FVF::F_LIT, RCache.Vertex.Buffer(), RCache.QuadIB);
			if (m_Def) shader	= m_Def->m_CachedShader;
		}
	}
}

void CParticleEffect::OnDeviceDestroy()
{
	if (m_Def){
		if (m_Def->m_Flags.is(CPEDef::dfSprite)){
			geom.destroy		();
			shader.destroy		();
		}    
	}
}

#ifndef REDITOR
//----------------------------------------------------
IC void FillSprite_fpu	(FVF::LIT*& pv, const Fvector& T, const Fvector& R, const Fvector& pos, const Fvector2& lt, const Fvector2& rb, float r1, float r2, u32 clr, float angle)
{
	float sa	= _sin(angle);  
	float ca	= _cos(angle);  
	
	Fvector Vr, Vt;
	
	Vr.x 		= T.x*r1*sa+R.x*r1*ca;
	Vr.y 		= T.y*r1*sa+R.y*r1*ca;
	Vr.z 		= T.z*r1*sa+R.z*r1*ca;
	
	Vt.x 		= T.x*r2*ca-R.x*r2*sa;
	Vt.y 		= T.y*r2*ca-R.y*r2*sa;
	Vt.z 		= T.z*r2*ca-R.z*r2*sa;

	Fvector 	a,b,c,d;

	a.sub		(Vt,Vr);
	b.add		(Vt,Vr);

	c.invert	(a);
	d.invert	(b);

	pv->set		(d.x+pos.x,d.y+pos.y,d.z+pos.z, clr, lt.x,rb.y);	pv++;
	pv->set		(a.x+pos.x,a.y+pos.y,a.z+pos.z, clr, lt.x,lt.y);	pv++;
	pv->set		(c.x+pos.x,c.y+pos.y,c.z+pos.z, clr, rb.x,rb.y);	pv++;
	pv->set		(b.x+pos.x,b.y+pos.y,b.z+pos.z,	clr, rb.x,lt.y);	pv++;
}
/*
__forceinline void fsincos( const float angle , float &sine , float &cosine )
{ __asm {
    fld			DWORD PTR [angle]
    fsincos
    mov			eax , DWORD PTR [cosine]
    fstp		DWORD PTR [eax]
    mov			eax , DWORD PTR [sine]
    fstp		DWORD PTR [eax]
} }*/


IC void FillSprite	(FVF::LIT*& pv, const Fvector& T, const Fvector& R, const Fvector& pos, const Fvector2& lt, const Fvector2& rb, float r1, float r2, u32 clr, float sina , float cosa )
{
	__m128 Vr, Vt, _T , _R , _pos , _zz , _sa , _ca , a , b , c , d;

	_sa = _mm_set1_ps( sina );
	_ca = _mm_set1_ps( cosa );

	_T = _mm_load_ss( (float*) &T.x );
	_T = _mm_loadh_pi( _T , (__m64*) &T.y );

	_R = _mm_load_ss( (float*) &R.x );
	_R = _mm_loadh_pi( _R , (__m64*) &R.y );

	_pos = _mm_load_ss( (float*) &pos.x );
	_pos = _mm_loadh_pi( _pos , (__m64*) &pos.y );

	_zz = _mm_setzero_ps();

	Vr = _mm_mul_ps( _mm_set1_ps( r1 ) , _mm_add_ps( _mm_mul_ps( _T , _sa ) , _mm_mul_ps( _R , _ca ) ) );
	Vt = _mm_mul_ps( _mm_set1_ps( r2 ) , _mm_sub_ps( _mm_mul_ps( _T , _ca ) , _mm_mul_ps( _R , _sa ) ) );

	a = _mm_sub_ps( Vt , Vr );
	b = _mm_add_ps( Vt , Vr );
	c = _mm_sub_ps( _zz , a );
	d = _mm_sub_ps( _zz , b );

	a = _mm_add_ps( a , _pos );
	d = _mm_add_ps( d , _pos );
	b = _mm_add_ps( b , _pos );
	c = _mm_add_ps( c , _pos );

	_mm_store_ss( (float*) &pv->p.x , d );
	_mm_storeh_pi( (__m64*) &pv->p.y , d );
	pv->color = clr;
	pv->t.set( lt.x , rb.y );
	pv++;

	_mm_store_ss( (float*) &pv->p.x , a );
	_mm_storeh_pi( (__m64*) &pv->p.y , a );
	pv->color = clr;
	pv->t.set( lt.x , lt.y );
	pv++;

	_mm_store_ss( (float*) &pv->p.x , c );
	_mm_storeh_pi( (__m64*) &pv->p.y , c );
	pv->color = clr;
	pv->t.set( rb.x , rb.y );
	pv++;

	_mm_store_ss( (float*) &pv->p.x , b );
	_mm_storeh_pi( (__m64*) &pv->p.y , b );
	pv->color = clr;
	pv->t.set( rb.x , lt.y );
	pv++;
}

IC void FillSprite	(FVF::LIT*& pv, const Fvector& pos, const Fvector& dir, const Fvector2& lt, const Fvector2& rb, float r1, float r2, u32 clr, float sina , float cosa )
{
	const Fvector& T 	= dir;
	Fvector R; 	

	// R.crossproduct(T,Device->vCameraDirection).normalize_safe();

	__m128 _t , _t1 , _t2 , _r , _r1 , _r2 ;

	// crossproduct

	_t = _mm_load_ss( (float*) &T.x );
	_t = _mm_loadh_pi( _t , (__m64*) &T.y );

	_r = _mm_load_ss( (float*) &Device->vCameraDirection.x );
	_r = _mm_loadh_pi( _r , (__m64*) &Device->vCameraDirection.y );

	_t1 = _mm_shuffle_ps( _t , _t , _MM_SHUFFLE( 0 , 3 , 1 , 2 ) );
	_t2 = _mm_shuffle_ps( _t , _t , _MM_SHUFFLE( 2 , 0 , 1 , 3 ) );

	_r1 = _mm_shuffle_ps( _r , _r , _MM_SHUFFLE( 2 , 0 , 1 , 3 ) );
	_r2 = _mm_shuffle_ps( _r , _r , _MM_SHUFFLE( 0 , 3 , 1 , 2 ) );

	_t1 = _mm_mul_ps( _t1 , _r1 );
	_t2 = _mm_mul_ps( _t2 , _r2 );

	_t1 = _mm_sub_ps( _t1 , _t2 );			// z | y | 0 | x

	// normalize_safe

	_t2 = _mm_mul_ps( _t1 , _t1 );			// zz | yy | 00 | xx
	_r1 = _mm_movehl_ps( _t2 , _t2 );		// zz | yy | zz | yy
	_t2 = _mm_add_ss( _t2 , _r1 );			// zz | yy | 00 | xx + yy
	_r1 = _mm_shuffle_ps( _r1 , _r1 , _MM_SHUFFLE( 1 , 1 , 1 , 1 ) );		// zz | zz | zz | zz
	_t2 = _mm_add_ss( _t2 , _r1 );			// zz | yy | 00 | xx + yy + zz

	_r1 = _mm_set_ss( std::numeric_limits<float>::min() );

	if ( _mm_comigt_ss( _t2 , _r1 ) ) {
		_t2 = _mm_rsqrt_ss( _t2 );
		_t2 = _mm_shuffle_ps( _t2 , _t2 , _MM_SHUFFLE( 0 , 0 , 0 , 0 ) );
		_t1 = _mm_mul_ps( _t1 , _t2 );
	}

	_mm_store_ss( (float*) &R.x , _t1 );
	_mm_storeh_pi( (__m64*) &R.y , _t1 );

	FillSprite( pv , T , R , pos , lt , rb , r1 , r2 , clr , sina , cosa );
}

extern ENGINE_API float		psHUD_FOV;

struct PRS_PARAMS {
	FVF::LIT* pv;
	u32 p_from;
	u32 p_to;
	PAPI::Particle* particles;
	CParticleEffect* pPE;
};

__forceinline void magnitude_sse( Fvector &vec , float &res )
{
	__m128 tv,tu;

	tv = _mm_load_ss( (float*) &vec.x );		// tv = 0 | 0 | 0 | x
	tv = _mm_loadh_pi( tv , (__m64*) &vec.y );	// tv = z | y | 0 | x
	tv = _mm_mul_ps( tv , tv );					// tv = zz | yy | 0 | xx
	tu = _mm_movehl_ps( tv , tv );				// tu = zz | yy | zz | yy
	tv = _mm_add_ss( tv , tu );					// tv = zz | yy | 0 | xx + yy
	tu = _mm_shuffle_ps( tu , tu , _MM_SHUFFLE( 1 , 1 , 1 , 1 ) ); // tu = zz | zz | zz | zz
	tv = _mm_add_ss( tv , tu );					// tv = zz | yy | 0 | xx + yy + zz
	tv = _mm_sqrt_ss( tv );						// tv = zz | yy | 0 | sqrt( xx + yy + zz )
	_mm_store_ss( (float*) &res , tv );
}

void ParticleRenderStream( LPVOID lpvParams )
{
			float sina = 0.0f , cosa = 1.0f;
			float angle = 0.0f;

			PRS_PARAMS* pParams = (PRS_PARAMS *) lpvParams;

			FVF::LIT* pv = pParams->pv;
			u32 p_from = pParams->p_from;
			u32 p_to = pParams->p_to;
			PAPI::Particle* particles = pParams->particles;
			CParticleEffect &pPE = *pParams->pPE;

			for(u32 i = p_from; i < p_to; i++){
				PAPI::Particle &m = particles[i];
				Fvector2 lt,rb;
				lt.set			(0.f,0.f);
				rb.set			(1.f,1.f);

				_mm_prefetch( (char*) &particles[i + 1] , _MM_HINT_NTA );
				
				if ( angle != m.rot.x) 
				{
					angle = m.rot.x;
					cosa = cosf(angle);
					sina = sinf(angle);
				}

				_mm_prefetch( 64 + (char*) &particles[i + 1] , _MM_HINT_NTA );

				if (pPE.m_Def->m_Flags.is(CPEDef::dfFramed))
					pPE.m_Def->m_Frame.CalculateTC(iFloor(float(m.frame)/255.f),lt,rb);

				float r_x		= m.size.x*0.5f;
				float r_y		= m.size.y*0.5f;
				float speed;
				BOOL speed_calculated = FALSE;

				if (pPE.m_Def->m_Flags.is(CPEDef::dfVelocityScale)){
					magnitude_sse( m.vel , speed );
					speed_calculated = TRUE;
					r_x			+= speed*pPE.m_Def->m_VelocityScale.x;
					r_y			+= speed*pPE.m_Def->m_VelocityScale.y;
				}

				if (pPE.m_Def->m_Flags.is(CPEDef::dfAlignToPath)){
					if ( ! speed_calculated )
						magnitude_sse( m.vel , speed );
                    if ((speed<EPS_S)&&pPE.m_Def->m_Flags.is(CPEDef::dfWorldAlign)){
                    	Fmatrix	M;  	
                        M.setXYZ			(pPE.m_Def->m_APDefaultRotation);
						if (pPE.m_RT_Flags.is(CParticleEffect::flRT_XFORM)){
                            Fvector p;
                            pPE.m_XFORM.transform_tiny(p,m.pos);
	                        M.mulA_43		(pPE.m_XFORM);
                            FillSprite		(pv,M.k,M.i,p,lt,rb,r_x,r_y,m.color,sina,cosa);
                        }else{
                            FillSprite		(pv,M.k,M.i,m.pos,lt,rb,r_x,r_y,m.color,sina,cosa);
                        }
                    }else if ((speed>=EPS_S)&&pPE.m_Def->m_Flags.is(CPEDef::dfFaceAlign)){
                    	Fmatrix	M;  		M.identity();
                        M.k.div				(m.vel,speed);            
                        M.j.set 			(0,1,0);	if (_abs(M.j.dotproduct(M.k))>.99f)  M.j.set(0,0,1);
                        M.i.crossproduct	(M.j,M.k);	M.i.normalize	();
                        M.j.crossproduct   	(M.k,M.i);	M.j.normalize  ();
						if (pPE.m_RT_Flags.is(CParticleEffect::flRT_XFORM)){
                            Fvector p;
                            pPE.m_XFORM.transform_tiny(p,m.pos);
	                        M.mulA_43		(pPE.m_XFORM);
                            FillSprite		(pv,M.j,M.i,p,lt,rb,r_x,r_y,m.color,sina,cosa);
                        }else{
                            FillSprite		(pv,M.j,M.i,m.pos,lt,rb,r_x,r_y,m.color,sina,cosa);
                        }
                    }else{
						Fvector 			dir;
                        if (speed>=EPS_S)	dir.div	(m.vel,speed);
                        else				dir.setHP(-pPE.m_Def->m_APDefaultRotation.y,-pPE.m_Def->m_APDefaultRotation.x);
						if (pPE.m_RT_Flags.is(CParticleEffect::flRT_XFORM)){
                            Fvector p,d;
                            pPE.m_XFORM.transform_tiny	(p,m.pos);
                            pPE.m_XFORM.transform_dir	(d,dir);
                            FillSprite	(pv,p,d,lt,rb,r_x,r_y,m.color,sina,cosa);
                        }else{
                            FillSprite	(pv,m.pos,dir,lt,rb,r_x,r_y,m.color,sina,cosa);
                        }
                    }
				}else{
					if (pPE.m_RT_Flags.is(CParticleEffect::flRT_XFORM)){
						Fvector p;
						pPE.m_XFORM.transform_tiny	(p,m.pos);
						FillSprite	(pv,Device->vCameraTop,Device->vCameraRight,p,lt,rb,r_x,r_y,m.color,sina,cosa);
					}else{
						FillSprite	(pv,Device->vCameraTop,Device->vCameraRight,m.pos,lt,rb,r_x,r_y,m.color,sina,cosa);
					}
				}
			}
}

void CParticleEffect::Render(float )
{
	u32			dwOffset,dwCount;
	// Get a pointer to the particles in gp memory
    PAPI::Particle* particles;
    u32 			p_cnt;
    ParticleManager()->GetParticles(m_HandleEffect,particles,p_cnt);

	if(p_cnt>0){
		if (m_Def&&m_Def->m_Flags.is(CPEDef::dfSprite)){
			FVF::LIT* pv_start	= (FVF::LIT*)RCache.Vertex.Lock(p_cnt*4*4,geom->vb_stride,dwOffset);
			FVF::LIT* pv		= pv_start;

			u32 nWorkers = ttapi_GetWorkersCount();

			if ( p_cnt < ( nWorkers * 64 ) )
				nWorkers = 1;

			PRS_PARAMS* prsParams = (PRS_PARAMS*) _alloca( sizeof(PRS_PARAMS) * nWorkers );

			// Give ~1% more for the last worker
			// to minimize wait in final spin
			u32 nSlice = p_cnt / 128;

			u32 nStep = ( ( p_cnt - nSlice ) / nWorkers );

			for ( u32 i = 0 ; i < nWorkers ; ++i ) {
				prsParams[i].pv = pv + i*nStep*4;
				prsParams[i].p_from = i * nStep;
				prsParams[i].p_to = ( i == ( nWorkers - 1 ) ) ? p_cnt : ( prsParams[i].p_from + nStep );
				prsParams[i].particles = particles;
				prsParams[i].pPE = this;
				ttapi_AddWorker( ParticleRenderStream , (LPVOID) &prsParams[i] );
			}

			ttapi_RunAllWorkers();

			dwCount = p_cnt<<2;

			RCache.Vertex.Unlock(dwCount,geom->vb_stride);
			if (dwCount)    
			{
#ifndef REDITOR
				Fmatrix Pold						= Device->mProject;
				Fmatrix FTold						= Device->mFullTransform;
				if(GetHudMode())
				{
					Device->mProject.build_projection(	deg2rad(psHUD_FOV*Device->fFOV), 
														Device->fASPECT, 
														VIEWPORT_NEAR, 
														g_pGamePersistent->Environment().CurrentEnv->far_plane);

					Device->mFullTransform.mul	(Device->mProject, Device->mView);
					RCache.set_xform_project	(Device->mProject);
					RImplementation.rmNear		();
					ApplyTexgen(Device->mFullTransform);
				}
#endif

				RCache.set_xform_world	(Fidentity);
				RCache.set_Geometry		(geom);

                RCache.set_CullMode		(m_Def->m_Flags.is(CPEDef::dfCulling)?(m_Def->m_Flags.is(CPEDef::dfCullCCW)?CULL_CCW:CULL_CW):CULL_NONE);
				RCache.Render	   		(D3DPT_TRIANGLELIST,dwOffset,0,dwCount,0,dwCount/2);
                RCache.set_CullMode		(CULL_CCW	); 
#ifndef REDITOR
				if(GetHudMode())
				{
					RImplementation.rmNormal	();
					Device->mProject				= Pold;
					Device->mFullTransform		= FTold;
					RCache.set_xform_project	(Device->mProject);
					ApplyTexgen(Device->mFullTransform);
				}
#endif
			}
		}
	}
}


#else

//----------------------------------------------------
IC void FillSprite	(FVF::LIT*& pv, const Fvector& T, const Fvector& R, const Fvector& pos, const Fvector2& lt, const Fvector2& rb, float r1, float r2, u32 clr, float angle)
{
	float sa	= _sin(angle);  
	float ca	= _cos(angle);  
	Fvector Vr, Vt;
	Vr.x 		= T.x*r1*sa+R.x*r1*ca;
	Vr.y 		= T.y*r1*sa+R.y*r1*ca;
	Vr.z 		= T.z*r1*sa+R.z*r1*ca;
	Vt.x 		= T.x*r2*ca-R.x*r2*sa;
	Vt.y 		= T.y*r2*ca-R.y*r2*sa;
	Vt.z 		= T.z*r2*ca-R.z*r2*sa;

	Fvector 	a,b,c,d;
	a.sub		(Vt,Vr);
	b.add		(Vt,Vr);
	c.invert	(a);
	d.invert	(b);
	pv->set		(d.x+pos.x,d.y+pos.y,d.z+pos.z, clr, lt.x,rb.y);	pv++;
	pv->set		(a.x+pos.x,a.y+pos.y,a.z+pos.z, clr, lt.x,lt.y);	pv++;
	pv->set		(c.x+pos.x,c.y+pos.y,c.z+pos.z, clr, rb.x,rb.y);	pv++;
	pv->set		(b.x+pos.x,b.y+pos.y,b.z+pos.z,	clr, rb.x,lt.y);	pv++;
}

IC void FillSprite	(FVF::LIT*& pv, const Fvector& pos, const Fvector& dir, const Fvector2& lt, const Fvector2& rb, float r1, float r2, u32 clr, float angle)
{
	float sa	= _sin(angle);  
	float ca	= _cos(angle);  
	const Fvector& T 	= dir;
	Fvector R; 	R.crossproduct(T,Device->vCameraDirection).normalize_safe();
	Fvector Vr, Vt;
	Vr.x 		= T.x*r1*sa+R.x*r1*ca;
	Vr.y 		= T.y*r1*sa+R.y*r1*ca;
	Vr.z 		= T.z*r1*sa+R.z*r1*ca;
	Vt.x 		= T.x*r2*ca-R.x*r2*sa;
	Vt.y 		= T.y*r2*ca-R.y*r2*sa;
	Vt.z 		= T.z*r2*ca-R.z*r2*sa;

	Fvector 	a,b,c,d;
	a.sub		(Vt,Vr);
	b.add		(Vt,Vr);
	c.invert	(a);
	d.invert	(b);
	pv->set		(d.x+pos.x,d.y+pos.y,d.z+pos.z, clr, lt.x,rb.y);	pv++;
	pv->set		(a.x+pos.x,a.y+pos.y,a.z+pos.z, clr, lt.x,lt.y);	pv++;
	pv->set		(c.x+pos.x,c.y+pos.y,c.z+pos.z, clr, rb.x,rb.y);	pv++;
	pv->set		(b.x+pos.x,b.y+pos.y,b.z+pos.z,	clr, rb.x,lt.y);	pv++;
}

extern ENGINE_API float		psHUD_FOV;
void CParticleEffect::Render(float )
{
}

#endif  // REDITOR
