/*
This file is part of OpenJK.

    OpenJK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    OpenJK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenJK.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright 2013 OpenJK

#include "g_headers.h"

#include "g_local.h"
#include "b_local.h"
#include "wp_saber.h"
#include "w_local.h"


//---------------
//	Bryar Pistol
//---------------

//---------------------------------------------------------
void OLDFUNCTION_WP_FireBryarPistol( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = !alt_fire ? weaponData[WP_BLASTER_PISTOL].damage : weaponData[WP_BLASTER_PISTOL].altDamage;

	// DMG FUMO
	if ( ent && ent->client && ent->client->NPC_class == CLASS_BESPIN_COP )
	{
		damage *= 1.5;
	}
	// DMG VUOTO
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_SHADOWTROOPER )
	{
		damage *= 2; // Elementali del VUOTO fanno 50% di danno in più!
	}
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
		|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
	{//force sight 2+ gives perfect aim
		//FIXME: maybe force sight level 3 autoaims some?
		if ( ent->NPC && ent->NPC->currentAim < 5 )
		{
			vec3_t	angs;

			vectoangles( forwardVec, angs );

			if ( ent->client->NPC_class == CLASS_SABOTEUR || ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_BESPIN_COP )
			{//*sigh*, hack to make impworkers less accurate without affecteing imperial officer accuracy
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
			}
			else
			{
				angs[PITCH] += ( crandom() * ((5-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * ((5-ent->NPC->currentAim)*0.25f) );
			}

			AngleVectors( angs, forwardVec, NULL, NULL );
		}
	}

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t	*missile = CreateMissile( start, forwardVec, BRYAR_PISTOL_VEL, 10000, ent, alt_fire );

	missile->classname = "bryar_proj";
	if ( ent->s.weapon == WP_BLASTER_PISTOL )
	{//*SIGH*... I hate our weapon system...
		missile->s.weapon = ent->s.weapon;
	}

	if ( alt_fire )
	{
		int count = ( level.time - ent->client->ps.weaponChargeTime ) / BRYAR_CHARGE_UNIT;

		if ( count < 1 )
		{
			count = 1;
		}
		else if ( count > 5 )
		{
			count = 5;
		}

		damage *= count;
		missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_NO_KNOCKBACK;
	//missile->methodOfDeath = MOD_DARK;
if ( alt_fire )
{

	if ( ent && ent->client )
	{		
			switch ( ent->client->NPC_class )
			{	
				// ANIMALI E PIANTE 
				case CLASS_IVY:
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_SHARK: // flying creatures
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Salt Fired" );
					G_PlayEffect( G_EffectIndex( "salt/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Fired" );
					G_PlayEffect( G_EffectIndex( "dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
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
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Knife Fired" );
					G_PlayEffect( G_EffectIndex( "knife/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
				}
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Glyph Knife Fired" );
					G_PlayEffect( G_EffectIndex( "knife2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
					missile->splashMethodOfDeath = MOD_HOLY;
					missile->splashDamage = 10; 
				}
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Fired" );
					G_PlayEffect( G_EffectIndex( "dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Madness Fired" );
					G_PlayEffect( G_EffectIndex( "madness/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "smoker2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashMethodOfDeath = MOD_SMOKE;
					missile->splashDamage = 15; 
				}
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Knife Fired" );
					G_PlayEffect( G_EffectIndex( "knife2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
					missile->splashMethodOfDeath = MOD_HOLY;
					missile->splashDamage = 15; 
				}
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
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
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_WATERREAVER: // light reaver class 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Salt Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Salt/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Ash Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Ash/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
				}
				break;
				case CLASS_RAZIEL:// life reaver class 
				{
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
					damage *= 1.25;
				}
				break;
				case CLASS_RAZIEL2:// life reaver class 
				{
					// Dmg on sarafan.
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_RAZIEL3:// life reaver class 
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "smoke/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
			
				}
				break;
				// Boss del Fumo, Respen con la Mieti di Fumo
					// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "void/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
			
				}
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_REBORN:			// VAMPIRES 
				case CLASS_VAMPIRE: // act like reborns. alliance standard vampire classes  
				case CLASS_VAMPIRE_DARK: // vae
				case CLASS_VAMPIRE_BLOOD: // air vampire class - acrobatic bleeding vampire
		   		case CLASS_VAMPIRE_WATER: // Shaar MNaik evoluted water vampire class with water resistance. 
				case CLASS_VAMPIRE_ELF: // vampira elfa 
				case CLASS_VAMPIRE_ELF2: // vampire elf blue armor 
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
	     		{ // Stun dei giovani vampiri NON infligge danno! 
					// Dmg on sarafan.
					missile->damage *= 0;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				{ 
					missile->damage *= 0.25;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// Old Vampires and guardians deals more damage. 
				case CLASS_NECROMANCER: // Vampire Necromancer
				case CLASS_VAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_VAMPIRE_NECRO2:// Deathmask with full powers 
				case CLASS_KYLE:	// VAMPIRE BOSS CLASS (KAIN)
				case CLASS_KAIN:// Kain with full powers
				case CLASS_VAMPIRE_ELF3: // vampire elf blue armor rainbow lion class - dimension guardian 
				// Conflict Guardians 
				case CLASS_VAMPIRE_FROST: // for Kainh conflict guardian with frost powers. 
				case CLASS_VAMPIRE_FEAR2: // classe phobos knight of fear infernale 
				{
					missile->damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				{
					missile->damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				{
					missile->damage *= 0.75;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// WOLF, BAT AND MIST FORM DO NOT SHOOT MISSILE. 
				case CLASS_WOLFFORM: // for vampires mutated into wolves
				case CLASS_BATFORM: // for vampires mutated into bats 
				case CLASS_WOLFKAIN2:// Kain BO2
				case CLASS_BATKAIN2: // Kain bo2 bat form 
				case CLASS_BATVAMPIRE_WATER: // Shaar Naik Bat form
				case CLASS_BATVAMPIRE_ELF: // vampire elf bat form 
				case CLASS_BATVAMPIRE_ELF2: // vampireelf khyroptera form 
				case CLASS_WOLFVAMPIRE_ELF: // evoluted elf vampire class with earth connection. pretty strong. 
				case CLASS_LIONVAMPIRE_ELF: // vampireelf blue armor 
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
				case CLASS_BATVAMPIRE_ELF3: // vampireelf khyroptera form full power. 
				case CLASS_LIONVAMPIRE_ELF2: // vampireelf blue armor lion vampireelf with rainbows - Dimensione Guardian
				case CLASS_WOLFVAMPIRE_FEAR2: // classe phobos knight of fear infernale, Conflict Guardian
				case CLASS_BATVAMPIRE_FEAR2: // Phobos demonic bat form 
				case CLASS_BATVAMPIRE_FROST: // Kainh bat form 
				// MIST FORM // - NON ATTACCANO
				case CLASS_MISTFORM: // for vampires mutated into fog 
				case CLASS_MISTVAMPIRE_DARK: // Mist vae 
				case CLASS_MISTKAIN2:// Mist Kain BO2 
				case CLASS_MISTVAMPIRE_WATER:// shaar naik mist form 
				case CLASS_MISTVAMPIRE_BLOOD: // Mist osil
				case CLASS_MISTVAMPIRE_ELF:// vampireelf basic mist form
				case CLASS_MISTVAMPIRE_ELF2: // vampireelf Iridu mist form
				case CLASS_HELLMIST: // samah'el mist form 
		 		case CLASS_MISTVAMPIRE_SARAFAN:  // joachim 
				case CLASS_MISTVAMPIRE_FEAR: // mist phobos 
		 		case CLASS_MISTNECROMANCER: // Mist soul 
				case CLASS_MISTVAMPIRE_QUEEN: // Mist sheer 
		  		case CLASS_MISTKAIN:// Mist default kain 
				case CLASS_MISTVORADOR: // Mist vorador 
				case CLASS_MISTJANOS: // Mist janos audron 
				case CLASS_MISTKAIN3: // Mist Kain prophecy
				case CLASS_MISTVAMPIRE_ELF3:// vampireelf paladine mist form
				case CLASS_MISTVAMPIRE_FEAR2:// Mist phobos fear knight 
				{
					return; 
				}
				break;
				// Altri Psichici
				case CLASS_MENTALIST: 
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "silence2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
				}
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoke Fired" );
					G_PlayEffect( G_EffectIndex( "smoke2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
				}
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Ash Fired" );
					G_PlayEffect( G_EffectIndex( "ash2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
				}
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "void2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
			
				}
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Salt Fired" );
					G_PlayEffect( G_EffectIndex( "salt2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dust2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "Mind/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				{
				missile->damage *= 3;
				gi.Printf( S_COLOR_GREEN, "NUll Fired" );
				G_PlayEffect( G_EffectIndex( "Dimension/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_DIMENSION;
				}
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				{
				missile->damage *= 3;
				gi.Printf( S_COLOR_GREEN, "Knife Fired" );
				G_PlayEffect( G_EffectIndex( "Conflict/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_CONFLICT;	
				}
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Nature Fired" );
					G_PlayEffect( G_EffectIndex( "Nature/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NATURE;
				}
				break;
				case CLASS_ARIEL:
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Balance Fired" );
					G_PlayEffect( G_EffectIndex( "Balance/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_BALANCE;
				}
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Chaos Fired" );
					G_PlayEffect( G_EffectIndex( "Chaos/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_CHAOS;
				}
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Time Fired" );
					G_PlayEffect( G_EffectIndex( "time/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_TIME;
				}
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "energy Fired" );
					G_PlayEffect( G_EffectIndex( "energy/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ENERGY;
				}
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 20; 
					missile->splashMethodOfDeath = MOD_SMOKE;

			}
			break;
        	case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
			{
				missile->damage *= 3;
				gi.Printf( S_COLOR_GREEN, "Necro Fired" );
				G_PlayEffect( G_EffectIndex( "Necro/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_NECRO;
			}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_THRALL: 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Stun Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
			
				}
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				{
					// Dmg on sarafan.
					damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Silence/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
			
				}
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
		        {
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "void/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
			
				}
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
				{
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
			
				}
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				{
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "ash Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "ash/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
			
				}
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				{
					// Dmg on sarafan.
					damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Stun Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
			
				}
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_HELLGOLEM: // golem infernale
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_DECAL:
				{
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
			
				}
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		{
					// Dmg on sarafan.
					damage *= 2.5;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
			
				}
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   {
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
			
				}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoker Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_SMOKE;
				}
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				{
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Darkness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "light Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "madness/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "silence3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
				}
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
				}
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Void3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
				}
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Salt Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Salt3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Infernal Knives Fired" );
					G_PlayEffect( G_EffectIndex( "hellknife/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
					missile->splashMethodOfDeath = MOD_HELL; 
				}
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				{
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Void Fired" );
					G_PlayEffect( G_EffectIndex( "void3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
				}
				break;	

				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				{
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "absorb3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				{
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	{
					damage *= 2.5;
					gi.Printf( S_COLOR_GREEN, "Madness Fired" );
					G_PlayEffect( G_EffectIndex( "madness2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
			}
			break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;	
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "silence4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
				}
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dust4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_SMOKE;
			
				}
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Void4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_VOID;
			
				}
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Salt Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Salt4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_SALT;
			
				}
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "knife Fired" );
					G_PlayEffect( G_EffectIndex( "hellknife/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_METAL;
			
				}
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				{
					// Dmg on sarafan.
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				{
					// Dmg on sarafan.
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Null Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Null/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NULL;
				}
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				{
				missile->damage *= 1.5;
				G_PlayEffect( G_EffectIndex( "Stagnation/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_STAGNATION;	
				}
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				{
				missile->damage *= 1.5;
				G_PlayEffect( G_EffectIndex( "Corruption/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_CORRUPTION;	
				}
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				{
				missile->damage *= 1.5;
				G_PlayEffect( G_EffectIndex( "entropy/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_ENTROPY;	
				}
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				{
					missile->damage *= 1.5;
						G_PlayEffect( G_EffectIndex( "Timeless/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_TIMELESS;
				}
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				{
					missile->damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Antimatter Fired" );
					G_PlayEffect( G_EffectIndex( "antimatter/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ANTIMATTER;
				}
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				{
					missile->damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 15; 
					missile->splashMethodOfDeath = MOD_SMOKE;
				}
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Null Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Null2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NULL;
				}
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				{
				missile->damage *= 2;
				gi.Printf( S_COLOR_GREEN, "Stagnation Fired" );
				G_PlayEffect( G_EffectIndex( "Stagnation2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_STAGNATION;
				}
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				{
				missile->damage *= 2;
				gi.Printf( S_COLOR_GREEN, "Corruption Fired" );
				G_PlayEffect( G_EffectIndex( "Corruption2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_CORRUPTION;
				}
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
	           {
				missile->damage *= 2;
				gi.Printf( S_COLOR_GREEN, "Entropy Fired" );
				G_PlayEffect( G_EffectIndex( "Entropy2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_ENTROPY;
				}
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
	            {
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Timeless Fired" );
					G_PlayEffect( G_EffectIndex( "Timeless2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
	            {
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Antimatter Fired" );
					G_PlayEffect( G_EffectIndex( "Antimatter2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_LIGHT;
				}
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
		        {
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 20; 
					missile->splashMethodOfDeath = MOD_SMOKE;
				}
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				{
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				{
					// Dmg on sarafan.
					damage *= 5;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_FIGHTER:
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "stunner Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "bryar0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_BRYAR_ALT;
				}
				break;
			}
		
    }


	/*
	// THE STUN 
			if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_KYLE ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_TUSKEN ||
				ent->client->NPC_class == CLASS_MELC || 
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_FLIER2 ||
				ent->client->NPC_class == CLASS_SWAMPTROOPER ||
				ent->client->NPC_class == CLASS_FISH ||
				ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_VAMPIRE ||   
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST ||
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 ||  
				ent->client->NPC_class == CLASS_KAIN3 ||
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS ||  
				ent->client->NPC_class == CLASS_NECROMANCER ||  
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||  
				ent->client->NPC_class == CLASS_VAMPIRE_DARK ||  
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO ||   
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 || 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN ||  
				ent->client->NPC_class == CLASS_VAMPIRE_ELF ||  
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||  
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER ||  
				ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE ||
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		{
			gi.Printf( S_COLOR_GREEN "Stun Fired" );
			damage *= 0.5;
			missile->methodOfDeath = MOD_MIND;
			G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE SMOG 
	else if (	ent->client->NPC_class == CLASS_BESPIN_COP ||
				ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class == CLASS_TANKREAVER || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class == CLASS_REELO ||
				ent->client->NPC_class == CLASS_CLAW ||
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON ||
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_FIREREAVER ||
				ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			damage *= 1.5;
			missile->methodOfDeath = MOD_SMOKE;
			G_PlayEffect( G_EffectIndex( "bryar2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE VOID 
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_PROBE || 
				ent->client->NPC_class == CLASS_POLTER )
	{
		{
			damage *= 2;
			missile->methodOfDeath = MOD_VOID;
			G_PlayEffect( G_EffectIndex( "bryar3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE KNIFE
	else if ( ent->client->NPC_class == CLASS_PRISONER ||
				ent->client->NPC_class == CLASS_SCOUT || 
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_ARCHER || 
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 ||
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_LIZARD ||
				ent->client->NPC_class == CLASS_JAN )
	{
		{
			damage *= 0.75;
			missile->methodOfDeath = MOD_METAL;
			G_PlayEffect( G_EffectIndex( "knife/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROTIC
	else if ( ent->client->NPC_class == CLASS_GLIDER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_ARIEL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO || 
				ent->client->NPC_class == CLASS_MURJJ ||
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_DEMON_BLADE ||
				ent->client->NPC_class == CLASS_DEMON_POISON ||
				ent->client->NPC_class == CLASS_DEMON_ELECTRO ||
				ent->client->NPC_class == CLASS_ATST ||
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_SAND_CREATURE ||
				ent->client->NPC_class == CLASS_MOUSE ||
				ent->client->NPC_class == CLASS_PROPHET ||
				ent->client->NPC_class == CLASS_CENOBITA )
	{
		{
			damage *= 2.5;
			missile->methodOfDeath = MOD_NECRO;
			G_PlayEffect( G_EffectIndex( "absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE ENTROPY 
	else if ( ent->client->NPC_class == CLASS_ALCHEMIST ||
				ent->client->NPC_class == CLASS_DESANN ||
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_GOLEM_ICE || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		{
			damage *= 3;
			missile->methodOfDeath = MOD_DARK;
			G_PlayEffect( G_EffectIndex( "entropy/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT 
	else if (	ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_WIZARD ||  
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD ||
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_GALAKMECH ||
				ent->client->NPC_class == CLASS_RAZIEL || 
				ent->client->NPC_class == CLASS_NECROREAVER || 
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_LIGHTREAVER || 
				ent->client->NPC_class == CLASS_DRUIDREAVER || 
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_SPIDERREAVER || 
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_EARTHREAVER || 				
				ent->client->NPC_class == CLASS_BARTENDER ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_IMPWORKER || 				
				ent->client->NPC_class == CLASS_INTERROGATOR ||
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 ||
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_SWAMP || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_WATER || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_HOLY ||  
				ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_WATER || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_METAL || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY )
			{
		{
			gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
	
			damage *= 0.5;
			missile->methodOfDeath = MOD_MIND;
			G_PlayEffect( G_EffectIndex( "bryar/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			missile->methodOfDeath = MOD_BRYAR_ALT;
			G_PlayEffect( G_EffectIndex( "bryar0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}*/
}
else 
{
	if ( ent && ent->client )
	{		
			switch ( ent->client->NPC_class )
			{	
				// ANIMALI E PIANTE 
				case CLASS_IVY:
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_SHARK: // flying creatures
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Salt Fired" );
					G_PlayEffect( G_EffectIndex( "salt/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Fired" );
					G_PlayEffect( G_EffectIndex( "dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
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
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Knife Fired" );
					G_PlayEffect( G_EffectIndex( "knife/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
				}
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Glyph Knife Fired" );
					G_PlayEffect( G_EffectIndex( "knife2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
					missile->splashMethodOfDeath = MOD_HOLY;
					missile->splashDamage = 10; 
				}
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Fired" );
					G_PlayEffect( G_EffectIndex( "dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Madness Fired" );
					G_PlayEffect( G_EffectIndex( "madness/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "smoker2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashMethodOfDeath = MOD_SMOKE;
					missile->splashDamage = 15; 
				}
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Knife Fired" );
					G_PlayEffect( G_EffectIndex( "knife2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
					missile->splashMethodOfDeath = MOD_HOLY;
					missile->splashDamage = 15; 
				}
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
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
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_WATERREAVER: // light reaver class 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Salt Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Salt/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Ash Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Ash/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
				}
				break;
				case CLASS_RAZIEL:// life reaver class 
				{
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
					damage *= 1.25;
				}
				break;
				case CLASS_RAZIEL2:// life reaver class 
				{
					// Dmg on sarafan.
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_RAZIEL3:// life reaver class 
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "smoke/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
			
				}
				break;
					// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "void/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
			
				}
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_REBORN:			// VAMPIRES 
				case CLASS_VAMPIRE: // act like reborns. alliance standard vampire classes  
				case CLASS_VAMPIRE_DARK: // vae
				case CLASS_VAMPIRE_BLOOD: // air vampire class - acrobatic bleeding vampire
		   		case CLASS_VAMPIRE_WATER: // Shaar MNaik evoluted water vampire class with water resistance. 
				case CLASS_VAMPIRE_ELF: // vampira elfa 
				case CLASS_VAMPIRE_ELF2: // vampireelf blue armor 
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
	     		{ // Stun dei giovani vampiri NON infligge danno! 
					// Dmg on sarafan.
					missile->damage *= 0;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				{ 
					missile->damage *= 0.25;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// Old Vampires and guardians deals more damage. 
				case CLASS_NECROMANCER: // Vampire Necromancer
				case CLASS_VAMPIRE_QUEEN: // for Sheer and Deathmask. Fight like tavion. vampire necro. 
				case CLASS_VAMPIRE_NECRO2:// Deathmask with full powers 
				case CLASS_KYLE:	// VAMPIRE BOSS CLASS (KAIN)
				case CLASS_KAIN:// Kain with full powers
				case CLASS_VAMPIRE_ELF3: // vampireelf blue armor rainbow lion class - dimension guardian 
				// Conflict Guardians 
				case CLASS_VAMPIRE_FROST: // for Kainh conflict guardian with frost powers. 
				case CLASS_VAMPIRE_FEAR2: // classe phobos knight of fear infernale 
				{
					missile->damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				{
					missile->damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				{
					missile->damage *= 0.75;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// WOLF, BAT AND MIST FORM DO NOT SHOOT MISSILE. 
				case CLASS_WOLFFORM: // for vampires mutated into wolves
				case CLASS_BATFORM: // for vampires mutated into bats 
				case CLASS_WOLFKAIN2:// Kain BO2
				case CLASS_BATKAIN2: // Kain bo2 bat form 
				case CLASS_BATVAMPIRE_WATER: // Shaar Naik Bat form
				case CLASS_BATVAMPIRE_ELF: // vampireelf bat form 
				case CLASS_BATVAMPIRE_ELF2: // vampireelf khyroptera form 
				case CLASS_WOLFVAMPIRE_ELF: // evoluted elf vampire class with earth connection. pretty strong. 
				case CLASS_LIONVAMPIRE_ELF: // vampireelf blue armor 
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
				case CLASS_BATVAMPIRE_ELF3: // vampireelf khyroptera form full power. 
				case CLASS_LIONVAMPIRE_ELF2: // vampireelf blue armor lion vampireelf with rainbows - Dimensione Guardian
				case CLASS_WOLFVAMPIRE_FEAR2: // classe phobos knight of fear infernale, Conflict Guardian
				case CLASS_BATVAMPIRE_FEAR2: // Phobos demonic bat form 
				case CLASS_BATVAMPIRE_FROST: // Kainh bat form 
				// MIST FORM // - NON ATTACCANO
				case CLASS_MISTFORM: // for vampires mutated into fog 
				case CLASS_MISTVAMPIRE_DARK: // Mist vae 
				case CLASS_MISTKAIN2:// Mist Kain BO2 
				case CLASS_MISTVAMPIRE_WATER:// shaar naik mist form 
				case CLASS_MISTVAMPIRE_BLOOD: // Mist osil
				case CLASS_MISTVAMPIRE_ELF:// vampireelf basic mist form
				case CLASS_MISTVAMPIRE_ELF2: // vampireelf Iridu mist form
				case CLASS_HELLMIST: // samah'el mist form 
		 		case CLASS_MISTVAMPIRE_SARAFAN:  // joachim 
				case CLASS_MISTVAMPIRE_FEAR: // mist phobos 
		 		case CLASS_MISTNECROMANCER: // Mist soul 
				case CLASS_MISTVAMPIRE_QUEEN: // Mist sheer 
		  		case CLASS_MISTKAIN:// Mist default kain 
				case CLASS_MISTVORADOR: // Mist vorador 
				case CLASS_MISTJANOS: // Mist janos audron 
				case CLASS_MISTKAIN3: // Mist Kain prophecy
				case CLASS_MISTVAMPIRE_ELF3:// vampireelf paladine mist form
				case CLASS_MISTVAMPIRE_FEAR2:// Mist phobos fear knight 
				{
					return; 
				}
				break;
				// Altri Psichici
				case CLASS_MENTALIST: 
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "silence2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
				}
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoke Fired" );
					G_PlayEffect( G_EffectIndex( "smoke2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
				}
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Ash Fired" );
					G_PlayEffect( G_EffectIndex( "ash2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
				}
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "void2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
			
				}
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Salt Fired" );
					G_PlayEffect( G_EffectIndex( "salt2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dust2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				{
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "stun3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Stun Fired" );
					G_PlayEffect( G_EffectIndex( "Mind/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
				}
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				{
				missile->damage *= 3;
				gi.Printf( S_COLOR_GREEN, "NUll Fired" );
				G_PlayEffect( G_EffectIndex( "Dimension/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_DIMENSION;
				}
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				{
				missile->damage *= 3;
				gi.Printf( S_COLOR_GREEN, "Knife Fired" );
				G_PlayEffect( G_EffectIndex( "Conflict/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_CONFLICT;	
				}
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Nature Fired" );
					G_PlayEffect( G_EffectIndex( "Nature/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NATURE;
				}
				break;
				case CLASS_ARIEL:
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Balance Fired" );
					G_PlayEffect( G_EffectIndex( "Balance/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_BALANCE;
				}
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Absorb Fired" );
					G_PlayEffect( G_EffectIndex( "absorb3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Chaos Fired" );
					G_PlayEffect( G_EffectIndex( "Chaos/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_CHAOS;
				}
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Time Fired" );
					G_PlayEffect( G_EffectIndex( "time/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_TIME;
				}
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "energy Fired" );
					G_PlayEffect( G_EffectIndex( "energy/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ENERGY;
				}
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				{
					missile->damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 20; 
					missile->splashMethodOfDeath = MOD_SMOKE;

			}
			break;
        	case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
			{
				missile->damage *= 3;
				gi.Printf( S_COLOR_GREEN, "Necro Fired" );
				G_PlayEffect( G_EffectIndex( "Necro/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_NECRO;
			}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_THRALL: 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Stun Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "stun/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
			
				}
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				{
					// Dmg on sarafan.
					damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Silence/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
			
				}
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
		        {
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "void/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
			
				}
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
				{
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
			
				}
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				{
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "ash Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "ash/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ASH;
			
				}
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				{
					// Dmg on sarafan.
					damage *= 0.5;
					gi.Printf( S_COLOR_GREEN, "Stun Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "stun/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MIND;
			
				}
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_HELLGOLEM: // golem infernale
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_DECAL:
				{
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dark/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
			
				}
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
			
				}
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		{
					// Dmg on sarafan.
					damage *= 2.5;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
			
				}
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   {
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
			
				}
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
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Smoker Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_SMOKE;
				}
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				{
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Darkness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "light Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "madness/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "silence3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
				}
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "dust3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
				}
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Void3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
				}
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Salt Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Salt3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SALT;
				}
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	{
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Infernal Knives Fired" );
					G_PlayEffect( G_EffectIndex( "hellknife/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_METAL;
					missile->splashMethodOfDeath = MOD_HELL; 
				}
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				{
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Void Fired" );
					G_PlayEffect( G_EffectIndex( "void3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
				}
				break;	

				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				{
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "absorb3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				{
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	{
					damage *= 2.5;
					gi.Printf( S_COLOR_GREEN, "Madness Fired" );
					G_PlayEffect( G_EffectIndex( "madness2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
			}
			break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;	
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Silence Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "silence4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SILENCE;
				}
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dark4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_DARK;
				}
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Dust4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_DUST;
			
				}
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "smoke Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Smoke4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_SMOKE;
			
				}
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Void4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_VOID;
			
				}
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Salt Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Salt4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_SALT;
			
				}
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "knife Fired" );
					G_PlayEffect( G_EffectIndex( "hellknife/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 10; 
					missile->splashMethodOfDeath = MOD_METAL;
			
				}
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				{
					// Dmg on sarafan.
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				{
					// Dmg on sarafan.
					damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Null Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Null/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NULL;
				}
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				{
				missile->damage *= 1.5;
				G_PlayEffect( G_EffectIndex( "Stagnation/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_STAGNATION;	
				}
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				{
				missile->damage *= 1.5;
				G_PlayEffect( G_EffectIndex( "Corruption/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_CORRUPTION;	
				}
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				{
				missile->damage *= 1.5;
				G_PlayEffect( G_EffectIndex( "entropy/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_ENTROPY;	
				}
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				{
					missile->damage *= 1.5;
						G_PlayEffect( G_EffectIndex( "Timeless/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_TIMELESS;
				}
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				{
					missile->damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Antimatter Fired" );
					G_PlayEffect( G_EffectIndex( "antimatter/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_ANTIMATTER;
				}
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				{
					missile->damage *= 1.5;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 15; 
					missile->splashMethodOfDeath = MOD_SMOKE;
				}
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				{
					// Dmg on sarafan.
					damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Null Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Null2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NULL;
				}
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				{
				missile->damage *= 2;
				gi.Printf( S_COLOR_GREEN, "Stagnation Fired" );
				G_PlayEffect( G_EffectIndex( "Stagnation2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_STAGNATION;
				}
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				{
				missile->damage *= 2;
				gi.Printf( S_COLOR_GREEN, "Corruption Fired" );
				G_PlayEffect( G_EffectIndex( "Corruption2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_CORRUPTION;
				}
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
	           {
				missile->damage *= 2;
				gi.Printf( S_COLOR_GREEN, "Entropy Fired" );
				G_PlayEffect( G_EffectIndex( "Entropy2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
				missile->methodOfDeath = MOD_ENTROPY;
				}
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
	            {
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Timeless Fired" );
					G_PlayEffect( G_EffectIndex( "Timeless2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
	            {
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Antimatter Fired" );
					G_PlayEffect( G_EffectIndex( "Antimatter2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_LIGHT;
				}
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
		        {
					missile->damage *= 2;
					gi.Printf( S_COLOR_GREEN, "Smoker Fired" );
					G_PlayEffect( G_EffectIndex( "Smoker2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_GLYPH;
					missile->splashDamage = 20; 
					missile->splashMethodOfDeath = MOD_SMOKE;
				}
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				{
					// Dmg on sarafan.
					damage *= 3;
					gi.Printf( S_COLOR_GREEN, "Madness Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Madness3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_MADNESS;
				}
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				{
					// Dmg on sarafan.
					damage *= 5;
					gi.Printf( S_COLOR_GREEN, "Absorb Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "Absorb4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_NECRO;
				}
				break;
				case CLASS_FIGHTER:
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "stunner Bolt Fired" );
					G_PlayEffect( G_EffectIndex( "bryar0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_BRYAR_ALT;
				}
				break;



			}
		
    }
	

				// Frecce d'ombra 
				/*case CLASS_SABOTEUR:		// SHADOW WARRIOR CLASS 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Arrow Fired" );
					G_PlayEffect( G_EffectIndex( "DarkArrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DARK;
				}
				break;*/
				/*
				// Frecce Fumogene
				case CLASS_BESPIN_COP: 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dark Arrow Fired" );
					G_PlayEffect( G_EffectIndex( "SmokeArrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_SMOKE;
				}
				break;
				// freccia di polvere 
				case CLASS_THRALL: // elite thrall fighting class 
				{
					// Dmg on sarafan.
					damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Dust Arrow Fired" );
					G_PlayEffect( G_EffectIndex( "dustArrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_DUST;
			
				}*/
				
				/*
				// Frecce del Vuoto
				case CLASS_SHADOWTROOPER: 
				{
					missile->damage *= 1;
					gi.Printf( S_COLOR_GREEN, "Void Arrow Fired" );
					G_PlayEffect( G_EffectIndex( "VoidArrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
					missile->methodOfDeath = MOD_VOID;
				}
				break;*/
				
}

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 0;

	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}

extern void FX_AddLine( vec3_t start, vec3_t end, float stScale, 
							float width, float dwidth, 
							float sAlpha, float eAlpha, 
							int life, qhandle_t shader, int flags );
//---------------
//	Bryar Pistol
//---------------

extern void Jedi_Decloak( gentity_t *self );

//---------------------------------------------------------
// MELEE WEAPONS LOGIC 

void WP_FireMelee( gentity_t *ent, qboolean alt_fire )
{	
	// THERE IS SOMETHING THAT CRASH HERE. I NOT KNOW WHAT! 
	// CRASH IF YOU USE OTHER WEAPONS THAT HAVE NOT THE MELEE BALLISTIC 
	// CRASH WHEN HITTEN ENEMIES! 

	gentity_t	*tr_ent;
	trace_t		tr;
	vec3_t		mins, maxs, end;
	gunData_t	*gun = NULL; 

if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}

// DAMAGE, EFX ETC ETC ETC... - ANIMATIONS ARE SETTED INTO BG_PMOVE.CPP 
	int			damage = ent->s.number ? (g_spskill->integer*2)+1 : 3;
	float		range = ent->s.number ? 64 : 32;
	VectorMA( muzzle, range, forwardVec, end );
	VectorSet( maxs, 6, 6, 6 );
	VectorScale( maxs, -1, mins );
	gi.trace ( &tr, muzzle, mins, maxs, end, ent->s.number, MASK_SHOT, (EG2_Collision)0, 0 );
	int dflags = DAMAGE_NO_KNOCKBACK;
	if ( tr.entityNum >= ENTITYNUM_WORLD )
	{
		return;
	}
	tr_ent = &g_entities[tr.entityNum];
	if ( tr_ent && tr_ent->takedamage )
	{
		if ( alt_fire ) 
		{
			// set damage 
			if ( ent->s.number != 0 ) /// NPCS melees 
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy, dflags, ent->client->gs.gun->Gun_Alt_MOD );
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium, dflags, ent->client->gs.gun->Gun_Alt_MOD );
					}

				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard, dflags, ent->client->gs.gun->Gun_Alt_MOD );
					}
				}
				
				// add a flesh impact effect on victim 
				if ( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect ) 
				{
					G_PlayEffect( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect, tr.endpos, forwardVec );
				}
				// Electrocute 
				if ( ent->client->gs.gun->gunFlags2&GFL_ALT_ELECTROCUTE )
				{
					tr_ent->s.powerups |= ( 1 << PW_SHOCKED );
					tr_ent->client->ps.powerups[PW_SHOCKED] = level.time + ent->client->gs.gun->Gun_Alt_ElectrocuteDuration;
				}
				// REMOVAL BODY NON FUNZIONA
				// DISMEMBER NON FUNZIONA, 
				// add disgregate effect? 
				if ( ent->client->gs.gun->gunFlags2&GFL_ALT_DISGREGATE )
				{
					if (  tr_ent &&  tr_ent->client && tr_ent->health <= 0 )
						{
							tr_ent->s.powerups |= ( 1 << PW_DISRUPTION );
							tr_ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( ent->client->gs.gun->Gun_Alt_Disgregate_Effect )
							{
								if ( tr_ent->ghoul2.size() && tr_ent->headBolt != -1 )
								{//FIXME: what if already playing effect?
									G_PlayEffect( ent->client->gs.gun->Gun_Alt_Disgregate_Effect, tr_ent->playerModel, tr_ent->chestBolt, tr_ent->s.number, tr_ent->currentOrigin ); // same effect, more huge, for disintegration 
								}
							}
						}
				}
				// Dismemberment 
				//G_Sound( tr_ent, G_SoundIndex( va("sound/weapons/melee/punch%d", Q_irand(1, 4)) ) );
				if ( ent->NPC && (ent->NPC->aiFlags&NPCAI_HEAVY_MELEE) || ent->client->gs.gun->gunFlags2&GFL_ALT_DISMEMBER )
				{ //4x damage for heavy melee class
			
					dflags &= ~DAMAGE_NO_KNOCKBACK;
					dflags |= DAMAGE_DISMEMBER;
				}
			}
			// Brushes 
			else if ( tr_ent->svFlags & SVF_GLASS_BRUSH || ( tr_ent->svFlags & SVF_BBRUSH && tr_ent->material == 12 )) // material grate...we are breaking a grate!
			{
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, 999, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD ); // smash that puppy
				// add a wall impact effect on hit brushes 
				if ( ent->client->gs.gun->Gun_Alt_WallImpact_Effect ) 
				{
					G_PlayEffect( ent->client->gs.gun->Gun_Alt_WallImpact_Effect, tr.endpos, forwardVec );
				}
			}
			// Set Player Damage
			else  
			{	
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_AltDamage, dflags, ent->client->gs.gun->Gun_Alt_MOD );
				// add a flesh impact effect on victim 
				if ( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect ) 
				{
					G_PlayEffect( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect, tr.endpos, forwardVec );
				}
				// Electrocute 
				if ( ent->client->gs.gun->gunFlags2&GFL_ALT_ELECTROCUTE )
				{
					tr_ent->s.powerups |= ( 1 << PW_SHOCKED );
					tr_ent->client->ps.powerups[PW_SHOCKED] = level.time + ent->client->gs.gun->Gun_Alt_ElectrocuteDuration;
				}
				// add disgregate effect? 
				if ( ent->client->gs.gun->gunFlags2&GFL_ALT_DISGREGATE )
				{
					if (  tr_ent &&  tr_ent->client && tr_ent->health <= 0 )
						{
							tr_ent->s.powerups |= ( 1 << PW_DISRUPTION );
							tr_ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( ent->client->gs.gun->Gun_Alt_Disgregate_Effect )
							{
								if ( tr_ent->ghoul2.size() && tr_ent->headBolt != -1 )
								{//FIXME: what if already playing effect?
									G_PlayEffect( ent->client->gs.gun->Gun_Alt_Disgregate_Effect, tr_ent->playerModel, tr_ent->chestBolt, tr_ent->s.number, tr_ent->currentOrigin ); // same effect, more huge, for disintegration 
								}
							}
						}
				}
				// Dismemberment 
				//G_Sound( tr_ent, G_SoundIndex( va("sound/weapons/melee/punch%d", Q_irand(1, 4)) ) );
				if ( ent->client->gs.gun->gunFlags2&GFL_ALT_DISMEMBER )
				{ //4x damage for heavy melee class
			
					dflags &= ~DAMAGE_NO_KNOCKBACK;
					dflags |= DAMAGE_DISMEMBER;
				}
			}
		}
		else // MAIN FIRE 
		{
		// set damage 
		if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_NPC_Damage_Easy, dflags, ent->client->gs.gun->Gun_MOD );
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_NPC_Damage_Medium, dflags, ent->client->gs.gun->Gun_MOD );
					}

				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_NPC_Damage_Hard, dflags, ent->client->gs.gun->Gun_MOD );
					}
				}
				// add a flesh impact effect on victim 
				if ( ent->client->gs.gun->Gun_FleshImpact_Effect ) 
				{
					G_PlayEffect( ent->client->gs.gun->Gun_FleshImpact_Effect, tr.endpos, forwardVec );
				}
				// Electrocute 
				if ( ent->client->gs.gun->gunFlags2&GFL_ELECTROCUTE )
				{
					tr_ent->s.powerups |= ( 1 << PW_SHOCKED );
					tr_ent->client->ps.powerups[PW_SHOCKED] = level.time + ent->client->gs.gun->Gun_ElectrocuteDuration;
				}
				// add disgregate effect? 
				if ( ent->client->gs.gun->gunFlags2&GFL_DISGREGATE )
				{
					if (  tr_ent &&  tr_ent->client && tr_ent->health <= 0 )
						{
							tr_ent->s.powerups |= ( 1 << PW_DISRUPTION );
							tr_ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							G_FreeEntity( tr_ent );
							if ( ent->client->gs.gun->Gun_Disgregate_Effect )
							{
								if ( tr_ent->ghoul2.size() && tr_ent->headBolt != -1 )
								{//FIXME: what if already playing effect?
									G_PlayEffect( ent->client->gs.gun->Gun_Disgregate_Effect, tr_ent->playerModel, tr_ent->chestBolt, tr_ent->s.number, tr_ent->currentOrigin ); // same effect, more huge, for disintegration 
								}
							}
						}
				}
				// Dismemberment 
				//G_Sound( tr_ent, G_SoundIndex( va("sound/weapons/melee/punch%d", Q_irand(1, 4)) ) );
				if ( ent->client->gs.gun->gunFlags2&GFL_DISMEMBER )
				{ //4x damage for heavy melee class
					dflags &= ~DAMAGE_NO_KNOCKBACK;
					dflags |= DAMAGE_DISMEMBER;
				}
			}
			else if ( tr_ent->svFlags & SVF_GLASS_BRUSH || ( tr_ent->svFlags & SVF_BBRUSH && tr_ent->material == 12 )) // material grate...we are breaking a grate!
			{
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, 999, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_MOD ); // smash that puppy
				// add a wall impact effect on hit brushes 
				if ( ent->client->gs.gun->Gun_WallImpact_Effect ) 
				{
					G_PlayEffect( ent->client->gs.gun->Gun_WallImpact_Effect, tr.endpos, forwardVec );
				}
			}
			else // set playa damage 
			{	
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_AltDamage, dflags, ent->client->gs.gun->Gun_MOD );
				// add a flesh impact effect on victim 
				if ( ent->client->gs.gun->Gun_FleshImpact_Effect ) 
				{
					G_PlayEffect( ent->client->gs.gun->Gun_FleshImpact_Effect, tr.endpos, forwardVec );
				}
				// Electrocute 
				if ( ent->client->gs.gun->gunFlags2&GFL_ELECTROCUTE )
				{
					tr_ent->s.powerups |= ( 1 << PW_SHOCKED );
					tr_ent->client->ps.powerups[PW_SHOCKED] = level.time + ent->client->gs.gun->Gun_ElectrocuteDuration;
				}
				// add disgregate effect? 
				if ( ent->client->gs.gun->gunFlags2&GFL_DISGREGATE )
				{
					if (  tr_ent &&  tr_ent->client && tr_ent->health <= 0 )
						{
							tr_ent->s.powerups |= ( 1 << PW_DISRUPTION );
							tr_ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( ent->client->gs.gun->Gun_Disgregate_Effect )
							{
								if ( tr_ent->ghoul2.size() && tr_ent->headBolt != -1 )
								{//FIXME: what if already playing effect?
									G_PlayEffect( ent->client->gs.gun->Gun_Disgregate_Effect, tr_ent->playerModel, tr_ent->chestBolt, tr_ent->s.number, tr_ent->currentOrigin ); // same effect, more huge, for disintegration 
								}
							}
						}
				}
				// Dismemberment 
				//G_Sound( tr_ent, G_SoundIndex( va("sound/weapons/melee/punch%d", Q_irand(1, 4)) ) );
				if ( ent->client->gs.gun->gunFlags2&GFL_DISMEMBER )
				{ //4x damage for heavy melee class
			
					dflags &= ~DAMAGE_NO_KNOCKBACK;
					dflags |= DAMAGE_DISMEMBER;
				}
			}
		}
	}
}


void WP_FireBaton( gentity_t *ent, qboolean alt_fire )
{
	gentity_t	*tr_ent;
	trace_t		tr;
	vec3_t		mins, maxs, end, start;
	//G_Sound( ent, G_SoundIndex( "sound/weapons/baton/fire" )); USE FOR CUSTOMIZE SOUNDS! 
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );
	gunData_t	*gun = NULL; 
	// first off, set altmuzzle effect 
  if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}

  // set range of action of shot 
  if ( alt_fire ) 
	 { 
		 if ( ent->client->gs.gun->Gun_Alt_Range ) 
		 {
			VectorMA( start, ent->client->gs.gun->Gun_Alt_Range, forwardVec, end );
			VectorSet( maxs, 5, 5, 5 );
			VectorScale( maxs, -1, mins );
			gi.trace ( &tr, start, mins, maxs, end, ent->s.number, CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP, (EG2_Collision)0, 0 );
		}
	}
  else // main fire range
	 { 
		 if ( ent->client->gs.gun->Gun_Range ) 
		 {
			VectorMA( start, ent->client->gs.gun->Gun_Range, forwardVec, end );
			VectorSet( maxs, 5, 5, 5 );
			VectorScale( maxs, -1, mins );
			gi.trace ( &tr, start, mins, maxs, end, ent->s.number, CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP, (EG2_Collision)0, 0 );
		}
	}
  	if ( tr.entityNum >= ENTITYNUM_WORLD || tr.entityNum < 0 )
	{
		return;
	}
	tr_ent = &g_entities[tr.entityNum];
	
// customize effect of hitted enemies and other features 
if ( tr_ent && tr_ent->takedamage && tr_ent->client )
{
	if ( alt_fire )
	{
	
		if ( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect, tr.endpos, tr.plane.normal );  
		}
		// add electrocuting effect? 
		if ( ent->client->gs.gun->gunFlags2&GFL_ALT_ELECTROCUTE )
		{
			tr_ent->s.powerups |= ( 1 << PW_SHOCKED );
			tr_ent->client->ps.powerups[PW_SHOCKED] = level.time + ent->client->gs.gun->Gun_Alt_ElectrocuteDuration;
		}
		// add disgregate effect? 
		if ( ent->client->gs.gun->gunFlags2&GFL_ALT_DISGREGATE )
			{
				if (  tr_ent &&  tr_ent->client && tr_ent->health <= 0 )
					{
						tr_ent->s.powerups |= ( 1 << PW_DISRUPTION );
						tr_ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						if ( ent->client->gs.gun->Gun_Alt_Disgregate_Effect )
						{
							if ( tr_ent->ghoul2.size() && tr_ent->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( ent->client->gs.gun->Gun_Alt_Disgregate_Effect, tr_ent->playerModel, tr_ent->chestBolt, tr_ent->s.number, tr_ent->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
					}
				
			}
		// set damage 
		if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD );
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD );
					}

				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD );
					}
				}
             }
			else if ( tr_ent->svFlags & SVF_GLASS_BRUSH || ( tr_ent->svFlags & SVF_BBRUSH && tr_ent->material == 12 )) // material grate...we are breaking a grate!
			{
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, 999, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD ); // smash that puppy
				if ( ent->client->gs.gun->Gun_Alt_WallImpact_Effect )
				{
					//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
					G_PlayEffect( ent->client->gs.gun->Gun_Alt_WallImpact_Effect, tr.endpos, tr.plane.normal );  
				}
			}
			else // set playa damage 
			{	
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_AltDamage, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD );
			}
	}
	else // MAIN FIRE 
	{
	if ( ent->client->gs.gun->Gun_FleshImpact_Effect )
		{
			G_PlayEffect( ent->client->gs.gun->Gun_FleshImpact_Effect, tr.endpos, tr.plane.normal );  
		}
		// add electrocuting effect? 
		if ( ent->client->gs.gun->gunFlags2&GFL_ELECTROCUTE )
		{
			tr_ent->s.powerups |= ( 1 << PW_SHOCKED );
			tr_ent->client->ps.powerups[PW_SHOCKED] = level.time + ent->client->gs.gun->Gun_ElectrocuteDuration;
		}
		// add disgregate effect? 
		if ( ent->client->gs.gun->gunFlags2&GFL_DISGREGATE )
			{
				if (  tr_ent &&  tr_ent->client && tr_ent->health <= 0 )
					{
						tr_ent->s.powerups |= ( 1 << PW_DISRUPTION );
						tr_ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
						if ( ent->client->gs.gun->Gun_Disgregate_Effect )
						{
							if ( tr_ent->ghoul2.size() && tr_ent->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( ent->client->gs.gun->Gun_Disgregate_Effect, tr_ent->playerModel, tr_ent->chestBolt, tr_ent->s.number, tr_ent->currentOrigin ); // same effect, more huge, for disintegration 
							}
						}
					}

				
			}
		// set damage 
		if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_NPC_Damage_Easy, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_MOD );
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_NPC_Damage_Medium, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_MOD );
					}

				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
					{ 
						G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_NPC_Damage_Hard, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_MOD );
					}
				}
             }
			else if ( tr_ent->svFlags & SVF_GLASS_BRUSH || ( tr_ent->svFlags & SVF_BBRUSH && tr_ent->material == 12 )) // material grate...we are breaking a grate!
			{
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, 999, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_MOD ); // smash that puppy
				if ( ent->client->gs.gun->Gun_WallImpact_Effect )
				{
					G_PlayEffect( ent->client->gs.gun->Gun_WallImpact_Effect, tr.endpos, tr.plane.normal );  
				}
		
			}
			else // set playa damage 
			{	
				G_Damage( tr_ent, ent, ent, forwardVec, tr.endpos, ent->client->gs.gun->Gun_Damage, DAMAGE_NO_KNOCKBACK, ent->client->gs.gun->Gun_MOD );
			}
	}
}

}

//---------------------------------------------------------
/// RIFLE WEAPON LOGIC 
static void WP_FireNormal( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	start;
	int velocity	= BRYAR_PISTOL_VEL;
	int		damage = !alt_fire ? weaponData[WP_BLASTER_PISTOL].damage : weaponData[WP_BLASTER_PISTOL].altDamage;
	gunData_t *gun = NULL;

	// DMG FUMO
	if ( ent && ent->client && ent->client->NPC_class == CLASS_BESPIN_COP )
	{
		damage *= 1.5;
	}
	// DMG VUOTO
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_SHADOWTROOPER )
	{
		damage *= 2; // Elementali del VUOTO fanno 50% di danno in più!
	}

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

    if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
	// THE STUN 
			/*if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_KYLE ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_TUSKEN ||
				ent->client->NPC_class == CLASS_MELC || 
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_FLIER2 ||
				ent->client->NPC_class == CLASS_SWAMPTROOPER ||
				ent->client->NPC_class == CLASS_FISH ||
				ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_VAMPIRE ||   
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST ||
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 ||  
				ent->client->NPC_class == CLASS_KAIN3 ||
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS ||  
				ent->client->NPC_class == CLASS_NECROMANCER ||  
				ent->client->NPC_class == CLASS_VAMPIREQUEEN ||  
				ent->client->NPC_class == CLASS_DARKVAMPIRE ||  
				ent->client->NPC_class == CLASS_MASKVAMPIRE ||   
				ent->client->NPC_class == CLASS_MASKVAMPIRE2 || 
				ent->client->NPC_class == CLASS_HELLVAMPIRE ||  
				ent->client->NPC_class == CLASS_HELLVAMPIRE2 || 
				ent->client->NPC_class == CLASS_AIRVAMPIRE || 
				ent->client->NPC_class == CLASS_BARDVAMPIRE || 
				ent->client->NPC_class == CLASS_WOLFVAMPIRE ||  
				ent->client->NPC_class == CLASS_ELFVAMPIRE ||  
				ent->client->NPC_class == CLASS_ELFVAMPIRE2 ||  
				ent->client->NPC_class == CLASS_ELFVAMPIRE3 || 
				ent->client->NPC_class == CLASS_WATERVAMPIRE ||  
				ent->client->NPC_class == CLASS_FROSTVAMPIRE || 
				ent->client->NPC_class == CLASS_ARCHERVAMPIRE || 
				ent->client->NPC_class == CLASS_SARVAMPIRE ||
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_DUMAHVAMPIRE || 
				ent->client->NPC_class == CLASS_MELCVAMPIRE || 
				ent->client->NPC_class == CLASS_SARVAMPIRE )
	{
		{
			damage *= 0.5;
			G_PlayEffect( G_EffectIndex( "stun/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE SMOG 
	else if (	ent->client->NPC_class == CLASS_BESPIN_COP ||
				ent->client->NPC_class == CLASS_GOLEM_SMOG || 
				ent->client->NPC_class == CLASS_TANKREAPER || 
				ent->client->NPC_class == CLASS_SPIDERREAPER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAPER3 || 
				ent->client->NPC_class == CLASS_REELO ||
				ent->client->NPC_class == CLASS_CLAW ||
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON ||
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_FIREREAPER ||
				ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "bryar2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE VOID 
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_PROBE || 
				ent->client->NPC_class == CLASS_POLTER )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "bryar3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE KNIFE
	else if ( ent->client->NPC_class == CLASS_PRISONER ||
				ent->client->NPC_class == CLASS_SCOUT || 
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_ARCHER || 
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 ||
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_LIZARD ||
				ent->client->NPC_class == CLASS_JAN )
	{
		{
			damage *= 0.75;
			G_PlayEffect( G_EffectIndex( "knife/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROTIC
	else if ( ent->client->NPC_class == CLASS_GLIDER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_ARIEL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO || 
				ent->client->NPC_class == CLASS_MURJJ ||
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_DEMON_BLADE ||
				ent->client->NPC_class == CLASS_DEMON_POISON ||
				ent->client->NPC_class == CLASS_DEMON_ELECTRO ||
				ent->client->NPC_class == CLASS_ATST ||
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_SAND_CREATURE ||
				ent->client->NPC_class == CLASS_MOUSE ||
				ent->client->NPC_class == CLASS_PROPHET ||
				ent->client->NPC_class == CLASS_CENOBITA )
	{
		{
			damage *= 2.5;
			G_PlayEffect( G_EffectIndex( "absorb/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE ENTROPY 
	else if ( ent->client->NPC_class == CLASS_ALCHEMIST ||
				ent->client->NPC_class == CLASS_DESANN ||
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_GOLEM_FROST || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		{
			damage *= 3;
			G_PlayEffect( G_EffectIndex( "entropy/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT 
	else if (	ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_WIZARD ||  
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD ||
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_GALAKMECH ||
				ent->client->NPC_class == CLASS_RAZIEL || 
				ent->client->NPC_class == CLASS_NECROREAPER || 
				ent->client->NPC_class == CLASS_DARKREAPER || 
				ent->client->NPC_class == CLASS_LIGHTREAPER || 
				ent->client->NPC_class == CLASS_ARCANEREAPER || 
				ent->client->NPC_class == CLASS_ICEREAPER || 
				ent->client->NPC_class == CLASS_SPIDERREAPER || 
				ent->client->NPC_class == CLASS_AIRREAPER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_STONEREAPER || 				
				ent->client->NPC_class == CLASS_BARTENDER ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_IMPWORKER || 				
				ent->client->NPC_class == CLASS_INTERROGATOR ||
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 ||
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_SWAMP || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_WATER || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_HOLY ||  
				ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_WATER || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_METAL || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY )
			{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "bryar/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "bryar0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}*/
}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

	// THE STUN 
	/*if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_KYLE ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_TUSKEN ||
				ent->client->NPC_class == CLASS_MELC || 
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_FLIER2 ||
				ent->client->NPC_class == CLASS_SWAMPTROOPER ||
				ent->client->NPC_class == CLASS_FISH ||
				ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_VAMPIRE ||   
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 ||  
				ent->client->NPC_class == CLASS_KAIN3 ||
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS ||  
				ent->client->NPC_class == CLASS_NECROMANCER ||  
				ent->client->NPC_class == CLASS_VAMPIREQUEEN ||  
				ent->client->NPC_class == CLASS_DARKVAMPIRE ||  
				ent->client->NPC_class == CLASS_MASKVAMPIRE ||   
				ent->client->NPC_class == CLASS_MASKVAMPIRE2 || 
				ent->client->NPC_class == CLASS_HELLVAMPIRE ||  
				ent->client->NPC_class == CLASS_HELLVAMPIRE2 || 
				ent->client->NPC_class == CLASS_AIRVAMPIRE || 
				ent->client->NPC_class == CLASS_BARDVAMPIRE || 
				ent->client->NPC_class == CLASS_WOLFVAMPIRE ||  
				ent->client->NPC_class == CLASS_ELFVAMPIRE ||  
				ent->client->NPC_class == CLASS_ELFVAMPIRE2 ||  
				ent->client->NPC_class == CLASS_ELFVAMPIRE3 || 
				ent->client->NPC_class == CLASS_WATERVAMPIRE ||  
				ent->client->NPC_class == CLASS_FROSTVAMPIRE || 
				ent->client->NPC_class == CLASS_ARCHERVAMPIRE || 
				ent->client->NPC_class == CLASS_SARVAMPIRE ||
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_DUMAHVAMPIRE || 
				ent->client->NPC_class == CLASS_MELCVAMPIRE || 
				ent->client->NPC_class == CLASS_SARVAMPIRE )
	{
		{
			damage *= 0.5;
			G_PlayEffect( G_EffectIndex( "stun/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE SMOG 
	else if (	ent->client->NPC_class == CLASS_BESPIN_COP ||
				ent->client->NPC_class == CLASS_GOLEM_SMOG || 
				ent->client->NPC_class == CLASS_TANKREAPER || 
				ent->client->NPC_class == CLASS_SPIDERREAPER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAPER3 || 
				ent->client->NPC_class == CLASS_REELO ||
				ent->client->NPC_class == CLASS_CLAW ||
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON ||
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_FIREREAPER ||
				ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "bryar2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE VOID 
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_PROBE || 
				ent->client->NPC_class == CLASS_POLTER )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "bryar3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE KNIFE
	else if ( ent->client->NPC_class == CLASS_PRISONER ||
				ent->client->NPC_class == CLASS_SCOUT || 
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_ARCHER || 
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 ||
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_LIZARD ||
				ent->client->NPC_class == CLASS_JAN )
	{
		{
			damage *= 0.75;
			G_PlayEffect( G_EffectIndex( "knife/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROTIC
	else if ( ent->client->NPC_class == CLASS_GLIDER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_ARIEL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO || 
				ent->client->NPC_class == CLASS_MURJJ ||
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_DEMON_BLADE ||
				ent->client->NPC_class == CLASS_DEMON_POISON ||
				ent->client->NPC_class == CLASS_DEMON_ELECTRO ||
				ent->client->NPC_class == CLASS_ATST ||
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_SAND_CREATURE ||
				ent->client->NPC_class == CLASS_MOUSE ||
				ent->client->NPC_class == CLASS_PROPHET ||
				ent->client->NPC_class == CLASS_CENOBITA )
	{
		{
			damage *= 2.5;
			G_PlayEffect( G_EffectIndex( "absorb/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE ENTROPY 
	else if ( ent->client->NPC_class == CLASS_ALCHEMIST ||
				ent->client->NPC_class == CLASS_DESANN ||
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_GOLEM_FROST || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		{
			damage *= 3;
			G_PlayEffect( G_EffectIndex( "entropy/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT 
	else if ( ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_WIZARD ||  
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD ||
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL || 
				ent->client->NPC_class == CLASS_NECROREAPER || 
				ent->client->NPC_class == CLASS_DARKREAPER || 
				ent->client->NPC_class == CLASS_LIGHTREAPER || 
				ent->client->NPC_class == CLASS_ARCANEREAPER || 
				ent->client->NPC_class == CLASS_ICEREAPER || 
				ent->client->NPC_class == CLASS_SPIDERREAPER || 
				ent->client->NPC_class == CLASS_AIRREAPER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_STONEREAPER || 				
				ent->client->NPC_class == CLASS_BARTENDER ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_IMPWORKER || 				
				ent->client->NPC_class == CLASS_INTERROGATOR ||
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 ||
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_SWAMP || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_WATER || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_HOLY ||  
				ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_WATER || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_METAL || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "bryar/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "bryar0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}*/
}


	if ( alt_fire )
	{
		if ( ent->client->gs.gun->Gun_Alt_Speed && ent->client->gs.gun->Gun_Alt_Life )
		{
			// PROJECTILE - DEFAULT PARAMETERS SHOOTS. 
			WP_MissileTargetHint(ent, start, forwardVec);
			// customize speed and duration of alt missile 
			gentity_t	*missile = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent, alt_fire );
			missile->classname = "bryar_alt_proj";
			missile->damage = damage;
			missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	 		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
			// we don't want it to bounce forever
			missile->bounceCount = 0;
			if ( ent->client->gs.gun->Gun_Alt_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
				if ( ent->client->gs.gun->Gun_AltSplashDamage )
				{
					missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}
			// ALT DAMAGE CUSTOMIZATION 
			// IF entity is an NPC 
			// If ent is an NPC 
			if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
					}
				}
             }
			 else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_AltDamage;
			}
			// ALT SPLASH DAMAGE AND SPLASH RADIUS!!! 
			// splash damage 
			if ( ent->client->gs.gun->Gun_AltSplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;  
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_AltSplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;  
			}
			// ALT FIRE BALISTIC CODE CUSTOMIZATIONS 
			if ( ent->s.weapon == WP_BLASTER_PISTOL )
			{//*SIGH*... I hate our weapon system...
				missile->s.weapon = ent->s.weapon;
			}
			// BRYAR BALISTIC - ALT FIRE CHARGING SHOOT. 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_CHARGESHOOT ) // Gravity Affect Shoot
			{
				if ( ent->client->gs.gun->Gun_Alt_Charge_Unit ) // amount of charge unit 
				{//*SIGH*... I hate our weapon system...
					int count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Alt_Charge_Unit;
					if ( count < 1 )
					{
						count = 1;
					}
					else if ( count > 5 )
					{
						count = 5;
					}
					damage *= count;
					missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
				}
			}
		// REPEATER 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
			// FLECHETTE ALT 
		   if ( ent->client->gs.gun->gunFlags&GFL_ALT_SHRAPNELSHOOT ) // alt shrapnel of flechette enabled  
			{
				gi.Printf( S_COLOR_GREEN "Shooting Alt Shrapnel Shoot!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Alt_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce;
				}
			}
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_BOUNCEHALFSHOOT ) // half bounce enabled   
			{
				gi.Printf( S_COLOR_GREEN "Shooting Alt Half bounce!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_HALF;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Alt_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce;
				}
			}
			// BOWCASTER
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_DIVIDESHOOT ) // if Alt divide shoot is 1, shoot 
			// a divide shoot. the number is the Gun Alt Divide Shoot parm. 
			{	
			}	
			// ALT BOUNCE SHOOT
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_BOUNCESHOOT ) // alt bounce of bowcaster alt enabled  
			{
				gi.Printf( S_COLOR_GREEN "Shooting a Alt Bouncing Shoot!\n" , ent );
				//ent->client->sess.missionStats.shotsFired++;
				// NON RIESCO A CAPIRE PERCHE' CRASHA NELL'ALTFIRE!!! 
				//missile->s.eFlags |= EF_BOUNCE;
				if ( ent->client->gs.gun->Gun_Alt_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce;
				}
			}
		}
	}
	else // MAIN FIRE 
	{
		if ( ent->client->gs.gun->Gun_Speed && ent->client->gs.gun->Gun_Life )
		{
			// PROJECTILE - DEFAULT PARAMETERS SHOOTS. 
			WP_MissileTargetHint(ent, start, forwardVec);
			// customize speed and duration of alt missile 
			gentity_t	*missile = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent, alt_fire );
			missile->classname = "bryar_proj";
			missile->damage = damage;
			missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	 		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
			// we don't want it to bounce forever
			missile->bounceCount = 0;
			if ( ent->client->gs.gun->Gun_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
				if ( ent->client->gs.gun->Gun_SplashDamage )
				{
					missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}

			// MAIN DAMAGE CUSTOMIZATION 
			// IF entity is an NPC 
			// If ent is an NPC 
			if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
					{
						damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
					}
				}
             }
			else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_Damage;
			}
			// main splash damage 
			if ( ent->client->gs.gun->Gun_SplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage; 
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_SplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius; 
			}
			// MAIN FIRE BALISTIC CUSTOMIZATION 
			if ( ent->s.weapon == WP_BLASTER_PISTOL )
			{//*SIGH*... I hate our weapon system...
				missile->s.weapon = ent->s.weapon;
			}
			// BRYAR BALISTIC - FIRE CHARGING SHOOT AMOUNT 
			if ( ent->client->gs.gun->gunFlags&GFL_CHARGESHOOT ) // Gravity Affect Shoot
			{
				if ( ent->client->gs.gun->Gun_Charge_Unit ) // amount of charge unit 
				{//*SIGH*... I hate our weapon system...
					int count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Charge_Unit;
					if ( count < 1 )
					{
						count = 1;
					}
					else if ( count > 5 )
					{
						count = 5;
					}
					damage *= count;
					missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
				}
			}
			// BLASTER RIFLE
			/*if ( ent->client->gs.gun->Gun_Spread ) // Amount of Spread of Gun as blaster rifle and repeater 
			{
			}
			// Sniper Rifle 
			if (ent->client->gs.gun->Gun_Zoom ) // gun can zoom 
			{
			//		Gun_Zoom;		    // if is 1, the gun can zoom.
			//   Gun_StartZoomFov;         // Starting FOV when zooming
			//   Gun_EndZoomFov;            // Max FOV when zooming
			// 	Gun_ZoomTime;               // Time in milliseconds it takes to zoom all the way in
			// *Gun_ZoomMask;				// Zoom Mask of Gun
			}*/
			// REPEATER 
			if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Main Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
			// FLECHETTE 
			if ( ent->client->gs.gun->gunFlags&GFL_SHRAPNELSHOOT ) // shrapnel of flechette enabled  
			{
				gi.Printf( S_COLOR_GREEN "Shooting Shrapnel Shoot!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce;
				}
			}
			if ( ent->client->gs.gun->gunFlags&GFL_BOUNCEHALFSHOOT ) // half bounce enabled   
			{
				gi.Printf( S_COLOR_GREEN "Shooting half bounce!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_HALF;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce;
				}
			}
				if ( ent->client->gs.gun->gunFlags&GFL_BOUNCESHOOT ) // colpo rimbalzato 
			{
				gi.Printf( S_COLOR_GREEN "Shooting Bouncing Shoot!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE;
				if ( ent->client->gs.gun->Gun_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce;
				}
			}
		}
	}

	
	
	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}

