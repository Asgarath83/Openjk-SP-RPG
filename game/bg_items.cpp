
#include "../qcommon/q_shared.h"
#include "bg_items.h"
#include "bg_weapons.h"
#ifdef _GAME
#include "g_local.h"
#elif defined (IN_UI)
#include "ui/ui_local.h"
#else
#include <../cgame/cg_local.h>
#endif
#include <../cjson/cJSON.h>
/*
#if defined(_GAME)
    #include "g_local.h"
#elif defined(_CGAME)
    #include "../cgame/cg_local.h"*/


itemData_t* itemLookupTable;

const stringID_table_s itemPacketNames[] = {
	ENUM2STRING(IPT_ADD),
	ENUM2STRING(IPT_REM),
	ENUM2STRING(IPT_ADDACI),
	ENUM2STRING(IPT_CLEAR),
	ENUM2STRING(IPT_OPEN),
	ENUM2STRING(IPT_QUANT),
	ENUM2STRING(IPT_RESET),
	ENUM2STRING(IPT_EQUIP),
	ENUM2STRING(IPT_UNEQUIP),
	{ nullptr, IPT_NULL }
};

const stringID_table_s tradePacketNames[] = {
	ENUM2STRING(IPT_TRADE),
	ENUM2STRING(IPT_TRADEOPEN),
	ENUM2STRING(IPT_TRADETRANS),
	ENUM2STRING(IPT_TRADESINGLE),
	ENUM2STRING(IPT_TRADEREADY),
	ENUM2STRING(IPT_TRADECREDITS),
	{ nullptr, IPT_NULL }
};

/*
====================
ItemPacketFromName
====================
*/

itemPacketType_t ItemPacketFromName(const char* szPacketName) {
	int i = 0;
	while (itemPacketNames[i].name != nullptr) {
		if (!Q_stricmp(itemPacketNames[i].name, szPacketName)) {
			return (itemPacketType_t)itemPacketNames[i].id;
		}
		i++;
	}
	return IPT_NULL;
}

/*
====================
ItemPacketName
====================
*/

const char* ItemPacketName(const int itemPacket) 
{
	return itemPacketNames[itemPacket].name;
}

/*
====================
TradePacketFromName
====================
*/
itemTradePacketType_t TradePacketFromName(const char* szPacketName) {
	int i = 0;
	while (tradePacketNames[i].name != nullptr) 
	{
		if (!Q_stricmp(tradePacketNames[i].name, szPacketName)) 
		{
			return (itemTradePacketType_t)tradePacketNames[i].id;
		}
		i++;
	}
	return IPT_TRADECANCEL;
}

/*
====================
TradePacketName
====================
*/

const char* TradePacketName(const int itemPacket) 
{
	return tradePacketNames[itemPacket].name;
}

/*
====================
GetNextFreeItemSlot
====================
*/
static int GetNextFreeItemSlot ( void )
{
    int i = 1;
    for ( i = 1; i < MAX_ITEM_TABLE_SIZE; i++ )
    {
        if ( !itemLookupTable[i].itemID )
        {
            return i;
        }
    }
    
    return 0;
}

/*
====================
GetItemByWeaponIndex
====================
*/

itemData_t *GetItemByWeaponIndex(int weaponIndex)
{
	int i;
	for (i = 0; i < MAX_ITEM_TABLE_SIZE; i++)
	{
		itemData_t *item = &itemLookupTable[i];
		if (item->itemID && item->weaponData.varID == weaponIndex)
		{
			return item;
		}
	}

	return NULL;
}

/*
====================
FindItemByInternal
====================
*/
int FindItemByInternal(const char *internalName)
{
	for (int i = 1; i < MAX_ITEM_TABLE_SIZE; i++)
	{
		if (itemLookupTable[i].itemID)
		{
			if (Q_stricmp(itemLookupTable[i].internalName, internalName) == 0)
			{
				return i;
			}
		}
	}
	return 0;
}

/*
====================
PrintItemList
====================
*/
void PrintItemList(void)
{
	for (int i = 0; i < MAX_ITEM_TABLE_SIZE; i++)
	{
		if (itemLookupTable[i].itemID)
		{
			gi.Printf("%8i\t%s\n", itemLookupTable[i].itemID, itemLookupTable[i].internalName);
		}
	}
}

/*
====================
HasWeaponItem
====================
*/
qboolean HasWeaponItem ( int clientNum, int weaponId )
{


	centity_t* cent = &cg_entities[0];
	int weaponNum;
    // Hate having to do it like this, but seems like the only way atm...
//#ifdef _CGAME
	for (auto it = cent->gent->inventory->begin(); it != cent->gent->inventory->end(); ++it)
    {
		if (!it->id || !it->id->itemID)
			break;
		if (it->id->itemType != ITEM_WEAPON)
			continue;
		weaponNum = GetWeaponIndex(it->id->weaponData.weapon, it->id->weaponData.variation);
		if( weaponNum == weaponId )
			return qtrue;
    }
//#else
    gentity_t *ent = &g_entities[clientNum];
	for(auto it = ent->inventory->begin(); it != ent->inventory->end(); ++it) 
	{
		if(!it->id || !it->id->itemID) {
			continue;
		}
		if(it->id->itemType != ITEM_WEAPON) {
			continue;
		}
		weaponNum = GetWeaponIndex(it->id->weaponData.weapon, it->id->weaponData.variation);
		if(weaponNum == weaponId) {
			return qtrue;
		}
	}
//#endif

    return qfalse;
}

