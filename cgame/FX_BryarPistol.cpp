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

// Bryar Pistol Weapon Effects

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
-------------------------

	MAIN FIRE

-------------------------
FX_BryarProjectileThink
-------------------------
*/
void FX_BryarProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
	{
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{

			
		if ( cent->gent->owner->client->gs.gun->Gun_Bullet_Effect )
			{
				gi.Printf( S_COLOR_GREEN "Customize Shot Effect from *.GUN File! None = gun/shot" , cent );
				G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_Bullet_Effect, cent->lerpOrigin, forward  );  
				//theFxScheduler.PlayEffect( cent->gent->owner->client->gs.gun->Gun_Bullet_Effect, cent->lerpOrigin, forward );
			}

		/*
			switch ( cent->gent->owner->client->NPC_class )
			{
				// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// animali 
				case CLASS_BEAST: // animals 
				case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti
				//theFxScheduler.PlayEffect( "dark/NPCshot", cent->lerpOrigin, forward );
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "knife/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL2:// life reaver class 
					G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				G_PlayEffect( "smoke/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Boss del Fumo, Respen con la Mieti di Fumo
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/NPCshot" , cent->lerpOrigin, forward  );  
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
	     		G_PlayEffect( "stun/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				G_PlayEffect( "time/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dust/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "silence/NPCshot" , cent->lerpOrigin, forward  );  
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
		       	G_PlayEffect( "void/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoke/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "salt/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/NPCshot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/NPCshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoker/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/NPCshot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/NPCshot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/NPCshot" , cent->lerpOrigin, forward  );  
				break;
				
           }*/
		}
	}
	else
	{
		
	   if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{

			if ( cent->gent->owner->client->gs.gun->Gun_Bullet_Effect )
			{
				gi.Printf( S_COLOR_GREEN "Customize Shot Effect from *.GUN File! None = gun/shot" , cent );
				G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_Bullet_Effect, cent->lerpOrigin, forward );  
			}

			/*
			switch ( cent->gent->owner->client->NPC_class )
			{
				// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				//theFxScheduler.PlayEffect( "dark/shot", cent->lerpOrigin, forward );
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "knife/shot" , cent->lerpOrigin, forward  );  
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/shot" , cent->lerpOrigin, forward  );  
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/shot" , cent->lerpOrigin, forward  );  
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL2:// life reaver class 
					G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				// Boss del Fumo, Respen con la Mieti di Fumo
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/shot" , cent->lerpOrigin, forward  );  
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/shot" , cent->lerpOrigin, forward  );  
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
	     		G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/shot" , cent->lerpOrigin, forward  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				G_PlayEffect( "time/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/shot" , cent->lerpOrigin, forward  );  
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/shot" , cent->lerpOrigin, forward  );  
				break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dust/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "silence/shot" , cent->lerpOrigin, forward  );  
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
		       	G_PlayEffect( "void/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/shot" , cent->lerpOrigin, forward  );  
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "salt/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/shot" , cent->lerpOrigin, forward  );  
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoker/shot" , cent->lerpOrigin, forward  );  
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/shot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/shot" , cent->lerpOrigin, forward  );  
				break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/shot" , cent->lerpOrigin, forward  );  
				break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/shot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/shot" , cent->lerpOrigin, forward  );  
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/shot" , cent->lerpOrigin, forward  );  
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/shot" , cent->lerpOrigin, forward  );  
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/shot" , cent->lerpOrigin, forward  );  
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/shot" , cent->lerpOrigin, forward  );  
				break;
			default:
				break;
			}*/
		}
	}
}

void FX_BowProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
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
				theFxScheduler.PlayEffect( "forcearrow/NPCshot", cent->lerpOrigin, forward );
				break; 
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				theFxScheduler.PlayEffect( "windarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "stonearrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "sonicarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "waterarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "bow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "holyarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "blackarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "vamparrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "necroarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "glypharrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "poisonarrow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "firearrow/NPCshot", cent->lerpOrigin, forward );
				break;  
			default:
				break;
				}
			}
		
		//theFxScheduler.PlayEffect( "bow/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.forcearrowShotEffect, cent->lerpOrigin, forward );
				break; 
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				theFxScheduler.PlayEffect( cgs.effects.windarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.stonearrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.sonicarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.waterarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.bowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.holyarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.darkarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.vamparrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.necroarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.glypharrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.poisonarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.firearrowShotEffect, cent->lerpOrigin, forward );
				break;  
			default:
				break;
				}
			}
		//theFxScheduler.PlayEffect( cgs.effects.bowShotEffect, cent->lerpOrigin, forward );
	}
}

