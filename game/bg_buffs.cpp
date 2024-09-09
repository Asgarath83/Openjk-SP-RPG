#include "bg_buffs.h"
#include "bg_damage.h"
#include <../cjson/cJSON.h>



#ifdef _GAME
#include "g_local.h"
#elif defined (IN_UI)
#include "ui/ui_local.h"
#else
#include <../cgame/cg_local.h>
#endif

#if defined(_GAME)
#include "g_local.h"
#elif defined(_CGAME)
#include "cgame/cg_local.h"
#elif defined(IN_UI)
#include "ui/ui_local.h"
#endif

#define MAX_BUFF_FILESIZE	16384
#define MAX_BUFF_ERRLEN		1024

Buff_t	buffTable[MAX_BUFFS];
static int	nLastUsedBuff = 0;

// Determines if this entityState has a freezing buff associated with it

qboolean HasFreezingBuff(entityState_t* es)
{
	for (int i = 0; i < PLAYERBUFF_BITS; i++)
	{
		if (es->buffsActive & (1 << i))
		{// this stuff continue On Cg_players.cpp also for visual stuff! 
			Buff_t* pBuff = &buffTable[es->buffs[i].buffID];
			if (pBuff->passive.overridePmoveType.first)
			{
				if (pBuff->passive.overridePmoveType.second == PM_FREEZE ||
					pBuff->passive.overridePmoveType.second == PM_LOCK)
				{
					return qtrue;
				}
			}
		}
	}

	return qfalse;
}

// Removes all buffs of a certain category on a playerstate
void RemoveBuffCategory(const char* buffCategory, playerState_t* ps)
{
	for (int i = 0; i < PLAYERBUFF_BITS; i++)
	{
		if (ps->buffsActive & (1 << i))
		{
			Buff_t* pBuff = &buffTable[ps->buffs[i].buffID];
			if (!Q_stricmp(pBuff->category, buffCategory))
			{
				ps->buffsActive &= ~(1 << i); // remove this buff
			}
		}
	}
}

// Removes all buffs that have the waterRemoval flag set
void CheckWaterRemoval(playerState_t* ps)
{
	for (int i = 0; i < PLAYERBUFF_BITS; i++)
	{
		if (ps->buffsActive & (1 << i))
		{
			Buff_t* pBuff = &buffTable[ps->buffs[i].buffID];
			if (pBuff->cancel.waterRemoval)
			{
				ps->buffsActive &= ~(1 << i); // remove this buff
			}
		}
	}
}

// Removes all buffs that have the rollRemoval flag set
void CheckRollRemoval(playerState_t* ps)
{
	// Was unchecked. but why?!?!?
	for (int i = 0; i < PLAYERBUFF_BITS; i++)
	{
		if (ps->buffsActive & (1 << i))
		{
			Buff_t* pBuff = &buffTable[ps->buffs[i].buffID];
			if (pBuff->cancel.rollRemoval)
			{
				//ps->buffsActive &= ~(1 << i); // remove this buff
			}
		}
	}
}

// Resolves a buff name to an index in the lookup table
int ResolveBuffName(const char* szBuffName)
{
	for (int i = 0; i < nLastUsedBuff; i++)
	{
		if (!Q_stricmp(buffTable[i].name, szBuffName))
		{
			return i;
		}
	}
	return -1;
}

// Converts a string into a pmove type
static int ParsePmoveType(const char* str)
{
	if (!Q_stricmp("PM_NORMAL", str))
	{
		return PM_NORMAL;
	}
	else if (!Q_stricmp("PM_JETPACK", str))
	{
		return PM_JETPACK;
	}
	else if (!Q_stricmp("PM_FLOAT", str))
	{
		return PM_FLOAT;
	}
	else if (!Q_stricmp("PM_NOCLIP", str))
	{
		return PM_NOCLIP;
	}
	else if (!Q_stricmp("PM_SPECTATOR", str))
	{
		return PM_SPECTATOR;
	}
	else if (!Q_stricmp("PM_DEAD", str))
	{
		return PM_DEAD;
	}
	else if (!Q_stricmp("PM_FREEZE", str))
	{
		return PM_FREEZE;
	}
	else if (!Q_stricmp("PM_INTERMISSION", str))
	{
		return PM_INTERMISSION;
	}
	else if (!Q_stricmp("PM_SPINTERMISSION", str))
	{
		return PM_SPINTERMISSION;
	}
	else if (!Q_stricmp("PM_LOCK", str))
	{
		return PM_LOCK;
	}
	else if (!Q_stricmp("PM_NOMOVE", str))
	{
		return PM_NOMOVE;
	}
	return PM_NORMAL;
}