/*
====================
FindItemDataByName

Finds an item instance based on internal name
====================
*/
itemData_t* FindItemDataByName(const char* internalName) 
{
	for (int i = 0; i < MAX_ITEM_TABLE_SIZE; i++) 
	{
		itemData_t* pItemData = &itemLookupTable[i];
		if (pItemData->itemID == 0) {
			continue;
		}
		if (!Q_stricmp(pItemData->internalName, internalName)) {
			return pItemData;
		}
	}
	return nullptr;
}

/*
====================
SendItemPacket

Server only-send an item packet
====================
*/
//#ifdef _GAME
void SendItemPacket(itemPacketType_t packetType, gentity_t* ent, void* memData, int intData, int intData2) {
	const char* packetName = ItemPacketName(packetType);
	char packet[MAX_STRING_CHARS] = {0};
	switch(packetType) {
		case IPT_ADD:
		case IPT_ADDACI:
			{
				itemInstance_t* pItem = (itemInstance_t*)memData;
				Com_sprintf(packet, sizeof(packet), "pInv %s %i %i", packetName, pItem->id->itemID, pItem->quantity);
			}
			break;
		case IPT_REM:
		case IPT_EQUIP:
		case IPT_UNEQUIP:
			{
				int itemSlot = intData;
				Com_sprintf(packet, sizeof(packet), "pInv %s %i", packetName, itemSlot);
			}
			break;
		case IPT_OPEN:
		case IPT_CLEAR:
			Com_sprintf(packet, sizeof(packet), "pInv %s", packetName);
			break;
		case IPT_QUANT:
			Com_sprintf(packet, sizeof(packet), "pInv %s %i %i", packetName, intData, intData2);
			break;
		case IPT_RESET:
			Com_sprintf(packet, sizeof(packet), "pInv %s %i ", packetName, intData);
			{
				for(auto it = ent->inventory->begin(); it != ent->inventory->end(); ++it) {
					// big thanks to Daggo for fixing this
					Q_strncpyz(packet, va("%s %i %i", packet, it->id->itemID, it->quantity), sizeof(packet));
				}
			}
			break;
		default:
			break;
	}
	gi.SendServerCommand(ent - g_entities, packet);
}
//#endif

/*
====================
ReceivedItemPacket

Client only-received an item packet
====================
*/
//#ifdef _CGAME
extern void CG_FillACISlot(int itemNum, int slot);
extern void CG_ACIPostFix(int itemSlot);

void ReceivedItemPacket(itemPacketType_t packetType) 
{
	centity_t* cent = &cg_entities[0];


	switch (packetType) {
		case IPT_ADD:
			{
				// Add the item to our inventory
				int itemID = atoi(CG_Argv(2));
				int itemQuantity = atoi(CG_Argv(3));
				itemInstance_t item = ItemInstance(itemID, itemQuantity);
				GiveItem(item); 
			}
			break;
		case IPT_ADDACI:
			{
				// Add the item to our inventory
				int itemID = atoi(CG_Argv(2));
				int itemQuantity = atoi(CG_Argv(3));
				itemInstance_t item = ItemInstance(itemID, itemQuantity);
				GiveItem(item);
				CG_FillACISlot(cent->gent->inventory->size() - 1, -1);	// WARNING: this should be avoided with items that can stack
			}
			break;
		case IPT_QUANT:
			{
				// Change the quantity on an item stack
				int itemStack = atoi(CG_Argv(2));
				int newQuant = atoi(CG_Argv(3));
				(*cent->gent->inventory)[itemStack].quantity = newQuant;
			}
			break;
		case IPT_REM:
			// Remove the item from our inventory
			{
				int itemStack = atoi(CG_Argv(2));
			//	RemoveItemStack(itemStack);
			// WE NEED TO FIX THAT ... 
				CG_ACIPostFix(itemStack);
			}
			
			break;
		case IPT_CLEAR:
			// Clear the inventory
			cent->gent->inventory->clear();
			break;
		case IPT_OPEN:
			// Open the inventory menu
			//uiImports->InventoryNotify(INVENTORYNOTIFY_OPEN);
			// WE NEED TO FIX THAT LATER QQQ 
			break;
		case IPT_RESET:
			// Clear the inventory and fill it with fresh data (usually from a vid_restart)
			cent->gent->inventory->clear();
			{
				int numItems = atoi(CG_Argv(2));
				for (int i = 0; i < numItems; i++) {
					int itemID = atoi(CG_Argv(3 + (2 * i)));
					int quant = atoi(CG_Argv(4 + (2 * i)));
					itemInstance_t item = ItemInstance(itemID, quant);
					cent->gent->inventory->push_back(item);
				}
			}
			break;
		case IPT_EQUIP:
			// Equipped an item
			{
				int invID = atoi(CG_Argv(2));
				if(invID < 0 || invID >= cent->gent->inventory->size()) 
				{
					return;
				}
				(*cent->gent->inventory)[invID].equipped = qtrue;

				// In the case of armor, we need to be smart enough to recognize that we could be overriding
				// a previous piece of equipment. BUT, if we send two packets, there's no guarantee that they're in order.
				// So we simulate the behavior on the client.
				/// QQQQ WE NOT HAVE AGAIN ARMOR ENABLED! UNDERSTOOD??? 
				/*for (int i = 0; i < cent->gent->inventory->size(); i++)
				{
					if (i != invID && (*cent->gent->inventory)[i].id->itemType == ITEM_ARMOR)
					{
						if ((*cent->gent->inventory)[i].id->armorData.pArm->slot == (*cent->gent->inventory)[invID].id->armorData.pArm->slot)
						{
							(*cent->inventory[i].equipped = qfalse;
							break;
						}
					}
				}*/
			}
			break;
		case IPT_UNEQUIP:
			// Unequipped an item
			{
				int invID = atoi(CG_Argv(2));
				if(invID < 0 || invID >= cent->gent->inventory->size()) {
					return;
				}
				(*cent->gent->inventory)[invID].equipped = qfalse;
			}
			break;
		default:
			break;
	}
}
//#endif

