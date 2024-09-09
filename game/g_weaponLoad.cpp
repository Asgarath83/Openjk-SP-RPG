/*
This file is part of Jedi Academy.

    Jedi Academy is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Jedi Academy is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jedi Academy.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright 2001-2013 Raven Software

// g_weaponLoad.cpp 
// fills in memory struct with ext_dat\weapons.dat

// this is excluded from PCH usage 'cos it looks kinda scary to me, being game and ui.... -Ste

#ifdef _USRDLL	//UI dll

#include "../ui/gameinfo.h"
#include "weapons.h"
extern	gameinfo_import_t	gi;
extern weaponData_t weaponData[];
extern ammoData_t ammoData[];

#else	//we are in the game

// ONLY DO THIS ON THE GAME SIDE
#include "g_local.h"

typedef struct {
	const char	*name;
	void	(*func)(centity_t *cent, const struct weaponInfo_s *weapon );
} func_t;

// Bryar
void FX_BryarProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BryarAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );

// Bow
void FX_BowProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BowAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );

// Howler Sonic shot
void FX_HowlerProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );
void FX_HowlerAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );

// Blood Gun shot
void FX_BloodGunProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BloodGunAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );


// Blaster
void FX_BlasterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BlasterAltFireThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Bowcaster
void FX_BowcasterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Heavy Repeater
void FX_RepeaterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_RepeaterAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// DEMP2
void FX_DEMP2_ProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_DEMP2_AltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// POISON
void FX_Poison_ProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_Poison_AltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );


// Golan Arms Flechette
void FX_FlechetteProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_FlechetteAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Cannon 
void FX_CannonProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_CannonAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );


// Personal Rocket Launcher
void FX_RocketProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_RocketAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Concussion Rifle
void FX_ConcProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Sonic shoot
void FX_SonicProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );


// Emplaced weapon
void FX_EmplacedProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Turret weapon
void FX_TurretProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// ATST Main weapon
void FX_ATSTMainProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// ATST Side weapons
void FX_ATSTSideMainProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_ATSTSideAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

//Tusken projectile
void FX_TuskenShotProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

//Noghri projectile
void FX_NoghriShotProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Table used to attach an extern missile function string to the actual cgame function
func_t	funcs[] = {
	{"bryar_func",			FX_BryarProjectileThink},
	{"bryar_alt_func",		FX_BryarAltProjectileThink},
	{"bow_func",			FX_BowProjectileThink},
	{"bow_alt_func",		FX_BowAltProjectileThink},
	{"howler_func",			FX_HowlerProjectileThink},
	{"howler_alt_func",		FX_HowlerAltProjectileThink},
	{"bloodgun_func",		FX_BloodGunProjectileThink},
	{"bloodgun_alt_func",	FX_BloodGunAltProjectileThink},
	{"blaster_func",		FX_BlasterProjectileThink},
	{"blaster_alt_func",	FX_BlasterAltFireThink},
	{"bowcaster_func",		FX_BowcasterProjectileThink},
	{"repeater_func",		FX_RepeaterProjectileThink},
	{"repeater_alt_func",	FX_RepeaterAltProjectileThink},
	{"demp2_func",			FX_DEMP2_ProjectileThink},
	{"demp2_alt_func",		FX_DEMP2_AltProjectileThink},
    {"poison_func",			FX_Poison_ProjectileThink},
	{"poison_alt_func",		FX_Poison_AltProjectileThink},
	{"flechette_func",		FX_FlechetteProjectileThink},
	{"flechette_alt_func",	FX_FlechetteAltProjectileThink},
	{"cannon_func",			FX_CannonProjectileThink},
	{"cannon_alt_func",		FX_CannonAltProjectileThink},
	{"rocket_func",			FX_RocketProjectileThink},
	{"rocket_alt_func",		FX_RocketAltProjectileThink},
	{"conc_func",			FX_ConcProjectileThink},
	{"sonic_func",			FX_SonicProjectileThink},
	{"emplaced_func",		FX_EmplacedProjectileThink},
	{"turret_func",			FX_TurretProjectileThink},
	{"atstmain_func",		FX_ATSTMainProjectileThink},
	{"atst_side_alt_func",	FX_ATSTSideAltProjectileThink},
	{"atst_side_main_func",	FX_ATSTSideMainProjectileThink},
	{"tusk_shot_func",		FX_TuskenShotProjectileThink},
	{"noghri_shot_func",	FX_NoghriShotProjectileThink},
	{NULL,					NULL}
};
#endif

qboolean playerUsableWeapons[WP_NUM_WEAPONS] =
{
	qtrue,//WP_NONE,
	qtrue, //WP_SABER,			// Spada - Tutte le spade 
	qtrue, // WP_BLASTER_PISTOL,	// Pistole - Key 2
	qtrue, // WP_BLASTER, // Fucili - Key 3
	qtrue, // WP_DISRUPTOR, // Fucili da Cecchino - Key 4
	qtrue, // WP_REPEATER, // Mitragliatore - Key 5
	qtrue, // WP_FLECHETTE, // Carabine - Key 6
	qtrue, //	WP_BOWCASTER, // Armi Pesanti - Key 7
	qtrue, //	WP_ROCKET_LAUNCHER, // Lanciarazzi - Key 8
	qtrue, // 	WP_NOGHRI_STICK, // BioHazard - armi biochimiche, - Key 9
	qtrue, // Armi Esotiche e Speciali - Key 0
	qtrue, // Cannone a mano, artiglieria - Key '
	qtrue, // Granate - Roll ì 
	qtrue, // WP_TRIP_MINE, //Mine - Roll ì
	qtrue, //WP_DET_PACK, // Cariche Esplosive - Roll ì 
	qtrue, // WP_MELEE,			// Corpo a Corpo, arma universale di combattimento fisico. - Key 2 
	qtrue, //WP_BOW,			// Archi e Frecce - Key 3
	qtrue, // WP_CROSSBOW,	// Balestre - Key 4  
	qtrue, // WP_SLING,		// Fionde -  KEY5
	qtrue, // WP_THROWABLE,	// Armi lanciabili: Pugnali, giavellotti, Bolas, Kamas - KEY 6, 
	qtrue, // WP_STUN_BATON,		// Bastoni Stordenti e Armi da Autodifesa - Key 7 
	qtrue, // WP_TUSKEN_STAFF,	// Cerbottane - Key 8 
	qtrue, // Moschetto - Key 9
	qtrue, // Cannone a Mano - KEY0
	qtrue, // WP_HOOKER,			// Rampino, Grapple Hook Key ' 
	qtrue, // WP_SCEPTER,			// Scettro - Key ì
	qtrue, // WP_DARK,		// proiettili di Tenebra - KEY 2
	qtrue, // WP_LIGHT,		// proiettili di Luce - Roll KEY 3 
	qtrue, // WP_LIGHTNING,		// proiettili Folgore Disgregatrice - Roll KEY 3 
	qtrue, // WP_FIRE,	// proiettili di Fuoco - Roll KEY 4 
	qtrue, // WP_FLAME,	// proiettili di Fuoco e Fiamma Aggregatrice - Roll KEY 4 
	qtrue, // WP_AIR,		// proiettili d'Aria - KEY 5 
	qtrue, // WP_WATER,			// proiettili d'Acqua - KEY 6
	qtrue, // WP_EARTH,		// proiettili di Terra - KEY 7 
	qtrue, // WP_SONIC,		// proiettili di Suono - KEY 8 
	qtrue, // WP_PSYCHIC,		// proiettili psichici - KEY 9 
	qtrue, // WP_WOOD,		// proiettili di Legno - Roll Key 0
	qtrue, // WP_METAL,		// proiettili di Metallo,- Roll KEY 0
	qtrue, // WP_SPIRIT, // proiettili di Spirito - Roll Key ' 
	qtrue, // WP_POISON, // Proiettili di Veleno: - Roll Key '
	qtrue, // WP_HOLY,		// proiettili Sacri - KEY ì  
	qtrue, // WP_NECRO,		// proiettili necromantici - KEY ì
	qtrue, // WP_BRYAR_PISTOL,	// Arma Stordente - Key 2 
	qtrue, // WP_EMPLACED_GUN,	// Laser Primari - Key 3 
	qtrue, // WP_ATST_MAIN,		// Dardi e Obici - Key 4 
	qtrue, // WP_ATST_SIDE,		// Torpedo - Key 5 
	qtrue, // WP_TIE_FIGHTER,		// Armi a Raggio - Key 6 
	qtrue, // WP_JAWA,			// Armi a dispersione - Key 7  
	qtrue, // WP_BOT_LASER,		// Armi a Impulso e Bombe al Plasma - Key 8  
	qtrue, // WP_RAPID_FIRE_CONC,	// Cannoni e Hyperlaser - Key 0
	qtrue, // WP_ANTIMATTER,		// Arma ad Antimateria - Key ' 
	qtrue, // WP_TURRET,			// Torretta mitragliatrice lanciaraggi 
	qtrue, // evocazione slot 1 guerriero. KEY 2
	qtrue, // evocazione slot 2 mago. KEY 3
	qtrue,// evocazione slot  3 tiratore. KEY 4
	qtrue,	// evocazione slot 4 speciale. KEY 5
	// CONFIG 6: MORPHING (Mutamorfosi in elementali, animali, etc) 
	// POSSON ESSER FATTE CON I POTERI DELLA FORZA, SORRY
	qtrue,// Forma 1 entità terrestre Key 6
	qtrue,// Forma 2 entità alata Key 7
	qtrue,// Forma 3 entità marina Key 8
	qtrue, // Forma 4 elementale o speciale Key 9
	qtrue, // Dispositivi di Occultamento - key 1 - protezione da luce o raggi
	qtrue, // Occhiali e Visori, sistemi di rivelazione, radar key 2. 
	qtrue, // Protezione del calore e dalla siccità - key 3
	qtrue, // Velocizzare, volare, protezione dal vuoto, tute spaziali. - key 4
	qtrue, // nuotare, protezione dall'annegamento, dall'umidità e dal gelo - key 5
	qtrue, // protezione e difesa fisica, difesa dalla pressione e dagli urti - key 6
	qtrue, // protezione sonica o auricolari, strumento musicale- key 7
	qtrue, // strumento psichico, o protezione psichica - key 8
	qtrue, // antispora, antidoto al veleno delle piante, o fiore di bach o erba.  - key 9
	qtrue, // Utensile metallico di vario tipo. Oggetto a controllo remoto - Key 0
	qtrue, // Difesa magica, oggetto spirituale, Kit medico - Key ì
	qtrue, // Antidoto Chimico o al veleno, stimolanti, droga. 
	
};



//qboolean COM_ParseInt( char **data, int *i );
//qboolean COM_ParseString( char **data, char **s ); 
//qboolean COM_ParseFloat( char **data, float *f );




//qboolean COM_ParseInt( char **data, int *i );
//qboolean COM_ParseString( char **data, char **s ); 
//qboolean COM_ParseFloat( char **data, float *f );

struct wpnParms_s
{
	int	weaponNum;	// Current weapon number
	int	ammoNum;
} wpnParms;

void WPN_Ammo (const char **holdBuf);
void WPN_AmmoIcon (const char **holdBuf);
void WPN_AmmoMax (const char **holdBuf);
void WPN_AmmoLowCnt (const char **holdBuf);
void WPN_AmmoType (const char **holdBuf);
void WPN_EnergyPerShot (const char **holdBuf);
void WPN_FireTime (const char **holdBuf);
void WPN_FiringSnd (const char **holdBuf);
void WPN_AltFiringSnd(const char **holdBuf );
void WPN_StopSnd( const char **holdBuf );
//void WPN_FlashSnd (char **holdBuf);
//void WPN_AltFlashSnd (char **holdBuf);
void WPN_ChargeSnd (const char **holdBuf);
void WPN_AltChargeSnd (const char **holdBuf);
void WPN_SelectSnd (const char **holdBuf);
void WPN_Range (const char **holdBuf);
void WPN_WeaponClass ( const char **holdBuf);
void WPN_WeaponIcon (const char **holdBuf);
void WPN_WeaponModel (const char **holdBuf);
void WPN_WeaponType (const char **holdBuf);
void WPN_AltEnergyPerShot (const char **holdBuf);
void WPN_AltFireTime (const char **holdBuf);
void WPN_AltRange (const char **holdBuf);
void WPN_BarrelCount(const char **holdBuf);
void WPN_MissileName(const char **holdBuf);
void WPN_AltMissileName(const char **holdBuf);
void WPN_MissileSound(const char **holdBuf);
void WPN_AltMissileSound(const char **holdBuf);
void WPN_MissileLight(const char **holdBuf);
void WPN_AltMissileLight(const char **holdBuf);
void WPN_MissileLightColor(const char **holdBuf);
void WPN_AltMissileLightColor(const char **holdBuf);
void WPN_FuncName(const char **holdBuf);
void WPN_AltFuncName(const char **holdBuf);
void WPN_MissileHitSound(const char **holdBuf);
void WPN_AltMissileHitSound(const char **holdBuf);
void WPN_MuzzleEffect(const char **holdBuf);
void WPN_AltMuzzleEffect(const char **holdBuf);

// OPENJK ADD

void WPN_Damage(const char **holdBuf);
void WPN_AltDamage(const char **holdBuf);
void WPN_SplashDamage(const char **holdBuf);
void WPN_SplashRadius(const char **holdBuf);
void WPN_AltSplashDamage(const char **holdBuf);
void WPN_AltSplashRadius(const char **holdBuf);

// Legacy weapons.dat force fields
void WPN_FuncSkip(const char **holdBuf);

typedef struct 
{
	const char	*parmName;
	void	(*func)(const char **holdBuf);
} wpnParms_t;

// This is used as a fallback for each new field, in case they're using base files --eez
const int defaultDamage[] = {
	0,							// WP_NONE
	0,							// WP_SABER				// handled elsewhere
	BRYAR_PISTOL_DAMAGE,		// WP_BLASTER_PISTOL
	BLASTER_DAMAGE,				// WP_BLASTER
	DISRUPTOR_MAIN_DAMAGE,		// WP_DISRUPTOR
	REPEATER_DAMAGE,			// WP_REPEATER
	FLECHETTE_DAMAGE,			// WP_FLECHETTE
	BOWCASTER_DAMAGE,			// WP_BOWCASTER
	ROCKET_DAMAGE,				// WP_ROCKET_LAUNCHER
	0,							// WP_NOGHRI_STICK
	DEMP2_DAMAGE,				// WP_DEMP2
	CONC_DAMAGE,				// WP_CONCUSSION
	TD_DAMAGE,					// WP_THERMAL
	LT_DAMAGE,					// WP_TRIP_MINE
	FLECHETTE_MINE_DAMAGE,		// WP_DET_PACK			// HACK, this is what the code sez.
	0,							// WP_MELEE				// handled by the melee attack function
	0,							// WP_BOW
	0,							// WP_CROSSBOW
	0,							// WP_SLING
	0,							// WP_THROWABLE 
	STUN_BATON_DAMAGE,			// WP_STUN_BATON
	0,							// WP_TUSKEN_STAFF
	0,							// WP_TUSKEN_RIFLE
	CANNON_DAMAGE,				// WP_CANNON
	0,							// WP_HOOKER
	0,							// WP_SCEPTER
	0,							// WP_DARK
	0,							// WP_LIGHT
	0,							// WP_LIGHTNING
	0,							// WP_FIRE
	0,							// WP_FLAME
	0,							// WP_AIR
	0,							// WP_WATER
	0,							// WP_EARTH,
	0,							// WP_SONIC
	0,							// WP_PSYCHIC
	0,							// WP_WOOD, 
	0,							// WP_METAL, 
	POISON_DAMAGE,				// WP_POISON,	
	0,							// WP_SPIRIT
	0,							// WP_POISON
	BLOODGUN_DAMAGE,			// WP_HOLY
	BLOODGUN_DAMAGE,			// WP_NECRO
	BRYAR_PISTOL_DAMAGE,		// WP_BRYAR_PISTOL
	EMPLACED_DAMAGE,			// WP_EMPLACED_GUN
	ATST_MAIN_DAMAGE,			// WP_ATST_MAIN
	ATST_SIDE_MAIN_DAMAGE,		// WP_ATST_SIDE
	EMPLACED_DAMAGE,			// WP_TIE_FIGHTER
	BRYAR_PISTOL_DAMAGE,		// WP_JAWA
	BRYAR_PISTOL_DAMAGE,		// WP_BOT_LASER
	EMPLACED_DAMAGE,			// WP_RAPID_FIRE_CONC,
	0,							// WP_ANTIMATTER,
	0,							// WP_DEFENSE,
	0,							// WP_POSITRON
	0,							// WP_TURRET 
	0,							// SUMMON1
	0,							// SUMMON2
	0,							// SUMMON3
	0,							// SUMMON4
	0,							// MORPH1
	0,							// MORPH2
	0,							// MORPH3
	0,							// MORPH4
	0,							// ITEM1
	0,							// ITEM2
	0,							// ITEM3
	0,							// ITEM4
	0,							// ITEM5
	0,							// ITEM6
	0,							// ITEM7
	0,							// ITEM8
	0,							// ITEM9
	0,							// ITEM10
	0,							// ITEM11
	0,							// ITEM12

};

const int defaultAltDamage[] = {
	0,							// WP_NONE
	0,							// WP_SABER				// handled elsewhere
	BRYAR_PISTOL_DAMAGE,		// WP_BLASTER_PISTOL
	BLASTER_DAMAGE,				// WP_BLASTER
	DISRUPTOR_ALT_DAMAGE,		// WP_DISRUPTOR
	REPEATER_ALT_DAMAGE,			// WP_REPEATER
	FLECHETTE_ALT_DAMAGE,			// WP_FLECHETTE
	BOWCASTER_DAMAGE,			// WP_BOWCASTER
	ROCKET_DAMAGE,				// WP_ROCKET_LAUNCHER
	0,							// WP_NOGHRI_STICK
	DEMP2_ALT_DAMAGE,			// WP_DEMP2
	CONC_ALT_DAMAGE,			// WP_CONCUSSION
	TD_ALT_DAMAGE,				// WP_THERMAL
	LT_DAMAGE,					// WP_TRIP_MINE
	FLECHETTE_MINE_DAMAGE,		// WP_DET_PACK			// HACK, this is what the code sez.
	0,							// WP_MELEE				// handled by the melee attack function
	0,							// WP_BOW
	0,							// WP_CROSSBOW
	0,							// WP_SLING
	0,							// WP_THROWABLE 
	STUN_BATON_ALT_DAMAGE,		// WP_STUN_BATON
	0,							// WP_TUSKEN_STAFF
	0,							// WP_TUSKEN_RIFLE
	CANNON_ALT_DAMAGE,				// WP_CANNON
	0,							// WP_HOOKER
	0,							// WP_SCEPTER
	0,							// WP_DARK
	0,							// WP_LIGHT
	0,							// WP_LIGHTNING
	0,							// WP_FIRE
	0,							// WP_FLAME
	0,							// WP_AIR
	0,							// WP_WATER
	0,							// WP_EARTH,
	0,							// WP_SONIC
	0,							// WP_PSYCHIC
	0,							// WP_WOOD, 
	0,							// WP_METAL, 
	POISON_ALT_DAMAGE,				// WP_POISON,	
	0,							// WP_SPIRIT
	0,							// WP_POISON
	BLOODGUN_DAMAGE,			// WP_HOLY
	BLOODGUN_DAMAGE,			// WP_NECRO
	BRYAR_PISTOL_DAMAGE,		// WP_BRYAR_PISTOL
	EMPLACED_DAMAGE,			// WP_EMPLACED_GUN
	ATST_MAIN_DAMAGE,			// WP_ATST_MAIN
	ATST_SIDE_ALT_DAMAGE,		// WP_ATST_SIDE
	EMPLACED_DAMAGE,			// WP_TIE_FIGHTER
	BRYAR_PISTOL_DAMAGE,		// WP_JAWA
	BRYAR_PISTOL_DAMAGE,		// WP_BOT_LASER
	EMPLACED_DAMAGE,			// WP_RAPID_FIRE_CONC,
	0,							// WP_ANTIMATTER,
	0,							// WP_DEFENSE,
	0,							// WP_POSITRON
	0,							// WP_TURRET 
	0,							// SUMMON1
	0,							// SUMMON2
	0,							// SUMMON3
	0,							// SUMMON4
	0,							// MORPH1
	0,							// MORPH2
	0,							// MORPH3
	0,							// MORPH4
	0,							// ITEM1
	0,							// ITEM2
	0,							// ITEM3
	0,							// ITEM4
	0,							// ITEM5
	0,							// ITEM6
	0,							// ITEM7
	0,							// ITEM8
	0,							// ITEM9
	0,							// ITEM10
	0,							// ITEM11
	0,							// ITEM12
};

const int defaultSplashDamage[] = {
		0,							// WP_NONE
	0,							// WP_SABER				// handled elsewhere
	BRYAR_PISTOL_DAMAGE,		// WP_BLASTER_PISTOL
	BLASTER_DAMAGE,				// WP_BLASTER
	DISRUPTOR_MAIN_DAMAGE,		// WP_DISRUPTOR
	REPEATER_DAMAGE,			// WP_REPEATER
	FLECHETTE_DAMAGE,			// WP_FLECHETTE
	BOWCASTER_SPLASH_DAMAGE,	// WP_BOWCASTER
	ROCKET_SPLASH_DAMAGE,		// WP_ROCKET_LAUNCHER
	0,							// WP_NOGHRI_STICK
	DEMP2_DAMAGE,				// WP_DEMP2
	CONC_SPLASH_DAMAGE,				// WP_CONCUSSION
	TD_SPLASH_DAM,				// WP_THERMAL
	LT_SPLASH_DAM,				// WP_TRIP_MINE
	FLECHETTE_MINE_SPLASH_DAMAGE, // WP_DET_PACK			// HACK, this is what the code sez.
	0,							// WP_MELEE				// handled by the melee attack function
	0,							// WP_BOW
	0,							// WP_CROSSBOW
	0,							// WP_SLING
	0,							// WP_THROWABLE 
	STUN_BATON_DAMAGE,			// WP_STUN_BATON
	0,							// WP_TUSKEN_STAFF
	0,							// WP_TUSKEN_RIFLE
	CANNON_DAMAGE,				// WP_CANNON
	0,							// WP_HOOKER
	0,							// WP_SCEPTER
	0,							// WP_DARK
	0,							// WP_LIGHT
	0,							// WP_LIGHTNING
	0,							// WP_FIRE
	0,							// WP_FLAME
	0,							// WP_AIR
	0,							// WP_WATER
	0,							// WP_EARTH,
	0,							// WP_SONIC
	0,							// WP_PSYCHIC
	0,							// WP_WOOD, 
	0,							// WP_METAL, 
	POISON_DAMAGE,				// WP_POISON,	
	0,							// WP_SPIRIT
	0,							// WP_POISON
	BLOODGUN_DAMAGE,			// WP_HOLY
	BLOODGUN_DAMAGE,			// WP_NECRO
	BRYAR_PISTOL_DAMAGE,		// WP_BRYAR_PISTOL
	EMPLACED_DAMAGE,			// WP_EMPLACED_GUN
	ATST_MAIN_DAMAGE,			// WP_ATST_MAIN
	ATST_SIDE_MAIN_SPLASH_DAMAGE, // WP_ATST_SIDE
	EMPLACED_DAMAGE,			// WP_TIE_FIGHTER
	BRYAR_PISTOL_DAMAGE,		// WP_JAWA
	BRYAR_PISTOL_DAMAGE,		// WP_BOT_LASER
	EMPLACED_DAMAGE,			// WP_RAPID_FIRE_CONC,
	0,							// WP_ANTIMATTER,
	0,							// WP_DEFENSE,
	0,							// WP_POSITRON
	0,							// WP_TURRET 
	0,							// SUMMON1
	0,							// SUMMON2
	0,							// SUMMON3
	0,							// SUMMON4
	0,							// MORPH1
	0,							// MORPH2
	0,							// MORPH3
	0,							// MORPH4
	0,							// ITEM1
	0,							// ITEM2
	0,							// ITEM3
	0,							// ITEM4
	0,							// ITEM5
	0,							// ITEM6
	0,							// ITEM7
	0,							// ITEM8
	0,							// ITEM9
	0,							// ITEM10
	0,							// ITEM11
	0,							// ITEM12
};

const float defaultSplashRadius[] = {
	0.0f,							// WP_NONE
	0.0f,							// WP_SABER
	0.0f,							// WP_BLASTER_PISTOL
	0.0f,							// WP_BLASTER
	0.0f,							// WP_DISRUPTOR
	0.0f,							// WP_REPEATER
	0.0f,							// WP_FLECHETTE
	BOWCASTER_SPLASH_RADIUS,		// WP_BOWCASTER
	ROCKET_SPLASH_RADIUS,			// WP_ROCKET_LAUNCHER
	0.0f,							// WP_NOGHRI_STICK
	0.0f,							// WP_DEMP2
	CONC_SPLASH_RADIUS,				// WP_CONCUSSION
	TD_SPLASH_RAD,					// WP_THERMAL
	LT_SPLASH_RAD,					// WP_TRIP_MINE
	FLECHETTE_MINE_SPLASH_RADIUS,	// WP_DET_PACK		// HACK, this is what the code sez.
	0.0f,							// WP_MELEE				// handled by the melee attack function
	0.0f,							// WP_BOW
	0.0f,							// WP_CROSSBOW
	0.0f,							// WP_SLING
	0.0f,							// WP_THROWABLE 
	0.0f,						// WP_STUN_BATON
	0.0f,							// WP_TUSKEN_STAFF
	0.0f,							// WP_TUSKEN_RIFLE
	0.0f,					// WP_CANNON
	0.0f,							// WP_HOOKER
	0.0f,							// WP_SCEPTER
	0.0f,							// WP_DARK
	0.0f,							// WP_LIGHT
	0.0f,							// WP_LIGHTNING
	0.0f,							// WP_FIRE
	0.0f,							// WP_FLAME
	0.0f,							// WP_AIR
	0.0f,							// WP_WATER
	0.0f,							// WP_EARTH,
	0.0f,							// WP_SONIC
	0.0f,							// WP_PSYCHIC
	0.0f,							// WP_WOOD, 
	0.0f,							// WP_METAL, 
	0.0f,				// WP_POISON,	
	0.0f,							// WP_SPIRIT
	0.0f,							// WP_POISON
	0.0f,			// WP_HOLY
	0.0f,			// WP_NECRO
	0.0f,		// WP_BRYAR_PISTOL
	0.0f,			// WP_EMPLACED_GUN
	0.0f,			// WP_ATST_MAIN
	ATST_SIDE_MAIN_SPLASH_RADIUS,// WP_ATST_SIDE
	0.0f,			// WP_TIE_FIGHTER
	0.0f,			// WP_JAWA
	0.0f,			// WP_BOT_LASER
	0.0f,			// WP_RAPID_FIRE_CONC,
	0.0f,							// WP_ANTIMATTER,
	0.0f,							// WP_DEFENSE,
	0.0f,							// WP_POSITRON
	0.0f,							// WP_TURRET 
	0.0f,							// SUMMON1
	0.0f,							// SUMMON2
	0.0f,							// SUMMON3
	0.0f,							// SUMMON4
	0.0f,							// MORPH1
	0.0f,							// MORPH2
	0.0f,							// MORPH3
	0.0f,							// MORPH4
	0.0f,							// ITEM1
	0.0f,							// ITEM2
	0.0f,							// ITEM3
	0.0f,							// ITEM4
	0.0f,							// ITEM5
	0.0f,							// ITEM6
	0.0f,							// ITEM7
	0.0f,							// ITEM8
	0.0f,							// ITEM9
	0.0f,							// ITEM10
	0.0f,							// ITEM11
	0.0f,							// ITEM12
	
};

const int defaultAltSplashDamage[] = {
	0,								// WP_NONE
	0,								// WP_SABER			// handled elsewhere
	0,								// WP_BLASTER_PISTOL
	0,								// WP_BLASTER
	0,								// WP_DISRUPTOR
	REPEATER_ALT_SPLASH_DAMAGE,			// WP_REPEATER
	FLECHETTE_ALT_SPLASH_DAM,			// WP_FLECHETTE
	BOWCASTER_SPLASH_DAMAGE,			// WP_BOWCASTER
	ROCKET_SPLASH_DAMAGE,				// WP_ROCKET_LAUNCHER
	0,							// WP_NOGHRI_STICK
	DEMP2_ALT_DAMAGE,			// WP_DEMP2
	0,							// WP_CONCUSSION
	TD_ALT_SPLASH_DAM,				// WP_THERMAL
	TD_ALT_SPLASH_DAM,				// WP_TRIP_MINE
	FLECHETTE_MINE_SPLASH_DAMAGE,	// WP_DET_PACK		// HACK, this is what the code sez.
	0,							// WP_MELEE				// handled by the melee attack function
	0,							// WP_BOW
	0,							// WP_CROSSBOW
	0,							// WP_SLING
	0,							// WP_THROWABLE 
	0,							// WP_STUN_BATON
	0,							// WP_TUSKEN_STAFF
	0,							// WP_TUSKEN_RIFLE
	CANNON_DAMAGE,				// WP_CANNON
	0,							// WP_HOOKER
	0,							// WP_SCEPTER
	0,							// WP_DARK
	0,							// WP_LIGHT
	0,							// WP_LIGHTNING
	0,							// WP_FIRE
	0,							// WP_FLAME
	0,							// WP_AIR
	0,							// WP_WATER
	0,							// WP_EARTH,
	0,							// WP_SONIC
	0,							// WP_PSYCHIC
	0,							// WP_WOOD, 
	0,							// WP_METAL, 
	POISON_DAMAGE,				// WP_POISON,	
	0,							// WP_SPIRIT
	0,							// WP_POISON
	BLOODGUN_DAMAGE,			// WP_HOLY
	BLOODGUN_DAMAGE,			// WP_NECRO
	0,							// WP_BRYAR_PISTOL
	0,							// WP_EMPLACED_GUN
	0,							// WP_ATST_MAIN
	ATST_SIDE_ALT_SPLASH_DAMAGE,		// WP_ATST_SIDE
	0,							// WP_TIE_FIGHTER
	0,							// WP_JAWA
	0,							// WP_BOT_LASER
	0,			// WP_RAPID_FIRE_CONC,
	0,							// WP_ANTIMATTER,
	0,							// WP_DEFENSE,
	0,							// WP_POSITRON
	0,							// WP_TURRET 
	0,							// SUMMON1
	0,							// SUMMON2
	0,							// SUMMON3
	0,							// SUMMON4
	0,							// MORPH1
	0,							// MORPH2
	0,							// MORPH3
	0,							// MORPH4
	0,							// ITEM1
	0,							// ITEM2
	0,							// ITEM3
	0,							// ITEM4
	0,							// ITEM5
	0,							// ITEM6
	0,							// ITEM7
	0,							// ITEM8
	0,							// ITEM9
	0,							// ITEM10
	0,							// ITEM11
	0,							// ITEM12

};

const float defaultAltSplashRadius[] = {
	0.0f,							// WP_NONE
	0.0f,							// WP_SABER		// handled elsewhere
	0.0f,							// WP_BLASTER_PISTOL
	0.0f,							// WP_BLASTER
	0.0f,							// WP_DISRUPTOR
	REPEATER_ALT_SPLASH_RADIUS,			// WP_REPEATER
	FLECHETTE_ALT_SPLASH_RAD,			// WP_FLECHETTE
	BOWCASTER_SPLASH_RADIUS,			// WP_BOWCASTER
	ROCKET_SPLASH_RADIUS,				// WP_ROCKET_LAUNCHER
	0.0f,							// WP_NOGHRI_STICK
	DEMP2_ALT_SPLASHRADIUS,		// WP_DEMP2
	CONC_DAMAGE,				// WP_CONCUSSION
	TD_ALT_SPLASH_RAD,			// WP_THERMAL
	LT_SPLASH_RAD,				// WP_TRIP_MINE
	FLECHETTE_ALT_SPLASH_RAD,	// WP_DET_PACK		// HACK, this is what the code sez.
	0.0f,							// WP_MELEE				// handled by the melee attack function
	0.0f,							// WP_BOW
	0.0f,							// WP_CROSSBOW
	0.0f,							// WP_SLING
	0.0f,							// WP_THROWABLE 
	STUN_BATON_DAMAGE,			// WP_STUN_BATON
	0.0f,							// WP_TUSKEN_STAFF
	0.0f,							// WP_TUSKEN_RIFLE
	CANNON_ALT_SPLASH_RAD,		// WP_CANNON
	0.0f,							// WP_HOOKER
	0.0f,							// WP_SCEPTER
	0.0f,							// WP_DARK
	0.0f,							// WP_LIGHT
	0.0f,							// WP_LIGHTNING
	0.0f,							// WP_FIRE
	0.0f,							// WP_FLAME
	0.0f,							// WP_AIR
	0.0f,							// WP_WATER
	0.0f,							// WP_EARTH,
	0.0f,							// WP_SONIC
	0.0f,							// WP_PSYCHIC
	0.0f,							// WP_WOOD, 
	0.0f,							// WP_METAL, 
	POISON_ALT_SPLASHRADIUS,		// WP_POISON,	
	0.0f,							// WP_SPIRIT
	0.0f,							// WP_POISON
	0.0f,							// WP_HOLY
	0.0f,							// WP_NECRO
	0.0f,							// WP_BRYAR_PISTOL
	0.0f,							// WP_EMPLACED_GUN
	0.0f,							// WP_ATST_MAIN
	0.0f,							// WP_ATST_SIDE
	0.0f,							// WP_TIE_FIGHTER
	0.0f,							// WP_JAWA
	0.0f,							// WP_BOT_LASER
	0.0f,							// WP_RAPID_FIRE_CONC,
	0.0f,							// WP_ANTIMATTER,
	0.0f,							// WP_DEFENSE,
	0.0f,							// WP_POSITRON
	0.0f,							// WP_TURRET 
	0.0f,							// SUMMON1
	0.0f,							// SUMMON2
	0.0f,							// SUMMON3
	0.0f,							// SUMMON4
	0.0f,							// MORPH1
	0.0f,							// MORPH2
	0.0f,							// MORPH3
	0.0f,							// MORPH4
	0.0f,							// ITEM1
	0.0f,							// ITEM2
	0.0f,							// ITEM3
	0.0f,							// ITEM4
	0.0f,							// ITEM5
	0.0f,							// ITEM6
	0.0f,							// ITEM7
	0.0f,							// ITEM8
	0.0f,							// ITEM9
	0.0f,							// ITEM10
	0.0f,							// ITEM11
	0.0f,							// ITEM12

};

wpnParms_t WpnParms[] = 
{
	{ "ammo",				WPN_Ammo },	//ammo
	{ "ammoicon",			WPN_AmmoIcon },
	{ "ammomax",			WPN_AmmoMax },
	{ "ammolowcount",		WPN_AmmoLowCnt }, //weapons
	{ "ammotype",			WPN_AmmoType },
	{ "energypershot",	WPN_EnergyPerShot },
	{ "fireTime",			WPN_FireTime },
	{ "firingsound",		WPN_FiringSnd },
	{ "altfiringsound",	WPN_AltFiringSnd },
//	{ "flashsound",		WPN_FlashSnd },
//	{ "altflashsound",	WPN_AltFlashSnd },
	{ "stopsound",		WPN_StopSnd },
	{ "chargesound",		WPN_ChargeSnd },
	{ "altchargesound",	WPN_AltChargeSnd },
	{ "selectsound",		WPN_SelectSnd },
	{ "range",			WPN_Range },
	{ "weaponclass",		WPN_WeaponClass },
	{ "weaponicon",		WPN_WeaponIcon },
	{ "weaponmodel",		WPN_WeaponModel },
	{ "weapontype",		WPN_WeaponType },
	{ "altenergypershot",	WPN_AltEnergyPerShot },
	{ "altfireTime",		WPN_AltFireTime },
	{ "altrange",			WPN_AltRange },
	{ "barrelcount",		WPN_BarrelCount },
	{ "missileModel",		WPN_MissileName },
	{ "altmissileModel", 	WPN_AltMissileName },
	{ "missileSound",		WPN_MissileSound },
	{ "altmissileSound", 	WPN_AltMissileSound },
	{ "missileLight",		WPN_MissileLight },
	{ "altmissileLight", 	WPN_AltMissileLight },
	{ "missileLightColor",WPN_MissileLightColor },
	{ "altmissileLightColor",	WPN_AltMissileLightColor },
	{ "missileFuncName",		WPN_FuncName },
	{ "altmissileFuncName",	WPN_AltFuncName },
	{ "missileHitSound",		WPN_MissileHitSound },
	{ "altmissileHitSound",	WPN_AltMissileHitSound },
	{ "muzzleEffect",			WPN_MuzzleEffect },
	{ "altmuzzleEffect",		WPN_AltMuzzleEffect },
	// OPENJK NEW FIELDS
	{ "damage",			WPN_Damage },
	{ "altdamage",		WPN_AltDamage },
	{ "splashDamage",		WPN_SplashDamage },
	{ "splashRadius",		WPN_SplashRadius },
	{ "altSplashDamage",	WPN_AltSplashDamage },
	{ "altSplashRadius",	WPN_AltSplashRadius },

	// Old legacy files contain these, so we skip them to shut up warnings
	{ "firingforce",		WPN_FuncSkip },
	{ "chargeforce",		WPN_FuncSkip },
	{ "altchargeforce",	WPN_FuncSkip },
	{ "selectforce",		WPN_FuncSkip },
};

const int WPN_PARM_MAX =  sizeof(WpnParms) / sizeof(WpnParms[0]);

void WPN_FuncSkip( const char **holdBuf)
{
	SkipRestOfLine(holdBuf);
}

void WPN_WeaponType( const char **holdBuf)
{
	int weaponNum;
	const char	*tokenStr;

	if (COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	// FIXME : put this in an array (maybe a weaponDataInternal array???)
	if (!Q_stricmp(tokenStr,"WP_NONE"))	
	{	
		weaponNum = WP_NONE;
	}
	else if (!Q_stricmp(tokenStr,"WP_SABER"))	
	{	
		weaponNum = WP_SABER;
	}
	// TIRATORE 
	else if (!Q_stricmp(tokenStr,"WP_BLASTER_PISTOL"))	
	{	
		weaponNum = WP_BLASTER_PISTOL;
	}
	else if (!Q_stricmp(tokenStr,"WP_BLASTER"))	
	{	
		weaponNum = WP_BLASTER;
	}
	else if (!Q_stricmp(tokenStr,"WP_DISRUPTOR"))	
	{	
		weaponNum = WP_DISRUPTOR;
	}
	else if (!Q_stricmp(tokenStr,"WP_REPEATER"))	
	{	
		weaponNum = WP_REPEATER;
	}
	else if (!Q_stricmp(tokenStr,"WP_FLECHETTE"))	
	{	
		weaponNum = WP_FLECHETTE;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOWCASTER"))	
	{	
		weaponNum = WP_BOWCASTER;
	}
	else if (!Q_stricmp(tokenStr,"WP_ROCKET_LAUNCHER"))	
	{	
		weaponNum = WP_ROCKET_LAUNCHER;
	}
	else if (!Q_stricmp(tokenStr,"WP_NOGHRI_STICK"))
	{	
		weaponNum = WP_NOGHRI_STICK;
	}
	else if (!Q_stricmp(tokenStr,"WP_DEMP2"))	
	{	
		weaponNum = WP_DEMP2;
	}
	else if (!Q_stricmp(tokenStr,"WP_CONCUSSION"))	
	{	
		weaponNum = WP_CONCUSSION;
	}
	else if (!Q_stricmp(tokenStr,"WP_THERMAL"))	
	{	
		weaponNum = WP_THERMAL;
	}
	else if (!Q_stricmp(tokenStr,"WP_TRIP_MINE"))	
	{	
		weaponNum = WP_TRIP_MINE;
	}
	else if (!Q_stricmp(tokenStr,"WP_DET_PACK"))	
	{
		weaponNum = WP_DET_PACK;
	}
	// GUERRIERO
	else if (!Q_stricmp(tokenStr,"WP_MELEE"))	
	{
		weaponNum = WP_MELEE;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOW"))
	{
		weaponNum = WP_BOW;
	}
	else if (!Q_stricmp(tokenStr,"WP_CROSSBOW"))
	{
		weaponNum = WP_CROSSBOW;
	}
	else if (!Q_stricmp(tokenStr,"WP_SLING"))
	{
		weaponNum = WP_SLING;
	}
	else if (!Q_stricmp(tokenStr,"WP_THROWABLE"))	
	{	
		weaponNum = WP_THROWABLE;
	}
	else if (!Q_stricmp(tokenStr,"WP_STUN_BATON"))	
	{	
		weaponNum = WP_STUN_BATON;
	}
	else if (!Q_stricmp(tokenStr,"WP_TUSKEN_STAFF"))	
	{
		weaponNum = WP_TUSKEN_STAFF;
	}
	else if (!Q_stricmp(tokenStr,"WP_TUSKEN_RIFLE"))	
	{
		weaponNum = WP_TUSKEN_RIFLE;
	}
	else if (!Q_stricmp(tokenStr,"WP_CANNON"))
	{
		weaponNum = WP_CANNON;
	}
	else if (!Q_stricmp(tokenStr,"WP_HOOKER"))
	{
		weaponNum = WP_HOOKER;
	}
	else if (!Q_stricmp(tokenStr,"WP_SCEPTER"))
	{
		weaponNum = WP_SCEPTER;
	}
	// MAGO 
		else if (!Q_stricmp(tokenStr,"WP_DARK"))
	{
		weaponNum = WP_DARK;
	}
	else if (!Q_stricmp(tokenStr,"WP_LIGHT"))
	{
		weaponNum = WP_LIGHT;
	}
	else if (!Q_stricmp(tokenStr,"WP_LIGHTNING"))
	{
		weaponNum = WP_LIGHTNING;
	}
	else if (!Q_stricmp(tokenStr,"WP_FIRE"))	
	{	
		weaponNum = WP_FIRE;
	}
	else if (!Q_stricmp(tokenStr,"WP_FLAME"))	
	{	
		weaponNum = WP_FLAME;
	}
	else if (!Q_stricmp(tokenStr,"WP_AIR"))	
	{
		weaponNum = WP_AIR;
	}
	else if (!Q_stricmp(tokenStr,"WP_WATER"))	
	{
		weaponNum = WP_WATER;
	}
	else if (!Q_stricmp(tokenStr,"WP_EARTH"))
	{
		weaponNum = WP_EARTH;
	}
	else if (!Q_stricmp(tokenStr,"WP_SONIC"))
	{
		weaponNum = WP_SONIC;
	}
	else if (!Q_stricmp(tokenStr,"WP_PSYCHIC"))
	{
		weaponNum = WP_PSYCHIC;
	}
	else if (!Q_stricmp(tokenStr,"WP_WOOD"))	
	{	
		weaponNum = WP_WOOD;
	}
	else if (!Q_stricmp(tokenStr,"WP_METAL"))	
	{
		weaponNum = WP_METAL;
	}
	else if (!Q_stricmp(tokenStr,"WP_SPIRIT"))	
	{
		weaponNum = WP_SPIRIT;
	}
	else if (!Q_stricmp(tokenStr,"WP_POISON"))
	{
		weaponNum = WP_POISON;
	}
	else if (!Q_stricmp(tokenStr,"WP_HOLY"))
	{
		weaponNum = WP_HOLY;
	}
	else if (!Q_stricmp(tokenStr,"WP_NECRO"))
	{
		weaponNum = WP_NECRO;
	}
	// VEICOLO 
	else if (!Q_stricmp(tokenStr,"WP_BRYAR_PISTOL"))
	{
		weaponNum = WP_BRYAR_PISTOL;
	}
	else if (!Q_stricmp(tokenStr,"WP_EMPLACED_GUN"))
	{
		weaponNum = WP_EMPLACED_GUN;
	}
	else if (!Q_stricmp(tokenStr,"WP_ATST_MAIN"))
	{
		weaponNum = WP_ATST_MAIN;
	}
	else if (!Q_stricmp(tokenStr,"WP_ATST_SIDE"))	
	{	
		weaponNum = WP_ATST_SIDE;
	}
	else if (!Q_stricmp(tokenStr,"WP_TIE_FIGHTER"))	
	{	
		weaponNum = WP_TIE_FIGHTER;
	}
	else if (!Q_stricmp(tokenStr,"WP_JAWA"))	
	{
		weaponNum = WP_JAWA;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOT_LASER"))	
	{
		weaponNum = WP_BOT_LASER;
	}
	else if (!Q_stricmp(tokenStr,"WP_RAPID_FIRE_CONC"))
	{
		weaponNum = WP_RAPID_FIRE_CONC;
	}
	else if (!Q_stricmp(tokenStr,"WP_ANTIMATTER"))
	{
		weaponNum = WP_ANTIMATTER;
	}
	else if (!Q_stricmp(tokenStr,"WP_DEFENSE"))
	{
		weaponNum = WP_DEFENSE;
	}
	else if (!Q_stricmp(tokenStr,"WP_DRONE"))
	{
		weaponNum = WP_DRONE;
	}
	else if (!Q_stricmp(tokenStr,"WP_POSITRON"))
	{
		weaponNum = WP_POSITRON;
	}
	else if (!Q_stricmp(tokenStr,"WP_TURRET"))
	{
		weaponNum = WP_TURRET;
	}
	// INVENTARIO LO PUOI METTERE QUA SOTTO... 
	// EVOCAZIONI 
	else if (!Q_stricmp(tokenStr,"WP_SUMMON1"))
	{
		weaponNum = WP_SUMMON1;
	}
	else if (!Q_stricmp(tokenStr,"WP_SUMMON2"))
	{
		weaponNum = WP_SUMMON2;
	}
	else if (!Q_stricmp(tokenStr,"WP_SUMMON3"))
	{
		weaponNum = WP_SUMMON3;
	}
	else if (!Q_stricmp(tokenStr,"WP_SUMMON4"))
	{
		weaponNum = WP_SUMMON4;
	}
	// METAMORFOSI 
	else if (!Q_stricmp(tokenStr,"WP_MORPH1"))
	{
		weaponNum = WP_MORPH1;
	}
	else if (!Q_stricmp(tokenStr,"WP_MORPH2"))
	{
		weaponNum = WP_MORPH2;
	}
	else if (!Q_stricmp(tokenStr,"WP_MORPH3"))
	{
		weaponNum = WP_MORPH3;
	}
	else if (!Q_stricmp(tokenStr,"WP_MORPH4"))
	{
		weaponNum = WP_MORPH4;
	}
	// INVENTARIO - OGGETTI
	else if (!Q_stricmp(tokenStr,"WP_ITEM1"))
	{
		weaponNum = WP_ITEM1;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM2"))
	{
		weaponNum = WP_ITEM2;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM3"))
	{
		weaponNum = WP_ITEM3;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM4"))
	{
		weaponNum = WP_ITEM4;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM5"))
	{
		weaponNum = WP_ITEM5;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM6"))
	{
		weaponNum = WP_ITEM6;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM7"))
	{
		weaponNum = WP_ITEM7;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM8"))
	{
		weaponNum = WP_ITEM8;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM9"))
	{
		weaponNum = WP_ITEM9;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM10"))
	{
		weaponNum = WP_ITEM10;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM11"))
	{
		weaponNum = WP_ITEM11;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM12"))
	{
		weaponNum = WP_ITEM12;
	}
/*	else if (!Q_stricmp(tokenStr,"WP_FOOD1"))
	{
		weaponNum = WP_FOOD1;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD2"))
	{
		weaponNum = WP_FOOD2;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD3"))
	{
		weaponNum = WP_FOOD3;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD4"))
	{
		weaponNum = WP_FOOD4;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD5"))
	{
		weaponNum = WP_FOOD5;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD6"))
	{
		weaponNum = WP_FOOD6;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD7"))
	{
		weaponNum = WP_FOOD7;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD8"))
	{
		weaponNum = WP_FOOD8;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD9"))
	{
		weaponNum = WP_FOOD9;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD10"))
	{
		weaponNum = WP_FOOD10;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD11"))
	{
		weaponNum = WP_FOOD11;
	}
	else if (!Q_stricmp(tokenStr,"WP_FOOD12"))
	{
		weaponNum = WP_FOOD12;
	}

	else if (!Q_stricmp(tokenStr,"WP_BOOK1"))
	{
		weaponNum = WP_BOOK1;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK2"))
	{
		weaponNum = WP_BOOK2;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK3"))
	{
		weaponNum = WP_BOOK3;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK4"))
	{
		weaponNum = WP_BOOK4;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK5"))
	{
		weaponNum = WP_BOOK5;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK6"))
	{
		weaponNum = WP_BOOK6;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK7"))
	{
		weaponNum = WP_BOOK7;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK8"))
	{
		weaponNum = WP_BOOK8;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK9"))
	{
		weaponNum = WP_BOOK9;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK10"))
	{
		weaponNum = WP_BOOK10;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK11"))
	{
		weaponNum = WP_BOOK11;
	}
	else if (!Q_stricmp(tokenStr,"WP_BOOK12"))
	{
		weaponNum = WP_BOOK12;
	}*/
	else
	{
		weaponNum = 0;
		gi.Printf(S_COLOR_YELLOW"WARNING: bad weapontype in external weapon data '%s'\n", tokenStr);
	}
	wpnParms.weaponNum = weaponNum;
}

