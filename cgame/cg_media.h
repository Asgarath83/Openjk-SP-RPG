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

#ifndef __CG_MEDIA_H_
#define __CG_MEDIA_H_

#define	NUM_CROSSHAIRS		9

typedef enum {
	FOOTSTEP_STONEWALK,
	FOOTSTEP_STONERUN,
	FOOTSTEP_METALWALK,
	FOOTSTEP_METALRUN,
	FOOTSTEP_PIPEWALK,
	FOOTSTEP_PIPERUN,
	FOOTSTEP_SPLASH,
	FOOTSTEP_WADE,
	FOOTSTEP_SWIM,
	FOOTSTEP_SNOWWALK,
	FOOTSTEP_SNOWRUN,
	FOOTSTEP_SANDWALK,
	FOOTSTEP_SANDRUN,
	FOOTSTEP_GRASSWALK,
	FOOTSTEP_GRASSRUN,
	FOOTSTEP_DIRTWALK,
	FOOTSTEP_DIRTRUN,
	FOOTSTEP_MUDWALK,
	FOOTSTEP_MUDRUN,
	FOOTSTEP_GRAVELWALK,
	FOOTSTEP_GRAVELRUN,
	FOOTSTEP_RUGWALK,
	FOOTSTEP_RUGRUN,
	FOOTSTEP_WOODWALK,
	FOOTSTEP_WOODRUN,

	FOOTSTEP_TOTAL
} footstep_t;

#define ICON_WEAPONS	0
#define ICON_FORCE		1
#define ICON_INVENTORY	2

#define MAX_HUD_TICS	4


typedef struct HUDMenuItem_s
{
	const char		*menuName;
	const char		*itemName;
	int				xPos;
	int				yPos;
	int				width;
	int				height;
	vec4_t			color;
	qhandle_t		background;
} HUDMenuItem_t;
extern HUDMenuItem_t healthTics[];
extern HUDMenuItem_t armorTics[];
extern HUDMenuItem_t ammoTics[];
extern HUDMenuItem_t forceTics[];
extern HUDMenuItem_t otherHUDBits[];


typedef enum 
{
	OHB_HEALTHAMOUNT = 0,
	OHB_ARMORAMOUNT,
	OHB_FORCEAMOUNT,
	OHB_AMMOAMOUNT,
	OHB_SABERSTYLE_STRONG,
	OHB_SABERSTYLE_MEDIUM,
	OHB_SABERSTYLE_FAST,
	OHB_SCANLINE_LEFT,
	OHB_SCANLINE_RIGHT,
	OHB_FRAME_LEFT,
	OHB_FRAME_RIGHT,
	OHB_MAX
} otherhudbits_t;

#define NUM_CHUNK_MODELS	4

enum 
{
	CHUNK_METAL1 = 0,
	CHUNK_METAL2,
	CHUNK_ROCK1,
	CHUNK_ROCK2,
	CHUNK_ROCK3,
	CHUNK_CRATE1,
	CHUNK_CRATE2,
	CHUNK_WHITE_METAL,
	NUM_CHUNK_TYPES
};

