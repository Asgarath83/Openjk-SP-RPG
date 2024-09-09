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

// Filename:-	weapons.h
//
// Note that this is now included from both server and game modules, so don't include any other header files
//	within this one that might break stuff...

#ifndef __WEAPONS_H__
#define __WEAPONS_H__

typedef enum //# weapon_e
{
	WP_NONE,
	// Player weapons
	WP_SABER,// spada 
	// armi da tiratore configurazione 1 
	WP_BLASTER_PISTOL,	// Pistole Key 2
	WP_BLASTER, // Fucili Key 3 
	WP_DISRUPTOR, // Fucili da Cecchino - Key 4
	WP_REPEATER, // Mitragliatore - Key 5
	WP_FLECHETTE, // Carabine - Key 6
	WP_BOWCASTER, // Armi Pesanti - Key 7
	WP_ROCKET_LAUNCHER, // Lanciarazzi - Key 8
	WP_NOGHRI_STICK, // BioHazard - armi biochimiche, - Key 9
	// o caricate a tossine vegetali, animali, micetiche, infezioni
	WP_DEMP2, // Armi Esotiche e Speciali, arma a controllo remoto - Key 0
	WP_CONCUSSION, // Cannone a mano, artiglieria - Key '
	WP_THERMAL, // Granate - Roll ì 
	WP_TRIP_MINE, //Mine - Roll ì
	 WP_DET_PACK, // Cariche Esplosive - Roll ì 
	//------------------------------------------------------------------------
	//CONFIG2-. ARMI DA GUERRIERO 
	// WP_SABER,		// - spada arma universale, - KEY 1
	WP_MELEE,			// Corpo a Corpo, arma universale di combattimento fisico. - Key 2 
	WP_BOW,			// Archi e Frecce - Key 3
	WP_CROSSBOW,	// Balestre - Key 4  
	WP_SLING,		// Fionde -  KEY5
	WP_THROWABLE,	// Armi lanciabili: Pugnali, giavellotti, Bolas, Kamas - KEY 6, 
	WP_STUN_BATON,		// Bastoni Stordenti e Armi da Autodifesa - Key 7 
	WP_TUSKEN_STAFF,	// Cerbottane - Key 8 
	WP_TUSKEN_RIFLE,	// Moschetto - Key 9
	WP_CANNON,			// Cannone a Mano - KEY0
	WP_HOOKER,			// Rampino, Grapple Hook Key ' 
	WP_SCEPTER,			// Scettro - Key ì
	//-------------------------------------------------------------------
	// NOSGOTH: gli scettri e le armi lancia energia dei Guardiani e dei Signori degli Hylden
	// CONFIG: 3: ARMI DA MAGO  
	// WP_SABER,	// Spada, arma universale - KEY 1
	WP_DARK,		// proiettili di Tenebra - KEY 2
	// ---------------------------------------------------------------
	WP_LIGHT,		// proiettili di Luce - Roll KEY 3 
	WP_LIGHTNING,		// proiettili Folgore Disgregatrice - Roll KEY 3 
	WP_FIRE,	// proiettili di Fuoco - Roll KEY 4 
	WP_FLAME,	// proiettili di Fuoco e Fiamma Aggregatrice - Roll KEY 4 
	WP_AIR,		// proiettili d'Aria - KEY 5 
	WP_WATER,			// proiettili d'Acqua - KEY 6
	WP_EARTH,		// proiettili di Terra - KEY 7 
	WP_SONIC,		// proiettili di Suono - KEY 8 
    WP_PSYCHIC,		// proiettili psichici - KEY 9 
	WP_WOOD,		// proiettili di Legno - Roll Key 0
	WP_METAL,		// proiettili di Metallo,- Roll KEY 0
	WP_SPIRIT, // proiettili di Spirito - Roll Key ' 
	WP_POISON, // Proiettili di Veleno: - Roll Key '
	WP_HOLY,		// proiettili Sacri - KEY ì  
	WP_NECRO,		// proiettili necromantici - KEY ì
// --------------------------------------------------------------------
    // CONFIG 4: Armi dei Caccia e VEICOLI, TANK, VELIVOLI SOTTOMARINI ETC e NPC 
	// WP_SABER		// scudo difensivo - Key 1
	WP_BRYAR_PISTOL,	// Arma Stordente - Key 2 
	WP_EMPLACED_GUN,// Laser Primari - Key 3 
	WP_ATST_MAIN,	// Dardi e Obici - Key 4 
	WP_ATST_SIDE,		// Torpedo - Key 5 
	WP_TIE_FIGHTER,		// Armi a Raggio - Key 6 
	WP_JAWA,			// Armi a dispersione - Key 7  
	WP_BOT_LASER,		// Armi a Impulso e Bombe al Plasma - Key 8  
	WP_RAPID_FIRE_CONC,	// Cannoni e Hyperlaser - Key 0
	WP_ANTIMATTER,		// Razzi ad Antimateria, Key ' 
	WP_DEFENSE,		// Difese dei caccia: scudi, chaff, flare etc - Roll Key ì
	WP_DRONE,		// Mine e contromisure dei caccia - Roll Key ì
	WP_POSITRON,		// Cannone ad antimateria, arma suprema. Roll Key ì 
	WP_TURRET,			// Torretta mitragliatrice lanciaraggi
	// ----------------------------------------------------------
	// MAGIE SPECIALI  
	// SUMMONINGS EVOCAZIONI! - CONFIG 5 - they have just few ammos - USABILI SOLO DA MENU "EVOCAZIONI"
	// EVOCAZIONI - USALE PER SPAWNARE CREATURE E NON PER I TEAMMATES, CHE INVECE
	// POSSON ESSER GESTITE DAL MENU E FATE CON I POTERI DELLA FORZA 
	WP_SUMMON1,			// evocazione slot 2. KEY 2 - Guerriero 
	WP_SUMMON2,			// evocazione slot 3. KEY 3 - Mago
	WP_SUMMON3,			// evocazione slot 4. KEY 4 - Tiratore
	WP_SUMMON4,			// evocazione slot 5. KEY 5 - Speciale 
	// metamorfosi 
	WP_MORPH1,// Forma 1 dark Key 6 - Animale di Terra
	WP_MORPH2,// Forma 2 light Key 7 - Animale d'Aria 
	//////////////////////////////////////////////////////////////////////
	WP_MORPH3,// Forma 3 fire Key 8 - Animale d'Acqua 
	WP_MORPH4, // Forma 4 air Key 9 - Elementale o speciale 
	// INVENTARIO CONFIG 6: ITEMS
	WP_ITEM1, // Dispositivi di Occultamento - key 1 - protezione da luce o raggi
	WP_ITEM2, // Occhiali e Visori, sistemi di rivelazione, radar key 2. 
	WP_ITEM3, // Protezione del calore e dalla siccità - key 3
	WP_ITEM4, // Velocizzare, volare, protezione dal vuoto, tute spaziali. - key 4
	WP_ITEM5, // nuotare, protezione dall'annegamento, dall'umidità e dal gelo - key 5
	WP_ITEM6, // protezione e difesa fisica, difesa dalla pressione e dagli urti - key 6
	WP_ITEM7, // protezione sonica o auricolari, strumento musicale- key 7
	WP_ITEM8, // strumento psichico, o protezione psichica - key 8
	WP_ITEM9, // antispora, antidoto al veleno delle piante, o fiore di bach o erba.  - key 9
	WP_ITEM10, // Utensile metallico di vario tipo. Oggetto a controllo remoto - Key 0
	WP_ITEM11, // Difesa magica, oggetto spirituale, Kit medico - Key ì
	WP_ITEM12, // Antidoto Chimico o al veleno, stimolanti, droga. 
	// Forma 12 Poison / Necro  Key ì*/
	WP_NUM_WEAPONS 
	//# #eol
} weapon_t;

#define FIRST_WEAPON		WP_SABER		// this is the first weapon for next and prev weapon switching
#define MAX_PLAYER_WEAPONS	WP_NUM_WEAPONS - 1 // WP_POISON		// this is the max you can switch to and get with the give all. - FIXME: it's actually this one *minus* one... why?
extern qboolean playerUsableWeapons[WP_NUM_WEAPONS];
// WP_STUN_BATON

// WEAPON LIBRARY! 


typedef struct
{
	vec3_t		gunmuzzlePoint;
	vec3_t		gunmuzzlePointOld;
	vec3_t		gunmuzzleDir;
	vec3_t		gunmuzzleDirOld;
} shootInfo_t;
#define MAX_SHOOTS 12


typedef enum
{
	GUN_HOLSTER_INVALID = -1,
	GUN_HOLSTER_NONE = 0,
	GUN_HOLSTER_HIPS,
	GUN_HOLSTER_BACK,
	GUN_HOLSTER_LHIP
} gun_holster_locations_t;



// LISTE DELLE ARMI 

typedef enum //# Blade_e 
{
	BLADE_NONE, // free slot now use . marker -
	SOULREAVER, //
	BLADE_MAX
} Blade_t; 
// LISTA DI TUTTE LE MIETITRICI DI SOUL REAVER, DI QUELLE DEGLI EMBLEMI DEI PILASTRI
// DELLE REAVER PURE E IBRIDE DEI MIETITORI D'ANIME
// DI TUTTE LE SPADE E LE ARMI BRANDITE DAI SARAFAN E DAGLI HYLDEN E DA TUTTI GLI
// SPADACCINI DI NOSGOTH DI TUTTI I GIOCHI DELLA LOK!)
// (LASCIA STARE, QUELLE FACCIO IO SON DAVVERO TROPPE)

typedef enum //# Pistol_e 
{
	PISTOL_NONE, // free slot now use . marker -
	PISTOL1, //
	PISTOL_MAX
} Pistol_t; 

typedef enum //# Rifle_e 
{
	RIFLE_NONE, // free slot now use . marker -
	RIFLE1, //
	RIFLE_MAX
} Rifle_t; 


typedef enum //# Sniper_Rifle_e 
{
	SNIPER_RIFLE_NONE, // free slot now use . marker -
	SNIPER_RIFLE1, //
	SNIPER_RIFLE_MAX
} Sniper_Rifle_t; 


typedef enum //# Repeater_e 
{
	REPEATER_NONE, // free slot now use . marker -
	REPEATER1, //
	REPEATER_MAX
} Repeater_t; 

typedef enum //# Carabine_e 
{
	CARABINE_NONE, // free slot now use . marker -
	CARABINE1, //
	CARABINE_MAX
} Carabine_t; 


typedef enum //# HeavyGun_e 
{
	HG_NONE, // free slot now use . marker -
	HG1, //
	HG_MAX
} HeavyGun_t; 


typedef enum //# RPG_e 
{
	RPG_NONE, // free slot now use . marker -
	RPG1, //
	RPG_MAX
} RPG_t; 


typedef enum //# Biohazard_e 
{
	BIOHAZARD_NONE, // free slot now use . marker -
	BIOHAZARD1, //
	BIOHAZARD_MAX
} Biohazard_t; 



typedef enum //# Exotic_e 
{
	EXOTIC_NONE, // free slot now use . marker -
	EXOTIC1, //
	EXOTIC_MAX
} Exotic_t; 


typedef enum //# Cannon_e 
{
	CANNON_NONE, // free slot now use . marker -
	CANNON1, //
	CANNON_MAX
} Cannon_t;  // Artiglieria 


typedef enum //# Grenade_e 
{
	GRENADE_NONE, // free slot now use . marker -
	GRENADE1, //
	GRENADE_MAX
} Grenade_t;  // Granate 

typedef enum //# Mine_e 
{
	MINE_NONE, // free slot now use . marker -
	MINE1, //
	MINE_MAX
} Mine_t;  // Mine laser 


typedef enum //# Bomb_e 
{
	BOMB_NONE, // free slot now use . marker -
	BOMB1, //
	BOMB_MAX
} Bomb_t;  // Bombe, esplosivo con detonatore a distanza

//------------------------------------------------------------------------
	//CONFIG2-. ARMI DA GUERRIERO 
	// WP_SABER,		// - spada arma universale, - KEY 1
	

typedef enum //# Melee_e 
{
	MELEE_NONE, // free slot now use . marker -
	MELEE1, //
	MELEE_MAX
} Melee_t; 

typedef enum //# Bow_e 
{
	BOW_NONE, // free slot now use . marker -
	BOW1, //
	BOW_MAX
} Bow_t; 

typedef enum //# Crossbow_e 
{
	CROSSBOW_NONE, // free slot now use . marker -
	CROSSBOW1, //
	CROSSBOW_MAX
} Crossbow_t; 


typedef enum //# Sling_e 
{
	SLING_NONE, // free slot now use . marker -
	SLING1, //
	SLING_MAX
} Sling_t; // fionde 


typedef enum //# Throwable_e 
{
	THROWABLE_NONE, // free slot now use . marker -
	THROWABLE1, //
	THROWABLE_MAX
} Throwable_t; // Pugnali, giavellotti, shuriken etc etc 


typedef enum //# Baton_e 
{
	BATON_NONE, // free slot now use . marker -
	BATON1, //
	BATON_MAX
} Baton_t; //  bastoni da autodifesa e taser


typedef enum //# Staff_e 
{
	STAFF_NONE, // free slot now use . marker -
	STAFF1, //
	STAFF_MAX
} Staff_t; // Spranghe, clave, cose con cui picchiare la gente

typedef enum //# Musket_e 
{
	MUSKET_NONE, // free slot now use . marker -
	MUSKET1, //
	MUSKET_MAX
} Musket_t; // Moschetto o Archibugio: spari o infilzi il nemico

typedef enum //# HandCannon_e 
{
	HANDCANNON_NONE, // free slot now use . marker -
	HANDCANNON1, //
	HANDCANNON_MAX
} HandCannon_t; // Cannone a Mano, (tipo cacciatori sparapalle di sr2) 


typedef enum //# Hooker_e
{
	HOOKER_NONE, // free slot now use . marker -
	HOOKER1, //
	HOOKER_MAX
} Hooker_t; // Grapple Hook per scalare i muri (Xado può usar le sue ragnatele, ad esempio
// rek degli strali di ghiaccio e così via)
 

typedef enum //# Scepter_e
{
	SCEPTER_NONE, // free slot now use . marker -
	SCEPTER1, //
	SCEPTER_MAX
} Scepter_t;
// Scettro magico degli stregoni 
	//-------------------------------------------------------------------
	// NOSGOTH: gli scettri e le armi lancia energia dei Guardiani e dei Signori degli Hylden
	// CONFIG: 3: ARMI DA MAGO  
	// WP_SABER,	// Spada, arma universale - KEY 1
	

typedef enum //# Dark_e 
{
	DARK_NONE, // free slot now use . marker -
	DARK1, //
	DARK_MAX
} Dark_t; 

typedef enum //# Light_e 
{
	LIGHT_NONE, // free slot now use . marker -
	LIGHT1, //
	LIGHT_MAX
} Light_t; 

typedef enum //# Lightning_e 
{
	LIGHTNING_NONE, // free slot now use . marker -
	LIGHTNING1, //
	LIGHTNING_MAX
} Lightning_t; 

typedef enum //# Fire_e 
{
	FIRE_NONE, // free slot now use . marker -
	FIRE1, //
	FIRE_MAX
} Fire_t; // foco! 

typedef enum //# Flame_e 
{
	FLAME_NONE, // free slot now use . marker -
	FLAME1, //
	FLAME_MAX
} Flame_t;  

typedef enum //# Air_e
{
	AIR_NONE, // free slot now use . marker -
	AIR1, //
	AIR_MAX
} Air_t; //  proiettili d'aria 

typedef enum //# water_e 
{
	WATER_NONE, // free slot now use . marker -
	WATER1, //
	WATER_MAX
} Water_t; // Spranghe, clave, cose con cui picchiare la gente

typedef enum //# Earth_e 
{
	EARTH_NONE, // free slot now use . marker -
	EARTH1, //
	EARTH_MAX
} Earth_t; //

typedef enum //# Sonic_e 
{
	SONIC_NONE, // free slot now use . marker -
	SONIC1, //
	SONIC_MAX
} Sonic_t; // Attacchi Sonici

typedef enum //# Psychic_e
{
	PSYCHIC_NONE, // free slot now use . marker -
	PSYCHIC1, //
	PSYCHIC_MAX
} Psychic_t; // Atk Psichici

typedef enum //# Wood_e
{
	WOOD_NONE, // free slot now use . marker -
	WOOD1, //
	WOOD_MAX
} Wood_t;  // proiettili scagliati da vegetali, funghi e bestie mutanti

typedef enum //# Metal_e
{
	METAL_NONE, // free slot now use . marker -
	METAL1, //
	METAL_MAX
} Metal_t;  // attacchi magici di elemento Metallo!


typedef enum //# Spirit_e
{
	SPIRIT_NONE, // free slot now use . marker -
	SPIRIT1, //
	SPIRIT_MAX
} Spirit_t;  // proiettili spirituali


typedef enum //# Poison_e
{
	POISON_NONE, // free slot now use . marker -
	POISON1, //
	POISON_MAX
} Poison_t;  // proiettili spirituali


typedef enum //# Holy_e
{
	HOLY_NONE, // free slot now use . marker -
	HOLY1, //
	HOLY_MAX
} Holy_t;  // proiettili sacri

 
typedef enum //# Necro_e
{
	NECRO_NONE, // free slot now use . marker -
	NECRO1, //
	NECRO_MAX
} Necro_t;  // proiettili di magia nera \ infernale 

// --------------------------------------------------------------------
    // CONFIG 4: Armi dei Caccia e VEICOLI, TANK, VELIVOLI SOTTOMARINI ETC e NPC 
	// WP_SABER		// scudo difensivo - Key 1




typedef enum //# Stunner_e 
{
	STUNNER_NONE, // free slot now use . marker -
	STUNNER1, //
	STUNNER_MAX
} Stunner_t; 


typedef enum //# laser_e 
{
	LASER_NONE, // free slot now use . marker -
	LASER1, //
	LASER_MAX
} Laser_t; 


typedef enum //# dardus_e 
{
	DARDUS_NONE, // free slot now use . marker -
	DARDUS1, //
	DARDUS_MAX
} Dardus_t; 


typedef enum //# Torpedo_e 
{
	TORPEDO_NONE, // free slot now use . marker -
	TORPEDO1, //
	TORPEDO_MAX
} Torpedo_t; 

typedef enum //# RayGun_e 
{
	RAYGUN_NONE, // free slot now use . marker -
	RAYGUN1, //
	RAYGUN_MAX
} RayGun_t; 

typedef enum //# Spreader_e 
{
	SPREADER_NONE, // free slot now use . marker -
	SPREADER1, //
	SPREADER_MAX
} Spreader_t; // foco! 

typedef enum //# Pulse_e 
{
	PULSE_NONE, // free slot now use . marker -
	PULSE1, //
	PULSE_MAX
} Pulse_t; 

typedef enum //# Hyperlaser_e 
{
	HYPERLASER_NONE, // free slot now use . marker -
	HYPERLASER1, //
	HYPERLASER_MAX
} Hyperlaser_t; 


typedef enum //# Rocket_e 
{
	ROCKET_NONE, // free slot now use . marker -
	ROCKET1, //
	ROCKET_MAX
} Rocket_t; 


typedef enum //# Defense_e 
{
	DEFENSE_NONE, // free slot now use . marker -
	DEFENSE1, //
	DEFENSE_MAX
} Defense_t; 

typedef enum //# Drone_e 
{
	DRONE_NONE, // free slot now use . marker -
	DRONE1, //
	DRONE_MAX
} Drone_t; 

typedef enum //# Positron_e 
{
	POSITRON_NONE, // free slot now use . marker -
	POSITRON1, //
	POSITRON_MAX
}Positron_t; 

// MAGIC POWERS -----------------------------------------------------------
typedef enum //# Sense_e 
{
	SENSE_NONE, // free slot now use . marker -
	SENSE1, //
	SENSE_MAX
} Sense_t;  

typedef enum //# Speed_e 
{
	SPEED_NONE, // free slot now use . marker -
	SPEED1, //
	SPEED_MAX
} Speed_t;  


typedef enum //# SpecialMove_e 
{
	SPECIALMOVE_NONE, // free slot now use . marker -
	SPECIALMOVE1, //
	SPECIALMOVE_MAX
} SpecialMove_t;  

typedef enum //# Push_e 
{
	PUSH_NONE, // free slot now use . marker -
	PUSH1, //
	PUSH_MAX
} Push_t;  

typedef enum //# TK_e 
{
	TK_NONE, // free slot now use . marker -
	TK1, //
	TK_MAX
} Telekinesis_t;  

typedef enum //# BladeDefense_e 
{
	BLADEDEFENSE_NONE, // free slot now use . marker -
	BLADEDEFENSE1, //
	BLADEDEFENSE_MAX
} BladeDefense_t; 

typedef enum //# BladeOffense_e 
{
	BLADEOFFENSE_NONE, // free slot now use . marker -
	BLADEOFFENSE1, //
	BLADEOFFENSE_MAX
} BladeOffense_t; 

typedef enum //# BladeThrow_e 
{
	BLADETHROW_NONE, // free slot now use . marker -
	BLADETHROW1, //
	BLADETHROW_MAX
} BladeThrow_t; 

typedef enum //# Heal_e 
{
	HEAL_NONE, // free slot now use . marker -
	HEAL1, //
	HEAL_MAX
} Heal_t; 


typedef enum //# Arcane_e 
{
	ARCANE_NONE, // free slot now use . marker -
	ARCANE1, //
	ARCANE_MAX
} Arcane_t; // BUFF AND CURSE ! 


typedef enum //# Protect_e 
{
	PROTECT_NONE, // free slot now use . marker -
	PROTECT1, //
	PROTECT_MAX
} Protect_t; // BUFF AND CURSE ! 


typedef enum //# MagicProtect_e 
{
	MAGICPROTECT_NONE, // free slot now use . marker -
	MAGICPROTECT1, //
	MAGICPROTECT_MAX
} MagicProtect_t; // BUFF AND CURSE ! 



typedef enum //# Grip_e 
{
	GRIP_NONE, // free slot now use . marker -
	GRIP1, //
	GRIP_MAX
} Grip_t; // BUFF AND CURSE ! 

typedef enum //# Power_e 
{
	POWER_NONE, // free slot now use . marker -
	POWER1, //
	POWER_MAX
} Power_t; // BUFF AND CURSE ! 

typedef enum //# Burst_e 
{
	BURST_NONE, // free slot now use . marker -
	BURST1, //
	BURST_MAX
} Burst_t; // BUFF AND CURSE ! 

typedef enum //# Drain_e 
{
	DRAIN_NONE, // free slot now use . marker -
	DRAIN1, //
	DRAIN_MAX
} Drain_t; // BUFF AND CURSE ! 

typedef enum //# Stun_e
{
	STUN_NONE, // free slot now use . marker -
	STUN1, //
	STUN_MAX
} Stun_t; //  proiettili d'aria 

typedef enum //# Charm_e 
{
	CHARM_NONE, // free slot now use . marker -
	CHARM1, //
	CHARM_MAX
} Charm_t; // Spranghe, clave, cose con cui picchiare la gente

typedef enum //# Possession_e 
{
	POSSESSION_NONE, // free slot now use . marker -
	POSSESSION1, //
	POSSESSION_MAX
} Possession_t; //

typedef enum //# Freeze_e 
{
	FREEZE_NONE, // free slot now use . marker -
	FREEZE1, //
	FREEZE_MAX
} Freeze_t; // Attacchi Sonici

typedef enum //# HealOther_e
{
	HEALOTHER_NONE, // free slot now use . marker -
	HEALOTHER1, //
	HEALOTHER_MAX
} HealOther_t; // Atk Psichici

typedef enum //# SpecialSpell_e
{
	SPECIALSPELL_NONE, // free slot now use . marker -
	SPECIALSPELL1, //
	SPECIALSPELL_MAX
} SpecialSpell_t;  // proiettili scagliati da vegetali, funghi e bestie mutanti

// SHOCKWAVES e ATK D'URTO
typedef enum //# DarkAOE_e
{
	DARKAOE_NONE, // free slot now use . marker -
	DARKAOE1, //
	DARKAOE_MAX
} DarkAoe_t;  // attacchi magici di elemento Tenebra!

typedef enum //# LightAoe_e
{
	LIGHTAOE_NONE, // free slot now use . marker -
	LIGHTAOE1, //
	LIGHTAOE_MAX
} LightAoe_t;  // Aoe di luce o elettromagnetico 

typedef enum //# FireAOE_e
{
	FIREAOE_NONE, // free slot now use . marker -
	FIREAOE1, //
	FIREAOE_MAX
} FireAoe_t;  // attacchi magici di elemento Fuoco

typedef enum //# AirAoe_e
{
	AIRAOE_NONE, // free slot now use . marker -
	AIRAOE1, //
	AIRAOE_MAX
} AirAoe_t;  // attacchi d'urto dìaria

typedef enum //# WaterAOE_e
{
	WATERAOE_NONE, // free slot now use . marker -
	WATERAOE1, //
	WATERAOE_MAX
} WaterAoe_t;  // attacchi d'urto di elemento Acqua!

typedef enum //# EarthAoe_e
{
	EARTHAOE_NONE, // free slot now use . marker -
	EARTHAOE1, //
	EARTHAOE_MAX
} EarthAoe_t;  // Attacco d'urto di Terra

typedef enum //# SonicAOE_e
{
	SONICAOE_NONE, // free slot now use . marker -
	SONICAOE1, //
	SONICAOE_MAX
} SonicAoe_t;  // attacco d'urto Sonico O Psichico

typedef enum //# ArcaneAoe_e
{
	ARCANEAOE_NONE, // free slot now use . marker -
	ARCANEAOE1, //
	ARCANEAOE_MAX
} ArcaneAoe_t;  // Glifo di Forza, AOE di Legno, AOE di Metallo 

typedef enum //# HolyAoe_e
{
	HOLYAOE_NONE, // free slot now use . marker -
	HOLYAOE1, //
	HOLYAOE_MAX
} HolyAoe_t;  // attacco d'urto spirituale o sacro 
 
typedef enum //# NecroAoe_e
{
	NECROAOE_NONE, // free slot now use . marker -
	NECROAOE1, //
	NECROAOE_MAX
} NecroAoe_t;  // Attacco AOE venefico o necrotico 

// ----------------------------------------------------------
	// MAGIE SPECIALI  
	// SUMMONINGS EVOCAZIONI! - CONFIG 5 - they have just few ammos - USABILI SOLO DA MENU "EVOCAZIONI"
	// EVOCAZIONI - USALE PER SPAWNARE CREATURE E NON PER I TEAMMATES, CHE INVECE
	// POSSON ESSER GESTITE DAL MENU E FATE CON I POTERI DELLA FORZA 
