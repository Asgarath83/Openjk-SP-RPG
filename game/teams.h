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

#ifndef TEAMS_H
#define TEAMS_H

typedef enum //# team_e
{
	TEAM_FREE,			// caution, some code checks a team_t via "if (!team_t_varname)" so I guess this should stay as entry 0, great or what? -slc
	TEAM_PLAYER,
	TEAM_ENEMY,
	TEAM_NEUTRAL,	// most droids are team_neutral, there are some exceptions like Probe,Seeker,Interrogator

	//# #eol
	TEAM_NUM_TEAMS
} team_t;

extern stringID_table_t TeamTable[];

// This list is made up from the model directories, this MUST be in the same order as the ClassNames array in NPC_stats.cpp
typedef enum 
{
	CLASS_NONE,				// hopefully this will never be used by an npc, just covering all bases
	// THE LEGACY OF KAIN CLASSES 
	CLASS_ATST,				// ELDER GOD CLASS 
	CLASS_BARTENDER,		// ICE ELEMENTAL CLASS 
	CLASS_BESPIN_COP,		// SMOKE ELEMENTAL CLASS 
	CLASS_CLAW,				// TURELIM BOSS AND MAGMA ELEMENTAL CLASS 
	CLASS_COMMANDO,			// UNDEADS 
	CLASS_DESANN,			// HYLDEN LORD CLASS 
	CLASS_FISH,				// RAHABIM BOSS AND NECROMANTIC CLASS 
	CLASS_FLIER2,			// ZEPHONIM BOSS AND PSYCHIC CLASS 
	CLASS_GALAK,			// VAMPIRE HUNTERS BOSS CLASS 
	CLASS_GLIDER,			// MELCHIAHIM BOSS CLASS 
	CLASS_GONK,				// GOLEM AND EARTH ELEMENTAL CREATURES 
	CLASS_GRAN,				// TURELIM VAMPIRES
	CLASS_HOWLER,			// SONIC CREATURE CLASS 
	CLASS_RANCOR,			// SLUAGH 
	CLASS_SAND_CREATURE,	// ELDER GOD TENTACLE AND MOUTH MONSTER 
	CLASS_WAMPA,			// HELLFIRE DEMONIC CLASS 
	CLASS_IMPERIAL,			// PROPHET, CENOBITIC, DEMON CULTIST CLASS
	CLASS_IMPWORKER,		// ALCHEMIST AND WIZARD CLASS 
	CLASS_INTERROGATOR,		// POISON FLYING CREATURE CLASS 
	CLASS_JAN,				// SARAFAN BOSS CLASS 
	CLASS_JEDI,				// REAPER CLASS 
	CLASS_KYLE,				// VAMPIRE BOSS CLASS (KAIN)
	CLASS_LANDO,			// ASTRAL CLASS 
	CLASS_LIZARD,			// DUMAHIM CLASS AND CRYSTAL ELEMENTAL MONSTER 
	CLASS_LUKE,				// REAPER BOSS CLASS (RAZIEL)
	CLASS_MARK1,			// WITCH ARMOR BOSS CLASS 
	CLASS_MARK2,			// WITCH ARMOR CLASS
	CLASS_GALAKMECH,		// CRYSTAL MONSTER 
	CLASS_MINEMONSTER,		// POISON CLASS AND DEMON OF POISONS
	CLASS_MONMOTHA,			// ELF CLASS 
	CLASS_MORGANKATARN,		// SUN ELEMENTAL CREATURE CLASS 
	CLASS_MOUSE,			// LESSER GHOSTS AND ELDER GOD FIGHTING CLASS
	CLASS_MURJJ,			// LICH AND NECROMANCER CLASS 
	CLASS_PRISONER,			// HUMANS 
	CLASS_PROBE,			// POLTERGEIST AND FLOATING SPIRIT WIND CLASS
	CLASS_PROTOCOL,			// SUN GOLEMS CLASS 
	CLASS_R2D2,				// MECHANICAL GOLEM CLASS 
	CLASS_R5D2,				// MECHANICAL GOLEM CLASS 2
	CLASS_REBEL,			// FOG CLASS 
	CLASS_REBORN,			// VAMPIRES 
	CLASS_REELO,			// SPIDERS 
	CLASS_REMOTE,			// ARTIFACT SHOOTER ARROW TRAP
	CLASS_RODIAN,			// VAMPIRESLAYERS AND IRONGUARD 
	CLASS_SEEKER,			// SEEKER ARTIFACTS
	CLASS_SENTRY,			// SENTRY ARTIFACT
	CLASS_SHADOWTROOPER,	// VOID ELEMENTAL DEMON, SHADOW BOSSES 
	CLASS_SABOTEUR,			// SHADOW WARRIOR CLASS 
	CLASS_STORMTROOPER,		// SARAFAN 
	CLASS_SWAMP,			// WATER CREATURES 
	CLASS_SWAMPTROOPER,		// RAHABIM VAMPIRES 
	CLASS_NOGHRI,			// ZEPHONIM VAMPIRES 
	CLASS_TAVION,			// HYLDEN CLASS 
	CLASS_ALORA,			// RAHABIM SPECIAL CLASS 
	CLASS_TRANDOSHAN,		// WEREWOLF, DRUID, BEAST AND FERAL CREATURES 
	CLASS_UGNAUGHT,			// MUD MONSTERS, BLOBS, 
	CLASS_JAWA,				// GHOSTS 
	CLASS_WEEQUAY,			// DUMAHIM, STONE ELEMENTAL CLASS 
	CLASS_TUSKEN,			// MELCHIAHIM 
	CLASS_BOBAFETT,			// DEMONIC BOSS CLASS - STORM ELEMENTAL CREATURES
	CLASS_ROCKETTROOPER,	// STORM AND FLYING CREATURES 
	CLASS_SABER_DROID,		// LIGHTNING GOLEM CLASS 
	CLASS_ASSASSIN_DROID,	// ANCIENT VAMPIRE GHOSTS, SHIELDED GOLEM AND DEMONS
	CLASS_HAZARD_TROOPER,	// WARM AND FIRE CREATURES 
	CLASS_PLAYER,			// PLAYER 
	CLASS_VEHICLE,			// ANIMALS 
	CLASS_MANDA,			// Mandalorians
	// NEW LOK CLASSES - ELITE CLASSES WITH SWITCH WEAPONS, ALT FIRE, FLY AND OTHER FEATURES
	// Spectral Creature
	CLASS_SLUAGH, // Like Rancor, but they not devour enemies. 
	CLASS_REAPER, // Like spectral rockettrooper
	CLASS_DREADNOUGHT, // Like spectral hazardtrooper
	CLASS_GHOST, // Like jawa class. is a class for Human and ectoplasmic pacific creature
	// the Circle of Nine Ghosts
	CLASS_ARIEL_GHOST, // Ariel Ghost, Guardian of Balance
	CLASS_ARIEL_EVIL, // Ariel Corrupted ghost 
	CLASS_NUPRAPTOR, // Nupraptor ghost, guardian of Mind
	CLASS_AZIMUTH, // Azimuth Ghost, guardian of Dimension
	CLASS_MALEK, // Malek ghost, guardian of Conflict 
	CLASS_BANE, // Bane Ghost, guardian of Nature
	CLASS_DEJOULE, // Dejoule Ghost, guardian of Energy
	CLASS_MOEBIUS, // Moebius ghost, guardian of time (moebius summoned by another age)
	CLASS_MOEBIUS_GHOST, // the ghost of the summoned moebius. 
	CLASS_ANACROTHE, // the Ghost of Anacrothe, guardian of State
	CLASS_MORTANIUS, // the ghost of Mortanius, guardian of Death 
	// the Vampiric Hybrid Forges Bosses 
	CLASS_VAMPIRE_GHOST, // for Vampire Ghosts. alt fire, switch, shield, like assassin_droid 
	CLASS_VAMPIRE_GHOST_DARK, // Ancient vampire darkness ghost
	CLASS_VAMPIRE_GHOST_LIGHT, //Ancient vampire light ghost
	CLASS_VAMPIRE_GHOST_FIRE, // Ancient vampire fire ghost
	CLASS_VAMPIRE_GHOST_AIR, //Ancient vampire air ghost
	CLASS_VAMPIRE_GHOST_WATER, // Ancient vampire water ghost
	CLASS_VAMPIRE_GHOST_EARTH, //Ancient vampire earth ghost
	CLASS_VAMPIRE_GHOST_SOUND, // Ancient vampire sonic ghost
	// Vampire ghosts Bosses of the Hybrid and New Forges 
	CLASS_VAMPIRE_GHOST_ASH, // Ancient vampire darkness ghost
	CLASS_VAMPIRE_GHOST_VOID, //Ancient vampire void ghost
	CLASS_VAMPIRE_GHOST_SALT, // Ancient vampire salt ghost
	CLASS_VAMPIRE_GHOST_DUST, //Ancient vampire dust ghost
	CLASS_VAMPIRE_GHOST_SILENCE, // Ancient vampire silence ghost
	CLASS_VAMPIRE_GHOST_RADIANCE, // Ancient vampire radiance ghost
	CLASS_VAMPIRE_GHOST_LIGHTNING, //Ancient vampire lightning ghost
	CLASS_VAMPIRE_GHOST_FOG, // Ancient vampire fog ghost
	CLASS_VAMPIRE_GHOST_CRYSTAL, //Ancient vampire crystal ghost
	CLASS_VAMPIRE_GHOST_ASTRAL, // Ancient vampire astral ghost
	CLASS_VAMPIRE_GHOST_LAVA, // Ancient vampire lava ghost
	CLASS_VAMPIRE_GHOST_SMOKE, //Ancient vampire smoke ghost
	CLASS_VAMPIRE_GHOST_ICE, // Ancient vampire ice ghost
	CLASS_VAMPIRE_GHOST_MUD, //Ancient vampire mud ghost
	CLASS_VAMPIRE_GHOST_WARM, // Ancient vampire warm ghost
	CLASS_VAMPIRE_GHOST_STORM, //Ancient vampire storm ghost
	CLASS_VAMPIRE_GHOST_WAVE, // Ancient vampire wave ghost
	CLASS_VAMPIRE_GHOST_STONE, //Ancient vampire stone ghost
	CLASS_VAMPIRE_GHOST_POISON, // Ancient vampire poison ghost
	CLASS_VAMPIRE_GHOST_NECRO, // Ancient vampire necro ghost
	CLASS_VAMPIRE_GHOST_SPIRIT, // Ancient vampire spirit ghost
	CLASS_VAMPIRE_GHOST_HOLY, // Ancient vampire holy ghost
	CLASS_VAMPIRE_GHOST_LIFE, // ancient vampire ghost of Life 
	CLASS_VAMPIRE_BALANCE_GHOST, // ancient vampire balance ghost
	// Humans
	CLASS_HUNTER, // Ironguard // Altfire, switch, 
	CLASS_SCOUT,  // Scout, altfire, switch, 
	CLASS_ALCHEMIST, // Alchemist altfire, switch, - USA LE ARMI DA FUOCO GLIFICHE!
	CLASS_PROPHET, // Prophet, altfire, switch, 
	CLASS_DRUID, // altfire, switch, human magician class with connection to nature and holy
	CLASS_SARAFAN, // human \ warrior elite class with connection to Holy 
	CLASS_WIZARD, // Human wizard class with altfire and switch elements. 
	CLASS_ARCHER, // Human sniper elite class. 
	CLASS_WARRIOR, // Human generic warrior class, fight light noghri AI. 
	CLASS_CENOBITA, // Like Wizard, but with Emplaced element connection, they can summon demons. altfire, switch, shield. 
	// Other magical creatures
	CLASS_ELF, // Elvens clans of Nortwest, generic stuff. 0.5 to nature, neutral to elements, x2 to death
	CLASS_LYCAN, // For werewolf elite fighter class. they also scream like an howler 
	// Vampires e Reapers 
	CLASS_MELC, // dive like sand creature, fight like a zombie, eat like a vampire. 
	CLASS_SUMMONER,// melc variant with alt fire and switch support
	CLASS_ZEPH, // Spider vampire classes 
	CLASS_DECEIVER,// Psychic - cloacked vampire class 
	CLASS_ZEPHQUEEN, // Zephonim queen eggs producer
	CLASS_SNEAKER, // young hunter assassin dumahim
	CLASS_DUMAHIM, // weak to fire, earth resistance, immune to sun. 
	CLASS_RAHABIM,// immune to water, weak to fire, deadly to light, act like swamptrooper class
	CLASS_TYRANT, // assault berserk Turelim
	CLASS_TURELIM,// act like gran class, weak to sound, resistance to fire- 
	CLASS_SENTINEL, // razielhim corrupted vampire
	CLASS_RAZIELHIM, // raziel clan vampire 
	CLASS_VAMPIRE, // act like reborns. alliance standard vampire classes  
	CLASS_VAMPIRE_DEMON,// Samah'el - turelim boss classes 
	CLASS_KAIN,// Kain with full powers
	CLASS_KAIN2, // Kain bo2 era 
	CLASS_KAIN3,// Kain sion of balance
	CLASS_UMAH, // Vorador daughter 
	CLASS_ARIEL, // Ariel balance ghost 
	CLASS_VORADOR, // vorador class 
	CLASS_JANOS, // janos audron class 
	CLASS_NECROMANCER, // Vampire Necromancer 
	CLASS_VAMPIRE_QUEEN, // for Sheer and Deathmask. Fight like tavion. vampire necro. 
	CLASS_VAMPIRE_DARK, // vae 
	CLASS_VAMPIRE_NECRO, // Soul Liutenant  
	CLASS_VAMPIRE_NECRO2,// Deathmask with full powers 
	CLASS_VAMPIRE_FEAR, // fire vampire class - vampire of fear. 
	CLASS_VAMPIRE_FEAR2, // classe phobos knight of fear infernale 
	CLASS_VAMPIRE_BLOOD, // air vampire class - acrobatic bleeding vampire
	CLASS_VAMPIRE_SOUND, // bard vampire class. 
	CLASS_VAMPIRE_LYCAN, // werewolf Vampire Class. 
	CLASS_VAMPIRE_ELF, // evoluted elf vampire class with earth connection. pretty strong. 
	CLASS_VAMPIRE_ELF2, // Elf Vampire blue armor 
	CLASS_VAMPIRE_ELF3, // Elf Vampire blue armor rainbow lion class
	CLASS_VAMPIRE_WATER, // evoluted water vampire class with water resistance. 
	CLASS_VAMPIRE_FROST, // for Kainh conflict guardian with frost powers. 
	CLASS_VAMPIRE_ARCHER, // Archer - light vampire
	CLASS_HALFVAMPIRE, // Half human \ sarafan vampire 
	CLASS_VAMPIRE_BOUNTY, // Half vampire - dumahim
	CLASS_VAMPIRE_ZOMBIE, // half vampire - melchiahim 
	CLASS_VAMPIRE_SARAFAN,  // joachim 
	CLASS_WOLFFORM, // for vampires mutated into wolves
	CLASS_WOLFKAIN,// Kain with full powers
	CLASS_WOLFKAIN2,// Kain BO2
	CLASS_WOLFKAIN3,// Kain Sion of balance 
	CLASS_WOLFVORADOR, // vorador class 
	CLASS_WOLFJANOS, // janos audron class 
	CLASS_WOLFNECROMANCER, // Vampire Necromancer 
	CLASS_WOLFVAMPIRE_QUEEN, // for Sheer and Deathmask. Fight like tavion. vampire necro. 
	CLASS_WOLFVAMPIRE_FEAR, // fire vampire class - vampire of fear. 
	CLASS_WOLFVAMPIRE_FEAR2, // classe phobos knight of fear infernale 
	CLASS_HELLWOLF, // Samah'el metalupo 
	CLASS_WOLFVAMPIRE_LYCAN, // werewolf Vampire Class. 
	CLASS_WOLFVAMPIRE_ELF, // evoluted elf vampire class with earth connection. pretty strong. 
	CLASS_LIONVAMPIRE_ELF, // Elf Vampire blue armor 
	CLASS_LIONVAMPIRE_ELF2, // Elf Vampire blue armor lion Elf Vampire with rainbows 
	CLASS_WOLFHALFVAMPIRE, // Half human \ sarafan vampire 
	CLASS_ESKANDOR, // Hybrid vampire 
	// BAT FORM 
	CLASS_BATFORM, // for vampires mutated into bats 
	// PERSONAL BAT FORM
	CLASS_BATKAIN, // Kain Bat form
	CLASS_BATKAIN2, // Kain bo2 bat form 
	CLASS_BATKAIN3, // kain prophecy bat form 
	CLASS_BATVORADOR, // Vorador Bat form 
	CLASS_BATNECROMANCER, // Soul Bat form 
	CLASS_BATVAMPIRE_QUEEN, // Sheer Bat form 
	CLASS_BATVAMPIRE_FROST, // Kainh bat form 
	CLASS_HELLBAT, // Samah'el Colibri
	CLASS_BATVAMPIRE_FEAR, // Phobos bat form 
	CLASS_BATVAMPIRE_FEAR2, // Phobos demonic bat form 
	CLASS_BATVAMPIRE_WATER, // Shaar Naik Bat form
	CLASS_BATVAMPIRE_ELF, // Elf Vampire bat form 
	CLASS_BATVAMPIRE_ELF2, // Elf Vampire khyroptera form 
	CLASS_BATVAMPIRE_ELF3, // Elf Vampire khyroptera form full power. 
	// MIST FORM 
	CLASS_MISTFORM, // for vampires mutated into fog 
	// PERSONAL MIST FORM 
	CLASS_MISTKAIN,// Mist default kain 
	CLASS_MISTKAIN2,// Mist Kain BO2 
	CLASS_MISTKAIN3, // Mist Kain prophecy
	CLASS_MISTVORADOR, // Mist vorador 
	CLASS_MISTJANOS, // Mist janos audron 
	CLASS_MISTNECROMANCER, // Mist soul 
	CLASS_MISTVAMPIRE_QUEEN, // Mist sheer 
	CLASS_MISTVAMPIRE_DARK, // Mist vae 
	CLASS_MISTVAMPIRE_FEAR, // mist phobos 
	CLASS_MISTVAMPIRE_FEAR2,// Mist phobos fear knight 
	CLASS_MISTVAMPIRE_BLOOD, // Mist osil 
	CLASS_MISTVAMPIRE_WATER,// shaar naik mist form 
	CLASS_HELLMIST, // samah'el mist form 
	CLASS_MISTVAMPIRE_SARAFAN, // Joachim mist form 
	CLASS_MISTVAMPIRE_ELF,// Elf Vampire basic mist form
	CLASS_MISTVAMPIRE_ELF2, // Elf Vampire Iridu mist form
	CLASS_MISTVAMPIRE_ELF3,// Elf Vampire paladine mist form
	// REAVERS
	CLASS_REAVER, // STANDARD REAVER CLASSES 
	CLASS_RAZIEL,// reaver with spirit reaver 
	CLASS_RAZIEL2,// reaver with holy reaver
	CLASS_RAZIEL3, // reaver with life reaver 
	CLASS_NECROREAVER, // necro reaver class 
	CLASS_DARKREAVER, // dark reaver class
	CLASS_TANKREAVER, // tank reaver class 
	CLASS_WARMREAVER, // dark warm reaver class 
	CLASS_LIGHTREAVER,// light reaver class 
	CLASS_DRUIDREAVER, // druid reaver class 
	CLASS_DRAGONREAVER, // dragonide class
	CLASS_WATERREAVER, // water reaver class 
	CLASS_ICEREAVER, // ice reaver class 
	CLASS_SPIDERREAVER,// Spider reaver class 
	CLASS_SPIDERREAVER2,// Xado mud reaver
	CLASS_SPIDERREAVER3,// xado poison reaver
	CLASS_FIREREAVER, // fire - turelim reaver class 
	CLASS_AIRREAVER, // air reaver class act like rebel class 
	CLASS_MENTALIST, // psychic reaver class, act like Luke class 
	CLASS_EARTHREAVER, // Earth reaver class. 
	// Hyldens - usano artigli o armi da fuoco hylden! 
	CLASS_HYLDEN, // Elite hylden blacksmith glyphic class 
	CLASS_HYLDEN_FORCE, // Fabbro dei Glifi - Forza
	CLASS_HYLDEN_STONE, // Fabbro dei Glifi - Pietra 
	CLASS_HYLDEN_SOUND, // Fabbro dei Glifi - Suono
	CLASS_HYLDEN_WATER, // Fabbro dei Glifi - Acqua
	CLASS_HYLDEN_FIRE, // Fabbro dei Glifi - Fuoco
	CLASS_HYLDEN_LIGHT, // Fabbro dei Glifi - Luce 
	CLASS_HYLDEN_DARK, // Fabbro dei Glifi - assassino
	CLASS_HYLDEN_WARRIOR, // Guerriero Hylden

	CLASS_HYLDEN_MADNESS, // Mastro della Follia Hylden
	CLASS_HYLDEN_NULL, // Mastro del Nulla Hylden
	CLASS_HYLDEN_STAGNATION, // Mastro della Stagnazione Hylden
	CLASS_HYLDEN_CORRUPTION, // Mastro della Corruzione Hylden
	CLASS_HYLDEN_ENTROPY, // Mastro dell'Entropia Hylden 
	CLASS_HYLDEN_TIMELESS, // Mastro Rubatempo Hydeln
	CLASS_HYLDEN_ANTIMATTER, // Mastro dell'Antimateria
	CLASS_HYLDEN_GLYPH, // Mastro dei Glifi Hylden
	CLASS_HYLDEN_CHAOS, // Mastro del Chaos Hylden
	// Hylden Bosses 
	CLASS_HYLDENLORD, // For Hylden king and Hylden warlorcks. Hash Ak Gik
	CLASS_HYLDENLORD_MADNESS, // Signore della Follia Hylden
	CLASS_HYLDENLORD_NULL, // Signore del Nulla Hylden
	CLASS_HYLDENLORD_STAGNATION, // Signore della Stagnazione Hylden
	CLASS_HYLDENLORD_CORRUPTION, // Signore della Corruzione Hylden
	CLASS_HYLDENLORD_ENTROPY, // Signore dell'Entropia Hylden 
	CLASS_HYLDENLORD_TIMELESS, // Signore Rubatempo Hydeln
	CLASS_HYLDENLORD_ANTIMATTER, // Signore dell'Antimateria Hylden
	CLASS_HYLDENLORD_GLYPH, // Signore dei Glifi Hylden
	CLASS_HYLDENLORD_CHAOS, // Signore del Caos Hylden
	CLASS_HYLDENKING, // the King of the Hylden: usa l'emblema del caos completo! 
	// Demons 
	CLASS_DEMON_DARK, // demone ombra 
	CLASS_DEMON_LIGHT, // demone della luce
	CLASS_DEMON_FIRE,// For Fire demon . 
	CLASS_DEMON_AIR, // demone dell'aria 
	CLASS_DEMON_WATER, // demone acquatico
	CLASS_DEMON_ICE, // demone del ghiaccio
	CLASS_DEMON_EARTH, // demone della terra
	CLASS_DEMON_SOUND, // demone del suono
	CLASS_DEMON_POISON,// for poison demon 
	CLASS_DEMON_NECRO, // demon necromantic
	CLASS_DEMON_BLADE, // for demonblades 
	CLASS_DEMON_ELECTRO, // for demon lightnings
	CLASS_DEMON_TAUROC, // demoni taurini del fuoco rosso
	CLASS_DEMON_TAUROC_POISON,// demoni taurini del veleno
	CLASS_DEMON_TAUROC_BLACK, // demoni taurini del fuoco nero 
	CLASS_DEMON_TAUROC_ELECTRO, // demoni taurini elettrici
	// Undeads 
	CLASS_THRALL, // elite thrall fighting class 
	CLASS_POLTER, // act like rockettrooper class but their undead
	// Mutanti
	CLASS_MUTANT, // mutante del dark eden 
	// Elementals 
	CLASS_DARK, // darkness elite class
	CLASS_LIGHT, // light- electro-sun elite class 
	CLASS_FIRE, // fire-warm element class 
	CLASS_WATER,// water class 
	CLASS_AIR, // wind fog class  
	CLASS_EARTH,// earth elemental class 
	CLASS_POISON, // poison elemental
	CLASS_NECRO,// for lichs 
	CLASS_SPIRIT, // Spirit elementals
	CLASS_HOLY, // for holy creatures 
	CLASS_LIFE, // elementals 
	CLASS_IVY, // plants classes 
	CLASS_SONIC, // fight like reborn, scream like howlers 
	
	CLASS_SILENCE, // Elementali del Silenzio 
	CLASS_ASH, // Elementali della Cenere
	CLASS_VOID, // Elementali del Vuoto
	CLASS_SALT, // elementali del Sale
	CLASS_DUST, // Elementali della Polvere 
	CLASS_ASTRAL, // Elementali Astrali
	CLASS_RADIANCE, // Elementali della Radianza
	CLASS_LIGHTNING, // Elementali del Fulmine
	CLASS_FOG, // elementali della Nebbia
	CLASS_CRYSTAL, // Elementali del Cristallo
	CLASS_ICE, // elementale di ghiaccio
	CLASS_SMOKE,// elementale di fumo
	CLASS_LAVA,// elementale di lava
	CLASS_MUD,// elementale di fango
	CLASS_WAVE, // elementale delle maree
	CLASS_STORM,// elementale di tempesta
	CLASS_WARM,// elementale di calore
	CLASS_STONE,// elementale di pietra
	CLASS_METAL, // elementale metallico
	// CREATURES
	CLASS_BIRD, // flying creatures
	CLASS_BEAST, // animals 
	CLASS_SHARK, // water creatrues 
	// ELDER GOD
	CLASS_TENTACLE, // act like sand creature 
	CLASS_ELDERGOD, // the elder god class 
	// GOLEMS // Strong saber resistance, they cannot jump! 
	CLASS_GOLEM_DARK, // golem della tenebra
	CLASS_GOLEM_LIGHT,// golem della luce
	CLASS_GOLEM_GOLD, // golem aureo 
	CLASS_GOLEM_FIRE, // golem di fuoco 
	CLASS_GOLEM_WATER,// golem d'acqua
	CLASS_GOLEM_AIR,// golem dell'aria
	CLASS_GOLEM,// golem di terra
	CLASS_GOLEM_SONIC, // golem sonico
	CLASS_GOLEM_SPIRIT, // Golem di Spirito
	CLASS_GOLEM_HOLY, // golem sacro 
	CLASS_GOLEM_LIFE, // golem di vita 
	CLASS_GOLEM_SILENCE, // golem del silenzio
	CLASS_GOLEM_ASH, // golem di cenere 
	CLASS_GOLEM_VOID, // golem del vuoto
	CLASS_GOLEM_SALT, // golem di sale 
	CLASS_GOLEM_DUST, // golem di polvere 
	CLASS_GOLEM_ASTRAL, // Golem Astrale
	CLASS_GOLEM_RADIANCE, // Golem Radiante
	CLASS_GOLEM_LIGHTNING, // golem del fulmine
	CLASS_GOLEM_FOG, // golem della nebbia 
	CLASS_GOLEM_CRYSTAL, // golem di cristallo 
	CLASS_GOLEM_ICE, // golem di ghiaccio
	CLASS_GOLEM_SMOKE, // golem di fumo
	CLASS_GOLEM_LAVA, // golem di lava
	CLASS_GOLEM_MUD, // golem di fango
	CLASS_GOLEM_WAVE, // golem delle maree
	CLASS_GOLEM_STORM, // golem di tempesta
	CLASS_GOLEM_WARM, // golem di calore
	CLASS_GOLEM_STONE, // golem di pietra
	CLASS_GOLEM_IVY, // golem pianta
	CLASS_GOLEM_METAL, // golem metallico 
	CLASS_DECAL, // elementale o golem di decale 
	// DEVILGOLEM
	CLASS_HELLGOLEM, 
	CLASS_GOLEM_NECRO,
	CLASS_GOLEM_POISON, 
	// Naeryan Forma Eterea
	CLASS_NAERYANMIST,
	CLASS_NAERYANMIST2, 
	CLASS_NAERYANMIST3, 
	CLASS_DOLL, 
	CLASS_TEDDYBEAR,
	CLASS_DOLLMAKER,
	// ASTROFANTASY CLASSES 
	CLASS_VESU, // fire resistance, weak to frost
	CLASS_TORCH, // fire flying class
	CLASS_PYRO,// fire beasts
	CLASS_PYROFISH, // warm resistance water monsters 
	CLASS_KHROLEN,// light resistance class 
	CLASS_DAKTUR, // light bird class  
	CLASS_CRALATOR, // sand beast 
	CLASS_LADAQ, // light water beast 
	CLASS_VEGETER,// animated plant 
	CLASS_VEGRET, // psychic plant queen 
	CLASS_MONKER, // jungle class 
	CLASS_APER,// jungle evil class 
	CLASS_CRANION,// psychic alien cop class
	CLASS_ROBOPOC, // cranion androids
	CLASS_SHAMOR, // psychic alien 
	CLASS_NARK, // standard mountain nark class 
	CLASS_GELIOT,// ice nark
	CLASS_BEZ,// fire nark
	CLASS_FORLOK,// forest nark and nemphis
	CLASS_ARLAAN,// wind nark
	CLASS_LITER,// dwarf nark 
	CLASS_COGON, // light nark
	CLASS_SONARI,// bards nark 
	CLASS_KWARG,// desert nark 
	CLASS_GORK, // dark nark 
	CLASS_FAIR, // Orlan, good spirit class 
	CLASS_OGRE,// Demonic nark 
	CLASS_GUTUK, // undead nark 
	CLASS_VARK,// vampire nark  
	CLASS_WARK, // underworld bat devil nark 
	CLASS_GOBLIN, // nark thieves 
	CLASS_CENTOPULOS, // horsearmored aliens 
	CLASS_KOROTBEAST, // armored beast
	CLASS_RAGAN, // water aliens
	CLASS_IDRUSBEAST,	// fish aliens
	CLASS_POSUS, // Emperor of ragan 
	CLASS_FALKOR, // winged aliens
	CLASS_DRAGON, // winged icarus beast
	CLASS_ICARUSBEAST, // icarus generic beast
	CLASS_SFERX,// frost alien 
	CLASS_SFEROID, // frost cryus creature 
	CLASS_WINGOID, // winged cryus creature 
	CLASS_ANTWER, // ant - men class 
	CLASS_SPELEOBEAST, // creatures of karsus. (immune to darkness, weak to light)
	CLASS_SKEELDER, // evil class. immune to poisons, weak to holy. 
	CLASS_OVLON, // Skeelder necromantic ghost 
	CLASS_NECROBEAST, // skeelder crazy animals 
	CLASS_NECROBIRD, // skeelder crazy flying monsters 
	CLASS_DARKDRAGON, // Dragons of erebus
	CLASS_VOD,// Voice of Darkness super lich class  */
	CLASS_FIGHTER, // Fighter npc class for air \ space battles. - generic fighter 
	CLASS_CANNON, // blaster and seeker turrets
	CLASS_TANK, // armored tank battle tank
	CLASS_MANX, // meccanic spider battle tank
	CLASS_WALKER, // for mech like atat
	CLASS_SKIMMER, // for water hovercraft tanks 
	CLASS_FYRON, // vesus fighters 
	CLASS_NIUMUN, // Khrolen fighters - contrapposto ai caccia swarm 
	CLASS_SPIDER, // Vegeter automatic fighters
	CLASS_IVER, // Monker fighter - contrapposto ai caccia reclute skeelder 
	CLASS_ASHAK, // Aper fighter - bombardieri skeelder 
	CLASS_HOVERSTAR, // cranion fighters 
	CLASS_TRANSFER, // Cranion cruiser
	CLASS_KOSMER, // Cranion mothership
	CLASS_ANCLE, // Centopulos bombers - contrapposto ai caccia scrabble 
	CLASS_TRIDEB, // Ragan interceptor 
	CLASS_EAGLE, // Falkor aerofighter - contrapposti ai caccia classici skeelder Scorpions
	CLASS_BOWLER, // Sferx cloacked fighter 
	CLASS_STINGER, // Antwer interceptor 
	CLASS_SKULL, // Skeelders fighters class 
	CLASS_VECTOR, // skeelder fighter carryer
	CLASS_DESTROYER, // assault warship
	CLASS_NIGHTCRAWLER, // Dreadnought warship 
	CLASS_STATION, // Space station
	CLASS_OLON, // Hero fighter ship 
	CLASS_NUM_CLASSES
} class_t;

