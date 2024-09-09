*
	JSON framework for C

	Copyright (c) 2010 Lourens "BobaFett" Elzinga

	Based on cJSON by Dave Gamble

    Changes:

  * Cleaned up code formatting to make it much more readable
  * Renamed print to serialize
  * Serializer now uses a stringbuilder instead of a tremendous amount of mallocs
  * Added usage of dynamic arrays and hashtables to drastically speed up lookup times for arrays and objects
  * JSON Type #defines are now internal
  * Array and Object functions now contain sanity checks (to ensure an array/object is being used)
  * cJSON struct is now internal, and is typedef'd as void
  * Replaced all instances of sprintf by snprintf to remove the risk of overflows (#defined as sprintf_s for windows compiles)
  * Added functions to obtain item values as a specific type (with default value in case of an error or incompatible type)
  * Added functions to determine the type of an item
  * Removed 'references'. They are unsafe and not very useful.
  * Added item duplication
  * Added new create functions (for booleans and integers)
  * The string serializer now supports unprintable characters ( < ANSI 32 without \x equivalent )
  * Deleting linked nodes is no longer possible
  * Added a safe version of cJSON_Delete which also clears the pointer if deleting was successful
  * Added function to insert items in arrays
  * Added function to swap 2 items in arrays
  * Added functions to clear arrays and objects
  * Added extended lookup, to allow retreival of deeply nested items in 1 call
  * Implemented iterator functions to iterate over objects (and arrays)
  * The parser now uses a stream buffer to track the current position
  * Errors during parsing now produce a proper error instead of silently failing
  * Fixed memory leaks in parser
  * The parser supports C-style comments (both single-line and block comments)
  * The string parser and serializer is now properly UTF-8 aware, and will encode and decode characters properly (non-ANSI chars are converted to '?')
  * The parser now supports memory pooling, which dramatically increases parse and free time (at the cost of the result being read-only)

*/


#ifndef cJSON__h
#define cJSON__h

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __cJSON_INTERNAL
typedef void cJSON;
typedef void cJSONStream;
#endif

typedef struct cJSON_Hooks {
      void *(*malloc_fn)(size_t sz);
	  void *(*realloc_fn)(void *ptr, size_t sz);
      void (*free_fn)(void *ptr);
} cJSON_Hooks;

// Supply malloc, realloc and free functions to cJSON
extern void cJSON_InitHooks(cJSON_Hooks* hooks);

// Supply a block of JSON, and this returns a cJSON object you can interrogate. Call cJSON_Delete when finished.
// cJSON_ParsePooled utilizes a memory pool. This makes the parsing and freeing much faster, but also makes the structure read-only.
extern cJSON *cJSON_Parse(const char *data, char *error, size_t errorlen);
extern cJSON *cJSON_ParsePooled(const char *data, char *error, size_t errorlen);

// Serialize a cJSON entity to text for transfer/storage. Optionally with formatting to make it more human-readable. Free the const char* when finished.
extern const char *cJSON_Serialize(cJSON *item, int format);

// Delete a cJSON entity and all subentities.
extern void   cJSON_Delete(cJSON *c);

// Delete a cJSON entity and all subentities, and clear the pointer to it as well (if successful)
extern void	  cJSON_SafeDelete(cJSON **c);

// Returns the number of items in an arry (or object).
extern int	  cJSON_GetArraySize(cJSON *arry);

// Retrieve item number "item" from arry "arry". Returns NULL if unsuccessful.
extern cJSON *cJSON_GetArrayItem(cJSON *arry, int item);

// Get item "string" from object. Case insensitive.
extern cJSON *cJSON_GetObjectItem(cJSON *object, const char *string);

// Extended Item Lookup.
// Allows complex nested lookups
// 
// Format example:
// sub[5].myvar
// [2]
// test.sub[24].something[4].var

extern cJSON *cJSON_Lookup(cJSON *item, const char *path);

// Iterator functions
// Example usage:
//
// for (item = cJSON_GetFirstItem(object); item; item = cJSON_GetNextItem(item)) { ... }
//
extern cJSON *cJSON_GetFirstItem(cJSON *object);
extern cJSON *cJSON_GetNextItem(cJSON *item);

// Returns the key of an item (only valid for items that are part of an object)
extern const char * cJSON_GetItemKey(cJSON *item);

// Duplicate a cJSON item and all subentities
extern cJSON *cJSON_DuplicateItem(cJSON *item);

// These calls create a cJSON item of the appropriate type.
extern cJSON *cJSON_CreateNull();
extern cJSON *cJSON_CreateTrue();
extern cJSON *cJSON_CreateFalse();
extern cJSON *cJSON_CreateBoolean(int boolean);
extern cJSON *cJSON_CreateNumber(double num);
extern cJSON *cJSON_CreateInteger(int num);
extern cJSON *cJSON_CreateString(const char *string);
extern cJSON *cJSON_CreateArray();
extern cJSON *cJSON_CreateObject();

// These utilities create an array of count items.
extern cJSON *cJSON_CreateIntArray(int count, int *number);
extern cJSON *cJSON_CreateFloatArray(int count, float *numbers);
extern cJSON *cJSON_CreateDoubleArray(int count, double *numbers);
extern cJSON *cJSON_CreateStringArray(int count, const char **strings);

// Same as above, but uses varargs instead of arrays
extern cJSON *cJSON_CreateIntArrayv(int count, ...);
extern cJSON *cJSON_CreateFloatArrayv(int count, ...);
extern cJSON *cJSON_CreateDoubleArrayv(int count, ...);
extern cJSON *cJSON_CreateStringArrayv(int count, ...);

// Read an array and store the values.
extern int cJSON_ReadIntArray(cJSON *arry, int count, int *number);
extern int cJSON_ReadFloatArray(cJSON *arry, int count, float *numbers);
extern int cJSON_ReadDoubleArray(cJSON *arry, int count, double *numbers);
extern int cJSON_ReadStringArray(cJSON *arry, int count, const char **strings);

// Same as above, but uses varargs instead of arrays
extern int cJSON_ReadIntArrayv(cJSON *arry, int count, ...);
extern int cJSON_ReadFloatArrayv(cJSON *arry, int count, ...);
extern int cJSON_ReadDoubleArrayv(cJSON *arry, int count, ...);
extern int cJSON_ReadStringArrayv(cJSON *arry, int count, ...);

// Append item to the specified array/object.
extern void cJSON_AddItemToArray(cJSON *arry, cJSON *item);
extern void cJSON_InsertItemInArray(cJSON *arry, cJSON *item, int before);
extern void	cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);

