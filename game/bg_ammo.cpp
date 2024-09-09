#include "../cjson/cJSON.h"
#include "bg_ammo.h"
#include "bg_public.h"
#include "g_local.h"
#include "../cgame/cg_local.h"
/*
#define AMMO_NAMEBUFFER	8192
#define AMMO_FILEBUFFER	16386
#define AMMO_ERRBUFFER	1024
*//*
NewAmmo_t ammoTable[MAX_AMMO_TYPES];
int numAmmoLoaded = 0;*/


//const vec3_t defaultvec3 = { 1.0, 1.0, 1.0 };
/*
============================
GetAmmo

Gets a specific ammo type by name
============================
*/
// Overload 1: Get by string reference
/*
NewAmmo_t *GetAmmo(const char *ammoName) 
{
	for (int i = 0; i < numAmmoLoaded; i++) 
	{
		if (!Q_stricmp(ammoName, ammoTable[i].name)) 
		{
			return &ammoTable[i];
		}
	}
	return nullptr;
}

// Overload 2: Get by integer
NewAmmo_t *GetAmmo(const int ammoIndex) 
{
	if (ammoIndex < 0 || ammoIndex >= MAX_AMMO_TYPES) {
		return nullptr;
	}
	return &ammoTable[ammoIndex];
}

/*
============================
GiveAmmo

Not technically a BG function, it's a utility to prevent overflow
============================
*/
/*
void GiveAmmo(gentity_t* ent, NewAmmo_t* ammo, qboolean max, int amount) 
{
	if (max) 
	{
		ent->client->ammoTable[ammo->ammoIndex] = ammo->ammoMax;
	}
	else 
	{
		ent->client->ammoTable[ammo->ammoIndex] += amount;
		if (ent->client->ammoTable[ammo->ammoIndex] > ammo->ammoMax) 
		{
			ent->client->ammoTable[ammo->ammoIndex] = ammo->ammoMax;
		}
	}
}
/*
==============================
GetRefillAmmoCost

Gets the cost of refilling ammo, given an array of ammo and a weapon
==============================
*/
/*
int GetRefillAmmoCost(unsigned short* ammo, weaponData_t* wp)
{
	int totalCost = 0;
	for (int i = 0; i < wp->numFiringModes; i++)
	{
		NewAmmo_t* ammoType = wp->firemodes[i].ammoDefault;
		if (ammoType == nullptr)
		{
			continue;	// bad linkage - shouldn't happen
		}

		int ammoCount = ammoType->ammoMax - ammo[ammoType->ammoIndex];
		totalCost += ammoCount * ammoType->pricePerUnit;
	}
	return totalCost;
}

/*
============================
GetAllAmmoSubstitutions

Gets all substitutions for a specifc ammo type
Also includes the original ammo type in the list
============================
*/
/*
void GetAllAmmoSubstitutions(int ammoIndex, std::vector<NewAmmo_t*>& outSubs) 
{
	if (ammoIndex < 0 || ammoIndex >= numAmmoLoaded) 
	{
		return;
	}

	outSubs.clear();

	NewAmmo_t* find = &ammoTable[ammoIndex];
	for (int i = 0; i < numAmmoLoaded; i++) 
	{
		NewAmmo_t* thisAmmo = &ammoTable[i];
		if (thisAmmo->pSub == find) 
		{
			outSubs.push_back(thisAmmo);
		}
	}
}

/*
============================
WeaponAcceptsAlternateAmmo

Returns true if the weapon accepts alternate ammo, false otherwise
============================
*/
/*
//qboolean WeaponAcceptsAlternateAmmo(int weapon, int variation) 
bool WeaponAcceptsAlternateAmmo(int weapon, int variation) 
{
	weaponData_t* wp = GetWeaponData(weapon, variation);

	switch (weapon) 
	{
		case WP_SABER:
		case WP_NONE:
		case WP_MELEE:
			return qfalse;
		default:
			break;
	}

	if (wp->firemodes[0].clipSize <= 0) 
	{
		return qfalse;
	}

	if (wp->firemodes[0].useQuantity) 
	{
		return qfalse;
	}

	return qtrue;
}

/*
============================
AmmoIsBasedOn

Returns true if the ammo in arg 1 is based on (can be substituted for) the ammo in arg 2
============================
*/
/*
qboolean AmmoIsBasedOn(int ammoTypeIndex, int basedOnIndex)
{
	
	std::vector<NewAmmo_t*> ammoTypes;
	GetAllAmmoSubstitutions(basedOnIndex, ammoTypes);

	for (auto it = ammoTypes.begin(); it != ammoTypes.end(); ++it)
	{
		if ((*it)->ammoIndex == ammoTypeIndex)
		{
			return qtrue;
		}
	}

	return qfalse;
}

/*
============================
ApplyAmmoOverride

Applies an ammo override (if present)
============================
*/
/*
void ApplyAmmoOverride(int& value, const complexAmmoOverride<int>& field) 
{
	if (!field.bIsPresent) 
	{
		return;
	}

	if (field.bSet) 
	{
		value = field.set;
	}
	if (field.bAdd) 
	{
		value += field.add;
	}
	if (field.multiply) 
	{
		value *= field.multiply;
	}
}

void ApplyAmmoOverride(double& value, const complexAmmoOverride<double>& field) 
{
	if (!field.bIsPresent) 
	{
		return;
	}

	if (field.bSet) 
	{
		value = field.set;
	}
	if (field.bAdd) 
	{
		value += field.add;
	}
	if (field.multiply) 
	{
		value *= field.multiply;
	}
}

void ApplySimpleAmmoOverride(int& value, const std::pair<qboolean, int>& field)
{
	if (!field.first)
	{
		return;
	}
	value = field.second;
}

void ApplySimpleAmmoOverride(double& value, const std::pair<qboolean, double>& field)
{
	if (!field.first)
	{
		return;
	}
	value = field.second;
}

/*
============================

============================
*/
/*
qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, int>& field)
{
	return field.first;
}

qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, double>& field)
{
	return field.first;
}

qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, std::string>& field)
{
	return field.first;
}

qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, std::vector<std::string>>& field)
{
	return field.first;
}

qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, vec3_t>& field)
{
	return field.first;
}

/*
============================
ParseAmmoOverride_Int

Parses a single ammo override
============================
*/
/*
static void ParseAmmoOverride_Int(cJSON* json, const char* name, complexAmmoOverride<int>& field) 
{
	cJSON* node = cJSON_GetObjectItem(json, name);
	if (!node) 
	{
		field.bIsPresent = qfalse;
	}
	else 
	{
		cJSON* child;

		field.bIsPresent = qtrue;
		field.bAdd = qfalse;
		field.bSet = qfalse;
		field.bMultiply = qfalse;

		child = cJSON_GetObjectItem(node, "set");
		if (child) 
		{
			field.bSet = qtrue;
			field.set = cJSON_ToInteger(child);
		}

		child = cJSON_GetObjectItem(node, "add");
		if (child) 
		{
			field.bAdd = qtrue;
			field.add = cJSON_ToInteger(child);
		}

		child = cJSON_GetObjectItem(node, "multiply");
		if (child) 
		{
			field.bMultiply = qtrue;
			field.multiply = cJSON_ToNumber(child);
		}
	}
}

/*
============================
ParseAmmoOverride_Float

Parses a single ammo override
============================
*/
/*
static void ParseAmmoOverride_Float(cJSON* json, const char* name, complexAmmoOverride<double>& field) 
{
	cJSON* node = cJSON_GetObjectItem(json, name);
	if (!node) 
	{
		field.bIsPresent = qfalse;
	}
	else 
	{
		cJSON* child;

		field.bIsPresent = qtrue;
		field.bAdd = qfalse;
		field.bSet = qfalse;
		field.bMultiply = qfalse;

		child = cJSON_GetObjectItem(node, "set");
		if (child) {
			field.bSet = qtrue;
			field.set = cJSON_ToNumber(child);
		}

		child = cJSON_GetObjectItem(node, "add");
		if (child) {
			field.bAdd = qtrue;
			field.add = cJSON_ToNumber(child);
		}

		child = cJSON_GetObjectItem(node, "multiply");
		if (child) {
			field.bMultiply = qtrue;
			field.multiply = cJSON_ToNumber(child);
		}
	}
}

/*
============================
ParseAmmoOverride_Means

Parses a means of death override
============================
*/
/*
static void ParseAmmoOverride_Means(cJSON* json, const char* name, std::pair<qboolean, int>& field) 
{
	cJSON* node = cJSON_GetObjectItem(json, name);
	if (!node) 
	{
		field.first = qfalse;
	}
	else {
		field.first = qtrue;
		field.second = GetMeansOfDamageIndex(cJSON_ToString(json));
	}
}

/*
============================
ParseSimpleOverrideString

============================
*/
/*
static void ParseSimpleOverrideString(std::pair<qboolean, std::string>& field, const char* nodeName, cJSON* json)
{ 
 //INCOMPATIBLE WITH VS2010!!! 
	cJSON* child = cJSON_GetObjectItem(json, nodeName);
	if (child)
	{
		field.first = qtrue;
		field.second = cJSON_ToString(child);
	}
	else
	{
		field.first = qfalse;
	}
}

/*
============================
ParseSimpleOverrideFloat

============================
*/
/*
static void ParseSimpleOverrideFloat(std::pair<qboolean, float>& field, const char* nodeName, cJSON* json)
{
	cJSON* child = cJSON_GetObjectItem(json, nodeName);
	if (child)
	{
		field.first = qtrue;
		field.second = cJSON_ToNumber(child);
	}
	else
	{
		field.first = qfalse;
	}
}

/*
============================
ParseSimpleOverrideInt

============================
*/
/*
static void ParseSimpleOverrideInt(std::pair<qboolean, int>& field, const char* nodeName, cJSON* json)
{
	cJSON* child = cJSON_GetObjectItem(json, nodeName);
	if (child)
	{
		field.first = qtrue;
		field.second = cJSON_ToInteger(json);
	}
	else
	{
		field.first = qfalse;
	}
}

/*
============================
ParseSimpleOverrideVector

============================
*/
/*
static void ParseSimpleOverrideVector(std::pair<qboolean, std::vector<std::string>>& field, const char* nodeName, cJSON* json)
{
	cJSON* child = cJSON_GetObjectItem(json, nodeName);
	if (child)
	{
		field.first = qtrue;
		for (int i = 0; i < cJSON_GetArraySize(child); i++)
		{
			field.second.push_back(cJSON_ToString(cJSON_GetArrayItem(child, i)));
		}
	}
	else
	{
		field.first = qfalse;
	}
}

/*
============================
ParseSimpleOverrideVec3

============================
*/
/*
static void ParseSimpleOverrideVec3(std::pair<qboolean, vec3_t>& field, const char* nodeName, cJSON* json)
{
	cJSON* child = cJSON_GetObjectItem(json, nodeName);
	int max;

	if (child)
	{
		field.first = qtrue;
		max = cJSON_GetArraySize(child);
		if (max != 3)
		{
			field.second[0] = field.second[1] = field.second[2] = 1.0f;
		}
		else
		{
			for (int i = 0; i < max; i++)
			{
				field.second[i] = cJSON_ToNumber(cJSON_GetArrayItem(child, i));
			}
		}
	}
	else
	{
		field.first = qfalse;
	}
}

/*
============================
ParseAmmoOverrideVisuals

Processes the visual aspect of an ammo override
============================
*/
/*
static void ParseAmmoOverrideVisuals(NewAmmo_t* ammo, cJSON* json)
{
	cJSON* child;

	if (json == nullptr)
	{
		ammo->visualOverrides.crosshairShader.first = qfalse;
		goto dead1;
	}
	ParseSimpleOverrideString(ammo->visualOverrides.crosshairShader, "crosshairShader", json);
	// CANNOT WORK INTO VS2010 WE NEED TO FIX WITH EEZ... 

	// weaponRender/weaponFire
	child = cJSON_GetObjectItem(json, "fire");
	if (child)
	{
		ParseAmmoOverride_Float(child, "muzzleLightIntensity", ammo->visualOverrides.muzzleLightIntensity);
		ParseSimpleOverrideVec3(ammo->visualOverrides.muzzleLightColor, "muzzleLightColor", child);
		ParseSimpleOverrideString(ammo->visualOverrides.chargingEffect, "chargingEffect", child);
		ParseSimpleOverrideString(ammo->visualOverrides.muzzleEffect, "muzzleEffect", child);
		ParseSimpleOverrideVector(ammo->visualOverrides.fireSound, "fireSound", child);
	}
	else
	{
dead1:
		ammo->visualOverrides.muzzleLightIntensity.bIsPresent = qfalse;
		ammo->visualOverrides.muzzleLightColor.first = qfalse;
		ammo->visualOverrides.chargingEffect.first = qfalse;
		ammo->visualOverrides.muzzleEffect.first = qfalse;
		ammo->visualOverrides.fireSound.first = qfalse;
		if (json == nullptr)
		{
			goto dead2;
		}
	}

	// traceline
	child = cJSON_GetObjectItem(json, "traceline");
	if (child)
	{
		ParseSimpleOverrideString(ammo->visualOverrides.traceline.shader, "shader", child);
		ParseAmmoOverride_Float(child, "minSize", ammo->visualOverrides.traceline.minSize);
		ParseAmmoOverride_Float(child, "maxSize", ammo->visualOverrides.traceline.maxSize);
		ParseAmmoOverride_Int(child, "lifeTime", ammo->visualOverrides.traceline.lifeTime);
	}
	else
	{
dead2:
		ammo->visualOverrides.traceline.shader.first = qfalse;
		ammo->visualOverrides.traceline.minSize.bIsPresent = qfalse;
		ammo->visualOverrides.traceline.maxSize.bIsPresent = qfalse;
		ammo->visualOverrides.traceline.lifeTime.bIsPresent = qfalse;
		if (json == nullptr)
		{
			goto dead3;
		}
	}

	// projectile
	child = cJSON_GetObjectItem(json, "projectile");
	if (child)
	{
		ParseSimpleOverrideString(ammo->visualOverrides.projectile.projectileModel, "model", child);
		ParseSimpleOverrideString(ammo->visualOverrides.projectile.projectileEffect, "effect", child);
		ParseSimpleOverrideString(ammo->visualOverrides.projectile.runSound, "runSound", child);
		ParseAmmoOverride_Float(child, "lightIntensity", ammo->visualOverrides.projectile.lightIntensity);
		ParseSimpleOverrideVec3(ammo->visualOverrides.projectile.lightColor, "lightColor", child);
		ParseSimpleOverrideString(ammo->visualOverrides.projectile.deathEffect, "miss", child);
		ParseSimpleOverrideString(ammo->visualOverrides.projectile.impactEffect, "hit", child);
		ParseSimpleOverrideString(ammo->visualOverrides.projectile.deflectEffect, "deflect", child);
	}
	else
	{
dead3:
		ammo->visualOverrides.projectile.projectileModel.first = qfalse;
		ammo->visualOverrides.projectile.projectileEffect.first = qfalse;
		ammo->visualOverrides.projectile.runSound.first = qfalse;;
		ammo->visualOverrides.projectile.lightIntensity.bIsPresent = qfalse;
		ammo->visualOverrides.projectile.lightColor.first = qfalse;
		ammo->visualOverrides.projectile.deathEffect.first = qfalse;
		ammo->visualOverrides.projectile.impactEffect.first = qfalse;
		ammo->visualOverrides.projectile.deflectEffect.first = qfalse;
	}
}

/*
============================
ParseAmmoOverrides

Overrides are things such as changing damage, ...
============================
*/