extern stringID_table_t ClassTable[];


/* FOR COPY \ PASTE FASTLY CLASS INSIDE WP_SABER.CPP FOR FORCE POWERS 

							case CLASS_PLAYER:			 
							case CLASS_VEHICLE:			
							case CLASS_MANDA:			/

// HUMANS
							case CLASS_PRISONER: 
							case CLASS_RODIAN:
							case CLASS_ARCHER:  
							case CLASS_WARRIOR:  
							case CLASS_HUNTER: 
							case CLASS_SCOUT:   
							case CLASS_ALCHEMIST:
							case CLASS_GALAK: 
							case CLASS_DRUID: 
							case CLASS_TRANDOSHAN:  
							case CLASS_SARAFAN: 
							case CLASS_STORMTROOPER: 
							case CLASS_WIZARD:  
							case CLASS_IMPWORKER: 
							case CLASS_PROPHET: 
 							case CLASS_CENOBITA:
							case CLASS_IMPERIAL:
							case CLASS_JAN: 

// BEASTS 
							case CLASS_INTERROGATOR: 	
							case CLASS_REELO: 
							case CLASS_BIRD: 
							case CLASS_BEAST: 
							case CLASS_LYCAN:
							case CLASS_SHARK: 
	
		
// REAPERS
 							case CLASS_JEDI:
							case CLASS_REAVER: 
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
							case CLASS_EARTHREAVER: 
// REAPERS BOSS CLASS 
							case CLASS_RAZIEL: 
							case CLASS_HOLY: 
							case CLASS_LUKE: 
							case CLASS_MENTALIST: 


// VAMPIRES			
							// NON BOSS 
							case CLASS_REBORN:
							case CLASS_VAMPIRE:   
							case CLASS_VAMPIRE_DEMON: 
							case CLASS_KAIN2: 
							case CLASS_UMAH:  
							case CLASS_VAMPIRE_DARK:  
							case CLASS_VAMPIRE_NECRO:   
							case CLASS_VAMPIRE_FEAR:  
							case CLASS_VAMPIRE_BLOOD: 
							case CLASS_VAMPIRE_SOUND:  
							case CLASS_VAMPIRE_LYCAN: 
							case CLASS_MONMOTHA: 
							case CLASS_VAMPIRE_ELF: 
							case CLASS_ALORA:  
							case CLASS_VAMPIRE_WATER:  
							case CLASS_HALFVAMPIRE:  
							case CLASS_RAZIELHIM:  
							case CLASS_VAMPIRE_BOUNTY: 
							case CLASS_VAMPIRE_ZOMBIE:  
							case CLASS_VAMPIRE_SARAFAN:  
// BOSS 
							case CLASS_KYLE: 
							case CLASS_KAIN:
							case CLASS_KAIN3:   
							case CLASS_VORADOR: 
							case CLASS_JANOS: 
							case CLASS_VAMPIRE_FROST:  
							case CLASS_VAMPIRE_ARCHER:  
							case CLASS_NECROMANCER:   
							case CLASS_VAMPIRE_QUEEN:  
							case CLASS_VAMPIRE_FEAR2: 
							case CLASS_VAMPIRE_NECRO2:  
							case CLASS_VAMPIRE_ELF2: 
							case CLASS_VAMPIRE_ELF3: 
	
// FORMS 
							case CLASS_WOLFFORM: 
							case CLASS_WOLFKAIN:
							case CLASS_WOLFKAIN2:	
							case CLASS_WOLFKAIN3: 
	                        case CLASS_WOLFVORADOR: 
							case CLASS_WOLFJANOS:  
							case CLASS_WOLFNECROMANCER:  
							case CLASS_WOLFVAMPIRE_QUEEN:  
							case CLASS_WOLFVAMPIRE_FEAR: 
							case CLASS_WOLFVAMPIRE_FEAR2: 
							case CLASS_HELLWOLF: 
							case CLASS_WOLFVAMPIRE_LYCAN: 
							case CLASS_WOLFVAMPIRE_ELF:
							case CLASS_LIONVAMPIRE_ELF:
							case CLASS_LIONVAMPIRE_ELF2: 
							case CLASS_WOLFHALFVAMPIRE:
							case CLASS_ESKANDOR: 
							// BAT FORM 
							case CLASS_BATFORM:  
							case CLASS_BATKAIN: 
							case CLASS_BATKAIN2: 
							case CLASS_BATKAIN3: 
							case CLASS_BATVORADOR: 
							case CLASS_BATNECROMANCER: 
							case CLASS_BATVAMPIRE_QUEEN:  
							case CLASS_BATVAMPIRE_FROST: 
							case CLASS_HELLBAT:		
							case CLASS_BATVAMPIRE_FEAR:  
							case CLASS_BATVAMPIRE_FEAR2: 
							case CLASS_BATVAMPIRE_WATER: 
							case CLASS_BATVAMPIRE_ELF: 
							case CLASS_BATVAMPIRE_ELF2: 
							case CLASS_BATVAMPIRE_ELF3: 
	// MIST FORM 
							case CLASS_MISTFORM: 
	// PERSONAL MIST FORM 
							case CLASS_MISTKAIN:   
							case CLASS_MISTKAIN2: 
							case CLASS_MISTKAIN3: 
							case CLASS_MISTVORADOR: 
							case CLASS_MISTJANOS:  
							case CLASS_MISTNECROMANCER: 
							case CLASS_MISTVAMPIRE_QUEEN:  
							case CLASS_MISTVAMPIRE_DARK: 
							case CLASS_MISTVAMPIRE_FEAR: 
							case CLASS_MISTVAMPIRE_FEAR2:   
							case CLASS_MISTVAMPIRE_BLOOD: 
							case CLASS_MISTVAMPIRE_WATER:   
							case CLASS_HELLMIST:  
							case CLASS_MISTVAMPIRE_SARAFAN: 



// CORRUPTED VAMPIRES	
							case CLASS_TUSKEN: 
							case CLASS_MELC: 
							case CLASS_SUMMONER:
							case CLASS_ZEPH:  
							case CLASS_NOGHRI: 
							case CLASS_DECEIVER:
							case CLASS_WEEQUAY:    
							case CLASS_DUMAHIM:
							case CLASS_SWAMPTROOPER: 
							case CLASS_RAHABIM:   
							case CLASS_GRAN: 
							case CLASS_TURELIM:

// CORRUPTED BOSSES 
							case CLASS_GLIDER: 
							case CLASS_FLIER2: 
							case CLASS_LIZARD: 
							case CLASS_FISH: 
							case CLASS_CLAW:
// DEMONS
							case CLASS_WAMPA: 
							case CLASS_MINEMONSTER: 
							case CLASS_DEMON_FIRE:   
							case CLASS_DEMON_POISON:
							case CLASS_DEMON_BLADE:   
							case CLASS_DEMON_ELECTRO: 
// HYLDENS
							case CLASS_TAVION:
							case CLASS_HYLDEN: 
							case CLASS_DESANN: 
							case CLASS_HYLDENLORD:  

// ARTIFACTS 
							case CLASS_MARK1:
							case CLASS_MARK2: 
							case CLASS_R2D2:				// MECHANICAL GOLEM CLASS 
							case CLASS_R5D2:
							case CLASS_REMOTE:	
							case CLASS_SEEKER: 
							case CLASS_SENTRY: 
// ELEMENTALS 
							case CLASS_SABOTEUR: 
							case CLASS_DARK:
							case CLASS_GOLEM_DARK: 
							case CLASS_GOLEM_SMOKE: 
							case CLASS_BESPIN_COP: 
							case CLASS_GOLEM_VOID: 
							case CLASS_VOID: 
							case CLASS_SHADOWTROOPER: 

							case CLASS_LIGHT:
							case CLASS_GOLEM_LIGHT: 
							case CLASS_GOLEM_GOLD: 
							case CLASS_SABER_DROID: 
							case CLASS_ASSASSIN_DROID: 
							case CLASS_PROTOCOL: 
							case CLASS_MORGANKATARN: 

							case CLASS_FIRE: 
							case CLASS_GOLEM_FIRE: 
							case CLASS_HELLGOLEM: 
							case CLASS_HAZARD_TROOPER: 
							case CLASS_GOLEM_LAVA: 
							case CLASS_CLAW: 
				
							case CLASS_WATER:  
							case CLASS_SWAMP: 
							case CLASS_GOLEM_WATER: 
							case CLASS_BARTENDER: 
							case CLASS_GOLEM_ICE: 
							case CLASS_CRYSTAL:
							case CLASS_GALAKMECH: 

							case CLASS_AIR: 
							case CLASS_GOLEM_AIR: 
							case CLASS_GOLEM_METAL: 
							case CLASS_REBEL:			 
							case CLASS_ROCKETTROOPER:
							case CLASS_BOBAFETT: 

							case CLASS_GONK: 
							case CLASS_UGNAUGHT: 
							case CLASS_GOLEM: 
							case CLASS_EARTH: 
				
							case CLASS_HOWLER:
							case CLASS_SONIC: 
							case CLASS_GOLEM_SONIC: 
							case CLASS_LANDO: 
	
							case CLASS_IVY: 
							case CLASS_POISON: 
							case CLASS_GOLEM_POISON: 

							case CLASS_GOLEM_HOLY: 
							case CLASS_HOLY: 
							case CLASS_LUKE: 

// UNDEADS
							case CLASS_COMMANDO:
							case CLASS_MURJJ: 
							case CLASS_NECRO: 
							case CLASS_GOLEM_NECRO:
							case CLASS_THRALL: 
							case CLASS_POLTER:
							case CLASS_PROBE: 
// WRAITHS
							case CLASS_ARIEL:
							case CLASS_SLUAGH:
							case CLASS_RANCOR: 
							case CLASS_REAPER: 
							case CLASS_DREADNOUGHT: 
							case CLASS_GHOST:
							case CLASS_JAWA: 
							case CLASS_VAMPIRE_GHOST:
							case CLASS_MOUSE: 

// ELDER GOD 
							case CLASS_TENTACLE: 
							case CLASS_ELDERGOD:
							case CLASS_SAND_CREATURE:
							case CLASS_ATST:				 
	





*/