// -----------------------------------------
// SPREAD SHOT LOGIC 
// -------------------------------------------------
static void WP_FireSpread( gentity_t *ent, vec3_t start, vec3_t dir, qboolean alt_fire )
//---------------------------------------------------------
{
	int velocity	= BRYAR_PISTOL_VEL;
	int		damage = !alt_fire ? weaponData[WP_BLASTER_PISTOL].damage : weaponData[WP_BLASTER_PISTOL].altDamage;
	gunData_t *gun = NULL;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

    if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}


	if ( alt_fire )
	{
		if ( ent->client->gs.gun->Gun_Alt_Speed && ent->client->gs.gun->Gun_Alt_Life )
		{
			// PROJECTILE - DEFAULT PARAMETERS SHOOTS. 
			WP_MissileTargetHint(ent, start, forwardVec);
			// customize speed and duration of alt missile 
			gentity_t	*missile = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent, alt_fire );
			missile->classname = "bryar_alt_proj";
			missile->damage = damage;
			missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	 		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
			// we don't want it to bounce forever
			missile->bounceCount = 0;
			// customize alt means of damage 
			if ( ent->client->gs.gun->Gun_Alt_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
				if ( ent->client->gs.gun->Gun_AltSplashDamage )
				{
					missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}
			if ( ent->client->gs.gun->Gun_Alt_MOD2 )
			{
				missile->methodOfDeath2 = ent->client->gs.gun->Gun_Alt_MOD2;
				if ( ent->client->gs.gun->Gun_AltSplashDamage )
				{
					missile->splashMethodOfDeath2 = ent->client->gs.gun->Gun_Alt_MOD2;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}
			if ( ent->client->gs.gun->Gun_Alt_MOD3 )
			{
				missile->methodOfDeath3 = ent->client->gs.gun->Gun_Alt_MOD3;
				if ( ent->client->gs.gun->Gun_AltSplashDamage )
				{
					missile->splashMethodOfDeath3 = ent->client->gs.gun->Gun_Alt_MOD3;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}


			// ALT DAMAGE CUSTOMIZATION 
			// IF entity is an NPC 
			// If ent is an NPC 
			if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
					}
				}
             }
			 else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_AltDamage;
			}
			// ALT SPLASH DAMAGE AND SPLASH RADIUS!!! 
			// splash damage 
			if ( ent->client->gs.gun->Gun_AltSplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;  
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_AltSplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;  
			}
			// ALT FIRE BALISTIC CODE CUSTOMIZATIONS 
			if ( ent->s.weapon == WP_BLASTER_PISTOL )
			{//*SIGH*... I hate our weapon system...
				missile->s.weapon = ent->s.weapon;
			}
			// BRYAR BALISTIC - ALT FIRE CHARGING SHOOT. 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_CHARGESHOOT ) // Gravity Affect Shoot
			{
				if ( ent->client->gs.gun->Gun_Alt_Charge_Unit ) // amount of charge unit 
				{//*SIGH*... I hate our weapon system...
					int count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Alt_Charge_Unit;
					if ( count < 1 )
					{
						count = 1;
					}
					else if ( count > 5 )
					{
						count = 5;
					}
					damage *= count;
					missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
				}
			}
			// Bounce shoot customization!
		//	missile->s.eFlags |= EF_BOUNCE;
		//	missile->bounceCount = 3;
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
			// DECLOACKING SABOTEURS, IF THIS IS ON 1. 
			if ( ent->client->gs.gun->gunFlags2&GFL_ALT_DECLOAK ) 
			{
				Saboteur_Decloak( ent, Q_irand( 3000, 10000 ) );
			}
			// SET SPREADING OF SHOOTS! 
			/*if ( ent->client->gs.gun->Gun_Alt_Spread )
			{
				angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Alt_Spread;
				angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Alt_Spread;
			}*/
		}
	}
	else // MAIN FIRE 
	{
		if ( ent->client->gs.gun->Gun_Speed && ent->client->gs.gun->Gun_Life )
		{
			// PROJECTILE - DEFAULT PARAMETERS SHOOTS. 
			WP_MissileTargetHint(ent, start, forwardVec);
			// customize speed and duration of alt missile 
			gentity_t	*missile = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent, alt_fire );
			missile->classname = "bryar_proj";
			missile->damage = damage;
			missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	 		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
			// we don't want it to bounce forever
			missile->bounceCount = 0;
			//missile->methodOfDeath = MOD_BRYAR;
			if ( ent->client->gs.gun->Gun_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
				if ( ent->client->gs.gun->Gun_SplashDamage )
				{
					missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}
			if ( ent->client->gs.gun->Gun_MOD2 )
			{
				missile->methodOfDeath2 = ent->client->gs.gun->Gun_MOD2;
				if ( ent->client->gs.gun->Gun_SplashDamage )
				{
					missile->splashMethodOfDeath2 = ent->client->gs.gun->Gun_MOD2;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}
			if ( ent->client->gs.gun->Gun_MOD3 )
			{
				missile->methodOfDeath3 = ent->client->gs.gun->Gun_MOD3;
				if ( ent->client->gs.gun->Gun_SplashDamage )
				{
					missile->splashMethodOfDeath3 = ent->client->gs.gun->Gun_MOD3;
					//missile->methodOfDeath = MOD_BRYAR_ALT;
				}
			}
			
			// MAIN DAMAGE CUSTOMIZATION 
			// IF entity is an NPC 
			// If ent is an NPC 
			if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
					{
						damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
					}
				}
             }
			else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_Damage;
			}
			// main splash damage 
			if ( ent->client->gs.gun->Gun_SplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage; 
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_SplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius; 
			}
			// MAIN FIRE BALISTIC CUSTOMIZATION 
			if ( ent->s.weapon == WP_BLASTER_PISTOL )
			{//*SIGH*... I hate our weapon system...
				missile->s.weapon = ent->s.weapon;
			}
			// BRYAR BALISTIC - FIRE CHARGING SHOOT AMOUNT 
			if ( ent->client->gs.gun->gunFlags&GFL_CHARGESHOOT ) // Gravity Affect Shoot
			{
				if ( ent->client->gs.gun->Gun_Charge_Unit ) // amount of charge unit 
				{//*SIGH*... I hate our weapon system...
					int count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Charge_Unit;
					if ( count < 1 )
					{
						count = 1;
					}
					else if ( count > 5 )
					{
						count = 5;
					}
					damage *= count;
					missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
				}
			}
			// DECLOACKING SABOTEURS, IF THIS IS ON 1. 
			if ( ent->client->gs.gun->gunFlags2&GFL_ALT_DECLOAK ) 
			{
				Saboteur_Decloak( ent, Q_irand( 3000, 10000 ) );
				Jedi_Decloak ( ent ); 
			}
			// SET SPREADING OF SHOOTS! 
			/*if ( ent->client->gs.gun->Gun_Spread )
			{
				angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Spread;
				angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Spread;
			}*/
		}
	}

	
	
	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}

