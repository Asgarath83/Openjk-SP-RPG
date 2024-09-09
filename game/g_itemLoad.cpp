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

//g_itemLoad.cpp
//reads in ext_data\items.dat to bg_itemlist[]

// leave this line at the top for all g_xxxx.cpp files...
#include "g_headers.h"




#include "g_local.h"
#include "g_items.h"

#define PICKUPSOUND "sound/weapons/w_pkup.wav"

//qboolean COM_ParseInt( char **data, int *i );
//qboolean COM_ParseString( char **data, char **s ); 
//qboolean COM_ParseFloat( char **data, float *f );

extern gitem_t	bg_itemlist[];

struct itemParms_s
{
	int	itemNum;
} itemParms;


static void IT_ClassName (const char **holdBuf);
static void IT_Count (const char **holdBuf);
static void IT_Icon (const char **holdBuf);
static void IT_Min (const char **holdBuf);
static void IT_Max (const char **holdBuf);
static void IT_Name (const char **holdBuf);
static void IT_PickupSound (const char **holdBuf);
static void IT_Tag (const char **holdBuf);
static void IT_Type (const char **holdBuf);
static void IT_WorldModel (const char **holdBuf);


typedef struct 
{
	const char	*parmName;
	void	(*func)(const char **holdBuf);
} itemParms_t;


#define IT_PARM_MAX 10

itemParms_t ItemParms[IT_PARM_MAX] = 
{
	{ "itemname",			IT_Name },
	{ "classname",		IT_ClassName },
	{ "count",			IT_Count },
	{ "icon",				IT_Icon },
	{ "min",				IT_Min },
	{ "max",				IT_Max },
	{ "pickupsound",		IT_PickupSound },
	{ "tag",				IT_Tag },
	{ "type",				IT_Type },
	{ "worldmodel",		IT_WorldModel },
};

static void IT_SetDefaults()
{

	bg_itemlist[itemParms.itemNum].mins[0] = -16;
	bg_itemlist[itemParms.itemNum].mins[1] = -16;
	bg_itemlist[itemParms.itemNum].mins[2] = -2;

	bg_itemlist[itemParms.itemNum].maxs[0] = 16;
	bg_itemlist[itemParms.itemNum].maxs[1] = 16;
	bg_itemlist[itemParms.itemNum].maxs[2] = 16;


	bg_itemlist[itemParms.itemNum].pickup_sound = PICKUPSOUND;	//give it a default sound
	bg_itemlist[itemParms.itemNum].precaches = NULL;
	bg_itemlist[itemParms.itemNum].sounds = NULL;
}

