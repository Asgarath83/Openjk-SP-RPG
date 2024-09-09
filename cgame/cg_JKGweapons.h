// cg_weapons.h
// Copyright (c) 2013 Jedi Knight Galaxies

#ifndef CG_WEAPONS_H
#define CG_WEAPONS_H

#include "../qcommon/q_shared.h"

// Forward declaration
typedef struct weaponInfo_s weaponInfo_t;
typedef struct centity_s centity_t;

typedef enum weaponAnimNum_e
{
    ABASE,
    E11_IDLE,
    WPROOT
} weaponAnimNum_t;

typedef struct weaponDrawData_s
{
    union
    {
        struct
        {
            float muzzleLightIntensity;
            vec3_t muzzleLightColor;
            fxHandle_t chargingEffect;
            fxHandle_t muzzleEffect;
        } generic;
    } weaponRender;
    
    union
    {
        struct
        {
            sfxHandle_t fireSound[8];
            int fireSoundCount;
        } generic;
    } weaponFire;
    
    union
    {
        struct
        {
            sfxHandle_t tracelineShader;
            float minSize;
            float maxSize;
            int lifeTime;
        } generic;
    } tracelineRender;
    
    union
    {
        sfxHandle_t chargingSound;
    } weaponCharge;
    
    union
    {
        struct
        {
            qhandle_t projectileModel;
            fxHandle_t projectileEffect;
            sfxHandle_t runSound;
            
            float lightIntensity;
            vec3_t lightColor;
            
            fxHandle_t  deathEffect;
        } generic;
    } projectileRender;
    
    union
    {
        struct
        {
            fxHandle_t impactEffect;
        } generic;
        
        struct
        {
            sfxHandle_t stickSound;
        } explosive;
        
        struct
        {
            fxHandle_t impactEffect;
            fxHandle_t shockwaveEffect;
        } grenade;
    } projectileMiss;
    
    union
    {
        struct
        {
            fxHandle_t impactEffect;
        } generic;
        
        struct
        {
            fxHandle_t impactEffect;
            fxHandle_t shockwaveEffect;
        } grenade;
    } projectileHitPlayer;
    
    union
    {
        struct
        {
            fxHandle_t deflectEffect;
        } generic;
    } projectileDeflected;
    
    union
    {
        struct
        {
            sfxHandle_t bounceSound[2];
        } grenade;
    } grenadeBounce;
    
    union
    {
        struct
        {
            void *g2Model;
            float g2Radius;
        } detpack;
        
        struct
        {
            void *g2Model;
            float g2Radius;
            fxHandle_t lineEffect;
        } tripmine;
    } explosiveRender;
    
    union
    {
        struct
        {
            fxHandle_t explodeEffect;
        } generic;
    } explosiveBlow;
    
    union
    {
        sfxHandle_t armSound;
    } explosiveArm;
} weaponDrawData_t;

typedef struct weaponEventsHandler_s
{
    const char *handlerName;

    void (*WeaponRenderWorld) ( centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammo, unsigned char firingMode, const vec3_t angles );
    void (*WeaponRenderView) ( const weaponDrawData_t *weaponData );
    void (*WeaponFire) ( centity_t *cent, const weaponDrawData_t *weaponData, unsigned char firingMode );
    void (*WeaponCharge) ( const centity_t *cent, const weaponDrawData_t *weaponData );
    
    void (*TracelineRender) ( const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t start, const vec3_t end );
    void (*GrenadeBounce) ( const centity_t *cent, const weaponDrawData_t *weaponData );
    
    void (*ExplosiveRender) ( const centity_t *cent, const weaponDrawData_t *weaponData, unsigned char firingMode );
    void (*ExplosiveBlow) ( const centity_t *cent, const weaponDrawData_t *weaponData );
    void (*ExplosiveArm) ( const centity_t *cent, const weaponDrawData_t *weaponData );
    
    void (*ProjectileRender) ( const centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData );
    void (*ProjectileMiss) ( const centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t impactOrigin, const vec3_t impactNormal );
    void (*ProjectileDeath) ( const centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t impactOrigin, const vec3_t impactNormal );
    void (*ProjectileHitPlayer) ( const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t impactOrigin, const vec3_t impactNormal );
    void (*ProjectileDeflected) ( const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal );
} weaponEventsHandler_t;

void CG_InitWeapons ( void );
weaponInfo_t *CG_WeaponInfo ( unsigned int weaponNum, unsigned int variation );
weaponInfo_t *CG_WeaponInfoUnsafe ( unsigned int weaponNum, unsigned int variation );
weaponInfo_t *CG_NextFreeWeaponInfo ( void );

void ChargeWeapon ( const centity_t *cent, qboolean altFire );
void FireWeapon ( centity_t *cent, qboolean altFire );
void RenderWeaponWorldModel ( centity_t *cent, const vec3_t angles );
void RenderWeaponViewModel ( void );

void RenderTraceline ( const centity_t *cent, const vec3_t start, const vec3_t end, unsigned char firingMode );
void BounceGrenade ( const centity_t *cent, unsigned char firingMode );

void BlowExplosive ( const centity_t *cent, qboolean altFire );
void RenderExplosive ( const centity_t *cent, unsigned char firingMode );
void JKG_ArmExplosive ( const centity_t *cent, unsigned char firingMode );

void RenderProjectile ( const centity_t *cent, unsigned char firingMode );
void RenderProjectileMiss ( const centity_t *cent, const vec3_t origin, const vec3_t direction, qboolean altFire );
void RenderProjectileDeath ( const centity_t *cent, const vec3_t origin, const vec3_t direction, unsigned char firingMode );
void RenderProjectileHitPlayer ( const centity_t *cent, const vec3_t origin, const vec3_t direction, qboolean altFire );

void ToggleScope ( const centity_t *cent );
void ZoomScope ( const centity_t *cent );
void RenderScope ( const centity_t *cent );

void SetWeaponEventsHandler ( weaponInfo_t *weaponInfo, const char *eventHandlerName, unsigned char firingMode );
qboolean ShouldRenderWeaponViewModel ( const centity_t *cent, const playerState_t *ps );

#endif