// all of the model, shader, and sound references that are
// loaded at gamestate time are stored in cgMedia_t
// Other media that can be tied to clients, weapons, or items are
// stored in the clientInfo_t, itemInfo_t, weaponInfo_t, and powerupInfo_t
typedef struct {
	qhandle_t	charsetShader;
	qhandle_t	whiteShader;

	qhandle_t	crosshairShader[NUM_CROSSHAIRS];
	qhandle_t	backTileShader;
//	qhandle_t	noammoShader;

	qhandle_t	numberShaders[11];
	qhandle_t	smallnumberShaders[11];
	qhandle_t	chunkyNumberShaders[11];

	qhandle_t	loadTick;
	qhandle_t	loadTickCap;

	// ALL FORCE ICON POWERS SHADERS
	// Heals 
	qhandle_t	heal_icon;
	qhandle_t	heal_v_icon;
	qhandle_t	heal_v_icon2;
	qhandle_t	heal_d_icon;
	qhandle_t	heal_h_icon;
	qhandle_t	heal_s_icon; 
	qhandle_t	healother_icon;


	// Jump
	qhandle_t	jump_icon;
	// Speed
	qhandle_t	speed_icon;
	// Telekinesis
	qhandle_t	push_icon;
	qhandle_t	pull_icon;
	qhandle_t	grip_icon; 
	// Sword skills
	qhandle_t	saber_attack; 
	qhandle_t	saber_throw; 
	qhandle_t	saber_defend;
	// Rage
	qhandle_t	icon_rage; 
	qhandle_t	icon_berserk; 
	qhandle_t	icon_trance;
	qhandle_t	icon_fear; 
	qhandle_t	icon_runeshield;
	qhandle_t	icon_tkshield;
	qhandle_t	icon_eldershield;
	qhandle_t	icon_poisonshield; 
	qhandle_t	icon_frostarmor;
	qhandle_t	icon_crystalarmor;
	qhandle_t	icon_windshield;
	qhandle_t	icon_shockshield;
	qhandle_t	icon_stormshield;
	qhandle_t	icon_cruel;
	qhandle_t	icon_deathfear;
	qhandle_t	icon_firerage; 

	// REPELS 
	qhandle_t	icon_protect;
	qhandle_t	icon_protect2;
	qhandle_t	icon_absorb; 
	// DRAINS 
	qhandle_t	icon_bloodgout;
	qhandle_t	icon_bloodshower;
	qhandle_t	icon_souldrain;
	qhandle_t	icon_necrodrain;
	qhandle_t	icon_hyldendrain;
	qhandle_t	icon_demondrain; 
	// VAMPIRES DARK POWERS 
	qhandle_t	icon_light;
	qhandle_t	icon_echo;
	qhandle_t	icon_echo2;
	qhandle_t	icon_revelation;
	qhandle_t	icon_humanrevel;
	qhandle_t	icon_hyldenrevel;
	qhandle_t	icon_elderrevel;
	qhandle_t	icon_stun;
	qhandle_t	icon_feartrick;
	qhandle_t	icon_hate;
	qhandle_t	icon_controlmind;
	qhandle_t	icon_incapacitate; 
	qhandle_t	icon_batform; 
	qhandle_t	icon_birdform; // demonic
	qhandle_t	icon_chyroptera; 
	qhandle_t	icon_mistform; 
	qhandle_t	icon_mistform2; // fog of fear
	qhandle_t	icon_mistform3; // demonic
	qhandle_t	icon_wolfform; 
	qhandle_t	icon_wolfform2; // double head
	qhandle_t	icon_hellwolf; // metalupo
	qhandle_t	icon_lionform; // lion 
	qhandle_t	icon_battovampire;
	qhandle_t	icon_misttovampire; 
	qhandle_t	icon_wolftovampire; 
	// Sarafan sun gift
	qhandle_t	icon_holystun; 
	qhandle_t	icon_holyhate;
	qhandle_t	icon_holycontrolmind; 
	qhandle_t	icon_holyincapacitate; 
	qhandle_t	icon_holywrack; 
	qhandle_t	icon_holyfear; //? 

	// special stasis power 
	qhandle_t	icon_frostcoffin;
	qhandle_t	icon_webbed; 
	qhandle_t	icon_stoned; 
	qhandle_t	icon_hollystasis;
	qhandle_t	icon_ivytrap; 
	qhandle_t	icon_gravitytrap; 

	qhandle_t	icon_proxy; 
	qhandle_t	icon_fear2;
	qhandle_t	icon_wrack;
	// GLYPH POWERS 
	qhandle_t	icon_forceglyph;
	qhandle_t	icon_stoneglyph;
	qhandle_t	icon_soundglyph;
	qhandle_t	icon_waterglyph;
	qhandle_t	icon_fireglyph;
	qhandle_t	icon_sunglyph;
	// THE REAVERS 
	qhandle_t	icon_darkreaver;
	qhandle_t	icon_smogreaver;
	qhandle_t	icon_voidreaver;

	qhandle_t	icon_lightreaver;
	qhandle_t	icon_lightningreaver;
	qhandle_t	icon_sunreaver;
	qhandle_t	icon_firereaver;
	qhandle_t	icon_warmreaver;
	qhandle_t	icon_magmareaver;
	qhandle_t	icon_airreaver;
	qhandle_t	icon_fogreaver;
	qhandle_t	icon_stormreaver;
	qhandle_t	icon_waterreaver;
	qhandle_t	icon_icereaver;
	qhandle_t	icon_crystalreaver;
	qhandle_t	icon_earthreaver;
	qhandle_t	icon_mudreaver;
	qhandle_t	icon_stonereaver;
	qhandle_t	icon_dustreaver;
	qhandle_t	icon_poisonreaver;
	qhandle_t	icon_deathreaver;
	qhandle_t	icon_spectralreaver;
	qhandle_t	icon_spiritreaver;
	qhandle_t	icon_holyreaver;
	qhandle_t	icon_lifereaver;
	qhandle_t	icon_soundreaver;
	qhandle_t	icon_astralreaver;
	// THE PILLARS 
	qhandle_t	icon_mindreaver;
	qhandle_t	icon_dimensionreaver;
	qhandle_t	icon_conflictreaver;
	qhandle_t	icon_naturereaver;
	qhandle_t	icon_energyreaver;
	qhandle_t	icon_timereaver;
	qhandle_t	icon_statesreaver;
	qhandle_t	icon_deathreaver2;
	qhandle_t	icon_soulreaver;
	qhandle_t	icon_bloodreaver;
	qhandle_t	icon_balancereaver; 
	// ARCANE ATTACKS 
	qhandle_t	icon_energybolt;
	qhandle_t	icon_lightning;
	qhandle_t	icon_lightning2; 
	qhandle_t	icon_web;
	qhandle_t	icon_ghost; 	
	qhandle_t	icon_life;
	qhandle_t	icon_hollyfire;
	qhandle_t	icon_necro;
	qhandle_t	icon_psychic;
	qhandle_t	icon_crystal;
	qhandle_t	icon_stone;
	qhandle_t	icon_water;
	qhandle_t	icon_fire;
	qhandle_t	icon_skull;
	qhandle_t	icon_lava;
	qhandle_t	icon_poison;
	qhandle_t	icon_hell;
	qhandle_t	icon_hylden;
	qhandle_t	icon_greenlightning;
	qhandle_t	icon_yellowlightning;
	qhandle_t	icon_wind;
	qhandle_t	icon_stormwind;
	qhandle_t	icon_eterform;
	qhandle_t	icon_mistoffear;
	qhandle_t	icon_burst;
	qhandle_t	icon_ice;
	qhandle_t	icon_dark;
	qhandle_t	icon_void;
	qhandle_t	icon_smog;
	qhandle_t	icon_sonic;
	qhandle_t	icon_acid;
	qhandle_t	icon_astral;
	qhandle_t	icon_fireball;
	qhandle_t	icon_holly;
	
	//			HUD artwork
	int			currentBackground;
	qhandle_t	weaponbox;
	qhandle_t	weaponIconBackground;
	qhandle_t	forceIconBackground;
	qhandle_t	inventoryIconBackground;
	qhandle_t	turretComputerOverlayShader;
	qhandle_t	turretCrossHairShader;

//Chunks
	qhandle_t	chunkModels[NUM_CHUNK_TYPES][4];
	sfxHandle_t	chunkSound;
	sfxHandle_t	grateSound;
	sfxHandle_t	rockBreakSound;
	sfxHandle_t	rockBounceSound[2];
	sfxHandle_t	metalBounceSound[2];
	sfxHandle_t	glassChunkSound;
	sfxHandle_t	crateBreakSound[2];

	// Saber shaders
	//-----------------------------
	qhandle_t	forceCoronaShader;
	qhandle_t	saberBlurShader;
	qhandle_t	swordTrailShader;
	qhandle_t	yellowDroppedSaberShader; // glow

	qhandle_t	redSaberGlowShader;
	qhandle_t	redSaberCoreShader;
	qhandle_t	orangeSaberGlowShader;
	qhandle_t	orangeSaberCoreShader;
	qhandle_t	yellowSaberGlowShader;
	qhandle_t	yellowSaberCoreShader;
	qhandle_t	greenSaberGlowShader;
	qhandle_t	greenSaberCoreShader;
	qhandle_t	blueSaberGlowShader;
	qhandle_t	blueSaberCoreShader;
	qhandle_t	purpleSaberGlowShader;
	qhandle_t	purpleSaberCoreShader;
	qhandle_t	red2SaberGlowShader;
	qhandle_t	red2SaberCoreShader;
	qhandle_t	whiteSaberGlowShader;
	qhandle_t	whiteSaberCoreShader;
	qhandle_t	whiteblueSaberGlowShader;
	qhandle_t	whiteblueSaberCoreShader;
	qhandle_t	green2SaberGlowShader;
	qhandle_t	green2SaberCoreShader;
	qhandle_t	cyanSaberGlowShader;
	qhandle_t	cyanSaberCoreShader;
	qhandle_t	brownSaberGlowShader;
	qhandle_t	brownSaberCoreShader;
	qhandle_t	darkSaberGlowShader;
	qhandle_t	darkSaberCoreShader;

	qhandle_t	explosionModel;
	qhandle_t	surfaceExplosionShader;

	qhandle_t	halfShieldModel;

	qhandle_t	solidWhiteShader;
	qhandle_t	electricBodyShader;
	qhandle_t	electricBody2Shader;
	qhandle_t	refractShader;
	qhandle_t	boltShader;
	qhandle_t	redray;
	// Disruptor zoom graphics
	qhandle_t	disruptorMask;
	qhandle_t	disruptorInsert;
	qhandle_t	disruptorLight;
	qhandle_t	disruptorInsertTick;

		// Sniper Rifle MASKS
	qhandle_t	sniperMask;
	qhandle_t	sniperInsert;
	qhandle_t	sniperLight;
	qhandle_t	sniperInsertTick;

	qhandle_t	sniperMask2;
	qhandle_t	sniperInsert2;
	qhandle_t	sniperLight2;
	qhandle_t	sniperInsertTick2;

	qhandle_t	sniperMask3;
	qhandle_t	sniperInsert3;
	qhandle_t	sniperLight3;
	qhandle_t	sniperInsertTick3;

	qhandle_t	sniperMask4;
	qhandle_t	sniperInsert4;
	qhandle_t	sniperLight4;
	qhandle_t	sniperInsertTick4;

	qhandle_t	sniperMask5;
	qhandle_t	sniperInsert5;
	qhandle_t	sniperLight5;
	qhandle_t	sniperInsertTick5;

	qhandle_t	sniperMask6;
	qhandle_t	sniperInsert6;
	qhandle_t	sniperLight6;
	qhandle_t	sniperInsertTick6;

	qhandle_t	sniperMask7;
	qhandle_t	sniperInsert7;
	qhandle_t	sniperLight7;
	qhandle_t	sniperInsertTick7;

	qhandle_t	sniperMask8;
	qhandle_t	sniperInsert8;
	qhandle_t	sniperLight8;
	qhandle_t	sniperInsertTick8;

	qhandle_t	sniperMask9;
	qhandle_t	sniperInsert9;
	qhandle_t	sniperLight9;
	qhandle_t	sniperInsertTick9;

	qhandle_t	sniperMask10;
	qhandle_t	sniperInsert10;
	qhandle_t	sniperLight10;
	qhandle_t	sniperInsertTick10;

	qhandle_t	sniperMask11;
	qhandle_t	sniperInsert11;
	qhandle_t	sniperLight11;
	qhandle_t	sniperInsertTick11;

	qhandle_t	sniperMask12;
	qhandle_t	sniperInsert12;
	qhandle_t	sniperLight12;
	qhandle_t	sniperInsertTick12;



	// Binocular graphics
	qhandle_t	binocularCircle;
	qhandle_t	binocularMask;
	qhandle_t	binocularArrow;
	qhandle_t	binocularTri;
	qhandle_t	binocularStatic;
	qhandle_t	binocularOverlay;

	// LA Goggles graphics
	qhandle_t	laGogglesStatic;
	qhandle_t	laGogglesMask;
	qhandle_t	laGogglesSideBit;
	qhandle_t	laGogglesBracket;
	qhandle_t	laGogglesArrow;

	// wall mark shaders
	qhandle_t	scavMarkShader;
	qhandle_t	rivetMarkShader;

	qhandle_t	shadowMarkShader;
	qhandle_t	wakeMarkShader;
	qhandle_t	fsrMarkShader;
	qhandle_t	fslMarkShader;
	qhandle_t	fshrMarkShader;
	qhandle_t	fshlMarkShader;

	qhandle_t	damageBlendBlobShader;

	// fonts...
	//
	qhandle_t	qhFontSmall;
	qhandle_t	qhFontMedium;

	// special effects models / etc.
	qhandle_t	personalShieldShader;
	qhandle_t	cloakedShader;

	// Interface media
	qhandle_t	ammoslider;
	qhandle_t	emplacedHealthBarShader;

	qhandle_t	dataPadFrame;
	qhandle_t	DPForcePowerOverlay;

	qhandle_t	bdecal_burnmark1;
	qhandle_t	bdecal_saberglowmark;

	qhandle_t	messageLitOn;
	qhandle_t	messageLitOff;
	qhandle_t	messageObjCircle;

	qhandle_t	batteryChargeShader;
	qhandle_t	useableHint;

	qhandle_t	levelLoad;

	//new stuff for Jedi Academy
	//force power icons
//	qhandle_t	forcePowerIcons[NUM_FORCE_POWERS];
	qhandle_t	rageRecShader;
	qhandle_t	playerShieldDamage;
	qhandle_t	forceSightBubble;
	qhandle_t	forceShell;
	qhandle_t	runeShell;
	qhandle_t	sightShell;
	qhandle_t	drainShader;

	// sounds
	sfxHandle_t disintegrateSound;
	sfxHandle_t disintegrate2Sound;
	sfxHandle_t disintegrate3Sound;

	sfxHandle_t	grenadeBounce1;
	sfxHandle_t	grenadeBounce2;

	sfxHandle_t	flechetteStickSound;
	sfxHandle_t	detPackStickSound;
	sfxHandle_t	tripMineStickSound;
	sfxHandle_t	cannonStickSound;
	
	sfxHandle_t	selectSound;
	sfxHandle_t	selectSound2;
	sfxHandle_t	overchargeSlowSound;
	sfxHandle_t overchargeFastSound;
	sfxHandle_t	overchargeLoopSound;
	sfxHandle_t	overchargeEndSound;

//	sfxHandle_t	useNothingSound;
	sfxHandle_t	footsteps[FOOTSTEP_TOTAL][4];

//	sfxHandle_t talkSound;
	sfxHandle_t	noAmmoSound;
	
	sfxHandle_t landSound;
	sfxHandle_t rollSound;
	sfxHandle_t messageLitSound;

	sfxHandle_t	batteryChargeSound;

	sfxHandle_t watrInSound;
	sfxHandle_t watrOutSound;
	sfxHandle_t watrUnSound;

	sfxHandle_t lavaInSound;
	sfxHandle_t lavaOutSound;
	sfxHandle_t lavaUnSound;

	sfxHandle_t noforceSound;

	// Zoom
	sfxHandle_t	zoomStart;
	sfxHandle_t	zoomLoop;
	sfxHandle_t	zoomEnd;
	sfxHandle_t	disruptorZoomLoop;
	
	//new stuff for Jedi Academy
	sfxHandle_t	drainSound;

} cgMedia_t;