static void IT_Name(const char **holdBuf)
{
	int itemNum;
	const char	*tokenStr;

	if (COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}


	if (!Q_stricmp(tokenStr,"ITM_NONE"))	
	{
		itemNum = ITM_NONE;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_SABER_PICKUP"))	
	{
			itemNum = ITM_SABER_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BLASTER_PISTOL_PICKUP"))	
	{
			itemNum = ITM_BLASTER_PISTOL_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BLASTER_PICKUP"))	
	{
		itemNum = ITM_BLASTER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_DISRUPTOR_PICKUP"))	
	{
		itemNum = ITM_DISRUPTOR_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_REPEATER_PICKUP"))	
	{
		itemNum = ITM_REPEATER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_FLECHETTE_PICKUP"))	
	{
		itemNum = ITM_FLECHETTE_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_BOWCASTER_PICKUP"))	
	{
		itemNum = ITM_BOWCASTER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_ROCKET_LAUNCHER_PICKUP"))	
	{
		itemNum = ITM_ROCKET_LAUNCHER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_NOGHRI_STICK_PICKUP"))	
	{
		itemNum = ITM_NOGHRI_STICK_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_DEMP2_PICKUP"))	
	{
		itemNum = ITM_DEMP2_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_CONCUSSION_RIFLE_PICKUP"))	
	{
		itemNum = ITM_CONCUSSION_RIFLE_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_THERMAL_DET_PICKUP"))	
	{
		itemNum = ITM_THERMAL_DET_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_TRIP_MINE_PICKUP"))	
	{
		itemNum = ITM_TRIP_MINE_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_DET_PACK_PICKUP"))	
	{
		itemNum = ITM_DET_PACK_PICKUP;
	}	
	// GUERRIERO	
	else if (!Q_stricmp(tokenStr,"ITM_MELEE"))	
	{
			itemNum = ITM_MELEE;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BOW_PICKUP"))	
	{
		itemNum = ITM_BOW_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_CROSSBOW_PICKUP"))	
	{
		itemNum = ITM_CROSSBOW_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_SLING_PICKUP"))	
	{
		itemNum = ITM_SLING_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_THROWABLE_PICKUP"))	
	{
		itemNum = ITM_THROWABLE_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_STUN_BATON_PICKUP"))	
	{
		itemNum = ITM_STUN_BATON_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_TUSKEN_STAFF_PICKUP"))	
	{
		itemNum = ITM_TUSKEN_STAFF_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_TUSKEN_RIFLE_PICKUP"))	
	{
		itemNum = ITM_TUSKEN_RIFLE_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_CANNON_PICKUP"))	
	{
		itemNum = ITM_CANNON_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_HOOKER_PICKUP"))	
	{
		itemNum = ITM_HOOKER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_SCEPTER_PICKUP"))	
	{
		itemNum = ITM_SCEPTER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_DARK_PICKUP"))	
	{
		itemNum = ITM_DARK_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_LIGHT_PICKUP"))	
	{
		itemNum = ITM_LIGHT_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_LIGHTNING_PICKUP"))	
	{
		itemNum = ITM_LIGHTNING_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_FIRE_PICKUP"))	
	{
		itemNum = ITM_FIRE_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_FLAME_PICKUP"))	
	{
		itemNum = ITM_FLAME_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_AIR_PICKUP"))	
	{
		itemNum = ITM_AIR_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_WATER_PICKUP"))	
	{
		itemNum = ITM_WATER_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_EARTH_PICKUP"))	
	{
		itemNum = ITM_EARTH_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_SONIC_PICKUP"))	
	{
		itemNum = ITM_SONIC_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_PSYCHIC_PICKUP"))	
	{
		itemNum = ITM_PSYCHIC_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_WOOD_PICKUP"))	
	{
		itemNum = ITM_WOOD_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_METAL_PICKUP"))	
	{
		itemNum = ITM_METAL_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_SPIRIT_PICKUP"))	
	{
		itemNum = ITM_SPIRIT_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_POISON_PICKUP"))	
	{
		itemNum = ITM_POISON_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_HOLY_PICKUP"))	
	{
		itemNum = ITM_HOLY_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_NECRO_PICKUP"))	
	{
		itemNum = ITM_NECRO_PICKUP;
	}	
	else if (!Q_stricmp(tokenStr,"ITM_BRYAR_PISTOL_PICKUP"))	
	{
		itemNum = ITM_BRYAR_PISTOL_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_EMPLACED_GUN_PICKUP"))	
	{
		itemNum = ITM_EMPLACED_GUN_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ATST_MAIN_PICKUP"))	
	{
		itemNum = ITM_ATST_MAIN_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ATST_SIDE_PICKUP"))	
	{
		itemNum = ITM_ATST_SIDE_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_TIE_FIGHTER_PICKUP"))	
	{
		itemNum = ITM_TIE_FIGHTER_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_JAWA_PICKUP"))	
	{
		itemNum = ITM_JAWA_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BOT_LASER_PICKUP"))	
	{
		itemNum = ITM_BOT_LASER_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_RAPID_FIRE_CONC_PICKUP"))	
	{
		itemNum = ITM_RAPID_FIRE_CONC_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ANTIMATTER_PICKUP"))	
	{
		itemNum = ITM_ANTIMATTER_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_DEFENSE_PICKUP"))	
	{
		itemNum = ITM_DEFENSE_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_DRONE_PICKUP"))	
	{
		itemNum = ITM_DRONE_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_POSITRON_PICKUP"))	
	{
		itemNum = ITM_POSITRON_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_TURRET_PICKUP"))	
	{
		itemNum = ITM_TURRET_PICKUP;
	}
	// INVENTARIO EVOCAZIONI
	else if (!Q_stricmp(tokenStr,"ITM_SUMMON1_PICKUP"))	
	{
		itemNum = ITM_SUMMON1_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SUMMON2_PICKUP"))	
	{
		itemNum = ITM_SUMMON2_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SUMMON3_PICKUP"))	
	{
		itemNum = ITM_SUMMON3_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SUMMON4_PICKUP"))	
	{
		itemNum = ITM_SUMMON4_PICKUP;
	}
	// FORME 
	else if (!Q_stricmp(tokenStr,"ITM_MORPH1_PICKUP"))	
	{
		itemNum = ITM_MORPH1_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_MORPH2_PICKUP"))	
	{
		itemNum = ITM_MORPH2_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_MORPH3_PICKUP"))	
	{
		itemNum = ITM_MORPH3_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_MORPH4_PICKUP"))	
	{
		itemNum = ITM_MORPH4_PICKUP;
	}
	// OGGETTI 
	else if (!Q_stricmp(tokenStr,"ITM_ITEM1_PICKUP"))	
	{
		itemNum = ITM_ITEM1_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM2_PICKUP"))	
	{
		itemNum = ITM_ITEM2_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM3_PICKUP"))	
	{
		itemNum = ITM_ITEM3_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM4_PICKUP"))	
	{
		itemNum = ITM_ITEM4_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM5_PICKUP"))	
	{
		itemNum = ITM_ITEM5_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM6_PICKUP"))	
	{
		itemNum = ITM_ITEM6_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM7_PICKUP"))	
	{
		itemNum = ITM_ITEM7_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM8_PICKUP"))	
	{
		itemNum = ITM_ITEM8_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM9_PICKUP"))	
	{
		itemNum = ITM_ITEM9_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM10_PICKUP"))	
	{
		itemNum = ITM_ITEM10_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM11_PICKUP"))	
	{
		itemNum = ITM_ITEM11_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_ITEM12_PICKUP"))	
	{
		itemNum = ITM_ITEM12_PICKUP;
	}
	//ammo
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_FORCE_PICKUP"))
	{
		itemNum = ITM_AMMO_FORCE_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_BLASTER_PICKUP"))
	{
		itemNum = ITM_AMMO_BLASTER_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_POWERCELL_PICKUP"))
	{
		itemNum = ITM_AMMO_POWERCELL_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_METAL_BOLTS_PICKUP"))
	{
		itemNum = ITM_AMMO_METAL_BOLTS_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_ROCKETS_PICKUP"))
	{
		itemNum = ITM_AMMO_ROCKETS_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_EMPLACED_PICKUP"))
	{
		itemNum = ITM_AMMO_EMPLACED_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_THERMAL_PICKUP"))
	{
		itemNum = ITM_AMMO_THERMAL_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_TRIPMINE_PICKUP"))
	{
		itemNum = ITM_AMMO_TRIPMINE_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_AMMO_DETPACK_PICKUP"))
	{
		itemNum = ITM_AMMO_DETPACK_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_HEAL_PICKUP"))
	{
		itemNum = ITM_FORCE_HEAL_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_LEVITATION_PICKUP"))
	{
		itemNum = ITM_FORCE_LEVITATION_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_SPEED_PICKUP"))
	{
		itemNum = ITM_FORCE_SPEED_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_PUSH_PICKUP"))
	{
		itemNum = ITM_FORCE_PUSH_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_PULL_PICKUP"))
	{
		itemNum = ITM_FORCE_PULL_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_TELEPATHY_PICKUP"))
	{
		itemNum = ITM_FORCE_TELEPATHY_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_GRIP_PICKUP"))
	{
		itemNum = ITM_FORCE_GRIP_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_LIGHTNING_PICKUP"))
	{
		itemNum = ITM_FORCE_LIGHTNING_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_FORCE_SABERTHROW_PICKUP"))
	{
		itemNum = ITM_FORCE_SABERTHROW_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BATTERY_PICKUP"))	
	{
		itemNum = ITM_BATTERY_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SEEKER_PICKUP"))	
	{
		itemNum = ITM_SEEKER_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SHIELD_PICKUP"))	
	{
			itemNum = ITM_SHIELD_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BACTA_PICKUP"))	
	{
			itemNum = ITM_BACTA_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_DATAPAD_PICKUP"))	
	{
			itemNum = ITM_DATAPAD_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BINOCULARS_PICKUP"))	
	{
			itemNum = ITM_BINOCULARS_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SENTRY_GUN_PICKUP"))	
	{
			itemNum = ITM_SENTRY_GUN_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_LA_GOGGLES_PICKUP"))	
	{
			itemNum = ITM_LA_GOGGLES_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_MEDPAK_PICKUP"))	
	{
			itemNum = ITM_MEDPAK_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SHIELD_SM_PICKUP"))	
	{
			itemNum = ITM_SHIELD_SM_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SHIELD_LRG_PICKUP"))	
	{
			itemNum = ITM_SHIELD_LRG_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_GOODIE_KEY_PICKUP"))	
	{
			itemNum = ITM_GOODIE_KEY_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SECURITY_KEY_PICKUP"))	
	{
			itemNum = ITM_SECURITY_KEY_PICKUP;
	}
	else
	{
		itemNum = 0;
		gi.Printf("WARNING: bad itemname in external item data '%s'\n", tokenStr);
	}

	itemParms.itemNum = itemNum;	
//	++bg_numItems;

	IT_SetDefaults();
}

static void IT_ClassName(const char **holdBuf)
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
		gi.Printf("WARNING: weaponclass too long in external ITEMS.DAT '%s'\n", tokenStr);
	}

	bg_itemlist[itemParms.itemNum].classname = G_NewString(tokenStr);

//	Q_strncpyz(bg_itemlist[itemParms.itemNum].classname,tokenStr,len);

}

static void IT_WorldModel(const char **holdBuf)
{
	int len;
	const char	*tokenStr;

	if (COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf("WARNING: world model too long in external ITEMS.DAT '%s'\n", tokenStr);
	}

	bg_itemlist[itemParms.itemNum].world_model = G_NewString(tokenStr);

//	Q_strncpyz(bg_itemlist[itemParms.itemNum].world_model[0],tokenStr,len);

}

static void IT_Tag(const char **holdBuf)
{
	int tag;
	const char	*tokenStr;

	if (COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	if (!Q_stricmp(tokenStr,"WP_NONE"))	
	{
		tag = WP_NONE;
	}
	// SHOOTER
	else if (!Q_stricmp(tokenStr,"WP_SABER"))	
	{
		tag = WP_SABER;
	}
	else if (!Q_stricmp(tokenStr,"WP_BLASTER_PISTOL"))	
		{
			tag = WP_BLASTER_PISTOL;
		}
	else if (!Q_stricmp(tokenStr,"WP_BLASTER"))	
		{
			tag = WP_BLASTER;
		}
	else if (!Q_stricmp(tokenStr,"WP_DISRUPTOR"))	
		{
			tag = WP_DISRUPTOR;
		}
	else if (!Q_stricmp(tokenStr,"WP_REPEATER"))	
		{
			tag = WP_REPEATER;
		}
	else if (!Q_stricmp(tokenStr,"WP_FLECHETTE"))	
		{
			tag = WP_FLECHETTE;
		}
	else if (!Q_stricmp(tokenStr,"WP_BOWCASTER"))	
		{
			tag = WP_BOWCASTER;
		}
	else if (!Q_stricmp(tokenStr,"WP_ROCKET_LAUNCHER"))	
		{
			tag = WP_ROCKET_LAUNCHER;
		}
	else if (!Q_stricmp(tokenStr,"WP_NOGHRI_STICK"))
		{	
			tag = WP_NOGHRI_STICK;
		}
	else if (!Q_stricmp(tokenStr,"WP_DEMP2"))	
		{
			tag = WP_DEMP2;
		}
	else if (!Q_stricmp(tokenStr,"WP_CONCUSSION"))	
		{
			tag = WP_CONCUSSION;
		}
	else if (!Q_stricmp(tokenStr,"WP_THERMAL"))	
		{
			tag = WP_THERMAL;
		}
	else if (!Q_stricmp(tokenStr,"WP_TRIP_MINE"))	
		{
			tag = WP_TRIP_MINE;
		}
	else if (!Q_stricmp(tokenStr,"WP_DET_PACK"))	
		{	
			tag = WP_DET_PACK;
		}
	// GUERRIERO 
	else if (!Q_stricmp(tokenStr,"WP_MELEE"))	
		{
			tag = WP_MELEE;
		}
	else if (!Q_stricmp(tokenStr,"WP_BOW"))	
		{
			tag = WP_BOW;
		}
	else if (!Q_stricmp(tokenStr,"WP_CROSSBOW"))	
		{
			tag = WP_CROSSBOW;
		}
	else if (!Q_stricmp(tokenStr,"WP_SLING"))	
		{
			tag = WP_SLING;
		}
	else if (!Q_stricmp(tokenStr,"WP_THROWABLE"))	
		{
			tag = WP_THROWABLE;
		}
	else if (!Q_stricmp(tokenStr,"WP_STUN_BATON"))	
		{
			tag = WP_STUN_BATON;
		}
	else if (!Q_stricmp(tokenStr,"WP_TUSKEN_STAFF"))
		{
			tag = WP_TUSKEN_STAFF;
		}
	else if (!Q_stricmp(tokenStr,"WP_TUSKEN_RIFLE"))
		{
			tag = WP_TUSKEN_RIFLE;
		}
	else if (!Q_stricmp(tokenStr,"WP_CANNON"))
		{
			tag = WP_CANNON;
		}
	else if (!Q_stricmp(tokenStr,"WP_HOOKER"))	
		{
			tag = WP_HOOKER;
		}
	else if (!Q_stricmp(tokenStr,"WP_SCEPTER"))
		{
			tag = WP_SCEPTER;
		}
	// WIZARD 
	else if (!Q_stricmp(tokenStr,"WP_DARK"))	
		{
			tag = WP_DARK;
		}
	else if (!Q_stricmp(tokenStr,"WP_LIGHT"))	
		{
			tag = WP_LIGHT;
		}
	else if (!Q_stricmp(tokenStr,"WP_LIGHTNING"))	
		{
		 tag = WP_LIGHTNING;
		}
	else if (!Q_stricmp(tokenStr,"WP_FIRE"))	
		{
			tag = WP_FIRE;
		}
	else if (!Q_stricmp(tokenStr,"WP_FLAME"))	
		{	
			tag = WP_FLAME;
		}
	else if (!Q_stricmp(tokenStr,"WP_AIR"))	
		{
			tag = WP_AIR;
		}
	else if (!Q_stricmp(tokenStr,"WP_WATER"))
		{
			tag = WP_WATER;
		}
	else if (!Q_stricmp(tokenStr,"WP_EARTH"))
		{
			tag = WP_EARTH;
		}
	else if (!Q_stricmp(tokenStr,"WP_SONIC"))
		{
			tag = WP_SONIC;
		}
	else if (!Q_stricmp(tokenStr,"WP_PSYCHIC"))	
		{
			tag = WP_PSYCHIC;
		}
	else if (!Q_stricmp(tokenStr,"WP_WOOD"))
		{
			tag = WP_WOOD;
		}
	else if (!Q_stricmp(tokenStr,"WP_METAL"))	
		{
			tag = WP_METAL;
		}
	else if (!Q_stricmp(tokenStr,"WP_SPIRIT"))
		{
			tag = WP_SPIRIT;
		}
	else if (!Q_stricmp(tokenStr,"WP_POISON"))	
		{
			tag = WP_POISON;
		}
	else if (!Q_stricmp(tokenStr,"WP_HOLY"))
		{
			tag = WP_HOLY;
		}
	else if (!Q_stricmp(tokenStr,"WP_NECRO"))	
		{
			tag = WP_NECRO;
		}
	else if (!Q_stricmp(tokenStr,"WP_BRYAR_PISTOL"))
	{
		tag = WP_BRYAR_PISTOL;
	}
	else if (!Q_stricmp(tokenStr,"WP_EMPLACED_GUN"))
	{
		tag = WP_EMPLACED_GUN;
	}
	else if (!Q_stricmp(tokenStr,"WP_ATST_MAIN"))
	{	tag = WP_ATST_MAIN;
}
	else if (!Q_stricmp(tokenStr,"WP_ATST_SIDE"))
	{	tag = WP_ATST_SIDE;
	}
	else if (!Q_stricmp(tokenStr,"WP_TIE_FIGHTER"))
	{
			tag = WP_TIE_FIGHTER;
	}
	else if (!Q_stricmp(tokenStr,"WP_JAWA"))
	{	
		tag = WP_JAWA;
	
	}
	else if (!Q_stricmp(tokenStr,"WP_BOT_LASER"))	
	{	
		tag = WP_BOT_LASER;
	}
	else if (!Q_stricmp(tokenStr,"WP_RAPID_FIRE_CONC"))
	{	
		tag = WP_RAPID_FIRE_CONC;
	}
	else if (!Q_stricmp(tokenStr,"WP_ANTIMATTER"))	
	{
		tag = WP_ANTIMATTER;
	}
	else if (!Q_stricmp(tokenStr,"WP_DEFENSE"))	
	{
			tag = WP_DEFENSE;
	}
	else if (!Q_stricmp(tokenStr,"WP_DRONE"))	
	{
			tag = WP_DRONE;
	}
	else if (!Q_stricmp(tokenStr,"WP_POSITRON"))	
	{
			tag = WP_POSITRON;
	}
	else if (!Q_stricmp(tokenStr,"WP_TURRET"))
	{	
		tag = WP_TURRET;
	}
	// NEW INVENTORY 
	// INVENTARIO CONFIG 5: ITEMS
	else if (!Q_stricmp(tokenStr,"WP_SUMMON1"))	
	{
		tag = WP_SUMMON1;
	}
	else if (!Q_stricmp(tokenStr,"WP_SUMMON2"))	
	{
		tag = WP_SUMMON2;
	}
	else if (!Q_stricmp(tokenStr,"WP_SUMMON3"))	
	{
		tag = WP_SUMMON3;
	}
	else if (!Q_stricmp(tokenStr,"WP_SUMMON4"))	
	{
		tag = WP_SUMMON4;
	}
	// forma 
	else if (!Q_stricmp(tokenStr,"WP_MORPH1"))	
	{
		tag = WP_MORPH1;
	}
	else if (!Q_stricmp(tokenStr,"WP_MORPH2"))	
	{
		tag = WP_MORPH2;
	}
	else if (!Q_stricmp(tokenStr,"WP_MORPH3"))	
	{
		tag = WP_MORPH3;
	}
	else if (!Q_stricmp(tokenStr,"WP_MORPH4"))	
	{
		tag = WP_MORPH4;
	}
	// Artefatti 
	else if (!Q_stricmp(tokenStr,"WP_ITEM1"))	
	{
		tag = WP_ITEM1;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM2"))	
	{
		tag = WP_ITEM2;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM3"))	
	{
		tag = WP_ITEM3;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM4"))	
	{
		tag = WP_ITEM4;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM5"))	
	{
		tag = WP_ITEM5;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM6"))	
	{
		tag = WP_ITEM6;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM7"))
	{
		tag = WP_ITEM7;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM8"))
	{
		tag = WP_ITEM8;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM9"))
	{	
		tag = WP_ITEM9;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM10"))	
	{
		tag = WP_ITEM10;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM11"))
	{
		tag = WP_ITEM11;
	}
	else if (!Q_stricmp(tokenStr,"WP_ITEM12"))	
	{
		tag = WP_ITEM12;
	}
	// AMMOS 
	else if (!Q_stricmp(tokenStr,"AMMO_FORCE"))
	{
		tag = AMMO_FORCE;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_BLASTER"))	
	{
		tag = AMMO_BLASTER;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_POWERCELL"))	
	{
		tag = AMMO_POWERCELL;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_METAL_BOLTS"))	
	{
		tag = AMMO_METAL_BOLTS;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_ROCKETS"))	
	{
		tag = AMMO_ROCKETS;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_EMPLACED"))	
	{
		tag = AMMO_EMPLACED;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_THERMAL"))	
	{
		tag = AMMO_THERMAL;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_TRIPMINE"))	
	{
		
		tag = AMMO_TRIPMINE;
	}
	else if (!Q_stricmp(tokenStr,"AMMO_DETPACK"))	
	{ 
		tag = AMMO_DETPACK;
	}
	else if (!Q_stricmp(tokenStr,"FP_HEAL"))	
	{
		tag = FP_HEAL;
	}
	else if (!Q_stricmp(tokenStr,"FP_LEVITATION"))	
	{
		tag = FP_LEVITATION;
	}
	else if (!Q_stricmp(tokenStr,"FP_SPEED"))	
	{
		tag = FP_SPEED;
	}
	else if (!Q_stricmp(tokenStr,"FP_PUSH"))	
	{
		tag = FP_PUSH;
	}
	else if (!Q_stricmp(tokenStr,"FP_PULL"))	
	{
		tag = FP_PULL;
	}
	else if (!Q_stricmp(tokenStr,"FP_TELEPATHY"))	
	{
		tag = FP_TELEPATHY;
	}
	else if (!Q_stricmp(tokenStr,"FP_SABERTHROW"))	
	{
		tag = FP_SABERTHROW;
	}
	else if (!Q_stricmp(tokenStr,"FP_STUN"))	
	{
		tag = FP_STUN;
	}
	else if (!Q_stricmp(tokenStr,"FP_HATE"))	
	{
		tag = FP_HATE;
	}
	else if (!Q_stricmp(tokenStr,"FP_CONTROLMIND"))	
	{
		tag = FP_CONTROLMIND;
	}
	else if (!Q_stricmp(tokenStr,"FP_FREEZE"))	
	{
		tag = FP_FREEZE;
	}
	else if (!Q_stricmp(tokenStr,"FP_HEALOTHER"))	
	{
		tag = FP_HEALOTHER;
	}
	else if (!Q_stricmp(tokenStr,"FP_WRACK"))	
	{
		tag = FP_WRACK;
	}
	else if (!Q_stricmp(tokenStr,"FP_LIGHTNING"))	
	{
		tag = FP_LIGHTNING;
	}
	else if (!Q_stricmp(tokenStr,"FP_SABERTHROW"))	
	{
		tag = FP_SABERTHROW;
	}
	// GLYPHS
	else if (!Q_stricmp(tokenStr,"FP_FORCEGLYPH"))	
	{
		tag = FP_FORCEGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_STONEGLYPH"))	
	{
		tag = FP_STONEGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_SOUNDGLYPH"))	
	{
		tag = FP_SOUNDGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_WATERGLYPH"))	
	{
		tag = FP_WATERGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_FIREGLYPH"))	
	{
		tag = FP_FIREGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_SUNGLYPH"))	
	{
		tag = FP_SUNGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_WINDGLYPH"))	
	{
		tag = FP_WINDGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_DARKGLYPH"))	
	{
		tag = FP_DARKGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_HOLYGLYPH"))	
	{
		tag = FP_HOLYGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"FP_NECROGLYPH"))	
	{
		tag = FP_NECROGLYPH;
	}
	else if (!Q_stricmp(tokenStr,"ITM_BATTERY_PICKUP"))	
	{
		tag = ITM_BATTERY_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"INV_SEEKER"))	
	{
		tag = INV_SEEKER;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SHIELD_PICKUP"))	
	{
		tag = ITM_SHIELD_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"INV_BACTA_CANISTER"))	
	{
		tag = INV_BACTA_CANISTER;
	}
	else if (!Q_stricmp(tokenStr,"ITM_DATAPAD_PICKUP"))	
	{
		tag = ITM_DATAPAD_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"INV_ELECTROBINOCULARS"))	
	{
		tag = INV_ELECTROBINOCULARS;
	}
	else if (!Q_stricmp(tokenStr,"INV_SENTRY"))	
	{
		tag = INV_SENTRY;
	}
	else if (!Q_stricmp(tokenStr,"INV_LIGHTAMP_GOGGLES"))	
	{
		tag = INV_LIGHTAMP_GOGGLES;
	}
	else if (!Q_stricmp(tokenStr,"INV_GOODIE_KEY"))	
	{
		tag = INV_GOODIE_KEY;
	}
	else if (!Q_stricmp(tokenStr,"INV_SECURITY_KEY"))	
	{
		tag = INV_SECURITY_KEY;
	}
	else if (!Q_stricmp(tokenStr,"ITM_MEDPAK_PICKUP"))	
	{
		tag = ITM_MEDPAK_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SHIELD_SM_PICKUP"))
	{
		tag = ITM_SHIELD_SM_PICKUP;
	}
	else if (!Q_stricmp(tokenStr,"ITM_SHIELD_LRG_PICKUP"))	
	{
		tag = ITM_SHIELD_LRG_PICKUP;
	}
	else
	{
		tag = WP_BRYAR_PISTOL;
		//This error was slipping through too much, causing runaway exceptions and shutting down, so now it's a real error when not in Final
#ifndef FINAL_BUILD
		G_Error("ERROR: bad tagname in external item data '%s'\n", tokenStr);
#else
		gi.Printf("WARNING: bad tagname in external item data '%s'\n", tokenStr);
#endif
	}
	bg_itemlist[itemParms.itemNum].giTag = tag;

}

static void IT_Type(const char **holdBuf)
{
	int type;
	const char	*tokenStr;

	if (COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	if (!Q_stricmp(tokenStr,"IT_BAD"))	
		type = IT_BAD;
	else if (!Q_stricmp(tokenStr,"IT_WEAPON"))	
		type = IT_WEAPON;
	else if (!Q_stricmp(tokenStr,"IT_AMMO"))	
		type = IT_AMMO;
	else if (!Q_stricmp(tokenStr,"IT_ARMOR"))
		type = IT_ARMOR;
	else if (!Q_stricmp(tokenStr,"IT_HEALTH"))
		type = IT_HEALTH;
	else if (!Q_stricmp(tokenStr,"IT_HOLDABLE"))	
		type = IT_HOLDABLE;
	else if (!Q_stricmp(tokenStr,"IT_BATTERY"))	
		type = IT_BATTERY;
	else if (!Q_stricmp(tokenStr,"IT_HOLOCRON"))	
		type = IT_HOLOCRON;
	else
	{
		type = IT_BAD;
		gi.Printf("WARNING: bad itemname in external item data '%s'\n", tokenStr);
	}

	bg_itemlist[itemParms.itemNum].giType = (itemType_t) type;

}

static void IT_Icon(const char **holdBuf)
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
		gi.Printf("WARNING: icon too long in external ITEMS.DAT '%s'\n", tokenStr);
	}

	bg_itemlist[itemParms.itemNum].icon = G_NewString(tokenStr);
}

static void IT_Count(const char **holdBuf)
{
	int		tokenInt;

	if ( COM_ParseInt(holdBuf,&tokenInt)) 
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000 )) // FIXME :What are the right values?
	{
		gi.Printf("WARNING: bad Count in external item data '%d'\n", tokenInt);
		return;
	}
	bg_itemlist[itemParms.itemNum].quantity = tokenInt;

}


static void IT_Min(const char **holdBuf)
{
	int		tokenInt;
	int		i;

	for (i=0;i<3;++i)
	{
		if ( COM_ParseInt(holdBuf,&tokenInt)) 
		{
			SkipRestOfLine(holdBuf);
			return;
		}

		bg_itemlist[itemParms.itemNum].mins[i] = tokenInt;
	}

}

static void IT_Max(const char **holdBuf)
{
	int		tokenInt;
	int		i;

	for (i=0;i<3;++i)
	{
		if ( COM_ParseInt(holdBuf,&tokenInt)) 
		{
			SkipRestOfLine(holdBuf);
			return;
		}

		bg_itemlist[itemParms.itemNum].maxs[i] = tokenInt;
	}

}

static void IT_PickupSound(const char **holdBuf)
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
		gi.Printf("WARNING: Pickup Sound too long in external ITEMS.DAT '%s'\n", tokenStr);
	}

	bg_itemlist[itemParms.itemNum].pickup_sound = G_NewString(tokenStr);
}

static void IT_ParseWeaponParms(const char **holdBuf)
{
	const char	*token;
	int		i;


	while (holdBuf)
	{
		token = COM_ParseExt( holdBuf, qtrue );

		if (!Q_stricmp( token, "}" ))	// End of data for this weapon
			break;

		// Loop through possible parameters
		for (i=0;i<IT_PARM_MAX;++i)
		{
			if (!Q_stricmp(token,ItemParms[i].parmName))	
			{
				ItemParms[i].func(holdBuf);
				break;
			}
		}

		if (i < IT_PARM_MAX)	// Find parameter???
		{
			continue;
		}

		Com_Printf("^3WARNING: bad parameter in external item data '%s'\n", token);
		SkipRestOfLine(holdBuf);
	}
}


static void IT_ParseParms(const char *buffer)
{
	const char	*holdBuf;
	const char	*token;

//	bg_numItems = 0;
	holdBuf = buffer;
	COM_BeginParseSession();

	while ( holdBuf ) 
	{
		token = COM_ParseExt( &holdBuf, qtrue );

		if ( !Q_stricmp( token, "{" ) ) 
		{
			IT_ParseWeaponParms(&holdBuf);
		}
		 
	}

	COM_EndParseSession(  );

//	--bg_numItems;

}


void IT_LoadItemParms (void)
{
	char *buffer;
	int len;

	len = gi.FS_ReadFile("ext_data/items.dat",(void **) &buffer);

	IT_ParseParms(buffer);

	gi.FS_FreeFile( buffer );	//let go of the buffer
}