/*
====================
SendTradePacket

Server only-send a trade packet to the client
====================
*/
//#ifdef _GAME
void SendTradePacket(itemTradePacketType_t packetType, gentity_t* ent, gentity_t* other, void* memData, int intData, int intData2) 
{
	const char* packetName = TradePacketName(packetType);
	char packet[MAX_STRING_CHARS] = {0};
	if(!ent || !other) {
		return;
	}
	switch(packetType) {
		case IPT_TRADE: // Sent when an item is modified. We need to resend all
		case IPT_TRADEOPEN: // Sent when we should open the trade menu
			{
				int numItems = intData;
				itemInstance_t* pItems = (itemInstance_t*)memData;
				Com_sprintf(packet, sizeof(packet), "pTrade %s %i %i ", packetName, other->s.number, numItems);
				for(int i = 0; i < numItems; i++) {
					itemInstance_t* pItem = &pItems[i];
					Q_strcat(packet, sizeof(packet), va("%i %i ", pItem->id->itemID, pItem->quantity));
				}
			}
			break;
		case IPT_TRADECANCEL:
		case IPT_TRADETRANS:
		case IPT_TRADEREADY:
			Com_sprintf(packet, sizeof(packet), "pTrade %s", packetType);
			break;
		case IPT_TRADESINGLE:
			{
				itemInstance_t* item = (itemInstance_t*)memData;
				int credits = intData;
				Com_sprintf(packet, sizeof(packet), "pTrade %s %i %i %i", packetName, credits, item->id->itemID, item->quantity);
			}
			break;
		case IPT_TRADECREDITS:
			{
				int credits = intData;
				Com_sprintf(packet, sizeof(packet), "pTrade %s %i", packetName, credits);
			}
			break;
	}
	gi.SendServerCommand(ent - g_entities, packet);
}
//#endif

/*
====================
ReceivedTradePacket

Server only-send a trade packet to the client
====================
*/
//#ifdef _CGAME
extern void OpenShopMenu_f(void);

void ReceivedTradePacket(itemTradePacketType_t packet) {
	switch (packet) {
		// TODO: generalize sending/receiving of item instances
		case IPT_TRADE:
			cg.otherTradeItems->clear();
			if (cg.currentlyTradingWith == ENTITYNUM_NONE) {
				Com_Error(ERR_DROP, "IPT_TRADE sent while not trading");
				return;
			}
			{
				int otherEntity = atoi(CG_Argv(2));
				if (otherEntity != cg.currentlyTradingWith) {
					Com_Printf("WARNING: Orphaned trade entity %i (improperly terminated trade?)\n", cg.currentlyTradingWith);
				}
				int numItems = atoi(CG_Argv(3));
				for (int i = 0; i < numItems; i++) {
					int itemID = atoi(CG_Argv(4 + (2*i)));
					int quantity = atoi(CG_Argv(5 + (2*i)));
					itemInstance_t item = ItemInstance(itemID, quantity);
					cg.otherTradeItems->push_back(item);
				}
				cg.currentlyTradingWith = otherEntity;
			}
			break;
		case IPT_TRADEOPEN:
			cg.otherTradeItems->clear();
			cg.ourTradeItems->clear();
			{
				// Parse the input, including the items
				int otherEntity = atoi(CG_Argv(2));
				int numItems = atoi(CG_Argv(3));
				for (int i = 0; i < numItems; i++) {
					int itemID = atoi(CG_Argv(4 + (2 * i)));
					int quantity = atoi(CG_Argv(5 + (2 * i)));
					itemInstance_t item = ItemInstance(itemID, quantity);
					cg.otherTradeItems->push_back(item);
				}
				cg.currentlyTradingWith = otherEntity;

				// Open the appropriate menu, if not in a demo
				/*if (!cg.demoPlayback) 
				{
					centity_t* cent = &cg_entities[otherEntity];
					if (cent->currentState.eType == ET_PLAYER) {
						// TODO - trade between players
					}
/*					else if (cent->currentState.eType == ET_NPC) 
					{
					OpenShopMenu_f();
					} 
					else {
						// TODO - corpse/container looting
					}
				}*/
			}
			break;
		case IPT_TRADETRANS:
			// Transfer the items
			{
				// First remove the items from our inventory. THIS MUST BE DONE THE SAME WAY ON THE SERVER!
				for (auto it = cg.ourTradeItems->begin(); it != cg.ourTradeItems->end(); ++it) {
					RemoveItemNonNetworked(*it);
				}

				// Add the new items to our inventory
				for (auto it = cg.otherTradeItems->begin(); it != cg.otherTradeItems->end(); ++it) {
					GiveItem(*it);
				}
			}
			cg.otherTradeItems->clear();
			cg.ourTradeItems->clear();
			cg.currentlyTradingWith = ENTITYNUM_NONE;
			break;
		case IPT_TRADESINGLE:
			{
				int credits = atoi(CG_Argv(2));
				int itemID = atoi(CG_Argv(3));
				int quantity = atoi(CG_Argv(4));
				itemInstance_t item = ItemInstance(itemID, quantity);
				GiveItem(item);
			}
			cg.ourTradeItems->clear();
			break;
		case IPT_TRADECANCEL:
			cg.otherTradeItems->clear();
			cg.ourTradeItems->clear();
			cg.currentlyTradingWith = ENTITYNUM_NONE;
			break;
		case IPT_TRADEREADY:
			break;
		case IPT_TRADECREDITS:
			cg.otherTradeCredits = atoi(CG_Argv(2));
			break;
	}
}
//#endif