void FX_HowlerProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
	{
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				case CLASS_LANDO: 
				theFxScheduler.PlayEffect( "astral/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "howler/NPCshot", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.astralShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.howlerShotEffect, cent->lerpOrigin, forward );
				break; 
			default:
				break;
				}
			}
	}
	
	
	/*if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
	{
		theFxScheduler.PlayEffect( "howler/NPCshot", cent->lerpOrigin, forward );
	}
	else
	{
		theFxScheduler.PlayEffect( cgs.effects.howlerShotEffect, cent->lerpOrigin, forward );
	}*/
}

void FX_BloodGunProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
	{
		theFxScheduler.PlayEffect( "bloodgun/NPCshot", cent->lerpOrigin, forward );
	}
	else
	{
		theFxScheduler.PlayEffect( cgs.effects.bloodgunShotEffect, cent->lerpOrigin, forward );
	}
}

/*
-------------------------
FX_BryarHitWall
-------------------------
*/

// Frecce Elementali
void FX_BowHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.bowWallImpactEffect, origin, normal );
}

void FX_Bow0HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.vamparrowWallImpactEffect, origin, normal );
}

void FX_Bow1HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.darkarrowWallImpactEffect, origin, normal );
}

void FX_Bow2HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.shockarrowWallImpactEffect, origin, normal );
}

void FX_Bow3HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.firearrowWallImpactEffect, origin, normal );
}

void FX_Bow4HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.windarrowWallImpactEffect, origin, normal );
}

void FX_Bow5HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.waterarrowWallImpactEffect, origin, normal );
}

void FX_Bow6HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.stonearrowWallImpactEffect, origin, normal );
}

void FX_Bow7HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.holyarrowWallImpactEffect, origin, normal );
}

void FX_Bow8HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.necroarrowWallImpactEffect, origin, normal );
}

void FX_Bow9HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.poisonarrowWallImpactEffect, origin, normal );
}

void FX_Bow10HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.sonicarrowWallImpactEffect, origin, normal );
}

void FX_Bow11HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.glypharrowWallImpactEffect, origin, normal );
}

void FX_Bow12HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.forcearrowWallImpactEffect, origin, normal );
}
//TENEBRE 
void FX_DarkHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.darkWallImpactEffect, origin, normal );
}
// FUMO
void FX_SmokeHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.smokeWallImpactEffect, origin, normal );
}
// VUOTO
void FX_VoidHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.voidWallImpactEffect, origin, normal );
}

// SONIC 
void FX_HowlerHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.howlerWallImpactEffect, origin, normal );
}

void FX_AstralHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.astralWallImpactEffect, origin, normal );
}

void FX_BloodGunHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.bloodgunWallImpactEffect, origin, normal );
}

void FX_AbsorbHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.absorbWallImpactEffect, origin, normal );
}
// KNIFES 
void FX_KnifeHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.knifeWallImpactEffect, origin, normal );
}
// ENTROPY
void FX_EntropyHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.entropyWallImpactEffect, origin, normal );
}
// STUN 
void FX_StunHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.stunWallImpactEffect, origin, normal );
}

// STUN GUN
void FX_StunnerHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.stunner0WallImpactEffect, origin, normal );
}
/*
-------------------------
FX_BryarHitPlayer
-------------------------
*/

//// FRECCE ELEMENTALI 
void FX_BowHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.bowFleshImpactEffect, origin, normal );
}

void FX_Bow0HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.vamparrowFleshImpactEffect, origin, normal );
}

void FX_Bow1HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.darkarrowFleshImpactEffect, origin, normal );
}

void FX_Bow2HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stonearrowFleshImpactEffect, origin, normal );
}

void FX_Bow3HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.firearrowFleshImpactEffect, origin, normal );
}

void FX_Bow4HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.windarrowFleshImpactEffect, origin, normal );
}

void FX_Bow5HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.waterarrowFleshImpactEffect, origin, normal );
}

void FX_Bow6HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stonearrowFleshImpactEffect, origin, normal );
}

void FX_Bow7HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.holyarrowFleshImpactEffect, origin, normal );
}

void FX_Bow8HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.necroarrowFleshImpactEffect, origin, normal );
}

void FX_Bow9HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.poisonarrowFleshImpactEffect, origin, normal );
}

void FX_Bow10HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.sonicarrowFleshImpactEffect, origin, normal );
}

void FX_Bow11HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.glypharrowFleshImpactEffect, origin, normal );
}

void FX_Bow12HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.forcearrowFleshImpactEffect, origin, normal );
}

// TENEBRE
void FX_DarkHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.darkFleshImpactEffect, origin, normal );
}
// FUMO

void FX_SmokeHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.smokeFleshImpactEffect, origin, normal );
}

// VUOTO

void FX_VoidHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.voidFleshImpactEffect, origin, normal );
}

void FX_HowlerHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.howlerFleshImpactEffect, origin, normal );
}

void FX_AstralHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.astralFleshImpactEffect, origin, normal );
}

void FX_BloodGunHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.bloodgunFleshImpactEffect, origin, normal );
}