typedef enum //# World_e
{
	WORLD_NONE,
	// ASTROFANTASY RACES 
	VORKAN,
	NUBIKUS,
	ZOLTAN,
	SELVARUS,
	DEMETRIUS,
	KIRDUS, 
	KOROT, 
	IDRUS, 
	ICARUS, 
	CRYUS, 
	KARSUS, 
	EREBO, 
	SPACEVOID, 
	UNDERWATER,
	NUM_WORLDS, 
} World_t;
extern stringID_table_t WorldTable[];


typedef enum //# Level_e
{
	LEVEL_NONE,
	// ASTROFANTASY RACES
	// LEVEL0, //100HP 100 MP 1 mp reg secondo
	LEVEL1, // 110 HP 
	LEVEL2, // 120 HP
	LEVEL3, // 130 HP
	LEVEL4, // 140 HP
	LEVEL5, // 150 HP 110 MP 
	LEVEL6, // 160 HP
	LEVEL7, // 170 HP
	LEVEL8, // 180 HP
	LEVEL9, // 190 HP 
	LEVEL10,// 200 HP 120 MP 2 mp/reg secondo 
	LEVEL11,// 210 HP 
	LEVEL12,// 220 HP
	LEVEL13,// 230 HP 
	LEVEL14,// 240 HP 
	LEVEL15, // 250 HP
	LEVEL16, // 260 HP
	LEVEL17, // 270 HP
	LEVEL18, // 280 HP
	LEVEL19, // 290 HP
	LEVEL20, // 300 HP 130 MP 3 mp/reg/sec 
	LEVEL21, // 310 HP
	LEVEL22, // 320 HP
	LEVEL23, // 330 HP
	LEVEL24, // 340 HP
	LEVEL25, // 350 HP
	LEVEL26, // 360 HP 
	LEVEL27, // 370 HP 
	LEVEL28, // 380 HP 
	LEVEL29, // 390 HP 
	LEVEL30, // 400 HP 140 MP 4 mp regen secondo 
	LEVEL31, // 410 HP 
	LEVEL32, // 420 HP 
	LEVEL33, // 430 HP 
	LEVEL34, // 440 HP
	LEVEL35, // 450 HP
	LEVEL36, // 460 HP
	LEVEL37, // 470 HP
	LEVEL38, // 480 HP
	LEVEL39, // 490 HP
	LEVEL40, // 500 HP 150 MP 5 mp reg secondo 
	LEVEL41, // 510 HP
	LEVEL42, // 520 HP
	LEVEL43, // 530 HP
	LEVEL44, // 540 HP
	LEVEL45, // 550 HP
	LEVEL46, // 560 HP 
	LEVEL47, // 570 HP 
	LEVEL48, // 580 HP 
	LEVEL49, // 590 HP 
	LEVEL50, // 600 HP 160 MP 6 mp reg secondo 
	LEVEL51, // 610 HP 
	LEVEL52, // 620 HP 
	LEVEL53, // 630 HP 
	LEVEL54, // 640 HP
	LEVEL55, // 650 HP
	LEVEL56, // 660 HP
	LEVEL57, // 670 HP
	LEVEL58, // 680 HP
	LEVEL59, // 690 HP
	LEVEL60, // 700 HP 170 MP 7 mp reg secondo 
	LEVEL61, // 710 HP
	LEVEL62, // 720 HP
	LEVEL63, // 730 HP
	LEVEL64, // 740 HP
	LEVEL65, // 750 HP 175 MP 8 mp reg secondo 
	LEVEL66, // 760 HP 
	LEVEL67, // 770 HP 
	LEVEL68, // 780 HP 
	LEVEL69, // 790 HP 
	LEVEL70, // 800 HP 180 MP 9  mp reg secondo 
	LEVEL71, // 810 HP 
	LEVEL72, // 820 HP 
	LEVEL73, // 830 HP 
	LEVEL74, // 840 HP
	LEVEL75, // 850 HP 190 MP 10 mp reg secondo 
	LEVEL76, // 860 HP
	LEVEL77, // 870 HP
	LEVEL78, // 880 HP
	LEVEL79, // 890 HP
	LEVEL80, // 900 HP 200 MP 11 reg secondo 
	LEVEL81, // 910 HP 220 MP 
	LEVEL82, // 920 HP 240 MP
	LEVEL83, // 930 HP 260 MP
	LEVEL84, // 940 HP 280 MP
	LEVEL85, // 950 HP 300 MP 12 reg secondo
	LEVEL86, // 960 HP 320 MP 
	LEVEL87, // 970 HP 340 MP
	LEVEL88, // 980 HP 360 MP 
	LEVEL89, // 990 HP 380 MP
	LEVEL90, // 1000 HP 400 MP 13 reg secondo 
	LEVEL91, // 1000 HP 420 MP 
	LEVEL92, // 1000 HP 440 MP 
	LEVEL93, // 1000 HP 460 MP  
	LEVEL94, // 1000 HP 480 MP 
	LEVEL95, // 1000 HP 500 MP 14 reg secondo 
	LEVEL96, // 1000 HP 520 MP 
	LEVEL97, // 1000 HP 540 MP 
	LEVEL98, // 1000 HP 560 MP  
	LEVEL99, // 1000 HP 580 MP  
	LEVEL100,// 1000 HP 600 MP 15 reg secondo 
	LEVEL101, // 1000 HP 620 MP
	LEVEL102, // 1000 HP 640 MP 
	LEVEL103, // 1000 HP 660 MP 
	LEVEL104, // 1000 HP 680 MP 
	LEVEL105, // 1000 HP 700 MP 16 reg secondo
	LEVEL106, // 1000 HP 720 MP 
	LEVEL107, // 1000 HP 740 MP  
	LEVEL108, // 1000 HP 760 MP 
	LEVEL109, // 1000 HP 780 MP 
	LEVEL110, // 1000 HP 800 MP 17 reg secondo  
	LEVEL111,// 2000  HP 820 MP 
	LEVEL112, // 1000 HP 840 MP  
	LEVEL113, // 1000 HP 860 MP 
	LEVEL114, // 1000 HP 880 MP 
	LEVEL115, // 1000 HP 900 MP 18 reg secondo  
	LEVEL116, // 1000 HP 920 MP 
	LEVEL117, // 1000 HP 940 MP   
	LEVEL118, // 1000 HP 960 MP 
	LEVEL119, // 1000 HP 980 MP 19 reg secondo
	LEVEL120, // 1000 HP 1000 MP 20 reg secondo 
	NUM_LEVELS, 
} Level_t;
extern stringID_table_t LevelTable[];