static void ParseAmmoOverrides(NewAmmo_t* ammo, cJSON* json) 
{
	
	/*ParseAmmoOverride_Means(json, "means", ammo->overrides.means);
	ParseAmmoOverride_Means(json, "splashmeans", ammo->overrides.splashmeans);


	ParseAmmoOverride_Int(json, "damage", ammo->overrides.damage);
	ParseAmmoOverride_Int(json, "projectiles", ammo->overrides.projectiles);
	ParseAmmoOverride_Float(json, "splashRange", ammo->overrides.splashRange);
	ParseAmmoOverride_Float(json, "collisionSize", ammo->overrides.collisionSize);
	ParseAmmoOverride_Float(json, "recoil", ammo->overrides.recoil);
	ParseAmmoOverride_Int(json, "ammocost", ammo->overrides.ammocost);
	ParseAmmoOverride_Int(json, "fireDelay", ammo->overrides.fireDelay);
	ParseAmmoOverride_Int(json, "bounces", ammo->overrides.bounces);
	ParseAmmoOverride_Int(json, "accuracyRatingBase", ammo->overrides.accuracyRatingBase);
	ParseAmmoOverride_Int(json, "accuracyRatingPerShot", ammo->overrides.accuracyRatingPerShot);
	ParseAmmoOverride_Float(json, "knockback", ammo->overrides.knockback);
	ParseAmmoOverride_Float(json, "speed", ammo->overrides.speed);

	ParseSimpleOverrideInt(ammo->overrides.useGravity, "useGravity", json);

	ParseSimpleOverrideInt(ammo->overrides.hitscan, "hitscan", json);

	ParseAmmoOverrideVisuals(ammo, cJSON_GetObjectItem(json, "visuals"));*/
}