//--------------------------------------------
void WPN_WeaponClass(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if (COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 32)
	{
		len = 32;
		gi.Printf(S_COLOR_YELLOW"WARNING: weaponclass too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].classname,tokenStr,len);
}


//--------------------------------------------
void WPN_WeaponModel(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: weaponMdl too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].weaponMdl,tokenStr,len);
}

//--------------------------------------------
void WPN_WeaponIcon(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: weaponIcon too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].weaponIcon,tokenStr,len);
}

//--------------------------------------------
void WPN_AmmoType(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < AMMO_NONE ) || (tokenInt >= AMMO_MAX ))
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad Ammotype in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].ammoIndex = tokenInt;
}

//--------------------------------------------
void WPN_AmmoLowCnt(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 200 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad Ammolowcount in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].ammoLow = tokenInt;
}

//--------------------------------------------
void WPN_FiringSnd(const char **holdBuf)
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: firingSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].firingSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_AltFiringSnd( const char **holdBuf )
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: altFiringSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altFiringSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_StopSnd( const char **holdBuf )
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: stopSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].stopSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_ChargeSnd(const char **holdBuf)
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: chargeSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].chargeSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_AltChargeSnd(const char **holdBuf)
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: altChargeSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altChargeSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_SelectSnd( const char **holdBuf )
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString( holdBuf,&tokenStr )) 
	{
		return;
	}

	len = strlen( tokenStr );
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: selectSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz( weaponData[wpnParms.weaponNum].selectSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_FireTime(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad Firetime in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].fireTime = tokenInt;
}

