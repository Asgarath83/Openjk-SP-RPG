#include "bg_damage.h"
#ifdef _GAME
#include "g_local.h"
#elif defined (IN_UI)
#include "ui/ui_local.h"
#else
#include <../cgame/cg_local.h>
#endif
#include <../cjson/cJSON.h>
#define MAX_MEANSOFDAMAGE_BUFFER	32768
#define MAX_MEANSOFDAMAGE_ERROR		1024
std::vector<meansOfDamage_t> allMeansOfDamage;

// Fuck VS 2010! 
extern double cJSON_ToNumber(cJSON *item); 
extern int cJSON_IsArray(cJSON *item);

/*
=========================
ParseJSONColor
=========================
*/

static qboolean ParseJSONColor(cJSON* json, vec4_t& color)
{
	if (!cJSON_IsArray(json))
	{
		return qfalse;
	}

	for (int i = 0; i < cJSON_GetArraySize(json) && i < 4; i++)
	{
		color[i] = cJSON_ToNumber(cJSON_GetArrayItem(json, i));
	}

	return qtrue;
}

/*
=========================
ParseSingleMeansOfDamage

=========================
*/

static void ParseSingleMeansOfDamage(const char* name, cJSON* json) 
{
	meansOfDamage_t mod = { 0 };
	cJSON* jsonNode;
	cJSON* child;
	
	Q_strncpyz(mod.ref, name, sizeof(mod.ref));

	jsonNode = cJSON_GetObjectItem(json, "hitmarkerExplosives");
	mod.hitmarkerExplosives = cJSON_ToBooleanOpt(jsonNode, qfalse);

	jsonNode = cJSON_GetObjectItem(json, "inventoryName");
	Q_strncpyz(mod.inventoryName, cJSON_ToStringOpt(jsonNode, ""), MAX_QPATH);

	jsonNode = cJSON_GetObjectItem(json, "disintegrate");
	mod.disintegrate = cJSON_ToBooleanOpt(jsonNode, qfalse);

	jsonNode = cJSON_GetObjectItem(json, "modifiers");
	if (jsonNode) {
		child = cJSON_GetObjectItem(jsonNode, "ignoreArmor");
		mod.modifiers.ignoreArmor = cJSON_ToBooleanOpt(child, qfalse);

		child = cJSON_GetObjectItem(jsonNode, "ignoreShield");
		mod.modifiers.ignoreShield = cJSON_ToBooleanOpt(child, qfalse);

		child = cJSON_GetObjectItem(jsonNode, "armor");
		mod.modifiers.armor = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "shield");
		mod.modifiers.shield = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "organic");
		mod.modifiers.organic = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "droid");
		mod.modifiers.droid = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "beast");
		mod.modifiers.beast = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "lightside");
		mod.modifiers.lightside = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "darkside");
		mod.modifiers.darkside = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "hazardtrooper");
		mod.modifiers.hazardtrooper = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "sandcreature");
		mod.modifiers.sandcreature = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "assassindroid");
		mod.modifiers.assassindroid = cJSON_ToNumberOpt(child, 1.0);
		
		// legacy of Kain modifiers. 
		child = cJSON_GetObjectItem(jsonNode, "ghost");
		mod.modifiers.ghost = cJSON_ToBooleanOpt(child, qfalse);

		child = cJSON_GetObjectItem(jsonNode, "archon");
		mod.modifiers.archon = cJSON_ToBooleanOpt(child, qfalse);

		child = cJSON_GetObjectItem(jsonNode, "tentacle");
		mod.modifiers.tentacle = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "eldergod");
		mod.modifiers.eldergod = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "vampireghost");
		mod.modifiers.vampireghost = cJSON_ToNumberOpt(child, 1.0);
				
		// mortal creatures
		child = cJSON_GetObjectItem(jsonNode, "druid"); 
		mod.modifiers.druid = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "werewolf"); 
		mod.modifiers.werewolf = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "prophet"); 
		mod.modifiers.prophet = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "sarafan"); 
		mod.modifiers.sarafan = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "wizard"); 
		mod.modifiers.wizard = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "cenobita"); 
		mod.modifiers.cenobita = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "melchiahim"); 
		mod.modifiers.melchiahim = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "summoner"); 
		mod.modifiers.summoner = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "melkboss"); 
		mod.modifiers.melkboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "zephonim"); 
		mod.modifiers.zephonim = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "deceiver"); 
		mod.modifiers.deceiver = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "zephboss"); 
		mod.modifiers.zephboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "dumahim"); 
		mod.modifiers.dumahim = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "dumahboss"); 
		mod.modifiers.dumahboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "rahabim"); 
		mod.modifiers.rahabim = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "rahabboss"); 
		mod.modifiers.rahabboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "turelim");
		mod.modifiers.turelim = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "tyrant"); 
		mod.modifiers.tyrant = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "turelboss"); 
		mod.modifiers.turelboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "vampire"); 
		mod.modifiers.vampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "vampire_demon"); 
		mod.modifiers.vampire_demon = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "vampireboss"); // elder vampires 
		mod.modifiers.vampireboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "pureghost"); // ariel 
		mod.modifiers.pureghost = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "darkvampire"); 
		mod.modifiers.darkvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "maskvampire"); 
		mod.modifiers.maskvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "necrovampire"); 
		mod.modifiers.necrovampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "hellvampire"); 
		mod.modifiers.hellvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "airvampire"); 
		mod.modifiers.airvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "bardvampire"); 
		mod.modifiers.bardvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "wolfvampire"); 
		mod.modifiers.wolfvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "elfvampire"); 
		mod.modifiers.elfvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "halfvampire"); // half vampire  
		mod.modifiers.halfvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "frostvampire"); 
		mod.modifiers.frostvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "archervampire"); 
		mod.modifiers.archervampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "razielhim"); 
		mod.modifiers.razielhim = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "razielboss"); 
		mod.modifiers.razielboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "halfdemon"); 
		mod.modifiers.halfdemon = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "halfmelc"); 
		mod.modifiers.halfmelc = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "sarvampire"); 
		mod.modifiers.sarvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "shockvampire"); 
		mod.modifiers.shockvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "spidervampire"); 
		mod.modifiers.spidervampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "poisonvampire"); 
		mod.modifiers.poisonvampire = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "wolfform"); 
		mod.modifiers.wolfform = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "lionform"); 
		mod.modifiers.lionform = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "hellwolf"); 
		mod.modifiers.hellwolf = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "demonwolf");
		mod.modifiers.demonwolf = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "batform"); 
		mod.modifiers.batform = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "hellbat"); 
		mod.modifiers.hellbat = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "demonbat"); 
		mod.modifiers.demonbat = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "mistform"); 
		mod.modifiers.mistform = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "fearmist"); 
		mod.modifiers.fearmist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "darkmist"); 
		mod.modifiers.darkmist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "vapormist"); 
		mod.modifiers.vapormist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "bloodmist"); 
		mod.modifiers.bloodmist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "hellmist"); 
		mod.modifiers.hellmist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "elfmist"); 
		mod.modifiers.elfmist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "sarmist"); 
		mod.modifiers.sarmist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "necromist"); 
		mod.modifiers.necromist = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "reaver");  
		mod.modifiers.reaver = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "reaverboss"); 
		mod.modifiers.reaverboss = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "necroreaver");  
		mod.modifiers.necroreaver = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "darkreaver"); 
		mod.modifiers.darkreaver = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "tankreaver"); 
		mod.modifiers.tankreaver = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "lightreaver"); 
		mod.modifiers.lightreaver = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "arcanereaver"); 
		mod.modifiers.arcanereaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "waterreaver");
		mod.modifiers.waterreaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "icereaver"); 
		mod.modifiers.icereaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "spiderreaver");  
		mod.modifiers.spiderreaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "firereaver"); 
		mod.modifiers.firereaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "airreaver"); 
		mod.modifiers.airreaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "mentalist"); 
		mod.modifiers.mentalist = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "stonereaver"); 
		mod.modifiers.stonereaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "venomreaver"); 
		mod.modifiers.venomreaver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "psychic"); 
		mod.modifiers.psychic = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "planeshifter"); 
		mod.modifiers.planeshifter = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "wraitharmor");
		mod.modifiers.wraitharmor = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "naturist"); 
		mod.modifiers.naturist = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "energist"); 
		mod.modifiers.energist = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "timestreamer"); 
		mod.modifiers.timestreamer = cJSON_ToNumberOpt(child, 1.0);

		child = cJSON_GetObjectItem(jsonNode, "alchemist"); 
		mod.modifiers.alchemist = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "hylden");
		mod.modifiers.hylden = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "hyldenlord"); 
		mod.modifiers.hyldenlord = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "demon"); 
		mod.modifiers.demon = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "demonfire"); 
		mod.modifiers.demonfire = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "demonpoison");
		mod.modifiers.demonpoison = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "demonblade");   
		mod.modifiers.demonblade = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "demonelectro"); 
		mod.modifiers.demonelectro = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "thrall"); 
		mod.modifiers.thrall = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "poltergeist"); 
		mod.modifiers.poltergeist = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "shadow"); 
		mod.modifiers.shadow = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "voidness"); 
		mod.modifiers.voidness = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "sun"); 
		mod.modifiers.sun = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "water");  
		mod.modifiers.water = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "crystal"); 
		mod.modifiers.crystal = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "gale"); 
		mod.modifiers.gale = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "earth"); 
		mod.modifiers.earth = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "necro"); 
		mod.modifiers.necro = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "holy"); 
		mod.modifiers.holy = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "poison"); 
		mod.modifiers.poison = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "ivy"); 
		mod.modifiers.ivy = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "sonic"); 
		mod.modifiers.sonic = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemdark"); 
		mod.modifiers.golemdark = cJSON_ToNumberOpt(child, 1.0); 
		 
		child = cJSON_GetObjectItem(jsonNode, "golemlight"); 
		mod.modifiers.golemlight = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemgold"); 
		mod.modifiers.golemgold = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemfire"); 
		mod.modifiers.golemfire = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemwater"); 
		mod.modifiers.golemwater = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemfrost"); 
		mod.modifiers.golemfrost = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemair"); 
		mod.modifiers.golemair = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemmetal"); 
		mod.modifiers.golemmetal = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemsonic"); 
		mod.modifiers.golemsonic = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemholy"); 
		mod.modifiers.golemholy = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golem"); 
		mod.modifiers.golem = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "hellgolem"); 
		mod.modifiers.hellgolem = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "necrogolem"); 
		mod.modifiers.necrogolem = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemsmog"); 
		mod.modifiers.golemsmog = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemvoid"); 
		mod.modifiers.golemvoid = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golempoison"); 
		mod.modifiers.golempoison = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "golemlava"); 
		mod.modifiers.golemlava = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "ethereal"); 
		mod.modifiers.ethereal = cJSON_ToNumberOpt(child, 1.0); 
			
	
		// Now astrofantasy creatures! 
		// FIRST WORLD: VORKAN, VULCANIC WORLD! 
		child = cJSON_GetObjectItem(jsonNode, "vorkan"); 
		mod.modifiers.vorkan = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "blob"); 
		mod.modifiers.blob = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "blobster"); 
		mod.modifiers.blobster = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "vidra"); 
		mod.modifiers.vidra = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "carminia"); 
		mod.modifiers.carminia = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "volvox"); 
		mod.modifiers.volvox = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "pirosca"); 
		mod.modifiers.pirosca = cJSON_ToNumberOpt(child, 1.0);  

		child = cJSON_GetObjectItem(jsonNode, "ameboid"); 
		mod.modifiers.ameboid = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "centix"); 
		mod.modifiers.centix = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "pkela"); 
		mod.modifiers.pkela = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "konja"); 
		mod.modifiers.konja = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "rak"); 
		mod.modifiers.rak = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "pawk"); 
		mod.modifiers.pawk = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "limulus"); 
		mod.modifiers.limulus = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kerlak"); 
		mod.modifiers.kerlak = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "scorpion"); 
		mod.modifiers.scorpion = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "hypopod"); 
		mod.modifiers.hypopod = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "hypomech"); 
		mod.modifiers.hypomech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "snag"); 
		mod.modifiers.snag = cJSON_ToNumberOpt(child, 1.0); 
		
		child = cJSON_GetObjectItem(jsonNode, "pirocefalus"); 
		mod.modifiers.pirocefalus = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "zmija"); 
		mod.modifiers.zmija = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "vron"); 
		mod.modifiers.vron = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "spongia"); 
		mod.modifiers.spongia = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "torch"); 
		mod.modifiers.torch = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "forger"); 
		mod.modifiers.forger = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "branote"); 
		mod.modifiers.branote = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "shogoth"); 
		mod.modifiers.shogoth = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "jackal"); 
		mod.modifiers.jackal = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "myriad"); 
		mod.modifiers.myriad = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "vyvern"); 
		mod.modifiers.vyvern = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kikima"); 
		mod.modifiers.kikima = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "skorpius"); 
		mod.modifiers.skorpius = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "pirkar"); 
		mod.modifiers.pirkar = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "volvoma"); 
		mod.modifiers.volvoma = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "vesu"); 
		mod.modifiers.vesu = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "anorian"); 
		mod.modifiers.anorian = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kos"); 
		mod.modifiers.kos = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "zhorian"); 
		mod.modifiers.zhorian = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kromag"); 
		mod.modifiers.kromag = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "krodan"); 
		mod.modifiers.krodan = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "gorkan"); 
		mod.modifiers.gorkan = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "sed"); 
		mod.modifiers.sed = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "divinator"); 
		mod.modifiers.divinator = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "cannork"); 
		mod.modifiers.cannork = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "zarha"); 
		mod.modifiers.zarha = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "farha"); 
		mod.modifiers.farha = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "konga"); 
		mod.modifiers.konga = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "sorha"); 
		mod.modifiers.sorha = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "trikka"); 
		mod.modifiers.trikka = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kavel");  
		mod.modifiers.kavel = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "striga"); 
		mod.modifiers.striga = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "asher"); 
		mod.modifiers.asher = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "smoker"); 
		mod.modifiers.smoker = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "tarter"); 
		mod.modifiers.tarter = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "rusal"); 
		mod.modifiers.rusal = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "friggan");
		mod.modifiers.friggan = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "clayer"); 
		mod.modifiers.clayer = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "obsidier"); 
		mod.modifiers.obsidier = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "mudgolem"); 
		mod.modifiers.mudgolem = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "irongolem"); 
		mod.modifiers.irongolem = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "puppet"); 
		mod.modifiers.puppet = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "miniature"); 
		mod.modifiers.miniature = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "salamech"); 
		mod.modifiers.salamech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "pourer"); 
		mod.modifiers.pourer = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "tetramech"); 
		mod.modifiers.tetramech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "spidermech"); 
		mod.modifiers.spidermech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "crabmech"); 
		mod.modifiers.crabmech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "skormech"); 
		mod.modifiers.skormech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "recon"); 
		mod.modifiers.recon = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "scramech"); 
		mod.modifiers.scramech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "octomech"); 
		mod.modifiers.octomech = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "torturator"); 
		mod.modifiers.torturator = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "yulo"); 
		mod.modifiers.yulo = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "diver"); 
		mod.modifiers.diver = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "ignitek"); 
		mod.modifiers.ignitek = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "gladiator"); 
		mod.modifiers.gladiator = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "yerkken");
		mod.modifiers.yerkken = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "scavenger"); 
		mod.modifiers.scavenger = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "haunter"); 
		mod.modifiers.haunter = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "screamer"); 
		mod.modifiers.screamer = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "burner"); 
		mod.modifiers.burner = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "flyer"); 
		mod.modifiers.flyer = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "farg"); 
		mod.modifiers.farg = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kongar"); 
		mod.modifiers.kongar = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kef");  
		mod.modifiers.kef = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "washak"); 
		mod.modifiers.washak = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "sorr"); 
		mod.modifiers.sorr = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "keeper"); 
		mod.modifiers.keeper = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "ziva"); 
		mod.modifiers.ziva = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "mosok");
		mod.modifiers.mosok = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "jura"); 
		mod.modifiers.jura = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "dazbor"); 
		mod.modifiers.dazbor = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "devana"); 
		mod.modifiers.devana = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "owena"); 
		mod.modifiers.owena = cJSON_ToNumberOpt(child, 1.0);  

		child = cJSON_GetObjectItem(jsonNode, "ivonna"); 
		mod.modifiers.ivonna = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "khor");
		mod.modifiers.khor = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "zwan"); 
		mod.modifiers.zwan = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "svarg");
		mod.modifiers.svarg = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "vesna"); 
		mod.modifiers.vesna = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "stribo"); 
		mod.modifiers.stribo = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "jarion"); 
		mod.modifiers.jarion = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "wik"); 
		mod.modifiers.wik = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "magrok");
		mod.modifiers.magrok = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "dusah");
		mod.modifiers.dusah = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "arrukk"); 
		mod.modifiers.arrukk = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "likho"); 
		mod.modifiers.likho = cJSON_ToNumberOpt(child, 1.0); 
		
		child = cJSON_GetObjectItem(jsonNode, "veles");
		mod.modifiers.veles = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "babaya"); 
		mod.modifiers.babaya = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "chuva"); 
		mod.modifiers.chuva = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "bogatyr"); 
		mod.modifiers.bogatyr = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "bozna"); 
		mod.modifiers.bozna = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "morena"); 
		mod.modifiers.morena = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "fefnor"); 
		mod.modifiers.fefnor = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "pekle"); 
		mod.modifiers.pekle = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "radigost"); 
		mod.modifiers.radigost = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "zhymar"); 
		mod.modifiers.zhymar = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "tryglav"); 
		mod.modifiers.tryglav = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "horz"); 
		mod.modifiers.horz = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "boruta"); 
		mod.modifiers.boruta = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "borevit"); 
		mod.modifiers.borevit = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "cockatrix"); 
		mod.modifiers.cockatrix = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "bagiennik"); 
		mod.modifiers.bagiennik = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "kardag"); 
		mod.modifiers.kardag = cJSON_ToNumberOpt(child, 1.0); 

		child = cJSON_GetObjectItem(jsonNode, "cernobog"); 
		mod.modifiers.cernobog = cJSON_ToNumberOpt(child, 1.0); 

		/*
		// DEMETRIUS RACES 
		float demetrius; 
		float cranion; 
		float cefistan;
		float esper; 
		float yoshir; 
		float salomme; 
		float nezo; 
		float linervus;  
		float omh; 
		float jori;
		float gheor; 
		float evros; 
		float memnas; 
		float intellian;
		float selenita; 
		float robopoc; 
	// Demetrius Animals creatures
		float mushroom; 
		float swimalg; 
		float comitre; // musk
		float sponge; 
		float alcyon; // coral 
		float ceterum; // jelly fish  
		float hydra; 
		float plasmoid; 
		float worm; 
		float ubinoto; // verme 
		float geubo; // lombrico 
		float corda; // anfiosso
		float ostreum; // ostrica
		float snail; // cochlea; lumaca
		float nautila; // nautiloide
		float miriapod; 
		float gnatoke; 
		float opabinia; 
		float hallucigenia; 
		float tribby;
		float anomalocaris; 
		float aster; //starfish 
		float wiwaxia; 
		float fish; 
		float bug; 
		float frog; // bufone; 
		float snake; // anguish;  
		float turtle;  // testudina
		float diploid; // diplocaulo 
		float klamida; // klamidosauso 
		float lizard; 
		float raptor;
		float bird; 
		float dedera; // galline
		float camelus; // struzzi
		float sfenish; // pinguini  
		float cincia; // wood bird  
		float feles;  
		float elefos; // elefante 
		float mouse; // topo mus
		float erikus; // riccio
		float tato; // armadillo
		float ungulash; 
		float horse; // equus; 
		float wolf; // lycam 
		float bear; // BEORO, 
		float seal; // 	SIGLISH, foca
		float vesper; // bat
		float deer; //	PYGARGO, rupicapra
		float luter; // lontra 
		float monkey; 
		float lemur; 
		// Demetrius godness
		float mineria; 
		float miras;
		float astreus; 
		float adosh; 
		float olexa; 
		float farash; 
		float efesia; 
		float elish; 
		float maresh; 
		float crisara; 
		float delphon; 
		float lilium; 
		float megatron; // Cranion supercomputer
	// FOR THE ROBOPOC, SEE ZOLTAN LIST. 
	// Khrolen Animals
	  float nubikus; 
	  float aljar; // anguilloide 
	  float almaan; 
	  float shajara; // albero fenice
	  float rophir; // rana alata 
	  float mawray; // murena (serpente marino) 
	  float yuban; // serpente del deserto 
      float bunazar;  
	  float mosox; //mosoide 
	  float ladaq; // ittioide
	  float cralator; 
	  float daktur; 
	  float dik;  // galloide 
	  float saph; 
	  float phatan; //  
	  float dhibia; // mosquito
	  float formox; 
	  float omaan; // ofiurida. 
	  float rambut; // ragno stalatore pungente
	  float sarib; // scorpione tessitore 
	  float tursuk; // pianta carnivora
	// Khrolen Etnic folks
	  float khrolen; 
	  float agula; 
	  float xetol; 
	  float ilzam; 
	  float ebstraa; 
	  float haldaa; 
	  float orgalian; 
	  float jezwar;
	  float shizarin; 
	  float oghta; 
	  float zolag; 
	  float yangha; // navigatori 
	  float xerazim; // antichi isis del passato. 
	  float chor; 
	// magical creatures 
	  float abqa; // genio 
	  float mummy;// non morto   
	  float mayit; // non morto 
	  float ramalen; // spirito delle sabbie
	  float ifrit; // spirito del fuoco
	  float shabal; // fantasma del deserto 
	  float sham; // spirito del sole
	 float najima; // spirito degli astri 
	 float jabal; // gigante della terra
	 float haya; // spirito della vita 
	 float naxa; // spirito serpente
	// godness
	 float tolk; 
	 float onub;
	 float oris;
	 float krif;
	 float xeth;
	 float bestex;
	 float omor;
	 float cork; 
	 float attoth; 
	 float niumun; 
	 //////// ZOLTAN
	 float zoltan; 
	 //Animals
	float pnosa; 
	float irakta; 
	float kuhu; 
	float sucker; 
	float tarthoid; 
	float tricero; 
	float shocksnake; 
	// Planter
	float neuralg; 
	float zoia;
	float ninphal;
	float ninphtower; 
	float cactopod; 
	float xalma; 
	float wooddryad; 
	// Vegeter
	float vegeter;
	float vekla;
	float veknosa; 
	float veghusk;
	float vegnita; 
	float vegpha; 
	float veghem; 
	float groundiver; 
	float vegmolder; 
	float vegpira;; 
	float vegga;
	float vegsna;
	float vekker; 
	float penthas; 
	float vunkta; 
	float adduma; 
	float vegret;
	// ROBOPOC
	float robochoper; 
	float robotruc; 
	float ofidoc;
	float spideroc;
	float crabboc;
	float sgorboc; 
	float octopoc; 
	float recpoc;
	float scarpoc; 
	float trivelpoc;
	float cleaner; 
	float honpoc;
	float phonopoc; 
	float aeron; 
	float gladioc; 
	float cyaner; 
	// SELVARUS 
	// animals
	float slemer; 
	float floven; 
	float khyanl; 
	float bolina; 
	float gnatonk;
	float gnatek;
	float greenwidower; 
	float moum; 
	float memwbaw;
	float meganeura; 
	float kenda; 
	float trey; 
	float krong; 
	float krapthor; 
	float shishara; 
	float psaalab; 
	float krapen; 
	float baochom; 
	float kokob;
	float quetfloatak; 
	float babeo; 
	float thnika;
	float rodent; 
	float kromoon; 
	float smilodon; 
	float tix; 
	float protomoper; 
	float moper; 
	float vatrongo; 
	float elembam; 
	float rinonte; 
	float growa; 
	// monker 
	float monker;
	float vuuru;
	float ling;
	float ligda;
	float bosen;
	float billig; 
	float aara;
	float lugor; 
	float swart;
	float wimda;
	float geesa;
	float saak;
	float aper; 
	// magical creatures
	float comix; // coniglio sacro
	float benix; // floatebra cieca
	float sobatignela; // scorpione malefico
	float ntinti; // foca
	float koko; // delfino
	float mningi; // mostro
	float cipak; // cipactli. krong gigante. afloatteco 
	float tialte; // mostro marino serpente 
	float civata; // civtateo; spiriti non morti del vodoo; dannati; scheletrici; vampirici; donne
	float floatifloatima; // tfloatitfloatimime; i "mostri del cielo"; erano divinità-stelle o demoni.
	float floataund; // spiriti del suono 
	// godness 
	float phlung; // fuoco
	float thamopo; // energia; \ via
	float ponlu; // luce
	float yb; // notte
	float chenga; // foresta
	float sathanga; // animali 
	float bhoomi; // terra
	float tuka; // marini e fiumio
	float khvalla; // spiriti dell'aria serpenti piumati
	float slasma; // morti 
	float wotta; // spiriti delle cose
	// 4 spiriti supremi monker
	float dhom; // armonia
	float nahual; // spirito del bene
	float tecci; // spirito di ovolt; ispirato a teccifloattecatl
	float djanga; 
	//evil aper godness 
	float xaresh; 
	float albar; 
	float zalu;
	float ishmar; 
	float mictla;// demone re dei morti
	float kali; // dea della morte 
	float xolotl; 
	float centfloaton; // conigli infernali 
	float shiba;// la distruttrice 
	// KIRDUS 
	// animals
	float kirdus; 
	float alurko; 
	float alope; 
	float ekinope;
	float pteraspe; 
	float snamper;
	float spirolaid; 
	float spirochete;
	float spirococcus;
	float geit; // capride  
	float rodicornus; 
	float vleer; // caprottero
	float krifter; // gigantostraco
	float sprinkaan; // antrottero
	float glimmer; // emittero - insetti
	float chamlion; // camaleonte iguana
	float hydropside; // idropside
	float haged; // saura; \ rettili
	float pek; // piccacei; uccelli minori
	float gryphon;
	float roc; 
	// etnic nark groups
	float geliot; 
	float bezegoth; 
	float forlok;
	float cogon;
	float arlaan;
	float kwarg; 
	float liter;
	float sonar;
	float monar; 
	float wark; 
	float gork;
	float okur; 
	float netarian; 
	float arkanian; 
	float orc; 
	float satir;
	float nibelhor; 
	// nark magical creatures
	float fair; 
	float shernak;
	float dragon; 
	float dwarf; 
	float goblin;
	float windigo; 
	float blossom; // fiore mutato assassino
	float grotto; // giganti  
	float ogre; 
	float orthas; 
	float gutuk;
	float skeleton; 
	float ghoul; 
	float revenant; 
	float stezaar;
	float reaper; 
	float necrodragon; 
	float salamander; 
	float mutant;
	float blacksatir; 
	float imp;
	float pyrogoth; 
	float vansteen; // golem 
	float zaarsteen; // golem di sangue
	float stonemaster; 
	float occultist; 
	float lich; 
	float arcilich; 
	// eroi supremi 
	float nemedhes;
	float vainamon; 
	float krimhilda; 
	float xengort; 
	float xarzo;
	// godness
	float crowulf; 
	float nemphis; 
	float igner;
	float eikar;
	float aegir; // signore del mare 
	float zon;
	float maan;
	float nephala;
	float aarde;
	float gerucht;
	float jomur; 
	// evil godness \	demons 
	float versterf;
 	float medelus;
	float stilte; 
	float verstand; 
	float grendel; 
	float lok; 
	// KOROT CREATURES
	float korot; 
	float xila;//xalga
	float bromia;
	float pennatula;
	float lasiavus;
	float kitas;//xpiantaxstrangolantex
	float nemnos;//xanemonexgigante
	float gorgia;//xmedusax
	float skoulikos;//xvermex
	float sfoungara;//xfungoxsottomarino
	float kito;//xmolluscox
	float xituwa;//xlumancax
	float skrilapos;//xpolpox
	float satantopod;//xscolopendra
	//xbruchix
	float kampia;
	float mirikasti;
	float chelato;//xbrucoxcrostaceo
	float epidromeo;//xnecrofagox
	float gigantostraco;
	float skorpiopode;
	float sukoka;//xscorpionexcoccodrillox
	float kavuro;//xgranchioxgigante
	float acklante;//xgranchioxmantidexacklay
	float katfloataro;//xpescexriccio
	float angura;//xcetrioloxdixmarexx
	float krino;//xgiglioxdixmara
	float thalassa;//xsanguisugaxbloodgaufloater
	float ascidia;
	float psar;//xpesce
	float verda;//xstorione
	float celacan;//xcelacanto
	float dinyctus;//xpescexmostro
	float karcharia;//xsqualoxcorafloatato
	float placo;//xostracodermaxpescexcorafloatfloatato
	float estemmo;//xestemmosucox
	float diploderma;
	float krimaka;//xserpexgigantex
	float savra;//xlucertolaxaxseixfloatampex
	float vrissak;//xvaranoxaxseixfloatampe
	float tuatara;//xinkuat;iguanaxdellexpaludixax3xocchi,.
	float ankila;//xdinosauroxconxseixfloatampexextrexocchi;similexadxunxgigantescoxarmadillo.
	float kelenke;
	float shrako;//xuccelloxgigantexassassinoxx
	float phoro;
	float trichoto;
	float tapiron;
	float mirminko;//xscimmianxformichiere
	float trick;//xlontra
	float glypto;//xglitptodontexdixpw
	float maimo;//xscimmioaxxaseixarti.
	float tenonte;
	float cothero;
	float thyla;//xmastinoxgigantex
	float kentas;//xcentaurixselvaggix
	float rhinox;//xrinocernetexcarpaxarmadilloxmontanoù
	float sarko;//xorsoxbradipoxgigante.x
	float orrex;//striga xbufaloxrinocenrontexgrifloatfloatlyx
	float zorrag;
	float anthrako;//xippopotamo
	float megisto;//xpuma
	float xune;//xnexux
	//xpopolafloationix
	float talamos;
	float efesto;
	float ermete;
	float vestale;
	float eracle;
	float alake;
	float ilita;
	float aris;
	float paneo;
	float clapo;
	float demo;
	float ipneo;
	float lipare;
	float kraso;
	float erimante;
	float ghoro;
	float khron;
	float fezo;
	float artemis;
	float parna;
	float ura;
	float tarak;
	//xmagicalxcreature
	float oracma;
	float larus;
	float titan;
	float cyclop;
	float trioculus;
	float lestrigon;
	float pirkagia;//xsputafuoco
	float gerion;
	float lerna;// l'idra 
	float ippogrifo;
	float sfinx;
	float esperide;
	float nereide;
	float pegasus;
	float lione;
	float fatuo;//xfuocoxfatuox
	float tartaro;
	float bongol;//xgolemxd'ossax
	float typhoon;//xdragoxtifone
	float tefrago;//xprogeniexdixcenere
	float leuca;//xninfaxalberoxsacra
	float scilla;
	float cariddi;
	float stinfalo;//shrakoxdixferrox
	float pleiade;
	float voro;
	float ogigia;
	float deodon;//xcinghialexgigante
	float medea;//xstrega
	float amorpho;
	float phantas;
	float ortro;
	float tavro;
	float chiron;
	float methila;
	float elafe;
	float aracne;
	float eleade;
	float cria;
	float cerberus;
	float kalopolo;
	float colosso;
	float gegon;//xmostroxdixpietraxdelxsottosuolo
	float vrykola;//xcentauroxvampiro
	float fasma;//xcentauroxspettrox
	float ipnofago;//xmangiasonno
	float psicofago;//xmangiamente
	float necrofago;//xmangiaxmorte
	float amaxon;//xamafloatfloatone
	float narciso;
	float ioto;
	float europe;
	float tizio;//xbrigantix
	float callisto;
	float olico;//xsacroxcotheroxdixdiomede.x
	//xgodnessx
	float orlan;
	float aries;
	//xtoro:xvedixtavro
	float gemine;
	float cancer;
	 //xleone;vedixlione;
	float virgo;
	float libra;
	//xscorpionexvedixscorpionixgiganti
	//xsagittarioxvedixchirone
	float amaltea;//xcapricorno
	float aquarius;//xganimedex
	// IDRUS
	float idrus; 
	// animals
	float squedo; 
	float eondra; // squedo natante
	float dinia; 
	float parvanca;
	float tribrachio;
	float windermer; // squedo strisicante
	float troua; // spugna
	// meduse
	float ciatide;  
	float thalloca; // corallo
	float norcia; // anemone gigante
	float graptolite; 
	float edemsa; // caravella portoghese 
	float bobbit; // verme lombrico con mandicole taglienti
	float funisia; // sanguisuga
	float serpula; // polichete; sessile
	float ottoia; // conchiglia 
	float huitre; // ostriche
	float segra; // maiale di mare 
	float lumacon; // lumaca : tipi bellerofa; cardioa; emiola; escarga; omphalo
	// gasteropodi
	float ammonite; 
	float octocele; // polpo
	float nautocta; 
	// artropodi millepied
	float aysha; // marino; fondale 
	float mulion; //natante 
	float miriak; //millepiedi tessitore talpa gigante.
	// trilobiti
	float egiroka; // lobas; limulo; floatifosuro. // anomalocaride
	// crostacei
	float creva; // gamberetto
	float tetron;// granchio
	float herpa;  // paguro
	float homarda; // aragosta
	// granchi ragno \ 
	float argyro; // rana
	float mismen; // mimetco
	float marato; // saltatore 
	float mirmefloat; // formigca
	float smaragdo; // legionario ù
	// scorpioni d'acqua
	float euripteride; // marino; missote; abissale; pterigoto; fluviale.
	float dapha; // pulce d'acqua
 	float cochon; // onisco
	float barnaclo; // balano
	// echinodermi 
	float astrospa; // stella marina scalatrice
	float blastoide; // vario 
	float eutoid; // terrestre dei ghiacci
	float lucoide; // stella di luce
	float edriosta; // stella tentacolo.
	float furcaster; // ofiuridi: pentremita; tholla.
	float boucla; // riccio sfera con aculei retrattili 
	float echinosferide; // dei ghiacci
	float oursin; // lancia aculei. 
	float slimer; // spazzino
	float eterosta; // gigli di mare; sanguisuga etc. 
	float tunica; // ascidia
	float pica; // anfiosso
	// pesci
	// ostracodermi 
	float agna; // senza mandibola 
	float alesipo; // pterittoide
	float botrio; // pesce corazzato 
	float cauche; // dinictis
	float drepana; // razza ostraca
	float elicoprion; // anguilla
	// cartilaginei 
	float edesto; // manta; mants gigante; squalo elefante.
	float elasmo; // squalo del mare; eugero; squalo costiero;. 
	float gemua; // razza mutante
	float iboda; // pesce pila. 
	float ornitoprione; // luccio
	float requien; // martello
	// ossei
	float baieso; // chicco di riso
	float bulambo;// black moon necrofago 
	float fleura; // pesce fiore
	float mola; // pesce sole \ luna
	float morminco; // pesce elefante
	float muletta; // triglia
	float pandaso; // mastino;storione; gattuccio; con grosse guance
	// può andar fuori dall'acqua
	float pilana;// pesce generico marino  
	float penuko; // pesce pinguino
	float pike; // luccio
	float pikume; // barbo pesce generico d'acqua dolce 
	float pikurla; // pesce palla
	float punana; // tonno
	float pusko; // scorpione
	float volaro; // pesce rondine stensiella pesci volanti 
	float rinopto; // narvalo. 
	// trachino; leccia
	float vekane; // pesce vipera
	float vuku;//  pesce diavolo
	// pesci oggetto
	float amofe; // pesce amorfo; 
	float ikeia; //pesce sega; utensile
	float kaha; // pesce pala; minerario; litorio.
	float kahiai; // pesce seagrub
	float kahippo; // ippocampo 
	float kaia; // pesce rame elettrico
	float kaiaia; // pesce ascia; 
	float kaiame; // pesce unicorno \ chirurgo
	float kehu; // pesce chiave
	float kenton; // pesce scatola \ cassaforte
	float pakeda; // pesce zaino \ marsupio
	float kodhok; // rana psscatricie. 
	float pono; // domestico
	float pukelo; // pesce granchio
	float puokto; // pesce polpo
	float qapu; // pesce vela. 
	float pleco; // ventosa
	float renuko; // pulitore; 
	float oroko; // pesce gatto spazzino mangiarifiuti 
	float rukua; // pesce pietra militare 
	float tukea; // pesce mistico
	float yaya; // pesce sybil
	// cetacei
	float torpilla; // torpedine
	float wakapa; // balena
	float daufo; // delfino
	// pesci mistici
	float crapsico; // psichico
	float phantome; // pesce fantasma
	float cotto; // pesce coltello.
	float aptera; // piranha! 
	// pesci abissali
	float nageo; // pesce scafandro.
	float craptera; // kodhok malvagio abissale.;
	float mendako; // polpo dumbo.
	float harong; // re delle aringhe
	float voctok; // vampiroctele 
	// nautocta gigante
	float goreq; // squalo goblin
	// pesci stega
	float dipnoo;
	float oloptico; // trosa; eustena; oloptico.
	float latimera; // celacanto
	float mutrino; // pesce ornitorinco
	float ripidario; // perioftalmo
	float stega; // ittiostega
	float axolotl; // tritone albino
	float cacope; // drago rana
	float camlia; // camaleonte leone.
	float elipe; // orso luopo anfibio.
	// estemmo; vedi kotor 
	float iguanuga; // iguana tartaruga
	float lepidon; // lucertola con tetsta a boomerang.
	float koakoa; // anaconda
	float mosope; // ippopotamo
	float nexuro; // salamandra gigante parlante
	// rettili 
	float simora; // salamandra lucertola 
	// diploca: vedi lepidon
	float doser; // serpente murena
	float labida; // lucertola iguna
	float liken; // scimmia lucertola
	float mesosa; // mesosauro;
	float nostrago; // lucertola elefante
	float pareia; // tartaruga
	float scutosa; // scutosauro
	float sphenaco; // sfenacodonte
	float vipion;// // vipione. 
	// mistical e magical 
	float zaratan; 
	float carcino; 
	float ozena; 
	float kutor; 
	float haphgur; 
	float thanork;
	float ishtrik;
	float quinota; 
	float jomungo; 
	float bozumo; 
	float mellorg; 
	float yuko; 
	float hyppogra; 
	float phroth; 
	float shuruth; 
	// peoples
	float ragan; 
	float meron; // viola: cernie anfibie. droga. 
	float nauga; // blu: iguane nettuno ; minaccia dell’inquinamento marino. 
	// popolo di posus
	float maquere; // azzurri: sgombri \ scorfani  urano  minaccia della dimensione
	float remu; // bianchi: merluzzi \ pesci gatto saturno  ; minaccia del tempo
	float raina; // verdi:  ile  venere  minaccia della natura 
	float kaila; // bruni: tartarughe torpedine  mercurio  minaccia degli stati
	float rouger; // rossi: pesce scorpione marte ; minaccia del conflitto
	float jauna; // gialli serpentiformi tipo serpentrefloat di pw. sole minaccia dell’energia 
	float lyerh; // lyerh: mante pesci abissali; \ pesce vipera plutone  minaccia della morte. 
	// godness
	float posus; 
	// icarus
	// animals
	float icarus; 
	float plaguer; // batterio gigante
	float plesen; // aeromuffa
	float letayo; // fungo volante 
	float vodorov; // alga nastrescente 
	float moskisk; // superalga 
	float lishay;
	float morskoia; // pennatule
	float svetov; // fiore volante
	float cherva; // verme \ mollusco terrestre 
	float gusen; // verme articolato gigante
	float planor; // medusa assassina volante planoriofloatile 
	float nubiloide; // verme nuvola gigante mangia città 
	float oblako; // nuvola vivente 
	float osmino; // polpo alato \ polipoidi
	float midiakka;  //lamellocefali \ mollusco marino 
	float dushom; // 	mongolfieridi
	float naseko; // insetto icarese artridi 
	float yesmer; // mostro millepiedi mangiacadaveri millepiede serpente 
	float babochka; // insetto alato 
	float bapaho; // esasauro
	float yasker; // teatrasauro 
	float kryla; // saurottero - uccello rettile pipistrello pterodattilo
	float dargoon; // dragone 
	float lomoor; // lomorapti 
	float probon; // probonte
	float ptisa; // uccello generico
	// riunirli in categorie 
	float skonna; // esseri primordiali 
	float celophe; // celophiside
	float herrer; // herrerasauroo
	float rincosa; // rincosauro; mostro psichico boss 
	float protero; // mangiatore di insetti proterosuchus
	float uzha; // mostro simil dinosauro del triassico
	float melano; // mammifero primitivo icarese (su icarus i mammiferi non hanno successo evolutivo)(
	float cinognato; // terapside
	float nayabron; // corazza primordiale.// rettioli corazzati - terra 
 	float anchiso; // anchisauro - tipo brontosauro
	float gherron; // gerrontorace - diploauclo idrese
	float protosa; // coccodrillo lucertola
	float mifloatosa; // dinosauro mostrosuo chimera mifloatosauro . è il predatore inconstratsato degli oceani e delle terre icaresi
	float mastospe; // massospondilo
	float placodo; // placodonte. tartararuga icarese
	float tekodo; // mostro sismico. tecodontosauro
	float vozda; // mostri marini primordiali 
	float askepto; // askeptosauro - mostro marino 
	float dianoga; // mostro tipo shadow of empire ch einfesta le fogne
	float lystrosa; // dinosauro lucertola anfibia. lystrosauro
	float mifloato; // ittiosauro con coda da anguilla e becco da storione
	float notosa; // mostro degli oceani; notosauro. rettile marino fluviale
	float plateo; // plateosauro; dinosauro sferico dei ghiacci
	float floatifofloat; // floatifosuro - limulo; mostro acquatico gigante. ben più pericoloso di quelli di vorkan
	float pervoza; // uccello primordiale . 
	float eudimor; // eudirmorfodonde; pterodattilo elementale - tipo mephit di ded
	float kuhen; // kuehneosauro - tipo icarosauro; lucertola alata dei boschi con ali costola
	float peten; // rettile volante peteinosauro
	float podopto; // lucertola alata come icarosauro
	float tosuko; // coccodrillo uccello ornitosuchus 
	float steklo; // rettili di cristallo. 
	float clevo; // clevosauro ; montagne; terre di cristallo
	float etereo; // rettile vitreo heterodontosauro
	float kristall; // cristallo vivente
	float kristakker; // mostro di cristallo 2 rancor ai  
	float ruzion; // mostro delle pianure di vetro di sidrosforo. rutiodonte
	//  mostri delle terre mobili;
	// fatte di metallo liquido; sidrosforo;
	// rodoglass; cristallo; sidrosforo; vulcani gelati; terre di metallo; terre radiattive
	float skaar; // esaformi (uccelli con mani) avi dei falkor  
	// terre sospese; terre nuvola del grande vortice polare
	float lesotho; // dinosauro acrobatico
	float trilofo; // trilofosauro; mostro trifido scalatore
	float ziviz; // mostro della terra viva
	float bordal; // mostro della terra di cristallo
	float rozen; // mostro della terra di vetro di sidrosforo
	float plazer; // mostro di sidrosforo 
	float zidror; //mostro di sidrosforo 
	// peoples
	float gallas; // falkor armeni;
	float irish; // falkor del paradiso; arcobaleno. equatoriali ed esotici 
	float oriana; // falkor scarlatti cavalcadragh; guerrieri; baschiri.
	float dilan; // delle nevi; gufo pinguino sterna artica; ucraini di città di cristallo
	float crigg; // falkor silvano pappagallo aramaco; lappone
	float vulpus; // falkor pavone \ martin pescatore. acquatici. ciuvaschi
	float konda; // falkor del deserto; rapaci; giallinacei; \ mongoli - tartari
	float skree; // falkor neri delle grotte. gufi; corvi neri. ceceni
	float anakra; // falkor anatra \ paperi lol (boothand duck not disney stuff!) 
	// - le creauture elementali di icarus sono uccelli che usano il potere degli elementi storm
	float sferzer; //s1 attacchi termici; elettromagnetici. disgregazione; radiattivo. 
	float raden; // s2danno da luce; materia; esplosivo.  elevata rigeneraizone della mana. absorb. 
	float acren; // s3questo tipo di creeature elementali storm son dotate di continua rigeneraizone della mana.
	// danni elementali di ogni genere incrementati. invulnerabili dai danni mistici.
	float therzen; // s4 creatura elettromagnetica; danni di luce; fulmine; radianza; radiazioni; ionico.
	// sono letali sui robot e sulle macchine. usano therzio e negaprinio 
	float parzen; // s5 queste creature sono in grado di disgregare le vittime.
	float zenpa; // s6 questi esser usano il parzio beta; posson aumentare a dismisura di dimensone quando muoiono
	// o miniaturizzare le vittime. coloro che uccidon per miniaturizzazioe sono disgregati.;
	// le vittime uccise per ingrandimento sono mutate in buchi neri.
	float ufen; // s7 creature notturne affini al buio; alla morte; e al gelo che vivon nelle terre di cristallo. 
	// son immuni ai danni da luce; laser; elettromagnetici; tenebra e radiazione; acqua; gelo; magia nera.
	// posson rendersi invisibili abilità di cloacking. 
	float negapren; // s8 categorie di creature elettriche terribli e devastanti che lanciano attacchi aoe
	// di folgore; energia e tempesta assolutamente letali. volano per levitaizone
	// magnetica. sono anche in grado di generar uragani e tempeste di fulmini.
	float puller; // s9 creature in grado di alterar la massa dei corpi. posson disgregare le vittime ; tramutarle in tachioni
	// e spedirle nell'iperspazio. son in grado di comunicar istantameneamente fra di loro per mezzo di emissione
	// di ultraonde e hanno una mente collettiva. 
	float acrimer; // s10 infliggon danni di tempo; animspeedscale e movespeedscale assurdamnte veloci. fp_speed a 5
	// sono esseri temporali in grado di spostarsi fra passato presente e futuro e di storcer il tempo. 
	float camber; // s11 alterano la roccia e la gravità; infliggon danni astrali e son dotati di telecinesi. 
	// posson generar wormhole con cui teletrasportarsi da una parte all'altra del pianeta. 
	float breker; // s12 sono esseri kamikaze affini ai cultist destroyer che esplodono emettendo devasanti quantità di antimateria
	// e di energia esplosiva. sono pericolosissimi e uno di loro può far esploder una intera città. 
	float cafer; // s13 questi essei sono di metallo liquido superfludio e dimorano nel mantello del pianeta.
	// posson assujmer ogni forma e ibnfliggon dannin da metallo; sidrosfrro; elettricitrà. 
	// hanno effetti simli a isterio; entrofio; tetropio; antibio; alvidio e ncrebio. 
	// poteri: polverizzazione delle vittime; danno da veleno immenso.
	// emissione di vapori invecchianti che innetsano processi di necrosi e decomposizione. 
	// cotrollon di non morti di metallo liquido senza anima. 
	// son immuni ad ogni danno; deboli soloo agli elementi sacri! 
	float ruber; // s14 questi esseri non sono influenzati dalla gravità pertanto possono volare liberamnete.
	// sono creature di cristallo fatte di rubanio o vetro. hanno devastanti poteri 
	// telecinetici.  
	float effimer; // gli esseri di tale tipo posson volar ad altissime quote cadendo verso l'alto. 
	// son aerodinamici ed estremanmente veloci oltre che eterei e gassosi.
	float vozduch; // creature di pura aria; velocissime. 
	float clamer; // creature resistenti ai veleni fisici e spirituali e al vampirismo. 
	// cacciate per ricavarne droga.
	float koller; // queste creature son in grado di combinar fra di loro attacchi elementali
	// multipli colpendo con elementi ed energie ibride in modo randomico. 
	// uccisi droppano kollium.
	float catmer; // esseri in grado di generare vortici e tempeste di sabbia e cristallo.
	float phocrer; // esseri che infliggon danno termico; resistentissimi a calore e fuoco. 
	// gli uccelli di questo tipo sono tipo calcid the flame di pw.
	float vecher; // strida e attacchi sonici come gli howler. son uccelli sonici corridori.
	float temny; // questi uccelli son in grado di rendersi invisibili e attaccan con magie di tenebra.
	float wozzer; // creature che usano il potere di volterio; elettrolitio; relettrio; morio; cuzerio
	// e aurio. infliggon danno di luce; sacro; fulmine; radianza; metallo e son invincibli alla magia.
	// dimoranon nei cumulonembi e nelle nubi tempestose
	float sthroma; // questi esseri usano andio; corio; relettrio e irio.
	// danno da vento; pioggia; tempesta; gas; vuoto; pressione. molto agili e veloci. 
	// sembrano fatti di nuvole. quesrti elementi son usati anche dai rettili volanti 
	// del pianeta 
	float xiller; // omuncolo di gelo e cristallo in grado di abbassar le temperature a valori
	// prosismi allo zero assoluto e di evocar blizzard. estremanente pericooso; immune a fuoco e gelo.
	// usa allopirio come sostanza principale. 
	// strenio e tenterio: son usati dai rettili corazzati del pianeta. 
	// danno di terra; immuni alla terra; fp_protect a 3. possonn esser anche immuni ai danni fisici.
	float zirek; // questi esseri sono simili alle donne di metallo di lothranis e posson lanciar
	// attacchi di energia elementale aoe estremanente potenti. 
	float seeker; // queste creature usano il potere del baralio e hanno poteri psichici fp_sense a 3. però son sorde.
	float kronoser; // i kronoser sono pericolosi elementali di t	empo. vedi acrimio. 
	// uccisi droppano zaffiri di kronisio. 
	float freden; // i freden sono esseri che disgregano gli oggetti e infliggon danni da radiazione; molto pericolosi. 
	float radeon; // radeo questi esseri brillano come soli. immuni alla luce; se colpiti da attacchib di luce li acculano al loro interno per poi riemeterli sotto forma di aoe.
	// son affini alla luce come potere.
	float crayer; // esseri che attirano enormi quantità d'aria e che le usano contro i nemici. 
	// hanon totale dominio sull'aria. i crayer vivon nell'occhio degli immensi uragani del pianeta. 
	float iber; // questi esseri son in grado di infondersi di magia multielementale e di infligger danni random
	// di luce; buio; sacro; veleno; vita; morte; fuoco; terra; aria; acqua; gelo e calore; spazio e tempo. 
	// questi esseri son anche di metallo liquido deformabile a memoria di forma.;
	float ofloatene; // cristallo di ofloatirio senzientie. è un temibile essere gassoso dal nucleo nero e lucente; 
	// in grado di strappar l'ossigeno dalle vittime e di risucchiarlo in sé.
	float menser; // esseri rarissimi con abilità simili agli shamoriti dei cranion; intriis di
	// poteri telepatici devasanti. 
	// abbinali elementi charm e storm agli uccelli icaresi.  ogni specie ne detien un potere.
	//tinamide;
	float straus; // struzzo
	float rheida; //  rheiformes
    float wiki; // kiwi
	float casuar; // casuario
	float lebed; //  cigno; 
	float petuk; // galliformi.   megapodidi; numidi; quaglie; fagiani
	float gavin; //  gaviforme paperi; duck etc. 
	float sfenifloat; // pinguino
	float procella; // mariniprocellari.  diomedeidae; procellariidae; hydrobatidae; oceanitidae
	float svassus; // svasso 
	float pheton; //  phaethontiformes   phaethontidae  phoenicopteriformes    phoenicopteridae
	float cycon; // cicogna   ciconiiformes     ciconiidae
	float peleca; // pellicano   pelecaniformes
	// suliforme  
    float thresk; // uccello non morto cafornio 
 	float ardeide; // uccello di piertra - cambrio 
	float scopia; //uccello scorpione 
	float balaen; // uccello balena. esplosivo 
	// threskiornithidae; ardeidae; scopidae; balaenicipitidae; pelecanidae
	float phrega; //  sulidae; phalacrocoracidae; anhingidae; fregatidae
	// mostri elementali 
	float cathar; // avvoltoi 
	float sagittar; // serpentari 
	float pandion; // pandion
	float orel; // aquila 
	//  accipitriformes
    //  cathartidae; sagittariidae; pandionidae; accipitridae
	float falcon; // rapaci     falconiformes     falconidae
	float otide; //   otidiformes      otididae
	float magasca; // mesitornithiformes     mesitornithidae . madagascar
	float cariam; // cariamide 
	float pygifo; //  eurypygiformes eurypygidae; rhynochetidae
	// uccelli con corna e sputafuoco \ kagu e tarabuso 
	float gru; //  gruiformes
    // helior; // uccello del sole.
	// rallis; // rallidae; 
	//phosphor; // uccello del psophiidae; gruidae; 
	// aramid; // aramidae
	float chayka;  // gabbiano 
    // turnicidae; burhinidae; chionidae; pluvianellidae; haematopodidae; dromadidae; ibidorhynchidae; recurvirostridae; charadriidae; pluvianidae; rostratulidae; jacanidae; pedionomidae; thinocoridae; scolopacidae; glareolidae; laridae; stercorariidae; alcidae
	float pterocle; //  pterocliformes  pteroclidae
	float golub; //   columbiformes   columbidae 
	float popuga; // pappagalli 
	float hoatzin; //  opisthocomiformes     opisthocomidae
	float musofago; //  musophagiformes     musophagidae uccello alfeico
	float kokoo; //  cuculiformes       cuculidae ufonio 
	float fillin; //  gufo ufonio 
	float caprimul; // nittibi; guaciari
	float apus; // uccelli insetto senza piedi; come i colibrì. uccelli impollinatori
	float colide; //  coliiformes
    float trogol; // trogoniforme 
	float curol; //   leptosomiformes uccello guaritore
	float corace; // coraciiformes
	float vorona; // corvo ufonio 
	float dyatel; // picchio. piciforme galbulidae; bucconidae; capitonidae; semnornithidae; ramphastidae; megalaimidae; lybiidae; indicatoridae; picidae
	float vorobe; // passero
	// creatures mistich demoniache falkor  - oppopsti agli dei 
	float kimer; // chimaera - fuoco 
	float wiwer; // viverna - cristallo liquido
	float nephele; // nefele - tempesta 
	float floatolotl; // mostro simile a quetzalcoatl
	float hek;  // ehecatl; spirito del vento gelido e distruttore
	float garpya; // arpia; mostro della terra. 
	// godness
	float orian;// fuoco \ sidrosforo 
	float vulpur; // mare 
	float craig; // aria 
	float xilog; // gelo 
	float scriek; // terra
	float gal; // luce 
	// CRYUS
	// germi
	float criofagus; 
	float criococcus; 
	float criea; 
	float criozoo;
	// piante
	float prismer; // prismalghe
	float kristala; // cristallidi
	float baraphula; // fiore di ghiaccio 
	float gamsha;  // crioerbe
	float rukha; // criofloatili (alberi di cristallo)
	float cyau; // funghi: cloromiceti; feomiceti; rodomiceti
	float criusa; // medusa adiposa 
	float criorme; // verme adiposo
	float lola;	// ammoniti dei ghiacci  
	float aktopa; // polpo dei ghiacci 
	float cipleka; // criolumache
	float kamala; // miriofloat
	// gigachite tipo obaminie lumaca con conchiglie dorsali 
	// artonfloat; artropodi 
	float jingha; // aragosta \ scorpione del gelo 
	float makura; // ragno dei ghiacci 
	float kekada; // paguro gelido
	float kriketa; // tetronfloat simile a grillo corridore 
	float kamila; // formica; mantide; insetto in generale 
	float macha; //  pesce 
	float sarka; // pesce a squame silicee 
	float chapa; // anfibio foca (fan girini lol ) 
	float sahan; // orsidi
	float mathi; // criotopi 
	float caran; // uccello sferottere 
	float weizerfloat; // uccelli pescatori
	float kerfloat; // uccleli rapaci kerfloat  rapaci
	float bhamger;  // fruifloat - mangiano painte di cristallo
	float thala; // ittiosferidi manta 
	float hvela; // ceton 
	float dalphin; // ittion 
	float sarpa; // ofidoidi
	float ghumrek; // echinoidi
	float kanaga; // saltoidi
	float ekro; // polpoidi
	float bamda; //  antropoidi
	float guphar; // gufaruko; speleolidi (ragni) 
	float tila; //	litosferidi (talpe)
	float syala; // necrosferidi (sciacalli)
	float barnara; // termosferidi
	float botafloat; // fitosferidi
	float katilom; // razosferidi falciatori corazzati con lingue a frusta occhi composti zanne da vampiro
	// e schiena ricoperta da acueli
	// etnic group 
	float sferfloat; 
	float sferon;
	float neron; 
	// creature elementali \ divinità 
	// ogni divinità ha dei mostri elemental che usan la sua stessa classe.
	float grin; // dio cometa che viaggia fra i mondi a portar la vita per panspermia
	float parsi; // ginfa spiritio custodi di grin - ognuna è legata a simbolo atavico
	float varsa; // pioggia / acqua 
	float hium; // neve; gelo; freddo. 
	float surya; // sole
	float nyaya;	// grandine; dea della giustizia
	float nilago; // metano - fuoco azzurro 
	float vijaya; // luce - vittoria 
	float udana; 	// vento; ninfa alata 
	float candra; // luna-buio-grotta (buono) 
	// malvagi 
	float ciso; // gelo (malvagio) 
	float gupha; // luna-buio grotta (malvagio) 
	float balna; // fuoco ardente malefico. 
	float amdhi; // spirito delle bufere 
	// karsus
	// animals
	float shuy; // bubbone gigante; peste bacteria
	float blastoderma; //mostro infettante della peste
	float sheng; // lattugoide. 
	float tai; // muschioidi
	float zaol; // alga algomiceti
	float hua; // floromiceti
	float cao; // era
	float zhongshi; // erba fida 
	float shu; //	necralberi
	float pofuda; // saccotelminti
	float yongba; // nototelminti
	float melyur; // anoculopiscidi - pesci senza occhi 
	float qinwa; // radaranuri
	float floatiyi; // lucertola drago sotterranea.
	float juzhi; // talpoide 
	float blanf; // chiroide pippistrelli radar
	float polpolk; // yuhak
	float haizhe; // mostro medusafluttante delle cavenre 
	float ruko; // 	necroloom
	float florolk;  // gfloatun florolk
	// artridi
	float minia; // trivelleoide; trismulloidi
	float zanbu; // trilobiti
	float xiezi; // eureride; - aragoste granchi scorpioni d'acqua
	float xie; // granchio 
	float kunchon; // insettoidi- bigbug
	float ytok; //  simili a coccinelle 
	float formix; // formifloat formiche giganti 
	float cicax; // cikafloat  grilli cicala
	float manx; // manfloat tipo mantidi grillo 
	float splax; // splafloat scarabei
	float lymel; // lymel: libellule e api 
	float zarz; // zarz; zanzare
	float scrab; // scarabei 
	// peoples
	float antwer; // razza insettoide
	float arcadian; // arcadico
	float zoff; // zoffese
	float formigian; // formigian 
	// magical elemental monster creatures
	float buleo; // tardigrado del vuoto creatura malefica. 
	float zenbuk; // verme abissale del pianeta immune a metallo; calore e lava.
	// divinità arcadica
	float huzin; // verme del deserto adorato dai zoffesi 
	float tamenza; // coleottero dei ghiacci; divinità formigiana
	// legendary japan creature mithology
	// godness comuni a tutti i popoli  
	float an;  // la volta; il grembo; il "cielo" ultraterreno; lo spazio.
	float ki; // terra; considerato come tutta la speleosfera. 
	float enil; // aria 
	float enki; // acqua 
	float anunna; // assemblea divina; gruppo di dei.
	// divinità zoffesi 
	float anki; // asteroide; personificato nelle 3 lune del pianeta. 
	float aruru; //il principio creatore.; 
	float ashnan; // la dea custode dell'erba fida. 
	float ninz; // bau / ninisinna /gula (dea della medicina) 
	float bel; // belili (dea della luce; consorte del dio solare bel) 
	float meku; // dimpemekug (dio scriba degli inferi) 
	float zibzu;  // dumu-zi-abzu (dio della vegetazione ) 
	float gilgam; // eroe divino antwer enkidu gilgamesh (mitico re di uruk; protagonista dell'epopea di gilgamesh) 
	// haia (sposo di nidaba) 
	float eresh; // la dea del buio sotterraneo ched tutto avvolte. 
	float gesthin; // la dea dello stige che da la vita col suo lago. geshtinanna 
	float kur; // malvagio dio del sottomonto
	float inanh; //inanna; dea madre. 
	float ish; // ishkur - governa il clima nella speleosfera. 
	float izthar; // adorato dagli arcadici; dirime le divergenze e porta la pac.e 
	float lahar; // custode delle bestie e del bestiame.
	float nammu; // il mare primordiale; l'essenza stessa dello stige
	float nanna; // (dio della luna) 
	float nida; // nidaba (dea della saggezza; della scrittura e della letteratura) 
	float ninazu; // (dea degli inferi) 
	float ninlil; // (dea dell'aria e del grano) 
	float ningal; // (madre di utu e dea della luna) 
	float utu; // dio della luce. 
	float ziuz; // ziusudra; il primo antwer; dai cui 3 figli derivaron i popoli antwer attualim che si salvò dal "diluvio"
	// causato dall'allagamento di tutta la speleosfera ad opera dello stige. 
	float humbaba; // guardiano del giaridno dell'aldilà;
	float izhum; // ishum; dio del fuoco
	float keret; // secondo eroe ; signore dei ragni 
	float lamazu; // terzo eroe; signore delle mantidi
	float nizrok; // dio della pietra
	float nusku; // signore delle lucciole fuoco e luce; o meglio delle creature che brillano nel buio. 
	// spiriti arcadici - divinità indu
	float zhramak; // bhrama 
	float veda;  // saggi ancestrali depositari del sapere ancestrale cosmico
	float vushna; // visnu? 
	float kriznak; // krisha
	// spoiriti formigiani 
	float izan; // divinità gemelle formigiane creatrici dei primi antwer. 
	float yaza; // gli 8 antwer ancestrali; da cui ebbero origine tutte le tipologie
	// della loro specie e tutte le creature insettoidi del loro mondo. 
	//( awazi iyo (shikoku) ogi tzuku iki tzuzi (tsushima)
	// zad; yamato (successivamente honshu)
	// demoni antwer che vivon nelle oscure terre dell'antartico
	float yomi; // i signori della morte 
	float kami; // gli oggetti animati. 
	float kappa; // mostri scimmia l'altra razzaz del pianeta; qua poco più che belve ferali
	// accerrimi nemici di tutti gli antwer; imprigionati nel gelido sud
	float kitzun; // l'astuta e infide volpe del sud; che comanda un'armata di ratti che spargon
	// ovunque pestilenza e dolore. 
	float nue; // mostro mutaforme con testa da scimmia; corpo da cane procione; zampe di tigre; la coda di serpe 
	// è un ladro infido.
	float ono; // l'orco degli antwer
	float tanuko; // cane procione (può fondersi alle stalattiiti
	//( e alle stalagmiti della speleosfera per nascondersi e passar inosservato.
	// è affine allo stregatto) 
	float wayang; // son i tengu ;i goblin degli antwer
	float tzukumo; // tsukumogami. barattolo da tè animato? boss kami
	float yama; // yamauba; strega
	float yokai; // spettro buono 
	float yataga; // spettro corvo 
	float allip; // omuncoli; pupazzi.
	float yureo; // fantasma reaper; malvagio 
	// tipi di spettri: 
	float jiba; // jiba- jibakurei: spettro di suicida o rimpianto. infesta un luogo.
	float hyor; // hyoirei: uno spettro che si impossessa del corpo di un vivente. 
	float onro; // onryo: spirito vendicativo. torna a perseguitare chi li ha maltrattati 
	float ubu; // ubume : spiriti di madri morte nel dare alla luce un figlio.-
	float goryo; // spirito di aristrocratico morto per intrighi di palazzo o tradimento.
	float funay; // funayurei spiriti di marinai morti nello stige; 
	float zazi; // fantasmi di bambini; dispettosi; poltergeist
	float gaki; // persone morte per i propri vizi. son vampiri ripugnanti perennemente affamati e assetati.
	float jikini; // necrofagi  
	float iki; // ikiryo; // fantasmi di vivi o di persone morenti! l'anima si separa dal corpo di chi si vuol vendicar; per andar a perseguitar il nemico.
	float genpei; // fantasma di samurai; morto in battaglia.
	float banshee; // fantasma seduttore. 
	// erebo 
	// creatures
	float patofitus; 
	float plasmer; 
	float amoeba; 
	float patal; // alghe nere 
	float patom; // patomuschiodi
	float dionea; //dionee 
	float witrees; 
	float wictus; 
	float pumpkin;
	float florotox;  
	float pyrotox; 
	float warm; 
	float tremor; 
	float starler; 
	float snailer; 
	float coppershark; 
 	float moray; //murena
	float manta;  
	float piranha; 
	float seashark; 
	float wayshark; 
	float lisk; //lisca 
	float iguanoid; 
	float saurin; 
	float reptile;
	float triceratop; 
	float stego;
	float rex; 
	float blackdragon;
	float tardragon; 
	float vampat; 
	float hedger; 
	float vermin; // topo 
	float aracnoide; 
	float naker; // millepiede  
	float narpius; // scorpione
	float bumblebee; // vespe giganti
	float scarafus; // scarabeo gigante. 
	float skelide; 
	float skfish; // lisca di pesce 
	float skdiplo; // diplo schelidi
	float skfrog; // rana sceheltro 
	float sklizard; // lucertola d'ossa. 
	float skbird; // skeelder volante 
	float skcow; // skeelder rancor
	float skwolf; // skeleder wampa
	float bonker; // skeelder monkey 
	// skeelder race and etnical groups
	float slobber; // mutanti skeelder; uomini di fango 
	float necrotail; // uomini scorpione tatuati. sole; zefloatas. 
	float barabek; // uomini blatta; klahom
	float morpher; // mutaforma 
	float witcher; // horroria female witch
	float sorcerer; // horroria male wizard 
	float momor; // stregoni mostruosi; signori della magia opposti
	// ai servi di xadroom
	// evil sorcerer. stregoni cinesi vestiti con tuniche alla confucio  nettuno 
	float reticulan; // grigi; grigi tipo kamonoani urano  klahom
	// portan incubi e controllano i manicomi dell'impero.
	float devil; // diavoli! i più terribli avversari degli skeelder 
	// demons of orghaus; categories. 
	float aigipan; // caprini: corna caprine sulla fronte e zampe di capra) 
	float arren;  // privi di narici o privi di nas
	float antipod; // i piedi rivolti all'indietro) 
	float monokle; // (con un solo occhio sulla fronte) 
	float artabafloat; // (quadrupedi) 
	float astom; // (privi di bocca) 
	float blem; // (privi di testa e con la faccia sul ventre) 
	float brachistom; //  labbra e occhi cuciti! 
	float dcyclop; // giganti e con un solo occhio.
	float cynocefalus; // testa di cane
	float fomor; // testa di capra. 
	float gegen; // corna 
	float gorgad; // corpo pelloso. 
	float imante; //  gambe fini e piatte come strisce di cuoio
	float ipporus; // zoccoli da cavallo 
	float nisicaste; // 3-4 occhi
	float panozi; // orecchie enormi.turelim style
	float raab; // rahabim style.;
	float pigmer; // demoni nani
	float sciapod; // con una sola gamba e un grande piede. 
	// skeelder etnical groups 
	float skeelder; // defauult
	float raska; // artici; simili agli unnni con grosse teste a epra
	float anglorgian; // ossati;emaciati; lineamenti scozzesi; inglesi e transilvani; saturno. 
	// freddi; gentili; astuti e ingannatori 
	float swamper; // delle paludi; con capelli cadenti e setosi e aspetto da gollum
	float horrorian;  // ossati con lunghi capelli alla eddie; cagneschi venere. rozzi; ignoranti.
	// zotici; insensibli e crudeli. dei montanare e dei contadini insomma. 
	float zexan;//scorpioneschi e tatuati western i più feroci; aggressivi e sanguinari. 
	float klahom;
	float indi; // mohicani e tribali assassini luna. seguaci del pentavirato. 
	// fanno anche sacrifici umani. tatuati e conciati come pellerosse cannibali.
	float kuzeno; // evil genius. grandi teste da cranion poteri psichici limitati 
	float bloodgauzer; // skeelder bloodsucker mutant
	float vod; // 
	float nubilan; // spettri ovlon
	float ruddy; // anime bianche; grige; nere 
	// skeelder godness
	float alvid; 
	// for horroria 
	float hagraven; // arpia
	float basilisk; // basilisco
	float haunt; // erebo cerbero
	float caradrius; // caradrio
	float yale; // 
	float fauno; 
	float lycan; 
	float crocote; // uomo coccodrillo 
	float naga; // uomo serpe 
	float leviatan; 
	float darksfinfloat; 
	float darkvyvern;
	float unicornus; 
	float syren;
	float chupacapra;
	float xamax; // erebus amazzone 
	float chimaera;
	// for zefloatas 
	float annuna; // dei sanguinari primordiali 
	// quetzal; zaund; 	cicactli; cicavateo; 
	// nahual; 	floatolotl; ; // acchiappasogni  
	// anglorgia
	float dacius; 
	float babau;// acchiappasogni 
	// klahom; 
	float frankie; 
	float hitchhiker; // autostoppista fantasma
	float ghostdriver; // pilota di auto fantasma
	float jason; // serial killer 
	float dyman; // alligatore delle fogne
	float choppervyper; 
	float spooner;
	float morgellon;
	// nubila
	float jikin; 
	float hella; 
	float nightmare; 
	float succubus; 
	float puck;
	float jinn;
	float eris; 
	// a questa classe appartengon i seguenti mostri: 
	float moros; 
	float ker;
	float tanath;
	float hypnos; 
	float onerion;
	float momo;
	float oizys; 
	float nemesi;
	float apate; 
	float philote; 
	float geras; 
	float keres;
	float letum;
	float lysymele;
	float epifron;
	float styx; 
	float ate;
	float makhai;
	float ponono;
	float limo;
	float algea;
	float isminar;
	float fonor;
	float androktas;
	float neikea;
	float pseudos; 
	float amfiolo;
	float horkos; 
	// momon island godness
	float primeval; // entita' ancestrali skeelder pre impero 
	float aita;
	float aipan;
	float ani;
	float alpan;
	float aplu; 
	float artume;
	float atunis;
	float charun;
	float culsu;  
	float februus; 
	float laran; 
	float lasa;
	float losna; 
	float mania; 
	float mantus;
	float maris; 
	float northia; 
	float satres; // time streamer
	float selvan;	
	float semia; 
	float sethlan; 
	float suri; 
	float tagehe;
	float taitle; 
	float usil; // black sun
	float vanth; 
	float veive; 
	float vicar; 
	float veltrun; 
	// STARSHIPS AND SPACE CREATURES! 
	// BOATS, // for sea battle fantasy
	float quadrireme; // scialuppa vesu 
	float frisian; // // nave da guerra dei Kos
	float galea;  // nave da guerra media dei vesu
	float galeon; // nave da guerra vesu
	float submarine; // sommergibile in generile / yerkken dei gorkan
	float barge; // chiatta o grossa nave da trasporto; generica.
	float speedboat; // motoscafo; nave veloce a motore 
	float scuba; // batiscafo generico
	float steamboat; // battello a vapore. 
	float vessel; // veliero generico 
	float idrocar; // idroscafo dei cranion 
	float drakar; // nave vichinga nark 
	float playa; // centopulos sottomarino squalo 
	// ragan e falkor: vedi skimmer
	float idrolaz; // navi falkor 
	float idrobob; // for sferfloat
	float junk; // giunca \ nave formigiana 
	float tinker; // petroliera o nave della flotta skeelder 
	// volanti
	float alian; // aliante vesu
	float mintada; // mongolfiera khrolen \ generica 
	float zemba; // aerostato dei monker 
	float hovercar; // vettura dei cranion 
	float joen; // galeoni volanti dei nark!
	float airlaz; // aerei dei falkor 
	float aerobob; // aerei sferfloat 
	float trichopter; // elicottero skeelder - spesso d'assalto
	// class_zeppelin; // dirigibili \ mongolfiere etc. 
	// class_octopus; // as probe droid
	// space battleships
	// class_pod // generic container;
	float sphere; // contiene esplosivi 
	float fueltank; // contiene carburante 
	float silo; // contiene cibo
	float penta; // contiene sostanze organiche.  
	float efloata; // contiene tecnologia. 
	float moric; // contiene sostanze arcane. è come i magazzini vetonga. vedi monker 
	float clessidra; // contiene fluidi 
	float gastank; // contiene gas
	float bell; // contiene rifiuti.;
	float ark; // contiene creature viventi. 
	float diamond; // energetici 
	float frosty; // contiene refrigeeranti 
	float bofloat; // contiene minerali. 
	float rombus; // contiene armi 
	float torus; // costrittore magnetico usato per scagliare e spostar i cargo.
	float canister; // ricevitore magnetico che riceve i cargo. 
	// cargo 
	float tug; // rimorchiatore e riparatore.
	float miner; // rimorchiatore minerario.
	float fregata; // nave da trasporto modulare 
	float container; // nave porta container sopracitati
	float junker; // addetto alla nettezza urbana costmica.
	float swingminer; // piazza mine o le rimuove usato dai militari. 
	// torrette difese e satelliti d'attacco. 
	float pyro; // scaglia obici incandescenti che fondon lo scafo delle navi.
	// usa attacchi incendiari  usa elementi termporei 
	float sunsprider; // lancia dardi di plasma e radianza.usa elementi energei
	float ionic; // lancia impulsi elettromagnetici. usa energei 
	float srhapner; // canonne che lancia granole a frammentazione. usa metalli arcani
	float shipmolder; // lancia tossine velenose che sciolgono gli scafi. 
	// infligge danno da alchemico \ veleno 
	float nymphayer; // disgregatore. è equipaggiato con gli elementi storm.  
	float arborean; // una difesa organica usata dai vegeter come controarea
	// sparge a raggiera semi e gusci spinosi che scaglian attacchi bioorganici
	// di tipo mishmolder o disgregatori. 
	float nuonner; // terrificante cannone che scaglia armi nuoniche. 
	float tblaster; // installazione montata sugli asteroidi cannoni laser. 
	float tseeker;// torrette specializzate nel lanciar siluri
	float zilzal; // cannoni che lanciano bombe e proiettili sismici. 
	float scrumbler; // atk psichici rivolti ai piloti; non ai caccia in se. 
	float sadat; // questa categoria lancia onde soniche e vibrazioni che mandan 
	// in frantumi gli scafi con le loro "urla" 
	float tcryer; // difese che lanciano attacchi congelanti o proietitli fluidi. 
	float moai; // gradius head stones? 
	float nefiler; // scaglian attacchi di elementi atmòs si trovan nelle installazioni
	// delle nebulose. 
	float darkner; // usano elementi oscuri e negromantici. 
	float mooner; // usano armi mistiche lunari e\ o artificali d'aavanguarida 
	float interdictor; // usani armi subspaziali. 
	// truppe di terra 
	float grabber; // skeelder e truppe di invasione 
	float empire; 
	float alliance; // alleati 
	// tipi di hovercraft
	float quagmire; // è in grado di spostarsi sopra laghi di acido; melma; sabbie mobili; paludi
	float sourer; // si sposta sopra acido o fluidi corrosivi.
	float magmer; // è in grado di spostarsi sopra la lava
	float sandcrawler; // tipico dei krholen
	float tarpitter; // veleggia sopra i laghi di catra
	float uro; // veleggia sopra i laghi di catrame di alvid ha forma a cupola
	float zidkler; // usato dai falkor per spostarsi sopra le distese di cristallo
	float sauger; // a forma di ciambella; usata dai falkor per spostarsi sopra il metallo liquido e il sidrosforo. 
	float skimmer; // usato dai ragan sabbia
	float climber; // per mondi rocciosi. 
	float lyan; // vola sopra gli alberi  
	float tractor; // agricolo va su erba e prati   
	float floater; // antigravitaizonale 
	// carri armati e camminatori 
	float kafuker; // yulo \ carro dei vesu. posson esser armati e d'assalto; pieni di kaful. 
	// corron su binari sopra la lava. 
	float tank; // tank laser 
	float babbada; // carro armato dei khrolen. spara obici esplosivi is already as creature for all tanks creature i guess
	// sometime you fight a tank also as humanoid... 
	float vcab; // carri steampunk dei monker 
	float manfloat; // il più diffuso; a forma di insetto; di vario tipo.
	float sapher;// camminatore khrolen simil pterodattilo meccanico
	float robotolk; // camminatore d'assalto khrolen e mech multiarma in generale
	float mech; // mech multiarma: corron sui cingoli e hanno braccia cannoneggianti. 
	float ducker; // camminatore su pattini dei ragan
	float jumper; // robot saltellante tipico degli sferfloat  e dei mondi lunari; difficili da colpire. 
	float tortoide; // corazzato e simile ad una enorme tartaruga;camminatore d'assalto ragan
	// classe usata anche per i falkor 
	// basi
	float dagoon; // base a cupola da cui decollano caccia standard
	float gamuun; // base da cui decollano i caccia flyer skeelder d'elite.
	float flower; // base da cui decollano caccia vegetali \ vegeter etc.
	float kamooon; // base insettoide da cui decollano i caccia antwer
	float buller; // bolla gigante da cui attaccano creature ameboidi.
	float tower; // torre di lancio di caccia; tipica della marina spaziale.
	// comune nei mondi con liquidi mefitici di vario tipo. 
	// camminatori 
	// class_walker; // big armored walker as atst
	float baocha; // camminatore monker
	float elembante; // camminatore d'assalto monker 
	// class_sat; // telecomunication sat. 
	float sat; // default sat
	float tilter; // this asteroid freeze the ships near
	float charmer; // this asteroid invert cmd directions of player if is close! 
	float wavetronik; // genera attacchi aoe di vario tipo a chi si avvicina tropppo
	float efloatplorer; // sonda esplorativa. 
	float spiotronik; // satellite spia
	float biotronik; // satellite di ricerca
	float ultrasat; // comunicazioni iperspaziali; tachiofari. 
	float ipnosat; // satellici ipnotici
	// class_shuttle; // navetta e sonde
	float shallop; // scialuppa militare; fa la spola fra navi e stazioni  
	float pioneer; // navetta di esplorazione
	float searcher; // navetta di scienziati
	float albatros; // navetta passeggeri 
	float yacht; // nave di lusso per viaggi da sogno e crociere nello spazio
	float epod; // guscio di salvataggio 
	// patrolship
	// ostili che trovano. 
	float ird; // pattuglia usata dai khrolen
	float ragal; //  pattuglia usata dai falkor 
	float zakkar; // ispirati ai garrul di gradius . 
	// ricognitore vesu; vortica su se stesso e può speronare le vittime
	float venus;// ricognitore monker. non attacca; esamina e torna indietro
	float mazaar; // ricognitore bombardiere dei centopulos 
	float fose; // ricognitore dei ragan simile ad un trilobite 
	float uska; // usato tipicamene dalla piuma rossa.
	float urgol; // ricognitore \ cannone mobile tipico degli sferfloat; dalla forma a cupola
	float zeb; // mina spaziale spia invibile usata dagli antwer.
	float rush; // satellite usatao dai nashar 
	// caccia
	// fighter typs of my races. 
	float hoverstar; // cranion fighters 
	float fyron;// vesu fighters 
	float knuby; // khrolen fighters 
	float spider; // vegeter robo fighters
	float yver; // monker fighter bomber 
	float ashak; // aper bomber 
	float ancle; // centopulos fighter 
	float trideb; // ragan interceptor 
	float eagle; // falkor fighrers
	float bowler; // sferfloat spy ships
	float stinger; // antwer fighters
	float fly; // skeelder training fighters
	float razor; // raider and smuggler skeelder fighters
	float scrabble; // skeelder escort fighter
	float bomber; // skeelder bombers 
	float vyper; // skeelder military assault fighter 
	float skull; // elite skeelder fighter
	// usati dagli smuggler
	// navi da guerra 
	float avenger; // vesu - simile ad un falco da guerra klingon 
	float sunspray; // khrolen - ispirato alla firespray di star wars 
	float sinflower;// vegeter 
	float ambras; // monker  ispirato ai pynook è un quetzak con occhi in ormio 
	float bomberboat; // centopulos 
	float blastboat; // ragan 
	float preybird;  //  falkor
	float starchaser;// sferfloat
	float swarm; // antwer 
	float armory; // nave di rifonirmneto armi
	float fueltanker; // nave di rifornimento carburante; energia; riparaizone. 
	// skeelder
	// navi d'assalto e vettori  
	float destroyer; // generic distruttore o assault ship
	float kafship; // cannoniera vesu con kafuk spaziali?
	float striker; // khrolen 
	float bulka; // monker 
	float diendor;// nave paralizzante dei cranion  
	float marauder; // ragan 
	float karraka; // dei falkor 
	float neutron; // sferfloat 
	float lancer; // degli antwer 
	float shrouder; // degli skeelder  
	// vettori 
	float vector; // portacaccia skeelder 
	float carrier; // portacaccia dell'unione
	float alvarium; // nave alveare degli antwer  
	float floaterem; // vegeter porta penthas \ spider. 
	float escorter; // nave da scorta e di supporto 
	// incrociatori 
	float moren; // incrociatore che lancia obici di fiamme nere. avenger; // vesu 
	float interceptor; // khrolen
	float gunship; // monker cannoniera 
	float spreader; // vegeter 
	float kosmer; // cranion
	float defender; // centopulos lenta e possente; da difesa-
	float escaper; // ragan cloakshape; // ragan dotata di cloacking; versatile e crea diversivi
	float nebulon; // corvetta dei falkor 
	float ssss;// sferfloat system stealth starship come una sfera borfloat giroscopica gigante
	float lionant; // formicaleone; nave d'assalto degli antwer
	float nightcrawler; // skeelder 
	// stazioni  
	float shiptrader; // commerciale 
	float shipbuilder; // cantiere spaziale
	float array; // stazione di rivelamento. tachiofaro  
	float commrel; // stazione di comunicazione \ tachiofaro 
	float dioneus; // vegeter station organica 
	float shadir; // stazione monker \ aper 
	float minzar; // stazione militare hq cranion
	float enclave; // stazione khrolen; avamposto alleato generico 
	float talon; // piattaforma di difesa - portacaccia
	float lair; // stazioine della piuma rossa 
	float kolumbus; // stazione turistica. 
	float mining; // installazione mineraria asteroidale. 
	float biosphere; // biosfera
	float ovlonor; // stazione fantasma 
	// mine
	float mine; // profloatimity mine 
	float minea; // chaser
	float mineb; // shooter laser \ ionic ionic
	float minec; // laser \ missile switch weapon 
	//class_stargate; // portale
	float stargate; 
	float puzzlegate; // si attiva colpendo le luci nel modo corretto.
	float wormhole; // distorsione spaziotemporale - buco nero 
	float quasar; // quasar! 
	//class_nav; // navigation boa. 
	float oloboa; // olographic boa 
	float astroboa; // boa fisica
	float hyperboa; // boa iperspaziale 
	//class_asteroid; 
	float dustoid; // space dust clouid
	float rockoid; // asteroid of rock
	float goldoid; // asteroid of light 
	float ironoid; // metal black asteroid
	float deltar; // asteroid efloatplosive and radiactive
	float iceoid; // ice asteroid 
	//class_astrolaser; // space cannon defences. 
	//class_biostar; // creature of space. 
	float phoenifloat; // fenici; vivon su mondi lavici.
	float globus; // fatti di plasma; energia e energia e radianza; vivon nelle stelle 
	float yersa; // terribili virus spaziali 
	float starflower; // piante spaziali. hanno come petali vele e pannelli solari
	float starfly; // libellule e insetti spaziali. biologia silicea
	float nairnama; // astronavi viventi 
	float ziffer; // dimensionali; vivon nell'iperspazio; distorcon spazio e tempo.
	float dogon; // creature siderali capaci di manipolare gli elemeneti arcani.
	// son esseri mistici ed elementali 
	float ameboide; // gigantesco unicellulare tipico degli oceani della luna gamma idrese e dei mondi fluidi. 
	float antennoide; // chaos e atomi giganti; questi esseri provengon dalle pulsar son atomi giganti e usano 
	// attacchi di materia storm 
	float cometa; // planetoide gelido senziente che fa attacchi congelanti. si trovano nella periferia del sistema
	float lithos; // asteroidi senzienti che vivon attorno al pianeta karsus
	float hooler; // progenie del vuoto e dell'entropia; vivon nei buchi neri e usano attacchi nuonici
	float orlana; */
	}
	else {
		// defaults
		mod.modifiers.armor = 1.0f;
		mod.modifiers.droid = 1.0f;
		mod.modifiers.organic = 1.0f;
		mod.modifiers.shield = 1.0f;
		mod.modifiers.beast = 1.0f;
		mod.modifiers.lightside = 1.0f;
		mod.modifiers.darkside = 1.0f;
		mod.modifiers.hazardtrooper = 1.0f;
		mod.modifiers.sandcreature = 1.0f;
		mod.modifiers.assassindroid = 1.0f;
		// legacy of Kain modifiers. 
		mod.modifiers.ghost = 1.0f; 
		mod.modifiers.archon = 1.0f;
		mod.modifiers.tentacle = 1.0f;
		mod.modifiers.eldergod = 1.0f;
		mod.modifiers.vampireghost = 1.0f;
		mod.modifiers.druid = 1.0f; 
		mod.modifiers.werewolf = 1.0f; 
		mod.modifiers.prophet = 1.0f;
		mod.modifiers.sarafan = 1.0f;
		mod.modifiers.wizard = 1.0f;
		mod.modifiers.cenobita = 1.0f;
		mod.modifiers.melchiahim = 1.0f;
		mod.modifiers.summoner = 1.0f;
		mod.modifiers.melkboss = 1.0f;
		mod.modifiers.zephonim = 1.0f;
		mod.modifiers.deceiver = 1.0f;
		mod.modifiers.zephboss = 1.0f;
		mod.modifiers.dumahim =  1.0f;
		mod.modifiers.dumahboss = 1.0f;
		mod.modifiers.rahabim = 1.0f;
		mod.modifiers.rahabboss = 1.0f;
		mod.modifiers.turelim = 1.0f;
		mod.modifiers.tyrant = 1.0f; 
		mod.modifiers.turelboss = 1.0f;
		mod.modifiers.vampire = 1.0f;
		mod.modifiers.vampire_demon = 1.0f;
		mod.modifiers.vampireboss = 1.0f;
		mod.modifiers.pureghost = 1.0f;
		mod.modifiers.darkvampire =  1.0f;
		mod.modifiers.maskvampire =  1.0f;
		mod.modifiers.necrovampire = 1.0f;
		mod.modifiers.hellvampire =  1.0f;
		mod.modifiers.airvampire = 1.0f; 
		mod.modifiers.bardvampire = 1.0f; 
		mod.modifiers.wolfvampire = 1.0f; 
		mod.modifiers.elfvampire = 1.0f; 
		mod.modifiers.halfvampire = 1.0f; // half vampire  
		mod.modifiers.frostvampire = 1.0f; 
		mod.modifiers.archervampire = 1.0f; 
		mod.modifiers.razielhim = 1.0f; 
		mod.modifiers.razielboss = 1.0f; 
		mod.modifiers.halfdemon = 1.0f; 
		mod.modifiers.halfmelc = 1.0f; 
		mod.modifiers.sarvampire = 1.0f; 
		mod.modifiers.shockvampire = 1.0f; 
		mod.modifiers.spidervampire = 1.0f; 
		mod.modifiers.poisonvampire = 1.0f; 
		mod.modifiers.wolfform = 1.0f; 
		mod.modifiers.lionform = 1.0f; 
		mod.modifiers.hellwolf = 1.0f; 
		mod.modifiers.demonwolf = 1.0f;
		mod.modifiers.batform = 1.0f; 
		mod.modifiers.hellbat = 1.0f; 
		mod.modifiers.demonbat = 1.0f; 
		mod.modifiers.mistform = 1.0f; 
		mod.modifiers.fearmist = 1.0f; 
		mod.modifiers.darkmist = 1.0f; 
		mod.modifiers.vapormist = 1.0f; 
		mod.modifiers.bloodmist = 1.0f; 
		mod.modifiers.hellmist = 1.0f; 
		mod.modifiers.elfmist = 1.0f; 
		mod.modifiers.sarmist = 1.0f; 
		mod.modifiers.necromist = 1.0f; 
		mod.modifiers.reaver = 1.0f;  
		mod.modifiers.reaverboss = 1.0f; 
		mod.modifiers.necroreaver = 1.0f;  
		mod.modifiers.darkreaver = 1.0f; 
		mod.modifiers.tankreaver = 1.0f; 
		mod.modifiers.lightreaver = 1.0f; 
		mod.modifiers.arcanereaver = 1.0f; 
		mod.modifiers.waterreaver = 1.0f;
		mod.modifiers.icereaver = 1.0f; 
		mod.modifiers.spiderreaver = 1.0f;  
		mod.modifiers.firereaver = 1.0f; 
		mod.modifiers.airreaver = 1.0f; 
		mod.modifiers.mentalist = 1.0f; 
		mod.modifiers.stonereaver = 1.0f; 
		mod.modifiers.venomreaver = 1.0f; 
		mod.modifiers.psychic = 1.0f; 
		mod.modifiers.planeshifter = 1.0f; 
		mod.modifiers.wraitharmor = 1.0f; 
		mod.modifiers.naturist = 1.0f; 
		mod.modifiers.energist = 1.0f; 
		mod.modifiers.timestreamer = 1.0f; 
		mod.modifiers.alchemist = 1.0f; 
		mod.modifiers.hylden = 1.0f;
		mod.modifiers.hyldenlord = 1.0f; 
		mod.modifiers.demon = 1.0f; 
		mod.modifiers.demonfire = 1.0f; 
		mod.modifiers.demonpoison = 1.0f;
		mod.modifiers.demonblade = 1.0f;   
		mod.modifiers.demonelectro = 1.0f; 
		mod.modifiers.thrall = 1.0f; 
		mod.modifiers.poltergeist = 1.0f; 
		mod.modifiers.shadow = 1.0f; 
		mod.modifiers.voidness = 1.0f; 
		mod.modifiers.sun = 1.0f; 
		mod.modifiers.water = 1.0f;  
		mod.modifiers.crystal = 1.0f; 
		mod.modifiers.gale = 1.0f; 
		mod.modifiers.earth = 1.0f; 
		mod.modifiers.necro = 1.0f; 
		mod.modifiers.holy = 1.0f; 
		mod.modifiers.poison = 1.0f; 
		mod.modifiers.ivy = 1.0f; 
		mod.modifiers.sonic = 1.0f; 
		mod.modifiers.golemdark = 1.0f; 
		mod.modifiers.golemlight = 1.0f; 
		mod.modifiers.golemgold = 1.0f; 
		mod.modifiers.golemfire = 1.0f; 
		mod.modifiers.golemwater = 1.0f; 
		mod.modifiers.golemfrost = 1.0f; 
		mod.modifiers.golemair = 1.0f; 
		mod.modifiers.golemmetal = 1.0f; 
		mod.modifiers.golemsonic = 1.0f; 
		mod.modifiers.golemholy = 1.0f; 
		mod.modifiers.golem = 1.0f; 
		mod.modifiers.hellgolem = 1.0f; 
		mod.modifiers.necrogolem = 1.0f; 
		mod.modifiers.golemsmog = 1.0f; 
		mod.modifiers.golemvoid = 1.0f; 
		mod.modifiers.golempoison = 1.0f; 
		mod.modifiers.golemlava = 1.0f; 
		mod.modifiers.ethereal = 1.0f; 
		// Now astrofantasy creatures! 
		// FIRST WORLD: VORKAN, VULCANIC WORLD! 
		mod.modifiers.vorkan = 1.0f; 
		mod.modifiers.blob = 1.0f; 
		mod.modifiers.blobster = 1.0f; 
		mod.modifiers.vidra = 1.0f; 
		mod.modifiers.carminia = 1.0f; 
		mod.modifiers.volvox = 1.0f; 
		mod.modifiers.pirosca = 1.0f; 
		mod.modifiers.ameboid = 1.0f; 
		mod.modifiers.centix = 1.0f; 
		mod.modifiers.pkela = 1.0f; 
		mod.modifiers.konja = 1.0f; 
		mod.modifiers.rak = 1.0f; 
		mod.modifiers.pawk = 1.0f; 
		mod.modifiers.limulus = 1.0f; 
		mod.modifiers.kerlak = 1.0f; 
		mod.modifiers.scorpion = 1.0f; 
		mod.modifiers.hypopod = 1.0f; 
		mod.modifiers.hypomech = 1.0f; 
		mod.modifiers.snag = 1.0f; 
		mod.modifiers.pirocefalus = 1.0f; 
		mod.modifiers.zmija = 1.0f; 
		mod.modifiers.vron = 1.0f; 
		mod.modifiers.spongia = 1.0f; 
		mod.modifiers.torch = 1.0f; 
		mod.modifiers.forger = 1.0f; 
		mod.modifiers.branote = 1.0f; 
		mod.modifiers.shogoth = 1.0f; 
		mod.modifiers.jackal = 1.0f; 
		mod.modifiers.myriad = 1.0f; 
		mod.modifiers.vyvern = 1.0f; 
		mod.modifiers.kikima = 1.0f; 
		mod.modifiers.skorpius = 1.0f; 
		mod.modifiers.pirkar = 1.0f; 
		mod.modifiers.volvoma = 1.0f; 
		mod.modifiers.vesu = 1.0f; 
		mod.modifiers.anorian = 1.0f; 
		mod.modifiers.kos = 1.0f; 
		mod.modifiers.zhorian = 1.0f; 
		mod.modifiers.kromag = 1.0f; 
		mod.modifiers.krodan = 1.0f; 
		mod.modifiers.gorkan = 1.0f; 
		mod.modifiers.sed = 1.0f; 
		mod.modifiers.divinator = 1.0f; 
		mod.modifiers.cannork = 1.0f; 
		mod.modifiers.zarha = 1.0f; 
		mod.modifiers.farha = 1.0f; 
		mod.modifiers.konga = 1.0f; 
		mod.modifiers.sorha = 1.0f; 
		mod.modifiers.trikka = 1.0f; 
		mod.modifiers.kavel = 1.0f;  
		mod.modifiers.striga = 1.0f; 
		mod.modifiers.asher = 1.0f; 
		mod.modifiers.smoker = 1.0f; 
		mod.modifiers.tarter = 1.0f; 
		mod.modifiers.rusal = 1.0f; 
		mod.modifiers.friggan = 1.0f;
		mod.modifiers.clayer = 1.0f; 
		mod.modifiers.obsidier = 1.0f; 
		mod.modifiers.mudgolem = 1.0f; 
		mod.modifiers.irongolem = 1.0f; 
		mod.modifiers.puppet = 1.0f; 
		mod.modifiers.miniature = 1.0f; 
		mod.modifiers.salamech = 1.0f; 
		mod.modifiers.pourer = 1.0f; 
		mod.modifiers.tetramech = 1.0f; 
		mod.modifiers.spidermech = 1.0f; 
		mod.modifiers.crabmech = 1.0f; 
		mod.modifiers.skormech = 1.0f; 
		mod.modifiers.recon = 1.0f; 
		mod.modifiers.scramech = 1.0f; 
		mod.modifiers.octomech = 1.0f; 
		mod.modifiers.torturator = 1.0f; 
		mod.modifiers.yulo = 1.0f; 
		mod.modifiers.diver = 1.0f; 
		mod.modifiers.ignitek = 1.0f; 
		mod.modifiers.gladiator = 1.0f; 
		mod.modifiers.yerkken = 1.0f;
		mod.modifiers.scavenger = 1.0f; 
		mod.modifiers.haunter = 1.0f; 
		mod.modifiers.screamer = 1.0f; 
		mod.modifiers.burner = 1.0f; 
		mod.modifiers.flyer = 1.0f; 
		mod.modifiers.farg = 1.0f; 
		mod.modifiers.kongar = 1.0f; 
		mod.modifiers.kef = 1.0f;  
		mod.modifiers.washak = 1.0f; 
		mod.modifiers.sorr = 1.0f; 
		mod.modifiers.keeper = 1.0f; 
		mod.modifiers.ziva = 1.0f; 
		mod.modifiers.mosok = 1.0f;
		mod.modifiers.jura = 1.0f; 
		mod.modifiers.dazbor = 1.0f; 
		mod.modifiers.devana = 1.0f; 
		mod.modifiers.owena = 1.0f; 
		mod.modifiers.ivonna = 1.0f; 
		mod.modifiers.khor = 1.0f;
		mod.modifiers.zwan = 1.0f; 
		mod.modifiers.svarg = 1.0f;
		mod.modifiers.vesna = 1.0f; 
		mod.modifiers.stribo = 1.0f; 
		mod.modifiers.jarion = 1.0f; 
		mod.modifiers.wik = 1.0f; 
		mod.modifiers.magrok = 1.0f;
		mod.modifiers.dusah = 1.0f;
		mod.modifiers.arrukk = 1.0f; 
		mod.modifiers.likho = 1.0f; 
		mod.modifiers.veles = 1.0f;
		mod.modifiers.babaya = 1.0f; 
		mod.modifiers.chuva = 1.0f; 
		mod.modifiers.bogatyr = 1.0f; 
		mod.modifiers.bozna = 1.0f; 
		mod.modifiers.morena = 1.0f; 
		mod.modifiers.fefnor = 1.0f; 
		mod.modifiers.pekle = 1.0f; 
		mod.modifiers.radigost = 1.0f; 
		mod.modifiers.zhymar = 1.0f; 
		mod.modifiers.tryglav = 1.0f; 
		mod.modifiers.horz = 1.0f; 
		mod.modifiers.boruta = 1.0f; 
		mod.modifiers.borevit = 1.0f; 
		mod.modifiers.cockatrix = 1.0f; 
		mod.modifiers.bagiennik = 1.0f; 
		mod.modifiers.kardag = 1.0f; 
		mod.modifiers.cernobog = 1.0f; 
	}

	jsonNode = cJSON_GetObjectItem(json, "dismemberment");
	if (jsonNode) {
		child = cJSON_GetObjectItem(json, "canDismember");
		mod.dismemberment.canDismember = cJSON_ToBooleanOpt(child, qfalse);

		child = cJSON_GetObjectItem(json, "blowChunks");
		mod.dismemberment.blowChunks = cJSON_ToBooleanOpt(child, qfalse);
	}

	jsonNode = cJSON_GetObjectItem(json, "plums");
	if (jsonNode) {
		child = cJSON_GetObjectItem(jsonNode, "hide");
		mod.plums.noDamagePlums = cJSON_ToBooleanOpt(child, 0);

		child = cJSON_GetObjectItem(jsonNode, "damageColor");
		if (child) {
			if (ParseJSONColor(child, mod.plums.overrideDamagePlumColor)) {
				mod.plums.overrideDamagePlum = qtrue;
			}
		}

		child = cJSON_GetObjectItem(jsonNode, "lowDamageColor");
		if (child) {
			if (ParseJSONColor(child, mod.plums.overrideLowDamagePlumColor)) {
				mod.plums.overrideLowDamagePlum = qtrue;
			}
		}

		child = cJSON_GetObjectItem(jsonNode, "shieldDamageColor");
		if (child) {
			if (ParseJSONColor(child, mod.plums.overrideShieldDamagePlumColor)) {
				mod.plums.overrideShieldDamagePlum = qtrue;
			}
		}
	}
	allMeansOfDamage.push_back(mod);



}

