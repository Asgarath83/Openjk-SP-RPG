
#include "../cgame/cg_local.h"
///// i got some damned OBJ errors about the jetpacl stuff
//// so for cheat it i paste the contain of bg_jetpack.cpp here 
#include "bg_armor.h"
#include "bg_jetpack.h"
#include "../cjson/cJSON.h"
#include "bg_public.h"
#include "g_local.h"
#include "../cgame/cg_local.h"

#define JETPACK_NAMEBUFFER	8192
#define JETPACK_FILEBUFFER	4096
#define JETPACK_ERRBUFFER	1024

jetpackData_t jetpackTable[MAX_JETPACKS];
int numLoadedJetpacks;

/*
============================
ParseJetpackMovement

Parses the "movement" block of a jetpack.
============================
*/
static void ParseJetpackMovement(cJSON* jsonNode, jetpackData_t& jetpackData) 
{
//	cJSON* json;
//	cJSON* jsonNode;
	cJSON* child;

	child = cJSON_GetObjectItem(jsonNode, "thrustAllowed");
	jetpackData.move.thrustAllowed = cJSON_ToBooleanOpt(child, true);
	
	child = cJSON_GetObjectItem(jsonNode, "fwdThrustAllowed");
	jetpackData.move.fwdThrustAllowed = cJSON_ToBooleanOpt(child, true);
	
	child = cJSON_GetObjectItem(jsonNode, "hoverGravity");
	jetpackData.move.hoverGravity = cJSON_ToNumberOpt(child, -1.0);

	child = cJSON_GetObjectItem(jsonNode, "forwardMove");
	jetpackData.move.forwardMove = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "backMove");
	jetpackData.move.backMove = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "sideMove");
	jetpackData.move.sideMove = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "downMove");
	jetpackData.move.downMove = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "thrustFwd");
	jetpackData.move.thrustFwd = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "thrustBack");
	jetpackData.move.thrustBack = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "thrustSide");
	jetpackData.move.thrustSide = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "thrustUp");
	jetpackData.move.thrustUp = cJSON_ToNumberOpt(child, 1.0);

	child = cJSON_GetObjectItem(jsonNode, "fwdThrustAmt");
	jetpackData.move.fwdThrustAmt = cJSON_ToNumberOpt(child, 2.0);
}

/*
============================
ParseJetpackVisuals

Parses the visual elements of a jetpack.
FIXME: Should precache?
============================
*/
static void ParseJetpackVisuals(cJSON* jsonNode, jetpackData_t& jetpackData) 
{
	cJSON* child;
	int boltSize;

	child = cJSON_GetObjectItem(jsonNode, "modelName");
	Q_strncpyz(jetpackData.visuals.modelName, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "effectBolts");
	boltSize = cJSON_GetArraySize(child);
	for (int i = 0; i < boltSize; i++) {
		cJSON* arrayObj = cJSON_GetArrayItem(child, i);
		std::string bolt = cJSON_ToString(arrayObj);
		jetpackData.visuals.effectBolts.push_back(bolt);
	}

	child = cJSON_GetObjectItem(jsonNode, "hoverEffect");
	Q_strncpyz(jetpackData.visuals.hoverEffect, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "thrustEffect");
	Q_strncpyz(jetpackData.visuals.thrustEffect, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "jetEffect");
	Q_strncpyz(jetpackData.visuals.jetEffect, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "idleSound");
	Q_strncpyz(jetpackData.visuals.idleSound, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "thrustSound");
	Q_strncpyz(jetpackData.visuals.thrustSound, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "activateSound");
	Q_strncpyz(jetpackData.visuals.activateSound, cJSON_ToStringOpt(child, ""), MAX_QPATH);

	child = cJSON_GetObjectItem(jsonNode, "deactivateSound");
	Q_strncpyz(jetpackData.visuals.deactivateSound, cJSON_ToStringOpt(child, ""), MAX_QPATH);
}

