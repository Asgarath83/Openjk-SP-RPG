///////////////////////////////////////////// //////////////
///////////////////////////////////////////////////////////
 /// //////////////////////////////////////////////////////
///////////////QQQQJKGQQQQQQ CLIENT GAME WEAPONS... 
//// LAST CHANCE OF FIX THAT, OKAY?? -.- 

// cg_weapons.h

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



#define MAX_WEAPON_ANIMATIONS (16)
typedef struct GWInfo_s {
	   unsigned int    weaponNum;
    unsigned int    variation;

	qboolean		registered;
	gitem_t			*item;

	qhandle_t		handsModel;			// the hands don't actually draw, they just position the weapon
	void            *g2WorldModel;      // Used to be in g2WeaponInstances
	qhandle_t		weaponModel;		// this is the pickup model
	qhandle_t		viewModel;			// this is the in-view model used by the player
	qhandle_t		barrelModels[4];
	qhandle_t		flashModel;
	
	void            *g2ViewModel;
	qhandle_t       viewModelSkin;
	animation_t     viewModelAnims[MAX_WEAPON_ANIMATIONS];

	vec3_t			weaponMidpoint;		// so it will rotate centered instead of by tag

	float			flashDlight;
	vec3_t			flashDlightColor;

	qhandle_t		hudIcon;
	qhandle_t		hudNAIcon;

	qhandle_t		ammoModel;
	
	qhandle_t       fireModeIndicator;
	qhandle_t       groupedIndicators[3];
	indicatorType_t indicatorType;
	
	vec3_t          gunPosition;
	vec3_t          ironsightsPosition;
	float           ironsightsFov;
	
    qhandle_t       scopeShader;
    sfxHandle_t     scopeStartSound;
    sfxHandle_t     scopeStopSound;
    int             scopeSoundLoopTime;
    sfxHandle_t     scopeLoopSound;
	
	//const weaponEventsHandler_t *primaryEventsHandler;
	//const weaponEventsHandler_t *altEventsHandler;

	const weaponEventsHandler_t *eventsHandler[MAX_FIREMODES];
	
	//weaponDrawData_t primDrawData;
	//weaponDrawData_t altDrawData;
	weaponDrawData_t drawData[MAX_FIREMODES];

	sfxHandle_t		flashSound[4];		// fast firing weapons randomly choose
	sfxHandle_t		firingSound;
	sfxHandle_t		chargeSound;
	fxHandle_t		muzzleEffect;
	qhandle_t		missileModel;
	sfxHandle_t		missileSound;
	void			(*missileTrailFunc)( centity_t *, const struct weaponInfo_s *wi );
	float			missileDlight;
	vec3_t			missileDlightColor;
	int				missileRenderfx;
	sfxHandle_t		missileHitSound;

	sfxHandle_t		altFlashSound[4];
	sfxHandle_t		altFiringSound;
	sfxHandle_t		altChargeSound;
	fxHandle_t		altMuzzleEffect;
	qhandle_t		altMissileModel;
	sfxHandle_t		altMissileSound;
	void			(*altMissileTrailFunc)( centity_t *, const struct weaponInfo_s *wi );
	float			altMissileDlight;
	vec3_t			altMissileDlightColor;
	int				altMissileRenderfx;
	sfxHandle_t		altMissileHitSound;

	sfxHandle_t		selectSound;

	sfxHandle_t		readySound;
	float			trailRadius;
	float			wiTrailTime;
} 
GWInfo_t;


void GW_InitWeapons ( void );
GWInfo_t *GWInfo ( unsigned int weaponNum, unsigned int variation );
GWInfo_t *GWWeaponInfoUnsafe ( unsigned int weaponNum, unsigned int variation );
GWInfo_t *GWNextFreeWeaponInfo ( void );
// JKG GALAXY 
void ChargeWeapon ( const centity_t *cent, qboolean altFire );
void FireWeapon ( centity_t *cent, qboolean altFire );
void RenderWeaponWorldModel ( centity_t *cent, const vec3_t angles );
void RenderWeaponViewModel ( void );

void RenderTraceline ( const centity_t *cent, const vec3_t start, const vec3_t end, unsigned char firingMode );
void BounceGrenade ( const centity_t *cent, unsigned char firingMode );

void BlowExplosive ( const centity_t *cent, qboolean altFire );
void RenderExplosive ( const centity_t *cent, unsigned char firingMode );
void ArmExplosive ( const centity_t *cent, unsigned char firingMode );

void RenderProjectile ( const centity_t *cent, unsigned char firingMode );
void RenderProjectileMiss ( const centity_t *cent, const vec3_t origin, const vec3_t direction, qboolean altFire );
void RenderProjectileDeath ( const centity_t *cent, const vec3_t origin, const vec3_t direction, unsigned char firingMode );
void RenderProjectileHitPlayer ( const centity_t *cent, const vec3_t origin, const vec3_t direction, qboolean altFire );

void ToggleScope ( const centity_t *cent );
void ZoomScope ( const centity_t *cent );
void RenderScope ( const centity_t *cent );

void SetWeaponEventsHandler ( weaponInfo_t *weaponInfo, const char *eventHandlerName, unsigned char firingMode );
qboolean ShouldRenderWeaponViewModel ( const centity_t *cent, const playerState_t *ps );