// ------------------------------------------------
// RAY GUN WEAPONS LOGIC 
// ---------------------------------------------------
RayGun_Colors_t RayGunColor( const char *RayGunName ) 
{
	if ( !Q_stricmp( RayGunName, "crimson" ) ) 
	{
		return RAY_CRIMSON;
	}
	if ( !Q_stricmp( RayGunName, "red" ) ) 
	{
		return RAY_RED;
	}
	if ( !Q_stricmp( RayGunName, "orange" ) ) 
	{
		return RAY_ORANGE;
	}
	if ( !Q_stricmp( RayGunName, "yellow" ) ) 
	{
		return RAY_YELLOW;
	}
	if ( !Q_stricmp( RayGunName, "brown" ) ) 
	{
		return RAY_BROWN;
	}
	if ( !Q_stricmp( RayGunName, "green" ) ) 
	{
		return RAY_GREEN;
	}
	if ( !Q_stricmp( RayGunName, "blue" ) ) 
	{
		return RAY_BLUE;
	}
	if ( !Q_stricmp( RayGunName, "azure" ) ) 
	{
		return RAY_AZURE;
	}
	if ( !Q_stricmp( RayGunName, "cyan" ) ) 
	{
		return RAY_CYAN;
	}
	if ( !Q_stricmp( RayGunName, "indigo" ) ) 
	{
		return RAY_INDIGO;
	}
	if ( !Q_stricmp( RayGunName, "purple" ) ) 
	{
		return RAY_PURPLE;
	}
	if ( !Q_stricmp( RayGunName, "pink" ) ) 
	{
		return RAY_PINK;
	}
	if ( !Q_stricmp( RayGunName, "white" ) ) 
	{
		return RAY_WHITE;
	}
	if ( !Q_stricmp( RayGunName, "iris" ) ) 
	{
		return RAY_IRIS;
	}
	if ( !Q_stricmp( RayGunName, "grey" ) ) 
	{
		return RAY_GREY;
	}
	if ( !Q_stricmp( RayGunName, "black" ) ) 
	{
		return RAY_BLACK;
	}
	if ( !Q_stricmp( RayGunName, "random" ) ) 
	{
		return ((RayGun_Colors_t)(Q_irand( RAY_ELECTRIC_CRIMSON, RAY_ELECTRIC_BLACK )));
	}
	if ( !Q_stricmp( RayGunName, "electriccrimson" ) ) 
	{
		return RAY_ELECTRIC_CRIMSON;
	}
	if ( !Q_stricmp( RayGunName, "electricred" ) ) 
	{
		return RAY_ELECTRIC_RED;
	}
	if ( !Q_stricmp( RayGunName, "electricorange" ) ) 
	{
		return RAY_ELECTRIC_ORANGE;
	}
	if ( !Q_stricmp( RayGunName, "electricyellow" ) ) 
	{
		return RAY_ELECTRIC_YELLOW;
	}
	if ( !Q_stricmp( RayGunName, "electricbrown" ) ) 
	{
		return RAY_ELECTRIC_BROWN;
	}
	if ( !Q_stricmp( RayGunName, "electricgreen" ) ) 
	{
		return RAY_ELECTRIC_GREEN;
	}
	if ( !Q_stricmp( RayGunName, "electricblue" ) ) 
	{
		return RAY_ELECTRIC_BLUE;
	}
	if ( !Q_stricmp( RayGunName, "electricazure" ) ) 
	{
		return RAY_ELECTRIC_AZURE;
	}
	if ( !Q_stricmp( RayGunName, "electriccyan" ) ) 
	{
		return RAY_ELECTRIC_CYAN;
	}
	if ( !Q_stricmp( RayGunName, "electricindigo" ) ) 
	{
		return RAY_ELECTRIC_INDIGO;
	}
	if ( !Q_stricmp( RayGunName, "electricpurple" ) ) 
	{
		return RAY_ELECTRIC_PURPLE;
	}
	if ( !Q_stricmp( RayGunName, "electricpink" ) ) 
	{
		return RAY_ELECTRIC_PINK;
	}
	if ( !Q_stricmp( RayGunName, "electricwhite" ) ) 
	{
		return RAY_ELECTRIC_WHITE;
	}
	if ( !Q_stricmp( RayGunName, "electricgrey" ) ) 
	{
		return RAY_ELECTRIC_GREY;
	}
	if ( !Q_stricmp( RayGunName, "electricblack" ) ) 
	{
		return RAY_ELECTRIC_BLACK;
	}
	if ( !Q_stricmp( RayGunName, "electricrandom" ) ) 
	{
		return ((RayGun_Colors_t)(Q_irand( RAY_ELECTRIC_RED, RAY_ELECTRIC_BLACK )));
	}
	return RAY_BLUE;
}

void WP_FireRay( gentity_t *ent, qboolean alt_fire )
{
	int			damage = weaponData[WP_BLASTER_PISTOL].damage;
	gunData_t *gun = NULL; 
	qboolean	render_impact = qtrue;
	vec3_t		start, end, spot;
	trace_t		tr;
	gentity_t	*traceEnt = NULL, *tent;
	float		dist, shotDist, shotRange = 8192;
	//float		dist, shotDist, ShotRange = ent->client->gs.gun->Gun_Range;
	//( ent->client->gs.gun->Gun_Muzzle_Effect )


// FIRST OFF SET DAMAGE. 
	if ( ent->client->gs.gun->Gun_Damage || ent->client->gs.gun->Gun_AltDamage ||
	ent->client->gs.gun->Gun_NPC_Damage_Easy || ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy ||
	ent->client->gs.gun->Gun_NPC_Damage_Medium || ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium ||
	ent->client->gs.gun->Gun_NPC_Damage_Hard || ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard ) 

{	
	// If ent is an NPC 
	if ( ent->s.number != 0 )
	{
		// on level easy 
		if ( g_spskill->integer == 0 )
		{
			// npc easy altfire damage 
			 if ( alt_fire )
			 {
				 damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
			 }
			 else // npc easy main damage
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;;
			 }
		}
		// on level medium 
		else if ( g_spskill->integer == 1 )
		{
			// npc medium altfire damage 
			 if ( alt_fire )
			 {
				 damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
			 }
			 else // npc medium main damage
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
			 }
		}
		// on level hard // 
		else if ( g_spskill->integer > 1 )
		{
			// npc hard alt fire 
			 if ( alt_fire )
			 {
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
			 }
			 // npc hard main fire 
			 else 
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
			}
		}
	}
	else // customize damage for player 
	{
		if ( alt_fire )
		 {
			 damage = ent->client->gs.gun->Gun_AltDamage;
		}
		else // npc medium main damage
		{
			damage = ent->client->gs.gun->Gun_Damage;
		}

		
	}
}

// SET THE MUZZLE EFFECT 
if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
}



	// CREATE THE RAY SHOOT! 
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );
	WP_MissileTargetHint(ent, start, forwardVec);
	VectorMA( start, shotRange, forwardVec, end );

	int ignore = ent->s.number;
	int traces = 0;
	while ( traces < 10 )
	{//need to loop this in case we hit a Jedi who dodges the shot
		gi.trace( &tr, start, NULL, NULL, end, ignore, MASK_SHOT, G2_RETURNONHIT, 0 );

		traceEnt = &g_entities[tr.entityNum];
		if ( traceEnt // CAN DODGE ONLY: SCOUT WITH BOW, DUMAHIM REAPERS, AND DUMAHIM BOSSES. 
			&& ( traceEnt->s.weapon == WP_BLASTER_PISTOL || (traceEnt->client && (traceEnt->client->NPC_class == CLASS_BOBAFETT||traceEnt->client->NPC_class == CLASS_WEEQUAY && traceEnt->s.weapon == WP_THERMAL ||traceEnt->client->NPC_class == CLASS_LIZARD ) ) ) )
		{//FIXME: need a more reliable way to know we hit a jedi? Only Dumahim with dodge skill can avoid this shot! 
			if ( Jedi_DodgeEvasion( traceEnt, ent, &tr, HL_NONE ) )
			{//act like we didn't even hit him
				VectorCopy( tr.endpos, start );
				ignore = tr.entityNum;
				traces++;
				continue;
			}
		}
		//a Jedi is not dodging this shot
		break;
	}

	if ( tr.surfaceFlags & SURF_NOIMPACT ) 
	{
		render_impact = qfalse;
	}

	// always render a shot beam, doing this the old way because I don't much feel like overriding the effect.
	if ( alt_fire ) 
	{
		if ( ent->client->gs.gun->Gun_Alt_Ray_Effect )
		{
			gi.Printf( S_COLOR_YELLOW "I Am Shooting a Ray!\n" , ent );
			gi.Printf( S_COLOR_RED "I am Shooting a Red Ray. Do you see it?\n" , ent );
//			tent = G_TempEntity( tr.endpos, EV_GUN_ALT_RAY1 );
			tent->svFlags |= SVF_BROADCAST;
			VectorCopy( muzzle, tent->s.origin2 );
		}
	}
	else 
	{
		if ( ent->client->gs.gun->Gun_Ray_Effect )
		{
			gi.Printf( S_COLOR_YELLOW "I Am Shooting a Ray!\n" , ent );
			gi.Printf( S_COLOR_RED "I am Shooting a Red Ray. Do you see it?\n" , ent );
//			tent = G_TempEntity( tr.endpos, EV_GUN_RAY1 );
			tent->svFlags |= SVF_BROADCAST;
			VectorCopy( muzzle, tent->s.origin2 );
		}
	}
	
// VISUAL EFFECTS OF THE RAY 
	if ( render_impact )
	{
		if ( tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage )
		{
			// Create a simple impact type mark that doesn't last long in the world
			// Customize Flesh Impact Effect 
			if ( alt_fire ) // if altfire 
			{
				if ( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect )
				{
				}		G_PlayEffect( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect, tr.endpos, tr.plane.normal );
			
			} 
			else // Main Fire 
			{
				if ( ent->client->gs.gun->Gun_FleshImpact_Effect )
				{
					G_PlayEffect( ent->client->gs.gun->Gun_FleshImpact_Effect, tr.endpos, tr.plane.normal );
				}
			} 


			if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
			{
				ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
			} 

			int hitLoc = G_GetHitLocFromTrace( &tr, MOD_DISRUPTOR );
			if ( traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GALAKMECH )
			{//hehe
				// (customize ray damage) 
				if ( alt_fire )
				{
					if ( ent->client->gs.gun->Gun_Alt_MOD )
					{
						G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 3, DAMAGE_DEATH_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD, hitLoc );
					}
				}
				else 
				{
					G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 3, DAMAGE_DEATH_KNOCKBACK, ent->client->gs.gun->Gun_MOD, hitLoc );
				}
			}
			else
			{
				// (customize ray damage) 
				if ( alt_fire )
				{
					if ( ent->client->gs.gun->Gun_Alt_MOD )
					{
						G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 3, DAMAGE_DEATH_KNOCKBACK, ent->client->gs.gun->Gun_Alt_MOD, hitLoc );
					}
				}
				else 
				{
					G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 3, DAMAGE_DEATH_KNOCKBACK, ent->client->gs.gun->Gun_MOD, hitLoc );
				}

				//G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_DEATH_KNOCKBACK, MOD_BRYAR, hitLoc );
			}

		}
		else // Ray Miss NPCs or Players 
		{
			if ( alt_fire ) // if altfire 
			{
				if ( ent->client->gs.gun->Gun_Alt_WallImpact_Effect )
				{
					G_PlayEffect( ent->client->gs.gun->Gun_Alt_WallImpact_Effect, tr.endpos, tr.plane.normal );
				}
			} 
			else // Main Fire 
			{
				if ( ent->client->gs.gun->Gun_WallImpact_Effect )
				{
					G_PlayEffect( ent->client->gs.gun->Gun_WallImpact_Effect, tr.endpos, tr.plane.normal );
				}
			}
			if ( alt_fire ) // add bezier 
			{
				// we only make this mark on things that can't break or move
//				tent = G_TempEntity( tr.endpos, EV_GUN_ALT_RAY1_MISS ); // here a lot of work for customize alt miss! 
				tent->svFlags |= SVF_BROADCAST;
				VectorCopy( tr.plane.normal, tent->pos1 );
			}
			else 
			{
				// we only make this mark on things that can't break or move
//				tent = G_TempEntity( tr.endpos, EV_GUN_RAY1_MISS ); // here a lot of work for customize alt miss! 
				tent->svFlags |= SVF_BROADCAST;
				VectorCopy( tr.plane.normal, tent->pos1 );
			}
		}
	}

	shotDist = shotRange * tr.fraction;

	for ( dist = 0; dist < shotDist; dist += 64 )
	{
		//FIXME: on a really long shot, this could make a LOT of alerts in one frame...
		VectorMA( start, dist, forwardVec, spot );
		AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
	}
	VectorMA( start, shotDist-4, forwardVec, spot );
	AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
}

void WP_FireRay2( gentity_t *ent, qboolean alt_fire )
{
	int			damage = weaponData[WP_BLASTER_PISTOL].altDamage, skip, traces = 2;
	qboolean	render_impact = qtrue;
	vec3_t		start, end;
	vec3_t		muzzle2, spot, dir;
	trace_t		tr;
	gentity_t	*traceEnt, *tent;
	float		dist, shotDist, shotRange = 8192;
	qboolean	hitDodged = qfalse, fullCharge = qfalse;
	gunData_t *gun = NULL;
	VectorCopy( muzzle, muzzle2 ); // making a backup copy

	// The trace start will originate at the eye so we can ensure that it hits the crosshair.
	// Set Muzzle Effect! 
	
// SET THE MUZZLE EFFECT 
if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
}
	
// Now Customize The Damage this is a little different respect other weapons 
if ( ent->NPC )
{
	if ( alt_fire ) 
	{
		switch ( g_spskill->integer )
		{
		case 0:
			if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy ) 
			{
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy; 			
			}
			break;
		case 1:
			if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium ) 
			{
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium; 			
			}
			break;
		case 2:
		default:
			if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard ) 
			{
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard; 			
			}
			break;
		}
		VectorCopy( muzzle, start );
		fullCharge = qtrue;
	}
	else // Main Fire 
	{
		switch ( g_spskill->integer )
		{
		case 0:
			if ( ent->client->gs.gun->Gun_NPC_Damage_Easy ) 
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Easy; 			
			}
			break;
		case 1:
			if ( ent->client->gs.gun->Gun_NPC_Damage_Medium ) 
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Medium; 			
			}
			break;
		case 2:
		default:
			if ( ent->client->gs.gun->Gun_NPC_Damage_Hard ) 
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Hard; 			
			}
			break;
		}
		VectorCopy( muzzle, start );
		fullCharge = qtrue;
	}
}
else // Set Player Damage! 
{
	VectorCopy( ent->client->renderInfo.eyePoint, start );
	AngleVectors( ent->client->renderInfo.eyeAngles, forwardVec, NULL, NULL );
	// don't let NPC's do charging
	int count = ( level.time - ent->client->ps.weaponChargeTime - 50 ) / DISRUPTOR_CHARGE_UNIT;
	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count >= 10 )
	{
		count = 10;
		fullCharge = qtrue;
	}
	// more powerful charges go through more things
	if ( count < 3 )
	{
		traces = 1;
	}
	else if ( count < 6 )
	{
		traces = 2;
	}
	//else do full traces
	if ( alt_fire ) 
	{
		if ( ent->client->gs.gun->Gun_AltDamage ) 
		{
			damage = damage * count + ent->client->gs.gun->Gun_AltDamage * ent->client->gs.gun->Gun_Alt_Charge_Unit; // give a boost to low charge shots 0.5 is default value! 
		}
	}
	else 
	{
		if ( ent->client->gs.gun->Gun_Damage ) 
		{
			damage = damage * count + ent->client->gs.gun->Gun_Damage * ent->client->gs.gun->Gun_Charge_Unit; // give a boost to low charge shots 0.5 is default value! 
		}
	}
}

// FLESH AND WALL IMPACT EFXs 
	skip = ent->s.number;
	for ( int i = 0; i < traces; i++ )
	{
		VectorMA( start, shotRange, forwardVec, end );
		//NOTE: if you want to be able to hit guys in emplaced guns, use "G2_COLLIDE, 10" instead of "G2_RETURNONHIT, 0"
		//alternately, if you end up hitting an emplaced_gun that has a sitter, just redo this one trace with the "G2_COLLIDE, 10" to see if we it the sitter
		gi.trace( &tr, start, NULL, NULL, end, skip, MASK_SHOT, G2_COLLIDE, 10 );//G2_RETURNONHIT, 0 );
		if ( tr.surfaceFlags & SURF_NOIMPACT ) 
		{
			render_impact = qfalse;
		}
		if ( tr.entityNum == ent->s.number )
		{
			// should never happen, but basically we don't want to consider a hit to ourselves?
			// Get ready for an attempt to trace through another person
			VectorCopy( tr.endpos, muzzle2 );
			VectorCopy( tr.endpos, start );
			skip = tr.entityNum;
#ifdef _DEBUG
			gi.Printf( "BAD! Ray shot somehow traced back and hit the owner!\n" );			
#endif
			continue;
		}

		if ( tr.fraction >= 1.0f )
		{
			// draw the beam but don't do anything else
			break;
		}

		traceEnt = &g_entities[tr.entityNum];

		if ( traceEnt //&& traceEnt->NPC // CAN DODGE ONLY: SCOUT WITH BOW, DUMAHIM REAPERS, AND DUMAHIM BOSSES. 
			&& ((traceEnt->client && (traceEnt->client->NPC_class == CLASS_BOBAFETT||traceEnt->client->NPC_class == CLASS_WEEQUAY && traceEnt->s.weapon == WP_THERMAL || traceEnt->client->NPC_class == CLASS_LIZARD ) ) ) )
		{//FIXME: need a more reliable way to know we hit a jedi?
			hitDodged = Jedi_DodgeEvasion( traceEnt, ent, &tr, HL_NONE );
			//acts like we didn't even hit him
		}
		if ( !hitDodged )
		{ // Enemy is Hitted by the shot 
			if ( render_impact )
			{
				if (( tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage ) 
					|| !Q_stricmp( traceEnt->classname, "misc_model_breakable" ) 
					|| traceEnt->s.eType == ET_MOVER )
				{ // HITTED ENEMY! 
					if ( alt_fire ) 
					{
						if ( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect ) 
						{
							// Create a simple impact type mark that doesn't last long in the world
							G_PlayEffect( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect, tr.endpos, tr.plane.normal );
						}
						if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
						{//NOTE: hitting multiple ents can still get you over 100% accuracy
							ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
						} 

						int hitLoc = G_GetHitLocFromTrace( &tr, MOD_BRYAR_ALT );
						if ( traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GALAKMECH )
						{//hehe
							if ( ent->client->gs.gun->Gun_Alt_MOD )
							{
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 10, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, fullCharge ? ent->client->gs.gun->Gun_Alt_MOD : ent->client->gs.gun->Gun_MOD, hitLoc );
							}
							break;
						}
						// (customize ray damage) 
						if ( ent->client->gs.gun->Gun_Alt_MOD )
						{
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_DEATH_KNOCKBACK|DAMAGE_NO_HIT_LOC, fullCharge ? ent->client->gs.gun->Gun_Alt_MOD : ent->client->gs.gun->Gun_MOD, hitLoc );
						}
						//G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, fullCharge ? MOD_BRYAR_ALT : MOD_BRYAR, hitLoc );
						if ( traceEnt->s.eType == ET_MOVER )
						{//stop the traces on any mover
							break;
						}
					}
					else 
					// main fire 
					{

						if ( ent->client->gs.gun->Gun_FleshImpact_Effect ) 
						{
							// Create a simple impact type mark that doesn't last long in the world
							G_PlayEffect( ent->client->gs.gun->Gun_FleshImpact_Effect, tr.endpos, tr.plane.normal );
						}
						if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
						{//NOTE: hitting multiple ents can still get you over 100% accuracy
							ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
						} 

						int hitLoc = G_GetHitLocFromTrace( &tr, MOD_BRYAR_ALT );
						if ( traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GALAKMECH )
						{//hehe
							if ( ent->client->gs.gun->Gun_MOD )
							{
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 10, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, fullCharge ? ent->client->gs.gun->Gun_MOD : ent->client->gs.gun->Gun_Alt_MOD, hitLoc );
							}
							break;
						}
						// (customize ray damage) 
						if ( ent->client->gs.gun->Gun_MOD )
						{
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_DEATH_KNOCKBACK|DAMAGE_NO_HIT_LOC, fullCharge ? ent->client->gs.gun->Gun_MOD : ent->client->gs.gun->Gun_Alt_MOD, hitLoc );
						}
						//G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, fullCharge ? MOD_BRYAR_ALT : MOD_BRYAR, hitLoc );
						if ( traceEnt->s.eType == ET_MOVER )
						{//stop the traces on any mover
							break;
						}
					}
					
				}
				else // MISSED ENEMY! 
				{
					if ( alt_fire )
					{
						// we only make this mark on things that can't break or move
//						tent = G_TempEntity( tr.endpos, EV_GUN_ALT_RAY2_MISS );
						tent->svFlags |= SVF_BROADCAST;
						VectorCopy( tr.plane.normal, tent->pos1 );
						break; // hit solid, but doesn't take damage, so stop the shot...we _could_ allow it to shoot through walls, might be cool?
					}
					else 
					{
						// we only make this mark on things that can't break or move
	//					tent = G_TempEntity( tr.endpos, EV_GUN_RAY2_MISS );
						tent->svFlags |= SVF_BROADCAST;
						VectorCopy( tr.plane.normal, tent->pos1 );
						break; // hit solid, but doesn't take damage, so stop the shot...we _could_ allow it to shoot through walls, might be cool?
					}
				}

			}
			else // not rendering impact, must be a skybox or other similar thing?
			{
				break; // don't try anymore traces
			}
		}
		// Get ready for an attempt to trace through another person
		VectorCopy( tr.endpos, muzzle2 );
		VectorCopy( tr.endpos, start );
		skip = tr.entityNum;
		hitDodged = qfalse;
	}
// RAY EFFECTS 
	if ( alt_fire ) 
	{
		//just draw one solid beam all the way to the end...
//		tent = G_TempEntity( tr.endpos, EV_GUN_ALT_RAY2 );
		tent->svFlags |= SVF_BROADCAST;
		tent->alt_fire = fullCharge; // mark us so we can alter the effect
		VectorCopy( muzzle, tent->s.origin2 );
		// now go along the trail and make sight events
		VectorSubtract( tr.endpos, muzzle, dir );
		shotDist = VectorNormalize( dir );
		//FIXME: if shoot *really* close to someone, the alert could be way out of their FOV
		for ( dist = 0; dist < shotDist; dist += 64 )
		{
			//FIXME: on a really long shot, this could make a LOT of alerts in one frame...
			VectorMA( muzzle, dist, dir, spot );
			AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
		}
		//FIXME: spawn a temp ent that continuously spawns sight alerts here?  And 1 sound alert to draw their attention?
		VectorMA( start, shotDist-4, forwardVec, spot );
		AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
	}
	else 
	{
		//just draw one solid beam all the way to the end...
//		tent = G_TempEntity( tr.endpos, EV_GUN_RAY2 );
		tent->svFlags |= SVF_BROADCAST;
		VectorCopy( muzzle, tent->s.origin2 );
		// now go along the trail and make sight events
		VectorSubtract( tr.endpos, muzzle, dir );
		shotDist = VectorNormalize( dir );
		//FIXME: if shoot *really* close to someone, the alert could be way out of their FOV
		for ( dist = 0; dist < shotDist; dist += 64 )
		{
			//FIXME: on a really long shot, this could make a LOT of alerts in one frame...
			VectorMA( muzzle, dist, dir, spot );
			AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
		}
		//FIXME: spawn a temp ent that continuously spawns sight alerts here?  And 1 sound alert to draw their attention?
		VectorMA( start, shotDist-4, forwardVec, spot );
		AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
	}
	
}


void WP_FireRay3( gentity_t *ent, qboolean alt_fire )
{
	int			damage = weaponData[WP_BLASTER_PISTOL].altDamage, skip, traces = 3;
	qboolean	render_impact = qtrue;
	vec3_t		start, end;
	vec3_t		muzzle2, spot, dir;
	trace_t		tr;
	gentity_t	*traceEnt, *tent;
	float		dist, shotDist, shotRange = 8192;
	qboolean	hitDodged = qfalse;
	gunData_t *gun = NULL;
	// First Off Set a Muzzle Effects, Damage and Spreading of Shots!  
	if ( alt_fire )
	{
		// Alt Muzzle 
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect ) // Alt Muzzle Fire 
		{		
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
		if ( ent->client->gs.gun->Gun_Alt_Bounce_Effect ) // Ray not bounce , bounceffect is used for customize ring
		{
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
		// Customize Damage! 
		if ( ent->NPC )
		{
			switch ( g_spskill->integer )
			{
				case 0:
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy ) 
					{
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
					}
				break;
				case 1:
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium ) 
					{
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
					}
				break;
				case 2:
			default:
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard ) 
					{
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
					}
				break;
			}
		}
		else // Playa 
		{
			if ( ent->client->gs.gun->Gun_AltDamage ) 
				{
					damage = ent->client->gs.gun->Gun_AltDamage;
				}
		}
		if ( ent->client->gs.gun->Gun_Alt_Spread ) 
		 {
			 if (ent->s.number >= MAX_CLIENTS)// NPCS 
			 {
				 vec3_t angles;
				vectoangles(forwardVec, angles);
				angles[PITCH] += ( crandom() * (ent->client->gs.gun->Gun_Alt_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				angles[YAW]	  += ( crandom() * (ent->client->gs.gun->Gun_Alt_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				AngleVectors(angles, forwardVec, vrightVec, up);			
			}
			else // Playa 
			{
				 vec3_t angles;
				vectoangles(forwardVec, angles);
				angles[PITCH] += ( crandom() * (ent->client->gs.gun->Gun_Alt_Spread*0.25f));//was 0.5f
				angles[YAW]	  += ( crandom() * (ent->client->gs.gun->Gun_Alt_Spread*0.25f));//was 0.5f
				AngleVectors(angles, forwardVec, vrightVec, up);			
			}
	   }
	}
	else // MAIN FIRE 
	{
	  // Main Muzzle 
		if ( ent->client->gs.gun->Gun_Muzzle_Effect ) //  Muzzle Fire 
		{		
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint );
		}
		if ( ent->client->gs.gun->Gun_Bounce_Effect ) // Ray not bounce , bounceffect is used for customize ring
		{
			G_PlayEffect( ent->client->gs.gun->Gun_Bounce_Effect, ent->client->renderInfo.handRPoint );
		}
		// Customize Damage! 
		if ( ent->NPC )
		{
			switch ( g_spskill->integer )
			{
				case 0:
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy ) 
					{
						damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
					}
				break;
				case 1:
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium ) 
					{
						damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
					}
				break;
				case 2:
			default:
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard ) 
					{
						damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
					}
					break;
			}
		}
		else // Playa 
		{
			if ( ent->client->gs.gun->Gun_AltDamage ) 
				{
					damage = ent->client->gs.gun->Gun_AltDamage;
				}
		}
		if ( ent->client->gs.gun->Gun_Alt_Spread ) 
		 {
			 if (ent->s.number >= MAX_CLIENTS)// NPCS 
			 {
				 vec3_t angles;
				vectoangles(forwardVec, angles);
				angles[PITCH] += ( crandom() * (ent->client->gs.gun->Gun_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				angles[YAW]	  += ( crandom() * (ent->client->gs.gun->Gun_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				AngleVectors(angles, forwardVec, vrightVec, up);			
			}
			else // Playa 
			{
				 vec3_t angles;
				vectoangles(forwardVec, angles);
				angles[PITCH] += ( crandom() * (ent->client->gs.gun->Gun_Spread*0.25f));//was 0.5f
				angles[YAW]	  += ( crandom() * (ent->client->gs.gun->Gun_Spread*0.25f));//was 0.5f
				AngleVectors(angles, forwardVec, vrightVec, up);			
			}
	   }
	}

	//Shove us backwards for half a second
	VectorMA( ent->client->ps.velocity, -200, forwardVec, ent->client->ps.velocity );
	ent->client->ps.groundEntityNum = ENTITYNUM_NONE;
	if ( (ent->client->ps.pm_flags&PMF_DUCKED) )
	{//hunkered down
		ent->client->ps.pm_time = 100;
	}
	else
	{
		ent->client->ps.pm_time = 250;
	}
	ent->client->ps.pm_flags |= PMF_TIME_KNOCKBACK|PMF_TIME_NOFRICTION;
	//FIXME: only if on ground?  So no "rocket jump"?  Or: (see next FIXME)
	//FIXME: instead, set a forced ucmd backmove instead of this sliding
	VectorCopy( muzzle, muzzle2 ); // making a backup copy
	// The trace start will originate at the eye so we can ensure that it hits the crosshair.
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );
	skip = ent->s.number;
	//Make it a little easier to hit guys at long range
	vec3_t shot_mins, shot_maxs;
	VectorSet( shot_mins, -1, -1, -1 );
	VectorSet( shot_maxs, 1, 1, 1 );

	for ( int i = 0; i < traces; i++ )
	{
		VectorMA( start, shotRange, forwardVec, end );
		//NOTE: if you want to be able to hit guys in emplaced guns, use "G2_COLLIDE, 10" instead of "G2_RETURNONHIT, 0"
		//alternately, if you end up hitting an emplaced_gun that has a sitter, just redo this one trace with the "G2_COLLIDE, 10" to see if we it the sitter
		//gi.trace( &tr, start, NULL, NULL, end, skip, MASK_SHOT, G2_COLLIDE, 10 );//G2_RETURNONHIT, 0 );
		gi.trace( &tr, start, shot_mins, shot_maxs, end, skip, MASK_SHOT, G2_COLLIDE, 10 );//G2_RETURNONHIT, 0 );
		if ( tr.surfaceFlags & SURF_NOIMPACT ) 
		{
			render_impact = qfalse;
		}
		if ( tr.entityNum == ent->s.number )
		{
			// should never happen, but basically we don't want to consider a hit to ourselves?
			// Get ready for an attempt to trace through another person
			VectorCopy( tr.endpos, muzzle2 );
			VectorCopy( tr.endpos, start );
			skip = tr.entityNum;
#ifdef _DEBUG
			gi.Printf( "BAD! Concussion gun shot somehow traced back and hit the owner!\n" );			
#endif
			continue;
		}
		if ( tr.fraction >= 1.0f )
		{
			// draw the beam but don't do anything else
			break;
		}

		traceEnt = &g_entities[tr.entityNum];

		if ( traceEnt //&& traceEnt->NPC  // CAN DODGE ONLY: Who use this, the Necromancers, the Summoners, the Rahabim, the Sarafan Lords and the Liches.  
			&& ( (traceEnt->client && (traceEnt->client->NPC_class == CLASS_GLIDER||traceEnt->client->NPC_class == CLASS_FISH||traceEnt->client->NPC_class == CLASS_JAN|| traceEnt->client->NPC_class == CLASS_COMMANDO && traceEnt->s.weapon == WP_CONCUSSION || traceEnt->client->NPC_class == CLASS_MURJJ ) ) ) )
		{//FIXME: need a more reliable way to know we hit a jedi?
			hitDodged = Jedi_DodgeEvasion( traceEnt, ent, &tr, HL_NONE );
			//acts like we didn't even hit him
		}
		if ( !hitDodged ) // YOU HIT AN ENEMY, PLAY A CUSTOM EFFECT!
		{
			if ( render_impact )
						if (( tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage ) 
					|| !Q_stricmp( traceEnt->classname, "misc_model_breakable" ) 
					|| traceEnt->s.eType == ET_MOVER )
			{
					if ( alt_fire) 
					{
						if ( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect )
						{
							G_PlayEffect( ent->client->gs.gun->Gun_Alt_FleshImpact_Effect, tr.endpos, tr.plane.normal );
						}
					}
					else // main fire 
					{
						if ( ent->client->gs.gun->Gun_FleshImpact_Effect )
						{
							G_PlayEffect( ent->client->gs.gun->Gun_FleshImpact_Effect, tr.endpos, tr.plane.normal );
						}
					}
				}
					if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
					{//NOTE: hitting multiple ents can still get you over 100% accuracy
						ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
					} 

					// NO KNOCKBACK CLASS 
					int hitLoc = G_GetHitLocFromTrace( &tr, MOD_BRYAR_ALT );
					qboolean noKnockBack = (traceEnt->flags&FL_NO_KNOCKBACK);//will be set if they die, I want to know if it was on *before* they died
					
					if ( alt_fire ) 
					{
						if ( ent->client->gs.gun->Gun_Alt_MOD )
						{
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 10, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, ent->client->gs.gun->Gun_Alt_MOD, hitLoc );
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, ent->client->gs.gun->Gun_Alt_MOD, hitLoc );
						}
					} 
					else 
					{
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 10, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, ent->client->gs.gun->Gun_MOD, hitLoc );
								G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, ent->client->gs.gun->Gun_MOD, hitLoc );
					}
					 
					//do knockback and knockdown manually
					if ( traceEnt->client )
					{//only if we hit a client
						vec3_t pushDir;
						VectorCopy( forwardVec, pushDir );
						if ( pushDir[2] < 0.2f )
						{
							pushDir[2] = 0.2f;
						}//hmm, re-normalize?  nah...
						//if ( traceEnt->NPC || Q_irand(0,g_spskill->integer+1) )
						{
							if ( !noKnockBack )
							{//knock-backable
								G_Throw( traceEnt, pushDir, 200 );
								if ( traceEnt->client->NPC_class == CLASS_ROCKETTROOPER )
								{
									traceEnt->client->ps.pm_time = Q_irand( 1500, 3000 );
								}
							}
							if ( traceEnt->health > 0 )
							{//alive
								if ( G_HasKnockdownAnims( traceEnt ) )
								{//knock-downable
									G_Knockdown( traceEnt, ent, pushDir, 400, qtrue );
								}
							}
						}
					}

					if ( traceEnt->s.eType == ET_MOVER )
					{//stop the traces on any mover
						break;
					}
		}
		else // You Miss the Shot! 
		{
			if ( alt_fire ) 
			{
				// we only make this mark on things that can't break or move
//				tent = G_TempEntity( tr.endpos, EV_GUN_ALT_RAY3_MISS ); // here a lot of work for customize alt miss! 
				tent->svFlags |= SVF_BROADCAST;
				VectorCopy( tr.plane.normal, tent->pos1 );
				break; // hit solid, but doesn't take damage, so stop the shot...we _could_ allow it to shoot through walls, might be cool?
			}
			else 
			{
				// we only make this mark on things that can't break or move
				//tent = G_TempEntity( tr.endpos, EV_GUN_RAY3_MISS ); // here a lot of work for customize alt miss! 
				tent->svFlags |= SVF_BROADCAST;
				VectorCopy( tr.plane.normal, tent->pos1 );
				break; // hit solid, but doesn't take damage, so stop the shot...we _could_ allow it to shoot through walls, might be cool?
			}

		}
		// Get ready for an attempt to trace through another person
		VectorCopy( tr.endpos, muzzle2 );
		VectorCopy( tr.endpos, start );
		skip = tr.entityNum;
		hitDodged = qfalse;
	}

	if ( alt_fire ) 
		{
		// DRAW THE BEAM 
		//just draw one beam all the way to the end
//		tent = G_TempEntity( tr.endpos, EV_GUN_ALT_RAY3 );
		tent->svFlags |= SVF_BROADCAST;
		VectorCopy( muzzle, tent->s.origin2 );
		// now go along the trail and make sight events
		VectorSubtract( tr.endpos, muzzle, dir );
		shotDist = VectorNormalize( dir );
		//FIXME: if shoot *really* close to someone, the alert could be way out of their FOV
		for ( dist = 0; dist < shotDist; dist += 64 )
		{
			//FIXME: on a really long shot, this could make a LOT of alerts in one frame...
			VectorMA( muzzle, dist, dir, spot );
			AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
			//FIXME: creates *way* too many effects, make it one effect somehow?
		}
		//FIXME: spawn a temp ent that continuously spawns sight alerts here?  And 1 sound alert to draw their attention?
		VectorMA( start, shotDist-4, forwardVec, spot );
		AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );		
	}
	else 
	{
			// DRAW THE BEAM 
		//just draw one beam all the way to the end
//		tent = G_TempEntity( tr.endpos, EV_GUN_RAY3 );
		tent->svFlags |= SVF_BROADCAST;
		VectorCopy( muzzle, tent->s.origin2 );
		// now go along the trail and make sight events
		VectorSubtract( tr.endpos, muzzle, dir );
		shotDist = VectorNormalize( dir );
		//FIXME: if shoot *really* close to someone, the alert could be way out of their FOV
		for ( dist = 0; dist < shotDist; dist += 64 )
		{
			//FIXME: on a really long shot, this could make a LOT of alerts in one frame...
			VectorMA( muzzle, dist, dir, spot );
			AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
			//FIXME: creates *way* too many effects, make it one effect somehow?
		}
		//FIXME: spawn a temp ent that continuously spawns sight alerts here?  And 1 sound alert to draw their attention?
		VectorMA( start, shotDist-4, forwardVec, spot );
		AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );	
	}

}