void FX_AbsorbHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.absorbFleshImpactEffect, origin, normal );
}
// KNIFE

void FX_KnifeHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.knifeFleshImpactEffect, origin, normal );
}

// ENTROPY

void FX_EntropyHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.entropyFleshImpactEffect, origin, normal );
}

// STUN
void FX_StunHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stunFleshImpactEffect, origin, normal );
}

void FX_StunnerHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stunner0FleshImpactEffect, origin, normal );
}
/*
-------------------------

	ALT FIRE

-------------------------
FX_BryarAltProjectileThink
-------------------------
*/
void FX_BryarAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	// see if we have some sort of extra charge going on
	for ( int t = 1; t < cent->gent->count; t++ )
	{
		if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
		{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{

				if ( cent->gent->owner->client->gs.gun->Gun_Alt_Bullet_Effect )
				{
					//gi.Printf( S_COLOR_GREEN "Customize Alt Bullet Effect from *.GUN File! None = gun/altshot" , cent );
					//G_PlayEffect( G_EffectIndex( cent->gent->owner->client->gs.gun->Gun_Alt_Bullet_Effect ), cent->lerpOrigin, forward  );  
					G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_Alt_Bullet_Effect, cent->lerpOrigin, forward  );  
			
				}

				/*switch ( cent->gent->owner->client->NPC_class )
				{
												// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "knife/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL2:// life reaver class 
					G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				G_PlayEffect( "smoke/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Boss del Fumo, Respen con la Mieti di Fumo
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/crackleshot" , cent->lerpOrigin, forward  );  
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
	     		G_PlayEffect( "stun/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				G_PlayEffect( "time/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dust/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "silence/crackleshot" , cent->lerpOrigin, forward  );  
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
		       	G_PlayEffect( "void/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoke/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "salt/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/crackleshot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/crackleshot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoker/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/crackleshot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/crackleshot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/crackleshot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/crackleshot" , cent->lerpOrigin, forward  );  
				break;
			default:
				break;				
			}*/
		}
		// just add ourselves over, and over, and over when we are charged
		//theFxScheduler.PlayEffect( cgs.effects.bryarPowerupShotEffect, cent->lerpOrigin, forward );
	}

	 if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
			{

				if ( cent->gent->owner->client->gs.gun->Gun_Alt_Bullet_Effect )
				{
					//gi.Printf( S_COLOR_GREEN "Customize AltShot Effect from *.GUN File! None = gun/altshot" , cent );
					G_PlayEffect( cent->gent->owner->client->gs.gun->Gun_Alt_Bullet_Effect, cent->lerpOrigin, forward  );  
				}

/*				switch ( cent->gent->owner->client->NPC_class )
				{
				// ANIMALI E PIANTE 
				case CLASS_IVY:
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				// animali 
				case CLASS_BEAST: // animals 
					case CLASS_SWAMP: // animali palustri 
				case CLASS_INTERROGATOR: // POISON FLYING CREATURE CLASS 
				case CLASS_BIRD: // water creatrues 
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_SHARK: // flying creatures
				G_PlayEffect( "salt/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_REELO:			// SPIDERS 
				// Werewolves 
				case CLASS_TRANDOSHAN:		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
				case CLASS_LYCAN: // Werewolves  
				case CLASS_MUTANT: // mutanti 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "knife/shot" , cent->lerpOrigin, forward  );  
				break;
				// Pugnale glifico
				case CLASS_SCOUT:// Scout, altfire, switch, 
				// Guerrieri sarafan: pugnali d'ombra
				case CLASS_STORMTROOPER: // SARAFAN 
				// Sarafan Warriors
				case CLASS_SARAFAN: // Sarafan Warrior human \ warrior elite class with connection to Holy 
				G_PlayEffect( "knife2/shot" , cent->lerpOrigin, forward  );  
				break;
				// stregoni 
				case CLASS_WIZARD: // Sorcerer and Magician
				case CLASS_DRUID: // Domators and Druids
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_IMPERIAL:// PROPHET, CENOBITIC, DEMON CULTIST CLASS
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_CENOBITA: // Cenobita demonic priest 
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GALAK: // VAMPIRE HUNTERS BOSS CLASS
				G_PlayEffect( "smoker2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_JAN: 	// SARAFAN BOSS CLASS 
				G_PlayEffect( "knife2/shot" , cent->lerpOrigin, forward  );  
				break;
				// elfi 
				case CLASS_MONMOTHA:	// ELF CLASS 
				case CLASS_ELF:		// Elf class 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WATERREAVER: // light reaver class 
				G_PlayEffect( "salt/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_WARMREAVER: // elizabeth, daughter of respen
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DRAGONREAVER: // dragonid class 
				case CLASS_FIREREAVER: // fire - turelim reaver class 
				G_PlayEffect( "ash/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL:// life reaver class 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL2:// life reaver class 
					G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_RAZIEL3:// life reaver class 
				G_PlayEffect( "dark2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Bleed e Respen e Colere con la Reaver di Fumo
				case CLASS_TANKREAVER: // tank reaver class 
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				// Boss del Fumo, Respen con la Mieti di Fumo
				case CLASS_LUKE:	// REAPER BOSS CLASS (RAZIEL)
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				// Shifter con la Forgia del Vuoto
				case CLASS_DARKREAVER:
				case CLASS_VOID: // elementali di vuoto
				case CLASS_GOLEM_VOID: // golem vuoto
				case CLASS_SHADOWTROOPER:
				G_PlayEffect( "void/shot" , cent->lerpOrigin, forward  );  
				break;
				// Polvere 
				// Xado e il proiettile di Polvere 
				case CLASS_SPIDERREAVER:// Spider reaver class 
				case CLASS_SPIDERREAVER2:// Xado mud reaver
				case CLASS_SPIDERREAVER3:// xado poison reaver
				G_PlayEffect( "dust/shot" , cent->lerpOrigin, forward  );  
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
	     		G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;
				// ZEPHONIM - Zephohim stun deal some damage 
				case CLASS_NOGHRI: 	// ZEPHONIM VAMPIRES young
				case CLASS_ZEPH: // Spider vampire classes wallclimber
				case CLASS_DECEIVER:// Psychic - cloacked vampire class 
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Boss dei vampiri corrotti 
				case CLASS_FISH:				// RAHABIM BOSS AND NECROMANTIC CLASS 
				case CLASS_ESKANDOR: // Razielhim boss 
				case CLASS_LIZARD:			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
				case CLASS_CLAW: // turelim bosses 
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ZEPHQUEEN: // ZEPHONIM BOSS AND PSYCHIC CLASS - MORE DMG!
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
 				// Leggendary Vampires deals full stun damage
				case CLASS_JANOS: // janos audron class 
				case CLASS_VORADOR: // vorador class 
				case CLASS_KAIN3:// Kain sion of balance
				G_PlayEffect( "stun3/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri di Vampiri 
				case CLASS_VAMPIRE_GHOST: // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
				case CLASS_VAMPIRE_GHOST_LIGHT: //Ancientvampire light ghost
			  	case CLASS_VAMPIRE_GHOST_RADIANCE: // Ancient vampire radiance ghost
				G_PlayEffect( "stun2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_DARK: // Spettro delle Tenebre
				G_PlayEffect( "dark2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SOUND: // Ancient vampire sonic ghost
				case CLASS_VAMPIRE_GHOST_SILENCE: // Ancient vampire silence ghost
			    case CLASS_VAMPIRE_GHOST_ASTRAL: // Ancient vampire astral ghost
				G_PlayEffect( "silence2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_FIRE: // Ancient vampire fire ghost
	         	case CLASS_VAMPIRE_GHOST_SMOKE: //
				case CLASS_VAMPIRE_GHOST_WARM: // Ancient vampire warm ghost
				G_PlayEffect( "smoke2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_ASH:
				case CLASS_VAMPIRE_GHOST_LAVA: // Ancient vampire lava ghost
				G_PlayEffect( "ash2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_AIR: //Ancient vampire air ghost
				case CLASS_VAMPIRE_GHOST_LIGHTNING: //Ancient vampire lightning ghost
				case CLASS_VAMPIRE_GHOST_FOG: // Ancient vampire fog ghost
				case CLASS_VAMPIRE_GHOST_STORM: //Ancient vampire storm ghost
				case CLASS_VAMPIRE_GHOST_VOID:
				// Boss forgia del Vuoto
				G_PlayEffect( "void2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_WATER: // Ancient vampire water ghost
				case CLASS_VAMPIRE_GHOST_SALT: // Ancient Vampire Salt Ghost
				case CLASS_VAMPIRE_GHOST_ICE: // Ancient vampire ice ghost
				case CLASS_VAMPIRE_GHOST_WAVE: // Ancient vampire wave ghost
				G_PlayEffect( "salt2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_EARTH: //Ancient vampire earth ghost
				case CLASS_VAMPIRE_GHOST_MUD: //Ancient vampire mud ghost
				case CLASS_VAMPIRE_GHOST_CRYSTAL: //Ancient vampire crystal ghost
			    case CLASS_VAMPIRE_GHOST_STONE: //Ancient vampire stone ghost
				case CLASS_VAMPIRE_GHOST_DUST: //Ancient vampire dust ghost
				G_PlayEffect( "dust2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Vampire ghosts Bosses of the Hybrid and New Forges 
				case CLASS_VAMPIRE_GHOST_POISON: // Ancient vampire poison ghost
				case CLASS_VAMPIRE_GHOST_NECRO: // Ancient vampire necro ghost
				G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_VAMPIRE_GHOST_SPIRIT: // Ancient vampire spirit ghost
				case CLASS_VAMPIRE_GHOST_HOLY: // Ancient vampire holy ghost
				case CLASS_VAMPIRE_GHOST_LIFE: // ancient vampire ghost of Life 
				case CLASS_VAMPIRE_BALANCE_GHOST: // ancient vampire balance ghost
				G_PlayEffect( "stun3/shot" , cent->lerpOrigin, forward  );  
				break;
				// HUMAN BOSSES - THE CIRCLE OF NINE! 
				case CLASS_NUPRAPTOR: // Nupraptor ghost, guardian of Mind
				G_PlayEffect( "Mind/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_AZIMUTH: // Azimuth Ghost, guardian of Dimension
				G_PlayEffect( "Dimension/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MALEK: // Malek ghost, guardian of Conflict 
				G_PlayEffect( "Conflict/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_BANE: // Bane Ghost, guardian of Nature
				G_PlayEffect( "Nature/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL:
				G_PlayEffect( "balance/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_GHOST:
				case CLASS_MOEBIUS_GHOST: // the ghost of the summoned moebius. 
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ARIEL_EVIL: // Ariel Corrupted ghost 
				G_PlayEffect( "chaos/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOEBIUS: // Moebius ghost, guardian of time (moebius summoned by another age)
				G_PlayEffect( "time/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEJOULE: // Dejoule Ghost, guardian of Energy
				G_PlayEffect( "energy/shot" , cent->lerpOrigin, forward  );  
				break;
				case	CLASS_ANACROTHE: // the Ghost of Anacrothe, guardian of State
				G_PlayEffect( "smoker3/shot" , cent->lerpOrigin, forward  );  
				break;
        		case CLASS_MORTANIUS: // the ghost of Mortanius, guardian of Death 
				G_PlayEffect( "necro/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "dust/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_THRALL: 
				G_PlayEffect( "dust/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIGHT: // light- electro-sun elite class 
			    case CLASS_GOLEM_LIGHT: // Golem lucente
				case CLASS_RADIANCE: // Elementali della Radianza
				case CLASS_PROTOCOL: // SUN GOLEMS CLASS 
				case CLASS_GOLEM_RADIANCE: // Golem Radiante
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;
				// I raggianti della Luce subiranno una flessione di danno
				// Idem Syl con la Radianza 
				case CLASS_MORGANKATARN: // SUN ELEMENTAL CREATURE CLASS 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "silence/shot" , cent->lerpOrigin, forward  );  
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
		       	G_PlayEffect( "void/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoke/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_ASH: // Elementali della Cenere
				case CLASS_GOLEM_ASH: // golem di cenere 
				case CLASS_LAVA:// elementale di lava
		 		case CLASS_GOLEM_LAVA: // golem di lava
				G_PlayEffect( "ash/shot" , cent->lerpOrigin, forward  );  
				break;
				// Rekius continuerà a tirar attacchi d'ombra 
				case CLASS_BARTENDER:	// ICE ELEMENTAL CLASS 
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "salt/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GOLEM_IVY: // golem delle piante
				G_PlayEffect( "stun/shot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_MINEMONSTER:		// POISON CLASS
				case CLASS_POISON:
			    case CLASS_GOLEM_POISON: // golem del veleno
				case CLASS_NECRO: 
		        case CLASS_GOLEM_NECRO: // golem della morte
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HELLGOLEM: // golem infernale
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DECAL:
				G_PlayEffect( "dark3/shot" , cent->lerpOrigin, forward  );  
				break;
				// shielded golems
				case CLASS_ASSASSIN_DROID:	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
				G_PlayEffect( "dark/shot" , cent->lerpOrigin, forward  );  
				break;
				// ELEMENTALI 
				// - Inutile contro gli elementali e i golem! 
			   // Elementali della Spirito 
				 case CLASS_SPIRIT: // Elementali di Spirito
			     case CLASS_GOLEM_SPIRIT: // Golem di Spirito
			 	G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				// Elementali del Sacro 
			   case CLASS_HOLY: // for holy creatures 
			   // Elementali della Vita 
			   	case CLASS_GOLEM_HOLY: // for golem holy 
		   		G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_LIFE: // elementale di vita 
			   // Golem della Spirito 
			   // Golem del Sacro 
		       case CLASS_GOLEM_LIFE: // golem di vita 
			   // Elementali della Decale  
			   	G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
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
				G_PlayEffect( "smoker/shot" , cent->lerpOrigin, forward  );  
				break;
				// NON MORTI 
				case CLASS_SUMMONER:// melc variant with alt fire and switch support
				case CLASS_NECROREAVER: // necro reaver class
			   case CLASS_COMMANDO: // UNDEADS 
	  		    case CLASS_POLTER: // act like rockettrooper class but their undead
			    case CLASS_PROBE:		// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_GLIDER: //MELCHIAHIM BOSS CLASS 
				G_PlayEffect( "absorb2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MURJJ: 		// LICH AND NECROMANCER CLASS 
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				// Spettri: lancian un attacco necrotico di tenebre che succhia la vita
				case CLASS_GHOST: // Like jawa class. is a class for Human and ectoplasmic pacific creature
				case CLASS_JAWA:	// GHOSTS 
				case CLASS_SLUAGH: // Like Rancor, but they not devour enemies. 
				case CLASS_RANCOR:	// SLUAGH 
				case CLASS_REAPER: // Like spectral rockettrooper
				case CLASS_DREADNOUGHT: // Like spectral hazardtrooper
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				// PAZZIA 
				// Demoni  NON VENGONO STORDITI
		        case CLASS_DEMON_DARK: // demone ombra 
				G_PlayEffect( "dark3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_LIGHT: // demone della luce
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_SOUND: // demone del suono
				G_PlayEffect( "silence/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_EARTH: // demone della terra
				G_PlayEffect( "dust3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_FIRE:// For Fire demon . 
				G_PlayEffect( "smoke3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_AIR: // demone dell'aria 
				case CLASS_DEMON_ELECTRO: // for demon lightnings
				G_PlayEffect( "void3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_WATER: // demone acquatico
				case CLASS_DEMON_ICE: // demone gelido
				G_PlayEffect( "salt3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_POISON:// for poison demon 
				case CLASS_DEMON_NECRO: // demon necromantic
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_BLADE: // for demonblades 
			 	G_PlayEffect( "hellknife/shot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_DEMON_TAUROC_ELECTRO: // demoni taurini elettrici
				G_PlayEffect( "void3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC_POISON:// demoni taurini del veleno
				G_PlayEffect( "absorb3/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DEMON_TAUROC: // demoni taurini del fuoco rosso
				case CLASS_DEMON_TAUROC_BLACK: // demoni taurini del fuoco nero 
				G_PlayEffect( "smoke3/shot" , cent->lerpOrigin, forward  );  
				break;
				
			case CLASS_WAMPA:  // HELLFIRE DEMONIC CLASS, (usare per i demoni taurini)
			case CLASS_BOBAFETT: // DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
		  	G_PlayEffect( "madness2/shot" , cent->lerpOrigin, forward  );  
				break;
			    // Profetesse: metà umane, metà hylden - NON VIENE STORDITA 
				case CLASS_PROPHET: // Prophet, altfire, switch, 
				// Hylden NON VENGONO STORDITI 
				case CLASS_HYLDEN: // Elite hylden blacksmith glyphic class 
				case CLASS_TAVION:			// HYLDEN CLASS - boss woman
				case CLASS_HYLDEN_LIGHT: // Fabbro dei Glifi - Luce 
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_SOUND: // Fabbro dei Glifi - Suono
				G_PlayEffect( "silence4/shot" , cent->lerpOrigin, forward  );  
				break;	
				case CLASS_HYLDEN_DARK: // Fabbro dei Glifi - assassino
				G_PlayEffect( "dark4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STONE: // Fabbro dei Glifi - Pietra 
				G_PlayEffect( "dust4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FIRE: // Fabbro dei Glifi - Fuoco
				G_PlayEffect( "smoke4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_FORCE: // Fabbro dei Glifi - Forza
				G_PlayEffect( "void4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WATER: // Fabbro dei Glifi - Acqua
				G_PlayEffect( "salt4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_WARRIOR: // Guerriero Hylden
				G_PlayEffect( "hellknife/shot" , cent->lerpOrigin, forward  );  
				break;
				// maestri dell'emblema del caos 
				case CLASS_HYLDEN_CHAOS: // Mastro del Chaos Hylden
                case CLASS_HYLDEN_MADNESS: // maestro della Follia
				G_PlayEffect( "madness/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_NULL: // Mastro del Nulla Hylden
				G_PlayEffect( "null/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_STAGNATION: // Mastro della Stagnazione Hylden
				G_PlayEffect( "stagnation/shot" , cent->lerpOrigin, forward  );  
				break;
				// temp 
                case CLASS_HYLDEN_CORRUPTION: // Mastro della Corruzione Hylden - lancia atk veleno
				G_PlayEffect( "corruption/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ENTROPY: // Mastro dell'Entropia Hylden - lancia atk entropia 
				G_PlayEffect( "entropy/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_TIMELESS: // Mastro Rubatempo HyLDEN - lancia atk tempo 
				G_PlayEffect( "timeless/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_ANTIMATTER: // Mastro dell'Antimateria
				G_PlayEffect( "antimatter/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDEN_GLYPH: // Mastro dei Glifi Hylden
				G_PlayEffect( "smoker/shot" , cent->lerpOrigin, forward  );  
				break;
				// Hylden Bosses 
				case CLASS_HYLDENLORD_MADNESS: // maestro della Follia
				G_PlayEffect( "madness2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_NULL: // Signore del Nulla Hylden
				G_PlayEffect( "null/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_STAGNATION: // Signore della Stagnazione Hylden
				G_PlayEffect( "stagnation2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_CORRUPTION: // Signore della Corruzione Hylden
				G_PlayEffect( "corruption2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ENTROPY: // Signore dell'Entropia Hylden 
				G_PlayEffect( "entropy2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_TIMELESS: // Signore Rubatempo Hydeln
				G_PlayEffect( "timeless2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_ANTIMATTER: // Signore dell'Antimateria Hylden
				G_PlayEffect( "antimatter2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_HYLDENLORD_GLYPH: // Signore dei Glifi Hylden
				G_PlayEffect( "smoker2/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_DESANN:			// HYLDEN LORD CLASS 
				case CLASS_HYLDENLORD: // For Hylden king and Hylden warlorcks. 
				case CLASS_HYLDENLORD_CHAOS: // Signore del Caos Hylden
				G_PlayEffect( "madness3/shot" , cent->lerpOrigin, forward  );  
				break;
				// ANZIANO
				case CLASS_TENTACLE: // act like sand creature 
				case CLASS_SAND_CREATURE:	// ELDER GOD TENTACLE AND MOUTH MONSTER 
				G_PlayEffect( "absorb/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_MOUSE:			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
				case CLASS_ATST:				// ELDER GOD CLASS 
				case CLASS_ELDERGOD: // the elder god class 
				G_PlayEffect( "absorb4/shot" , cent->lerpOrigin, forward  );  
				break;
				case CLASS_FIGHTER:
				G_PlayEffect( "bryar0/shot" , cent->lerpOrigin, forward  );  
				break;
			}*/
		}
		// theFxScheduler.PlayEffect( cgs.effects.bryarShotEffect, cent->lerpOrigin, forward );
	 }
}