/*
============================
ParseJetpack

Parses a jetpack file, from memory.
============================
*/
static qboolean ParseJetpack(char* buffer, const char* fileName, jetpackData_t& jetpackData) 
{
	char errorBuffer[JETPACK_ERRBUFFER] = {0};
	cJSON* json;
	cJSON* jsonNode;
	
	json = cJSON_ParsePooled(buffer, errorBuffer, sizeof(errorBuffer));
	if (json == nullptr) {
		Com_Printf(S_COLOR_RED "%s: %s\n", fileName, errorBuffer);
		return qfalse;
	}

	// Basic information
	jsonNode = cJSON_GetObjectItem(json, "ref");
	if (!jsonNode) {
		Com_Printf(S_COLOR_RED "%s doesn't contain a valid ref name!\n", fileName);
		cJSON_Delete(json);
		return qfalse;
	}
	Q_strncpyz(jetpackData.ref, cJSON_ToString(jsonNode), sizeof(jetpackData.ref));

	jsonNode = cJSON_GetObjectItem(json, "fuelCapacity");
	jetpackData.fuelCapacity = cJSON_ToIntegerOpt(jsonNode, 100);

	jsonNode = cJSON_GetObjectItem(json, "fuelConsumption");
	jetpackData.fuelConsumption = cJSON_ToNumberOpt(jsonNode, 1.0);

	jsonNode = cJSON_GetObjectItem(json, "thrustConsumption");
	jetpackData.thrustConsumption = cJSON_ToNumberOpt(jsonNode, 2.0);

	jsonNode = cJSON_GetObjectItem(json, "fuelRegeneration");
	jetpackData.fuelRegeneration = cJSON_ToNumberOpt(jsonNode, 1.0);

	// Movement related fields
	jsonNode = cJSON_GetObjectItem(json, "movement");
	ParseJetpackMovement(jsonNode, jetpackData);

	// Visuals
	jsonNode = cJSON_GetObjectItem(json, "visuals");
	ParseJetpackVisuals(jsonNode, jetpackData);

	cJSON_Delete(json);
	return qtrue;
}

/*
============================
LoadJetpack

Loads an individual jetpack (.jet) file.
Called on both the client and the server.
============================
*/
static qboolean LoadJetpack(const char* fileName, const char* dir, jetpackData_t& jetpackData) {
	int fileLen;
	fileHandle_t f;
	char fileBuffer[JETPACK_FILEBUFFER];

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

	if ((fileLen + 1) >= JETPACK_FILEBUFFER) 
	{
		gi.FS_FCloseFile(f);
		Com_Printf(S_COLOR_RED "%s is too big - max file size %d bytes (%.2f kb)\n", fileName, JETPACK_FILEBUFFER, JETPACK_FILEBUFFER / 1024.0f);
		return qfalse;
	}

	gi.FS_Read(&fileBuffer, fileLen, f);
	fileBuffer[fileLen] = '\0';
	gi.FS_FCloseFile(f);

	return ParseJetpack(fileBuffer, fileName, jetpackData);
}

/*
============================
LoadJetpacks

Loads all of the jetpack items.
Called on both the client and the server.
============================
*/
void LoadJetpacks() 
{
	char jetpackFiles[JETPACK_NAMEBUFFER] = {0}; 
	const char* jetpack;
	const char* jetpackDir = "ext_data/jetpacks/";
	int failed = 0;
	int numFiles;

	numFiles = gi.FS_GetFileList(jetpackDir, ".jet", jetpackFiles, sizeof(jetpackFiles));
	jetpack = jetpackFiles;

	Com_Printf("------- INIT Jetpacks!!! -------\n");

	for (int i = 0; i < numFiles; i++) 
	{
		if (!LoadJetpack(jetpack, jetpackDir, jetpackTable[numLoadedJetpacks])) {
			failed++;
			jetpack += strlen(jetpack) + 1;
			continue;
		}

		numLoadedJetpacks++;
		jetpack += strlen(jetpack) + 1;
	}

	Com_Printf("Jetpacks: %d successful, %d failed.\n", numLoadedJetpacks, failed);
	Com_Printf("-------------------------------------\n");
}

/*
============================
FindJetpackByName

Should be used sparingly.
============================
*/
jetpackData_t* FindJetpackByName(const char* jetpackName) 
{
	for (int i = 0; i < numLoadedJetpacks; i++) {
		jetpackData_t* jetData = &jetpackTable[i];
		if (!Q_stricmp(jetData->ref, jetpackName)) {
			return jetData;
		}
	}
	return nullptr;
}


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//////////////////// ARMOR FILE //////////////////////////
#define ARMOR_NAMEBUFFER	8192
#define ARMOR_FILEBUFFER	4096
#define ARMOR_ERRBUFFER		1024