// Remove/Detatch items from Arrays/Objects.
extern cJSON *cJSON_DetachItemFromArray(cJSON *arry, int which);
extern void   cJSON_DeleteItemFromArray(cJSON *arry, int which);
extern cJSON *cJSON_DetachItemFromObject(cJSON *object, const char *string);
extern void   cJSON_DeleteItemFromObject(cJSON *object, const char *string);

// Clear all items from Arrays/Objects
extern void cJSON_ClearItemsFromObject(cJSON *object);
extern void cJSON_ClearItemsFromArray(cJSON *arry);
	
// Update array items.
extern void cJSON_ReplaceItemInArray(cJSON *arry, int which, cJSON *newitem);
extern void cJSON_SwapItemsInArray(cJSON *arry, int item1, int item2);
extern void cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem);

// Convenience functions: Add item to object
#define cJSON_AddNullToObject(object, name)			cJSON_AddItemToObject(object, name, cJSON_CreateNull())
#define cJSON_AddTrueToObject(object, name)			cJSON_AddItemToObject(object, name, cJSON_CreateTrue())
#define cJSON_AddFalseToObject(object, name)		cJSON_AddItemToObject(object, name, cJSON_CreateFalse())
#define cJSON_AddBooleanToObject(object, name, b)	cJSON_AddItemToObject(object, name, cJSON_CreateBoolean(b))
#define cJSON_AddIntegerToObject(object, name, i)	cJSON_AddItemToObject(object, name, cJSON_CreateInteger(i))
#define cJSON_AddNumberToObject(object, name, n)	cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
#define cJSON_AddStringToObject(object, name, s)	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))