void FX_BowAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	// see if we have some sort of extra charge going on
	for ( int t = 1; t < cent->gent->count; t++ )
	{
		if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
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
				theFxScheduler.PlayEffect( cgs.effects.forcearrowPowerupShotEffect, cent->lerpOrigin, forward );
				break; 
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				theFxScheduler.PlayEffect( cgs.effects.windarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.stonearrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.sonicarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.waterarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.bowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.holyarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.darkarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.vamparrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.necroarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.glypharrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.poisonarrowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.firearrowPowerupShotEffect, cent->lerpOrigin, forward );
				break;  				
			default:
				break;				
			}
		}
		// just add ourselves over, and over, and over when we are charged
		//theFxScheduler.PlayEffect( cgs.effects.bowPowerupShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.forcearrowShotEffect, cent->lerpOrigin, forward );
				break; 
				// WIND
				case CLASS_BIRD: 
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				theFxScheduler.PlayEffect( cgs.effects.windarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.stonearrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.sonicarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.waterarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.bowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.holyarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.darkarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.vamparrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.necroarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.glypharrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.poisonarrowShotEffect, cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( cgs.effects.firearrowShotEffect, cent->lerpOrigin, forward );
				break;  
			default:
				break;
				}
			}
	}	
	//theFxScheduler.PlayEffect( cgs.effects.bowShotEffect, cent->lerpOrigin, forward );
}