armorData_t armorTable[MAX_ARMOR_ITEMS];
int numLoadedArmor;

static armorG2Reference_t g2Table[MAX_ARMOR_G2];
static int numUsedG2;

// FIXME: move into bg_g2_utils

int G2_GetNumberOfSurfaces_(const char *modelPath)
{
	mdxmHeader_t header;
	fileHandle_t f;
	int fileLen = gi.FS_FOpenFile(modelPath, &f, FS_READ);
	if (fileLen == -1 || !f)
	{
		gi.Printf("Failed to open the model %s.\n", modelPath);
		return 0;
	}

	if (fileLen < sizeof(mdxmHeader_t))
	{
		gi.Printf("Invalid model file %s.\n", modelPath);
		return 0;
	}

	gi.FS_Read(&header, sizeof(mdxmHeader_t), f);
	gi.FS_FCloseFile(f);

	return header.numSurfaces;
}

/*
=====================
RegisterArmorGhoul2

=====================
*/
// QQQ FOR MOMENT NOT KNOW HOT TO FIX 
//THE MISSING OBJ OF CHAR MODELNAME

/*int CG_HandleAppendedSkin( char *modelName )
{
	
	char skinName[MAX_QPATH];
	char *p;
	qhandle_t skinID = 0;
	int i = 0;

	//see if it has a skin name
	p = Q_strrchr(modelName, '*');

	if (p)
	{ //found a *, we should have a model name before it and a skin name after it.
		*p = 0; //terminate the modelName string at this point, then go ahead and parse to the next 0 for the skin.
		p++;

		while (p && *p)
		{
			skinName[i] = *p;
			i++;
			p++;
		}
		skinName[i] = 0;

		if (skinName[0]) 
		{ //got it, register the skin under the model path.
			char baseFolder[MAX_QPATH];

			strcpy(baseFolder, modelName);
			p = Q_strrchr(baseFolder, '/'); //go back to the first /, should be the path point

			if (p)
			{ //got it.. terminate at the slash and register.
				char *useSkinName;

				*p = 0;

				if (strchr(skinName, '|'))
				{//three part skin
					useSkinName = va("%s/|%s", baseFolder, skinName);
				}
				else
				{
					useSkinName = va("%s/model_%s.skin", baseFolder, skinName);
				}

				skinID = gi.RE_RegisterSkin(useSkinName);
			}
		}
	}

	return skinID;
}*/
/*
armorG2Reference_t* RegisterArmorGhoul2(const char* ref, char* model) 
{
	char strSkin[MAX_QPATH];
	// Check and see if we've already registered a GHOUL2 instance with this refname
	for (int i = 0; i < numUsedG2; i++) 
	{
		if (!Q_stricmp(g2Table[i].ref, ref)) {
			return &g2Table[i];
		}
	}

	if (numUsedG2 >= MAX_ARMOR_G2) 
	{
		Com_Printf(S_COLOR_RED "Too many armor GHOUL2 instances. Ask the programmers for more space!\n");
		return nullptr; // too many
	}

	// If not, try and register it
	// TODO: skins
	Q_strncpyz(g2Table[numUsedG2].ref, ref, MAX_QPATH);

	// Strip out the stuff after the *
	char* find = Q_strrchr(model, '*');
	if (find != nullptr)
		Q_strncpyz(model, model, find - model);
	
    //g2Table[numUsedG2].skin = CG_HandleAppendedSkin(model);
	gi.G2API_JKGInitGhoul2Model(&g2Table[numUsedG2].instance, model, 0, 0, 0, 0, 0);
	int iSkinID = gi.RE_RegisterSkin( strSkin );
	if ( iSkinID )
	{
		gi.G2API_JKGSetSkin(g2Table[numUsedG2].instance, 0, g2Table[numUsedG2].skin, g2Table[numUsedG2].skin);
	}
	g2Table[numUsedG2].numSurfaces = G2_GetNumberOfSurfaces_(model);
	return &g2Table[numUsedG2++];
}*/