// Convenience functions: Add item to array
#define cJSON_AddNullToArray(arry)			cJSON_AddItemToArray(arry, cJSON_CreateNull())
#define cJSON_AddTrueToArray(arry)			cJSON_AddItemToArray(arry, cJSON_CreateTrue())
#define cJSON_AddFalseToArray(arry)			cJSON_AddItemToArray(arry, cJSON_CreateFalse())
#define cJSON_AddBooleanToArray(arry, b)	cJSON_AddItemToArray(arry, cJSON_CreateBoolean(b))
#define cJSON_AddIntegerToArray(arry, i)	cJSON_AddItemToArray(arry, cJSON_CreateInteger(i))
#define cJSON_AddNumberToArray(arry, n)		cJSON_AddItemToArray(arry, cJSON_CreateNumber(n))
#define cJSON_AddStringToArray(arry, s)		cJSON_AddItemToArray(arry, cJSON_CreateString(s))

// Value types
extern int cJSON_IsNULL(cJSON *item);
extern int cJSON_IsTrue(cJSON *item);
extern int cJSON_IsFalse(cJSON *item);
extern int cJSON_IsBoolean(cJSON *item);
extern int cJSON_IsNumber(cJSON *item);
extern int cJSON_IsString(cJSON *item);
extern int cJSON_IsArray(cJSON *item);
extern int cJSON_IsObject(cJSON *item);

// Returns whether or not the cJSON entity is linked (part of an array or object).
// Linked items cannot be deleted directly. Delete the array/object they're part of to delete them.
extern int cJSON_IsLinked(cJSON *item);

// Retrive values. If something goes wrong (wrong type, etc) the return value will be 0.
extern int			cJSON_ToBoolean(cJSON *item);
extern double		cJSON_ToNumber(cJSON *item);
extern int			cJSON_ToInteger(cJSON *item);
extern const char * cJSON_ToString(cJSON *item);

// Retreive values with default value (if they're of a wrong type or nonexistant)
extern int			cJSON_ToBooleanOpt(cJSON *item, int defval);
extern double		cJSON_ToNumberOpt(cJSON *item, double defval);
extern int			cJSON_ToIntegerOpt(cJSON *item, int defval);
extern const char *	cJSON_ToStringOpt(cJSON *item, const char *defval);

// Retrive values directly. No type checking or default values.
extern int			cJSON_ToBooleanRaw(cJSON *item);
extern double		cJSON_ToNumberRaw(cJSON *item);
extern int			cJSON_ToIntegerRaw(cJSON *item);
extern const char * cJSON_ToStringRaw(cJSON *item);

// Set values (and change types) of cJSON entities
extern void cJSON_SetStringValue(cJSON *item, const char *value);
extern void cJSON_SetNumberValue(cJSON *item, double number);
extern void cJSON_SetIntegerValue(cJSON *item, int integer);
extern void cJSON_SetBooleanValue(cJSON *item, int boolean);
extern void cJSON_SetNULLValue(cJSON *item);

// JSON Stream Writer API

extern cJSONStream *cJSON_Stream_New(int maxDepth, int formatted, size_t bufferInitSize, size_t bufferBlockSize);
extern const char *cJSON_Stream_Finalize(cJSONStream *stream);
extern void cJSON_Stream_WriteNull(cJSONStream *stream, const char *key);
extern void cJSON_Stream_WriteTrue(cJSONStream *stream, const char *key);
extern void cJSON_Stream_WriteFalse(cJSONStream *stream, const char *key);
extern void cJSON_Stream_WriteBoolean(cJSONStream *stream, const char *key, int boolean);
extern void cJSON_Stream_WriteString(cJSONStream *stream, const char *key, const char *string);
extern void cJSON_Stream_WriteInteger(cJSONStream *stream, const char *key, int value);
extern void cJSON_Stream_WriteNumber(cJSONStream *stream, const char *key, double value);
extern void cJSON_Stream_BeginArray(cJSONStream *stream, const char *key);
extern void cJSON_Stream_BeginObject(cJSONStream *stream, const char *key);
extern void cJSON_Stream_EndBlock(cJSONStream *stream);


#ifdef __cplusplus
}
#endif

#endif