//--------------------------------------------
void WPN_Range(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad Range in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].range = tokenInt;
}

//--------------------------------------------
void WPN_EnergyPerShot(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad EnergyPerShot in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].energyPerShot = tokenInt;
}

//--------------------------------------------
void WPN_AltFireTime(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad altFireTime in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].altFireTime = tokenInt;
}

//--------------------------------------------
void WPN_AltRange(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad AltRange in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].altRange = tokenInt;
}

//--------------------------------------------
void WPN_AltEnergyPerShot(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad AltEnergyPerShot in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].altEnergyPerShot = tokenInt;
}

//--------------------------------------------
void WPN_Ammo(const char **holdBuf)
{
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	if (!Q_stricmp(tokenStr,"AMMO_NONE"))	
		wpnParms.ammoNum = AMMO_NONE;
	else if (!Q_stricmp(tokenStr,"AMMO_FORCE"))	
		wpnParms.ammoNum = AMMO_FORCE;
	else if (!Q_stricmp(tokenStr,"AMMO_BLASTER"))	
		wpnParms.ammoNum = AMMO_BLASTER;
	else if (!Q_stricmp(tokenStr,"AMMO_POWERCELL"))	
		wpnParms.ammoNum = AMMO_POWERCELL;
	else if (!Q_stricmp(tokenStr,"AMMO_METAL_BOLTS"))	
		wpnParms.ammoNum = AMMO_METAL_BOLTS;
	else if (!Q_stricmp(tokenStr,"AMMO_ROCKETS"))	
		wpnParms.ammoNum = AMMO_ROCKETS;
	else if (!Q_stricmp(tokenStr,"AMMO_EMPLACED"))	
		wpnParms.ammoNum = AMMO_EMPLACED;
	else if (!Q_stricmp(tokenStr,"AMMO_THERMAL"))	
		wpnParms.ammoNum = AMMO_THERMAL;
	else if (!Q_stricmp(tokenStr,"AMMO_TRIPMINE"))	
		wpnParms.ammoNum = AMMO_TRIPMINE;
	else if (!Q_stricmp(tokenStr,"AMMO_DETPACK"))	
		wpnParms.ammoNum = AMMO_DETPACK;
	else
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad ammotype in external weapon data '%s'\n", tokenStr);
		wpnParms.ammoNum = 0;
	}
}