/*
============================
ParseSingleAmmo

Reads a single ammo entry from a .ammo file
============================
*/

/*
static qboolean ParseSingleAmmo(cJSON* json) 
{
	const char* name = cJSON_GetItemKey(json);
	NewAmmo_t* ammo = &ammoTable[numAmmoLoaded];
	cJSON* child;

	ammo->ammoIndex = numAmmoLoaded;
	Q_strncpyz(ammo->name, name, sizeof(ammo->name));

	Q_strncpyz(ammo->shortname, cJSON_ToStringOpt(cJSON_GetObjectItem(json, "shortname"), ""), sizeof(ammo->shortname));

	// Parse substitutes (we need to establish the link later)
	ammo->pSub = nullptr;
	child = cJSON_GetObjectItem(json, "basedOn");
	if (child) 
	{
		Q_strncpyz(ammo->substitute, cJSON_ToString(child), sizeof(ammo->substitute));
	}
	else 
	{
		Q_strncpyz(ammo->substitute, ammo->name, sizeof(ammo->substitute));
	}

	child = cJSON_GetObjectItem(json, "max");
	ammo->ammoMax = cJSON_ToIntegerOpt(child, 999);

	child = cJSON_GetObjectItem(json, "pricePerUnit");
	ammo->pricePerUnit = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(json, "overrides");
	if (child) 
	{
		ParseAmmoOverrides(ammo, child); 
		//QQQ FOR MOMENT WE CANNOT END THAT... -.- 
	}

	return qtrue;
}

/*
============================
LoadAmmo

Loads an individual jetpack (.jet) file.
Called on both the client and the server.
============================
*/
/*
static qboolean LoadAmmo(const char* fileName, const char* dir) 
{
	int fileLen;
	fileHandle_t f;
	char fileBuffer[AMMO_FILEBUFFER];
	char error[AMMO_ERRBUFFER];

	fileLen = gi.FS_FOpenFile(va("%s%s", dir, fileName), &f, FS_READ);

	if (!f || fileLen == -1) 
	{
		Com_Printf(S_COLOR_RED "Could not read %s\n", fileName);
		return qfalse;
	}

	if (fileLen == 0) 
	{
		gi.FS_FCloseFile(f);
		Com_Printf(S_COLOR_RED "%s is blank\n", fileName);
		return qfalse;
	}

	if ((fileLen + 1) >= AMMO_FILEBUFFER) 
	{
		gi.FS_FCloseFile(f);
		Com_Printf(S_COLOR_RED "%s is too big - max file size %d bytes (%.2f kb)\n", fileName, AMMO_FILEBUFFER, AMMO_FILEBUFFER / 1024.0f);
		return qfalse;
	}

	gi.FS_Read(&fileBuffer, fileLen, f);
	fileBuffer[fileLen] = '\0';
	gi.FS_FCloseFile(f);

	cJSON* json = cJSON_ParsePooled(fileBuffer, error, AMMO_ERRBUFFER);
	if (!json) 
	{
		Com_Printf(S_COLOR_RED "%s: %s\n", fileName, error);
		return qfalse;
	}

	// Read each node in the json file from top level as an ammo_t
	cJSON* child;
	for (child = cJSON_GetFirstItem(json); child; child = cJSON_GetNextItem(child)) 
	{
		if (numAmmoLoaded >= MAX_AMMO_TYPES) {
			Com_Printf("MAX_AMMO_TYPES exceeded\n");
			break;
		}
		if (ParseSingleAmmo(child)) 
		{
			numAmmoLoaded++;
		}

	}


	cJSON_Delete(json);
	return qtrue;
}

/*
========================
InitializeAmmo

Initializes the ammo subsystem.
========================
*/

/*
void InitializeAmmo ( void )
{
	char ammoFiles[AMMO_NAMEBUFFER] = {0};
	const char* ammo;
	const char* ammoDir = "ext_data/ammo/";
	int failed = 0;
	int numFiles;
	
	numFiles = 
	gi.FS_GetFileList(ammoDir, ".ammo", ammoFiles, sizeof(ammoFiles));
	ammo = ammoFiles;

	Com_Printf("------- LOADING AMMO LIBRARY! -------\n");
	for (int i = 0; i < numFiles; i++) 
	{
		/*if (!LoadAmmo(ammo, ammoDir)) 
		{
			failed++;
			ammo += strlen(ammo) + 1;
			continue;
		}*/

	/*	ammo += strlen(ammo) + 1;
	}

	Com_Printf("Ammo: %d successful, %d failed.\n", numAmmoLoaded, failed);
	Com_Printf("-------------------------------------\n");

	// Link up all of the substitutes
	for (int i = 0; i < numAmmoLoaded; i++) 
	{
		ammoTable[i].pSub = GetAmmo(ammoTable[i].substitute);
	}
}*/