// EVOCAZIONI 	
typedef enum //# Summon1_e
{
	SUMMON1_NONE, // free slot now use . marker -
	SUMMON1, //
	SUMMON1_MAX
} Summon1_t;  // Attacco AOE venefico o necrotico 

typedef enum //# Summon2_e
{
	SUMMON2_NONE, // free slot now use . marker -
	SUMMON2, //
	SUMMON2_MAX
} Summon2_t;  // Attacco AOE venefico o necrotico 

typedef enum //# Summon3_e
{
	SUMMON3_NONE, // free slot now use . marker -
	SUMMON3, //
	SUMMON3_MAX
} Summon3_t;  // Attacco AOE venefico o necrotico 

typedef enum //# Summon4_e
{
	SUMMON4_NONE, // free slot now use . marker -
	SUMMON4, //
	SUMMON4_MAX
} Summon4_t;  // Attacco AOE venefico o necrotico 

// TRASMUTAZIONI 	
typedef enum //# MORPH1_e
{
	MORPH1_NONE, // free slot now use . marker -
	MORPH1, //
	MORPH1_MAX
} Morph1_t;  // Nosgoth: Wolf Forms

typedef enum //# MORPH2_e
{
	MORPH2_NONE, // free slot now use . marker -
	MORPH2, //
	MORPH2_MAX
} Morph2_t;  // Nosgoth: Bat Forms 

typedef enum //# MORPH3_e
{
	MORPH3_NONE, // free slot now use . marker -
	MORPH3, //
	MORPH3_MAX
} Morph3_t;  // Nosgoth: Mist Forms 

typedef enum //# MORPH4_e
{
	MORPH4_NONE, // free slot now use . marker -
	MORPH4, //
	MORPH4_MAX
} Morph4_t;  // Nosgoth: Disguise Human Form or Special Form 

// INVENTARIO !!!! 

typedef enum //# ITEM1_e
{
	ITEM1_NONE, // free slot now use . marker -
	ITEM1, //
	ITEM1_MAX
} Item1_t;  // Attacco AOE venefico o necrotico 

typedef enum //# ITEM2_e
{
	ITEM2_NONE, // free slot now use . marker -
	ITEM2, //
	ITEM2_MAX
} Item2_t;  // Attacco AOE venefico o necrotico 

typedef enum //# ITEM3_e
{
	ITEM3_NONE, // free slot now use . marker -
	ITEM3, //
	ITEM3_MAX
} Item3_t;  // Attacco AOE venefico o necrotico 

typedef enum //# ITEM4_e
{
	ITEM4_NONE, // free slot now use . marker -
	ITEM4, //
	ITEM4_MAX
} Item4_t;  // Attacco AOE venefico o necrotico 

typedef enum //# Item5_e
{
	ITEM5_NONE, // free slot now use . marker -
	ITEM5, //
	ITEM5_MAX
} Item5_t;  // Nosgoth: Wolf Forms

typedef enum //# ITEM6_e
{
	ITEM6_NONE, // free slot now use . marker -
	ITEM6, //
	ITEM6_MAX
} Item6_t;  // Nosgoth: Bat Forms 

typedef enum //# ITEM7_e
{
	ITEM7_NONE, // free slot now use . marker -
	ITEM7, //
	ITEM7_MAX
} Item7_t;  // Nosgoth: Mist Forms 

typedef enum //# ITEM8_e
{
	ITEM8_NONE, // free slot now use . marker -
	ITEM8, //
	ITEM8_MAX
} Item8_t;  // Nosgoth: Disguise Human Form or Special Form 

typedef enum //# Item9_e
{
	ITEM9_NONE, // free slot now use . marker -
	ITEM9, //
	ITEM9_MAX
} Item9_t;  // Nosgoth: Wolf Forms

typedef enum //# ITEM10_e
{
	ITEM10_NONE, // free slot now use . marker -
	ITEM10, //
	ITEM10_MAX
} Item10_t;  // Nosgoth: Bat Forms 

typedef enum //# ITEM11_e
{
	ITEM11_NONE, // free slot now use . marker -
	ITEM11, //
	ITEM11_MAX
} Item11_t;  // Nosgoth: Mist Forms 

typedef enum //# ITEM12_e
{
	ITEM12_NONE, // free slot now use . marker -
	ITEM12, //
	ITEM12_MAX
} Item12_t;  // Nosgoth: Disguise Human Form or Special Form 

typedef enum //# chargeshader_e
{
	// charge shaders stuff 
	CHARGE_NONE,
	CHARGE_CRIMSON,
	CHARGE_RED,
	CHARGE_ORANGE,
	CHARGE_YELLOW, 
	CHARGE_BROWN, 
	CHARGE_GREEN,
	CHARGE_BLUE, 
	CHARGE_AZURE, 
	CHARGE_CYAN, 
	CHARGE_INDIGO,
	CHARGE_PURPLE, 
	CHARGE_PINK, 
	CHARGE_WHITE, 
	CHARGE_GREY, 
	CHARGE_BLACK, 
	// electric rays
	CHARGE_ELECTRIC_CRIMSON,
	CHARGE_ELECTRIC_RED,
	CHARGE_ELECTRIC_ORANGE,
	CHARGE_ELECTRIC_YELLOW, 
	CHARGE_ELECTRIC_BROWN, 
	CHARGE_ELECTRIC_GREEN,
	CHARGE_ELECTRIC_BLUE, 
	CHARGE_ELECTRIC_AZURE, 
	CHARGE_ELECTRIC_CYAN, 
	CHARGE_ELECTRIC_INDIGO,
	CHARGE_ELECTRIC_PURPLE, 
	CHARGE_ELECTRIC_PINK, 
	CHARGE_ELECTRIC_WHITE, 
	CHARGE_ELECTRIC_GREY, 
	CHARGE_ELECTRIC_BLACK, 
	// Raggi Onirici
	CHARGE_DREAM,
	CHARGE_DREAMFIRE,
	CHARGE_DREAMLIGHT,
	CHARGE_DREAMSPIRIT,
	CHARGE_DREAMSONIC, 
	CHARGE_DREAMPSYCHIC,
	CHARGE_DREAMEARTH,
	CHARGE_DREAMWOOD,
	CHARGE_DREAMAIR,
	CHARGE_DREAMMETAL,
	CHARGE_DREAMWATER,
	CHARGE_DREAMDARK,
	CHARGE_DREAMPOISON,
	// Elemental Rays
	CHARGE_CREATION,
	CHARGE_DESTRUCTION,
	CHARGE_FIRE,
	CHARGE_FIRE_CRIMSON,
	CHARGE_FIRE_RED,
	CHARGE_FIRE_ORANGE,
	CHARGE_FIRE_YELLOW, 
	CHARGE_FIRE_BROWN, 
	CHARGE_FIRE_GREEN,
	CHARGE_FIRE_BLUE, 
	CHARGE_FIRE_AZURE, 
	CHARGE_FIRE_CYAN, 
	CHARGE_FIRE_INDIGO,
	CHARGE_FIRE_PURPLE, 
	CHARGE_FIRE_PINK, 
	CHARGE_FIRE_WHITE, 
	CHARGE_FIRE_IRIS, 
	CHARGE_FIRE_GREY, 
	CHARGE_FIRE_BLACK, 
	CHARGE_SMOKE,
	CHARGE_SPARK, 
	CHARGE_FLAME,
	CHARGE_WARM,
	CHARGE_STEAM,
	CHARGE_LAVA,
	CHARGE_FIREWORK,
	CHARGE_CONFLICT,
	CHARGE_EMBER,
	CHARGE_TORCH,
	CHARGE_FIREMETAL,
	CHARGE_RUST,
	CHARGE_STEEL,
	CHARGE_FATUUS, 
	CHARGE_FIREPOISON,
	CHARGE_BLOOD,
	CHARGE_ASH,
	// Light Rays
	CHARGE_LIGHT,
	CHARGE_ECLIPSE,
	CHARGE_SUN,
	CHARGE_RADIANCE,
	CHARGE_LIGHTNING,
	CHARGE_VAPOR,
	CHARGE_SAND,
	CHARGE_ROAR,
	CHARGE_MIRAGE,
	CHARGE_OASIS,
	CHARGE_LIGHTMETAL,
	CHARGE_WRAITH,
	CHARGE_FLASH,
	CHARGE_LIGHTPOISON,  // magnesium
	CHARGE_IRIS, 
	CHARGE_MUMMY, 
	// Spirit Rays
	CHARGE_SPIRIT, 
	CHARGE_SPACE, 
	CHARGE_THUNDER,
	CHARGE_EXPLOSION,
	CHARGE_BANG,
	CHARGE_TIDE,
	CHARGE_FORCE,
	CHARGE_VOICE,
	CHARGE_SING,
	CHARGE_HYMN,
	CHARGE_GRASS,
	CHARGE_SPIRITMETAL,// zinco 
	CHARGE_VISION,
	CHARGE_SPIRITPOISON,// fosforo 
	CHARGE_AMBROSIA,
	CHARGE_ICORE, 
	// Sound
	CHARGE_SONIC, 
	CHARGE_INSTINCT,
	CHARGE_LOGIC, 
	CHARGE_BLAZE,
	CHARGE_TORNADO,
	CHARGE_TSUNAMI,
	CHARGE_QUAKE,
	CHARGE_REVERB,
	CHARGE_AURA,
	CHARGE_WHISTLE,
	CHARGE_MUSIC,
	CHARGE_SONICMETAL,
	CHARGE_SONICPOISON,// cloro 
	CHARGE_SCREAM, 
	CHARGE_SPIDERWEB,
	CHARGE_PEACE,
	CHARGE_VOODOO,
	// Pensiero
	CHARGE_PSYCHIC,
	CHARGE_BLIND,
	CHARGE_JOY,
	CHARGE_RAGE,
	CHARGE_CONFUSION,
	CHARGE_SAD,
	CHARGE_APATHY,
	CHARGE_NOISE,
	CHARGE_POSSESSION,
	CHARGE_CHARMING,
	CHARGE_SHOCK,
	CHARGE_FOCUS,
	CHARGE_SLEEP,
	CHARGE_HEAL,
	CHARGE_FEAR,
	CHARGE_PSYCHICMETAL, // palladio
	CHARGE_PSYCHICPOISON, // dendrite 
	// Terra
	CHARGE_EARTH,
	CHARGE_OSSIDIAN,
	CHARGE_CRYSTAL,
	CHARGE_METEOR, 
	CHARGE_DRY, 
	CHARGE_DUST, 
	CHARGE_COLD, 
	CHARGE_JADE, 
	CHARGE_STONE,
	CHARGE_BREAK,
	CHARGE_SANDSTONE,
	CHARGE_COAL,
	CHARGE_EARTHMETAL,// piombo
	CHARGE_MAGIC,
	CHARGE_EARTHPOISON,
	CHARGE_MILK,
	CHARGE_BONE,
	// Legno
	CHARGE_WOOD,
	CHARGE_SPORE,
	CHARGE_MUSHROOM,
	CHARGE_LEAF,
	CHARGE_RESIN,
	CHARGE_SMELL,
	CHARGE_STINK,
	CHARGE_LYMPH, 
	CHARGE_ROOT,
	CHARGE_BARK,
	CHARGE_GUM,
	CHARGE_NUT,
	CHARGE_QUILL,
	CHARGE_THORN,
	CHARGE_BRAMBLE,
	CHARGE_STING,
	CHARGE_WOODMETAL,// alluminio 
	CHARGE_WOODPOISON, 
	CHARGE_DRYAD,
	CHARGE_LATEX,
	CHARGE_POLLEN,
	CHARGE_SEED,
	CHARGE_FLOWER,
	CHARGE_FRUIT,
	CHARGE_AMBER,
	// Aria
	CHARGE_AIR,
	CHARGE_VACUUM,
	CHARGE_STORM,
	CHARGE_METHANE,
	CHARGE_COLDFIRE, // fuoco di ghiaccio
	CHARGE_WIND,
	CHARGE_RAIN,
	CHARGE_SNOW,
	CHARGE_HAILSTORM,
	CHARGE_GEYSER,
	CHARGE_BELL,
	CHARGE_MIST,
	CHARGE_DEW,
	CHARGE_AIRMETAL,// mercurio 
	CHARGE_BREATH,
	CHARGE_FROST,
	CHARGE_AIRPOISON, 
	CHARGE_TRUTH,
	CHARGE_HOAX,
	// Metallo
	CHARGE_METAL,
	CHARGE_OPAQUE,
	CHARGE_GLOW,
	CHARGE_PLASMA,
	CHARGE_CLOUD,
	CHARGE_ACID,
	CHARGE_ALACAL,
	CHARGE_GLASS,
	CHARGE_VIBRATION,
	CHARGE_SUPERFLUID,
	CHARGE_SUPERCONDUCTOR,
	CHARGE_BIRD,
	CHARGE_SUPERMETAL,//platino
	CHARGE_METALPOISON, // uranio 
	CHARGE_HOLOGRAM,
	CHARGE_SYNC,
	CHARGE_VOID,
	CHARGE_SHP,
	// Acqua
	CHARGE_WATER,
	CHARGE_INK,
	CHARGE_PEARL,
	CHARGE_SEASHELL,
	CHARGE_CORAL,
	CHARGE_NHN,
	CHARGE_ICE,
	CHARGE_FOG,
	CHARGE_MUD,
	CHARGE_ECHO,
	CHARGE_INFRASONIC,
	CHARGE_ULTRASONIC,
	CHARGE_BUBBLE,
	CHARGE_KELP, 
	CHARGE_WATERMETAL,
	CHARGE_SOUL,
	CHARGE_WATERPOISON,
	CHARGE_BLESSEDWATER,
	CHARGE_TAR,
	// Tenebra
	CHARGE_DARK,
	CHARGE_MOON,
	CHARGE_FIREFLY,
	CHARGE_CANDLE,
	CHARGE_SWARM,
	CHARGE_TEAR,
	CHARGE_CAVE,
	CHARGE_BUZZ,
	CHARGE_MEMORY,
	CHARGE_EBONY,
	CHARGE_DARKMETAL,
	CHARGE_SHADOW,
	CHARGE_DARKPOISON,// selenio 
	CHARGE_HONEY,
	CHARGE_NIGHTMARE, 
	// Veleno
	CHARGE_POISON,
	CHARGE_TOXIN, 
	CHARGE_VENOM, 
	CHARGE_BLACKHOLE,
	CHARGE_FALLOUT,
	CHARGE_NUKE,
	CHARGE_BAT,
	CHARGE_OIL,
	CHARGE_FREEZE,
	CHARGE_FRACAS,
	CHARGE_MUTE,
	CHARGE_PUMPKIN,
	CHARGE_WORMWOOD, 
	CHARGE_POISONMETAL,// iridio 
	CHARGE_SORCERY,
	CHARGE_SUPERPOISON,// arsenico 
	CHARGE_EXORCISM,
	CHARGE_PLAGUE,
	CHARGE_ALCHEMIC, 
	// Preesistenza
	CHARGE_SOULFIRE,
	CHARGE_SOULLIGHT,
	CHARGE_SOULSPIRIT,
	CHARGE_SOULSONIC, 
	CHARGE_SOULPSYCHIC,
	CHARGE_SOULEARTH,
	CHARGE_SOULWOOD,
	CHARGE_SOULAIR,
	CHARGE_SOULMETAL,
	CHARGE_SOULWATER,
	CHARGE_SOULDARK,
	CHARGE_SOULPOISON,
	CHARGE_SOULCREATION,
	CHARGE_SOULDESTRUCTION,
	CHARGE_SOULTIME,
	// Siderale
	CHARGE_ASTRAL,
	CHARGE_ASTRALFIRE,
	CHARGE_ASTRALLIGHT,
	CHARGE_ASTRALSPIRIT,
	CHARGE_ASTRALSONIC, 
	CHARGE_ASTRALPSYCHIC,
	CHARGE_ASTRALEARTH,
	CHARGE_ASTRALWOOD,
	CHARGE_ASTRALAIR,
	CHARGE_ASTRALMETAL,
	CHARGE_ASTRALWATER,
	CHARGE_ASTRALDARK,
	CHARGE_ASTRALPOISON,
	CHARGE_ASTRALHOLY,
	CHARGE_ASTRALNECRO,
	CHARGE_ASTRALCREATION,
	CHARGE_ASTRALDESTRUCTION,
	CHARGE_ASTRALMAGIC,
	// Temporale
	CHARGE_TIMEFIRE,
	CHARGE_TIMELIGHT,
	CHARGE_TIMESPIRIT,
	CHARGE_TIMESONIC, 
	CHARGE_TIMEPSYCHIC,
	CHARGE_TIMEEARTH,
	CHARGE_TIMEWOOD,
	CHARGE_TIMEAIR,
	CHARGE_TIMEMETAL,
	CHARGE_TIMEWATER,
	CHARGE_TIMEDARK,
	CHARGE_TIMEPOISON,
	// Iperspazio 
	CHARGE_HYPERSPACE, 
	CHARGE_GRAVITY,
	CHARGE_PHOTON,
	CHARGE_GLUON,
	CHARGE_BOSON,
	CHARGE_ANTIGRAVITY,
	CHARGE_QUARK,
	CHARGE_PROTON,
	CHARGE_NEUTRON,
	CHARGE_ELECTRON,
	CHARGE_NEUTRIN,
	// Heaven
	CHARGE_HEAVEN, 
	CHARGE_HEAVENLIGHTNING_CRIMSON,
	CHARGE_HEAVENLIGHTNING_RED,
	CHARGE_HEAVENLIGHTNING_ORANGE,
	CHARGE_HEAVENLIGHTNING_YELLOW, 
	CHARGE_HEAVENLIGHTNING_BROWN, 
	CHARGE_HEAVENLIGHTNING_GREEN,
	CHARGE_HEAVENLIGHTNING_BLUE, 
	CHARGE_HEAVENLIGHTNING_AZURE, 
	CHARGE_HEAVENLIGHTNING_CYAN, 
	CHARGE_HEAVENLIGHTNING_INDIGO,
	CHARGE_HEAVENLIGHTNING_PURPLE, 
	CHARGE_HEAVENLIGHTNING_PINK, 
	CHARGE_HEAVENLIGHTNING_WHITE, 
	CHARGE_HEAVENLIGHTNING_GREY, 
	CHARGE_HEAVENLIGHTNING_BLACK, 
	CHARGE_HEAVENFIRE,
	CHARGE_HEAVENFIRE_CRIMSON,
	CHARGE_HEAVENFIRE_RED,
	CHARGE_HEAVENFIRE_ORANGE,
	CHARGE_HEAVENFIRE_YELLOW, 
	CHARGE_HEAVENFIRE_BROWN, 
	CHARGE_HEAVENFIRE_GREEN,
	CHARGE_HEAVENFIRE_BLUE, 
	CHARGE_HEAVENFIRE_AZURE, 
	CHARGE_HEAVENFIRE_CYAN, 
	CHARGE_HEAVENFIRE_INDIGO,
	CHARGE_HEAVENFIRE_PURPLE, 
	CHARGE_HEAVENFIRE_PINK, 
	CHARGE_HEAVENFIRE_WHITE, 
	CHARGE_HEAVENFIRE_IRIS, 
	CHARGE_HEAVENFIRE_GREY, 
	CHARGE_HEAVENFIRE_BLACK, 
	CHARGE_HEAVENSMOKE,
	CHARGE_HEAVENSPARK, 
	CHARGE_HEAVENFLAME,
	CHARGE_HEAVENWARM,
	CHARGE_HEAVENSTEAM,
	CHARGE_HEAVENLAVA,
	CHARGE_HEAVENFIREWORK,
	CHARGE_HEAVENCONFLICT,
	CHARGE_HEAVENEMBER,
	CHARGE_HEAVENTORCH,
	CHARGE_HEAVENFIREMETAL,
	CHARGE_HEAVENRUST,
	CHARGE_HEAVENSTEEL,
	CHARGE_HEAVENFATUUS, 
	CHARGE_HEAVENFIREPOISON,
	CHARGE_HEAVENBLOOD,
	CHARGE_HEAVENASH,
	// Light Rays
	CHARGE_HEAVENLIGHT,
	CHARGE_HEAVENECLIPSE,
	CHARGE_HEAVENSUN,
	CHARGE_HEAVENRADIANCE,
	CHARGE_HEAVENLIGHTNING,
	CHARGE_HEAVENVAPOR,
	CHARGE_HEAVENSAND,
	CHARGE_HEAVENROAR,
	CHARGE_HEAVENMIRAGE,
	CHARGE_HEAVENOASIS,
	CHARGE_HEAVENLIGHTMETAL,
	CHARGE_HEAVENWRAITH,
	CHARGE_HEAVENFLASH,
	CHARGE_HEAVENLIGHTPOISON,  // magnesium
	CHARGE_HEAVENIRIS, 
	CHARGE_HEAVENMUMMY, 
	// Spirit Rays
	CHARGE_HEAVENSPIRIT, 
	CHARGE_HEAVENSPACE, 
	CHARGE_HEAVENTHUNDER,
	CHARGE_HEAVENEXPLOSION,
	CHARGE_HEAVENBANG,
	CHARGE_HEAVENTIDE,
	CHARGE_HEAVENFORCE,
	CHARGE_HEAVENVOICE,
	CHARGE_HEAVENSING,
	CHARGE_HEAVENHYMN,
	CHARGE_HEAVENGRASS,
	CHARGE_HEAVENSPIRITMETAL,// zinco 
	CHARGE_HEAVENVISION,
	CHARGE_HEAVENSPIRITPOISON,// fosforo 
	CHARGE_HEAVENAMBROSIA,
	CHARGE_HEAVENICORE, 
	// Sound
	CHARGE_HEAVENSONIC, 
	CHARGE_HEAVENINSTINCT,
	CHARGE_HEAVENLOGIC, 
	CHARGE_HEAVENBLAZE,
	CHARGE_HEAVENTORNADO,
	CHARGE_HEAVENTSUNAMI,
	CHARGE_HEAVENQUAKE,
	CHARGE_HEAVENREVERB,
	CHARGE_HEAVENAURA,
	CHARGE_HEAVENWHISTLE,
	CHARGE_HEAVENMUSIC,
	CHARGE_HEAVENSONICMETAL,
	CHARGE_HEAVENSONICPOISON,// cloro 
	CHARGE_HEAVENSCREAM, 
	CHARGE_HEAVENSPIDERWEB,
	CHARGE_HEAVENPEACE,
	CHARGE_HEAVENVOODOO,
	// Pensiero
	CHARGE_HEAVENPSYCHIC,
	CHARGE_HEAVENBLIND,
	CHARGE_HEAVENJOY,
	CHARGE_HEAVENRAGE,
	CHARGE_HEAVENCONFUSION,
	CHARGE_HEAVENSAD,
	CHARGE_HEAVENAPATHY,
	CHARGE_HEAVENNOISE,
	CHARGE_HEAVENPOSSESSION,
	CHARGE_HEAVENCHARMING,
	CHARGE_HEAVENSHOCK,
	CHARGE_HEAVENFOCUS,
	CHARGE_HEAVENSLEEP,
	CHARGE_HEAVENHEAL,
	CHARGE_HEAVENFEAR,
	CHARGE_HEAVENPSYCHICMETAL, // palladio
	CHARGE_HEAVENPSYCHICPOISON, // dendrite 
	// Terra
	CHARGE_HEAVENEARTH,
	CHARGE_HEAVENOSSIDIAN,
	CHARGE_HEAVENCRYSTAL,
	CHARGE_HEAVENMETEOR, 
	CHARGE_HEAVENDRY, 
	CHARGE_HEAVENDUST, 
	CHARGE_HEAVENCOLD, 
	CHARGE_HEAVENJADE, 
	CHARGE_HEAVENSTONE,
	CHARGE_HEAVENBREAK,
	CHARGE_HEAVENSANDSTONE,
	CHARGE_HEAVENCOAL,
	CHARGE_HEAVENEARTHMETAL,// piombo
	CHARGE_HEAVENMAGIC,
	CHARGE_HEAVENEARTHPOISON,
	CHARGE_HEAVENMILK,
	CHARGE_HEAVENBONE,
	// Legno
	CHARGE_HEAVENWOOD,
	CHARGE_HEAVENSPORE,
	CHARGE_HEAVENMUSHROOM,
	CHARGE_HEAVENLEAF,
	CHARGE_HEAVENRESIN,
	CHARGE_HEAVENSMELL,
	CHARGE_HEAVENSTINK,
	CHARGE_HEAVENLYMPH, 
	CHARGE_HEAVENROOT,
	CHARGE_HEAVENBARK,
	CHARGE_HEAVENGUM,
	CHARGE_HEAVENNUT,
	CHARGE_HEAVENQUILL,
	CHARGE_HEAVENTHORN,
	CHARGE_HEAVENBRAMBLE,
	CHARGE_HEAVENSTING,
	CHARGE_HEAVENWOODMETAL,// alluminio 
	CHARGE_HEAVENWOODPOISON, 
	CHARGE_HEAVENDRYAD,
	CHARGE_HEAVENLATEX,
	CHARGE_HEAVENPOLLEN,
	CHARGE_HEAVENSEED,
	CHARGE_HEAVENFLOWER,
	CHARGE_HEAVENFRUIT,
	CHARGE_HEAVENAMBER,
	// Aria
	CHARGE_HEAVENAIR,
	CHARGE_HEAVENVACUUM,
	CHARGE_HEAVENSTORM,
	CHARGE_HEAVENMETHANE,
	CHARGE_HEAVENCOLDFIRE, // fuoco di ghiaccio
	CHARGE_HEAVENWIND,
	CHARGE_HEAVENRAIN,
	CHARGE_HEAVENSNOW,
	CHARGE_HEAVENHAILSTORM,
	CHARGE_HEAVENGEYSER,
	CHARGE_HEAVENBELL,
	CHARGE_HEAVENMIST,
	CHARGE_HEAVENDEW,
	CHARGE_HEAVENAIRMETAL,// mercurio 
	CHARGE_HEAVENBREATH,
	CHARGE_HEAVENFROST,
	CHARGE_HEAVENAIRPOISON, 
	CHARGE_HEAVENTRUTH,
	CHARGE_HEAVENHOAX,
	// Metallo
	CHARGE_HEAVENMETAL,
	CHARGE_HEAVENOPAQUE,
	CHARGE_HEAVENGLOW,
	CHARGE_HEAVENPLASMA,
	CHARGE_HEAVENCLOUD,
	CHARGE_HEAVENACID,
	CHARGE_HEAVENALACAL,
	CHARGE_HEAVENGLASS,
	CHARGE_HEAVENVIBRATION,
	CHARGE_HEAVENSUPERFLUID,
	CHARGE_HEAVENSUPERCONDUCTOR,
	CHARGE_HEAVENBIRD,
	CHARGE_HEAVENSUPERMETAL,//platino
	CHARGE_HEAVENMETALPOISON, // uranio 
	CHARGE_HEAVENHOLOGRAM,
	CHARGE_HEAVENSYNC,
	CHARGE_HEAVENVOID,
	CHARGE_HEAVENSHP,
	// Acqua
	CHARGE_HEAVENWATER,
	CHARGE_HEAVENINK,
	CHARGE_HEAVENPEARL,
	CHARGE_HEAVENSEASHELL,
	CHARGE_HEAVENCORAL,
	CHARGE_HEAVENNHN,
	CHARGE_HEAVENICE,
	CHARGE_HEAVENFOG,
	CHARGE_HEAVENMUD,
	CHARGE_HEAVENECHO,
	CHARGE_HEAVENINFRASONIC,
	CHARGE_HEAVENULTRASONIC,
	CHARGE_HEAVENBUBBLE,
	CHARGE_HEAVENKELP, 
	CHARGE_HEAVENWATERMETAL,
	CHARGE_HEAVENSOUL,
	CHARGE_HEAVENWATERPOISON,
	CHARGE_HEAVENBLESSEDWATER,
	CHARGE_HEAVENTAR,
	// Tenebra
	CHARGE_HEAVENDARK,
	CHARGE_HEAVENMOON,
	CHARGE_HEAVENFIREFLY,
	CHARGE_HEAVENCANDLE,
	CHARGE_HEAVENSWARM,
	CHARGE_HEAVENTEAR,
	CHARGE_HEAVENCAVE,
	CHARGE_HEAVENBUZZ,
	CHARGE_HEAVENMEMORY,
	CHARGE_HEAVENEBONY,
	CHARGE_HEAVENDARKMETAL,
	CHARGE_HEAVENSHADOW,
	CHARGE_HEAVENDARKPOISON,// selenio 
	CHARGE_HEAVENHONEY,
	CHARGE_HEAVENNIGHTMARE, 
	// Veleno
	CHARGE_HEAVENPOISON,
	CHARGE_HEAVENTOXIN, 
	CHARGE_HEAVENVENOM, 
	CHARGE_HEAVENBLACKHOLE,
	CHARGE_HEAVENFALLOUT,
	CHARGE_HEAVENNUKE,
	CHARGE_HEAVENBAT,
	CHARGE_HEAVENOIL,
	CHARGE_HEAVENFREEZE,
	CHARGE_HEAVENFRACAS,
	CHARGE_HEAVENMUTE,
	CHARGE_HEAVENPUMPKIN,
	CHARGE_HEAVENWORMWOOD, 
	CHARGE_HEAVENPOISONMETAL,// iridio 
	CHARGE_HEAVENSORCERY,
	CHARGE_HEAVENSUPERPOISON,// arsenico 
	CHARGE_HEAVENEXORCISM,
	CHARGE_HEAVENPLAGUE,
	CHARGE_HEAVENALCHEMIC, 
	CHARGE_HOLY,
	CHARGE_HOLYFIRE, 
	// Light Rays
	CHARGE_HOLYLIGHT,
	// Spirit Rays
	CHARGE_HOLYSPIRIT, 
	// Sound
	CHARGE_HOLYSONIC, 
	// Pensiero
	CHARGE_HOLYPSYCHIC,
	// Terra
	CHARGE_HOLYEARTH,
	// Legno
	CHARGE_HOLYWOOD,
	// Aria
	CHARGE_HOLYAIR,
	// Metallo
	CHARGE_HOLYMETAL,
	// Acqua
	CHARGE_HOLYWATER,
	// Tenebra
	CHARGE_HOLYDARK,
	// Veleno
	CHARGE_HOLYPOISON,
	// Dimensione Divina
	CHARGE_ENERGY,
	CHARGE_LIFE,
	CHARGE_DIMENSION,
	CHARGE_NATURE,
	CHARGE_ORDER, 
	CHARGE_TIME,
	CHARGE_LOVE,
	CHARGE_WORD,
	CHARGE_RANDOM,
	CHARGE_MIND,
	CHARGE_HARMONY, 
	CHARGE_DEATH, 
	// Limbo
	CHARGE_GHOST, 
	CHARGE_GHOSTFIRE,
	CHARGE_GHOSTLIGHT,
	CHARGE_GHOSTSPIRIT,
	CHARGE_GHOSTSONIC, 
	CHARGE_GHOSTPSYCHIC,
	CHARGE_GHOSTEARTH,
	CHARGE_GHOSTWOOD,
	CHARGE_GHOSTAIR,
	CHARGE_GHOSTMETAL,
	CHARGE_GHOSTWATER,
	CHARGE_GHOSTDARK,
	CHARGE_GHOSTPOISON,
	CHARGE_GHOSTHOLY,
	CHARGE_GHOSTNECRO,
	CHARGE_GHOSTACID,
	CHARGE_GHOSTWEB,
	CHARGE_GHOSTMAGIC,
	CHARGE_GHOSTMORY,
	CHARGE_NECROFIRE,
	CHARGE_NECROLIGHT,
	CHARGE_NECROSPIRIT,
	CHARGE_NECROSONIC, 
	CHARGE_NECROPSYCHIC,
	CHARGE_NECROEARTH,
	CHARGE_NECROWOOD,
	CHARGE_NECROAIR,
	CHARGE_NECROMETAL,
	CHARGE_NECROWATER,
	CHARGE_NECRODARK,
	CHARGE_NECROPOISON,
	CHARGE_NECROHOLY,
	CHARGE_NECRO,
	CHARGE_NECROACID,
	CHARGE_NECROWEB,
	CHARGE_NECROMAGIC,
	CHARGE_NECROMORY,
	// Inferi
	CHARGE_HELL, 
	CHARGE_HELLLIGHTNING_CRIMSON,
	CHARGE_HELLLIGHTNING_RED,
	CHARGE_HELLLIGHTNING_ORANGE,
	CHARGE_HELLLIGHTNING_YELLOW, 
	CHARGE_HELLLIGHTNING_BROWN, 
	CHARGE_HELLLIGHTNING_GREEN,
	CHARGE_HELLLIGHTNING_BLUE, 
	CHARGE_HELLLIGHTNING_AZURE, 
	CHARGE_HELLLIGHTNING_CYAN, 
	CHARGE_HELLLIGHTNING_INDIGO,
	CHARGE_HELLLIGHTNING_PURPLE, 
	CHARGE_HELLLIGHTNING_PINK, 
	CHARGE_HELLLIGHTNING_WHITE, 
	CHARGE_HELLLIGHTNING_GREY, 
	CHARGE_HELLLIGHTNING_BLACK, 
	CHARGE_HELLFIRE,
	CHARGE_HELLFIRE_CRIMSON,
	CHARGE_HELLFIRE_RED,
	CHARGE_HELLFIRE_ORANGE,
	CHARGE_HELLFIRE_YELLOW, 
	CHARGE_HELLFIRE_BROWN, 
	CHARGE_HELLFIRE_GREEN,
	CHARGE_HELLFIRE_BLUE, 
	CHARGE_HELLFIRE_AZURE, 
	CHARGE_HELLFIRE_CYAN, 
	CHARGE_HELLFIRE_INDIGO,
	CHARGE_HELLFIRE_PURPLE, 
	CHARGE_HELLFIRE_PINK, 
	CHARGE_HELLFIRE_WHITE, 
	CHARGE_HELLFIRE_IRIS, 
	CHARGE_HELLFIRE_GREY, 
	CHARGE_HELLFIRE_BLACK, 
	CHARGE_HELLSMOKE,
	CHARGE_HELLSPARK, 
	CHARGE_HELLFLAME,
	CHARGE_HELLWARM,
	CHARGE_HELLSTEAM,
	CHARGE_HELLLAVA,
	CHARGE_HELLFIREWORK,
	CHARGE_HELLCONFLICT,
	CHARGE_HELLEMBER,
	CHARGE_HELLTORCH,
	CHARGE_HELLFIREMETAL,
	CHARGE_HELLRUST,
	CHARGE_HELLSTEEL,
	CHARGE_HELLFATUUS, 
	CHARGE_HELLFIREPOISON,
	CHARGE_HELLBLOOD,
	CHARGE_HELLASH,
	// Light Rays
	CHARGE_HELLLIGHT,
	CHARGE_HELLECLIPSE,
	CHARGE_HELLSUN,
	CHARGE_HELLRADIANCE,
	CHARGE_HELLLIGHTNING,
	CHARGE_HELLVAPOR,
	CHARGE_HELLSAND,
	CHARGE_HELLROAR,
	CHARGE_HELLMIRAGE,
	CHARGE_HELLOASIS,
	CHARGE_HELLLIGHTMETAL,
	CHARGE_HELLWRAITH,
	CHARGE_HELLFLASH,
	CHARGE_HELLLIGHTPOISON,  // magnesium
	CHARGE_HELLIRIS, 
	CHARGE_HELLMUMMY, 
	// Spirit Rays
	CHARGE_HELLSPIRIT, 
	CHARGE_HELLSPACE, 
	CHARGE_HELLTHUNDER,
	CHARGE_HELLEXPLOSION,
	CHARGE_HELLBANG,
	CHARGE_HELLTIDE,
	CHARGE_HELLFORCE,
	CHARGE_HELLVOICE,
	CHARGE_HELLSING,
	CHARGE_HELLHYMN,
	CHARGE_HELLGRASS,
	CHARGE_HELLSPIRITMETAL,// zinco 
	CHARGE_HELLVISION,
	CHARGE_HELLSPIRITPOISON,// fosforo 
	CHARGE_HELLAMBROSIA,
	CHARGE_HELLICORE, 
	// Sound
	CHARGE_HELLSONIC, 
	CHARGE_HELLINSTINCT,
	CHARGE_HELLLOGIC, 
	CHARGE_HELLBLAZE,
	CHARGE_HELLTORNADO,
	CHARGE_HELLTSUNAMI,
	CHARGE_HELLQUAKE,
	CHARGE_HELLREVERB,
	CHARGE_HELLAURA,
	CHARGE_HELLWHISTLE,
	CHARGE_HELLMUSIC,
	CHARGE_HELLSONICMETAL,
	CHARGE_HELLSONICPOISON,// cloro 
	CHARGE_HELLSCREAM, 
	CHARGE_HELLSPIDERWEB,
	CHARGE_HELLPEACE,
	CHARGE_HELLVOODOO,
	// Pensiero
	CHARGE_HELLPSYCHIC,
	CHARGE_HELLBLIND,
	CHARGE_HELLJOY,
	CHARGE_HELLRAGE,
	CHARGE_HELLCONFUSION,
	CHARGE_HELLSAD,
	CHARGE_HELLAPATHY,
	CHARGE_HELLNOISE,
	CHARGE_HELLPOSSESSION,
	CHARGE_HELLCHARMING,
	CHARGE_HELLSHOCK,
	CHARGE_HELLFOCUS,
	CHARGE_HELLSLEEP,
	CHARGE_HELLHEAL,
	CHARGE_HELLFEAR,
	CHARGE_HELLPSYCHICMETAL, // palladio
	CHARGE_HELLPSYCHICPOISON, // dendrite 
	// Terra
	CHARGE_HELLEARTH,
	CHARGE_HELLOSSIDIAN,
	CHARGE_HELLCRYSTAL,
	CHARGE_HELLMETEOR, 
	CHARGE_HELLDRY, 
	CHARGE_HELLDUST, 
	CHARGE_HELLCOLD, 
	CHARGE_HELLJADE, 
	CHARGE_HELLSTONE,
	CHARGE_HELLBREAK,
	CHARGE_HELLSANDSTONE,
	CHARGE_HELLCOAL,
	CHARGE_HELLEARTHMETAL,// piombo
	CHARGE_HELLMAGIC,
	CHARGE_HELLEARTHPOISON,
	CHARGE_HELLMILK,
	CHARGE_HELLBONE,
	// Legno
	CHARGE_HELLWOOD,
	CHARGE_HELLSPORE,
	CHARGE_HELLMUSHROOM,
	CHARGE_HELLLEAF,
	CHARGE_HELLRESIN,
	CHARGE_HELLSMELL,
	CHARGE_HELLSTINK,
	CHARGE_HELLLYMPH, 
	CHARGE_HELLROOT,
	CHARGE_HELLBARK,
	CHARGE_HELLGUM,
	CHARGE_HELLNUT,
	CHARGE_HELLQUILL,
	CHARGE_HELLTHORN,
	CHARGE_HELLBRAMBLE,
	CHARGE_HELLSTING,
	CHARGE_HELLWOODMETAL,// alluminio 
	CHARGE_HELLWOODPOISON, 
	CHARGE_HELLDRYAD,
	CHARGE_HELLLATEX,
	CHARGE_HELLPOLLEN,
	CHARGE_HELLSEED,
	CHARGE_HELLFLOWER,
	CHARGE_HELLFRUIT,
	CHARGE_HELLAMBER,
	// Aria
	CHARGE_HELLAIR,
	CHARGE_HELLVACUUM,
	CHARGE_HELLSTORM,
	CHARGE_HELLMETHANE,
	CHARGE_HELLCOLDFIRE, // fuoco di ghiaccio
	CHARGE_HELLWIND,
	CHARGE_HELLRAIN,
	CHARGE_HELLSNOW,
	CHARGE_HELLHAILSTORM,
	CHARGE_HELLGEYSER,
	CHARGE_HELLBELL,
	CHARGE_HELLMIST,
	CHARGE_HELLDEW,
	CHARGE_HELLAIRMETAL,// mercurio 
	CHARGE_HELLBREATH,
	CHARGE_HELLFROST,
	CHARGE_HELLAIRPOISON, 
	CHARGE_HELLTRUTH,
	CHARGE_HELLHOAX,
	// Metallo
	CHARGE_HELLMETAL,
	CHARGE_HELLOPAQUE,
	CHARGE_HELLGLOW,
	CHARGE_HELLPLASMA,
	CHARGE_HELLCLOUD,
	CHARGE_HELLACID,
	CHARGE_HELLALACAL,
	CHARGE_HELLGLASS,
	CHARGE_HELLVIBRATION,
	CHARGE_HELLSUPERFLUID,
	CHARGE_HELLSUPERCONDUCTOR,
	CHARGE_HELLBIRD,
	CHARGE_HELLSUPERMETAL,//platino
	CHARGE_HELLMETALPOISON, // uranio 
	CHARGE_HELLHOLOGRAM,
	CHARGE_HELLSYNC,
	CHARGE_HELLVOID,
	CHARGE_HELLSHP,
	// Acqua
	CHARGE_HELLWATER,
	CHARGE_HELLINK,
	CHARGE_HELLPEARL,
	CHARGE_HELLSEASHELL,
	CHARGE_HELLCORAL,
	CHARGE_HELLNHN,
	CHARGE_HELLICE,
	CHARGE_HELLFOG,
	CHARGE_HELLMUD,
	CHARGE_HELLECHO,
	CHARGE_HELLINFRASONIC,
	CHARGE_HELLULTRASONIC,
	CHARGE_HELLBUBBLE,
	CHARGE_HELLKELP, 
	CHARGE_HELLWATERMETAL,
	CHARGE_HELLSOUL,
	CHARGE_HELLWATERPOISON,
	CHARGE_HELLBLESSEDWATER,
	CHARGE_HELLTAR,
	// Tenebra
	CHARGE_HELLDARK,
	CHARGE_HELLMOON,
	CHARGE_HELLFIREFLY,
	CHARGE_HELLCANDLE,
	CHARGE_HELLSWARM,
	CHARGE_HELLTEAR,
	CHARGE_HELLCAVE,
	CHARGE_HELLBUZZ,
	CHARGE_HELLMEMORY,
	CHARGE_HELLEBONY,
	CHARGE_HELLDARKMETAL,
	CHARGE_HELLSHADOW,
	CHARGE_HELLDARKPOISON,// selenio 
	CHARGE_HELLHONEY,
	CHARGE_HELLNIGHTMARE, 
	// Veleno
	CHARGE_HELLPOISON,
	CHARGE_HELLTOXIN, 
	CHARGE_HELLVENOM, 
	CHARGE_HELLBLACKHOLE,
	CHARGE_HELLFALLOUT,
	CHARGE_HELLNUKE,
	CHARGE_HELLBAT,
	CHARGE_HELLOIL,
	CHARGE_HELLFREEZE,
	CHARGE_HELLFRACAS,
	CHARGE_HELLMUTE,
	CHARGE_HELLPUMPKIN,
	CHARGE_HELLWORMWOOD, 
	CHARGE_HELLPOISONMETAL,// iridio 
	CHARGE_HELLSORCERY,
	CHARGE_HELLSUPERPOISON,// arsenico 
	CHARGE_HELLEXORCISM,
	CHARGE_HELLPLAGUE,
	CHARGE_HELLALCHEMIC, 
	// Dimensione Abissale
	CHARGE_NAAMLOS,
	CHARGE_NAAMLOS1,
	CHARGE_NAAMLOS2,
	CHARGE_NAAMLOS3,
	CHARGE_NAAMLOS4,
	CHARGE_NAAMLOS5,
	CHARGE_NAAMLOS6,
	CHARGE_NAAMLOS7,
	CHARGE_NAAMLOS8,
	CHARGE_NAAMLOS9,
	CHARGE_NAAMLOS10,
	CHARGE_NAAMLOS11,
	CHARGE_NAAMLOS12,
	CHARGE_NAAMLOS13,
	CHARGE_NAAMLOS14,
	CHARGE_NAAMLOS15,
	CHARGE_NAAMLOS16,
	CHARGE_NAAMLOS17,
	CHARGE_NAAMLOS18,
	CHARGE_NAAMLOS19,
	CHARGE_NAAMLOS20,
	CHARGE_NAAMLOS21,
	CHARGE_NAAMLOS22,
	CHARGE_NAAMLOS23,
	CHARGE_NAAMLOS24,
	CHARGE_NAAMLOS25,
	CHARGE_STAGNATION,
	CHARGE_ENTROPY,
	CHARGE_OBLIVION,
	CHARGE_POLLUTION,
	CHARGE_CHAOS,
	CHARGE_NIHILUS,
	CHARGE_HATE,
	CHARGE_LIE,
	CHARGE_TEDIUM,
	CHARGE_MADNESS,
	CHARGE_ABADDON,
	CHARGE_GREEDY,
	CHARGE_EVIL,
	CHARGE_MAX
} ChargeShader_t;
// GUN VISUAL EFFECT AND BALISTIC PARMS 