void FX_HowlerAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	// see if we have some sort of extra charge going on
	for ( int t = 1; t < cent->gent->count; t++ )
	{
		if ( cent->gent->owner->client->NPC_class == CLASS_LANDO )
		{
			theFxScheduler.PlayEffect( cgs.effects.astralPowerupShotEffect, cent->lerpOrigin, forward );
		}
		else if (cent->gent->owner->client->NPC_class != CLASS_LANDO )
		{
			theFxScheduler.PlayEffect( cgs.effects.howlerPowerupShotEffect, cent->lerpOrigin, forward );// just add ourselves over, and over, and over when we are charged
		}
	}

	if ( cent->gent->owner->client->NPC_class == CLASS_LANDO )
		{
			theFxScheduler.PlayEffect( cgs.effects.astralShotEffect, cent->lerpOrigin, forward );
		}
		else if (cent->gent->owner->client->NPC_class != CLASS_LANDO )
		{
			theFxScheduler.PlayEffect( cgs.effects.howlerShotEffect, cent->lerpOrigin, forward );
		}
}

void FX_BloodGunAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}

	// see if we have some sort of extra charge going on
	for ( int t = 1; t < cent->gent->count; t++ )
	{
		// just add ourselves over, and over, and over when we are charged
		theFxScheduler.PlayEffect( cgs.effects.bloodgunPowerupShotEffect, cent->lerpOrigin, forward );
	}

	theFxScheduler.PlayEffect( cgs.effects.bloodgunShotEffect, cent->lerpOrigin, forward );
}