/*
=====================
ParseArmorVisuals

=====================
*/
void ParseArmorVisuals(cJSON* json, armorData_t& armor) 
{
	char ghoul2Ref[MAX_QPATH] = {0};
	char ghoul2Model[MAX_QPATH]= {0};
	std::string ghoul2Surface;
	cJSON* jsonNode;
	cJSON* child;
	int count;

	// FIXME: is this really needed on the server?
	jsonNode = cJSON_GetObjectItem(json, "model");
	Q_strncpyz(ghoul2Model, cJSON_ToStringOpt(jsonNode, ""), MAX_QPATH);
	jsonNode = cJSON_GetObjectItem(json, "modelGroup");
	Q_strncpyz(ghoul2Ref, cJSON_ToStringOpt(jsonNode, ""), MAX_QPATH);
	if (ghoul2Ref[0]) 
	{
		//armor.visuals.pGHOUL2 = RegisterArmorGhoul2(ghoul2Ref, ghoul2Model);
		// MISSING *char model obj function -.- QQQ
	}

	jsonNode = cJSON_GetObjectItem(json, "motionBone");
	Q_strncpyz(armor.visuals.motionBone, cJSON_ToStringOpt(jsonNode, ""), MAX_QPATH);

	// Handle surfaces to turn on/off.
	// These are handled as an array in the JSON file
	jsonNode = cJSON_GetObjectItem(json, "armorOnSurfaces");
	if (jsonNode) 
	{
		count = cJSON_GetArraySize(jsonNode);
		for (int i = 0; i < count; i++) {
			child = cJSON_GetArrayItem(jsonNode, i);
			ghoul2Surface = cJSON_ToStringOpt(child, "");
			armor.visuals.armorOnSurfs.push_back(ghoul2Surface);
		}
	}

	jsonNode = cJSON_GetObjectItem(json, "bodyOffSurfaces");
	if (jsonNode) 
	{
		count = cJSON_GetArraySize(jsonNode);
		for (int i = 0; i < count; i++) {
			child = cJSON_GetArrayItem(jsonNode, i);
			ghoul2Surface = cJSON_ToStringOpt(child, "");
			armor.visuals.bodyOffSurfs.push_back(ghoul2Surface);
		}
	}

	// The sound for equipping the piece
	jsonNode = cJSON_GetObjectItem(json, "equippedSound");
	Q_strncpyz(armor.visuals.equipSound, cJSON_ToStringOpt(jsonNode, "sound/weapons/a_pkup.wav"), MAX_QPATH);
}

/*
=====================
ArmorSlotFromText

=====================
*/
int ArmorSlotFromText(const char* text) {
	if (!Q_stricmp("head", text)) {
		return ARMSLOT_HEAD;
	}
	else if (!Q_stricmp("neck", text)) {
		return ARMSLOT_NECK; // for wings and such 
	}
	else if (!Q_stricmp("torso", text)) {
		return ARMSLOT_TORSO;
	}
	else if (!Q_stricmp("robe", text)) {
		return ARMSLOT_ROBE;
	}
	else if (!Q_stricmp("legs", text)) {
		return ARMSLOT_LEGS;
	}
	else if (!Q_stricmp("gloves", text)) {
		return ARMSLOT_GLOVES;
	}
	else if (!Q_stricmp("boots", text)) {
		return ARMSLOT_BOOTS;
	}
	else if (!Q_stricmp("shoulder", text)) {
		return ARMSLOT_SHOULDER;
	}
	else if (!Q_stricmp("implant", text)) {
		return ARMSLOT_IMPLANTS;
	}
	else if (!Q_stricmp("aura", text)) {
		return ARMSLOT_INVISIBLE;
	}
	else if (!Q_stricmp("tail", text)) {
		return ARMSLOT_TAIL;
	}
	else 
	{
		return ARMSLOT_MAX;
	}
}