typedef enum //# raygun_color_e
{
	// Laser beam rays 
	RAY_NONE,
	RAY_CRIMSON,
	RAY_RED,
	RAY_ORANGE,
	RAY_YELLOW, 
	RAY_BROWN, 
	RAY_GREEN,
	RAY_BLUE, 
	RAY_AZURE, 
	RAY_CYAN, 
	RAY_INDIGO,
	RAY_PURPLE, 
	RAY_PINK, 
	RAY_WHITE, 
	RAY_GREY, 
	RAY_BLACK, 
	// electric rays
	RAY_ELECTRIC_CRIMSON,
	RAY_ELECTRIC_RED,
	RAY_ELECTRIC_ORANGE,
	RAY_ELECTRIC_YELLOW, 
	RAY_ELECTRIC_BROWN, 
	RAY_ELECTRIC_GREEN,
	RAY_ELECTRIC_BLUE, 
	RAY_ELECTRIC_AZURE, 
	RAY_ELECTRIC_CYAN, 
	RAY_ELECTRIC_INDIGO,
	RAY_ELECTRIC_PURPLE, 
	RAY_ELECTRIC_PINK, 
	RAY_ELECTRIC_WHITE, 
	RAY_ELECTRIC_GREY, 
	RAY_ELECTRIC_BLACK, 
	// Raggi Onirici
	RAY_DREAM,
	RAY_DREAMFIRE,
	RAY_DREAMLIGHT,
	RAY_DREAMSPIRIT,
	RAY_DREAMSONIC, 
	RAY_DREAMPSYCHIC,
	RAY_DREAMEARTH,
	RAY_DREAMWOOD,
	RAY_DREAMAIR,
	RAY_DREAMMETAL,
	RAY_DREAMWATER,
	RAY_DREAMDARK,
	RAY_DREAMPOISON,
	// Elemental Rays
	RAY_CREATION,
	RAY_DESTRUCTION,
	RAY_FIRE,
	RAY_FIRE_CRIMSON,
	RAY_FIRE_RED,
	RAY_FIRE_ORANGE,
	RAY_FIRE_YELLOW, 
	RAY_FIRE_BROWN, 
	RAY_FIRE_GREEN,
	RAY_FIRE_BLUE, 
	RAY_FIRE_AZURE, 
	RAY_FIRE_CYAN, 
	RAY_FIRE_INDIGO,
	RAY_FIRE_PURPLE, 
	RAY_FIRE_PINK, 
	RAY_FIRE_WHITE, 
	RAY_FIRE_IRIS, 
	RAY_FIRE_GREY, 
	RAY_FIRE_BLACK, 
	RAY_SMOKE,
	RAY_SPARK, 
	RAY_FLAME,
	RAY_WARM,
	RAY_STEAM,
	RAY_LAVA,
	RAY_FIREWORK,
	RAY_CONFLICT,
	RAY_EMBER,
	RAY_TORCH,
	RAY_FIREMETAL,
	RAY_RUST,
	RAY_STEEL,
	RAY_FATUUS, 
	RAY_FIREPOISON,
	RAY_BLOOD,
	RAY_ASH,
	// Light Rays
	RAY_LIGHT,
	RAY_ECLIPSE,
	RAY_SUN,
	RAY_RADIANCE,
	RAY_LIGHTNING,
	RAY_VAPOR,
	RAY_SAND,
	RAY_ROAR,
	RAY_MIRAGE,
	RAY_OASIS,
	RAY_LIGHTMETAL,
	RAY_WRAITH,
	RAY_FLASH,
	RAY_LIGHTPOISON,  // magnesium
	RAY_IRIS, 
	RAY_MUMMY, 
	// Spirit Rays
	RAY_SPIRIT, 
	RAY_SPACE, 
	RAY_THUNDER,
	RAY_EXPLOSION,
	RAY_BANG,
	RAY_TIDE,
	RAY_FORCE,
	RAY_VOICE,
	RAY_SING,
	RAY_HYMN,
	RAY_GRASS,
	RAY_SPIRITMETAL,// zinco 
	RAY_VISION,
	RAY_SPIRITPOISON,// fosforo 
	RAY_AMBROSIA,
	RAY_ICORE, 
	// Sound
	RAY_SONIC, 
	RAY_INSTINCT,
	RAY_LOGIC, 
	RAY_BLAZE,
	RAY_TORNADO,
	RAY_TSUNAMI,
	RAY_QUAKE,
	RAY_REVERB,
	RAY_AURA,
	RAY_WHISTLE,
	RAY_MUSIC,
	RAY_SONICMETAL,
	RAY_SONICPOISON,// cloro 
	RAY_SCREAM, 
	RAY_SPIDERWEB,
	RAY_PEACE,
	RAY_VOODOO,
	// Pensiero
	RAY_PSYCHIC,
	RAY_BLIND,
	RAY_JOY,
	RAY_RAGE,
	RAY_CONFUSION,
	RAY_SAD,
	RAY_APATHY,
	RAY_NOISE,
	RAY_POSSESSION,
	RAY_CHARMING,
	RAY_SHOCK,
	RAY_FOCUS,
	RAY_SLEEP,
	RAY_HEAL,
	RAY_FEAR,
	RAY_PSYCHICMETAL, // palladio
	RAY_PSYCHICPOISON, // dendrite 
	// Terra
	RAY_EARTH,
	RAY_OSSIDIAN,
	RAY_CRYSTAL,
	RAY_METEOR, 
	RAY_DRY, 
	RAY_DUST, 
	RAY_COLD, 
	RAY_JADE, 
	RAY_STONE,
	RAY_BREAK,
	RAY_SANDSTONE,
	RAY_COAL,
	RAY_EARTHMETAL,// piombo
	RAY_MAGIC,
	RAY_EARTHPOISON,
	RAY_MILK,
	RAY_BONE,
	// Legno
	RAY_WOOD,
	RAY_SPORE,
	RAY_MUSHROOM,
	RAY_LEAF,
	RAY_RESIN,
	RAY_SMELL,
	RAY_STINK,
	RAY_LYMPH, 
	RAY_ROOT,
	RAY_BARK,
	RAY_GUM,
	RAY_NUT,
	RAY_QUILL,
	RAY_THORN,
	RAY_BRAMBLE,
	RAY_STING,
	RAY_WOODMETAL,// alluminio 
	RAY_WOODPOISON, 
	RAY_DRYAD,
	RAY_LATEX,
	RAY_POLLEN,
	RAY_SEED,
	RAY_FLOWER,
	RAY_FRUIT,
	RAY_AMBER,
	// Aria
	RAY_AIR,
	RAY_VACUUM,
	RAY_STORM,
	RAY_METHANE,
	RAY_COLDFIRE, // fuoco di ghiaccio
	RAY_WIND,
	RAY_RAIN,
	RAY_SNOW,
	RAY_HAILSTORM,
	RAY_GEYSER,
	RAY_BELL,
	RAY_MIST,
	RAY_DEW,
	RAY_AIRMETAL,// mercurio 
	RAY_BREATH,
	RAY_FROST,
	RAY_AIRPOISON, 
	RAY_TRUTH,
	RAY_HOAX,
	// Metallo
	RAY_METAL,
	RAY_OPAQUE,
	RAY_GLOW,
	RAY_PLASMA,
	RAY_CLOUD,
	RAY_ACID,
	RAY_ALACAL,
	RAY_GLASS,
	RAY_VIBRATION,
	RAY_SUPERFLUID,
	RAY_SUPERCONDUCTOR,
	RAY_BIRD,
	RAY_SUPERMETAL,//platino
	RAY_METALPOISON, // uranio 
	RAY_HOLOGRAM,
	RAY_SYNC,
	RAY_VOID,
	RAY_SHP,
	// Acqua
	RAY_WATER,
	RAY_INK,
	RAY_PEARL,
	RAY_SEASHELL,
	RAY_CORAL,
	RAY_NHN,
	RAY_ICE,
	RAY_FOG,
	RAY_MUD,
	RAY_ECHO,
	RAY_INFRASONIC,
	RAY_ULTRASONIC,
	RAY_BUBBLE,
	RAY_KELP, 
	RAY_WATERMETAL,
	RAY_SOUL,
	RAY_WATERPOISON,
	RAY_BLESSEDWATER,
	RAY_TAR,
	// Tenebra
	RAY_DARK,
	RAY_MOON,
	RAY_FIREFLY,
	RAY_CANDLE,
	RAY_SWARM,
	RAY_TEAR,
	RAY_CAVE,
	RAY_BUZZ,
	RAY_MEMORY,
	RAY_EBONY,
	RAY_DARKMETAL,
	RAY_SHADOW,
	RAY_DARKPOISON,// selenio 
	RAY_HONEY,
	RAY_NIGHTMARE, 
	// Veleno
	RAY_POISON,
	RAY_TOXIN, 
	RAY_VENOM, 
	RAY_BLACKHOLE,
	RAY_FALLOUT,
	RAY_NUKE,
	RAY_BAT,
	RAY_OIL,
	RAY_FREEZE,
	RAY_FRACAS,
	RAY_MUTE,
	RAY_PUMPKIN,
	RAY_WORMWOOD, 
	RAY_POISONMETAL,// iridio 
	RAY_SORCERY,
	RAY_SUPERPOISON,// arsenico 
	RAY_EXORCISM,
	RAY_PLAGUE,
	RAY_ALCHEMIC, 
	// Preesistenza
	RAY_SOULFIRE,
	RAY_SOULLIGHT,
	RAY_SOULSPIRIT,
	RAY_SOULSONIC, 
	RAY_SOULPSYCHIC,
	RAY_SOULEARTH,
	RAY_SOULWOOD,
	RAY_SOULAIR,
	RAY_SOULMETAL,
	RAY_SOULWATER,
	RAY_SOULDARK,
	RAY_SOULPOISON,
	RAY_SOULCREATION,
	RAY_SOULDESTRUCTION,
	RAY_SOULTIME,
	// Siderale
	RAY_ASTRAL,
	RAY_ASTRALFIRE,
	RAY_ASTRALLIGHT,
	RAY_ASTRALSPIRIT,
	RAY_ASTRALSONIC, 
	RAY_ASTRALPSYCHIC,
	RAY_ASTRALEARTH,
	RAY_ASTRALWOOD,
	RAY_ASTRALAIR,
	RAY_ASTRALMETAL,
	RAY_ASTRALWATER,
	RAY_ASTRALDARK,
	RAY_ASTRALPOISON,
	RAY_ASTRALHOLY,
	RAY_ASTRALNECRO,
	RAY_ASTRALCREATION,
	RAY_ASTRALDESTRUCTION,
	RAY_ASTRALMAGIC,
	// Temporale
	RAY_TIMEFIRE,
	RAY_TIMELIGHT,
	RAY_TIMESPIRIT,
	RAY_TIMESONIC, 
	RAY_TIMEPSYCHIC,
	RAY_TIMEEARTH,
	RAY_TIMEWOOD,
	RAY_TIMEAIR,
	RAY_TIMEMETAL,
	RAY_TIMEWATER,
	RAY_TIMEDARK,
	RAY_TIMEPOISON,
	// Iperspazio 
	RAY_HYPERSPACE, 
	RAY_GRAVITY,
	RAY_PHOTON,
	RAY_GLUON,
	RAY_BOSON,
	RAY_ANTIGRAVITY,
	RAY_QUARK,
	RAY_PROTON,
	RAY_NEUTRON,
	RAY_ELECTRON,
	RAY_NEUTRIN,
	// Heaven
	RAY_HEAVEN, 
	RAY_HEAVENLIGHTNING_CRIMSON,
	RAY_HEAVENLIGHTNING_RED,
	RAY_HEAVENLIGHTNING_ORANGE,
	RAY_HEAVENLIGHTNING_YELLOW, 
	RAY_HEAVENLIGHTNING_BROWN, 
	RAY_HEAVENLIGHTNING_GREEN,
	RAY_HEAVENLIGHTNING_BLUE, 
	RAY_HEAVENLIGHTNING_AZURE, 
	RAY_HEAVENLIGHTNING_CYAN, 
	RAY_HEAVENLIGHTNING_INDIGO,
	RAY_HEAVENLIGHTNING_PURPLE, 
	RAY_HEAVENLIGHTNING_PINK, 
	RAY_HEAVENLIGHTNING_WHITE, 
	RAY_HEAVENLIGHTNING_GREY, 
	RAY_HEAVENLIGHTNING_BLACK, 
	RAY_HEAVENFIRE,
	RAY_HEAVENFIRE_CRIMSON,
	RAY_HEAVENFIRE_RED,
	RAY_HEAVENFIRE_ORANGE,
	RAY_HEAVENFIRE_YELLOW, 
	RAY_HEAVENFIRE_BROWN, 
	RAY_HEAVENFIRE_GREEN,
	RAY_HEAVENFIRE_BLUE, 
	RAY_HEAVENFIRE_AZURE, 
	RAY_HEAVENFIRE_CYAN, 
	RAY_HEAVENFIRE_INDIGO,
	RAY_HEAVENFIRE_PURPLE, 
	RAY_HEAVENFIRE_PINK, 
	RAY_HEAVENFIRE_WHITE, 
	RAY_HEAVENFIRE_IRIS, 
	RAY_HEAVENFIRE_GREY, 
	RAY_HEAVENFIRE_BLACK, 
	RAY_HEAVENSMOKE,
	RAY_HEAVENSPARK, 
	RAY_HEAVENFLAME,
	RAY_HEAVENWARM,
	RAY_HEAVENSTEAM,
	RAY_HEAVENLAVA,
	RAY_HEAVENFIREWORK,
	RAY_HEAVENCONFLICT,
	RAY_HEAVENEMBER,
	RAY_HEAVENTORCH,
	RAY_HEAVENFIREMETAL,
	RAY_HEAVENRUST,
	RAY_HEAVENSTEEL,
	RAY_HEAVENFATUUS, 
	RAY_HEAVENFIREPOISON,
	RAY_HEAVENBLOOD,
	RAY_HEAVENASH,
	// Light Rays
	RAY_HEAVENLIGHT,
	RAY_HEAVENECLIPSE,
	RAY_HEAVENSUN,
	RAY_HEAVENRADIANCE,
	RAY_HEAVENLIGHTNING,
	RAY_HEAVENVAPOR,
	RAY_HEAVENSAND,
	RAY_HEAVENROAR,
	RAY_HEAVENMIRAGE,
	RAY_HEAVENOASIS,
	RAY_HEAVENLIGHTMETAL,
	RAY_HEAVENWRAITH,
	RAY_HEAVENFLASH,
	RAY_HEAVENLIGHTPOISON,  // magnesium
	RAY_HEAVENIRIS, 
	RAY_HEAVENMUMMY, 
	// Spirit Rays
	RAY_HEAVENSPIRIT, 
	RAY_HEAVENSPACE, 
	RAY_HEAVENTHUNDER,
	RAY_HEAVENEXPLOSION,
	RAY_HEAVENBANG,
	RAY_HEAVENTIDE,
	RAY_HEAVENFORCE,
	RAY_HEAVENVOICE,
	RAY_HEAVENSING,
	RAY_HEAVENHYMN,
	RAY_HEAVENGRASS,
	RAY_HEAVENSPIRITMETAL,// zinco 
	RAY_HEAVENVISION,
	RAY_HEAVENSPIRITPOISON,// fosforo 
	RAY_HEAVENAMBROSIA,
	RAY_HEAVENICORE, 
	// Sound
	RAY_HEAVENSONIC, 
	RAY_HEAVENINSTINCT,
	RAY_HEAVENLOGIC, 
	RAY_HEAVENBLAZE,
	RAY_HEAVENTORNADO,
	RAY_HEAVENTSUNAMI,
	RAY_HEAVENQUAKE,
	RAY_HEAVENREVERB,
	RAY_HEAVENAURA,
	RAY_HEAVENWHISTLE,
	RAY_HEAVENMUSIC,
	RAY_HEAVENSONICMETAL,
	RAY_HEAVENSONICPOISON,// cloro 
	RAY_HEAVENSCREAM, 
	RAY_HEAVENSPIDERWEB,
	RAY_HEAVENPEACE,
	RAY_HEAVENVOODOO,
	// Pensiero
	RAY_HEAVENPSYCHIC,
	RAY_HEAVENBLIND,
	RAY_HEAVENJOY,
	RAY_HEAVENRAGE,
	RAY_HEAVENCONFUSION,
	RAY_HEAVENSAD,
	RAY_HEAVENAPATHY,
	RAY_HEAVENNOISE,
	RAY_HEAVENPOSSESSION,
	RAY_HEAVENCHARMING,
	RAY_HEAVENSHOCK,
	RAY_HEAVENFOCUS,
	RAY_HEAVENSLEEP,
	RAY_HEAVENHEAL,
	RAY_HEAVENFEAR,
	RAY_HEAVENPSYCHICMETAL, // palladio
	RAY_HEAVENPSYCHICPOISON, // dendrite 
	// Terra
	RAY_HEAVENEARTH,
	RAY_HEAVENOSSIDIAN,
	RAY_HEAVENCRYSTAL,
	RAY_HEAVENMETEOR, 
	RAY_HEAVENDRY, 
	RAY_HEAVENDUST, 
	RAY_HEAVENCOLD, 
	RAY_HEAVENJADE, 
	RAY_HEAVENSTONE,
	RAY_HEAVENBREAK,
	RAY_HEAVENSANDSTONE,
	RAY_HEAVENCOAL,
	RAY_HEAVENEARTHMETAL,// piombo
	RAY_HEAVENMAGIC,
	RAY_HEAVENEARTHPOISON,
	RAY_HEAVENMILK,
	RAY_HEAVENBONE,
	// Legno
	RAY_HEAVENWOOD,
	RAY_HEAVENSPORE,
	RAY_HEAVENMUSHROOM,
	RAY_HEAVENLEAF,
	RAY_HEAVENRESIN,
	RAY_HEAVENSMELL,
	RAY_HEAVENSTINK,
	RAY_HEAVENLYMPH, 
	RAY_HEAVENROOT,
	RAY_HEAVENBARK,
	RAY_HEAVENGUM,
	RAY_HEAVENNUT,
	RAY_HEAVENQUILL,
	RAY_HEAVENTHORN,
	RAY_HEAVENBRAMBLE,
	RAY_HEAVENSTING,
	RAY_HEAVENWOODMETAL,// alluminio 
	RAY_HEAVENWOODPOISON, 
	RAY_HEAVENDRYAD,
	RAY_HEAVENLATEX,
	RAY_HEAVENPOLLEN,
	RAY_HEAVENSEED,
	RAY_HEAVENFLOWER,
	RAY_HEAVENFRUIT,
	RAY_HEAVENAMBER,
	// Aria
	RAY_HEAVENAIR,
	RAY_HEAVENVACUUM,
	RAY_HEAVENSTORM,
	RAY_HEAVENMETHANE,
	RAY_HEAVENCOLDFIRE, // fuoco di ghiaccio
	RAY_HEAVENWIND,
	RAY_HEAVENRAIN,
	RAY_HEAVENSNOW,
	RAY_HEAVENHAILSTORM,
	RAY_HEAVENGEYSER,
	RAY_HEAVENBELL,
	RAY_HEAVENMIST,
	RAY_HEAVENDEW,
	RAY_HEAVENAIRMETAL,// mercurio 
	RAY_HEAVENBREATH,
	RAY_HEAVENFROST,
	RAY_HEAVENAIRPOISON, 
	RAY_HEAVENTRUTH,
	RAY_HEAVENHOAX,
	// Metallo
	RAY_HEAVENMETAL,
	RAY_HEAVENOPAQUE,
	RAY_HEAVENGLOW,
	RAY_HEAVENPLASMA,
	RAY_HEAVENCLOUD,
	RAY_HEAVENACID,
	RAY_HEAVENALACAL,
	RAY_HEAVENGLASS,
	RAY_HEAVENVIBRATION,
	RAY_HEAVENSUPERFLUID,
	RAY_HEAVENSUPERCONDUCTOR,
	RAY_HEAVENBIRD,
	RAY_HEAVENSUPERMETAL,//platino
	RAY_HEAVENMETALPOISON, // uranio 
	RAY_HEAVENHOLOGRAM,
	RAY_HEAVENSYNC,
	RAY_HEAVENVOID,
	RAY_HEAVENSHP,
	// Acqua
	RAY_HEAVENWATER,
	RAY_HEAVENINK,
	RAY_HEAVENPEARL,
	RAY_HEAVENSEASHELL,
	RAY_HEAVENCORAL,
	RAY_HEAVENNHN,
	RAY_HEAVENICE,
	RAY_HEAVENFOG,
	RAY_HEAVENMUD,
	RAY_HEAVENECHO,
	RAY_HEAVENINFRASONIC,
	RAY_HEAVENULTRASONIC,
	RAY_HEAVENBUBBLE,
	RAY_HEAVENKELP, 
	RAY_HEAVENWATERMETAL,
	RAY_HEAVENSOUL,
	RAY_HEAVENWATERPOISON,
	RAY_HEAVENBLESSEDWATER,
	RAY_HEAVENTAR,
	// Tenebra
	RAY_HEAVENDARK,
	RAY_HEAVENMOON,
	RAY_HEAVENFIREFLY,
	RAY_HEAVENCANDLE,
	RAY_HEAVENSWARM,
	RAY_HEAVENTEAR,
	RAY_HEAVENCAVE,
	RAY_HEAVENBUZZ,
	RAY_HEAVENMEMORY,
	RAY_HEAVENEBONY,
	RAY_HEAVENDARKMETAL,
	RAY_HEAVENSHADOW,
	RAY_HEAVENDARKPOISON,// selenio 
	RAY_HEAVENHONEY,
	RAY_HEAVENNIGHTMARE, 
	// Veleno
	RAY_HEAVENPOISON,
	RAY_HEAVENTOXIN, 
	RAY_HEAVENVENOM, 
	RAY_HEAVENBLACKHOLE,
	RAY_HEAVENFALLOUT,
	RAY_HEAVENNUKE,
	RAY_HEAVENBAT,
	RAY_HEAVENOIL,
	RAY_HEAVENFREEZE,
	RAY_HEAVENFRACAS,
	RAY_HEAVENMUTE,
	RAY_HEAVENPUMPKIN,
	RAY_HEAVENWORMWOOD, 
	RAY_HEAVENPOISONMETAL,// iridio 
	RAY_HEAVENSORCERY,
	RAY_HEAVENSUPERPOISON,// arsenico 
	RAY_HEAVENEXORCISM,
	RAY_HEAVENPLAGUE,
	RAY_HEAVENALCHEMIC, 
	RAY_HOLY,
	RAY_HOLYFIRE, 
	// Light Rays
	RAY_HOLYLIGHT,
	// Spirit Rays
	RAY_HOLYSPIRIT, 
	// Sound
	RAY_HOLYSONIC, 
	// Pensiero
	RAY_HOLYPSYCHIC,
	// Terra
	RAY_HOLYEARTH,
	// Legno
	RAY_HOLYWOOD,
	// Aria
	RAY_HOLYAIR,
	// Metallo
	RAY_HOLYMETAL,
	// Acqua
	RAY_HOLYWATER,
	// Tenebra
	RAY_HOLYDARK,
	// Veleno
	RAY_HOLYPOISON,
	// Dimensione Divina
	RAY_ENERGY,
	RAY_LIFE,
	RAY_DIMENSION,
	RAY_NATURE,
	RAY_ORDER, 
	RAY_TIME,
	RAY_LOVE,
	RAY_WORD,
	RAY_RANDOM,
	RAY_MIND,
	RAY_HARMONY, 
	RAY_DEATH, 
	// Limbo
	RAY_GHOST, 
	RAY_GHOSTFIRE,
	RAY_GHOSTLIGHT,
	RAY_GHOSTSPIRIT,
	RAY_GHOSTSONIC, 
	RAY_GHOSTPSYCHIC,
	RAY_GHOSTEARTH,
	RAY_GHOSTWOOD,
	RAY_GHOSTAIR,
	RAY_GHOSTMETAL,
	RAY_GHOSTWATER,
	RAY_GHOSTDARK,
	RAY_GHOSTPOISON,
	RAY_GHOSTHOLY,
	RAY_GHOSTNECRO,
	RAY_GHOSTACID,
	RAY_GHOSTWEB,
	RAY_GHOSTMAGIC,
	RAY_GHOSTMORY,
	RAY_NECROFIRE,
	RAY_NECROLIGHT,
	RAY_NECROSPIRIT,
	RAY_NECROSONIC, 
	RAY_NECROPSYCHIC,
	RAY_NECROEARTH,
	RAY_NECROWOOD,
	RAY_NECROAIR,
	RAY_NECROMETAL,
	RAY_NECROWATER,
	RAY_NECRODARK,
	RAY_NECROPOISON,
	RAY_NECROHOLY,
	RAY_NECRO,
	RAY_NECROACID,
	RAY_NECROWEB,
	RAY_NECROMAGIC,
	RAY_NECROMORY,
	// Inferi
	RAY_HELL, 
	RAY_HELLLIGHTNING_CRIMSON,
	RAY_HELLLIGHTNING_RED,
	RAY_HELLLIGHTNING_ORANGE,
	RAY_HELLLIGHTNING_YELLOW, 
	RAY_HELLLIGHTNING_BROWN, 
	RAY_HELLLIGHTNING_GREEN,
	RAY_HELLLIGHTNING_BLUE, 
	RAY_HELLLIGHTNING_AZURE, 
	RAY_HELLLIGHTNING_CYAN, 
	RAY_HELLLIGHTNING_INDIGO,
	RAY_HELLLIGHTNING_PURPLE, 
	RAY_HELLLIGHTNING_PINK, 
	RAY_HELLLIGHTNING_WHITE, 
	RAY_HELLLIGHTNING_GREY, 
	RAY_HELLLIGHTNING_BLACK, 
	RAY_HELLFIRE,
	RAY_HELLFIRE_CRIMSON,
	RAY_HELLFIRE_RED,
	RAY_HELLFIRE_ORANGE,
	RAY_HELLFIRE_YELLOW, 
	RAY_HELLFIRE_BROWN, 
	RAY_HELLFIRE_GREEN,
	RAY_HELLFIRE_BLUE, 
	RAY_HELLFIRE_AZURE, 
	RAY_HELLFIRE_CYAN, 
	RAY_HELLFIRE_INDIGO,
	RAY_HELLFIRE_PURPLE, 
	RAY_HELLFIRE_PINK, 
	RAY_HELLFIRE_WHITE, 
	RAY_HELLFIRE_IRIS, 
	RAY_HELLFIRE_GREY, 
	RAY_HELLFIRE_BLACK, 
	RAY_HELLSMOKE,
	RAY_HELLSPARK, 
	RAY_HELLFLAME,
	RAY_HELLWARM,
	RAY_HELLSTEAM,
	RAY_HELLLAVA,
	RAY_HELLFIREWORK,
	RAY_HELLCONFLICT,
	RAY_HELLEMBER,
	RAY_HELLTORCH,
	RAY_HELLFIREMETAL,
	RAY_HELLRUST,
	RAY_HELLSTEEL,
	RAY_HELLFATUUS, 
	RAY_HELLFIREPOISON,
	RAY_HELLBLOOD,
	RAY_HELLASH,
	// Light Rays
	RAY_HELLLIGHT,
	RAY_HELLECLIPSE,
	RAY_HELLSUN,
	RAY_HELLRADIANCE,
	RAY_HELLLIGHTNING,
	RAY_HELLVAPOR,
	RAY_HELLSAND,
	RAY_HELLROAR,
	RAY_HELLMIRAGE,
	RAY_HELLOASIS,
	RAY_HELLLIGHTMETAL,
	RAY_HELLWRAITH,
	RAY_HELLFLASH,
	RAY_HELLLIGHTPOISON,  // magnesium
	RAY_HELLIRIS, 
	RAY_HELLMUMMY, 
	// Spirit Rays
	RAY_HELLSPIRIT, 
	RAY_HELLSPACE, 
	RAY_HELLTHUNDER,
	RAY_HELLEXPLOSION,
	RAY_HELLBANG,
	RAY_HELLTIDE,
	RAY_HELLFORCE,
	RAY_HELLVOICE,
	RAY_HELLSING,
	RAY_HELLHYMN,
	RAY_HELLGRASS,
	RAY_HELLSPIRITMETAL,// zinco 
	RAY_HELLVISION,
	RAY_HELLSPIRITPOISON,// fosforo 
	RAY_HELLAMBROSIA,
	RAY_HELLICORE, 
	// Sound
	RAY_HELLSONIC, 
	RAY_HELLINSTINCT,
	RAY_HELLLOGIC, 
	RAY_HELLBLAZE,
	RAY_HELLTORNADO,
	RAY_HELLTSUNAMI,
	RAY_HELLQUAKE,
	RAY_HELLREVERB,
	RAY_HELLAURA,
	RAY_HELLWHISTLE,
	RAY_HELLMUSIC,
	RAY_HELLSONICMETAL,
	RAY_HELLSONICPOISON,// cloro 
	RAY_HELLSCREAM, 
	RAY_HELLSPIDERWEB,
	RAY_HELLPEACE,
	RAY_HELLVOODOO,
	// Pensiero
	RAY_HELLPSYCHIC,
	RAY_HELLBLIND,
	RAY_HELLJOY,
	RAY_HELLRAGE,
	RAY_HELLCONFUSION,
	RAY_HELLSAD,
	RAY_HELLAPATHY,
	RAY_HELLNOISE,
	RAY_HELLPOSSESSION,
	RAY_HELLCHARMING,
	RAY_HELLSHOCK,
	RAY_HELLFOCUS,
	RAY_HELLSLEEP,
	RAY_HELLHEAL,
	RAY_HELLFEAR,
	RAY_HELLPSYCHICMETAL, // palladio
	RAY_HELLPSYCHICPOISON, // dendrite 
	// Terra
	RAY_HELLEARTH,
	RAY_HELLOSSIDIAN,
	RAY_HELLCRYSTAL,
	RAY_HELLMETEOR, 
	RAY_HELLDRY, 
	RAY_HELLDUST, 
	RAY_HELLCOLD, 
	RAY_HELLJADE, 
	RAY_HELLSTONE,
	RAY_HELLBREAK,
	RAY_HELLSANDSTONE,
	RAY_HELLCOAL,
	RAY_HELLEARTHMETAL,// piombo
	RAY_HELLMAGIC,
	RAY_HELLEARTHPOISON,
	RAY_HELLMILK,
	RAY_HELLBONE,
	// Legno
	RAY_HELLWOOD,
	RAY_HELLSPORE,
	RAY_HELLMUSHROOM,
	RAY_HELLLEAF,
	RAY_HELLRESIN,
	RAY_HELLSMELL,
	RAY_HELLSTINK,
	RAY_HELLLYMPH, 
	RAY_HELLROOT,
	RAY_HELLBARK,
	RAY_HELLGUM,
	RAY_HELLNUT,
	RAY_HELLQUILL,
	RAY_HELLTHORN,
	RAY_HELLBRAMBLE,
	RAY_HELLSTING,
	RAY_HELLWOODMETAL,// alluminio 
	RAY_HELLWOODPOISON, 
	RAY_HELLDRYAD,
	RAY_HELLLATEX,
	RAY_HELLPOLLEN,
	RAY_HELLSEED,
	RAY_HELLFLOWER,
	RAY_HELLFRUIT,
	RAY_HELLAMBER,
	// Aria
	RAY_HELLAIR,
	RAY_HELLVACUUM,
	RAY_HELLSTORM,
	RAY_HELLMETHANE,
	RAY_HELLCOLDFIRE, // fuoco di ghiaccio
	RAY_HELLWIND,
	RAY_HELLRAIN,
	RAY_HELLSNOW,
	RAY_HELLHAILSTORM,
	RAY_HELLGEYSER,
	RAY_HELLBELL,
	RAY_HELLMIST,
	RAY_HELLDEW,
	RAY_HELLAIRMETAL,// mercurio 
	RAY_HELLBREATH,
	RAY_HELLFROST,
	RAY_HELLAIRPOISON, 
	RAY_HELLTRUTH,
	RAY_HELLHOAX,
	// Metallo
	RAY_HELLMETAL,
	RAY_HELLOPAQUE,
	RAY_HELLGLOW,
	RAY_HELLPLASMA,
	RAY_HELLCLOUD,
	RAY_HELLACID,
	RAY_HELLALACAL,
	RAY_HELLGLASS,
	RAY_HELLVIBRATION,
	RAY_HELLSUPERFLUID,
	RAY_HELLSUPERCONDUCTOR,
	RAY_HELLBIRD,
	RAY_HELLSUPERMETAL,//platino
	RAY_HELLMETALPOISON, // uranio 
	RAY_HELLHOLOGRAM,
	RAY_HELLSYNC,
	RAY_HELLVOID,
	RAY_HELLSHP,
	// Acqua
	RAY_HELLWATER,
	RAY_HELLINK,
	RAY_HELLPEARL,
	RAY_HELLSEASHELL,
	RAY_HELLCORAL,
	RAY_HELLNHN,
	RAY_HELLICE,
	RAY_HELLFOG,
	RAY_HELLMUD,
	RAY_HELLECHO,
	RAY_HELLINFRASONIC,
	RAY_HELLULTRASONIC,
	RAY_HELLBUBBLE,
	RAY_HELLKELP, 
	RAY_HELLWATERMETAL,
	RAY_HELLSOUL,
	RAY_HELLWATERPOISON,
	RAY_HELLBLESSEDWATER,
	RAY_HELLTAR,
	// Tenebra
	RAY_HELLDARK,
	RAY_HELLMOON,
	RAY_HELLFIREFLY,
	RAY_HELLCANDLE,
	RAY_HELLSWARM,
	RAY_HELLTEAR,
	RAY_HELLCAVE,
	RAY_HELLBUZZ,
	RAY_HELLMEMORY,
	RAY_HELLEBONY,
	RAY_HELLDARKMETAL,
	RAY_HELLSHADOW,
	RAY_HELLDARKPOISON,// selenio 
	RAY_HELLHONEY,
	RAY_HELLNIGHTMARE, 
	// Veleno
	RAY_HELLPOISON,
	RAY_HELLTOXIN, 
	RAY_HELLVENOM, 
	RAY_HELLBLACKHOLE,
	RAY_HELLFALLOUT,
	RAY_HELLNUKE,
	RAY_HELLBAT,
	RAY_HELLOIL,
	RAY_HELLFREEZE,
	RAY_HELLFRACAS,
	RAY_HELLMUTE,
	RAY_HELLPUMPKIN,
	RAY_HELLWORMWOOD, 
	RAY_HELLPOISONMETAL,// iridio 
	RAY_HELLSORCERY,
	RAY_HELLSUPERPOISON,// arsenico 
	RAY_HELLEXORCISM,
	RAY_HELLPLAGUE,
	RAY_HELLALCHEMIC, 
	// Dimensione Abissale
	RAY_NAAMLOS,
	RAY_NAAMLOS1,
	RAY_NAAMLOS2,
	RAY_NAAMLOS3,
	RAY_NAAMLOS4,
	RAY_NAAMLOS5,
	RAY_NAAMLOS6,
	RAY_NAAMLOS7,
	RAY_NAAMLOS8,
	RAY_NAAMLOS9,
	RAY_NAAMLOS10,
	RAY_NAAMLOS11,
	RAY_NAAMLOS12,
	RAY_NAAMLOS13,
	RAY_NAAMLOS14,
	RAY_NAAMLOS15,
	RAY_NAAMLOS16,
	RAY_NAAMLOS17,
	RAY_NAAMLOS18,
	RAY_NAAMLOS19,
	RAY_NAAMLOS20,
	RAY_NAAMLOS21,
	RAY_NAAMLOS22,
	RAY_NAAMLOS23,
	RAY_NAAMLOS24,
	RAY_NAAMLOS25,
	RAY_STAGNATION,
	RAY_ENTROPY,
	RAY_OBLIVION,
	RAY_POLLUTION,
	RAY_CHAOS,
	RAY_NIHILUS,
	RAY_HATE,
	RAY_LIE,
	RAY_TEDIUM,
	RAY_MADNESS,
	RAY_ABADDON,
	RAY_GREEDY,
	RAY_EVIL,
	RAY_MAX
} RayGun_Colors_t;