// Parses buff canceling
static qboolean ParseBuffCanceling(cJSON* json, Buff_t* pBuff)
{
	cJSON* child = cJSON_GetObjectItem(json, "priority");
	pBuff->cancel.priority = cJSON_ToIntegerOpt(child, 50);

	child = cJSON_GetObjectItem(json, "noBuffStack");
	pBuff->cancel.noBuffStack = cJSON_ToBooleanOpt(child, false);

	child = cJSON_GetObjectItem(json, "noCategoryStack");
	pBuff->cancel.noCategoryStack = cJSON_ToBooleanOpt(child, false);

	child = cJSON_GetObjectItem(json, "waterRemoval");
	pBuff->cancel.waterRemoval = cJSON_ToBooleanOpt(child, false);

	child = cJSON_GetObjectItem(json, "rollRemoval");
	pBuff->cancel.rollRemoval = cJSON_ToBooleanOpt(child, false);

	child = cJSON_GetObjectItem(json, "cancelOther");
	if (child)
	{
		for (int i = 0; i < cJSON_GetArraySize(child); i++)
		{
			cJSON* arrayElement = cJSON_GetArrayItem(child, i);
			OtherBuffCancel_t otherCancel;

			cJSON* arrayChild = cJSON_GetObjectItem(arrayElement, "category");
			Q_strncpyz(otherCancel.category, cJSON_ToStringOpt(arrayChild, ""), BUFF_CATEGORY_LEN);

			arrayChild = cJSON_GetObjectItem(arrayElement, "priority");
			otherCancel.priority = cJSON_ToIntegerOpt(arrayChild, 50);

			pBuff->cancel.other.push_back(otherCancel);
		}
	}
	return qtrue;
}

// Parse buff damage
static qboolean ParseBuffDamage(cJSON* json, Buff_t* pBuff)
{
	cJSON* child = cJSON_GetObjectItem(json, "damage");
	pBuff->damage.damage = cJSON_ToIntegerOpt(child, 0);

	child = cJSON_GetObjectItem(json, "percentage");
	pBuff->damage.percentage = cJSON_ToBooleanOpt(child, false);

	child = cJSON_GetObjectItem(json, "deadly");
	pBuff->damage.deadly = cJSON_ToBooleanOpt(child, true);	//buffs are deadly by default

 	child = cJSON_GetObjectItem(json, "means");
	pBuff->damage.meansOfDeath = GetMeansOfDamageIndex(cJSON_ToStringOpt(child, "MOD_UNKNOWN"));

	child = cJSON_GetObjectItem(json, "damageRate");
	pBuff->damage.damageRate = cJSON_ToIntegerOpt(child, 500);
	return qtrue;
}

// Parse buff passive data
static qboolean ParseBuffPassiveData(cJSON* json, Buff_t* pBuff)
{
	// These are a bit different from the others in that they override stuff
	cJSON* child = cJSON_GetObjectItem(json, "pmove");
	if (child)
	{
		pBuff->passive.overridePmoveType.first = qtrue;
		pBuff->passive.overridePmoveType.second = ParsePmoveType(cJSON_ToStringOpt(child, "PM_NORMAL"));
	}
	else
	{
		pBuff->passive.overridePmoveType.first = qfalse;
	}
	return qtrue;
}

//#ifdef _CGAME // WE ARE INTO SP, SO I THINK THIS IS GET OFF! 
static qboolean ParseBuffVisuals(cJSON* json, Buff_t* pBuff)
{
	cJSON* child = cJSON_GetObjectItem(json, "efx");
	cJSON* subchild;

	if (child)
	{
		pBuff->visuals.useEFX = qtrue;
		subchild = cJSON_GetObjectItem(child, "effect");
		Q_strncpyz(pBuff->visuals.efx, cJSON_ToStringOpt(subchild, ""), BUFF_EFX_LEN);

		subchild = cJSON_GetObjectItem(child, "bolt");
		Q_strncpyz(pBuff->visuals.efxBolt, cJSON_ToStringOpt(subchild, ""), BUFF_BOLT_LEN);

		subchild = cJSON_GetObjectItem(child, "debounce");
		pBuff->visuals.efxDebounce = cJSON_ToIntegerOpt(subchild, 100);
	}
	else
	{
		pBuff->visuals.useEFX = qfalse;
	}

	child = cJSON_GetObjectItem(json, "shader");
	if (child)
	{
		pBuff->visuals.useShader = qtrue;
		subchild = cJSON_GetObjectItem(child, "shader");
		Q_strncpyz(pBuff->visuals.shader, cJSON_ToStringOpt(subchild, ""), BUFF_EFX_LEN);

		subchild = cJSON_GetObjectItem(child, "shaderRGBA");
		for (int i = 0; i < cJSON_GetArraySize(subchild); i++)
		{
			cJSON* arrayItem = cJSON_GetArrayItem(subchild, i);
			if (i < 4)
			{
				pBuff->visuals.shaderRGBA[i] = cJSON_ToIntegerOpt(arrayItem, 0);
			}
		}

		subchild = cJSON_GetObjectItem(child, "shaderLen");
		pBuff->visuals.shaderLen = cJSON_ToIntegerOpt(subchild, 400);
	}
	else
	{
		pBuff->visuals.useShader = qfalse;
	}
	return qtrue;
}
//#endif