// Stored FX handles
//--------------------
typedef struct
{
	// BLASTER PISTOL EFX
	// DARKNESS
	fxHandle_t	darkShotEffect;
	fxHandle_t	darkPowerupShotEffect;
	fxHandle_t	darkWallImpactEffect;
	fxHandle_t	darkWallImpactEffect2;
	fxHandle_t	darkWallImpactEffect3;
	fxHandle_t	darkFleshImpactEffect;

	// SMOKE ELEMENT
	fxHandle_t	smokeShotEffect;
	fxHandle_t	smokePowerupShotEffect;
	fxHandle_t	smokeWallImpactEffect;
	fxHandle_t	smokeWallImpactEffect2;
	fxHandle_t	smokeWallImpactEffect3;
	fxHandle_t	smokeFleshImpactEffect;

	// VOID ELEMENT 
	fxHandle_t	voidShotEffect;
	fxHandle_t	voidPowerupShotEffect;
	fxHandle_t	voidWallImpactEffect;
	fxHandle_t	voidWallImpactEffect2;
	fxHandle_t	voidWallImpactEffect3;
	fxHandle_t	voidFleshImpactEffect;

	// DUST 
	fxHandle_t	dustShotEffect;
	fxHandle_t	dustPowerupShotEffect;
	fxHandle_t	dustWallImpactEffect;
	fxHandle_t	dustWallImpactEffect2;
	fxHandle_t	dustWallImpactEffect3;
	fxHandle_t	dustFleshImpactEffect;
	
	// KNIFE ELEMENT
	fxHandle_t	knifeShotEffect;
	fxHandle_t	knifePowerupShotEffect;
	fxHandle_t	knifeWallImpactEffect;
	fxHandle_t	knifeWallImpactEffect2;
	fxHandle_t	knifeWallImpactEffect3;
	fxHandle_t	knifeFleshImpactEffect;

	// KNIFE ELEMENT
	fxHandle_t	knife2ShotEffect;
	fxHandle_t	knife2PowerupShotEffect;
	fxHandle_t	knife2WallImpactEffect;
	fxHandle_t	knife2WallImpactEffect2;
	fxHandle_t	knife2WallImpactEffect3;
	fxHandle_t	knife2FleshImpactEffect;


	// ABSORB ELEMENT 
	fxHandle_t	absorbShotEffect;
	fxHandle_t	absorbPowerupShotEffect;
	fxHandle_t	absorbWallImpactEffect;
	fxHandle_t	absorbWallImpactEffect2;
	fxHandle_t	absorbWallImpactEffect3;
	fxHandle_t	absorbFleshImpactEffect;


	// STUN
	fxHandle_t	stunShotEffect;
	fxHandle_t	stunPowerupShotEffect;
	fxHandle_t	stunWallImpactEffect;
	fxHandle_t	stunWallImpactEffect2;
	fxHandle_t	stunWallImpactEffect3;
	fxHandle_t	stunFleshImpactEffect;

	//MADNESS 
	fxHandle_t	madnessShotEffect;
	fxHandle_t	madnessPowerupShotEffect;
	fxHandle_t	madnessWallImpactEffect;
	fxHandle_t	madnessWallImpactEffect2;
	fxHandle_t	madnessWallImpactEffect3;
	fxHandle_t	madnessFleshImpactEffect;


	//SMOKER 
	fxHandle_t	smokerShotEffect;
	fxHandle_t	smokerPowerupShotEffect;
	fxHandle_t	smokerWallImpactEffect;
	fxHandle_t	smokerWallImpactEffect2;
	fxHandle_t	smokerWallImpactEffect3;
	fxHandle_t	smokerFleshImpactEffect;


	//// For Bow weapon for archer and scout class . act like tusken rifle weapon! 
	// BOW - BRYAR PISTOL
	fxHandle_t	bowShotEffect;
	fxHandle_t	bowPowerupShotEffect;
	fxHandle_t	bowWallImpactEffect;
	fxHandle_t	bowWallImpactEffect2;
	fxHandle_t	bowWallImpactEffect3;
	fxHandle_t	bowFleshImpactEffect;
	// Dark Arrow 
	fxHandle_t	darkarrowShotEffect;
	fxHandle_t	darkarrowPowerupShotEffect;
	fxHandle_t	darkarrowWallImpactEffect;
	fxHandle_t	darkarrowWallImpactEffect2;
	fxHandle_t	darkarrowWallImpactEffect3;
	fxHandle_t	darkarrowFleshImpactEffect;

	// Smoke Arrow 
	fxHandle_t	smokearrowShotEffect;
	fxHandle_t	smokearrowPowerupShotEffect;
	fxHandle_t	smokearrowWallImpactEffect;
	fxHandle_t	smokearrowWallImpactEffect2;
	fxHandle_t	smokearrowWallImpactEffect3;
	fxHandle_t	smokearrowFleshImpactEffect;

	// Void Arrow 
	fxHandle_t	voidarrowShotEffect;
	fxHandle_t	voidarrowPowerupShotEffect;
	fxHandle_t	voidarrowWallImpactEffect;
	fxHandle_t	voidarrowWallImpactEffect2;
	fxHandle_t	voidarrowWallImpactEffect3;
	fxHandle_t	voidarrowFleshImpactEffect;

	// Dust Arrow 
	fxHandle_t	dustarrowShotEffect;
	fxHandle_t	dustarrowPowerupShotEffect;
	fxHandle_t	dustarrowWallImpactEffect;
	fxHandle_t	dustarrowWallImpactEffect2;
	fxHandle_t	dustarrowWallImpactEffect3;
	fxHandle_t	dustarrowFleshImpactEffect;









	// Fire arrow 
	fxHandle_t	firearrowShotEffect;
	fxHandle_t	firearrowPowerupShotEffect;
	fxHandle_t	firearrowWallImpactEffect;
	fxHandle_t	firearrowWallImpactEffect2;
	fxHandle_t	firearrowWallImpactEffect3;
	fxHandle_t	firearrowFleshImpactEffect;
	// Force arrow 
	fxHandle_t	forcearrowShotEffect;
	fxHandle_t	forcearrowPowerupShotEffect;
	fxHandle_t	forcearrowWallImpactEffect;
	fxHandle_t	forcearrowWallImpactEffect2;
	fxHandle_t	forcearrowWallImpactEffect3;
	fxHandle_t	forcearrowFleshImpactEffect;
	// Wind Arrow 
	fxHandle_t	windarrowShotEffect;
	fxHandle_t	windarrowPowerupShotEffect;
	fxHandle_t	windarrowWallImpactEffect;
	fxHandle_t	windarrowWallImpactEffect2;
	fxHandle_t	windarrowWallImpactEffect3;
	fxHandle_t	windarrowFleshImpactEffect;
	// Water Arrow 
	fxHandle_t	waterarrowShotEffect;
	fxHandle_t	waterarrowPowerupShotEffect;
	fxHandle_t	waterarrowWallImpactEffect;
	fxHandle_t	waterarrowWallImpactEffect2;
	fxHandle_t	waterarrowWallImpactEffect3;
	fxHandle_t	waterarrowFleshImpactEffect;
	// Stone Arrow
	fxHandle_t	stonearrowShotEffect;
	fxHandle_t	stonearrowPowerupShotEffect;
	fxHandle_t	stonearrowWallImpactEffect;
	fxHandle_t	stonearrowWallImpactEffect2;
	fxHandle_t	stonearrowWallImpactEffect3;
	fxHandle_t	stonearrowFleshImpactEffect;
	// Sonic Arrow
	fxHandle_t	sonicarrowShotEffect;
	fxHandle_t	sonicarrowPowerupShotEffect;
	fxHandle_t	sonicarrowWallImpactEffect;
	fxHandle_t	sonicarrowWallImpactEffect2;
	fxHandle_t	sonicarrowWallImpactEffect3;
	fxHandle_t	sonicarrowFleshImpactEffect;
	// Electric Arrow
	fxHandle_t	shockarrowShotEffect;
	fxHandle_t	shockarrowPowerupShotEffect;
	fxHandle_t	shockarrowWallImpactEffect;
	fxHandle_t	shockarrowWallImpactEffect2;
	fxHandle_t	shockarrowWallImpactEffect3;
	fxHandle_t	shockarrowFleshImpactEffect;
	// Holy Arrow 
	fxHandle_t	holyarrowShotEffect;
	fxHandle_t	holyarrowPowerupShotEffect;
	fxHandle_t	holyarrowWallImpactEffect;
	fxHandle_t	holyarrowWallImpactEffect2;
	fxHandle_t	holyarrowWallImpactEffect3;
	fxHandle_t	holyarrowFleshImpactEffect;
	// Necrotic Arrow 
	fxHandle_t	necroarrowShotEffect;
	fxHandle_t	necroarrowPowerupShotEffect;
	fxHandle_t	necroarrowWallImpactEffect;
	fxHandle_t	necroarrowWallImpactEffect2;
	fxHandle_t	necroarrowWallImpactEffect3;
	fxHandle_t	necroarrowFleshImpactEffect;
	// Poison Arrow 
	fxHandle_t	poisonarrowShotEffect;
	fxHandle_t	poisonarrowPowerupShotEffect;
	fxHandle_t	poisonarrowWallImpactEffect;
	fxHandle_t	poisonarrowWallImpactEffect2;
	fxHandle_t	poisonarrowWallImpactEffect3;
	fxHandle_t	poisonarrowFleshImpactEffect;
	// Vampiric Arrow 
	fxHandle_t	vamparrowShotEffect;
	fxHandle_t	vamparrowPowerupShotEffect;
	fxHandle_t	vamparrowWallImpactEffect;
	fxHandle_t	vamparrowWallImpactEffect2;
	fxHandle_t	vamparrowWallImpactEffect3;
	fxHandle_t	vamparrowFleshImpactEffect;

	// Glyphic Arrow 
	fxHandle_t	glypharrowShotEffect;
	fxHandle_t	glypharrowPowerupShotEffect;
	fxHandle_t	glypharrowWallImpactEffect;
	fxHandle_t	glypharrowWallImpactEffect2;
	fxHandle_t	glypharrowWallImpactEffect3;
	fxHandle_t	glypharrowFleshImpactEffect;

	// JAWA - SONIC PISTOL
	fxHandle_t	howlerShotEffect;
	fxHandle_t	howlerPowerupShotEffect;
	fxHandle_t	howlerWallImpactEffect;
	fxHandle_t	howlerWallImpactEffect2;
	fxHandle_t	howlerWallImpactEffect3;
	fxHandle_t	howlerFleshImpactEffect;
	// ASTRAL - ASTRAL PROJECTILES
	fxHandle_t	astralShotEffect;
	fxHandle_t	astralPowerupShotEffect;
	fxHandle_t	astralWallImpactEffect;
	fxHandle_t	astralWallImpactEffect2;
	fxHandle_t	astralWallImpactEffect3;
	fxHandle_t	astralFleshImpactEffect;


	// BLOODGUN - PROPHET PISTOL
	fxHandle_t	bloodgunShotEffect;
	fxHandle_t	bloodgunPowerupShotEffect;
	fxHandle_t	bloodgunWallImpactEffect;
	fxHandle_t	bloodgunWallImpactEffect2;
	fxHandle_t	bloodgunWallImpactEffect3;
	fxHandle_t	bloodgunFleshImpactEffect;

	

	// ENTROPY ELEMENT
	fxHandle_t	entropyShotEffect;
	fxHandle_t	entropyPowerupShotEffect;
	fxHandle_t	entropyWallImpactEffect;
	fxHandle_t	entropyWallImpactEffect2;
	fxHandle_t	entropyWallImpactEffect3;
	fxHandle_t	entropyFleshImpactEffect;




	// BLASTER
	fxHandle_t	blasterShotEffect;
	fxHandle_t	blasterOverchargeEffect;
	fxHandle_t	blasterWallImpactEffect;
	fxHandle_t	blasterFleshImpactEffect;
	
	// WARM
	fxHandle_t	warmShotEffect;
//	fxHandle_t	warmOverchargeEffect;
	fxHandle_t	warmWallImpactEffect;
	fxHandle_t	warmFleshImpactEffect;

	// MAGMA
	fxHandle_t	magmaShotEffect;
//	fxHandle_t	magmaOverchargeEffect;
	fxHandle_t	magmaWallImpactEffect;
	fxHandle_t	magmaFleshImpactEffect;

	// Vamp fire 
	fxHandle_t	vampfireShotEffect;
	//fxHandle_t	blasterOverchargeEffect;
	fxHandle_t	vampfireWallImpactEffect;
	fxHandle_t	vampfireFleshImpactEffect;

	// hylden fire
	fxHandle_t	greenfireShotEffect;
//	fxHandle_t	warmOverchargeEffect;
	fxHandle_t	greenfireWallImpactEffect;
	fxHandle_t	greenfireFleshImpactEffect;

	// Hell fire
	fxHandle_t	hellfireShotEffect;
//	fxHandle_t	magmaOverchargeEffect;
	fxHandle_t	hellfireWallImpactEffect;
	fxHandle_t	hellfireFleshImpactEffect;

	// Witched fire
	fxHandle_t	ghostfireShotEffect;
//	fxHandle_t	magmaOverchargeEffect;
	fxHandle_t	ghostfireWallImpactEffect;
	fxHandle_t	ghostfireFleshImpactEffect;

	// poison fire
	fxHandle_t	poisonfireShotEffect;
//	fxHandle_t	magmaOverchargeEffect;
	fxHandle_t	poisonfireWallImpactEffect;
	fxHandle_t	poisonfireFleshImpactEffect;

	// BOWCASTER
	fxHandle_t	bowcasterShotEffect;
	fxHandle_t	bowcasterBounceEffect;
	fxHandle_t	bowcasterImpactEffect;

	// FULMINE
	fxHandle_t	shockShotEffect;
	fxHandle_t	shockBounceEffect;
	fxHandle_t	shockImpactEffect;

	// RADIANZA 
	fxHandle_t	sunShotEffect;
	fxHandle_t	sunBounceEffect;
	fxHandle_t	sunImpactEffect;

	// ENERGY BOLT
	fxHandle_t	ebShotEffect;
	fxHandle_t	ebBounceEffect;
	fxHandle_t	ebImpactEffect;
	fxHandle_t	eb2ImpactEffect;
	
	// LAME DI LUCE
	fxHandle_t	lightknifeShotEffect;
	fxHandle_t	lightknifeBounceEffect;
	fxHandle_t	lightknifeImpactEffect;
	fxHandle_t	lightknife2ImpactEffect;

	// TESCHI DI LUCE
	fxHandle_t	lightskullShotEffect;
	fxHandle_t	lightskullBounceEffect;
	fxHandle_t	lightskullImpactEffect;
	fxHandle_t	lightskull2ImpactEffect;
	// FOLLIA   
	fxHandle_t	madShotEffect;
	fxHandle_t	madBounceEffect;
	fxHandle_t	madImpactEffect;
	fxHandle_t	mad2ImpactEffect;

	// EARTH
	fxHandle_t	flechetteShotEffect;
	fxHandle_t	flechetteAltShotEffect;
	fxHandle_t	flechetteShotDeathEffect;
	fxHandle_t	flechetteFleshImpactEffect;
	fxHandle_t	flechetteRicochetEffect;
	// FANGO
	fxHandle_t	mudShotEffect;
	fxHandle_t	mudAltShotEffect;
	fxHandle_t	mudShotDeathEffect;
	fxHandle_t	mudFleshImpactEffect;
	fxHandle_t	mudRicochetEffect;
	// PIETRA 
	fxHandle_t	stoneShotEffect;
	fxHandle_t	stoneAltShotEffect;
	fxHandle_t	stoneShotDeathEffect;
	fxHandle_t	stoneFleshImpactEffect;
	fxHandle_t	stoneRicochetEffect;
	
/*	fxHandle_t	dustShotEffect;
	fxHandle_t	dustAltShotEffect;
	fxHandle_t	dustShotDeathEffect;
	fxHandle_t	dustFleshImpactEffect;
	fxHandle_t	dustRicochetEffect;*/
	// ASH
	fxHandle_t	ashShotEffect;
	fxHandle_t	ashAltShotEffect;
	fxHandle_t	ashShotDeathEffect;
	fxHandle_t	ashFleshImpactEffect;
	fxHandle_t	ashRicochetEffect;
	// IVY 
	fxHandle_t	ivyShotEffect;
	fxHandle_t	ivyAltShotEffect;
	fxHandle_t	ivyShotDeathEffect;
	fxHandle_t	ivyFleshImpactEffect;
	fxHandle_t	ivyRicochetEffect;
	// HELLISH IVY
	fxHandle_t	ivy2ShotEffect;
	fxHandle_t	ivy2AltShotEffect;
	fxHandle_t	ivy2ShotDeathEffect;
	fxHandle_t	ivy2FleshImpactEffect;
	fxHandle_t	ivy2RicochetEffect;
	// THORN
	fxHandle_t	thornShotEffect;
	fxHandle_t	thornAltShotEffect;
	fxHandle_t	thornShotDeathEffect;
	fxHandle_t	thornFleshImpactEffect;
	fxHandle_t	thornRicochetEffect;
	// SPECTRAL
	fxHandle_t	spectralShotEffect;
	fxHandle_t	spectralAltShotEffect;
	fxHandle_t	spectralShotDeathEffect;
	fxHandle_t	spectralFleshImpactEffect;
	fxHandle_t	spectralRicochetEffect;
	// GREEN CRYSTAL
	fxHandle_t	gcrystalShotEffect;
	fxHandle_t	gcrystalAltShotEffect;
	fxHandle_t	gcrystalShotDeathEffect;
	fxHandle_t	gcrystalFleshImpactEffect;
	fxHandle_t	gcrystalRicochetEffect;
	// STASIS 
	fxHandle_t	stasisShotEffect;
	fxHandle_t	stasisAltShotEffect;
	fxHandle_t	stasisShotDeathEffect;
	fxHandle_t	stasisFleshImpactEffect;
	fxHandle_t	stasisRicochetEffect;
	// ACID 
	fxHandle_t	acidShotEffect;
	fxHandle_t	acidAltShotEffect;
	fxHandle_t	acidShotDeathEffect;
	fxHandle_t	acidFleshImpactEffect;
	fxHandle_t	acidRicochetEffect;
	
	// CANNON
	fxHandle_t	cannonShotEffect;
	fxHandle_t	cannonAltShotEffect;
	fxHandle_t	cannonShotDeathEffect;
	fxHandle_t	cannonFleshImpactEffect;
	fxHandle_t	cannonRicochetEffect;


	//FIGHTER CLASSES FX weapons
	fxHandle_t	stunner0ShotEffect;
	fxHandle_t	stunner0PowerupShotEffect;
	fxHandle_t	stunner0WallImpactEffect;
	fxHandle_t	stunner0WallImpactEffect2;
	fxHandle_t	stunner0WallImpactEffect3;
	fxHandle_t	stunner0FleshImpactEffect;
	fxHandle_t	greenlaser0ShotEffect;
	fxHandle_t	greenlaser0OverchargeEffect;
	fxHandle_t	greenlaser0WallImpactEffect;
	fxHandle_t	greenlaser0FleshImpactEffect;
	fxHandle_t	dardus0ShotEffect;
	fxHandle_t	dardus0BounceEffect;
	fxHandle_t	dardus0ImpactEffect;
	fxHandle_t	shrapnel0ShotEffect;
	fxHandle_t	shrapnel0AltShotEffect;
	fxHandle_t	shrapnel0ShotDeathEffect;
	fxHandle_t	shrapnel0FleshImpactEffect;
	fxHandle_t	shrapnel0RicochetEffect;

	//FORCE BEHAVOUR STATES EFFECTS 
	// Stunning
	fxHandle_t	forceConfusion;
	
	// Arcane attack
	// Electric 
	fxHandle_t	forceLightning;
	fxHandle_t	forceLightningWide;
	
	fxHandle_t	forceDrain;
	fxHandle_t	forceDrainWide;

	//fxHandle_t	forceInvincibility;
	fxHandle_t	forceHeal;
	fxHandle_t	forceRage;
	
	//footstep effects
	fxHandle_t footstepMud;
	fxHandle_t footstepSand;
	fxHandle_t footstepSnow;
	fxHandle_t footstepGravel;
	//landing effects
	fxHandle_t landingMud;
	fxHandle_t landingSand;
	fxHandle_t landingDirt;
	fxHandle_t landingSnow;
	fxHandle_t landingGravel;
} cgEffects_t;


// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournement restart is done, allowing
// all clients to begin playing instantly
#define STRIPED_LEVELNAME_VARIATIONS 3	// sigh, to cope with levels that use text from >1 SP file (plus 1 for common)
typedef struct {
	gameState_t		gameState;			// gamestate from server
	glconfig_t		glconfig;			// rendering configuration

	int				serverCommandSequence;	// reliable command stream counter
	
	// parsed from serverinfo
	int				dmflags;
	int				teamflags;
	int				timelimit;
	int				maxclients;
	char			mapname[MAX_QPATH];
	char			stripLevelName[STRIPED_LEVELNAME_VARIATIONS][MAX_QPATH];

	//
	// locally derived information from gamestate
	//
	qhandle_t		model_draw[MAX_MODELS];
	sfxHandle_t		sound_precache[MAX_SOUNDS];
// Ghoul2 start
	qhandle_t		skins[MAX_CHARSKINS];

// Ghoul2 end

	int				numInlineModels;
	qhandle_t		inlineDrawModel[MAX_SUBMODELS];
	vec3_t			inlineModelMidpoints[MAX_SUBMODELS];

	clientInfo_t	clientinfo[MAX_CLIENTS];

	// media
	cgMedia_t		media;

	// effects
	cgEffects_t		effects;

} cgs_t;

extern	cgs_t			cgs;

#endif //__CG_MEDIA_H_