//--------------------------------------------
void WPN_AmmoIcon(const char **holdBuf)
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: ammoicon too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(ammoData[wpnParms.ammoNum].icon,tokenStr,len);

}

//--------------------------------------------
void WPN_AmmoMax(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000 )) 
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad Ammo Max in external weapon data '%d'\n", tokenInt);
		return;
	}
	ammoData[wpnParms.ammoNum].max = tokenInt;
}

//--------------------------------------------
void WPN_BarrelCount(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 4 )) 
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad Range in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].numBarrels = tokenInt;
}


//--------------------------------------------
static void WP_ParseWeaponParms(const char **holdBuf)
{
	const char	*token;
	int		i;


	while (holdBuf)
	{
		token = COM_ParseExt( holdBuf, qtrue );

		if (!Q_stricmp( token, "}" ))	// End of data for this weapon
			break;

		// Loop through possible parameters
		for (i=0;i<WPN_PARM_MAX;++i)
		{
			if (!Q_stricmp(token,WpnParms[i].parmName))	
			{
				WpnParms[i].func(holdBuf);
				break;
			}
		}

		if (i < WPN_PARM_MAX)	// Find parameter???
		{
			continue;
		}
		Com_Printf("^3WARNING: bad parameter in external weapon data '%s'\n", token);
	}
}

//--------------------------------------------
void WPN_MissileName(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: MissileName too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].missileMdl,tokenStr,len);

}

//--------------------------------------------
void WPN_AltMissileName(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: AltMissileName too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].alt_missileMdl,tokenStr,len);

}


//--------------------------------------------
void WPN_MissileHitSound(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: MissileHitSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].missileHitSound,tokenStr,len);
}

//--------------------------------------------
void WPN_AltMissileHitSound(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: AltMissileHitSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altmissileHitSound,tokenStr,len);
}

//--------------------------------------------
void WPN_MissileSound(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: MissileSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].missileSound,tokenStr,len);

}


//--------------------------------------------
void WPN_AltMissileSound(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: AltMissileSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].alt_missileSound,tokenStr,len);

}

//--------------------------------------------
void WPN_MissileLightColor(const char **holdBuf)
{
	int i;
	float	tokenFlt;

	for (i=0;i<3;++i)
	{
		if ( COM_ParseFloat(holdBuf,&tokenFlt)) 
		{
			SkipRestOfLine(holdBuf);
			continue;
		}

		if ((tokenFlt < 0) || (tokenFlt > 1 ))
		{
			gi.Printf(S_COLOR_YELLOW"WARNING: bad missilelightcolor in external weapon data '%f'\n", tokenFlt);
			continue;
		}
		weaponData[wpnParms.weaponNum].missileDlightColor[i] = tokenFlt;
	}

}

//--------------------------------------------
void WPN_AltMissileLightColor(const char **holdBuf)
{
	int i;
	float	tokenFlt;

	for (i=0;i<3;++i)
	{
		if ( COM_ParseFloat(holdBuf,&tokenFlt)) 
		{
			SkipRestOfLine(holdBuf);
			continue;
		}

		if ((tokenFlt < 0) || (tokenFlt > 1 ))
		{
			gi.Printf(S_COLOR_YELLOW"WARNING: bad altmissilelightcolor in external weapon data '%f'\n", tokenFlt);
			continue;
		}
		weaponData[wpnParms.weaponNum].alt_missileDlightColor[i] = tokenFlt;
	}

}


//--------------------------------------------
void WPN_MissileLight(const char **holdBuf)
{
	float	tokenFlt;

	if ( COM_ParseFloat(holdBuf,&tokenFlt)) 
	{
		SkipRestOfLine(holdBuf);
	}

	if ((tokenFlt < 0) || (tokenFlt > 255 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad missilelight in external weapon data '%f'\n", tokenFlt);
	}
	weaponData[wpnParms.weaponNum].missileDlight = tokenFlt;
}

//--------------------------------------------
void WPN_AltMissileLight(const char **holdBuf)
{
	float	tokenFlt;

	if ( COM_ParseFloat(holdBuf,&tokenFlt)) 
	{
		SkipRestOfLine(holdBuf);
	}

	if ((tokenFlt < 0) || (tokenFlt > 255 )) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW"WARNING: bad altmissilelight in external weapon data '%f'\n", tokenFlt);
	}
	weaponData[wpnParms.weaponNum].alt_missileDlight = tokenFlt;
}


//--------------------------------------------
void WPN_FuncName(const char **holdBuf)
{
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}
	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL
	int len = strlen(tokenStr);

	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: FuncName '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	for ( func_t* s=funcs ; s->name ; s++ ) {
		if ( !Q_stricmp(s->name, tokenStr) ) {
			// found it
			weaponData[wpnParms.weaponNum].func = (void*)s->func;
			return;
		}
	}
	gi.Printf(S_COLOR_YELLOW"WARNING: FuncName '%s' in external WEAPONS.DAT does not exist\n", tokenStr);
#endif
}


//--------------------------------------------
void WPN_AltFuncName(const char **holdBuf)
{
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL
	int len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: AltFuncName '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	for ( func_t* s=funcs ; s->name ; s++ ) {
		if ( !Q_stricmp(s->name, tokenStr) ) {
			// found it
			weaponData[wpnParms.weaponNum].altfunc = (void*)s->func;
			return;
		}
	}
	gi.Printf(S_COLOR_YELLOW"WARNING: AltFuncName %s in external WEAPONS.DAT does not exist\n", tokenStr);

#endif
}

//--------------------------------------------
void WPN_MuzzleEffect(const char **holdBuf)
{
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}
	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL

	int len = strlen(tokenStr);

	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: MuzzleEffect '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	G_EffectIndex( tokenStr );
	Q_strncpyz(weaponData[wpnParms.weaponNum].mMuzzleEffect,tokenStr,len);

#endif
}

//--------------------------------------------
void WPN_AltMuzzleEffect(const char **holdBuf)
{
	const char	*tokenStr;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}
	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL

	int len = strlen(tokenStr);

	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: AltMuzzleEffect '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	G_EffectIndex( tokenStr );
	Q_strncpyz(weaponData[wpnParms.weaponNum].mAltMuzzleEffect,tokenStr,len);

#endif
}

//--------------------------------------------

void WPN_Damage(const char **holdBuf)
{
	const char *tokenStr;

	if( COM_ParseString(holdBuf,&tokenStr))
	{
		return;
	}

	weaponData[wpnParms.weaponNum].damage = atoi( tokenStr );
}

//--------------------------------------------

void WPN_AltDamage(const char **holdBuf)
{
	const char *tokenStr;

	if( COM_ParseString(holdBuf,&tokenStr))
	{
		return;
	}

	weaponData[wpnParms.weaponNum].altDamage = atoi( tokenStr );
}

//--------------------------------------------

void WPN_SplashDamage(const char **holdBuf)
{
	const char *tokenStr;

	if( COM_ParseString(holdBuf,&tokenStr))
	{
		return;
	}

	weaponData[wpnParms.weaponNum].splashDamage = atoi( tokenStr );
}

//--------------------------------------------

void WPN_SplashRadius(const char **holdBuf)
{
	const char *tokenStr;

	if( COM_ParseString(holdBuf,&tokenStr))
	{
		return;
	}

	weaponData[wpnParms.weaponNum].splashRadius = (float)atof( tokenStr );
}

//--------------------------------------------

void WPN_AltSplashDamage(const char **holdBuf)
{
	const char *tokenStr;

	if( COM_ParseString(holdBuf,&tokenStr))
	{
		return;
	}

	weaponData[wpnParms.weaponNum].altSplashDamage = atoi( tokenStr );
}

//--------------------------------------------

void WPN_AltSplashRadius(const char **holdBuf)
{
	const char *tokenStr;

	if( COM_ParseString(holdBuf,&tokenStr))
	{
		return;
	}

	weaponData[wpnParms.weaponNum].altSplashRadius = (float)atof( tokenStr );
}

//--------------------------------------------
static void WP_ParseParms(const char *buffer)
{
	const char	*holdBuf;
	const char	*token;

	holdBuf = buffer;
	COM_BeginParseSession();

	while ( holdBuf ) 
	{
		token = COM_ParseExt( &holdBuf, qtrue );

		if ( !Q_stricmp( token, "{" ) ) 
		{
			WP_ParseWeaponParms(&holdBuf);
		}
		 
	}

	COM_EndParseSession(  );

}

//--------------------------------------------
void WP_LoadWeaponParms (void)
{
	char *buffer;
	int len;

	len = gi.FS_ReadFile("ext_data/weapons.dat",(void **) &buffer);

	if (len == -1)
	{
		Com_Error(ERR_FATAL,"Cannot find ext_data/weapons.dat!\n");		
	}

	// initialise the data area
	memset(weaponData, 0, WP_NUM_WEAPONS * sizeof(weaponData_t));	

	// put in the default values, because backwards compatibility is awesome!
	for(int i = 0; i < WP_NUM_WEAPONS; i++)
	{
		weaponData[i].damage = defaultDamage[i];
		weaponData[i].altDamage = defaultAltDamage[i];
		weaponData[i].splashDamage = defaultSplashDamage[i];
		weaponData[i].altSplashDamage = defaultAltSplashDamage[i];
		weaponData[i].splashRadius = defaultSplashRadius[i];
		weaponData[i].altSplashRadius = defaultAltSplashRadius[i];
	}

	WP_ParseParms(buffer);

	gi.FS_FreeFile( buffer );	//let go of the buffer
}





//----------------------------------------------------------------------
////////////// OKAY WE TRY THE LAST CHANCE!!! ///////////////////////////
// GUN LIBRARY LAST TEMPTATIVE! 
// ---------------------------------------------------------------------

// we INIT for moment, the Weapon.data stuff inside the GUN libraries!!! 
// in that way the NPC can choice a file that tecnically should override they
// weapon stuff... not sure if it can work,,.. but we should try 
/*
/*
int WP_SetGunModel( gclient_t *client, class_t npcClass )
// USE FOR CUSTMIZE MODEL OF WEAPONS OF YOUR NPCS?!?!? 
{//FIXME: read from NPCs.cfg
	
}*/
#define MAX_GUN_DATA_SIZE (10 * 1024 * 1024) // Max value original is 0x80000
char	GunParms[MAX_GUN_DATA_SIZE];