/*
====================
ItemInstance

Creates a new itemInstance object.
This will cap at max quantity.
====================
*/
itemInstance_t ItemInstance(itemData_t* pItemData, const int quantity) 
{
	itemInstance_t returnValue = { nullptr, 0, false };

	if (pItemData == nullptr) {
		return returnValue;
	}
	returnValue.quantity = quantity;
	returnValue.id = pItemData;
	if (pItemData->maxStack != 0 && quantity > pItemData->maxStack) {
		returnValue.quantity = pItemData->maxStack;
	}
	return returnValue;
}

itemInstance_t ItemInstance(const char* internalName, const int quantity) 
{
	itemData_t* pItemData = FindItemDataByName(internalName);
	return ItemInstance(pItemData, quantity);
}

itemInstance_t ItemInstance(const int itemID, const int quantity) 
{
	itemData_t* pItemData = &itemLookupTable[itemID];
	return ItemInstance(pItemData, quantity);
}

/*
====================
FirstStack

Checks to see if a container (vector of item instances) contains this item
Returns the location of the first stack of this item instance
====================
*/
int FirstStack(const std::vector<itemInstance_t>& container, const int itemID) 
{
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->id != nullptr && it->id->itemID == itemID) {
			return it - container.begin();
		}
	}
	return -1;
}

/*
====================
NextStack

Finds the next stack in a vector of item instances, starting from the current.
====================
*/

int NextStack(const std::vector<itemInstance_t>& container, const int itemID, const int prevStack) 
{
	for (auto it = container.begin() + prevStack + 1; it != container.end(); ++it) 
	{
		if (it->id != nullptr && it->id->itemID == itemID) {
			return it - container.begin();
		}
	}
	return -1;
}

/*
====================
GiveItem

Make sure that the behavior matches the server!!
====================
*/

//#ifdef _CGAME
void GiveItem(itemInstance_t item) 
{
	GiveItemNonNetworked(item);
}
//#else

void GiveItem(gentity_t* ent, itemInstance_t item, qboolean ACI) 
{
	// Basic checks
	/*if(!item.id || !item.id->itemID) {
		return;
	}

	GiveItemNonNetworked(ent, item);

	// Network data to the client
	if(ACI) 
	{
		SendItemPacket(IPT_ADDACI, ent, &item, 0, 0);
	} else {
		SendItemPacket(IPT_ADD, ent, &item, 0, 0);
	}*/
}
//#endif

/*
====================
GiveItemNonNetworked

Should only be used with regards to trade.
On the server, this is also internally called from BG_GiveItem.
====================
*/
//#ifdef _GAME
void GiveItemNonNetworked(gentity_t* ent, itemInstance_t item) 
{
	// Basic checks
	if (!item.id || !item.id->itemID) {
		return;
	}

	// Fill any incomplete stacks.
	int nItemID = item.id->itemID;
	int nMaxStack = item.id->maxStack;
	if (nMaxStack > 0) {
		for (int nStack = FirstStack(*ent->inventory, nItemID); nStack != -1; nStack = NextStack(*ent->inventory, nItemID, nStack)) {
			if ((*ent->inventory)[nStack].quantity < nMaxStack) {
				int diff = nMaxStack - (*ent->inventory)[nStack].quantity;
				if (diff < item.quantity) {
					item.quantity -= diff;
					(*ent->inventory)[nStack].quantity += diff;
				}
				else {
					(*ent->inventory)[nStack].quantity += item.quantity;
					item.quantity = 0;
					break;
				}
			}
		}
		if (item.quantity <= 0) {
			return;
		}
	}

	// Add the new item stack to the inventory
	ent->inventory->push_back(item);
}//elif _CGAME
//#ifdef _CGAME
void GiveItemNonNetworked(itemInstance_t item)
{
	centity_t* cent = &cg_entities[0];
	// Basic checks
	if (!item.id || !item.id->itemID) {
		return;
	}

	// The player cannot actually acquire ammo items.
	if (item.id->itemType == ITEM_AMMO)
	{
		return;
	}

	// Fill any incomplete stacks.
	int nItemID = item.id->itemID;
	int nMaxStack = item.id->maxStack;
	if (nMaxStack > 0) {
		for (int nStack = FirstStack(*cent->gent->inventory, nItemID); nStack != -1; nStack = NextStack(*cent->gent->inventory, nItemID, nStack)) {
			if ((*cent->gent->inventory)[nStack].quantity < nMaxStack) {
				int diff = nMaxStack - (*cent->gent->inventory)[nStack].quantity;
				if (diff < item.quantity) {
					item.quantity -= diff;
					(*cent->gent->inventory)[nStack].quantity += diff;
				}
				else {
					(*cent->gent->inventory)[nStack].quantity += item.quantity;
					item.quantity = 0;
					break;
				}
			}
		}
		if (item.quantity <= 0) {
			return;
		}
	}
	cent->gent->inventory->push_back(item);

	// If this item is a weapon, which is not already in our ACI, and the ACI is not full, add it.
	if(item.id->itemType == ITEM_WEAPON) {
		bool bInACIAlready = false;
		int nFreeACISlot = -1;
		for(int i = 0; i < MAX_ACI_SLOTS; i++) {
			if(cg.playerACI[i] == -1 && nFreeACISlot == -1) {
				nFreeACISlot = i;
				continue;
			} else if(cg.playerACI[i] == -1) {
				continue;
			} else if(cg.playerACI[i] >= cent->gent->inventory->size()) {
				// This item in our ACI is invalid, remove it
				cg.playerACI[i] = -1;
				continue;
			}
			if(!Q_stricmp((*cent->gent->inventory)[cg.playerACI[i]].id->internalName, item.id->internalName)) {
				bInACIAlready = true; 
			}
			if(bInACIAlready && nFreeACISlot >= 0) { // already found everything we need to know, just die
				break;
			}
		}

		if (!bInACIAlready && nFreeACISlot != -1) 
		{
			cg.playerACI[nFreeACISlot] = cent->gent->inventory->size() - 1;
		}
	}
}
//#endif