/*
-------------------------
FX_BryarAltHitWall
-------------------------
*/
// TENEBRE
void FX_DarkAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.darkWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.darkWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.darkWallImpactEffect, origin, normal );
		break;
	}
}


void FX_SmokeAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.smokeWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.smokeWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.smokeWallImpactEffect, origin, normal );
		break;
	}
}

void FX_VoidAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.voidWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.voidWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.voidWallImpactEffect, origin, normal );
		break;
	}
}

void FX_KnifeAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.knifeWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.knifeWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.knifeWallImpactEffect, origin, normal );
		break;
	}
}

void FX_AbsorbAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.absorbWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.absorbWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.absorbWallImpactEffect, origin, normal );
		break;
	}
}

void FX_StunAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.stunWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.stunWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.stunWallImpactEffect, origin, normal );
		break;
	}
}

void FX_EntropyAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.entropyWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.entropyWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.entropyWallImpactEffect, origin, normal );
		break;
	}
}
// FRECCE ELEMENTALI 
void FX_BowAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.bowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.bowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.bowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow0AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.vamparrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.vamparrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.vamparrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow1AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.darkarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.darkarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.darkarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow2AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.shockarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.shockarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.shockarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow3AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.firearrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.firearrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.firearrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow4AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.windarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.windarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.windarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow5AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.waterarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.waterarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.waterarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow6AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.stonearrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.stonearrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.stonearrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow7AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.holyarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.holyarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.holyarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow8AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.necroarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.necroarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.necroarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow9AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.poisonarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.poisonarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.poisonarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow10AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.sonicarrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.sonicarrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.sonicarrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow11AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.glypharrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.glypharrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.glypharrowWallImpactEffect, origin, normal );
		break;
	}
}