void WP_GunFreeStrings( gunData_t &gun )
{
	if (gun.Gun_Name && gi.bIsFromZone(gun.Gun_Name , TAG_G_ALLOC)) {
		gi.Free (gun.Gun_Name);
		gun.Gun_Name=0;
	}
	if (gun.Gun_FullName && gi.bIsFromZone(gun.Gun_FullName , TAG_G_ALLOC)) {
		gi.Free (gun.Gun_FullName);
		gun.Gun_FullName=0;
	}
	if (gun.Gun_Model && gi.bIsFromZone(gun.Gun_Model , TAG_G_ALLOC)) {
		gi.Free (gun.Gun_Model);
		gun.Gun_Model=0;
	}
	if (gun.Gun_Skin && gi.bIsFromZone(gun.Gun_Skin , TAG_G_ALLOC)) {
		gi.Free (gun.Gun_Skin);
		gun.Gun_Skin=0;
	}
}



void WP_GunSetDefaults( gunData_t *gun, qboolean setGuns = qtrue )
{
	//Set defaults so that, if it fails, there's at least something there
	gi.Printf( S_COLOR_YELLOW, "Precaching Basic Gun Weapon\n" );
	// cache name 
	gun->Gun_Type = WP_BLASTER_PISTOL; 
	gun->Gun_Name = NULL;
	gun->Gun_FullName = NULL;
	// cache model 
	gun->Gun_Model = "models/weapons2/gun/gun_w.glm";
	gun->Gun_Skin = NULL;
	gun->Gun_Missile_Model = 0; 
	gun->Gun_Alt_Missile_Model = 0; 
	// cache sounds 
	gun->Gun_Missile_Sound = 0;
	gun->Gun_Alt_Missile_Sound = 0;
	gun->Gun_Charge_Sound = 0; 
	gun->Gun_Alt_Charge_Sound = 0; 
	gun->Gun_Loop_Sound = 0;
	gun->Gun_Select_Sound = NULL;
	// gun->Gun_Type = WP_BLASTER_PISTOL; - the weapon type 
	// Ammo Stuff 
	// Cache Ammo 
	gun->Gun_AmmoMax = 300; 	// Max Ammo for this gun
	gun->Gun_AmmoIndex = 4; // metallic bolts / bullets 
	gun->Gun_AmmoLow = 20;
	gun->Gun_AmmoPerShot = 1; 
	gun->Gun_AltAmmoPerShot = 1;
	// UI - THEY WAS CHARS 
//	gun->Gun_Icon = GUN_NONE;  // name of weapon icon file  
//	gun->Gun_Desc = GUN_NONE; // String description of the gun 
//	gun->Gun_DisplayName = GUN_NONE; // the name which is displayed on HUD. 
//	gun->Gun_Icon = 	"gfx/hud/w_icon_2"; 	// Name of weapon icon file
//	gun->Gun_Desc = "GUN_DESC";		// String Descriprion of Gun
//	gun->Gun_DisplayName = "GUN_NAME"; // The name which is to be displayed on the HUD.
	// BALLISTIC 
	gun->Gun_NumBarrels = 1;		// How many barrels (quante canne) have the gun?
	// Ballistic 
	// Generic 
	gun->Gun_Fire_Time = 500;		// Amount of time between two shoots
	gun->Gun_Alt_Fire_Time = 400;	// Amount of time between alt firings shoot. 
	gun->Gun_NPC_Fire_Time = 500; // Amount of time between two shoots for NPC
	gun->Gun_NPC_Alt_Fire_Time = 400; //Amount of time between two shoots for alt fire NPC
	gun->Gun_Speed = 2500; // speed of main fire 
	gun->Gun_Alt_Speed = 2500; // speed of alt fire 
	gun->Gun_Life = 10000; // life duration of projectile
	gun->Gun_Alt_Life = 10000; // life duration of alt projectile 
	gun->Gun_Range = 8192;			// Range of gun
	gun->Gun_Alt_Range = 8192;		// Range of gun alt fire
	// BRYAR BALISTIC 
	gun->Gun_Charge_Unit = 200.0f;		// gun charge unit 
	gun->Gun_Alt_Charge_Unit = 200.0f; // gun alt charge unit 
	// BLASTER RIFLE
	gun->Gun_Spread = 0.0f;			// Amount of Spread of Gun as blaster rifle - repeater
	gun->Gun_Alt_Spread = 0.0f;	// Amount of Spread of Alt Fire of Gun as blaster rifle - repeater
	// DISRUPTOR 
    // SNIPER RIFLE 
	gun->Gun_ZoomFov = 80.0f;        // Max FOV when zooming
	gun->Gun_ZoomMask = "gfx/2d/cropCircle2";			// Zoom Mask of Gun
	gun->Gun_ZoomRotateMask = "gfx/2d/cropCircle";	// Second Zoom Mask of Gun
	gun->Gun_ZoomCharge = "gfx/2d/crop_charge"; // shader for zoom charge 
	gun->Gun_ZoomTick = "gfx/2d/insertTick"; // shader of ammo ticks 
	gun->Gun_ZoomSoundStart =  "sound/weapons/disruptor/zoomstart.wav";	// Zoom Sound Start of Weapon 
	gun->Gun_ZoomSoundEnd =  "sound/weapons/disruptor/zoomend.wav";  // Zoom End Sound of Weapon 
	gun->Gun_Crosshair = "gfx/2d/crosshaira.tga";			// Set Gun Crosshair 
	// REPEATER 
	gun->Gun_Gravity = 0.0f;		// Amount of gravity of projectiles 
	gun->Gun_Alt_Gravity = 0.0f;	// Amount of gravity of alt projectile 
	gun->Gun_AntiGravity = 0.0f;	 // Amount of Antigravity of Projectile 
	gun->Gun_Alt_AntiGravity = 0.0f; // Amount of Antigravity of Alt Projectile 
	// FLECHETTE 
	gun->Gun_Num_Shot = 1;		// number of projectiles shooted into the moment
	gun->Gun_Alt_Num_Shot = 1;	// number of projectile shooted into the same moment
	gun->Gun_Num_Bounce = 0;		// Amount of Bouncing of projectile 
	gun->Gun_Alt_Num_Bounce = 0;	// Amount of bouncing of alt projectile 
 	gun->Gun_Time_BombShot = 0;		// how many time before projectile explode , like flechette mines. 
	gun->Gun_Alt_Time_BombShot = 0;  // how many time before projectile alt fire explode , like flechette mines
	// STUN BATON 
	gun->Gun_ElectrocuteDuration = 0;	// How many time duration of electrocution  
	gun->Gun_Alt_ElectrocuteDuration = 0;// How Many Time duration of electrocution for alt fire
	gun->Gun_StunTime = 0;			// How many Time Enemy is stunned by the gun  
	gun->Gun_Alt_StunTime = 0;		// How many Time enemy is stunned by alt fire gun
	// THERMAL 
	gun->Gun_Time_Grenade_Time = 0;		//  how many time take the grenade before explode 
	gun->Gun_Alt_Time_Grenade_Time = 0;	// how many time take the grenade before explode  
	// Poison Default setting 
	gun->Gun_PoisonDamage = 0;	// amount of damage set with poison
	gun->Gun_Alt_PoisonDamage = 0; // amount of damage set with alt poison
	gun->Gun_PoisonTime = 0;			// amount of time duration of poison
	gun->Gun_Alt_PoisonTime = 0;		// amount of time duration of alt poison
	gun->Gun_Poison_Effect = NULL;	// main fire poison effect on enemy 
	gun->Gun_Alt_Poison_Effect = NULL; // alt fire poison effect on enemy 
	gun->Gun_PoisonAnim = -1;	// animation played for poison main attack 
	gun->Gun_Alt_PoisonAnim = -1; // animation played for poison alt attack 
//#ifndef _USRDLL
	// Visual effects 
	gun->Gun_Muzzle_Effect = 0;  // Main Shoot Muzzle Effect 
	gun->Gun_Alt_Muzzle_Effect = 0; // Alt Shoot Muzzle Effect
	gun->Gun_Bullet_Effect = 0; // Main Shoot Muzzle Effect 
	gun->Gun_Alt_Bullet_Effect = 0; // Alt Shoot Muzzle Effect
	gun->Gun_Ray_Effect = RAY_NONE;	// Shader of main gun ray
	gun->Gun_Alt_Ray_Effect = RAY_NONE;	// Shader of alt gun ray
	gun->Gun_WallImpact_Effect = 0; // Main Shoot Muzzle Effect 
	gun->Gun_Alt_WallImpact_Effect = 0; // Alt Shoot Muzzle Effect
	gun->Gun_FleshImpact_Effect = 0; // Main Shoot Muzzle Effect 
	gun->Gun_Alt_FleshImpact_Effect = 0; // Alt Shoot Muzzle Effect
	gun->Gun_Bounce_Effect = 0; // Main Shoot Muzzle Effect 
	gun->Gun_Alt_Bounce_Effect = 0; // Alt Shoot Muzzle Effect
	gun->Gun_Charge_Shader_Effect = CHARGE_NONE; // effect draw on barrel when weapon charge the shoot
	gun->Gun_Alt_Charge_Shader_Effect = CHARGE_NONE; // effect draw on barrel wehn weapon charge alt shoot. 
	gun->Gun_Disgregate_Effect = 0; // effect played on enemy when is disgregated
	gun->Gun_Alt_Disgregate_Effect = 0; // effect played on enemy when is disgregaded by alt shoot
	gun->Gun_Explosion_Effect = 0; 
// #endif

	// Damages 
	gun->Gun_Damage = 30; // damage of main bullet
	gun->Gun_AltDamage = 45; // damage of alt bullet 
	gun->Gun_SplashDamage = 0; // damage main aoe 
	gun->Gun_AltSplashDamage = 0; // damage alt aoe 
	gun->Gun_SplashRadius = 0.0f; // aoe main radius 
	gun->Gun_AltSplashRadius = 0.0f; // aoe splash radius 
	gun->Gun_SplashKnockback = 0.0f;	// aoe knockback 
	gun->Gun_AltSplashKnockback = 0.0f; // aoe alt knockback 
	gun->Gun_NPC_Damage_Easy = 10;
	gun->Gun_NPC_Damage_Medium = 15;
	gun->Gun_NPC_Damage_Hard = 20; 
	gun->Gun_Alt_NPC_Damage_Easy = 10;
	gun->Gun_Alt_NPC_Damage_Medium = 15;
	gun->Gun_Alt_NPC_Damage_Hard = 20; 
	// MODS 
	gun->Gun_MOD = MOD_UNKNOWN;  // Set MOD for main Attack 
	gun->Gun_MOD2 = MOD_UNKNOWN; // Set second MOD for main Attack 
	gun->Gun_MOD3 = MOD_UNKNOWN;// Set third MOD for main Attack 
	gun->Gun_Alt_MOD = MOD_UNKNOWN; // Set Means of Damage for Alt Attack 
	gun->Gun_Alt_MOD2 = MOD_UNKNOWN; // Set second Means of Damage for Alt Attack 
	gun->Gun_Alt_MOD2 = MOD_UNKNOWN; // Set third Means of Damage for Alt Attack 
		
	//done in game (server-side code)
	gun->GunMoveSpeedScale = 1.0f;				//1.0 - you move faster/slower when using this saber
	gun->GunAnimSpeedScale = 1.0f;				//1.0 - plays normal attack animations faster/slower
	//Animation 
	gun->Gun_Charger_Anim = -1; // Animation when weapon is reloaded. 
	gun->Gun_Alt_Charger_Anim = -1; // Animation for alt charging shoot
	gun->Gun_ShootAnim = -1;			// shoot animation for main attack
	gun->Gun_Alt_ShootAnim = -1;		// shoot animation for alt attack
	gun->Gun_ReadyAnim = -1;			// ready animation for main attack
	gun->Gun_WalkAnim = -1;			// shoot animation for main attack
	gun->Gun_RunAnim = -1;			// run animation for main attack
	gun->Gun_DropAnim = -1; // animation for drop or put away gun
	gun->Gun_Hit_Animation = -1; // animation played from enemy hitted. 
	gun->Gun_Alt_Hit_Animation = -1; // animation played from enemy hitted by alt shoots. 
	// OTHER STUFF 
	gun->Gun_MagicRestriction = 0;		//Magic and Skils that cannot be used while this Gun is on (bitfield) 

	// FLAGS 
	gun->gunFlags = 0;	//see all the GFL_ flags
	gun->gunFlags2 = 0;	//see all the GFL2_ flags
	gun->gunFlags3 = 0;	//see all the GFL3_ flags
	
//=========================================================================================================================================
}