typedef enum //# Armor_e
{
	ARMOR_NONE,
	// ASTROFANTASY RACES 
	// physical armors 
	ARMOR1, // 100 ARMOR POINT  
	ARMOR2, // 120 ARMOR POINT 
	ARMOR3, // 150 ARMOR POINT 
	ARMOR4, // 200 ARMOR POINT 
	ARMOR5, // 250 ARMOR POINT 
	ARMOR6, // 300 ARMOR POINT 
	ARMOR7, // 350 ARMOR POINT 
	ARMOR8, // 400 ARMOR POINT
	ARMOR9, // 450 ARMOR POINT
	ARMOR10, // 500 ARMOR POINT 
	ARMOR11, // 550 ARMOR POINT 
	ARMOR12, // 600 ARMOR POINT 
	ARMOR13, // 650 ARMOR POINT 
	ARMOR14, // 700 ARMOR POINT
	ARMOR15, // 750 ARMOR POINT 
	ARMOR16, // 800 ARMOR POINT 
	ARMOR17, // 850 ARMOR POINT 
	ARMOR18, // 900 ARMOR POINT 
	ARMOR19, // 950 ARMOR POINT 
	ARMOR20, // 1000 ARMOR POINT 
	NUM_ARMORS, 
} Armor_t;
extern stringID_table_t ArmorTable[];


#endif	// #ifndef TEAMS_H
