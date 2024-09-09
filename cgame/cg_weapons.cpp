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

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"
#include "../game/wp_saber.h"
#include "../game/g_vehicles.h"

#include "../game/anims.h"

extern void CG_LightningBolt( centity_t *cent, vec3_t origin );

#define	PHASER_HOLDFRAME	2
extern void G_SoundOnEnt( gentity_t *ent, soundChannel_t channel, const char *soundPath );
const char *CG_DisplayBoxedText(int iBoxX, int iBoxY, int iBoxWidth, int iBoxHeight, 
								const char *psText, int iFontHandle, float fScale,
								const vec4_t v4Color);


extern void G_PlayEffect( int fxID, const vec3_t origin );
/*


/*
=================
CG_RegisterGun

The server says this Gun is used on this level
=================
*/
// WEAPONDATA diventa GUNINFO 
// WEAPONINFO diventa GUNINFO2 chiaro?!?!?

void CG_RegisterGun( int GunNum )
{
	// GunData is readed by weapons.h and from GUN library
	// the parameters of GunInfo are copyed into GunInfo struct on g_shared.h. 
	GunInfo_t		cg_guns[MAX_GUNS];
	GunInfo_t		*GunInfo;
	gunData_t		*gunData;
	gunData = NULL;
	gitem_t			*gun_item, *gun_ammo;
	char			path[MAX_QPATH];
	vec3_t			mins, maxs;
	int				i;
	GunInfo = &cg_guns[GunNum]; // cggun fix it! 
	// error checking
	if ( GunNum == 0 ) {
		return;
	}

	if ( GunInfo->Gun_Registered ) 
	{
		return;
	}

	// clear out the memory we use
	memset( GunInfo, 0, sizeof( *GunInfo ) );
	GunInfo->Gun_Registered = qtrue;

	// find the gun item in the item list
	for ( gun_item = bg_itemlist + 1 ; gun_item->classname ; gun_item++ ) {
		if ( gun_item->giType == IT_WEAPON && gun_item->giTag == GunNum ) {
			GunInfo->gun_item = gun_item;
			break;
		}
	}
	// if we couldn't find which weapon this is, give us an error
	if ( !gun_item->classname ) 
	{
		CG_Error( "Couldn't find item for gun %s\nNeed to update Items.dat!", gunData[GunNum].Gun_Name);
	}
	CG_RegisterItemVisuals( gun_item - bg_itemlist );
	// set up in view weapon model
	GunInfo->Gun_WeaponModel = cgi_R_RegisterModel( gunData[GunNum].Gun_Model );
	{//in case the weaponmodel isn't _w, precache the _w.glm
		char Gun_Weapon_Model[64];
		// ARRIVATO QUI 
		
		Q_strncpyz (Gun_Weapon_Model, gunData[GunNum].Gun_Model, sizeof(Gun_Weapon_Model));	
		if (char *spot = strstr(Gun_Weapon_Model, ".md3") ) 
		{
			*spot = 0;
			spot = strstr(Gun_Weapon_Model, "_w");//i'm using the in view weapon array instead of scanning the item list, so put the _w back on
			if (!spot) 
			{
				Q_strcat (Gun_Weapon_Model, sizeof(Gun_Weapon_Model), "_w");
			}
			Q_strcat (Gun_Weapon_Model, sizeof(Gun_Weapon_Model), ".glm");	//and change to ghoul2
		}
		gi.G2API_PrecacheGhoul2Model( Gun_Weapon_Model ); // correct way is item->world_model
	}
	if ( GunInfo->Gun_WeaponModel == 0 )
	{
		CG_Error( "Couldn't find gun model %s for gun %s\n", gunData[GunNum].Gun_Model, gunData[GunNum].Gun_Name );
		return;
	}
	
	// calc midpoint for rotation
	cgi_R_ModelBounds( GunInfo->Gun_WeaponModel, mins, maxs );
	for ( i = 0 ; i < 3 ; i++ ) {
		GunInfo->Gun_Midpoint[i] = mins[i] + 0.5 * ( maxs[i] - mins[i] );
	}
	
	// setup the shader we will use for the icon
	// Registra la shader dalla libreria esterna alla libreria interna
/*	if (gunData[GunNum].Gun_Icon[0])
	{
		gunInfo->Gun_Icon = cgi_R_RegisterShaderNoMip( gunData[GunNum].Gun_Icon);
		gunInfo->Gun_IconNoAmmo = cgi_R_RegisterShaderNoMip( va( "%s_na", gunData[GunNum].Gun_Icon));
	} */
	
	// Set Ammo for Gun 
	for ( gun_ammo = bg_itemlist + 1 ; gun_ammo->classname ; gun_ammo++ ) {	
		if ( gun_ammo->giType == IT_AMMO && gun_ammo->giTag == gunData[GunNum].Gun_AmmoIndex) 
		{
			break;
		}
	}
	if ( gun_ammo->classname && gun_ammo->world_model ) {
		GunInfo->Gun_AmmoModel = cgi_R_RegisterModel( gun_ammo->world_model );
	}
	
	for (i=0; i< gunData[GunNum].Gun_NumBarrels; i++) {
		Q_strncpyz( path, gunData[GunNum].Gun_Model, sizeof(path) );
		COM_StripExtension( path, path, sizeof(path) );
		if (i)
		{
			//char	crap[50];
			//Com_sprintf(crap, sizeof(crap), "_barrel%d.md3", i+1 );
			//strcat ( path, crap );
			Q_strcat( path, sizeof(path), va("_barrel%d.md3", i+1) );
		}
		else
			Q_strcat( path, sizeof(path), "_barrel.md3" );
		GunInfo->Gun_BarrelModel[i] = cgi_R_RegisterModel( path );
	}

	// set up the world model for the weapon
	GunInfo->Gun_WeaponWorldModel = cgi_R_RegisterModel( gun_item->world_model );
	if ( !GunInfo->Gun_WeaponWorldModel) 
	{
		GunInfo->Gun_WeaponWorldModel = GunInfo->Gun_WeaponModel;
	}
	// set up the hand that holds the in view weapon - assuming we have one
	Q_strncpyz( path, gunData[GunNum].Gun_Model, sizeof(path) );
	COM_StripExtension( path, path, sizeof(path) );
	Q_strcat( path, sizeof(path), "_hand.md3" );
	GunInfo->Gun_HandsModel = cgi_R_RegisterModel( path );

	if ( !GunInfo->Gun_HandsModel ) 
	{
		GunInfo->Gun_HandsModel = cgi_R_RegisterModel( "models/weapons2/briar_pistol/briar_pistol_hand.md3" );
	}
	// Register Missile Model 
	if (gunData[GunNum].Gun_Missile_Model) 
	{
		GunInfo->Gun_Missile_Model = cgi_R_RegisterModel( gunData[GunNum].Gun_Missile_Model );
	}
	// Register alt missile model 
	if (gunData[GunNum].Gun_Alt_Missile_Model) 
	{
		GunInfo->Gun_Alt_Missile_Model = cgi_R_RegisterModel( gunData[GunNum].Gun_Alt_Missile_Model );
	}
	
	if (gunData[GunNum].Gun_Charge_Sound) 
	{
		GunInfo->Gun_Charge_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Charge_Sound );
	}
	if (gunData[GunNum].Gun_Alt_Charge_Sound) 
	{
		GunInfo->Gun_Alt_Charge_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Alt_Charge_Sound );
	}
	
	
	if (gunData[GunNum].Gun_Select_Sound ) 
	{
		GunInfo->Gun_Select_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Select_Sound );
	}
		
	// give us missile models if we should
	// need to fix these... 
	if (gunData[GunNum].Gun_Missile_Model ) 	
	{
		GunInfo->Gun_Missile_Model = cgi_R_RegisterModel(gunData[GunNum].Gun_Missile_Model );
	}
	if (gunData[GunNum].Gun_Alt_Missile_Model )
	{
		GunInfo->Gun_Alt_Missile_Model = cgi_R_RegisterModel(gunData[GunNum].Gun_Alt_Missile_Model );
	}
	// Loop sound of weapon, like concussion weapon 
	if (gunData[GunNum].Gun_Loop_Sound) 
	{
		GunInfo->Gun_Loop_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Loop_Sound );
	}
	if ( GunNum ) 
	{
		cgs.media.sniperMask		= cgi_R_RegisterShader( gunData[GunNum].Gun_ZoomMask );
		//weaponData[weaponNum].mAltMuzzleEffect
		cgs.media.sniperInsert		= cgi_R_RegisterShader( gunData[GunNum].Gun_ZoomRotateMask );

	}
	// Set Loop Sound of the weapon
	if (gunData[GunNum].Gun_Loop_Sound ) 
	{
		GunInfo->Gun_Loop_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Loop_Sound );
	}
	// Main Missile Sound
	if (gunData[GunNum].Gun_Missile_Sound ) 
	{
		GunInfo->Gun_Missile_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Missile_Sound );
	}
	// Alt Missile Sound
	if (gunData[GunNum].Gun_Alt_Missile_Sound ) 
	{
		GunInfo->Gun_Alt_Missile_Sound = cgi_S_RegisterSound( gunData[GunNum].Gun_Alt_Missile_Sound );
	}


	// register the visual effects of gun library
	/*if (GunInfo[GunNum].Gun_Bullet_Effect ) 
	{
		theFxScheduler.RegisterEffect( GunInfo[GunNum].Gun_Bullet_Effect );
	}

	switch ( GunNum ) //extra client only stuff
	{
		case WP_BLASTER_PISTOL:
		if (GunInfo[GunNum].Gun_Bullet_Effect ) 
		{
			theFxScheduler.RegisterEffect( GunInfo[GunNum].Gun_Bullet_Effect );
		}
		break;
	}*/

	// WEAPONDATA diventa GUNINFO 
// WEAPONINFO diventa GUNINFO2 chiaro?!?!?

	//GunInfo2->Gun_Icon = cgi_R_RegisterShaderNoMip( GunInfo[GunNum].Gun_Icon);
//	GunInfo2->Gun_Bullet_Effect_Registered = theFxScheduler.RegisterEffect( GunInfo[GunNum].Gun_Bullet_Effect );
	
	//weaponInfo->weaponIcon = cgi_R_RegisterShaderNoMip( weaponData[weaponNum].weaponIcon);

}



/*
=================
CG_RegisterWeapon

The server says this item is used on this level
=================
*/
void CG_RegisterWeapon( int weaponNum ) {
	weaponInfo_t	*weaponInfo;
	gitem_t			*item, *ammo;
	char			path[MAX_QPATH];
	vec3_t			mins, maxs;
	int				i;

	weaponInfo = &cg_weapons[weaponNum];

	// error checking
	if ( weaponNum == 0 ) {
		return;
	}

	if ( weaponInfo->registered ) {
		return;
	}

	// clear out the memory we use
	memset( weaponInfo, 0, sizeof( *weaponInfo ) );
	weaponInfo->registered = qtrue;

	// find the weapon in the item list
	for ( item = bg_itemlist + 1 ; item->classname ; item++ ) {
		if ( item->giType == IT_WEAPON && item->giTag == weaponNum ) {
			weaponInfo->item = item;
			break;
		}
	}
	// if we couldn't find which weapon this is, give us an error
	if ( !item->classname ) {
		CG_Error( "Couldn't find item for weapon %s\nNeed to update Items.dat!", weaponData[weaponNum].classname);
	}
	CG_RegisterItemVisuals( item - bg_itemlist );

	// set up in view weapon model
	weaponInfo->weaponModel = cgi_R_RegisterModel( weaponData[weaponNum].weaponMdl );
	{//in case the weaponmodel isn't _w, precache the _w.glm
		char weaponModel[64];
		
		Q_strncpyz (weaponModel, weaponData[weaponNum].weaponMdl, sizeof(weaponModel));	
		if (char *spot = strstr(weaponModel, ".md3") ) 
		{
			*spot = 0;
			spot = strstr(weaponModel, "_w");//i'm using the in view weapon array instead of scanning the item list, so put the _w back on
			if (!spot) 
			{
				Q_strcat (weaponModel, sizeof(weaponModel), "_w");
			}
			Q_strcat (weaponModel, sizeof(weaponModel), ".glm");	//and change to ghoul2
		}
		gi.G2API_PrecacheGhoul2Model( weaponModel ); // correct way is item->world_model
	}

	if ( weaponInfo->weaponModel == 0 )
	{
		CG_Error( "Couldn't find weapon model %s for weapon %s\n", weaponData[weaponNum].weaponMdl, weaponData[weaponNum].classname);
		return;
	}

	// calc midpoint for rotation
	cgi_R_ModelBounds( weaponInfo->weaponModel, mins, maxs );
	for ( i = 0 ; i < 3 ; i++ ) {
		weaponInfo->weaponMidpoint[i] = mins[i] + 0.5 * ( maxs[i] - mins[i] );
	}

	// setup the shader we will use for the icon
	if (weaponData[weaponNum].weaponIcon[0])
	{
		weaponInfo->weaponIcon = cgi_R_RegisterShaderNoMip( weaponData[weaponNum].weaponIcon);
		weaponInfo->weaponIconNoAmmo = cgi_R_RegisterShaderNoMip( va("%s_na",weaponData[weaponNum].weaponIcon));
	}

	for ( ammo = bg_itemlist + 1 ; ammo->classname ; ammo++ ) {	
		if ( ammo->giType == IT_AMMO && ammo->giTag == weaponData[weaponNum].ammoIndex) {
			break;
		}
	}

	if ( ammo->classname && ammo->world_model ) {
		weaponInfo->ammoModel = cgi_R_RegisterModel( ammo->world_model );
	}

	for (i=0; i< weaponData[weaponNum].numBarrels; i++) {
		Q_strncpyz( path, weaponData[weaponNum].weaponMdl, sizeof(path) );
		COM_StripExtension( path, path, sizeof(path) );
		if (i)
		{
			//char	crap[50];
			//Com_sprintf(crap, sizeof(crap), "_barrel%d.md3", i+1 );
			//strcat ( path, crap );
			Q_strcat( path, sizeof(path), va("_barrel%d.md3", i+1) );
		}
		else
			Q_strcat( path, sizeof(path), "_barrel.md3" );
		weaponInfo->barrelModel[i] = cgi_R_RegisterModel( path );
	}


	// set up the world model for the weapon
	weaponInfo->weaponWorldModel = cgi_R_RegisterModel( item->world_model );
	if ( !weaponInfo->weaponWorldModel) {
		weaponInfo->weaponWorldModel = weaponInfo->weaponModel;
	}

	// set up the hand that holds the in view weapon - assuming we have one
	Q_strncpyz( path, weaponData[weaponNum].weaponMdl, sizeof(path) );
	COM_StripExtension( path, path, sizeof(path) );
	Q_strcat( path, sizeof(path), "_hand.md3" );
	weaponInfo->handsModel = cgi_R_RegisterModel( path );

	if ( !weaponInfo->handsModel ) {
		weaponInfo->handsModel = cgi_R_RegisterModel( "models/weapons2/briar_pistol/briar_pistol_hand.md3" );
	}

	// register the sounds for the weapon
	if (weaponData[weaponNum].firingSnd[0]) {
		weaponInfo->firingSound = cgi_S_RegisterSound( weaponData[weaponNum].firingSnd );
	}
	if (weaponData[weaponNum].altFiringSnd[0]) {
		weaponInfo->altFiringSound = cgi_S_RegisterSound( weaponData[weaponNum].altFiringSnd );
	}
	if (weaponData[weaponNum].stopSnd[0]) {
		weaponInfo->stopSound = cgi_S_RegisterSound( weaponData[weaponNum].stopSnd );
	}
	if (weaponData[weaponNum].chargeSnd[0]) {
		weaponInfo->chargeSound = cgi_S_RegisterSound( weaponData[weaponNum].chargeSnd );
	}
	if (weaponData[weaponNum].altChargeSnd[0]) {
		weaponInfo->altChargeSound = cgi_S_RegisterSound( weaponData[weaponNum].altChargeSnd );
	}
	if (weaponData[weaponNum].selectSnd[0]) {
		weaponInfo->selectSound = cgi_S_RegisterSound( weaponData[weaponNum].selectSnd );
	}

	// give us missile models if we should
	if (weaponData[weaponNum].missileMdl[0]) 	{
		weaponInfo->missileModel = cgi_R_RegisterModel(weaponData[weaponNum].missileMdl );
	}
	if (weaponData[weaponNum].alt_missileMdl[0]) 	{
		weaponInfo->alt_missileModel = cgi_R_RegisterModel(weaponData[weaponNum].alt_missileMdl );
	}
	if (weaponData[weaponNum].missileSound[0]) {
		weaponInfo->missileSound = cgi_S_RegisterSound( weaponData[weaponNum].missileSound );
	}
	if (weaponData[weaponNum].alt_missileSound[0]) {
		weaponInfo->alt_missileSound = cgi_S_RegisterSound( weaponData[weaponNum].alt_missileSound );
	}
	if (weaponData[weaponNum].missileHitSound[0]) {
		weaponInfo->missileHitSound = cgi_S_RegisterSound( weaponData[weaponNum].missileHitSound );
	}
	if (weaponData[weaponNum].altmissileHitSound[0]) {
		weaponInfo->altmissileHitSound = cgi_S_RegisterSound( weaponData[weaponNum].altmissileHitSound );
	}
	if ( weaponData[weaponNum].mMuzzleEffect[0] ) 
	{
		weaponData[weaponNum].mMuzzleEffectID = theFxScheduler.RegisterEffect( weaponData[weaponNum].mMuzzleEffect );
	}
	if ( weaponData[weaponNum].mAltMuzzleEffect[0] ) 
	{
		weaponData[weaponNum].mAltMuzzleEffectID = theFxScheduler.RegisterEffect( weaponData[weaponNum].mAltMuzzleEffect );
	}

	//fixme: don't really need to copy these, should just use directly
	// give ourselves the functions if we can
	if (weaponData[weaponNum].func)
	{
		weaponInfo->missileTrailFunc = (void (*)(struct centity_s *,const struct weaponInfo_s *))weaponData[weaponNum].func;
	}
	if (weaponData[weaponNum].altfunc)
	{
		weaponInfo->alt_missileTrailFunc = (void (*)(struct centity_s *,const struct weaponInfo_s *))weaponData[weaponNum].altfunc;
	}

	switch ( weaponNum )	//extra client only stuff
	{
	case WP_SABER:
		//saber/force FX
		theFxScheduler.RegisterEffect( "sparks/spark_nosnd" );//was "sparks/spark"
		theFxScheduler.RegisterEffect( "sparks/blood_sparks2" );
		theFxScheduler.RegisterEffect( "force/force_touch" );
		theFxScheduler.RegisterEffect( "saber/saber_block" );
		theFxScheduler.RegisterEffect( "saber/saber_cut" );
		//theFxScheduler.RegisterEffect( "saber/limb_bolton" );
		theFxScheduler.RegisterEffect( "saber/fizz" );
		theFxScheduler.RegisterEffect( "saber/boil" );
		//theFxScheduler.RegisterEffect( "saber/fire" );//was "sparks/spark"

		cgs.effects.forceHeal			= theFxScheduler.RegisterEffect( "force/heal2" );
	//cgs.effects.forceInvincibility	= theFxScheduler.RegisterEffect( "force/invin" );
		// BEHAVOUR STATES EFFECTS 
		cgs.effects.forceConfusion		= theFxScheduler.RegisterEffect( "force/confusion" );
		// ARCANE ATTACKS EFFECTS		
		cgs.effects.forceLightning		= theFxScheduler.RegisterEffect( "force/lightning" );
		cgs.effects.forceLightningWide	= theFxScheduler.RegisterEffect( "force/lightningwide" );
			
		//new Jedi Academy force power effects
		/*cgs.effects.forceDrain		= theFxScheduler.RegisterEffect( "mp/drain" );
		cgs.effects.forceDrainWide	= theFxScheduler.RegisterEffect( "mp/drainwide" );*/
		//cgs.effects.forceDrained	= theFxScheduler.RegisterEffect( "mp/drainhit");*/
		// register rage effect 
		cgs.effects.forceRage		= theFxScheduler.RegisterEffect( "force/rage2" );
		
		//saber sounds
		//cgi_S_RegisterSound( "sound/weapons/saber/saberon.wav" );
		//cgi_S_RegisterSound( "sound/weapons/saber/enemy_saber_on.wav" );
		cgi_S_RegisterSound( "sound/weapons/saber/saberonquick.wav" );
		//cgi_S_RegisterSound( "sound/weapons/saber/saberoff.wav" );
		//cgi_S_RegisterSound( "sound/weapons/saber/enemy_saber_off.wav" );
		cgi_S_RegisterSound( "sound/weapons/saber/saberspinoff.wav" );
		cgi_S_RegisterSound( "sound/weapons/saber/saberoffquick.wav" );
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberbounce%d.wav", i ) );
		}
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberhit%d.wav", i ) );
		}
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberhitwall%d.wav", i ) );
		}
		for ( i = 1; i < 10; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberblock%d.wav", i ) );
		}
		/*
		for ( i = 1; i < 6; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberhum%d.wav", i ) );
		}
		*/
		for ( i = 1; i < 10; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberhup%d.wav", i ) );
		}
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/saberspin%d.wav", i ) );
		}
		cgi_S_RegisterSound( "sound/weapons/saber/saber_catch.wav" );
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/bounce%d.wav", i ) );
		}
		cgi_S_RegisterSound( "sound/weapons/saber/hitwater.wav" );
		cgi_S_RegisterSound( "sound/weapons/saber/boiling.wav" );
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/saber/rainfizz%d.wav", i ) );
		}
		cgi_S_RegisterSound( "sound/movers/objects/saber_slam" );

		//force sounds
		cgi_S_RegisterSound( "sound/weapons/force/heal.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/speed.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/speedloop.mp3" );
		for ( i = 1; i < 5; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/force/heal%d_m.mp3", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/heal%d_f.mp3", i ) );
		}
		cgi_S_RegisterSound( "sound/weapons/force/lightning.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/lightning2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/web.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/web2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/necro.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/necro2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/poison.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/poison2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/hell.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/hell2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/hylden.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/hylden2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/greenlightning.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/greenlightning2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/ghost.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/ghost2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/dark.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/dark2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/smog.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/smog2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/void.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/void2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/yellowlightning.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/yellowlightning2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/burst.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/burst2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/fire.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/fire2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/wind.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/wind2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/water.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/water2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/lava.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/lava2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/ice.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/ice2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/stone.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/stone2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/sonic.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/sonic2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/astral.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/astral2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/fireball.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/fireball2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/firewhite.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/firewhite2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/hollyfire.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/hollyfire2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/holly.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/holly2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/fire.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/fire2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/life.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/life2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/skull.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/skull2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/acid.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/acid2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/time.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/time2.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/stun.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/stun2.wav");
		cgi_S_RegisterSound( "sound/weapons/force/crystal.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/crystal2.wav");
		for ( i = 1; i < 4; i++ )
		{
			cgi_S_RegisterSound( va( "sound/weapons/force/lightninghit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/webhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/necrohit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/poisonhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/hellhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/hyldenhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/greenlightninghit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/ghosthit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/darkhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/smoghit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/voidhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/yellowlightninghit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/bursthit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/firehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/windhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/waterhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/lavahit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/icehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/stonehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/sonichit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/astralhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/fireballhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/firewhitehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/hollyfirehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/hollyhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/lifehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/skullhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/acidhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/timehit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/stunhit%d.wav", i ) );
			cgi_S_RegisterSound( va( "sound/weapons/force/crystalhit%d.wav", i ) );
		}
		cgi_S_RegisterSound( "sound/weapons/force/push.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/pull.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/jump.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/jumpbuild.wav" );
		cgi_S_RegisterSound( "sound/weapons/force/grip.mp3" );
		//new Jedi Academy force sounds
		cgi_S_RegisterSound( "sound/weapons/force/absorb.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/absorbhit.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/absorbloop.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/protect.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/protecthit.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/protectloop.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/rage.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/ragehit.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/rageloop.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/see.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/seeloop.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/drain.mp3" );
		cgi_S_RegisterSound( "sound/weapons/force/drained.mp3" );
		//force graphics
		cgs.media.playerShieldDamage = cgi_R_RegisterShader("gfx/misc/personalshield");
		//cgs.media.forceSightBubble = cgi_R_RegisterShader("gfx/misc/sightbubble");
		//cgs.media.forceShell = cgi_R_RegisterShader("powerups/forceshell");
		cgs.media.forceShell = cgi_R_RegisterShader("gfx/misc/forceprotect");
		cgs.media.runeShell = cgi_R_RegisterShader("gfx/misc/forcerune");
		cgs.media.sightShell = cgi_R_RegisterShader("powerups/sightshell");
		cgi_R_RegisterShader( "gfx/2d/jsense" );
		cgi_R_RegisterShader( "gfx/2d/jsense2" );
		cgi_R_RegisterShader( "gfx/2d/jecho" );
		cgi_R_RegisterShader( "gfx/2d/jsense4" );
		cgi_R_RegisterShader( "gfx/2d/jsense5" );
		cgi_R_RegisterShader( "gfx/2d/jsense6" );
		//force effects - FIXME: only if someone has these powers?
		theFxScheduler.RegisterEffect( "force/rage2" );
		theFxScheduler.RegisterEffect( "force/berserk" );
		theFxScheduler.RegisterEffect( "force/fear" );
		theFxScheduler.RegisterEffect( "force/trance" );
		theFxScheduler.RegisterEffect( "force/tkshield" );
		theFxScheduler.RegisterEffect( "force/eldershield" );
		//theFxScheduler.RegisterEffect( "force/heal_joint" );
		theFxScheduler.RegisterEffect( "force/heal2" );
		theFxScheduler.RegisterEffect( "force/drain_hand" );

		//saber graphics
		cgs.media.saberBlurShader			= cgi_R_RegisterShader("gfx/effects/sabers/saberBlur");
		cgs.media.swordTrailShader			= cgi_R_RegisterShader("gfx/effects/sabers/swordTrail");
		cgs.media.yellowDroppedSaberShader	= cgi_R_RegisterShader("gfx/effects/yellow_glow");
		cgi_R_RegisterShader( "gfx/effects/saberDamageGlow" );
		cgi_R_RegisterShader( "gfx/effects/solidWhite_cull" );
		cgi_R_RegisterShader( "gfx/effects/forcePush" );
		cgi_R_RegisterShader( "gfx/effects/saberFlare" );
		cgs.media.redSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/red_glow" );
		cgs.media.redSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/red_line" );
		cgs.media.orangeSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/orange_glow" );
		cgs.media.orangeSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/orange_line" );
		cgs.media.yellowSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/yellow_glow" );
		cgs.media.yellowSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/yellow_line" );
		cgs.media.greenSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/green_glow" );
		cgs.media.greenSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/green_line" );
		cgs.media.blueSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/blue_glow" );
		cgs.media.blueSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/blue_line" );
		cgs.media.purpleSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/purple_glow" );
		cgs.media.purpleSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/purple_glow" );

		// NEW COLORS 
		cgs.media.darkSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/dark_line" );
		cgs.media.darkSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/dark_glow" );
		cgs.media.red2SaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/red2_glow" );
		cgs.media.red2SaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/red2_line" );
		cgs.media.whiteSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/white_glow" );
		cgs.media.whiteSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/white_line" );
		cgs.media.whiteblueSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/whiteblue_glow" );
		cgs.media.whiteblueSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/whiteblue_line" );
		cgs.media.green2SaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/green2_glow" );
		cgs.media.green2SaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/green2_line" );
		cgs.media.cyanSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/cyan_glow" );
		cgs.media.cyanSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/cyan_line" );
		cgs.media.brownSaberGlowShader		= cgi_R_RegisterShader( "gfx/effects/sabers/brown_glow" );
		cgs.media.brownSaberCoreShader		= cgi_R_RegisterShader( "gfx/effects/sabers/brown_line" );
		

		cgs.media.forceCoronaShader			= cgi_R_RegisterShaderNoMip( "gfx/hud/force_swirl" );

		//new Jedi Academy force graphics
		cgs.media.drainShader				= cgi_R_RegisterShader( "gfx/misc/redLine" );

		//for grip slamming into walls
		theFxScheduler.RegisterEffect( "env/impact_dustonly" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch1.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch2.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch3.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch4.mp3" );

		//For kicks with saber staff...
		theFxScheduler.RegisterEffect( "melee/kick_impact" );

		//Kothos beam
		cgi_R_RegisterShader( "gfx/misc/dr1" );
		break;

	case WP_BRYAR_PISTOL://Edit the names in the the (" Bow shoot ")
		// Darkness arrow
		cgs.effects.darkarrowShotEffect = theFxScheduler.RegisterEffect("blackarrow/shot");
		theFxScheduler.RegisterEffect("darkarrow/NPCshot");
		cgs.effects.darkarrowPowerupShotEffect = theFxScheduler.RegisterEffect("blackarrow/crackleShot");
		cgs.effects.darkarrowWallImpactEffect = theFxScheduler.RegisterEffect("blackarrow/wall_impact");
		cgs.effects.darkarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("blackarrow/wall_impact2");
		cgs.effects.darkarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("blackarrow/wall_impact3");
		cgs.effects.darkarrowFleshImpactEffect = theFxScheduler.RegisterEffect("blackarrow/flesh_impact");

		// smoke arrow
		cgs.effects.smokearrowShotEffect = theFxScheduler.RegisterEffect("blackarrow/shot");
		theFxScheduler.RegisterEffect("blackarrow/NPCshot");
		cgs.effects.smokearrowPowerupShotEffect = theFxScheduler.RegisterEffect("blackarrow/crackleShot");
		cgs.effects.smokearrowWallImpactEffect = theFxScheduler.RegisterEffect("blackarrow/wall_impact");
		cgs.effects.smokearrowWallImpactEffect2 = theFxScheduler.RegisterEffect("blackarrow/wall_impact2");
		cgs.effects.smokearrowWallImpactEffect3 = theFxScheduler.RegisterEffect("blackarrow/wall_impact3");
		cgs.effects.smokearrowFleshImpactEffect = theFxScheduler.RegisterEffect("blackarrow/flesh_impact");

		// void arrow
		cgs.effects.voidarrowShotEffect = theFxScheduler.RegisterEffect("blackarrow/shot");
		theFxScheduler.RegisterEffect("blackarrow/NPCshot");
		cgs.effects.voidarrowPowerupShotEffect = theFxScheduler.RegisterEffect("blackarrow/crackleShot");
		cgs.effects.voidarrowWallImpactEffect = theFxScheduler.RegisterEffect("blackarrow/wall_impact");
		cgs.effects.voidarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("blackarrow/wall_impact2");
		cgs.effects.voidarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("blackarrow/wall_impact3");
		cgs.effects.voidarrowFleshImpactEffect = theFxScheduler.RegisterEffect("blackarrow/flesh_impact");

		// dust arrow
		cgs.effects.dustarrowShotEffect = theFxScheduler.RegisterEffect("blackarrow/shot");
		theFxScheduler.RegisterEffect("blackarrow/NPCshot");
		cgs.effects.dustarrowPowerupShotEffect = theFxScheduler.RegisterEffect("blackarrow/crackleShot");
		cgs.effects.dustarrowWallImpactEffect = theFxScheduler.RegisterEffect("blackarrow/wall_impact");
		cgs.effects.dustarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("blackarrow/wall_impact2");
		cgs.effects.dustarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("blackarrow/wall_impact3");
		cgs.effects.dustarrowFleshImpactEffect = theFxScheduler.RegisterEffect("blackarrow/flesh_impact");
		///////////////////////////////////////////
		// Light arrow 
		cgs.effects.bowShotEffect = theFxScheduler.RegisterEffect("bow/shot");
		theFxScheduler.RegisterEffect("bow/NPCshot");
		cgs.effects.bowPowerupShotEffect = theFxScheduler.RegisterEffect("bow/crackleShot");
		cgs.effects.bowWallImpactEffect = theFxScheduler.RegisterEffect("bow/wall_impact");
		cgs.effects.bowWallImpactEffect2 = theFxScheduler.RegisterEffect("bow/wall_impact2");
		cgs.effects.bowWallImpactEffect3 = theFxScheduler.RegisterEffect("bow/wall_impact3");
		cgs.effects.bowFleshImpactEffect = theFxScheduler.RegisterEffect("bow/flesh_impact");
		// Undead arrow
		cgs.effects.necroarrowShotEffect = theFxScheduler.RegisterEffect("necroarrow/shot");
		theFxScheduler.RegisterEffect("bow/NPCshot");
		cgs.effects.necroarrowPowerupShotEffect = theFxScheduler.RegisterEffect("necroarrow/crackleShot");
		cgs.effects.necroarrowWallImpactEffect = theFxScheduler.RegisterEffect("necroarrow/wall_impact");
		cgs.effects.necroarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("necroarrow/wall_impact2");
		cgs.effects.necroarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("necroarrow/wall_impact3");
		cgs.effects.necroarrowFleshImpactEffect = theFxScheduler.RegisterEffect("necroarrow/flesh_impact");
				// Electric arrow
		cgs.effects.shockarrowShotEffect = theFxScheduler.RegisterEffect("shockarrow/shot");
		theFxScheduler.RegisterEffect("shockarrow/NPCshot");
		cgs.effects.shockarrowPowerupShotEffect = theFxScheduler.RegisterEffect("shockarrow/crackleShot");
		cgs.effects.shockarrowWallImpactEffect = theFxScheduler.RegisterEffect("shockarrow/wall_impact");
		cgs.effects.shockarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("shockarrow/wall_impact2");
		cgs.effects.shockarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("shockarrow/wall_impact3");
		cgs.effects.shockarrowFleshImpactEffect = theFxScheduler.RegisterEffect("shockarrow/flesh_impact");
		// Fire Arrow
		cgs.effects.firearrowShotEffect = theFxScheduler.RegisterEffect("firearrow/shot");
		theFxScheduler.RegisterEffect("firearrow/NPCshot");
		cgs.effects.firearrowPowerupShotEffect = theFxScheduler.RegisterEffect("firearrow/crackleShot");
		cgs.effects.firearrowWallImpactEffect = theFxScheduler.RegisterEffect("firearrow/wall_impact");
		cgs.effects.firearrowWallImpactEffect2 = theFxScheduler.RegisterEffect("firearrow/wall_impact2");
		cgs.effects.firearrowWallImpactEffect3 = theFxScheduler.RegisterEffect("firearrow/wall_impact3");
		cgs.effects.firearrowFleshImpactEffect = theFxScheduler.RegisterEffect("firearrow/flesh_impact");
		// force arrow
		cgs.effects.forcearrowShotEffect = theFxScheduler.RegisterEffect("forcearrow/shot");
		theFxScheduler.RegisterEffect("forcearrow/NPCshot");
		cgs.effects.forcearrowPowerupShotEffect = theFxScheduler.RegisterEffect("forcearrow/crackleShot");
		cgs.effects.forcearrowWallImpactEffect = theFxScheduler.RegisterEffect("forcearrow/wall_impact");
		cgs.effects.forcearrowWallImpactEffect2 = theFxScheduler.RegisterEffect("forcearrow/wall_impact2");
		cgs.effects.forcearrowWallImpactEffect3 = theFxScheduler.RegisterEffect("forcearrow/wall_impact3");
		cgs.effects.forcearrowFleshImpactEffect = theFxScheduler.RegisterEffect("forcearrow/flesh_impact");
		// wind arrow
		cgs.effects.windarrowShotEffect = theFxScheduler.RegisterEffect("windarrow/shot");
		theFxScheduler.RegisterEffect("windarrow/NPCshot");
		cgs.effects.windarrowPowerupShotEffect = theFxScheduler.RegisterEffect("windarrow/crackleShot");
		cgs.effects.windarrowWallImpactEffect = theFxScheduler.RegisterEffect("windarrow/wall_impact");
		cgs.effects.windarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("windarrow/wall_impact2");
		cgs.effects.windarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("windarrow/wall_impact3");
		cgs.effects.windarrowFleshImpactEffect = theFxScheduler.RegisterEffect("windarrow/flesh_impact");
		// water arrow
		cgs.effects.waterarrowShotEffect = theFxScheduler.RegisterEffect("waterarrow/shot");
		theFxScheduler.RegisterEffect("waterarrow/NPCshot");
		cgs.effects.waterarrowPowerupShotEffect = theFxScheduler.RegisterEffect("waterarrow/crackleShot");
		cgs.effects.waterarrowWallImpactEffect = theFxScheduler.RegisterEffect("waterarrow/wall_impact");
		cgs.effects.waterarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("waterarrow/wall_impact2");
		cgs.effects.waterarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("waterarrow/wall_impact3");
		cgs.effects.waterarrowFleshImpactEffect = theFxScheduler.RegisterEffect("waterarrow/flesh_impact");
		// stone arrow
		cgs.effects.stonearrowShotEffect = theFxScheduler.RegisterEffect("stonearrow/shot");
		theFxScheduler.RegisterEffect("stonearrow/NPCshot");
		cgs.effects.stonearrowPowerupShotEffect = theFxScheduler.RegisterEffect("stonearrow/crackleShot");
		cgs.effects.stonearrowWallImpactEffect = theFxScheduler.RegisterEffect("stonearrow/wall_impact");
		cgs.effects.stonearrowWallImpactEffect2 = theFxScheduler.RegisterEffect("stonearrow/wall_impact2");
		cgs.effects.stonearrowWallImpactEffect3 = theFxScheduler.RegisterEffect("stonearrow/wall_impact3");
		cgs.effects.stonearrowFleshImpactEffect = theFxScheduler.RegisterEffect("stonearrow/flesh_impact");
		// holy arrow 
		cgs.effects.holyarrowShotEffect = theFxScheduler.RegisterEffect("holyarrow/shot");
		theFxScheduler.RegisterEffect("holyarrow/NPCshot");
		cgs.effects.holyarrowPowerupShotEffect = theFxScheduler.RegisterEffect("holyarrow/crackleShot");
		cgs.effects.holyarrowWallImpactEffect = theFxScheduler.RegisterEffect("holyarrow/wall_impact");
		cgs.effects.holyarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("holyarrow/wall_impact2");
		cgs.effects.holyarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("holyarrow/wall_impact3");
		cgs.effects.holyarrowFleshImpactEffect = theFxScheduler.RegisterEffect("holyarrow/flesh_impact");
		// sonic arrow
		cgs.effects.sonicarrowShotEffect = theFxScheduler.RegisterEffect("sonicarrow/shot");
		theFxScheduler.RegisterEffect("sonicarrow/NPCshot");
		cgs.effects.sonicarrowPowerupShotEffect = theFxScheduler.RegisterEffect("sonicarrow/crackleShot");
		cgs.effects.sonicarrowWallImpactEffect = theFxScheduler.RegisterEffect("sonicarrow/wall_impact");
		cgs.effects.sonicarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("sonicarrow/wall_impact2");
		cgs.effects.sonicarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("sonicarrow/wall_impact3");
		cgs.effects.sonicarrowFleshImpactEffect = theFxScheduler.RegisterEffect("sonicarrow/flesh_impact");
		// poison arrow
		cgs.effects.poisonarrowShotEffect = theFxScheduler.RegisterEffect("poisonarrow/shot");
		theFxScheduler.RegisterEffect("poisonarrow/NPCshot");
		cgs.effects.poisonarrowPowerupShotEffect = theFxScheduler.RegisterEffect("poisonarrow/crackleShot");
		cgs.effects.poisonarrowWallImpactEffect = theFxScheduler.RegisterEffect("poisonarrow/wall_impact");
		cgs.effects.poisonarrowWallImpactEffect2 = theFxScheduler.RegisterEffect("poisonarrow/wall_impact2");
		cgs.effects.poisonarrowWallImpactEffect3 = theFxScheduler.RegisterEffect("poisonarrow/wall_impact3");
		cgs.effects.poisonarrowFleshImpactEffect = theFxScheduler.RegisterEffect("poisonarrow/flesh_impact");
		//vampiric
		cgs.effects.vamparrowShotEffect = theFxScheduler.RegisterEffect("vamparrow/shot");
		theFxScheduler.RegisterEffect("vamparrow/NPCshot");
		cgs.effects.vamparrowPowerupShotEffect = theFxScheduler.RegisterEffect("vamparrow/crackleShot");
		cgs.effects.vamparrowWallImpactEffect = theFxScheduler.RegisterEffect("vamparrow/wall_impact");
		cgs.effects.vamparrowWallImpactEffect2 = theFxScheduler.RegisterEffect("vamparrow/wall_impact2");
		cgs.effects.vamparrowWallImpactEffect3 = theFxScheduler.RegisterEffect("vamparrow/wall_impact3");
		cgs.effects.vamparrowFleshImpactEffect = theFxScheduler.RegisterEffect("vamparrow/flesh_impact");
		//hylden 
		cgs.effects.glypharrowShotEffect = theFxScheduler.RegisterEffect("glyphicarrow/shot");
		theFxScheduler.RegisterEffect("glyphicarrow/NPCshot");
		cgs.effects.glypharrowPowerupShotEffect = theFxScheduler.RegisterEffect("glypharrow/crackleShot");
		cgs.effects.glypharrowWallImpactEffect = theFxScheduler.RegisterEffect("glypharrow/wall_impact");
		cgs.effects.glypharrowWallImpactEffect2 = theFxScheduler.RegisterEffect("glyphcarrow/wall_impact2");
		cgs.effects.glypharrowWallImpactEffect3 = theFxScheduler.RegisterEffect("glypharrow/wall_impact3");
		cgs.effects.glypharrowFleshImpactEffect = theFxScheduler.RegisterEffect("glypharrow/flesh_impact");

		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("bow/muzzle_flash");
		theFxScheduler.RegisterEffect("bow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("blackarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("blackarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("firearrow/muzzle_flash");
		theFxScheduler.RegisterEffect("firearrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("shockarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("shockarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("windarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("windarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("waterarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("waterarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("stonearrow/muzzle_flash");
		theFxScheduler.RegisterEffect("stonearrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("vamparrow/muzzle_flash");
		theFxScheduler.RegisterEffect("vamparrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necroarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("necroarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("poisonarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("poisonarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("holyarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("holyarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("sonicarrow/muzzle_flash");
		theFxScheduler.RegisterEffect("sonicarrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("glypharrow/muzzle_flash");
		theFxScheduler.RegisterEffect("glypharrow/altmuzzle_flash");
		theFxScheduler.RegisterEffect("forcearrow/muzzle_flash");
		theFxScheduler.RegisterEffect("forcearrow/altmuzzle_flash");
		break;

	case WP_BLASTER_PISTOL://Edit the names in the the (" Darkness shoot ")
		// STUNNER
		cgs.effects.stunner0ShotEffect = theFxScheduler.RegisterEffect("bryar0/shot");
		theFxScheduler.RegisterEffect("bryar0/NPCshot");
		cgs.effects.stunner0PowerupShotEffect = theFxScheduler.RegisterEffect("bryar0/crackleShot");
		cgs.effects.stunner0WallImpactEffect = theFxScheduler.RegisterEffect("bryar0/wall_impact");
		cgs.effects.stunner0WallImpactEffect2 = theFxScheduler.RegisterEffect("bryar0/wall_impact2");
		cgs.effects.stunner0WallImpactEffect3 = theFxScheduler.RegisterEffect("bryar0/wall_impact3");
		cgs.effects.stunner0FleshImpactEffect = theFxScheduler.RegisterEffect("bryar0/flesh_impact");
				
		// TENEBRA 
		cgs.effects.darkShotEffect = theFxScheduler.RegisterEffect("dark/shot");
		theFxScheduler.RegisterEffect("dark/NPCshot");
		cgs.effects.darkPowerupShotEffect = theFxScheduler.RegisterEffect("dark/crackleShot");
		cgs.effects.darkWallImpactEffect = theFxScheduler.RegisterEffect("dark/wall_impact");
		cgs.effects.darkWallImpactEffect2 = theFxScheduler.RegisterEffect("dark/wall_impact2");
		cgs.effects.darkWallImpactEffect3 = theFxScheduler.RegisterEffect("dark/wall_impact3");
		cgs.effects.darkFleshImpactEffect = theFxScheduler.RegisterEffect("dark/flesh_impact");
		// FUMO
		cgs.effects.smokeShotEffect = theFxScheduler.RegisterEffect("smoke/shot");
		theFxScheduler.RegisterEffect("smoke/NPCshot");
		cgs.effects.smokePowerupShotEffect = theFxScheduler.RegisterEffect("smoke/crackleShot");
		cgs.effects.smokeWallImpactEffect = theFxScheduler.RegisterEffect("smoke/wall_impact");
		cgs.effects.smokeWallImpactEffect2 = theFxScheduler.RegisterEffect("smoke/wall_impact2");
		cgs.effects.smokeWallImpactEffect3 = theFxScheduler.RegisterEffect("smoke/wall_impact3");
		cgs.effects.smokeFleshImpactEffect = theFxScheduler.RegisterEffect("smoke/flesh_impact");
		// VUOTO
		cgs.effects.voidShotEffect = theFxScheduler.RegisterEffect("void/shot");
		theFxScheduler.RegisterEffect("void/NPCshot");
		cgs.effects.voidPowerupShotEffect = theFxScheduler.RegisterEffect("void/crackleShot");
		cgs.effects.voidWallImpactEffect = theFxScheduler.RegisterEffect("void/wall_impact");
		cgs.effects.voidWallImpactEffect2 = theFxScheduler.RegisterEffect("void/wall_impact2");
		cgs.effects.voidWallImpactEffect3 = theFxScheduler.RegisterEffect("void/wall_impact3");
		cgs.effects.voidFleshImpactEffect = theFxScheduler.RegisterEffect("void/flesh_impact");
		
		// DUST 
		cgs.effects.dustShotEffect = theFxScheduler.RegisterEffect("dust/shot");
		theFxScheduler.RegisterEffect("dust/NPCshot");
		cgs.effects.dustPowerupShotEffect = theFxScheduler.RegisterEffect("dust/crackleShot");
		cgs.effects.dustWallImpactEffect = theFxScheduler.RegisterEffect("dust/wall_impact");
		cgs.effects.dustWallImpactEffect2 = theFxScheduler.RegisterEffect("dust/wall_impact2");
		cgs.effects.dustWallImpactEffect3 = theFxScheduler.RegisterEffect("dust/wall_impact3");
		cgs.effects.dustFleshImpactEffect = theFxScheduler.RegisterEffect("dust/flesh_impact");
	
		// KNIVES 
		cgs.effects.knifeShotEffect = theFxScheduler.RegisterEffect("knife/shot");
		theFxScheduler.RegisterEffect("knife/NPCshot");
		cgs.effects.knifePowerupShotEffect = theFxScheduler.RegisterEffect("knife/crackleShot");
		cgs.effects.knifeWallImpactEffect = theFxScheduler.RegisterEffect("knife/wall_impact");
		cgs.effects.knifeWallImpactEffect2 = theFxScheduler.RegisterEffect("knife/wall_impact2");
		cgs.effects.knifeWallImpactEffect3 = theFxScheduler.RegisterEffect("knife/wall_impact3");
		cgs.effects.knifeFleshImpactEffect = theFxScheduler.RegisterEffect("knife/flesh_impact");
		
		// PUGNALE SACRO GLIFICO KNIVES 
		cgs.effects.knife2ShotEffect = theFxScheduler.RegisterEffect("knife2/shot");
		theFxScheduler.RegisterEffect("knife/NPCshot");
		cgs.effects.knife2PowerupShotEffect = theFxScheduler.RegisterEffect("knife2/crackleShot");
		cgs.effects.knife2WallImpactEffect = theFxScheduler.RegisterEffect("knife2/wall_impact");
		cgs.effects.knife2WallImpactEffect2 = theFxScheduler.RegisterEffect("knife2/wall_impact2");
		cgs.effects.knife2WallImpactEffect3 = theFxScheduler.RegisterEffect("knife2/wall_impact3");
		cgs.effects.knife2FleshImpactEffect = theFxScheduler.RegisterEffect("knife2/flesh_impact");
		
	


		// NECROTIC SPHERES 
		cgs.effects.absorbShotEffect = theFxScheduler.RegisterEffect("absorb/shot");
		theFxScheduler.RegisterEffect("absorb/NPCshot");
		cgs.effects.absorbPowerupShotEffect = theFxScheduler.RegisterEffect("absorb/crackleShot");
		cgs.effects.absorbWallImpactEffect = theFxScheduler.RegisterEffect("absorb/wall_impact");
		cgs.effects.absorbWallImpactEffect2 = theFxScheduler.RegisterEffect("absorb/wall_impact2");
		cgs.effects.absorbWallImpactEffect3 = theFxScheduler.RegisterEffect("absorb/wall_impact3");
		cgs.effects.absorbFleshImpactEffect = theFxScheduler.RegisterEffect("absorb/flesh_impact");
		
		// VAMPIRIC STUN 
		//cgs.effects.stunShotEffect = theFxScheduler.RegisterEffect("stun/shot");
		//theFxScheduler.RegisterEffect("stun/NPCshot");
		//cgs.effects.stunPowerupShotEffect = theFxScheduler.RegisterEffect("stun/crackleShot");
		cgs.effects.stunWallImpactEffect = theFxScheduler.RegisterEffect("stun/wall_impact");
		cgs.effects.stunWallImpactEffect2 = theFxScheduler.RegisterEffect("stun/wall_impact2");
		cgs.effects.stunWallImpactEffect3 = theFxScheduler.RegisterEffect("stun/wall_impact3");
		cgs.effects.stunFleshImpactEffect = theFxScheduler.RegisterEffect("stun/flesh_impact");
		
		// FUMOGENO ALCHEMICO 
		cgs.effects.smokerShotEffect = theFxScheduler.RegisterEffect("smoker/shot");
		theFxScheduler.RegisterEffect("smoke/NPCshot");
		cgs.effects.smokerPowerupShotEffect = theFxScheduler.RegisterEffect("smoker/crackleShot");
		cgs.effects.smokerWallImpactEffect = theFxScheduler.RegisterEffect("smoker/wall_impact");
		cgs.effects.smokerWallImpactEffect2 = theFxScheduler.RegisterEffect("smoker/wall_impact2");
		cgs.effects.smokerWallImpactEffect3 = theFxScheduler.RegisterEffect("smoker/wall_impact3");
		cgs.effects.smokerFleshImpactEffect = theFxScheduler.RegisterEffect("smoker/flesh_impact");
	
		// ENTROPY
		cgs.effects.entropyShotEffect = theFxScheduler.RegisterEffect("entropy/shot");
		theFxScheduler.RegisterEffect("entropy/NPCshot");
		cgs.effects.entropyPowerupShotEffect = theFxScheduler.RegisterEffect("entropy/crackleShot");
		cgs.effects.entropyWallImpactEffect = theFxScheduler.RegisterEffect("entropy/wall_impact");
		cgs.effects.entropyWallImpactEffect2 = theFxScheduler.RegisterEffect("entropy/wall_impact2");
		cgs.effects.entropyWallImpactEffect3 = theFxScheduler.RegisterEffect("entropy/wall_impact3");
		cgs.effects.entropyFleshImpactEffect = theFxScheduler.RegisterEffect("entropy/flesh_impact");


		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("bryar/muzzle_flash");
		theFxScheduler.RegisterEffect("bryar/altmuzzle_flash");
		theFxScheduler.RegisterEffect("bryar2/muzzle_flash");
		theFxScheduler.RegisterEffect("bryar2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("bryar3/muzzle_flash");
		theFxScheduler.RegisterEffect("bryar3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("knife/muzzle_flash");
		theFxScheduler.RegisterEffect("knife/altmuzzle_flash");
		theFxScheduler.RegisterEffect("stun/muzzle_flash");
		theFxScheduler.RegisterEffect("stun/altmuzzle_flash");
		theFxScheduler.RegisterEffect("absorb/muzzle_flash");
		theFxScheduler.RegisterEffect("absorb/altmuzzle_flash");
		theFxScheduler.RegisterEffect("entropy/muzzle_flash");
		theFxScheduler.RegisterEffect("entropy/altmuzzle_flash");
		break;
	case WP_JAWA://Edit the names in the the (" Sonic weapon ")
		cgs.effects.howlerShotEffect			= theFxScheduler.RegisterEffect( "howler/shot" );
		theFxScheduler.RegisterEffect( "howler/NPCshot" );
		cgs.effects.howlerPowerupShotEffect	= theFxScheduler.RegisterEffect( "howler/crackleShot" );
		cgs.effects.howlerWallImpactEffect	= theFxScheduler.RegisterEffect( "howler/wall_impact" );
		cgs.effects.howlerWallImpactEffect2	= theFxScheduler.RegisterEffect( "howler/wall_impact2" );
		cgs.effects.howlerWallImpactEffect3	= theFxScheduler.RegisterEffect( "howler/wall_impact3" );
		cgs.effects.howlerFleshImpactEffect	= theFxScheduler.RegisterEffect( "howler/flesh_impact" );

		// ASTRALE
		cgs.effects.astralShotEffect			= theFxScheduler.RegisterEffect( "astral/shot" );
		theFxScheduler.RegisterEffect( "astral/NPCshot" );
		cgs.effects.astralPowerupShotEffect	= theFxScheduler.RegisterEffect( "astral/crackleShot" );
		cgs.effects.astralWallImpactEffect	= theFxScheduler.RegisterEffect( "astral/wall_impact" );
		cgs.effects.astralWallImpactEffect2	= theFxScheduler.RegisterEffect( "astral/wall_impact2" );
		cgs.effects.astralWallImpactEffect3	= theFxScheduler.RegisterEffect( "astral/wall_impact3" );
		cgs.effects.astralFleshImpactEffect	= theFxScheduler.RegisterEffect( "astral/flesh_impact" );
		theFxScheduler.RegisterEffect("howler/muzzle_flash");
		theFxScheduler.RegisterEffect("howler/altmuzzle_flash");
		theFxScheduler.RegisterEffect("astral/muzzle_flash");
		theFxScheduler.RegisterEffect("astral/altmuzzle_flash");
	
	/*case WP_BLOODGUN://Edit the names in the the (" Prophet blood pistol ")
		cgs.effects.bloodgunShotEffect			= theFxScheduler.RegisterEffect( "bloodgun/shot" );
		theFxScheduler.RegisterEffect( "bloodgun/NPCshot" );
		cgs.effects.bloodgunPowerupShotEffect	= theFxScheduler.RegisterEffect( "bloodgun/crackleShot" );
		cgs.effects.bloodgunWallImpactEffect	= theFxScheduler.RegisterEffect( "bloodgun/wall_impact" );
		cgs.effects.bloodgunWallImpactEffect2	= theFxScheduler.RegisterEffect( "bloodgun/wall_impact2" );
		cgs.effects.bloodgunWallImpactEffect3	= theFxScheduler.RegisterEffect( "bloodgun/wall_impact3" );
		cgs.effects.bloodgunFleshImpactEffect	= theFxScheduler.RegisterEffect( "bloodgun/flesh_impact" );

		// Note....these are temp shared effects
		theFxScheduler.RegisterEffect( "blaster/deflect" );
		theFxScheduler.RegisterEffect( "blaster/smoke_bolton" ); // note: this will be called game side
		break;*/

	case WP_BLASTER:
		// GREEN LASER 
		cgs.effects.greenlaser0ShotEffect			= theFxScheduler.RegisterEffect( "blaster0/shot" );
													theFxScheduler.RegisterEffect( "blaster0/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.greenlaser0WallImpactEffect		= theFxScheduler.RegisterEffect( "blaster0/wall_impact" );
		cgs.effects.greenlaser0FleshImpactEffect	= theFxScheduler.RegisterEffect( "blaster0/flesh_impact" );
	
		// FIRE 
		cgs.effects.blasterShotEffect			= theFxScheduler.RegisterEffect( "blaster/shot" );
													theFxScheduler.RegisterEffect( "blaster/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.blasterWallImpactEffect		= theFxScheduler.RegisterEffect( "blaster/wall_impact" );
		cgs.effects.blasterFleshImpactEffect	= theFxScheduler.RegisterEffect( "blaster/flesh_impact" );
		theFxScheduler.RegisterEffect( "blaster/deflect" );
		theFxScheduler.RegisterEffect( "blaster/smoke_bolton" ); // note: this will be called game side
		// WARM EFFECTS
		cgs.effects.warmShotEffect			= theFxScheduler.RegisterEffect( "blaster2/shot" );
											   theFxScheduler.RegisterEffect( "blaster2/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.warmWallImpactEffect		= theFxScheduler.RegisterEffect( "blaster2/wall_impact" );
		cgs.effects.warmFleshImpactEffect	= theFxScheduler.RegisterEffect( "blaster2/flesh_impact" );

		// MAGMA EFFECTS
		cgs.effects.magmaShotEffect			= theFxScheduler.RegisterEffect( "blaster3/shot" );
											   theFxScheduler.RegisterEffect( "blaster3/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.magmaWallImpactEffect		= theFxScheduler.RegisterEffect( "blaster3/wall_impact" );
		cgs.effects.magmaFleshImpactEffect	= theFxScheduler.RegisterEffect( "blaster3/flesh_impact" );

		// VAMPIRIC FIRE  EFFECTS
		cgs.effects.vampfireShotEffect			= theFxScheduler.RegisterEffect( "fire_blood/shot" );
											   theFxScheduler.RegisterEffect( "fire_blood/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.vampfireWallImpactEffect		= theFxScheduler.RegisterEffect( "fire_blood/wall_impact" );
		cgs.effects.vampfireFleshImpactEffect	= theFxScheduler.RegisterEffect( "fire_blood/flesh_impact" );

		// HELL FIRE EFFECTS
		cgs.effects.hellfireShotEffect			= theFxScheduler.RegisterEffect( "fire_hell/shot" );
											   theFxScheduler.RegisterEffect( "fire_hell/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.hellfireWallImpactEffect		= theFxScheduler.RegisterEffect( "fire_hell/wall_impact" );
		cgs.effects.hellfireFleshImpactEffect	= theFxScheduler.RegisterEffect( "fire_hell/flesh_impact" );

		// GREENFIRE EFFECTS
		cgs.effects.greenfireShotEffect			= theFxScheduler.RegisterEffect( "fire_green/shot" );
											   theFxScheduler.RegisterEffect( "fire_green/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.greenfireWallImpactEffect		= theFxScheduler.RegisterEffect( "fire_green/wall_impact" );
		cgs.effects.greenfireFleshImpactEffect	= theFxScheduler.RegisterEffect( "fire_green/flesh_impact" );
		// POISONFIRE EFFECTS
		cgs.effects.poisonfireShotEffect			= theFxScheduler.RegisterEffect( "fire_poison/shot" );
											   theFxScheduler.RegisterEffect( "fire_poison/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.poisonfireWallImpactEffect		= theFxScheduler.RegisterEffect( "fire_poison/wall_impact" );
		cgs.effects.poisonfireFleshImpactEffect	= theFxScheduler.RegisterEffect( "fire_poison/flesh_impact" );
		// GHOSTFIRE EFFECTS
		cgs.effects.ghostfireShotEffect			= theFxScheduler.RegisterEffect( "fire_spectral/shot" );
											   theFxScheduler.RegisterEffect( "fire_spectral/NPCshot" );
//		cgs.effects.blasterOverchargeEffect		= theFxScheduler.RegisterEffect( "blaster/overcharge" );
		cgs.effects.ghostfireWallImpactEffect		= theFxScheduler.RegisterEffect( "fire_spectral/wall_impact" );
		cgs.effects.ghostfireFleshImpactEffect	= theFxScheduler.RegisterEffect( "fire_spectral/flesh_impact" );

		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("blaster/muzzle_flash");
		theFxScheduler.RegisterEffect("blaster/altmuzzle_flash");
		theFxScheduler.RegisterEffect("blaster2/muzzle_flash");
		theFxScheduler.RegisterEffect("blaster2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("blaster3/muzzle_flash");
		theFxScheduler.RegisterEffect("blaster3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("fire_green/muzzle_flash");
		theFxScheduler.RegisterEffect("fire_green/altmuzzle_flash");
		theFxScheduler.RegisterEffect("fire_poison/muzzle_flash");
		theFxScheduler.RegisterEffect("fire_poison/altmuzzle_flash");
		theFxScheduler.RegisterEffect("fire_spectral/muzzle_flash");
		theFxScheduler.RegisterEffect("fire_spectral/altmuzzle_flash");
		theFxScheduler.RegisterEffect("fire_blood/muzzle_flash");
		theFxScheduler.RegisterEffect("fire_blood/altmuzzle_flash");
		theFxScheduler.RegisterEffect("fire_hell/muzzle_flash");
		theFxScheduler.RegisterEffect("fire_hell/altmuzzle_flash");
		break;

	case WP_DISRUPTOR:
		theFxScheduler.RegisterEffect( "disruptor/wall_impact" );
		theFxScheduler.RegisterEffect( "disruptor/flesh_impact" );
		theFxScheduler.RegisterEffect( "disruptor/alt_miss" );
		theFxScheduler.RegisterEffect( "disruptor/alt_hit" );
		theFxScheduler.RegisterEffect( "disruptor/line_cap" );
		theFxScheduler.RegisterEffect( "disruptor/death_smoke" );

		cgi_R_RegisterShader( "gfx/effects/redLine" );
		cgi_R_RegisterShader( "gfx/misc/whiteline2" );
		cgi_R_RegisterShader( "gfx/effects/smokeTrail" );
		cgi_R_RegisterShader( "gfx/effects/burn" );

		cgi_R_RegisterShaderNoMip( "gfx/2d/crop_charge" );

		// zoom sounds
		cgi_S_RegisterSound( "sound/weapons/disruptor/zoomstart.wav" );
		cgi_S_RegisterSound( "sound/weapons/disruptor/zoomend.wav" );
		cgs.media.disruptorZoomLoop = cgi_S_RegisterSound( "sound/weapons/disruptor/zoomloop.wav" );

		// Disruptor gun zoom interface
		cgs.media.disruptorMask			= cgi_R_RegisterShader( "gfx/2d/cropCircle2");
		cgs.media.disruptorInsert		= cgi_R_RegisterShader( "gfx/2d/cropCircle");
		cgs.media.disruptorLight		= cgi_R_RegisterShader( "gfx/2d/cropCircleGlow" );
		cgs.media.disruptorInsertTick	= cgi_R_RegisterShader( "gfx/2d/insertTick" );
		break;

	case WP_BOWCASTER:
		// DARDUS 
		cgs.effects.dardus0ShotEffect		= theFxScheduler.RegisterEffect( "bowcaster0/shot" );
		cgs.effects.dardus0BounceEffect	= theFxScheduler.RegisterEffect( "bowcaster0/bounce_wall" );
		cgs.effects.dardus0ImpactEffect	= theFxScheduler.RegisterEffect( "bowcaster0/explosion" );
		theFxScheduler.RegisterEffect( "bowcaster0/deflect" );

		// LIGHT 
		cgs.effects.bowcasterShotEffect		= theFxScheduler.RegisterEffect( "bowcaster/shot" );
		cgs.effects.bowcasterBounceEffect	= theFxScheduler.RegisterEffect( "bowcaster/bounce_wall" );
		cgs.effects.bowcasterImpactEffect	= theFxScheduler.RegisterEffect( "bowcaster/explosion" );
		theFxScheduler.RegisterEffect( "bowcaster/deflect" );
		// FULMINE
		cgs.effects.shockShotEffect		= theFxScheduler.RegisterEffect( "bowcaster2/shot" );
		cgs.effects.shockBounceEffect	= theFxScheduler.RegisterEffect( "bowcaster2/bounce_wall" );
		cgs.effects.shockImpactEffect	= theFxScheduler.RegisterEffect( "bowcaster2/explosion" );
		theFxScheduler.RegisterEffect( "bowcaster2/deflect" );
		// RADIANZA 
		cgs.effects.sunShotEffect		= theFxScheduler.RegisterEffect( "bowcaster3/shot" );
		cgs.effects.sunBounceEffect	= theFxScheduler.RegisterEffect( "bowcaster3/bounce_wall" );
		cgs.effects.sunImpactEffect	= theFxScheduler.RegisterEffect( "bowcaster3/explosion" );
		theFxScheduler.RegisterEffect( "bowcaster3/deflect" );

		// LIGHT KNIFE
		cgs.effects.lightknifeShotEffect		= theFxScheduler.RegisterEffect( "light_knife/shot" );
		cgs.effects.lightknifeImpactEffect	= theFxScheduler.RegisterEffect( "light_knife/explosion" );
		cgs.effects.lightknife2ImpactEffect	= theFxScheduler.RegisterEffect( "light_knife/flesh_impact" );
		cgs.effects.lightknifeBounceEffect	= theFxScheduler.RegisterEffect( "light_knife/bounce_wall" );
		theFxScheduler.RegisterEffect( "light_knife/deflect" );

		// SKULL LIGHT 
		cgs.effects.lightskullShotEffect		= theFxScheduler.RegisterEffect( "light_skull/shot" );
		cgs.effects.lightskullImpactEffect	= theFxScheduler.RegisterEffect( "light_skull/explosion" );
		cgs.effects.lightskull2ImpactEffect	= theFxScheduler.RegisterEffect( "light_skull/flesh_impact" );
		cgs.effects.lightskullBounceEffect	= theFxScheduler.RegisterEffect( "light_skull/bounce_wall" );
		theFxScheduler.RegisterEffect( "light_skull/deflect" );

		// MADNESS
		cgs.effects.madShotEffect		= theFxScheduler.RegisterEffect( "madness/shot" );
		cgs.effects.madImpactEffect	= theFxScheduler.RegisterEffect( "madness/explosion" );
		cgs.effects.mad2ImpactEffect	= theFxScheduler.RegisterEffect( "madness/flesh_impact" );
		cgs.effects.madBounceEffect	= theFxScheduler.RegisterEffect( "madness/bounce_wall" );
		theFxScheduler.RegisterEffect( "madness/deflect" );
		// ENERGY BOLT 
		cgs.effects.ebShotEffect		= theFxScheduler.RegisterEffect( "energy_bolt/shot" );
		cgs.effects.ebImpactEffect	= theFxScheduler.RegisterEffect( "energy_bolt/explosion" );
		cgs.effects.eb2ImpactEffect	= theFxScheduler.RegisterEffect( "energy_bolt/flesh_impact" );
		cgs.effects.ebBounceEffect	= theFxScheduler.RegisterEffect( "energy_bolt/bounce_wall" );
		theFxScheduler.RegisterEffect( "energy_bolt/deflect" );

		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("bowcaster/muzzle_flash");
		theFxScheduler.RegisterEffect("bowcaster/altmuzzle_flash");
		theFxScheduler.RegisterEffect("bowcaster2/muzzle_flash");
		theFxScheduler.RegisterEffect("bowcaster2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("bowcaster3/muzzle_flash");
		theFxScheduler.RegisterEffect("bowcaster3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("light_knife/muzzle_flash");
		theFxScheduler.RegisterEffect("light_knife/altmuzzle_flash");
		theFxScheduler.RegisterEffect("light_skull/muzzle_flash");
		theFxScheduler.RegisterEffect("light_skull/altmuzzle_flash");
		theFxScheduler.RegisterEffect("energy_bolt/muzzle_flash");
		theFxScheduler.RegisterEffect("energy_bolt/altmuzzle_flash");
		theFxScheduler.RegisterEffect("madness/muzzle_flash");
		theFxScheduler.RegisterEffect("madness/altmuzzle_flash");	
		break;

	case WP_REPEATER:
		// PLASMA 
		theFxScheduler.RegisterEffect( "repeater0/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "repeater0/projectile" );
		theFxScheduler.RegisterEffect( "repeater0/alt_projectile" );
		theFxScheduler.RegisterEffect( "repeater0/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater0/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater0/flesh_impact" );
		theFxScheduler.RegisterEffect( "repeater0/concussion" );

		// WATER 
		theFxScheduler.RegisterEffect( "repeater/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "repeater/projectile" );
		theFxScheduler.RegisterEffect( "repeater/alt_projectile" );
		theFxScheduler.RegisterEffect( "repeater/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "repeater/concussion" );
		// HYDROTK
		theFxScheduler.RegisterEffect( "hydrotk/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "hydrotk/projectile" );
		theFxScheduler.RegisterEffect( "hydrotk/alt_projectile" );
		theFxScheduler.RegisterEffect( "hydrotk/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "hydrotk/concussion" );
		// HYDROTK BOSS
		theFxScheduler.RegisterEffect( "hydrotk2/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "hydrotk2/projectile" );
		theFxScheduler.RegisterEffect( "hydrotk2/alt_projectile" );
		theFxScheduler.RegisterEffect( "hydrotk2/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "hydrotk2/concussion" );
		// HOLY WATER 
		theFxScheduler.RegisterEffect( "holywater/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "holywater/projectile" );
		theFxScheduler.RegisterEffect( "holywater/alt_projectile" );
		theFxScheduler.RegisterEffect( "holywater/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "holywater/concussion" );
		theFxScheduler.RegisterEffect( "holywater/concussion2" );

		// GHIACCIO
		theFxScheduler.RegisterEffect( "repeater2/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "repeater2/projectile" );
		theFxScheduler.RegisterEffect( "repeater2/alt_projectile" );
		theFxScheduler.RegisterEffect( "repeater2/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "repeater2/concussion" );
		// CRISTALLO 
		theFxScheduler.RegisterEffect( "repeater3/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "repeater3/projectile" );
		theFxScheduler.RegisterEffect( "repeater3/alt_projectile" );
		theFxScheduler.RegisterEffect( "repeater3/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "repeater3/concussion" );

		// VAPOR
		theFxScheduler.RegisterEffect( "vapor/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "vapor/projectile" );
		theFxScheduler.RegisterEffect( "vapor/alt_projectile" );
		theFxScheduler.RegisterEffect( "vapor/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "vapor/concussion" );
		// TAR 
		theFxScheduler.RegisterEffect( "tar/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "tar/projectile" );
		theFxScheduler.RegisterEffect( "tar/alt_projectile" );
		theFxScheduler.RegisterEffect( "tar/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "tar/concussion" );
		// ELDER TAR 
		theFxScheduler.RegisterEffect( "eldertar/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "eldertar/projectile" );
		theFxScheduler.RegisterEffect( "eldertar/alt_projectile" );
		theFxScheduler.RegisterEffect( "eldertar/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "eldertar/concussion2" );
		theFxScheduler.RegisterEffect( "eldertar/concussion" );
		// GREENMUD - FONT OF PUTRESCENSE 
		theFxScheduler.RegisterEffect( "putrescence/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "putrescence/projectile" );
		theFxScheduler.RegisterEffect( "putrescence/alt_projectile" );
		theFxScheduler.RegisterEffect( "putrescence/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "putrescence/concussion2" );
		theFxScheduler.RegisterEffect( "putrescence/concussion" );

		// HELLMUD  - FONT OF PUTRESCENSE DEMONS 
		theFxScheduler.RegisterEffect( "hellputrescence/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "hellputrescence/projectile" );
		theFxScheduler.RegisterEffect( "hellputrescence/alt_projectile" );
		theFxScheduler.RegisterEffect( "hellputrescence/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "hellputrescence/concussion2" );
		theFxScheduler.RegisterEffect( "hellputrescence/concussion" );

		//NECROTIC - A necromantic water atk 
		theFxScheduler.RegisterEffect( "necrotic/muzzle_smoke" );
		theFxScheduler.RegisterEffect( "necrotic/projectile" );
		theFxScheduler.RegisterEffect( "necrotic/alt_projectile" );
		theFxScheduler.RegisterEffect( "necrotic/wall_impact" );
//		theFxScheduler.RegisterEffect( "repeater/alt_wall_impact2" );
//		theFxScheduler.RegisterEffect( "repeater/flesh_impact" );
		theFxScheduler.RegisterEffect( "necrotic/concussion2" );
		theFxScheduler.RegisterEffect( "necrotic/concussion" );


		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("repeater/muzzle_flash");
		theFxScheduler.RegisterEffect("repeater/altmuzzle_flash");
		theFxScheduler.RegisterEffect("repeater2/muzzle_flash");
		theFxScheduler.RegisterEffect("repeater2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("repeater3/muzzle_flash");
		theFxScheduler.RegisterEffect("repeater3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("vapor/muzzle_flash");
		theFxScheduler.RegisterEffect("vapor/altmuzzle_flash");
		theFxScheduler.RegisterEffect("tar/muzzle_flash");
		theFxScheduler.RegisterEffect("tar/altmuzzle_flash");
		theFxScheduler.RegisterEffect("eldertar/muzzle_flash");
		theFxScheduler.RegisterEffect("eldertar/altmuzzle_flash");
		theFxScheduler.RegisterEffect("eldertar/flesh_impact");
		theFxScheduler.RegisterEffect("holywater/muzzle_flash");
		theFxScheduler.RegisterEffect("holywater/altmuzzle_flash");	
		theFxScheduler.RegisterEffect("holywater/flesh_impact");
		theFxScheduler.RegisterEffect("hydrotk/muzzle_flash");
		theFxScheduler.RegisterEffect("hydrotk/altmuzzle_flash");
		theFxScheduler.RegisterEffect("hydrotk2/muzzle_flash");
		theFxScheduler.RegisterEffect("hydrotk2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("putrescence/muzzle_flash");
		theFxScheduler.RegisterEffect("putrescence/altmuzzle_flash");
		theFxScheduler.RegisterEffect("putrescence/flesh_impact");
		theFxScheduler.RegisterEffect("hellputrescence/muzzle_flash");
		theFxScheduler.RegisterEffect("hellputrescence/altmuzzle_flash");
		theFxScheduler.RegisterEffect("hellputrescence/flesh_impact");
		theFxScheduler.RegisterEffect("necrotic/muzzle_flash");
		theFxScheduler.RegisterEffect("necrotic/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necrotic/flesh_impact");
		break;

	case WP_DEMP2:
		// EPULSE 
		theFxScheduler.RegisterEffect( "demp00/projectile" );
		theFxScheduler.RegisterEffect( "demp00/wall_impact" );
		theFxScheduler.RegisterEffect( "demp00/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp00/altDetonate" );
		
		// BENEDICTUM
		theFxScheduler.RegisterEffect( "demp/projectile" );
		theFxScheduler.RegisterEffect( "demp/wall_impact" );
		theFxScheduler.RegisterEffect( "demp/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp/altDetonate" );
		// SPECTRAL
		theFxScheduler.RegisterEffect( "demp0/projectile" );
		theFxScheduler.RegisterEffect( "demp0/wall_impact" );
		theFxScheduler.RegisterEffect( "demp0/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp0/altDetonate" );
		// HELL WRACK 
		theFxScheduler.RegisterEffect( "demp1/projectile" );
		theFxScheduler.RegisterEffect( "demp1/wall_impact" );
		theFxScheduler.RegisterEffect( "demp1/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp1/altDetonate" );

		// SPIRITO
		theFxScheduler.RegisterEffect( "demp2/projectile" );
		theFxScheduler.RegisterEffect( "demp2/wall_impact" );
		theFxScheduler.RegisterEffect( "demp2/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp2/altDetonate" );
		cgi_R_RegisterShader( "gfx/effects/demp2shell" );

		// SACRO
		theFxScheduler.RegisterEffect( "demp3/projectile" );
		theFxScheduler.RegisterEffect( "demp3/wall_impact" );
		theFxScheduler.RegisterEffect( "demp3/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp3/altDetonate" );
		// VITA
		theFxScheduler.RegisterEffect( "demp4/projectile" );
		theFxScheduler.RegisterEffect( "demp4/wall_impact" );
		theFxScheduler.RegisterEffect( "demp4/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp4/altDetonate" );
		// FUOCO ASGHY 
		theFxScheduler.RegisterEffect( "demp5/projectile" );
		theFxScheduler.RegisterEffect( "demp5/wall_impact" );
		theFxScheduler.RegisterEffect( "demp5/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp5/altDetonate" );
		// HOLY FIRE
		theFxScheduler.RegisterEffect( "demp6/projectile" );
		theFxScheduler.RegisterEffect( "demp6/wall_impact" );
		theFxScheduler.RegisterEffect( "demp6/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp6/altDetonate" );
		// SPECTRAL RIVERBERO 
		theFxScheduler.RegisterEffect( "demp7/projectile" );
		theFxScheduler.RegisterEffect( "demp7/wall_impact" );
		theFxScheduler.RegisterEffect( "demp7/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp7/altDetonate" );
		// BALANCE 
		theFxScheduler.RegisterEffect( "demp8/projectile" );
		theFxScheduler.RegisterEffect( "demp8/wall_impact" );
		theFxScheduler.RegisterEffect( "demp8/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp8/altDetonate" );
		// WRACK  
		theFxScheduler.RegisterEffect( "demp9/projectile" );
		theFxScheduler.RegisterEffect( "demp9/wall_impact" );
		theFxScheduler.RegisterEffect( "demp9/flesh_impact" );
		theFxScheduler.RegisterEffect( "demp9/altDetonate" );

		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("demp/muzzle_flash");
		theFxScheduler.RegisterEffect("demp/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp0/muzzle_flash");
		theFxScheduler.RegisterEffect("demp0/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp1/muzzle_flash");
		theFxScheduler.RegisterEffect("demp1/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp2/muzzle_flash");
		theFxScheduler.RegisterEffect("demp2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp3/muzzle_flash");
		theFxScheduler.RegisterEffect("demp3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp4/muzzle_flash");
		theFxScheduler.RegisterEffect("demp4/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp5/flesh_impact");
		theFxScheduler.RegisterEffect("demp5/muzzle_flash");
		theFxScheduler.RegisterEffect("demp6/altmuzzle_flash");	
		theFxScheduler.RegisterEffect("demp6/flesh_impact");
		theFxScheduler.RegisterEffect("demp7/muzzle_flash");
		theFxScheduler.RegisterEffect("demp7/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp8/muzzle_flash");
		theFxScheduler.RegisterEffect("demp8/altmuzzle_flash");
		theFxScheduler.RegisterEffect("demp9/muzzle_flash");
		theFxScheduler.RegisterEffect("demp9/altmuzzle_flash");
		break;
	case WP_POISON:
		theFxScheduler.RegisterEffect( "poison/projectile" );
		theFxScheduler.RegisterEffect( "poison/wall_impact" );
		theFxScheduler.RegisterEffect( "poison/flesh_impact" );
		theFxScheduler.RegisterEffect( "poison/altDetonate" );
		cgi_R_RegisterShader( "gfx/effects/poisonshell" );
		break;
		
	case WP_ATST_MAIN:
		theFxScheduler.RegisterEffect( "atst/shot" );
		theFxScheduler.RegisterEffect( "atst/wall_impact" );
		theFxScheduler.RegisterEffect( "atst/flesh_impact" );
		theFxScheduler.RegisterEffect( "atst/droid_impact" );
		break;

	case WP_ATST_SIDE:
		// For the ALT fire
		theFxScheduler.RegisterEffect( "atst/side_alt_shot" );
		theFxScheduler.RegisterEffect( "atst/side_alt_explosion" );
	
		// For the regular fire
		theFxScheduler.RegisterEffect( "atst/side_main_shot" );
		theFxScheduler.RegisterEffect( "atst/side_main_impact" );
		break;

	case WP_FLECHETTE:
		// SHRAPNEL
		cgs.effects.shrapnel0ShotEffect				= theFxScheduler.RegisterEffect( "flechette0/shot" );
		cgs.effects.shrapnel0AltShotEffect			= theFxScheduler.RegisterEffect( "flechette0/alt_shot" );
		cgs.effects.shrapnel0ShotDeathEffect		= theFxScheduler.RegisterEffect( "flechette0/wall_impact" ); // shot death
		cgs.effects.shrapnel0FleshImpactEffect		= theFxScheduler.RegisterEffect( "flechette0/flesh_impact" );
		cgs.effects.shrapnel0RicochetEffect			= theFxScheduler.RegisterEffect( "flechette0/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette0/explosion" );
		theFxScheduler.RegisterEffect( "flechette0/alt_blow" );

		// EARTH 
		cgs.effects.flechetteShotEffect				= theFxScheduler.RegisterEffect( "flechette/shot" );
		cgs.effects.flechetteAltShotEffect			= theFxScheduler.RegisterEffect( "flechette/alt_shot" );
		cgs.effects.flechetteShotDeathEffect		= theFxScheduler.RegisterEffect( "flechette/wall_impact" ); // shot death
		cgs.effects.flechetteFleshImpactEffect		= theFxScheduler.RegisterEffect( "flechette/flesh_impact" );
		cgs.effects.flechetteRicochetEffect			= theFxScheduler.RegisterEffect( "flechette/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "flechette/alt_blow" );

		// FANGO
		cgs.effects.mudShotEffect				= theFxScheduler.RegisterEffect( "flechette2/shot" );
		cgs.effects.mudAltShotEffect			= theFxScheduler.RegisterEffect( "flechette2/alt_shot" );
		cgs.effects.mudShotDeathEffect		= theFxScheduler.RegisterEffect( "flechette2/wall_impact" ); // shot death
		cgs.effects.mudFleshImpactEffect		= theFxScheduler.RegisterEffect( "flechette2/flesh_impact" );
		cgs.effects.mudRicochetEffect			= theFxScheduler.RegisterEffect( "flechette2/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "flechette2/alt_blow" );
		// PIETRA
		cgs.effects.stoneShotEffect				= theFxScheduler.RegisterEffect( "flechette3/shot" );
		cgs.effects.stoneAltShotEffect			= theFxScheduler.RegisterEffect( "flechette3/alt_shot" );
		cgs.effects.stoneShotDeathEffect		= theFxScheduler.RegisterEffect( "flechette3/wall_impact" ); // shot death
		cgs.effects.stoneFleshImpactEffect		= theFxScheduler.RegisterEffect( "flechette3/flesh_impact" );
		cgs.effects.stoneRicochetEffect			= theFxScheduler.RegisterEffect( "flechette3/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "flechette3/alt_blow" );

		// ASH
		cgs.effects.ashShotEffect				= theFxScheduler.RegisterEffect( "ash/shot" );
		cgs.effects.ashAltShotEffect			= theFxScheduler.RegisterEffect( "ash/alt_shot" );
		cgs.effects.ashShotDeathEffect		= theFxScheduler.RegisterEffect( "ash/wall_impact" ); // shot death
		cgs.effects.ashFleshImpactEffect		= theFxScheduler.RegisterEffect( "ash/flesh_impact" );
		cgs.effects.ashRicochetEffect			= theFxScheduler.RegisterEffect( "ash/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "ash/alt_blow" );

		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "dust/alt_blow" );
		// IVY
		cgs.effects.ivyShotEffect				= theFxScheduler.RegisterEffect( "ivy/shot" );
		cgs.effects.ivyAltShotEffect			= theFxScheduler.RegisterEffect( "ivy/alt_shot" );
		cgs.effects.ivyShotDeathEffect		= theFxScheduler.RegisterEffect( "ivy/wall_impact" ); // shot death
		cgs.effects.ivyFleshImpactEffect		= theFxScheduler.RegisterEffect( "ivy/flesh_impact" );
		cgs.effects.ivyRicochetEffect			= theFxScheduler.RegisterEffect( "ivy/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "ivy/alt_blow" );

		// HELLISH IVY
		cgs.effects.ivy2ShotEffect				= theFxScheduler.RegisterEffect( "ivy2/shot" );
		cgs.effects.ivy2AltShotEffect			= theFxScheduler.RegisterEffect( "ivy2/alt_shot" );
		cgs.effects.ivy2ShotDeathEffect		= theFxScheduler.RegisterEffect( "ivy2/wall_impact" ); // shot death
		cgs.effects.ivy2FleshImpactEffect		= theFxScheduler.RegisterEffect( "ivy2/flesh_impact" );
		cgs.effects.ivy2RicochetEffect			= theFxScheduler.RegisterEffect( "ivy2/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "ivy2/alt_blow" );


		// ACID
		cgs.effects.acidShotEffect				= theFxScheduler.RegisterEffect( "acid/shot" );
		cgs.effects.acidAltShotEffect			= theFxScheduler.RegisterEffect( "acid/alt_shot" );
		cgs.effects.acidShotDeathEffect		= theFxScheduler.RegisterEffect( "acid/wall_impact" ); // shot death
		cgs.effects.acidFleshImpactEffect		= theFxScheduler.RegisterEffect( "acid/flesh_impact" );
		cgs.effects.acidRicochetEffect			= theFxScheduler.RegisterEffect( "acid/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "acid/alt_blow" );
		// SPECTRAL
		cgs.effects.spectralShotEffect				= theFxScheduler.RegisterEffect( "spectral/shot" );
		cgs.effects.spectralAltShotEffect			= theFxScheduler.RegisterEffect( "spectral/alt_shot" );
		cgs.effects.spectralShotDeathEffect		= theFxScheduler.RegisterEffect( "spectral/wall_impact" ); // shot death
		cgs.effects.spectralFleshImpactEffect		= theFxScheduler.RegisterEffect( "spectral/flesh_impact" );
		cgs.effects.spectralRicochetEffect			= theFxScheduler.RegisterEffect( "spectral/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "spectral/alt_blow" );
		// THORN
		cgs.effects.thornShotEffect				= theFxScheduler.RegisterEffect( "thorn/shot" );
		cgs.effects.thornAltShotEffect			= theFxScheduler.RegisterEffect( "thorn/alt_shot" );
		cgs.effects.thornShotDeathEffect		= theFxScheduler.RegisterEffect( "thorn/wall_impact" ); // shot death
		cgs.effects.thornFleshImpactEffect		= theFxScheduler.RegisterEffect( "thorn/flesh_impact" );
		cgs.effects.thornRicochetEffect			= theFxScheduler.RegisterEffect( "thorn/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "thorn/alt_blow" );

		// STASIS
		cgs.effects.stasisShotEffect				= theFxScheduler.RegisterEffect( "stasis/shot" );
		cgs.effects.stasisAltShotEffect			= theFxScheduler.RegisterEffect( "stasis/alt_shot" );
		cgs.effects.stasisShotDeathEffect		= theFxScheduler.RegisterEffect( "stasis/wall_impact" ); // shot death
		cgs.effects.stasisFleshImpactEffect		= theFxScheduler.RegisterEffect( "stasis/flesh_impact" );
		cgs.effects.stasisRicochetEffect			= theFxScheduler.RegisterEffect( "stasis/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "stasis/alt_blow" );

		// GREEN CRYSTAL
		cgs.effects.gcrystalShotEffect				= theFxScheduler.RegisterEffect( "gcrystal/shot" );
		cgs.effects.gcrystalAltShotEffect			= theFxScheduler.RegisterEffect( "gcrystal/alt_shot" );
		cgs.effects.gcrystalShotDeathEffect		= theFxScheduler.RegisterEffect( "gcrystal/wall_impact" ); // shot death
		cgs.effects.gcrystalFleshImpactEffect		= theFxScheduler.RegisterEffect( "gcrystal/flesh_impact" );
		cgs.effects.gcrystalRicochetEffect			= theFxScheduler.RegisterEffect( "gcrystal/ricochet" );

//		theFxScheduler.RegisterEffect( "flechette/explosion" );
		theFxScheduler.RegisterEffect( "gcrystal/alt_blow" );

		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("flechette/muzzle_flash");
		theFxScheduler.RegisterEffect("flechette/altmuzzle_flash");
		theFxScheduler.RegisterEffect("flechette2/muzzle_flash");
		theFxScheduler.RegisterEffect("flechette2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("flechette3/muzzle_flash");
		theFxScheduler.RegisterEffect("flechette3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("acid/muzzle_flash");
		theFxScheduler.RegisterEffect("acid/altmuzzle_flash");
		theFxScheduler.RegisterEffect("thorn/muzzle_flash");
		theFxScheduler.RegisterEffect("thorn/altmuzzle_flash");
		theFxScheduler.RegisterEffect("ash/muzzle_flash");
		theFxScheduler.RegisterEffect("ash/altmuzzle_flash");
		theFxScheduler.RegisterEffect("dust/muzzle_flash");
		theFxScheduler.RegisterEffect("dust/altmuzzle_flash");	
		theFxScheduler.RegisterEffect("spectral/muzzle_flash");
		theFxScheduler.RegisterEffect("spectral/altmuzzle_flash");
		theFxScheduler.RegisterEffect("ivy/muzzle_flash");
		theFxScheduler.RegisterEffect("ivy/altmuzzle_flash");
		theFxScheduler.RegisterEffect("ivy2/muzzle_flash");
		theFxScheduler.RegisterEffect("ivy2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("stasis/muzzle_flash");
		theFxScheduler.RegisterEffect("stasis/altmuzzle_flash");
		theFxScheduler.RegisterEffect("stasis/muzzle_flash");
		theFxScheduler.RegisterEffect("stasis/altmuzzle_flash");
		theFxScheduler.RegisterEffect("gcrystal/muzzle_flash");
		theFxScheduler.RegisterEffect("gcrystal/altmuzzle_flash");
		break;

        case WP_CANNON:
		cgs.effects.cannonShotEffect				= theFxScheduler.RegisterEffect( "cannon/shot" );
		cgs.effects.cannonAltShotEffect			= theFxScheduler.RegisterEffect( "cannon/alt_shot" );
		cgs.effects.cannonShotDeathEffect		= theFxScheduler.RegisterEffect( "cannon/wall_impact" ); // shot death
		cgs.effects.cannonFleshImpactEffect		= theFxScheduler.RegisterEffect( "cannon/flesh_impact" );
		cgs.effects.cannonRicochetEffect			= theFxScheduler.RegisterEffect( "cannon/ricochet" );

//		theFxScheduler.RegisterEffect( "cannon/explosion" );
		theFxScheduler.RegisterEffect( "cannon/alt_blow" );
		break;

	case WP_ROCKET_LAUNCHER:
		// SEEKER 
		theFxScheduler.RegisterEffect( "rocket0/shot" );
		theFxScheduler.RegisterEffect( "rocket0/explosion" );

		// AIR 
		theFxScheduler.RegisterEffect( "rocket/shot" );
		theFxScheduler.RegisterEffect( "rocket/explosion" );
		// NEBBIA
		theFxScheduler.RegisterEffect( "rocket2/shot" );
		theFxScheduler.RegisterEffect( "rocket2/explosion" );
		// TEMPESTA
		theFxScheduler.RegisterEffect( "rocket3/shot" );
		theFxScheduler.RegisterEffect( "rocket3/explosion" );
		// FLAY
		theFxScheduler.RegisterEffect( "rocket4/shot" );
		theFxScheduler.RegisterEffect( "rocket4/explosion" );
		theFxScheduler.RegisterEffect( "rocket4/flesh_impact" );
		// BLACK HOLE
		theFxScheduler.RegisterEffect( "blackhole/shot" );
		theFxScheduler.RegisterEffect( "blackhole/hitp" );
		theFxScheduler.RegisterEffect( "blackhole/explosion" );
		// ELDER GOD SPHERE
		theFxScheduler.RegisterEffect( "eldersphere/shot" );
		theFxScheduler.RegisterEffect( "eldersphere/altshot" );
		theFxScheduler.RegisterEffect( "eldersphere/explosion" );
		theFxScheduler.RegisterEffect( "eldersphere/altexplosion" );
		theFxScheduler.RegisterEffect( "eldersphere/flesh_impact" );
		theFxScheduler.RegisterEffect( "eldersphere/altflesh_impact" );

		// THRALL BREATH
		theFxScheduler.RegisterEffect( "thrallbreath/shot" );
		theFxScheduler.RegisterEffect( "thrallbreath/hitp" );
		theFxScheduler.RegisterEffect( "thrallbreath/explosion" );
		// POISON FOG
		theFxScheduler.RegisterEffect( "poisonfog/shot" );
		theFxScheduler.RegisterEffect( "poisonfog/hitp" );
		theFxScheduler.RegisterEffect( "poisonfog/explosion" );
		// SPECTRAL FOG
		theFxScheduler.RegisterEffect( "spectralfog/shot" );
		theFxScheduler.RegisterEffect( "spectralfog/hitp" );
		theFxScheduler.RegisterEffect( "spectralfog/explosion" );
		// SANDSTORM
		theFxScheduler.RegisterEffect( "sandstorm/shot" );
		theFxScheduler.RegisterEffect( "sandstorm/hitp" );
		theFxScheduler.RegisterEffect( "sandstorm/explosion" );
		// TELEKINESIS
		theFxScheduler.RegisterEffect( "tk/shot" );
		theFxScheduler.RegisterEffect( "tk/hitp" );
		theFxScheduler.RegisterEffect( "tk/explosion" );
		// GRAVITY
		theFxScheduler.RegisterEffect( "gravity/shot" );
		theFxScheduler.RegisterEffect( "gravity/hitp" );
		theFxScheduler.RegisterEffect( "gravity/explosion" );
		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("rocket/muzzle_flash");
		theFxScheduler.RegisterEffect("rocket/altmuzzle_flash");
		theFxScheduler.RegisterEffect("rocket2/muzzle_flash");
		theFxScheduler.RegisterEffect("rocket2/altmuzzle_flash");
		theFxScheduler.RegisterEffect("rocket3/muzzle_flash");
		theFxScheduler.RegisterEffect("rocket3/altmuzzle_flash");
		theFxScheduler.RegisterEffect("rocket4/muzzle_flash");
		theFxScheduler.RegisterEffect("rocket4/altmuzzle_flash");
		theFxScheduler.RegisterEffect("tk/muzzle_flash");
		theFxScheduler.RegisterEffect("tk/altmuzzle_flash");
		theFxScheduler.RegisterEffect("sandstorm/muzzle_flash");
		theFxScheduler.RegisterEffect("sandstorm/altmuzzle_flash");
		theFxScheduler.RegisterEffect("eldersphere/muzzle_flash");
		theFxScheduler.RegisterEffect("eldersphere/altmuzzle_flash");	
		theFxScheduler.RegisterEffect("blackhole/muzzle_flash");
		theFxScheduler.RegisterEffect("blackhole/altmuzzle_flash");
		theFxScheduler.RegisterEffect("thrallbreath/muzzle_flash");
		theFxScheduler.RegisterEffect("thrallbreath/altmuzzle_flash");
		theFxScheduler.RegisterEffect("poisonfog/muzzle_flash");
		theFxScheduler.RegisterEffect("poisonfog/altmuzzle_flash");
		theFxScheduler.RegisterEffect("spectralfog/muzzle_flash");
		theFxScheduler.RegisterEffect("spectralfog/altmuzzle_flash");
		theFxScheduler.RegisterEffect("gravity/muzzle_flash");
		theFxScheduler.RegisterEffect("gravity/altmuzzle_flash");
		cgi_R_RegisterShaderNoMip( "gfx/2d/wedge" );
		cgi_R_RegisterShaderNoMip( "gfx/2d/lock" );

		cgi_S_RegisterSound( "sound/weapons/rocket/lock.wav" );
		cgi_S_RegisterSound( "sound/weapons/rocket/tick.wav" );
		break;

	case WP_CONCUSSION:
		// ROCKET ANTIMATTER! 
		//Primary
		theFxScheduler.RegisterEffect( "concussion0/shot" );
		theFxScheduler.RegisterEffect( "concussion0/explosion" );
		//Alt
		theFxScheduler.RegisterEffect( "concussion0/alt_miss" );
		theFxScheduler.RegisterEffect( "concussion0/alt_hit" );
		theFxScheduler.RegisterEffect( "concussion0/alt_ring" );
		
		//HELL STRIKE 
		//Primary
		theFxScheduler.RegisterEffect( "concussion/shot" );
		theFxScheduler.RegisterEffect( "concussion/explosion" );
		//Alt
		theFxScheduler.RegisterEffect( "concussion/alt_miss" );
		theFxScheduler.RegisterEffect( "concussion/alt_hit" );
		theFxScheduler.RegisterEffect( "concussion/alt_ring" );

		// NECROMANTIC POWERS! 
		// BLACK FIRE 
		theFxScheduler.RegisterEffect( "blackfire/shot" );
		theFxScheduler.RegisterEffect( "blackfire/explosion" );
		theFxScheduler.RegisterEffect( "blackfire/alt_miss" );
		theFxScheduler.RegisterEffect( "blackfire/alt_hit" );
		theFxScheduler.RegisterEffect( "blackfire/alt_ring" );
		// SPIRITH DEATH  
		theFxScheduler.RegisterEffect( "spirith_death/shot" );
		theFxScheduler.RegisterEffect( "spirith_death/explosion" );
		theFxScheduler.RegisterEffect( "spirith_death/alt_miss" );
		theFxScheduler.RegisterEffect( "spirith_death/alt_hit" );
		theFxScheduler.RegisterEffect( "spirith_death/alt_ring" );
		// IMMOLATE 
		theFxScheduler.RegisterEffect( "immolate/shot" );
		theFxScheduler.RegisterEffect( "immolate/explosion" );
		theFxScheduler.RegisterEffect( "immolate/alt_miss" );
		theFxScheduler.RegisterEffect( "immolate/alt_hit" );
		theFxScheduler.RegisterEffect( "immolate/alt_ring" );
		// HOLYIMMOLATE 
		theFxScheduler.RegisterEffect( "holyimmolate/shot" );
		theFxScheduler.RegisterEffect( "holyimmolate/explosion" );
		theFxScheduler.RegisterEffect( "holyimmolate/alt_miss" );
		theFxScheduler.RegisterEffect( "holyimmolate/alt_hit" );
		theFxScheduler.RegisterEffect( "holyimmolate/alt_ring" );
		// NECRO MIND
		theFxScheduler.RegisterEffect( "necro_fog/shot" );
		theFxScheduler.RegisterEffect( "necro_fog/explosion" );
		theFxScheduler.RegisterEffect( "necro_fog/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_fog/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_fog/alt_ring" );
		// NECRO DIMENSION  
		theFxScheduler.RegisterEffect( "necro_dim/shot" );
		theFxScheduler.RegisterEffect( "necro_dim/explosion" );
		theFxScheduler.RegisterEffect( "necro_dim/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_dim/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_dim/alt_ring" );
		// NECRO CONFLICT 
		theFxScheduler.RegisterEffect( "necro_conflict/shot" );
		theFxScheduler.RegisterEffect( "necro_conflict/explosion" );
		theFxScheduler.RegisterEffect( "necro_conflict/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_conflict/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_conflict/alt_ring" );
		// NECRO NATURE
		theFxScheduler.RegisterEffect( "necro_nature/shot" );
		theFxScheduler.RegisterEffect( "necro_nature/explosion" );
		theFxScheduler.RegisterEffect( "necro_nature/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_nature/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_nature/alt_ring" );
		// NECRO ENERGY 
		theFxScheduler.RegisterEffect( "necro_energy/shot" );
		theFxScheduler.RegisterEffect( "necro_energy/explosion" );
		theFxScheduler.RegisterEffect( "necro_energy/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_energy/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_energy/alt_ring" );
		// NECRO ELECTRO 
		theFxScheduler.RegisterEffect( "necro_shock/shot" );
		theFxScheduler.RegisterEffect( "necro_shock/explosion" );
		theFxScheduler.RegisterEffect( "necro_shock/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_shock/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_shock/alt_ring" );
		// NECRO POISON
		theFxScheduler.RegisterEffect( "necro_poison/shot" );
		theFxScheduler.RegisterEffect( "necro_poison/explosion" );
		theFxScheduler.RegisterEffect( "necro_poison/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_poison/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_poison/alt_ring" );
		// NECRO TIME  
		theFxScheduler.RegisterEffect( "necro_time/shot" );
		theFxScheduler.RegisterEffect( "necro_time/explosion" );
		theFxScheduler.RegisterEffect( "necro_time/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_time/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_time/alt_ring" );
		// NECRO STONE  
		theFxScheduler.RegisterEffect( "necro_stone/shot" );
		theFxScheduler.RegisterEffect( "necro_stone/explosion" );
		theFxScheduler.RegisterEffect( "necro_stone/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_stone/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_stone/alt_ring" );
		// NECRO CHAOS
		theFxScheduler.RegisterEffect( "necro_chaos/shot" );
		theFxScheduler.RegisterEffect( "necro_chaos/explosion" );
		theFxScheduler.RegisterEffect( "necro_chaos/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_chaos/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_chaos/alt_ring" );
		// NECRO DEATH  
		theFxScheduler.RegisterEffect( "necro/shot" );
		theFxScheduler.RegisterEffect( "necro/explosion" );
		theFxScheduler.RegisterEffect( "necro/alt_miss" );
		theFxScheduler.RegisterEffect( "necro/alt_hit" );
		theFxScheduler.RegisterEffect( "necro/alt_ring" );
		// NECRO SMOG  
		theFxScheduler.RegisterEffect( "necro_smog/shot" );
		theFxScheduler.RegisterEffect( "necro_smog/explosion" );
		theFxScheduler.RegisterEffect( "necro_smog/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_smog/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_smog/alt_ring" );
		// NECRO VOID  
		theFxScheduler.RegisterEffect( "necro_void/shot" );
		theFxScheduler.RegisterEffect( "necro_void/explosion" );
		theFxScheduler.RegisterEffect( "necro_void/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_void/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_void/alt_ring" );
		// NECRO MUD  
		theFxScheduler.RegisterEffect( "necro_mud/shot" );
		theFxScheduler.RegisterEffect( "necro_mud/explosion" );
		theFxScheduler.RegisterEffect( "necro_mud/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_mud/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_mud/alt_ring" );
		// NECRO CRYSTAL  
		theFxScheduler.RegisterEffect( "necro_crystal/shot" );
		theFxScheduler.RegisterEffect( "necro_crystal/explosion" );
		theFxScheduler.RegisterEffect( "necro_crystal/alt_miss" );
		theFxScheduler.RegisterEffect( "necro_crystal/alt_hit" );
		theFxScheduler.RegisterEffect( "necro_crystal/alt_ring" );
		// REGISTERING MUZZLE FLASHES 
		theFxScheduler.RegisterEffect("concussion/muzzle_flash");
		theFxScheduler.RegisterEffect("concussion/altmuzzle_flash");
		theFxScheduler.RegisterEffect("spirith_death/muzzle_flash");
		theFxScheduler.RegisterEffect("spirith_death/altmuzzle_flash");
		theFxScheduler.RegisterEffect("blackfire/muzzle_flash");
		theFxScheduler.RegisterEffect("blackfire/altmuzzle_flash");
		theFxScheduler.RegisterEffect("immolate/muzzle_flash");
		theFxScheduler.RegisterEffect("immolate/altmuzzle_flash");
		theFxScheduler.RegisterEffect("holyimmolate/muzzle_flash");
		theFxScheduler.RegisterEffect("holyimmolate/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_fog/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_fog/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_mud/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_mud/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_poison/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_poison/altmuzzle_flash");	
		theFxScheduler.RegisterEffect("necro_smog/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_smog/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_dim/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_dim/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_conflict/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_conflict/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_nature/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_nature/altmuzzle_flash");	
		theFxScheduler.RegisterEffect("necro_time/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_time/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_energy/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_energy/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_stone/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_stone/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_crystal/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_crystal/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_void/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_void/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro_chaos/muzzle_flash");
		theFxScheduler.RegisterEffect("necro_chaos/altmuzzle_flash");
		theFxScheduler.RegisterEffect("necro/muzzle_flash");
		theFxScheduler.RegisterEffect("necro/altmuzzle_flash");

		//not used (eventually)?
		cgi_R_RegisterShader( "gfx/effects/blueLine" );
		cgi_R_RegisterShader( "gfx/misc/whiteline2" );
		/*cgi_R_RegisterShader( "gfx/effects/sabers/purple_Line" );
		cgi_R_RegisterShader( "gfx/effects/sabers/red_line" );
		cgi_R_RegisterShader( "gfx/effects/sabers/yellow_line" );
		cgi_R_RegisterShader( "gfx/effects/sabers/orange_line" );
		cgi_R_RegisterShader( "gfx/effects/sabers/blue_line" );
		cgi_R_RegisterShader( "gfx/effects/sabers/green_line" );
		cgi_R_RegisterShader( "gfx/misc/blueLine" );
		cgi_R_RegisterShader( "gfx/damage/burnmark3" );
		cgi_R_RegisterShader( "gfx/effects/black_smoke" );*/
		break;

	case WP_THERMAL:
		cgs.media.grenadeBounce1		= cgi_S_RegisterSound( "sound/weapons/thermal/bounce1.wav" );
		cgs.media.grenadeBounce2		= cgi_S_RegisterSound( "sound/weapons/thermal/bounce2.wav" );

		cgi_S_RegisterSound( "sound/weapons/thermal/thermloop.wav" );
		cgi_S_RegisterSound( "sound/weapons/thermal/warning.wav" );
		// BOMBA ACIDA 
		theFxScheduler.RegisterEffect( "thermal/explosion" );
		theFxScheduler.RegisterEffect( "thermal/shockwave" );

		// BOMBA OSCURA
		theFxScheduler.RegisterEffect( "thermal2/explosion" );
		theFxScheduler.RegisterEffect( "thermal2/shockwave" );
		// BOMBA DI LUCE
		theFxScheduler.RegisterEffect( "thermal3/explosion" );
		theFxScheduler.RegisterEffect( "thermal3/shockwave" );
		// BOMBA DI FUOCO
		theFxScheduler.RegisterEffect( "thermal4/explosion" );
		theFxScheduler.RegisterEffect( "thermal4/shockwave" );
		// BOMBA DI GHIACCIO
		theFxScheduler.RegisterEffect( "thermal5/explosion" );
		theFxScheduler.RegisterEffect( "thermal5/shockwave" );
		// BOMBA DI SUONO
		theFxScheduler.RegisterEffect( "thermal6/explosion" );
		theFxScheduler.RegisterEffect( "thermal6/shockwave" );
		// BOMBA DI PIETRA
		theFxScheduler.RegisterEffect( "thermal7/explosion" );
		theFxScheduler.RegisterEffect( "thermal7/shockwave" );
		// BOMBA DI FORZA 
		theFxScheduler.RegisterEffect( "thermal8/explosion" );
		theFxScheduler.RegisterEffect( "thermal8/shockwave" );
		// BOMBA ELETTRICA 
		theFxScheduler.RegisterEffect( "thermal9/explosion" );
		theFxScheduler.RegisterEffect( "thermal9/shockwave" );
		// BOMBA SACRA
		theFxScheduler.RegisterEffect( "thermal10/explosion" );
		theFxScheduler.RegisterEffect( "thermal10/shockwave" );
		// BOMBA MORTIFERA 
		theFxScheduler.RegisterEffect( "thermal11/explosion" );
		theFxScheduler.RegisterEffect( "thermal11/shockwave" );
		// BOMBA GLIFICA
		theFxScheduler.RegisterEffect( "thermal12/explosion" );
		theFxScheduler.RegisterEffect( "thermal12/shockwave" );
		// BOMBA MAZZA
		theFxScheduler.RegisterEffect( "thermal13/explosion" );
		theFxScheduler.RegisterEffect( "thermal13/shockwave" );
		// PIANTE SPUNTONE
		theFxScheduler.RegisterEffect( "thermal14/explosion" );
		theFxScheduler.RegisterEffect( "thermal14/shockwave" );
		// CRISTALLI DI GHIACCIO 
		theFxScheduler.RegisterEffect( "thermal15/explosion" );
		theFxScheduler.RegisterEffect( "thermal15/shockwave" );
		// CROCI 
		theFxScheduler.RegisterEffect( "thermal16/explosion" );
		theFxScheduler.RegisterEffect( "thermal16/shockwave" );
		// BARA 
		theFxScheduler.RegisterEffect( "thermal17/explosion" );
		theFxScheduler.RegisterEffect( "thermal17/shockwave" );
		// ANGELO 
		theFxScheduler.RegisterEffect( "thermal18/explosion" );
		theFxScheduler.RegisterEffect( "thermal18/shockwave" );
		break;

	case WP_TRIP_MINE:
		theFxScheduler.RegisterEffect( "tripMine/explosion" );
		theFxScheduler.RegisterEffect( "tripMine/laser" );
		theFxScheduler.RegisterEffect( "tripMine/laserImpactGlow" );
		theFxScheduler.RegisterEffect( "tripMine/glowBit" );
		cgs.media.tripMineStickSound = cgi_S_RegisterSound( "sound/weapons/laser_trap/stick.wav" );
		cgi_S_RegisterSound( "sound/weapons/laser_trap/warning.wav" );
		cgi_S_RegisterSound( "sound/weapons/laser_trap/hum_loop.wav" );
		// RADICI IMPALANTI
		theFxScheduler.RegisterEffect( "evilplant/explosion" );
		theFxScheduler.RegisterEffect( "evilplant/laser" );
		theFxScheduler.RegisterEffect( "evilplant/laserImpactGlow" );
		theFxScheduler.RegisterEffect( "evilplant/glowBit" );
		cgs.media.tripMineStickSound = cgi_S_RegisterSound( "sound/weapons/evilplant/stick.wav" );
		cgi_S_RegisterSound( "sound/weapons/evilplant/warning.wav" );
		cgi_S_RegisterSound( "sound/weapons/evilplant/hum_loop.wav" );
		break;

	case WP_DET_PACK:
		theFxScheduler.RegisterEffect( "detpack/explosion.efx" );
		
		// RAZIELHIM BOMB
		theFxScheduler.RegisterEffect( "detpack2/explosion.efx" );
		// RUNE
		theFxScheduler.RegisterEffect( "detpack3/explosion.efx" );

		cgs.media.detPackStickSound = cgi_S_RegisterSound( "sound/weapons/detpack/stick.wav" );
		cgi_R_RegisterModel( "models/weapons2/detpack/detpack.md3" );
		cgi_S_RegisterSound( "sound/weapons/detpack/warning.wav" );
		cgi_S_RegisterSound( "sound/weapons/explosions/explode5.wav" );
		break;

	case WP_EMPLACED_GUN:
		theFxScheduler.RegisterEffect( "emplaced/shot" );
		theFxScheduler.RegisterEffect( "emplaced/shotNPC" );
		theFxScheduler.RegisterEffect( "emplaced/wall_impact" );
		//E-Web, too, can't tell here which one you wanted, so...
		theFxScheduler.RegisterEffect( "eweb/shot" );
		theFxScheduler.RegisterEffect( "eweb/shotNPC" );
		theFxScheduler.RegisterEffect( "eweb/wall_impact" );
		theFxScheduler.RegisterEffect( "eweb/flesh_impact" );

		cgi_R_RegisterShader( "models/map_objects/imp_mine/turret_chair_dmg" );
		cgi_R_RegisterShader( "models/map_objects/imp_mine/turret_chair_on" );

		cgs.media.emplacedHealthBarShader		= cgi_R_RegisterShaderNoMip( "gfx/hud/health_frame" );
		cgs.media.turretComputerOverlayShader	= cgi_R_RegisterShaderNoMip( "gfx/hud/generic_target" );
		cgs.media.turretCrossHairShader			= cgi_R_RegisterShaderNoMip( "gfx/2d/panel_crosshair" );
		break;

	case WP_MELEE:
	case WP_TUSKEN_STAFF:
		//TEMP
		theFxScheduler.RegisterEffect( "melee/punch_impact" );
		theFxScheduler.RegisterEffect( "melee/kick_impact" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch1.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch2.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch3.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch4.mp3" );
		break;

	case WP_STUN_BATON:
		cgi_R_RegisterShader( "gfx/effects/stunPass" );
		theFxScheduler.RegisterEffect( "stunBaton/flesh_impact" );
		//TEMP
		cgi_S_RegisterSound( "sound/weapons/melee/punch1.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch2.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch3.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch4.mp3" );
		cgi_S_RegisterSound( "sound/weapons/baton/fire" );
		break;

	case WP_TURRET:
		theFxScheduler.RegisterEffect( "turret/shot" );
		theFxScheduler.RegisterEffect( "turret/wall_impact" );
		theFxScheduler.RegisterEffect( "turret/flesh_impact" );
		break;

	case WP_TUSKEN_RIFLE:
		//melee
		theFxScheduler.RegisterEffect( "melee/punch_impact" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch1.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch2.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch3.mp3" );
		cgi_S_RegisterSound( "sound/weapons/melee/punch4.mp3" );
		//fire
		theFxScheduler.RegisterEffect( "tusken/shot" );
		theFxScheduler.RegisterEffect( "tusken/hit" );
		theFxScheduler.RegisterEffect( "tusken/hitwall" );
		// force
		theFxScheduler.RegisterEffect( "tusken2/shot" );
		theFxScheduler.RegisterEffect( "tusken2/hit" );
		theFxScheduler.RegisterEffect( "tusken2/hitwall" );
		// stone
		theFxScheduler.RegisterEffect( "tusken3/shot" );
		theFxScheduler.RegisterEffect( "tusken3/hit" );
		theFxScheduler.RegisterEffect( "tusken3/hitwall" );
		// sound
		theFxScheduler.RegisterEffect( "tusken4/shot" );
		theFxScheduler.RegisterEffect( "tusken4/hit" );
		theFxScheduler.RegisterEffect( "tusken4/hitwall" );
		// water
		theFxScheduler.RegisterEffect( "tusken5/shot" );
		theFxScheduler.RegisterEffect( "tusken5/hit" );
		theFxScheduler.RegisterEffect( "tusken5/hitwall" );
		// sunlight 
		theFxScheduler.RegisterEffect( "tusken6/shot" );
		theFxScheduler.RegisterEffect( "tusken6/hit" );
		theFxScheduler.RegisterEffect( "tusken6/hitwall" );
		// holy
		theFxScheduler.RegisterEffect( "tusken7/shot" );
		theFxScheduler.RegisterEffect( "tusken7/hit" );
		theFxScheduler.RegisterEffect( "tusken7/hitwall" );
		// vampires
		theFxScheduler.RegisterEffect( "tusken8/shot" );
		theFxScheduler.RegisterEffect( "tusken8/hit" );
		theFxScheduler.RegisterEffect( "tusken8/hitwall" );
		// hylden
		theFxScheduler.RegisterEffect( "tusken9/shot" );
		theFxScheduler.RegisterEffect( "tusken9/hit" );
		theFxScheduler.RegisterEffect( "tusken9/hitwall" );
		// veleno 
		theFxScheduler.RegisterEffect( "tusken10/shot" );
		theFxScheduler.RegisterEffect( "tusken10/hit" );
		theFxScheduler.RegisterEffect( "tusken10/hitwall" );
		// undead
		theFxScheduler.RegisterEffect( "tusken0/shot" );
		theFxScheduler.RegisterEffect( "tusken0/hit" );
		theFxScheduler.RegisterEffect( "tusken0/hitwall" );
		// dark 
		theFxScheduler.RegisterEffect( "tusken1/shot" );
		theFxScheduler.RegisterEffect( "tusken1/hit" );
		theFxScheduler.RegisterEffect( "tusken1/hitwall" );
		// NEW MUZZLE FLASH EFX 
		theFxScheduler.RegisterEffect( "tusken/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken2/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken3/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken4/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken5/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken6/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken7/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken8/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken9/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken0/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken10/muzzle_flash" );
		theFxScheduler.RegisterEffect( "tusken1/muzzle_flash" );
		break;

	case WP_SCEPTER:
		//???
		break;

	case WP_NOGHRI_STICK:
		//fire
		theFxScheduler.RegisterEffect( "noghri_stick/shot" );
		theFxScheduler.RegisterEffect( "noghri_stick/flesh_impact" );
		//explosion
		theFxScheduler.RegisterEffect( "noghri_stick/gas_cloud" );
		//cgi_S_RegisterSound("sound/weapons/noghri/smoke.wav");
		break;

	case WP_TIE_FIGHTER:
		theFxScheduler.RegisterEffect( "ships/imp_blastershot" );
		break;

	}
}

/*
=================
CG_RegisterItemVisuals

The server says this item is used on this level
=================
*/
void CG_RegisterItemVisuals( int itemNum ) {
	itemInfo_t		*itemInfo;
	gitem_t			*item;

	itemInfo = &cg_items[ itemNum ];
	if ( itemInfo->registered ) {
		return;
	}

	item = &bg_itemlist[ itemNum ];

	memset( itemInfo, 0, sizeof( *itemInfo ) );
	itemInfo->registered = qtrue;

	itemInfo->models = cgi_R_RegisterModel( item->world_model );

	if ( item->icon && item->icon[0] )
	{
		itemInfo->icon = cgi_R_RegisterShaderNoMip( item->icon );
	}
	else
	{
		itemInfo->icon = -1;
	}

	if ( item->giType == IT_WEAPON ) 
	{
		CG_RegisterWeapon( item->giTag );
	}

	// some ammo types are actually the weapon, like in the case of explosives
	if ( item->giType == IT_AMMO ) 
	{
		switch( item->giTag )
		{
		case AMMO_THERMAL:
			CG_RegisterWeapon( WP_THERMAL );
			break;
		case AMMO_TRIPMINE:
			CG_RegisterWeapon( WP_TRIP_MINE );
			break;
		case AMMO_DETPACK:
			CG_RegisterWeapon( WP_DET_PACK );
			break;
		}
	}


	if ( item->giType == IT_HOLDABLE )
	{
		// This should be set up to actually work.
		switch( item->giTag )
		{
		case INV_SEEKER:
			cgi_S_RegisterSound("sound/chars/seeker/misc/fire.wav");
			cgi_S_RegisterSound( "sound/chars/seeker/misc/hiss.wav");
			theFxScheduler.RegisterEffect( "env/small_explode");

			CG_RegisterWeapon( WP_BLASTER );
			break;

		case INV_SENTRY:
			CG_RegisterWeapon( WP_TURRET );
			cgi_S_RegisterSound( "sound/player/use_sentry" );
			break;

		case INV_ELECTROBINOCULARS:
			// Binocular interface
			cgs.media.binocularCircle		= cgi_R_RegisterShader( "gfx/2d/binCircle" );
			cgs.media.binocularMask			= cgi_R_RegisterShader( "gfx/2d/binMask" );
			cgs.media.binocularArrow		= cgi_R_RegisterShader( "gfx/2d/binSideArrow" );
			cgs.media.binocularTri			= cgi_R_RegisterShader( "gfx/2d/binTopTri" );
			cgs.media.binocularStatic		= cgi_R_RegisterShader( "gfx/2d/binocularWindow" );
			cgs.media.binocularOverlay		= cgi_R_RegisterShader( "gfx/2d/binocularNumOverlay" );
			break;

		case INV_LIGHTAMP_GOGGLES:
			// LA Goggles Shaders
			cgs.media.laGogglesStatic		= cgi_R_RegisterShader( "gfx/2d/lagogglesWindow" );
			cgs.media.laGogglesMask			= cgi_R_RegisterShader( "gfx/2d/amp_mask" );
			cgs.media.laGogglesSideBit		= cgi_R_RegisterShader( "gfx/2d/side_bit" );
			cgs.media.laGogglesBracket		= cgi_R_RegisterShader( "gfx/2d/bracket" );
			cgs.media.laGogglesArrow		= cgi_R_RegisterShader( "gfx/2d/bracket2" );
			break;

		case INV_BACTA_CANISTER:
			for ( int i = 1; i < 5; i++ )
			{
				cgi_S_RegisterSound( va( "sound/weapons/force/heal%d_m.mp3", i ) );
				cgi_S_RegisterSound( va( "sound/weapons/force/heal%d_f.mp3", i ) );
			}
			break;
		}
	}
}


/*
========================================================================================

VIEW WEAPON

========================================================================================
*/

/*
=================
CG_MapTorsoToWeaponFrame

animations MUST match the defined pattern!
the weapon hand animation has 3 anims, 
	6 frames of attack
	4 frames of drop
	5 frames of raise

  if the torso anim does not match these lengths, it will not animate correctly!
=================
*/
extern qboolean ValidAnimFileIndex ( int index );
int CG_MapTorsoToWeaponFrame( const clientInfo_t *ci, int frame, int animNum, int weaponNum, int firing ) 
{
	// we should use the animNum to map a weapon frame instead of relying on the torso frame
	if ( !ValidAnimFileIndex( ci->animFileIndex ) )
	{
		return 0;
	}
	animation_t *animations = level.knownAnimFileSets[ci->animFileIndex].animations;
	int ret=0;

	switch( animNum )
	{
	case TORSO_WEAPONREADY1:
	case TORSO_WEAPONREADY2:
	case TORSO_WEAPONREADY3:
	case TORSO_WEAPONREADY4:
	case TORSO_WEAPONREADY10:
		ret = 0;
		break;

	case TORSO_DROPWEAP1:
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 5 ) 
		{
			ret = frame - animations[animNum].firstFrame + 6;
		}
		else
		{
//			assert(0);
		}
		break;

	case TORSO_RAISEWEAP1:
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 4 ) 
		{
			ret = frame - animations[animNum].firstFrame + 6 + 5;
		}
		else
		{
//			assert(0);
		}
		break;

	case BOTH_ATTACK1:
	case BOTH_ATTACK2:
	case BOTH_ATTACK3:
	case BOTH_ATTACK4:
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 6 ) 
		{
			ret = 1 + ( frame - animations[animNum].firstFrame );
		}
		else
		{
//			assert(0);
		}
		break;
	default:
		break;
	}	

	return ret;
}

/*
==============
CG_CalculateWeaponPosition
==============
*/
void CG_CalculateWeaponPosition( vec3_t origin, vec3_t angles ) 
{
	float	scale;
	int		delta;
	float	fracsin;

	VectorCopy( cg.refdef.vieworg, origin );
	VectorCopy( cg.refdefViewAngles, angles );

	// on odd legs, invert some angles
	if ( cg.bobcycle & 1 ) {
		scale = -cg.xyspeed;
	} else {
		scale = cg.xyspeed;
	}

	// gun angles from bobbing
	angles[ROLL] += scale * cg.bobfracsin * 0.0075;
	angles[YAW] += scale * cg.bobfracsin * 0.01;
	angles[PITCH] += cg.xyspeed * cg.bobfracsin * 0.0075;

	// drop the weapon when landing
	delta = cg.time - cg.landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		origin[2] += cg.landChange*0.25 * delta / LAND_DEFLECT_TIME;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		origin[2] += cg.landChange*0.25 * 
			(LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta) / LAND_RETURN_TIME;
	}

#if 0
	// drop the weapon when stair climbing
	delta = cg.time - cg.stepTime;
	if ( delta < STEP_TIME/2 ) {
		origin[2] -= cg.stepChange*0.25 * delta / (STEP_TIME/2);
	} else if ( delta < STEP_TIME ) {
		origin[2] -= cg.stepChange*0.25 * (STEP_TIME - delta) / (STEP_TIME/2);
	}
#endif

	// idle drift
	scale = /*cg.xyspeed + */40;
	fracsin = sin( cg.time * 0.001 );
	angles[ROLL] += scale * fracsin * 0.01;
	angles[YAW] += scale * fracsin * 0.01;
	angles[PITCH] += (scale * 0.5f ) * fracsin * 0.01;
}

/*
======================
CG_MachinegunSpinAngle
======================
*/
/*
#define		SPIN_SPEED	0.9
#define		COAST_TIME	1000
static float	CG_MachinegunSpinAngle( centity_t *cent ) {
	int		delta;
	float	angle;
	float	speed;

	delta = cg.time - cent->pe.barrelTime;
	if ( cent->pe.barrelSpinning ) {
		angle = cent->pe.barrelAngle + delta * SPIN_SPEED;
	} else {
		if ( delta > COAST_TIME ) {
			delta = COAST_TIME;
		}

		speed = 0.5 * ( SPIN_SPEED + (float)( COAST_TIME - delta ) / COAST_TIME );
		angle = cent->pe.barrelAngle + delta * speed;
	}

	if ( cent->pe.barrelSpinning == !(cent->currentState.eFlags & EF_FIRING) ) {
		cent->pe.barrelTime = cg.time;
		cent->pe.barrelAngle = AngleNormalize360( angle );
		cent->pe.barrelSpinning = !!(cent->currentState.eFlags & EF_FIRING);
	}

	return angle;
}
*/
/*
Ghoul2 Insert Start
*/
// set up the appropriate ghoul2 info to a refent
void CG_SetGhoul2InfoRef( refEntity_t *ent, refEntity_t	*s1)
{
	ent->ghoul2 = s1->ghoul2;
	VectorCopy( s1->modelScale, ent->modelScale);
	ent->radius = s1->radius;
	VectorCopy( s1->angles, ent->angles);
}


//--------------------------------------------------------------------------
static void CG_DoMuzzleFlash( centity_t *cent, vec3_t org, vec3_t dir, weaponData_t *wData )
{
	// Handle muzzle flashes, really this could just be a qboolean instead of a time.......
	if ( cent->muzzleFlashTime > 0 )
	{
		cent->muzzleFlashTime  = 0;
		const char *effect = NULL;

//		CG_PositionEntityOnTag( &flash, &gun, gun.hModel, "tag_flash");

		// Try and get a default muzzle so we have one to fall back on
		if ( wData->mMuzzleEffect[0] )
		{
			effect = &wData->mMuzzleEffect[0];
		}
		
		if ( cent->altFire )
		{
			// We're alt-firing, so see if we need to override with a custom alt-fire effect
			if ( wData->mAltMuzzleEffect[0] )
			{
				effect = &wData->mAltMuzzleEffect[0];
			}
		}

		if (/*( cent->currentState.eFlags & EF_FIRING || cent->currentState.eFlags & EF_ALT_FIRING ) &&*/ effect )
		{
			if (( cent->gent && cent->gent->NPC ) || cg.renderingThirdPerson )
			{
				theFxScheduler.PlayEffect( effect, org, dir );
			}
			else
			{
				// We got an effect and we're firing, so let 'er rip.
				theFxScheduler.PlayEffect( effect, cent->currentState.clientNum );
			}
		}
	}
	else
	{
//		CG_PositionRotatedEntityOnTag( &flash, &gun, weapon->weaponModel, "tag_flash", NULL);
	}
}

/*
Ghoul2 Insert End
*/

/*
==============
CG_AddViewWeapon

Add the weapon, and flash for the player's view
==============
*/
extern int PM_TorsoAnimForFrame( gentity_t *ent, int torsoFrame );
extern float CG_ForceSpeedFOV( void );

void CG_AddViewWeapon( playerState_t *ps ) 
{
	refEntity_t	hand;
	refEntity_t	flash;
	vec3_t		angles;
	const weaponInfo_t	*weapon;
	weaponData_t  *wData;
	centity_t	*cent;
	float		fovOffset, leanOffset;

	// no gun if in third person view
	if ( cg.renderingThirdPerson )
		return;

	if ( ps->pm_type == PM_INTERMISSION )
		return;

	cent = &cg_entities[ps->clientNum];

	if ( ps->eFlags & EF_LOCKED_TO_WEAPON )
	{
		return;
	}

	/*if ( cent->gent && cent->gent->client && cent->gent->client->ps.forcePowersActive&(1<<FP_LIGHTNING) )
	{//doing the electrocuting
		vec3_t temp;//tAng, fxDir, 
		//VectorSet( tAng, cent->pe.torso.pitchAngle, cent->pe.torso.yawAngle, 0 );

		VectorCopy( cent->gent->client->renderInfo.handLPoint, temp );
		VectorMA( temp, -5, cg.refdef.viewaxis[0], temp );
		if ( cent->gent->client->ps.forcePowerLevel[FP_LIGHTNING] > FORCE_LEVEL_2 )
		{//arc
			if ( cent->gent->client->NPC_class == CLASS_REELO )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWebwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ALORA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_JEDI )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_LUKE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLifewide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_REBORN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_KYLE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_TRANDOSHAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHollyfirewide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_TUSKEN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecrowide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GLIDER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecrowide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_NOGHRI )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStunwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_FLIER2 )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStunwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_WEEQUAY )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceCrystalwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_LIZARD )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStonewide, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_SWAMPTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSkullwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_FISH )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSkullwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GRAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_CLAW )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHellwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MOUSE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGhostwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MINEMONSTER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forcePoisonwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_INTERROGATOR )
						{
							theFxScheduler.PlayEffect( cgs.effects.forcePoisonwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_WAMPA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHellwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_BOBAFETT )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHellwide, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_TAVION )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHyldenwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_DESANN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGreenLightningwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MORGANKATARN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MONMOTHA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_JAWA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGhostwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_PROBE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWindwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ROCKETTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWindwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_COMMANDO )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecrowide, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_MURJJ )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecrowide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SEEKER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_REMOTE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MARK1 )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MARK2 )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SABOTEUR )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceDarkwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SHADOWTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceDarkwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SABER_DROID )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceYellowLightningwide, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_PROTOCOL )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceBurstwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_HAZARD_TROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLavawide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GALAKMECH )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceIcewide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_BARTENDER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceIcewide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SWAMP )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWaterwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GONK )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStonewide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_HOWLER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSonicwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ASSASSIN_DROID )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ATST )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceBurstwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SAND_CREATURE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGreenLightningwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_PRISONER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceAcidwide, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_BESPIN_COP )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_REBEL )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_LANDO )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceAstralwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_RODIAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceFireballwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GALAK )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceFireballwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_JAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHollywide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_STORMTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHollywide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_IMPWORKER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceFirewide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_IMPERIAL )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHellwide, temp, cg.refdef.viewaxis );
						}
			else
			{
				theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
			}
			//theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
		}
		else
		{//line
					if ( cent->gent->client->NPC_class == CLASS_REELO )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWeb, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ALORA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_JEDI )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_LUKE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLife, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_REBORN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_KYLE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_TRANDOSHAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHollyfire, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_TUSKEN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecro, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GLIDER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecro, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_NOGHRI )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStun, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_FLIER2 )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStun, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_WEEQUAY )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceCrystal, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_LIZARD )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStone, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_SWAMPTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSkull, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_FISH )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSkull, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GRAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_CLAW )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHell, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MOUSE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGhost, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MINEMONSTER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forcePoison, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_INTERROGATOR )
						{
							theFxScheduler.PlayEffect( cgs.effects.forcePoison, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_WAMPA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHell, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_BOBAFETT )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHell, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_TAVION )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHylden, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_DESANN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGreenLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MORGANKATARN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MONMOTHA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_JAWA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGhost, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_PROBE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWind, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ROCKETTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWind, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_COMMANDO )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecro, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_MURJJ )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecro, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SEEKER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_REMOTE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MARK1 )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MARK2 )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SABOTEUR )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceDark, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SHADOWTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceDark, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SABER_DROID )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceYellowLightning, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_PROTOCOL )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceBurst, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_HAZARD_TROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLava, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GALAKMECH )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceIce, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_BARTENDER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceIce, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SWAMP )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceWater, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GONK )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceStone, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_HOWLER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSonic, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ASSASSIN_DROID )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_ATST )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceBurst, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_SAND_CREATURE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceGreenLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_PRISONER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceAcid, temp, cg.refdef.viewaxis );
						}
						else if ( cent->gent->client->NPC_class == CLASS_BESPIN_COP )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_REBEL )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_LANDO )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceAstral, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_RODIAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceFireball, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_GALAK )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceFireball, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_JAN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHolly, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_STORMTROOPER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHolly, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_IMPWORKER )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceFire, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_IMPERIAL )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHell, temp, cg.refdef.viewaxis );
						}
			else
			{
				theFxScheduler.PlayEffect( cgs.effects.forceLightningWide, temp, cg.refdef.viewaxis );
			}
			//theFxScheduler.PlayEffect( cgs.effects.forceLightning, temp, cg.refdef.viewaxis[0] );
		}
	}*/

/*	if ( cent->gent && cent->gent->client && cent->gent->client->ps.forcePowersActive&(1<<FP_DRAIN) )
	{//doing the draining
		vec3_t temp;//tAng, fxDir, 
		//VectorSet( tAng, cent->pe.torso.pitchAngle, cent->pe.torso.yawAngle, 0 );

		VectorCopy( cent->gent->client->renderInfo.handLPoint, temp );
		VectorMA( temp, -5, cg.refdef.viewaxis[0], temp );
		if ( cent->gent->client->ps.forcePowerLevel[FP_DRAIN] > FORCE_LEVEL_2 )
		{//arc
			//vec3_t	fxAxis[3];
			//AnglesToAxis( tAng, fxAxis );
			/*theFxScheduler.PlayEffect( cgs.effects.forceDrainWide, temp, cg.refdef.viewaxis );*/
					/*if ( cent->gent->client->NPC_class == CLASS_REELO || cent->gent->client->NPC_class == CLASS_JEDI ||
						 cent->gent->client->NPC_class == CLASS_RANCOR || cent->gent->client->NPC_class == CLASS_LUKE ||
						 cent->gent->client->NPC_class == CLASS_MOUSE || cent->gent->client->NPC_class == CLASS_JAWA ||
						 cent->gent->client->NPC_class == CLASS_ATST || cent->gent->client->NPC_class == CLASS_SAND_CREATURE ||
						 cent->gent->client->NPC_class == CLASS_ROCKETTROOPER || cent->gent->client->NPC_class == CLASS_ASSASSIN_DROID || 			
						 cent->gent->client->NPC_class == CLASS_PROBE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSouldrainwide, temp, cg.refdef.viewaxis );
						} 
					else if ( cent->gent->client->NPC_class == CLASS_KYLE || cent->gent->client->NPC_class == CLASS_REBORN ||
						 cent->gent->client->NPC_class == CLASS_TUSKEN || cent->gent->client->NPC_class == CLASS_GLIDER ||
						 cent->gent->client->NPC_class == CLASS_NOGHRI || cent->gent->client->NPC_class == CLASS_FLIER2 ||
						 cent->gent->client->NPC_class == CLASS_WEEQUAY || cent->gent->client->NPC_class == CLASS_LIZARD ||
						 cent->gent->client->NPC_class == CLASS_SWAMPTROOPER || cent->gent->client->NPC_class == CLASS_FISH ||
						 cent->gent->client->NPC_class == CLASS_NOGHRI || cent->gent->client->NPC_class == CLASS_FLIER2 ||
						 cent->gent->client->NPC_class == CLASS_GRAN || cent->gent->client->NPC_class == CLASS_CLAW ||
						 cent->gent->client->NPC_class == CLASS_MONMOTHA || cent->gent->client->NPC_class == CLASS_ALORA ||
						 cent->gent->client->NPC_class == CLASS_SWAMP || cent->gent->client->NPC_class == CLASS_INTERROGATOR )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceDrainWide, temp, cg.refdef.viewaxis );
					}
					else if ( cent->gent->client->NPC_class == CLASS_MURJJ || cent->gent->client->NPC_class == CLASS_COMMANDO ||
						 cent->gent->client->NPC_class == CLASS_PRISONER || cent->gent->client->NPC_class == CLASS_REBEL ||
						 cent->gent->client->NPC_class == CLASS_BESPIN_COP || cent->gent->client->NPC_class == CLASS_SABOTEUR ||
						 cent->gent->client->NPC_class == CLASS_SHADOWTROOPER || cent->gent->client->NPC_class == CLASS_STORMTROOPER ||
						 cent->gent->client->NPC_class == CLASS_SWAMPTROOPER || cent->gent->client->NPC_class == CLASS_FISH ||
						 cent->gent->client->NPC_class == CLASS_TRANDOSHAN || cent->gent->client->NPC_class == CLASS_SWAMP ||
						 cent->gent->client->NPC_class == CLASS_IMPERIAL || cent->gent->client->NPC_class == CLASS_IMPWORKER ||
						 cent->gent->client->NPC_class == CLASS_JAN || cent->gent->client->NPC_class == CLASS_LANDO  ||
						 cent->gent->client->NPC_class == CLASS_GALAK || cent->gent->client->NPC_class == CLASS_RODIAN ||
						 cent->gent->client->NPC_class == CLASS_SABER_DROID || cent->gent->client->NPC_class == CLASS_PROTOCOL ||
						 cent->gent->client->NPC_class == CLASS_HAZARD_TROOPER || cent->gent->client->NPC_class == CLASS_GALAKMECH ||
						 cent->gent->client->NPC_class == CLASS_BARTENDER || cent->gent->client->NPC_class == CLASS_GONK ||
						 cent->gent->client->NPC_class == CLASS_R2D2 || cent->gent->client->NPC_class == CLASS_R5D2 ||
						 cent->gent->client->NPC_class == CLASS_MARK1 || cent->gent->client->NPC_class == CLASS_MARK2 || 
						 cent->gent->client->NPC_class == CLASS_HOWLER )  
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecrodrainwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MORGANKATARN || cent->gent->client->NPC_class == CLASS_MINEMONSTER ||
						 cent->gent->client->NPC_class == CLASS_BOBAFETT || cent->gent->client->NPC_class == CLASS_WAMPA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHelldrainwide, temp, cg.refdef.viewaxis );
						}
					else if ( cent->gent->client->NPC_class == CLASS_TAVION || cent->gent->client->NPC_class == CLASS_DESANN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHyldendrainwide, temp, cg.refdef.viewaxis );
						}
		}
		else
		{//line
			//AngleVectors( tAng, fxDir, NULL, NULL );
			/*theFxScheduler.PlayEffect( cgs.effects.forceDrain, temp, cg.refdef.viewaxis[0] );*/
			/*if ( cent->gent->client->NPC_class == CLASS_REELO || cent->gent->client->NPC_class == CLASS_JEDI ||
						 cent->gent->client->NPC_class == CLASS_RANCOR || cent->gent->client->NPC_class == CLASS_LUKE ||
						 cent->gent->client->NPC_class == CLASS_MOUSE || cent->gent->client->NPC_class == CLASS_JAWA ||
						 cent->gent->client->NPC_class == CLASS_ATST || cent->gent->client->NPC_class == CLASS_SAND_CREATURE ||
						 cent->gent->client->NPC_class == CLASS_ROCKETTROOPER || cent->gent->client->NPC_class == CLASS_ASSASSIN_DROID || 			
						 cent->gent->client->NPC_class == CLASS_PROBE )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceSouldrain, temp, cg.refdef.viewaxis[0] );
						} 
					else if ( cent->gent->client->NPC_class == CLASS_KYLE || cent->gent->client->NPC_class == CLASS_REBORN ||
						 cent->gent->client->NPC_class == CLASS_TUSKEN || cent->gent->client->NPC_class == CLASS_GLIDER ||
						 cent->gent->client->NPC_class == CLASS_NOGHRI || cent->gent->client->NPC_class == CLASS_FLIER2 ||
						 cent->gent->client->NPC_class == CLASS_WEEQUAY || cent->gent->client->NPC_class == CLASS_LIZARD ||
						 cent->gent->client->NPC_class == CLASS_SWAMPTROOPER || cent->gent->client->NPC_class == CLASS_FISH ||
						 cent->gent->client->NPC_class == CLASS_NOGHRI || cent->gent->client->NPC_class == CLASS_FLIER2 ||
						 cent->gent->client->NPC_class == CLASS_GRAN || cent->gent->client->NPC_class == CLASS_CLAW ||
						 cent->gent->client->NPC_class == CLASS_MONMOTHA || cent->gent->client->NPC_class == CLASS_ALORA ||
						 cent->gent->client->NPC_class == CLASS_SWAMP || cent->gent->client->NPC_class == CLASS_INTERROGATOR )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceDrain, temp, cg.refdef.viewaxis[0] );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MURJJ || cent->gent->client->NPC_class == CLASS_COMMANDO ||
						 cent->gent->client->NPC_class == CLASS_PRISONER || cent->gent->client->NPC_class == CLASS_REBEL ||
						 cent->gent->client->NPC_class == CLASS_BESPIN_COP || cent->gent->client->NPC_class == CLASS_SABOTEUR ||
						 cent->gent->client->NPC_class == CLASS_SHADOWTROOPER || cent->gent->client->NPC_class == CLASS_STORMTROOPER ||
						 cent->gent->client->NPC_class == CLASS_SWAMPTROOPER || cent->gent->client->NPC_class == CLASS_FISH ||
						 cent->gent->client->NPC_class == CLASS_TRANDOSHAN || cent->gent->client->NPC_class == CLASS_SWAMP ||
						 cent->gent->client->NPC_class == CLASS_IMPERIAL || cent->gent->client->NPC_class == CLASS_IMPWORKER ||
						 cent->gent->client->NPC_class == CLASS_JAN || cent->gent->client->NPC_class == CLASS_LANDO  ||
						 cent->gent->client->NPC_class == CLASS_GALAK || cent->gent->client->NPC_class == CLASS_RODIAN ||
						 cent->gent->client->NPC_class == CLASS_SABER_DROID || cent->gent->client->NPC_class == CLASS_PROTOCOL ||
						 cent->gent->client->NPC_class == CLASS_HAZARD_TROOPER || cent->gent->client->NPC_class == CLASS_GALAKMECH ||
						 cent->gent->client->NPC_class == CLASS_BARTENDER || cent->gent->client->NPC_class == CLASS_GONK ||
						 cent->gent->client->NPC_class == CLASS_R2D2 || cent->gent->client->NPC_class == CLASS_R5D2 ||
						 cent->gent->client->NPC_class == CLASS_MARK1 || cent->gent->client->NPC_class == CLASS_MARK2 ||
						 cent->gent->client->NPC_class == CLASS_HOWLER )  				 
						{
							theFxScheduler.PlayEffect( cgs.effects.forceNecrodrain, temp, cg.refdef.viewaxis[0] );
						}
					else if ( cent->gent->client->NPC_class == CLASS_MORGANKATARN || cent->gent->client->NPC_class == CLASS_MINEMONSTER ||
						 cent->gent->client->NPC_class == CLASS_BOBAFETT || cent->gent->client->NPC_class == CLASS_WAMPA )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHelldrain, temp, cg.refdef.viewaxis[0] );
						}
					else if ( cent->gent->client->NPC_class == CLASS_TAVION || cent->gent->client->NPC_class == CLASS_DESANN )
						{
							theFxScheduler.PlayEffect( cgs.effects.forceHyldendrain, temp, cg.refdef.viewaxis[0] );
						}
		}
	}*/

	// allow the gun to be completely removed
	if ( !cg_drawGun.integer || cg.zoomMode ) 
	{
		vec3_t		origin;

		// special hack for lightning guns...
		VectorCopy( cg.refdef.vieworg, origin );
		VectorMA( origin, -10, cg.refdef.viewaxis[2], origin );
		VectorMA( origin, 16, cg.refdef.viewaxis[0], origin );
// Doesn't look like we'll have lightning style guns.  Clean this crap up when we are sure about this.
//		CG_LightningBolt( cent, origin );	

		// We should still do muzzle flashes though...
		CG_RegisterWeapon( ps->weapon );
		weapon = &cg_weapons[ps->weapon];
		wData =  &weaponData[ps->weapon];

		CG_DoMuzzleFlash( cent, origin, cg.refdef.viewaxis[0], wData );

		// If we don't update this, the muzzle flash point won't even be updated properly
		VectorCopy( origin, cent->gent->client->renderInfo.muzzlePoint );
		VectorCopy( cg.refdef.viewaxis[0], cent->gent->client->renderInfo.muzzleDir );

		cent->gent->client->renderInfo.mPCalcTime = cg.time;
		return;
	}

	// drop gun lower at higher fov
	float actualFOV;
	if ( (cg.snap->ps.forcePowersActive&(1<<FP_SPEED)) && player->client->ps.forcePowerDuration[FP_SPEED] )//cg.renderingThirdPerson && 
	{
		actualFOV = CG_ForceSpeedFOV();
	}
	else
	{
		actualFOV = (cg.overrides.active&CG_OVERRIDE_FOV) ? cg.overrides.fov : cg_fov.value;
	}

	if ( actualFOV > 80 ) 
	{
		fovOffset = -0.1 * ( actualFOV - 80 );
	} 
	else 
	{
		fovOffset = 0;
	}

	if ( ps->leanofs != 0 )
	{	//add leaning offset
		leanOffset = ps->leanofs * 0.25f;
		fovOffset += abs(ps->leanofs) * -0.1f;
	}
	else
	{
		leanOffset = 0;
	}

	CG_RegisterWeapon( ps->weapon );
	weapon = &cg_weapons[ps->weapon];
	wData =  &weaponData[ps->weapon];

	memset (&hand, 0, sizeof(hand));

	if ( ps->weapon == WP_STUN_BATON || ps->weapon == WP_CONCUSSION )
	{
		cgi_S_AddLoopingSound( cent->currentState.number, 
			cent->lerpOrigin, 
			vec3_origin, 
			weapon->firingSound );
	}

	// set up gun position
	CG_CalculateWeaponPosition( hand.origin, angles );

	vec3_t extraOffset;
	extraOffset[0] = extraOffset[1] = extraOffset[2] = 0.0f;

	if( ps->weapon == WP_TUSKEN_RIFLE || ps->weapon == WP_NOGHRI_STICK || ps->weapon == WP_TUSKEN_STAFF )
	{
		extraOffset[0] = 2;
		extraOffset[1] = -3;
		extraOffset[2] = -6;
	}

	VectorMA( hand.origin, cg_gun_x.value+extraOffset[0], cg.refdef.viewaxis[0], hand.origin );
	VectorMA( hand.origin, (cg_gun_y.value+leanOffset+extraOffset[1]), cg.refdef.viewaxis[1], hand.origin );
	VectorMA( hand.origin, (cg_gun_z.value+fovOffset+extraOffset[2]), cg.refdef.viewaxis[2], hand.origin );
	//VectorMA( hand.origin, 0, cg.refdef.viewaxis[0], hand.origin );
	//VectorMA( hand.origin, (0+leanOffset), cg.refdef.viewaxis[1], hand.origin );
	//VectorMA( hand.origin, (0+fovOffset), cg.refdef.viewaxis[2], hand.origin );

	AnglesToAxis( angles, hand.axis );

	// map torso animations to weapon animations
#ifndef FINAL_BUILD
	if ( cg_gun_frame.integer ) 
	{
		// development tool
		hand.frame = hand.oldframe = cg_gun_frame.integer;
		hand.backlerp = 0;
	} 
	else 
#endif
	{
		// get clientinfo for animation map
		const clientInfo_t	*ci = &cent->gent->client->clientInfo;
		int torsoAnim = cent->gent->client->ps.torsoAnim;//pe.torso.animationNumber;
		float currentFrame;
		int startFrame,endFrame,flags;
		float animSpeed;
		if (cent->gent->lowerLumbarBone>=0&& gi.G2API_GetBoneAnimIndex(&cent->gent->ghoul2[cent->gent->playerModel], cent->gent->lowerLumbarBone, cg.time, &currentFrame, &startFrame, &endFrame, &flags, &animSpeed,0) )
		{
			hand.oldframe = CG_MapTorsoToWeaponFrame( ci,floor(currentFrame), torsoAnim, cent->currentState.weapon, ( cent->currentState.eFlags & EF_FIRING ) );
			hand.frame = CG_MapTorsoToWeaponFrame( ci,ceil(currentFrame), torsoAnim, cent->currentState.weapon, ( cent->currentState.eFlags & EF_FIRING ) );
			hand.backlerp=1.0f-(currentFrame-floor(currentFrame));
			if ( cg_debugAnim.integer == 1 && cent->currentState.clientNum == 0 )
			{
				Com_Printf( "Torso frame %d to %d makes Weapon frame %d to %d\n", cent->pe.torso.oldFrame,  cent->pe.torso.frame, hand.oldframe, hand.frame );
			}
		}
		else
		{
//			assert(0); // no idea what to do here
			hand.oldframe=0;
			hand.frame=0;
			hand.backlerp=0.0f;
		}
	}

	// add the weapon(s) - FIXME: allow for 2 weapons generically, not just 2 sabers?
	int	numSabers = 1;
	if ( cent->gent->client->ps.dualSabers )
	{
		numSabers = 2;
	}
	for ( int saberNum = 0; saberNum < numSabers; saberNum++ )
	{
		refEntity_t	gun;
		memset (&gun, 0, sizeof(gun));

		gun.hModel = weapon->weaponModel;
		if (!gun.hModel) 
		{
			return;
		}

		AnglesToAxis( angles, gun.axis );
		CG_PositionEntityOnTag( &gun, &hand, weapon->handsModel, "tag_weapon");

		gun.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON;

	//---------
		// OK, we are making an assumption here that if we have the phaser that it is always on....
		//FIXME: if saberInFlight, need to draw empty hand guiding it
		if ( cent->gent && cent->gent->client && cent->currentState.weapon == WP_SABER )
		{
			vec3_t org_, axis_[3];

			for ( int bladeNum = 0; bladeNum < cent->gent->client->ps.saber[saberNum].numBlades; bladeNum++ )
			{
				//FIXME: need to get from tag_flash2 for saberstaff's second blade?
				CG_GetTagWorldPosition( &gun, "tag_flash", org_, axis_ );
				//loop this and do for both blades
				if ( cent->gent->client->ps.saber[0].blade[0].active && cent->gent->client->ps.saber[0].blade[0].length < cent->gent->client->ps.saber[0].blade[0].lengthMax )
				{
					cent->gent->client->ps.saber[0].blade[0].length += cg.frametime*0.03;
					if ( cent->gent->client->ps.saber[0].blade[0].length > cent->gent->client->ps.saber[0].blade[0].lengthMax )
					{
						cent->gent->client->ps.saber[0].blade[0].length = cent->gent->client->ps.saber[0].blade[0].lengthMax;
					}
				}
		//		FX_Saber( org_, axis_[0], cent->gent->client->ps.saberLength, 2.0 + crandom() * 0.2f, cent->gent->client->ps.saberColor );
				if ( saberNum == 0 && bladeNum == 0 )
				{
					VectorCopy( axis_[0], cent->gent->client->renderInfo.muzzleDir );
				}
				else
				{//need these points stored here when in 1st person saber
					VectorCopy(org_, cent->gent->client->ps.saber[saberNum].blade[bladeNum].muzzlePoint);
				}
				VectorCopy( axis_[0], cent->gent->client->ps.saber[saberNum].blade[bladeNum].muzzleDir );
			}
		}
		//---------

		//	CG_AddRefEntityWithPowerups( &gun, cent->currentState.powerups, cent->gent );
			cgi_R_AddRefEntityToScene( &gun );

	/*	if ( ps->weapon == WP_STUN_BATON )
		{
			gun.shaderRGBA[0] = gun.shaderRGBA[1] = gun.shaderRGBA[2] = 25;

			gun.customShader = cgi_R_RegisterShader( "gfx/effects/stunPass" );
			gun.renderfx = RF_RGB_TINT | RF_FIRST_PERSON | RF_DEPTHHACK;
			cgi_R_AddRefEntityToScene( &gun );
		}
	*/
		// add the spinning barrel[s]
		for (int i = 0; (i < wData->numBarrels); i++)	
		{
			refEntity_t	barrel;
			memset( &barrel, 0, sizeof( barrel ) );
			barrel.hModel = weapon->barrelModel[i];

			//VectorCopy( parent->lightingOrigin, barrel.lightingOrigin );
			//barrel.shadowPlane = parent->shadowPlane;
			barrel.renderfx = gun.renderfx;
			angles[YAW] = 0;
			angles[PITCH] = 0;
	//		if ( ps->weapon == WP_TETRION_DISRUPTOR) {
	//			angles[ROLL] = CG_MachinegunSpinAngle( cent );
	//		} else {
				angles[ROLL] = 0;//CG_MachinegunSpinAngle( cent );
	//		}

			AnglesToAxis( angles, barrel.axis );
			if (!i) 
			{
				CG_PositionRotatedEntityOnTag( &barrel, &hand, weapon->handsModel, "tag_barrel", NULL );
			} else 
			{
				CG_PositionRotatedEntityOnTag( &barrel, &hand, weapon->handsModel, va("tag_barrel%d",i+1), NULL );
			}

			cgi_R_AddRefEntityToScene( &barrel );
		}

		memset (&flash, 0, sizeof(flash));

		// Seems like we should always do this in case we have an animating muzzle flash....that way we can always store the correct muzzle dir, etc.
		CG_PositionEntityOnTag( &flash, &gun, gun.hModel, "tag_flash");

		CG_DoMuzzleFlash( cent, flash.origin, flash.axis[0], wData );

		if ( cent->gent && cent->gent->client )
		{
			if ( saberNum == 0 )
			{
				VectorCopy(flash.origin, cent->gent->client->renderInfo.muzzlePoint);
				VectorCopy(flash.axis[0], cent->gent->client->renderInfo.muzzleDir);
			}
	//		VectorNormalize( cent->gent->client->renderInfo.muzzleDir );
			cent->gent->client->renderInfo.mPCalcTime = cg.time;

			//CG_LightningBolt( cent, flash.origin ); 
		}
	}

	// Do special charge bits
	//-----------------------
	if (( ps->weaponstate == WEAPON_CHARGING_ALT && ps->weapon == WP_BRYAR_PISTOL ) 
			|| ( ps->weaponstate == WEAPON_CHARGING_ALT && ps->weapon == WP_BLASTER_PISTOL )
			|| ( ps->weapon == WP_BOWCASTER && ps->weaponstate == WEAPON_CHARGING_ALT )
			|| ( ps->weapon == WP_DEMP2 && ps->weaponstate == WEAPON_CHARGING_ALT )
			|| ( ps->weapon == WP_POISON && ps->weaponstate == WEAPON_CHARGING_ALT )
			)
	{
		int		shader = 0;
		float	val = 0.0f, scale = 1.0f;
		vec3_t	WHITE	= {1.0f,1.0f,1.0f};

		if ( ps->weapon == WP_BRYAR_PISTOL )
		{
			// Hardcoded max charge time of 1 second
			val = ( cg.time - ps->weaponChargeTime ) * 0.001f;
			shader = cgi_R_RegisterShader( "gfx/effects/bryarFrontFlash" );
		}
		if ( ps->weapon == WP_BLASTER_PISTOL )
		{
			// Hardcoded max charge time of 1 second
			val = ( cg.time - ps->weaponChargeTime ) * 0.001f;
			shader = cgi_R_RegisterShader( "gfx/damage/burnmark3" );
		}
		else if ( ps->weapon == WP_BOWCASTER )
		{
			// Hardcoded max charge time of 1 second
			val = ( cg.time - ps->weaponChargeTime ) * 0.001f;
			shader = cgi_R_RegisterShader( "gfx/effects/bryarFrontFlash" );
		}
		else if ( ps->weapon == WP_DEMP2 )
		{
			// Hardcoded max charge time of 1 second
			val = ( cg.time - ps->weaponChargeTime ) * 0.001f;
			shader = cgi_R_RegisterShader( "gfx/misc/lightningFlash" );
			scale = 1.75f;
		}
		else if ( ps->weapon == WP_POISON )
		{
			// Hardcoded max charge time of 1 second
			val = ( cg.time - ps->weaponChargeTime ) * 0.001f;
			shader = cgi_R_RegisterShader( "gfx/effects/greenFrontFlash" );
			scale = 1.5f;
		}
		if ( val < 0.0f )
		{
			val = 0.0f;
		}
		else if ( val > 1.0f )
		{
			val = 1.0f;
			CGCam_Shake( 0.1f, 100 );
		}
		else
		{
			CGCam_Shake( val * val * 0.3f, 100 );
		}

		val += random() * 0.5f;

		FX_AddSprite( flash.origin, NULL, NULL, 3.0f * val * scale, 0.0f, 0.7f, 0.7f, WHITE, WHITE, random() * 360, 0.0f, 1.0f, shader, FX_USE_ALPHA | FX_DEPTH_HACK );
	}

	// Check if the heavy repeater is finishing up a sustained burst
	//-------------------------------
	if ( ps->weapon == WP_REPEATER && ps->weaponstate == WEAPON_FIRING )
	{
		if ( cent->gent && cent->gent->client && cent->gent->client->ps.weaponstate != WEAPON_FIRING )
		{
			int	ct = 0;

			// the more continuous shots we've got, the more smoke we spawn
			if ( cent->gent->client->ps.weaponShotCount > 60 ) {
				ct = 5;
			}
			else if ( cent->gent->client->ps.weaponShotCount > 35 ) {
				ct = 3;
			}
			else if ( cent->gent->client->ps.weaponShotCount > 15 ) {
				ct = 1;
			}

			for ( int i = 0; i < ct; i++ )
			{
				theFxScheduler.PlayEffect( "repeater/muzzle_smoke", cent->currentState.clientNum );
			}

			cent->gent->client->ps.weaponShotCount = 0;
		}
	}
}

/*
==============================================================================

WEAPON SELECTION

==============================================================================
*/

/*
===================
CG_WeaponCheck
===================
*/
int CG_WeaponCheck( int weaponIndex ) 
{
	int				value;
	
	if ( weaponIndex == WP_SABER)
	{
		return qtrue;
	}


	value = weaponData[weaponIndex].energyPerShot < weaponData[weaponIndex].altEnergyPerShot 
							? weaponData[weaponIndex].energyPerShot 
							: weaponData[weaponIndex].altEnergyPerShot;

	if( !cg.snap )
	{
		return qfalse;
	}
	
	// check how much energy(ammo) it takes to fire this weapon against how much ammo we have
	if ( value > cg.snap->ps.ammo[weaponData[weaponIndex].ammoIndex] )
	{
		value = qfalse;
	}
	else
	{
		value = qtrue;
	}

	return value;
}

int cgi_UI_GetItemText(char *menuFile,char *itemName, char *text);

//const char *weaponDesc[13] = 
const char *weaponDesc[WP_NUM_WEAPONS - 1] =
{
"SABER_DESC",
"NEW_BLASTER_PISTOL_DESC",
"BLASTER_RIFLE_DESC",
"DISRUPTOR_RIFLE_DESC",
"BOWCASTER_DESC",
"HEAVYREPEATER_DESC",
"DEMP2_DESC",
"FLECHETTE_DESC",
"MERR_SONN_DESC",
"THERMAL_DETONATOR_DESC",
"TRIP_MINE_DESC",
"DET_PACK_DESC",
"CONCUSSION_DESC",
"MELEE_DESC",
"ATST_MAIN_DESC",
"ATST_SIDE_DESC",
"STUN_BATON_DESC",
"BLASTER_PISTOL_DESC",
"EMPLACED_GUN_DESC",
"BOT_LASER_DESC",
"TURRET_DESC",
"TIE_FIGHTER_DESC",
"RAPID_CONCUSSION_DESC",
"JAWA_DESC",
"TUSKEN_RIFLE_DESC",
"TUSKEN_STAFF_DESC",
"SCEPTER_DESC",
"NOGHRI_STICK_DESC",
};

/*
===================
CG_DrawDataPadWeaponSelect

Allows user to cycle through the various weapons currently owned and view the description
===================
*/
void CG_DrawDataPadWeaponSelect( void ) 
{
	int				i;
	int				weaponCount,weaponSelectI;
	int				holdX;
	int				sideLeftIconCnt,sideRightIconCnt;
	int				holdCount,iconCnt;
	char			text[1024]={0};
	qboolean drewConc = qfalse;

	// showing weapon select clears pickup item display, but not the blend blob
	cg.itemPickupTime = 0;

	//weaponBitFlag = cg.snap->ps.stats[ STAT_WEAPONS ]; fuck 

	// count the number of weapons owned
	weaponCount = 0;
	//for ( i = 1 ; i < 16 ; i++ ) 
	for ( i = 1 ; i < WP_NUM_WEAPONS ; i++ )
	{
		//if ( weaponBitFlag & ( 1 << i ) ) 
		if ( cg.snap->ps.weapons[i] )
		{
			weaponCount++;
		}
	}

	if (weaponCount == 0)	// If no weapons, don't display
	{
		return;
	}

	const short sideMax = 3;	// Max number of icons on the side

	// Calculate how many icons will appear to either side of the center one
	holdCount = weaponCount - 1;	// -1 for the center icon
	if (holdCount == 0)			// No icons to either side
	{
		sideLeftIconCnt = 0;
		sideRightIconCnt = 0;
	}
	else if (weaponCount > (2*sideMax))	// Go to the max on each side
	{
		sideLeftIconCnt = sideMax;
		sideRightIconCnt = sideMax;
	}
	else							// Less than max, so do the calc
	{
		sideLeftIconCnt = holdCount/2;
		sideRightIconCnt = holdCount - sideLeftIconCnt;
	}

	// This seems to be a problem if datapad comes up too early
	if (cg.DataPadWeaponSelect<FIRST_WEAPON)
	{
		cg.DataPadWeaponSelect = FIRST_WEAPON;
	}
	//else if (cg.DataPadWeaponSelect>13)
	else if (cg.DataPadWeaponSelect>=WP_NUM_WEAPONS)
	{
		//cg.DataPadWeaponSelect = 13;
		cg.DataPadWeaponSelect = WP_NUM_WEAPONS - 1;
	}

	// What weapon does the player currently have selected
	if ( cg.DataPadWeaponSelect == WP_CONCUSSION )
	{
		weaponSelectI = WP_FLECHETTE;
	}
	else
	{
		weaponSelectI = cg.DataPadWeaponSelect - 1;
	}
	if (weaponSelectI<1)
	{
	//	weaponSelectI = 13;
	//}	
		weaponSelectI = WP_NUM_WEAPONS - 1;
	}	




	const int smallIconSize = 40;
	const int bigIconSize = 80;
	const int bigPad = 64;
	const int pad = 32;

	const int centerXPos = 320;
	const int graphicYPos = 340;


	// Left side ICONS
	// Work backwards from current icon
	holdX = centerXPos - ((bigIconSize/2) + bigPad + smallIconSize);

	cgi_R_SetColor( colorTable[CT_WHITE] );
	for (iconCnt=1;iconCnt<(sideLeftIconCnt+1);weaponSelectI--)
	{
		if ( weaponSelectI == WP_CONCUSSION )
		{
			weaponSelectI--;
		}
		else if ( weaponSelectI == WP_FLECHETTE && !drewConc && cg.DataPadWeaponSelect != WP_CONCUSSION )
		{
			weaponSelectI = WP_CONCUSSION;
		}

		if (weaponSelectI<1)
		{
			//weaponSelectI = 13;
			weaponSelectI = WP_NUM_WEAPONS - 1;
		}

		//if ( !(weaponBitFlag & ( 1 << weaponSelectI )))	// Does he have this weapon?
		if ( !(cg.snap->ps.weapons[weaponSelectI]))	// Does he have this weapon?
		{
			if ( weaponSelectI == WP_CONCUSSION )
			{
				drewConc = qtrue;
				weaponSelectI = WP_ROCKET_LAUNCHER;
			}
			continue;
		}

		++iconCnt;					// Good icon

		if (weaponData[weaponSelectI].weaponIcon[0])
		{
			weaponInfo_t	*weaponInfo;
			CG_RegisterWeapon( weaponSelectI );	
			weaponInfo = &cg_weapons[weaponSelectI];

			if (!CG_WeaponCheck(weaponSelectI))
			{
				CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, weaponInfo->weaponIconNoAmmo );
			}
			else
			{
				CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, weaponInfo->weaponIcon );
			}

			holdX -= (smallIconSize+pad);
		}

		if ( weaponSelectI == WP_CONCUSSION )
		{
			drewConc = qtrue;
			weaponSelectI = WP_ROCKET_LAUNCHER;
		}
	}

	// Current Center Icon
	cgi_R_SetColor(colorTable[CT_WHITE]);

	if (weaponData[cg.DataPadWeaponSelect].weaponIcon[0])
	{
		weaponInfo_t	*weaponInfo;
		CG_RegisterWeapon( cg.DataPadWeaponSelect );	
		weaponInfo = &cg_weapons[cg.DataPadWeaponSelect];

			// Draw graphic to show weapon has ammo or no ammo
		if (!CG_WeaponCheck(cg.DataPadWeaponSelect))
		{
			CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2))+10, bigIconSize, bigIconSize, weaponInfo->weaponIconNoAmmo );
		}
		else
		{
			CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2))+10, bigIconSize, bigIconSize, weaponInfo->weaponIcon );
		}
	}

	if ( cg.DataPadWeaponSelect == WP_CONCUSSION )
	{
		weaponSelectI = WP_ROCKET_LAUNCHER;
	}
	else
	{
		weaponSelectI = cg.DataPadWeaponSelect + 1;
	}

	//if (weaponSelectI> 13)
	if (weaponSelectI>= WP_NUM_WEAPONS)
	{
		weaponSelectI = 1;
	}

	// Right side ICONS
	// Work forwards from current icon
	cgi_R_SetColor(colorTable[CT_WHITE]);
	holdX = centerXPos + (bigIconSize/2) + bigPad;
	for (iconCnt=1;iconCnt<(sideRightIconCnt+1);weaponSelectI++)
	{
		if ( weaponSelectI == WP_CONCUSSION )
		{
			weaponSelectI++;
		}
		else if ( weaponSelectI == WP_ROCKET_LAUNCHER && !drewConc && cg.DataPadWeaponSelect != WP_CONCUSSION )
		{
			weaponSelectI = WP_CONCUSSION;
		}
		//if (weaponSelectI>13)
		if (weaponSelectI>= WP_NUM_WEAPONS)
		{
			weaponSelectI = 1;
		}

		//if ( !(weaponBitFlag & ( 1 << weaponSelectI )))	// Does he have this weapon?
		if ( !(cg.snap->ps.weapons[weaponSelectI]))	// Does he have this weapon?
		{
			if ( weaponSelectI == WP_CONCUSSION )
			{
				drewConc = qtrue;
				weaponSelectI = WP_FLECHETTE;
			}
			continue;
		}

		++iconCnt;					// Good icon

		if (weaponData[weaponSelectI].weaponIcon[0])
		{
			weaponInfo_t	*weaponInfo;
			CG_RegisterWeapon( weaponSelectI );	
			weaponInfo = &cg_weapons[weaponSelectI];

			// Draw graphic to show weapon has ammo or no ammo
			if (!CG_WeaponCheck(i))
			{
				CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, weaponInfo->weaponIconNoAmmo );
			}
			else
			{
				CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, weaponInfo->weaponIcon );
			}


			holdX += (smallIconSize+pad);
		}
		if ( weaponSelectI == WP_CONCUSSION )
		{
			drewConc = qtrue;
			weaponSelectI = WP_FLECHETTE;
		}
	}

	// Print the weapon description
	//cgi_SP_GetStringTextString( va("SP_INGAME_%s",weaponDesc[cg.DataPadWeaponSelect-1]), text, sizeof(text) );
	if (!cgi_SP_GetStringTextString( va("SP_INGAME_%s",weaponDesc[cg.DataPadWeaponSelect-1]), text, sizeof(text) ))
	{
		cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",weaponDesc[cg.DataPadWeaponSelect-1]), text, sizeof(text) );
	}



	if (text[0])
	{
		const short textboxXPos = 40;
		const short textboxYPos = 60;
		const int	textboxWidth = 560;
		const int	textboxHeight = 300;
		const float	textScale = 1.0f;

		CG_DisplayBoxedText(
			textboxXPos, textboxYPos,
			textboxWidth, textboxHeight,
			text,
			4,
			textScale,
			colorTable[CT_WHITE]	
				);
	}

	cgi_R_SetColor( NULL );
}

/*
===================
CG_DrawDataPadIconBackground

Draw the proper background graphic for the icons being displayed on the datapad
===================
*/
void CG_DrawDataPadIconBackground(const int backgroundType)
{
//	const int		graphicXPos = 40;
//	const int		graphicYPos = 340;
//	const short		graphicHeight = 60;
//	const short		graphicWidth = 560;
//	qhandle_t		background;

/*
	if (backgroundType == ICON_INVENTORY)	// Display inventory background?
	{
		background = cgs.media.inventoryIconBackground;
	}
	else if (backgroundType == ICON_WEAPONS)	// Display weapon background?
	{
		background = cgs.media.weaponIconBackground;
	}
	else 	// Display force background?
	{
		background = cgs.media.forceIconBackground;
	}

	cgi_R_SetColor( colorTable[CT_WHITE] );	// Let the graphic set the color

	CG_DrawPic( graphicXPos, 
		graphicYPos+(graphicHeight/2), 
		graphicWidth, 
		-graphicHeight, 
		background);	// Top half

	CG_DrawPic( graphicXPos, 
		graphicYPos+(graphicHeight/2),
		graphicWidth, 
		graphicHeight, 
		background);	// Bottom half

*/
}

/*
===============
SetWeaponSelectTime
===============
*/
void SetWeaponSelectTime(void)
{

	if (((cg.inventorySelectTime + WEAPON_SELECT_TIME) > cg.time) ||	// The Inventory HUD was currently active to just swap it out with Force HUD
		((cg.forcepowerSelectTime + WEAPON_SELECT_TIME) > cg.time))		// The Force HUD was currently active to just swap it out with Force HUD
	{
		cg.inventorySelectTime = 0;
		cg.forcepowerSelectTime = 0;
		cg.weaponSelectTime = cg.time + 130.0f;
	}
	else
	{
		cg.weaponSelectTime = cg.time;
	}
}

/*
===================
CG_DrawWeaponSelect
===================
*/
extern Vehicle_t *G_IsRidingVehicle( gentity_t *ent );
extern bool G_IsRidingTurboVehicle( gentity_t *ent );

void CG_DrawWeaponSelect( void ) 
{
	int		i;
	// int		bits; FUCK
	int		count;
	int		smallIconSize,bigIconSize;
	int		holdX,x,y,x2,y2,w2,h2,pad;
	int		sideLeftIconCnt,sideRightIconCnt;
	int		sideMax,holdCount,iconCnt;
	int		height;
	vec4_t	calcColor;
	vec4_t	textColor = { .875f, .718f, .121f, 1.0f };
	int		yOffset = 0;
	bool	isOnVeh = false;
	qboolean drewConc = qfalse;

	if ((cg.weaponSelectTime+WEAPON_SELECT_TIME)<cg.time)	// Time is up for the HUD to display
	{
		return;
	}

	// don't display if dead
	if ( cg.predicted_player_state.stats[STAT_HEALTH] <= 0 ) 
	{
		return; 
	}

	cg.iconSelectTime = cg.weaponSelectTime;

	// showing weapon select clears pickup item display, but not the blend blob
	//cg.itemPickupTime = 0;

	// bits = cg.snap->ps.stats[ STAT_WEAPONS ]; REMOVED FOR 64BIT WEAPONS 

	// count the number of weapons owned
	count = 0;
	isOnVeh = (G_IsRidingVehicle(cg_entities[0].gent)!=0);
 	//for ( i = 1 ; i < MAX_PLAYER_WEAPONS ; i++ ) 
	for ( i = 1 ; i < WP_NUM_WEAPONS ; i++ )
	{
		//if ((bits & ( 1 << i ))  &&
			//(!isOnVeh || i==WP_NONE || i==WP_SABER || i==WP_BLASTER)) 
		if ((cg.snap->ps.weapons[i])  &&
			playerUsableWeapons[i] &&
			(!isOnVeh || i==WP_NONE || i==WP_SABER || i==WP_BLASTER)) 
		{
			count++;
		}
	}

	if (count == 0)	// If no weapons, don't display
	{
		return;
	}

	sideMax = 3;	// Max number of icons on the side

	// Calculate how many icons will appear to either side of the center one
	holdCount = count - 1;	// -1 for the center icon
	if (holdCount == 0)			// No icons to either side
	{
		sideLeftIconCnt = 0;
		sideRightIconCnt = 0;
	}
	else if (count > (2*sideMax))	// Go to the max on each side
	{
		sideLeftIconCnt = sideMax;
		sideRightIconCnt = sideMax;
	}
	else							// Less than max, so do the calc
	{
		sideLeftIconCnt = holdCount/2;
		sideRightIconCnt = holdCount - sideLeftIconCnt;
	}

	if ( cg.weaponSelect == WP_CONCUSSION )
	{
		i = WP_FLECHETTE;
	}
	else
	{
		i = cg.weaponSelect - 1;
	}
	if (i<1)
	{
		//i = MAX_PLAYER_WEAPONS;
		i = WP_NUM_WEAPONS;
	}

	smallIconSize = 40;
	bigIconSize = 80;
	pad = 12;

	if (!cgi_UI_GetMenuInfo("weaponselecthud",&x2,&y2,&w2,&h2))
	{
		return;
	}
	x = 320;
	y = 410;

	// Background
	memcpy(calcColor, colorTable[CT_WHITE], sizeof(vec4_t));
	calcColor[3] = .60f;
	cgi_R_SetColor( calcColor);					

	// Left side ICONS
	cgi_R_SetColor( calcColor);					
	// Work backwards from current icon
	holdX = x - ((bigIconSize/2) + pad + smallIconSize);
	height = smallIconSize * cg.iconHUDPercent;
	drewConc = qfalse;

	for (iconCnt=1;iconCnt<(sideLeftIconCnt+1);i--)
	{
		if ( i == WP_CONCUSSION )
		{
			i--;
		}
		else if ( i == WP_FLECHETTE && !drewConc && cg.weaponSelect != WP_CONCUSSION )
		{
			i = WP_CONCUSSION;
		}
		if (i<1)
		{
			//i = MAX_PLAYER_WEAPONS;
			i = WP_NUM_WEAPONS;
		}

		//if ( !(bits & ( 1 << i )))	// Does he have this weapon?
		if ( !(cg.snap->ps.weapons[i] && playerUsableWeapons[i]) )	// Does he have this weapon?
		{
			if ( i == WP_CONCUSSION )
			{
				drewConc = qtrue;
				i = WP_ROCKET_LAUNCHER;
			}
			continue;
		}
		if (isOnVeh)
		{
			if ( i != WP_NONE && i!=WP_SABER && i!=WP_BLASTER )
			{
				if ( i == WP_CONCUSSION )
				{
					drewConc = qtrue;
					i = WP_ROCKET_LAUNCHER;
				}
				continue;	// Don't draw anything else if on a vehicle
			}
		}

		++iconCnt;					// Good icon

		if (weaponData[i].weaponIcon[0])
		{
			weaponInfo_t	*weaponInfo;
			CG_RegisterWeapon( i );	
			weaponInfo = &cg_weapons[i];

			if (!CG_WeaponCheck(i))
			{
				CG_DrawPic( holdX, y+10+yOffset, smallIconSize, smallIconSize, weaponInfo->weaponIconNoAmmo );
			}
			else
			{
				CG_DrawPic( holdX, y+10+yOffset, smallIconSize, smallIconSize, weaponInfo->weaponIcon );
			}

			holdX -= (smallIconSize+pad);
		}
		if ( i == WP_CONCUSSION )
		{
			drewConc = qtrue;
			i = WP_ROCKET_LAUNCHER;
		}
	}

	// Current Center Icon
	height = bigIconSize * cg.iconHUDPercent;
	cgi_R_SetColor(NULL);
	if (weaponData[cg.weaponSelect].weaponIcon[0])
	{
		weaponInfo_t	*weaponInfo;
		CG_RegisterWeapon( cg.weaponSelect );	
		weaponInfo = &cg_weapons[cg.weaponSelect];

		if (!CG_WeaponCheck(cg.weaponSelect))
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2))+10+yOffset, bigIconSize, bigIconSize, weaponInfo->weaponIconNoAmmo );
		}
		else
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2))+10+yOffset, bigIconSize, bigIconSize, weaponInfo->weaponIcon );
		}
	}

	if ( cg.weaponSelect == WP_CONCUSSION )
	{
		i = WP_ROCKET_LAUNCHER;
	}
	else
	{
		i = cg.weaponSelect + 1;
	}
	//if (i> MAX_PLAYER_WEAPONS)
	if (i>= WP_NUM_WEAPONS)
	{
		i = 1;
	}

	// Right side ICONS
	// Work forwards from current icon
	cgi_R_SetColor( calcColor);					
	holdX = x + (bigIconSize/2) + pad;
	height = smallIconSize * cg.iconHUDPercent;
	drewConc = qfalse;
	for (iconCnt=1;iconCnt<(sideRightIconCnt+1);i++)
	{
		if ( i == WP_CONCUSSION )
		{
			i++;
		}
		else if ( i == WP_ROCKET_LAUNCHER && !drewConc && cg.weaponSelect != WP_CONCUSSION )
		{
			i = WP_CONCUSSION;
		}
		//if (i>MAX_PLAYER_WEAPONS)
		if (i>= WP_NUM_WEAPONS)
		{
			i = 1;
		}

		//if ( !(bits & ( 1 << i )))	// Does he have this weapon?
		if ( !(cg.snap->ps.weapons[i] && playerUsableWeapons[i]))	// Does he have this weapon?
		{
			if ( i == WP_CONCUSSION )
			{
				drewConc = qtrue;
				i = WP_FLECHETTE;
			}
			continue;
		}
		if (isOnVeh)
		{
			if ( i != WP_NONE && i!=WP_SABER && i!=WP_BLASTER )
			{
				if ( i == WP_CONCUSSION )
				{
					drewConc = qtrue;
					i = WP_FLECHETTE;
				}
				continue;	// Don't draw anything else if on a vehicle
			}
		}

		++iconCnt;					// Good icon

		if (weaponData[i].weaponIcon[0])
		{
			weaponInfo_t	*weaponInfo;
			CG_RegisterWeapon( i );	
			weaponInfo = &cg_weapons[i];
			// No ammo for this weapon?
			if (!CG_WeaponCheck(i))
			{
				CG_DrawPic( holdX, y+10+yOffset, smallIconSize, smallIconSize, weaponInfo->weaponIconNoAmmo );
			}
			else
			{
				CG_DrawPic( holdX, y+10+yOffset, smallIconSize, smallIconSize, weaponInfo->weaponIcon );
			}


			holdX += (smallIconSize+pad);
		}
		if ( i == WP_CONCUSSION )
		{
			drewConc = qtrue;
			i = WP_FLECHETTE;
		}
	}

	gitem_t *item = cg_weapons[ cg.weaponSelect ].item;

	// draw the selected name
	if ( item && item->classname && item->classname[0] ) 
	{
		char text[1024];
		
		if ( cgi_SP_GetStringTextString( va("SP_INGAME_%s",item->classname), text, sizeof( text )))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24)+yOffset, text, textColor, cgs.media.qhFontSmall, -1, 1.0f);
		}
		else if ( cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",item->classname), text, sizeof( text )))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24)+yOffset, text, textColor, cgs.media.qhFontSmall, -1, 1.0f);
		}
	}

	cgi_R_SetColor( NULL );
}


/*
===============
CG_WeaponSelectable
===============
*/
qboolean CG_WeaponSelectable( int i, int original, qboolean dpMode ) 
{
	int	usage_for_weap;
	if (i >= WP_NUM_WEAPONS || !playerUsableWeapons[i])
	//if (i > MAX_PLAYER_WEAPONS)
	{	
#ifndef FINAL_BUILD
		Com_Printf("CG_WeaponSelectable() passed illegal index of %d!\n",i);
#endif
		return qfalse;
	}

	if ( cg.weaponSelectTime + 200 > cg.time )
	{//TEMP standard weapon cycle debounce for E3 because G2 can't keep up with fast weapon changes
		return qfalse;
	}

	//FIXME: this doesn't work below, can still cycle too fast!
	if ( original == WP_SABER && cg.weaponSelectTime + 500 > cg.time )
	{//when sqitch to lightsaber, have to stay there for at least half a second!
		return qfalse;
	}

	if ( G_IsRidingVehicle(cg_entities[0].gent) )
	{
		if (G_IsRidingTurboVehicle(cg_entities[0].gent) || (i!=WP_NONE && i!=WP_SABER && i!=WP_BLASTER) )
		{ 
			return qfalse;
		}
	}

	if (( weaponData[i].ammoIndex != AMMO_NONE ) && !dpMode )
	{//weapon uses ammo, see if we have any
		usage_for_weap = weaponData[i].energyPerShot < weaponData[i].altEnergyPerShot 
									? weaponData[i].energyPerShot 
									: weaponData[i].altEnergyPerShot;

		if ( cg.snap->ps.ammo[weaponData[i].ammoIndex] - usage_for_weap < 0 ) 
		{
			if ( i != WP_DET_PACK ) // detpack can be switched to...should possibly check if there are any stuck to a wall somewhere?
			{
				// This weapon doesn't have enough ammo to shoot either the main or the alt-fire
				return qfalse;
			}
		}
	}

	//if (!(cg.snap->ps.stats[ STAT_WEAPONS ] & ( 1 << i ))) 
	if (!(cg.snap->ps.weapons[i]))
	{
		// Don't have this weapon to start with.
		return qfalse;
	}

	return qtrue;
}

void CG_ToggleATSTWeapon( void )
{
	if ( cg.weaponSelect == WP_ATST_MAIN )
	{
		cg.weaponSelect = WP_ATST_SIDE;
	}
	else
	{
		cg.weaponSelect = WP_ATST_MAIN;
	}
//	cg.weaponSelectTime = cg.time;
	SetWeaponSelectTime();
}

// SWITCH WEAPONS AVAILABLE FOR MIST BAT AND WOLF FORM ! 

void CG_ToggleFormsWeapon( void )
{
	if ( cg.weaponSelect == WP_SABER )
	{
		cg.weaponSelect = WP_MELEE;
	}
	else
	{
		cg.weaponSelect = WP_SABER;
	}
//	cg.weaponSelectTime = cg.time;
	SetWeaponSelectTime();
}

void CG_ToggleMistWeapon( void )
{
	if ( cg.weaponSelect == WP_MELEE )
	{
		cg.weaponSelect = WP_NONE;
	}
	else
	{
		cg.weaponSelect = WP_MELEE;
	}
//	cg.weaponSelectTime = cg.time;
	SetWeaponSelectTime();
}

// Weapons Available for Fighters 
void CG_ToggleFighterWeapon( void )
{
	if ( cg.weaponSelect == WP_SABER )
	// Deflector Weapon shield 
	{
		cg.weaponSelect = WP_BRYAR_PISTOL;
	}
	else if ( cg.weaponSelect == WP_BRYAR_PISTOL )
	// stunner
	{
		cg.weaponSelect = WP_EMPLACED_GUN; 
	}
	// laser primari  
	else if ( cg.weaponSelect == WP_EMPLACED_GUN )
	{
		cg.weaponSelect = WP_ATST_MAIN;
	}// Dardi e Obici 
	else if ( cg.weaponSelect == WP_ATST_MAIN )
	{
		cg.weaponSelect = WP_ATST_SIDE;
	}
	// Torpedo
	else if ( cg.weaponSelect == WP_ATST_SIDE )
	{
		cg.weaponSelect = WP_TIE_FIGHTER;
	}
	//Armi a raggio - disgreatore
	else if ( cg.weaponSelect == WP_TIE_FIGHTER )
	{
		cg.weaponSelect = WP_JAWA;
	}
	// Armi a dispersione   
	else if ( cg.weaponSelect == WP_JAWA )
	{
		cg.weaponSelect = WP_BOT_LASER;
	}
	// Armi a impulso e bombe al plasma
	else if ( cg.weaponSelect == WP_BOT_LASER )
	{
		cg.weaponSelect = WP_RAPID_FIRE_CONC;
	}
	// Cannoni e Hyperlaser
	else if ( cg.weaponSelect == WP_RAPID_FIRE_CONC )
	{
		cg.weaponSelect = WP_ANTIMATTER;
	}
	// Razzi ad Antimateria 
	else if ( cg.weaponSelect == WP_ANTIMATTER )
	{
		cg.weaponSelect = WP_DEFENSE;
	}
	// Difesa del caccia: scudi, chaff, flare, shaprnels
	else if ( cg.weaponSelect == WP_DEFENSE )
	{
		cg.weaponSelect = WP_DRONE;
	}
	// Droni, e armi a controllo remoto
	else if ( cg.weaponSelect == WP_DRONE )
	{	
		cg.weaponSelect = WP_POSITRON;
	}
	// CANNONE AD ANTIMETERIA POSITRONICO
	else if ( cg.weaponSelect == WP_POSITRON )
	{
		cg.weaponSelect = WP_BRYAR_PISTOL;
	}
	//	cg.weaponSelectTime = cg.time;
	SetWeaponSelectTime();
}

void CG_PlayerLockedWeaponSpeech( int jumping )
{
extern qboolean Q3_TaskIDPending( gentity_t *ent, taskID_t taskType );
	static int speechDebounceTime = 0;
	if ( !in_camera )
	{//not in a cinematic
		if ( speechDebounceTime < cg.time )
		{//spoke more than 3 seconds ago
			if ( !Q3_TaskIDPending( &g_entities[0], TID_CHAN_VOICE ) )
			{//not waiting on a scripted sound to finish
				if( !jumping )
				{
					if( random() > 0.5 )
					{
						G_SoundOnEnt( player, CHAN_VOICE, va( "sound/chars/kyle/09kyk015.wav" ));
					}
					else
					{
						G_SoundOnEnt( player, CHAN_VOICE, va( "sound/chars/kyle/09kyk016.wav" ));
					}
				}
				else
				{
					G_SoundOnEnt( player, CHAN_VOICE, va( "sound/chars/kyle/16kyk007.wav" ));
				}
				speechDebounceTime = cg.time + 3000;
			}
		}
	}
}

/*
===============
CG_NextWeapon_f
===============
*/
void CG_NextWeapon_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	/*
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}
	*/

	if( g_entities[0].flags & FL_LOCK_PLAYER_WEAPONS )
	{
		CG_PlayerLockedWeaponSpeech( qfalse );
		return;
	}

	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_ATST )
	{
		CG_ToggleATSTWeapon();
		return;
	}

	// Fighers weapons
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_FIGHTER )
	{
		CG_ToggleFighterWeapon();
		return;
	}

	// WOLF AND BAT FORM CAN USE ONLY SABER AND MELEE AND NONE WEAPONS! 
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFFORM ||		
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLWOLF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 
		 // BAT FORM  
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATFORM ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLBAT || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF3 )
	{
		CG_ToggleFormsWeapon();
		return;
	}
	// MIST FORM 
	if(	g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST3 )
	{
		CG_ToggleMistWeapon(); 
		return; 
	}	

	if ( cg.snap->ps.eFlags & EF_LOCKED_TO_WEAPON )
	{
		// can't do any sort of weapon switching when in the emplaced gun
		return;
	}

	if ( cg.snap->ps.viewEntity )
	{
		// yeah, probably need a better check here
		if ( g_entities[cg.snap->ps.viewEntity].client && ( g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_R5D2 
				|| g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_R2D2 
				|| g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_MOUSE ))
		{
			return;
		}
	}

	original = cg.weaponSelect;

	int firstWeapon = FIRST_WEAPON;
	if (G_IsRidingVehicle(&g_entities[cg.snap->ps.viewEntity]))
	{
		firstWeapon = 0;	// include WP_NONE here
	}

	//for ( i = 0 ; i <= MAX_PLAYER_WEAPONS ; i++ ) 
	for ( i = 0 ; i < WP_NUM_WEAPONS ; i++ )
	{
		/////////////////////// 
		// NEXT WEAPON ROLL COMPLETE FOR NAVIGATE (TEMPORANY. LATER I'LL SEPARATE ALL THE STUFF) 
		if ( cg.weaponSelect == WP_SABER )
		{ // the saber is the way for quick access to items. 
			cg.weaponSelect = WP_ITEM1;
		}
		else if ( cg.weaponSelect == WP_BLASTER_PISTOL )
		{
			cg.weaponSelect = WP_BLASTER;
		}
		else if ( cg.weaponSelect == WP_BLASTER )
		{
			cg.weaponSelect = WP_DISRUPTOR;
		}
		else if ( cg.weaponSelect == WP_DISRUPTOR )
		{
			cg.weaponSelect = WP_REPEATER;
		}
		else if ( cg.weaponSelect == WP_REPEATER )
		{
			cg.weaponSelect = WP_FLECHETTE;
		}
		else if ( cg.weaponSelect == WP_FLECHETTE )
		{
			cg.weaponSelect = WP_BOWCASTER;
		}
		else if ( cg.weaponSelect == WP_BOWCASTER )
		{
			cg.weaponSelect = WP_ROCKET_LAUNCHER;
		}
		else if ( cg.weaponSelect == WP_ROCKET_LAUNCHER )
		{
			cg.weaponSelect = WP_NOGHRI_STICK;
		}
		else if ( cg.weaponSelect == WP_NOGHRI_STICK )
		{
			cg.weaponSelect = WP_DEMP2;
		}
		else if ( cg.weaponSelect == WP_DEMP2 )
		{
			cg.weaponSelect = WP_CONCUSSION;
		}
		else if ( cg.weaponSelect == WP_CONCUSSION )
		{
			cg.weaponSelect = WP_THERMAL;
		}
		// SEPARATED ROLL 
		else if ( cg.weaponSelect == WP_THERMAL )
		{
			cg.weaponSelect = WP_TRIP_MINE;
		}
		else if ( cg.weaponSelect == WP_TRIP_MINE )
		{
			cg.weaponSelect = WP_DET_PACK;
		}
		else if ( cg.weaponSelect == WP_DET_PACK )
		{
			cg.weaponSelect = firstWeapon; // CLOSED ROLL GUERRIERO
		}
		
		//------------------------------------------------------------------------
		//CONFIG2-. ARMI DA GUERRIERO 
		else if ( cg.weaponSelect == WP_MELEE )
		{
			cg.weaponSelect = WP_BOW;
		}
		else if ( cg.weaponSelect == WP_BOW )
		{
			cg.weaponSelect = WP_CROSSBOW;
		}
		else if ( cg.weaponSelect == WP_CROSSBOW )
		{
			cg.weaponSelect = WP_SLING;
		}
		else if ( cg.weaponSelect == WP_SLING )
		{
			cg.weaponSelect = WP_THROWABLE;
		}
		else if ( cg.weaponSelect == WP_THROWABLE )
		{
			cg.weaponSelect = WP_STUN_BATON;
		}
		else if ( cg.weaponSelect == WP_STUN_BATON )
		{
			cg.weaponSelect = WP_TUSKEN_STAFF;
		}
		else if ( cg.weaponSelect == WP_TUSKEN_STAFF )
		{
			cg.weaponSelect = WP_TUSKEN_RIFLE;
		}
		else if ( cg.weaponSelect == WP_TUSKEN_RIFLE )
		{
			cg.weaponSelect = WP_CANNON;
		}
		else if ( cg.weaponSelect == WP_CANNON )
		{
			cg.weaponSelect = WP_HOOKER;
		}
		else if ( cg.weaponSelect == WP_HOOKER )
		{
			cg.weaponSelect = WP_SCEPTER;
		}
		// WARRIOR GRENADES 
		else if ( cg.weaponSelect == WP_SCEPTER )
		{
			cg.weaponSelect = WP_THERMAL;
		}
		else if ( cg.weaponSelect == WP_THERMAL )
		{
			cg.weaponSelect = WP_TRIP_MINE;
		}
		else if ( cg.weaponSelect == WP_TRIP_MINE )
		{
			cg.weaponSelect = WP_DET_PACK;
		}
		// HACKED 
		else if ( cg.weaponSelect == WP_DET_PACK )
		{
			cg.weaponSelect = firstWeapon;
		}
		//-------------------------------------------------------------------
		// CONFIG: 3: ARMI DA MAGO  
		else if ( cg.weaponSelect == WP_DARK )
		{
			cg.weaponSelect = WP_LIGHT;
		}
		else if ( cg.weaponSelect == WP_LIGHT )
		{
			cg.weaponSelect = WP_FIRE;
		}
		else if ( cg.weaponSelect == WP_FIRE )
		{
			cg.weaponSelect = WP_AIR;
		}
		else if ( cg.weaponSelect == WP_AIR )
		{
			cg.weaponSelect = WP_WATER;
		}
		else if ( cg.weaponSelect == WP_WATER )
		{
			cg.weaponSelect = WP_EARTH;
		}
		else if ( cg.weaponSelect == WP_EARTH )
		{
			cg.weaponSelect = WP_SONIC;
		}
		else if ( cg.weaponSelect == WP_SONIC )
		{
			cg.weaponSelect = WP_PSYCHIC;
		}
		else if ( cg.weaponSelect == WP_PSYCHIC )
		{
			cg.weaponSelect = WP_WOOD;
		}
		else if ( cg.weaponSelect == WP_WOOD )
		{
			cg.weaponSelect = WP_SPIRIT;
		}
		else if ( cg.weaponSelect == WP_SPIRIT )
		{
			cg.weaponSelect = WP_HOLY;
		}
		else if ( cg.weaponSelect == WP_HOLY )
		{
			cg.weaponSelect = WP_SUMMON1;
		}
		else if ( cg.weaponSelect == WP_NECRO )
		{
			cg.weaponSelect = WP_SUMMON1;
		}
		// CONFIG 4: Armi dei Caccia e NPC 
		else if ( cg.weaponSelect == WP_BRYAR_PISTOL )
		// stunner
		{
			cg.weaponSelect = WP_EMPLACED_GUN; 
		}
		// laser primari  
		else if ( cg.weaponSelect == WP_EMPLACED_GUN )
		{
			cg.weaponSelect = WP_ATST_MAIN;
		}// Dardi e Obici 
		else if ( cg.weaponSelect == WP_ATST_MAIN )
		{
			cg.weaponSelect = WP_ATST_SIDE;
		}
		// Torpedo
		else if ( cg.weaponSelect == WP_ATST_SIDE )
		{
			cg.weaponSelect = WP_TIE_FIGHTER;
		}
		//Armi a raggio - disgreatore
		else if ( cg.weaponSelect == WP_TIE_FIGHTER )
		{
			cg.weaponSelect = WP_JAWA;
		}
		// Armi a dispersione   
		else if ( cg.weaponSelect == WP_JAWA )
		{	
			cg.weaponSelect = WP_BOT_LASER;
		}
		// Armi a impulso e bombe al plasma
		else if ( cg.weaponSelect == WP_BOT_LASER )
		{
			cg.weaponSelect = WP_RAPID_FIRE_CONC;
		}
		// Cannoni e Hyperlaser
		else if ( cg.weaponSelect == WP_RAPID_FIRE_CONC )
		{
			cg.weaponSelect = WP_ANTIMATTER;
		}
		// Razzi ad Antimateria 
		else if ( cg.weaponSelect == WP_ANTIMATTER )
		{
			cg.weaponSelect = WP_DEFENSE;
		}
		// Difesa del caccia: scudi, chaff, flare, shaprnels
		else if ( cg.weaponSelect == WP_DEFENSE )
		{
			cg.weaponSelect = WP_DRONE;
		}
		// Droni, e armi a controllo remoto
		else if ( cg.weaponSelect == WP_DRONE )
		{	
			cg.weaponSelect = WP_POSITRON;
		}
		// CANNONE AD ANTIMETERIA POSITRONICO
		else if ( cg.weaponSelect == WP_POSITRON )
		{
			cg.weaponSelect = firstWeapon;

		}
		// INVENTARIO CONFIG 5 EVOCAZIONI E MORPH
		else if ( cg.weaponSelect == WP_SUMMON1 )
		{
			cg.weaponSelect = WP_SUMMON2;
		}
		else if ( cg.weaponSelect == WP_SUMMON2 )
		{
			cg.weaponSelect = WP_SUMMON3;
		}
		else if ( cg.weaponSelect == WP_SUMMON3 )
		{
			cg.weaponSelect = WP_SUMMON4;
		}
		else if ( cg.weaponSelect == WP_MORPH1 )
		{
			cg.weaponSelect = WP_MORPH2;
		}
		else if ( cg.weaponSelect == WP_MORPH2 )
		{
			cg.weaponSelect = WP_MORPH3;
		}
		else if ( cg.weaponSelect == WP_MORPH3 )
		{
			cg.weaponSelect = WP_MORPH4;
		}
		else if ( cg.weaponSelect == WP_MORPH4 )
		{
			cg.weaponSelect = firstWeapon;
		}
		// INVENTARIO CONFIG 6: ITEMS
		else if ( cg.weaponSelect == WP_ITEM1 )
		{
			cg.weaponSelect = WP_ITEM2;
		}
		else if ( cg.weaponSelect == WP_ITEM2 )
		{
			cg.weaponSelect = WP_ITEM3;
		}
		else if ( cg.weaponSelect == WP_ITEM3 )
		{
			cg.weaponSelect = WP_ITEM4;
		}
		else if ( cg.weaponSelect == WP_ITEM4 )
		{
			cg.weaponSelect = WP_ITEM5;
		}
		else if ( cg.weaponSelect == WP_ITEM5 )
		{
			cg.weaponSelect = WP_ITEM6;
		}
		else if ( cg.weaponSelect == WP_ITEM6 )
		{
			cg.weaponSelect = WP_ITEM7;
		}
		else if ( cg.weaponSelect == WP_ITEM7 )
		{
			cg.weaponSelect = WP_ITEM8;
		}
		else if ( cg.weaponSelect == WP_ITEM8 )
		{
			cg.weaponSelect = WP_ITEM9;
		}
		else if ( cg.weaponSelect == WP_ITEM9 )
		{
			cg.weaponSelect = WP_ITEM10;
		}
		else if ( cg.weaponSelect == WP_ITEM10 )
		{
			cg.weaponSelect = WP_ITEM11;
		}
		else if ( cg.weaponSelect == WP_ITEM11 )
		{
			cg.weaponSelect = WP_ITEM12;
		}
		else if ( cg.weaponSelect == WP_ITEM12 )
		{
			cg.weaponSelect = firstWeapon;
		}
		else
		{
			cg.weaponSelect++;
		}
		//if ( cg.weaponSelect < firstWeapon || cg.weaponSelect > MAX_PLAYER_WEAPONS) { 
			//cg.weaponSelect = firstWeapon; 
		if ( cg.weaponSelect < firstWeapon || cg.weaponSelect >= WP_NUM_WEAPONS) {
			cg.weaponSelect = firstWeapon; 
		}
		
		if ( CG_WeaponSelectable( cg.weaponSelect, original, qfalse ) ) 
		{	
//			cg.weaponSelectTime = cg.time;
			SetWeaponSelectTime();
			return;
		}
	}

	cg.weaponSelect = original;
}

/*
===============
CG_DPNextWeapon_f
===============
*/
void CG_DPNextWeapon_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	/*
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}
	*/

	original = cg.DataPadWeaponSelect;

	for ( i = 0 ; i < WP_NUM_WEAPONS ; i++ )
	//for ( i = 0 ; i <= MAX_PLAYER_WEAPONS ; i++ ) 
	{
		if ( cg.DataPadWeaponSelect == WP_SABER )
		{
			cg.DataPadWeaponSelect = WP_BLASTER_PISTOL;
		}
		if ( cg.DataPadWeaponSelect == WP_BLASTER_PISTOL )
		{
			cg.DataPadWeaponSelect = WP_BLASTER;
		}
		else if ( cg.DataPadWeaponSelect == WP_BLASTER )
		{
			cg.DataPadWeaponSelect = WP_DISRUPTOR;
		}
		else if ( cg.DataPadWeaponSelect == WP_DISRUPTOR )
		{
			cg.DataPadWeaponSelect = WP_REPEATER;
		}
		else if ( cg.DataPadWeaponSelect == WP_REPEATER )
		{
			cg.DataPadWeaponSelect = WP_FLECHETTE;
		}
		else if ( cg.DataPadWeaponSelect == WP_FLECHETTE )
		{
			cg.DataPadWeaponSelect = WP_BOWCASTER;
		}
		else if ( cg.DataPadWeaponSelect == WP_BOWCASTER )
		{
			cg.DataPadWeaponSelect = WP_ROCKET_LAUNCHER;
		}
		else if ( cg.DataPadWeaponSelect == WP_ROCKET_LAUNCHER )
		{
			cg.DataPadWeaponSelect = WP_NOGHRI_STICK;
		}
		else if ( cg.DataPadWeaponSelect == WP_NOGHRI_STICK )
		{
			cg.DataPadWeaponSelect = WP_DEMP2;
		}
		else if ( cg.DataPadWeaponSelect == WP_DEMP2 )
		{
			cg.DataPadWeaponSelect = WP_CONCUSSION;
		}
		else if ( cg.DataPadWeaponSelect == WP_CONCUSSION )
		{
			cg.DataPadWeaponSelect = WP_THERMAL;
		}
		// SEPARATED ROLL 
		else if ( cg.DataPadWeaponSelect == WP_THERMAL )
		{
			cg.DataPadWeaponSelect = WP_TRIP_MINE;
		}
		else if ( cg.DataPadWeaponSelect == WP_TRIP_MINE )
		{
			cg.DataPadWeaponSelect = WP_DET_PACK;
		}
		else if ( cg.DataPadWeaponSelect == WP_DET_PACK )
		{
			cg.DataPadWeaponSelect = WP_MELEE; // TEMP 
		}
		//------------------------------------------------------------------------
		//CONFIG2-. ARMI DA GUERRIERO 
		else if ( cg.DataPadWeaponSelect == WP_MELEE )
		{
			cg.DataPadWeaponSelect = WP_BOW;
		}
		else if ( cg.DataPadWeaponSelect == WP_BOW )
		{
			cg.DataPadWeaponSelect = WP_CROSSBOW;
		}
		else if ( cg.DataPadWeaponSelect == WP_CROSSBOW )
		{
			cg.DataPadWeaponSelect = WP_SLING;
		}
		else if ( cg.DataPadWeaponSelect == WP_SLING )
		{
			cg.DataPadWeaponSelect = WP_THROWABLE;
		}
		else if ( cg.DataPadWeaponSelect == WP_THROWABLE )
		{
			cg.DataPadWeaponSelect = WP_STUN_BATON;
		}
		else if ( cg.DataPadWeaponSelect == WP_STUN_BATON )
		{
			cg.DataPadWeaponSelect = WP_TUSKEN_STAFF;
		}
		else if ( cg.DataPadWeaponSelect == WP_TUSKEN_STAFF )
		{
			cg.DataPadWeaponSelect = WP_TUSKEN_RIFLE;
		}
		else if ( cg.DataPadWeaponSelect == WP_TUSKEN_RIFLE )
		{
			cg.DataPadWeaponSelect = WP_CANNON;
		}
		else if ( cg.DataPadWeaponSelect == WP_CANNON )
		{
			cg.DataPadWeaponSelect = WP_HOOKER;
		}
		else if ( cg.DataPadWeaponSelect == WP_HOOKER )
		{
			cg.DataPadWeaponSelect = WP_SCEPTER;
		}
		else if ( cg.DataPadWeaponSelect == WP_SCEPTER )
		{
			cg.DataPadWeaponSelect = WP_DARK;
		}
		//-------------------------------------------------------------------
		// CONFIG: 3: ARMI DA MAGO  
		else if ( cg.DataPadWeaponSelect == WP_DARK )
		{
			cg.DataPadWeaponSelect = WP_LIGHT;
		}
		else if ( cg.DataPadWeaponSelect == WP_LIGHT )
		{
			cg.DataPadWeaponSelect = WP_LIGHTNING;
		}
		else if ( cg.DataPadWeaponSelect == WP_LIGHTNING )
		{
			cg.DataPadWeaponSelect = WP_FIRE;
		}
		else if ( cg.DataPadWeaponSelect == WP_FIRE )
		{
			cg.DataPadWeaponSelect = WP_FLAME;
		}
		else if ( cg.DataPadWeaponSelect == WP_FLAME )
		{
			cg.DataPadWeaponSelect = WP_AIR;
		}
		else if ( cg.DataPadWeaponSelect == WP_AIR )
		{
			cg.DataPadWeaponSelect = WP_WATER;
		}
		else if ( cg.DataPadWeaponSelect == WP_WATER )
		{
			cg.DataPadWeaponSelect = WP_EARTH;
		}
		else if ( cg.DataPadWeaponSelect == WP_EARTH )
		{
			cg.DataPadWeaponSelect = WP_SONIC;
		}
		else if ( cg.DataPadWeaponSelect == WP_SONIC )
		{
			cg.DataPadWeaponSelect = WP_PSYCHIC;
		}
		else if ( cg.DataPadWeaponSelect == WP_PSYCHIC )
		{
			cg.DataPadWeaponSelect = WP_WOOD;
		}
		else if ( cg.DataPadWeaponSelect == WP_WOOD )
		{
			cg.DataPadWeaponSelect = WP_METAL;
		}
		else if ( cg.DataPadWeaponSelect == WP_METAL )
		{
			cg.DataPadWeaponSelect = WP_SPIRIT;
		}
		else if ( cg.DataPadWeaponSelect == WP_SPIRIT )
		{
			cg.DataPadWeaponSelect = WP_POISON;
		}
		else if ( cg.DataPadWeaponSelect == WP_POISON )
		{
		cg.DataPadWeaponSelect = WP_HOLY;
		}
			else if ( cg.DataPadWeaponSelect == WP_HOLY )
		{
			cg.DataPadWeaponSelect = WP_NECRO;
		}
		else if ( cg.DataPadWeaponSelect == WP_NECRO )
		{
			cg.DataPadWeaponSelect = WP_BRYAR_PISTOL;

		}
		// CONFIG 4: Armi dei Caccia e NPC 
		else if ( cg.DataPadWeaponSelect == WP_BRYAR_PISTOL )
		// stunner
		{
			cg.DataPadWeaponSelect = WP_EMPLACED_GUN; 
		}
		// laser primari  
		else if ( cg.DataPadWeaponSelect == WP_EMPLACED_GUN )
		{
			cg.DataPadWeaponSelect = WP_ATST_MAIN;
		}// Dardi e Obici 
		else if ( cg.DataPadWeaponSelect == WP_ATST_MAIN )
		{
			cg.DataPadWeaponSelect = WP_ATST_SIDE;
		}
		// Torpedo
		else if ( cg.DataPadWeaponSelect == WP_ATST_SIDE )
		{
			cg.DataPadWeaponSelect = WP_TIE_FIGHTER;
		}
		//Armi a raggio - disgreatore
		else if ( cg.DataPadWeaponSelect == WP_TIE_FIGHTER )
		{
			cg.DataPadWeaponSelect = WP_JAWA;
		}
		// Armi a dispersione   
		else if ( cg.DataPadWeaponSelect == WP_JAWA )
		{	
			cg.DataPadWeaponSelect = WP_BOT_LASER;
		}
		// Armi a impulso e bombe al plasma
		else if ( cg.DataPadWeaponSelect == WP_BOT_LASER )
		{
			cg.DataPadWeaponSelect = WP_RAPID_FIRE_CONC;
		}
		// Cannoni e Hyperlaser
		else if ( cg.DataPadWeaponSelect == WP_RAPID_FIRE_CONC )
		{
			cg.DataPadWeaponSelect = WP_ANTIMATTER;
		}
		// Razzi ad Antimateria 
		else if ( cg.DataPadWeaponSelect == WP_ANTIMATTER )
		{
			cg.DataPadWeaponSelect = WP_DEFENSE;
		}
		// Difesa del caccia: scudi, chaff, flare, shaprnels
		else if ( cg.DataPadWeaponSelect == WP_DEFENSE )
		{
			cg.DataPadWeaponSelect = WP_DRONE;
		}
		// Droni, e armi a controllo remoto
		else if ( cg.DataPadWeaponSelect == WP_DRONE )
		{	
			cg.DataPadWeaponSelect = WP_POSITRON;
		}
		// CANNONE AD ANTIMETERIA POSITRONICO
		else if ( cg.DataPadWeaponSelect == WP_POSITRON )
		{
			cg.DataPadWeaponSelect = WP_SUMMON1;
		}
		// INVENTARIO CONFIG 5: ITEMS
		else if ( cg.DataPadWeaponSelect == WP_SUMMON1 )
		{
			cg.DataPadWeaponSelect = WP_SUMMON2;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON2 )
		{
			cg.DataPadWeaponSelect = WP_SUMMON3;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON3 )
		{
			cg.DataPadWeaponSelect = WP_SUMMON4;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON4 )
		{
			cg.DataPadWeaponSelect = WP_MORPH1;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH1 )
		{
			cg.DataPadWeaponSelect = WP_MORPH2;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH2 )
		{
			cg.DataPadWeaponSelect = WP_MORPH3;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH3 )
		{
			cg.DataPadWeaponSelect = WP_MORPH4;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH4 )
		{
			cg.DataPadWeaponSelect = WP_ITEM1;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM1 )
		{
			cg.DataPadWeaponSelect = WP_ITEM2;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM2 )
		{
			cg.DataPadWeaponSelect = WP_ITEM3;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM3 )
		{
			cg.DataPadWeaponSelect = WP_ITEM4;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM4 )
		{
			cg.DataPadWeaponSelect = WP_ITEM5;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM5 )
		{
			cg.DataPadWeaponSelect = WP_ITEM6;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM6 )
		{
			cg.DataPadWeaponSelect = WP_ITEM7;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM7 )
		{
			cg.DataPadWeaponSelect = WP_ITEM8;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM8 )
		{
			cg.DataPadWeaponSelect = WP_ITEM9;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM9 )
		{
			cg.DataPadWeaponSelect = WP_ITEM10;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM10 )
		{
			cg.DataPadWeaponSelect = WP_ITEM11;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM11 )
		{
			cg.DataPadWeaponSelect = WP_ITEM12;
		}
		// HACKED 
		else if ( cg.DataPadWeaponSelect == WP_ITEM12 )
		{
			cg.DataPadWeaponSelect = FIRST_WEAPON;
		}
		else
		{
			cg.DataPadWeaponSelect++;
		}

		//if ( cg.DataPadWeaponSelect < FIRST_WEAPON || cg.DataPadWeaponSelect > MAX_PLAYER_WEAPONS) { 
//			cg.DataPadWeaponSelect = FIRST_WEAPON; 
		if ( cg.DataPadWeaponSelect < FIRST_WEAPON || cg.DataPadWeaponSelect >= WP_NUM_WEAPONS ) {
			cg.DataPadWeaponSelect = FIRST_WEAPON; 
		}
		
		if ( CG_WeaponSelectable( cg.DataPadWeaponSelect, original, qtrue ) ) 
		{	
			return;
		}
	}

	cg.DataPadWeaponSelect = original;
}

/*
===============
CG_DPPrevWeapon_f
===============
*/
void CG_DPPrevWeapon_f( void ) 
{
	int		i;
	int		original;

	if ( !cg.snap ) 
	{
		return;
	}

	/*
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) 
	{
		return;
	}
	*/

	original = cg.DataPadWeaponSelect;

	//for ( i = 0 ; i <= MAX_PLAYER_WEAPONS ; i++ ) 
	for ( i = 0 ; i < WP_NUM_WEAPONS ; i++ )
	{
		// CONFIG 5: Armi dei Caccia e NPC 
		if ( cg.DataPadWeaponSelect == FIRST_WEAPON )
		{
		   cg.DataPadWeaponSelect = WP_ITEM12;
		}
		// ITEMS 
		else if ( cg.DataPadWeaponSelect == WP_ITEM12 )
		{
			cg.DataPadWeaponSelect = WP_ITEM11;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM11 )
		{
			cg.DataPadWeaponSelect = WP_ITEM10;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM10 )
		{
			cg.DataPadWeaponSelect = WP_ITEM9;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM9 )
		{
			cg.DataPadWeaponSelect = WP_ITEM8;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM8 )
		{
			cg.DataPadWeaponSelect = WP_ITEM7;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM7)
		{
			cg.DataPadWeaponSelect = WP_ITEM6;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM6 )
		{
			cg.DataPadWeaponSelect = WP_ITEM5;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM5 )
		{
			cg.DataPadWeaponSelect = WP_ITEM4;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM4 )
		{
			cg.DataPadWeaponSelect = WP_ITEM3;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM3 )
		{
			cg.DataPadWeaponSelect = WP_ITEM2;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM2 )
		{
			cg.DataPadWeaponSelect = WP_ITEM1;
		}
		else if ( cg.DataPadWeaponSelect == WP_ITEM1 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_MORPH4;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH4 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_MORPH3;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH3 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_MORPH2;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH2 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_MORPH1;
		}
		else if ( cg.DataPadWeaponSelect == WP_MORPH1 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_SUMMON4;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON4 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_SUMMON3;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON3 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_SUMMON2;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON2 )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_SUMMON1;
		}
		else if ( cg.DataPadWeaponSelect == WP_SUMMON1 )
		{
			cg.DataPadWeaponSelect = WP_POSITRON;
		}
		else if ( cg.DataPadWeaponSelect == WP_POSITRON )
		{
			cg.DataPadWeaponSelect = WP_DRONE;
		}
		else if ( cg.DataPadWeaponSelect == WP_DRONE )
		// stunner
		{
			cg.DataPadWeaponSelect = WP_DEFENSE; 
		}
		// laser primari  
		else if ( cg.DataPadWeaponSelect == WP_DEFENSE )
		{
			cg.DataPadWeaponSelect = WP_ANTIMATTER;
		}// Dardi e Obici 
		else if ( cg.DataPadWeaponSelect == WP_ANTIMATTER )
		{
			cg.DataPadWeaponSelect = WP_RAPID_FIRE_CONC;
		}
		// Torpedo
		else if ( cg.DataPadWeaponSelect == WP_RAPID_FIRE_CONC )
		{
			cg.DataPadWeaponSelect = WP_BOT_LASER;
		}
		//Armi a raggio - disgreatore
		else if ( cg.DataPadWeaponSelect == WP_BOT_LASER )
		{
			cg.DataPadWeaponSelect = WP_JAWA;
		}
		// Armi a dispersione   
		else if ( cg.DataPadWeaponSelect == WP_JAWA )
		{	
			cg.DataPadWeaponSelect = WP_TIE_FIGHTER;
		}
		// Armi a impulso e bombe al plasma
		else if ( cg.DataPadWeaponSelect == WP_TIE_FIGHTER )
		{
			cg.DataPadWeaponSelect = WP_ATST_SIDE;
		}
		// Cannoni e Hyperlaser
		else if ( cg.DataPadWeaponSelect == WP_ATST_SIDE )
		{
			cg.DataPadWeaponSelect = WP_ATST_MAIN;
		}
		// Razzi ad Antimateria 
		else if ( cg.DataPadWeaponSelect == WP_ATST_MAIN )
		{
			cg.DataPadWeaponSelect = WP_EMPLACED_GUN;
		}
		// Difesa del caccia: scudi, chaff, flare, shaprnels
		else if ( cg.DataPadWeaponSelect == WP_EMPLACED_GUN )
		{
			cg.DataPadWeaponSelect = WP_BRYAR_PISTOL;
		}
		// Droni, e armi a controllo remoto
		else if ( cg.DataPadWeaponSelect == WP_BRYAR_PISTOL )
		{
			cg.DataPadWeaponSelect = WP_NECRO;
		}
		// SPELLS 
		else if ( cg.DataPadWeaponSelect == WP_NECRO )
		{
			cg.DataPadWeaponSelect = WP_HOLY;
		}
		else if ( cg.DataPadWeaponSelect == WP_HOLY )
		{
			cg.DataPadWeaponSelect = WP_POISON;
		}
		else if ( cg.DataPadWeaponSelect == WP_POISON )
		{
			cg.DataPadWeaponSelect = WP_SPIRIT;
		}
		else if ( cg.DataPadWeaponSelect == WP_SPIRIT )
		{
			cg.DataPadWeaponSelect = WP_METAL;
		}
		else if ( cg.DataPadWeaponSelect == WP_METAL )
		{
			cg.DataPadWeaponSelect = WP_WOOD;
		}
		else if ( cg.DataPadWeaponSelect == WP_WOOD)
		{
			cg.DataPadWeaponSelect = WP_PSYCHIC;
		}
		else if ( cg.DataPadWeaponSelect == WP_PSYCHIC )
		{
			cg.DataPadWeaponSelect = WP_SONIC;
		}
		else if ( cg.DataPadWeaponSelect == WP_SONIC )
		{
			cg.DataPadWeaponSelect = WP_EARTH;
		}
		else if ( cg.DataPadWeaponSelect == WP_EARTH )
		{
			cg.DataPadWeaponSelect = WP_WATER;
		}
		else if ( cg.DataPadWeaponSelect == WP_WATER )
		{
			cg.DataPadWeaponSelect = WP_AIR;
		}
		else if ( cg.DataPadWeaponSelect == WP_AIR )
		{
			cg.DataPadWeaponSelect = WP_FLAME;
		}
		else if ( cg.DataPadWeaponSelect == WP_FLAME )
		{ 
			cg.DataPadWeaponSelect = WP_FIRE;
		}
		else if ( cg.DataPadWeaponSelect == WP_FIRE )
		{
			cg.DataPadWeaponSelect = WP_LIGHTNING;
		}
		else if ( cg.DataPadWeaponSelect == WP_LIGHTNING )
		{
			cg.DataPadWeaponSelect = WP_LIGHT;
		}
		else if ( cg.DataPadWeaponSelect == WP_LIGHT )
		{
			cg.DataPadWeaponSelect = WP_DARK;
		}
		else if ( cg.DataPadWeaponSelect == WP_DARK )
		{
			// HACKED
			cg.DataPadWeaponSelect = WP_SCEPTER;
		}
		// WARRIOR
		else if ( cg.DataPadWeaponSelect == WP_SCEPTER )
		{
			cg.DataPadWeaponSelect = WP_HOOKER;
		}
		else if ( cg.DataPadWeaponSelect == WP_HOOKER )
		{
			cg.DataPadWeaponSelect = WP_CANNON;
		}
		else if ( cg.DataPadWeaponSelect == WP_CANNON )
		{
			cg.DataPadWeaponSelect = WP_TUSKEN_RIFLE;
		}
		else if ( cg.DataPadWeaponSelect == WP_TUSKEN_RIFLE )
		{
			cg.DataPadWeaponSelect = WP_TUSKEN_STAFF;
		}
		else if ( cg.DataPadWeaponSelect == WP_TUSKEN_STAFF )
		{
			cg.DataPadWeaponSelect = WP_STUN_BATON;
		}
		else if ( cg.DataPadWeaponSelect == WP_STUN_BATON)
		{
			cg.DataPadWeaponSelect = WP_THROWABLE;
		}
		else if ( cg.DataPadWeaponSelect == WP_THROWABLE )
		{
			cg.DataPadWeaponSelect = WP_SLING;
		}
		else if ( cg.DataPadWeaponSelect == WP_SLING )
		{
			cg.DataPadWeaponSelect = WP_CROSSBOW;
		}
		else if ( cg.DataPadWeaponSelect == WP_CROSSBOW )
		{
			cg.DataPadWeaponSelect = WP_BOW;
		}
		else if ( cg.DataPadWeaponSelect == WP_BOW )
		{
			cg.DataPadWeaponSelect = WP_MELEE;
		}
		else if ( cg.DataPadWeaponSelect == WP_MELEE )
		{
			// HACKED 
			cg.DataPadWeaponSelect = WP_DET_PACK;
		}
		else if ( cg.DataPadWeaponSelect == WP_DET_PACK )
		{// HACKED 
			cg.DataPadWeaponSelect = WP_TRIP_MINE;
		}
		else if ( cg.DataPadWeaponSelect == WP_TRIP_MINE )
		{
			cg.DataPadWeaponSelect = WP_THERMAL;
		}
		else if ( cg.DataPadWeaponSelect == WP_THERMAL )
		{
			cg.DataPadWeaponSelect = WP_CONCUSSION;
		}
		else if ( cg.DataPadWeaponSelect == WP_CONCUSSION )
		{
			cg.DataPadWeaponSelect = WP_DEMP2;
		}
		else if ( cg.DataPadWeaponSelect == WP_DEMP2 )
		{
			cg.DataPadWeaponSelect = WP_NOGHRI_STICK;
		}
		else if ( cg.DataPadWeaponSelect == WP_NOGHRI_STICK )
		{
			cg.DataPadWeaponSelect = WP_ROCKET_LAUNCHER;
		}
		else if ( cg.DataPadWeaponSelect == WP_ROCKET_LAUNCHER)
		{
			cg.DataPadWeaponSelect = WP_BOWCASTER;
		}
		else if ( cg.DataPadWeaponSelect == WP_BOWCASTER )
		{
			cg.DataPadWeaponSelect = WP_FLECHETTE;
		}
		else if ( cg.DataPadWeaponSelect == WP_FLECHETTE )
		{
			cg.DataPadWeaponSelect = WP_REPEATER;
		}
		else if ( cg.DataPadWeaponSelect == WP_REPEATER )
		{
			cg.DataPadWeaponSelect = WP_DISRUPTOR;
		}
		else if ( cg.DataPadWeaponSelect == WP_DISRUPTOR )
		{
			cg.DataPadWeaponSelect = WP_BLASTER;
		}
		else if ( cg.DataPadWeaponSelect == WP_BLASTER )
		{
			cg.DataPadWeaponSelect = WP_BLASTER_PISTOL;
		}
		else if ( cg.DataPadWeaponSelect == WP_BLASTER_PISTOL )
		{// HACKED 
			cg.DataPadWeaponSelect = FIRST_WEAPON;
		}
		else
		{
			cg.DataPadWeaponSelect--;
		}

		/*if ( cg.DataPadWeaponSelect < FIRST_WEAPON || cg.DataPadWeaponSelect > MAX_PLAYER_WEAPONS) 
		{ 
			cg.DataPadWeaponSelect = MAX_PLAYER_WEAPONS;
		}*/
		if ( cg.DataPadWeaponSelect < FIRST_WEAPON || cg.DataPadWeaponSelect >= WP_NUM_WEAPONS )
		{ 
			cg.DataPadWeaponSelect = WP_NUM_WEAPONS;
		}

		
		if ( CG_WeaponSelectable( cg.DataPadWeaponSelect, original, qtrue ) ) 
		{
			return;
		}
	}

	cg.DataPadWeaponSelect = original;
}

/*
===============
CG_PrevWeapon_f
===============
*/
void CG_PrevWeapon_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	/*
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}
	*/

	if( g_entities[0].flags & FL_LOCK_PLAYER_WEAPONS )
	{
		CG_PlayerLockedWeaponSpeech( qfalse );
		return;
	}

	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_ATST )
	{
		CG_ToggleATSTWeapon();
		return;
	}

	// Fighers weapons
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_FIGHTER )
	{
		CG_ToggleFighterWeapon();
		return;
	}

	// WOLF AND BAT FORM CAN USE ONLY SABER AND MELEE AND NONE WEAPONS! 
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFFORM ||		
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLWOLF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
 
		 // BAT FORM  
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATFORM ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLBAT || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF3 )
	{
		CG_ToggleFormsWeapon();
		return;
	}
	// MIST FORM 
	if(	g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST3 )

	{
		CG_ToggleMistWeapon(); 
		return; 
	}

	if ( cg.snap->ps.eFlags & EF_LOCKED_TO_WEAPON )
	{
		// can't do any sort of weapon switching when in the emplaced gun
		return;
	}

	if ( cg.snap->ps.viewEntity )
	{
		// yeah, probably need a better check here
		if ( g_entities[cg.snap->ps.viewEntity].client && ( g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_R5D2 
				|| g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_R2D2 
				|| g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_MOUSE ))
		{
			return;
		}
	}

	original = cg.weaponSelect;

	int firstWeapon = FIRST_WEAPON;
	if (G_IsRidingVehicle(&g_entities[cg.snap->ps.viewEntity]))
	{
		firstWeapon = 0;	// include WP_NONE here
	}

	//for ( i = 0 ; i <= MAX_PLAYER_WEAPONS ; i++ ) {

	for ( i = 0 ; i < WP_NUM_WEAPONS ; i++ ) {
		//*SIGH*... Hack to put concussion rifle before rocketlauncher
		// CONFIG 5: Armi dei Caccia e NPC 
		// ITEMS INVERSED ROLLING 
		if ( cg.weaponSelect == WP_ITEM12 )
		{
			cg.weaponSelect = WP_ITEM11;
		}
		else if ( cg.weaponSelect == WP_ITEM11 )
		{
			cg.weaponSelect = WP_ITEM10;
		}
		else if ( cg.weaponSelect == WP_ITEM10 )
		{
			cg.weaponSelect = WP_ITEM9;
		}
		else if ( cg.weaponSelect == WP_ITEM9 )
		{
			cg.weaponSelect = WP_ITEM8;
		}
		else if ( cg.weaponSelect == WP_ITEM8 )
		{
			cg.weaponSelect = WP_ITEM7;
		}
		else if ( cg.weaponSelect == WP_ITEM7)
		{
			cg.weaponSelect = WP_ITEM6;
		}
		else if ( cg.weaponSelect == WP_ITEM6 )
		{
			cg.weaponSelect = WP_ITEM5;
		}
		else if ( cg.weaponSelect == WP_ITEM5 )
		{
			cg.weaponSelect = WP_ITEM4;
		}
		else if ( cg.weaponSelect == WP_ITEM4 )
		{
			cg.weaponSelect = WP_ITEM3;
		}
		else if ( cg.weaponSelect == WP_ITEM3 )
		{
			cg.weaponSelect = WP_ITEM2;
		}
		else if ( cg.weaponSelect == WP_ITEM2 )
		{
			cg.weaponSelect = WP_ITEM1;
		}
		else if ( cg.weaponSelect == WP_ITEM1 )
		{ 
			cg.weaponSelect = WP_ITEM12;
		}
		// FORME 
		else if ( cg.weaponSelect == WP_MORPH4)
		{
			cg.weaponSelect = WP_MORPH3;
		}
		else if ( cg.weaponSelect == WP_MORPH3 )
		{
			cg.weaponSelect = WP_MORPH2;
		}
		else if ( cg.weaponSelect == WP_MORPH2 )
		{
			cg.weaponSelect = WP_MORPH1;
		}
		else if ( cg.weaponSelect == WP_MORPH1 )
		{
			cg.weaponSelect = WP_SUMMON4;
		}
		else if ( cg.weaponSelect == WP_SUMMON4 )
		{
			cg.weaponSelect = WP_SUMMON3;
		}
		else if ( cg.weaponSelect == WP_SUMMON3 )
		{
			cg.weaponSelect = WP_SUMMON2;
		}
		else if ( cg.weaponSelect == WP_SUMMON2 )
		{ 
			cg.weaponSelect = WP_SUMMON1;
		}
		else if ( cg.weaponSelect == WP_SUMMON2 )
		{ 
			cg.weaponSelect = WP_SUMMON1;
		}
		else if ( cg.weaponSelect == WP_SUMMON1 )
		{ 
			cg.weaponSelect = firstWeapon;
		}
		// VEICOLI 
		else if ( cg.weaponSelect == WP_POSITRON )
		{
			cg.weaponSelect = WP_DRONE;
		}
		else if ( cg.weaponSelect == WP_DRONE )
		{
			cg.weaponSelect = WP_DEFENSE;
		}
		else if ( cg.weaponSelect == WP_DEFENSE )
		// stunner
		{
			cg.weaponSelect = WP_ANTIMATTER; 
		}
		// Dardi e Obici 
		else if ( cg.weaponSelect == WP_ANTIMATTER )
		{
			cg.weaponSelect = WP_RAPID_FIRE_CONC;
		}
		// Torpedo
		else if ( cg.weaponSelect == WP_RAPID_FIRE_CONC )
		{
			cg.weaponSelect = WP_BOT_LASER;
		}
		//Armi a raggio - disgreatore
		else if ( cg.weaponSelect == WP_BOT_LASER )
		{
			cg.weaponSelect = WP_JAWA;
		}
		// Armi a dispersione   
		else if ( cg.weaponSelect == WP_JAWA )
		{	
			cg.weaponSelect = WP_TIE_FIGHTER;
		}
		// Armi a impulso e bombe al plasma
		else if ( cg.weaponSelect == WP_TIE_FIGHTER )
		{
			cg.weaponSelect = WP_ATST_SIDE;
		}
		// Cannoni e Hyperlaser
		else if ( cg.weaponSelect == WP_ATST_SIDE )
		{
			cg.weaponSelect = WP_ATST_MAIN;
		}
		// Razzi ad Antimateria 
		else if ( cg.weaponSelect == WP_ATST_MAIN )
		{
			cg.weaponSelect = WP_EMPLACED_GUN;
		}
		// Difesa del caccia: scudi, chaff, flare, shaprnels
		else if ( cg.weaponSelect == WP_EMPLACED_GUN )
		{
			cg.weaponSelect = WP_BRYAR_PISTOL;
		}
		// Droni, e armi a controllo remoto
		else if ( cg.weaponSelect == WP_BRYAR_PISTOL )
		{	
			// HACKED 
			cg.weaponSelect = firstWeapon;
		}
		// SPELLS 
		else if ( cg.weaponSelect == WP_NECRO )
		{
			cg.weaponSelect = WP_HOLY;
		}
		else if ( cg.weaponSelect == WP_HOLY )
		{
			cg.weaponSelect = WP_POISON;
		}
		else if ( cg.weaponSelect == WP_POISON )
		{
			cg.weaponSelect = WP_SPIRIT;
		}
		else if ( cg.weaponSelect == WP_SPIRIT )
		{
			cg.weaponSelect = WP_METAL;
		}
		else if ( cg.weaponSelect == WP_METAL )
		{
			cg.weaponSelect = WP_WOOD;
		}
		else if ( cg.weaponSelect == WP_WOOD)
		{
			cg.weaponSelect = WP_PSYCHIC;
		}
		else if ( cg.weaponSelect == WP_PSYCHIC )
		{
			cg.weaponSelect = WP_SONIC;
		}
		else if ( cg.weaponSelect == WP_SONIC )
		{
			cg.weaponSelect = WP_EARTH;
		}
		else if ( cg.weaponSelect == WP_EARTH )
		{
			cg.weaponSelect = WP_WATER;
		}
		else if ( cg.weaponSelect == WP_WATER )
		{
			cg.weaponSelect = WP_AIR;
		}
		else if ( cg.weaponSelect == WP_AIR )
		{
			cg.weaponSelect = WP_FLAME;
		}
		else if ( cg.weaponSelect == WP_FLAME )
		{ 
			cg.weaponSelect = WP_FIRE;
		}
		else if ( cg.weaponSelect == WP_FIRE )
		{
			cg.weaponSelect = WP_LIGHTNING;
		}
		else if ( cg.weaponSelect == WP_LIGHTNING )
		{
			cg.weaponSelect = WP_LIGHT;
		}
		else if ( cg.weaponSelect == WP_LIGHT )
		{
			cg.weaponSelect = WP_DARK;
		}
		else if ( cg.weaponSelect == WP_DARK )
		{
			// HACKED
			cg.weaponSelect = FIRST_WEAPON;
		}
		// WARRIOR
		else if ( cg.weaponSelect == WP_SCEPTER )
		{
			cg.weaponSelect = WP_HOOKER;
		}
		else if ( cg.weaponSelect == WP_HOOKER )
		{
			cg.weaponSelect = WP_CANNON;
		}
		else if ( cg.weaponSelect == WP_CANNON )
		{
			cg.weaponSelect = WP_TUSKEN_RIFLE;
		}
		else if ( cg.weaponSelect == WP_TUSKEN_RIFLE )
		{
			cg.weaponSelect = WP_TUSKEN_STAFF;
		}
		else if ( cg.weaponSelect == WP_TUSKEN_STAFF )
		{
			cg.weaponSelect = WP_STUN_BATON;
		}
		else if ( cg.weaponSelect == WP_STUN_BATON)
		{
			cg.weaponSelect = WP_THROWABLE;
		}
		else if ( cg.weaponSelect == WP_THROWABLE )
		{
			cg.weaponSelect = WP_SLING;
		}
		else if ( cg.weaponSelect == WP_SLING )
		{
			cg.weaponSelect = WP_CROSSBOW;
		}
		else if ( cg.weaponSelect == WP_CROSSBOW )
		{
			cg.weaponSelect = WP_BOW;
		}
		else if ( cg.weaponSelect == WP_BOW )
		{
			cg.weaponSelect = WP_MELEE;
		}
		else if ( cg.weaponSelect == WP_MELEE )
		{
			// HACKED 
			cg.weaponSelect = firstWeapon;
		}
		else if ( cg.weaponSelect == WP_DET_PACK )
		{
			cg.weaponSelect = WP_TRIP_MINE;
		}
			else if ( cg.weaponSelect == WP_TRIP_MINE )
		{
			cg.weaponSelect = WP_THERMAL;
		}
		else if ( cg.weaponSelect == WP_THERMAL )
		{
			cg.weaponSelect = WP_DET_PACK;
		}
		else if ( cg.weaponSelect == WP_CONCUSSION )
		{
			cg.weaponSelect = WP_DEMP2;
		}
		else if ( cg.weaponSelect == WP_DEMP2 )
		{
			cg.weaponSelect = WP_NOGHRI_STICK;
		}
		else if ( cg.weaponSelect == WP_NOGHRI_STICK )
		{
			cg.weaponSelect = WP_ROCKET_LAUNCHER;
		}
		else if ( cg.weaponSelect == WP_ROCKET_LAUNCHER)
		{
			cg.weaponSelect = WP_BOWCASTER;
		}
		else if ( cg.weaponSelect == WP_BOWCASTER )
		{
			cg.weaponSelect = WP_FLECHETTE;
		}
		else if ( cg.weaponSelect == WP_FLECHETTE )
		{
			cg.weaponSelect = WP_REPEATER;
		}
		else if ( cg.weaponSelect == WP_REPEATER )
		{
			cg.weaponSelect = WP_DISRUPTOR;
		}
		else if ( cg.weaponSelect == WP_DISRUPTOR )
		{
			cg.weaponSelect = WP_BLASTER;
		}
		else if ( cg.weaponSelect == WP_BLASTER )
		{
			cg.weaponSelect = WP_BLASTER_PISTOL;
		}
		else if ( cg.weaponSelect == WP_BLASTER_PISTOL )
		{// HACKED 
			cg.weaponSelect = firstWeapon;
		}
		else
		{
			cg.weaponSelect--;
		}


		//if ( cg.weaponSelect < firstWeapon || cg.weaponSelect > MAX_PLAYER_WEAPONS) { 
			//cg.weaponSelect = MAX_PLAYER_WEAPONS;
		if ( cg.weaponSelect < firstWeapon || cg.weaponSelect >= WP_NUM_WEAPONS ) {
			cg.weaponSelect = WP_NUM_WEAPONS;
		}
		
		if ( CG_WeaponSelectable( cg.weaponSelect, original, qfalse ) ) 
		{
			SetWeaponSelectTime();
//			cg.weaponSelectTime = cg.time;
			return;
		}
	}

	cg.weaponSelect = original;
}



/*
void CG_ChangeWeapon( int num )

  Meant to be called from the normal game, so checks the game-side weapon inventory data
*/
void CG_ChangeWeapon( int num )
{
	gentity_t	*player = &g_entities[0];

	if ( num < WP_NONE || num >= WP_NUM_WEAPONS ) 
	{
		return;
	}

	if( player->flags & FL_LOCK_PLAYER_WEAPONS )
	{
		CG_PlayerLockedWeaponSpeech( qfalse );
		return;
	}

	//if ( player->client != NULL && !(player->client->ps.stats[STAT_WEAPONS] & ( 1 << num )) ) 
	if ( player->client != NULL && !(player->client->ps.weapons[num]) )
	{
		return;		// don't have the weapon
	}

	// because we don't have an empty hand model for the thermal, don't allow selecting that weapon if it has no ammo
	if ( num == WP_THERMAL )
	{
		if ( cg.snap && cg.snap->ps.ammo[AMMO_THERMAL] <= 0 ) 
		{
			return;
		}
	}
	
	// because we don't have an empty hand model for the thermal, don't allow selecting that weapon if it has no ammo
	if ( num == WP_TRIP_MINE )
	{
		if ( cg.snap && cg.snap->ps.ammo[AMMO_TRIPMINE] <= 0 ) 
		{
			return;
		}
	}

	SetWeaponSelectTime();
//	cg.weaponSelectTime = cg.time;
	cg.weaponSelect = num;
}

/*
===============
CG_Weapon_f
===============
*/
void CG_Weapon_f( void ) 
{
	int		num;

	if ( cg.weaponSelectTime + 200 > cg.time )
	{
		return;
	}

	if ( !cg.snap ) {
		return;
	}
	/*
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}
	*/

	if( g_entities[0].flags & FL_LOCK_PLAYER_WEAPONS )
	{
		CG_PlayerLockedWeaponSpeech( qfalse );
		return;
	}

	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_ATST )
	{
		CG_ToggleATSTWeapon();
		return;
	}

	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_FIGHTER )
	{
		CG_ToggleFighterWeapon();
		return;
	}

	// WOLF AND BAT FORM CAN USE ONLY SABER AND MELEE AND NONE WEAPONS! 
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFFORM ||		
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLWOLF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		  
		 // BAT FORM  
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATFORM ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLBAT || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF3 )
	{
		CG_ToggleFormsWeapon();
		return;
	}
	// MIST FORM 
	if(	g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST3 )

	{
		CG_ToggleMistWeapon(); 
		return; 
	}

	if ( cg.snap->ps.eFlags & EF_LOCKED_TO_WEAPON )
	{
		// can't do any sort of weapon switching when in the emplaced gun
		return;
	}

	if ( cg.snap->ps.viewEntity )
	{
		// yeah, probably need a better check here
		if ( g_entities[cg.snap->ps.viewEntity].client && ( g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_R5D2 
				|| g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_R2D2 
				|| g_entities[cg.snap->ps.viewEntity].client->NPC_class == CLASS_MOUSE ))
		{
			return;
		}
	}

	num = atoi( CG_Argv( 1 ) );

	if ( num < WP_NONE || num >= WP_NUM_WEAPONS ) {
		return;
	}

	if ( num == WP_SABER )
	{//lightsaber
		//if ( ! ( cg.snap->ps.stats[STAT_WEAPONS] & ( 1 << num ) ) )
		if ( ! ( cg.snap->ps.weapons[num] ) )
		{//don't have saber, try stun baton
			//if ( ( cg.snap->ps.stats[STAT_WEAPONS] & ( 1 << WP_STUN_BATON ) ) )
			if ( ( cg.snap->ps.weapons[WP_STUN_BATON] ) )
			{
				num = WP_STUN_BATON;
			}
			else
			{//don't have stun baton, use fists
				num = WP_MELEE;
			}
		}
		else if ( num == cg.snap->ps.weapon )
		{//already have it up, let's try to toggle it
			if ( !in_camera )
			{//player can't activate/deactivate saber when in a cinematic
				//can't toggle it if not holding it and not controlling it or dead
				if ( cg.predicted_player_state.stats[STAT_HEALTH] > 0 && (!cg_entities[0].gent->client->ps.saberInFlight || (&g_entities[cg_entities[0].gent->client->ps.saberEntityNum] != NULL && g_entities[cg_entities[0].gent->client->ps.saberEntityNum].s.pos.trType == TR_LINEAR) ) )
				{//it's either in-hand or it's under telekinetic control
					if ( cg_entities[0].gent->client->ps.SaberActive() )
					{//a saber is on
						if ( cg_entities[0].gent->client->ps.dualSabers 
							&& cg_entities[0].gent->client->ps.saber[1].Active() )
						{//2nd saber is on, turn it off, too
							cg_entities[0].gent->client->ps.saber[1].Deactivate();
						}
						cg_entities[0].gent->client->ps.saber[0].Deactivate();
						if ( cg_entities[0].gent->client->ps.saberInFlight )
						{//play it on the saber
							cgi_S_UpdateEntityPosition( cg_entities[0].gent->client->ps.saberEntityNum, g_entities[cg_entities[0].gent->client->ps.saberEntityNum].currentOrigin );
							cgi_S_StartSound (NULL, cg_entities[0].gent->client->ps.saberEntityNum, CHAN_AUTO, cgs.sound_precache[cg_entities[0].gent->client->ps.saber[0].soundOff] );
						}
						else
						{
							cgi_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.sound_precache[cg_entities[0].gent->client->ps.saber[0].soundOff] );
						}
					}
					else
					{//turn them both on
						cg_entities[0].gent->client->ps.SaberActivate();
					}
				}
			}
		}
	}
	// ROLL GRENADES 
	else if ( num >= WP_THERMAL && num <= WP_DET_PACK ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_THERMAL && cg.snap->ps.weapon <= WP_DET_PACK )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_THERMAL;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_DET_PACK )
			{
				weap = WP_THERMAL;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_DET_PACK )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}
	// ROLL FLAME
	else if ( num >= WP_FIRE && num <= WP_FLAME ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_FIRE && cg.snap->ps.weapon <= WP_FLAME )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_FIRE;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_FLAME )
			{
				weap = WP_FIRE;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_FLAME )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}
	// ROLL LIGHTNING
	else if ( num >= WP_LIGHT && num <= WP_LIGHTNING ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_LIGHT && cg.snap->ps.weapon <= WP_LIGHTNING )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_LIGHT;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_LIGHTNING )
			{
				weap = WP_LIGHT;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_LIGHTNING )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}
	// ROLL METAL
	else if ( num >= WP_WOOD && num <= WP_METAL ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_WOOD && cg.snap->ps.weapon <= WP_METAL )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_WOOD;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_METAL )
			{
				weap = WP_WOOD;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_METAL )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}
	// ROLL POISON
	else if ( num >= WP_SPIRIT && num <= WP_POISON ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_SPIRIT && cg.snap->ps.weapon <= WP_POISON )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_SPIRIT;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_POISON )
			{
				weap = WP_SPIRIT;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_POISON )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}
	// ROLL NECRO
	else if ( num >= WP_HOLY && num <= WP_NECRO ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_HOLY && cg.snap->ps.weapon <= WP_NECRO )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_HOLY;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_NECRO )
			{
				weap = WP_HOLY;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_NECRO )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}

	// ROLL FIGHTER
	else if ( num >= WP_DEFENSE && num <= WP_POSITRON ) // these weapons cycle
	{
		int weap, i = 0;

		if ( cg.snap->ps.weapon >= WP_DEFENSE && cg.snap->ps.weapon <= WP_POSITRON )
		{
			// already in cycle range so start with next cycle item
			weap = cg.snap->ps.weapon + 1;
		}
		else
		{
			// not in cycle range, so start with thermal detonator
			weap = WP_DEFENSE;
		}

		// prevent an endless loop
		while ( i <= 4 )
		{
			if ( weap > WP_POSITRON )
			{
				weap = WP_DEFENSE;
			}

			if ( cg.snap->ps.ammo[weaponData[weap].ammoIndex] > 0 || weap == WP_POSITRON )
			{
				if ( CG_WeaponSelectable( weap, cg.snap->ps.weapon, qfalse ) ) 
				{	
					num = weap;
					break;
				}
			}

			weap++;
			i++;
		}
	}

	if (!CG_WeaponSelectable(num, cg.snap->ps.weapon, qfalse))
	{
		return;
	}

	SetWeaponSelectTime();
//	cg.weaponSelectTime = cg.time;
	cg.weaponSelect = num;
}

/*
===================
CG_OutOfAmmoChange

The current weapon has just run out of ammo
===================
*/
void CG_OutOfAmmoChange( void ) {
	int		i;
	int		original;

	if ( cg.weaponSelectTime + 200 > cg.time )
		return;

	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_ATST )
	{
		CG_ToggleATSTWeapon();
		return;
	}
	
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_FIGHTER )
	{
		CG_ToggleFighterWeapon();
		return;
	}
	// WOLF AND BAT FORM CAN USE ONLY SABER AND MELEE AND NONE WEAPONS! 
	if( g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFFORM ||		
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLWOLF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 
		 // BAT FORM  
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATFORM ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLBAT || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_BATVAMPIRE_ELF3 )
	{
		CG_ToggleFormsWeapon();
		return;
	}
	// MIST FORM 
	if(	g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTKAIN3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVORADOR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTJANOS || 
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTNECROMANCER ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_HELLMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_MISTVAMPIRE_ELF3 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST2 ||
		 g_entities[0].client && g_entities[0].client->NPC_class == CLASS_NAERYANMIST3 )

	{
		CG_ToggleMistWeapon(); 
		return; 
	}

	original = cg.weaponSelect;

	for ( i = WP_ROCKET_LAUNCHER; i > 0 ; i-- ) 
	{	
		// We don't want the emplaced, melee, or explosive devices here
		if ( original != i && CG_WeaponSelectable( i, original, qfalse ) )
		{
			SetWeaponSelectTime();
			cg.weaponSelect = i;
			break;
		}
	}

	if ( cg_autoswitch.integer != 1 )
	{
		// didn't have that, so try these. Start with thermal...
		for ( i = WP_THERMAL; i <= WP_DET_PACK; i++ ) 
		{	
			// We don't want the emplaced, or melee here
			if ( original != i && CG_WeaponSelectable( i, original, qfalse ) )
			{
				if ( i == WP_DET_PACK && cg.snap->ps.ammo[weaponData[i].ammoIndex] <= 0 )
				{
					// crap, no point in switching to this
				}
				else
				{
					SetWeaponSelectTime();
					cg.weaponSelect = i;
					break;
				}
			}
		}
	}

	// try stun baton as a last ditch effort
	if ( CG_WeaponSelectable( WP_STUN_BATON, original, qfalse ))
	{
		SetWeaponSelectTime();
		cg.weaponSelect = WP_STUN_BATON;
	}
}



/*
===================================================================================================

WEAPON EVENTS

===================================================================================================
*/

/*
================
CG_FireWeapon

Caused by an EV_FIRE_WEAPON event
================
*/
void CG_FireWeapon( centity_t *cent, qboolean alt_fire ) 
{
	entityState_t *ent;
	weaponInfo_t	*weap;

	ent = &cent->currentState;
	if ( ent->weapon == WP_NONE ) {
		return;
	}
	if ( ent->weapon >= WP_NUM_WEAPONS ) {
		CG_Error( "CG_FireWeapon: ent->weapon >= WP_NUM_WEAPONS" );
		return;
	}
	//if ( ent->weapon == WP_TUSKEN_RIFLE && cent->gent->client) 
	if ( (ent->weapon == WP_TUSKEN_RIFLE || ent->weapon == WP_NOGHRI_STICK) && cent->gent->client)
	{ 	
		if (cent->gent->client->ps.torsoAnim==BOTH_TUSKENATTACK1 ||
		cent->gent->client->ps.torsoAnim==BOTH_TUSKENATTACK2 ||
		cent->gent->client->ps.torsoAnim==BOTH_TUSKENATTACK3 ||
		cent->gent->client->ps.torsoAnim==BOTH_TUSKENLUNGE1)
		{
			return;
		}
	}

	weap = &cg_weapons[ ent->weapon ];

	// mark the entity as muzzle flashing, so when it is added it will
	// append the flash to the weapon model
	cent->muzzleFlashTime = cg.time;
	cent->altFire = alt_fire;

	// lightning type guns only does this this on initial press
	if ( ent->weapon == WP_SABER )
	{
		if ( cent->pe.lightningFiring ) 
		{
/*			if ( ent->weapon == WP_DREADNOUGHT )
			{
				cgi_FF_EnsureFX( fffx_Laser3 );
			}
*/
			return;
		}
	}

	// Do overcharge sound that get's added to the top
/*	if (( ent->powerups & ( 1<<PW_WEAPON_OVERCHARGE )))
	{
		if ( alt_fire )
		{
			switch( ent->weapon )
			{
			case WP_THERMAL:
			case WP_DET_PACK:
			case WP_TRIP_MINE:
			case WP_ROCKET_LAUNCHER:
			case WP_FLECHETTE:
				// these weapon fires don't overcharge
				break;

			case WP_BLASTER:
				cgi_S_StartSound( NULL, ent->number, CHAN_AUTO, cgs.media.overchargeFastSound );
				break;

			default:
				cgi_S_StartSound( NULL, ent->number, CHAN_AUTO, cgs.media.overchargeSlowSound );
				break;
			}
		}
		else
		{
			switch( ent->weapon )
			{
			case WP_THERMAL:
			case WP_DET_PACK:
			case WP_TRIP_MINE:
			case WP_ROCKET_LAUNCHER:
				// these weapon fires don't overcharge
				break;

			case WP_REPEATER:
				cgi_S_StartSound( NULL, ent->number, CHAN_AUTO, cgs.media.overchargeFastSound );
				break;

			default:
				cgi_S_StartSound( NULL, ent->number, CHAN_AUTO, cgs.media.overchargeSlowSound );
				break;
			}
		}
	}*/
}

/*
=================
CG_BounceEffect

Caused by an EV_BOUNCE | EV_BOUNCE_HALF event
=================
*/
void CG_BounceEffect( centity_t *cent, int weapon, vec3_t origin, vec3_t normal )
{
	switch( weapon )
	{
	case WP_THERMAL:
		if ( rand() & 1 ) {
			cgi_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.grenadeBounce1 );
		} else {
			cgi_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.grenadeBounce2 );
		}
		break;

	case WP_BOWCASTER:
		//theFxScheduler.PlayEffect( cgs.effects.bowcasterBounceEffect, origin, normal );
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 {
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_LIGHTREAVER: 
			case CLASS_LYCAN: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_SABER_DROID:
			case CLASS_ASSASSIN_DROID: 
			theFxScheduler.PlayEffect( cgs.effects.shockBounceEffect, origin, normal );
			break;
			case CLASS_MORGANKATARN:
			case CLASS_GOLEM_GOLD: 
			case CLASS_PROTOCOL: 
			case CLASS_JAN: 
			case CLASS_CLAW: 
			theFxScheduler.PlayEffect( cgs.effects.sunBounceEffect, origin, normal );
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA:
			case CLASS_MONMOTHA:
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_LIZARD:
			case CLASS_SWAMPTROOPER:
			case CLASS_REBEL:
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_WAMPA: 
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_TURELIM:
			case CLASS_VAMPIRE:
			case CLASS_VAMPIRE_DEMON:
			case CLASS_VAMPIRE_GHOST: 
			case CLASS_KAIN:
			case CLASS_KAIN2: 
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR:
			case CLASS_JANOS:
			case CLASS_NECROMANCER: 
			case CLASS_VAMPIRE_QUEEN:
			case CLASS_VAMPIRE_DARK: 
			case CLASS_VAMPIRE_NECRO: 
			case CLASS_VAMPIRE_NECRO2:
			case CLASS_VAMPIRE_FEAR: 
			case CLASS_VAMPIRE_FEAR2:  
			case CLASS_VAMPIRE_BLOOD:
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN: 
			case CLASS_VAMPIRE_ELF: 
			case CLASS_VAMPIRE_ELF2:
			case CLASS_VAMPIRE_ELF3:
			case CLASS_VAMPIRE_WATER: 
			case CLASS_VAMPIRE_FROST:  
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE: 
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_ZOMBIE:
			case CLASS_VAMPIRE_SARAFAN: 
			case CLASS_AIR: 
			case CLASS_GOLEM_AIR: 
			case CLASS_BIRD: 
			case CLASS_IVY: 
			theFxScheduler.PlayEffect( cgs.effects.ebBounceEffect, origin, normal );
			break; 
			case CLASS_GALAK:
			case CLASS_RODIAN:
			case CLASS_MARK1:
			case CLASS_MARK2: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			theFxScheduler.PlayEffect( cgs.effects.lightknifeBounceEffect, origin, normal );
			break;
			case CLASS_TAVION:
			case CLASS_DESANN:
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_SAND_CREATURE:
			case CLASS_ATST:
			case CLASS_SENTRY:
			case CLASS_MOUSE: 
			case CLASS_TENTACLE:  
			case CLASS_ELDERGOD: 
			case CLASS_SONIC: 
			case CLASS_GOLEM_SONIC: 
			case CLASS_PROPHET: 
			case CLASS_DECEIVER: 
			case CLASS_FLIER2: 
			case CLASS_BEAST:  
			case CLASS_SHARK:
			theFxScheduler.PlayEffect( cgs.effects.madBounceEffect, origin, normal );
			break;
			case CLASS_ARIEL: 
			case CLASS_MINEMONSTER:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_FIRE: 
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_SABOTEUR: 
			case CLASS_MURJJ:
			case CLASS_NECRO: 
			case CLASS_THRALL: 
			case CLASS_IMPERIAL: 
			case CLASS_CENOBITA: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_POISON: 
			case CLASS_GOLEM_DARK: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_NECRO:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_GOLEM_VOID:
			case CLASS_GOLEM_POISON: 
			case CLASS_GOLEM_LAVA:
			case CLASS_POLTER:
			theFxScheduler.PlayEffect( cgs.effects.lightskullBounceEffect, origin, normal );
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER:
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_GALAKMECH: 
			case CLASS_PRISONER: 
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER: 
			case CLASS_REELO: 
			case CLASS_REMOTE: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP: 
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_GONK: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID:
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:			
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER:		 
			case CLASS_LIGHT: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_CRYSTAL: 
			case CLASS_EARTH: 
			case CLASS_HOLY:  
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:			
			case CLASS_GOLEM:
			case CLASS_GOLEM_HOLY: 
			theFxScheduler.PlayEffect( cgs.effects.bowcasterBounceEffect, origin, normal );
			break; 
		default:
			break;
			}
		}
		break;

	case WP_FLECHETTE:
		// theFxScheduler.PlayEffect( "flechette/ricochet", origin, normal );

		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 		{
				switch ( cent->gent->owner->client->NPC_class )
				{
					// FANGO
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 	
				theFxScheduler.PlayEffect( "flechette2/ricochet", origin, normal );
				break;
				// PIETRA 
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( "flechette3/ricochet", origin, normal );
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( "acid/ricochet", origin, normal );
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				theFxScheduler.PlayEffect( "ivy/ricochet", origin, normal );
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( "ivy2/ricochet", origin, normal );
				break;
				// THORN 
				case CLASS_SWAMP: 
				theFxScheduler.PlayEffect( "thorn/ricochet", origin, normal );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				theFxScheduler.PlayEffect( "spectral/ricochet", origin, normal );
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( "stasis/ricochet", origin, normal );
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
				theFxScheduler.PlayEffect( "dust/ricochet", origin, normal );
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				theFxScheduler.PlayEffect( "ash/ricochet", origin, normal );
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				theFxScheduler.PlayEffect( "gcrystal/ricochet", origin, normal );
				break;
				// DEFAULT 
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				theFxScheduler.PlayEffect( "flechette/ricochet", origin, normal );
				break; 				
			default:
				break;
				}
			}
		break;

	case WP_CANNON:
		theFxScheduler.PlayEffect( "cannon/ricochet", origin, normal );
		break;

	default:
		if ( rand() & 1 ) {
			cgi_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.grenadeBounce1 );
		} else {
			cgi_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.grenadeBounce2 );
		}
		break;
	}
}

//----------------------------------------------------------------------
void CG_MissileStick( centity_t *cent, int weapon, vec3_t position )
//----------------------------------------------------------------------
{
	sfxHandle_t snd = 0;

	switch( weapon )
	{
	case WP_FLECHETTE:
		snd = cgs.media.flechetteStickSound;
		break;

	case WP_DET_PACK:
		snd = cgs.media.detPackStickSound;
		break;

	case WP_TRIP_MINE:
		snd = cgs.media.tripMineStickSound;
		break;

	case WP_CANNON:
		snd = cgs.media.cannonStickSound;
		break;
	}

	if ( snd )
	{
		cgi_S_StartSound( NULL, cent->currentState.number, CHAN_AUTO, snd );
	}
}

/*
=================
CG_MissileHitWall

Caused by an EV_MISSILE_MISS event, or directly by local bullet tracing
=================
*/
void CG_MissileHitWall( centity_t *cent, int weapon, vec3_t origin, vec3_t dir, qboolean altFire ) 
{
	int parm;

	switch( weapon )
	{
	case WP_BRYAR_PISTOL:
		if ( altFire )
		{
			parm = 0;

			if ( cent->gent )
			{
				parm += cent->gent->count;
			}

			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_MENTALIST: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				FX_Bow12AltHitWall( origin, dir, parm );
				break;
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				FX_Bow4AltHitWall( origin, dir, parm );
				break; 
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				FX_Bow6AltHitWall( origin, dir, parm );
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_Bow10AltHitWall( origin, dir, parm );
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				FX_Bow5AltHitWall( origin, dir, parm );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				FX_BowAltHitWall( origin, dir, parm );
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				FX_Bow7AltHitWall( origin, dir, parm );
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				FX_Bow1AltHitWall( origin, dir, parm );
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				FX_Bow0AltHitWall( origin, dir, parm );
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				FX_Bow8AltHitWall( origin, dir, parm );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				FX_Bow11AltHitWall( origin, dir, parm );
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				FX_Bow9AltHitWall( origin, dir, parm );
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				FX_Bow3AltHitWall( origin, dir, parm );
				break;  
			default:
				break;
				}
			}

			//FX_BowAltHitWall( origin, dir, parm );
		}
		else
		{

			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_MENTALIST: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				FX_Bow12HitWall( origin, dir );
				break;
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				FX_Bow4HitWall( origin, dir );
				break; 
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				FX_Bow6HitWall( origin, dir );
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_Bow10HitWall( origin, dir );
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				FX_Bow5HitWall( origin, dir );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				FX_BowHitWall( origin, dir );
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				FX_Bow7HitWall( origin, dir );
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				FX_Bow1HitWall( origin, dir );
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				FX_Bow0HitWall( origin, dir );
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				FX_Bow8HitWall( origin, dir );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				FX_Bow11HitWall( origin, dir );
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				FX_Bow9HitWall( origin, dir );
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				FX_Bow3HitWall( origin, dir );
				break;  
			default:
				break;
				}
			}

			//FX_BowHitWall( origin, dir );
		}
		break;
	case WP_BLASTER_PISTOL:
		if ( altFire )
		{
			parm = 0;

			if ( cent->gent )
			{
				parm += cent->gent->count;
			}

			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		   {
			    if ( cent->gent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect )
				{
					// if rayshoot is enabled effect is played by other place 
					if ( cent->gent->owner->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT2 ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT3 ) 
					{
						gi.Printf( S_COLOR_RED "i am NOT playing wall effect here! Read from WP_disruptor.cpp\n" , cent );
					}	
					else 
					{
						//gi.Printf( S_COLOR_GREEN "Customize Alt Wall Impact Effect from *.GUN File! None = gun/altwall_impact" , cent );
						G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect, origin, dir );  
						//theFxScheduler.PlayEffect( cent->gent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect, origin, dir );
					}
				}

				
			/*switch ( cent->gent->owner->client->NPC_class )
			{
				// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				// animali 
				case CLASS_BEAST: // animals 
				case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/wall_impact" , origin, dir  );  
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				// Umani: lanciano pugnale 
				// Humanity 
				case CLASS_PRISONER:	// HUMANS 
				case CLASS_RODIAN: // VAMPIRESLAYERS AND IRONGUARD 
				case CLASS_HUNTER: // Ironguard // Altfire,
				case CLASS_WARRIOR: // Generic Warrior 
		    	case CLASS_ARCHER: // Generic Archer Human sniper elite class. 
				// armature stregate
				case CLASS_MARK1:			// WITCH ARMOR BOSS CLASS 
				case CLASS_MARK2:			// WITCH ARMOR CLASS				
				case CLASS_METAL: // elementale del Metallo
				case CLASS_GOLEM_METAL: // golem del metallo 
				G_PlayEffect( "knife/wall_impact" , origin, dir  );  
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/wall_impact" , origin, dir  );  
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/wall_impact" , origin, dir  );  
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/wall_impact" , origin, dir  );  
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				// MIETITORI: Proiettile impregnato di Tenebra
				// REAVERS OF ALLIANCE 
				case CLASS_JEDI:				// REAPER CLASS 
				case CLASS_REAVER: // STANDARD REAVER CLASSES 
				case CLASS_LIGHTREAVER:// light reaver class 
				case CLASS_DRUIDREAVER: // druid reaver class 
				case CLASS_AIRREAVER: // air reaver class act like rebel class 
				case CLASS_NAERYANMIST:	
				case CLASS_NAERYANMIST2: 
				case CLASS_NAERYANMIST3: 
				case CLASS_ICEREAVER: // ice reaver class 
				case CLASS_EARTHREAVER: // Earth reaver class. 
		 		// ombre ed elementali d'ombra
				case CLASS_DARK: // darkness elite class
				case CLASS_GOLEM_DARK: // golem tenebroso
				case CLASS_SABOTEUR: // soldato ombra 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/wall_impact" , origin, dir  );  
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/wall_impact" , origin, dir  );  
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/wall_impact" , origin, dir  );  
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_RAZIEL2:// life reaver class 
					G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/wall_impact" , origin, dir  );  
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/wall_impact" , origin, dir  );  
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/wall_impact" , origin, dir  );  
				break;
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_REBORN:			// VAMPIRES 
				case CLASS_VAMPIRE: // act like reborns. alliance standard vampire classes  
				case CLASS_VAMPIRE_DARK: // vae
				case CLASS_VAMPIRE_BLOOD: // air vampire class - acrobatic bleeding vampire
		   		case CLASS_VAMPIRE_WATER: // Shaar MNaik evoluted water vampire class with water resistance. 
				case CLASS_VAMPIRE_ELF: // vampira elfa 
				case CLASS_VAMPIRE_ELF2: // Elf Vampire blue armor 
				case CLASS_VAMPIRE_SOUND: // bard vampire class. 
				case CLASS_VAMPIRE_BOUNTY: // Half vampire - dumahim
				case CLASS_VAMPIRE_ARCHER: // Archer - light vampire
				case CLASS_UMAH: // Vorador daughter 
				case CLASS_KAIN2: // Young Kain 
				case CLASS_VAMPIRE_NECRO: // Soul Liutenant  
				case CLASS_VAMPIRE_ZOMBIE: // half vampire - melchiahim 
				case CLASS_VAMPIRE_DEMON:// Samah'el - turelim boss classes 
				case CLASS_VAMPIRE_LYCAN: // werewolf Vampire Class. 
				case CLASS_VAMPIRE_SARAFAN:  // joachim 
				case CLASS_VAMPIRE_FEAR: // he's a trickster fire vampire class - vampire of fear. 
				case CLASS_HALFVAMPIRE: // Half human \ sarafan vampire 
				// Vampiri Corrotti 
				// MELCHIAHIM
				case CLASS_TUSKEN:	// MELCHIAHIM youngs
				case CLASS_MELC: // dive like sand creature, fight like a zombie, eat like a vampire. 
				// DUMAHIM
				case CLASS_WEEQUAY:	// Dumahim, young warrior vampires 
				case CLASS_DUMAHIM: // dumahim expert vampires 
				case CLASS_SNEAKER: // dumahim assassins 
				// RAHABIM
				case CLASS_SWAMPTROOPER:		// YOUNG RAHABIM VAMPIRES 
				case CLASS_ALORA:			// RAHABIM SPECIAL CLASS - Sirens
				case CLASS_RAHABIM:				// immune to water, weak to fire, deadly to light, act like swamptrooper class
				// TURELIM
				// Vampires e Reapers 
				case CLASS_GRAN:	// young TURELIM VAMPIRES
				case CLASS_TURELIM:// adult turelim. act like gran class, weak to sound, resistance to fire- 
				case CLASS_TYRANT: // Turelim elite fight class 
				// Razielhim
				case CLASS_FLIER2: // razielhim young vampires
				case CLASS_RAZIELHIM: // razielhim vampires
				case CLASS_SENTINEL: // razielhim elite class
	     		G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				break;
				// Old Vampires and guardians deals more damage. 
				case CLASS_NECROMANCER: // Vampire Necromancer
				case CLASS_VAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_VAMPIRE_NECRO2:// Deathmask with full powers 
				case CLASS_KYLE:	// VAMPIRE BOSS CLASS (KAIN)
				case CLASS_KAIN:// Kain with full powers
				case CLASS_VAMPIRE_ELF3: // Elf Vampire blue armor rainbow lion class - dimension guardian 
				// Conflict Guardians 
				case CLASS_VAMPIRE_FROST: // for Kainh conflict guardian with frost powers. 
				case CLASS_VAMPIRE_FEAR2: // classe phobos knight of fear infernale 
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/wall_impact" , origin, dir  );  
				break;
				break;
				// WOLF, BAT AND MIST FORM DO NOT SHOOT MISSILE. 
				case CLASS_WOLFFORM: // for vampires mutated into wolves
				case CLASS_BATFORM: // for vampires mutated into bats 
				case CLASS_WOLFKAIN2:// Kain BO2
				case CLASS_BATKAIN2: // Kain bo2 bat form 
				case CLASS_BATVAMPIRE_WATER: // Shaar Naik Bat form
				case CLASS_BATVAMPIRE_ELF: // Elf Vampire bat form 
				case CLASS_BATVAMPIRE_ELF2: // Elf Vampire khyroptera form 
				case CLASS_WOLFVAMPIRE_ELF: // evoluted elf vampire class with earth connection. pretty strong. 
				case CLASS_LIONVAMPIRE_ELF: // Elf Vampire blue armor 
				case CLASS_HELLWOLF: // Samah'el metalupo 
				case CLASS_HELLBAT: // Samah'el Colibri
				case CLASS_WOLFVAMPIRE_LYCAN: // werewolf Vampire Class. 
		     	case CLASS_WOLFVAMPIRE_FEAR: // fire vampire class - vampire of fear. 
				case CLASS_BATVAMPIRE_FEAR: // Phobos bat form 
				case CLASS_WOLFHALFVAMPIRE: // Half human \ sarafan vampire 
		   		case CLASS_WOLFNECROMANCER: // Vampire Necromancer 
				case CLASS_WOLFVAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_BATNECROMANCER: // Soul Bat form 
				case CLASS_BATVAMPIRE_QUEEN: // Sheer Bat form 
				case CLASS_WOLFKAIN:// Kain with full powers
				case CLASS_WOLFKAIN3:// Kain Sion of balance 
				case CLASS_WOLFVORADOR: // vorador class 
				case CLASS_WOLFJANOS: // janos audron class 
				case CLASS_BATKAIN: // Kain Bat form
				case CLASS_BATVORADOR: // Vorador Bat form 
				case CLASS_BATKAIN3: // kain prophecy bat form 
				case CLASS_BATVAMPIRE_ELF3: // Elf Vampire khyroptera form full power. 
				case CLASS_LIONVAMPIRE_ELF2: // Elf Vampire blue armor lion Elf Vampire with rainbows - Dimensione Guardian
				case CLASS_WOLFVAMPIRE_FEAR2: // classe phobos knight of fear infernale, Conflict Guardian
				case CLASS_BATVAMPIRE_FEAR2: // Phobos demonic bat form 
				case CLASS_BATVAMPIRE_FROST: // Kainh bat form 
				// MIST FORM // - NON ATTACCANO
				case CLASS_MISTFORM: // for vampires mutated into fog 
				case CLASS_MISTVAMPIRE_DARK: // Mist vae 
				case CLASS_MISTKAIN2:// Mist Kain BO2 
				case CLASS_MISTVAMPIRE_WATER:// shaar naik mist form 
				case CLASS_MISTVAMPIRE_BLOOD: // Mist osil
				case CLASS_MISTVAMPIRE_ELF:// Elf Vampire basic mist form
				case CLASS_MISTVAMPIRE_ELF2: // Elf Vampire Iridu mist form
				case CLASS_HELLMIST: // samah'el mist form 
		 		case CLASS_MISTVAMPIRE_SARAFAN:  // joachim 
				case CLASS_MISTVAMPIRE_FEAR: // mist phobos 
		 		case CLASS_MISTNECROMANCER: // Mist soul 
				case CLASS_MISTVAMPIRE_QUEEN: // Mist sheer 
		  		case CLASS_MISTKAIN:// Mist default kain 
				case CLASS_MISTVORADOR: // Mist vorador 
				case CLASS_MISTJANOS: // Mist janos audron 
				case CLASS_MISTKAIN3: // Mist Kain prophecy
				case CLASS_MISTVAMPIRE_ELF3:// Elf Vampire paladine mist form
				case CLASS_MISTVAMPIRE_FEAR2:// Mist phobos fear knight 
				{
					return; 
				}
				break;
				// Altri Psichici
				case CLASS_MENTALIST: 
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/wall_impact" , origin, dir  );  
				break;
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/wall_impact" , origin, dir  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/wall_impact" , origin, dir  );  
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				G_PlayEffect( "time/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/wall_impact" , origin, dir  );  
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/wall_impact" , origin, dir  );  
				break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/wall_impact" , origin, dir  );  
				break;
				// ELEMENTALI 
				// i Golem di terra posson lanciar un proiettile di polvere
				case CLASS_GONK:				// GOLEM AND EARTH ELEMENTAL CREATURES 
				case CLASS_EARTH:// earth elemental class 
				case CLASS_GOLEM: // golem della terra 
				case CLASS_CRYSTAL: // Elementali del Cristallo
				case CLASS_GOLEM_CRYSTAL: // golem di cristallo 
				case CLASS_UGNAUGHT: // MUD MONSTERS, BLOBS, 
				case CLASS_MUD:// elementale di fango
				case CLASS_STONE:// elementale di pietra
				case CLASS_GOLEM_MUD: // golem di fango
				case CLASS_GOLEM_STONE: // golem di pietra
				case CLASS_DUST:
				case CLASS_GOLEM_DUST:
				G_PlayEffect( "dust/wall_impact" , origin, dir  );  
				break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/wall_impact" , origin, dir  );  
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_SONIC: // elementale sonico
				case CLASS_GOLEM_SONIC:// golem di suono
				// Asgarath con l'astrale 
				case CLASS_LANDO:	// ASTRAL CLASS 
				// I golem d'astrale 
				case CLASS_ASTRAL: // Elementali Astrali
				case CLASS_GOLEM_ASTRAL: // Golem Astrale
				case CLASS_SILENCE:
				case CLASS_GOLEM_SILENCE:
				case CLASS_HOWLER:			// SONIC CREATURE CLASS 
				G_PlayEffect( "silence/wall_impact" , origin, dir  );  
				break;
				// gli elementali di Nebbia 
				case CLASS_FOG: // elementali della Nebbia
	            case CLASS_GOLEM_FOG: // golem della nebbia 
		        case CLASS_REBEL:			// FOG CLASS 
				case CLASS_ROCKETTROOPER:	// STORM AND FLYING CREATURES 
				// I Golem d'Aria, lanceranno deboli attacchi di Vuoto.
				case CLASS_AIR: // wind fog class  
	     		case CLASS_GOLEM_AIR: // golem dell'aria 
			  	case CLASS_SABER_DROID:		// LIGHTNING GOLEM CLASS 
			    case CLASS_LIGHTNING: // Elementali del Fulmine
			    case CLASS_GOLEM_LIGHTNING: // golem del fulmine
				case CLASS_STORM:// elementale di tempesta
				case CLASS_GOLEM_STORM: // golem di tempesta
		       	G_PlayEffect( "void/wall_impact" , origin, dir  );  
				break;
				// Il golem del fuoco e i raggianti del fuoco, lanciano deboli attacchi di fumo 
				case CLASS_FIRE: // fire-warm element class 
				case CLASS_GOLEM_FIRE: // golem di fuoco
			    case CLASS_WARM: // Elementali di calore 
				case CLASS_GOLEM_WARM: // golem del silenzio
				case CLASS_HAZARD_TROOPER:	// WARM AND FIRE CREATURES 
				case CLASS_SMOKE:
				case CLASS_GOLEM_SMOKE:
				case CLASS_BESPIN_COP:
				G_PlayEffect( "smoke/wall_impact" , origin, dir  );  
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/wall_impact" , origin, dir  );  
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_WATER:// water class 
				case CLASS_GOLEM_WATER: // golem dell'acqua
				case CLASS_GOLEM_SALT: // golem di sale 
				case CLASS_SALT: // elementali del Sale
				case CLASS_GALAKMECH:		// CRYSTAL MONSTER 
				case CLASS_GOLEM_ICE: // golem di ghiaccio
				case CLASS_ICE: // elementale di ghiaccio
				case CLASS_WAVE: // elementale delle maree
				case CLASS_GOLEM_WAVE: // golem delle maree
				// Sale e Silenzio hanno la tenebra 
				G_PlayEffect( "salt/wall_impact" , origin, dir  );  
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/wall_impact" , origin, dir  );  
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				// Pistola Glifica Fumogena 
				case CLASS_IMPWORKER: // ALCHEMIST AND WIZARD CLASS 	
				case CLASS_ALCHEMIST:  // Alchemist altfire, switch,
				// costrutti tecnomagici 
				case CLASS_REMOTE:			// ARTIFACT SHOOTER ARROW TRAP
				case CLASS_SEEKER:			// SEEKER ARTIFACTS
				case CLASS_SENTRY:			// SENTRY ARTIFACT
				case CLASS_R2D2:				// MECHANICAL GOLEM CLASS 
				case CLASS_R5D2:				// MECHANICAL GOLEM CLASS 2
				G_PlayEffect( "smoker/wall_impact" , origin, dir  );  
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/wall_impact" , origin, dir  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/wall_impact" , origin, dir  );  
				break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/wall_impact" , origin, dir  );  
				break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/wall_impact" , origin, dir  );  
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/wall_impact" , origin, dir  );  
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/wall_impact" , origin, dir  );  
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/wall_impact" , origin, dir  );  
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/wall_impact" , origin, dir  );  
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/wall_impact" , origin, dir );  
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/wall_impact" , origin, dir  );  
				break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/wall_impact" , origin, dir  );  
				break;
			default:
				break;				
			}*/
		   }
		}
		else
		{
		   if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		   {

			   if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		    {
				if ( cent->gent->owner->client->gs.gun->Gun_WallImpact_Effect )
					{
						
						if ( cent->gent->owner->client->gs.gun->gunFlags&GFL_RAYSHOOT ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_RAYSHOOT2 ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_RAYSHOOT3 ) 
						 // if rayshoot is enabled do not play this effect
						{
							gi.Printf( S_COLOR_RED "i am NOT playing wall effect here! Read from WP_disruptor.cpp\n" , cent );
						}	
						else 
						{
							//gi.Printf( S_COLOR_GREEN "Customize Alt Wall Impact Effect from *.GUN File! None = gun/altwall_impact" , cent );
							G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_WallImpact_Effect, origin, dir );  
						}
				}


	/*		switch ( cent->gent->owner->client->NPC_class )
			{
								// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				// animali 
				case CLASS_BEAST: // animals 
				case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/wall_impact" , origin, dir  );  
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				// Umani: lanciano pugnale 
				// Humanity 
				case CLASS_PRISONER:	// HUMANS 
				case CLASS_RODIAN: // VAMPIRESLAYERS AND IRONGUARD 
				case CLASS_HUNTER: // Ironguard // Altfire,
				case CLASS_WARRIOR: // Generic Warrior 
		    	case CLASS_ARCHER: // Generic Archer Human sniper elite class. 
				// armature stregate
				case CLASS_MARK1:			// WITCH ARMOR BOSS CLASS 
				case CLASS_MARK2:			// WITCH ARMOR CLASS				
				case CLASS_METAL: // elementale del Metallo
				case CLASS_GOLEM_METAL: // golem del metallo 
				G_PlayEffect( "knife/wall_impact" , origin, dir  );  
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/wall_impact" , origin, dir  );  
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/wall_impact" , origin, dir  );  
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/wall_impact" , origin, dir  );  
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				// MIETITORI: Proiettile impregnato di Tenebra
				// REAVERS OF ALLIANCE 
				case CLASS_JEDI:				// REAPER CLASS 
				case CLASS_REAVER: // STANDARD REAVER CLASSES 
				case CLASS_LIGHTREAVER:// light reaver class 
				case CLASS_DRUIDREAVER: // druid reaver class 
				case CLASS_AIRREAVER: // air reaver class act like rebel class 
				case CLASS_NAERYANMIST:	
				case CLASS_NAERYANMIST2: 
				case CLASS_NAERYANMIST3: 
				case CLASS_ICEREAVER: // ice reaver class 
				case CLASS_EARTHREAVER: // Earth reaver class. 
		 		// ombre ed elementali d'ombra
				case CLASS_DARK: // darkness elite class
				case CLASS_GOLEM_DARK: // golem tenebroso
				case CLASS_SABOTEUR: // soldato ombra 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/wall_impact" , origin, dir  );  
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/wall_impact" , origin, dir  );  
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/wall_impact" , origin, dir  );  
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_RAZIEL2:// life reaver class 
					G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/wall_impact" , origin, dir  );  
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
			    G_PlayEffect( "smoke/wall_impact" , origin, dir  );  
				break;
				// Boss del Fumo, Respen con la Mieti di Fumo
					// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/wall_impact" , origin, dir  );  
				break;
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_REBORN:			// VAMPIRES 
				case CLASS_VAMPIRE: // act like reborns. alliance standard vampire classes  
				case CLASS_VAMPIRE_DARK: // vae
				case CLASS_VAMPIRE_BLOOD: // air vampire class - acrobatic bleeding vampire
		   		case CLASS_VAMPIRE_WATER: // Shaar MNaik evoluted water vampire class with water resistance. 
				case CLASS_VAMPIRE_ELF: // vampira elfa 
				case CLASS_VAMPIRE_ELF2: // Elf Vampire blue armor 
				case CLASS_VAMPIRE_SOUND: // bard vampire class. 
				case CLASS_VAMPIRE_BOUNTY: // Half vampire - dumahim
				case CLASS_VAMPIRE_ARCHER: // Archer - light vampire
				case CLASS_UMAH: // Vorador daughter 
				case CLASS_KAIN2: // Young Kain 
				case CLASS_VAMPIRE_NECRO: // Soul Liutenant  
				case CLASS_VAMPIRE_ZOMBIE: // half vampire - melchiahim 
				case CLASS_VAMPIRE_DEMON:// Samah'el - turelim boss classes 
				case CLASS_VAMPIRE_LYCAN: // werewolf Vampire Class. 
				case CLASS_VAMPIRE_SARAFAN:  // joachim 
				case CLASS_VAMPIRE_FEAR: // he's a trickster fire vampire class - vampire of fear. 
				case CLASS_HALFVAMPIRE: // Half human \ sarafan vampire 
				// Vampiri Corrotti 
				// MELCHIAHIM
				case CLASS_TUSKEN:	// MELCHIAHIM youngs
				case CLASS_MELC: // dive like sand creature, fight like a zombie, eat like a vampire. 
				// DUMAHIM
				case CLASS_WEEQUAY:	// Dumahim, young warrior vampires 
				case CLASS_DUMAHIM: // dumahim expert vampires 
				case CLASS_SNEAKER: // dumahim assassins 
				// RAHABIM
				case CLASS_SWAMPTROOPER:		// YOUNG RAHABIM VAMPIRES 
				case CLASS_ALORA:			// RAHABIM SPECIAL CLASS - Sirens
				case CLASS_RAHABIM:				// immune to water, weak to fire, deadly to light, act like swamptrooper class
				// TURELIM
				// Vampires e Reapers 
				case CLASS_GRAN:	// young TURELIM VAMPIRES
				case CLASS_TURELIM:// adult turelim. act like gran class, weak to sound, resistance to fire- 
				case CLASS_TYRANT: // Turelim elite fight class 
				// Razielhim
				case CLASS_FLIER2: // razielhim young vampires
				case CLASS_RAZIELHIM: // razielhim vampires
				case CLASS_SENTINEL: // razielhim elite class
	     		G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				break;
				// Old Vampires and guardians deals more damage. 
				case CLASS_NECROMANCER: // Vampire Necromancer
				case CLASS_VAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_VAMPIRE_NECRO2:// Deathmask with full powers 
				case CLASS_KYLE:	// VAMPIRE BOSS CLASS (KAIN)
				case CLASS_KAIN:// Kain with full powers
				case CLASS_VAMPIRE_ELF3: // Elf Vampire blue armor rainbow lion class - dimension guardian 
				// Conflict Guardians 
				case CLASS_VAMPIRE_FROST: // for Kainh conflict guardian with frost powers. 
				case CLASS_VAMPIRE_FEAR2: // classe phobos knight of fear infernale 
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/wall_impact" , origin, dir  );  
				break;
				break;
				// WOLF, BAT AND MIST FORM DO NOT SHOOT MISSILE. 
				case CLASS_WOLFFORM: // for vampires mutated into wolves
				case CLASS_BATFORM: // for vampires mutated into bats 
				case CLASS_WOLFKAIN2:// Kain BO2
				case CLASS_BATKAIN2: // Kain bo2 bat form 
				case CLASS_BATVAMPIRE_WATER: // Shaar Naik Bat form
				case CLASS_BATVAMPIRE_ELF: // Elf Vampire bat form 
				case CLASS_BATVAMPIRE_ELF2: // Elf Vampire khyroptera form 
				case CLASS_WOLFVAMPIRE_ELF: // evoluted elf vampire class with earth connection. pretty strong. 
				case CLASS_LIONVAMPIRE_ELF: // Elf Vampire blue armor 
				case CLASS_HELLWOLF: // Samah'el metalupo 
				case CLASS_HELLBAT: // Samah'el Colibri
				case CLASS_WOLFVAMPIRE_LYCAN: // werewolf Vampire Class. 
		     	case CLASS_WOLFVAMPIRE_FEAR: // fire vampire class - vampire of fear. 
				case CLASS_BATVAMPIRE_FEAR: // Phobos bat form 
				case CLASS_WOLFHALFVAMPIRE: // Half human \ sarafan vampire 
		   		case CLASS_WOLFNECROMANCER: // Vampire Necromancer 
				case CLASS_WOLFVAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_BATNECROMANCER: // Soul Bat form 
				case CLASS_BATVAMPIRE_QUEEN: // Sheer Bat form 
				case CLASS_WOLFKAIN:// Kain with full powers
				case CLASS_WOLFKAIN3:// Kain Sion of balance 
				case CLASS_WOLFVORADOR: // vorador class 
				case CLASS_WOLFJANOS: // janos audron class 
				case CLASS_BATKAIN: // Kain Bat form
				case CLASS_BATVORADOR: // Vorador Bat form 
				case CLASS_BATKAIN3: // kain prophecy bat form 
				case CLASS_BATVAMPIRE_ELF3: // Elf Vampire khyroptera form full power. 
				case CLASS_LIONVAMPIRE_ELF2: // Elf Vampire blue armor lion Elf Vampire with rainbows - Dimensione Guardian
				case CLASS_WOLFVAMPIRE_FEAR2: // classe phobos knight of fear infernale, Conflict Guardian
				case CLASS_BATVAMPIRE_FEAR2: // Phobos demonic bat form 
				case CLASS_BATVAMPIRE_FROST: // Kainh bat form 
				// MIST FORM // - NON ATTACCANO
				case CLASS_MISTFORM: // for vampires mutated into fog 
				case CLASS_MISTVAMPIRE_DARK: // Mist vae 
				case CLASS_MISTKAIN2:// Mist Kain BO2 
				case CLASS_MISTVAMPIRE_WATER:// shaar naik mist form 
				case CLASS_MISTVAMPIRE_BLOOD: // Mist osil
				case CLASS_MISTVAMPIRE_ELF:// Elf Vampire basic mist form
				case CLASS_MISTVAMPIRE_ELF2: // Elf Vampire Iridu mist form
				case CLASS_HELLMIST: // samah'el mist form 
		 		case CLASS_MISTVAMPIRE_SARAFAN:  // joachim 
				case CLASS_MISTVAMPIRE_FEAR: // mist phobos 
		 		case CLASS_MISTNECROMANCER: // Mist soul 
				case CLASS_MISTVAMPIRE_QUEEN: // Mist sheer 
		  		case CLASS_MISTKAIN:// Mist default kain 
				case CLASS_MISTVORADOR: // Mist vorador 
				case CLASS_MISTJANOS: // Mist janos audron 
				case CLASS_MISTKAIN3: // Mist Kain prophecy
				case CLASS_MISTVAMPIRE_ELF3:// Elf Vampire paladine mist form
				case CLASS_MISTVAMPIRE_FEAR2:// Mist phobos fear knight 
				{
					return; 
				}
				break;
				// Altri Psichici
				case CLASS_MENTALIST: 
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/wall_impact" , origin, dir  );  
				break;
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/wall_impact" , origin, dir  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/wall_impact" , origin, dir  );  
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				G_PlayEffect( "time/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/wall_impact" , origin, dir  );  
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/wall_impact" , origin, dir  );  
				break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/wall_impact" , origin, dir  );  
				break;
				// ELEMENTALI 
				// i Golem di terra posson lanciar un proiettile di polvere
				case CLASS_GONK:				// GOLEM AND EARTH ELEMENTAL CREATURES 
				case CLASS_EARTH:// earth elemental class 
				case CLASS_GOLEM: // golem della terra 
				case CLASS_CRYSTAL: // Elementali del Cristallo
				case CLASS_GOLEM_CRYSTAL: // golem di cristallo 
				case CLASS_UGNAUGHT: // MUD MONSTERS, BLOBS, 
				case CLASS_MUD:// elementale di fango
				case CLASS_STONE:// elementale di pietra
				case CLASS_GOLEM_MUD: // golem di fango
				case CLASS_GOLEM_STONE: // golem di pietra
				case CLASS_DUST:
				case CLASS_GOLEM_DUST:
				G_PlayEffect( "dust/wall_impact" , origin, dir  );  
				break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/wall_impact" , origin, dir  );  
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_SONIC: // elementale sonico
				case CLASS_GOLEM_SONIC:// golem di suono
				// Asgarath con l'astrale 
				case CLASS_LANDO:	// ASTRAL CLASS 
				// I golem d'astrale 
				case CLASS_ASTRAL: // Elementali Astrali
				case CLASS_GOLEM_ASTRAL: // Golem Astrale
				case CLASS_SILENCE:
				case CLASS_GOLEM_SILENCE:
				case CLASS_HOWLER:			// SONIC CREATURE CLASS 
				G_PlayEffect( "silence/wall_impact" , origin, dir  );  
				break;
				// gli elementali di Nebbia 
				case CLASS_FOG: // elementali della Nebbia
	            case CLASS_GOLEM_FOG: // golem della nebbia 
		        case CLASS_REBEL:			// FOG CLASS 
				case CLASS_ROCKETTROOPER:	// STORM AND FLYING CREATURES 
				// I Golem d'Aria, lanceranno deboli attacchi di Vuoto.
				case CLASS_AIR: // wind fog class  
	     		case CLASS_GOLEM_AIR: // golem dell'aria 
			  	case CLASS_SABER_DROID:		// LIGHTNING GOLEM CLASS 
			    case CLASS_LIGHTNING: // Elementali del Fulmine
			    case CLASS_GOLEM_LIGHTNING: // golem del fulmine
				case CLASS_STORM:// elementale di tempesta
				case CLASS_GOLEM_STORM: // golem di tempesta
		       	G_PlayEffect( "void/wall_impact" , origin, dir  );  
				break;
				// Il golem del fuoco e i raggianti del fuoco, lanciano deboli attacchi di fumo 
				case CLASS_FIRE: // fire-warm element class 
				case CLASS_GOLEM_FIRE: // golem di fuoco
			    case CLASS_WARM: // Elementali di calore 
				case CLASS_GOLEM_WARM: // golem del silenzio
				case CLASS_HAZARD_TROOPER:	// WARM AND FIRE CREATURES 
				case CLASS_SMOKE:
				case CLASS_GOLEM_SMOKE:
				case CLASS_BESPIN_COP:
				G_PlayEffect( "smoke/wall_impact" , origin, dir  );  
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/wall_impact" , origin, dir  );  
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				case CLASS_WATER:// water class 
				case CLASS_GOLEM_WATER: // golem dell'acqua
				case CLASS_GOLEM_SALT: // golem di sale 
				case CLASS_SALT: // elementali del Sale
				case CLASS_GALAKMECH:		// CRYSTAL MONSTER 
				case CLASS_GOLEM_ICE: // golem di ghiaccio
				case CLASS_ICE: // elementale di ghiaccio
				case CLASS_WAVE: // elementale delle maree
				case CLASS_GOLEM_WAVE: // golem delle maree
				// Sale e Silenzio hanno la tenebra 
				G_PlayEffect( "salt/wall_impact" , origin, dir  );  
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/wall_impact" , origin, dir  );  
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/wall_impact" , origin, dir  );  
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/wall_impact" , origin, dir  );  
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				// Pistola Glifica Fumogena 
				case CLASS_IMPWORKER: // ALCHEMIST AND WIZARD CLASS 	
				case CLASS_ALCHEMIST:  // Alchemist altfire, switch,
				// costrutti tecnomagici 
				case CLASS_REMOTE:			// ARTIFACT SHOOTER ARROW TRAP
				case CLASS_SEEKER:			// SEEKER ARTIFACTS
				case CLASS_SENTRY:			// SENTRY ARTIFACT
				case CLASS_R2D2:				// MECHANICAL GOLEM CLASS 
				case CLASS_R5D2:				// MECHANICAL GOLEM CLASS 2
				G_PlayEffect( "smoker/wall_impact" , origin, dir  );  
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/wall_impact" , origin, dir  );  
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/wall_impact" , origin, dir  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/wall_impact" , origin, dir  );  
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/wall_impact" , origin, dir  );  
				break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/wall_impact" , origin, dir  );  
				break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/wall_impact" , origin, dir  );  
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/wall_impact" , origin, dir  );  
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/wall_impact" , origin, dir  );  
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/wall_impact" , origin, dir  );  
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/wall_impact" , origin, dir  );  
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/wall_impact" , origin, dir  );  
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/wall_impact" , origin, dir );  
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/wall_impact" , origin, dir  );  
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/wall_impact" , origin, dir  );  
				break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/wall_impact" , origin, dir  );  
				break; 
			default:
				break;			
				}*/
		   }
		}
     }
	break;
	case WP_JAWA:
		if ( altFire )
		{
			parm = 0;

			if ( cent->gent )
			{
				parm += cent->gent->count;
			}

			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_LANDO: 
				FX_AstralAltHitWall( origin, dir, parm );
				break;
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_PROPHET:  
				case CLASS_DRUID: 
				case CLASS_LYCAN:  
				case CLASS_SARAFAN: 
				case CLASS_ARCHER: 
				case CLASS_WARRIOR: 
				case CLASS_CENOBITA:  
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_ARIEL: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_DEMON_FIRE:
				case CLASS_DEMON_POISON:
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
				case CLASS_FIRE:  
				case CLASS_WATER: 
				case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
				case CLASS_HOLY:  
				case CLASS_POISON: 
				case CLASS_IVY:  
				case CLASS_SONIC: 
				case CLASS_BIRD: 
				case CLASS_BEAST:  
				case CLASS_SHARK:  
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_FIRE:
				case CLASS_GOLEM_WATER:
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GOLEM_POISON: 
				case CLASS_GOLEM_LAVA:
				case CLASS_BESPIN_COP:
				case CLASS_REELO:
				case CLASS_CLAW:
				case CLASS_HAZARD_TROOPER:
				case CLASS_UGNAUGHT: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE: 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_MONMOTHA: 
				case CLASS_WAMPA:
				case CLASS_TUSKEN:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_GRAN: 
				case CLASS_PRISONER:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_GALAK: 
				case CLASS_LIZARD:
				case CLASS_JAN: 
				case CLASS_GLIDER:
				case CLASS_JAWA:
				case CLASS_MURJJ:
				case CLASS_MINEMONSTER:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE:
				case CLASS_DESANN:
				case CLASS_TAVION: 
				case CLASS_SENTRY: 
				case CLASS_BARTENDER:
				case CLASS_COMMANDO: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_RANCOR: 
				case CLASS_IMPERIAL: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_INTERROGATOR:
				case CLASS_JEDI: 
				case CLASS_LUKE: 
				case CLASS_GALAKMECH: 
				case CLASS_MORGANKATARN: 
				case CLASS_PROTOCOL: 
				case CLASS_R2D2: 
				case CLASS_R5D2:
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_SABOTEUR: 
				case CLASS_STORMTROOPER: 
				case CLASS_SWAMP: 
				case CLASS_NOGHRI: 
				case CLASS_TRANDOSHAN: 
				case CLASS_WEEQUAY: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_SABER_DROID: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_HowlerAltHitWall( origin, dir, parm ); 
				break; 
			default:
				break;
				}
			}
		}
		else
		{
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 		{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_LANDO: 
				FX_AstralHitWall( origin, dir );
				break;
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_PROPHET:  
				case CLASS_DRUID: 
				case CLASS_LYCAN:  
				case CLASS_SARAFAN: 
				case CLASS_ARCHER: 
				case CLASS_WARRIOR: 
				case CLASS_CENOBITA:  
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_ARIEL: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_DEMON_FIRE:
				case CLASS_DEMON_POISON:
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
				case CLASS_FIRE:  
				case CLASS_WATER: 
				case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
				case CLASS_HOLY:  
				case CLASS_POISON: 
				case CLASS_IVY:  
				case CLASS_SONIC: 
				case CLASS_BIRD: 
				case CLASS_BEAST:  
				case CLASS_SHARK:  
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_FIRE:
				case CLASS_GOLEM_WATER:
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GOLEM_POISON: 
				case CLASS_GOLEM_LAVA:
				case CLASS_BESPIN_COP:
				case CLASS_REELO:
				case CLASS_CLAW:
				case CLASS_HAZARD_TROOPER:
				case CLASS_UGNAUGHT: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE: 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_MONMOTHA: 
				case CLASS_WAMPA:
				case CLASS_TUSKEN:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_GRAN: 
				case CLASS_PRISONER:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_GALAK: 
				case CLASS_LIZARD:
				case CLASS_JAN: 
				case CLASS_GLIDER:
				case CLASS_JAWA:
				case CLASS_MURJJ:
				case CLASS_MINEMONSTER:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE:
				case CLASS_DESANN:
				case CLASS_TAVION: 
				case CLASS_SENTRY: 
				case CLASS_BARTENDER:
				case CLASS_COMMANDO: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_RANCOR: 
				case CLASS_IMPERIAL: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_INTERROGATOR:
				case CLASS_JEDI: 
				case CLASS_LUKE: 
				case CLASS_GALAKMECH: 
				case CLASS_MORGANKATARN: 
				case CLASS_PROTOCOL: 
				case CLASS_R2D2: 
				case CLASS_R5D2:
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_SABOTEUR: 
				case CLASS_STORMTROOPER: 
				case CLASS_SWAMP: 
				case CLASS_NOGHRI: 
				case CLASS_TRANDOSHAN: 
				case CLASS_WEEQUAY: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_SABER_DROID: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_HowlerHitWall( origin, dir ); 
				break; 
			default:
				break;
				}
			}
		}
		break;
	/*	case WP_BLOODGUN:
		if ( altFire )
		{
			parm = 0;

			if ( cent->gent )
			{
				parm += cent->gent->count;
			}

			FX_BloodGunAltHitWall( origin, dir, parm );
		}
		else
		{
			FX_BloodGunHitWall( origin, dir );
		}
		break;*/

	case WP_BLASTER:
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER:
			case CLASS_SABOTEUR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_IMPERIAL:
			case CLASS_WAMPA:
			case CLASS_BOBAFETT:
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_VAMPIRE_DEMON: 
			case CLASS_FIREREAVER:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_AIR: 			
			case CLASS_GOLEM_AIR:
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT:
			case CLASS_GOLEM_GOLD:
			case CLASS_LYCAN:  
			FX_WarmHitWall( origin, dir );
			break;
			case CLASS_CLAW:
			case CLASS_EARTH: 
			case CLASS_GOLEM: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_REELO: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GONK: 
			case CLASS_CRYSTAL:
			case CLASS_VAMPIRE_GHOST: 
			FX_MagmaHitWall( origin, dir );
			break;
			case CLASS_ALORA: 
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_FLIER2: 
			case CLASS_NOGHRI:
			case CLASS_SWAMPTROOPER:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_MONMOTHA: 
			case CLASS_DECEIVER: 
			case CLASS_ZEPH: 
			case CLASS_TURELIM: 
			case CLASS_RAHABIM: 
			case CLASS_VAMPIRE:   
			case CLASS_KAIN:
			case CLASS_KAIN2:  
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR: 
			case CLASS_JANOS:  
			case CLASS_NECROMANCER:  
			case CLASS_VAMPIRE_QUEEN:  
			case CLASS_VAMPIRE_DARK:  
			case CLASS_VAMPIRE_NECRO:   
			case CLASS_VAMPIRE_NECRO2: 
			case CLASS_VAMPIRE_FEAR:  
			case CLASS_VAMPIRE_BLOOD: 
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN:  
			case CLASS_VAMPIRE_ELF:  
			case CLASS_VAMPIRE_ELF2:  
			case CLASS_VAMPIRE_ELF3: 
			case CLASS_VAMPIRE_WATER:  
			case CLASS_VAMPIRE_FROST: 
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE:
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_SARAFAN:
			case CLASS_GOLEM_DARK: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_GOLEM_VOID:
			FX_VampFireHitWall( origin, dir );
			break;
			case CLASS_MOUSE: 
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_CENOBITA: 
			FX_HellFireHitWall ( origin, dir );
			break;
			case CLASS_TUSKEN:
			case CLASS_VAMPIRE_ZOMBIE: 
			case CLASS_GLIDER:
			case CLASS_MURJJ: 
			case CLASS_JAWA:
			case CLASS_SENTRY:
			case CLASS_RANCOR:
			case CLASS_GHOST: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_VAMPIRE_FEAR2: 
			case CLASS_PROPHET: 
			case CLASS_POLTER:
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO:
			FX_GhostFireHitWall ( origin, dir );
			break;
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			FX_GreenFireHitWall( origin, dir );
			break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_LIZARD:
			case CLASS_MINEMONSTER: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_IVY: 
			case CLASS_BEAST: 
			case CLASS_BIRD:
			case CLASS_SHARK: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_INTERROGATOR: 
			FX_PoisonFireHitWall ( origin, dir );
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_MARK1: 
			case CLASS_MARK2: 
			case CLASS_GALAKMECH: 
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 
			case CLASS_PROTOCOL: 
			case CLASS_REBEL: 
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_ROCKETTROOPER: 
			case CLASS_SABER_DROID: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID: 
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_THRALL: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_HOLY:  
			case CLASS_SONIC: 
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			case CLASS_GOLEM_METAL:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			FX_BlasterWeaponHitWall ( origin, dir );
			break;
			case CLASS_FIGHTER: 
			FX_GreenLaserHitWall ( origin, dir ); 
		default:
			break;
			}
		}
		//FX_BlasterWeaponHitWall( origin, dir );
		break;

	case WP_BOWCASTER:
		// FX_BowcasterHitWall( origin, dir );
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_LIGHTREAVER: 
			case CLASS_LYCAN: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_SABER_DROID:
			case CLASS_ASSASSIN_DROID: 
			FX_ShockHitWall( origin, dir );
			break;
			case CLASS_MORGANKATARN:
			case CLASS_GOLEM_GOLD: 
			case CLASS_PROTOCOL: 
			case CLASS_JAN: 
			case CLASS_CLAW: 
			FX_SunHitWall( origin, dir );
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA:
			case CLASS_MONMOTHA:
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_LIZARD:
			case CLASS_SWAMPTROOPER:
			case CLASS_REBEL:
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_WAMPA: 
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_TURELIM:
			case CLASS_VAMPIRE:
			case CLASS_VAMPIRE_DEMON:
			case CLASS_VAMPIRE_GHOST: 
			case CLASS_KAIN:
			case CLASS_KAIN2: 
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR:
			case CLASS_JANOS:
			case CLASS_NECROMANCER: 
			case CLASS_VAMPIRE_QUEEN:
			case CLASS_VAMPIRE_DARK: 
			case CLASS_VAMPIRE_NECRO: 
			case CLASS_VAMPIRE_NECRO2:
			case CLASS_VAMPIRE_FEAR: 
			case CLASS_VAMPIRE_FEAR2:  
			case CLASS_VAMPIRE_BLOOD:
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN: 
			case CLASS_VAMPIRE_ELF: 
			case CLASS_VAMPIRE_ELF2:
			case CLASS_VAMPIRE_ELF3:
			case CLASS_VAMPIRE_WATER: 
			case CLASS_VAMPIRE_FROST:  
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE: 
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_ZOMBIE:
			case CLASS_VAMPIRE_SARAFAN: 
			case CLASS_AIR: 
			case CLASS_GOLEM_AIR: 
			case CLASS_BIRD: 
			case CLASS_IVY: 
			FX_EbHitWall( origin, dir );
			break; 
			case CLASS_GALAK:
			case CLASS_RODIAN:
			case CLASS_MARK1:
			case CLASS_MARK2: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			FX_LightKnifeHitWall( origin, dir );
			break;
			case CLASS_TAVION:
			case CLASS_DESANN:
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_SAND_CREATURE:
			case CLASS_ATST:
			case CLASS_SENTRY:
			case CLASS_MOUSE: 
			case CLASS_TENTACLE:  
			case CLASS_ELDERGOD: 
			case CLASS_SONIC: 
			case CLASS_GOLEM_SONIC: 
			case CLASS_PROPHET: 
			case CLASS_DECEIVER: 
			case CLASS_FLIER2: 
			case CLASS_BEAST:  
			case CLASS_SHARK:
			FX_MadnessHitWall( origin, dir );
			break;
			case CLASS_ARIEL: 
			case CLASS_MINEMONSTER:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_FIRE: 
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_SABOTEUR: 
			case CLASS_MURJJ:
			case CLASS_NECRO: 
			case CLASS_THRALL: 
			case CLASS_IMPERIAL: 
			case CLASS_CENOBITA: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_POISON: 
			case CLASS_GOLEM_DARK: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_NECRO:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_GOLEM_VOID:
			case CLASS_GOLEM_POISON: 
			case CLASS_GOLEM_LAVA:
			case CLASS_POLTER:
			FX_LightSkullHitWall( origin, dir );
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER:
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_GALAKMECH: 
			case CLASS_PRISONER: 
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER: 
			case CLASS_REELO: 
			case CLASS_REMOTE: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP: 
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_GONK: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID:
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:			
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER:		 
			case CLASS_LIGHT: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_CRYSTAL: 
			case CLASS_EARTH: 
			case CLASS_HOLY:  
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:			
			case CLASS_GOLEM:
			case CLASS_GOLEM_HOLY: 
			FX_BowcasterHitWall ( origin, dir );
			break; 
			case CLASS_FIGHTER:
			FX_DardusHitWall( origin, dir );
			break; 
		default:
			break;
			}
		}
		break;
	case WP_REPEATER:
		/*if ( altFire )
		{
			FX_RepeaterAltHitWall( origin, dir );
		}
		else
		{
			FX_RepeaterHitWall( origin, dir );
		}*/
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_RAZIELHIM: 
				case CLASS_BARTENDER:
				case CLASS_LYCAN:
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST:
				case CLASS_GOLEM_ICE: 
				case CLASS_POLTER: 
				case CLASS_PROBE: 
				FX_IceAltHitWall( origin, dir );
				break;
				case CLASS_ESKANDOR: 
				case CLASS_GALAKMECH:
				case CLASS_CRYSTAL: 
				case CLASS_WEEQUAY:
				case CLASS_GONK: 
				case CLASS_GOLEM: 
				case CLASS_EARTH: 
				FX_CrystalAltHitWall( origin, dir );
				break;
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_TUSKEN:
				case CLASS_LIZARD:
				case CLASS_MONMOTHA: 
				case CLASS_GLIDER:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2: 
				case CLASS_GRAN: 
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_RANCOR:
				case CLASS_SLUAGH: 
				case CLASS_MINEMONSTER: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
			  	case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_BEAST: 
				FX_GreenMudAltHitWall( origin, dir );
				break;
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_GOLEM_NECRO: 
				case CLASS_NECRO: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN: 
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_MURJJ: 
				case CLASS_THRALL: 
				FX_NecroticAltHitWall( origin, dir );
				break; 
				case CLASS_IMPERIAL:
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_WAMPA:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_DEMON_FIRE: 
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN: 
				FX_HellMudAltHitWall( origin, dir );
				break;
				case CLASS_INTERROGATOR:
				case CLASS_HAZARD_TROOPER: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_CLAW: 
				case CLASS_JAWA: 
				case CLASS_GHOST: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD:
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_BIRD: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR:
				case CLASS_LIGHT: 
				case CLASS_GOLEM_SMOKE: 
				FX_VaporAltHitWall( origin, dir );
				break;
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE:
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_SENTRY:
				case CLASS_MOUSE:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				FX_ElderTarAltHitWall( origin, dir );
				break;
				case CLASS_SABOTEUR:
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_REELO: 
				case CLASS_ALCHEMIST: 
				case CLASS_IVY: 
				case CLASS_SWAMP: 
				FX_TarAltHitWall( origin, dir );
				break;
				case CLASS_RODIAN:
				case CLASS_GALAK:
				case CLASS_STORMTROOPER:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_ARIEL: 	
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				FX_HolyWaterAltHitWall( origin, dir );
				break;
				case CLASS_ALORA:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SWAMPTROOPER: 
				case CLASS_GOLEM_WATER:	
				FX_TkWaterAltHitWall( origin, dir );
				break;
				case CLASS_FISH: 
				FX_TkWater2AltHitWall( origin, dir );
				break;
				case CLASS_HOWLER: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_MORGANKATARN: 
				case CLASS_PRISONER: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DRUID: 
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
			 	case CLASS_SPIDERREAVER: 				
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 			  
				case CLASS_WATER:			 
			  	case CLASS_SONIC: 
				case CLASS_SHARK:  
				case CLASS_GOLEM_METAL:			
				case CLASS_GOLEM_SONIC:			 
				FX_RepeaterAltHitWall( origin, dir );
				break; 
				case CLASS_FIGHTER:
				FX_PlasmaBombAltHitWall( origin, dir );
				break;  
			default:
				break;
				}
			}
		}
		else
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_BARTENDER:
				case CLASS_RAZIELHIM: 
				case CLASS_LYCAN:
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST:
				case CLASS_GOLEM_ICE: 
				case CLASS_POLTER: 
				case CLASS_PROBE: 
				FX_IceHitWall( origin, dir );
				break;
				case CLASS_GALAKMECH:
				case CLASS_ESKANDOR: 
				case CLASS_CRYSTAL: 
				case CLASS_WEEQUAY:
				case CLASS_GONK: 
				case CLASS_GOLEM: 
				case CLASS_EARTH: 
				FX_CrystalHitWall( origin, dir );
				break;
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_TUSKEN:
				case CLASS_LIZARD:
				case CLASS_MONMOTHA: 
				case CLASS_GLIDER:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2: 
				case CLASS_GRAN: 
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_RANCOR:
				case CLASS_SLUAGH: 
				case CLASS_MINEMONSTER: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
			  	case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_BEAST: 
				FX_GreenMudHitWall( origin, dir );
				break;
				case CLASS_IMPERIAL:
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_WAMPA:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_DEMON_FIRE: 
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN: 
				FX_HellMudHitWall( origin, dir );
				break;
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_GOLEM_NECRO: 
				case CLASS_NECRO: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN: 
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_MURJJ: 
				case CLASS_THRALL: 
				FX_NecroticHitWall (origin, dir );
				break; 
				case CLASS_INTERROGATOR:
				case CLASS_HAZARD_TROOPER: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_CLAW: 
				case CLASS_JAWA: 
				case CLASS_GHOST: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD:
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_BIRD: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR:
				case CLASS_LIGHT: 
				case CLASS_GOLEM_SMOKE: 
				FX_VaporHitWall( origin, dir );
				break;
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE:
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_SENTRY:
				case CLASS_MOUSE:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				FX_ElderTarHitWall( origin, dir );
				break;
				case CLASS_SABOTEUR:
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_REELO: 
				case CLASS_ALCHEMIST: 
				case CLASS_IVY: 
				case CLASS_SWAMP: 
				FX_TarHitWall( origin, dir );
				break;
				case CLASS_RODIAN:
				case CLASS_GALAK:
				case CLASS_STORMTROOPER:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_ARIEL: 	
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				FX_HolyWaterHitWall( origin, dir );
				break;
				case CLASS_ALORA:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SWAMPTROOPER: 
				case CLASS_GOLEM_WATER:	
				FX_TkWaterHitWall( origin, dir );
				break;
				case CLASS_FISH: 
				FX_TkWater2HitWall( origin, dir );
				break;
				case CLASS_HOWLER: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_MORGANKATARN: 
				case CLASS_PRISONER: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DRUID: 
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
			 	case CLASS_SPIDERREAVER: 				
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 			  
				case CLASS_WATER:			 
			  	case CLASS_SONIC: 
				case CLASS_SHARK:  
				case CLASS_GOLEM_METAL:			
				case CLASS_GOLEM_SONIC:			 
				FX_RepeaterHitWall( origin, dir );
				break; 
				case CLASS_FIGHTER:
				FX_PlasmaHitWall( origin, dir );
				break;  
			default:
				break;
				}
			}
		}
		break;

	case WP_DEMP2:
		if ( altFire )
		{
		}
		else
		{
			// FX_DEMP2_HitWall( origin, dir );
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
			// BENEDICTUM
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_RODIAN:
			case CLASS_PRISONER:
			case CLASS_GALAK:
			case CLASS_INTERROGATOR:
			case CLASS_SWAMP:
			case CLASS_WATER: 
			case CLASS_GOLEM_WATER: 
			case CLASS_GOLEM_ICE: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_SEEKER:
			case CLASS_REMOTE: 
			FX_DEMP_HitWall( origin, dir );
			break;
			// SPECTRAL
			case CLASS_REELO:
			case CLASS_RANCOR:
			case CLASS_SENTRY:
			case CLASS_MINEMONSTER:
			case CLASS_SABOTEUR:
			case CLASS_SHADOWTROOPER:
			case CLASS_ASSASSIN_DROID:
			case CLASS_MORGANKATARN:
			case CLASS_HAZARD_TROOPER:
			case CLASS_REBEL: 
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT:
			case CLASS_BARTENDER:
			case CLASS_UGNAUGHT:
			case CLASS_JEDI:
			case CLASS_REAVER: 
			case CLASS_LIZARD: 
			case CLASS_WEEQUAY: 
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
				FX_DEMP0_HitWall( origin, dir );
					break;
				// HELL WRACK
			case CLASS_PROPHET: 
			case CLASS_CENOBITA: 
			case CLASS_WAMPA:
			case CLASS_TAVION:
			case CLASS_IMPERIAL:
			case CLASS_DESANN:
			case CLASS_ATST:
			case CLASS_MOUSE:
			case CLASS_SAND_CREATURE:
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_MURJJ: 	
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_DEMON_FIRE:
			case CLASS_DEMON_POISON:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_FIRE: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_GRAN: 
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FLIER2:
			case CLASS_FISH:
			case CLASS_SWAMPTROOPER:			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_HELLGOLEM: 
			case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				FX_DEMP1_HitWall( origin, dir );
				break;
			// SPIRIT
			case CLASS_RAZIEL: 
			case CLASS_IMPWORKER: 
				FX_DEMP2_HitWall( origin, dir );
				break;
				// HOLY
			case CLASS_HOLY: 
			case CLASS_GOLEM_HOLY: 
			case CLASS_VAMPIRE_GHOST: 
				FX_DEMP3_HitWall( origin, dir );
				break;
				// LIFE 
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_BEAST: 
				case CLASS_BIRD: 
				case CLASS_SHARK: 
				FX_DEMP4_HitWall( origin, dir );
				break;
				// DRUIDIC FIRE
				case CLASS_TRANDOSHAN:
				case CLASS_LANDO: 
				case CLASS_DRUIDREAVER: 
				case CLASS_DRUID: 
				case CLASS_LYCAN: 
				FX_DEMP5_HitWall( origin, dir );
				break;
				// HOLY FIRE
				case CLASS_JAN:
				case CLASS_SARAFAN: 
				case CLASS_STORMTROOPER: 
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_WIZARD: 
				FX_DEMP6_HitWall( origin, dir );
				break;
				// REVERB
				case CLASS_GALAKMECH: 
				case CLASS_GONK: 
				case CLASS_NECROREAVER:
				case CLASS_JAWA:
				case CLASS_PROBE:
				case CLASS_COMMANDO:
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_HOWLER: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
			  	case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
			  	case CLASS_SONIC: 
			 	case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
			 	case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GHOST: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_DEMP7_HitWall( origin, dir );
				break;
				// BALANCE 
				case CLASS_KYLE: 
				case CLASS_KAIN3: 
				case CLASS_ARIEL: 
				FX_DEMP8_HitWall( origin, dir );
				break;
				// WRACK IMPORTANT: NEVER. NEVER GET THIS TO AN NPC!   
				//case CLASS_GALAKMECH:
				//case CLASS_GONK:
				case CLASS_REBORN:
				case CLASS_ALORA:
				case CLASS_MONMOTHA:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				FX_DEMP9_HitWall( origin, dir );
				break; 
			case CLASS_FIGHTER:
			FX_PULSE_HitWall( origin, dir );
			break; 
			default:
				break;
				}
			}
		}
		break;

	case WP_POISON:
		if ( altFire )
		{
		}
		else
		{
			FX_Poison_HitWall( origin, dir );
		}
		break;

	case WP_FLECHETTE:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FANGO				
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 	
				theFxScheduler.PlayEffect( "flechette2/alt_blow", origin, dir );
				break;
				// PIETRA 
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( "flechette3/alt_blow", origin, dir );				
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( "acid/alt_blow", origin, dir );				
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				theFxScheduler.PlayEffect( "ivy/alt_blow", origin, dir );				
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( "ivy2/alt_blow", origin, dir );
				break;
				// THORN 
				case CLASS_SWAMP: 
				theFxScheduler.PlayEffect( "thorn/alt_blow", origin, dir );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				theFxScheduler.PlayEffect( "spectral/alt_blow", origin, dir );				
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( "stasis/alt_blow", origin, dir );				
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
				theFxScheduler.PlayEffect( "dust/alt_blow", origin, dir );				
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				theFxScheduler.PlayEffect( "ash/alt_blow", origin, dir );
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				theFxScheduler.PlayEffect( "gcrystal/alt_blow", origin, dir );
				break;
				// DEFAULT 
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				theFxScheduler.PlayEffect( "flechette/alt_blow", origin, dir );
				break; 
				case CLASS_FIGHTER:
				theFxScheduler.PlayEffect( "flechette0/alt_blow", origin, dir );
				break; 
			default:
				break;
				}
			}
		}
		else
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 {
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FANGO				
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 	
				FX_MudHitWall( origin, dir );
				break;
				// PIETRA 
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				FX_StoneHitWall( origin, dir );
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_AcidHitWall( origin, dir );
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				FX_IvyHitWall( origin, dir );
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				FX_Ivy2HitWall( origin, dir );
				break;
				// THORN 
				case CLASS_SWAMP: 
				FX_ThornHitWall( origin, dir );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				FX_SpectralHitWall( origin, dir );
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				FX_StasisHitWall( origin, dir );
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
				FX_DustHitWall( origin, dir );
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				FX_AshHitWall( origin, dir );
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				FX_GcrystalHitWall( origin, dir );
				break;
				// DEFAULT 
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_FlechetteWeaponHitWall( origin, dir );
				break; 
				case CLASS_FIGHTER:
				FX_ShrapnelHitWall( origin, dir );
				break; 
			default:
				break;
				}
			}
		}
		/*if ( altFire )
		{
			theFxScheduler.PlayEffect( "flechette/alt_blow", origin, dir );
		}
		else
		{
			FX_FlechetteWeaponHitWall( origin, dir );
		}*/
		break;

		case WP_CANNON:
		if ( altFire )
		{
			theFxScheduler.PlayEffect( "cannon/alt_blow", origin, dir );
		}
		else
		{
			FX_CannonWeaponHitWall( origin, dir );
		}
		break;

	case WP_ROCKET_LAUNCHER:
		
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_GOLEM_GOLD: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_SABER_DROID:
			case CLASS_PROTOCOL: 
			case CLASS_PROBE: 
			case CLASS_POLTER: 
			case CLASS_AIRREAVER: 
			case CLASS_REBEL: 
			case CLASS_SHARK:
			case CLASS_LYCAN:  
			case CLASS_WATER:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			FX_FogHitWall ( origin, dir );
			break;
			case CLASS_ROCKETTROOPER:
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_BOBAFETT:  
			FX_StormHitWall ( origin, dir );
			break;
			case CLASS_MURJJ:
			case CLASS_THRALL: 
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO: 
			case CLASS_FISH:
			case CLASS_GLIDER:
			case CLASS_MARK1:
			case CLASS_MARK2:
			case CLASS_IMPERIAL: 
			case CLASS_PROPHET: 
			FX_ThrallBreathHitWall ( origin, dir );
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA: 
			case CLASS_MONMOTHA: 
			case CLASS_FLIER2: 
			case CLASS_LIZARD: 
			case CLASS_SWAMPTROOPER: 
			case CLASS_NOGHRI:
			case CLASS_TUSKEN: 
			case CLASS_WAMPA: 
			case CLASS_VAMPIRE:
			case CLASS_VAMPIRE_DEMON:
			case CLASS_KAIN:
			case CLASS_KAIN2: 
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR:
			case CLASS_JANOS:
			case CLASS_NECROMANCER: 
			case CLASS_VAMPIRE_QUEEN:
			case CLASS_VAMPIRE_DARK: 
			case CLASS_VAMPIRE_NECRO: 
			case CLASS_VAMPIRE_NECRO2:
			case CLASS_VAMPIRE_FEAR: 
			case CLASS_VAMPIRE_FEAR2:  
			case CLASS_VAMPIRE_BLOOD:
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN: 
			case CLASS_VAMPIRE_ELF: 
			case CLASS_VAMPIRE_ELF2:
			case CLASS_VAMPIRE_ELF3:
			case CLASS_VAMPIRE_WATER: 
			case CLASS_VAMPIRE_FROST:  
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE: 
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_ZOMBIE: 
			case CLASS_VAMPIRE_SARAFAN: 
			case CLASS_FIRE: 
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_ZEPH: 
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM: 
			case CLASS_WEEQUAY:
			case CLASS_VAMPIRE_GHOST: 
			FX_FlayHitWall ( origin, dir );
			break;
			case CLASS_MOUSE:
			case CLASS_SENTRY:
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
				if ( altFire )
				{
					FX_ElderSphereAltHitWall ( origin, dir );
				}
				else
				{
					FX_ElderSphereHitWall( origin, dir );
				}
				break;
			case CLASS_SHADOWTROOPER: 
			case CLASS_GOLEM_VOID: 
			case CLASS_VOID: 
			case CLASS_DARKREAVER: 
			case CLASS_DARK: 
			case CLASS_GOLEM_DARK: 
			case CLASS_SABOTEUR:
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
				FX_BlackHoleHitWall ( origin, dir );
				break;
			case CLASS_MINEMONSTER: 
			case CLASS_IVY: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_DEMON_POISON: 
			case CLASS_BEAST: 
			case CLASS_REELO: 
			case CLASS_ARCHER:
			case CLASS_ALCHEMIST: 
				FX_PoisonFogHitWall ( origin, dir );
				break;
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_GHOST: 
			case CLASS_DREADNOUGHT: 
			case CLASS_REAPER: 
			case CLASS_ARIEL: 
			case CLASS_CENOBITA: 			 
				FX_SpectralFogHitWall ( origin, dir );
				break;
			case CLASS_GRAN:
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
				FX_TkHitWall ( origin, dir );
				break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_GONK:
			case CLASS_GOLEM: 
			case CLASS_EARTH: 
			case CLASS_EARTHREAVER: 
			case CLASS_SPIDERREAVER2: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GALAKMECH: 
			case CLASS_SARAFAN: 
			case CLASS_DEMON_BLADE:
			case CLASS_SCOUT: 
				FX_SandStormHitWall ( origin, dir );
				break;
			case CLASS_LANDO: 
				FX_GravityHitWall ( origin, dir );
				break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 			
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LUKE: 			
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 			
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:  
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 			
			case CLASS_HAZARD_TROOPER: 			
			case CLASS_PLAYER: 
			case CLASS_VEHICLE:
			case CLASS_HUNTER: 			
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_FIREREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_CRYSTAL: 
			case CLASS_AIR:  			 
			case CLASS_HOLY: 			  
			case CLASS_SONIC: 
			case CLASS_BIRD: 			
			case CLASS_WARRIOR: 
			case CLASS_DRUID:
			case CLASS_GOLEM_AIR:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			case CLASS_GOLEM_SMOKE: 
			FX_RocketHitWall ( origin, dir );
			break; 
			case CLASS_FIGHTER: 
			FX_SeekerHitWall ( origin, dir );
			break; 
		default:
			break;
			}
		}
//		FX_RocketHitWall( origin, dir );
		break;

	case WP_CONCUSSION:
		if ( altFire )
		{
		}
		else
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// HELL STRIKE 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_GLIDER:
				case CLASS_TUSKEN:
				case CLASS_MELC:
				case CLASS_SUMMONER: 		
				FX_ConcHitWall ( origin, dir );
				break;
				// BLACK FIRE 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH: 
				case CLASS_RAHABIM: 
				case CLASS_COMMANDO: 
				case CLASS_LYCAN: 
				case CLASS_CENOBITA: 
				case CLASS_WIZARD: 
				FX_BlackFireHitWall ( origin, dir );
				break;
				// SPIRITH DEATH  
				case CLASS_REBORN:
				case CLASS_ALORA:
				case CLASS_MONMOTHA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE:
				case CLASS_KAIN2: 
				case CLASS_UMAH: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_SARAFAN:  
				FX_SDHitWall ( origin, dir );
				break;
				// IMMOLATE 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN3:
				case CLASS_TURELIM: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_CLAW: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_VAMPIRE_DEMON: 
				FX_ImmolationHitWall ( origin, dir );
				break;
				// HOLYIMMOLATE 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_TRANDOSHAN:
				case CLASS_STORMTROOPER:
				case CLASS_PRISONER:
				case CLASS_RODIAN:
				case CLASS_IMPWORKER:
				case CLASS_GALAK:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_SEEKER:
				case CLASS_REMOTE:
				case CLASS_DRUIDREAVER: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				FX_HolyImmolationHitWall ( origin, dir );
				break;
				// NECRO MIND
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_REBEL: 
				case CLASS_MENTALIST:
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_BIRD: 
				FX_MindHitWall ( origin, dir );
				break;
				// NECRO DIMENSION
				case CLASS_HOWLER:
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_LANDO: 
				FX_DimHitWall ( origin, dir );
				break;
				// NECRO CONFLICT 
				case CLASS_FIREREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_HAZARD_TROOPER:
				case CLASS_GRAN:
				case CLASS_VAMPIRE_FROST: 
				case CLASS_BOBAFETT: 
				case CLASS_WAMPA: 
				case CLASS_DEMON_FIRE: 
				FX_ConflictHitWall ( origin, dir );
				break;
				// NECRO NATURE
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_DRUID: 
				FX_NatureHitWall ( origin, dir );
				break;
				// NECRO ENERGY 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_MORGANKATARN:
				case CLASS_PROTOCOL: 
				FX_EnergyHitWall ( origin, dir );
				break;
				// NECRO ELECTRO 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID: 
				FX_ElectroHitWall ( origin, dir );
				break; 
				// NECRO POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_BEAST: 
				case CLASS_MINEMONSTER:
				case CLASS_REELO: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON:
				FX_NecroPoisonHitWall ( origin, dir );
				break;
				// NECRO TIME 
				case CLASS_ROCKETTROOPER:
				case CLASS_VAMPIRE_ELF3: 
				FX_TimeHitWall ( origin, dir );
				break;
				// NECRO STONE  
				case CLASS_GONK:
				case CLASS_LIZARD:
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 
				FX_RockHitWall ( origin, dir );
				break;
				// NECRO CHAOS
				case CLASS_PROPHET: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_SENTRY: 
				FX_ChaosHitWall ( origin, dir );
				break;
				// NECRO DEATH  
				case CLASS_ARIEL: 
				case CLASS_GHOST: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_JEDI:
				case CLASS_REAVER: 
				case CLASS_NECROREAVER: 
				case CLASS_THRALL: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_JAWA:
				case CLASS_MURJJ: 
				case CLASS_RANCOR: 
				case CLASS_PLAYER:
				case CLASS_VEHICLE: 
				FX_NecroHitWall ( origin, dir );
				break;
				// NECRO SMOG 
				case CLASS_BESPIN_COP: 				
				case CLASS_DARK: 			 
				case CLASS_GOLEM_DARK: 
				case CLASS_TANKREAVER: 
				case CLASS_GOLEM_SMOKE: 
				FX_NecrosmogHitWall ( origin, dir );
				break;
				// NECRO VOID  
				case CLASS_SABOTEUR: 
				case CLASS_SHADOWTROOPER: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				FX_NecrovoidHitWall ( origin, dir );
				break;
				// NECRO MUD 
				case CLASS_SWAMP:
				case CLASS_INTERROGATOR:
				case CLASS_SHARK: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_SPIDERREAVER:
				FX_NecromudHitWall ( origin, dir );
				break;
				// NECRO CRYSTAL 
				case CLASS_WEEQUAY:
				case CLASS_BARTENDER:
				case CLASS_GOLEM_ICE: 
				case CLASS_ICEREAVER: 
				case CLASS_CRYSTAL: 
				case CLASS_GALAKMECH: 
				FX_NecroCrystalHitWall ( origin, dir );
					break;
				case CLASS_FIGHTER: 	
				FX_AMHitWall ( origin, dir );
					break;
				default:
					break;
				}
			}
		}
		// FX_ConcHitWall( origin, dir );
		break;

	case WP_THERMAL:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				if ( cent->gent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect )
				{
						theFxScheduler.PlayEffect( cent->gent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect, origin, dir );
				}


				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_ARCHER: 
				case CLASS_REAVER: 
				case CLASS_MENTALIST: 
				case CLASS_JEDI:
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PROBE: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_AIRREAVER: 
				theFxScheduler.PlayEffect( "thermal8/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal8/shockwave", origin );
				break;
				// STONE
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF:
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 				
				case CLASS_MONMOTHA:
				case CLASS_GONK:
				case CLASS_UGNAUGHT: 
				theFxScheduler.PlayEffect( "thermal7/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal7/shockwave", origin );
				break;
				// SOUND 
				case CLASS_BIRD: 
				case CLASS_LYCAN: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_TRANDOSHAN: 
				case CLASS_HOWLER: 
				case CLASS_REBEL: 
				case CLASS_VAMPIRE_SOUND: 
				theFxScheduler.PlayEffect( "thermal6/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal6/shockwave", origin );
				break;
				// WATER 
				case CLASS_WARRIOR: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SHARK: 
				case CLASS_ALORA: 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_SWAMP:
				theFxScheduler.PlayEffect( "thermal5/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal5/shockwave", origin );
				break;
				// SUNLIGHT 
				case CLASS_ALCHEMIST:				 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GALAK:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				theFxScheduler.PlayEffect( "thermal3/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal3/shockwave", origin );
				break;
				// ELETTRICA 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_GRAN: 
				case CLASS_SABER_DROID:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT: 
				theFxScheduler.PlayEffect( "thermal9/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal9/shockwave", origin );
				break;
				// HOLY 
				case CLASS_CENOBITA: 
				case CLASS_STORMTROOPER:
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_LUKE: 
				theFxScheduler.PlayEffect( "thermal10/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal10/shockwave", origin );
				break;
				// DARKNESS 
				case CLASS_VAMPIRE: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:			 
				case CLASS_VAMPIRE_BOUNTY:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_SHADOWTROOPER:
				case CLASS_BESPIN_COP: 
				case CLASS_SABOTEUR: 
				theFxScheduler.PlayEffect( "thermal2/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal2/shockwave", origin );
				break;
				// ACIDIC 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN2: 
				case CLASS_KAIN3: 
				case CLASS_DUMAHIM: 
				case CLASS_WEEQUAY:
				case CLASS_LIZARD: 
				case CLASS_INTERROGATOR: 
				case CLASS_R2D2:
				case CLASS_R5D2: 	
				case CLASS_REELO: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:	
				case CLASS_DEMON_POISON: 
				case CLASS_BEAST:  			  
				case CLASS_GOLEM_POISON:
				case CLASS_POISON: 
				theFxScheduler.PlayEffect( "thermal/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal/shockwave", origin );
				break;
				// UNDEAD 
				case CLASS_TENTACLE: 
				case CLASS_ELDERGOD: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_NECROREAVER: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_RANCOR:
				case CLASS_MURJJ: 
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_ZOMBIE: 
				theFxScheduler.PlayEffect( "thermal11/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal11/shockwave", origin );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				theFxScheduler.PlayEffect( "thermal12/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal12/shockwave", origin );
				break;
				// BOMBA DI FUOCO 
				case CLASS_RAZIELHIM: 
				case CLASS_ESKANDOR: 
				case CLASS_HUNTER:			 
				case CLASS_DEMON_FIRE:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_FIREREAVER: 
				case CLASS_TURELIM: 
				case CLASS_FIRE: 
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_REBORN: 
				case CLASS_WAMPA: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER: 
				case CLASS_RODIAN: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "thermal4/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal4/shockwave", origin );
				break;  
				// MACE BOMB
				case CLASS_SCOUT: 
				case CLASS_MARK1: 
				case CLASS_MARK2:				 
				theFxScheduler.PlayEffect( "thermal13/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal13/shockwave", origin );
				break; 
				// PIANTE SPUNTONE
				case CLASS_IVY: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO: 
				case CLASS_PRISONER: 
				theFxScheduler.PlayEffect( "thermal14/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal14/shockwave", origin );
				break; 
				// CRISTALLI DI GHIACCIO 
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_BARTENDER:
				case CLASS_GALAKMECH: 
				theFxScheduler.PlayEffect( "thermal15/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal15/shockwave", origin );
				break; 
				// CROCI 
				case CLASS_POLTER: 
				case CLASS_JAWA:
				case CLASS_GHOST: 
				theFxScheduler.PlayEffect( "thermal16/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal16/shockwave", origin );
				break; 
				// BARA 
				case CLASS_COMMANDO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				theFxScheduler.PlayEffect( "thermal17/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal17/shockwave", origin );
				break; 
				// ANGELO 
				case CLASS_JANOS: 
				case CLASS_JAN: 
				case CLASS_ARIEL: 
				case CLASS_SARAFAN: 
				theFxScheduler.PlayEffect( "thermal18/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal18/shockwave", origin );
				break;				
			default:
				break;
				}
			}
		}
		else 
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
			 if ( cent->gent->owner->client->gs.gun->Gun_WallImpact_Effect )
				{
						theFxScheduler.PlayEffect( cent->gent->owner->client->gs.gun->Gun_WallImpact_Effect, origin, dir );
				}

				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_ARCHER: 
				case CLASS_REAVER: 
				case CLASS_MENTALIST: 
				case CLASS_JEDI:
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PROBE: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_AIRREAVER: 
				theFxScheduler.PlayEffect( "thermal8/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal8/shockwave", origin );
				break;
				// STONE
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF:
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 				
				case CLASS_MONMOTHA:
				case CLASS_GONK:
				case CLASS_UGNAUGHT: 
				theFxScheduler.PlayEffect( "thermal7/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal7/shockwave", origin );
				break;
				// SOUND 
				case CLASS_BIRD: 
				case CLASS_LYCAN: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_TRANDOSHAN: 
				case CLASS_HOWLER: 
				case CLASS_REBEL: 
				case CLASS_VAMPIRE_SOUND: 
				theFxScheduler.PlayEffect( "thermal6/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal6/shockwave", origin );
				break;
				// WATER 
				case CLASS_WARRIOR: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SHARK: 
				case CLASS_ALORA: 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_SWAMP:
				theFxScheduler.PlayEffect( "thermal5/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal5/shockwave", origin );
				break;
				// SUNLIGHT 
				case CLASS_ALCHEMIST:				 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GALAK:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				theFxScheduler.PlayEffect( "thermal3/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal3/shockwave", origin );
				break;
				// ELETTRICA 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_GRAN: 
				case CLASS_SABER_DROID:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT: 
				theFxScheduler.PlayEffect( "thermal9/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal9/shockwave", origin );
				break;
				// HOLY 
				case CLASS_CENOBITA: 
				case CLASS_STORMTROOPER:
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_LUKE: 
				theFxScheduler.PlayEffect( "thermal10/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal10/shockwave", origin );
				break;
				// DARKNESS 
				case CLASS_VAMPIRE: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:			 
				case CLASS_VAMPIRE_BOUNTY:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_SHADOWTROOPER:
				case CLASS_BESPIN_COP: 
				case CLASS_SABOTEUR: 
				theFxScheduler.PlayEffect( "thermal2/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal2/shockwave", origin );
				break;
				// ACIDIC 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN2: 
				case CLASS_KAIN3: 
				case CLASS_DUMAHIM: 
				case CLASS_WEEQUAY:
				case CLASS_LIZARD: 
				case CLASS_INTERROGATOR: 
				case CLASS_R2D2:
				case CLASS_R5D2: 	
				case CLASS_REELO: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:	
				case CLASS_DEMON_POISON: 
				case CLASS_BEAST:  			  
				case CLASS_GOLEM_POISON:
				case CLASS_POISON: 
				theFxScheduler.PlayEffect( "thermal/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal/shockwave", origin );
				break;
				// UNDEAD 
				case CLASS_TENTACLE: 
				case CLASS_ELDERGOD: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_NECROREAVER: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_RANCOR:
				case CLASS_MURJJ: 
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_ZOMBIE: 
				theFxScheduler.PlayEffect( "thermal11/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal11/shockwave", origin );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				theFxScheduler.PlayEffect( "thermal12/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal12/shockwave", origin );
				break;
				// BOMBA DI FUOCO 
				case CLASS_RAZIELHIM: 
				case CLASS_ESKANDOR: 
				case CLASS_HUNTER:			 
				case CLASS_DEMON_FIRE:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_FIREREAVER: 
				case CLASS_TURELIM: 
				case CLASS_FIRE: 
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_REBORN: 
				case CLASS_WAMPA: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER: 
				case CLASS_RODIAN: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "thermal4/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal4/shockwave", origin );
				break;  
				// MACE BOMB
				case CLASS_SCOUT: 
				case CLASS_MARK1: 
				case CLASS_MARK2:				 
				theFxScheduler.PlayEffect( "thermal13/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal13/shockwave", origin );
				break; 
				// PIANTE SPUNTONE
				case CLASS_IVY: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO: 
				case CLASS_PRISONER: 
				theFxScheduler.PlayEffect( "thermal14/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal14/shockwave", origin );
				break; 
				// CRISTALLI DI GHIACCIO 
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_BARTENDER:
				case CLASS_GALAKMECH: 
				theFxScheduler.PlayEffect( "thermal15/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal15/shockwave", origin );
				break; 
				// CROCI 
				case CLASS_POLTER: 
				case CLASS_JAWA:
				case CLASS_GHOST: 
				theFxScheduler.PlayEffect( "thermal16/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal16/shockwave", origin );
				break; 
				// BARA 
				case CLASS_COMMANDO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				theFxScheduler.PlayEffect( "thermal17/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal17/shockwave", origin );
				break; 
				// ANGELO 
				case CLASS_JANOS: 
				case CLASS_JAN: 
				case CLASS_ARIEL: 
				case CLASS_SARAFAN: 
				theFxScheduler.PlayEffect( "thermal18/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal18/shockwave", origin );
				break;				
			default:
				break;
				}
			}
		}
		//theFxScheduler.PlayEffect( "thermal/explosion", origin, dir );
		//theFxScheduler.PlayEffect( "thermal/shockwave", origin );
		break;

	case WP_EMPLACED_GUN:
		FX_EmplacedHitWall( origin, dir, (cent->gent&&cent->gent->alt_fire) );
		break;

	case WP_ATST_MAIN:
		FX_ATSTMainHitWall( origin, dir );
		break;

	case WP_ATST_SIDE:
		if ( altFire )
		{
			theFxScheduler.PlayEffect( "atst/side_alt_explosion", origin, dir );
		}
		else
		{
			theFxScheduler.PlayEffect( "atst/side_main_impact", origin, dir );
		}
		break;

	case WP_TRIP_MINE:
		theFxScheduler.PlayEffect( "tripmine/explosion", origin, dir );
		break;

	case WP_DET_PACK:
		theFxScheduler.PlayEffect( "detpack/explosion", origin, dir );
		break;

	case WP_TURRET:
		theFxScheduler.PlayEffect( "turret/wall_impact", origin, dir );
		break;

	case WP_TUSKEN_RIFLE:
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 		{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_BIRD: 
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				FX_Tusken2ShotWeaponHitWall ( origin, dir );
				break;
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				FX_Tusken3ShotWeaponHitWall ( origin, dir );
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_Tusken4ShotWeaponHitWall ( origin, dir );
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				FX_Tusken5ShotWeaponHitWall ( origin, dir );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				FX_Tusken6ShotWeaponHitWall ( origin, dir );
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				FX_Tusken7ShotWeaponHitWall ( origin, dir );
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				FX_Tusken1ShotWeaponHitWall ( origin, dir );
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				FX_Tusken8ShotWeaponHitWall ( origin, dir );
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				FX_Tusken0ShotWeaponHitWall ( origin, dir );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				FX_Tusken9ShotWeaponHitWall ( origin, dir );
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				FX_Tusken10ShotWeaponHitWall ( origin, dir );
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				FX_TuskenShotWeaponHitWall ( origin, dir );				
				break;  
			default:
				break;
				}
			}
		//FX_TuskenShotWeaponHitWall( origin, dir );
		break;

	case WP_NOGHRI_STICK:
		FX_NoghriShotWeaponHitWall( origin, dir );
		break;
	}
}

/*
-------------------------
CG_MissileHitPlayer
-------------------------
*/
extern void			G_SetViewEntity( gentity_t *self, gentity_t *viewEntity );
void CG_MissileHitPlayer( centity_t *cent, int weapon, vec3_t origin, vec3_t dir, qboolean altFire ) 
{
	gentity_t *other = NULL;
	gentity_t *self = NULL; 
	qboolean	humanoid = qtrue;
	vec3_t	pushDir;
	if ( cent->gent )
	{
		other = &g_entities[cent->gent->s.otherEntityNum];
		if( other->client )
		{
			class_t	npc_class = other->client->NPC_class;
			// check for all droids, maybe check for certain monsters if they're considered non-humanoid..?		
			if ( npc_class == CLASS_SEEKER || npc_class == CLASS_PROBE || npc_class == CLASS_MOUSE ||
				 npc_class == CLASS_GONK || npc_class == CLASS_R2D2 || npc_class == CLASS_R5D2 ||
				 npc_class == CLASS_PROTOCOL || npc_class == CLASS_MARK1 || npc_class == CLASS_MARK2 ||
				 npc_class == CLASS_INTERROGATOR || npc_class == CLASS_ATST || npc_class == CLASS_SENTRY )
			{
				humanoid = qfalse;
			}
		}
	}

	
		
	switch( weapon )
	{
	case WP_BRYAR_PISTOL:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_MENTALIST: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				FX_Bow12AltHitPlayer( origin, dir, humanoid );
				break;
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				FX_Bow4AltHitPlayer( origin, dir, humanoid );
				break; 
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				FX_Bow6AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 4000;
					other->painDebounceTime += 4000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 4000;
				}
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_Bow10AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					
				}
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				FX_Bow5AltHitPlayer( origin, dir, humanoid );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				FX_BowAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						}
				}
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				FX_Bow7AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 10;  
					other->client->holyTime = cg.time + 5000; 
				}
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				FX_Bow1AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					
				}
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				FX_Bow0AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->drainDamage = 1;  
					other->client->drainTime = cg.time + 4000; 
				}
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				FX_Bow8AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 4;  
					other->client->necroTime = cg.time + 6000; 
					other->client->ps.torsoAnimTimer += 6000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				FX_Bow11AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->hyldenDamage = 6;  
					other->client->hyldenTime = cg.time + 5000; 
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						}
				}
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				FX_Bow9AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->poisonDamage = 3;  
					other->client->poisonTime = cg.time + 4000; 
				}
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				FX_Bow3AltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->fireDamage = 2;  
					other->client->fireTime = cg.time + 1000; 
				}
				break;  
			default:
				break;
				}
			}
			
			//FX_BowAltHitPlayer( origin, dir, humanoid );


		}
		else
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_MENTALIST: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				FX_Bow12HitPlayer( origin, dir, humanoid );
				break;
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				FX_Bow4HitPlayer( origin, dir, humanoid );
				break; 
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				FX_Bow6HitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 2000;
					other->painDebounceTime += 2000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 2000;
				}
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_Bow10HitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					
				}
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				FX_Bow5HitPlayer( origin, dir, humanoid );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				FX_BowHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						}
				}
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				FX_Bow7HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 10;  
					other->client->holyTime = cg.time + 2500; 
				}
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				FX_Bow1HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					
				}
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				FX_Bow0HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->drainDamage = 1;  
					other->client->drainTime = cg.time + 2000; 
				}
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				FX_Bow8HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 4;  
					other->client->necroTime = cg.time + 3000; 
					other->client->ps.torsoAnimTimer += 3000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				FX_Bow11HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->hyldenDamage = 6;  
					other->client->hyldenTime = cg.time + 2500; 
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						}
				}
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				FX_Bow9HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->poisonDamage = 3;  
					other->client->poisonTime = cg.time + 2000; 
				}
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				FX_Bow3HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->fireDamage = 1;  
					other->client->fireTime = cg.time + 1000; 
				}
				break;  
			default:
				break;
				}
			}
			
			//FX_BowHitPlayer( origin, dir, humanoid );
		}
		break;
	case WP_BLASTER_PISTOL:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{

				if ( cent->gent->owner->client->gs.gun->Gun_Alt_FleshImpact_Effect )
				{
					if ( cent->gent->owner->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT2 ||
						cent->gent->owner->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT3	) // if rayshoot is enabled do not play this effect
					{
						//G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Alt_FleshImpact_Effect ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); 
						gi.Printf( S_COLOR_RED "i am NOT playing wall effect here! Read from WP_disruptor.cpp\n" , cent );
					}	
					else 
					{
						G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_Alt_FleshImpact_Effect, other->playerModel, other->playerModel, other->s.number, other->currentOrigin ); 
					}
				}
				// play an animation on hitted enemy 
				if ( cent->gent->owner->client->gs.gun->Gun_Alt_Hit_Animation )
						{
							NPC_SetAnim( other, SETANIM_LEGS, cent->gent->owner->client->gs.gun->Gun_Alt_Hit_Animation, SETANIM_FLAG_NORMAL );
							NPC_SetAnim( other, SETANIM_TORSO, cent->gent->owner->client->gs.gun->Gun_Alt_Hit_Animation, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
							other->client->ps.torsoAnimTimer += cent->gent->owner->client->gs.gun->Gun_Alt_PoisonTime;  
							other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
						}

				//Alt  Poison Effect 
				if ( cent->gent->owner->client->gs.gun->Gun_Alt_PoisonDamage )
				{
				 	if ( other && other->client ) 
					{
    //						other->client->gunDamage2 = cent->gent->owner->client->gs.gun->Gun_Alt_PoisonDamage; 
						// other parameters 
						// 1 - poison duration 
	/*					if ( cent->gent->owner->client->gs.gun->Gun_Alt_PoisonTime )
						{
							other->client->gunTime2 = cg.time + cent->gent->owner->client->gs.gun->Gun_Alt_PoisonTime;
						}*/
						// 2 - poison effect 
						if ( cent->gent->owner->client->gs.gun->Gun_Alt_Poison_Effect )
						{
							// play effect on head or on torso?!?! 
							G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Alt_Poison_Effect ), other->playerModel, other->playerModel, other->s.number, other->currentOrigin, cent->gent->owner->client->gs.gun->Gun_Alt_PoisonTime, qtrue ); 
						}
						// 3 - Poison Animation 
						if ( cent->gent->owner->client->gs.gun->Gun_Alt_PoisonAnim )
						{
							NPC_SetAnim( other, SETANIM_LEGS, cent->gent->owner->client->gs.gun->Gun_Alt_PoisonAnim, SETANIM_FLAG_NORMAL );
							NPC_SetAnim( other, SETANIM_TORSO, cent->gent->owner->client->gs.gun->Gun_Alt_PoisonAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
							other->client->ps.torsoAnimTimer += cent->gent->owner->client->gs.gun->Gun_Alt_PoisonTime;  // animation duration is the same of poison duration 
							other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
						}
						
					}
				}
				// stunning enemy, enemy can't shoot for time amount. 
				if ( cent->gent->owner->client->gs.gun->Gun_StunTime )
				{
					other->client->ps.weaponTime = cent->gent->owner->client->gs.gun->Gun_StunTime;
				}



				// Do electrocution 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_ALT_ELECTROCUTE )
				{
					if ( other && other->client ) 
					{
						other->s.powerups |= ( 1 << PW_SHOCKED );
						other->client->ps.powerups[PW_SHOCKED] = cg.time + cent->gent->owner->client->gs.gun->Gun_Alt_ElectrocuteDuration;
					}
				
				}
				
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_ALT_DISGREGATE )
				{
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							
							if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->Gun_Alt_Disgregate_Effect )
							{
								if ( other->ghoul2.size() && other->headBolt != -1 )
								{//FIXME: what if already playing effect?
//									G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Alt_Disgregate_Effect ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
								}
							}
						}
				
				}
				// DECLOAKS 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_ALT_DECLOAK )
				{
					if ( other && other->client ) 
					{
						Saboteur_Decloak( other, -1 );
//						Jedi_Decloak ( other ); 
			
					}
			   }
				// REMOVE SHIELDS FROM A SHIELDED ENEMY 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags&GFL_UNSHIELD )
				{
					if ( other && other->client ) 
					{
						other->flags &= ~FL_SHIELDED;
						other->client->ps.stats[STAT_ARMOR] = 0;
						other->client->ps.powerups[PW_GALAK_SHIELD] = 0;
//						gi.G2API_SetSurfaceOnOff( &other->ghoul2[other->playerModel], "force_shield", TURN_OFF );
					}
			   }
				
/*
		switch ( cent->gent->owner->client->NPC_class )
		{
				
					// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;		
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Umani: lanciano pugnale 
				// Humanity 
				case CLASS_PRISONER:	// HUMANS 
				case CLASS_RODIAN: // VAMPIRESLAYERS AND IRONGUARD 
				case CLASS_HUNTER: // Ironguard // Altfire,
				case CLASS_WARRIOR: // Generic Warrior 
		    	case CLASS_ARCHER: // Generic Archer Human sniper elite class. 
				// armature stregate
				case CLASS_MARK1:			// WITCH ARMOR BOSS CLASS 
				case CLASS_MARK2:			// WITCH ARMOR CLASS				
				case CLASS_METAL: // elementale del Metallo
				case CLASS_GOLEM_METAL: // golem del metallo 
				G_PlayEffect( "knife/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}					
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// MIETITORI: Proiettile impregnato di Tenebra
				// REAVERS OF ALLIANCE 
				case CLASS_JEDI:				// REAPER CLASS 
				case CLASS_REAVER: // STANDARD REAVER CLASSES 
				case CLASS_LIGHTREAVER:// light reaver class 
				case CLASS_DRUIDREAVER: // druid reaver class 
				case CLASS_AIRREAVER: // air reaver class act like rebel class 
				case CLASS_NAERYANMIST:	
				case CLASS_NAERYANMIST2: 
				case CLASS_NAERYANMIST3: 
				case CLASS_ICEREAVER: // ice reaver class 
				case CLASS_EARTHREAVER: // Earth reaver class. 
		 		// ombre ed elementali d'ombra
				case CLASS_DARK: // darkness elite class
				case CLASS_GOLEM_DARK: // golem tenebroso
				case CLASS_SABOTEUR: // soldato ombra 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}					
				break;
				case CLASS_RAZIEL2:// life reaver class 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_REBORN:			// VAMPIRES 
				case CLASS_VAMPIRE: // act like reborns. alliance standard vampire classes  
				case CLASS_VAMPIRE_DARK: // vae
				case CLASS_VAMPIRE_BLOOD: // air vampire class - acrobatic bleeding vampire
		   		case CLASS_VAMPIRE_WATER: // Shaar MNaik evoluted water vampire class with water resistance. 
				case CLASS_VAMPIRE_ELF: // vampira elfa 
				case CLASS_VAMPIRE_ELF2: // Elf Vampire blue armor 
				case CLASS_VAMPIRE_SOUND: // bard vampire class. 
				case CLASS_VAMPIRE_BOUNTY: // Half vampire - dumahim
				case CLASS_VAMPIRE_ARCHER: // Archer - light vampire
				case CLASS_UMAH: // Vorador daughter 
				case CLASS_KAIN2: // Young Kain 
				case CLASS_VAMPIRE_NECRO: // Soul Liutenant  
				case CLASS_VAMPIRE_ZOMBIE: // half vampire - melchiahim 
				case CLASS_VAMPIRE_DEMON:// Samah'el - turelim boss classes 
				case CLASS_VAMPIRE_LYCAN: // werewolf Vampire Class. 
				case CLASS_VAMPIRE_SARAFAN:  // joachim 
				case CLASS_VAMPIRE_FEAR: // he's a trickster fire vampire class - vampire of fear. 
				case CLASS_HALFVAMPIRE: // Half human \ sarafan vampire 
				// Vampiri Corrotti 
				// MELCHIAHIM
				case CLASS_TUSKEN:	// MELCHIAHIM youngs
				case CLASS_MELC: // dive like sand creature, fight like a zombie, eat like a vampire. 
				// DUMAHIM
				case CLASS_WEEQUAY:	// Dumahim, young warrior vampires 
				case CLASS_DUMAHIM: // dumahim expert vampires 
				case CLASS_SNEAKER: // dumahim assassins 
				// RAHABIM
				case CLASS_SWAMPTROOPER:		// YOUNG RAHABIM VAMPIRES 
				case CLASS_ALORA:			// RAHABIM SPECIAL CLASS - Sirens
				case CLASS_RAHABIM:				// immune to water, weak to fire, deadly to light, act like swamptrooper class
				// TURELIM
				// Vampires e Reapers 
				case CLASS_GRAN:	// young TURELIM VAMPIRES
				case CLASS_TURELIM:// adult turelim. act like gran class, weak to sound, resistance to fire- 
				case CLASS_TYRANT: // Turelim elite fight class 
				// Razielhim
				case CLASS_FLIER2: // razielhim young vampires
				case CLASS_RAZIELHIM: // razielhim vampires
				case CLASS_SENTINEL: // razielhim elite class
	     		G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				// Old Vampires and guardians deals more damage. 
				case CLASS_NECROMANCER: // Vampire Necromancer
				case CLASS_VAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_VAMPIRE_NECRO2:// Deathmask with full powers 
				case CLASS_KYLE:	// VAMPIRE BOSS CLASS (KAIN)
				case CLASS_KAIN:// Kain with full powers
				case CLASS_VAMPIRE_ELF3: // Elf Vampire blue armor rainbow lion class - dimension guardian 
				// Conflict Guardians 
				case CLASS_VAMPIRE_FROST: // for Kainh conflict guardian with frost powers. 
				case CLASS_VAMPIRE_FEAR2: // classe phobos knight of fear infernale 
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/flesh_impact" , origin, dir  );  
			if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// WOLF, BAT AND MIST FORM DO NOT SHOOT MISSILE. 
				case CLASS_WOLFFORM: // for vampires mutated into wolves
				case CLASS_BATFORM: // for vampires mutated into bats 
				case CLASS_WOLFKAIN2:// Kain BO2
				case CLASS_BATKAIN2: // Kain bo2 bat form 
				case CLASS_BATVAMPIRE_WATER: // Shaar Naik Bat form
				case CLASS_BATVAMPIRE_ELF: // Elf Vampire bat form 
				case CLASS_BATVAMPIRE_ELF2: // Elf Vampire khyroptera form 
				case CLASS_WOLFVAMPIRE_ELF: // evoluted elf vampire class with earth connection. pretty strong. 
				case CLASS_LIONVAMPIRE_ELF: // Elf Vampire blue armor 
				case CLASS_HELLWOLF: // Samah'el metalupo 
				case CLASS_HELLBAT: // Samah'el Colibri
				case CLASS_WOLFVAMPIRE_LYCAN: // werewolf Vampire Class. 
		     	case CLASS_WOLFVAMPIRE_FEAR: // fire vampire class - vampire of fear. 
				case CLASS_BATVAMPIRE_FEAR: // Phobos bat form 
				case CLASS_WOLFHALFVAMPIRE: // Half human \ sarafan vampire 
		   		case CLASS_WOLFNECROMANCER: // Vampire Necromancer 
				case CLASS_WOLFVAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_BATNECROMANCER: // Soul Bat form 
				case CLASS_BATVAMPIRE_QUEEN: // Sheer Bat form 
				case CLASS_WOLFKAIN:// Kain with full powers
				case CLASS_WOLFKAIN3:// Kain Sion of balance 
				case CLASS_WOLFVORADOR: // vorador class 
				case CLASS_WOLFJANOS: // janos audron class 
				case CLASS_BATKAIN: // Kain Bat form
				case CLASS_BATVORADOR: // Vorador Bat form 
				case CLASS_BATKAIN3: // kain prophecy bat form 
				case CLASS_BATVAMPIRE_ELF3: // Elf Vampire khyroptera form full power. 
				case CLASS_LIONVAMPIRE_ELF2: // Elf Vampire blue armor lion Elf Vampire with rainbows - Dimensione Guardian
				case CLASS_WOLFVAMPIRE_FEAR2: // classe phobos knight of fear infernale, Conflict Guardian
				case CLASS_BATVAMPIRE_FEAR2: // Phobos demonic bat form 
				case CLASS_BATVAMPIRE_FROST: // Kainh bat form 
				// MIST FORM // - NON ATTACCANO
				case CLASS_MISTFORM: // for vampires mutated into fog 
				case CLASS_MISTVAMPIRE_DARK: // Mist vae 
				case CLASS_MISTKAIN2:// Mist Kain BO2 
				case CLASS_MISTVAMPIRE_WATER:// shaar naik mist form 
				case CLASS_MISTVAMPIRE_BLOOD: // Mist osil
				case CLASS_MISTVAMPIRE_ELF:// Elf Vampire basic mist form
				case CLASS_MISTVAMPIRE_ELF2: // Elf Vampire Iridu mist form
				case CLASS_HELLMIST: // samah'el mist form 
		 		case CLASS_MISTVAMPIRE_SARAFAN:  // joachim 
				case CLASS_MISTVAMPIRE_FEAR: // mist phobos 
		 		case CLASS_MISTNECROMANCER: // Mist soul 
				case CLASS_MISTVAMPIRE_QUEEN: // Mist sheer 
		  		case CLASS_MISTKAIN:// Mist default kain 
				case CLASS_MISTVORADOR: // Mist vorador 
				case CLASS_MISTJANOS: // Mist janos audron 
				case CLASS_MISTKAIN3: // Mist Kain prophecy
				case CLASS_MISTVAMPIRE_ELF3:// Elf Vampire paladine mist form
				case CLASS_MISTVAMPIRE_FEAR2:// Mist phobos fear knight 
				{
					return; 
				}
				break;
				// Altri Psichici
				case CLASS_MENTALIST: 
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/flesh_impact" , origin, dir  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/flesh_impact" , origin, dir  );  
				break;
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	G_PlayEffect( "time/flesh_impact" , origin, dir  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// ELEMENTALI 
				// i Golem di terra posson lanciar un proiettile di polvere
				case CLASS_GONK:				// GOLEM AND EARTH ELEMENTAL CREATURES 
				case CLASS_EARTH:// earth elemental class 
				case CLASS_GOLEM: // golem della terra 
				case CLASS_CRYSTAL: // Elementali del Cristallo
				case CLASS_GOLEM_CRYSTAL: // golem di cristallo 
				case CLASS_UGNAUGHT: // MUD MONSTERS, BLOBS, 
				case CLASS_MUD:// elementale di fango
				case CLASS_STONE:// elementale di pietra
				case CLASS_GOLEM_MUD: // golem di fango
				case CLASS_GOLEM_STONE: // golem di pietra
				case CLASS_DUST:
				case CLASS_GOLEM_DUST:
				G_PlayEffect( "dust/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_SONIC: // elementale sonico
				case CLASS_GOLEM_SONIC:// golem di suono
				// Asgarath con l'astrale 
				case CLASS_LANDO:	// ASTRAL CLASS 
				// I golem d'astrale 
				case CLASS_ASTRAL: // Elementali Astrali
				case CLASS_GOLEM_ASTRAL: // Golem Astrale
				case CLASS_SILENCE:
				case CLASS_GOLEM_SILENCE:
				case CLASS_HOWLER:			// SONIC CREATURE CLASS 
				G_PlayEffect( "silence/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// gli elementali di Nebbia 
				case CLASS_FOG: // elementali della Nebbia
	            case CLASS_GOLEM_FOG: // golem della nebbia 
		        case CLASS_REBEL:			// FOG CLASS 
				case CLASS_ROCKETTROOPER:	// STORM AND FLYING CREATURES 
				// I Golem d'Aria, lanceranno deboli attacchi di Vuoto.
				case CLASS_AIR: // wind fog class  
	     		case CLASS_GOLEM_AIR: // golem dell'aria 
			  	case CLASS_SABER_DROID:		// LIGHTNING GOLEM CLASS 
			    case CLASS_LIGHTNING: // Elementali del Fulmine
			    case CLASS_GOLEM_LIGHTNING: // golem del fulmine
				case CLASS_STORM:// elementale di tempesta
				case CLASS_GOLEM_STORM: // golem di tempesta
		       	G_PlayEffect( "void/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Il golem del fuoco e i raggianti del fuoco, lanciano deboli attacchi di fumo 
				case CLASS_FIRE: // fire-warm element class 
				case CLASS_GOLEM_FIRE: // golem di fuoco
			    case CLASS_WARM: // Elementali di calore 
				case CLASS_GOLEM_WARM: // golem del silenzio
				case CLASS_HAZARD_TROOPER:	// WARM AND FIRE CREATURES 
				case CLASS_SMOKE:
				case CLASS_GOLEM_SMOKE:
				case CLASS_BESPIN_COP:
				G_PlayEffect( "smoke/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_WATER:// water class 
				case CLASS_GOLEM_WATER: // golem dell'acqua
				case CLASS_GOLEM_SALT: // golem di sale 
				case CLASS_SALT: // elementali del Sale
				case CLASS_GALAKMECH:		// CRYSTAL MONSTER 
				case CLASS_GOLEM_ICE: // golem di ghiaccio
				case CLASS_ICE: // elementale di ghiaccio
				case CLASS_WAVE: // elementale delle maree
				case CLASS_GOLEM_WAVE: // golem delle maree
				// Sale e Silenzio hanno la tenebra 
				G_PlayEffect( "salt/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Pistola Glifica Fumogena 
				case CLASS_IMPWORKER: // ALCHEMIST AND WIZARD CLASS 	
				case CLASS_ALCHEMIST:  // Alchemist altfire, switch,
				// costrutti tecnomagici 
				case CLASS_REMOTE:			// ARTIFACT SHOOTER ARROW TRAP
				case CLASS_SEEKER:			// SEEKER ARTIFACTS
				case CLASS_SENTRY:			// SENTRY ARTIFACT
				case CLASS_R2D2:				// MECHANICAL GOLEM CLASS 
				case CLASS_R5D2:				// MECHANICAL GOLEM CLASS 2
				G_PlayEffect( "smoker/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/flesh_impact" , origin, dir  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
			if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}		break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/flesh_impact" , origin, dir );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/flesh_impact" , origin, dir );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
			default:
				break;			

				/*

					
					
					
					// SMOKE
				case CLASS_BESPIN_COP:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_TANKREAVER: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_SPIDERREAVER3: 
				case CLASS_REELO:
				case CLASS_CLAW:
				case CLASS_GOLEM_LAVA: 
				case CLASS_HAZARD_TROOPER:
				case CLASS_GOLEM_FIRE: 
				case CLASS_FIRE: 
				case CLASS_UGNAUGHT: 
				case CLASS_DEMON_FIRE: 
				case CLASS_WAMPA: 
				case CLASS_HELLGOLEM: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON:
				case CLASS_IVY: 
				case CLASS_FIREREAVER:
				case CLASS_LYCAN: 
				FX_SmokeAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 3000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					//other->client->poisonDamage = 10; 
					//other->client->poisonTime = cg.time + 2000;
				}	
				break;
				// VOID 
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_PROBE: 
				case CLASS_POLTER: 
				FX_VoidAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// STUN 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_MONMOTHA: 
				case CLASS_TUSKEN:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_DECEIVER: 
				case CLASS_ZEPH: 
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM: 
				case CLASS_RAHABIM: 
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_GRAN:
				case CLASS_VAMPIRE:   
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_KAIN:
				case CLASS_KAIN2:  
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR: 
				case CLASS_JANOS:  
				case CLASS_NECROMANCER:  
				case CLASS_VAMPIRE_QUEEN:  
				case CLASS_VAMPIRE_DARK:  
				case CLASS_VAMPIRE_NECRO:   
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_VAMPIRE_FEAR:  
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN:  
				case CLASS_VAMPIRE_ELF:  
				case CLASS_VAMPIRE_ELF2:  
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_VAMPIRE_WATER:  
				case CLASS_VAMPIRE_FROST: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE:
				case CLASS_RAZIELHIM:  
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_VAMPIRE_GHOST: 
				FX_StunAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					gi.Printf( S_COLOR_RED, "Hitted by Psychic Damage" );
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 3000; 
				}	
				break;
				// KNIFE
				case CLASS_PRISONER:
				case CLASS_SCOUT: 
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_ARCHER: 
				case CLASS_SARAFAN: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_GALAK: 
				case CLASS_LIZARD:
				case CLASS_JAN: 
				FX_KnifeAltHitPlayer( origin, dir, humanoid );
				break; 
				// ABSORB
				case CLASS_GLIDER:
				case CLASS_JAWA:
				case CLASS_GHOST: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_ARIEL: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_MURJJ:
				case CLASS_MINEMONSTER:
				case CLASS_DEMON_BLADE:
				case CLASS_DEMON_POISON:
				case CLASS_DEMON_ELECTRO:
				case CLASS_ATST:
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE:
				case CLASS_PROPHET:
				case CLASS_CENOBITA:
				FX_AbsorbAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 3000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->necroDamage = 12; 
					other->client->necroTime = cg.time + 3000;
				}	
				break;
				// ENTROPY 
				case CLASS_ALCHEMIST:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION: 
				case CLASS_SENTRY: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_SHARK: 
				FX_EntropyAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client)
				{
					other->client->iceDamage = 12; 
					other->client->iceTime = cg.time + 3000;
					G_ClearEnemy( other	);
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 5000;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime = level.time + 5000;
				}
				break; 
				// TENEBRE...
				case CLASS_DRUID: 
				case CLASS_WIZARD:  
				case CLASS_BEAST: 
				case CLASS_BIRD:
				case CLASS_REAVER: 
				case CLASS_RAZIEL: 
				case CLASS_NECROREAVER: 
				case CLASS_DARKREAVER: 
				case CLASS_LIGHTREAVER: 
				case CLASS_DRUIDREAVER: 
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 				
				case CLASS_BARTENDER:
				case CLASS_COMMANDO:
				case CLASS_THRALL: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_RANCOR: 
				case CLASS_IMPERIAL: 
				case CLASS_IMPWORKER: 				
				case CLASS_INTERROGATOR:
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_GALAKMECH: 
				case CLASS_MORGANKATARN: 
				case CLASS_PROTOCOL: 
				case CLASS_R2D2: 
				case CLASS_R5D2:
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_SABOTEUR: 
				case CLASS_STORMTROOPER: 
				case CLASS_SWAMP: 
				case CLASS_NOGHRI: 
				case CLASS_TRANDOSHAN: 
				case CLASS_WEEQUAY: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_SABER_DROID: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DARK: 
				case CLASS_LIGHT: 
				case CLASS_WATER: 
				case CLASS_AIR: 
				case CLASS_EARTH: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_AIR: 
				case CLASS_GOLEM_METAL: 
				case CLASS_GOLEM: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_GOLEM_HOLY: 
				FX_DarkAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 3000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}		
				break; 
				case CLASS_FIGHTER: 
				FX_StunnerAltHitPlayer( origin, dir, humanoid );
				break;*/
			//default:
				//break;
			}
			
		}
		else 
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{


				 if ( cent->gent->owner->client->gs.gun->Gun_FleshImpact_Effect )
				{
					if ( cent->gent->owner->client->gs.gun->gunFlags&GFL_RAYSHOOT ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_RAYSHOOT2 ||
						 cent->gent->owner->client->gs.gun->gunFlags&GFL_RAYSHOOT3	) // if rayshoot is enabled do not play this effect
					{
						//G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Alt_FleshImpact_Effect ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); 
						gi.Printf( S_COLOR_RED "i am NOT playing flesh impact effect here! Read from WP_disruptor.cpp\n" , cent );
					}	
					else 
					{ 
						G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_FleshImpact_Effect, other->playerModel, other->playerModel, other->s.number, other->currentOrigin ); 
					}
				}
				if ( cent->gent->owner->client->gs.gun->Gun_Hit_Animation )
						{
							NPC_SetAnim( other, SETANIM_LEGS, cent->gent->owner->client->gs.gun->Gun_Hit_Animation, SETANIM_FLAG_NORMAL );
							NPC_SetAnim( other, SETANIM_TORSO, cent->gent->owner->client->gs.gun->Gun_Hit_Animation, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
							other->client->ps.torsoAnimTimer += cent->gent->owner->client->gs.gun->Gun_PoisonTime;  
							other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
						}

				 // Poison Effect 
				if ( cent->gent->owner->client->gs.gun->Gun_PoisonDamage )
				{
				 	if ( other && other->client ) 
					{
//						other->client->gunDamage = cent->gent->owner->client->gs.gun->Gun_PoisonDamage; 
						// other parameters 
						// 1 - poison duration 
						if ( cent->gent->owner->client->gs.gun->Gun_PoisonTime )
						{
	//						other->client->gunTime = cg.time + cent->gent->owner->client->gs.gun->Gun_PoisonTime;
						}
						// 2 - poison effect 
						if ( cent->gent->owner->client->gs.gun->Gun_Poison_Effect )
						{
							// play effect on head or on torso?!?! 
							G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Alt_Poison_Effect ), other->playerModel, other->playerModel, other->s.number, other->currentOrigin, cent->gent->owner->client->gs.gun->Gun_PoisonTime, qtrue ); 
						}
						// 3 - Poison Animation 
						if ( cent->gent->owner->client->gs.gun->Gun_PoisonAnim )
						{
							NPC_SetAnim( other, SETANIM_LEGS, cent->gent->owner->client->gs.gun->Gun_PoisonAnim, SETANIM_FLAG_NORMAL );
							NPC_SetAnim( other, SETANIM_TORSO, cent->gent->owner->client->gs.gun->Gun_PoisonAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
							other->client->ps.torsoAnimTimer += cent->gent->owner->client->gs.gun->Gun_PoisonTime;  // animation duration is the same of poison duration 
							other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
						}
						
					}
				}
				// stunning enemy, enemy can't shoot for time amount. 
				if ( cent->gent->owner->client->gs.gun->Gun_StunTime )
				{
					other->client->ps.weaponTime = cent->gent->owner->client->gs.gun->Gun_StunTime;
				}
				// Do electrocution 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_ELECTROCUTE )
				{
					if ( other && other->client ) 
					{
						other->s.powerups |= ( 1 << PW_SHOCKED );
						other->client->ps.powerups[PW_SHOCKED] = cg.time + cent->gent->owner->client->gs.gun->Gun_ElectrocuteDuration;
					}
				
				}

				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_DISGREGATE )
				{
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect )
							{
								if ( other->ghoul2.size() && other->headBolt != -1 )
								{//FIXME: what if already playing effect?
//									G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
								}
							}
						}
				
				}
				// DECLOAK 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_DECLOAK )
				{
					if ( other && other->client ) 
					{
						Saboteur_Decloak( other, -1 );
					//	Jedi_Decloak ( other ); 
						
					}
			   }
				// REMOVE SHIELDS FROM A SHIELDED ENEMY, and also remove cloaking. 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags&GFL_UNSHIELD )
				{
					if ( other && other->client ) 
					{
						other->flags &= ~FL_SHIELDED;
						other->client->ps.stats[STAT_ARMOR] = 0;
						other->client->ps.powerups[PW_GALAK_SHIELD] = 0;
//						gi.G2API_SetSurfaceOnOff( &other->ghoul2[other->playerModel], "force_shield", TURN_OFF );
						
						
					}
			   }
			// DO DISMEMBERMENT IF ENEMY IS KILLED! 
				if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->gunFlags2&GFL_DISMEMBER )
				{
					// IF I USE MOD_SABER, WEAPON DEALS LOW DAMAGE BUT APPLY DAMAGE.
					// IF I USE OTHER MODS, WEAPON DEALS DAMAGE BUT NOT DO DETACHINGS!
					/*if (  other &&  other->client && other->health <= 20 )
						{
							// enemy lost left hand! 
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1, HL_HAND_LT, qtrue );
							if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect )
							{	
								// if is possible play also an effect from detached part 
								if ( other->ghoul2.size() && other->headBolt != -1 )
								{//FIXME: what if already playing effect?
									G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect ), other->playerModel, other->handLBolt, other->s.number, other->currentOrigin ); 
								}
							}
						}
					else if (  other &&  other->client && other->health <= 10 )
						{
							// enemy lost right hand! 
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1, HL_HAND_RT, qtrue );
							if ( cent->gent && cent->gent->owner && cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect )
							{	
								// if is possible play also an effect from detached part 
								if ( other->ghoul2.size() && other->headBolt != -1 )
								{//FIXME: what if already playing effect?
									G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect ), other->playerModel, other->handRBolt, other->s.number, other->currentOrigin ); 
								}
							}
					}*/
					
					//G_Damage( other, cent, cent, origin, dir, cent->gent->client->gs.gun->Gun_Damage, DAMAGE_DISMEMBER, cent->gent->client->gs.gun->Gun_MOD, hitLoc );
					/*if (  other &&  other->client && other->health <= 0 )
						{
							// broken enemy into pieces! 
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_HEAD, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_WAIST, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_HAND_RT, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_HAND_LT, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_ARM_RT, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_ARM_LT, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_LEG_RT, qtrue );
							G_DoDismemberment( other, other->currentOrigin, MOD_SABER, 1000, HL_LEG_LT, qtrue );
							// if is possible play also an effect from detached part 
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								
								G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Disgregate_Effect ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); 
							}
					
						}*/
				
				}


				/*switch ( cent->gent->owner->client->NPC_class )
				{
										// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;		
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Umani: lanciano pugnale 
				// Humanity 
				case CLASS_PRISONER:	// HUMANS 
				case CLASS_RODIAN: // VAMPIRESLAYERS AND IRONGUARD 
				case CLASS_HUNTER: // Ironguard // Altfire,
				case CLASS_WARRIOR: // Generic Warrior 
		    	case CLASS_ARCHER: // Generic Archer Human sniper elite class. 
				// armature stregate
				case CLASS_MARK1:			// WITCH ARMOR BOSS CLASS 
				case CLASS_MARK2:			// WITCH ARMOR CLASS				
				case CLASS_METAL: // elementale del Metallo
				case CLASS_GOLEM_METAL: // golem del metallo 
				G_PlayEffect( "knife/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}					
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// MIETITORI: Proiettile impregnato di Tenebra
				// REAVERS OF ALLIANCE 
				case CLASS_JEDI:				// REAPER CLASS 
				case CLASS_REAVER: // STANDARD REAVER CLASSES 
				case CLASS_LIGHTREAVER:// light reaver class 
				case CLASS_DRUIDREAVER: // druid reaver class 
				case CLASS_AIRREAVER: // air reaver class act like rebel class 
				case CLASS_NAERYANMIST:	
				case CLASS_NAERYANMIST2: 
				case CLASS_NAERYANMIST3: 
				case CLASS_ICEREAVER: // ice reaver class 
				case CLASS_EARTHREAVER: // Earth reaver class. 
		 		// ombre ed elementali d'ombra
				case CLASS_DARK: // darkness elite class
				case CLASS_GOLEM_DARK: // golem tenebroso
				case CLASS_SABOTEUR: // soldato ombra 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}					
				break;
				case CLASS_RAZIEL2:// life reaver class 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				case CLASS_REBORN:			// VAMPIRES 
				case CLASS_VAMPIRE: // act like reborns. alliance standard vampire classes  
				case CLASS_VAMPIRE_DARK: // vae
				case CLASS_VAMPIRE_BLOOD: // air vampire class - acrobatic bleeding vampire
		   		case CLASS_VAMPIRE_WATER: // Shaar MNaik evoluted water vampire class with water resistance. 
				case CLASS_VAMPIRE_ELF: // vampira elfa 
				case CLASS_VAMPIRE_ELF2: // Elf Vampire blue armor 
				case CLASS_VAMPIRE_SOUND: // bard vampire class. 
				case CLASS_VAMPIRE_BOUNTY: // Half vampire - dumahim
				case CLASS_VAMPIRE_ARCHER: // Archer - light vampire
				case CLASS_UMAH: // Vorador daughter 
				case CLASS_KAIN2: // Young Kain 
				case CLASS_VAMPIRE_NECRO: // Soul Liutenant  
				case CLASS_VAMPIRE_ZOMBIE: // half vampire - melchiahim 
				case CLASS_VAMPIRE_DEMON:// Samah'el - turelim boss classes 
				case CLASS_VAMPIRE_LYCAN: // werewolf Vampire Class. 
				case CLASS_VAMPIRE_SARAFAN:  // joachim 
				case CLASS_VAMPIRE_FEAR: // he's a trickster fire vampire class - vampire of fear. 
				case CLASS_HALFVAMPIRE: // Half human \ sarafan vampire 
				// Vampiri Corrotti 
				// MELCHIAHIM
				case CLASS_TUSKEN:	// MELCHIAHIM youngs
				case CLASS_MELC: // dive like sand creature, fight like a zombie, eat like a vampire. 
				// DUMAHIM
				case CLASS_WEEQUAY:	// Dumahim, young warrior vampires 
				case CLASS_DUMAHIM: // dumahim expert vampires 
				case CLASS_SNEAKER: // dumahim assassins 
				// RAHABIM
				case CLASS_SWAMPTROOPER:		// YOUNG RAHABIM VAMPIRES 
				case CLASS_ALORA:			// RAHABIM SPECIAL CLASS - Sirens
				case CLASS_RAHABIM:				// immune to water, weak to fire, deadly to light, act like swamptrooper class
				// TURELIM
				// Vampires e Reapers 
				case CLASS_GRAN:	// young TURELIM VAMPIRES
				case CLASS_TURELIM:// adult turelim. act like gran class, weak to sound, resistance to fire- 
				case CLASS_TYRANT: // Turelim elite fight class 
				// Razielhim
				case CLASS_FLIER2: // razielhim young vampires
				case CLASS_RAZIELHIM: // razielhim vampires
				case CLASS_SENTINEL: // razielhim elite class
	     		G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				// Old Vampires and guardians deals more damage. 
				case CLASS_NECROMANCER: // Vampire Necromancer
				case CLASS_VAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_VAMPIRE_NECRO2:// Deathmask with full powers 
				case CLASS_KYLE:	// VAMPIRE BOSS CLASS (KAIN)
				case CLASS_KAIN:// Kain with full powers
				case CLASS_VAMPIRE_ELF3: // Elf Vampire blue armor rainbow lion class - dimension guardian 
				// Conflict Guardians 
				case CLASS_VAMPIRE_FROST: // for Kainh conflict guardian with frost powers. 
				case CLASS_VAMPIRE_FEAR2: // classe phobos knight of fear infernale 
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/flesh_impact" , origin, dir  );  
			if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// WOLF, BAT AND MIST FORM DO NOT SHOOT MISSILE. 
				case CLASS_WOLFFORM: // for vampires mutated into wolves
				case CLASS_BATFORM: // for vampires mutated into bats 
				case CLASS_WOLFKAIN2:// Kain BO2
				case CLASS_BATKAIN2: // Kain bo2 bat form 
				case CLASS_BATVAMPIRE_WATER: // Shaar Naik Bat form
				case CLASS_BATVAMPIRE_ELF: // Elf Vampire bat form 
				case CLASS_BATVAMPIRE_ELF2: // Elf Vampire khyroptera form 
				case CLASS_WOLFVAMPIRE_ELF: // evoluted elf vampire class with earth connection. pretty strong. 
				case CLASS_LIONVAMPIRE_ELF: // Elf Vampire blue armor 
				case CLASS_HELLWOLF: // Samah'el metalupo 
				case CLASS_HELLBAT: // Samah'el Colibri
				case CLASS_WOLFVAMPIRE_LYCAN: // werewolf Vampire Class. 
		     	case CLASS_WOLFVAMPIRE_FEAR: // fire vampire class - vampire of fear. 
				case CLASS_BATVAMPIRE_FEAR: // Phobos bat form 
				case CLASS_WOLFHALFVAMPIRE: // Half human \ sarafan vampire 
		   		case CLASS_WOLFNECROMANCER: // Vampire Necromancer 
				case CLASS_WOLFVAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_BATNECROMANCER: // Soul Bat form 
				case CLASS_BATVAMPIRE_QUEEN: // Sheer Bat form 
				case CLASS_WOLFKAIN:// Kain with full powers
				case CLASS_WOLFKAIN3:// Kain Sion of balance 
				case CLASS_WOLFVORADOR: // vorador class 
				case CLASS_WOLFJANOS: // janos audron class 
				case CLASS_BATKAIN: // Kain Bat form
				case CLASS_BATVORADOR: // Vorador Bat form 
				case CLASS_BATKAIN3: // kain prophecy bat form 
				case CLASS_BATVAMPIRE_ELF3: // Elf Vampire khyroptera form full power. 
				case CLASS_LIONVAMPIRE_ELF2: // Elf Vampire blue armor lion Elf Vampire with rainbows - Dimensione Guardian
				case CLASS_WOLFVAMPIRE_FEAR2: // classe phobos knight of fear infernale, Conflict Guardian
				case CLASS_BATVAMPIRE_FEAR2: // Phobos demonic bat form 
				case CLASS_BATVAMPIRE_FROST: // Kainh bat form 
				// MIST FORM // - NON ATTACCANO
				case CLASS_MISTFORM: // for vampires mutated into fog 
				case CLASS_MISTVAMPIRE_DARK: // Mist vae 
				case CLASS_MISTKAIN2:// Mist Kain BO2 
				case CLASS_MISTVAMPIRE_WATER:// shaar naik mist form 
				case CLASS_MISTVAMPIRE_BLOOD: // Mist osil
				case CLASS_MISTVAMPIRE_ELF:// Elf Vampire basic mist form
				case CLASS_MISTVAMPIRE_ELF2: // Elf Vampire Iridu mist form
				case CLASS_HELLMIST: // samah'el mist form 
		 		case CLASS_MISTVAMPIRE_SARAFAN:  // joachim 
				case CLASS_MISTVAMPIRE_FEAR: // mist phobos 
		 		case CLASS_MISTNECROMANCER: // Mist soul 
				case CLASS_MISTVAMPIRE_QUEEN: // Mist sheer 
		  		case CLASS_MISTKAIN:// Mist default kain 
				case CLASS_MISTVORADOR: // Mist vorador 
				case CLASS_MISTJANOS: // Mist janos audron 
				case CLASS_MISTKAIN3: // Mist Kain prophecy
				case CLASS_MISTVAMPIRE_ELF3:// Elf Vampire paladine mist form
				case CLASS_MISTVAMPIRE_FEAR2:// Mist phobos fear knight 
				{
					return; 
				}
				break;
				// Altri Psichici
				case CLASS_MENTALIST: 
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/flesh_impact" , origin, dir  );  
				break;
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/flesh_impact" , origin, dir  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/flesh_impact" , origin, dir  );  
				break;
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	;
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	G_PlayEffect( "time/flesh_impact" , origin, dir  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// ELEMENTALI 
				// i Golem di terra posson lanciar un proiettile di polvere
				case CLASS_GONK:				// GOLEM AND EARTH ELEMENTAL CREATURES 
				case CLASS_EARTH:// earth elemental class 
				case CLASS_GOLEM: // golem della terra 
				case CLASS_CRYSTAL: // Elementali del Cristallo
				case CLASS_GOLEM_CRYSTAL: // golem di cristallo 
				case CLASS_UGNAUGHT: // MUD MONSTERS, BLOBS, 
				case CLASS_MUD:// elementale di fango
				case CLASS_STONE:// elementale di pietra
				case CLASS_GOLEM_MUD: // golem di fango
				case CLASS_GOLEM_STONE: // golem di pietra
				case CLASS_DUST:
				case CLASS_GOLEM_DUST:
				G_PlayEffect( "dust/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_SONIC: // elementale sonico
				case CLASS_GOLEM_SONIC:// golem di suono
				// Asgarath con l'astrale 
				case CLASS_LANDO:	// ASTRAL CLASS 
				// I golem d'astrale 
				case CLASS_ASTRAL: // Elementali Astrali
				case CLASS_GOLEM_ASTRAL: // Golem Astrale
				case CLASS_SILENCE:
				case CLASS_GOLEM_SILENCE:
				case CLASS_HOWLER:			// SONIC CREATURE CLASS 
				G_PlayEffect( "silence/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// gli elementali di Nebbia 
				case CLASS_FOG: // elementali della Nebbia
	            case CLASS_GOLEM_FOG: // golem della nebbia 
		        case CLASS_REBEL:			// FOG CLASS 
				case CLASS_ROCKETTROOPER:	// STORM AND FLYING CREATURES 
				// I Golem d'Aria, lanceranno deboli attacchi di Vuoto.
				case CLASS_AIR: // wind fog class  
	     		case CLASS_GOLEM_AIR: // golem dell'aria 
			  	case CLASS_SABER_DROID:		// LIGHTNING GOLEM CLASS 
			    case CLASS_LIGHTNING: // Elementali del Fulmine
			    case CLASS_GOLEM_LIGHTNING: // golem del fulmine
				case CLASS_STORM:// elementale di tempesta
				case CLASS_GOLEM_STORM: // golem di tempesta
		       	G_PlayEffect( "void/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Il golem del fuoco e i raggianti del fuoco, lanciano deboli attacchi di fumo 
				case CLASS_FIRE: // fire-warm element class 
				case CLASS_GOLEM_FIRE: // golem di fuoco
			    case CLASS_WARM: // Elementali di calore 
				case CLASS_GOLEM_WARM: // golem del silenzio
				case CLASS_HAZARD_TROOPER:	// WARM AND FIRE CREATURES 
				case CLASS_SMOKE:
				case CLASS_GOLEM_SMOKE:
				case CLASS_BESPIN_COP:
				G_PlayEffect( "smoke/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_WATER:// water class 
				case CLASS_GOLEM_WATER: // golem dell'acqua
				case CLASS_GOLEM_SALT: // golem di sale 
				case CLASS_SALT: // elementali del Sale
				case CLASS_GALAKMECH:		// CRYSTAL MONSTER 
				case CLASS_GOLEM_ICE: // golem di ghiaccio
				case CLASS_ICE: // elementale di ghiaccio
				case CLASS_WAVE: // elementale delle maree
				case CLASS_GOLEM_WAVE: // golem delle maree
				// Sale e Silenzio hanno la tenebra 
				G_PlayEffect( "salt/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Pistola Glifica Fumogena 
				case CLASS_IMPWORKER: // ALCHEMIST AND WIZARD CLASS 	
				case CLASS_ALCHEMIST:  // Alchemist altfire, switch,
				// costrutti tecnomagici 
				case CLASS_REMOTE:			// ARTIFACT SHOOTER ARROW TRAP
				case CLASS_SEEKER:			// SEEKER ARTIFACTS
				case CLASS_SENTRY:			// SENTRY ARTIFACT
				case CLASS_R2D2:				// MECHANICAL GOLEM CLASS 
				case CLASS_R5D2:				// MECHANICAL GOLEM CLASS 2
				G_PlayEffect( "smoker/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/flesh_impact" , origin, dir  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/flesh_impact" , origin, dir  );  
			if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}		break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/flesh_impact" , origin, dir );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/flesh_impact" , origin, dir  );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/flesh_impact" , origin, dir );  
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 2000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;

				/*


				// SMOKE
				case CLASS_BESPIN_COP:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_TANKREAVER: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_SPIDERREAVER3: 
				case CLASS_REELO:
				case CLASS_CLAW:
				case CLASS_GOLEM_LAVA: 
				case CLASS_HAZARD_TROOPER:
				case CLASS_GOLEM_FIRE: 
				case CLASS_FIRE: 
				case CLASS_UGNAUGHT: 
				case CLASS_DEMON_FIRE: 
				case CLASS_WAMPA: 
				case CLASS_HELLGOLEM: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON:
				case CLASS_IVY: 
				case CLASS_FIREREAVER:
				case CLASS_LYCAN: 
				FX_SmokeHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 1500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					//other->client->poisonDamage = 10; 
					//other->client->poisonTime = cg.time + 2000;
				}	
				break;
				// VOID 
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_PROBE: 
				case CLASS_POLTER: 
				FX_VoidHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2000;  
					other->client->voidDamage = 5; 
					other->client->voidTime = cg.time + 1000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}	
				break;
				// STUN 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_MONMOTHA: 
				case CLASS_TUSKEN:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_DECEIVER: 
				case CLASS_ZEPH: 
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM: 
				case CLASS_RAHABIM: 
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_GRAN:
				case CLASS_VAMPIRE:   
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_KAIN:
				case CLASS_KAIN2:  
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR: 
				case CLASS_JANOS:  
				case CLASS_NECROMANCER:  
				case CLASS_VAMPIRE_QUEEN:  
				case CLASS_VAMPIRE_DARK:  
				case CLASS_VAMPIRE_NECRO:   
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_VAMPIRE_FEAR:  
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN:  
				case CLASS_VAMPIRE_ELF:  
				case CLASS_VAMPIRE_ELF2:  
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_VAMPIRE_WATER:  
				case CLASS_VAMPIRE_FROST: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE:
				case CLASS_RAZIELHIM:  
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_VAMPIRE_GHOST: 
				FX_StunHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					gi.Printf( S_COLOR_RED, "Hitted by Psychic Damage" );
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 1000; 
				}	
				break;
				// KNIFE
				case CLASS_PRISONER:
				case CLASS_SCOUT: 
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_ARCHER: 
				case CLASS_SARAFAN: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_GALAK: 
				case CLASS_LIZARD:
				case CLASS_JAN: 
				FX_KnifeHitPlayer( origin, dir, humanoid );
				break; 
				// ABSORB
				case CLASS_GLIDER:
				case CLASS_JAWA:
				case CLASS_GHOST: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_ARIEL: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_MURJJ:
				case CLASS_MINEMONSTER:
				case CLASS_DEMON_BLADE:
				case CLASS_DEMON_POISON:
				case CLASS_DEMON_ELECTRO:
				case CLASS_ATST:
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE:
				case CLASS_PROPHET:
				case CLASS_CENOBITA:
				FX_AbsorbHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 1500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->necroDamage = 6; 
					other->client->necroTime = cg.time + 1500;
				}	
				break;
				// ENTROPY 
				case CLASS_ALCHEMIST:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION: 
				case CLASS_SENTRY: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_SHARK: 
				FX_EntropyHitPlayer( origin, dir, humanoid );
				if ( other && other->client)
				{
					other->client->iceDamage = 6; 
					other->client->iceTime = cg.time + 1500;
					G_ClearEnemy( other	);
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 2500;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime = level.time + 2500;
				}
				break;
				// TENEBRE...
				case CLASS_DRUID: 
				case CLASS_WIZARD:  
				case CLASS_BEAST: 
				case CLASS_BIRD:
				case CLASS_REAVER: 
				case CLASS_RAZIEL: 
				case CLASS_NECROREAVER: 
				case CLASS_DARKREAVER: 
				case CLASS_LIGHTREAVER: 
				case CLASS_DRUIDREAVER: 
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 				
				case CLASS_BARTENDER:
				case CLASS_COMMANDO:
				case CLASS_THRALL: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_RANCOR: 
				case CLASS_IMPERIAL: 
				case CLASS_IMPWORKER: 				
				case CLASS_INTERROGATOR:
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_GALAKMECH: 
				case CLASS_MORGANKATARN: 
				case CLASS_PROTOCOL: 
				case CLASS_R2D2: 
				case CLASS_R5D2:
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_SABOTEUR: 
				case CLASS_STORMTROOPER: 
				case CLASS_SWAMP: 
				case CLASS_NOGHRI: 
				case CLASS_TRANDOSHAN: 
				case CLASS_WEEQUAY: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_SABER_DROID: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DARK: 
				case CLASS_LIGHT: 
				case CLASS_WATER: 
				case CLASS_AIR: 
				case CLASS_EARTH: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_AIR: 
				case CLASS_GOLEM_METAL: 
				case CLASS_GOLEM: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_GOLEM_HOLY: 
				FX_DarkHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 1500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}	
				break; 
				case CLASS_FIGHTER: 
				FX_StunnerHitPlayer( origin, dir, humanoid );
				break;
			default:
				break;
				}*/
			}
		}
		break;
	case WP_JAWA:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_LANDO: 
				FX_AstralAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime = level.time + 5000;
				}
				break;
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_PROPHET:  
				case CLASS_DRUID: 
				case CLASS_LYCAN:  
				case CLASS_SARAFAN: 
				case CLASS_ARCHER: 
				case CLASS_WARRIOR: 
				case CLASS_CENOBITA:  
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_ARIEL: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:  
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_DEMON_FIRE:
				case CLASS_DEMON_POISON:
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
				case CLASS_FIRE:  
				case CLASS_WATER: 
				case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
				case CLASS_HOLY:  
				case CLASS_POISON: 
				case CLASS_IVY:  
				case CLASS_SONIC: 
				case CLASS_BIRD: 
				case CLASS_BEAST:  
				case CLASS_SHARK:  
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_FIRE:
				case CLASS_GOLEM_WATER:
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GOLEM_POISON: 
				case CLASS_GOLEM_LAVA:
				case CLASS_BESPIN_COP:
				case CLASS_REELO:
				case CLASS_CLAW:
				case CLASS_HAZARD_TROOPER:
				case CLASS_UGNAUGHT: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE: 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_MONMOTHA: 
				case CLASS_WAMPA:
				case CLASS_TUSKEN:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_GRAN: 
				case CLASS_PRISONER:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_GALAK: 
				case CLASS_LIZARD:
				case CLASS_JAN: 
				case CLASS_GLIDER:
				case CLASS_JAWA:
				case CLASS_MURJJ:
				case CLASS_MINEMONSTER:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE:
				case CLASS_DESANN:
				case CLASS_TAVION: 
				case CLASS_SENTRY: 
				case CLASS_BARTENDER:
				case CLASS_COMMANDO: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_RANCOR: 
				case CLASS_IMPERIAL: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_INTERROGATOR:
				case CLASS_JEDI: 
				case CLASS_LUKE: 
				case CLASS_GALAKMECH: 
				case CLASS_MORGANKATARN: 
				case CLASS_PROTOCOL: 
				case CLASS_R2D2: 
				case CLASS_R5D2:
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_SABOTEUR: 
				case CLASS_STORMTROOPER: 
				case CLASS_SWAMP: 
				case CLASS_NOGHRI: 
				case CLASS_TRANDOSHAN: 
				case CLASS_WEEQUAY: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_SABER_DROID: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_HowlerAltHitPlayer( origin, dir, humanoid ); 
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
				}	
				break; 
			default:
				break;
				}
			}
		}
		else
		{
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_LANDO: 
				FX_AstralHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}	
				break;
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_PROPHET:  
				case CLASS_DRUID: 
				case CLASS_LYCAN:  
				case CLASS_SARAFAN: 
				case CLASS_ARCHER: 
				case CLASS_WARRIOR: 
				case CLASS_CENOBITA:  
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_ARIEL: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:  
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_DEMON_FIRE:
				case CLASS_DEMON_POISON:
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
				case CLASS_FIRE:  
				case CLASS_WATER: 
				case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
				case CLASS_HOLY:  
				case CLASS_POISON: 
				case CLASS_IVY:  
				case CLASS_SONIC: 
				case CLASS_BIRD: 
				case CLASS_BEAST:  
				case CLASS_SHARK:  
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_FIRE:
				case CLASS_GOLEM_WATER:
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GOLEM_POISON: 
				case CLASS_GOLEM_LAVA:
				case CLASS_BESPIN_COP:
				case CLASS_REELO:
				case CLASS_CLAW:
				case CLASS_HAZARD_TROOPER:
				case CLASS_UGNAUGHT: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE: 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_MONMOTHA: 
				case CLASS_WAMPA:
				case CLASS_TUSKEN:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_GRAN: 
				case CLASS_PRISONER:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_GALAK: 
				case CLASS_LIZARD:
				case CLASS_JAN: 
				case CLASS_GLIDER:
				case CLASS_JAWA:
				case CLASS_MURJJ:
				case CLASS_MINEMONSTER:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE:
				case CLASS_DESANN:
				case CLASS_TAVION: 
				case CLASS_SENTRY: 
				case CLASS_BARTENDER:
				case CLASS_COMMANDO: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_RANCOR: 
				case CLASS_IMPERIAL: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_INTERROGATOR:
				case CLASS_JEDI: 
				case CLASS_LUKE: 
				case CLASS_GALAKMECH: 
				case CLASS_MORGANKATARN: 
				case CLASS_PROTOCOL: 
				case CLASS_R2D2: 
				case CLASS_R5D2:
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_SABOTEUR: 
				case CLASS_STORMTROOPER: 
				case CLASS_SWAMP: 
				case CLASS_NOGHRI: 
				case CLASS_TRANDOSHAN: 
				case CLASS_WEEQUAY: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_SABER_DROID: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE:  
				FX_HowlerHitPlayer( origin, dir, humanoid ); 
				if ( other && other->client ) 
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 1000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}	
				break; 
			default:
				break;
				}
			}
		}
		break;
		
	/*case WP_BLOODGUN:
		if ( altFire )
		{
			FX_BloodGunAltHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				other->client->hyldenDamage = 15; 
				other->client->hyldenTime = cg.time + 5000;
			}	
		}
		else
		{
			FX_BloodGunHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				other->client->hyldenDamage = 7; 
				other->client->hyldenTime = cg.time + 2500;
			}
		}
		break;*/

	case WP_BLASTER:
		//FX_BlasterWeaponHitPlayer( other, origin, dir, humanoid );
		//break;
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER:
			case CLASS_SABOTEUR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_IMPERIAL:
			case CLASS_WAMPA:
			case CLASS_BOBAFETT:
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_VAMPIRE_DEMON: 
			case CLASS_FIREREAVER:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_AIR: 			
			case CLASS_GOLEM_AIR:
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT:
			case CLASS_GOLEM_GOLD:
			case CLASS_LYCAN:  
			FX_WarmHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->fireDamage = 7; 
					other->client->fireTime = cg.time + 2500;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}	
			break;
			case CLASS_CLAW:
			case CLASS_EARTH: 
			case CLASS_GOLEM: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_REELO: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GONK: 
			case CLASS_CRYSTAL:
			case CLASS_VAMPIRE_GHOST:
			FX_MagmaHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->lavaDamage = 10; 
					other->client->lavaTime = cg.time + 4000;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 4000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "blaster3/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}	
			break;
			case CLASS_ALORA: 
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_FLIER2: 
			case CLASS_NOGHRI:
			case CLASS_SWAMPTROOPER:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_MONMOTHA: 
			case CLASS_DECEIVER: 
			case CLASS_ZEPH: 
			case CLASS_TURELIM: 
			case CLASS_RAHABIM: 
			case CLASS_VAMPIRE:   
			case CLASS_KAIN:
			case CLASS_KAIN2:  
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR: 
			case CLASS_JANOS:  
			case CLASS_NECROMANCER:  
			case CLASS_VAMPIRE_QUEEN:  
			case CLASS_VAMPIRE_DARK:  
			case CLASS_VAMPIRE_NECRO:   
			case CLASS_VAMPIRE_NECRO2: 
			case CLASS_VAMPIRE_FEAR:  
			case CLASS_VAMPIRE_BLOOD: 
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN:  
			case CLASS_VAMPIRE_ELF:  
			case CLASS_VAMPIRE_ELF2:  
			case CLASS_VAMPIRE_ELF3: 
			case CLASS_VAMPIRE_WATER:  
			case CLASS_VAMPIRE_FROST: 
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE:
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_SARAFAN:
			case CLASS_GOLEM_DARK: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_GOLEM_VOID:
			FX_VampFireHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->drainDamage = 7; 
					other->client->drainTime = cg.time + 2500;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}	
			break;
			case CLASS_MOUSE: 
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_CENOBITA: 
			FX_HellFireHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->hellDamage = 10; 
					other->client->hellTime = cg.time + 3000;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 3000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "fire_hell/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}	
			break;
			case CLASS_TUSKEN:
			case CLASS_VAMPIRE_ZOMBIE: 
			case CLASS_GLIDER:
			case CLASS_MURJJ: 
			case CLASS_JAWA:
			case CLASS_SENTRY:
			case CLASS_RANCOR:
			case CLASS_GHOST: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_VAMPIRE_FEAR2: 
			case CLASS_PROPHET: 
			case CLASS_POLTER:
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO:
			FX_GhostFireHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->necroDamage = 1; 
					other->client->necroTime = cg.time + 2000;
				}	
			break;
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			FX_GreenFireHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->hyldenDamage = 15; 
					other->client->hyldenTime = cg.time + 5000;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "fire_green/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}	
			break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_LIZARD:
			case CLASS_MINEMONSTER: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_IVY: 
			case CLASS_BEAST: 
			case CLASS_BIRD:
			case CLASS_SHARK: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_INTERROGATOR: 
			FX_PoisonFireHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->poisonDamage = 10; 
					other->client->poisonTime = cg.time + 5000;
				}	
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_MARK1: 
			case CLASS_MARK2: 
			case CLASS_GALAKMECH: 
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 
			case CLASS_PROTOCOL: 
			case CLASS_REBEL: 
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_ROCKETTROOPER: 
			case CLASS_SABER_DROID: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID: 
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_THRALL: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_HOLY:  
			case CLASS_SONIC: 
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			case CLASS_GOLEM_METAL:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			FX_BlasterWeaponHitPlayer( other, origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->fireDamage = 5; 
					other->client->fireTime = cg.time + 1000;
				}	
			break; 
				case CLASS_FIGHTER: 
				FX_GreenLaserHitPlayer( other, origin, dir, humanoid );
				break;
			default:
				break;
			}
		}
		break;
	case WP_BOWCASTER:
		// FX_BowcasterHitPlayer( origin, dir, humanoid );
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{		
			case CLASS_LIGHTREAVER: 
			case CLASS_LYCAN: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_SABER_DROID:
			case CLASS_ASSASSIN_DROID: 
			FX_ShockHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
			other->s.powerups |= ( 1 << PW_SHOCKED );
			other->client->ps.powerups[PW_SHOCKED] = cg.time + 2000;
			}
			break;
			case CLASS_MORGANKATARN:
			case CLASS_GOLEM_GOLD: 
			case CLASS_PROTOCOL: 
			case CLASS_JAN: 
			case CLASS_CLAW: 
			FX_SunHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				other->s.powerups |= ( 1 << PW_SHOCKED );
				other->client->ps.powerups[PW_SHOCKED] = cg.time + 3000;
				other->client->sunDamage = 10;  
				other->client->sunTime = cg.time + 23000; 
				other->s.pos.trTime += 2000; // <- TEMPO PARALISI 
				/// FREEZE ANIMATION
				NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
				NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				other->client->ps.torsoAnimTimer += 2000;  
				other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "bowcastet3/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration of disruptor  - death_smoke
							}
						}
			}
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA:
			case CLASS_MONMOTHA:
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_LIZARD:
			case CLASS_SWAMPTROOPER:
			case CLASS_REBEL:
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_WAMPA: 
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_TURELIM:
			case CLASS_VAMPIRE:
			case CLASS_VAMPIRE_DEMON:
			case CLASS_VAMPIRE_GHOST: 
			case CLASS_KAIN:
			case CLASS_KAIN2: 
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR:
			case CLASS_JANOS:
			case CLASS_NECROMANCER: 
			case CLASS_VAMPIRE_QUEEN:
			case CLASS_VAMPIRE_DARK: 
			case CLASS_VAMPIRE_NECRO: 
			case CLASS_VAMPIRE_NECRO2:
			case CLASS_VAMPIRE_FEAR: 
			case CLASS_VAMPIRE_FEAR2:  
			case CLASS_VAMPIRE_BLOOD:
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN: 
			case CLASS_VAMPIRE_ELF: 
			case CLASS_VAMPIRE_ELF2:
			case CLASS_VAMPIRE_ELF3:
			case CLASS_VAMPIRE_WATER: 
			case CLASS_VAMPIRE_FROST:  
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE: 
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_ZOMBIE:
			case CLASS_VAMPIRE_SARAFAN: 
			case CLASS_AIR: 
			case CLASS_GOLEM_AIR: 
			case CLASS_BIRD: 
			case CLASS_IVY: 
			FX_EbHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
			other->s.powerups |= ( 1 << PW_SHOCKED );
			other->client->ps.powerups[PW_SHOCKED] = cg.time + 2500;
			}
			break; 
			case CLASS_GALAK:
			case CLASS_RODIAN:
			case CLASS_MARK1:
			case CLASS_MARK2: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			FX_LightKnifeHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
				NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				other->client->ps.torsoAnimTimer += 2000;  
				other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
			}
			break;
			case CLASS_TAVION:
			case CLASS_DESANN:
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_SAND_CREATURE:
			case CLASS_ATST:
			case CLASS_SENTRY:
			case CLASS_MOUSE: 
			case CLASS_TENTACLE:  
			case CLASS_ELDERGOD: 
			case CLASS_SONIC: 
			case CLASS_GOLEM_SONIC: 
			case CLASS_PROPHET: 
			case CLASS_DECEIVER: 
			case CLASS_FLIER2: 
			case CLASS_BEAST:  
			case CLASS_SHARK:
			FX_MadnessHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				other->client->sonicDamage = 5;  
				other->client->sonicTime = cg.time + 5000; 
				other->s.pos.trTime = cg.time + 5000; // <- TEMPO PARALISI 
				/// FREEZE ANIMATION
				NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
				NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				other->client->ps.torsoAnimTimer += 2000;  
				other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
			}
			break;
			case CLASS_ARIEL: 
			case CLASS_MINEMONSTER:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_FIRE: 
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_SABOTEUR: 
			case CLASS_MURJJ:
			case CLASS_NECRO: 
			case CLASS_THRALL: 
			case CLASS_IMPERIAL: 
			case CLASS_CENOBITA: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_POISON: 
			case CLASS_GOLEM_DARK: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_NECRO:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_GOLEM_VOID:
			case CLASS_GOLEM_POISON: 
			case CLASS_GOLEM_LAVA:
			case CLASS_POLTER:
			FX_LightSkullHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				other->s.powerups |= ( 1 << PW_SHOCKED );
				other->client->ps.powerups[PW_SHOCKED] = cg.time + 500;
				other->client->sunDamage = 3;  
				other->client->sunTime = cg.time + 3000; 
				other->client->necroDamage = 3;  
				other->client->necroTime = cg.time + 3000; 
			}	
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER:
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_GALAKMECH: 
			case CLASS_PRISONER: 
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER: 
			case CLASS_REELO: 
			case CLASS_REMOTE: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP: 
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_GONK: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID:
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:			
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER:		 
			case CLASS_LIGHT: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_CRYSTAL: 
			case CLASS_EARTH: 
			case CLASS_HOLY:  
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:			
			case CLASS_GOLEM:
			case CLASS_GOLEM_HOLY: 
			FX_BowcasterHitPlayer( origin, dir, humanoid );
			break; 
				case CLASS_FIGHTER: 
				FX_DardusHitPlayer( origin, dir, humanoid );
				break;
			default:
				break;
			}
		}
	break;
	case WP_REPEATER:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_BARTENDER:
				case CLASS_RAZIELHIM: 
				case CLASS_LYCAN:
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST:
				case CLASS_GOLEM_ICE: 
				case CLASS_POLTER: 
				case CLASS_PROBE: 
				FX_IceAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 7500;  
					other->client->ps.weaponTime += 7500;  
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 7500;
					
				}
				break;
				case CLASS_ESKANDOR: 
				case CLASS_GALAKMECH:
				case CLASS_CRYSTAL: 
				case CLASS_WEEQUAY:
				case CLASS_GONK: 
				case CLASS_GOLEM: 
				case CLASS_EARTH: 
				FX_CrystalAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime += 10000; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
				}
				break;
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_TUSKEN:
				case CLASS_LIZARD:
				case CLASS_MONMOTHA: 
				case CLASS_GLIDER:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2: 
				case CLASS_GRAN: 
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_RANCOR:
				case CLASS_SLUAGH: 
				case CLASS_MINEMONSTER: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
			  	case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_BEAST: 
				FX_GreenMudAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->acidDamage = 8;  
					other->client->acidTime = cg.time + 5000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "putrescence/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration states reaver \ acid thermal
							}
						}
				}
				break;
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_GOLEM_NECRO: 
				case CLASS_NECRO: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN: 
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_MURJJ: 
				case CLASS_THRALL: 
				FX_NecroticAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 6;  
					other->client->necroTime = cg.time + 6000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "necrotic/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration states reaver \ acid thermal
							}
						}
				}
				break; 
				case CLASS_IMPERIAL:
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_WAMPA:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_DEMON_FIRE: 
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN: 
				FX_HellMudAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->acidDamage = 12;  
					other->client->acidTime = cg.time + 5000; 
					other->client->hellDamage = 12;  
					other->client->hellTime = cg.time + 5000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "hellputrescence/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}
				break;
				case CLASS_INTERROGATOR:
				case CLASS_HAZARD_TROOPER: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_CLAW: 
				case CLASS_JAWA: 
				case CLASS_GHOST: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD:
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_BIRD: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR:
				case CLASS_LIGHT: 
				case CLASS_GOLEM_SMOKE: 
				FX_VaporAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 2000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}
				break;
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE:
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_SENTRY:
				case CLASS_MOUSE:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				FX_ElderTarAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime += 10000; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
					other->client->necroDamage = 20;  
					other->client->necroTime = cg.time + 10000; 
					other->client->acidDamage = 20;  
					other->client->acidTime = cg.time + 10000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "eldertar/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}
				break;
				case CLASS_SABOTEUR:
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_REELO: 
				case CLASS_ALCHEMIST: 
				case CLASS_IVY: 
				case CLASS_SWAMP: 
				FX_TarAltHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime += 5000; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
				}
				break;
				case CLASS_RODIAN:
				case CLASS_GALAK:
				case CLASS_STORMTROOPER:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_ARIEL: 	
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				FX_HolyWaterAltHitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_ALORA || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_SWAMPTROOPER || other && other->client->NPC_class == CLASS_FISH || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW || 
						other && other->client->NPC_class == CLASS_JEDI || other && other->client->NPC_class == CLASS_LUKE || 
						other && other->client->NPC_class == CLASS_WAMPA || other && other->client->NPC_class == CLASS_MINEMONSTER || 
						other && other->client->NPC_class == CLASS_TAVION ||other && other->client->NPC_class == CLASS_COMMANDO ||
						other && other->client->NPC_class == CLASS_MURJJ	 || other && other->client->NPC_class == CLASS_TRANDOSHAN ||
						other && other->client->NPC_class == CLASS_LANDO ) 
						{
							NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
							NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
							other->client->ps.torsoAnimTimer += 2000;  
							other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
							other->client->holyDamage = 5;  
							other->client->holyTime = cg.time + 7000; 
							other->client->waterDamage = 5;  
							other->client->waterTime = cg.time + 7000; 
						}
				}
				break;
				case CLASS_ALORA:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SWAMPTROOPER: 
				case CLASS_GOLEM_WATER:	
				FX_TkWaterAltHitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_VAMPIRE || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW )
						{
							other->client->waterDamage = 5;  
							other->client->waterTime = cg.time + 5000; 
						}
				}
				break;
				case CLASS_FISH: 
				FX_TkWater2AltHitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_VAMPIRE || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW )
						{
							other->client->waterDamage = 7;  
							other->client->waterTime = cg.time + 7000; 
						}
				}
				break;
				case CLASS_HOWLER: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_MORGANKATARN: 
				case CLASS_PRISONER: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DRUID: 
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
			 	case CLASS_SPIDERREAVER: 				
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 			  
				case CLASS_WATER:			 
			  	case CLASS_SONIC: 
				case CLASS_SHARK:  
				case CLASS_GOLEM_METAL:			
				case CLASS_GOLEM_SONIC:			 
				FX_RepeaterAltHitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_VAMPIRE || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW )
						{
							other->client->waterDamage = 2;  
							other->client->waterTime = cg.time + 2000; 
						}
				}
				break; 
				case CLASS_FIGHTER: 
				FX_PlasmaBombAltHitPlayer( origin, dir, humanoid );
				break;
			default:
				break;
				}
			}
		}
		else  
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_BARTENDER:
				case CLASS_RAZIELHIM: 
				case CLASS_LYCAN:
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST:
				case CLASS_GOLEM_ICE: 
				case CLASS_POLTER: 
				case CLASS_PROBE: 
				FX_IceHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->ps.torsoAnimTimer += 1000;  
					other->client->ps.weaponTime += 1000;  
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 1000;
				}
				break;
				case CLASS_ESKANDOR: 
				case CLASS_GALAKMECH:
				case CLASS_CRYSTAL: 
				case CLASS_WEEQUAY:
				case CLASS_GONK: 
				case CLASS_GOLEM: 
				case CLASS_EARTH: 
				FX_CrystalHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 2000;  
					other->client->ps.weaponTime += 2000; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 2000;
				}
				break;
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_TUSKEN:
				case CLASS_LIZARD:
				case CLASS_MONMOTHA: 
				case CLASS_GLIDER:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2: 
				case CLASS_GRAN: 
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_RANCOR:
				case CLASS_SLUAGH: 
				case CLASS_MINEMONSTER: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
			  	case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_BEAST: 
				FX_GreenMudHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->acidDamage = 4;  
					other->client->acidTime = cg.time + 2500; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "putrescence/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration states reaver \ acid thermal
							}
						}
					
				}
				break;
				case CLASS_IMPERIAL:
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_WAMPA:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_DEMON_FIRE: 
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN: 
				FX_HellMudHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->hellDamage = 6;  
					other->client->hellTime = cg.time + 2500; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "hellputrescence/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration states reaver \ acid thermal
							}
						}
				}
				break;
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_GOLEM_NECRO: 
				case CLASS_NECRO: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN: 
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_MURJJ: 
				case CLASS_THRALL: 
				FX_NecroticHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 6;  
					other->client->necroTime = cg.time + 4000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "necrotic/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration states reaver \ acid thermal
							}
						}
				}
				break; 
				case CLASS_INTERROGATOR:
				case CLASS_HAZARD_TROOPER: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_CLAW: 
				case CLASS_JAWA: 
				case CLASS_GHOST: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD:
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_BIRD: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR:
				case CLASS_LIGHT: 
				case CLASS_GOLEM_SMOKE: 
				FX_VaporHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 1000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}
				break;
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE:
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_SENTRY:
				case CLASS_MOUSE:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				FX_ElderTarHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 10;  
					other->client->necroTime = cg.time + 5000; 
					other->client->acidDamage = 10;  
					other->client->acidTime = cg.time + 5000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "eldertar/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime += 5000;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
				}
				break;
				case CLASS_SABOTEUR:
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_REELO: 
				case CLASS_ALCHEMIST: 
				case CLASS_IVY: 
				case CLASS_SWAMP: 
				FX_TarHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 500;  
					other->client->ps.weaponTime += 500; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 500;
				}
				break;
				case CLASS_RODIAN:
				case CLASS_GALAK:
				case CLASS_STORMTROOPER:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_ARIEL: 	
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				FX_HolyWaterHitPlayer( origin, dir, humanoid );
				if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_ALORA || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_SWAMPTROOPER || other && other->client->NPC_class == CLASS_FISH || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW || 
						other && other->client->NPC_class == CLASS_JEDI || other && other->client->NPC_class == CLASS_LUKE || 
						other && other->client->NPC_class == CLASS_WAMPA || other && other->client->NPC_class == CLASS_MINEMONSTER || 
						other && other->client->NPC_class == CLASS_TAVION ||other && other->client->NPC_class == CLASS_COMMANDO ||
						other && other->client->NPC_class == CLASS_MURJJ	 || other && other->client->NPC_class == CLASS_TRANDOSHAN ||
						other && other->client->NPC_class == CLASS_LANDO ) 
						{
							NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
							NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
							other->client->ps.torsoAnimTimer += 1000;  
							other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
							other->client->waterDamage = 3;  
							other->client->waterTime = cg.time + 3000; 
							other->client->holyDamage = 3;  
							other->client->holyTime = cg.time + 3000; 
				}
				break;
				case CLASS_ALORA:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SWAMPTROOPER: 
				case CLASS_GOLEM_WATER:	
				FX_TkWaterHitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_VAMPIRE || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW )
						{
							other->client->waterDamage = 3;  
							other->client->waterTime = cg.time + 3000; 
						}
				}
				break;
				case CLASS_FISH: 
				FX_TkWater2HitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_VAMPIRE || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW )
						{
							other->client->waterDamage = 4;  
							other->client->waterTime = cg.time + 4000; 
						}
				}
				break;
				case CLASS_HOWLER: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_MORGANKATARN: 
				case CLASS_PRISONER: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DRUID: 
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
			 	case CLASS_SPIDERREAVER: 				
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 			  
				case CLASS_WATER:			 
			  	case CLASS_SONIC: 
				case CLASS_SHARK:  
				case CLASS_GOLEM_METAL:			
				case CLASS_GOLEM_SONIC:			 
				FX_RepeaterHitPlayer( origin, dir, humanoid );
				{
					if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_VAMPIRE || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW )
						{
							other->client->waterDamage = 1;  
							other->client->waterTime = cg.time + 1000; 
						}
				}
				break; 
				case CLASS_FIGHTER: 
				FX_PlasmaHitPlayer( origin, dir, humanoid );
				break; 
			default:
				break;
				}
			}
		}
		break;

	case WP_DEMP2:
		if ( !altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
				{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// BENEDICTUM
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_RODIAN:
			case CLASS_PRISONER:
			case CLASS_GALAK:
			case CLASS_INTERROGATOR:
			case CLASS_SWAMP:
			case CLASS_WATER: 
			case CLASS_GOLEM_WATER: 
			case CLASS_GOLEM_ICE: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_SEEKER:
			case CLASS_REMOTE: 
			FX_DEMP_HitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
				{
					other->client->ps.torsoAnimTimer += 200;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
			// SPECTRAL
			case CLASS_REELO:
			case CLASS_RANCOR:
			case CLASS_SENTRY:
			case CLASS_MINEMONSTER:
			case CLASS_SABOTEUR:
			case CLASS_SHADOWTROOPER:
			case CLASS_ASSASSIN_DROID:
			case CLASS_MORGANKATARN:
			case CLASS_HAZARD_TROOPER:
			case CLASS_REBEL: 
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT:
			case CLASS_BARTENDER:
			case CLASS_UGNAUGHT:
			case CLASS_JEDI:
			case CLASS_REAVER: 
			case CLASS_LIZARD: 
			case CLASS_WEEQUAY: 
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
				FX_DEMP0_HitPlayer( origin, dir, humanoid );
					break;
				// HELL WRACK
			case CLASS_PROPHET: 
			case CLASS_CENOBITA: 
			case CLASS_WAMPA:
			case CLASS_TAVION:
			case CLASS_IMPERIAL:
			case CLASS_DESANN:
			case CLASS_ATST:
			case CLASS_MOUSE:
			case CLASS_SAND_CREATURE:
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_MURJJ: 	
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_DEMON_FIRE:
			case CLASS_DEMON_POISON:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_FIRE: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_GRAN: 
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FLIER2:
			case CLASS_FISH:
			case CLASS_SWAMPTROOPER:			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_HELLGOLEM: 
				FX_DEMP1_HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer = 7500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->hellDamage = 6;  
					other->client->hellTime = cg.time + 6000; 
				}
				break;
			// SPIRIT
			case CLASS_RAZIEL: 
			case CLASS_IMPWORKER: 
				FX_DEMP2_HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// HOLY
			case CLASS_HOLY: 
			case CLASS_GOLEM_HOLY: 
			case CLASS_VAMPIRE_GHOST: 
				FX_DEMP3_HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 7500; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 5;  
					other->client->holyTime = cg.time + 7500; 
				}
				break;
				// LIFE 
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_BEAST: 
				case CLASS_BIRD: 
				case CLASS_SHARK: 
				FX_DEMP4_HitPlayer( origin, dir, humanoid );
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_KNEES1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_KNEES1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->natureDamage = 5;  
					other->client->natureTime = cg.time + 10000; 
				}
				break;
				// DRUIDIC FIRE
				case CLASS_TRANDOSHAN:
				case CLASS_LANDO: 
				case CLASS_DRUIDREAVER: 
				case CLASS_DRUID: 
				case CLASS_LYCAN: 


				FX_DEMP5_HitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 2500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->natureDamage = 4;  
					other->client->natureTime = cg.time + 2500; 
				
				}
				break;
				// HOLY FIRE
				case CLASS_JAN:
				case CLASS_SARAFAN: 
				case CLASS_STORMTROOPER: 
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_WIZARD: 
				FX_DEMP6_HitPlayer( origin, dir, humanoid );
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 3000; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;  
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 7;  
					other->client->holyTime = cg.time + 3000; 
				
				}
				break;
				// REVERB
				case CLASS_GALAKMECH: 
				case CLASS_GONK: 
				case CLASS_NECROREAVER:
				case CLASS_JAWA:
				case CLASS_PROBE:
				case CLASS_COMMANDO:
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_HOWLER: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
			  	case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
			  	case CLASS_SONIC: 
			 	case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
			 	case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GHOST: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_DEMP7_HitPlayer( origin, dir, humanoid );
				break;
				// BALANCE 
				case CLASS_KYLE: 
				case CLASS_KAIN3: 
				case CLASS_ARIEL: 
				FX_DEMP8_HitPlayer( origin, dir, humanoid );
				{
					G_ClearEnemy( other );
					team_t	saveTeam = other->client->enemyTeam;
					other->client->enemyTeam = other->client->playerTeam;
					other->client->playerTeam = saveTeam;
				}
				break;
				// WRACK IMPORTANT: NEVER. NEVER GET THIS TO AN NPC!   
				//case CLASS_GALAKMECH:
				//case CLASS_GONK:
				case CLASS_REBORN:
				case CLASS_ALORA:
				case CLASS_MONMOTHA:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				FX_DEMP9_HitPlayer( origin, dir, humanoid );
				if ( other && other->client && other->s.weapon != WP_SABER ) 
				{
					// CAN CONTROL ONLY THE SHOOTERS MIND, THESE AVOID TO AFFECTED BOSSES 
					team_t	saveTeam = other->client->enemyTeam;
					other->client->enemyTeam = other->client->playerTeam;
					other->client->playerTeam = saveTeam;
					G_SetViewEntity( cent->gent->owner, other );
				}
				case CLASS_FIGHTER: 
				FX_PULSE_HitPlayer( origin, dir, humanoid );
				break; 
			default:
				break;
				}
			}
		}
		/*if ( !altFire )
		{
			FX_DEMP2_HitPlayer( origin, dir, humanoid );
		}*/
		break;

		case WP_POISON:
		if ( !altFire )
		{
			FX_Poison_HitPlayer( origin, dir, humanoid );
			other->client->poisonDamage = 10;  
			other->client->poisonTime = cg.time + 15000; 
				
		}
		break;

	case WP_FLECHETTE:
		if ( altFire )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FANGO
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 			
				if ( other && other->client )
				{
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
				}
				theFxScheduler.PlayEffect( "flechette2/alt_blow", origin, dir );
				break;
				// PIETRA
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				if ( other && other->client )
				{
					other->client->poisonDamage = 1;  
					other->client->poisonTime = cg.time + 15000; 
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 15000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime = level.time + 15000;

				}
				theFxScheduler.PlayEffect( "flechette3/alt_blow", origin, dir );
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( "acid/alt_blow", origin, dir );
				if ( other && other->client )
				{
					other->client->acidDamage = 2;  
					other->client->acidTime = cg.time + 15000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "acid/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				theFxScheduler.PlayEffect( "ivy/alt_blow", origin, dir );
				if ( other && other->client )
				{
					other->client->poisonDamage = 2;  
					other->client->poisonTime = cg.time + 5000; 
				}
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( "ivy2/alt_blow", origin, dir );
				if ( other && other->client )
				{
					other->client->hyldenDamage = 4;  
					other->client->hyldenTime = cg.time + 10000; 
				}
				break;
				// THORN 
				case CLASS_SWAMP: 
				theFxScheduler.PlayEffect( "thorn/alt_blow", origin, dir );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				theFxScheduler.PlayEffect( "spectral/alt_blow", origin, dir );
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				if ( other && other->client )
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
				}
				theFxScheduler.PlayEffect( "stasis/alt_blow", origin, dir );
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
				theFxScheduler.PlayEffect( "dust/alt_blow", origin, dir );
				if ( other && other->client )
				{
					other->client->stoneDamage = 4;  
					other->client->stoneTime = cg.time + 3000; 
					other->client->ps.torsoAnimTimer += 3000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				theFxScheduler.PlayEffect( "ash/alt_blow", origin, dir );
				if ( other && other->client )
				{
					other->client->smokeDamage = 10;  
					other->client->smokeTime = cg.time + 2000; 
					other->client->ps.torsoAnimTimer += 2000; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				if ( other && other->client )
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
				}
				theFxScheduler.PlayEffect( "gcrystal/alt_blow", origin, dir );
				break;
				// DEFAULT 		  
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				theFxScheduler.PlayEffect( "flechette/alt_blow", origin, dir );
				break; 
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "flechette0/alt_blow", origin, dir );
				break; 
			default:
				break;
				}
			}
		}
		else
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FANGO
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 	
				FX_MudHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.torsoAnimTimer += 2000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 2000;
				}
				break;
				// PIETRA 
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				FX_StoneHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->poisonDamage = 1;  
					other->client->poisonTime = cg.time + 5000; 
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime = level.time + 5000;

				}
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_AcidHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->acidDamage = 1;  
					other->client->acidTime = cg.time + 15000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "acid/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
				}
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				FX_IvyHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->poisonDamage = 1;  
					other->client->poisonTime = cg.time + 10000; 
				}
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				FX_Ivy2HitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->hyldenDamage = 2;  
					other->client->hyldenTime = cg.time + 20000; 
				}
				break;
				// THORN 
				case CLASS_SWAMP: 
				FX_ThornHitPlayer( origin, dir, humanoid );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				FX_SpectralHitPlayer( origin, dir, humanoid );
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				FX_StasisHitPlayer( origin, dir, humanoid );
				if ( other && other->client)
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
				}
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
				FX_DustHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->stoneDamage = 2;  
					other->client->stoneTime = cg.time + 1500; 
					other->client->ps.torsoAnimTimer += 1500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				FX_AshHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->smokeDamage = 5;  
					other->client->smokeTime = cg.time + 1000; 
					other->client->ps.torsoAnimTimer += 1000; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				FX_GcrystalHitPlayer( origin, dir, humanoid );
				if ( other && other->client)
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
				}
				break;
				// DEFAULT 
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				FX_FlechetteWeaponHitPlayer( origin, dir, humanoid );
				break; 
				case CLASS_FIGHTER: 				
				FX_ShrapnelHitPlayer( origin, dir, humanoid );
				break; 
			default:
				break;
				}
			}
		}
		/*if ( altFire )
		{
			theFxScheduler.PlayEffect( "flechette/alt_blow", origin, dir );
		}
		else
		{
			FX_FlechetteWeaponHitPlayer( origin, dir, humanoid );
		}*/	
		break;

		case WP_CANNON:
		if ( altFire )
		{
			theFxScheduler.PlayEffect( "cannon/alt_blow", origin, dir );
		}
		else
		{
			theFxScheduler.PlayEffect( "cannon/flesh_impact", origin, dir );
		}	
		break;
	case WP_ROCKET_LAUNCHER:
		// FX_RocketHitPlayer( origin, dir, humanoid );
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_GOLEM_GOLD: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_SABER_DROID:
			case CLASS_PROTOCOL: 
			case CLASS_PROBE: 
			case CLASS_POLTER: 
			case CLASS_AIRREAVER: 
			case CLASS_REBEL: 
			case CLASS_SHARK:
			case CLASS_LYCAN:  
			case CLASS_WATER:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			FX_FogHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 2500;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
			}
			break;
			case CLASS_ROCKETTROOPER:
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_BOBAFETT:  
			FX_StormHitPlayer( origin, dir, humanoid );
			if ( other && other->client ) 
			{
				G_ClearEnemy( other );
				other->client->ps.torsoAnimTimer += 5000;
				other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				other->s.powerups |= ( 1 << PW_SHOCKED );
				other->client->ps.powerups[PW_SHOCKED] = cg.time + 3000;
			}
			break;
			case CLASS_MURJJ:
			case CLASS_THRALL: 
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO: 
			case CLASS_FISH:
			case CLASS_GLIDER:
			case CLASS_MARK1:
			case CLASS_MARK2:
			case CLASS_IMPERIAL: 
			case CLASS_PROPHET: 
			FX_ThrallBreathHitPlayer( origin, dir, humanoid );
			if ( other && other->client )
				{
					other->client->necroDamage = 1;  
					other->client->necroTime = cg.time + 5000; 
				}
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA: 
			case CLASS_MONMOTHA: 
			case CLASS_FLIER2: 
			case CLASS_LIZARD: 
			case CLASS_SWAMPTROOPER: 
			case CLASS_NOGHRI:
			case CLASS_TUSKEN: 
			case CLASS_WAMPA: 
			case CLASS_VAMPIRE:
			case CLASS_VAMPIRE_DEMON:
			case CLASS_KAIN:
			case CLASS_KAIN2: 
			case CLASS_KAIN3:
			case CLASS_UMAH: 
			case CLASS_VORADOR:
			case CLASS_JANOS:
			case CLASS_NECROMANCER: 
			case CLASS_VAMPIRE_QUEEN:
			case CLASS_VAMPIRE_DARK: 
			case CLASS_VAMPIRE_NECRO: 
			case CLASS_VAMPIRE_NECRO2:
			case CLASS_VAMPIRE_FEAR: 
			case CLASS_VAMPIRE_FEAR2:  
			case CLASS_VAMPIRE_BLOOD:
			case CLASS_VAMPIRE_SOUND: 
			case CLASS_VAMPIRE_LYCAN: 
			case CLASS_VAMPIRE_ELF: 
			case CLASS_VAMPIRE_ELF2:
			case CLASS_VAMPIRE_ELF3:
			case CLASS_VAMPIRE_WATER: 
			case CLASS_VAMPIRE_FROST:  
			case CLASS_VAMPIRE_ARCHER: 
			case CLASS_HALFVAMPIRE: 
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_ZOMBIE: 
			case CLASS_VAMPIRE_SARAFAN: 
			case CLASS_FIRE: 
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_ZEPH: 
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM: 
			case CLASS_WEEQUAY: 
			case CLASS_VAMPIRE_GHOST: 
			FX_FlayHitPlayer( origin, dir, humanoid );
			if (  other &&  other->client && other->health <= 0 )
				{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
					if ( other->ghoul2.size() && other->headBolt != -1 )
					{//FIXME: what if already playing effect?
						G_PlayEffect( G_EffectIndex( "rocket4/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
					}
				}
			break;
			case CLASS_MOUSE:
			case CLASS_SENTRY:
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
				if ( altFire )
				{
					FX_ElderSphereAltHitPlayer ( origin, dir, humanoid );
				}
				else
				{
					FX_ElderSphereHitPlayer( origin, dir, humanoid );
				}
				break;
			case CLASS_SHADOWTROOPER: 
			case CLASS_GOLEM_VOID: 
			case CLASS_VOID: 
			case CLASS_DARKREAVER: 
			case CLASS_DARK: 
			case CLASS_GOLEM_DARK: 
			case CLASS_SABOTEUR:
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
				FX_BlackHoleHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->voidDamage = 10;  
					other->client->voidTime = cg.time + 10000; 
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				
				}
				break;
			case CLASS_MINEMONSTER: 
			case CLASS_IVY: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_DEMON_POISON: 
			case CLASS_BEAST: 
			case CLASS_REELO: 
			case CLASS_ARCHER:
			case CLASS_ALCHEMIST: 
				FX_PoisonFogHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->poisonDamage = 4;  
					other->client->poisonTime = cg.time + 10000; 				
				}
				break;
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_GHOST: 
			case CLASS_DREADNOUGHT: 
			case CLASS_REAPER: 
			case CLASS_ARIEL: 
			case CLASS_CENOBITA: 			 
				FX_SpectralFogHitPlayer( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->s.powerups |= ( 1 << PW_SHOCKED );
					other->client->ps.powerups[PW_SHOCKED] = cg.time + 500;
					other->client->necroDamage = 5;  
					other->client->necroTime = cg.time + 5000; 	
				}
				break;
			case CLASS_GRAN:
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
				FX_TkHitPlayer( origin, dir, humanoid );
				break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_GONK:
			case CLASS_GOLEM: 
			case CLASS_EARTH: 
			case CLASS_EARTHREAVER: 
			case CLASS_SPIDERREAVER2: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GALAKMECH: 
			case CLASS_SARAFAN: 
			case CLASS_DEMON_BLADE:
			case CLASS_SCOUT: 
				FX_SandStormHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->windDamage = 4;  
					other->client->windTime = cg.time + 5000; 
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				
				}
				break;
			case CLASS_LANDO: 
				FX_GravityHitPlayer( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.weaponTime += 5000;
					other->client->ps.torsoAnimTimer += 5000;  
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_KNEES1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_KNEES1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
				}
				break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 			
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LUKE: 			
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 			
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:  
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 			
			case CLASS_HAZARD_TROOPER: 			
			case CLASS_PLAYER: 
			case CLASS_VEHICLE:
			case CLASS_HUNTER: 			
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_FIREREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_CRYSTAL: 
			case CLASS_AIR:  			 
			case CLASS_HOLY: 			  
			case CLASS_SONIC: 
			case CLASS_BIRD: 			
			case CLASS_WARRIOR: 
			case CLASS_DRUID:
			case CLASS_GOLEM_AIR:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			case CLASS_GOLEM_SMOKE: 
			FX_RocketHitPlayer( origin, dir, humanoid );
			break; 
				case CLASS_FIGHTER: 				
				FX_SeekerHitPlayer( origin, dir, humanoid );
				break; 
		default:
			break;
			}
		}
		break;
	case WP_CONCUSSION:
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
					{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// HELL STRIKE 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_GLIDER:
				case CLASS_TUSKEN:
				case CLASS_MELC:
				case CLASS_SUMMONER: 		
				FX_ConcHitPlayer ( origin, dir, humanoid );
				break;
				// BLACK FIRE 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH: 
				case CLASS_RAHABIM: 
				case CLASS_COMMANDO: 
				case CLASS_LYCAN: 
				case CLASS_CENOBITA: 
				case CLASS_WIZARD: 
				FX_BlackFireHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;			;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->necroDamage = 5;  
					other->client->necroTime = cg.time + 10000; 
					if (  other &&  other->client && other->health <= 0 )
					{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
					if ( other->ghoul2.size() && other->headBolt != -1 )
						{//FIXME: what if already playing effect?
							G_PlayEffect( G_EffectIndex( "blackfire/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
						}
					}
				}
				break;
				// SPIRITH DEATH  
				case CLASS_REBORN:
				case CLASS_ALORA:
				case CLASS_MONMOTHA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE:
				case CLASS_KAIN2: 
				case CLASS_UMAH: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_SARAFAN:  
				FX_SDHitPlayer ( origin, dir, humanoid );
				break;
				// IMMOLATE 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN3:
				case CLASS_TURELIM: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_CLAW: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_VAMPIRE_DEMON: 
				FX_ImmolationHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 12000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;			;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->fireDamage = 10;  
					other->client->fireTime = cg.time + 12000; 
					other->client->necroDamage = 10;  
					other->client->necroTime = cg.time + 12000; 
					if (  other &&  other->client && other->health <= 0 )
					{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
					if ( other->ghoul2.size() && other->headBolt != -1 )
						{//FIXME: what if already playing effect?
							G_PlayEffect( G_EffectIndex( "immolate/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
						}
					}
				}
				break;
				// HOLYIMMOLATE 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_TRANDOSHAN:
				case CLASS_STORMTROOPER:
				case CLASS_PRISONER:
				case CLASS_RODIAN:
				case CLASS_IMPWORKER:
				case CLASS_GALAK:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_SEEKER:
				case CLASS_REMOTE:
				case CLASS_DRUIDREAVER: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				FX_HolyImmolationHitPlayer ( origin, dir, humanoid );
				if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_ALORA || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_SWAMPTROOPER || other && other->client->NPC_class == CLASS_FISH || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW || 
						other && other->client->NPC_class == CLASS_JEDI || other && other->client->NPC_class == CLASS_LUKE || 
						other && other->client->NPC_class == CLASS_WAMPA || other && other->client->NPC_class == CLASS_MINEMONSTER || 
						other && other->client->NPC_class == CLASS_TAVION ||other && other->client->NPC_class == CLASS_COMMANDO ||
						other && other->client->NPC_class == CLASS_MURJJ	 || other && other->client->NPC_class == CLASS_TRANDOSHAN ||
						other && other->client->NPC_class == CLASS_LANDO ) 
				{
					// AFFECTED ONLY VAMPIRES 
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 7000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 10;  
					other->client->holyTime = cg.time + 7000; 
					other->client->fireDamage = 10;  
					other->client->fireTime = cg.time + 7000; 
					if (  other &&  other->client && other->health <= 0 )
					{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
					if ( other->ghoul2.size() && other->headBolt != -1 )
						{//FIXME: what if already playing effect?
							G_PlayEffect( G_EffectIndex( "holyimmolate/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
						}
					}
				
				}
				break;
				// NECRO MIND
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_REBEL: 
				case CLASS_MENTALIST:
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_BIRD: 
				FX_MindHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 1000;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					
				}
				break;
				// NECRO DIMENSION
				case CLASS_HOWLER:
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_LANDO: 
				FX_DimHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_KNEES1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_KNEES1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
					other->painDebounceTime += 5000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 5000;
					other->client->sonicDamage = 8;  
					other->client->sonicTime = cg.time + 3000; 
				}
				break;
				// NECRO CONFLICT 
				case CLASS_FIREREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_HAZARD_TROOPER:
				case CLASS_GRAN:
				case CLASS_VAMPIRE_FROST: 
				case CLASS_BOBAFETT: 
				case CLASS_WAMPA: 
				case CLASS_DEMON_FIRE: 
				FX_ConflictHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) // A PRETTY POWERFUL IMMOLATION 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 12000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;			;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_KNEES1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_KNEES1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->fireDamage = 10;  
					other->client->fireTime = cg.time + 12000; 
					if (  other &&  other->client && other->health <= 0 )
					{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
					if ( other->ghoul2.size() && other->headBolt != -1 )
						{//FIXME: what if already playing effect?
							G_PlayEffect( G_EffectIndex( "necro_conflict/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
						}
					}
				}
				break;
				// NECRO NATURE
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_DRUID: 
				FX_NatureHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_PULLED_INAIR_F, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_PULLED_INAIR_F, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 5000;
					other->client->natureDamage = 15;  
					other->client->natureTime = cg.time + 2000; 
				}
				break;
				// NECRO ENERGY 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_MORGANKATARN:
				case CLASS_PROTOCOL: 
				FX_EnergyHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 5000;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->s.powerups |= ( 1 << PW_SHOCKED );
					other->client->ps.powerups[PW_SHOCKED] = cg.time + 3000;
					other->client->shockDamage = 12;  
					other->client->shockTime = cg.time + 3000; 
				
				}
				break;
				// NECRO ELECTRO 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID: 
				FX_ElectroHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->s.powerups |= ( 1 << PW_SHOCKED );
					other->client->ps.powerups[PW_SHOCKED] = cg.time + 2000;
					other->client->shockDamage = 6;  
					other->client->shockTime = cg.time + 1500; 
				
				}
				break; 
				// NECRO POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_BEAST: 
				case CLASS_MINEMONSTER:
				case CLASS_REELO: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON:
				FX_NecroPoisonHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->poisonDamage = 8;  
					other->client->poisonTime = cg.time + 10000; 
				}
				break;
				// NECRO TIME 
				case CLASS_ROCKETTROOPER:
				case CLASS_VAMPIRE_ELF3: 
				FX_TimeHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
					other->painDebounceTime += 5000;
					other->client->ps.weaponTime += 5000;
					other->client->ps.torsoAnimTimer += 5000;
				}
				break;
				// NECRO STONE  
				case CLASS_GONK:
				case CLASS_LIZARD:
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 
				FX_RockHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 7000;
					other->painDebounceTime += 7000;
					other->client->ps.weaponTime += 7000;
					other->client->ps.torsoAnimTimer += 7000;
				}
				break;
				// NECRO CHAOS
				case CLASS_PROPHET: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_SENTRY: 
				FX_ChaosHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->hyldenShockDamage = 20;  
					other->client->hyldenShockTime = cg.time + 10000; 
						if (  other &&  other->client && other->health <= 0 )
					{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						if ( other->ghoul2.size() && other->headBolt != -1 )
						{//FIXME: what if already playing effect?
							G_PlayEffect( G_EffectIndex( "necro_chaos/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
						}
					}
				}
				break;
				// NECRO DEATH  
				case CLASS_ARIEL: 
				case CLASS_GHOST: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_JEDI:
				case CLASS_REAVER: 
				case CLASS_NECROREAVER: 
				case CLASS_THRALL: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_JAWA:
				case CLASS_MURJJ: 
				case CLASS_RANCOR: 
				case CLASS_PLAYER:
				case CLASS_VEHICLE: 
				FX_NecroHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->necroDamage = 10;  
					other->client->necroTime = cg.time + 10000; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 10000;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// NECRO SMOG 
				case CLASS_BESPIN_COP: 				
				case CLASS_DARK: 			 
				case CLASS_GOLEM_DARK: 
				case CLASS_TANKREAVER: 
				case CLASS_GOLEM_SMOKE: 
				FX_NecrosmogHitPlayer ( origin, dir, humanoid );
					if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 6000; 
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer; 
					
				}
				break;
				// NECRO VOID  
				case CLASS_SABOTEUR: 
				case CLASS_SHADOWTROOPER: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				FX_NecrovoidHitPlayer ( origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_CHOKE1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_CHOKE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 8000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->voidDamage = 10; 
					other->client->voidTime = cg.time + 4000;
					VectorSubtract( other->currentOrigin, cent->gent->owner->currentOrigin, pushDir );
				}
				break;
				// NECRO MUD 
				case CLASS_SWAMP:
				case CLASS_INTERROGATOR:
				case CLASS_SHARK: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_SPIDERREAVER:
				FX_NecromudHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
					other->painDebounceTime += 5000;
					other->client->ps.weaponTime += 5000; 
				}
				break;
				// NECRO CRYSTAL 
				case CLASS_WEEQUAY:
				case CLASS_BARTENDER:
				case CLASS_GOLEM_ICE: 
				case CLASS_ICEREAVER: 
				case CLASS_CRYSTAL: 
				case CLASS_GALAKMECH: 
				FX_NecroCrystalHitPlayer ( origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
					other->painDebounceTime += 10000;
					other->client->ps.weaponTime += 10000;
					other->client->poisonDamage = 5;  
					other->client->poisonTime = cg.time + 10000; 
				}
					break; 
				case CLASS_FIGHTER: 				
				FX_AMHitPlayer( origin, dir, humanoid );
				// CRASHA!
				/*if ( other && other->client )
				{
					other->client->hyldenShockDamage = 50;  
					other->client->hyldenShockTime = cg.time + 10000; 
						if (  other &&  other->client && other->health <= 0 )
					{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						if ( other->ghoul2.size() && other->headBolt != -1 )
						{//FIXME: what if already playing effect?
							G_PlayEffect( G_EffectIndex( "antimatter/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
						}
					}
				}*/
				break; 
			default:
				break;
				}
			}
//		FX_ConcHitPlayer( origin, dir, humanoid );
		break;

	case WP_THERMAL:
	//	theFxScheduler.PlayEffect( "thermal/explosion", origin, dir );
	//	theFxScheduler.PlayEffect( "thermal/shockwave", origin );
	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
			
				// FORCE
				case CLASS_ARCHER: 
				case CLASS_REAVER: 
				case CLASS_MENTALIST: 
				case CLASS_JEDI:
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PROBE: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_AIRREAVER: 
				theFxScheduler.PlayEffect( "thermal8/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal8/shockwave", origin );
				break;
				// STONE
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF:
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 				
				case CLASS_MONMOTHA:
				case CLASS_GONK:
				case CLASS_UGNAUGHT: 
				theFxScheduler.PlayEffect( "thermal7/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal7/shockwave", origin );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 6000;
					other->painDebounceTime += 6000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}
				break;
				// SOUND 
				case CLASS_BIRD: 
				case CLASS_LYCAN: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_TRANDOSHAN: 
				case CLASS_HOWLER: 
				case CLASS_REBEL: 
				case CLASS_VAMPIRE_SOUND: 
				theFxScheduler.PlayEffect( "thermal6/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal6/shockwave", origin );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					
				}
				break;
				// WATER 
				case CLASS_WARRIOR: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SHARK: 
				case CLASS_ALORA: 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_SWAMP:
				theFxScheduler.PlayEffect( "thermal5/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal5/shockwave", origin );
				if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_ALORA || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_SWAMPTROOPER || other && other->client->NPC_class == CLASS_FISH || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW || 
						other && other->client->NPC_class == CLASS_JEDI || other && other->client->NPC_class == CLASS_LUKE || 
						other && other->client->NPC_class == CLASS_WAMPA || other && other->client->NPC_class == CLASS_MINEMONSTER || 
						other && other->client->NPC_class == CLASS_TAVION ||other && other->client->NPC_class == CLASS_COMMANDO ||
						other && other->client->NPC_class == CLASS_MURJJ	 || other && other->client->NPC_class == CLASS_TRANDOSHAN ||
						other && other->client->NPC_class == CLASS_LANDO ) 
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}
				break;
				// SUNLIGHT 
				case CLASS_ALCHEMIST:				 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GALAK:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				theFxScheduler.PlayEffect( "thermal3/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal3/shockwave", origin );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "thermal3/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
				}
				break;
				// ELETTRICA 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_GRAN: 
				case CLASS_SABER_DROID:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT: 
				theFxScheduler.PlayEffect( "thermal9/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal9/shockwave", origin );
				if ( other && other->client ) 
				{
					other->s.powerups |= ( 1 << PW_SHOCKED );
					other->client->ps.powerups[PW_SHOCKED] = cg.time + 5000;
				}
				break;
				// HOLY 
				case CLASS_CENOBITA: 
				case CLASS_STORMTROOPER:
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_LUKE: 
				theFxScheduler.PlayEffect( "thermal10/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal10/shockwave", origin );
				if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_ALORA || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_SWAMPTROOPER || other && other->client->NPC_class == CLASS_FISH || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW || 
						other && other->client->NPC_class == CLASS_JEDI || other && other->client->NPC_class == CLASS_LUKE || 
						other && other->client->NPC_class == CLASS_WAMPA || other && other->client->NPC_class == CLASS_MINEMONSTER || 
						other && other->client->NPC_class == CLASS_TAVION ||other && other->client->NPC_class == CLASS_COMMANDO ||
						other && other->client->NPC_class == CLASS_MURJJ	 || other && other->client->NPC_class == CLASS_TRANDOSHAN ||
						other && other->client->NPC_class == CLASS_LANDO ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 7000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 10;  
					other->client->holyTime = cg.time + 7000; 
				}
				break;
				// DARKNESS 
				case CLASS_VAMPIRE: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:			 
				case CLASS_VAMPIRE_BOUNTY:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_SHADOWTROOPER:
				case CLASS_BESPIN_COP: 
				case CLASS_SABOTEUR: 
				theFxScheduler.PlayEffect( "thermal2/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal2/shockwave", origin );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					
				}
				break;
				// ACIDIC 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN2: 
				case CLASS_KAIN3: 
				case CLASS_DUMAHIM: 
				case CLASS_WEEQUAY:
				case CLASS_LIZARD: 
				case CLASS_INTERROGATOR: 
				case CLASS_R2D2:
				case CLASS_R5D2: 	
				case CLASS_REELO: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:	
				case CLASS_DEMON_POISON: 
				case CLASS_BEAST:  			  
				case CLASS_GOLEM_POISON:
				case CLASS_POISON: 
				theFxScheduler.PlayEffect( "thermal/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->acidDamage = 10;  
					other->client->acidTime = cg.time + 10000; 
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "thermal/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
				}
				break;
				// UNDEAD 
				case CLASS_TENTACLE: 
				case CLASS_ELDERGOD: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_NECROREAVER: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_RANCOR:
				case CLASS_MURJJ: 
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_ZOMBIE: 
				theFxScheduler.PlayEffect( "thermal11/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal11/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 8;  
					other->client->necroTime = cg.time + 15000; 
					other->client->ps.torsoAnimTimer += 15000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// HYLDEN 
				case CLASS_PROPHET: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				theFxScheduler.PlayEffect( "thermal12/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal12/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->hyldenDamage = 11;  
					other->client->hyldenTime = cg.time + 11000; 
					other->client->ps.torsoAnimTimer += 11000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "thermal12/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
				}
				break;
				// BOMBA DI FUOCO 	
				case CLASS_RAZIELHIM: 
				case CLASS_ESKANDOR: 
				case CLASS_HUNTER:			 
				case CLASS_DEMON_FIRE:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_FIREREAVER: 
				case CLASS_TURELIM: 
				case CLASS_FIRE: 
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_REBORN: 
				case CLASS_WAMPA: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER: 
				case CLASS_RODIAN: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "thermal4/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal4/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->fireDamage = 5;  
					other->client->fireTime = cg.time + 3000; 
				}
				break;  
				// MACE BOMB
				case CLASS_SCOUT: 
				case CLASS_MARK1: 
				case CLASS_MARK2:				 
				theFxScheduler.PlayEffect( "thermal13/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal13/shockwave", origin );
				break; 
				// PIANTE SPUNTONE
				case CLASS_IVY: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO: 
				case CLASS_PRISONER: 
				theFxScheduler.PlayEffect( "thermal14/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal14/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->natureDamage = 4;  
					other->client->natureTime = cg.time + 10000; 
				}
				break; 
				// CRISTALLI DI GHIACCIO 
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_BARTENDER:
				case CLASS_GALAKMECH: 
				theFxScheduler.PlayEffect( "thermal15/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal15/shockwave", origin );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 7000;
					other->painDebounceTime += 7000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
				}
				break; 
				// CROCI 
				case CLASS_POLTER: 
				case CLASS_JAWA:
				case CLASS_GHOST: 
				theFxScheduler.PlayEffect( "thermal16/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal16/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 16;  
					other->client->necroTime = cg.time + 10000; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
					other->painDebounceTime += 10000;
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_PULLED_INAIR_B, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_PULLED_INAIR_B, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break; 
				// BARA 
				case CLASS_COMMANDO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				theFxScheduler.PlayEffect( "thermal17/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal17/shockwave", origin );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 8;  
					other->client->necroTime = cg.time + 8000; 
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 8000;
					other->painDebounceTime += 8000;
					other->client->ps.torsoAnimTimer += 8000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_PULLED_INAIR_F, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_PULLED_INAIR_F, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break; 
				// ANGELO 
				case CLASS_JANOS: 
				case CLASS_JAN: 
				case CLASS_ARIEL: 
				case CLASS_SARAFAN: 
				theFxScheduler.PlayEffect( "thermal18/explosion", origin, dir );
				theFxScheduler.PlayEffect( "thermal18/shockwave", origin );
				if ( other && other->client->NPC_class == CLASS_REBORN || other && other->client->NPC_class == CLASS_KYLE || 
						other && other->client->NPC_class == CLASS_ALORA || other && other->client->NPC_class == CLASS_MONMOTHA || 
						other && other->client->NPC_class == CLASS_TUSKEN || other && other->client->NPC_class == CLASS_GLIDER || 
						other && other->client->NPC_class == CLASS_NOGHRI || other && other->client->NPC_class == CLASS_FLIER2 || 
						other && other->client->NPC_class == CLASS_SWAMPTROOPER || other && other->client->NPC_class == CLASS_FISH || 
						other && other->client->NPC_class == CLASS_WEEQUAY || other && other->client->NPC_class == CLASS_LIZARD || 
						other && other->client->NPC_class == CLASS_GRAN || other && other->client->NPC_class == CLASS_CLAW || 
						other && other->client->NPC_class == CLASS_JEDI || other && other->client->NPC_class == CLASS_LUKE || 
						other && other->client->NPC_class == CLASS_WAMPA || other && other->client->NPC_class == CLASS_MINEMONSTER || 
						other && other->client->NPC_class == CLASS_TAVION ||other && other->client->NPC_class == CLASS_COMMANDO ||
						other && other->client->NPC_class == CLASS_MURJJ	 || other && other->client->NPC_class == CLASS_TRANDOSHAN ||
						other && other->client->NPC_class == CLASS_LANDO ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 12;  
					other->client->holyTime = cg.time + 10000; 
				}		 
				break;
			default:
				break;
				}
			}
		break;

	case WP_EMPLACED_GUN:
		FX_EmplacedHitPlayer( origin, dir, (cent->gent&&cent->gent->alt_fire) );
		if ( other && other->client ) 
		{
					other->client->hyldenDamage = 5;  
					other->client->hyldenTime = cg.time + 5500; 
					other->client->ps.torsoAnimTimer += 5500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( other->ghoul2.size() && other->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "emplaced/disintegration" ), other->playerModel, other->chestBolt, other->s.number, other->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
		}
		break;
	case WP_TRIP_MINE:
		theFxScheduler.PlayEffect( "tripmine/explosion", origin, dir );
		if ( other && other->client ) 
		{
					other->client->ps.torsoAnimTimer += 2000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		}
		break;

	case WP_DET_PACK:
		theFxScheduler.PlayEffect( "detpack/explosion", origin, dir );
		if ( other && other->client )
			{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 10000;
					other->painDebounceTime += 10000;
					other->client->ps.torsoAnimTimer += 10000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					
			}
		break;

	case WP_TURRET:
		theFxScheduler.PlayEffect( "turret/flesh_impact", origin, dir );
		break;

	case WP_ATST_MAIN:
		FX_EmplacedHitWall( origin, dir, qfalse );
		break;

	case WP_ATST_SIDE:
		if ( altFire )
		{
			theFxScheduler.PlayEffect( "atst/side_alt_explosion", origin, dir );
		}
		else
		{
			theFxScheduler.PlayEffect( "atst/side_main_impact", origin, dir );
		}
		break;
	case WP_TUSKEN_RIFLE:
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_BIRD: 
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				FX_Tusken2ShotWeaponHitPlayer( other, origin, dir, humanoid );
				break;
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				FX_Tusken3ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client )
				{
					other->client->ps.pm_type = PM_DEAD;
					other->s.pos.trTime += 5000;
					other->painDebounceTime += 5000;
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					other->client->ps.torsoAnimTimer += 5000;
				}
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				FX_Tusken4ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					
				}
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				FX_Tusken5ShotWeaponHitPlayer( other, origin, dir, humanoid );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				FX_Tusken6ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client )
				{
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						}
				}
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				FX_Tusken7ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->holyDamage = 10;  
					other->client->holyTime = cg.time + 3500; 
				}
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				FX_Tusken1ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					G_ClearEnemy( other );
					other->client->ps.torsoAnimTimer += 5000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					
				}
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				FX_Tusken8ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->drainDamage = 1;  
					other->client->drainTime = cg.time + 6000; 
				}
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				FX_Tusken0ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->necroDamage = 4;  
					other->client->necroTime = cg.time + 7500; 
					other->client->ps.torsoAnimTimer += 7500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				FX_Tusken9ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->hyldenDamage = 6;  
					other->client->hyldenTime = cg.time + 5500; 
					other->client->ps.torsoAnimTimer += 5500;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( other, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( other, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					if (  other &&  other->client && other->health <= 0 )
						{
							other->s.powerups |= ( 1 << PW_DISRUPTION );
							other->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						}
				}
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				FX_Tusken10ShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->poisonDamage = 3;  
					other->client->poisonTime = cg.time + 2000; 
				}
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				FX_TuskenShotWeaponHitPlayer( other, origin, dir, humanoid );
				if ( other && other->client ) 
				{
					other->client->fireDamage = 1;  
					other->client->fireTime = cg.time + 1000; 
				}
				break;  
			default:
				break;
				}
			}
		// FX_TuskenShotWeaponHitPlayer( other, origin, dir, humanoid );
		break;

	case WP_NOGHRI_STICK:
		FX_NoghriShotWeaponHitPlayer( other, origin, dir, humanoid );
		break;
	}
}