typedef struct
{
	char		*Gun_Name;						//entry in .GUN, if any
	char		*Gun_FullName;					//the "Proper Name" of the saber, shown in the UI
	char		*Gun_Model;					// Gun model
	char		*Gun_Skin;					// Gun custom skin
	
	//char	Gun_Classname[MAX_QPATH];	// Spawning name // WAS 32!!! 
	// SHOULD NOT BE NECESSARY. 
	// 3D Gun Appeareance 
	// Sounds - CHARS OR INT???
	char			*Gun_Missile_Model;		// Missile model for main shot
	char			*Gun_Alt_Missile_Model; // Missile model for Alt Shoot. 
	char  			*Gun_Missile_Sound;		// Sound made from missile of weapon, like a rocket loop
	char			*Gun_Alt_Missile_Sound; // Sound made from alt missile of weapon, like a rocket loop
	char			*Gun_Charge_Sound;		// sound to start when the weapon initiates the charging sequence
	char		   	*Gun_Alt_Charge_Sound;	// alt sound to start when the weapon initiates the charging sequence
	char			*Gun_Select_Sound;	// the sound to play when this weapon gets selected
	char			*Gun_Loop_Sound;  // Loop Sound played when weapon equipped
	char			*Gun_Low_Ammo_Sound; // Sound Played when weapon have low ammo bolts
	char			*Gun_NoAmmo_Sound; // Sound Played when ammo are depleted
	// - it was a char. this is a test! 
	weapon_t		Gun_Type;			// the weapon type 
	// Ammo Stuff 
	int		Gun_AmmoMax;		// Max Ammo for this gun
	int		Gun_AmmoIndex;		// Index to proper ammo slot
	int		Gun_AmmoLow;		// Count when ammo is low
	int		Gun_AmmoPerShot;	// Amount of ammo used per shot
	int		Gun_AltAmmoPerShot;	// Amount of ammo used for alt shot
	
	
	
	// BALLISTIC 
	int		Gun_NumBarrels;		// How many barrels (quante canne) have the gun?
	// Ballistic 
	int		Gun_Fire_Time;		// Amount of time between two shoots
	int		Gun_Alt_Fire_Time;	// Amount of time between alt firings shoot. 
	int		Gun_NPC_Fire_Time;	// Amount of time between two shoots for npc
	int		Gun_NPC_Alt_Fire_Time; // Amount of time between two alts shoots for npc
	float	Gun_Speed;			// Velocity of projectile 
	float	Gun_Alt_Speed;		// Velocity of alt projectile 
	int		Gun_Range;			// Range of gun
	int		Gun_Alt_Range;		// Range of gun alt fire
	int		Gun_Life;			// Life of projectile 
	int		Gun_Alt_Life;		// Life of Alt Projectile 
	// BRYAR BALISTIC 
	float Gun_Charge_Unit;		// gun charge unit 
	float Gun_Alt_Charge_Unit; // gun alt charge unit 
	// BLASTER RIFLE
	float		Gun_Spread;		// Amount of Spread of Gun as blaster rifle - repeater
	float		Gun_Alt_Spread;	// Amount of Spread of Alt Fire of Gun as blaster rifle - repeater
	// SNIPER RIFLE 
	float   Gun_ZoomFov;            // Max Main Zoom FOV 
	char	*Gun_ZoomSoundStart;    // Zoom Sound Start of Weapon 
	char	*Gun_ZoomSoundEnd;          // Zoom End Sound of Weapon 
	char	*Gun_ZoomMask;				// Zoom Mask of Gun
	char	*Gun_ZoomRotateMask;		// Rotating Mask of Gun 
	char	*Gun_ZoomTick;			// set shader of ammo counter of mask 
	char	*Gun_ZoomCharge;		// set shader of charge. 
	char	*Gun_Crosshair;			// Set Gun Crosshair 
	// REPEATER 
	float	Gun_Gravity;		// Amount of gravity of projectiles 
	float 	Gun_Alt_Gravity;	// Amount of gravity of alt projectile 
	float	Gun_AntiGravity;	// Amount of Antigravity of Projectile 
	float	Gun_Alt_AntiGravity; // Amount of Antigravity of Alt Projectile 
	// FLECHETTE 
	int		Gun_Num_Shot;		// number of projectiles shooted into the moment
	int		Gun_Alt_Num_Shot;	// number of projectile shooted into the same moment
	int		Gun_Num_Bounce;		// Amount of Bouncing of projectile 
	int		Gun_Alt_Num_Bounce;	// Amount of bouncing of alt projectile 
	int		Gun_Time_BombShot;		// how many second before explosive shoots detonate like alt flechette
	int		Gun_Alt_Time_BombShot;	// how many second before alt explosive shoots detonate like alt flechette
	// STUN BATON 
	int			Gun_ElectrocuteDuration;	// How many time duration of electrocution  
	int			Gun_Alt_ElectrocuteDuration;// How Many Time duration of electrocution for alt fire
	int			Gun_StunTime;			// How many Time Enemy is stunned by the gun  
	int			Gun_Alt_StunTime;		// How many Time enemy is stunned by alt fire gun
	// EXPLOSIVES 
	int			Gun_Time_Grenade_Time;		//  how many time take the grenade before explode 
	int			Gun_Alt_Time_Grenade_Time;	// how many time take the grenade before explode  
	int							Gun_PoisonDamage;	// amount of damage set with poison
	int							Gun_Alt_PoisonDamage; // amount of damage set with alt poison
	int							Gun_PoisonTime;			// amount of time duration of poison
	int							Gun_Alt_PoisonTime;		// amount of time duration of alt poison
	int							Gun_Hit_Animation;		// Set animation played by enemy hitted by gun
	int							Gun_Alt_Hit_Animation;	// Set animation played by enemy hitted by alt fire
//#ifndef _USRDLL
	// Visual effects 
	int 	Gun_Muzzle_Effect; // Main Shoot Muzzle Effect 
	int		Gun_Alt_Muzzle_Effect; // Alt Shoot Muzzle Effect
	int		Gun_Bullet_Effect; // Main Shoot Muzzle Effect 
	int		Gun_Alt_Bullet_Effect; // Alt Shoot Muzzle Effect
    RayGun_Colors_t Gun_Ray_Effect;	// Shader of main gun ray
	RayGun_Colors_t	Gun_Alt_Ray_Effect;	// Shader of alt gun ray
	int		Gun_WallImpact_Effect; // Main Shoot Muzzle Effect 
	int		Gun_Alt_WallImpact_Effect; // Alt Shoot Muzzle Effect
	int		Gun_FleshImpact_Effect; // Main Shoot Muzzle Effect 
	int 	Gun_Alt_FleshImpact_Effect; // Alt Shoot Muzzle Effect
	int	 	Gun_Bounce_Effect; // Main Shoot Muzzle Effect 
	int		Gun_Alt_Bounce_Effect; // Alt Shoot Muzzle Effect
	ChargeShader_t	Gun_Charge_Shader_Effect; // effect draw on barrel when weapon charge the shoot
	ChargeShader_t	Gun_Alt_Charge_Shader_Effect; // effect draw on barrel wehn weapon charge alt shoot. 
	int 	Gun_Disgregate_Effect;	// effect of disintegration
	int 	Gun_Alt_Disgregate_Effect; // alt effect of disintegration
//#endif
	// Damages 
	int		Gun_Damage; // damage of main bullet
	int		Gun_AltDamage; // damage of alt bullet 
	int		Gun_SplashDamage; // damage main aoe 
	int		Gun_AltSplashDamage; // damage alt aoe 
	float	Gun_SplashRadius; // aoe main radius 
	float	Gun_AltSplashRadius; // aoe splash radius 
	float   Gun_SplashKnockback;	// aoe knockback 
	float	Gun_AltSplashKnockback; // aoe alt knockback 
	int		Gun_NPC_Damage_Easy; //easy npc damage 
	int		Gun_NPC_Damage_Medium; // medium npc damage 
	int		Gun_NPC_Damage_Hard; // hard npc damage 
	int		Gun_Alt_NPC_Damage_Easy; // alt easy npc damage 
	int		Gun_Alt_NPC_Damage_Medium; // alt medium npc damage 
	int		Gun_Alt_NPC_Damage_Hard;  // alt hard npc damage 
		
	
	//done in game (server-side code)
	float		GunMoveSpeedScale;				//1.0 - you move faster/slower when using this saber
	float		GunAnimSpeedScale;				//1.0 - plays normal attack animations faster/slower
	//Animation 
	int		Gun_Charger_Anim; // Animation when charge the attack.
	int		Gun_Alt_Charger_Anim; // Animation for alt fire charging.
	int		Gun_ShootAnim;			// shoot animation for main attack
	int		Gun_Alt_ShootAnim;		// shoot animation for alt attack
	int		Gun_ReadyAnim;			// ready animation for main attack
	int		Gun_WalkAnim;			// shoot animation for main attack
	int		Gun_RunAnim;			// run animation for main attack
	int		Gun_DropAnim;			// animation for drop or put away gun
	// Other Stuff 
	int		Gun_MagicRestriction;		//Magic and Skils that cannot be used while this Gun is on (bitfield) 
	//===========================================================================================
	//these values are global to the saber, like all of the ones above
	int			gunFlags;				//from GFL_ list above
	int			gunFlags2;				//from GFL2_ list above
	int			gunFlags3;				//from GFL3_ list above
	int			Gun_MOD;	// Set Means of Damage for Main Attack 
	int			Gun_Alt_MOD; // Set Means of Damage for Alt Attack 
	int			Gun_MOD2; // Set second means of Damage of Main Atk
	int			Gun_Alt_MOD2; // set second means of damage of Main Atk
	int			Gun_MOD3;// Set third means of damage of Main Atk
	int			Gun_Alt_MOD3; // set third means of damage of Alt Attack.
	char		*Gun_Poison_Effect;	// Set Visible effect of poison of enemy
	char		*Gun_Alt_Poison_Effect; // Set Alt Visible Effect of Poison of Enemy
	int			Gun_PoisonAnim; // animation for enemy get poisoned by main fire
	int			Gun_Alt_PoisonAnim; //animation for enemy get poisoned by alt fire 
	char		*Gun_Explosion_Effect; // Set visual efx for explosives. 
	// UI - THEY WAS CHARS 
//	Pistol_t	Gun_Icon;		// Name of weapon icon file
//	Pistol_t	Gun_Desc;		// String Descriprion of Gun
//	Pistol_t	Gun_DisplayName; // The name which is to be displayed on the HUD.
	char		Gun_g2MarksShader[MAX_QPATH];	//none - if set, the game will use this shader for marks on enemies instead of the default "gfx/damage/saberglowmark"
	char		Gun_g2WeaponMarkShader[MAX_QPATH];	//none - if set, the game will ry to project this shader onto the weapon when it damages a person (good for a blood splatter on the weapon)
	gun_holster_locations_t gun_holsterPlace; // When you put the weapon where are not using it. 
} gunData_t;