void FX_Bow12AltHitWall( vec3_t origin, vec3_t normal, int power  )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.forcearrowWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.forcearrowWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.forcearrowWallImpactEffect, origin, normal );
		break;
	}
}

// SONIC 
void FX_HowlerAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.howlerWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.howlerWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.howlerWallImpactEffect, origin, normal );
		break;
	}
}

void FX_AstralAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.astralWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.astralWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.astralWallImpactEffect, origin, normal );
		break;
	}
}

void FX_BloodGunAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.bloodgunWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.bloodgunWallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.bloodgunWallImpactEffect, origin, normal );
		break;
	}
}

void FX_StunnerAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		theFxScheduler.PlayEffect( cgs.effects.stunner0WallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		theFxScheduler.PlayEffect( cgs.effects.stunner0WallImpactEffect2, origin, normal );
		break;

	default:
		theFxScheduler.PlayEffect( cgs.effects.stunner0WallImpactEffect, origin, normal );
		break;
	}
}

/*
-------------------------
FX_BryarAltHitPlayer
-------------------------
*/

//// FRECCE ELEMENTALI 
void FX_BowAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.bowFleshImpactEffect, origin, normal );
}

void FX_Bow0AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.vamparrowFleshImpactEffect, origin, normal );
}

void FX_Bow1AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.darkarrowFleshImpactEffect, origin, normal );
}