// ----------------------------------------------
// MULTIPLE SHOOT LOGIC FLECHETTE MAIN 
// ----------------------------------------------------
void WP_FireMultiple( gentity_t *ent, qboolean alt_fire )
{

	gunData_t *gun = NULL; 

	// CUSTOMIZE MUZZLE EFFECT 
	if ( alt_fire )
	{
		// Read Alt Muzzle Shot of Pistol from an External Library. 
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
	
	}
	else 
	{
		// Read Muzzle Shot of Pistol from an Exteral Library! 
		if ( ent->client->gs.gun->Gun_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
	}

// BALISTICA 
	if ( alt_fire ) 
	{

		vec3_t		fwd, angs, start;
		gentity_t	*missile;
		float		damage = ent->client->gs.gun->Gun_AltDamage, vel = ent->client->gs.gun->Gun_Alt_Speed;
		VectorCopy( muzzle, start );
		WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
		// CUSTOMIZE DAMAGE 
		// Do the damages
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_AltDamage )
			{
				damage = ent->client->gs.gun->Gun_AltDamage;
			}
		}

			for ( int i = 0; i < ent->client->gs.gun->Gun_Alt_Num_Shot; i++ )
			{
				vectoangles( forwardVec, angs );
				if ( i == 0 && ent->s.number == 0 )
				{
					// do nothing on the first shot for the player, this one will hit the crosshairs
				}
				else
				{
					angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Alt_Spread;
					angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Alt_Spread;
				}
				AngleVectors( angs, fwd, NULL, NULL );
				WP_MissileTargetHint(ent, start, fwd);
				missile = CreateMissile( start, fwd, vel, ent->client->gs.gun->Gun_Alt_Life, ent );
				missile->classname = "bryar_alt_proj";
				missile->s.weapon = WP_BLASTER_PISTOL;
				VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
				VectorScale( missile->maxs, -1, missile->mins );
				missile->damage = damage;
				missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
				// customize alt means of damage 
				if ( ent->client->gs.gun->Gun_Alt_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					if ( ent->client->gs.gun->Gun_AltSplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}
				missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
				// we don't want it to bounce forever
				//missile->bounceCount = Q_irand(1,2);
				//missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
				ent->client->sess.missionStats.shotsFired++;
				// Affect Gravity
				if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
				{
					missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
				}
				if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
				{
					missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
				}
				if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
				{
					missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
				}

			}

	}
	else // MAIN FIRE 
	{
		vec3_t		fwd, angs, start;
		gentity_t	*missile;
		float		damage = ent->client->gs.gun->Gun_Damage, vel = ent->client->gs.gun->Gun_Speed;
		VectorCopy( muzzle, start );
		WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
		// CUSTOMIZE DAMAGE 
		// Do the damages
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_Damage )
			{
				damage = ent->client->gs.gun->Gun_Damage;
			}
		}

			for ( int i = 0; i < ent->client->gs.gun->Gun_Num_Shot; i++ )
			{
				vectoangles( forwardVec, angs );
				if ( i == 0 && ent->s.number == 0 )
				{
					// do nothing on the first shot for the player, this one will hit the crosshairs
				}
				else
				{
					angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Spread;
					angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Spread;
				}
				AngleVectors( angs, fwd, NULL, NULL );
				WP_MissileTargetHint(ent, start, fwd);
				missile = CreateMissile( start, fwd, vel, ent->client->gs.gun->Gun_Life, ent );
				missile->classname = "bryar_proj";
				missile->s.weapon = WP_BLASTER_PISTOL;
				VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
				VectorScale( missile->maxs, -1, missile->mins );
				missile->damage = damage;
				missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
				// customize means of damage 
				if ( ent->client->gs.gun->Gun_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
					if ( ent->client->gs.gun->Gun_SplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}
				missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
				// we don't want it to bounce forever
				//missile->bounceCount = Q_irand(1,2);
				//missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
				{
					missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
				}
				if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
				{
					missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
				}
				if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
				{
					missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
				}
			}
	}
}


// ----------------------------------------
// FLECHETTE MAIN SHRAPNEL SHOOT LOGIC 
// --------------------------------------------
void WP_FireShrapnel( gentity_t *ent, qboolean alt_fire )
{
	vec3_t		fwd, angs, start;
	gentity_t	*missile;
	float		damage = weaponData[WP_BLASTER_PISTOL].damage, vel = ent->client->gs.gun->Gun_Speed;
	//damage = weaponData[WP_CANNON].damage, vel = CANNON_VEL;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	if ( ent->alt_fire )// alt fire customization 
	{
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
		// If we aren't the player, we will cut the velocity and damage of the shots
		if ( ent->s.number ) // NPC give a cut of damage 
		{
			damage *= 0.75f;
			vel *= 0.5f;
		}

		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_AltDamage )
			{
				damage = ent->client->gs.gun->Gun_AltDamage;
			}
		}

		for ( int i = 0; i < ent->client->gs.gun->Gun_Alt_Num_Shot; i++ )
		{
			vectoangles( forwardVec, angs );

			if ( i == 0 && ent->s.number == 0 )
			{
				// do nothing on the first shot for the player, this one will hit the crosshairs
			}
			else
			{ // default spread is 4 
				angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Alt_Spread;
				angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Alt_Spread;
			}

			AngleVectors( angs, fwd, NULL, NULL );

			WP_MissileTargetHint(ent, start, fwd);

			//missile = CreateMissile( start, fwd, vel, 10000, ent );
			missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent );
			missile->classname = "flech_proj";
			missile->s.weapon = WP_BLASTER_PISTOL; 
			// (will be the guntype var at code completed)
			VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
			VectorScale( missile->maxs, -1, missile->mins );

			missile->damage = damage;

			missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
		
			missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

			// we don't want it to bounce forever
			//missile->bounceCount = Q_irand(1,2); // not possible to customize for shrapnels 
			missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce; 
			missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
			ent->client->sess.missionStats.shotsFired++;
		
			// splashdamage, 
			if ( ent->client->gs.gun->Gun_AltSplashDamage )
			{
				missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;
			}
			// splashradius 
			if ( ent->client->gs.gun->Gun_AltSplashRadius )
			{
				missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;
			}
			// MOD
			if ( ent->client->gs.gun->Gun_Alt_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
			}
			// Gravity variables 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		}
	}
	else // main fire customization
	{
		if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
	
		// If we aren't the player, we will cut the velocity and damage of the shots
		if ( ent->s.number )
		{
			damage *= 0.75f;
			vel *= 0.5f;
		}
		
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_Damage )
			{
				damage = ent->client->gs.gun->Gun_Damage;
			}
		}
		// default is 6 
		for ( int i = 0; i < ent->client->gs.gun->Gun_Num_Shot; i++ )
		{



			vectoangles( forwardVec, angs );

			if ( i == 0 && ent->s.number == 0 )
			{
				// do nothing on the first shot for the player, this one will hit the crosshairs
			}
			else
			{
				//default spread is 4 
				angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Spread;
				angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Spread;
			}

			AngleVectors( angs, fwd, NULL, NULL );

			WP_MissileTargetHint(ent, start, fwd);

			//missile = CreateMissile( start, fwd, vel, 10000, ent );
			missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent );
			missile->classname = "flech_proj";
			missile->s.weapon = WP_BLASTER_PISTOL;
			
			VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
			VectorScale( missile->maxs, -1, missile->mins );

			missile->damage = damage;

			missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
		
			missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

			// we don't want it to bounce forever
			//missile->bounceCount = Q_irand(1,2);
			// not possible to customize for shrapnel ballistic 
			missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce; 

			missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
			ent->client->sess.missionStats.shotsFired++;
		
			// splashdamage, 
			if ( ent->client->gs.gun->Gun_SplashDamage )
			{
				missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;
			}
			// splashradius 
			if ( ent->client->gs.gun->Gun_SplashRadius )
			{
				missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius;
			}
			// MOD
			if ( ent->client->gs.gun->Gun_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
			}
			// Gravity variables 
			if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		
		}
	}
}

// ----------------------------------------
// FLECHETTE ALT SHRAPNEL SHOOT LOGIC 
// --------------------------------------------

//----------------------------------------------
void WP_gun_alt_blow( gentity_t *ent )
// add me to function.h and cpp. 
//----------------------------------------------
{
	EvaluateTrajectory( &ent->s.pos, level.time, ent->currentOrigin ); // Not sure if this is even necessary, but correct origins are cool?
	//G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_SplashDamage, ent->owner->client->gs.gun->Gun_SplashRadius, NULL, ent->owner->client->gs.gun->Gun_MOD );
	//G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
	if ( ent->alt_fire ) 
	{
		G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_AltSplashDamage, ent->owner->client->gs.gun->Gun_AltSplashRadius, NULL, ent->owner->client->gs.gun->Gun_Alt_MOD );
		G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
	}
	else 
	{
		G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_SplashDamage, ent->owner->client->gs.gun->Gun_SplashRadius, NULL, ent->owner->client->gs.gun->Gun_MOD );
		G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
	}
	G_FreeEntity( ent );
}

//------------------------------------------------------------------------------
static void WP_CreateShrapnelBouncyThing( vec3_t start, vec3_t fwd, gentity_t *ent )
//------------------------------------------------------------------------------
{
	// da aggiustar ancora:  
	// alt bounce 
	// missile efx
	// bounce efx 
	// explosion efx 

	if ( ent->alt_fire ) 
	{
		gentity_t	*missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Alt_Speed + random() * 700, ent->client->gs.gun->Gun_Alt_Life + random() * 2000, ent, qtrue );
//		missile->e_ThinkFunc = thinkF_WP_gun_alt_blow;
		missile->s.weapon = WP_BLASTER_PISTOL;
		missile->classname = "flech_alt";
		missile->mass = 4;
		// How 'bout we give this thing a size...
		VectorSet( missile->mins, -3.0f, -3.0f, -3.0f );
		VectorSet( missile->maxs, 3.0f, 3.0f, 3.0f );
		missile->clipmask = MASK_SHOT;
		missile->clipmask &= ~CONTENTS_CORPSE;
		//missile->s.eFlags |= EF_BOUNCE_HALF;
		missile->dflags = 0;	
		missile->svFlags = SVF_USE_CURRENT_ORIGIN;
		VectorCopy( start, missile->pos2 );
		// set damage 
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
				{
					missile->damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
				{
					missile->damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
				{
					missile->damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_AltDamage )
			{
				missile->damage = ent->client->gs.gun->Gun_AltDamage;
			}
		}

		// normal ones bounce, alt ones explode on impact
		// Gravity variables 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_AltSplashDamage )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;
		}
		if ( ent->client->gs.gun->Gun_AltSplashRadius )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_AltSplashRadius;
		}
		if ( ent->client->gs.gun->Gun_Alt_MOD ) 
		{
			missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
			missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
		}
	}
	else // Main Fire 
	{
		gentity_t	*missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Speed + random() * 700, ent->client->gs.gun->Gun_Life + random() * 2000, ent, qtrue );
//		missile->e_ThinkFunc = thinkF_WP_gun_alt_blow;
		missile->s.weapon = WP_BLASTER_PISTOL;
		missile->classname = "flech_alt";
		missile->mass = 4;
		// How 'bout we give this thing a size...
		VectorSet( missile->mins, -3.0f, -3.0f, -3.0f );
		VectorSet( missile->maxs, 3.0f, 3.0f, 3.0f );
		missile->clipmask = MASK_SHOT;
		missile->clipmask &= ~CONTENTS_CORPSE;
		//missile->s.eFlags |= EF_BOUNCE_HALF;// - this infamous flag crash. i not know why! -.-
		missile->dflags = 0;	
		missile->svFlags = SVF_USE_CURRENT_ORIGIN;
		VectorCopy( start, missile->pos2 );

		// set damage 
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
				{
					missile->damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
				{
					missile->damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
				{
					missile->damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_Damage )
			{
				missile->damage = ent->client->gs.gun->Gun_Damage;
			}
		}

		// normal ones bounce, alt ones explode on impact
		// Gravity variables 
		if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_SplashDamage )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;
		}
		if ( ent->client->gs.gun->Gun_SplashRadius )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_SplashRadius;
		}
		
		if ( ent->client->gs.gun->Gun_MOD ) 
		{
			missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
			missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
		}
	}
}


void WP_FireTimeBomb( gentity_t *ent, qboolean alt_fire )
{
	vec3_t 	dir, fwd, start, angs;
	vectoangles( forwardVec, angs );
	VectorCopy( muzzle, start );

	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	if ( ent->alt_fire )// alt fire customization 
	{
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
		for ( int i = 0; i < ent->client->gs.gun->Gun_Alt_Num_Shot; i++ )
		{
			VectorCopy( angs, dir );

			dir[PITCH] -= random() * ent->client->gs.gun->Gun_Alt_Spread; // make it fly upwards
			dir[YAW] += crandom() * ent->client->gs.gun->Gun_Alt_Spread;
			AngleVectors( dir, fwd, NULL, NULL );
	
			WP_CreateShrapnelBouncyThing( start, fwd, ent );
			ent->client->sess.missionStats.shotsFired++;
		}
	}
	else // main fire 
	{
		if ( ent->client->gs.gun->Gun_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
		
		for ( int i = 0;  i < ent->client->gs.gun->Gun_Num_Shot; i++ )
		{
			VectorCopy( angs, dir );

			dir[PITCH] -= random() * ent->client->gs.gun->Gun_Spread; // make it fly upwards
			dir[YAW] += crandom() * ent->client->gs.gun->Gun_Spread;
			AngleVectors( dir, fwd, NULL, NULL );
	
			WP_CreateShrapnelBouncyThing( start, fwd, ent );
			ent->client->sess.missionStats.shotsFired++;
		}

	}

	
}



// ------------------------------------------------
// BOWCASTER LOGIC: DIVIDE SHOOT 
// --------------------------------------------------
void WP_FireDivide( gentity_t *ent, qboolean alt_fire )
{
	
	int			damage	= weaponData[WP_BLASTER_PISTOL].damage, count;
	float		vel;
	vec3_t		angs, dir, start;
	gentity_t	*missile;
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	gunData_t *gun = NULL; 
	// CUSTOMIZE MUZZLE EFFECT 
	if ( alt_fire )
	{
		// Read Alt Muzzle Shot of Pistol from an External Library. 
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
	
	}
	else 
	{
		// Read Muzzle Shot of Pistol from an Exteral Library! 
		if ( ent->client->gs.gun->Gun_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}

	}
	
	// BALLISTIC CODE 
if ( alt_fire ) // ALT FIRE 
{
	// FIRST OFF, CUSTOMIZE THE DAMAGE. 
	// Do the damages
	if ( ent->s.number != 0 ) // NPC 
	{
		if ( g_spskill->integer == 0 )
		{ 
			if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
			{
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
			}
		}
		else if ( g_spskill->integer == 1 )
		{
			if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
			{
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
			}
		}
		else
		{
			if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
			{
				damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
			}
		}
	}
	else // PLAYA 
	{
		if ( ent->client->gs.gun->Gun_AltDamage )
		{
			damage = ent->client->gs.gun->Gun_AltDamage;
		}
	}
	// DIVIDE THE SHOOT! 
	count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Alt_Charge_Unit;
	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 5 )
	{ // Shot how many bolt are setted into GUN file 
		count = ent->client->gs.gun->Gun_Alt_Num_Shot;
	}
	if ( !(count & 1 ))
	{
		// if we aren't odd, knock us down a level
		count--;
	}
	WP_MissileTargetHint(ent, start, forwardVec);
	for ( int i = 0; i < count; i++ )
	{
		// create a range of different velocities
		vel = ent->client->gs.gun->Gun_Alt_Speed * ( crandom() * BOWCASTER_VEL_RANGE + 1.0f );
		vectoangles( forwardVec, angs );
		if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// add some slop to the fire direction
			// How many bolts are shooted?
			angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Alt_Num_Shot * 0.2f;
			angs[YAW]	+= ((i+0.5f) * ent->client->gs.gun->Gun_Alt_Num_Shot - count * 0.5f * ent->client->gs.gun->Gun_Alt_Num_Shot );
			if ( ent->NPC )
			{
				angs[PITCH] += ( crandom() * (ent->client->gs.gun->Gun_Alt_Num_Shot+(6-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * (ent->client->gs.gun->Gun_Alt_Num_Shot+(6-ent->NPC->currentAim)*0.25f) );
			}
		}
		AngleVectors( angs, dir, NULL, NULL );
		missile = CreateMissile( start, dir, vel, 10000, ent );
		missile->classname = "bryar_proj";
		missile->s.weapon = WP_BLASTER_PISTOL;
		VectorSet( missile->maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );
		missile->damage = damage;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
		// Set Alt Mod Damages 
		if ( ent->client->gs.gun->Gun_Alt_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					if ( ent->client->gs.gun->Gun_AltSplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
		missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;
		missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;
		// we don't want it to bounce
		ent->client->sess.missionStats.shotsFired++;
		// ADD GRAVITY EFFECT 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}

	}
}
else // MAIN FIRE 
{
	// FIRST OFF, CUSTOMIZE THE DAMAGE. 
	// Do the damages
	if ( ent->s.number != 0 ) // NPC 
	{
		if ( g_spskill->integer == 0 )
		{ 
			if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
			}
		}
		else if ( g_spskill->integer == 1 )
		{
			if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
			}
		}
		else
		{
			if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
			{
				damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
			}
		}
	}
	else // PLAYA 
	{
		if ( ent->client->gs.gun->Gun_Damage )
		{
			damage = ent->client->gs.gun->Gun_Damage;
		}
	}

	// DIVIDE THE SHOOT! 
	count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Charge_Unit;
	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 5 )
	{
		count = ent->client->gs.gun->Gun_Num_Shot;
	}
	if ( !(count & 1 ))
	{
		// if we aren't odd, knock us down a level
		count--;
	}
	WP_MissileTargetHint(ent, start, forwardVec);
	for ( int i = 0; i < count; i++ )
	{
		// create a range of different velocities
		vel = ent->client->gs.gun->Gun_Speed * ( crandom() * BOWCASTER_VEL_RANGE + 1.0f );
		vectoangles( forwardVec, angs );
		if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// add some slop to the fire direction
			// How many bolts are shooted?
			angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Num_Shot * 0.2f;
			angs[YAW]	+= ((i+0.5f) * BOWCASTER_ALT_SPREAD - count * 0.5f * ent->client->gs.gun->Gun_Num_Shot );
			if ( ent->NPC )
			{
				angs[PITCH] += ( crandom() * (ent->client->gs.gun->Gun_Num_Shot+(6-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * (ent->client->gs.gun->Gun_Num_Shot+(6-ent->NPC->currentAim)*0.25f) );
			}
		}

		AngleVectors( angs, dir, NULL, NULL );
		missile = CreateMissile( start, dir, vel, 10000, ent );
		missile->classname = "bryar_proj";
		missile->s.weapon = WP_BLASTER_PISTOL;
		VectorSet( missile->maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );
		missile->damage = damage;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
		// Set Alt Mod Damages 
		if ( ent->client->gs.gun->Gun_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
					if ( ent->client->gs.gun->Gun_SplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
		// Set Splash Damages 
		missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;
		missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius;
		// we don't want it to bounce
		ent->client->sess.missionStats.shotsFired++;
		// ADD GRAVITY EFFECT 
		if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
	}
}

}

// -------------------------------------------
// BOWCASTER ALT LOGIC: BOUNCE SHOOT
// ------------------------------------------
void WP_FireBounce( gentity_t *ent, qboolean alt_fire )
{
	vec3_t		fwd, angs, start;
	gentity_t	*missile;
	float		damage = weaponData[WP_BLASTER_PISTOL].damage, vel = ent->client->gs.gun->Gun_Speed;
	//damage = weaponData[WP_CANNON].damage, vel = CANNON_VEL;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	if ( ent->alt_fire )// alt fire customization 
	{
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
			{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
		// If we aren't the player, we will cut the velocity and damage of the shots
		if ( ent->s.number ) // NPC give a cut of damage 
		{
			damage *= 0.75f;
			vel *= 0.5f;
		}

		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
				{
					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_AltDamage )
			{
				damage = ent->client->gs.gun->Gun_AltDamage;
			}
		}

		for ( int i = 0; i < ent->client->gs.gun->Gun_Alt_Num_Shot; i++ )
		{
			vectoangles( forwardVec, angs );

			if ( i == 0 && ent->s.number == 0 )
			{
				// do nothing on the first shot for the player, this one will hit the crosshairs
			}
			else
			{ // default spread is 4 
				angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Alt_Spread;
				angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Alt_Spread;
			}

			AngleVectors( angs, fwd, NULL, NULL );

			WP_MissileTargetHint(ent, start, fwd);

			//missile = CreateMissile( start, fwd, vel, 10000, ent );
			missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent );
			missile->classname = "flech_proj";
			missile->s.weapon = WP_BLASTER_PISTOL; 
			// (will be the guntype var at code completed)
			VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
			VectorScale( missile->maxs, -1, missile->mins );

			missile->damage = damage;

			missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
		
			missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

			// we don't want it to bounce forever
			//missile->bounceCount = Q_irand(1,2); // not possible to customize for shrapnels 
			missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce; 
			missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
			ent->client->sess.missionStats.shotsFired++;
		
			// splashdamage, 
			if ( ent->client->gs.gun->Gun_AltSplashDamage )
			{
				missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;
			}
			// splashradius 
			if ( ent->client->gs.gun->Gun_AltSplashRadius )
			{
				missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;
			}
			// MOD
			if ( ent->client->gs.gun->Gun_Alt_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
			}
			// Gravity variables 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		}
	}
	else // main fire customization
	{
		if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
				//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
	
		// If we aren't the player, we will cut the velocity and damage of the shots
		if ( ent->s.number )
		{
			damage *= 0.75f;
			vel *= 0.5f;
		}
		
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
				{
					damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_Damage )
			{
				damage = ent->client->gs.gun->Gun_Damage;
			}
		}
		// default is 6 
		for ( int i = 0; i < ent->client->gs.gun->Gun_Num_Shot; i++ )
		{



			vectoangles( forwardVec, angs );

			if ( i == 0 && ent->s.number == 0 )
			{
				// do nothing on the first shot for the player, this one will hit the crosshairs
			}
			else
			{
				//default spread is 4 
				angs[PITCH] += crandom() * ent->client->gs.gun->Gun_Spread;
				angs[YAW]	+= crandom() * ent->client->gs.gun->Gun_Spread;
			}

			AngleVectors( angs, fwd, NULL, NULL );

			WP_MissileTargetHint(ent, start, fwd);

			//missile = CreateMissile( start, fwd, vel, 10000, ent );
			missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent );
			missile->classname = "flech_proj";
			missile->s.weapon = WP_BLASTER_PISTOL;
			
			VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
			VectorScale( missile->maxs, -1, missile->mins );

			missile->damage = damage;

			missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
		
			missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

			// we don't want it to bounce forever
			//missile->bounceCount = Q_irand(1,2);
			// not possible to customize for shrapnel ballistic 
			missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce; 

			missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
			ent->client->sess.missionStats.shotsFired++;
		
			// splashdamage, 
			if ( ent->client->gs.gun->Gun_SplashDamage )
			{
				missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;
			}
			// splashradius 
			if ( ent->client->gs.gun->Gun_SplashRadius )
			{
				missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius;
			}
			// MOD
			if ( ent->client->gs.gun->Gun_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
			}
			// Gravity variables 
			if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		
		}
	}
}
// ----------------------------------------------
//  ROCKET LOGIC 
// ---------------------------------------------------
void WP_FireAim( gentity_t *ent, qboolean alt_fire )
{
	gunData_t	*gun = NULL; 
	vec3_t	start;
	int		damage	= weaponData[WP_ROCKET_LAUNCHER].damage;
	// CUSTOMIZE BALLISTIC 
	if ( alt_fire ) 
	{
		float	vel = ent->client->gs.gun->Gun_Alt_Speed;
		VectorCopy( muzzle, start );
		WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
		gentity_t *missile = CreateMissile( start, forwardVec, vel, ent->client->gs.gun->Gun_Alt_Life, ent, alt_fire );
		missile->classname = "bryar_proj";
		missile->s.weapon = WP_BLASTER_PISTOL;
		missile->mass = 10;
		// Make it easier to hit things
		VectorSet( missile->maxs, ROCKET_SIZE, ROCKET_SIZE, ROCKET_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );
		missile->damage = damage;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
		// customize means of damage 
		if ( ent->client->gs.gun->Gun_Alt_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
				if ( ent->client->gs.gun->Gun_AltSplashDamage )
				{
					missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
				
				}
			}
		missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;
		missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;
		// we don't want it to ever bounce
		missile->bounceCount = 0;
// Set Alt Mod Damages 
		if ( ent->client->gs.gun->Gun_Alt_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					if ( ent->client->gs.gun->Gun_AltSplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}	
		int	lockEntNum, lockTime;
		if ( ent->NPC && ent->enemy )
		{
			lockEntNum = ent->enemy->s.number;
			lockTime = Q_irand( 600, 1200 );
		}
		else
		{
			lockEntNum = g_rocketLockEntNum;
			lockTime = g_rocketLockTime;
		}
		// we'll consider attempting to lock this little poochie onto some baddie.
		if ( (lockEntNum > 0 || (ent->NPC && lockEntNum >= 0)) && lockEntNum < ENTITYNUM_WORLD && lockTime > 0 )
		{
			// take our current lock time and divide that by 8 wedge slices to get the current lock amount
			int dif = ( level.time - lockTime ) / ( 1200.0f / 8.0f );

			if ( dif < 0 )
			{
				dif = 0;
			}
			else if ( dif > 8 )
			{
				dif = 8;
			}

			// if we are fully locked, always take on the enemy.  
			//	Also give a slight advantage to higher, but not quite full charges.  
			//	Finally, just give any amount of charge a very slight random chance of locking.
			if ( dif == 8 || random() * dif > 2 || random() > 0.97f )
			{
				missile->enemy = &g_entities[lockEntNum];

				if ( missile->enemy 
					&& missile->enemy->inuse )//&& DistanceSquared( missile->currentOrigin, missile->enemy->currentOrigin ) < 262144 && InFOV( missile->currentOrigin, missile->enemy->currentOrigin, missile->enemy->client->ps.viewangles, 45, 45 ) )
				{
					if ( missile->enemy->client
						&& (missile->enemy->client->ps.forcePowersKnown&(1<<FP_PUSH))
						&& missile->enemy->client->ps.forcePowerLevel[FP_PUSH] > FORCE_LEVEL_0 )
					{//have force push, don't flee from homing rockets
					}
					else
					{
						vec3_t dir, dir2;

						AngleVectors( missile->enemy->currentAngles, dir, NULL, NULL );
						AngleVectors( ent->client->renderInfo.eyeAngles, dir2, NULL, NULL );

						if ( DotProduct( dir, dir2 ) < 0.0f )
						{
							G_StartFlee( missile->enemy, ent, missile->enemy->currentOrigin, AEL_DANGER_GREAT, 3000, 5000 );
							if ( !TIMER_Done( missile->enemy, "flee" ) )
							{
								TIMER_Set( missile->enemy, "rocketChasing", 500 );
							}
						}
					}
				}
			}
		}
		VectorCopy( forwardVec, missile->movedir );
		missile->e_ThinkFunc = thinkF_rocketThink;
		missile->random = 1.0f;
		missile->nextthink = level.time + ROCKET_ALT_THINK_TIME;
	
		// ALT DAMAGE 
		// Do the damages
		// NPCS 
		if ( ent->s.number != 0 )
		{
			if ( g_spskill->integer == 0 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
				{

					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
				{

					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
				{

					damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
				}

			}
		}
		else // Playa 
		{
			if ( ent->client->gs.gun->Gun_AltDamage )
				{

					damage = ent->client->gs.gun->Gun_AltDamage;
				}

		}
		// ADD CUSTOM GRAVITY 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
		{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
		}
		if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
		{
			missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
		}
		if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
		{
			missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
		}
		// CUSTOMIZE ALT MUZZLE EFFECT 
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint );
		}
	}
	else // MAIN FIRE 
	{
		float	vel = ent->client->gs.gun->Gun_Speed;
		VectorCopy( muzzle, start );
		WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
		gentity_t *missile = CreateMissile( start, forwardVec, vel, ent->client->gs.gun->Gun_Life, ent, alt_fire );
		missile->classname = "bryar_proj";
		missile->s.weapon = WP_BLASTER_PISTOL;
		missile->mass = 10;
		// Make it easier to hit things
		VectorSet( missile->maxs, ROCKET_SIZE, ROCKET_SIZE, ROCKET_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );
		missile->damage = damage;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

		// customize means of damage 
		if ( ent->client->gs.gun->Gun_MOD )
			{
				missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
				if ( ent->client->gs.gun->Gun_SplashDamage )
				{
					missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
				
				}
			}
		missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;
		missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius;
		// we don't want it to ever bounce
		missile->bounceCount = 0;
		int	lockEntNum, lockTime;
		if ( ent->NPC && ent->enemy )
		{
			lockEntNum = ent->enemy->s.number;
			lockTime = Q_irand( 600, 1200 );
		}
		else
		{
			lockEntNum = g_rocketLockEntNum;
			lockTime = g_rocketLockTime;
		}
		// we'll consider attempting to lock this little poochie onto some baddie.
		if ( (lockEntNum > 0 || (ent->NPC && lockEntNum >= 0)) && lockEntNum < ENTITYNUM_WORLD && lockTime > 0 )
		{
			// take our current lock time and divide that by 8 wedge slices to get the current lock amount
			int dif = ( level.time - lockTime ) / ( 1200.0f / 8.0f );

			if ( dif < 0 )
			{
				dif = 0;
			}
			else if ( dif > 8 )
			{
				dif = 8;
			}

			// if we are fully locked, always take on the enemy.  
			//	Also give a slight advantage to higher, but not quite full charges.  
			//	Finally, just give any amount of charge a very slight random chance of locking.
			if ( dif == 8 || random() * dif > 2 || random() > 0.97f )
			{
				missile->enemy = &g_entities[lockEntNum];

				if ( missile->enemy 
					&& missile->enemy->inuse )//&& DistanceSquared( missile->currentOrigin, missile->enemy->currentOrigin ) < 262144 && InFOV( missile->currentOrigin, missile->enemy->currentOrigin, missile->enemy->client->ps.viewangles, 45, 45 ) )
				{
					if ( missile->enemy->client
						&& (missile->enemy->client->ps.forcePowersKnown&(1<<FP_PUSH))
						&& missile->enemy->client->ps.forcePowerLevel[FP_PUSH] > FORCE_LEVEL_0 )
					{//have force push, don't flee from homing rockets
					}
					else
					{
						vec3_t dir, dir2;

						AngleVectors( missile->enemy->currentAngles, dir, NULL, NULL );
						AngleVectors( ent->client->renderInfo.eyeAngles, dir2, NULL, NULL );

						if ( DotProduct( dir, dir2 ) < 0.0f )
						{
							G_StartFlee( missile->enemy, ent, missile->enemy->currentOrigin, AEL_DANGER_GREAT, 3000, 5000 );
							if ( !TIMER_Done( missile->enemy, "flee" ) )
							{
								TIMER_Set( missile->enemy, "rocketChasing", 500 );
							}
						}
					}
				}
			}
		}
		VectorCopy( forwardVec, missile->movedir );
		missile->e_ThinkFunc = thinkF_rocketThink;
		missile->random = 1.0f;
		missile->nextthink = level.time + ROCKET_ALT_THINK_TIME;
	
		// DAMAGE 
		// Do the damages
		// NPCS 
		if ( ent->s.number != 0 )
		{
			if ( g_spskill->integer == 0 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
				{

					damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
				{

					damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
				{

					damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
				}

			}
		}
		else // Playa 
		{
			if ( ent->client->gs.gun->Gun_Damage )
				{

					damage = ent->client->gs.gun->Gun_Damage;
				}

		}
		// ADD CUSTOM GRAVITY 
		if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
		{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
		}
		if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
		{
			missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
		}
		if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
		{
			missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
		}
		// CUSTOMIZE ALT MUZZLE EFFECT 
		if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint );
		}
	}
}

// ---------------------------------------------
// GAS WEAPON LOGIC
// -------------------------------------------
void WP_FireGas( gentity_t *ent, qboolean alt_fire )
{
	vec3_t	dir, angs;
	vectoangles( forwardVec, angs );
	gunData_t *gun = NULL;
	int		damage = 0; //!alt_fire ? weaponData[WP_BLASTER_PISTOL].damage : weaponData[WP_BLASTER_PISTOL].altDamage;
	// Spreading 
	if ( alt_fire ) 
	{ 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_SPREADSHOOT ) 
		{
			if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
			{//force sight 2+ gives perfect aim
				//FIXME: maybe force sight level 3 autoaims some?
				// add some slop to the main-fire direction
				if (ent->NPC)
				{
					angs[PITCH] += ( Q_flrand(-1.0f, 1.0f) * (ent->client->gs.gun->Gun_Alt_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
					angs[YAW]	+= ( Q_flrand(-1.0f, 1.0f) * (ent->client->gs.gun->Gun_Alt_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				}
				else
				{
					// add some slop to the main-fire direction
					angs[PITCH] += Q_flrand(-1.0f, 1.0f) * ent->client->gs.gun->Gun_Alt_Spread;
					angs[YAW]	+= Q_flrand(-1.0f, 1.0f) * ent->client->gs.gun->Gun_Alt_Spread;
				}
			}
		}
	}
	else // Main Fire Spreading 
	{
		if ( ent->client->gs.gun->gunFlags&GFL_SPREADSHOOT ) 
		{
			if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
			{//force sight 2+ gives perfect aim
				//FIXME: maybe force sight level 3 autoaims some?
				// add some slop to the main-fire direction
				if (ent->NPC)
				{
					angs[PITCH] += ( Q_flrand(-1.0f, 1.0f) * (ent->client->gs.gun->Gun_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
					angs[YAW]	+= ( Q_flrand(-1.0f, 1.0f) * (ent->client->gs.gun->Gun_Spread+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				}
				else
				{
					// add some slop to the main-fire direction
					angs[PITCH] += Q_flrand(-1.0f, 1.0f) * ent->client->gs.gun->Gun_Spread;
					angs[YAW]	+= Q_flrand(-1.0f, 1.0f) * ent->client->gs.gun->Gun_Spread;
				}
			}
		}
	}

	AngleVectors( angs, dir, NULL, NULL );

	// MUZZLE EFFECTS 
	if ( alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}
	// BALLISTIC -------------------------------
	// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
	int velocity	= 1200;
	WP_TraceSetStart( ent, muzzle, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	
	if ( alt_fire ) 
	{
		if ( ent->client->gs.gun->Gun_Alt_Speed && ent->client->gs.gun->Gun_Alt_Life )
		{
			// PROJECTILE - DEFAULT PARAMETERS SHOOTS. 
			WP_MissileTargetHint(ent, muzzle, forwardVec);
			// customize speed and duration of alt missile 
			gentity_t	*missile = CreateMissile( muzzle, forwardVec, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent, alt_fire );
			missile->classname = "bryar_alt_proj";
			missile->damage = damage;
			missile->dflags = DAMAGE_NO_KNOCKBACK;
	 		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
			// we don't want it to bounce forever
			// Set Alt Mod Damages 
			if ( ent->client->gs.gun->Gun_Alt_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					if ( ent->client->gs.gun->Gun_AltSplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}
			// ALT DAMAGE CUSTOMIZATION 
			// IF entity is an NPC 
			// If ent is an NPC 
			if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
					}
				}
             }
			 else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_AltDamage;
			}
			// ALT SPLASH DAMAGE AND SPLASH RADIUS!!! 
			// splash damage 
			if ( ent->client->gs.gun->Gun_AltSplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;  
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_AltSplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;  
			}
			// ALT FIRE BALISTIC CODE CUSTOMIZATIONS 
			if ( ent->s.weapon == WP_BLASTER_PISTOL )
			{//*SIGH*... I hate our weapon system...
				missile->s.weapon = ent->s.weapon;
			}
			// BRYAR BALISTIC - ALT FIRE CHARGING SHOOT. 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_CHARGESHOOT ) // Gravity Affect Shoot
			{
				if ( ent->client->gs.gun->Gun_Alt_Charge_Unit ) // amount of charge unit 
				{//*SIGH*... I hate our weapon system...
					int count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Alt_Charge_Unit;
					if ( count < 1 )
					{
						count = 1;
					}
					else if ( count > 5 )
					{
						count = 5;
					}
					damage *= count;
					missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
				}
			}
			// REPEATER 
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
			// FLECHETTE ALT 
		   if ( ent->client->gs.gun->gunFlags&GFL_ALT_SHRAPNELSHOOT ) // alt shrapnel of flechette enabled  
			{
				gi.Printf( S_COLOR_GREEN "Shooting Alt Shrapnel Shoot!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Alt_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce;
				}
			}
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_BOUNCEHALFSHOOT ) // half bounce enabled   
			{
				gi.Printf( S_COLOR_GREEN "Shooting Alt Half bounce!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_HALF;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Alt_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce;
				}
			}
			// ALT BOUNCE SHOOT
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_BOUNCESHOOT ) // alt bounce of bowcaster alt enabled  
			{
				gi.Printf( S_COLOR_GREEN "Shooting a Alt Bouncing Shoot!\n" , ent );
				//ent->client->sess.missionStats.shotsFired++;
				// NON RIESCO A CAPIRE PERCHE' CRASHA NELL'ALTFIRE!!! 
				//missile->s.eFlags |= EF_BOUNCE;
				if ( ent->client->gs.gun->Gun_Alt_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Alt_Num_Bounce;
				}
			}
		}
	}
	else // MAIN FIRE 
	{
		if ( ent->client->gs.gun->Gun_Speed && ent->client->gs.gun->Gun_Life )
		{
			// PROJECTILE - DEFAULT PARAMETERS SHOOTS. 
			WP_MissileTargetHint(ent, muzzle, forwardVec);
			// customize speed and duration of alt missile 
			gentity_t	*missile = CreateMissile( muzzle, forwardVec, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent, alt_fire );
			missile->classname = "bryar_proj";
			missile->damage = damage;
			missile->dflags = DAMAGE_NO_KNOCKBACK;
	 		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
			// we don't want it to bounce forever
			missile->bounceCount = 0;
			// Set  Damages 
			if ( ent->client->gs.gun->Gun_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
					if ( ent->client->gs.gun->Gun_SplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}



			// MAIN DAMAGE CUSTOMIZATION 
			// IF entity is an NPC 
			// If ent is an NPC 
			if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
					{
						damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
					}
				}
             }
			else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_Damage;
			}
			// main splash damage 
			if ( ent->client->gs.gun->Gun_SplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage; 
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_SplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius; 
			}
			// MAIN FIRE BALISTIC CUSTOMIZATION 
			if ( ent->s.weapon == WP_BLASTER_PISTOL )
			{//*SIGH*... I hate our weapon system...
				missile->s.weapon = ent->s.weapon;
			}
			// BRYAR BALISTIC - FIRE CHARGING SHOOT AMOUNT 
			if ( ent->client->gs.gun->gunFlags&GFL_CHARGESHOOT ) // Gravity Affect Shoot
			{
				if ( ent->client->gs.gun->Gun_Charge_Unit ) // amount of charge unit 
				{//*SIGH*... I hate our weapon system...
					int count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Charge_Unit;
					if ( count < 1 )
					{
						count = 1;
					}
					else if ( count > 5 )
					{
						count = 5;
					}
					damage *= count;
					missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
				}
			}
			// REPEATER 
			if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Main Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
			// FLECHETTE 
			if ( ent->client->gs.gun->gunFlags&GFL_SHRAPNELSHOOT ) // shrapnel of flechette enabled  
			{
				gi.Printf( S_COLOR_GREEN "Shooting Shrapnel Shoot!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce;
				}
			}
			if ( ent->client->gs.gun->gunFlags&GFL_BOUNCEHALFSHOOT ) // half bounce enabled   
			{
				gi.Printf( S_COLOR_GREEN "Shooting half bounce!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE_HALF;
				ent->client->sess.missionStats.shotsFired++;
				if ( ent->client->gs.gun->Gun_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce;
				}
			}
			// BOWCASTER
			if ( ent->client->gs.gun->gunFlags&GFL_BOUNCESHOOT ) // colpo rimbalzato 
			{
				gi.Printf( S_COLOR_GREEN "Shooting Bouncing Shoot!\n" , ent );
				missile->s.eFlags |= EF_BOUNCE;
				if ( ent->client->gs.gun->Gun_Num_Bounce )
				{
					gi.Printf( S_COLOR_RED "I Am bouncing?" , ent ); 
					missile->bounceCount = ent->client->gs.gun->Gun_Num_Bounce;
				}
			}
			
		}
	}

	
	
	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}


//---------------------------------------
// AOE WEAPON LOGIC 
//-----------------------------
void FireAoe_Detonate( gentity_t *ent )
//---------------------------------------------------------
{

	gi.Printf( S_COLOR_GREEN "FireAoe Detonate In esecuzione\n" , ent );
	G_SetOrigin( ent, ent->currentOrigin );

	// start the effects, unfortunately, I wanted to do some custom things that I couldn't easily do with the fx system, so part of it uses an event and localEntities
	if ( ent->owner->alt_fire ) 
	{
		if ( ent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect )
		{
			G_PlayEffect( ent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect, ent->currentOrigin, ent->pos1 );
		}
	}
	else 
	{
		if ( ent->owner->client->gs.gun->Gun_WallImpact_Effect )
		{
			G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin, ent->pos1 );
	    }
	}

	G_AddEvent( ent, EV_DEMP2_ALT_IMPACT, ent->count * 2 );
	ent->fx_time = level.time;
	ent->radius = 0;
	ent->nextthink = level.time + 50;
	ent->e_ThinkFunc = thinkF_DEMP2_AltRadiusDamage;
	ent->s.eType = ET_GENERAL; // make us a missile no longer

	// PASTED FROM DEMP2ALTRADIUSDAMAGE ThinkFunc
	float		frac = ( level.time - ent->fx_time ) / 1300.0f; // synchronize with demp2 effect
	float		dist, radius;
	gentity_t	*gent;
	gentity_t	*entityList[MAX_GENTITIES];
	int			numListedEntities, i, e;
	vec3_t		mins, maxs;
	vec3_t		v, dir;

	frac *= frac * frac; // yes, this is completely ridiculous...but it causes the shell to grow slowly then "explode" at the end
	
	radius = frac * 200.0f; // 200 is max radius...the model is aprox. 100 units tall...the fx draw code mults. this by 2.

	for ( i = 0 ; i < 3 ; i++ ) 
	{
		mins[i] = ent->currentOrigin[i] - radius;
		maxs[i] = ent->currentOrigin[i] + radius;
	}

	numListedEntities = gi.EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) 
	{
		gent = entityList[ e ];

		if ( !gent->takedamage || !gent->contents )
		{
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			if ( ent->currentOrigin[i] < gent->absmin[i] ) 
			{
				v[i] = gent->absmin[i] - ent->currentOrigin[i];
			} 
			else if ( ent->currentOrigin[i] > gent->absmax[i] ) 
			{
				v[i] = ent->currentOrigin[i] - gent->absmax[i];
			} 
			else 
			{
				v[i] = 0;
			}
		}

		// shape is an ellipsoid, so cut vertical distance in half`
		v[2] *= 0.5f;

		dist = VectorLength( v );

		if ( dist >= radius ) 
		{
			// shockwave hasn't hit them yet
			continue;
		}

		if ( dist < ent->radius )
		{
			// shockwave has already hit this thing...
			continue;
		}

		VectorCopy( gent->currentOrigin, v );
		VectorSubtract( v, ent->currentOrigin, dir);

		// push the center of mass higher than the origin so players get knocked into the air more
		dir[2] += 12;
		if ( ent->owner->alt_fire ) 
		{
			gi.Printf( S_COLOR_GREEN "Doing the Alt Damage?\n" , ent );
			G_Damage( gent, ent, ent->owner, dir, ent->currentOrigin, ent->owner->client->gs.gun->Gun_AltDamage, DAMAGE_DEATH_KNOCKBACK, ent->splashMethodOfDeath );
		}
		else 
		{	gi.Printf( S_COLOR_GREEN "Doing the damage?\n" , ent );
			G_Damage( gent, ent, ent->owner, dir, ent->currentOrigin, ent->owner->client->gs.gun->Gun_Damage, DAMAGE_DEATH_KNOCKBACK, ent->splashMethodOfDeath );
		}

		if ( gent->takedamage && gent->client ) 
		{
			/*gent->s.powerups |= ( 1 << PW_SHOCKED );
			gent->client->ps.powerups[PW_SHOCKED] = level.time + 2000;*/
			Saboteur_Decloak( gent, Q_irand( 3000, 10000 ) );
		}
	}

	// store the last fraction so that next time around we can test against those things that fall between that last point and where the current shockwave edge is
	ent->radius = radius;

	if ( frac < 1.0f )
	{
		// shock is still happening so continue letting it expand
		ent->nextthink = level.time + 50;
	}
}

void WP_FireAoe( gentity_t *ent, qboolean alt_fire )
{
	int		damage	= weaponData[WP_BRYAR_PISTOL].altDamage;
	int		count;
	vec3_t	start;
	trace_t	tr;
	gunData_t *gun = NULL; 

	// Muzzle Effects
	// MUZZLE EFFECTS 
if ( ent->alt_fire )
{
	// Read Alt Muzzle Shot of Pistol from an External Library. 
	if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Alt Muzzle Effect from *.GUN File! None = gun/altmuzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}
else 
{
	// Read Muzzle Shot of Pistol from an Exteral Library! 
	if ( ent->client->gs.gun->Gun_Muzzle_Effect )
		{
			//gi.Printf( S_COLOR_GREEN "Customize Muzzle Effect from *.GUN File! None = gun/muzzle_flash" , ent );
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}

}


	// BALLISTIC 
if ( alt_fire ) 
{
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Alt_Charge_Unit;
	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 3 )
	{
		count = 3;
	}
	damage *= ( 1 + ( count * ( count - 1 )));// yields damage of 12,36,84...gives a higher bonus for longer charge
	// the shot can travel a whopping 4096 units in 1 second. Note that the shot will auto-detonate at 4096 units...we'll see if this looks cool or not
	
	
	WP_MissileTargetHint(ent, start, forwardVec);
	gentity_t *missile = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent, qtrue );
	// letting it know what the charge size is.
	missile->count = count;
//	missile->speed = missile->nextthink;
	VectorCopy( tr.plane.normal, missile->pos1 );
	missile->classname = "bryar_proj";
	missile->s.weapon = WP_BLASTER_PISTOL;
	// Set Alt Mod Damages 
		if ( ent->client->gs.gun->Gun_Alt_MOD )
				{
					missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
					if ( ent->client->gs.gun->Gun_AltSplashDamage )
					{
						missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
						//missile->methodOfDeath = MOD_BRYAR_ALT;
					}
				}

	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	// we don't want it to ever bounce
	missile->bounceCount = 0;
	if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
					{ 
						damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
					}
				}
             }
			 else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_AltDamage;
			}
			// ALT SPLASH DAMAGE AND SPLASH RADIUS!!! 
			// splash damage 
			if ( ent->client->gs.gun->Gun_AltSplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;  
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_AltSplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius;  
			}
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
}
else // main fire 
{
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	count = ( level.time - ent->client->ps.weaponChargeTime ) / ent->client->gs.gun->Gun_Charge_Unit;
	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 3 )
	{
		count = 3;
	}
	damage *= ( 1 + ( count * ( count - 1 )));// yields damage of 12,36,84...gives a higher bonus for longer charge
	// the shot can travel a whopping 4096 units in 1 second. Note that the shot will auto-detonate at 4096 units...we'll see if this looks cool or not
	WP_MissileTargetHint(ent, start, forwardVec);
	gentity_t *missile = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent, qfalse );
	// letting it know what the charge size is.
	missile->count = count;