/*
=====================
ParseArmorFile

=====================
*/
static qboolean ParseArmorFile(char* buffer, const char* fileName, armorData_t& armorData) 
{
	char errorBuffer[ARMOR_ERRBUFFER] = {0};
	cJSON* json;
	cJSON* jsonNode;

	json = cJSON_ParsePooled(buffer, errorBuffer, sizeof(errorBuffer));
	if (json == nullptr) {
		Com_Printf(S_COLOR_RED "%s: %s\n", fileName, errorBuffer);
		return qfalse;
	}

	// Basic information
	jsonNode = cJSON_GetObjectItem(json, "ref");
	if (!jsonNode) 
	{
		Com_Printf(S_COLOR_RED "%s doesn't contain a valid ref name!\n", fileName);
		cJSON_Delete(json);
		return qfalse;
	}
	Q_strncpyz(armorData.ref, cJSON_ToString(jsonNode), sizeof(armorData.ref));

	jsonNode = cJSON_GetObjectItem(json, "slot");
	armorData.slot = ArmorSlotFromText(cJSON_ToStringOpt(jsonNode, ""));

	jsonNode = cJSON_GetObjectItem(json, "armor");
	armorData.armor = cJSON_ToIntegerOpt(jsonNode, 0);

	jsonNode = cJSON_GetObjectItem(json, "hp");
	armorData.hp = cJSON_ToIntegerOpt(jsonNode, 0);

	jsonNode = cJSON_GetObjectItem(json, "movemodifier");
	armorData.movemodifier = cJSON_ToNumberOpt(jsonNode, 1.0);

	jsonNode = cJSON_GetObjectItem(json, "visuals");
	ParseArmorVisuals(jsonNode, armorData);

	// ADD STUFF ALSO FOR THE ELEMENTAL VARIOUS RESISTANCE LATER, TO THE VARIOUS MOD... 
	
	cJSON_Delete(json);
	return qtrue;
}

/*
=====================
LoadArmorFile


=====================
*/
static qboolean LoadArmorFile(const char* fileName, const char* dir, armorData_t& armorData) 
{
	int fileLen;
	fileHandle_t f;
	char fileBuffer[ARMOR_FILEBUFFER];

	fileLen = gi.FS_FOpenFile(va("%s%s", dir, fileName), &f, FS_READ);

	if (!f || fileLen == -1) {
		Com_Printf(S_COLOR_RED "Could not read %s\n", fileName);
		return qfalse;
	}

	if (fileLen == 0) 
	{
		gi.FS_FCloseFile(f);
		Com_Printf(S_COLOR_RED "%s is blank\n", fileName);
		return qfalse;
	}

	if ((fileLen + 1) >= ARMOR_FILEBUFFER) 
	{
		gi.FS_FCloseFile(f);
		Com_Printf(S_COLOR_RED "%s is too big - max file size %d bytes (%.2f kb)\n", fileName, ARMOR_FILEBUFFER, ARMOR_FILEBUFFER / 1024.0f);
		return qfalse;
	}

	gi.FS_Read(&fileBuffer, fileLen, f);
	fileBuffer[fileLen] = '\0';
	gi.FS_FCloseFile(f);

	return ParseArmorFile(fileBuffer, fileName, armorData);
}

/*
=====================
LoadArmor

=====================
*/

void LoadArmor() 
{
	char armorFiles[ARMOR_NAMEBUFFER] = {0};
	const char* armor;
	const char* armorDir = "ext_data/armor/";
	int failed = 0;
	int numFiles;

	numFiles = gi.FS_GetFileList(armorDir, ".arm", armorFiles, sizeof(armorFiles));
	armor = armorFiles;

	Com_Printf("------- Inizialite Armor!!! -------\n");

	for (int i = 0; i < numFiles; i++) 
	{
		if (!LoadArmorFile(armor, armorDir, armorTable[numLoadedArmor])) 
		{
			failed++;
			armor += strlen(armor) + 1;
			continue;
		}

		numLoadedArmor++;
		armor += strlen(armor) + 1;
	}

	Com_Printf("Armor: %d successful, %d failed.\n", numLoadedArmor, failed);
	Com_Printf("-------------------------------------\n");
	// load also jetpack library now. 
	LoadJetpacks();
}

/*
=====================
UnloadArmor

=====================
*/
void UnloadArmor() 
{
	for (int i = 0; i < numUsedG2; i++) 
	{
		armorG2Reference_t* g2 = &g2Table[i];
		if (g2->instance && gi.G2_HaveWeGhoul2Models(g2->instance))

		{
			gi.G2API_CleanGhoul2Models(g_entities[i].ghoul2);
			// QQQ ERRRR NOPE. THAT'S DIFFERENT FOR SP (g2->instance);
			g2->instance = nullptr;
		}
	}
}

/*
=====================
FindArmorByName

=====================
*/
armorData_t* FindArmorByName(const char* ref) 
{
	for (int i = 0; i < numLoadedArmor && i < MAX_ARMOR_ITEMS; i++) {
		if (!Q_stricmp(armorTable[i].ref, ref)) 
		{
			return &armorTable[i];
		}
	}
	return nullptr;
}