//GUN FLAGS
#define GFL_CHARGESHOOT			(1<<0)// If is 1, main shoot is charged like bryar alt , blaster pistol alt. 
#define GFL_ALT_CHARGESHOOT		(1<<1)// If is 1, alt shoot is charged like bryar alt , blaster pistol alt. 
#define GFL_SPREADSHOOT			(1<<2)// if is 1, weapon shoot spreading
#define GFL_ALT_SPREADSHOOT		(1<<3)// if is 1, weapon alt shoot spreading
#define GFL_RAYSHOOT			(1<<4)// if is on 1, shoot a ray as disruptor instead of a projectile	
#define GFL_ALT_RAYSHOOT		(1<<5)// if is on 1,altfire shoot a ray as disruptor instead of a projectile	
#define GFL_RAYSHOOT2			(1<<6)// DISRUPTOR ALT SHOOT RAY 
#define GFL_ALT_RAYSHOOT2		(1<<7)// DISRUPTOR ALTFIRE ALT SHOOT RAY 
#define GFL_RAYSHOOT3			(1<<8)// CONCUSSION ALT SHOOT RAY 
#define GFL_ALT_RAYSHOOT3		(1<<9)// CONCUSSION ALTFIRE ALT SHOOT RAY
#define GFL_ZOOM				(1<<10)// if is on 1, weapon can zoom
#define GFL_GRAVITYSHOOT		(1<<11)// if is on 1, main fire shoot the explosive gravity bolt like alt repeater
#define GFL_ALT_GRAVITYSHOOT	(1<<12)// if is on 1, alt fire shoot the explosive gravity bolt like alt repeater
#define GFL_MULTIPLESHOOT		(1<<13)//if is on 1, main fire shoot multiple shoots
#define GFL_ALT_MULTIPLESHOOT		(1<<14)// if is on 1, alt fire shoot multiple shoots
#define GFL_SHRAPNELSHOOT			(1<<15)// Flechette Shrapnel Shot enable 
#define GFL_ALT_SHRAPNELSHOOT		(1<<16)//Flechette Alt fire Shrapnel Shot enable 
#define GFL_TIMEBOMBSHOOT			(1<<17)// // if is on 1, shoot a time explosive projectile 
#define GFL_ALT_TIMEBOMBSHOOT		(1<<18)// if is on 1, altfire shoot a time explosive projectile 
#define GFL_BOUNCESHOOT				(1<<19)// if is on 1, shoot bouncing projectiles 
#define GFL_ALT_BOUNCESHOOT			(1<<20)// if is on 1, alt fire shoot bouncing projectiles 
#define GFL_BOUNCEHALFSHOOT			(1<<21)// if is on 1, shoot bouncing projectiles 
#define GFL_ALT_BOUNCEHALFSHOOT		(1<<22)// if is on 1, shoot alt bouncing projectiles 
#define GFL_DIVIDESHOOT				(1<<23)// if is on 1, shoot divide projectiles as bowcaster 
#define GFL_ALT_DIVIDESHOOT			(1<<24)// if is on 1, alt fire shoot divide projectiles as bowcaster 
#define GFL_AOESHOOT				(1<<25)// if is 1, gun shoot create a deflagration like demp2 alt fire 
#define GFL_ALT_AOESHOOT			(1<<26)// if is 1, alt fire gun shoot create a deflagration like demp2 alt fire 
#define GFL_AIMSHOOT				(1<<27)	// if is 1, gun shoot aim missiles as rocket launcher
#define GFL_ALT_AIMSHOOT			(1<<28)	// if is 1, gun alt fire shoot aim missiles as rocket launcher
#define GFL_GASSHOOT				(1<<29)// if is 1, gun shoot gas projectile as noghri stick
#define GFL_ALT_GASSHOOT			(1<<30)// if is 1, gun shoot alt fire gas projectile as noghri stick
#define GFL_UNSHIELD				(1<<31)// if is 1, gun alt fire shoot gas projectile as noghri stick

// GunFlags2
#define GFL_ALT_UNSHIELD			(1<<0)//// if is 1, gun destroy shields 
#define GFL_ELECTROCUTE				(1<<1)// if is 1, gun shoot electric projecitles as demp2 
#define GFL_ALT_ELECTROCUTE			(1<<2)// if is 1, alt fire gun shoot electric projecitles as demp2 
#define GFL_DISGREGATE				(1<<3)// if is 1, enemy killed are disgregated by main fire 
#define GFL_ALT_DISGREGATE			(1<<4)// if is 1, enemy killed are disgregated by alt fire 
#define GFL_MELEE					(1<<5)// punchs as main fire 
#define GFL_ALT_MELEE				(1<<6)// punchs as alt fire 
#define GFL_MELEE2					(1<<7)// kick as main fire 
#define GFL_ALT_MELEE2				(1<<8)// kick as alt fire 
#define GFL_STAFFMELEE				(1<<9)//(TUSKEN STAFF, TUSKEN RIFLE ALT)
								// if is 1, Melee attack with a weapon is enabled 
#define GFL_ALT_STAFFMELEE			(1<<10)	// if is 1, Melee alt attack with a weapon is enabled 
#define GFL_BATON					(1<<11)	 // if is 1, shoot like stun baton   
#define GFL_ALT_BATON				(1<<12)	 // if is 1,altfire  shoot like stun baton   
#define GFL_GRENADE					(1<<13)// if is 1, shoot an explosive grenade like thermal alt
#define GFL_ALT_GRENADE				(1<<14)// if is 1, alt fire shoot an explosive grenade like thermal alt
#define GFL_TIMEGRENADE				(1<<15)// if is 1, shoot a time grenade as thermal main fire 
#define GFL_ALT_TIMEGRENADE			(1<<16)// if is 1, shoot an alt fire  time grenade as thermal main fire 
#define GFL_LASERMINE				(1<<17)// if is 1, shoot a laser mine 
#define GFL_ALT_LASERMINE			(1<<18)	// if is 1 shoot an alt laser mine
#define GFL_PROXYMINE				(1<<19)	// if is 1 fire shoot a proxy mine
#define GFL_ALT_PROXYMINE			(1<<20)	// if is 1 fire shoot an alt fire  proxy mine
#define GFL_BOMB					(1<<21)	// if is 1, place a bomb that can be activated on remote.
#define GFL_DECLOAK					(1<<22)// if is 1, decloak hitted enemy 
#define GFL_ALT_DECLOAK				(1<<23)// if is 1, alt decloak hitted enemy 
#define GFL_NOPARRY					(1<<24)// if is 1, saber can't parry the shoot  
#define GFL_ALT_NOPARRY				(1<<25)// if is 1, saber can't parry the alt shoot  
#define GFL_NODEFLECT				(1<<26)// if is 1, saber can't deflect the shoot  
#define GFL_ALT_NODEFLECT			(1<<27)// if is 1, saber can't deflect the alt shoot  
#define GFL_PUSHABLE				(1<<28)	// if is 1, projectile can be pushed away  
#define GFL_ALT_PUSHABLE			(1<<29)	// if is 1, alt projectile can be pushed away  
#define GFL_DISMEMBER				(1<<30)// if is 1, enemy killed is dismbered   
#define GFL_ALT_DISMEMBER			(1<<31)// if is 1, enemy killed by alt fireis dismbered   
// Gunflags3
#define GFL_NO_UNDERWATER			(1<<0)// if is 1 weapon can't be used underwater
#define GFL_TWOGUNS					(1<<1) // if is 1, player get two guns, each for hand! 


////////////// WEAPON GROUPS 
typedef enum {
WEAPS_ALL,
WEAPS_BLASTER,
WEAPS_HEAVY,// Glifiche
WEAPS_LIGHTBLASTER,// Arrows
WEAPS_HEAVYBLASTER,// nec Holy
WEAPS_EXPLOSIVE, // Explosive
WEAPS_MELEE, //Warrior
WEAPS_OTHER
} weaponGroup;

typedef enum {
WEAPS_FIGHTER,
WEAPS_LASER,
WEAPS_SEEKER,// Glifiche
WEAPS_BOMBS,// Arrows
WEAPS_CHAFF,// nec Holy
WEAPS_SPECIAL, // Explosive
WEAPS_OTHER2
} weaponGroup2;//for fighter


// AMMO_NONE must be first and AMMO_MAX must be last, cause weapon load validates based off of these vals
typedef enum //# ammo_e
{
	AMMO_NONE,
	AMMO_FORCE,		// AMMO_PHASER
	AMMO_BLASTER,	// AMMO_STARFLEET,
	AMMO_POWERCELL,	// AMMO_ALIEN,
	AMMO_METAL_BOLTS,
	AMMO_ROCKETS,
	AMMO_EMPLACED,
	AMMO_THERMAL,
	AMMO_TRIPMINE,
	AMMO_DETPACK,
	AMMO_MAX
} ammo_t;


typedef struct weaponData_s
{
	char	classname[32];		// Spawning name
	char	weaponMdl[64];		// Weapon Model
	char	firingSnd[64];		// Sound made when fired
	char	altFiringSnd[64];	// Sound made when alt-fired
//	char	flashSnd[64];		// Sound made by flash
//	char	altFlashSnd[64];	// Sound made by an alt-flash
	char	stopSnd[64];		// Sound made when weapon stops firing
	char	chargeSnd[64];		// sound to start when the weapon initiates the charging sequence
	char	altChargeSnd[64];	// alt sound to start when the weapon initiates the charging sequence
	char	selectSnd[64];		// the sound to play when this weapon gets selected

	int		ammoIndex;			// Index to proper ammo slot
	int		ammoLow;			// Count when ammo is low

	int		energyPerShot;		// Amount of energy used per shot
	int		fireTime;			// Amount of time between firings
	int		range;				// Range of weapon
	
	int		altEnergyPerShot;	// Amount of energy used for alt-fire
	int		altFireTime;		// Amount of time between alt-firings
	int		altRange;			// Range of alt-fire

	char	weaponIcon[64];		// Name of weapon icon file
	int		numBarrels;			// how many barrels should we expect for this weapon?

	char	missileMdl[64];		// Missile Model
	char	missileSound[64];	// Missile flight sound
	float  	missileDlight;		// what is says
	vec3_t 	missileDlightColor;	// ditto

	char	alt_missileMdl[64];		// Missile Model
	char	alt_missileSound[64];	// Missile sound
	float  	alt_missileDlight;		// what is says
	vec3_t 	alt_missileDlightColor;	// ditto

	char	missileHitSound[64];	// Missile impact sound
	char	altmissileHitSound[64];	// alt Missile impact sound
#ifndef _USRDLL
	void	*func;
	void	*altfunc;

	char	mMuzzleEffect[64];
	int		mMuzzleEffectID;
	char	mAltMuzzleEffect[64];
	int		mAltMuzzleEffectID;

#endif

	int		damage;
	int		altDamage;
	int		splashDamage;
	int		altSplashDamage;
	float	splashRadius;
	float	altSplashRadius;

} weaponData_t;


typedef struct ammoData_s
{
	char	icon[32];	// Name of ammo icon file
	int		max;		// Max amount player can hold of ammo
} ammoData_t;

// Bryar Pistol
//--------
#define BRYAR_PISTOL_VEL			2500
#define BRYAR_PISTOL_DAMAGE			15
#define BRYAR_CHARGE_UNIT			200.0f	// bryar charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove

// E11 Blaster
//---------
#define BLASTER_MAIN_SPREAD			0.5f
#define BLASTER_ALT_SPREAD			1.5f
#define BLASTER_NPC_SPREAD			0.5f
#define BLASTER_VELOCITY			2000
#define BLASTER_NPC_VEL_CUT			0.5f
#define BLASTER_NPC_HARD_VEL_CUT	0.7f
#define BLASTER_DAMAGE				20
#define	BLASTER_NPC_DAMAGE_EASY		10
#define	BLASTER_NPC_DAMAGE_NORMAL	15 // 14
#define	BLASTER_NPC_DAMAGE_HARD		20 // 18

// Tenloss Disruptor
//----------
#define DISRUPTOR_MAIN_DAMAGE			14
#define DISRUPTOR_NPC_MAIN_DAMAGE_EASY	5
#define DISRUPTOR_NPC_MAIN_DAMAGE_MEDIUM	10
#define DISRUPTOR_NPC_MAIN_DAMAGE_HARD	15

#define DISRUPTOR_ALT_DAMAGE			12
#define DISRUPTOR_NPC_ALT_DAMAGE_EASY	15
#define DISRUPTOR_NPC_ALT_DAMAGE_MEDIUM	25
#define DISRUPTOR_NPC_ALT_DAMAGE_HARD	30
#define DISRUPTOR_ALT_TRACES			3		// can go through a max of 3 entities
#define DISRUPTOR_CHARGE_UNIT			150.0f	// distruptor charging gives us one more unit every 150ms--if you change this, you'll have to do the same in bg_pmove

// Wookie Bowcaster
//----------
#define	BOWCASTER_DAMAGE			15
#define	BOWCASTER_VELOCITY			2500
#define	BOWCASTER_NPC_DAMAGE_EASY	7
#define	BOWCASTER_NPC_DAMAGE_NORMAL	10
#define	BOWCASTER_NPC_DAMAGE_HARD	15
#define BOWCASTER_SPLASH_DAMAGE		0
#define BOWCASTER_SPLASH_RADIUS		0.0f
#define BOWCASTER_SIZE				2

#define BOWCASTER_ALT_SPREAD		5.0f
#define BOWCASTER_VEL_RANGE			0.3f
#define BOWCASTER_CHARGE_UNIT		200.0f	// bowcaster charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove

// Heavy Repeater
//----------
#define REPEATER_SPREAD				1.4f
#define REPEATER_NPC_SPREAD			0.7f
#define	REPEATER_DAMAGE				10
#define	REPEATER_VELOCITY			2000
#define	REPEATER_NPC_DAMAGE_EASY	7
#define	REPEATER_NPC_DAMAGE_NORMAL	5
#define	REPEATER_NPC_DAMAGE_HARD	10

#define REPEATER_ALT_SIZE				3	// half of bbox size
#define	REPEATER_ALT_DAMAGE				60
#define REPEATER_ALT_SPLASH_DAMAGE		60
#define REPEATER_ALT_SPLASH_RADIUS		128.0f
#define	REPEATER_ALT_VELOCITY			1100
#define	REPEATER_ALT_NPC_DAMAGE_EASY	20
#define	REPEATER_ALT_NPC_DAMAGE_NORMAL	40
#define	REPEATER_ALT_NPC_DAMAGE_HARD	60

// DEMP2
//----------
#define	DEMP2_DAMAGE				20
#define	DEMP2_VELOCITY				2500
#define	DEMP2_NPC_DAMAGE_EASY		6
#define	DEMP2_NPC_DAMAGE_NORMAL		12
#define	DEMP2_NPC_DAMAGE_HARD		18
#define	DEMP2_SIZE					2		// half of bbox size

#define DEMP2_ALT_DAMAGE			20
#define DEMP2_CHARGE_UNIT			500.0f	// demp2 charging gives us one more unit every 500ms--if you change this, you'll have to do the same in bg_pmove
#define DEMP2_ALT_RANGE				4096
#define DEMP2_ALT_SPLASHRADIUS		256.0f