/*
====================
RemoveItemStack

Server tells client to remove item as well.
====================
*/

//#ifdef _GAME
extern void UnequipItem(gentity_t *ent, int iNum);
void RemoveItemStack(gentity_t* ent, int itemStack) {
	itemInstance_t item = (*ent->inventory)[itemStack];

	// If it's something we have equipped, remove it
	if (item.equipped)
	{
		UnequipItem(ent, itemStack);
	}

	ent->inventory->erase(ent->inventory->begin() + itemStack);

	SendItemPacket(IPT_REM, ent, nullptr, itemStack, 0);
}
//#else
//#ifdef _CGAME
void RemoveItemStack(int itemStack) 
{
	centity_t* cent = &cg_entities[0];
	cent->gent->inventory->erase(cent->gent->inventory->begin() + itemStack);
}
//#endif

/*
====================
RemoveItem

Server does NOT tell client to remove item
====================
*/


void RemoveItemNonNetworked(gentity_t* ent, itemInstance_t item) 
{
	itemData_t* pItemData = item.id;
	int quantity = item.quantity;

	for(auto it = ent->inventory->begin(); it != ent->inventory->end() && quantity > 0; ++it) {
		if (pItemData->itemID == it->id->itemID) {
			// This item ID matches
			if (quantity >= it->quantity) {
				quantity -= it->quantity;
				it = ent->inventory->erase(it);
			}
			else {
				it->quantity -= quantity;
				break;
			}
		}
	}
}
//#elif _CGAME
//#ifdef _CGAME
void RemoveItemNonNetworked(itemInstance_t item) 
{
	centity_t* cent = &cg_entities[0];
	itemData_t* pItemData = item.id;
	int quantity = item.quantity;
	for (auto it = cent->gent->inventory->begin(); it != cent->gent->inventory->end() && quantity > 0; ++it) {
		if (pItemData->itemID == it->id->itemID) {
			// This item ID matches
			if (quantity >= it->quantity) {
				quantity -= it->quantity;
				it = cent->gent->inventory->erase(it);
			}
			else {
				it->quantity -= quantity;
				break;
			}
		}
	}
}
//#endif

/*
====================
ChangeItemStackQuantity

Server tells client to change quantity as well.
====================
*/

void ChangeItemStackQuantity(gentity_t* ent, int itemStack, int newQuantity) {
	if(newQuantity <= 0) {
		RemoveItemStack(ent, itemStack);
		return;
	}
	(*ent->inventory)[itemStack].quantity = newQuantity;

	SendItemPacket(IPT_QUANT, ent, nullptr, itemStack, newQuantity);
}
//#ifdef _CGAME
void ChangeItemStackQuantity(int itemStack, int newQuantity) 
{
	centity_t* cent = &cg_entities[0];
	if (newQuantity == 0) {
		RemoveItemStack(itemStack);
		return;
	}
	(*cent->gent->inventory)[itemStack].quantity = newQuantity;
}
//#endif

/*
====================
AdjustItemStackQuantity

Adds/subtracts item stack quantity
====================
*/


void AdjustItemStackQuantity(gentity_t* ent, int itemStack, int adjustment) 
{
	if(itemStack < 0 || itemStack >= ent->inventory->size()) {
		gi.Printf("client %i tried to change stack quantity of invalid slot %i!!\n", ent->s.number);
		return;
	}
	ChangeItemStackQuantity(ent, itemStack, (*ent->inventory)[itemStack].quantity + adjustment);
}
//#ifdef _CGAME
void AdjustItemStackQuantity(int itemStack, int adjustment) 
{
	centity_t* cent = &cg_entities[0];

	ChangeItemStackQuantity(itemStack, (*cent->gent->inventory)[itemStack].quantity + adjustment);
}
//#endif

/*
====================
AddItemToACI
====================
*/