//	missile->speed = missile->nextthink;
	VectorCopy( tr.plane.normal, missile->pos1 );
	missile->classname = "bryar_proj";
	missile->s.weapon = WP_BLASTER_PISTOL;
	// Set Alt Mod Damages 
	if ( ent->client->gs.gun->Gun_MOD )
	{
		missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
		if ( ent->client->gs.gun->Gun_SplashDamage )
			{
				missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
				//missile->methodOfDeath = MOD_BRYAR_ALT;
			}
	}
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	// we don't want it to ever bounce
	missile->bounceCount = 0;
	if ( ent->s.number != 0 )
			{
				// on level easy 
				if ( g_spskill->integer == 0 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
					}
				}
				else if ( g_spskill->integer == 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
					}
				}
				else if ( g_spskill->integer > 1 )
				{
					if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
					{ 
						damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
					}
				}
             }
			 else // set playa damage 
			{
					damage = ent->client->gs.gun->Gun_Damage;
			}
			// ALT SPLASH DAMAGE AND SPLASH RADIUS!!! 
			// splash damage 
			if ( ent->client->gs.gun->Gun_SplashDamage ) // shockwave?
			{ // set damage 
				missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;  
			}
			// splash radius 
			if ( ent->client->gs.gun->Gun_SplashRadius ) // radius of shockwave
			{ // set radius  
				missile->splashRadius = ent->client->gs.gun->Gun_SplashRadius;  
			}
			if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
			if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
}

}

// ------------------------------------------------------
// SCEPTER WEAPON LOGIC
// -----------------------------------------------------



////////////////////////////////////////////////////////////////////////////////
//---------------------------// EXPLOSIVES BALLISTICS //----------------------//
////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------
void GrenadeExplode( gentity_t *ent )
//---------------------------------------------------------
{

	gunData_t	*GunInfo; 
	GunInfo = NULL; 

	if ( (ent->s.eFlags&EF_HELD_BY_SAND_CREATURE) )
	{
		ent->takedamage = qfalse; // don't allow double deaths!
		if ( ent->owner->alt_fire )
		{
			G_Damage( ent->activator, ent, ent->owner, vec3_origin, ent->currentOrigin, ent->owner->client->gs.gun->Gun_AltDamage, 0, ent->owner->client->gs.gun->Gun_Alt_MOD );
			G_Damage( ent->activator, ent, ent->owner, vec3_origin, ent->currentOrigin, ent->owner->client->gs.gun->Gun_AltSplashDamage, 0, ent->owner->client->gs.gun->Gun_Alt_MOD );
			// play explosion effect
			// a monster eat a grenade.. play a gore flesh impact effect! 
			if ( ent->owner->client->gs.gun->Gun_Alt_FleshImpact_Effect ) // play explosion and shockwave, all into one effect)
			{
				G_PlayEffect( ent->owner->client->gs.gun->Gun_Alt_FleshImpact_Effect, ent->currentOrigin );
			}
			G_FreeEntity( ent );
		}
		else // mainfire 
		{
			G_Damage( ent->activator, ent, ent->owner, vec3_origin, ent->currentOrigin, ent->owner->client->gs.gun->Gun_Damage, 0, ent->owner->client->gs.gun->Gun_MOD );
			G_Damage( ent->activator, ent, ent->owner, vec3_origin, ent->currentOrigin, ent->owner->client->gs.gun->Gun_SplashDamage, 0, ent->owner->client->gs.gun->Gun_MOD );
			// play explosion effect
			// a monster eat a grenade.. play a gore flesh impact effect! 
			if ( ent->owner->client->gs.gun->Gun_FleshImpact_Effect ) // play explosion and shockwave, all into one effect)
			{
				G_PlayEffect( ent->owner->client->gs.gun->Gun_FleshImpact_Effect, ent->currentOrigin );
			}
			G_FreeEntity( ent );
		}

	}
	if ( !ent->count )
	{
		if ( ent->owner->alt_fire )
		{
			if ( ent->owner->client->gs.gun->Gun_Alt_Charge_Sound )
			{ 
				G_Sound( ent, G_SoundIndex( ent->owner->client->gs.gun->Gun_Alt_Charge_Sound ));
			}
		}
		else 
		{
			if ( ent->owner->client->gs.gun->Gun_Charge_Sound )
			{ 
				G_Sound( ent, G_SoundIndex( ent->owner->client->gs.gun->Gun_Charge_Sound ));
			}
		}
		ent->count = 1;
		ent->nextthink = level.time + 800;
		ent->svFlags |= SVF_BROADCAST;//so everyone hears/sees the explosion?
	}
	else
	{
		// Count reach zero? Boom! 
		vec3_t	pos;
		VectorSet( pos, ent->currentOrigin[0], ent->currentOrigin[1], ent->currentOrigin[2] + 8 );
		ent->takedamage = qfalse; // don't allow double deaths!
		if ( ent->alt_fire ) // exploding alt fire grenade 
		{	
			// set splash radius and splashdamage explosion 
			if ( ent->owner->client->gs.gun->Gun_AltSplashRadius 
			&& ent->owner->client->gs.gun->Gun_AltSplashDamage )
			{	
				G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_AltSplashDamage, ent->owner->client->gs.gun->Gun_AltSplashRadius, NULL, ent->owner->client->gs.gun->Gun_Alt_MOD );
			}
			if ( ent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect )
			{
				G_PlayEffect( ent->owner->client->gs.gun->Gun_Alt_WallImpact_Effect, ent->currentOrigin );
			}

		}
		else // exploding main fire grenade 
		{	
			// set splash radius and splashdamage explosion 
			if ( ent->owner->client->gs.gun->Gun_SplashRadius 
			&& ent->owner->client->gs.gun->Gun_SplashDamage )
			{	
				G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_SplashDamage, ent->owner->client->gs.gun->Gun_SplashRadius, NULL, ent->owner->client->gs.gun->Gun_MOD );
			}
			if ( ent->owner->client->gs.gun->Gun_WallImpact_Effect )
			{
				G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
			}
		}
		// Inserire qua alterazioni di status vittime... 
		//G_PlayEffect( "thermal/shockwave", ent->currentOrigin );
		G_FreeEntity( ent ); // remove completely the grenade models. 
	}
}

//-------------------------------------------------------------------------------------------------------------
void Grenade_Die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod, int dFlags, int hitLoc )
//-------------------------------------------------------------------------------------------------------------
{
	GrenadeExplode( self );
}


//---------------------------------------------------------
void GrenadeThink( gentity_t *ent )
//---------------------------------------------------------
{
	
	int			count;
	qboolean	blow = qfalse;

	// grenade for the player do occasional radius checks and blow up if there are entities in the blast radius
	//	This is done so that the main fire is actually useful as an attack.  We explode anyway after delay expires.

	if ( (ent->s.eFlags&EF_HELD_BY_SAND_CREATURE) )
	{//blow once creature is underground (done with anim)
		//FIXME: chance of being spit out?  Especially if lots of delay left...
		ent->e_TouchFunc = touchF_NULL;//don't impact on anything
		if ( !ent->activator 
			|| !ent->activator->client
			|| !ent->activator->client->ps.legsAnimTimer )
		{//either something happened to the sand creature or it's done with it's attack anim
			//blow!
//			ent->e_ThinkFunc = thinkF_GrenadeExplode;
			ent->nextthink = level.time + Q_irand( 50, 2000 );
		}
		else
		{//keep checking
			ent->nextthink = level.time + TD_THINK_TIME;
		}
		return;
	}
	else if ( ent->delay > level.time )
	{
		//	Finally, we force it to bounce at least once before doing the special checks, otherwise it's just too easy for the player?
		if ( ent->has_bounced )
		{
				count = G_RadiusList( ent->currentOrigin, TD_TEST_RAD, ent, qtrue, ent_list );
				for ( int i = 0; i < count; i++ )
				{
					if ( ent_list[i]->s.number == 0 )
					{
						// avoid deliberately blowing up next to the player, no matter how close any enemy is..
						//	...if the delay time expires though, there is no saving the player...muwhaaa haa ha
						blow = qfalse;
						break;
					}
					else if ( ent_list[i]->client 
						&& ent_list[i]->client->NPC_class != CLASS_SAND_CREATURE//ignore sand creatures
						&& ent_list[i]->health > 0 )
					{
						//FIXME! sometimes the ent_list order changes, so we should make sure that the player isn't anywhere in this list
						blow = qtrue;
					}
				
			}
		}
		
	}
	else
	{
		// our death time has arrived, even if nothing is near us
		blow = qtrue;
	}

	if ( blow )
	{
//		ent->e_ThinkFunc = thinkF_GrenadeExplode;
		ent->nextthink = level.time + 50;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + TD_THINK_TIME;
	}
}

//---------------------------------------------------------
gentity_t *WP_FireGrenade( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	
	//gentity_t	*bolt;
	//centity_t	*cent; 
	vec3_t		dir, start;
	float		damageScale = 1.0f;
	gunData_t	*gun = NULL;

	VectorCopy( forwardVec, dir );
	VectorCopy( muzzle, start );

		

	if ( ent->s.number != 0 )
	{
		// If not the player, cut the damage a bit so we don't get pounded on so much
		damageScale = TD_NPC_DAMAGE_CUT;
	}

	if ( alt_fire )
	{
		// alt muzzle effect shot 
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect ) 
		{ 
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
	}
	else
	{
		// main muzzle effect shot 
		if ( ent->client->gs.gun->Gun_Muzzle_Effect ) 
		{ 
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
	}
	float chargeAmount = 1.0f; // default of full charge
	if ( ent->client )
	{
		chargeAmount = level.time - ent->client->ps.weaponChargeTime;
	}
	// get charge amount
	chargeAmount = chargeAmount / (float)ent->client->gs.gun->Gun_Alt_Speed;

	if ( chargeAmount > 1.0f )
	{
		chargeAmount = 1.0f;
	}
	else if ( chargeAmount < TD_MIN_CHARGE )
	{
		chargeAmount = TD_MIN_CHARGE;
	}
	
	if ( alt_fire )
	{

			gentity_t	*bolt = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent, alt_fire );
			bolt = G_Spawn();
			bolt->classname = "bryar_alt_proj";
			bolt->mass = 10;
			// make solid the grenade 
			// How 'bout we give this thing a size...
			VectorSet( bolt->mins, -4.0f, -4.0f, -4.0f );
			VectorSet( bolt->maxs, 4.0f, 4.0f, 4.0f );
			bolt->clipmask = MASK_SHOT;
			bolt->clipmask &= ~CONTENTS_CORPSE;
			bolt->contents = CONTENTS_SHOTCLIP;
			bolt->takedamage = qtrue;
			bolt->alt_fire = qtrue;
			bolt->health = 15;
//			bolt->e_DieFunc = dieF_Grenade_Die;
			WP_TraceSetStart( ent, start, bolt->mins, bolt->maxs );//make sure our start point isn't on the other side of a wall
//			bolt->e_ThinkFunc = thinkF_GrenadeExplode;
			bolt->alt_fire = qtrue; // Istantly blow up! 
			bolt->s.eType = ET_MISSILE;
			bolt->svFlags = SVF_USE_CURRENT_ORIGIN;
			bolt->s.weapon = WP_BLASTER_PISTOL;
			float	thrownSpeed = ent->client->gs.gun->Gun_Alt_Speed;
			/*const qboolean thisIsAShooter = !Q_stricmp( "misc_weapon_shooter", ent->classname);
			if (thisIsAShooter)
			{
				if (ent->delay != 0)
				{
					thrownSpeed = ent->delay;
				}
			}*/
			// normal ones bounce, alt ones explode on impact
			if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT )
			{	// grenade can be affected by gravity, or not.
				bolt->s.pos.trType = TR_GRAVITY; 
			}
			if ( ent->client->gs.gun->Gun_Alt_Gravity )
			{	// customize how many gravity affect the grenade.
				bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if  ( ent->client->gs.gun->Gun_Alt_AntiGravity )
			{	// customize how many gravity affect the grenade.
				bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
	
			bolt->owner = ent;
			VectorScale( dir, thrownSpeed * chargeAmount, bolt->s.pos.trDelta );
			if ( ent->health > 0 )
			{
				bolt->s.pos.trDelta[2] += 120;
			if ( (ent->NPC /*|| (ent->s.number && thisIsAShooter)*/
				 )&& ent->enemy )
			{//NPC or misc_weapon_shooter
				//FIXME: we're assuming he's actually facing this direction...
				vec3_t	target;
				VectorCopy( ent->enemy->currentOrigin, target );
				if ( target[2] <= start[2] )
				{
					vec3_t	vec;
					VectorSubtract( target, start, vec );
					VectorNormalize( vec );
					VectorMA( target, Q_flrand( 0, -32 ), vec, target );//throw a little short
				}	
				target[0] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[1] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[2] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
			}
			/*else if ( thisIsAShooter && ent->target && !VectorCompare( ent->pos1, vec3_origin ) )
			{//misc_weapon_shooter firing at a position
				WP_GrenadeLobFire( ent, start, ent->pos1, bolt->mins, bolt->maxs, bolt->clipmask, bolt->s.pos.trDelta, qtrue, ent->s.number, ent->enemy->s.number );
			}*/
		}
		
		// customize loop sound of thermal 
		if ( ent->client->gs.gun->Gun_Alt_Missile_Sound )
			{
				bolt->s.loopSound = G_SoundIndex( ent->client->gs.gun->Gun_Alt_Missile_Sound );
			}
		// customize damage 
		if ( ent->client->gs.gun->Gun_AltDamage )
		{
			bolt->damage = ent->client->gs.gun->Gun_AltDamage*damageScale; 
		}
		bolt->dflags = 0;
		// customize splashdamage 
		if ( ent->client->gs.gun->Gun_AltSplashDamage )
		{
			bolt->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage*damageScale; 
		}
		// customize splashradius 
		if ( ent->client->gs.gun->Gun_AltSplashRadius )
		{
			bolt->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius*damageScale; 
		}
		
		//bolt->s.weapon = WP_BLASTER_PISTOL; 
		if ( ent->client->gs.gun->Gun_Alt_MOD )
		{
			bolt->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
			bolt->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
		}
		bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, bolt->s.pos.trBase );
		SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
		VectorCopy (start, bolt->currentOrigin);
		VectorCopy( start, bolt->pos2 );
		return bolt;
	}
	else // main fire setting 
	{

			gentity_t	*bolt = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Speed, ent->client->gs.gun->Gun_Life, ent, alt_fire );
			bolt = G_Spawn();
			//bryar_proj
			bolt->classname = "bryar_proj";
			bolt->mass = 10;
			// make solid the grenade 
			// How 'bout we give this thing a size...
			VectorSet( bolt->mins, -4.0f, -4.0f, -4.0f );
			VectorSet( bolt->maxs, 4.0f, 4.0f, 4.0f );
			bolt->clipmask = MASK_SHOT;
			bolt->clipmask &= ~CONTENTS_CORPSE;
			bolt->contents = CONTENTS_SHOTCLIP;
			bolt->takedamage = qtrue;
			bolt->alt_fire = qtrue; //
			// altwallimpacteffect for grenade explosion
			// altfleshimpacteffect for body explosion. 
			bolt->health = 15;
//			bolt->e_DieFunc = dieF_Grenade_Die;
			WP_TraceSetStart( ent, start, bolt->mins, bolt->maxs );//make sure our start point isn't on the other side of a wall
			//bolt->e_ThinkFunc = thinkF_GrenadeExplode;
			float	thrownSpeed = ent->client->gs.gun->Gun_Speed;
		/*const qboolean thisIsAShooter = !Q_stricmp( "misc_weapon_shooter", ent->classname);
		if (thisIsAShooter)
		{
			if (ent->delay != 0)
			{
				thrownSpeed = ent->delay;
			}
		}*/

		
		// normal ones bounce, alt ones explode on impact
			if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT )
			{	// grenade can be affected by gravity, or not.
				bolt->s.pos.trType = TR_GRAVITY; 
			}
			if ( ent->client->gs.gun->Gun_Gravity )
			{	// customize how many gravity affect the grenade.
				bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
			if  ( ent->client->gs.gun->Gun_AntiGravity )
			{	// customize how many gravity affect the grenade.
				bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
			bolt->owner = ent;
			VectorScale( dir, thrownSpeed * chargeAmount, bolt->s.pos.trDelta );
			if ( ent->health > 0 )
			{
				bolt->s.pos.trDelta[2] += 120;
	
			if ( (ent->NPC /*|| (ent->s.number && thisIsAShooter)*/ 
			)&& ent->enemy )
			{//NPC or misc_weapon_shooter
				//FIXME: we're assuming he's actually facing this direction...
				vec3_t	target;
				VectorCopy( ent->enemy->currentOrigin, target );
				if ( target[2] <= start[2] )
				{
					vec3_t	vec;
					VectorSubtract( target, start, vec );
					VectorNormalize( vec );
					VectorMA( target, Q_flrand( 0, -32 ), vec, target );//throw a little short
				}	
				target[0] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[1] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[2] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
			}
			/*else if ( thisIsAShooter && ent->target && !VectorCompare( ent->pos1, vec3_origin ) )
			{//misc_weapon_shooter firing at a position
				WP_GrenadeLobFire( ent, start, ent->pos1, bolt->mins, bolt->maxs, bolt->clipmask, bolt->s.pos.trDelta, qtrue, ent->s.number, ent->enemy->s.number );
			}*/
		}
		
		// customize loop sound of thermal 
		if ( ent->client->gs.gun->Gun_Missile_Sound )
			{
				bolt->s.loopSound = G_SoundIndex( ent->client->gs.gun->Gun_Missile_Sound );
			}
		// customize damage 
		if ( ent->client->gs.gun->Gun_Damage )
		{
			bolt->damage = ent->client->gs.gun->Gun_Damage*damageScale; 
		}
		bolt->dflags = 0;
		// customize splashdamage 
		if ( ent->client->gs.gun->Gun_SplashDamage )
		{
			bolt->splashDamage = ent->client->gs.gun->Gun_SplashDamage*damageScale; 
		}
		// customize splashradius 
		if ( ent->client->gs.gun->Gun_SplashRadius )
		{
			bolt->splashRadius = ent->client->gs.gun->Gun_SplashRadius*damageScale; 
		}
		bolt->s.eType = ET_MISSILE;
		bolt->svFlags = SVF_USE_CURRENT_ORIGIN;
		bolt->s.weapon = WP_BLASTER_PISTOL; // why i can see missile model ONLY by weapon thermal??? 
		if ( ent->client->gs.gun->Gun_MOD )
		{
			bolt->methodOfDeath = ent->client->gs.gun->Gun_MOD;
			bolt->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
		}
		bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, bolt->s.pos.trBase );
		SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
		VectorCopy (start, bolt->currentOrigin);
		VectorCopy( start, bolt->pos2 );
		return bolt;

	}
}




//---------------------------------------------------------








gentity_t *WP_DropGrenade( gentity_t *ent )
//---------------------------------------------------------
{
	AngleVectors( ent->client->ps.viewangles, forwardVec, vrightVec, up );
	CalcEntitySpot( ent, SPOT_WEAPON, muzzle );
	return (WP_FireGrenade( ent, qfalse ));
}






void WP_timegrenade_blow( gentity_t *ent )
// add me to function.h and cpp. 
//----------------------------------------------
{
	EvaluateTrajectory( &ent->s.pos, level.time, ent->currentOrigin ); // Not sure if this is even necessary, but correct origins are cool?
	//G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_SplashDamage, ent->owner->client->gs.gun->Gun_SplashRadius, NULL, ent->owner->client->gs.gun->Gun_MOD );
	//G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
	if ( ent->alt_fire ) 
	{
		G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_AltSplashDamage, ent->owner->client->gs.gun->Gun_AltSplashRadius, NULL, ent->owner->client->gs.gun->Gun_Alt_MOD );
		G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
	}
	else 
	{
		G_RadiusDamage( ent->currentOrigin, ent->owner, ent->owner->client->gs.gun->Gun_SplashDamage, ent->owner->client->gs.gun->Gun_SplashRadius, NULL, ent->owner->client->gs.gun->Gun_MOD );
		G_PlayEffect( ent->owner->client->gs.gun->Gun_WallImpact_Effect, ent->currentOrigin );
	}
	G_FreeEntity( ent );
}