// Golan Arms Flechette
//---------
#define FLECHETTE_SHOTS				1
#define FLECHETTE_SPREAD			4.0f
#define FLECHETTE_DAMAGE			5
#define FLECHETTE_VEL				2000
#define FLECHETTE_SIZE				1

#define FLECHETTE_ALT_DAMAGE		10
#define FLECHETTE_ALT_SPLASH_DAM	10
#define FLECHETTE_ALT_SPLASH_RAD	128.0f

// NOT CURRENTLY USED
#define FLECHETTE_MINE_RADIUS_CHECK		200
#define FLECHETTE_MINE_VEL				1000
#define FLECHETTE_MINE_DAMAGE			100
#define FLECHETTE_MINE_SPLASH_DAMAGE	200
#define FLECHETTE_MINE_SPLASH_RADIUS	200.0f

// Personal Rocket Launcher
//---------
#define	ROCKET_VELOCITY				2250
#define	ROCKET_DAMAGE				30
#define	ROCKET_SPLASH_DAMAGE		20
#define	ROCKET_SPLASH_RADIUS		160.0f
#define ROCKET_NPC_DAMAGE_EASY		10
#define ROCKET_NPC_DAMAGE_NORMAL	15
#define ROCKET_NPC_DAMAGE_HARD		20
#define ROCKET_SIZE					3

#define	ROCKET_ALT_VELOCITY			(ROCKET_VELOCITY*0.5)
#define ROCKET_ALT_THINK_TIME		100

// Concussion Rifle
//---------
//primary
#define	CONC_VELOCITY				2500
#define	CONC_DAMAGE					75
#define CONC_NPC_SPREAD				0.7f
#define	CONC_NPC_DAMAGE_EASY		75
#define	CONC_NPC_DAMAGE_NORMAL		100
#define	CONC_NPC_DAMAGE_HARD		125
#define	CONC_SPLASH_DAMAGE			75
#define	CONC_SPLASH_RADIUS			300.0f
//alt
#define CONC_ALT_DAMAGE				225//100
#define CONC_ALT_NPC_DAMAGE_EASY	200
#define CONC_ALT_NPC_DAMAGE_MEDIUM	225
#define CONC_ALT_NPC_DAMAGE_HARD	250

// Emplaced Gun
//--------------
#define EMPLACED_VEL				2250	// very fast
#define EMPLACED_DAMAGE				200		// and very damaging
#define EMPLACED_SIZE				5		// make it easier to hit things

// ATST Main Gun
//--------------
#define ATST_MAIN_VEL				3000	// 
#define ATST_MAIN_DAMAGE			25		// 
#define ATST_MAIN_SIZE				3		// make it easier to hit things

// ATST Side Gun
//---------------
#define ATST_SIDE_MAIN_DAMAGE				75
#define ATST_SIDE_MAIN_VELOCITY				1500
#define ATST_SIDE_MAIN_NPC_DAMAGE_EASY		30
#define ATST_SIDE_MAIN_NPC_DAMAGE_NORMAL	40
#define ATST_SIDE_MAIN_NPC_DAMAGE_HARD		50
#define ATST_SIDE_MAIN_SIZE					4
#define ATST_SIDE_MAIN_SPLASH_DAMAGE		10	// yeah, pretty small, either zero out or make it worth having?
#define ATST_SIDE_MAIN_SPLASH_RADIUS		16.0f	// yeah, pretty small, either zero out or make it worth having?

#define ATST_SIDE_ALT_VELOCITY				1000
#define ATST_SIDE_ALT_NPC_VELOCITY			600
#define ATST_SIDE_ALT_DAMAGE				130

#define ATST_SIDE_ROCKET_NPC_DAMAGE_EASY	50
#define ATST_SIDE_ROCKET_NPC_DAMAGE_NORMAL	75
#define ATST_SIDE_ROCKET_NPC_DAMAGE_HARD	100

#define	ATST_SIDE_ALT_SPLASH_DAMAGE			50
#define	ATST_SIDE_ALT_SPLASH_RADIUS			200.0f
#define ATST_SIDE_ALT_ROCKET_SIZE			5
#define ATST_SIDE_ALT_ROCKET_SPLASH_SCALE	0.5f	// scales splash for NPC's

// Stun Baton
//--------------
#define STUN_BATON_DAMAGE			22
#define STUN_BATON_ALT_DAMAGE		22
#define STUN_BATON_RANGE			25

// Laser Trip Mine
//--------------
#define LT_DAMAGE			100
#define LT_SPLASH_RAD		256.0f
#define LT_SPLASH_DAM		45

#define LT_VELOCITY			250.0f
#define LT_ALT_VELOCITY		1000.0f

#define PROX_MINE_RADIUS_CHECK		190

#define LT_SIZE				3.0f
#define LT_ALT_TIME			2000
#define	LT_ACTIVATION_DELAY	1000
#define	LT_DELAY_TIME		50

// Thermal Detonator
//--------------
#define TD_DAMAGE			100
#define TD_NPC_DAMAGE_CUT	0.6f	// NPC thrown dets deliver only 60% of the damage that a player thrown one does
#define TD_SPLASH_RAD		128
#define TD_SPLASH_DAM		90
#define TD_VELOCITY			900
#define TD_MIN_CHARGE		0.15f
#define TD_TIME				4000
#define TD_THINK_TIME		300		// don't think too often?
#define TD_TEST_RAD			(TD_SPLASH_RAD * 0.8f) // no sense in auto-blowing up if exactly on the radius edge--it would hardly do any damage
#define TD_ALT_TIME			3000

#define TD_ALT_DAMAGE		100
#define TD_ALT_SPLASH_RAD	128.0f
#define TD_ALT_SPLASH_DAM	90
#define TD_ALT_VELOCITY		600
#define TD_ALT_MIN_CHARGE	0.15f
#define TD_ALT_TIME			3000

// Tusken Rifle Shot
//--------------
#define TUSKEN_RIFLE_VEL			2500	// fast
#define TUSKEN_RIFLE_DAMAGE_EASY	40		// damaging
#define TUSKEN_RIFLE_DAMAGE_MEDIUM	50		// very damaging
#define TUSKEN_RIFLE_DAMAGE_HARD	60		// extremely damaging

// Blood Gun
//--------
#define BLOODGUN_VEL			2500
#define BLOODGUN_DAMAGE			20
#define BLOODGUN_CHARGE_UNIT			200.0f	// bryar charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove

// Cannon
//---------
#define CANNON_SHOTS			5
#define CANNON_SPREAD			4.0f
#define CANNON_DAMAGE			6
#define CANNON_VEL				2000
#define CANNON_SIZE				1

#define CANNON_ALT_DAMAGE		15
#define CANNON_ALT_SPLASH_DAM	15
#define CANNON_ALT_SPLASH_RAD	128.0f

// NOT CURRENTLY USED
#define CANNON_MINE_RADIUS_CHECK		200
#define CANNON_MINE_VEL				1000
#define CANNON_MINE_DAMAGE			150
#define CANNON_MINE_SPLASH_DAMAGE	250
#define CANNON_MINE_SPLASH_RADIUS	250.0f

// Poison Shoot
//--------

#define	POISON_DAMAGE				10
#define	POISON_VELOCITY				1700
#define	POISON_NPC_DAMAGE_EASY		6
#define	POISON_NPC_DAMAGE_NORMAL	11
#define	POISON_NPC_DAMAGE_HARD		15
#define	POISON_SIZE					2	
#define POISON_ALT_DAMAGE			15
#define POISON_CHARGE_UNIT			400.0f	// demp2 charging gives us one more unit every 500ms--if you change this, you'll have to do the same in bg_pmove
#define POISON_ALT_RANGE			4096
#define POISON_ALT_SPLASHRADIUS		128.0f

#endif//#ifndef __WEAPONS_H__



// ------------------------------------------------------
// LEGACY OF KAIN MAGIE PER MIETITORI
// SIGILLI MIETITORI
/*
Sigilli Minori:
• Rephi - La Forza: questo incantamento raddoppia la forza del Mietitore che sarà in grado di
sollevare carichi maggiori e causare molti più danni. (Force Rage del Mietitore)
• Beog - Il Gigante: questo incantamento aumenta la resistenza del Mietitore, rendendolo più
massiccio e robusto e in grado di sopportare un maggior numero di ferite. 
(Force Protect del Mietitore)
• Wugoc - La Bestia: questo incantamento raddoppia il ritmo di rigenerazione delle ferite
nello Spectral.
(Force Heal del Mietitore)
• Firrin - Lo Scudo: questo incantamento permette di attivare una barriera che protegge da
tutti i danni per un minuto. Costo: 4 EP.
(Force Absorb del mietitore)
• Tiraza - Lo Sguardo: questo incantamento permette di scrutare il Regno Spettrale da quello
Materiale per individuare la presenza di anime, creature, portali, variazioni ambientali, ecc.
(Forse Sense del Mietitore)
• Shadir - La Fusione: questo incantamento permette di fondere 2 impregnature della
Mietitrice per generare un terzo elemento (p. es. fondere Fuoco e Luce per ottenere
Radianza).
(sbloccato con le forge) 
• Lisara - Il Sentiero: questo incantamento permette al Mietitore di muoversi al doppio della
sua normale velocità. Si applica sia agli spostamenti che al combattimento.
(force speed del mietitore)
• Dabor - La Statua: questo incantamento ricopre il Mietitore di uno strato di pietra per 15
minuti, durante i quali vengono rigenerate tutte le ferite fisiche. Costo: 5 EP.
(sbloccato da Anteo con la forgia della Pietra)
• Elion - La Sorte: questo incantamento permette di impregnare per un minuto la Mietitrice
con un elemento base diverso da quelli posseduti dal Mietitore.
(sbloccato nelle forge)
Sigilli Maggiori:Ashak - La Voce: questo incantamento permette di influenzare una qualunque creatura per
costringerla a fare ciò che il Mietitore desidera. I Guardiani sono probabilmente immuni a
questo potere. Costo: 6 EP.
(Force Stun e Hate del Mietitore)
• Mudh - L'Energia: questo incantamento permette di rigenerare tutti gli EP, una volta per
missione.
(Banca di Energia del Mietitore)
• Genhur - Il Corpo: questo incantamento permette di assumere la forma e la consistenza di
un elementale dello stesso tipo dell'attuale impregnatura della Mietitrice, per un giorno.
Costo: Tutti gli EP (minimo 20).
(Form del Mietitore: magari nello slot del wrack)
*/


// -----------------------------------------------------------
// ASTROFANTASY FORCE POWERS 

