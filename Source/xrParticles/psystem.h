#include "particle_param_handle.h"
#ifndef PSystemH
#define PSystemH
#pragma once

#ifdef SHIPPING
#define PARTICLES_API
#else
#ifdef XRPARTICLES_EXPORTS
#define PARTICLES_API __declspec(dllexport)
#else
#define PARTICLES_API __declspec(dllimport)
#endif
#endif

// Actually this must be < sqrt(MAXFLOAT) since we store this value squared.
#define P_MAXFLOAT 1.0e16f

#ifdef MAXINT
#define P_MAXINT MAXINT
#else
#define P_MAXINT 0x7fffffff
#endif

#define drand48() ::Random.randF()
// #define drand48() (((float) rand())/((float) RAND_MAX))
#include "PActionEnum.h"
#include "pvector.h"

namespace PAPI
{
    // A single particle
    struct Particle
    {
        enum
        {
            ANIMATE_CCW = (1 << 0),
        };
        pVector pos;     // 12
        pVector posB;    // 12
        pVector vel;     // 12
        pVector size;    // 12
        pVector rot;     // 12   60
        u32     color;   // 4
        float   age;     // 4
        u16     frame;   // 2
        Flags16 flags;   // 2
    };                   // 		72

    typedef void (*OnBirthParticleCB)(void* owner, u32 param, PAPI::Particle& P, u32 idx);
    typedef void (*OnDeadParticleCB)(void* owner, u32 param, PAPI::Particle& P, u32 idx);
    //////////////////////////////////////////////////////////////////////
    // Type codes for domains
    enum PDomainEnum
    {
        PDPoint                 = 0,    // Single point
        PDLine                  = 1,    // Line segment
        PDTriangle              = 2,    // Triangle
        PDPlane                 = 3,    // Arbitrarily-oriented plane
        PDBox                   = 4,    // Axis-aligned box
        PDSphere                = 5,    // Sphere
        PDCylinder              = 6,    // Cylinder
        PDCone                  = 7,    // Cone
        PDBlob                  = 8,    // Gaussian blob
        PDDisc                  = 9,    // Arbitrarily-oriented disc
        PDRectangle             = 10,   // Rhombus-shaped planar region
        domain_enum_force_dword = u32(-1)
    };
    struct ParticleAction;

    class IParticleManager
    {
    public:
        IParticleManager() {}
        virtual ~IParticleManager() {}

        // create&destroy
        virtual int             CreateEffect(u32 max_particles)                                                             = 0;
        virtual void            DestroyEffect(int effect_id)                                                                = 0;
        virtual int             CreateActionList()                                                                          = 0;
        virtual void            DestroyActionList(int alist_id)                                                             = 0;

        virtual ParticleAction* FindAction(int alist_id, PActionEnum Type) = 0;
        virtual ParticleAction* FindAction(int alist_id, xr_string Name) = 0;

        virtual void            FindAllFloatActions(int alist_id, xr_vector<ParticleAction*>& handles)                = 0;
        virtual void            FindAllVectorActions(int alist_id, xr_vector<ParticleAction*>& handles)               = 0;
        

        // control
        virtual void            PlayEffect(int effect_id, int alist_id)                                                     = 0;
        virtual void            StopEffect(int effect_id, int alist_id, BOOL deffered = TRUE)                               = 0;

        // update&render
        virtual void            Update(int effect_id, int alist_id, float dt)                                               = 0;
        virtual void            Render(int effect_id)                                                                       = 0;
        virtual void            Transform(int alist_id, const Fmatrix& m, const Fvector& velocity)                          = 0;

        // effect
        virtual void            RemoveParticle(int effect_id, u32 p_id)                                                     = 0;
        virtual void            SetMaxParticles(int effect_id, u32 max_particles)                                           = 0;
        virtual void            SetCallback(int effect_id, OnBirthParticleCB b, OnDeadParticleCB d, void* owner, u32 param) = 0;
        virtual void            GetParticles(int effect_id, Particle*& particles, u32& cnt)                                 = 0;
        virtual u32             GetParticlesCount(int effect_id)                                                            = 0;

        // action
        virtual ParticleAction* CreateAction(PActionEnum type)                                                              = 0;
        virtual u32             LoadActions(int alist_id, IReader& R)                                                       = 0;
        virtual void            SaveActions(int alist_id, IWriter& W)                                                       = 0;
    };

    PARTICLES_API IParticleManager* ParticleManager();
};       // namespace PAPI
#endif   // PSystemH