//------------------------------------------------------------------------------
static void WP_CreateTimeGrenade( vec3_t start, vec3_t fwd, gentity_t *ent )
//------------------------------------------------------------------------------
{
	// da aggiustar ancora:  
	// alt bounce 
	// missile efx
	// bounce efx 
	// explosion efx 

	if ( ent->alt_fire ) 
	{
		gentity_t	*missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Alt_Speed + random() * 700, ent->client->gs.gun->Gun_Alt_Life + random() * 2000, ent, qtrue );
//		missile->e_ThinkFunc = thinkF_WP_gun_alt_blow;
		missile->s.weapon = WP_BLASTER_PISTOL;
		missile->classname = "flech_alt";
		missile->mass = 4;
		// How 'bout we give this thing a size...
		VectorSet( missile->mins, -3.0f, -3.0f, -3.0f );
		VectorSet( missile->maxs, 3.0f, 3.0f, 3.0f );
		missile->clipmask = MASK_SHOT;
		missile->clipmask &= ~CONTENTS_CORPSE;
		//missile->s.eFlags |= EF_BOUNCE_HALF;
		missile->dflags = 0;	
		missile->svFlags = SVF_USE_CURRENT_ORIGIN;
		VectorCopy( start, missile->pos2 );
		// set damage 
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy )
				{
					missile->damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium )
				{
					missile->damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard )
				{
					missile->damage = ent->client->gs.gun->Gun_Alt_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_AltDamage )
			{
				missile->damage = ent->client->gs.gun->Gun_AltDamage;
			}
		}

		// normal ones bounce, alt ones explode on impact
		// Gravity variables 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_Alt_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_Alt_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_AltSplashDamage )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage;
		}
		if ( ent->client->gs.gun->Gun_AltSplashRadius )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_AltSplashRadius;
		}
		if ( ent->client->gs.gun->Gun_Alt_MOD ) 
		{
			missile->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
			missile->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
		}
	}
	else // Main Fire 
	{
		gentity_t	*missile = CreateMissile( start, fwd, ent->client->gs.gun->Gun_Speed + random() * 700, ent->client->gs.gun->Gun_Life + random() * 2000, ent, qtrue );
//		missile->e_ThinkFunc = thinkF_WP_gun_alt_blow;
		missile->s.weapon = WP_BLASTER_PISTOL;
		missile->classname = "flech_alt";
		missile->mass = 4;
		// How 'bout we give this thing a size...
		VectorSet( missile->mins, -3.0f, -3.0f, -3.0f );
		VectorSet( missile->maxs, 3.0f, 3.0f, 3.0f );
		missile->clipmask = MASK_SHOT;
		missile->clipmask &= ~CONTENTS_CORPSE;
		//missile->s.eFlags |= EF_BOUNCE_HALF;// - this infamous flag crash. i not know why! -.-
		missile->dflags = 0;	
		missile->svFlags = SVF_USE_CURRENT_ORIGIN;
		VectorCopy( start, missile->pos2 );

		// set damage 
		if ( ent->s.number != 0 ) // NPC 
		{
			if ( g_spskill->integer == 0 )
			{ 
				if ( ent->client->gs.gun->Gun_NPC_Damage_Easy )
				{
					missile->damage = ent->client->gs.gun->Gun_NPC_Damage_Easy;
				}
			}
			else if ( g_spskill->integer == 1 )
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Medium )
				{
					missile->damage = ent->client->gs.gun->Gun_NPC_Damage_Medium;
				}
			}
			else
			{
				if ( ent->client->gs.gun->Gun_NPC_Damage_Hard )
				{
					missile->damage = ent->client->gs.gun->Gun_NPC_Damage_Hard;
				}
			}
		}
		else // PLAYA 
		{
			if ( ent->client->gs.gun->Gun_Damage )
			{
				missile->damage = ent->client->gs.gun->Gun_Damage;
			}
		}

		// normal ones bounce, alt ones explode on impact
		// Gravity variables 
		if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT ) // Gravity Affect Shoot
			{
				missile->s.pos.trType = TR_GRAVITY; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_Gravity ) // Set Gravity Amount 
			{
				missile->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_AntiGravity ) // Set AntiGravity  
			{
				missile->s.pos.trDelta[2] += ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
			}
		if ( ent->client->gs.gun->Gun_SplashDamage )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_SplashDamage;
		}
		if ( ent->client->gs.gun->Gun_SplashRadius )
		{
			missile->splashDamage = ent->client->gs.gun->Gun_SplashRadius;
		}
		
		if ( ent->client->gs.gun->Gun_MOD ) 
		{
			missile->methodOfDeath = ent->client->gs.gun->Gun_MOD;
			missile->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
		}
	}
}
void WP_FireTimeGrenade( gentity_t *ent, qboolean alt_fire )
{
	gunData_t *gun = NULL; 
	vec3_t		dir, start;
	float		damageScale = 1.0f;
	VectorCopy( forwardVec, dir );
	VectorCopy( muzzle, start );
	if ( ent->s.number != 0 )
	{
		// If not the player, cut the damage a bit so we don't get pounded on so much
		damageScale = TD_NPC_DAMAGE_CUT;
	}

	// Main fires for the players do a little bit of extra thinking
	if ( alt_fire )
	{	
		float	thrownSpeed = ent->client->gs.gun->Gun_Alt_Speed;
		float chargeAmount = 1.0f; // default of full charge
		if ( ent->client )
		{
			chargeAmount = level.time - ent->client->ps.weaponChargeTime;
		}
		// get charge amount
		chargeAmount = chargeAmount / (float)ent->client->gs.gun->Gun_Alt_Speed;
		if ( chargeAmount > 1.0f )
		{
			chargeAmount = 1.0f;
		}
		else if ( chargeAmount < TD_MIN_CHARGE )
		{
			chargeAmount = TD_MIN_CHARGE;
		}
		//gentity_t	*bolt;
		//gentity_t	*bolt; 
		gentity_t  *bolt = CreateMissile( start, forwardVec, ent->client->gs.gun->Gun_Alt_Speed, ent->client->gs.gun->Gun_Alt_Life, ent, alt_fire );
		bolt = G_Spawn();
		//bolt->classname = "bryar_alt_proj";
		bolt->classname = "thermal_detonator";
		//  bryar_alt_proj
		bolt->mass = 10;
		// make solid the grenade 
		// How 'bout we give this thing a size...
		VectorSet( bolt->mins, -4.0f, -4.0f, -4.0f );
		VectorSet( bolt->maxs, 4.0f, 4.0f, 4.0f );
		bolt->clipmask = MASK_SHOT;
		bolt->clipmask &= ~CONTENTS_CORPSE;
		bolt->contents = CONTENTS_SHOTCLIP;
		bolt->takedamage = qtrue;
		bolt->health = 15;
//		bolt->e_DieFunc = dieF_Grenade_Die;
		WP_TraceSetStart( ent, start, bolt->mins, bolt->maxs );//make sure our start point isn't on the other side of a wall
		// act like a time grenade
		//bolt->e_ThinkFunc = thinkF_GrenadeThink; 
		bolt->nextthink = level.time + TD_THINK_TIME;
		// allow to grenade to bounce by walls
		bolt->owner = ent;
		bolt->s.eType = ET_MISSILE;
		bolt->svFlags = SVF_USE_CURRENT_ORIGIN;
		//bolt->s.weapon = WP_THERMAL;
		bolt->s.weapon = WP_BLASTER_PISTOL; 
		bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, bolt->s.pos.trBase );
		SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
		VectorCopy (start, bolt->currentOrigin);
		VectorCopy( start, bolt->pos2 );
		VectorScale( dir, thrownSpeed * chargeAmount, bolt->s.pos.trDelta );
		if ( ent->health > 0 )
		{
			bolt->s.pos.trDelta[2] += 120;
			if ( (ent->NPC 
//|| (ent->s.number && thisIsAShooter)
				 )&& ent->enemy )
			{//NPC 
				//FIXME: we're assuming he's actually facing this direction...
				vec3_t	target;
				VectorCopy( ent->enemy->currentOrigin, target );
				if ( target[2] <= start[2] )
				{
					vec3_t	vec;
					VectorSubtract( target, start, vec );
					VectorNormalize( vec );
					VectorMA( target, Q_flrand( 0, -32 ), vec, target );//throw a little short
				}	
				target[0] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[1] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[2] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				//WP_GrenadeLobFire( ent, start, target, bolt->mins, bolt->maxs, bolt->clipmask, bolt->s.pos.trDelta, qtrue, ent->s.number, ent->enemy->s.number );
			}
		}
		if ( ent->client->gs.gun->Gun_Alt_Life )
		{
			bolt->delay = level.time + ent->client->gs.gun->Gun_Alt_Life; // How long 'til she blows	
		}
		// main muzzle effect shot 
		if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect ) 
		{ 
			G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
		// customize loop sound of thermal 
		if ( ent->client->gs.gun->Gun_Alt_Missile_Sound )
			{
				bolt->s.loopSound = G_SoundIndex( ent->client->gs.gun->Gun_Alt_Missile_Sound );
		}
		// customize grenade model! 
		if ( ent->client->gs.gun->Gun_Alt_Missile_Model )
		{
			gi.G2API_InitGhoul2Model( bolt->ghoul2, ent->client->gs.gun->Gun_Alt_Missile_Model, G_ModelIndex( ent->client->gs.gun->Gun_Alt_Missile_Model ),
		NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
		// customize damage 
		if ( ent->client->gs.gun->Gun_AltDamage )
		{
			bolt->damage = ent->client->gs.gun->Gun_AltDamage*damageScale; 
		}
		bolt->dflags = 0;
		// customize splashdamage 
		if ( ent->client->gs.gun->Gun_AltSplashDamage )
		{
			bolt->splashDamage = ent->client->gs.gun->Gun_AltSplashDamage*damageScale; 
		}
		// customize splashradius 
		if ( ent->client->gs.gun->Gun_AltSplashRadius )
		{
			bolt->splashRadius = ent->client->gs.gun->Gun_AltSplashRadius*damageScale; 
		}
			if ( ent->client->gs.gun->Gun_Alt_MOD )
		{
			bolt->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
			bolt->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
		}
		// set grenade model 
	    if ( ent->client->gs.gun->Gun_Alt_Missile_Model ) 
		{
			gi.G2API_InitGhoul2Model( bolt->ghoul2, ent->client->gs.gun->Gun_Alt_Missile_Model, G_ModelIndex( ent->client->gs.gun->Gun_Alt_Missile_Model ),
			NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_GRAVITYSHOOT )
		{	// grenade can be affected by gravity, or not.
			bolt->s.pos.trType = TR_GRAVITY; 
		}
		if ( ent->client->gs.gun->Gun_Alt_Gravity )
		{	// customize how many gravity affect the grenade.
			bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_Gravity; // If is 1 , Shoot is Affected by gravity! 
		}
		if  ( ent->client->gs.gun->Gun_Alt_AntiGravity )
		{	// customize how many gravity affect the grenade.
			bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Alt_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
		}
	}
	else // main fire time grenade 
	{	
		float	thrownSpeed = ent->client->gs.gun->Gun_Alt_Speed;
		float chargeAmount = 1.0f; // default of full charge
		if ( ent->client )
		{
			chargeAmount = level.time - ent->client->ps.weaponChargeTime;
		}
		// get charge amount
		chargeAmount = chargeAmount / (float)ent->client->gs.gun->Gun_Alt_Speed;
		if ( chargeAmount > 1.0f )
		{
			chargeAmount = 1.0f;
		}
		else if ( chargeAmount < TD_MIN_CHARGE )
		{
			chargeAmount = TD_MIN_CHARGE;
		}
		gentity_t	*bolt;
		bolt = G_Spawn();
		bolt->classname = "thermal_detonator";
		bolt->mass = 10;
		// make solid the grenade 
		// How 'bout we give this thing a size...
		VectorSet( bolt->mins, -4.0f, -4.0f, -4.0f );
		VectorSet( bolt->maxs, 4.0f, 4.0f, 4.0f );
		bolt->clipmask = MASK_SHOT;
		bolt->clipmask &= ~CONTENTS_CORPSE;
		bolt->contents = CONTENTS_SHOTCLIP;
		bolt->takedamage = qtrue;
		bolt->health = 15;
//		bolt->e_DieFunc = dieF_Grenade_Die;
		WP_TraceSetStart( ent, start, bolt->mins, bolt->maxs );//make sure our start point isn't on the other side of a wall
		// act like a time grenade
//		bolt->e_ThinkFunc = thinkF_GrenadeThink; 
		bolt->nextthink = level.time + TD_THINK_TIME;
		// allow to grenade to bounce by walls
		//bolt->s.eFlags |= EF_BOUNCE_HALF;
		bolt->owner = ent;
		bolt->s.eType = ET_MISSILE;
		bolt->svFlags = SVF_USE_CURRENT_ORIGIN;
		bolt->s.weapon = WP_BLASTER_PISTOL;
		bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, bolt->s.pos.trBase );
		SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
		VectorCopy (start, bolt->currentOrigin);
		VectorCopy( start, bolt->pos2 );
		VectorScale( dir, thrownSpeed * chargeAmount, bolt->s.pos.trDelta );
		if ( ent->health > 0 )
		{
			bolt->s.pos.trDelta[2] += 120;
			if ( (ent->NPC
// || (ent->s.number && thisIsAShooter)*/
)&& ent->enemy )
			{//NPC Shooter
				//FIXME: we're assuming he's actually facing this direction...
				vec3_t	target;
				VectorCopy( ent->enemy->currentOrigin, target );
				if ( target[2] <= start[2] )
				{
					vec3_t	vec;
					VectorSubtract( target, start, vec );
					VectorNormalize( vec );
					VectorMA( target, Q_flrand( 0, -32 ), vec, target );//throw a little short
				}	
				target[0] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[1] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				target[2] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
				//WP_GrenadeLobFire( ent, start, target, bolt->mins, bolt->maxs, bolt->clipmask, bolt->s.pos.trDelta, qtrue, ent->s.number, ent->enemy->s.number );
			}
		}
		if ( ent->client->gs.gun->Gun_Life )
		{
			bolt->delay = level.time + ent->client->gs.gun->Gun_Life; // How long 'til she blows	
		}
		// main muzzle effect shot 
		if ( ent->client->gs.gun->Gun_Muzzle_Effect ) 
		{ 
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
		// customize loop sound of thermal 
		if ( ent->client->gs.gun->Gun_Missile_Sound )
			{
				bolt->s.loopSound = G_SoundIndex( ent->client->gs.gun->Gun_Missile_Sound );
			}
		// customize damage 
		if ( ent->client->gs.gun->Gun_Damage )
		{
			bolt->damage = ent->client->gs.gun->Gun_Damage*damageScale; 
		}
		bolt->dflags = 0;
		// customize splashdamage 
		if ( ent->client->gs.gun->Gun_SplashDamage )
		{
			bolt->splashDamage = ent->client->gs.gun->Gun_SplashDamage*damageScale; 
		}
		// customize splashradius 
		if ( ent->client->gs.gun->Gun_SplashRadius )
		{
			bolt->splashRadius = ent->client->gs.gun->Gun_SplashRadius*damageScale; 
		}
			if ( ent->client->gs.gun->Gun_MOD )
		{
			bolt->methodOfDeath = ent->client->gs.gun->Gun_MOD;
			bolt->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;
		}
		// set grenade model 
		if ( ent->client->gs.gun->Gun_Missile_Model ) 
		{
			gi.G2API_InitGhoul2Model( bolt->ghoul2, ent->client->gs.gun->Gun_Missile_Model, G_ModelIndex( ent->client->gs.gun->Gun_Alt_Missile_Model ),
			NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
		if ( ent->client->gs.gun->gunFlags&GFL_GRAVITYSHOOT )
		{	// grenade can be affected by gravity, or not.
			bolt->s.pos.trType = TR_GRAVITY; 
		}
		if ( ent->client->gs.gun->Gun_Gravity )
		{	// customize how many gravity affect the grenade.
			bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_Gravity; // If is 1 , Shoot is Affected by gravity! 
		}
		if  ( ent->client->gs.gun->Gun_AntiGravity )
		{	// customize how many gravity affect the grenade.
			bolt->s.pos.trDelta[2] -= ent->client->gs.gun->Gun_AntiGravity; // If is 1 , Shoot is Affected by gravity! 
		}
	}
}












#define PROXIMITY_MINE_STYLE 1
#define TRIPWIRE_MINESTYLE	2
// LASER MINE CODE 
//---------------------------------------------------------
void TouchLaserMine( gentity_t *ent, gentity_t *other, trace_t *trace )
//---------------------------------------------------------
{ // Laser mine touch the ground... 
	ent->s.eType = ET_GENERAL;

	// a tripwire so add draw line flag
	VectorCopy( trace->plane.normal, ent->movedir );

	// make it shootable
	VectorSet( ent->mins, -4, -4, -4 );
	VectorSet( ent->maxs, 4, 4, 4 );

	ent->clipmask = MASK_SHOT;
	ent->contents = CONTENTS_SHOTCLIP;
	ent->takedamage = qtrue;
	ent->health = 15;

	ent->e_DieFunc = dieF_WP_ExplosiveDie;
	ent->e_TouchFunc = touchF_NULL;

	// so we can trip it too
	ent->activator = ent->owner;
	ent->owner = NULL;

	WP_Stick( ent, trace );

	if ( ent->count == TRIPWIRE_MINESTYLE )
	{
		vec3_t		mins = {-4,-4,-4}, maxs = {4,4,4};//FIXME: global these
		trace_t		tr;
		VectorMA( ent->currentOrigin, 32, ent->movedir, ent->s.origin2 );
		gi.trace( &tr, ent->s.origin2, mins, maxs, ent->currentOrigin, ent->s.number, MASK_SHOT, G2_RETURNONHIT, 0 );
		VectorCopy( tr.endpos, ent->s.origin2 );

//		ent->e_ThinkFunc = thinkF_LaserMineThink;
	}
	ent->nextthink = level.time + LT_ACTIVATION_DELAY;
}

void LaserMineThink( gentity_t *ent )
//---------------------------------------------------------
{ // laser mine is activated 
	gentity_t	*traceEnt;
	vec3_t		end, mins = {-4,-4,-4}, maxs = {4,4,4};
	trace_t		tr;

	


	// turn on the beam effect
	if ( !(ent->s.eFlags & EF_FIRING ))
	{
		ent->s.eFlags |= EF_FIRING;
		if ( ent->activator->alt_fire )
		{
			// customize warning sound using charge sound 
			if ( ent->activator->client->gs.gun->Gun_Alt_Charge_Sound )
			{ 
				G_Sound( ent, G_SoundIndex( ent->activator->client->gs.gun->Gun_Alt_Charge_Sound ));
			}
			// customize loop sound of mine 
			if ( ent->activator->client->gs.gun->Gun_Alt_Missile_Sound )
			{ 
					ent->s.loopSound = G_SoundIndex( ent->activator->client->gs.gun->Gun_Alt_Missile_Sound );
			}
		}
		else if ( !ent->activator->alt_fire ) // main fire 
		{
			// customize warning sound using charge sound 
			if ( ent->activator->client->gs.gun->Gun_Charge_Sound )
			{ 
				G_Sound( ent, G_SoundIndex( ent->activator->client->gs.gun->Gun_Charge_Sound ));
			}
			// customize loop sound of mine 
			if ( ent->activator->client->gs.gun->Gun_Missile_Sound )
			{ 
					ent->s.loopSound = G_SoundIndex( ent->activator->client->gs.gun->Gun_Missile_Sound );
			}
		}
		else // normal default parameter 
		{ 
				ent->s.loopSound = G_SoundIndex( "sound/weapons/laser_trap/hum_loop.wav" );
				ent->s.eFlags |= EF_FIRING;
		}

	}

//	ent->e_ThinkFunc = thinkF_LaserMineThink;
	ent->nextthink = level.time + FRAMETIME;

	// Find the main impact point
	VectorMA( ent->s.pos.trBase, 2048, ent->movedir, end );
	gi.trace( &tr, ent->s.origin2, mins, maxs, end, ent->s.number, MASK_SHOT, G2_RETURNONHIT, 0 );
	
	traceEnt = &g_entities[ tr.entityNum ];

	// Adjust this so that the effect has a relatively fresh endpoint
	VectorCopy( tr.endpos, ent->pos4 );

	if ( traceEnt->client || tr.startsolid )
	{
		// go boom
		WP_Explode( ent );
		ent->s.eFlags &= ~EF_FIRING; // don't draw beam if we are dead
	}
	else
	{
		
		// FIXME: they need to avoid the beam!
		AddSoundEvent( ent->owner, ent->currentOrigin, ent->splashRadius*2, AEL_DANGER );
		AddSightEvent( ent->owner, ent->currentOrigin, ent->splashRadius*2, AEL_DANGER, 50 );
		
	}
}

//---------------------------------------------------------
void CreateLaserMine( gentity_t *laserTrap, vec3_t start, gentity_t *owner )
//---------------------------------------------------------
{
	gunData_t *gun = NULL; 
	if ( !VALIDSTRING( laserTrap->classname ))
	{
		// since we may be coming from a map placed trip mine, we don't want to override that class name....
		//	That would be bad because the player drop code tries to limit number of placed items...so it would have removed map placed ones as well.
		laserTrap->classname = "tripmine";
	}
	if ( owner->alt_fire ) // owner that shoot mine have alt fire mode. 
	{
		// customize alt splash damage
		if ( owner->client->gs.gun->Gun_AltSplashDamage )
		{
			laserTrap->splashDamage = owner->client->gs.gun->Gun_AltSplashDamage;
		}
		// customize alt splash radius 
		if ( owner->client->gs.gun->Gun_AltSplashRadius ) 
		{
			laserTrap->splashRadius = owner->client->gs.gun->Gun_AltSplashRadius;
		}
		// customize alt damage 
		if ( owner->client->gs.gun->Gun_AltDamage ) 
		{
			laserTrap->damage = owner->client->gs.gun->Gun_AltDamage;
		}
		// customize gun MOD 
		if ( owner->client->gs.gun->Gun_Alt_MOD )
		{
			laserTrap->methodOfDeath = owner->client->gs.gun->Gun_Alt_MOD;
			laserTrap->splashMethodOfDeath = owner->client->gs.gun->Gun_Alt_MOD;//? SPLASH;
		}
		laserTrap->s.eType = ET_MISSILE;
		laserTrap->svFlags = SVF_USE_CURRENT_ORIGIN;
		laserTrap->s.weapon = WP_TRIP_MINE;
		laserTrap->owner = owner;
		laserTrap->clipmask = (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP);//MASK_SHOT;
		laserTrap->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, laserTrap->s.pos.trBase );
		VectorCopy( start, laserTrap->currentOrigin);
		VectorCopy( start, laserTrap->pos2 ); // ?? wtf ?
		// Customize explosion effect 
		if ( owner->client->gs.gun->Gun_Explosion_Effect )
		{
			laserTrap->fxID = G_EffectIndex( owner->client->gs.gun->Gun_Explosion_Effect );
		}
//		laserTrap->e_TouchFunc = touchF_TouchLaserMine;
		laserTrap->s.radius = 60;
		VectorSet( laserTrap->s.modelScale, 1.0f, 1.0f, 1.0f );
		// customize explosion effect 
		if ( owner->client->gs.gun->Gun_Alt_Missile_Model ) 
		{
			gi.G2API_InitGhoul2Model( laserTrap->ghoul2, owner->client->gs.gun->Gun_Alt_Missile_Model, G_ModelIndex( owner->client->gs.gun->Gun_Alt_Missile_Model ),
			NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
	}
	else // main fire 
	{
		// customize splash damage
		if ( owner->client->gs.gun->Gun_SplashDamage )
		{
			laserTrap->splashDamage = owner->client->gs.gun->Gun_SplashDamage;
		}
		// customize splash radius 
		if ( owner->client->gs.gun->Gun_SplashRadius ) 
		{
			laserTrap->splashRadius = owner->client->gs.gun->Gun_SplashRadius;
		}
		// customize damage 
		if ( owner->client->gs.gun->Gun_Damage ) 
		{
			laserTrap->damage = owner->client->gs.gun->Gun_Damage;
		}
		if ( owner->client->gs.gun->Gun_MOD )
		{
			laserTrap->methodOfDeath = owner->client->gs.gun->Gun_MOD;
			laserTrap->splashMethodOfDeath = owner->client->gs.gun->Gun_MOD;//? SPLASH;
		}
		laserTrap->s.eType = ET_MISSILE;
		laserTrap->svFlags = SVF_USE_CURRENT_ORIGIN;
		laserTrap->s.weapon = WP_TRIP_MINE;
		laserTrap->owner = owner;
		laserTrap->clipmask = (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP);//MASK_SHOT;
		laserTrap->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, laserTrap->s.pos.trBase );
		VectorCopy( start, laserTrap->currentOrigin);
		VectorCopy( start, laserTrap->pos2 ); // ?? wtf ?
		// Customize explosion effect 
		if ( owner->client->gs.gun->Gun_Explosion_Effect )
		{
			laserTrap->fxID = G_EffectIndex( owner->client->gs.gun->Gun_Explosion_Effect );
		}
//		laserTrap->e_TouchFunc = touchF_TouchLaserMine;
		laserTrap->s.radius = 60;
		VectorSet( laserTrap->s.modelScale, 1.0f, 1.0f, 1.0f );
		// customize explosion effect 
		if ( owner->client->gs.gun->Gun_Missile_Model ) 
		{
			gi.G2API_InitGhoul2Model( laserTrap->ghoul2, owner->client->gs.gun->Gun_Missile_Model, G_ModelIndex( owner->client->gs.gun->Gun_Missile_Model ),
			NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
	}
}


//---------------------------------------------------------
static void WP_RemoveOldMines( gentity_t *ent )
//---------------------------------------------------------
{
	gentity_t	*found = NULL;
	int			trapcount = 0, i;
	int			foundLaserTraps[MAX_GENTITIES] = {ENTITYNUM_NONE};
	int			trapcount_org, lowestTimeStamp;
	int			removeMe;

	// see how many there are now
	while (( found = G_Find( found, FOFS(classname), "tripmine" )) != NULL )
	{
		if ( found->activator != ent ) // activator is really the owner?
		{
			continue;
		}
		foundLaserTraps[trapcount++] = found->s.number;
	}

	// now remove first ones we find until there are only 9 left
	found = NULL;
	trapcount_org = trapcount;
	lowestTimeStamp = level.time;

	while ( trapcount > 9 )
	{
		removeMe = -1;
		for ( i = 0; i < trapcount_org; i++ )
		{
			if ( foundLaserTraps[i] == ENTITYNUM_NONE )
			{
				continue;
			}
			found = &g_entities[foundLaserTraps[i]];
			if ( found->setTime < lowestTimeStamp )
			{
				removeMe = i;
				lowestTimeStamp = found->setTime;
			}
		}
		if ( removeMe != -1 )
		{
			//remove it... or blow it?
			if ( &g_entities[foundLaserTraps[removeMe]] == NULL )
			{
				break;
			}
			else
			{
				G_FreeEntity( &g_entities[foundLaserTraps[removeMe]] );
			}
			foundLaserTraps[removeMe] = ENTITYNUM_NONE;
			trapcount--;
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------
void WP_FireLaserMine( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{

	gunData_t *gun = NULL; 
	vec3_t		start;
	gentity_t	*laserTrap;

	// limit to 10 placed at any one time
	WP_RemoveOldMines( ent );

	//FIXME: surface must be within 64
	laserTrap = G_Spawn();

	if ( laserTrap )
	{
		// now make the new one
		VectorCopy( muzzle, start );
		WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

		CreateLaserMine( laserTrap, start, ent );

		// set player-created-specific fields
		laserTrap->setTime = level.time;//remember when we placed it

		laserTrap->s.eFlags |= EF_MISSILE_STICK;
		laserTrap->s.pos.trType = TR_GRAVITY;
		VectorScale( forwardVec, LT_VELOCITY, laserTrap->s.pos.trDelta );

		if ( alt_fire )
		{
			// Play Custom Alt Muzzle Flash Effect
			if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect ) 
			{ 
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
			laserTrap->count = TRIPWIRE_MINESTYLE;
		}
		else
		{
			// Play Custom Muzzle Flash Effect 
			if ( ent->client->gs.gun->Gun_Muzzle_Effect ) 
			{ 
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
			laserTrap->count = TRIPWIRE_MINESTYLE;
		}
	}
}


// PROXY MINES 
//---------------------------------------------------------
void TouchProxyMine( gentity_t *ent, gentity_t *other, trace_t *trace )
//---------------------------------------------------------
{
	ent->s.eType = ET_GENERAL;

	// a tripwire so add draw line flag
	VectorCopy( trace->plane.normal, ent->movedir );

	// make it shootable
	VectorSet( ent->mins, -4, -4, -4 );
	VectorSet( ent->maxs, 4, 4, 4 );

	ent->clipmask = MASK_SHOT;
	ent->contents = CONTENTS_SHOTCLIP;
	ent->takedamage = qtrue;
	ent->health = 15;

	ent->e_DieFunc = dieF_WP_ExplosiveDie;
	ent->e_TouchFunc = touchF_NULL;

	// so we can trip it too
	ent->activator = ent->owner;
	ent->owner = NULL;

	WP_Stick( ent, trace );

	if ( ent->count == PROXIMITY_MINE_STYLE )
	{
//		ent->e_ThinkFunc = thinkF_ProxyMineThink;
	}

	ent->nextthink = level.time + LT_ACTIVATION_DELAY;
}

// copied from flechette prox above...which will not be used if this gets approved
//---------------------------------------------------------
void ProxyMineThink( gentity_t *ent )
//---------------------------------------------------------
{
	int			count;
	qboolean	blow = qfalse;

	// first time through?
	if ( ent->count )
	{
		// play activated warning
		ent->count = 0;
		ent->s.eFlags |= EF_PROX_TRIP;
		
		if ( ent->activator->alt_fire )
		{
			// customize warning sound using charge sound 
			if ( ent->activator->client->gs.gun->Gun_Alt_Charge_Sound )
			{ 
				G_Sound( ent, G_SoundIndex( ent->activator->client->gs.gun->Gun_Alt_Charge_Sound ));
			}
		}
		else if ( !ent->activator->alt_fire ) // main fire 
		{
			// customize warning sound using charge sound 
			if ( ent->activator->client->gs.gun->Gun_Charge_Sound )
			{ 
				G_Sound( ent, G_SoundIndex( ent->activator->client->gs.gun->Gun_Charge_Sound ));
			}
			
		}
		//G_Sound( ent, G_SoundIndex( "sound/weapons/laser_trap/warning.wav" ));
	}

	// if it isn't time to auto-explode, do a small proximity check
	if ( ent->delay > level.time )
	{
		count = G_RadiusList( ent->currentOrigin, PROX_MINE_RADIUS_CHECK, ent, qtrue, ent_list );

		for ( int i = 0; i < count; i++ )
		{
			if ( ent_list[i]->client && ent_list[i]->health > 0 && ent->activator && ent_list[i]->s.number != ent->activator->s.number )
			{
				blow = qtrue;
				break;
			}
		}
	}
	else
	{
		// well, we must die now
		blow = qtrue;
	}

	if ( blow )
	{
//		G_Sound( ent, G_SoundIndex( "sound/weapons/flechette/warning.wav" ));
		ent->e_ThinkFunc = thinkF_WP_Explode;
		ent->nextthink = level.time + 200;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + 500;
	}
}

//---------------------------------------------------------

//---------------------------------------------------------
void CreateProxyMine( gentity_t *ProxyTrap, vec3_t start, gentity_t *owner )
//---------------------------------------------------------
{
	if ( !VALIDSTRING( ProxyTrap->classname ))
	{
		// since we may be coming from a map placed trip mine, we don't want to override that class name....
		//	That would be bad because the player drop code tries to limit number of placed items...so it would have removed map placed ones as well.
		ProxyTrap->classname = "tripmine";
	}
	if ( owner->alt_fire ) // owner that shoot mine have alt fire mode. 
	{
		// customize alt splash damage
		if ( owner->client->gs.gun->Gun_AltSplashDamage )
		{
			ProxyTrap->splashDamage = owner->client->gs.gun->Gun_AltSplashDamage;
		}
		// customize alt splash radius 
		if ( owner->client->gs.gun->Gun_AltSplashRadius ) 
		{
			ProxyTrap->splashRadius = owner->client->gs.gun->Gun_AltSplashRadius;
		}
		// customize alt damage 
		if ( owner->client->gs.gun->Gun_AltDamage ) 
		{
			ProxyTrap->damage = owner->client->gs.gun->Gun_AltDamage;
		}
		ProxyTrap->methodOfDeath = MOD_LASERTRIP;
		ProxyTrap->splashMethodOfDeath = MOD_LASERTRIP;//? SPLASH;
		ProxyTrap->s.eType = ET_MISSILE;
		ProxyTrap->svFlags = SVF_USE_CURRENT_ORIGIN;
		ProxyTrap->s.weapon = WP_TRIP_MINE;
		ProxyTrap->owner = owner;
		ProxyTrap->clipmask = (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP);//MASK_SHOT;
		ProxyTrap->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, ProxyTrap->s.pos.trBase );
		VectorCopy( start, ProxyTrap->currentOrigin);
		VectorCopy( start, ProxyTrap->pos2 ); // ?? wtf ?
		// Customize explosion effect 
		if ( owner->client->gs.gun->Gun_Explosion_Effect )
		{
			ProxyTrap->fxID = G_EffectIndex( owner->client->gs.gun->Gun_Explosion_Effect );
		}
//		ProxyTrap->e_TouchFunc = touchF_TouchProxyMine;
		ProxyTrap->s.radius = 60;
		VectorSet( ProxyTrap->s.modelScale, 1.0f, 1.0f, 1.0f );
		// customize explosion effect 
		if ( owner->client->gs.gun->Gun_Alt_Missile_Model ) 
		{
			gi.G2API_InitGhoul2Model( ProxyTrap->ghoul2, owner->client->gs.gun->Gun_Alt_Missile_Model, G_ModelIndex( owner->client->gs.gun->Gun_Alt_Missile_Model ),
			NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
	}
	else // main fire 
	{
		// customize splash damage
		if ( owner->client->gs.gun->Gun_SplashDamage )
		{
			ProxyTrap->splashDamage = owner->client->gs.gun->Gun_SplashDamage;
		}
		// customize splash radius 
		if ( owner->client->gs.gun->Gun_SplashRadius ) 
		{
			ProxyTrap->splashRadius = owner->client->gs.gun->Gun_SplashRadius;
		}
		// customize damage 
		if ( owner->client->gs.gun->Gun_Damage ) 
		{
			ProxyTrap->damage = owner->client->gs.gun->Gun_Damage;
		}
		ProxyTrap->methodOfDeath = MOD_LASERTRIP;
		ProxyTrap->splashMethodOfDeath = MOD_LASERTRIP;//? SPLASH;
		ProxyTrap->s.eType = ET_MISSILE;
		ProxyTrap->svFlags = SVF_USE_CURRENT_ORIGIN;
		ProxyTrap->s.weapon = WP_TRIP_MINE;
		ProxyTrap->owner = owner;
		ProxyTrap->clipmask = (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP);//MASK_SHOT;
		ProxyTrap->s.pos.trTime = level.time;		// move a bit on the very first frame
		VectorCopy( start, ProxyTrap->s.pos.trBase );
		VectorCopy( start, ProxyTrap->currentOrigin);
		VectorCopy( start, ProxyTrap->pos2 ); // ?? wtf ?
		// Customize explosion effect 
		if ( owner->client->gs.gun->Gun_Explosion_Effect )
		{
			ProxyTrap->fxID = G_EffectIndex( owner->client->gs.gun->Gun_Explosion_Effect );
		}
//		ProxyTrap->e_TouchFunc = touchF_TouchProxyMine;
		ProxyTrap->s.radius = 60;
		VectorSet( ProxyTrap->s.modelScale, 1.0f, 1.0f, 1.0f );
		// customize explosion effect 
		if ( owner->client->gs.gun->Gun_Missile_Model ) 
		{
			gi.G2API_InitGhoul2Model( ProxyTrap->ghoul2, owner->client->gs.gun->Gun_Missile_Model, G_ModelIndex( owner->client->gs.gun->Gun_Missile_Model ),
			NULL_HANDLE, NULL_HANDLE, 0, 0);
		}
	}
}



void WP_FireProxyMine( gentity_t *ent, qboolean alt_fire )
{

	vec3_t		start;
	gentity_t	*ProxyTrap;
	gunData_t *gun = NULL; 
	// limit to 10 placed at any one time
	WP_RemoveOldMines( ent );

	//FIXME: surface must be within 64
	ProxyTrap = G_Spawn();

	if ( ProxyTrap )
	{
		// now make the new one
		VectorCopy( muzzle, start );
		WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

		CreateProxyMine( ProxyTrap, start, ent );

		// set player-created-specific fields
		ProxyTrap->setTime = level.time;//remember when we placed it

		ProxyTrap->s.eFlags |= EF_MISSILE_STICK;
		ProxyTrap->s.pos.trType = TR_GRAVITY;
		VectorScale( forwardVec, LT_VELOCITY, ProxyTrap->s.pos.trDelta );

		if ( alt_fire )
		{
			// Play Custom Alt Muzzle Flash Effect
			if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect ) 
			{ 
				G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
			ProxyTrap->count = PROXIMITY_MINE_STYLE;
			ProxyTrap->delay = level.time + 40000; // will auto-blow in 40 seconds.
			// customize gun MOD 
			if ( ent->client->gs.gun->Gun_Alt_MOD )
			{
				ProxyTrap->methodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;
				ProxyTrap->splashMethodOfDeath = ent->client->gs.gun->Gun_Alt_MOD;//? SPLASH;
			}
		}
		else
		{
			// Play Custom Muzzle Flash Effect 
			if ( ent->client->gs.gun->Gun_Muzzle_Effect ) 
			{ 
				G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
			}
			ProxyTrap->count = PROXIMITY_MINE_STYLE;
			ProxyTrap->delay = level.time + 40000; // will auto-blow in 40 seconds.
			// customize gun MOD 
			if ( ent->client->gs.gun->Gun_MOD )
			{
				ProxyTrap->methodOfDeath = ent->client->gs.gun->Gun_MOD;
				ProxyTrap->splashMethodOfDeath = ent->client->gs.gun->Gun_MOD;//? SPLASH;
			}
		}
	}
}



//---------------------------------------------------------
static void WP_DropBomb( gentity_t *self, vec3_t start, vec3_t dir ) 
//---------------------------------------------------------
{
	
	// Chucking a new one
	AngleVectors( self->client->ps.viewangles, forwardVec, vrightVec, up );
	CalcMuzzlePoint( self, forwardVec, vrightVec, up, muzzle, 0 );
	VectorNormalize( forwardVec );
	VectorMA( muzzle, -4, forwardVec, muzzle );
	VectorCopy( muzzle, start );
	WP_TraceSetStart( self, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
	if ( self->client->gs.gun->Gun_Speed && self->client->gs.gun->Gun_Life )
	{
		gentity_t	*missile = CreateMissile( start, forwardVec, self->client->gs.gun->Gun_Speed, self->client->gs.gun->Gun_Life, self, qfalse );
		//gentity_t	*missile = CreateMissile( start, forwardVec, 300, 10000, self, qfalse );
		// customize explosion effect 
		if ( self->client->gs.gun->Gun_Explosion_Effect )
		{
			missile->fxID = G_EffectIndex( self->client->gs.gun->Gun_Explosion_Effect ); // if we set an explosion effect, explode death can use that instead
		}
		missile->classname = "detpack";
		missile->s.weapon = WP_DET_PACK;
		missile->s.pos.trType = TR_GRAVITY;
		missile->s.eFlags |= EF_MISSILE_STICK;
		missile->e_TouchFunc = touchF_charge_stick;
		// customize damage 
		if ( self->client->gs.gun->Gun_Damage )
		{
			missile->damage = self->client->gs.gun->Gun_Damage; 
		}
		// customize gun MOD 
		if ( self->client->gs.gun->Gun_MOD )
		{
			missile->methodOfDeath = self->client->gs.gun->Gun_MOD;
			if ( self->client->gs.gun->Gun_SplashDamage )
			{
				missile->splashMethodOfDeath = self->client->gs.gun->Gun_MOD;
			}
		}
		// customize splash damage
		if ( self->client->gs.gun->Gun_SplashDamage )
		{
			missile->splashDamage = self->client->gs.gun->Gun_SplashDamage; 
		}
		// customize splash radius 
		if ( self->client->gs.gun->Gun_SplashRadius )
		{
			missile->splashRadius = self->client->gs.gun->Gun_SplashRadius; 
		}
		missile->clipmask = (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SHOTCLIP);//MASK_SHOT;
		// we don't want it to ever bounce
		missile->bounceCount = 0;
		missile->s.radius = 30;
		VectorSet( missile->s.modelScale, 1.0f, 1.0f, 1.0f );
		// customize missile model 
		if ( self->client->gs.gun->Gun_Missile_Model )
		{
				gi.G2API_InitGhoul2Model( missile->ghoul2, self->client->gs.gun->Gun_Missile_Model, G_ModelIndex( self->client->gs.gun->Gun_Missile_Model ),
		NULL_HANDLE, NULL_HANDLE, 0, 0);
			 //	gi.G2API_InitGhoul2Model( missile->ghoul2, weaponData[WP_DET_PACK].missileMdl, G_ModelIndex( weaponData[WP_DET_PACK].missileMdl ),
		//NULL_HANDLE, NULL_HANDLE, 0, 0);
		}

	
		AddSoundEvent( NULL, missile->currentOrigin, 128, AEL_MINOR, qtrue );
		AddSightEvent( NULL, missile->currentOrigin, 128, AEL_SUSPICIOUS, 10 );
	}
}

//---------------------------------------------------------
void WP_FireBomb( gentity_t *ent, qboolean alt_fire )
{
	gunData_t *gun = NULL; 
	if ( !ent || !ent->client )
	{
		return;
	}

	if ( alt_fire  )
	{
		if ( ent->client->ps.eFlags & EF_PLANTED_CHARGE )
		{
			gentity_t *found = NULL;

			// loop through all ents and blow the crap out of them!
			while (( found = G_Find( found, FOFS( classname ), "detpack" )) != NULL )
			{
				if ( found->activator == ent )
				{
					VectorCopy( found->currentOrigin, found->s.origin );
					found->e_ThinkFunc = thinkF_WP_Explode;
					found->nextthink = level.time + 100 + random() * 100;
					if ( ent->client->gs.gun->Gun_Alt_Muzzle_Effect ) 
					{ 
						G_PlayEffect( ent->client->gs.gun->Gun_Alt_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
					}
					// would be nice if this actually worked?
					AddSoundEvent( NULL, found->currentOrigin, found->splashRadius*2, AEL_DANGER, qfalse, qtrue );//FIXME: are we on ground or not?
					AddSightEvent( NULL, found->currentOrigin, found->splashRadius*2, AEL_DISCOVERED, 100 );
				}
			}

			ent->client->ps.eFlags &= ~EF_PLANTED_CHARGE;
		}
	}
	else
	{	
		// Use Main Muzzle Shot for play sound of placing the bomb
		if ( ent->client->gs.gun->Gun_Muzzle_Effect ) 
		{ 
			G_PlayEffect( ent->client->gs.gun->Gun_Muzzle_Effect, ent->client->renderInfo.handRPoint);  
		}
		WP_DropBomb( ent, muzzle, forwardVec );
		ent->client->ps.eFlags |= EF_PLANTED_CHARGE;
	}
}

//---------------------------------------------------------
void WP_FireBryarPistol( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	// Two Different, Specular fire mod. one with spreading and one with no spreading
	// i not see other solutions for customize and fix spread. 
	gunData_t *gun = NULL; 
	// If this flag is active, and gun is on water, gun cant'shoot! 
	 int gunContents = 0;
	 if ( ent->client->gs.gun->gunFlags3&GFL_NO_UNDERWATER) 
       {//gun can stay on underwater
		   gunContents = gi.pointcontents( ent->client->renderInfo.muzzlePoint, ent->client->gs.gunEntityNum );
       }

	if ( alt_fire )
	{	
		// SET ALT SHOOT BALISTIC BY FLAGS! 
		// SPREADING 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_SPREADSHOOT )
		{
			gi.Printf( S_COLOR_BLUE "ALT SPREAD!\n" , ent );
			vec3_t	dir, angs;
			vectoangles( forwardVec, angs );
			angs[PITCH] += ( crandom() * ent->client->gs.gun->Gun_Alt_Spread);
			angs[YAW]	+= ( crandom() * ent->client->gs.gun->Gun_Alt_Spread);
			AngleVectors( angs, dir, NULL, NULL );
			WP_FireSpread( ent, muzzle, dir, alt_fire );
		}
		// ALT RAY SHOOT 
		if ( ent->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT ) // altrayshoot is one? shoot a ray! 
		{
			gi.Printf( S_COLOR_BLUE "ALT RAY!\n" , ent );
			// aka disruptor main fire 
		     WP_FireRay( ent, alt_fire );
		}
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT2 ) // altrayshoot2 is one? shoot a ray! 
		{
			gi.Printf( S_COLOR_BLUE "ALT SNIPER RAY!\n" , ent );
			// aka disruptor alt fire 
		     WP_FireRay2( ent, alt_fire );
		}
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_RAYSHOOT3 ) // altrayshoot3 is one? shoot a ray! 
		{
			gi.Printf( S_COLOR_BLUE "ALT CONCUSSION RAY!\n" , ent );
			// aka concussion alt fire 
		     WP_FireRay3( ent, alt_fire );
		}
		// FLECHETTE - MULTIPLE SHOT 
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_MULTIPLESHOOT )// if is 1 alt fire shoot multiple shoots
		{
			gi.Printf( S_COLOR_BLUE "ALT MULTIPLE!\n" , ent );
			WP_FireMultiple ( ent, alt_fire ); 
		}
		// FLECHETTE SHRAPNEL SHOT
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_SHRAPNELSHOOT )// if is 1 fire shoot shrapnels
		{
			gi.Printf( S_COLOR_BLUE "FIRE ALT SHRAPNEL!\n" , ent );
			WP_FireShrapnel ( ent, alt_fire ); 
		}
		// FLECHETTE TIME BOMB SHOOT
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_TIMEBOMBSHOOT )// if is 1 fire shoot bouncing explosive delay projectiles
		{
			gi.Printf( S_COLOR_BLUE "FIRE ALT TIMEBOMB!\n" , ent );
			WP_FireTimeBomb ( ent, alt_fire ); 
		}
		// BOWCASTER - DIVIDE SHOOTS 
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_DIVIDESHOOT )// if is on 1, shoot alt divide projectiles as bowcaster 
		{
			gi.Printf( S_COLOR_BLUE "ALT DIVIDE!\n" , ent );
			WP_FireDivide ( ent, alt_fire ); 
		}
		// BOWCASTER - BOUNCE SHOOTS 
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_BOUNCESHOOT )// if is on 1, shoot alt divide projectiles as bowcaster 
		{
			gi.Printf( S_COLOR_BLUE "MAIN BOUNCE SHOOT!\n" , ent );
			WP_FireBounce ( ent, alt_fire ); 
		}
		// ROCKET AIM SHOOT 
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_AIMSHOOT )// // if is 1, gun alt shoot aim missiles as rocket launcher 
		{
			gi.Printf( S_COLOR_BLUE "ALT AIM!\n" , ent );
			WP_FireAim ( ent, alt_fire ); 
		}
		// GAS WEAPON AS NOGHRI STICK
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_GASSHOOT )//  if is 1, gun shoot gas projectile as noghri stick 
		{
			gi.Printf( S_COLOR_BLUE "ALT GAS!\n" , ent );
			WP_FireGas ( ent, alt_fire ); 
		}
		// DEMP2 SHOT
		else if ( ent->client->gs.gun->gunFlags&GFL_ALT_AOESHOOT )//  if is 1, gun shoot like demp2 alt detonation 
		{
			gi.Printf( S_COLOR_BLUE "AOE RAY!\n" , ent );
			WP_FireAoe ( ent, alt_fire ); 
		}
		// MELEE SHOTS 
		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_MELEE )// Melee Attack (Punchs) 
		{
			gi.Printf( S_COLOR_BLUE "ALT MELEE1!\n" , ent );
			WP_FireMelee ( ent, alt_fire ); 
		}
		// MELEE SHOTS 2
		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_MELEE2 )// Melee Attack (Kicks)  
		{
			gi.Printf( S_COLOR_BLUE "ALT MELEE2!\n" , ent );
			WP_FireMelee ( ent, alt_fire ); 
		}
		// STAFF - TUSKEN RIFLE MELEE ATK  
		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_STAFFMELEE )// if is on 1, shoot a melee attack combo like tusken rifle-tusken staff altfire
		{
			gi.Printf( S_COLOR_BLUE "ALT STAFFMELEE!\n" , ent );
			WP_FireMelee ( ent, alt_fire ); 
		}
		// STUN BATON 
		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_BATON )// if is 1 shoot a stun baton 
		{
			gi.Printf( S_COLOR_BLUE "ALT BATON SHOT!\n" , ent );
			WP_FireBaton ( ent, alt_fire ); 
		}
		// GRENADE WEAPON 
		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_GRENADE )// if is on 1, alt shoot a grenade like thermal alt  
		{
			gi.Printf( S_COLOR_BLUE "ALT GRENADE!\n" , ent );
			WP_FireGrenade ( ent, alt_fire ); 
		}
 		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_TIMEGRENADE )// if is on 1, alt shoot a time grenade like thermal main 
		{
			gi.Printf( S_COLOR_BLUE "ALT TIME GRENADE!\n" , ent );
			WP_FireTimeGrenade ( ent, alt_fire ); 
		}
		// MINE WEAPON
		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_LASERMINE )// if is on 1, alt shoot a laser mine 
		{
			gi.Printf( S_COLOR_BLUE "ALT LASERMINE!\n" , ent );
			WP_FireLaserMine ( ent, alt_fire ); 
		}
 		else if ( ent->client->gs.gun->gunFlags2&GFL_ALT_PROXYMINE )// if is on 1, alt shoot a proxy mine 
		{
			gi.Printf( S_COLOR_BLUE "ALT PROXY MINE!\n" , ent );
			WP_FireProxyMine ( ent, alt_fire ); 
		}
		else if ( ent->client->gs.gun->gunFlags2&GFL_BOMB ) // if is 1, place a bomb that can be activated on remote. 
		{
			gi.Printf( S_COLOR_BLUE "FIRE BOMB!\n" , ent );
			WP_FireBomb( ent, alt_fire );
		}
		/*else //SET ALL FLAG OFF! 
		{
			gi.Printf( S_COLOR_BLUE "FIRE ALT NORMAL!\n" , ent );
			WP_FireNormal( ent, alt_fire );
		}*/

	}	
	else  // main fire 
	{	
		// SET MAIN SHOOT BALISTIC BY FLAGS! 
		// SPREADING 
		if ( ent->client->gs.gun->gunFlags&GFL_SPREADSHOOT )
		{
			gi.Printf( S_COLOR_BLUE "MAIN SPREAD!\n" , ent );
			vec3_t	dir, angs;
			vectoangles( forwardVec, angs );
			angs[PITCH] += ( crandom() * ent->client->gs.gun->Gun_Spread);
			angs[YAW]	+= ( crandom() * ent->client->gs.gun->Gun_Spread);
			AngleVectors( angs, dir, NULL, NULL );
			WP_FireSpread( ent, muzzle, dir, alt_fire );
		}
		// ALT RAY SHOOT 
		if ( ent->client->gs.gun->gunFlags&GFL_RAYSHOOT ) // rayshoot is one? shoot a ray! 
		{
			gi.Printf( S_COLOR_BLUE "RAY!\n" , ent );
			// aka disruptor main fire 
		     WP_FireRay( ent, alt_fire );
		}
		else if ( ent->client->gs.gun->gunFlags&GFL_RAYSHOOT2 ) //rayshoot2 is one? shoot a ray! 
		{
			gi.Printf( S_COLOR_BLUE "SNIPER RAY!\n" , ent );
			// aka disruptor alt fire 
		     WP_FireRay2( ent, alt_fire );
		}
		else if ( ent->client->gs.gun->gunFlags&GFL_RAYSHOOT3 ) // rayshoot3 is one? shoot a ray! 
		{
			gi.Printf( S_COLOR_BLUE "CONCUSSION RAY!\n" , ent );
			// aka concussion alt fire 
		     WP_FireRay3( ent, alt_fire );
		}
		// FLECHETTE - MULTIPLE SHOT 
		else if ( ent->client->gs.gun->gunFlags&GFL_MULTIPLESHOOT )// if is 1 fire shoot multiple shoots
		{
			gi.Printf( S_COLOR_BLUE "FIRE MULTIPLE!\n" , ent );
			WP_FireMultiple ( ent, alt_fire ); 
		}
		// FLECHETTE SHRAPNEL SHOT
		else if ( ent->client->gs.gun->gunFlags&GFL_SHRAPNELSHOOT )// if is 1 fire shoot shrapnels
		{
			gi.Printf( S_COLOR_BLUE "FIRE SHRAPNEL!\n" , ent );
			WP_FireShrapnel ( ent, alt_fire ); 
		}
		// FLECHETTE GRENADESHOOT
		else if ( ent->client->gs.gun->gunFlags&GFL_TIMEBOMBSHOOT )// if is 1 fire shoot bouncing explosive delay projectiles
		{
			gi.Printf( S_COLOR_BLUE "FIRE TIMEBOMB!\n" , ent );
			WP_FireTimeBomb ( ent, alt_fire ); 
		}
		// BOWCASTER - BOUNCE SHOOTS 
		else if ( ent->client->gs.gun->gunFlags&GFL_BOUNCESHOOT )// if is on 1, shoot alt divide projectiles as bowcaster 
		{
			gi.Printf( S_COLOR_BLUE "MAIN BOUNCE!\n" , ent );
			WP_FireBounce ( ent, alt_fire ); 
		}
		// BOWCASTER - DIVIDE SHOOTS 
		else if ( ent->client->gs.gun->gunFlags&GFL_DIVIDESHOOT )// if is on 1, shoot divide projectiles as bowcaster 
		{	gi.Printf( S_COLOR_BLUE "FIRE DIVIDE!\n" , ent );
			WP_FireDivide ( ent, alt_fire ); 
		}
		// ROCKET AIM SHOOT 
		else if ( ent->client->gs.gun->gunFlags&GFL_AIMSHOOT )// if is 1, gun shoot aim missiles as rocket launcher 
		{
			gi.Printf( S_COLOR_BLUE "AIM SHOOT!\n" , ent );
			WP_FireAim ( ent, alt_fire ); 
		}
		// GAS WEAPON AS NOGHRI STICK
		else if ( ent->client->gs.gun->gunFlags&GFL_GASSHOOT )//  if is 1, gun shoot gas projectile as noghri stick 
		{
			gi.Printf( S_COLOR_BLUE "GAS SHOT!\n" , ent );
			WP_FireGas ( ent, alt_fire ); 
		}
		else if ( ent->client->gs.gun->gunFlags&GFL_AOESHOOT )//  if is 1, gun shoot like demp2 alt detonation 
		{
			gi.Printf( S_COLOR_BLUE "AOE SHOT!\n" , ent );
			WP_FireAoe ( ent, alt_fire ); 
		}
		// MELEE SHOTS 
		else if ( ent->client->gs.gun->gunFlags2&GFL_MELEE )// Melee Attack (Punchs) 
		{
			gi.Printf( S_COLOR_BLUE "MELEE1!\n" , ent );
			WP_FireMelee ( ent, alt_fire ); 
		}
		// MELEE SHOTS 2
		else if ( ent->client->gs.gun->gunFlags2&GFL_MELEE2 )// Melee Attack (Kicks)  
		{
			WP_FireMelee ( ent, alt_fire ); 
		}
		// STAFF - TUSKEN RIFLE MELEE ATK  
		else if ( ent->client->gs.gun->gunFlags2&GFL_STAFFMELEE )// if is on 1, shoot a melee attack combo like tusken rifle-tusken staff altfire
		{
			gi.Printf( S_COLOR_BLUE "MELEESTAFF!\n" , ent );
			WP_FireMelee ( ent, alt_fire ); 
		}
		// STUN BATON 
		else if ( ent->client->gs.gun->gunFlags2&GFL_BATON )// if is 1 shoot a stun baton 
		{
			gi.Printf( S_COLOR_BLUE "BATON SHOT!\n" , ent );
			WP_FireBaton ( ent, alt_fire ); 
		}
		// GRENADE WEAPON 
		else if ( ent->client->gs.gun->gunFlags2&GFL_GRENADE )// if is on 1, shoot a grenade like thermal alt  
		{
			gi.Printf( S_COLOR_BLUE "GRENADE!\n" , ent );
			WP_FireGrenade ( ent, alt_fire ); 
		}
 		else if ( ent->client->gs.gun->gunFlags2&GFL_TIMEGRENADE )// if is on 1, shoot a time grenade like thermal main 
		{
			gi.Printf( S_COLOR_BLUE "TIME GRENADE!\n" , ent );
			WP_FireTimeGrenade ( ent, alt_fire ); 
		}
		// MINE WEAPON
		else if ( ent->client->gs.gun->gunFlags2&GFL_LASERMINE )// if is on 1, shoot a laser mine 
		{
			gi.Printf( S_COLOR_BLUE "LASER MINE!\n" , ent );
			WP_FireLaserMine ( ent, alt_fire ); 
		}
 		else if ( ent->client->gs.gun->gunFlags2&GFL_PROXYMINE )// if is on 1, shoot a proxy mine 
		{
			gi.Printf( S_COLOR_BLUE "PROXY MINE!\n" , ent );
			WP_FireProxyMine ( ent, alt_fire ); 
		}
		else if ( ent->client->gs.gun->gunFlags2&GFL_BOMB ) // if is 1, place a bomb that can be activated on remote. 
		{
			gi.Printf( S_COLOR_BLUE "FIRE BOMB!\n" , ent );
			WP_FireBomb( ent, alt_fire );
		}
		/*else 
		{
			gi.Printf( S_COLOR_BLUE "FIRE NORMAL!\n" , ent );
			WP_FireNormal( ent, alt_fire );
		}*/
	}
}