/*
=========================
ParseAllMeansOfDamage

=========================
*/
static qboolean ParseAllMeansOfDamage(const char* fileName, char* buffer) 
{
	char error[MAX_MEANSOFDAMAGE_ERROR] = { 0 };
	cJSON* json = cJSON_ParsePooled(buffer, error, MAX_MEANSOFDAMAGE_ERROR);
	cJSON* it;

	if (!json) {
		gi.Printf("Couldn't parse %s - %s\n", fileName, error);
		return qfalse;
	}

	for (it = cJSON_GetFirstItem(json); it; it = cJSON_GetNextItem(it)) {
		const char* name = cJSON_GetItemKey(it);
		ParseSingleMeansOfDamage(name, it);
	}
	
	return qtrue;
}

/*
=========================
LoadMeansOfDamage

=========================
*/
#define MEANSOFDAMAGE_FILE	"ext_data/tables/meansOfDamage.json"
void LoadMeansOfDamage() 
{
	fileHandle_t f;
	int fileLen = gi.FS_FOpenFile(MEANSOFDAMAGE_FILE, &f, FS_READ);
	// trap->FS_Open(MEANSOFDAMAGE_FILE, &f, FS_READ);
	gi.Printf("READING...\n", MEANSOFDAMAGE_FILE);
	Com_Printf("---------READING ext_data/tables/meansofdamage.json!\n");
	

	char buffer[MAX_MEANSOFDAMAGE_BUFFER] = { 0 };
	//now load in the json definition of MOD! definitions
	if (!f || fileLen == -1 || f == -1) {
		gi.Printf("Could not open %s for reading\n", MEANSOFDAMAGE_FILE);
		return;
	}
	
	if (fileLen <= 0) {
		gi.FS_FCloseFile(f);
		gi.Printf("%s is blank\n", MEANSOFDAMAGE_FILE);
		return;
	}

	if (fileLen >= MAX_MEANSOFDAMAGE_BUFFER) {
		gi.FS_FCloseFile(f);
		gi.Printf("%s is too big [%i >= %i (%i/%i KB)]\n", MEANSOFDAMAGE_FILE, fileLen, MAX_MEANSOFDAMAGE_BUFFER, fileLen / 1024, MAX_MEANSOFDAMAGE_BUFFER/1024);
		return;
	}

	gi.FS_Read(buffer, fileLen, f);
	buffer[fileLen] = '\0';
	gi.FS_FCloseFile(f);

	if (!ParseAllMeansOfDamage(MEANSOFDAMAGE_FILE, buffer)) {
		gi.Printf("Couldn't parse %s\n", MEANSOFDAMAGE_FILE);
	}
}

/*
=========================
GetMeansOfDamage

=========================
*/

/*### Overload 1 ###*/

meansOfDamage_t* GetMeansOfDamage(const std::string& ref) {
	for (int i = 0; i < allMeansOfDamage.size(); i++)
	{
		auto it = allMeansOfDamage.at(i);
		if (!Q_stricmp(it.ref, ref.c_str()))
		{
			return &(allMeansOfDamage[i]);
		}
	}
	return nullptr;
}

/*### Overload 2 ###*/

meansOfDamage_t* GetMeansOfDamage(int num) {
	if (num < 0 || num >= allMeansOfDamage.size()) {
		return nullptr;
	}

	return &(allMeansOfDamage[num]);
}

/*
=========================
GetMeansOfDamageIndex

=========================
*/

int GetMeansOfDamageIndex(const std::string& ref) {
	for (int i = 0; i < allMeansOfDamage.size(); i++) {
		if (!Q_stricmp(allMeansOfDamage[i].ref, ref.c_str())) {
			return i;
		}
	}
	return MOD_UNKNOWN;
}