extern qboolean G_ParseLiteral( const char **data, const char *string ); 
extern stringID_table_t WPTable[];
extern stringID_table_t FPTable[];
extern stringID_table_t MODTable[];
extern stringID_table_t Gun_Table[];
extern stringID_table_t RayGunTable[];
extern stringID_table_t ChargeGunTable[];
qboolean WP_GunParseParms( const char *GunName, gunData_t *gun/*, qboolean setGuns*/ ) 
{
	const char	*token;
	const char	*value;
	const char	*p;
	float	f;
	int		n;
	int		setGuns = 1;
	if ( !gun ) 
	{
		return qfalse;;
	}

	//Set defaults so that, if it fails, there's at least something there
	WP_GunSetDefaults( gun, setGuns ); //<- IF I USE IT, NPC LOST WEAPON STUFF AFTER 2 LOADGAMES!!

	if ( !GunName || !GunName[0] ) 
	{
		return qfalse;;
	}

	gun->Gun_Name = G_NewString( GunName );
	//try to parse it out
	p = GunParms;
	COM_BeginParseSession();

	// look for the right gun
	while ( p )
	{
		token = COM_ParseExt( &p, qtrue );
		if ( token[0] == 0 )
		{
			COM_EndParseSession(  );
			return qfalse;;
		}

		if ( !Q_stricmp( token, GunName ) ) 
		{
			break;
		}

		SkipBracedSection( &p );
	}
	if ( !p ) 
	{
		COM_EndParseSession(  );
		return qfalse;
	}

	if ( G_ParseLiteral( &p, "{" ) ) 
	{
		COM_EndParseSession(  );
		return qfalse;
	}
		
	// parse the saber info block
	while ( 1 ) 
	{
		token = COM_ParseExt( &p, qtrue );
		if ( !token[0] ) 
		{
			gi.Printf( S_COLOR_RED"ERROR: unexpected EOF while parsing '%s'\n", GunName );
			COM_EndParseSession(  );
			return qfalse;
		}

		if ( !Q_stricmp( token, "}" ) ) 
		{
			break;
		}

#ifdef _WIN32
#pragma region(Gun Parms)
#endif
	
		//Gun Full Name 
		//the "Proper Name" of the gun, shown in the UI
		if ( !Q_stricmp( token, "fullname" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_FullName = G_NewString( value );
			continue;
		}

	
	// Gun model
	if ( !Q_stricmp( token, "GunModel" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Model = G_NewString( value );
			continue;
		}
		
		// Gun custom skin
		if ( !Q_stricmp( token, "GunSkin" ) )
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Skin = G_NewString( value );
			continue;
		}
	    
		// 3D model of shooted missile from main fire  
		if ( !Q_stricmp( token, "missilemodel" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Missile_Model = G_NewString( value );
			continue;
		}

		// Sounds done when weapon shoot alt firing 
		if ( !Q_stricmp( token, "altmissilemodel" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Missile_Model = G_NewString( value );
			continue;
		}

		// ------------------------- SOUNDS -----------------------------
		// Sound Setting 
		// Loop Sound of weapon 
		if ( !Q_stricmp( token, "LoopSound" ) )
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Loop_Sound = G_NewString( value );
			continue;
		}

		// Main Missile Loop Sound of weapon 
		if ( !Q_stricmp( token, "MissileSound" ) )
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Missile_Sound = G_NewString( value );
			continue;
		}

		// Alt Missile Loop Sound of weapon 
		if ( !Q_stricmp( token, "AltMissileSound" ) )
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Missile_Sound = G_NewString( value );
			continue;
		}

		// sound to start when the weapon initiates the charging sequence
		if ( !Q_stricmp( token, "chargesound" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Charge_Sound = G_NewString( value );
			continue;
		}

		// sound to start when the weapon initiates the alt charging sequence
		if ( !Q_stricmp( token, "altchargesound" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Charge_Sound = G_NewString( value );
			continue;
		}

		
		// Sound Played when you select the weapon
		if ( !Q_stricmp( token, "selectsound" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Select_Sound = G_NewString( value );
			continue;
		}

		/*if ( !Q_stricmp( token, "selectSound" ) ) 
		{
			{
				const char	*tokenStr;
				int		len;

				if ( COM_ParseString( holdBuf,&tokenStr )) 
				{
						return;
				}

				len = strlen( tokenStr );
				len++;

				if (len > 64)
			{
				len = 64;
				gi.Printf(S_COLOR_YELLOW"WARNING: selectSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
				}

				Q_strncpyz( weaponData[wpnParms.weaponNum].selectSnd,tokenStr,len);
}*/


		

		// ----------------------------------
		// GUN TYPE
		// weapon Type: assign the weapon to a weaponslot. 
		if ( !Q_stricmp( token, "GunType" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
//			int GunType = GetIDForString( WPTable, value );
	/*		if ( GunType >= WP_NONE && GunType <= WP_NUM_WEAPONS )
			{
				gun->Gun_Type = (weapon_t)GunType;
			}*/
			continue;
		}

		// -------------------------------------
		// AMMO 
		// Ammo Stuff
		// Max Ammo for this gun 
		if ( !Q_stricmp( token, "ammomax" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AmmoMax = n;
			continue;
		}

		// Index of proper Ammo Slot used 
		if ( !Q_stricmp( token, "AmmoIndex" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AmmoIndex = n;
			continue;
		}

		if ( !Q_stricmp( token, "AmmoLow" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AmmoLow = n;
			continue;
		}

		// how many ammo consume a shot 
		if ( !Q_stricmp( token, "energypershot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AmmoPerShot = n;
			continue;
		}
		
		// how many ammo consume a shot 
		if ( !Q_stricmp( token, "altenergypershot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AltAmmoPerShot = n;
			continue;
		}
		/*
	// -------------------------------------------
	// ICON SHADERS 		
	// Icon for Gun 
	if ( !Q_stricmp( token, "icon" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int ICON = GetIDForString( Gun_Table, value );
			if ( ICON >= PISTOL_NONE && ICON <= PISTOL_MAX )
			{
				gun->Gun_Icon = (Gun_t)ICON;
			}
			continue;
		}
		
	// Description for Gun 
	if ( !Q_stricmp( token, "description" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int DESCRIPTION = GetIDForString( Gun_Table, value );
			if ( DESCRIPTION >= GUN_NONE && DESCRIPTION <= GUN_MAX )
			{
				gun->Gun_Desc = (Gun_t)DESCRIPTION;
			}
			continue;
		}*/
	
	// HUD gun name 
	/*if ( !Q_stricmp( token, "displayname" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int DISPLAYNAME = GetIDForString( Gun_Table, value );
			if ( DISPLAYNAME >= GUN_NONE && DISPLAYNAME <= GUN_MAX )
			{
				gun->Gun_DisplayName = (Gun_t)DISPLAYNAME;
			}
			continue;
		}*/

		// Gun NUM Barrel 
		if ( !Q_stricmp( token, "numbarrels" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_NumBarrels = n;
			continue;
		}

	// ----------------------------- BALLISTIC GENERIC BALLISTIC ---------
	// Amounts of the time between two shots 
	if ( !Q_stricmp( token, "firetime" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Fire_Time = n;
			continue;
		}

	// Amount of time between alt firings shoot. 
	if ( !Q_stricmp( token, "altfiretime" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Fire_Time = n;
			continue;
		}

	// Amount of time between shots for NPC -SET_SHOOT_SPACING 
	if ( !Q_stricmp( token, "NPCfiretime" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_NPC_Fire_Time = n;
			continue;
		}
	// Amount of time between alt shots for NPC - SET SHOOT SPACING
	if ( !Q_stricmp( token, "AltNPCfiretime" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_NPC_Alt_Fire_Time = n;
			continue;
		}


	// Projectile Velocity
	if ( !Q_stricmp( token, "Speed" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_Speed = f;
			continue;
		}
	// Alt Projectile Velocity
	if ( !Q_stricmp( token, "AltSpeed" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Speed = f;
			continue;
		}

	// Range of Gun 
	if ( !Q_stricmp( token, "range" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Range = n;
			continue;
		}
	// Range of gun alt shoot
	if ( !Q_stricmp( token, "altrange" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Range = n;
			continue;
		}

	// Life of Projectile 
	if ( !Q_stricmp( token, "missilelife" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Life = n;
			continue;
		}

	// Life Alt Projectile 
	if ( !Q_stricmp( token, "altmissilelife" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Life = n;
			continue;
		}

	
	// BRYAR BALISTIC 
	// If is 1, shoot is charged like bryar alt , blaster pistol alt. 
	if ( !Q_stricmp( token, "chargeshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_CHARGESHOOT;
			}
			continue;
		}
	
	// If is 1, Alt Shoot is Charged like bryar alt , blaster pistol alt 
	if ( !Q_stricmp( token, "altchargeshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_CHARGESHOOT;
			}
			continue;
		}
	

	// gun charge unit 
	if ( !Q_stricmp( token, "ChargeUnit" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Charge_Unit = f;
			continue;
		}
	
	 // gun alt charge unit 
	if ( !Q_stricmp( token, "AltChargeUnit" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Charge_Unit = f;
			continue;
		}

	//-----------------------------------------------------------------
	// BLASTER RIFLE
	// if is 1, weapon shoot spreading
	if ( !Q_stricmp( token, "spreadshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_SPREADSHOOT;
			}
			continue;
		}
	
	// if is 1, weapon alt shoot spreading
	if ( !Q_stricmp( token, "altspreadshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_SPREADSHOOT;
			}
			continue;
		}

	// Amount of Spread of Gun as blaster rifle - repeater
	if ( !Q_stricmp( token, "spread" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Spread = f;
			continue;
		}
	
	// Amount of Spread of Alt Fire of Gun as blaster rifle - repeater
	if ( !Q_stricmp( token, "altspread" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Spread = f;
			continue;
		}

	// DISRUPTOR - MAIN SHOOT RAY 
	// if is on 1, shoot a ray as disruptor instead of a projectile	
	if ( !Q_stricmp( token, "rayshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_RAYSHOOT;
			}
			continue;
		}

	// ray for alt fire
	if ( !Q_stricmp( token, "altrayshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_RAYSHOOT;
			}
			continue;
		}
	// DISRUPTOR ALT SHOOT RAY 
	if ( !Q_stricmp( token, "rayshot2" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_RAYSHOOT2;
			}
			continue;
		}

	// ray for alt fire
	if ( !Q_stricmp( token, "altrayshot2" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_RAYSHOOT2;
			}
			continue;
		}

	// CONCUSSION ALT SHOOT RAY 
	if ( !Q_stricmp( token, "rayshot3" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_RAYSHOOT3;
			}
			continue;
		}


	// ray for alt fire
	if ( !Q_stricmp( token, "altrayshot3" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_RAYSHOOT3;
			}
			continue;
		}

	  // SNIPER RIFLE 
	// if is on 1, weapon can zoom 	
	if ( !Q_stricmp( token, "zoom" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ZOOM;
			}
			continue;
		}

	// Max Zoom Fov 
	if ( !Q_stricmp( token, "zoomfov" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_ZoomFov = f;
			continue;
		}

		// Zoom Mask of Gun
		if ( !Q_stricmp( token, "ZoomMask" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_ZoomMask = G_NewString( value );
			continue;
		}
		
		// Zoom Alt Mask of Gun
		if ( !Q_stricmp( token, "ZoomMask2" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_ZoomRotateMask = G_NewString( value );
			continue;
		}

		// Zoom Mask Charge Shader
		if ( !Q_stricmp( token, "ZoomCharge" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_ZoomCharge = G_NewString( value );
			continue;
		}
		
		// Zoom Mask Tick Shader
		if ( !Q_stricmp( token, "ZoomTick" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_ZoomTick = G_NewString( value );
			continue;
		}

		// Start Zoom Sound
		if ( !Q_stricmp( token, "ZoomSoundStart" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_ZoomSoundStart = G_NewString( value );
			continue;
		}

		// End Zoom Sound
		if ( !Q_stricmp( token, "ZoomSoundEnd" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_ZoomSoundEnd = G_NewString( value );
			continue;
		}

	
	// Set Crosshair 
		if ( !Q_stricmp( token, "Crosshair" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Crosshair = G_NewString( value );
			continue;
		}

		// REPEATER 
		// if is on 1, main fire shoot the explosive gravity bolt like alt repeater
		if ( !Q_stricmp( token, "gravityshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_GRAVITYSHOOT;
			}
			continue;
		}

		// if is on 1, alt fire shoot the explosive gravity bolt like alt repeater
		if ( !Q_stricmp( token, "altgravityshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_GRAVITYSHOOT;
			}
			continue;
		}
	//  PROJECTILES VECTORS 
	// Amount of Gravity of Projectiles 
	if ( !Q_stricmp( token, "gravity" ) ) 
	{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Gravity = f;
			continue;
	}

	// Alt Amount of Gravity of Projectiles 
	if ( !Q_stricmp( token, "altgravity" ) ) 
	{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Gravity = f;
			continue;
	}

	// Amount of Antigravity of Project
	if ( !Q_stricmp( token, "antigravity" ) ) 
	{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_AntiGravity = f;
			continue;
	}
		
	// Amount of Alt Antigravity of Project
	if ( !Q_stricmp( token, "altantigravity" ) ) 
	{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				continue;
			}
			gun->Gun_Alt_AntiGravity = f;
			continue;
	}

	// FLECHETTE ---------------------------------------
	// if is on 1, main fire shoot multiple shoots
	if ( !Q_stricmp( token, "multipleshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_MULTIPLESHOOT;
			}
			continue;
		}
	
	// if is on 1, alt fire shoot multiple shoots
	if ( !Q_stricmp( token, "altmultipleshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_MULTIPLESHOOT;
			}
			continue;
		}
	// Number of projectiles shooted into the moment. 
	if ( !Q_stricmp( token, "numshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Num_Shot = n;
			continue;
		}

	// Alt Number of projectiles shooted into the same moment. 
	if ( !Q_stricmp( token, "altnumshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Num_Shot = n;
			continue;
		}

	// Flechette Shrapnel Shot enable  
	if ( !Q_stricmp( token, "shrapnelshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_SHRAPNELSHOOT;
			}
			continue;
		}
	// Flechette Alt Shrapnel
	if ( !Q_stricmp( token, "altshrapnelshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_SHRAPNELSHOOT;
			}
			continue;
		}

	// if is on 1, shoot a time explosive projectile 
	if ( !Q_stricmp( token, "timebombshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_TIMEBOMBSHOOT;
			}
			continue;
		}

		// if is on 1, alt fire shoot a time explosive projectiles 
		if ( !Q_stricmp( token, "alttimebombshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_TIMEBOMBSHOOT;
			}
			continue;
		}

		// BOWCASTER 
		// if is on 1, shoot bouncing projectiles 
		if ( !Q_stricmp( token, "bounceshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_BOUNCESHOOT;
			}
			continue;
		}
		
		// if is on 1, shoot bouncing projectiles 
		if ( !Q_stricmp( token, "altbounceshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_BOUNCESHOOT;
			}
			continue;
		}

		// Bounce Half Shoot  
		if ( !Q_stricmp( token, "bouncehalfshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_BOUNCEHALFSHOOT;
			}
			continue;
		}
		
		// alt bounce half shoot enabled 
		if ( !Q_stricmp( token, "altbouncehalfshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_BOUNCEHALFSHOOT;
			}
			continue;
		}

	// Number of Boucing of proiectile . 
	if ( !Q_stricmp( token, "numbounce" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Num_Bounce = n;
			continue;
		}

	// Amount of bouncing of alt projectile 
	if ( !Q_stricmp( token, "altnumbounce" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Num_Bounce = n;
			continue;
	}

	// if is on 1, shoot divide projectiles as bowcaster 
	if ( !Q_stricmp( token, "divideshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_DIVIDESHOOT;
			}
			continue;
		}
		
	//  if is on 1, shoot divide projectiles as bowcaster 
	if ( !Q_stricmp( token, "altdivideshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_DIVIDESHOOT;
			}
			continue;
		}

		 
	

	// if is 1, gun shoot create a deflagration like demp2 alt fire 
	if ( !Q_stricmp( token, "aoeshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_AOESHOOT;
			}
			continue;
		}

	// demp2 alt fire deflagration for secondary fire
	if ( !Q_stricmp( token, "altaoeshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_AOESHOOT;
			}
			continue;
		}
		
	// How many Time Enemy is stunned by the gun  
		if ( !Q_stricmp( token, "stuntime" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_StunTime = n;
			continue;
		}
		
		// How many Time enemy is stunned by alt fire gun
		if ( !Q_stricmp( token, "altstuntime" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_StunTime = n;
			continue;
		}

		// ROCKET -----------------------------------------------------------
		// if is 1, gun shoot aim missiles as rocket launcher
		if ( !Q_stricmp( token, "aimshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_AIMSHOOT;
			}
			continue;
		}
		

		// if is 1, gun show aim alt missile as rocket launcher 
		if ( !Q_stricmp( token, "altaimshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_AIMSHOOT;
			}
			continue;
		}
		
	// NOGHRI----------------------------------------
	// if is 1, gun shoot gas projectile as noghri stick
	if ( !Q_stricmp( token, "gasshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_GASSHOOT;
			}
			continue;
		}

	// if is 1, gun shoot gas alt projectile as noghri stick. 
	if ( !Q_stricmp( token, "altgasshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_ALT_GASSHOOT;
			}
			continue;
		}






		// UnShield 
		// if is 1, gun destroy shields  
		if ( !Q_stricmp( token, "unshield" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags |= GFL_UNSHIELD;
			}
			continue;
		}


		// GUN FLAG 2 
		// STUN BATON ---------------------------------------------
		// if is 1, gun shoot electric projecitles as demp2 
		if ( !Q_stricmp( token, "electrocute" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ELECTROCUTE;
			}
			continue;
		}

		// if is 1, gun shoot electric projectiles as demp2 
		if ( !Q_stricmp( token, "altelectrocute" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_ELECTROCUTE;
			}
			continue;
		}

	// how many time during electrocution 
	if ( !Q_stricmp( token, "ElectrocuteTime" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_ElectrocuteDuration = n;
			continue;
	}

	// how many time during electrocution for alt fire 
	if ( !Q_stricmp( token, "AltElectrocuteTime" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_ElectrocuteDuration = n;
			continue;
	}

		// DISGREGATION--------------------------------
		// if is 1, enemy killed are disgregated 
		if ( !Q_stricmp( token, "disgregate" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_DISGREGATE;
			}
			continue;
		}
		// if is 1, gun shoot electric projectiles as demp2 
		if ( !Q_stricmp( token, "altdisgregate" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_DISGREGATE;
			}
			continue;
		}
		// MELEE -----------------------------------------------------
		// punchs as main fire 
		if ( !Q_stricmp( token, "melee" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_MELEE;
			}
			continue;
		}

		// punchs as alt fire 
		if ( !Q_stricmp( token, "altmelee" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_MELEE;
			}
			continue;
		}

		// kick as main fire 
		if ( !Q_stricmp( token, "melee2" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_MELEE2;
			}
			continue;
		}

		// kicks as alt fire 
		if ( !Q_stricmp( token, "altmelee2" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_MELEE2;
			}
			continue;
		}


		//(TUSKEN STAFF, TUSKEN RIFLE ALT) 
		// if is 1, Melee attack with a weapon is enabled 
		if ( !Q_stricmp( token, "staffmelee" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_STAFFMELEE;
			}
			continue;
		}

		// if is 1, Melee alt attack with a weapon is enabled 
		if ( !Q_stricmp( token, "altstaffmelee" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_STAFFMELEE;
			}
			continue;
		}

		 // if is 1, shoot like stun baton   

	if ( !Q_stricmp( token, "batonshot" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_BATON;
			}
			continue;
		}

	// if is 1, shoot alt fire like stun baton 
	if ( !Q_stricmp( token, "altbatonshot" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_BATON;
			}
	}
		// EXPLOSIVES ------------------------------------------------
    	
	// if is 1, shoot an explosive grenade like thermal alt
	if ( !Q_stricmp( token, "grenade" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_GRENADE;
			}
			continue;
		}

	// if is 1, shoot an explosive grenade like thermal alt
	if ( !Q_stricmp( token, "altgrenade" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_GRENADE;
			}
	}


	// if is 1, shoot a time grenade as thermal main fire 
	if ( !Q_stricmp( token, "timegrenade" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_TIMEGRENADE;
			}
	}

	// if is 1, alt fire shoot a time grenade like thermal det. 
	if ( !Q_stricmp( token, "alttimegrenade" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_TIMEGRENADE;
			}
	}
	
	// if is 1 shoot a laser mine
	if ( !Q_stricmp( token, "lasermine" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_LASERMINE;
			}
	}

	// if is 1 shoot an alt laser mine
	if ( !Q_stricmp( token, "altlasermine" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_LASERMINE;
			}
	}

	// if is 1 Shoot a Proxy Mine
	if ( !Q_stricmp( token, "proxymine" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_PROXYMINE;
			}
	}


	// if is 1 , shoot a alt proxy mine
	if ( !Q_stricmp( token, "altproxymine" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_PROXYMINE;
			}
	}

 	// if is 1, place a bomb that can be activated on remote. 
	if ( !Q_stricmp( token, "bomb" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_BOMB;
			}
	}

	// if is 1, decloak hitted enemy 
	if ( !Q_stricmp( token, "decloak" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_DECLOAK;
			}
	}

	// if is 1, alt decloak an hitted enemy  
	if ( !Q_stricmp( token, "altdecloak" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_DECLOAK;
			}
	}

	// if is 1, saber can't parry the shoot  
	if ( !Q_stricmp( token, "noparry" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_NOPARRY;
			}
	}

	// if is 1, saber can't parry the alt shoot   
	if ( !Q_stricmp( token, "altnoparry" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_NOPARRY;
			}
	}
	
	// if is 1, saber can't deflect the shot  
	if ( !Q_stricmp( token, "nodeflect" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_NODEFLECT;
			}
	}

	// if is 1, alt saber can't deflect the shot   
	if ( !Q_stricmp( token, "altnodeflect" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_NODEFLECT;
			}
	}

	// if is 1, projectile can be pushed away   
	if ( !Q_stricmp( token, "pushable" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_PUSHABLE;
			}
	}

	// if is 1, alt projectile can be pushed away   
	if ( !Q_stricmp( token, "altpushable" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_PUSHABLE;
			}
	}

	// if is 1, enemy killed is dismembered    
	if ( !Q_stricmp( token, "dismember" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_DISMEMBER;
			}
	}

	// if is 1, alt fire dismember enemy killed    
	if ( !Q_stricmp( token, "altdismember" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_ALT_DISMEMBER;
			}
	}

	if ( !Q_stricmp( token, "twoguns" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags3 |= GFL_TWOGUNS;
			}
	}

// ---------------------- MODs -----------------------------------

	
	// Main Means of Damage 
	if ( !Q_stricmp( token, "MOD" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int MOD = GetIDForString( MODTable, value );
			if ( MOD >= 0 && MOD <= MOD_MAX ) // was just < 
			{
				gun->Gun_MOD = MOD;
			}
			continue;
		}

	// Second Means of Damage 
	if ( !Q_stricmp( token, "MOD2" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int MOD2 = GetIDForString( MODTable, value );
			if ( MOD2 >= 0 && MOD2 <= MOD_MAX ) // was just < 
			{
				gun->Gun_MOD2 = MOD2;
			}
			continue;
		}

	// Third Means of Damage 
	if ( !Q_stricmp( token, "MOD3" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int MOD3 = GetIDForString( MODTable, value );
			if ( MOD3 >= 0 && MOD3 <= MOD_MAX ) // was just < 
			{
				gun->Gun_MOD3 = MOD3;
			}
			continue;
		}

	

	// Alt Means of Damage 
	if ( !Q_stricmp( token, "AltMOD" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int ALTMOD = GetIDForString( MODTable, value );
			if ( ALTMOD >= 0 && ALTMOD <= MOD_MAX ) // was just < 
			{
				gun->Gun_Alt_MOD = ALTMOD;
			}
			continue;
		}

	// Second Alt Means of Damage 
	if ( !Q_stricmp( token, "AltMOD2" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int ALTMOD2 = GetIDForString( MODTable, value );
			if ( ALTMOD2 >= 0 && ALTMOD2 <= MOD_MAX ) // was just < 
			{
				gun->Gun_Alt_MOD2 = ALTMOD2;
			}
			continue;
		}

	// Third Alt Means of Damage 
	if ( !Q_stricmp( token, "AltMOD3" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int ALTMOD3 = GetIDForString( MODTable, value );
			if ( ALTMOD3 >= 0 && ALTMOD3 <= MOD_MAX ) // was just < 
			{
				gun->Gun_Alt_MOD3 = ALTMOD3;
			}
			continue;
		}
	

// ------------------------ POISON --------------------------------------
	
	// Primary Poison Hit Animation    
	if ( !Q_stricmp( token, "PoisonAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_PoisonAnim = anim;
			}
			continue;
		}
	
	// Alt Poison Hit Animation    
	if ( !Q_stricmp( token, "AltPoisonAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_Alt_PoisonAnim = anim;
			}
			continue;
		}
	// Poison Effect 	
	if ( !Q_stricmp( token, "PoisonEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Poison_Effect = G_NewString( value );
			continue;
		}

	// Alt Poison Effect  
	if ( !Q_stricmp( token, "AltPoisonEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Poison_Effect = G_NewString( value );
			continue;
		}

	// Main Fire Amount of Damage inflicted by poison
	if ( !Q_stricmp( token, "PoisonDamage" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_PoisonDamage = n;
			continue;
	}

	// Alt Fire Amount of Damage inflicted by poison
	if ( !Q_stricmp( token, "AltPoisonDamage" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_PoisonDamage = n;
			continue;
	}

	// Amount of time of poison duration 
	if ( !Q_stricmp( token, "PoisonTime" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_PoisonTime = n;
			continue;
	}

	// Amount of time of Alt Poison duration 
	if ( !Q_stricmp( token, "AltPoisonTime" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				continue;
			}
			gun->Gun_Alt_PoisonTime = n;
			continue;
	}

#ifndef _USRDLL
	// ----------------------------------------
	// VISUAL EFFECTS 
	// ------------------------------------------


	// Visual effects 
	// -- THEY ARE VERY NECESSARY?!?! 
	// -- 
	// Main Shoot 
	if ( !Q_stricmp( token, "muzzleEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Muzzle_Effect = G_EffectIndex( (char *)value );
			continue;
		}
	// Alt Muzzle Effect 
	if ( !Q_stricmp( token, "altmuzzleEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Muzzle_Effect = G_EffectIndex( (char *)value );
			continue;
		}
	
	// Shot Effect
	if ( !Q_stricmp( token, "ShotEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Bullet_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Alt Shot Effect 
	if ( !Q_stricmp( token, "AltShotEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Bullet_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Shader of main gun ray 
		if ( !Q_stricmp( token, "RayEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int MAINRAY = GetIDForString( RayGunTable, value );
			if ( MAINRAY >= RAY_NONE && MAINRAY <= RAY_MAX ) // was just < 
			{
				gun->Gun_Ray_Effect = (RayGun_Colors_t)MAINRAY;
			}
			continue;
		}
	
		// Shader of main gun ray 
		if ( !Q_stricmp( token, "AltRayEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int ALTRAY = GetIDForString( RayGunTable, value );
			if ( ALTRAY >= RAY_NONE && ALTRAY <= RAY_MAX ) // was just < 
			{
				gun->Gun_Alt_Ray_Effect = (RayGun_Colors_t)ALTRAY;
			}
			continue;
		}
	
	// Wall Impact Effect
	if ( !Q_stricmp( token, "WallImpactEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_WallImpact_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Alt Wall Impact Effect
	if ( !Q_stricmp( token, "AltWallImpactEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_WallImpact_Effect = G_EffectIndex( (char *)value );
			continue;
		}

		
	// Flesh Impact Effect
	if ( !Q_stricmp( token, "FleshImpactEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_FleshImpact_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Alt Flesh Impact Effect
	if ( !Q_stricmp( token, "AltFleshImpactEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_FleshImpact_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Bounce  Effect
	if ( !Q_stricmp( token, "BounceEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Bounce_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Alt Bounce  Effect
	if ( !Q_stricmp( token, "AltBounceEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Bounce_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Disgregate  Effect
	if ( !Q_stricmp( token, "DisgregateEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Disgregate_Effect = G_EffectIndex( (char *)value );
			continue;
		}

	// Alt Disgregate  Effect
	if ( !Q_stricmp( token, "AltDisgregateEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Alt_Disgregate_Effect = G_EffectIndex( (char *)value );
			continue;
		}


	// Charge Shader
		if ( !Q_stricmp( token, "ChargeEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int CHARGE = GetIDForString( ChargeGunTable, value );
			if ( CHARGE >= CHARGE_NONE && CHARGE <= CHARGE_MAX ) // was just < 
			{
				gun->Gun_Charge_Shader_Effect = (ChargeShader_t)CHARGE;
			}
			continue;
		}

	// Alt Charge Shader
		if ( !Q_stricmp( token, "AltChargeEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int ALTCHARGE = GetIDForString( ChargeGunTable, value );
			if ( ALTCHARGE >= CHARGE_NONE && ALTCHARGE <= CHARGE_MAX ) // was just < 
			{
				gun->Gun_Alt_Charge_Shader_Effect = (ChargeShader_t)ALTCHARGE;
			}
			continue;
		}

	// Explosion  Effect of grenades and explosive
	if ( !Q_stricmp( token, "ExplosionEffect" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			gun->Gun_Explosion_Effect = G_NewString( value );
			continue;
		}
#endif

// --------------------------------------------------------------
// Damages 
// --------------------------------------------------------------
		
		// DAMAGE !! 
		// main shoot damage 
		if ( !Q_stricmp( token, "damage" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_Damage = n;
			continue;
		}

		// alt shoot damage 
		if ( !Q_stricmp( token, "altdamage" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AltDamage = n;
			continue;
		}

		// main shoot splash damage 
		if ( !Q_stricmp( token, "splashdamage" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_SplashDamage = n;
			continue;
		}

		// alt shoot splash damage 
		if ( !Q_stricmp( token, "altsplashdamage" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AltSplashDamage = n;
			continue;
		}

	//splashRadius - aoe main radius 
		if ( !Q_stricmp( token, "splashRadius" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_SplashRadius = f;
			continue;
		}
	
		if ( !Q_stricmp( token, "altsplashRadius" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AltSplashRadius = f;
			continue;
		}

		// aoe knockback 
		if ( !Q_stricmp( token, "SplashKnockback" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_SplashKnockback = f;
			continue;
		}

		//Aoe Alt Knockback 
		if ( !Q_stricmp( token, "AltSplashKnockback" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_AltSplashKnockback = f;
			continue;
		}

		// NPC damage easy: damage deal from NPC on easy difficult
		if ( !Q_stricmp( token, "NPCdamageeasy" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_NPC_Damage_Easy = n;
			continue;


		}
		// NPC damage on medium difficulty
		if ( !Q_stricmp( token, "NPCdamagemedium" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_NPC_Damage_Medium = n;
			continue;
		}
		// NPC damage on hard difficulty
		if ( !Q_stricmp( token, "NPCdamagehard" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_NPC_Damage_Hard = n;
			continue;


		}
		// NPC alt fire damage on easy
		if ( !Q_stricmp( token, "altNPCdamageeasy" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_Alt_NPC_Damage_Easy = n;
			continue;


		}
		// NPC alt fire damage on medium
		if ( !Q_stricmp( token, "altNPCdamagemedium" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_Alt_NPC_Damage_Medium = n;
			continue;
		}
		// NPC alt fire damage on hard
		if ( !Q_stricmp( token, "altNPCdamagehard" ) ) 
		{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->Gun_Alt_NPC_Damage_Hard = n;
			continue;
		}
	
//-----------------------------------------------------------------------		
// MOVEMENT SPEED VARIABLES 
//-----------------------------------------------------------------------

		// set speed of movement 
		if ( !Q_stricmp( token, "moveSpeedScale" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->GunMoveSpeedScale = f;
			continue;
		}
		// set speed of animations 
		if ( !Q_stricmp( token, "AnimSpeedScale" ) ) 
		{
			if ( COM_ParseFloat( &p, &f ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			gun->GunAnimSpeedScale = f;
			continue;
		}
// ---------------------
// ANIMATION 



	// Animation when charge the attack.
	if ( !Q_stricmp( token, "ChargeAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_Charger_Anim = anim;
			}
			continue;
		}

	// Animations when you charge the alt attack 
	if ( !Q_stricmp( token, "AltChargeAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_Charger_Anim = anim;
			}
			continue;
		}
	
	// Animation of main shoot attack 
	if ( !Q_stricmp( token, "ShotAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_ShootAnim = anim;
			}
			continue;
		}
	
	
 	// Alt Shoot animation for alt attack
	if ( !Q_stricmp( token, "AltShotAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_Alt_ShootAnim = anim;
			}
			continue;
		}


	//readyAnim - ready animation for attack
	if ( !Q_stricmp( token, "readyAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_ReadyAnim = anim;
			}
			continue;
		}
	
	//Animation when walk with gun
	if ( !Q_stricmp( token, "WalkAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_WalkAnim = anim;
			}
			continue;
		}


	//Animation when Run with gun
	if ( !Q_stricmp( token, "RunAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_RunAnim = anim;
			}
			continue;
		}
	
	// animation for drop or put away gun
	if ( !Q_stricmp( token, "DropAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_DropAnim = anim;
			}
			continue;
		}


	// Animation played by entity hit by the gun main shot  
	if ( !Q_stricmp( token, "HitAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_Hit_Animation = anim;
			}
			continue;
		}
	
	
 	// Animation played by entity hit by the gun alt shot 
	if ( !Q_stricmp( token, "AltHitAnim" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
			int anim = GetIDForString( animTable, value );
			if ( anim >= 0 && anim < MAX_ANIMATIONS )
			{
				gun->Gun_Alt_Hit_Animation = anim;
			}
			continue;
		}

	// -----------------OTHER STUFF -------------------------------
	 
	// Special Water combat feature 
	// if is one, you can use the pistol also uderwater
	if ( !Q_stricmp( token, "nounderwater" ) ) 
	{
			if ( COM_ParseInt( &p, &n ) ) 
			{
				SkipRestOfLine( &p );
				continue;
			}
			if ( n )
			{
				gun->gunFlags2 |= GFL_NO_UNDERWATER;
			}
	}

	// Mana Restriction 
	//Magic and Skils that cannot be used while this Gun is on (bitfield) 
	if ( !Q_stricmp( token, "manaRestrict" ) ) 
		{
			if ( COM_ParseString( &p, &value ) ) 
			{
				continue;
			}
/*			int fp = GetIDForString( FPTable, value );
			if ( fp >= FP_FIRST && fp < NUM_FORCE_POWERS )
			{
				gun->Gun_MagicRestriction |= (1<<fp);
			}*/
			continue;
		}
		gi.Printf( "WARNING: unknown keyword '%s' while parsing '%s'\n", token, GunName );
		SkipRestOfLine( &p );
	}

	//FIXME: precache the saberModel(s)?
#ifdef _WIN32
#pragma endregion
#endif
	COM_EndParseSession(  );
	return qtrue; 
}

void WP_RemoveGun( gentity_t *ent, int GunNum )
{
	gi.Printf( S_COLOR_GREEN "I am Removing Current Gun!" , ent );
	if ( !ent || !ent->client )
	{
		return;
	}
	//reset everything for this gun just in case
	WP_GunSetDefaults( &ent->client->gs.gun[GunNum] );
	gi.Printf( S_COLOR_GREEN "I am Setting the Default Gun Parameter! Executing gunsetdefaults!" , ent );
	if ( ent->weaponModel[GunNum] > 0 )
	{
		gi.G2API_SetSkin( &ent->ghoul2[ent->weaponModel[GunNum]], -1, 0 );
		gi.G2API_RemoveGhoul2Model( ent->ghoul2, ent->weaponModel[GunNum] );
		ent->weaponModel[GunNum] = -1;
	}
}

int WP_SetGunModel( gclient_t *client, class_t npcClass )
{//FIXME: read from NPCs.cfg
	if ( client )
	{
		/*switch ( npcClass )
		{
		case CLASS_DESANN://Desann
			client->ps.saber[0].model = "models/weapons2/saber_desann/saber_w.glm";
			break;
		case CLASS_LUKE://Luke
			client->ps.saber[0].model = "models/weapons2/saber_luke/saber_w.glm";
			break;
		case CLASS_PLAYER://Kyle NPC and player
		case CLASS_KYLE://Kyle NPC and player
			client->ps.saber[0].model = "models/weapons2/saber/saber_w.glm";
			break;
		default://reborn and tavion and everyone else
			client->ps.saber[0].model = "models/weapons2/saber_reborn/saber_w.glm";
			break;
		}*/
		return ( G_ModelIndex( client->gs.gun[0].Gun_Model ) );
	}
	else
	{
		/*switch ( npcClass )
		{
		case CLASS_DESANN://Desann
			return ( G_ModelIndex( "models/weapons2/saber_desann/saber_w.glm" ) );
			break;
		case CLASS_LUKE://Luke
			return ( G_ModelIndex( "models/weapons2/saber_luke/saber_w.glm" ) );
			break;
		case CLASS_PLAYER://Kyle NPC and player
		case CLASS_KYLE://Kyle NPC and player
			return ( G_ModelIndex( "models/weapons2/saber/saber_w.glm" ) );
			break;
		default://reborn and tavion and everyone else
			return ( G_ModelIndex( "models/weapons2/saber_reborn/saber_w.glm" ) );
			break;
		}*/

		return ( G_ModelIndex( "models/weapons2/gun/gun_w.glm" ) );
	}
}

// for not get conflict with attachedweaponmodel 



void G_RemoveGunModels( gentity_t *ent )
{
	
	if ( ent->ghoul2.size() )
	{
		if ( ent->gunModel[0] > 0 )
		{
			gi.G2API_RemoveGhoul2Model( ent->ghoul2, ent->gunModel[0] );
			ent->gunModel[0] = -1;
		}
		if ( ent->gunModel[1] > 0 )
		{
			gi.G2API_RemoveGhoul2Model( ent->ghoul2, ent->gunModel[1] );
			ent->gunModel[1] = -1;
		}
	}
}

void G_CreateG2AttachedGunModel( gentity_t *ent, const char *gsGunModel, int boltNum, int GunNum )
{
	if (!gsGunModel)
	{
		assert (gsGunModel);
		return;
	}
	if ( ent->playerModel == -1 )
	{
		return;
	}
	if ( boltNum == -1 )
	{
		return;
	}

	if ( ent && ent->client && ent->client->NPC_class == CLASS_GALAKMECH )
	{//hack for galakmech, no gun models
		//ent->gunModel[0] = ent->gunModel[1] = -1;
		return;
	}
/*	if ( GunNum < 0 || GunNum >= MAX_INHAND_GUNS )
	{
		return;
	}*/
	char GunModel[64];

	strcpy (GunModel, gsGunModel);	
	if (char *spot = strstr(GunModel, ".md3") ) {
		*spot = 0;
		spot = strstr(GunModel, "_w");//i'm using the in view weapon array instead of scanning the item list, so put the _w back on
		if (!spot&&!strstr(GunModel, "noweap")) 
		{
			strcat (GunModel, "_w");
		}
		strcat (GunModel, ".glm");	//and change to ghoul2
	}

	// give us a gun model
	int GunModelIndex = G_ModelIndex( GunModel );
	if ( GunModelIndex )
	{
		//ent->gunModel[GunNum] = gi.G2API_InitGhoul2Model(ent->ghoul2, GunModel, GunModelIndex, NULL_HANDLE, NULL_HANDLE, 0, 0 );
		/*if ( ent->gunModel[GunNum] != -1 )
		{
			// attach it to the hand
/*			gi.G2API_AttachG2Model(&ent->ghoul2[ent->gunModel[GunNum]], &ent->ghoul2[ent->playerModel], 
						boltNum, ent->playerModel);
			// set up a bolt on the end so we can get where the sabre muzzle is - we can assume this is always bolt 0
			gi.G2API_AddBolt(&ent->ghoul2[ent->gunModel[GunNum]], "*flash");*/
	  		//gi.G2API_SetLodBias( &ent->ghoul2[ent->weaponModel[weaponNum]], 0 );
	//	}
	}
}

void WP_GunAddG2GunModels( gentity_t *ent, int SpecificGunNum )
{
	gi.Printf( S_COLOR_YELLOW "I am Adding Gun Model!\n" , ent );
	int GunNum = 0, MaxGun = 1;
	if ( SpecificGunNum != -1 && SpecificGunNum <= MaxGun )
	{
		GunNum = MaxGun = SpecificGunNum;
	}
	for ( ; GunNum <= MaxGun; GunNum++ )
	{
		if ( ent->weaponModel[GunNum] > 0 )
		{//we already have a weapon model in this slot
			//remove it
			gi.G2API_SetSkin( &ent->ghoul2[ent->weaponModel[GunNum]], -1, 0 );
			gi.G2API_RemoveGhoul2Model( ent->ghoul2, ent->weaponModel[GunNum] );
			ent->weaponModel[GunNum] = -1;
		}
		if ( GunNum > 0 )
		{//second gun
			if ( !ent->client->gs.dualGuns 
				//|| G_IsRidingVehicle( ent ) 
				)
			{//only have one saber or riding a vehicle and can only use one saber
				return;
			}
		}
		else if ( GunNum == 0 )
		{//first saber
			if ( ent->client->gs.gunInFlight )
			{//it's still out there somewhere, don't add it
				//FIXME: call it back?
				continue;
			}
		}
		int handBolt = ((GunNum == 0) ? ent->handRBolt : ent->handLBolt);
		if ( ent->client->gs.gun->Gun_Model || ent->client->gs.gun[GunNum].Gun_Model )
		{	
			gi.Printf( S_COLOR_RED "DO YOU SEE THE WEAPON MODEL????\n" , ent );
			G_CreateG2AttachedGunModel( ent, ent->client->gs.gun[GunNum].Gun_Model, handBolt, GunNum );
		}
		if ( ent->client->gs.gun[GunNum].Gun_Skin != NULL )
		{//if this saber has a customSkin, use it
			// lets see if it's out there
			int GunSkin = gi.RE_RegisterSkin( ent->client->gs.gun[GunNum].Gun_Skin );
			if ( GunSkin )
			{
				// put it in the config strings
				// and set the ghoul2 model to use it
				gi.G2API_SetSkin( &ent->ghoul2[ent->weaponModel[GunNum]], G_SkinIndex( ent->client->gs.gun[GunNum].Gun_Skin ), GunSkin );
			}
		}
	}
}

void WP_SetGunEntModelSkin( gentity_t *ent, gentity_t *gunent )
{

	
	int	GunModel = 0;
	qboolean	newGunModel = qfalse;
	//FIXME: get saberModel from NPCs.cfg
	if ( !ent->client->gs.gun[0].Gun_Model )
	{
		GunModel = WP_SetGunModel( ent->client, ent->client->NPC_class );
	}
	else
	{
		//got saberModel from NPCs.cfg
		GunModel = G_ModelIndex( ent->client->gs.gun[0].Gun_Model );
	}
	if ( GunModel && gunent->s.modelindex != GunModel )
	{
		if ( gunent->playerModel >= 0 )
		{//remove the old one, if there is one
			gi.G2API_RemoveGhoul2Model( gunent->ghoul2, gunent->playerModel );
		}
		//add the new one
		gunent->playerModel = gi.G2API_InitGhoul2Model( gunent->ghoul2, ent->client->gs.gun[0].Gun_Model, GunModel, NULL_HANDLE, NULL_HANDLE, 0, 0);
		gunent->s.modelindex = GunModel; 
		newGunModel = qtrue;
	}
	//set skin, too
	if ( ent->client->gs.gun[0].Gun_Skin == NULL )
	{
		gi.G2API_SetSkin( &gunent->ghoul2[0], -1, 0 );
	}
	else
	{//if this gun has a customSkin, use it
		// lets see if it's out there
		int GunSkin = gi.RE_RegisterSkin( ent->client->gs.gun[0].Gun_Skin );
		if ( GunSkin && (newGunModel || gunent->s.modelindex2 != GunSkin) )
		{
			// put it in the config strings
			// and set the ghoul2 model to use it
			gi.G2API_SetSkin( &gunent->ghoul2[0], G_SkinIndex( ent->client->gs.gun[0].Gun_Skin ), GunSkin );
			gunent->s.modelindex2 = GunSkin;
		}
	}
}


int WP_GunInitData( gentity_t *ent )
{
	gi.Printf( S_COLOR_GREEN "I am Initing Gun Data!" , ent );
	if ( !ent->client )
	{
		return 0;
	}
	if ( 1 )
	{
		VectorClear( ent->client->renderInfo.muzzlePoint );
		VectorClear( ent->client->renderInfo.muzzlePointOld );
		VectorClear( ent->client->renderInfo.muzzlePointNext );
		VectorClear( ent->client->renderInfo.muzzleDir );
		VectorClear( ent->client->renderInfo.muzzleDirOld );
		//VectorClear( ent->client->renderInfo.muzzleDirNext );
		for ( int GunNum = 0; GunNum < MAX_GUN; GunNum++ )
		{
	
		}
		if ( ent->client->gs.gunEntityNum <= 0 || ent->client->gs.gunEntityNum >= ENTITYNUM_WORLD )
		{//FIXME: if you do have a gun already, be sure to re-set the model if it's changed (say, via a script).
			gentity_t *gunent = G_Spawn();
			ent->client->gs.gunEntityNum = gunent->s.number;
			gunent->classname = "gun";
			
			gunent->s.eType = ET_GENERAL;
			gunent->svFlags = SVF_USE_CURRENT_ORIGIN;
			gunent->s.weapon = WP_BLASTER_PISTOL;
			// add all weapon slots... 
			gunent->s.weapon = WP_BLASTER;
			gunent->owner = ent;
			gunent->s.otherEntityNum = ent->s.number;
			//clear the enemy
			gunent->enemy = NULL;

			gunent->clipmask = MASK_SOLID | CONTENTS_ITEM; // or content solid?
			gunent->contents = CONTENTS_ITEM;//|CONTENTS_SHOTCLIP;

			VectorSet( gunent->mins, -3.0f, -3.0f, -3.0f );
			VectorSet( gunent->maxs, 3.0f, 3.0f, 3.0f );
			gunent->mass = 10;//necc?

			gunent->s.eFlags |= EF_NODRAW;
			gunent->svFlags |= SVF_NOCLIENT;
/*
Ghoul2 Insert Start
*/
			gunent->playerModel = -1;
			WP_SetGunEntModelSkin( ent, gunent );

			// set up a bolt on the end so we can get where the sabre muzzle is - we can assume this is always bolt 0
			gi.G2API_AddBolt( &gunent->ghoul2[0], "*flash" );
			ent->client->gs.gunInFlight = qfalse;
			ent->client->gs.gunEntityDist = 0;
			ent->client->gs.gunEntityState = GUNENTITYSTATE_LEAVING;
			// animazioni, codide ca completar in seguito. 
			//ent->client->gs.gunMove = ent->client->gs.gunMoveNext = LS_NONE;
			//FIXME: need a think function to create alerts when turned on or is on, etc.
		}
		else
		{//already have one, might just be changing sabers, register the model and skin and use them if different from what we're using now.
			WP_SetGunEntModelSkin( ent, &g_entities[ent->client->gs.gunEntityNum] );
		}
	}
	else
	{
		ent->client->gs.gunEntityNum = ENTITYNUM_NONE;
		ent->client->gs.gunInFlight = qfalse;
		ent->client->gs.gunEntityDist = 0;
		ent->client->gs.gunEntityState = GUNENTITYSTATE_LEAVING;
	}

	// Doppie armi, codice da completar in seguito.
	/*if ( ent->client->gs.dualGuns )
	{
		return 2;
	}*/
	
	return 1;
}

void WP_GunUpdateOldData( gentity_t *ent )
{
	if ( ent->client )
	{
		VectorCopy( ent->client->renderInfo.muzzlePoint, ent->client->renderInfo.muzzlePointOld );
		VectorCopy( ent->client->renderInfo.muzzleDir, ent->client->renderInfo.muzzleDirOld );
	}
}


void WP_SetGun( gentity_t *ent, int GunNum, const char *GunName )
{

	gi.Printf( S_COLOR_GREEN "I am Setting the Gun!" , ent );
	if ( !ent || !ent->client )
	{
		return;
	}
	if ( Q_stricmp( "none", GunName ) == 0 || Q_stricmp( "remove", GunName ) == 0 )
	{
		WP_RemoveGun( ent, GunNum );
		return;
	}
	if ( ent->weaponModel[GunNum] > 0 )
	{
		gi.G2API_RemoveGhoul2Model(ent->ghoul2, ent->weaponModel[GunNum]);
		ent->weaponModel[GunNum] = -1;
	}
	gi.Printf( S_COLOR_RED "I am PARSING GUN COMMANDS FOR GUN LIBRARY!" , ent );
	WP_GunParseParms( GunName, &ent->client->gs.gun[GunNum] );//get gun info
	
	/*if ( GunNum == 1 && (ent->client->gs.gun[1].gunFlags3&GFL_TWOGUNS) )
	{//not allowed to use a 2-handed saber as second saber
		WP_RemoveGun( ent, GunNum );
		return; 
	}*/
	G_ModelIndex( ent->client->gs.gun[GunNum].Gun_Model );
	WP_GunInitData( ent );
	
	if ( GunNum == 1 )
	{//now have 2 guns
		ent->client->gs.dualGuns = qtrue;
	}
	else if ( GunNum == 0 )
	{
		if ( ent->weaponModel[1] == -1 )
		{//don't have 2 sabers
			ent->client->gs.dualGuns = qfalse;
		}
	}
	G_RemoveGunModels( ent );
	WP_GunAddG2GunModels( ent, GunNum );
}

void WP_GunSetGun( gentity_t *ent, int GunNum, int BarrelNum, char *GunName )
{
	if ( !ent || !ent->client )
	{
		return;
	}
}

void WP_GunLoadParms( void ) 
{
// First off , init special external data stuff!
// this should override weapons stuff and force power stuff
// of characters into 2 different libraries... 
	
	int			len, totallen, gunExtFNLen, fileCnt, i;
	char		*buffer, *holdChar, *marker;
	char		gunExtensionListBuf[4096];			//	The list of file names read in

	//set where to store the first one
	totallen = 0;
	marker = GunParms;
	marker[0] = '\0';

	//now load in the .sab definitions
	fileCnt = gi.FS_GetFileList("ext_data/guns", ".gun", gunExtensionListBuf, sizeof(gunExtensionListBuf) );

	holdChar = gunExtensionListBuf;
	for ( i = 0; i < fileCnt; i++, holdChar += gunExtFNLen + 1 ) 
	{
		gunExtFNLen = strlen( holdChar );

		len = gi.FS_ReadFile( va( "ext_data/guns/%s", holdChar), (void **) &buffer );

		if ( len == -1 ) 
		{
			gi.Printf( "WP_GunLoadParms: error reading %s\n", holdChar );
		}
		else
		{
			gi.Printf( S_COLOR_GREEN "I am Reading %s\n",holdChar );
			if ( totallen && *(marker-1) == '}' )
			{//don't let it end on a } because that should be a stand-alone token
				strcat( marker, " " );
				totallen++;
				marker++; 
			}
			len = COM_Compress( buffer );

			if ( totallen + len >= MAX_GUN_DATA_SIZE ) 
			{
				G_Error( "WP_GunLoadParms: ran out of space before reading %s\n(you must make the .sab files smaller)", holdChar  );
			}
			strcat( marker, buffer );
			gi.FS_FreeFile( buffer );

			totallen += len;
			marker += len;
		}
	}
}