//#ifdef _CGAME
void AddItemToACI(int itemStackNum, int aciSlot) 
{
	// Basic checks
	centity_t* cent = &cg_entities[0];
	if (itemStackNum >= cent->gent->inventory->size()) {
		return;
	}

	int desiredItemID = (*cent->gent->inventory)[itemStackNum].id->itemID;
	if (desiredItemID == 0) 
	{
		return;
	}

	// Automatically unassign slots which have the same item ID as us
	// NOTENOTE: remember to remove this if we want to have items with different qualities, stats, etc
	for (int i = 0; i < MAX_ACI_SLOTS; i++) 
	{
		int thisSlot = cg.playerACI[i];
		if (thisSlot == -1) {
			continue;
		}
		if (thisSlot >= cent->gent->inventory->size()) 
		{
			cg.playerACI[i] = -1;
			continue;
		}
		if (!(*cent->gent->inventory)[thisSlot].id) 
		{
			continue;
		}
		if ((*cent->gent->inventory)[thisSlot].id->itemID == desiredItemID) 
		{
			cg.playerACI[i] = -1;
		}
	}

	// If the ACI slot is -1, assign it to the first available slot.
	if (aciSlot == -1) {
		for (int i = 0; i < MAX_ACI_SLOTS; i++) 
		{
			if (cg.playerACI[i] == -1) {
				cg.playerACI[i] = itemStackNum;
				return;
			}
		}
		return;	// Couldn't find an available slot
	}
	cg.playerACI[aciSlot] = itemStackNum;
}
//#endif

/*
====================
ConsumeItem
====================
*/


//extern void GLua_ConsumeItem(gentity_t* consumer, itemInstance_t* item);
qboolean ConsumeItem(gentity_t* ent, int itemStackNum) {
	itemInstance_t* item;
	int consumeAmount;

	if (itemStackNum < 0 || itemStackNum >= ent->inventory->size()) 
	{
		// Invalid inventory ID
		return qfalse;
	}

	item = &(*ent->inventory)[itemStackNum];
	if (item->id->itemType != ITEM_CONSUMABLE) {
		// Not a consumable item
		return qfalse;
	}

	consumeAmount = item->id->consumableData.consumeAmount;
	if (consumeAmount > item->quantity) {
		// Not enough quantity to consume this item
		return qfalse;
	}

	//GLua_ConsumeItem(ent, item);
	ChangeItemStackQuantity(ent, itemStackNum, item->quantity - consumeAmount);
	return qtrue;
}

/*
====================
LoadDefaultWeaponItems

Creates weightless weapon items if no item files exist for the weapons.
====================
*/

void LoadDefaultWeaponItems ( void )
{
	int i = 0;
    int end = 0;// NumberOfLoadedWeapons();  QQQ FOR MOMENT WE CANNOT USE!! FUCKED VS! 
    qboolean weaponHasItem[MAX_WEAPON_TABLE_SIZE] = { qfalse };
    
    for ( i = 1; i < MAX_ITEM_TABLE_SIZE; i++ )
    {
		itemData_t *item = &itemLookupTable[i];
        if ( !item->itemID )
        {
            continue;
        }
        
        if ( item->itemType != ITEM_WEAPON )
        {
            continue;
        }
        
		weaponHasItem[item->weaponData.varID] = qtrue;
    }
    
    for ( i = 0; i < end; i++ )
    {
        weaponData_t *weaponData;
        int weapon, variation;
        int itemID;
		itemData_t item;
        
        if ( weaponHasItem[i] )
        {
            continue;
        }
        
        if ( !GetWeaponByIndex (i, &weapon, &variation) )
        {
            break;
        }
        
        memset (&item, 0, sizeof (item));
        weaponData = GetWeaponData (weapon, variation);
        Q_strncpyz (item.displayName, weaponData->displayName, sizeof (item.displayName));
        itemID = GetNextFreeItemSlot();
        if ( !itemID )
        {
            Com_Printf ("Ran out of item space for weapons.\n");
            break;
        }
        
        item.itemID = itemID;
        item.itemType = ITEM_WEAPON;
		item.weaponData.weapon = weapon;
		item.weaponData.variation = variation;
		item.weaponData.varID = i;
#ifdef _CGAME
		Q_strncpyz(item.visuals.itemIcon, weaponData->visuals.icon, MAX_QPATH);
#endif
        
        itemLookupTable[itemID] = item;
    }
}

