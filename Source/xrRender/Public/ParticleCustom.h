#ifndef ParticleCustom_included
#define ParticleCustom_included
#pragma once

#include "../../xrParticles/particle_param_handle.h"
//#include "../../xrParticles/psystem.h"

#include "../../xrParticles/particle_actions.h"
#include "../../xrParticles/pvector.h"

class IParticleCustom
{
public:
    virtual ~IParticleCustom()
    {
        ;
    }

    virtual PAPI::Handle<float>         GetAlphaHandle(u32 EffectIndex)                                      = 0;
    virtual PAPI::Handle<PAPI::pVector> GetColorHandle(u32 EffectIndex)                                      = 0;
    virtual PAPI::Handle<PAPI::pVector> GetSizeHandle(u32 EffectIndex)                                       = 0;
    virtual PAPI::Handle<PAPI::pVector> GetVelocityHandle(u32 EffectIndex)                                   = 0;
    virtual PAPI::Handle<PAPI::pVector> GetRotationHandle(u32 EffectIndex)                                   = 0;

    virtual PAPI::Handle<float>         GetFloatHandle(xr_string Name)                                       = 0;
    virtual void                        GetAllFloatHandles(xr_vector<PAPI::ParticleAction*>& handles)           = 0;
    virtual PAPI::Handle<PAPI::pVector> GetVectorHandle(xr_string Name)                                      = 0;
    virtual void                        GetAllVectorHandles(xr_vector<PAPI::ParticleAction*>& handles)          = 0;

    virtual void  OnDeviceCreate()                                                     = 0;
    virtual void  OnDeviceDestroy()                                                    = 0;

    virtual void  UpdateParent(const Fmatrix& m, const Fvector& velocity, BOOL bXFORM) = 0;
    virtual void  OnFrame(u32 dt)                                                      = 0;

    virtual void  Play()                                                               = 0;
    virtual void  Stop(BOOL bDefferedStop = TRUE)                                      = 0;
    virtual BOOL  IsPlaying()                                                          = 0;

    virtual u32   ParticlesCount()                                                     = 0;

    virtual float GetTimeLimit()                                                       = 0;
    virtual BOOL  IsLooped()
    {
        return GetTimeLimit() < 0.f;
    }

    virtual const shared_str Name()             = 0;
    virtual void             SetHudMode(BOOL b) = 0;
    virtual BOOL             GetHudMode()       = 0;
};

#endif   //	ParticleCustom_included