// ------------------------------------------ LISTA PARAMETRI ---------------------------
// fullname: complete name of weapon. show into UI 
// gunmodel: 3d MODEL of weapon 
// stopsound1-2-3, altstopsound 1-2-3 : sound played when weapon stop of fire 
// loopsound, altloopsound : sound looped when weapon is active 
// chargesound1-2-3, altchargesound 1-2-3: sound played when weapon is charging shot
// selectsound: sound played when weapon is selected 
// Guntype: slot when is assigned the weapon 
// Ammomax: max amount of ammo of the weapon 
// AmmoIndex: type of ammo used 
// AmmoLow: amount of ammo that fire "low signal" 
// energypershot, altenergypershot: how many ammo consumed by a shot 
// icon: icon shader of weapon 
// description: name of weapon description on UI 
// displayname: name displayed when weapon is selected on HUD 
// numbarrels: number of weapon barrels 
// ------ BALLISTIC 
// firetime, altfiretime: time between two shoots 
// npcfiretime, altnpcfiretime: time between two shoots for NPCs. 
// range, altrange: range of shoot 
// spreashot, altspreadshot: if is 1, weapon spread the projectiles 
// spread, altspread: float, amount of spread and dispersion of projectiles
// rayshot2, altrayshot2: shoot a ray, like disruptor alt mode 
// rayshot3, altrayshot3: shoot a ray, like concussion alt mode 
// zoom, altzoom: if is 1, weapon can zoom 
// startzoomfov, altstartzoomfov: field of view of min zoom  
// endzoomfov, altendzoomfov: field of view of max zoom 
// zoomtime, altzoomtime: how many second weapon take for reach max zoom 
// zoommask, altzoommask: set 2d mask shader for zoom mode 
// crosshair: set crosshair shader 
// shrapnelshot, altshrapnelshot: if is 1, shoot shrapnels like flechette alt fires 
// timeshot, alttimeshot: timeamount for shrapnels for explode 
// bounceshot, altbounceshot: projectile can bounce! 
// bouncehalfshot, altbouncehalfshot: projectiles can half bounces 
// numbounce, altnumbounce: how many time projectiles can bounce 
// stuntime, altstuntime: set how many millisecond weapon stun victim 


// electrocute, altelectrocute: if 1, weapon add electrocute effect on victim 
// electrocutetime, altelectrocutetime: how many millisecond victim is electrocuted 
// digregate, altdisgregate: if is 1, weapon disintegrate enemy killeds 
// decloak, altdecloak: is if 1, projectile can decloack stealth enemies 
// noparry, altnoparry: is if 1, projectile cannot be parry by sabers 
// nodeflect, altnodeflect: is if 1, saber defense cannot deflect projectile 
// pushable, altpushable: is if 1, projectiles can be pushed away. 
// disgregateeffect, altdisgregateeffect: visual efx of disintegration
// melee, altmelee: shot a punch! 
// melee2, altmelee2: shot a kick! 
// meleestaff, altmeleestaff: use weapon melee for hit enemies close. 
// manarestrict: you can't use this power when this weapon is equipped. 

// --- Explosives 
// grenade, altgrenade: shoot a grenade like thermal alt that explode on impact
// timegrenade, alttimegrenade: shoot a time grenade that explode like thermalmain. 
// (use timeshot, alttimeshot, for set how many second for blow!)
// lasermine, altlasermine: shoot a laser mine
// proxymine, altproxymine: shoot a proxymine 
// zerogmine, altzerogmine: shoot a "zero g" mine that float on air 
// bomb: set a bomb that detonate with altfire
// zerogbomb: set a zero g bomb, that detonate with altfire 
// notonwater: if 1, gun can't shoot underwater 
// mainmod, altmainmod: set Means Of Damage of weapon
// poisontype, altpoisontype: set Means of Damage of poison
// poisondamage, altpoisondamage: amount of damage of poison
// poisontime, altpoisontime: how many time enemy can be poisoned

// VISUAL EFFECTS
// rayeffect, altrayeffect: efx of ray
// fleshimpacteffect, altfleshimpacteffect: effect when projectiles hit enemies 
// bounceeffect, altbounceeffect: effects when projectiles are bounced away 
// chargeeffect, altchargeeffect: effect when weapon is charged 

// DAMAGES 
// knockbackscale, altknockbackscale: how many knockback deal explosion

// ANIMATIONS
// movespeedscale: gun affect walk, run, yaw velocity of user 
// animspeedscale: gun affect animation velocity of user 
// chargeanim, altchargeanim: animations of weapon charging
// shotanim, altshotanim: animations when user fires 
// readyanim: weapon idle animation pose 
// walkanim: animation when user walk with weapon
// runanim: animation when user run with weapon 
// selectanim: animation when weapon is equipped 
// dropanim: animation when weapon is dropped 