/*
====================
LoadItem
====================
*/
int lastUsedItemID = 0;
extern stringID_table_t WPTable[]; // From bg_saga.c
static bool LoadItem(const char *itemFilePath, itemData_t *itemData)
{
	cJSON *json = NULL;
	cJSON *jsonNode = NULL;

	char error[MAX_STRING_CHARS];
	const char *str = NULL;
	int	item;

	char itemFileData[MAX_ITEM_FILE_LENGTH];
	fileHandle_t f;
	int fileLen = gi.FS_FOpenFile(itemFilePath, &f, FS_READ);

	if (!f || fileLen == -1)
	{
		Com_Printf(S_COLOR_RED "Unreadable or empty item file %s\n", itemFilePath);
		return false;
	}

	if ((fileLen + 1) >= MAX_ITEM_FILE_LENGTH)
	{
		gi.FS_FCloseFile(f);
		Com_Printf(S_COLOR_RED "%s item file too large\n", itemFilePath);
		return false;
	}

	gi.FS_Read(&itemFileData, fileLen, f);
	itemFileData[fileLen] = '\0';

	gi.FS_FCloseFile(f);

	json = cJSON_ParsePooled(itemFileData, error, sizeof(error));
	if (json == NULL)
	{
		Com_Printf(S_COLOR_RED "%s: %s\n", itemFilePath, error);
		return false;
	}

	//Basic Item Information
	jsonNode = cJSON_GetObjectItem(json, "name");
	str = cJSON_ToString(jsonNode);
	strcpy(itemData->displayName, str);

	jsonNode = cJSON_GetObjectItem(json, "internal");
	str = cJSON_ToString(jsonNode);
	strcpy(itemData->internalName, str);

	jsonNode = cJSON_GetObjectItem(json, "id");
	if (jsonNode) {
		item = cJSON_ToNumber(jsonNode);
		itemData->itemID = item;
	}
	else 
	{
		itemData->itemID = lastUsedItemID;
#ifdef _DEBUG
		Com_Printf("^3DEBUG: autoassigning item ID %i\n", itemData->itemID);
#endif
	}

	lastUsedItemID++;

	// Visuals
#ifdef _CGAME
	jsonNode = cJSON_GetObjectItem(json, "itemIcon");
	Q_strncpyz(itemData->visuals.itemIcon, cJSON_ToStringOpt(jsonNode, "gfx/Item_Icons/default.tga"), MAX_QPATH);
#endif

	jsonNode = cJSON_GetObjectItem(json, "itemtype");
	str = cJSON_ToString(jsonNode);
	if (Q_stricmp(str, "armor") == 0)
		itemData->itemType = ITEM_ARMOR;
	else if (Q_stricmp(str, "weapon") == 0)
		itemData->itemType = ITEM_WEAPON;
	else if (Q_stricmp(str, "clothing") == 0)
		itemData->itemType = ITEM_CLOTHING;
	else if (Q_stricmp(str, "consumable") == 0)
		itemData->itemType = ITEM_CONSUMABLE;
	else if (Q_stricmp(str, "shield") == 0)
		itemData->itemType = ITEM_SHIELD;
	else if (Q_stricmp(str, "jetpack") == 0)
		itemData->itemType = ITEM_JETPACK;
	else if (Q_stricmp(str, "ammo") == 0)
		itemData->itemType = ITEM_AMMO;
	else
		itemData->itemType = ITEM_UNKNOWN;

	jsonNode = cJSON_GetObjectItem(json, "weight");
	item = cJSON_ToNumber(jsonNode);
	itemData->weight = item;

	jsonNode = cJSON_GetObjectItem(json, "cost");
	item = cJSON_ToIntegerOpt(jsonNode, 100);
	itemData->baseCost = item;

	jsonNode = cJSON_GetObjectItem(json, "maxStack");
	item = cJSON_ToIntegerOpt(jsonNode, 1);
	itemData->maxStack = item;

	//Equipment Info
	if (itemData->itemType == ITEM_WEAPON) {
		//This is a weapon. Grab the data.
		jsonNode = cJSON_GetObjectItem(json, "weapon");
		str = cJSON_ToString(jsonNode);
		if (!atoi(str))
		{
			itemData->weaponData.weapon = GetIDForString(WPTable, str);
		}
		else
		{
			itemData->weaponData.weapon = atoi(str);
		}

		jsonNode = cJSON_GetObjectItem(json, "variation");
		item = cJSON_ToNumber(jsonNode);
		itemData->weaponData.variation = item;

		itemData->weaponData.varID = GetWeaponIndex(itemData->weaponData.weapon, itemData->weaponData.variation);
	}
	/*else if (itemData->itemType == ITEM_AMMO) 
	{
		jsonNode = cJSON_GetObjectItem(json, "ammoclass");
		Q_strncpyz(itemData->ammoData.ref, cJSON_ToStringOpt(jsonNode, ""), sizeof(itemData->ammoData.ref));
		itemData->ammoData.ammoIndex = GetAmmo(itemData->ammoData.ref) - ammoTable;

		jsonNode = cJSON_GetObjectItem(json, "ammoquantity");
		itemData->ammoData.quantity = cJSON_ToIntegerOpt(jsonNode, 1);
	}*/
	/*else if (itemData->itemType == ITEM_ARMOR) 
	{
		jsonNode = cJSON_GetObjectItem(json, "armor");
		
		Q_strncpyz(itemData->armorData.ref, cJSON_ToStringOpt(jsonNode, ""), sizeof(itemData->armorData.ref));
		
		itemData->armorData.pArm = FindArmorByName(itemData->armorData.ref);
		// FOR NOW WE HAVE NOT ARMOR! QQQ
	}*/ ///WE ADD LATERS.. 
	else if (itemData->itemType == ITEM_CONSUMABLE) {
		// consumeScript controls the script that gets run when we consume the item
		jsonNode = cJSON_GetObjectItem(json, "consumeScript");
		Q_strncpyz(itemData->consumableData.consumeScript, cJSON_ToStringOpt(jsonNode, "noscript"), MAX_CONSUMABLE_SCRIPTNAME);

		// consumeAmount controls the amount of items in the stack that get consumed
		jsonNode = cJSON_GetObjectItem(json, "consumeAmount");
		itemData->consumableData.consumeAmount = cJSON_ToIntegerOpt(jsonNode, 1);
	}
	else if (itemData->itemType == ITEM_SHIELD) {
		memset(&itemData->shieldData, 0, sizeof(itemData->shieldData));

		jsonNode = cJSON_GetObjectItem(json, "capacity");
		itemData->shieldData.capacity = cJSON_ToIntegerOpt(jsonNode, SHIELD_DEFAULT_CAPACITY);

		jsonNode = cJSON_GetObjectItem(json, "cooldown");
		itemData->shieldData.cooldown = cJSON_ToIntegerOpt(jsonNode, SHIELD_DEFAULT_COOLDOWN);

		jsonNode = cJSON_GetObjectItem(json, "regenrate");
		itemData->shieldData.regenrate = cJSON_ToIntegerOpt(jsonNode, SHIELD_DEFAULT_REGEN);

		jsonNode = cJSON_GetObjectItem(json, "rechargeSoundEffect");
		if (jsonNode) {
			Q_strncpyz(itemData->shieldData.rechargeSoundEffect, cJSON_ToString(jsonNode), MAX_QPATH);
		}

		jsonNode = cJSON_GetObjectItem(json, "brokenSoundEffect");
		if (jsonNode) {
			Q_strncpyz(itemData->shieldData.brokenSoundEffect, cJSON_ToString(jsonNode), MAX_QPATH);
		}

		jsonNode = cJSON_GetObjectItem(json, "equippedSoundEffect");
		if (jsonNode) {
			Q_strncpyz(itemData->shieldData.equippedSoundEffect, cJSON_ToString(jsonNode), MAX_QPATH);
		}

		jsonNode = cJSON_GetObjectItem(json, "chargedSoundEffect");
		if (jsonNode) {
			Q_strncpyz(itemData->shieldData.chargedSoundEffect, cJSON_ToString(jsonNode), MAX_QPATH);
		}

		jsonNode = cJSON_GetObjectItem(json, "malfunctionSoundEffect");
		if (jsonNode) {
			Q_strncpyz(itemData->shieldData.malfunctionSoundEffect, cJSON_ToString(jsonNode), MAX_QPATH);
		}
	}
	/*else if (itemData->itemType == ITEM_JETPACK) 
	{
		jsonNode = cJSON_GetObjectItem(json, "jetpack");
		Q_strncpyz(itemData->jetpackData.ref, cJSON_ToString(jsonNode), sizeof(itemData->jetpackData.ref));

		itemData->jetpackData.pJetpackData = FindJetpackByName(itemData->jetpackData.ref);
		if (itemData->jetpackData.pJetpackData == nullptr) {
			Com_Printf(S_COLOR_YELLOW "WARNING: %s is a jetpack, but doesn't have a valid reference to a .jet file!\n", itemFilePath);
		} 
	}*/ /// QQQ WE ADD JETPACKS LATERS... 

	cJSON_Delete(json);

	return qtrue;
}