// QUESTO DOVREBBE STARE NEI POTERI DELLA FORZA. MA Q_SHARED.H
// è compromesso quindi lo metto qua. 
// ABILITA' E POTERI DELLA FORZA DEI VESU, OZER, MIRLAR, LE VIRTU' 
/*
0 fuoco ed elettricità , fluido elettrico, memoria, intuizione. 
1 fuoco, essicazione, aridità. 
2 amore, charming, evocazione 
3 amore, forza e resistenza fisica 
4 fortuna in giochi e competizioni. Forza e reistenza magica
5 scrittura, letteratura, arte 
6 psichico, influsso lunare 
7 formule 
8 elettromagnetismo 
9 acqua, evocazione di elementali d'acqua, placare tempeste e burrasche marine 
10 controllo sulle bestie 
11carisma, trasmutazione dei metalli . Magia del sole e della luce 
12 conoscenza e supporto dei minion leadership. 
13 eloquenza 
14 filosofia e intuizione 
15 amore e amicizia mentale, astrale e fisica 
16 suono, simpatia, spiriti dell'aria, musica, charming amici e nemici 
17 studio delle lingue, magnetismo comprnsione delle lingue. 
18 idromanzia, evocazione di ondine ed elementali d'acqua 
19 idromanzia, alchimia dei fluidi 
20 elemento terra. geomanzia, connessione fra fuoco e terra, magma e lava 
21 giustizia, armonia, conoscenza della legge, 
22  Saggezza, intelligenza, sagacia 
23 taumaturgia del fuoco. Gnomi, cura malattie 
24 alchimia ed erboristeria. Elettromagnetismo per modificare la composizione delle pozioni 
25 magia mentale, astrale e fisica. Controllo mentale 
26 Astrologia. Effetti degli astri su piante, animali e minerali 
27 talismani e simboli 
28 profumi, medicine, psiche, corpo. 
29 idropatia e omeopatia 
30 arte della scherma , attacco e difesa. 

ABILITA' E POTERI DELLA FORZA DEI KHROLEN, GA'HIR  TOLKAR, POTENZE
1 sole, luce, elettricità, elettromagnetismo. 
2 origine e controllo delle passioni violente. Autocontrollo. Dominio di se stessi. 
3 magia degli elementi. Specialmente di tipo Natura, Sacro 
4 utilizzo delle forze elementali fuoco-terra-aria-acqua, sul corpo fisico. 
5 genesi e crescita della vita. Controllo sul soma\semi 
6 connessione con Aria. Connessione con lo Spirito. 
7 controllo della magia conness aall'aria e al vento. 
8 amore, e sacro. Distinzione fra sacralità, e perfezione fisica. 
9 resistenza al fuoco. Invulnerabilità. Alchimia: trasmutazione degli elementi. Tramutazione dell'acqua in ghiaccio. 
10proiezione della mente nelle sfere astrali. Trance, proiezione delle cose astrali sul piano fisico. 
11 cabala. Applicazione della virtù divina, in armonia alle leggi universali 
12 armonia verso le leggi della creazione. Abilità di combattere il caos e l'oscurità.
13 comprensione trascendentale delle religioni, delle filosofie e della simbologia 
14 connessione fra mente, spirito e corpo. comprensione della discordia e della disarmonia. 
Influenza della matrice astrale e mentale 
15 intuizione divina, onniscienza. 
16 canalizzazione dell'amore cosmico sul piano fisico, spirituale e astrale. teurgia
17 comprensione dell'anatomia umana, applicazione taumaturgica. 
18 Creazione di talismanu curativi. 
19 alchimia dei metalli. Fabbricazione di medicine alchemiche dorate per la cura delle malattie. 
20 dono della profezia. 
21 alta stima, potere e ricchezza nel mondo fisico. Favore delle persone importanti. 
22 yoga. Meditazione tantrica. Decofidicazione antichi testi. 
23 comprensione degli eventi fisici, mentali e astrali. Controllo di essi. Trasformazione della comprensione in azione. Delle cause in effetti. Comprensione della legge di attrazione, di come I pensieri e le azioni originano effetti e conseguenze, e quali. 
24 aumento della coscienza spirituale. Proiezione nel piano astrale senza la separazione dal corpo fisico. 
25 telecinesi, materializzazione, abilità medianiche. 
26 realizzazione dei desideri. Solo su piano fisico. 
27 effetti delle formule magiche sugli elementi o sugli elementali.
Obbedienza delle creature dell'aria e dell'acqua. 
28 idroamanzia. Maestria dlela magia dell'acqua. Cura delle malattie da parte dell'acqua, anche gravi. 
29 protezione da temporali, tempeste, elettricità. Controllo del clima. Placare o scatenare tempeste e piogge. 
(per una specie che vive in un mondo desertico è un potere non da poco! D: ) 
30 arte della lotta. 

ABILITA' E POTERI DELLA FORZA DEI CENTOPULOS,GILGAN,  EZHELOR, DOMINAZIONI 
0 metallurgia e e lavorazione dei metalli per mezzo del fuoco e dell'acqua indurendoli. Talento di Vulcano. 
1 vapore, vaporizzazione dei fluidi, motori a vapore... grado più adatto ai Gorkan Vesu, che ai centopulos, o allo steampunk in generale. 
2 controlla tutti I frutti della terra. Dona le conoscenze per la loro coltivazione, maturazione, essicazione, consumazione nei momenti di magra. Demetra. 
3 portare la pace nel mondo e fra tutti I popoli del mondo. Ermeti . 
4 fortuna, scommesse e gioco d'azzardo. Tyche. Ermeti. Affari, commercio. Capacità di portare fortuna e felicità a se stessi e agli altri. Tyche 
5 moralità, giustizia, equilibrio magico, saggezza e discernimento. Introspezione verso se stessi e verso le altre persone. Intuizione e riconoscimento. Comprensione di saggezza superiore. Zeus
6 pittura, ricamo, iscrizione, disegno, fotografia, cinema, arte grafica e figurativa. Krasi, 
7 conoscenza delle fasi lunari, delle 28 stazioni lunari, del ciclo della luna e del tempo. Krono 
8 protezione di pazzi, folli e lunatici. Cura della pazzia. Krasi.Ipno  
9 protezione durante I viaggi, specie via mare. Protezione da naufragi e tempeste. Talismani che difendono dai naufragi. Lipari. 
10 Evocazione di ogni tipo di essere, dominio mentale sulle creature, buone o malvage che siano. Uri. 
11 saggezza più grande. Accesso ai misteri più occulti e segreti, ma solo verso I saggi, non agli stolti. Khron. 
12 realizzazione degli ideali per mezzo di cabala. . 
13 questioni commercilali e monetarie. Numismatica, fabbricazione delle monete. Ermete. 
14 Educazione e cura dei bambini, scuola, istruzione. Atena. 
15 folklore e riti di ogni popolo. Corteggiamento, amore e matrimonio. Afrodite, alaki. Comprensione di tutti I riti, gli usi e I costumi fin dall'albore dei tempi. 
16 ilizia. Tutti gli orfani, abbandonati, espulsi e odiati sono sotto la speciale protezione di questo grado. A seconda del loro Karma, li aiuta tutti a sopportare il proprio destino più facilmente e aiuta in tutti i casi in cui la Divina Provvidenza lo consente, a risolvere i loro problemi. 
17 caccia di animali selvatici. Creazione di armi e trappole necessari alla loro cattura.  Artemide, Amazzoni. 
18 Astronomia, astrologia, influssi del cielo sulla terra, magia astrale e siderale. Uri. Urano. 
19 Cura delle malattie dannose come epilessia, cancro, alattie del midollo spinale, malattie incurabili o di cause sconosciute. Comprensione delle cause della malattia e dei metodi necessari a guarirla e a realizzarne le medicine. Clapi: Esculapio. 
20 felicità, pace, piacere e gioia . Ispirazione di momenti felici. Krasi e Parnas 
21 giustizia, legale o meno che sia. Zeus. 
22 scienza del colore, cromatismo, produzione dei colori, tinture, pitture, inchiostri, scrittura. Parnas 
23Introspezione e autoconsapevolezza al fine di raggiungere equilibrio fisico, magico, mentale,  e spirituale. Ipno. 
24 silenzio, magia del silenzio. Magia dall'effetto principalmente negativo. Tarak
25 coscienza e voce interiore, redenzione dei criminali attraverso prese di coscienza. 
26 prestidigitazione. Fenomeni magici, magia fenomenale di bassa lega. Illusionismo. 
27 comprensione del destino e del libero arbitrio. Capacità di cambiare il destino delle persone. Comprensione di quanto ci si spinga lontani con il libero arbitrio dal proprio fato, o se si viene meno ai propri doveri. Kron. 
28 magia della luce. Fabbricazione di amuleti o talismani. Creazione di sigilli per bandire gli esseri malefici di altri piani di esistenza. Talamòs.
29 cura delle acque dolci site sulla crosta terrestre. Localizzazione delle falde acquifere sotterranee. Capacità di trovar acqua potabile anche nel deserto più arido, senza bisogno del'uso della rabdomanzia. Protezione di tutti coloro che si occuopano di acquedotti e impianti idrico-fognari. 
30: Eracli: Offesa. 
30: Estali. Difesa 


ABILITA' E POTERI DELLA FORZA DEI MONKER, MWANGA, DJANGA, PRINCIPATI
0 trasmissione di amicizia, simpatia, benevolenza,  magia del fuoco, condensazione dei fluidi, far apparire scritte.  
FP_HATE
1 fenomeni naturali connessi al fuoco e al sacro. Miracoli. Perpetuazione della specie. FP_SUNGLYPH
2 conoscenza ermetica delle piante. Floromanzia. Uso delle piante per produrre incensi. FP_STUN
3 amicizia, ricchezza, erboristeria. Dalle piante posson esser estratte sostanze che donano salute, longevità e protezione dalle malattie. Sostanze spagiriche. FP_HEALOTHER
4 consigli sul piacere e sulla felicità terrena. FP_PUSH
5 conoscenza dei numeri, matematica.  FP_PULL 
6: intuizione e ispirazione per scrittura e progetti letterari. Dono della letteratura. Ispirazione  a comporre fiabe, leggende, saghe,  FP_LEVITATION, 
 7 occultismo. Romanzi che palrano di occultismo. Rivelazione di ciò che è nascosto e occulto, su piano mentale, e astrale. FP_SEE
8 aiuti occulti, riti, formule, gesti, incantesimi. Sviluppo capacità mentali, astrali, fisiche. Scrittura ispirata\dettata dagli spiriti.  FP_WRACK
9 magia della luna e della notte. Conoscenza degli influssi lunari sul mondo fisico, mentale e astrale. Astrologia della luna. FP_DARKGLYPH
10 occultismo: ipnotismo, magnetismo medico, controllo della mente altrui. MAI usare tale abilità per scopi ignobili. Pena l'istigazione di forze oscure.  Al fine di prevenire qualsiasi uso improprio di intelligenze ed esseri, ho desistito dal menzionare in questo lavoro le intelligenze e le teste della zona che cingono la terra che rappresentano le qualità negative. Ci sono tante teste negative quante ne esistono teste con qualità buone e positive.
FP_CONTROLMIND
11: fuoco, aura. Calore. onore, stima, dignità, amicizia, amore. FP_FIREGLYPH 
12: magia della natura. Controllo delle leggi naturali, fino a compiere cose prodigiose, al limite dlel'innaturale. Lato oscuro: stregoneria della natura. Voodismo  FP_NECROGLYPH 
13  arricchimento, affari, successo professionale. La pentola d'oro degli gnomi? FP_DRAIN
14 : fortuna, vittoria grazie alla Divina Provvidenza, se lo approva. NOTA: questo dipende SOLO dal volere e dalla volontà divina. Non può esser usata per nessuno scopo impuro o empio. Sempre raccomandarsi a Dio in tal senso.
FP_HOLYGLYPH 
15 °, arte, letterratura. FP_RAGE
16: amore e amicizia, talismani atti a tale scopo.  FP_FREEZE
17 magia dell'aria e dell'acqua, astrale, mentale, fisico FP_WINDGLYPH 
18: agricoltura, agrochimica. Agronomia.  crescita accelerata delle piante, miglioramento della produttività del suolo. Raccolti eccezionali. FP_SABERTHROW 
19 ° Toro: invenzioni tecniche a scopi agricoli e forestali. Protezione di campi e foreste. FP_GRIP 
20 polarizzazione elettrica. Capacità di far crescere un seme pianato in un albero entro un'ora. Talento dei fachiri indiani. (ah ecco perche' dormono sui chiodi come niente). FP_LIGHTNING 
21  chiarezza mentale, giudizio, intuizione, ispiraizone. FP_SPEED 
22 gastronomia, dietologia, Associazioni del cibo. Mangiare bene per sentirsi in forma? FP_HEAL
23 mineralogia e applicazione occulta dei minerali. Formule e incnatamento per la creazione di fludi, talismani, sostanze, arcanismo. Fabbricazione di specchi, scettri, bacchette magiche. FP_FORCEGLYPH
24: conoscenza del mondo sotterraneo, delle miniere, delle acque sotterranee, e dei vulcani. 
Uso arcano delle correnti magiche, laviche o acquatiche esistenti sotto la superficie della terra. Creazione di terremoti, geyser, eruzioni, fenomeni endogeni.Radbomanzia. Pendoli siderici.  FP_STONEGLYPH
25 linguaggio cosmico, cabala, tantra, formule.FP_SOUNDGLYPH 
26 comprensione delle religioni e della loro vera legalità. Discernimento fra religioni morali, etiche e legittime, paganesimo,e riti empi e immorali da abrogare. Comprensione e correzione dei dogmi scorretti o svianti, che allontanano dalla verità divina.  FP_ABSORB 
27 fisica cosmica universale. Metafisica, metapsichica. Poteri cosmici. Comprensione dei fenomeni naturali. 
FP_PROTECT
28 innesto di piante. Controllo di tutto ciò che è in acqua, e che vive nell'acqua. Comunicazione di ondine ed elementali d'acqua. Capacità di sopravvivere ad ogni naufragio. Abilità di nuoto. Capacità di placare burrasche. 
FP_WATERGLYPH 
29 controllo sugli esseri marini e acquatici. FP_TELEPATHY 
30 trappole e agguati: attacco FP_SABER_OFFENSE
30  trappole e agguati: difesa FP_SABER_DEFENSE 


ABILITA' E POTERI DELLA FORZA DEI CRANION, VANYA, ARALIA, OSSERVATORI
0 comprensione e controllo degli eventi politci del mondo fisico. Potere politico, diplomazia. 
lascia che le opinioni politiche abbiano esito negativo o positivo, come ordina la Divina Provvidenza, considerando lo sviluppo spirituale dell'umanità. Abilità oratoria, statismo. 
1 inventiva. Capacità di inventare. Impedisce che le invenzioni anacronistiche vengano realizzate prima che venga il loro tempo, o, che quelle che vengon realizzate, sian divulgate prematuramente, in quanto può esser controproducente per il progresso umano, o posson cader nelle mani sbagliate. 
2 arte fisica, scultura, pittura, etc. 
3 conoscenza dei fiori e del loro linguaggio. Conoscenza mistica dei fiori. Fiori di bach? 
4 amore, armonia e bellezza. Comprensione delle leggi che regolano simpatia e antipatia. Equilibrio. 
Comprensione della bellezza e dell'armonia che vi è in tutte le cose. 
5 insegnamento, scrittura, editoria. 
6 parola e scrittura. Espressione del linguaggio gestuale, mimico, verbale, scritto. Decodifica di lettere e testi antichi, decifrazione di antichi testi e di lingue morte. Calligrafia. 
 7 protezione dei poveri, dei repressi, dei disperati, dei reietti. Abilità di aiutare tossicodipendenti e drogati dall'uscire dalla loro dipendenza. 
8 alchimia. Legame fra corpo astrale e spirito. Immunità e protezione da attacchi astrali oscuri e da magia nera che posson danneggiare l'anima. Rigenerazione del corpo. Preserva dall'invecchiamento, dalle ferite da taglio, dalle ustioni e dalle coltellate.
9 alchimia medica. produzione di medicine alchemiche, incensom unguenti magici. Farmacologia. Trovare cure alle malattie. 
10  stima, successo, onore e ricchezza negli affari. Abilità in economia e commercio. Ingegneria, metallurgia, meccanica, abilità costruttive di macchine, ingranaggi etc. 
11 sacro. Custodia di tutte le reliquie, luoghi santi, templi, chiese, statue di santi, immagini di santi. Connessione col divino, santità. Comprensione di quadri, statue e monumenti come spiiriti intelligenti, idee personificate. Lato negativo: idolatria. (questo è proibito dal 1° comandamento)  
12 teologia, riti.  conoscenza dei riti religiosi. Maestro di cerimonia. Comprensione dei riti di ogni religione o setta, comprensione dei loro significati. 
13 medicina. Medicina moderna, chirurgia. Medicina chimico farmaceutica. Diagnostica. 
14 successo nel commercio. Conciatura di pelli, pellicce, cuoio e simili. Lavorazione della carta, impregnature magiche. 
15 agricoltura e silvicoltura. Contrllo di tutta la vegetazione. Floromanzia. Capacità di guidare, accelerare e ritardare la crescita delle piante. 
16 fantasia, immaginazione, saghe, fiabe, lettere. Talento in poesia  escrittura fantasy. Eloquenza. Comprensione della magia insita nelle fiabe, del loro sense of wonderful. 
17 architettura, ingengneria strutturale, proteggetazione di autostrade, bacini fluviali, opere acquatiche etc. 
18 profumi magici. Capacità di sintetizzare profumi soavi o odori pestilenziali. Causare amore, simpatia, sogni, aumentare potere di concentrazione per mezzo di profumie incensi. Aromaterapia. Trattamento delle malattie per mezzo degli odori. 
19° acqua, nuotare, immergersi, viaggiare nel mare. Talento nel costruire navi e mezzi acquatici, o sottomarini. Divinazione del futuro: visualizzare invenzioni future riguardanti il trasporto su e nell'acqua.  
20° fortuna in affari, giochi, borsa, speculazioni. 
21° legge materiale e giustizia divina. Retribuzione: punizione di coloro che nuociono alla persona. Il male inflitto alle vittime, si ritorce contro il rio. Tipo caos armor, o soul of vengeance skill dei psichici di pw. 
22° geomanzia. Idromanzia, piromanzia, floromanzo, aeromanzia, criomanzia,  tutti i generi di arte mantica. 
23° protezione dei propri averi dai ladri, dal furto, dalle rapine. Può persuadere i ladri a ravvedersi e a restituire la merce rubata o consegnarsi alla giustizia. Ritrovamento e recupero di oggetti smarriti. 
24° conoscenza tecnica e invenzioni relative a veicoli e mezzi di trasporto terrestri di ogni tipo, dal carro all'automobile. Protezione dagli incidenti stradali. Capacità di comprendere i carburanti e di cercare energie alternative ad atomiche o inquinanti. 
25° Grado psichico, da Mentalista. protegge la coscienza imana. Impedisce all'uomo medio di andare oltre i limiti del mondo fisico. Amplifica la coscienza e la consapevolezza. Controllo dello sviluppo e della maturità mentale di ogni essere umano. Psichiatria, guarigione e cura della follia altrui. Protezione dal pericolo din impazzire.  Telepatia. Lettura del pensiero. Capacità di influenzare la mente degli altri. Protezione dal suicidio. Punizione del suicidio: le anime dei suicidi restano nel limbo, prive di coscienza per tanto tempo quanto avrebbero dovuto vivere. Trasferimento della propria coscienza e ricordi da una mente all'altra. Sviluppo delle capacità della memoria oppure perdita dei ricordi. 
26° signore di tutte le creature alate, dalla farfalla all'aquila. Conoscenza di tutti gli animali volanti e loro controllo. 
27° custodia di chiavi e segreti magici. Riscoprire segreti perduti e dimenticati. Nel nostro attuale stato di sviluppo, guida e controlla l'aeronautica di tutto il mondo con tutte le sue invenzioni. 
Comprensione della gravitazione, della levitazione, del volo. Capacità di levitare, fluttuare, muoversi a velocità incredibili. Capacità di muoversi a velocità prossime a quellle della stessa rotazione terrestre. 
28° intelletto e coscienza. Aumenta l'intelletto e la capacità di comprendere, imparare e memorizzare, nei limiti di quanto è concesso ad una civiltà nel suo attuale stato di sviluppo. 
29° saggezza.maturata dall'esperienza e dalla maturita. Protezione dei segreti arcani da sciocchi, stolti e immaturi infondendo loro sospetto, incredulità, autoconcetto e opinioni unilaterali, impedendo così loro di comprendere e carpire tali segreti. Comprensione della differenza fra intelletto, sapienza e saggezza per mezzo dell'intuizione. 
30: difesa shamorita
30  offesa shamorita 


ABILITA' E POTERI DELLA FORZA DEI VEGETER, SICA,  MIRALIA, GIARDINIERI, 
0  introduce il mago alla Magia Divina. Conoscenza base della magia: il mago apprende a provocare determinate cause nel mondo arcano al fine di scatenare i relativi effetti sul piano mentale, astrale, e fisico. Non appena il mago controlla completamente questo tipo di pratica magica, è in grado di produrre cause molto prima della loro realizzazione, anche molti anni prima della loro realizzazione. Con ciò il mago diventa un maestro perfetto nel campo del controllo fatale e ha il suo Karma e il Karma di altre persone nelle sue mani. Considerando il suo sviluppo etico, il mago non produrrà mai alcuna causa che avrebbe effetti negativi su nessuno dei tre piani. 
A un tale mago verrà affidato un numero così elevato di segreti che non avrebbe mai pensato possibile.
1 elevazione della personalità. Influenza di uomini e animali secondo il proprio volere.  Carisma, incremento del proprio potere. Druidi blu? Druidi d'argento? 
2 simpatia. Magia mummica (?) creazione di fluidi condensati. 
3 indurimento del cuore. Difesa dalle delusioni in amicizia, amore, matrimonio. Forza, tenacia e caparbietà necessarie a combattere il fato avverso. Controllo del Saturno contro insomma. Gelioti, criomanzia
4 questo grado governa la reincarnazione delle anime, dal mondo dei defunti, e lo sviluppo nel grembo materno. Evocazione degli spiriti dei trapassati. Kwarg 
5 acquisce la percezione degli influssi mercuriani. Rende particolarmente recettivi e capaci di apprendere rapidamente. Migliora la comunicazione. Cabala di mercurio. 
6 talento nella tragedia e nella drammaturgia. 
7 capacità di percepire l'energia e la radioattività naturale del mondo, della terra e dei minerali. Difesa dalle energie malefiche e dalle radiazioni dannose. La sensibilità alle radiazioni elettomagnetiche è superiore a quella ottenuta da qualsiasi apparecchio elettronico. Druidi del fulmine 
8 anatomia occulta di uomini e animali. Comprensione e cura delle malattie, e della natura dei corpi astrali degli spiriti. Guaritori blu 
9 guida ad apprendimento, illuminazione, onniscienza, la comprensione ottenuta è proporzionale alla propria esperienza e maturità. Aumento della cognizione. Yoga. 
10 comprensione della nascita (o rinascita) delle anime e della loro incarnazione terrena. Comprensione della morte. Durata della vita terrena, o del tempo trascorso nel mondo ultraterreno fra due esistenze. Conoscenza della data di morte delle persone o della propria. (insokmma necromanzia, abbiam capito. -.- ) 
Gork 
11 rivelazione dell'invisibile. Comprensione degli elementi e delle forze del mondo astrale, e delle creature astrali. 
12 poteri ESP. chiaroveggenza, visione a distanza, oltre lo spazio o il tempo. Comprensione e invenzione di strumenti ottici come cannochiali e telescopi. Capacità di prevedere le invenzioni e le meraviglie del progresso futuro. Capacità di materializzare oggetti proveneinti dal futuro. (piccoli, e di piccola entità. Niente cambiamenti catastrofici che posson devastar il continuum.) sfere di cristallo. NOTA: non è possibile divulgare o dare alle persone comuni gli oggetti che vengono trascinati da altri tempi e luoghi, per prevenirne un abuso e uso sbagliato e sconsiderato. 
A meno che non sia mistiche a loro volta, le persone spitate dai poteri ESP non si renderanno conto di esserlo. Monari 
13 fisica e chimica: produzione di gas di ogni tipo. Assorbimento, liquefazione e ghiacciazione dell'acqua. Criomanzia e aeromanzia. Controllo della nebbia e della pioggia. Trasformazione dei liquidi in solidi o gas. Insomma, il potere del guardiano degli Stati. 
14 capacità di rilevare e misurare raggi ed energie terrestri per mezzo di bacchette e strumenti. Geomanzia.  
15 parsimonia, e  predizione di pericoli, caristie, brutti periodi e oscuri momenti. Capacità di risparmiare beni e risorse in vista del duro inverno, e dei momenti più oscuri dell'esistenza. Gelioti. 
16 botanica occulta, magia, geomanzia. 
17 conoscenza dei cristalli: ghiaccio, sale, rocce, quarzi, gemme preziose, capacità di produrli e generarli. Avanzata: capacità di trasmutare il proprio stesso corpo in forma cristallina. Geomanzia literese insomma. 
Druidi del gelo gelioti
18 speleologia, minatori, ingegneri minerali. Conoscenza del carbone, delle miniere, delle tecniche di estrazione e lavorazione dei minerali. Sapienza nanica, sapienza dei literesi. 
19 floromanzia. Erboristeria e medicina delle piante. Capacità di ottenere pozioni curative anche per le malattie più gravi dalle erbe, essenze e quintessenze. Magia dei Forlok. 
20 talento come cacciatore, scalatore, rocciatore e alpinista. Prestazioni fisiche eccezionali. Arcieri arcani 
21 custode della Divina Giustizia: controllo delle azioni dei poteri negativi nell'intera zona che circonda la terra e sul nostro pianeta in tutti e tre i piani, difesa dalla magia nera e dalle forze negative. Capacità di bilanciare il male col bene. Armonia e giustizia. 
22 speranza, sostegno delle persone misere o disperate. Lotta dell'angoscia e della delusione. Questo potere permette di impedire agli altri di suicidarsi o di rovinare la propria esistenza nel vittimismo e nell'autolesionismo. Intuzioni che fanno rinunciare ai gesti distruttivi, presa di coscienza, maturazione e consapevolezza. Le persone afflitte da depressione profonda posson esser salvate, redente, guidate a nuova vita, con una nuova consapevolezza di se stesse. Nel caso di persone morte suicide o tragicamente, si prende cura delle loro anime e fa si che possan esser salvate dal limbo e dalla disperazione, permettendo loro di apprendere e consocere comunque ciò che dovevano imparare dal cammino terreno. Se necessario, vengon guidate a nuova incarnazione. 
Comprensione del fatto che non è possiible cambiare il proprio destino tramite suicidio o autolesionismo. Si prolunga solo inutilmente la sofferenza. Kwarg. 
23 aeromanzia: controllo dell'aria, del vento, delle condizioni meteorologiche, del gelo. Freddo, caldo, calore, secco, temproali, gelate, meteorologia. Influsso del clima sulla vegetazione e sulle altre creature. Magia degli Arlaan. 
24 conoscenza della materia e dei suoi effetti chimici e fisici. Alchimia Literese. 
25 ° : custodia di tutte le chiave e dei segreti magici, che non posson esser ottenuti da persone stolte e immature. Nasconde e custodisce tutto ciò che deve esser segregato e protetto. Magia d'ombra. Di tenebra. 
26° sostegno magico nei momenti di grande angoscia. Capacità di salvare la vita al mago da morte certa, ma può esser compiuto tale atto solo una volta. Dono dell'invisibilità, o riflessi sovrumani. o capacità di abbattere avversari o nemici terribili o mettersi in salvo da pericoli mortali (insomma tipo spirith death). Bhezegoti, druidi guerrieri, Sfondatori Literesi 
Gli effetti si realizzan in decimi di secondo. Questa abilità NON può esser usata dagli evocatori. 
27° Capricorno: magia sonare: acustica musica, bardi e druidi sonari. Erudizione sull'uso di strumenti musicali, dai tom tom primitvi fino alla radio e al telefono moderni, alla musica e alle telecomunicazioni. Capacità di ascoltare da grandi distanze i fenomeni. Controllo dei suoni, produzione dei suoni e delle vibvrazioni acustiche. 
Se il mago raggiunge un certo grado di perfezione in questo, può materializzare questo fenomeno in modo tale che persino le persone magicamente non addestrate sono in grado di ascoltare le parole pronunciate con le loro orecchie fisiche. Al mago verranno fornite anche informazioni esatte su molti altri fenomeni che si riferiscono alla trasposizione acustica su distanze.
28 ° Capricorno: scultura di creta, gesso e argilla. Costruttori di artefatti e di golem literesi. 
Statue, golem, dono e talento dell'architettura. Magia curativa dei guaritori bruni literesi: capacità di curare per mezzo dell'argilla e dei cataplasmi magici. 
29° direzione e controllo del commercio ittico. Piscicoltura. Navigazione. Abilità dei corsari netariani e degli Okur
30°

ABILITA' E POTERI DELLA FORZA DEGLI SFERX, TURION, GRINDA, ARCANGELI 
0 fluido elettrico e la sua relazione con i piani mentale, astrale e fisico. 
Controllo dell'elettricità e delle invenzioni alla base dell'elettricità. Equilibrio dell'energia elettrica. 
Elettricisti, elettronica. Visione del futuro delle invenzioni che canalizzano e convogliiano l'energia della natura. Tale conoscenza non va mai usata a fini caotici o distruttivi. 
1 invenzioni  e progresso della tecnica. Visione del futuro della tecnica e delle invenzioni datre dall'uso delle macchine fra 50, 100 o 1000 anni. Creazione quindi di tecnologia anacronistica. Intuizione, buone idee in tal senso. 
2 correlazioni metafisiche e metapiscologiche di chimica e biologia. Cura delle malattie dovute ai collegamenti chimici e biochimici. Diagnostica. Comprensione delle qualità chimiche delle sostanze separate. Omeopatia metafisica. Applicazione dei raggi chimici su microcosmo e macrocosmo. 
3 intuizione e ispirazione dei pittori nel mondo fisico, atraverso immagini mentali, intuizione, ispirazione. Capacità di esprimere la vera bvellezza con l'arte della pittura, in base alla propria maturità, sia nel microcosmo che nel macrocosmo. In caso di grande estro e originalità, permete alla pittura di raggiungere la perfezione. Comprensione dei segreti riguardanti la composizione e la stesura dei colori. 
4 comprensione dei misteri della vita e della morte. Comprensione del tempo in cui un bambino può esser procreato per aver determinati talenti. Capacità di educare I bambini prima della loro nascita fisica. Donando loro abilità, facoltà e talenti innati. Comprensione del futuro destino e karma del nascituro, e del ruolo che il bimbo avrà nella vita. 
5 comprensione della mente e della sfera astrale di ogni animale terrestre, acquatico o celeste. 
Capacità di aumentare la consapevolezza degli animali della propria esistenza, e di influenzarli secondo la propria volontà. Ammaestramento degli animali. Empatia verso gli animali. Capacità di sentire e di provare a veder il mondo attraverso gli animali. Comprensione ed evocazione di famigli o della sorte che tocca alle anime degli animali dopo il loro trapasso fisico. 
6 comprensione dei simboli. facoltà di esprimere ogni forma esterna, in questo mondo fisico, con simboli nelle sue equivalenze mentali e astrali analoghi a le leggi del microcosmo e del macrocosmo. Espressione di leggi e qualità aldildà della loor forma esteriorre. 
Capacità di interpretare i sogni e di spiegare qualsiasi sogno. Comprensione della natura delle creature che dimorano nelle varie sfere dell'eisstenza. Nessuna creatura o spirito può ingannare il soggetto. Tutte le creature extraplanari con cui si hanno a che fare son costrette ad assumere l'aspetto più consono alle loro vere qualità ed essenza.
7 comunicazione. filosofia dell'occultismo. Comprensione delle leggi del mondo terreno. Eloquenza. Capacità di esprimere I concetti più difficili nel linguaggion verbale, fisico, intellettivo. Capacità di far comprendere anche I temi più occulti, filosofici e complessi anche ai profani di una disciplina. 
Intuzioni e ispirazioni aiutano ad esprimere i problemi in modo tale da evitare incomprensioni, equivoci, e malintesi. Miglioramento delle abilità mnemoniche e percettive, e compensazione di memoria e percezione carente. 
8 Equilibrio, armonia. leggi dell'armonia sul piano terreno, microcosmo e macrocosmo. Equilibrio magico.Capacità di raggiunger l'armonia di se stessi e con le altre persone. Capacità di creare armonia ed equilibrio. Dominio su conflitto e caos. Armonia magica fra i principi e le energie arcane elemnentali. 
9 Analogia. Tassonomia.  Capacità di comprendere e valutare le leggi dell'analogia nel mondo fisico e negli altri piani. Valutazione corretta degli esseri di qualsiasi sfera. Capacità di riconoscerei talenti di ogni essere, donati dalla Divina Provvidenza, Nessun essere può fingere di aver talenti o di sapere far qualcosa di cui invece è inabile. Classificazione e identificazione di qualsiasi essere dal suo aspetto esterno come colore, tono, forma, ecc.
10° capacità di penetrare e influenzare il cervello e l'intelletto umano nel modo più sottile. 
Incarnazione nelle facoltà intellettuali di qualsiasi persona. Capacità di esser attivi nello stato di coscienza di qualcun altro. Insomma.. possessione, controllo mentale. 
Capacità di aumentare o diminuire le facoltà mentali altrui. Capacità di richiamare ricordi e immagini del passato della mente di un'altra persona, o da provocare la rimozione totale dei ricordi. 
Dono ottenuto solo da persone con grande eticam, perche' non devon mai essere usate per il male o per distruggere un'altra mente. Capacità di conoscere tutto ciò che conosce un'altra persona, accedendo alle sue memorie. 
11° telepatia, trasferimento del pensiero e di immagini mentali ad altre persone. Capacità di suscitare nel corpo astrale deglle persone non contaminate dalla magia qualità speciali. 
12 diplomazia di successo. Carriera diplomatica. Segreti di mediazione, capacità di condurre anche le trattative più ostiche e difficili. 
13 letterautura filosofica. Capacità di procurarsi I libri giusti per tutto ciò che gli interessa della filosofia, scienza etc segugio di collezionabili. Per quanto concerne libri o conoscenze. 
14 ispirazione nella scrittura. Scelta delle parole e delle espressioni. Aiuto nella poesia. 
15 bellezza, armonia e splendore esterno. Cura della bellezza. Comprensione di modi e mezzi per rendersi belli, attraneti e affascinanti. 
16 favore delle persone importanti. Capacità di farsi amare da donne e da uomini, a patto che non segua le sue opinioni individuali ma si abbandoni all'ispirazione.  
17 segreti dell'amore. Dall'amore animale più infimo all'amore cosmico più alto. 
18 magia gesticolazione e ritualistica. Obbedienza da parte delle altre persone per mezzo dei gesti. Gesti per evocare Il potere dei piani. 
19 danza occulta, in particolare per quanto riguarda le danze con lo scopo di mettere in atto determinati poteri o di entrare in contatto con loro. Intuizione su qali danze suscitano facoltà straordinarie. Estasi mentale per mezzo dei balli. 
20 controllo su tutti coloro che si occupano di leggi e questioni legali. Custodia dei diritti umani, di giudici e avvocati. Difesa e protezione contro sentenze ingiuste. 
21 profezia e predizione.  Sagacia e intuizione. Chieromanzia, grafologia. Etc. 
22: artisti che lavorano per il pubblico:  attori teatrali, artisti circensi, virtuosi ecc. ispirazione nell'arte equestre, nel teatro, nella recitazione, nell'intrattenimento. Capacità di farsi amare dal pubblico. Successo nello spettacolo. Capacità di ispirare successo ad un'artista. 
23 casualità mentale e astrale di sentimenti, qualità di carattere, azioni fisiche, eventi etc. conoscenza della casualità. Capacità di prevedere il destino sul piano fisico, mentale e astrale(esito delle azioni, conseguenza finale). Preveggenza degli ostacoli che si frappongono alla realizzazione delle azioni e degli intenti. Comprensione dei metodi per eliminar e eridurre tali ostacoli. 
24 storia delle religioni, conoscenza di tutte le religioni che esistono nel mondo o che non esistono più. Comprensione esoteriche delle antiche sette e confraternite.
25 protezione di cerchi magici e mistici assocazioni e congreghe. Difesa dei propri colleghi. 
creazione, controllo, mantenimento e al decadimento di tali associazioni. Ottenimento di tutte le informazioni necessarie per comprendere tali associazioni o congreghe. Capacità di manipolarle. 
26 risoluzione dei problemi difficili per mezzo di libri, manoscritti e simili adatti. Capacità di trovare e di comprendere l'identità del proprio maestro arcano. E come fare a raggiungerlo. La qualità dell'insegnamento e della risoluzione dei problemi è proporzionale alla qualità e alla maturtià della persona che si cerca. 
27 analogie segrete, del microcosmo e del macrocosmo. Connessione e relazione della propria anima, spirito e corpo con l'universo. Comprensione di tale fenomeno. Uso pratico di tale conoscenza. 
28  connessione fra tutti i mistici e gli esoterici. Analogia magica. Capacità di attirare poteri superiori su questioni ed eventi esterni. Magia della natura connessa a luoghi o creature sacre. Capacità di attingere all'energia di luoghi e santuari. 
29 protezione di tutti coloro che praticano glin sport acquatici. 
Metodi e istruzioni appropriati, per primeggiare e sviluppare facoltà straordinarie nello sport; per esempio, registrazioni di velocità e nuoto a distanza, di nuoto sott'acqua, ecc. 
salvataggio di annegati o dispersi in mare. Protezione dall'annegamento. (mi sa che con houdini' ha fatto cilecca ) 

ABILITA' E POTERI DELLA FORZA DEI FALKOR, LOKAY,  GARLANE, CHERUBINI
0 FP_SABERTHROW
capacità di affilare le cose: forgiaturqa di coltelli, asce, spade e tutte le altre armi bianche. Tagliatura di pietre e gemme. Lo strumento necessario per l'affilatura e il taglio, la pietra per affilare, ha raggiunto la sua perfezione moderna grazie all'ispirazione di questo grado. Talento nel tagliare, affilare, macinare. 
1 FP_RAGE, 
maestria nella guerra: capacità di dirigere con successo le guerre e a vincerle. Difesa e protezione contro i nemici più ostici. Sopravvivenza durante un conflitto. Strategia. Difesa della pace. Militari, forze armate, polizia. 
2: FP_LEVITATION, 
immaginazione, creatività. 
3 : FP_STUN  
 poesia, giudizio e talento di comporre. Espressione dei problemi spirituali in versi e poesie, in modo da poterli analizzare  e risolvere in modo più semplice. 
4 FP_PROTECT,
Divina Misericordia. Protezione da ogni problema e da ogni male, nei limiti della legalità. Comprensione dei limiti della legalità e della misericordia, della linea che non va mai oltrepassata nel proprio opportunismo. 
5: FP_DARKGLYPH . 
filosofia cosmica dalle origini ai giorni nostri. Divinzione delle filosofie future. Capacità di accumulare una comprensione delle filosofie altrimenti inimmaginabile. 
6:  FP_GRIP,
mummificazione. Contenimento dell'entropia e della dissoluzione del corpo astrale e fisico. 
Preservazione dalla vecchiaia e dal destino. Prolungamneto della vita per il periodo desiderato. 
Resistenza alle malattie. Invulnerabilità ottenibile contro fuoco, acqua, veleno, o capacità di vivere senza cibo per molti anni, o di rimanere energico e resistente per tutto il tempo che vuole. 
Tuttavia si è comunque soggetti alla Divina Provvidenza e al fato ineluttabile. Immortali, non vuol dire invincibili. 
7: FP_PULL
L'abilità manuale. abilità professionali e dell'art dei lavori manuali. Precisione estrema dei movimenti e duttilità delle mani. Mani sopraffine.  
ricami, lavori a maglia, uncinetto, cucito, filatura e tutti gli altri lavori che richiedono una certa abilità manuale. Meccanica. Ingegneria di precisione. 
8  FP_WRACK,
assistenza della ricerca della verità e alla crescita personale. Permette di incontrare o conoscere le persone che guideranno nella crescita e in tale ricerca, guru, guide spirituali o quel che siano. 
9: FP_FREEZE 
rivelazione dei metodi più segreti per lo sviluppo spirituale individuale. Comprensione di quale e metodo segreto deve esser adottato per lo sviluppo di una persona. Custode delle chiavi di segreti magico verbali. Nota: il metodo vale SOLO per la persona interessata. 
10:  FP_SEE,
visione del'aura. magia e radiazione mentale, astrale e fisica. Capacità di discernere dalla radiazione emessa da un essere umano, dalla sua aura, il grado di maturità del suo corpo mentale e astrale. 
11 : FP_TELEPATHY 
applicazione della cabala su piano mentale, astrale e fisico. 
12 : FP_SPEED, 
ispirazione deli esploratori. Secondo lo sviluppo e la maturità dell'umanità, vengono svelati i segreti della natura. Talento nelle scienze naturali e nel progresso scientifico.
13: FP_NECROGLYPH 
ispirazione della scrittura nel campo di misticismo, occultismo e spiritualità. Ispirazioni meravigliose per portare su carta fatti misteriosi nella poesia o in versi meravigliosi. Drammaturgia mistica. 
14: FP_WINDGLYPH 
astrofisica, astronomia, contatti con abitanti di altri mondi. Comprensione del loro sviluppo e della loro maturità fisica, tecnologica e spirituale. 
15: FP_STONEGLYPH  
ricchezza e reputazione. Custodia dei tesori ubicati sulla terra, pietre preziose e tesori nascosti da mani umane. Dona l'abilità del cercatore di tesori. 
16 : FP_HATE,  
iniziazione alla cabala. Spiritualizzazione delle virtù divine nel mondo mentale, astrale e fisico. 
Comprensione dei metodi necessari ad utilizzare appieno le facoltà arcane della cabala. 
17: FP_WATERGLYPH,
magia alchemica. Preparazione di fluidi condensanti per diverse pratiche arcane. Ad alto livello porta conoscenza dell'arte della seduzione, amore, sessualità. 
Capacità di incantare I preparati condensati.  (conoscenza della materia oscura STORM?) 
18 : FP_SUNGLYPH, 
alchimia e magia elettrofisica. Comprensione dei fluidi elettromagnetici sull'oscillazione elettrica dei metalli. Trasmutazione dei metalli, superconduttori. Stati speciali della materia. Effetti producibili dapprima su piano fisico, poi astrale e mentale. 
19 FP_HEAL
cura delle malattie incurabili. Medicine alchemiche, spagiche, farmacologia avanzata. Creazione delle medicine per mezzo del fluido elettromagnetico. Cura alchemica. 
20: FP_ABSORB,  
protezione da ognin influenza negativa, che siano veleni, elementali, spiriti inferiori etc. 
conoscenza della misura di protezione adatta da usare per fronteggiare correttamente l'avversario in questione. Creazione e incantamenti a scopo difensivo di Talismani, armature, parafulmini arcani etc. 
21 FP_HEALOTHER 
giustizia e ingiustizia. Difesa delle persone in tribunale. Conforto delle persone perseguitate, innocenti, prigionieri etc. benedizione, benevolenza, pace interiore, aiuto nelle situazioni difficili. 
Aumento della forza di volontà.
22 FP_HOLYGLYPH,  
comprensione e saggezza, relativi al mistero cosmico della Creazione. 
23 FP_SOUNDGLYPH  
comprensione della musica delle sfere celesti. Abilità di esprimere tale musica celestiale con il canto, come anche idee e pensieri terreni. Orecchio musicale eccezionale, capace di distinguere le singole note. Percezione dei suoni oltre la soglia uditiva. 
24 FP_PUSH
controllo del rispetto dei giuramenti, portati davanti ad un tribunale, di lealtà, di amore, o verso il divino. Comprensione del senso di onore e dell'impegno che deriva dal prestare un guramento. 
Negativo: Capacità di infrangere un giuramento senza conseguenze karmiche
25: FP_LIGHTNING,  
magia rituale universale. Magia balistica. Riti individuali, riti sociali, demiurgia., analogia cosmica. Capacità di lanciare proiettili magici.
26: FP_CONTROLMIND,
comprensione dei metodi per vagare mentalmente e astralmente nei 3 piani. 
Viaggi verso le altre sfere, e le altre realtà. Comprensione delle leggi e misteri di altri mondi e piani di esistenza. Gestione magica di tali leggi. 
27 : FP_FORCEGLYPH
segreti della cabala delle sfere. Lavoro pratico con metodi appropriati riguardanti la magia delle sfere in tutti e tre gli aerei. Applicazione pratica di tali magie e relative possibilità.
28 : FP_FIREGLYPH,  
comprensione delle leggi dell'alchimia sintetica, della magia e della cabala. Arrichimento della conoscenza. Sintesi delle sostanze e degli elementi arcani artificiali. 
29: FP_DRAIN,
grado speciale. Custode di tutti gli alti misteri dell'alchimia. Capacità di impregnare seme e sangue dell'uomo con la magia. 


ABILITA' E POTERI DELLA FORZA DEI RAGAN, MIYA, ZAYON, SERAFINI 
0 incremento dei poteri creativi. Insegnamento di magia e poteri. Condensazione dei fluidi. Idromanzia di base. 
1 facilitazione delle difficoltà. Creazione di invenzioni che sollevano dalla fatica del lavoro manuale. Ibomanzia di base. 
2 invenzioni agricole. Preveggenza del futuro dell'agricoltura. Comprensione di come allevare e far cresere meglio le creature vegetali. Alghicoltura. Controllo delle alghe. 
3 controllo delle azioni, del Yoga e del Karma. Praticare il vero Karma-Yoga significa compiere buone azioni per nessun altro motivo se non per se stessi e in nessun modo al fine di ottenere qualsiasi tipo di ricompensa per farlo. Puro altruismo. Ricompensa mistica dell'altruismo. L'altruismo attira amore cosmico e karma positivo. Comprensione di tale verità. 
4 Tutti i tipi di arti, spettacoli, piaceri, benessere e felicità rientrano nella competenza. Aiuto nel divertimento e negli svaghi, alf ine di rilassarsi dallo studio arcano o intellettuale o dalla fatica del duro lavoro materiale. 
5 successo professionale e finanziario nelle arti e nel mestieri. 
6 successo e fortuna in tutti I campi della scienza, rapido apprendimento di ogni lavoor intellettuale. 
Capacità di ottenere facilmente il materoale necessario agli studi. Onniscienza nei campi della sapienza che si desidera apprendere, fino a che non vi saranno più segreti in tal senso. Ciò però si ottiene solo con la fede e la devozione più pura all'ideale della conoscenza. Studi privati. 
7 acquisizione di nozioni e sapienza scolastica e pubblica. Aiuto nel superare esami, interrogazoni, esami e corsi. 
8 Viaggio. Protezione dagli incidenti durante I viaggi, che sia per terra o per mare o via cielo. 
9 felicità, ricchezza e onore senza influenzare negativamente il karma. 
10 magia della luce. Comprensione della luce sul piano mentale, astrale e fisico. Che cosa è davvero la luce? 
11 proiezione della mente e del corpo astrale nelle altre sfere e negli altri piani di esistenza, anche al di là dei pianeti del proprio sistema solare. Viaggio mentale.
12 conoscenza della Verità. Capacità di trovare e ad acquisire libri per sapienza e conoscenza per effetto della sincronicità. 
13,ideale dell'arte in tutta la sua bellezza creativa. ispira scrittori, giornalisti, editori, poeti e altri artisti nel loro lavoro e li aiuta a ottenere successo in tutte le loro azioni. 
14 comprensione del linguaggio mimico e gestuale. Capacità di leggere il braille, di comunicazione con I sordo muti o con persone o creature che non comprendono o parlano la propria lingua per mezzo del linguaggio gestuale, mimico e facciale. Lettura delle labbra. 
Per I ragan: Eco comunicazione con animali e creature marine. Capacità di comunicare sott'acqua per gesti, o linguaggio eco. 
15  grande capacità di creare e di portare la pace. Diplomazia delle nazioni, famiglia, matrimonio, capacità di risolvere ogni screzio. Amore caritatevole e misericordioso del prossimo. Aiuto nel fare amicizia. Empatia. Capacità di ottenere amicizia degli altri. 
16 cura della sterilità femminile. Comprensione dei mezzi per determinare in anticipo sesso e caratterstiche dei nascituri.cura della sterilità e aumento della fertilità. Capacità di mantenere la propria viralità-fertilità anche in età anziana. 
17 conoscenza dei metodi segreti per fabbricare gli amuleti e i talismani più efficaci per servire amore, matrimonio e amicizia. Incantamento. Conoscenza degli elementi arcani. Capacità di entrar in contatto con gli esseri degli altri pianeti, adattando il corpo astrale all'interazione e comunicazione con loro. (più facile con gli spiriti di tali mondi che con i loro abitanti terreni). Questo potere può spiegare perche' i Cranion volgion rivolgersi ai Ragan e al loro imperatore dio per salvare la vita della madre di Yalish, ridotta in fin di vita da un Ovlon, mentre è incinta del'Eletto.  
18 grado ispiratore di invenzioni chimiche, soprattutto farmaceutiche. A seconda del suo grado di maturità, consente a questa o quell'innovazione in farmacia di essere conosciuta attraverso una persona adatta. Predilezione per medicina e chirurgia mentale, odontoiatria. 
“non solo l'uomo sarà quindi in grado di realizzare denti artificiali con materiali resistenti, ma nuove fantastiche invenzioni consentiranno di coltivare denti naturali negli adulti. Non appena l'umanità avrà raggiunto un certo grado di maturità, e persone idonee avranno rivelato il segreto per far cadere i denti cattivi da sole, senza la necessità di un'estrazione dolorosa, dovuta all'applicazione di determinate sostanze farmaceutiche. Successivamente, la persona interessata crescerà denti nuovi e sani in pochissimo tempo. L'uomo diventerà così padrone della crescita dei suoi denti. Lo stesso sarà il caso dei capelli dell'uomo. I capelli che diventano grigi o che cadono saranno quindi qualcosa che appartiene al passato. Ogni persona sarà in grado di scegliere il colore per i suoi capelli che gli piace, senza dover applicare alcuna tinta per capelli. Sebbene tutto ciò possa sembrare una fiaba e mettere in dubbio il lettore scettico, è la pura verità che verrà dimostrata dal futuro.”
19 sviluppo dell'intelletto. Crescita, arguzia, abilità oratoria. 
20 fortuna: felicità, ricchezza, successo materiale e soddisfazione. Comprensione di grandi tesorin di conoscenza spirituale. Chiedi e ti sarà dato insomma.  
21 protezione di persone molto importanti, condanna dei calunniatori e degli imbroglioni che voglion avvalersi di un innocente in tribunale. Se la persona è colpevole, questo grado garantisce clemenza e comprensione da parte della corte, (se non si tratta di crimini gravi). Difesa di tutti coloro che son esposti a gravi pericoli. 
22 comprensione della scienza naturale, delle leggi di natura: animale, vegetale e minerale. 
Comprensione profondità di queste leggi e loro applicazione magica. Rende amici ed esploratori della Natura. 
23 meditazione, e tramite essa, pace interiore e possibilità di realizzare e compiere prodigi incredibili. Necessario grande maturità e fede per sbloccare questa abilitòà. 
Evocazioni di subordinati. (ibomanzia, evocazione pesci oggetto) 
24 lettura del Karma. Comprensione di tutto il passato, presente e futuro di una persona, e della sua vera natura spirituale in ogni piano. Talento eccelso riservato a pochi. 
25 acquisizione dalle sfere e dai pianeti dei loro poteri in modo cabalistico. Gli effetti si rispecchaino nella realtà materiale. Potere difficilissimo da ottenere. 
26 rivelazione dei misteri del mkicrocosmo e macrocosmo. Capacità di acquisire la comprensione sacra senza dover scegliere fra santità e indiviualità magica. Vengon ricevute dalla  Divina Provvidenza determinate missioni e compiti che devono svolgere nel mondo terreno e che rendono 
insegnanti o assistenti dell'umanità. Sostanzialmente, realizzazione di grandi opere terrene per volere divino, per mandato, e non per egoismo. 
27 iniziazione e insegnamento del misticismo della cabala. Capacità di svilupparsi su piano mentale, astrale e fisico. Capacità di ottenere virtù divine perfette. Santità. 
Chi segue questi metodi segreti diventa sempre più maturo nell'adempiere con attenzione ad alcuni compiti secondo la volontà della Divina Provvidenza. Elevazione del mistico a livelli paragonabili alla divinità. (per carità -.- ) 
28° rivelazione dei misteri più segreti del'amore divino. Concessione di tale potere su piano mentale, astrale e fisico. Estasi e felicità e appagamento. Pace interiore. Santità? 
29 ° comprensione dell'interconnessione fra tuti gli elementi arcani e i fluidi dell'intero microcosmo e macrocosmo, tutte le sfere e su tutti i pianeti della zona che cingono la terra in tutti e tre i piani - mentale, astrale e fisico. Comprensione dell'applicaizone magica di tali effetti. 
Comprensione di composti chimici e e sostanze originali esistenti su altri pianeti, nonché dei loro effetti e influenza, di fatto completamente sconosciuti sulla nostra terra. Applicazione di tali sostanze astraliper scopi magici. Onniscienza, onnipotenza.Sostanzialmente, elevazione ad Orlan? Ad essere metafisico? Diventare così potente da dover rispondere solo al volere divino e all'ordine cosmico? 
 

ABILITA' E POTERI DELLA FORZA DEGLI ANTWER, LUDA , ANGELI CUSTODI 
0 calore e applicazione su alchimia e sparigica. Preparazione di piante e metalli e fluidi. Modifica di luce, tono, oscillazione. Alchimia. 
1 vulcani e terremoti. Capacità di prevederle in anticipo. Viaggi astrali sotto le profondità della terra, sorgenti termali, grott e con stalattiti o tesori nascosti o qualsivoglia cosa posta sotto la superficie della terra. Pirotecnica, fluidi elettrici e magnetici . Fuochi d'artificio magici. 
2 segreti erotici. Suscitare o sminuire l'amore. Capacità di farsi amare o attrarre il sesso opposto dal punto di vista erotico. Magia sessuale. Feromoni. 
3: simpatia-magia mummiale. Fluidi elettrici e magnetici. Conservazione del corpo, creazione di mummie? Capacità di produrre effetti imprevedibili e impensabili. 
4 °Costituzione forte e sana, robustezza, viso bello, attraente, occhi affascinanti. Abilità di mantenersi fisicamente giovani e piacenti anche in età avanzata. Capacità di infusione del corpo di sostanze che lo preservan dal derioramente. Capacità di apparire come una persona di mezza età anche se vecchio.preparazione di elisir magici da varie piante. 
5 ° Cancro) - fatti reali sul cosiddetto "messaggio attraverso l'aria" che è particolarmente noto in Oriente. Capacità di percepire tramite i flussi dell'aria suoni, parole di eventi che avvengono anche a grande distanza. Capacità di inviare tali messaggi anche in specifiche fonti. Capacità di compiere tale potere anche sul piano mnetale e astrale. In caso di grande esperienza può esser compiuto anche sul piano fisico. Capacità di provocare fenonemi arcani a distanza. 
6 ° Cancro: misteri e sistemi originali delle credenze religiose dei popoli più antichi del mondo. Magia delle persone delle ere passate, specialmente di sommi sacerdoti e sciamani. Capacità di riscoprire tali arti e di risvegliare tali antichissimi fenomeni. 
levitazione, controllo del fluido elettromagnetico. Capacità di contrastare la gravità e di volare. 
Capacità di rendere persone e oggetti leggeri come una piuma. 
Capacità di camminare sull'acqua senza affondare, o di librarsi in aria. 
Levitazione degli oggetti. 
7 ° ritmo, vibrazione. Capacità di percepirli e di comprendere quelli di ogni piano  e sfera esistenziale. Comprensione della vita. Perche' la vita è ritmo e vibrazione singergica di tutte le parti delle cellule. Vita e suono. 
8°: facoltà occulte, chiaroveggenza, psicometria e simili. Fabbricazione di specchi magici e loro incantamento. Cabala degli specchi. Aumento delle facoltà acquisite. Evocazione di famigli e di spiriti famigliari. Capacità di trattarli. Evocazione e dominio degli spiriti di controllo. 
9 ° controllo del tempo atmosferico per mezzo della magia dell'acqua o dei fluidi elettrici. Capacità di scatenare la pioggia o di farla cessare, di evocare fulmini, scatenare tempeste di grandine, come anche placarle. 
10°  iniziazione magica. Radiazione arcana. Saggeza universale, illuminazione e luce magica. Sostegno e sapienza alla gandalf. Luce magica per scacciar ombre o esplorare luoghi oscuri. 
11° trasmutazione di poteri magici, piante, minerali, metalli. Alchimia. 
12° facoltà di trovare, in qualsiasi momento, i mezzi giusti per esprimere e spiegare ciò che potrebbe essere comunicato nel linguaggio cosmico e nel linguaggio degli esseri e delle intelligenze. Linguaggio intellettuale. Linguaggio universale. Capacità di farsi capire da ogni creatura. 
13 °magia di tenebra. Invisibilità. Capacità di mimetizzarsi e proiettarsi in tutte le sfere. Capacità di camuffarsi fisicamente. 
14 °: smaterializzazione e rimaterializzazione degli oggetti.  NON permette di spostarli. 
15 °: fenomeni cosmici. Virtu divine, legalità microcosmo e macrocomo. Comprensione delle leggi divine. 
16° comprensione di come vengono applicati l''amore, la misericordia e tutte le virtù divine parallele nel rispetto della loro legalità, armonia e giustizia, in tutte le sfere, nei piani e nel mondo fisico. Teurgia e suoi segreti. Comprensione degli effetti karmici e cabalistici di tutte le sfere. 
Emanazione divina. 
17° comprensione dei segreti delle più sottili analogie sull'evoluzione del microcosmo e del macrocosmo. Conoscenza dell'atto di creazione. Comprensione dello scopo dell'esistenza di ogni cosa e delle finalità della Divina Provvidenza. .
18 ° Cancro: formule magiche e comprensione del loro potere sul piano mentale, astrale e fisico. 
Protezione da influenze negative e indesiderate. 
19° Cancro: evocazione con la mente di esseri subordinati e minion per proteggrersi da 
diversi pericoli, cattiveria e simili. Effettuare un'evocazione nella zona che circonda la terra con il solo corpo mentale è molto più difficile che nel mondo fisico, poiché lì non vi è sostegno alcuno per la coscienza.. comprensione della natura delle evocazioni. 
20 °comprensione di causa, scopo ed effetto di diverse leggi. Profonda saggezza. Sommi sacerdoti arcani. 
21° capacità di porre in essere i propri poteri sul piano mentale, astrale e fisico. 
Penetrazione profonda nell'Emanazione Divina, e nella Divina Provvidenza.
22 ° magia dello spazio. Padronananza del tempo, non solo nel mondo terreno, ma anche nelle sfere macrococmische. Controllo del tempo e dello spazio. Potere concesso solo a maghi davvero maturi. 
23 ° conoscenza e controllo dei vari gradi di oscillazione delle virtù, qualità e poteri divini che avvolgono il mondo. Cosa significa questa facoltà, non può essere spiegato con parole semplici; può essere afferrato solo da chi la pratica. 
24 ° esaminazione della Divina Provvidenza usando metodi e procedure di purificazione appropriati. Correlazione con la Luce Divina, la prima emanazione della Divina Provvidenza. Non è necessario menzionare che questa possibilità porta alle più alte illuminazioni. (serafini?) 
25 ° alleanze cosmiche all'interno delle sfere interplanetarie da visioni diverse, indipendentemente dal fatto che si verifichino in trance, durante il sonno o nei sogni ad occhi aperti; o se sono causati dall'aiuto di specchi magici. Comprensione delle cause che avvengono nelle sfere planetarie e degli effetti che si producono sull'esistenza. 
26 ° magia delle sfere cosmiche. Evocazione degli esseri di altre sfere cosmiche, come se venisse fatto nella realtà fisica. Magia cerimoniale. Comprensione delle leggi che vanno rispettate per compiere tali rischiosi riti. 
27 ° cabala e teurgia divina. Uso nei nomi divini nella cabala. virtù della Divina Emanazione, sulla sua causa ed effetto in ogni sfera planetaria, dal punto di vista cabalistico. 
28 °comprensione dei vari sistemi di saggezza. Rivelazione del modo individuale di viaggiare quando raggiunto un certo grado di maturità. Protezione della saggezza e delle sue chiavi. 
29 ° comprensione del modo in cui le operazioni magiche hanno la loro causa diretta nel mondo spirituale. E su come da lì, si propagano e producon effetti sugli altri piani, astrale, mentale e fisico. Comprensione delle leggi di causa ed effetto e delle leggi karmiche. 



ABILITA' E POTERI DELLA FORZA DEGLI SKEELDER, VORD, REDENTORI, GIUSTIZIERI, SENTINELLE
0 fuoco e magma terrestre. Elemento, fuoco, salamandre, uso pratico del fuoco. 
1 elettricità. Conoscenza e Invenzioni sull'elettricità. Capacità di realizzare invenzioni elettriche, che sfruttano l'energia dell'elettricità, in anticipo sui tempi. Fluido elettrico. Divinazione sulle macchine e le energie del futuro. 
2: istinto di autoconservazione. Istinti animali di autodifesa. Evocazione di animali custodi della natura per difenderla dai pericoli. Ispirazione degli istinti animali. Spinta alla lotta per la sopravvivenza, alla resistenza e alla diligenza. Forza di sopravvivenza. 
Capacità di infondere tale fortisismo impulso di sopravvivenza ad altre persone. Segreti dell'intelletto umano. 
3° protezione dell'amore dei bambini, dell'amore materno e dell'amore dei genitori e di tutto ciò che tiene unita una famiglia. Risoluzione delle liti e delle discordie delle famiglie. 
4 mammiferi e latte. Protezione delle infermiere, delle ostetrici, delle madri che allattano I bambini. 
Mungitura, latte come bevanda, fabbricazione di prodotti caseari. Lotta alle malattie per mezzo del latte. Segreti del latte e della lavorazione del latte. 
5 idealismo umano. Ideali fisici, mentali o astrali. Causa situazioni che rendono possibile la realizzazione di un'ideale sui 3 piani, in piccola o grande misura. Entusiasmo, resitenza necessari alla realizzazione degli ideali, secondo il proprio fato e maturità. 
In negativo: alterazione, cambiamento o scomparsa degli ideali. Divinazione degli ideali del futuro.
6: intelletto e facoltà intellettive, proporzionali all'età fisica, mentale e maturità delle persone. 
Conoscenza. Comprensione del compito che una persona deve adempiere nel mondo terreno. 
Illuminazione dell'intelletto, memoria straordinaria. Accesso a tutta la conoscenza che può esser afferrata dalla mente umana. 
7 protezione da tutte le malattie infettive e capacità di autocura. Ressistenza alle infezioni. 
Se le ragioni karmiche lo rendono necessario, o se risiede nel destino di una persona, non protegge da una infenzione. Conoscenza di molti mezzi di protezione contro i più diversi tipi di malattie infettive dal punto di vista magico. Poiché esiste anche la possibilità di influenza mentale o astrale, istruisce a proteggersi, dopo aver fatto conoscenza con speciali pratiche magiche, contro influssi negativi e agenti patogeni.  
8° perfezionamento della coscienza spirituale. Controllo e protezione dalle ossessioni, dalla follia, da qualsiasi altro fattore psichico sfavorevole. Mente fredda, pragmatica. Comunicazione con l'invisibile di qualsiasi sfera. Viaggio mentale e astrlale. Elevazione del corpo mentale e astrale nei vari piani. 
9medicina magica, taumaturgia. Capacità di trasfondere la propria energia e magnetismo vitale agli altri, rivitalizzandoli senza correre rischi. In senso negativo: vampirismo, succhiare energia agli altri. 
10 alchimia, trasmutazione dei metalli a secco. Realizzazione della pietra filosofale. 
11 iniziazione alla magia. Influenza cabalistica di uomini e animali. 
12 sole è vita. L'uomo è stato ispirato da questa idea da questo grado. Utilizzo del potere del sole per trarre energia o per sostentare la vita. Uso dei raggi solari per mezzo di filtri colorati 
per abolità mediche o guaritrici. Fototerapia con luci colorate. Cromoterapia. Se condotte in armonai a le leggi unviersali, le terapie che usanon la luce e l'energia solare hanno migliore capacità rispetto alle altre forma di medicina. Metodi magici di cura per mezzo della luce colorata. 
Magia della luce iridescente, e dei colori, in tutte le loro forme. 
13 imitazione nel mondo fisico. Imitazione delle gemme. Creazione di vetro, porcellana, pelle artificiale, gomma, plastica e tutti i prodotti sintetici. Divinazione delle invenzioni sintetiche che devon ancora venire. Arte imitatoria. Mimica. Ne consegue che i robot, come imitatori dell'umanità e delle creature animali, cadono sotto questo grado. 
14 invenzione di scherzi, battute, eventi sportivi, giochi per adulti e piccini. Giochi di carte, giochi sociali, giochi da tavolo. Giocattoli per bambini. È il grado di elzevir the dollmaker XD
15 apicoltura e miele. Dolci ottenuti dal miele. Zucchero, barbabietola dal sud, canna da zucchero d'oriente, industria dolciaria. Invenzione e produzione di miele, zucchero, dolci e sostanze simili. 
Trattamento di epatite, diabete e delle malattie causate da eccesso o carenza di zucchero nel pancreas e nel fegato. 
Ma che è? Il grado di willy wonka???
16 coltivazione della frutta. Frutta cruda frutta elaborata, frutta conservata. Cottura dei cibi, cuocere il pane, trovare e inventare nuovi piatti a base di carne. Gastronomia, cottura e cucina dei cibi. 
17 amore erotico, simpatia. Attrazione fra uomo e donna. Seduzione, ammaliamento. Evocazione della simpatia della persona desiderata, mantenimento della pace in famiglia. Magia dell'amore. Grado ammaliante, sensuale. 
18  chimica metallurgica,. Tecniche di estrazione e raffinazione dei metalli dai mienrali. 
Riconoscimento dei metalli. Sintesi dei metalli, leghe, fusione dei metalli. Decomposizione dei metalli in varin prodotti chimici, sostanze sintetiche, medicine. 
19 medicine estratte dai metalli e dalle loro combinazioni. Farmaci. Medicinali sintetici. Farmacologia moderna. Uso dei metalli per scopi medicinali. 
20 realizzazione dei desideri. Cambiare in buone le cattive condizioni di vita. Capacità di ottenere la felicità, di elevarsi dalla miseria. Comprensione dei modi e dei mezzi necessari per migliorare il proprio tenore di vita. Comprensione di quanto il destino sarà condizionato karmicamente da questo, e in che misura la Divina Provvidenza lo consente. Capacità di migliorare il destino di altre persone. 
21 intuizione, consulenza e assistenza per aiutare a fronteggiare a sangue freddo le situazioni più difficili della vita normale, senza fuggirne. Capacità di trovare il rimedio giusto per ogni situazione difficile. 
22 potere della fede e della convinzione. Creazione di grandi meraviglie nel campo fisico, mentale e astrale per mezzo della fede e della convinzione. Fiducia in se stessi? Negli altri? O in Dio? Non è specificato. 
23 diagnosi e analisi. Comprensione della causa di tutte le malattie del corpo astrale, mentale e fisico. Conoscenza dei mezzi per rimuovere tali cause e disarmonie. Medicina diagnostica. 
(ha senso questo, perche' questi gradi si trovano in seno alla costellazione dell'Ofiuco) 
24 medicina naturale in tutte le sue forme: erbe, dal trattamento delle acque, cura del corpo o altri metodi applicati nella medicina naturale, o da quegli oggetti che fino ad oggi sono rimasti completamente non rivelati. 
25 teurgia, cabbala, guarigione dalle malattie più gravi e mortali. Formule e incantesimi per la cura di malattie gravi. Esorcismi, miracoli, rottura di maledizioni o fatture. 
26 astronomia, orientamento, magnetismo. Percezione del campo magnetico naturale. Invenzione delle costellazioni, dei punti cardinali. Conoscenza della navigazione. Invenzione di bussola e ago magnetico. Capacità di trovare nuovi metodi, nuovi poteri e nuove invenzioni per orientarsi. Creazione di dispositivi di misurazione, igrometri, barometri, profondità del mare, pressione dell'acqua, etc. 
27 Trattamento delle acque. Idroterapia. Utilizzo del fluido elettromagnetico in connessione all'acua per curare le malattie e riconfigurare la struttura molecolare delle cellule. Stimolazione termica, armonia e salute del corpo. Conoscenza di nuovi metodi di idroterapia. 
28 idromanzia, poteri dell'acqua. Creazione artificiale dell'acqua. Elettrolisi. Irrigazione del suolo, deviazione dei fiumi, mulini ad acqua, centrali idroelettriche, costruzione di canali di irrigazione. 
Scoperta di nuovi metodi di regolare il flusso delle acque nel futuro. 
29 °: pesca, controllo e dominio sulle creature acquatiche. Capacità di catturare senza difficoltà le creature marine di ogni tipo. Richiamare e raccogliere pesci a mani nude. Capacità di domare e ipnotizzare qualsiasi tipo di animale acquatico, perfino squali, coccodrilli, rospi. 
*/