// BLOODGUN 
//---------------------------------------------------------
void WP_FireBloodGun( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = !alt_fire ? weaponData[WP_BLASTER_PISTOL].damage : weaponData[WP_BLASTER_PISTOL].altDamage;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
		|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
	{//force sight 2+ gives perfect aim
		//FIXME: maybe force sight level 3 autoaims some?
		if ( ent->NPC && ent->NPC->currentAim < 5 )
		{
			vec3_t	angs;

			vectoangles( forwardVec, angs );

			if ( ent->client->NPC_class == CLASS_IMPERIAL )
			{//*sigh*, hack to make impworkers less accurate without affecteing imperial officer accuracy
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
			}
			else
			{
				angs[PITCH] += ( crandom() * ((5-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * ((5-ent->NPC->currentAim)*0.25f) );
			}

			AngleVectors( angs, forwardVec, NULL, NULL );
		}
	}

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t	*missile = CreateMissile( start, forwardVec, BLOODGUN_VEL, 10000, ent, alt_fire );

	missile->classname = "bloodgun_proj";
	if ( ent->s.weapon == WP_BLASTER_PISTOL )
	{//*SIGH*... I hate our weapon system...
		missile->s.weapon = ent->s.weapon;
	}
	else
	{
		missile->s.weapon = WP_BLASTER_PISTOL;
	}

	if ( alt_fire )
	{
		int count = ( level.time - ent->client->ps.weaponChargeTime ) / BLOODGUN_CHARGE_UNIT;

		if ( count < 1 )
		{
			count = 1;
		}
		else if ( count > 5 )
		{
			count = 5;
		}

		damage *= count;
		missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;

	if ( alt_fire )
	{
		missile->methodOfDeath = MOD_BLOODGUN_ALT;
	}
	else
	{
		missile->methodOfDeath = MOD_BLOODGUN;
	}

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;

	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}

//---------------
//	ARCO GLIFICO 
//---------------

//---------------------------------------------------------
void WP_FireBow ( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = !alt_fire ? weaponData[WP_BRYAR_PISTOL].damage : weaponData[WP_BRYAR_PISTOL].altDamage;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	if ( alt_fire )
{
		// FORCE ARROW
		if (	ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_TANKREAVER ||
				ent->client->NPC_class ==  CLASS_MENTALIST || 
				ent->client->NPC_class ==  CLASS_TURELIM || 				
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_GRAN || 
				ent->client->NPC_class ==  CLASS_BESPIN_COP || 
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_WIZARD || 
				ent->client->NPC_class ==  CLASS_ARCHER )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "forcearrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if (	// WIND
				ent->client->NPC_class ==  CLASS_BIRD || 
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_PROBE || 
				ent->client->NPC_class ==  CLASS_ROCKETTROOPER ||
				ent->client->NPC_class ==  CLASS_BOBAFETT )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "windarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// STONE
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_LIZARD || 
				ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_JAN ||
				ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_GALAKMECH || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 	
				ent->client->NPC_class ==  CLASS_SPIDERREAVER ||
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 ||
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF3 || 
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE ) 
	{
		{
			damage *= 1.75;
			G_PlayEffect( G_EffectIndex( "stonearrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}  
	else if (  // SOUND 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN || 
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_LANDO ||
				ent->client->NPC_class ==  CLASS_HOWLER || 
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_DRUID ||
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ) 
	{
		{
			damage *= 1.2;
			G_PlayEffect( G_EffectIndex( "sonicarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}  
	else if (	// WATER 
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_SWAMP || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_FISH ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_SWAMPTROOPER || 
				ent->client->NPC_class ==  CLASS_ICEREAVER ||
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_RAHABIM ||
				ent->client->NPC_class ==  CLASS_ALORA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST )
				{
		{
			damage *= 1.3;
			G_PlayEffect( G_EffectIndex( "waterarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if (	// SUNLIGHT 
				ent->client->NPC_class ==  CLASS_GALAK ||
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID ||
				ent->client->NPC_class ==  CLASS_PROTOCOL ||
				ent->client->NPC_class ==  CLASS_MORGANKATARN || 
				ent->client->NPC_class ==  CLASS_LIGHTREAVER ||
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_SCOUT || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO ) 
	{
		{
			damage *= 2.5;
			G_PlayEffect( G_EffectIndex( "bow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// HOLY 
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY || 
				ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_MARK1 || 
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_SARAFAN )  
				{
		{
			damage *= 3;
			G_PlayEffect( G_EffectIndex( "holyarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// DARKNESS 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER ||
				ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_DARKREAVER ||
				ent->client->NPC_class ==  CLASS_DARK || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN ) 
	{
		{
			damage *= 0.5;
			G_PlayEffect( G_EffectIndex( "blackarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// VAMPIRIC 
				ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_NOGHRI || 
				ent->client->NPC_class ==  CLASS_FLIER2 ||
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class ==  CLASS_KAIN ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY ) 
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "vamparrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if ( 	// UNDEAD 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_RANCOR ||
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_NECROREAVER ||
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_POLTER )
				{
		{
			damage *= 3;
			G_PlayEffect( G_EffectIndex( "necroarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if (	// HYLDEN 
				ent->client->NPC_class ==  CLASS_PROPHET || 	
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_SENTRY ||
				ent->client->NPC_class ==  CLASS_ATST || 
				ent->client->NPC_class ==  CLASS_MOUSE ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE || 	
				ent->client->NPC_class ==  CLASS_TENTACLE ||  
				ent->client->NPC_class ==  CLASS_ELDERGOD ) 
				{
		{
			damage *= 3.5;
			G_PlayEffect( G_EffectIndex( "glypharrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// POISON
				ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON || 		
				ent->client->NPC_class ==  CLASS_DEMON_POISON ||
				ent->client->NPC_class ==  CLASS_BEAST || 				
				ent->client->NPC_class ==  CLASS_ALCHEMIST ) 
			{
		{
			damage *= 1.2;
			G_PlayEffect( G_EffectIndex( "poisonarrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	// FIRE ARROW 
	else if 
	(	ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_RODIAN || 
				ent->client->NPC_class ==  CLASS_GLIDER || 
				ent->client->NPC_class ==  CLASS_PRISONER || 
				ent->client->NPC_class ==  CLASS_IMPERIAL || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_PLAYER || 
				ent->client->NPC_class ==  CLASS_VEHICLE || 
				ent->client->NPC_class ==  CLASS_FIREREAVER ||
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_MELC ||  
				ent->client->NPC_class ==  CLASS_SUMMONER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class ==  CLASS_DEMON_FIRE || 
				ent->client->NPC_class ==  CLASS_HUNTER )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "firearrow/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "bryar0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}	
}
else 
{
	// FORCE ARROW
		if (	ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_TANKREAVER ||
				ent->client->NPC_class ==  CLASS_MENTALIST || 
				ent->client->NPC_class ==  CLASS_TURELIM || 				
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_GRAN || 
				ent->client->NPC_class ==  CLASS_BESPIN_COP || 
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_WIZARD || 
				ent->client->NPC_class ==  CLASS_ARCHER )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "forcearrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if (	// WIND
				ent->client->NPC_class ==  CLASS_BIRD || 
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_PROBE || 
				ent->client->NPC_class ==  CLASS_ROCKETTROOPER ||
				ent->client->NPC_class ==  CLASS_BOBAFETT )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "windarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// STONE
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_LIZARD || 
				ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_JAN ||
				ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_GALAKMECH || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 	
				ent->client->NPC_class ==  CLASS_SPIDERREAVER ||
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 ||
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF3 || 
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE ) 
	{
		{
			damage *= 1.25;
			G_PlayEffect( G_EffectIndex( "stonearrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}  
	else if (  // SOUND 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN || 
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_LANDO ||
				ent->client->NPC_class ==  CLASS_HOWLER || 
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_DRUID ||
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ) 
	{
		{
			damage *= 0.75;
			G_PlayEffect( G_EffectIndex( "sonicarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}  
	else if (	// WATER 
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_SWAMP || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_FISH ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_SWAMPTROOPER || 
				ent->client->NPC_class ==  CLASS_ICEREAVER ||
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_RAHABIM ||
				ent->client->NPC_class ==  CLASS_ALORA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST )
				{
		{
			damage *= 0.8;
			G_PlayEffect( G_EffectIndex( "waterarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if (	// SUNLIGHT 
				ent->client->NPC_class ==  CLASS_GALAK ||
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID ||
				ent->client->NPC_class ==  CLASS_PROTOCOL ||
				ent->client->NPC_class ==  CLASS_MORGANKATARN || 
				ent->client->NPC_class ==  CLASS_LIGHTREAVER ||
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_SCOUT || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO ) 
	{
		{
			damage *= 1.25;
			G_PlayEffect( G_EffectIndex( "bow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// HOLY 
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY || 
				ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_MARK1 || 
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_SARAFAN )  
				{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "holyarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// DARKNESS 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER ||
				ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_DARKREAVER ||
				ent->client->NPC_class ==  CLASS_DARK || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN ) 
	{
		{
			damage *= 0.25;
			G_PlayEffect( G_EffectIndex( "blackarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// VAMPIRIC 
				ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_NOGHRI || 
				ent->client->NPC_class ==  CLASS_FLIER2 ||
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class ==  CLASS_KAIN ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY ) 
	{
		{
			damage *= 0.75;
			G_PlayEffect( G_EffectIndex( "vamparrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if ( 	// UNDEAD 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_RANCOR ||
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_NECROREAVER ||
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_POLTER )
				{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "necroarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if (	// HYLDEN 
				ent->client->NPC_class ==  CLASS_PROPHET || 	
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_SENTRY ||
				ent->client->NPC_class ==  CLASS_ATST || 
				ent->client->NPC_class ==  CLASS_MOUSE ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE || 	
				ent->client->NPC_class ==  CLASS_TENTACLE ||  
				ent->client->NPC_class ==  CLASS_ELDERGOD ) 
				{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "glypharrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	else if (	// POISON
				ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON || 		
				ent->client->NPC_class ==  CLASS_DEMON_POISON ||
				ent->client->NPC_class ==  CLASS_BEAST || 				
				ent->client->NPC_class ==  CLASS_ALCHEMIST ) 
			{
		{
			damage *= 0.7;
			G_PlayEffect( G_EffectIndex( "poisonarrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	} 
	// FIRE ARROW 
	else if 
	(	ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_RODIAN || 
				ent->client->NPC_class ==  CLASS_GLIDER || 
				ent->client->NPC_class ==  CLASS_PRISONER || 
				ent->client->NPC_class ==  CLASS_IMPERIAL || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_PLAYER || 
				ent->client->NPC_class ==  CLASS_VEHICLE || 
				ent->client->NPC_class ==  CLASS_FIREREAVER ||
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_MELC ||  
				ent->client->NPC_class ==  CLASS_SUMMONER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class ==  CLASS_DEMON_FIRE || 
				ent->client->NPC_class ==  CLASS_HUNTER )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "firearrow/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "bryar0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}	
}
	if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
		|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
	{//force sight 2+ gives perfect aim
		//FIXME: maybe force sight level 3 autoaims some?
		if ( ent->NPC && ent->NPC->currentAim < 5 )
		{
			vec3_t	angs;

			vectoangles( forwardVec, angs );

			if ( ent->client->NPC_class == CLASS_STORMTROOPER || ent->client->NPC_class == CLASS_RODIAN ||  
				 ent->client->NPC_class == CLASS_JAN || ent->client->NPC_class == CLASS_LANDO || ent->client->NPC_class == CLASS_WIZARD ||
				 ent->client->NPC_class == CLASS_IMPWORKER || ent->client->NPC_class == CLASS_IMPERIAL  ||
				 ent->client->NPC_class == CLASS_SCOUT || ent->client->NPC_class == CLASS_ARCHER )
			{//*sigh*, hack to make impworkers more accurate without affecteing imperial officer accuracy
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*1.25f));//was 0.5f
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*1.25f));//was 0.5f
			}
			else
			{
				angs[PITCH] += ( crandom() * ((5-ent->NPC->currentAim)*1.25f) );
				angs[YAW]	+= ( crandom() * ((5-ent->NPC->currentAim)*1.25f) );
			}

			AngleVectors( angs, forwardVec, NULL, NULL );
		}
	}

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t	*missile = CreateMissile( start, forwardVec, BRYAR_PISTOL_VEL, 10000, ent, alt_fire );

	missile->classname = "bow_proj";
	if ( ent->s.weapon == WP_BRYAR_PISTOL )
	{//*SIGH*... I hate our weapon system...
		missile->s.weapon = ent->s.weapon;
	}

	if ( alt_fire )
	{
		int count = ( level.time - ent->client->ps.weaponChargeTime ) / BRYAR_CHARGE_UNIT;

		if ( count < 1 )
		{
			count = 1;
		}
		else if ( count > 5 )
		{
			count = 5;
		}

		damage *= count;
		missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;

	if ( alt_fire )
	{// MOD CANNON AMPLIEFED DMG OF CHARGED ARROWS! 
		// FORCE ARROW
		// FORCE ARROW
		if (	ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_TANKREAVER ||
				ent->client->NPC_class ==  CLASS_MENTALIST || 
				ent->client->NPC_class ==  CLASS_TURELIM || 				
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_GRAN || 
				ent->client->NPC_class ==  CLASS_BESPIN_COP || 
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_WIZARD || 
				ent->client->NPC_class ==  CLASS_ARCHER )
	{
		missile->methodOfDeath = MOD_CANNON;
	}
	else if (	// WIND
				ent->client->NPC_class ==  CLASS_BIRD || 
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_PROBE || 
				ent->client->NPC_class ==  CLASS_ROCKETTROOPER ||
				ent->client->NPC_class ==  CLASS_BOBAFETT )
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_ROCKET;
	} 
	else if (	// STONE
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_LIZARD || 
				ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_JAN ||
				ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_GALAKMECH || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 	
				ent->client->NPC_class ==  CLASS_SPIDERREAVER ||
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 ||
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF3 || 
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE ) 
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_FLECHETTE;
	}  
	else if (  // SOUND 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN || 
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_LANDO ||
				ent->client->NPC_class ==  CLASS_HOWLER || 
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_DRUID ||
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ) 
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_SONIC;
	}  
	else if (	// WATER 
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_SWAMP || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_FISH ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_SWAMPTROOPER || 
				ent->client->NPC_class ==  CLASS_ICEREAVER ||
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_RAHABIM ||
				ent->client->NPC_class ==  CLASS_ALORA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST )
				{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_REPEATER;
	}
	else if (	// SUNLIGHT 
				ent->client->NPC_class ==  CLASS_GALAK ||
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID ||
				ent->client->NPC_class ==  CLASS_PROTOCOL ||
				ent->client->NPC_class ==  CLASS_MORGANKATARN || 
				ent->client->NPC_class ==  CLASS_LIGHTREAVER ||
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_SCOUT || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO ) 
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_DISRUPTOR;
	} 
	else if (	// HOLY 
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY || 
				ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_MARK1 || 
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_SARAFAN )  
				{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_DEMP2;
	} 
	else if (	// DARKNESS 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER ||
				ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_DARKREAVER ||
				ent->client->NPC_class ==  CLASS_DARK || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN ) 
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_BRYAR;
	} 
	else if (	// VAMPIRIC 
				ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_NOGHRI || 
				ent->client->NPC_class ==  CLASS_FLIER2 ||
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class ==  CLASS_KAIN ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY ) 
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_FORCE_DRAIN;
	} 
	else if ( 	// UNDEAD 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_RANCOR ||
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_NECROREAVER ||
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_POLTER )
				{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_CONC;
	}
	else if (	// HYLDEN 
				ent->client->NPC_class ==  CLASS_PROPHET || 	
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_SENTRY ||
				ent->client->NPC_class ==  CLASS_ATST || 
				ent->client->NPC_class ==  CLASS_MOUSE ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE || 	
				ent->client->NPC_class ==  CLASS_TENTACLE ||  
				ent->client->NPC_class ==  CLASS_ELDERGOD ) 
				{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_EMPLACED;
	} 
	else if (	// POISON
				ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON || 		
				ent->client->NPC_class ==  CLASS_DEMON_POISON ||
				ent->client->NPC_class ==  CLASS_BEAST || 				
				ent->client->NPC_class ==  CLASS_ALCHEMIST ) 
	{
		missile->methodOfDeath = MOD_CANNON;
		missile->methodOfDeath = MOD_POISON;
	} 
	// FIRE ARROW 
	else if 
	(	ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_RODIAN || 
				ent->client->NPC_class ==  CLASS_GLIDER || 
				ent->client->NPC_class ==  CLASS_PRISONER || 
				ent->client->NPC_class ==  CLASS_IMPERIAL || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_PLAYER || 
				ent->client->NPC_class ==  CLASS_VEHICLE || 
				ent->client->NPC_class ==  CLASS_FIREREAVER ||
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_MELC ||  
				ent->client->NPC_class ==  CLASS_SUMMONER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class ==  CLASS_DEMON_FIRE || 
				ent->client->NPC_class ==  CLASS_HUNTER )
		{	
			missile->methodOfDeath = MOD_CANNON;
			missile->methodOfDeath = MOD_BLASTER;
	
		}//missile->methodOfDeath = MOD_CANNON_ALT;
	}
	else
	{// Normal arrows does only elemental dmg. 
		// FORCE ARROW
		if (	ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_TANKREAVER ||
				ent->client->NPC_class ==  CLASS_MENTALIST || 
				ent->client->NPC_class ==  CLASS_TURELIM || 				
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_GRAN || 
				ent->client->NPC_class ==  CLASS_BESPIN_COP || 
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_WIZARD || 
				ent->client->NPC_class ==  CLASS_ARCHER )
	{
		missile->methodOfDeath = MOD_CANNON;
	}
	else if (	// WIND
				ent->client->NPC_class ==  CLASS_BIRD || 
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_PROBE || 
				ent->client->NPC_class ==  CLASS_ROCKETTROOPER ||
				ent->client->NPC_class ==  CLASS_BOBAFETT )
	{
		missile->methodOfDeath = MOD_ROCKET;
	} 
	else if (	// STONE
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_LIZARD || 
				ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_JAN ||
				ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_GALAKMECH || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 	
				ent->client->NPC_class ==  CLASS_SPIDERREAVER ||
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 ||
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF3 || 
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE ) 
	{
		missile->methodOfDeath = MOD_FLECHETTE;
	}  
	else if (  // SOUND 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN || 
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_LANDO ||
				ent->client->NPC_class ==  CLASS_HOWLER || 
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_DRUID ||
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ) 
	{
		missile->methodOfDeath = MOD_SONIC;
	}  
	else if (	// WATER 
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_SWAMP || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_FISH ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_SWAMPTROOPER || 
				ent->client->NPC_class ==  CLASS_ICEREAVER ||
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_RAHABIM ||
				ent->client->NPC_class ==  CLASS_ALORA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST )
				{
		missile->methodOfDeath = MOD_REPEATER;
	}
	else if (	// SUNLIGHT 
				ent->client->NPC_class ==  CLASS_GALAK ||
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID ||
				ent->client->NPC_class ==  CLASS_PROTOCOL ||
				ent->client->NPC_class ==  CLASS_MORGANKATARN || 
				ent->client->NPC_class ==  CLASS_LIGHTREAVER ||
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_SCOUT || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO ) 
	{
		missile->methodOfDeath = MOD_DISRUPTOR;
	} 
	else if (	// HOLY 
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY || 
				ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_MARK1 || 
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_SARAFAN )  
				{
		missile->methodOfDeath = MOD_DEMP2;
	} 
	else if (	// DARKNESS 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER ||
				ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_DARKREAVER ||
				ent->client->NPC_class ==  CLASS_DARK || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN ) 
	{
		missile->methodOfDeath = MOD_BRYAR;
	} 
	else if (	// VAMPIRIC 
				ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_NOGHRI || 
				ent->client->NPC_class ==  CLASS_FLIER2 ||
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class ==  CLASS_KAIN ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class ==  CLASS_RAZIELHIM ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY ) 
	{
		missile->methodOfDeath = MOD_FORCE_DRAIN;
	} 
	else if ( 	// UNDEAD 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_RANCOR ||
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_NECROREAVER ||
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_POLTER )
				{
		missile->methodOfDeath = MOD_CONC;
	}
	else if (	// HYLDEN 
				ent->client->NPC_class ==  CLASS_PROPHET || 	
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_SENTRY ||
				ent->client->NPC_class ==  CLASS_ATST || 
				ent->client->NPC_class ==  CLASS_MOUSE ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE || 	
				ent->client->NPC_class ==  CLASS_TENTACLE ||  
				ent->client->NPC_class ==  CLASS_ELDERGOD ) 
				{
		missile->methodOfDeath = MOD_EMPLACED;
	} 
	else if (	// POISON
				ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON || 		
				ent->client->NPC_class ==  CLASS_DEMON_POISON ||
				ent->client->NPC_class ==  CLASS_BEAST || 				
				ent->client->NPC_class ==  CLASS_ALCHEMIST ) 
	{
		missile->methodOfDeath = MOD_POISON;
	} 
	// FIRE ARROW 
	else if 
	(	ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_RODIAN || 
				ent->client->NPC_class ==  CLASS_GLIDER || 
				ent->client->NPC_class ==  CLASS_PRISONER || 
				ent->client->NPC_class ==  CLASS_IMPERIAL || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_PLAYER || 
				ent->client->NPC_class ==  CLASS_VEHICLE || 
				ent->client->NPC_class ==  CLASS_FIREREAVER ||
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_MELC ||  
				ent->client->NPC_class ==  CLASS_SUMMONER ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class ==  CLASS_DEMON_FIRE || 
				ent->client->NPC_class ==  CLASS_HUNTER )
	{
		missile->methodOfDeath = MOD_BLASTER;
	}
	//missile->methodOfDeath = MOD_CANNON;
}

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;

	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}

//---------------
//	Sonic Jawa 
//---------------

//---------------------------------------------------------
void WP_FireSonic ( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = !alt_fire ? weaponData[WP_JAWA].damage : weaponData[WP_JAWA].altDamage;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	 if ( alt_fire )
{
	// LANDO CLASS DOES ASTRAL SHOT WITH DOUBLE DMG!  
	if ( ent->client->NPC_class == CLASS_LANDO )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "astral/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// HOWLER CLASS DOES 1.5 DMG 
	else if ( ent->client->NPC_class == CLASS_HOWLER || ent->client->NPC_class == CLASS_SONIC ||
		ent->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "howler/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// OTHER CLASS DOES DEFAULT DMG 
	else if ( ent->client->NPC_class != CLASS_LANDO && ent->client->NPC_class != CLASS_HOWLER &&
		ent->client->NPC_class != CLASS_SONIC && ent->client->NPC_class != CLASS_GOLEM_SONIC )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "howler/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
else 
{
	// LANDO CLASS DOES ASTRAL SHOT WITH DOUBLE DMG!  
	if ( ent->client->NPC_class == CLASS_LANDO )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "astral/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_HOWLER || ent->client->NPC_class == CLASS_SONIC ||
		ent->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "howler/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// OTHER CLASS DOES DEFAULT DMG 
	else if ( ent->client->NPC_class != CLASS_LANDO && ent->client->NPC_class != CLASS_HOWLER &&
		ent->client->NPC_class != CLASS_SONIC && ent->client->NPC_class != CLASS_GOLEM_SONIC )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "howler/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}



	if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
		|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
	{//force sight 2+ gives perfect aim
		//FIXME: maybe force sight level 3 autoaims some?
		if ( ent->NPC && ent->NPC->currentAim < 5 )
		{
			vec3_t	angs;

			vectoangles( forwardVec, angs );

			if ( ent->client->NPC_class == CLASS_HOWLER || ent->client->NPC_class == CLASS_LANDO )
			{//*sigh*, hack to make impworkers less accurate without affecteing imperial officer accuracy
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*1.25f));//was 0.5f
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*1.25f));//was 0.5f
			}
			else
			{
				angs[PITCH] += ( crandom() * ((5-ent->NPC->currentAim)*1.25f) );
				angs[YAW]	+= ( crandom() * ((5-ent->NPC->currentAim)*1.25f) );
			}

			AngleVectors( angs, forwardVec, NULL, NULL );
		}
	}

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t	*missile = CreateMissile( start, forwardVec, BRYAR_PISTOL_VEL, 10000, ent, alt_fire );

	missile->classname = "sonic_proj";
	if ( ent->s.weapon == WP_JAWA )
	{//*SIGH*... I hate our weapon system...
		missile->s.weapon = ent->s.weapon;
	}
	
	if ( alt_fire )
	{
		int count = ( level.time - ent->client->ps.weaponChargeTime ) / BRYAR_CHARGE_UNIT;

		if ( count < 1 )
		{
			count = 1;
		}
		else if ( count > 5 )
		{
			count = 5;
		}

		damage *= count;
		missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;

	if ( alt_fire )
	{
		missile->methodOfDeath = MOD_SONIC_ALT;
	}
	else
	{
		missile->methodOfDeath = MOD_SONIC;
	}

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;

	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}




//----------------------------------------------------------------------
// COSE DA FAR NEL CODICE 
/* 1 CODICE ARMI - COSE DA COMPLETARE 

X	char		*Gun_Name;						//entry in .GUN, if any
	char		*Gun_FullName;					//the "Proper Name" of the saber, shown in the UI
	char		*Gun_Model;					// Gun model:
- Non persiste nei salvataggi, sempra che però venga salvato con g_gun, ma perde e non memorizza la skin.
- Gli NPC perdono il modello dopo 2 salvataggi, dopodiché l'altfire crasha. 

	char		*Gun_Skin;					// Gun custom skin
	
	// Sounds - CHARS OR INT???
	char			*Gun_Missile_Model;		// Missile model for main shot
	char			*Gun_Alt_Missile_Model; // Missile model for Alt Shoot. 
	char			*Gun_Missile_Sound;		// Sound made from missile of weapon, like a rocket loop
	char			*Gun_Alt_Missile_Sound; // Sound made from alt missile of weapon, like a rocket loop
	char			*Gun_Charge_Sound;		// sound to start when the weapon initiates the charging sequence
X	char		   	*Gun_Alt_Charge_Sound;	// alt sound to start when the weapon initiates the charging sequence
X	char			*Gun_Select_Sound;	// the sound to play when this weapon gets selected
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
	Gun_t	Gun_Icon;		// Name of weapon icon file
	Gun_t	Gun_Desc;		// String Descriprion of Gun
	Gun_t	Gun_DisplayName; // The name which is to be displayed on the HUD.
	char		Gun_g2MarksShader[MAX_QPATH];	//none - if set, the game will use this shader for marks on enemies instead of the default "gfx/damage/saberglowmark"
	char		Gun_g2WeaponMarkShader[MAX_QPATH];	//none - if set, the game will ry to project this shader onto the weapon when it damages a person (good for a blood splatter on the weapon)
	gun_holster_locations_t gun_holsterPlace; // When you put the weapon where are not using it. 


	--------------------------- GRAPPLE HOOK----------------------------
	// Seguire il tutorial di quake 3 arena. 
	
	-------------------------------------- FORCE POWER LIBRARY DEFINITIONS! 
	// Name 
	Magic_Name;
	Magic_FullName; 
	//  General (all this parameters are from level 1 to level 5)
	MagicCoast;  // How many Magic Point coast to cast a power. 
	MagicRequireCoast; // How many Magic Point you need to have for cast the spell.
	MagicLoagindSound: Sound played when you Load the magic. 
	MagicCastSound: Sound Played when you cast the spell
	MagicLoopSound: Sound Played when spell is active
	MagicOffSound: Sound Played when spell stop. 
	Activation: if is 1, Power effect can be used for activate stuff like switchs etc
	Breaking: if is 1, Power can damage func_brekeable 
	LockMovement: if is 1, player can't move when use the power
	NoFight: if is 1, fighting and aggressive behavour break the spell
	LoadAnimation: animation played when you load the magic power
	CastAnimation: animation played when you cast the magic power
	LoopAnimation: animatiojn played when magic power is active
	StopAnimation: Animatiojn playeed when magic power stop

	// 0 Force Sense: 
	SenseMask: // 2dshader mask of force sense
	DeadlySight: if is 1, Enemy his hitted if you look at him.
	DeadlySightDamage: Amount of Damage enemy get if you are l
	DeadlySightType: what's happen to enemy hitted by deadly sight? electrocuted? poisoned?
	DeadlySightDisintegration: enemy killed are disintegratedd 
	HealSight: if is 1, Allied is healed if you are look him
	HealSightAmount: how many HP are restored for second wathinc an allied.
	AimFixAmount: how many reduce the spread and increase the precision of gunshoots
	
	// 1 Magic Jump: 
	JumpVector: 0 is jump up to down. 1 push player forward. 2 push player backward
	3 strafe the player left, 4 strafe the player right. 
	JumpFX: effect played when you use the jump power. 
	FlyFX: effect played when you use the fly power
	FlyLoopFX: effect played when you are flying
	FlyLoopSound: Sound played when you are flying
	SwimFx: effect played when you use the swim power
	SwimLoopFX: effect played when you use the swim power
	SwimLoopSound: sound effect played when you use the swim sound.
	
	// 2 Magic Speed 
	TimeScaleAmount: Amount of distorsion of timescale
	TimeScaleLoopSound: 
	TimeScaleLoopEffect: shader of timescale enabled
	// Magic Saber Defense
	
	3 DeflectAmount: amount of deflectionrate of saber weapon defense
	Deflecting: if is 1, shield deflect attack back to the enemies. 
	// if is 2, it deflect also medium weapon shoots. 
	// if is 3, it deflect also heavy weapon shoots. 
	DeflectPrecision: how many precise is to deflect bolts against shooter body back.
	DeflectFX: effect played by deflection
	4 // Magic Saber Throw: 
	How many precise is the weapon throwing boomerang. 

	//5 Magic Offense Buff:
	How many Sword or other melee weapons damage are increased by the buff?
	What weapons are affected? 
	
	// 6 Magic Push
	PushForce/PullForce: 1 just slam, 2 cause damage  3 poison the victim. 
	4 detach body if is killed 5 detach all body 
	6 disgregate enemy killed. 
	Push/PullType: what happen to enemy pushed\pulled? is electrocuted? stunned?
	paralized?
	Push/Pull heal or buff: can Buff allied or health?
	PushKnockbackforce:  force of the Pushing
	PushHFov: how many large is the shockwave
	PushVFov: how many height is the shockwave
	PushShader: shader blur of the shockwave
	PushFX: visual effect of the shockwave 
	// 7 Magic Pull 
	PullKnockbackforce:  intensity of the Pulling 
	PullHFov: how many large is the pulling
	PullVFov: how many height is the pulling
	PullShader: shader blur of the pullin
	PushFX: visual effect of the pulling
	PullingWeapon: if 1, enemy shootgun are pulled out by their hands. 


	// 8 HEAL
	Healamount: total amount of how many HP are healed
	Healrate: how many HP by second are restored
	HealFX: visual effect of Heal
	// 9 Protect
	MagicProtectShader: shader color that surroud protected body-
	MagicProtectFX: fx that surround the protected body. 
	MagicProtectDefenseAmount: amount of damage reduction against physical attack
	MagicProtectException: Lista dei MODS (means of damage) that are not shielded by Protection.

	// 10 Absorb
	AbsorbShader: shader color that surroud absorbing mana body-
	AbsorbFX: fx that surround the absorbing mana body. 
	AbsorbDefenseAmount: amount of magical damage reduction against magical attack
	AbsorbConversionAmount: amount of mana poin restored by absorb when player his hitted by black magic
	AbsorbException: Lista degli incantesimi che non sono neutralizzati dal potere di assorbimento
	
	// 11 Buff and Curse
	BuffType: what kind of buff you cast on enemy: protection, absorbing, moving, etc
	BuffProtectException: Physical Buff barrier will not protect against this physical damage
	BuffAbsorbException: Magical Buff barrier will njot protect against this damage
	(Altri del Buff prendili dagli status degli altir poteri)
	BuffShader: shader surround Buffed Allied.
	BuffFx: Effect surround buffed allied. 
	IsOnPlayer: if is 1, buff affect also the magic caster. 

	// 11 Curse 
	CurseType: what kind of cursing is: blinding, soffocating, electrocuting etc. 
	CurseDamageAmount;
	CursePoisonDamageAmount
	CursePoisonTime
	Disgregation: if enemy die, curse disgregate is body. 
	CurseHeadFX: effect that surround head of cursed enemy
	CurseBodyFX: effect that surround body of cursed enemy
	CurseLegsFX: effects that surround legs of cursed enemy
	CurseShader: Shader that surround cursed enemy.
	Auravisible: is 1, enemy is auravisible
	 
	// 12 Burst 
	BurstBuff: can buff allied hitted by this?
	BurstHeal: can Heal allied hitted by this?
	BurstType: what kind of burst affect the hitted enemy
	BurstDamageAmount
	BurstPoisonAmount
	BurstPoisonTime
	BurstDist: max distance of burst shoot
	BurstHFov: Burst Horizontal Range
	BurstVFox: Burst Vertical Range
	DoubleBurst - if is 1, shoot with both hands 
	BurstHandFX: effect played on the hand of wizard
	BurstFX: burst visual effect
	BurstShader: burst surround hitted enemy
	BurstWallImpactFX: effect of burst when impact against walls
	BurstHitFX: effect of burst when impact against folks 
	BurstMOD: means of damage of Burst 
	BurstDisgregation: if 1, enemy killed are disgregated 

	// 13 Drain . 
	DrainType: what kind of drain affect the hitted enemy
	DrainDamageAmount
	DrainRate
	DrainDist: distance of draining
	DrainHFov: Drain Horizontal Range
	DrainVFox: Drain Vertical Range
	DrainPoisonAmount: Drain amount of poisoning
	DrainPoisonTime: time duration of drain poison
	DoubleDrain - if is 1, drain with both hands 
	DrainHandFX: effect played on the hand of wizard
	DrainFX: burst visual effect
	DrainShader: shader surrounding drained enemy
	BurstWallImpactFX: effect of burst when impact against walls
	BurstHitFX: effect of burst when impact against folks 
	BurstMOD: means of damage of Burst 
	BurstDisgregation: if 1, enemy killed are disgregated 
	DrainAnimation: animation played of drained enemy
	DrainGrabAnimation: animation played of grabbed enemy,
	
	// 14 Grip
	GripForce: 1 just stuck enemy, 2 lift enemy on air, 3 cause damage. 
	4 detach enemy head if is killed 5 detach all body 
	6 disgregate enemy killed. 
	GripType: what's happen to gripped enemy
	GripDamageAmount 

	// 15 Rage
	HealthLost: how many health you loose using the power 
	ArmorLost: how many armor lose using the power 
	HealthDrainAmount: How many health consume using rage for seconde
	ArmorDrainAmount: how many armor you consume using rage for second?
	Invulnerability? 1 - character is on undying 2 - god mode when power is activated
	ForceAmount: How many is strenghten enemy that use it. 
	DefenseAmount: How many is strenghted the physical defense of the caster 
	AggressionAmount: How many is increased the damage of the weapons you are using
	SpeedAmount: Howmany is increased the speed of enemy that use it. 

	// 16 Magic Shoot (the wrack) 
	// - Tutti i parametri personalizzabili di un Magic Shoot 
	// corrispondono a quelli della libreria delle armi da fuoco.
	// 17 Magic Stun: Stuntime amount 
	StunnedFX, Stunnedshader
	// 18 Magic Charm: CharmType: fear? TeamChange? 
	// CharmedFX, CharmedShader
	// 19 Possessione: PossessionType: Normal? TeamChange? 
	// Suicide? (when you left possessed foe he suicide himself.
	PossessionFX, PossessionShader
	// 20 HealOther:
	- How Many HP are restored to Allied
	- Is Poison removed?
	-  what kind of debuffs is removed?
	- can resurrect a dead?
	HealOtherFX
	HealOtherShader
	// 21 Freeze: 
	- How many time enemy is freezed?
	- can be freezed more enemies?
	- Also PoisonDamage inflicted? 
	// Morphing and Summong: SEE Weapon Code. 
	AOE ATTACKS: Valgono tutti i parametri di force push\pull, Burst, Buff e Drain
	gli Attacchi AOE posson far questo, come anche Heal e curare gli alleati oppure
	avvelenare, o paralizzare i nemici o maledirli etc etc etc, con o senza onde d'urto
	22 dark
	23 light
	24 fire 
	25 air 
	26 water 
	27 earth 
	28 sound/psychic
	29 wood/Metal
	30 spirit/holy
	31 poison/necro
	
	---------------------------------------- MODS AND SPECIAL COMBAT LIBRARY DEFINITIONS ---------------
	AuraColor: NPC Aura Color for Force Sense, for team neutral and team all 
	EnemyAuraColor: NPC aura color if is an enemy
	AlliedAuraColor: NPC aura color if is allied 


extern stringID_table_t ArmorTable[];

// IDEA FOR FORCE POWER AS EXTERNAL DATA AL POSTO DI 
// DATA PER VEICOLI E AI
// TIPI POTERE: HEAL, RAGE, BOOST (speed), MOVE, CHARM, 
// BUFF, CURSE, SHIELD, ABSORB, FORCEFIELD, BUERST, DRAIN,
// BOLT, SIGHT, AOE, PUSH, PULL, Ten level of powers.
typedef enum //# AI_e
{
	AI_NONE,
	// THE ENEMY THAT USE THIS FIELD USE ALSO THE AI OF THIS CLASS!  
	AI_DEFAULT, // default AI 
	// ALIENS ANIMALS AI 
	AI_MINEMONSTER, // little land bug monsters generic earth monster. NOT jump! 
	AI_BUG,// like a little flying insect. they attack as Remote! 
	AI_BUG2, // they attack as interrogator. poison insects. 
	AI_POISONER, // like bug but on ground
	AI_FERAL, // wampa feral AI
	AI_BEAST, // rancor ai! for big animals monsters!  
	AI_HOWLER, // act as an howler
	AI_JUMPER, // as howler but not jump
	AI_FLYER, // NPC can fly and attack as a simple bird. use just one weapon.
	AI_FLOATFLY, // as probe, but animal. used for Icarus floating creatures 
	AI_FALKOR, // falkor attacker AI 
	AI_MAGICFLYER, // altfire, switch weap. improved wizard AI and flying creature! 
	AI_FLYBEAST, // flying and deadly as wampa
	AI_DRAGON, // flying and deadly as a rancor! 
	AI_MARCHER, // forced march AI 
	AI_FISH, // for underwater little enemies 
	AI_FISH2, // fish as remote attack 
	AI_EDGEFISH, // fish as sentry, attack. immune to water cannoit fly over water. 
	AI_FISHWALKER, // they walk into water ground 
	AI_FLOATFISH, // float into water, squids, jellyfish, etc. 
	AI_OCTOPUS, // deadly as Wampa, underwater 
	AI_SHARK, // deadly as Rancor but Underwater! 
	AI_GROUNDIVER, // Sand creature AI
	AI_SCREAMER, // an howler, but not jump silly 
	// HUMANOID AI
	AI_CIVILIAN, // Civilian AI 
	AI_RAIDER, // use tusken AI 
	AI_RIFLE, // sniper user with rifle 
	AI_SNIPER, // use disruptor AIM AI 
	AI_TROOPER, // generic AI soldier
	AI_GRENADIER, // granatier  
	AI_CHARMED, // posssesd AI.
	AI_ALTFIRE, // use alt fire attack of all weapons and switch weapons. 
	AI_SUPERTROOPER, // supersoldier with improved wizard AI. not shoot to teammate
	// not silly chase the player. more strategical switch weap and altfire.
	AI_FENCER, // old jedi AI // the most diffuse enemy AI. 
	AI_CULTIST, // force only wizard 
	AI_ARCANE, // super wizard AI attacker 
	AI_ROCKETTROOPER, // Rockettrooper NPC AI simple flier NPC 
	AI_FLAMETHROWER, // arcane AI but with flamethrower 
	AI_BOBA, // bobafett NPC but killable , and with flamethrower! 
	AI_JETPACK, // jetpack user user AI. As Boba fett, but NOT flamethrower
	AI_EMPLACED, // act like a cannon emplaced gun 
	AI_KAMIKAZE, // act like a cultist destroyer 
	// GENERIC DROID AI 
	AI_DROID, // droid 
	AI_ROBOT, // Static AI for characters as Protocol droid
	AI_MECH, // AI for character as R2D2. 
	AI_PROBE, // as probe droid 
	AI_SEEKER, // seeker AI
	AI_REMOTE, 
	AI_SENTRY, 
	AI_INTERROGATOR, // Torure droid 
	AI_MARK1, // mark1 droid
	AI_MARK2, // Mark2 droid
	AI_TORTURER, // torturator, interrogator AI + Saber Droid AI. 
	AI_SCREAMERDROID, // howler + droid can't jump. 
	AI_FENCERDROID, // like a saber droid  
	AI_KILLERDROID, // like a shielded assassin droid  
	AI_ATST, // as an ATST walker  
	// UNDEADS SPECIAL AI
	AI_ZOMBIE, // dive ground as a sand creature, attack as Zombie \ Melchiahim
	AI_WALLCLIMBER, // Zephonim wallclimber AI. 
	AI_SPIDER, // Spider AI wallclimbing and web texture. 
	AI_CREEPY, // a puppet that attack as default AI but with head tilting! 
	AI_POLTERGEIST, // creepy ghost fly, not tilting AI. 
	AI_GHOST, // as a creepy ghost fly, as Ovlon or Poltergeists.fly as probe,	AI_VOD, // voice of darkness AI.
	AI_ORLAN, // floating godness with switch weapons and altfire. 
	// ASTROFANFANTASY SPACE COMBAT AND BATTLE ON WORLD AI! 
	// Sea Vehicles AI 
	// landing troop AI
	AI_SKULLDROID, // astrofantasy skull droid AI 
	AI_TANK,// Astrofantasy giant tunk
	AI_MANX, // astrofantasy manx walker AI
	AI_WALKER, // Astrofantasy giant walker 
	AI_TURRET, // Astrofantasy Turret. main fire laser, alt fire rockets 
	AI_TURRET2, // turret that shoot missiles
	AI_TURRET3, // Turret that shoot epulse weapons ionic
	AI_TURRET4, // Turret with multiple weapons! switching + alt  
	// LAVA CREATURES
	AI_FORGER, // swim into the lava! 
	AI_LAVASKIMMER, // boat then go over the lava.
	// Water vehicles 
	AI_BOAT, // Astrofantasy Boat AI 
	AI_SKIMMER, // cruiser that go on the ater. 
	AI_SUBMARINE, // submarine AI
	// SKY AND SPACE VEHICLES 
	AI_FIGHTER, // StarFighters Ai 
	AI_VEHICLE, // is an NPC but is drivable? // FOR PLAYERS?!?!?
	AI_ANIMAL, // riding. 
	AI_SPEEDER, 
	AI_CAR, 
	AI_OLON, // player drivable fighter 
	// Astrofantasy space fight AI. 
	AI_ASTROTURRET, // static float enemy that shoot as Turret Entity. 
	AI_SAT, // satellite
	AI_MINE, // proxy space mine
	AI_WATERMINE, // floating water mine
	AI_LAVAMINE, // floating lava mine. 
	AI_PROXYMINE, // underwayer mine.
	AI_TUG, // tug rimochiatore. 
	AI_PACMAN, // little patrol ship 
	AI_CORVETTE, // medium ship 
	AI_CRUISER, // Astrofantasy starship Cruiser AI 
	AI_VECTOR, // Vector Cruiser AI
	AI_DESTROYER, // Destroyer Cruiser Ai 
	AI_DREADNOUGHT, // Armored Ship AI 
	AI_STATION, // Astrofantasy Station AI. 
	// FROM MOVIE BATTLE 2. 
	NUM_AI, 
} AI_t;
// THIS ALLUE TO USE CUSTOM AI 
extern stringID_table_t AITable[];

typedef enum //# COMBAT_e - ADD THE MOVIE BATTLE FEATURES.
{
	COMBAT_NONE, // nothing of special 
	COMBAT_BOXER: // use special kata like kyle boss 
	// ASTROFANTASY RACES 
	COMBAT_SEMIBOSS, // using semiboss AI as rosh or alora
	COMBAT_ACROBATIC, // Alora Jumps Enabled!  
	COMBAT_BOSS, // using boss AI. 
	COMBAT_SORCERER, // the guys use only force powers, as emperor, cultis, and kothos NPC.
	COMBAT_SHOOTMASTER, // the guy use two logged weapons, as Cultist Commando. 
	COMBAT_BOMB, // the guy blow up as a bomb, like 
	COMBAT_THROWER, // the guys throw back player as hazardtrooper
	COMBAT_SLAMMER, // the guy can serious injure foes as a rancor or a wampa, with huge knockback. 
	COMBAT_CLOAKED, // enemy can cloack as a shadowtrooper. 
	COMBAT_SNIPER, // sniper shooter and cloaked 
	COMBAT_FIGHTER, // fly as a fighter
	COMBAT_BIRD, // flying creature movement. 
	COMBAT_SCUBA, // underwater foe. water immunity, submarine movemnet. 
	COMBAT_SHIELD, // the foe have a physical shield as galak mech droid
	COMBAT_FIREFROSTSHIELD, // shield deal ice and fire dmg 
	COMBAT_SOLIDWINDSHIELD, // shield of earth, solid + repulsion 
	COMBAT_ASTRALSHIELD, // shield deal astral dmg. 
	COMBAT_CLOAKSHIELD, // enemy have a shield of darkess
	COMBAT_SUNSHIELD, // sun shield . enemy killed are disgregated.
	COMBAT_SHOCKSHIELD, // Enemy electrocute fors with his shield
	COMBAT_FIRESHIELD, // enemy with fire shield and assassin droid AI. 
	COMBAT_LAVASHIELD, // hell shield 
	COMBAT_DRAINSHIELD, // vampiric shield Drean life as force drain 
	COMBAT_HELLSHIELD, // Demonic shield burn + drain 
	COMBAT_REPULSESHIELD, // shield with wind and gthrow when activate. 
	COMBAT_WATERSHIELD, // enemy use a shield that protect from drown.
	COMBAT_FROSTSHIELD, 
	COMBAT_CRYSTALSHIELD, // a shield of crystal. deflecting bolts.
	COMBAT_SLIMESHIELD, // a shield of slime \ acid poison and melt enemy
	COMBAT_STONESHIELD, // physical shield, also, enemy have weapontime if they it. 
	COMBAT_POISONSHIELD, // Enemy have a shield of poison dmg
	COMBAT_ALCHSHIELD, // enemy have an alchemic shield
	COMBAT_WOODSHIELD, // enemy have a shield of ivy \ nature
	COMBAT_PSYCOSHIELD, // a shield that protect from all mind charm powers.
	COMBAT_SONICSHIELD, // shield sonic. sonicpain if you are hitted. 
	COMBAT_HOLYSHIELD, // shield is deadly for undeads. 
	COMBAT_NECROSHIELD, // shield is deadly for undeads. 
	COMBAT_TIMESHIELD, // shield inflict time dmg and timescale alteration. 
	COMBAT_DROID, // this guys is immune to force spells as a droid 
	COMBAT_NOFORCE, // this guy is immune to force power attacks! 
	COMBAT_NOBULLETS, // the guy cannot receive any kind of dmg from bullet weapons. only saber can kill!
	COMBAT_HEAVYWEAP, // this foe is weak only to heavy weapons 
	COMBAT_NOSABER, // enemy cannot be killed by saber weapons. 
	COMBAT_SENTRY, // release a sentry turret
	COMBAT_REMOTE, // release remote turrets
	COMBAT_DRONE, // drone
	COMBAT_FLAMETHROWER, // enemy use flamethrower as bobafett 
	COMBAT_ARMOUR, // enemy have armor
	COMBAT_RESPAWN, // enemy respawn when is killed!  
	COMBAT_DODGE, // dodge as jedi!
	COMBAT_HEALING, // heal the others NPCs. 
	COMBAT_ARCANESHIELDED, // skin deflect arcane projectiles
	COMBAT_BULLETSHIELDED, // skin deflectshoot weapons
	COMBAT_MAGNETICSHIELDED, // resist to phaser weapons 
	COMBAT_SHOCKWAVE, // enemy is stating and costantly produce shockwave
	// of his AOE force powers 
	COMBAT_SHIELD_RECHARGE, 
	COMBAT_HEALTH_RECHARGE, 
	COMBAT_DISGUISE, 
	NUM_COMBAT, 
} SpecialCombat_t;
	------------------------------------------- FX AMBIENTALI ----------------------
	Son segnati negli attacchi magici weapons.h
	---------------------------------------------- PORTE ROTANTI--------------------
	quake3 tutorial 
	-------------------------------------- CODICE CACCIA -----------------------------
	- first person cockpit
	- weapon system
	- NPC manovre d'attacco ed evasive 
	- AI varie 
	--------------------------------------- SAVEGAME E DEBUGGING ------------------
	









	*/