// Parses a single buff
static qboolean ParseBuff(const char* buffName, cJSON* json)
{
	Buff_t* pBuff = &buffTable[nLastUsedBuff++];
	cJSON* child;

	if (nLastUsedBuff > MAX_BUFFS)
	{
		Com_Printf("Too many buffs. Max is %i\n", MAX_BUFFS);
		return qfalse;
	}

	// copy the name first
	Q_strncpyz(pBuff->name, buffName, BUFF_NAME_LEN);

	// now read the fields
	child = cJSON_GetObjectItem(json, "category");
	Q_strncpyz(pBuff->category, cJSON_ToStringOpt(child, ""), BUFF_CATEGORY_LEN);

	child = cJSON_GetObjectItem(json, "canceling");
	if (!ParseBuffCanceling(child, pBuff))
	{
		Com_Printf("Couldn't parse buff canceling data\n");
		return qfalse;
	}

	child = cJSON_GetObjectItem(json, "damage");
	if (!ParseBuffDamage(child, pBuff))
	{
		Com_Printf("Couldn't parse buff damage\n");
		return qfalse;
	}

	child = cJSON_GetObjectItem(json, "passive");
	if (!ParseBuffPassiveData(child, pBuff))
	{
		Com_Printf("Couldn't parse buff passive data\n");
		return qfalse;
	}

//#ifdef _CGAME we are into SP, so get off that. 
	child = cJSON_GetObjectItem(json, "visuals");
	if (!ParseBuffVisuals(child, pBuff))
	{
		Com_Printf("Couldn't parse buff visuals\n");
		return qfalse;
	}
//#endif

	return qtrue;
}

// Parses the buffs file
static qboolean ParseBuffs(char* buffer)
{
	char error[MAX_BUFF_ERRLEN] = { 0 };
	cJSON* json = cJSON_ParsePooled(buffer, error, MAX_BUFF_ERRLEN);

	// If it's null, then we've encountered a parsing or analyzing error and need to die
	if (json == nullptr)
	{
		Com_Printf("^1%s\n", error);
		return qfalse;
	}

	// Iterate through all of the buffs in this file
	for (cJSON* it = cJSON_GetFirstItem(json); it; it = cJSON_GetNextItem(it))
	{
		if (!ParseBuff(cJSON_GetItemKey(it), it))
		{
			return qfalse;
		}
	}

	return qtrue;
}

// Reads the buffs file
void InitializeBuffs()
{
	fileHandle_t f;
	char buffer[MAX_BUFF_FILESIZE] = { 0 };

	int fileLen = gi.FS_FOpenFile("ext_data/tables/buffs.json", &f, FS_READ);
	Com_Printf("---------READING ext_data/tables/buffs.json!\n");
	if (fileLen == -1 || f == -1)
	{
		Com_Printf("Couldn't load buffs.json for reading");
		return;
	}

	if (fileLen == 0)
	{
		Com_Printf("buffs.json is blank");
		gi.FS_FCloseFile(f);
		return;
	}

	if (fileLen >= MAX_BUFF_FILESIZE)
	{
		Com_Printf("buffs.json is too large (%i > %i)", fileLen, MAX_BUFF_FILESIZE);
		gi.FS_FCloseFile(f);
		return;
	}
	gi.FS_Read(buffer, fileLen, f);
	buffer[fileLen] = '\0';
	gi.FS_FCloseFile(f);

	if (!ParseBuffs(buffer))
	{
		Com_Printf("Failed to parse buffs.json");
		return;
	}
}

// Get all of the buff names
void GetBuffNames(std::vector<std::string>& outBuffNames)
{
	for (int i = 0; i < nLastUsedBuff; i++)
	{
		outBuffNames.push_back(buffTable[i].name);
	}
}