/*
====================
LoadItems

Loads each individual item file
====================
*/
static bool LoadItems(void)
{
	int i, j;
	char itemFiles[8192];
	int numFiles = gi.FS_GetFileList("ext_data/items/", ".itm", itemFiles, sizeof(itemFiles));
	const char *itemFile = itemFiles;
	int successful = 0;
	int failed = 0;

	lastUsedItemID = 1;

	Com_Printf("------- Constructing Item Table -------\n");

	for (i = 0; i < numFiles; i++)
	{
		itemData_t dummy;
		if (!LoadItem(va("ext_data/items/%s", itemFile), &dummy))
		{
			failed++;
			itemFile += strlen(itemFile) + 1;
			continue;
		}

		if (dummy.itemID > 0 && dummy.itemID < MAX_ITEM_TABLE_SIZE)
		{
			successful++;
		}
		else
		{
			failed++;
			continue;
		}

		if (dummy.itemID >= MAX_ITEM_TABLE_SIZE){
			Com_Printf(S_COLOR_RED "ERROR: item ID out of range \nItem: ext_data/items/%s, ID: %d\n", itemFile, dummy.itemID);
			Com_Printf(S_COLOR_RED "Attempting to correct this now...\n");
			for (j = 1; j < MAX_ITEM_TABLE_SIZE; j++)
			{
				if (!itemLookupTable[j].itemID)
				{
					dummy.itemID = j;
					Com_Printf(S_COLOR_YELLOW "New itemID: %d\n", dummy.itemID);
					break;
				}
			}
		}

		if (itemLookupTable[dummy.itemID].itemID)
			Com_Printf(S_COLOR_YELLOW "Duplicate item id: %d\n", dummy.itemID);
		itemLookupTable[dummy.itemID] = dummy;

		itemFile += strlen(itemFile) + 1;
	}
	Com_Printf("Item Table: %d successful, %d failed.\n", successful, failed);
	Com_Printf("-------------------------------------\n");

	return successful > 0;
}

/*
====================
InitItems

Starts the loading process
====================
*/
void InitItems() 
{
	itemLookupTable = (itemData_t*)malloc(sizeof(itemData_t) * MAX_ITEM_TABLE_SIZE);
	if (itemLookupTable == nullptr)
	{
		Com_Error(ERR_DROP, "could not allocate memory for items...");
		return;
	}

	memset(itemLookupTable, 0, sizeof(itemData_t) * MAX_ITEM_TABLE_SIZE);

	if (LoadItems() == false) {
		Com_Error(ERR_DROP, "could not load items...");
		return;
	}
}

/*
=====================
ShutdownItems

Frees the memory associated with items
=====================
*/
void ShutdownItems() 
{
	if (itemLookupTable != nullptr)
	{
		free(itemLookupTable);
	}
}
