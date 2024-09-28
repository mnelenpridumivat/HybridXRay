#pragma once

namespace PAPI
{
    //////////////////////////////////////////////////////////////////////
    // Type codes for all actions
    enum PActionEnum
    {
        PAAvoidID,                      // Avoid entering the domain of space.
        PABounceID,                     // Bounce particles off a domain of space.
        PACallActionListID_obsolette,   //
        PACopyVertexBID,                // Set the secondary position from current position.
        PADampingID,                    // Dampen particle velocities.
        PAExplosionID,                  // An Explosion.
        PAFollowID,                     // Accelerate toward the previous particle in the effect.
        PAGravitateID,                  // Accelerate each particle toward each other particle.
        PAGravityID,                    // Acceleration in the given direction.
        PAJetID,                        //
        PAKillOldID,                    //
        PAMatchVelocityID,              //
        PAMoveID,                       //
        PAOrbitLineID,                  //
        PAOrbitPointID,                 //
        PARandomAccelID,                //
        PARandomDisplaceID,             //
        PARandomVelocityID,             //
        PARestoreID,                    //
        PASinkID,                       //
        PASinkVelocityID,               //
        PASourceID,                     //
        PASpeedLimitID,                 //
        PATargetColorID,                //
        PATargetSizeID,                 //
        PATargetRotateID,               //
        PATargetRotateDID,              //
        PATargetVelocityID,             //
        PATargetVelocityDID,            //
        PAVortexID,                     //
        PATurbulenceID,                 //
        PAScatterID,                    //
        PABindTimeLimitID,
        PABindMaxParticlesID,
        PABindSourceDomainID,
        PABindSourceVelocityID,
        PABindSourceRotationID,
        PABindSourceSizeID,
        PABindSourceColorID,
        PABindSourceAlphaID,
        PABindVelocityValueID,
        PABindVelocityScaleID,
        PABindVelocityAllowRotateID,
        PABindRotationValueID,
        PABindRotationScaleID,
        PABindSizeValueID,
        PABindSizeScaleID,
        PABindColorValueID,
        PABindColorAlphaID,
        PABindColorScaleID,
        PABindColorTimeFromID,
        PABindColorTimeToID,
        PANamedBindVelocityValueID,
        PANamedBindRotationValueID,
        PANamedBindSizeValueID,
        PANamedBindColorValueID,
        PANamedBindColorAlphaID,
        action_enum_force_dword = u32(-1)
    };
}   // namespace PAPI