void FX_Bow2AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stonearrowFleshImpactEffect, origin, normal );
}

void FX_Bow3AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.firearrowFleshImpactEffect, origin, normal );
}

void FX_Bow4AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.windarrowFleshImpactEffect, origin, normal );
}

void FX_Bow5AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.waterarrowFleshImpactEffect, origin, normal );
}

void FX_Bow6AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stonearrowFleshImpactEffect, origin, normal );
}

void FX_Bow7AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.holyarrowFleshImpactEffect, origin, normal );
}

void FX_Bow8AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.necroarrowFleshImpactEffect, origin, normal );
}

void FX_Bow9AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.poisonarrowFleshImpactEffect, origin, normal );
}

void FX_Bow10AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.sonicarrowFleshImpactEffect, origin, normal );
}

void FX_Bow11AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.glypharrowFleshImpactEffect, origin, normal );
}

void FX_Bow12AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.forcearrowFleshImpactEffect, origin, normal );
}

////////
// TENEBRE 

void FX_DarkAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.darkFleshImpactEffect, origin, normal );
}

void FX_SmokeAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.smokeFleshImpactEffect, origin, normal );
}

void FX_VoidAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.voidFleshImpactEffect, origin, normal );
}

void FX_KnifeAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.knifeFleshImpactEffect, origin, normal );
}

void FX_StunAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stunFleshImpactEffect, origin, normal );
}

void FX_AbsorbAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.absorbFleshImpactEffect, origin, normal );
}

void FX_EntropyAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.entropyFleshImpactEffect, origin, normal );
}

// Sonic and astral
void FX_HowlerAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.howlerFleshImpactEffect, origin, normal );
}

void FX_AstralAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.astralFleshImpactEffect, origin, normal );
}

void FX_BloodGunAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.bloodgunFleshImpactEffect, origin, normal );
}

void FX_StunnerAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.stunner0FleshImpactEffect, origin, normal );
}





