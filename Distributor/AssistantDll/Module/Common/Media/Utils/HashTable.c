/**************************************************************************/
/*                                                                        */
/* Copyright (c) 1999-2011 NetDragon Websoft Inc.                         */
/*                    网龙网络有限公司版权所有 1999-2011                  */
/*                                                                        */
/*  PROPRIETARY RIGHTS of NetDragon Websoft Inc. are involved in the      */
/*  subject matter of this material. All manufacturing, reproduction,     */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  本软件文档资料是网龙网络有限公司版权所有的合法资产，任何人士阅读和使  */
/*  用本资料必须获得相应的书面授权,承担保密责任和接受相应的法律约束。     */
/*                                                                        */
/**************************************************************************/

#include "HashTable.h"

typedef struct __nd_hash_table
{
	ND_INT32             size;
	ND_INT32             mod;
	ND_UINT32            mask;
	ND_INT32             nnodes;
	ND_INT32             noccupied;  /* nnodes + tombstones */

	void *			*keys;
	ND_UINT32           *hashes;
	void *			*values;

	ND_HashFunc_CB        hash_func;
	ND_EqualFunc_CB       key_equal_func;
	ND_INT32             ref_count;

	ND_DestroyNotify_CB   key_destroy_func;
	ND_DestroyNotify_CB   value_destroy_func;
}ND_HashTable;

typedef struct _nd_hash_table_iter
{
	void *	dummy1;
	void *	dummy2;
	void *	dummy3;
	ND_INT	dummy4;
	ND_BOOL	dummy5;
	void *	dummy6;
}ND_HashTableIter;

typedef struct
{
	ND_HashTable  *hash_table;
	void *     dummy1;
	void *     dummy2;
	ND_INT          position;
	ND_BOOL     dummy3;
	ND_INT          version;
}RealIter;

typedef struct _GList GList;
struct _GList
{
	void * data;
	GList *next;
	GList *prev;
};

#define HASH_TABLE_MIN_SHIFT 3  /* 1 << 3 == 8 buckets */

#define G_UNLIKELY(expr) (expr)
#define UNUSED_HASH_VALUE 0
#define TOMBSTONE_HASH_VALUE 1
#define HASH_IS_UNUSED(h_) ((h_) == UNUSED_HASH_VALUE)
#define HASH_IS_TOMBSTONE(h_) ((h_) == TOMBSTONE_HASH_VALUE)
#define HASH_IS_REAL(h_) ((h_) >= 2)

/* Each table size has an associated prime modulo (the first prime
* lower than the table size) used to find the initial bucket. Probing
* then works modulo 2^n. The prime modulo is necessary to get a
* good distribution with poor hash functions. */
static const ND_INT prime_mod [] =
{
	1,          /* For 1 << 0 */
	2,
	3,
	7,
	13,
	31,
	61,
	127,
	251,
	509,
	1021,
	2039,
	4093,
	8191,
	16381,
	32749,
	65521,      /* For 1 << 16 */
	131071,
	262139,
	524287,
	1048573,
	2097143,
	4194301,
	8388593,
	16777213,
	33554393,
	67108859,
	134217689,
	268435399,
	536870909,
	1073741789,
	2147483647  /* For 1 << 31 */
};

#define MAX(a,b) ((a) > (b) ? (a) : (b))

static void __remove_node( ND_HashTable * hash_table, ND_INT i, ND_BOOL notify );
static void __maybe_resize( ND_HashTable *hash_table );
//#################################################################################################
//##################################### 内部函数开始 ##############################################
//#################################################################################################

GList * __list_prepend( GList * list, void * data )
{
	GList * new_list;
  
	new_list = (GList *)malloc( sizeof( GList ) );
	new_list->data = data;
	new_list->next = list;
  
	if( list )
    {
		new_list->prev = list->prev;
		if( list->prev )
			list->prev->next = new_list;
		list->prev = new_list;
	}
	else
		new_list->prev = NULL;
  
	return new_list;
}

static void __set_shift( ND_HashTable *hash_table, ND_INT shift )
{
	ND_INT i;
	ND_UINT32 mask = 0;

	hash_table->size = 1 << shift;
	hash_table->mod  = prime_mod [shift];

	for( i = 0; i < shift; i++ )
    {
		mask <<= 1;
		mask |= 1;
	}

	hash_table->mask = mask;
}

static ND_INT __find_closest_shift( ND_INT n )
{
	ND_INT i;

	for( i = 0; n; i++ )
		n >>= 1;

	return i;
}

static void __set_shift_from_size( ND_HashTable *hash_table, ND_INT size )
{
	ND_INT shift;

	shift = __find_closest_shift( size );
	shift = MAX( shift, HASH_TABLE_MIN_SHIFT );

	__set_shift( hash_table, shift );
}

static void * __memdup( void * pSource, ND_UINT32 iSize )
{
	if( iSize > 0 )
	{
		void * p = malloc( iSize );
		if( p )
		{
			memcpy( p, pSource, iSize );
			return p;
		}
	}
	return NULL;
}

/*
 * __lookup_node:
 * @hash_table: our #ND_HashTable
 * @key: the key to lookup against
 * @hash_return: key hash return location
 * Return value: index of the described node
 *
 * Performs a lookup in the hash table, preserving extra information
 * usually needed for insertion.
 *
 * This function first computes the hash value of the key using the
 * user's hash function.
 *
 * If an entry in the table matching @key is found then this function
 * returns the index of that entry in the table, and if not, the
 * index of an unused node (empty or tombstone) where the key can be
 * inserted.
 *
 * The computed hash value is returned in the variable pointed to
 * by @hash_return. This is to save insertions from having to compute
 * the hash record again for the new record.
 */
static ND_UINT32 __lookup_node( ND_HashTable *hash_table, void * key, ND_UINT32 * hash_return )
{
	ND_UINT32 node_index;
	ND_UINT32 node_hash;
	ND_UINT32 hash_value;
	ND_UINT32 first_tombstone = 0;
	ND_BOOL have_tombstone = /*FALSE*/false;
	ND_UINT32 step = 0;

	hash_value = hash_table->hash_func (key);
	if (G_UNLIKELY (!HASH_IS_REAL (hash_value)))
	  hash_value = 2;

	*hash_return = hash_value;

	node_index = hash_value % hash_table->mod;
	node_hash = hash_table->hashes[node_index];

	while (!HASH_IS_UNUSED (node_hash))
	{
		/* We first check if our full hash values
		 * are equal so we can avoid calling the full-blown
		 * key equality function in most cases.
		 */
		if( node_hash == hash_value )
        {
			void * node_key = hash_table->keys[node_index];

			if( hash_table->key_equal_func )
			{
				if( hash_table->key_equal_func( node_key, key ) )
					return node_index;
			}
			else if( node_key == key )
            {
				return node_index;
            }
        }
		else if( HASH_IS_TOMBSTONE(node_hash) && !have_tombstone )
        {
			first_tombstone = node_index;
			have_tombstone = /*TRUE*/true;
        }

		step++;
		node_index += step;
		node_index &= hash_table->mask;
		node_hash = hash_table->hashes[node_index];
	}

	if( have_tombstone )
		return first_tombstone;

	return node_index;
}

/*
 * __remove_internal:
 * @hash_table: our #ND_HashTable
 * @key: the key to remove
 * @notify: %TRUE if the destroy notify handlers are to be called
 * Return value: %TRUE if a node was found and removed, else %FALSE
 *
 * Implements the common logic for the NDHT_Remove() and
 * NDHT_Steal() functions.
 *
 * Do a lookup of @key and remove it if it is found, calling the
 * destroy notify handlers only if @notify is %TRUE.
 */
static ND_BOOL __remove_internal( ND_HashTable *hash_table, void * key, ND_BOOL notify )
{
	ND_UINT32 node_index;
	ND_UINT32 node_hash;

	if( hash_table == NULL ) return /*FALSE*/false;

	node_index = __lookup_node (hash_table, key, &node_hash);

	if (!HASH_IS_REAL (hash_table->hashes[node_index]))
	  return /*FALSE*/false;;

	__remove_node (hash_table, node_index, notify);
	__maybe_resize (hash_table);

	return /*TRUE*/true;
}

/*
* __remove_all_nodes:
* @hash_table: our #ND_HashTable
* @notify: %TRUE if the destroy notify handlers are to be called
*
* Removes all nodes from the table.  Since this may be a precursor to
* freeing the table entirely, no resize is performed.
*
* If @notify is %TRUE then the destroy notify functions are called
* for the key and value of the hash node.
*/
static void __remove_all_nodes( ND_HashTable * hash_table, ND_BOOL notify )
{
	ND_INT i;
	void * key;
	void * value;

	hash_table->nnodes = 0;
	hash_table->noccupied = 0;

	if (!notify ||
		(hash_table->key_destroy_func == NULL &&
		hash_table->value_destroy_func == NULL))
	{
		memset (hash_table->hashes, 0, hash_table->size * sizeof (ND_UINT));
		memset (hash_table->keys, 0, hash_table->size * sizeof (void *));
		memset (hash_table->values, 0, hash_table->size * sizeof (void *));

		return;
	}

	for (i = 0; i < hash_table->size; i++)
	{
		if (HASH_IS_REAL (hash_table->hashes[i]))
		{
			key = hash_table->keys[i];
			value = hash_table->values[i];

			hash_table->hashes[i] = UNUSED_HASH_VALUE;
			hash_table->keys[i] = NULL;
			hash_table->values[i] = NULL;

			if (hash_table->key_destroy_func != NULL)
				hash_table->key_destroy_func (key);

			if (hash_table->value_destroy_func != NULL)
				hash_table->value_destroy_func (value);
		}
		else if (HASH_IS_TOMBSTONE (hash_table->hashes[i]))
		{
			hash_table->hashes[i] = UNUSED_HASH_VALUE;
		}
	}
}

/*
* __remove_node:
* @hash_table: our #ND_HashTable
* @node: pointer to node to remove
* @notify: %TRUE if the destroy notify handlers are to be called
*
* Removes a node from the hash table and updates the node count.
* The node is replaced by a tombstone. No table resize is performed.
*
* If @notify is %TRUE then the destroy notify functions are called
* for the key and value of the hash node.
*/
static void __remove_node( ND_HashTable * hash_table, ND_INT i, ND_BOOL notify )
{
	void * key;
	void * value;

	key = hash_table->keys[i];
	value = hash_table->values[i];

	/* Erect tombstone */
	hash_table->hashes[i] = TOMBSTONE_HASH_VALUE;

	/* Be GC friendly */
	hash_table->keys[i] = NULL;
	hash_table->values[i] = NULL;

	hash_table->nnodes--;

	if (notify && hash_table->key_destroy_func)
		hash_table->key_destroy_func (key);

	if (notify && hash_table->value_destroy_func)
		hash_table->value_destroy_func (value);

}

static void __iter_remove_or_steal( RealIter *ri, ND_BOOL notify )
{
	if( ri == NULL ) return;
	if( ri->position < 0 ) return;
	if( ri->position >= ri->hash_table->size ) return;

	__remove_node( ri->hash_table, ri->position, notify );

}

/*
* __insert_node:
* @hash_table: our #ND_HashTable
* @node_index: pointer to node to insert/replace
* @key_hash: key hash
* @key: key to replace with
* @value: value to replace with
*
* Inserts a value at @node_index in the hash table and updates it.
*/
static void __insert_node( ND_HashTable * hash_table, ND_UINT node_index, ND_UINT key_hash, void * key, void * value, ND_BOOL keep_new_key )
{
	ND_UINT old_hash;
	void * old_key;
	void * old_value;

	if( G_UNLIKELY( hash_table->keys == hash_table->values && key != value ) )
		hash_table->values = __memdup( hash_table->keys, sizeof (void *) * hash_table->size );

	old_hash = hash_table->hashes[node_index];
	old_key = hash_table->keys[node_index];
	old_value = hash_table->values[node_index];

	if( HASH_IS_REAL( old_hash ) )
	{
		if (keep_new_key)
			hash_table->keys[node_index] = key;
		hash_table->values[node_index] = value;
	}
	else
	{
		hash_table->keys[node_index] = key;
		hash_table->values[node_index] = value;
		hash_table->hashes[node_index] = key_hash;

		hash_table->nnodes++;

		if( HASH_IS_UNUSED (old_hash) )
		{
			/* We replaced an empty node, and not a tombstone */
			hash_table->noccupied++;
			__maybe_resize (hash_table);
		}
	}

	if( HASH_IS_REAL(old_hash) )
	{
		if (hash_table->key_destroy_func)
			hash_table->key_destroy_func (keep_new_key ? old_key : key);
		if (hash_table->value_destroy_func)
			hash_table->value_destroy_func (old_value);
	}
}

/*
 * __insert_internal:
 * @hash_table: our #ND_HashTable
 * @key: the key to insert
 * @value: the value to insert
 * @keep_new_key: if %TRUE and this key already exists in the table
 *   then call the destroy notify function on the old key.  If %FALSE
 *   then call the destroy notify function on the new key.
 *
 * Implements the common logic for the NDHT_Insert() and
 * NDHT_Replace() functions.
 *
 * Do a lookup of @key.  If it is found, replace it with the new
 * @value (and perhaps the new @key).  If it is not found, create a
 * new node.
 */
static void __insert_internal (ND_HashTable *hash_table, void *    key, void *    value, ND_BOOL    keep_new_key)
{
	ND_UINT32 key_hash;
	ND_UINT32 node_index;

	assert( hash_table != NULL );

	node_index = __lookup_node( hash_table, key, &key_hash );

	__insert_node( hash_table, node_index, key_hash, key, value, keep_new_key );
}

/*
* __foreach_remove_or_steal:
* @hash_table: our #ND_HashTable
* @func: the user's callback function
* @user_data: data for @func
* @notify: %TRUE if the destroy notify handlers are to be called
*
* Implements the common logic for g_hash_table_foreach_remove() and
* g_hash_table_foreach_steal().
*
* Iterates over every node in the table, calling @func with the key
* and value of the node (and @user_data).  If @func returns %TRUE the
* node is removed from the table.
*
* If @notify is true then the destroy notify handlers will be called
* for each removed node.
*/
static ND_UINT __foreach_remove_or_steal( ND_HashTable * hash_table, ND_HRFunc_CB func, void * user_data, ND_BOOL notify )
{
	ND_UINT deleted = 0;
	ND_INT i;

	for( i = 0; i < hash_table->size; i++ )
	{
		ND_UINT node_hash = hash_table->hashes[i];
		void * node_key = hash_table->keys[i];
		void * node_value = hash_table->values[i];

		if( HASH_IS_REAL(node_hash) && (*func)(node_key, node_value, user_data) )
		{
			__remove_node( hash_table, i, notify );
			deleted++;
		}
	}
	__maybe_resize (hash_table);
	return deleted;
}

/*
* __resize:
* @hash_table: our #ND_HashTable
*
* Resizes the hash table to the optimal size based on the number of
* nodes currently held.  If you call this function then a resize will
* occur, even if one does not need to occur.  Use
* g_hash_table_maybe_resize() instead.
*
* This function may "resize" the hash table to its current size, with
* the side effect of cleaning up tombstones and otherwise optimizing
* the probe sequences.
*/
static void __resize( ND_HashTable * hash_table )
{
	void **new_keys;
	void **new_values;
	ND_UINT *new_hashes;
	ND_INT old_size;
	ND_INT i;

	old_size = hash_table->size;
	__set_shift_from_size (hash_table, hash_table->nnodes * 2);

	new_keys = malloc( hash_table->size );
	if (hash_table->keys == hash_table->values)
		new_values = new_keys;
	else
		new_values = malloc( hash_table->size );
	new_hashes = malloc( hash_table->size );

	for (i = 0; i < old_size; i++)
	{
		ND_UINT node_hash = hash_table->hashes[i];
		ND_UINT hash_val;
		ND_UINT step = 0;

		if (!HASH_IS_REAL (node_hash))
			continue;

		hash_val = node_hash % hash_table->mod;

		while (!HASH_IS_UNUSED (new_hashes[hash_val]))
		{
			step++;
			hash_val += step;
			hash_val &= hash_table->mask;
		}

		new_hashes[hash_val] = hash_table->hashes[i];
		new_keys[hash_val] = hash_table->keys[i];
		new_values[hash_val] = hash_table->values[i];
	}

	if (hash_table->keys != hash_table->values)
		free( hash_table->values );

	free( hash_table->keys );
	free( hash_table->hashes );

	hash_table->keys = new_keys;
	hash_table->values = new_values;
	hash_table->hashes = new_hashes;

	hash_table->noccupied = hash_table->nnodes;
}

/*
* __maybe_resize:
* @hash_table: our #ND_HashTable
*
* Resizes the hash table, if needed.
*
* Essentially, calls __resize() if the table has strayed
* too far from its ideal size for its number of nodes.
*/
static void __maybe_resize( ND_HashTable *hash_table )
{
	ND_INT noccupied = hash_table->noccupied;
	ND_INT size = hash_table->size;

	if( ( size > hash_table->nnodes * 4 && size > 1 << HASH_TABLE_MIN_SHIFT ) || ( size <= noccupied + ( noccupied / 16 ) ) )
		__resize( hash_table );
}

//#################################################################################################
//##################################### 内部函数结束 ##############################################
//#################################################################################################

//#################################################################################################
//##################################### 外部接口开始 ##############################################
//#################################################################################################
/**
 * NDHT_New:
 * @hash_func: a function to create a hash value from a key.
 *   Hash values are used to determine where keys are stored within the
 *   #ND_HashTable data structure. The ND_DirectHash(), ND_IntHash(),
 *   ND_Int64Hash(), ND_DoubleHash() and ND_StrHash() functions are provided
 *   for some common types of keys.
 *   If hash_func is %NULL, ND_DirectHash() is used.
 * @key_equal_func: a function to check two keys for equality.  This is
 *   used when looking up keys in the #ND_HashTable.  The g_direct_equal(),
 *   ND_IntEqual(), ND_Int64Equal(), ND_DoubleEqual() and ND_StrEqual()
 *   functions are provided for the most common types of keys.
 *   If @key_equal_func is %NULL, keys are compared directly in a similar
 *   fashion to g_direct_equal(), but without the overhead of a function call.
 *
 * Creates a new #ND_HashTable with a reference count of 1.
 *
 * Return value: a new #ND_HashTable.
 **/
HANDLE NDHT_New( ND_HashFunc_CB hash_func, ND_EqualFunc_CB key_equal_func )
{
	return NDHT_NewFull( hash_func, key_equal_func, NULL, NULL );
}

/**
 * NDHT_NewFull:
 * @hash_func: a function to create a hash value from a key.
 * @key_equal_func: a function to check two keys for equality.
 * @key_destroy_func: a function to free the memory allocated for the key
 *   used when removing the entry from the #ND_HashTable or %NULL if you
 *   don't want to supply such a function.
 * @value_destroy_func: a function to free the memory allocated for the
 *   value used when removing the entry from the #ND_HashTable or %NULL if
 *   you don't want to supply such a function.
 *
 * Creates a new #ND_HashTable like NDHT_New() with a reference count
 * of 1 and allows to specify functions to free the memory allocated for the
 * key and value that get called when removing the entry from the #ND_HashTable.
 *
 * Return value: a new #ND_HashTable.
 **/
HANDLE NDHT_NewFull( ND_HashFunc_CB hash_func, ND_EqualFunc_CB key_equal_func, ND_DestroyNotify_CB key_destroy_func,ND_DestroyNotify_CB value_destroy_func )
{
	ND_HashTable *hash_table = NULL;

	hash_table = malloc( sizeof(ND_HashTable) );
	if( hash_table )
	{
		__set_shift( hash_table, HASH_TABLE_MIN_SHIFT );
		hash_table->nnodes             = 0;
		hash_table->noccupied          = 0;
		hash_table->hash_func          = hash_func ? hash_func : ND_DirectHash;
		hash_table->key_equal_func     = key_equal_func;
		hash_table->ref_count          = 1;
		hash_table->key_destroy_func   = key_destroy_func;
		hash_table->value_destroy_func = value_destroy_func;
		hash_table->keys               = malloc( sizeof(void*)*hash_table->size );
		memset( hash_table->keys, 0, sizeof(void*)*hash_table->size );
		hash_table->values             = hash_table->keys;
		hash_table->hashes             = malloc( sizeof(ND_UINT32)*hash_table->size );
		memset( hash_table->hashes, 0, sizeof(ND_UINT32)*hash_table->size );
	}

	return hash_table;
}

/**
 * NDHT_Destroy:
 * @hash_table: a #ND_HashTable.
 *
 * Destroys all keys and values in the #ND_HashTable and decrements its
 * reference count by 1. If keys and/or values are dynamically allocated,
 * you should either free them first or create the #ND_HashTable with destroy
 * notifiers using NDHT_NewFull(). In the latter case the destroy
 * functions you supplied will be called on all keys and values during the
 * destruction phase.
 **/
void NDHT_Destroy( HANDLE hash_table )
{
	assert (hash_table != NULL);

	NDHT_RemoveAll (hash_table);
	NDHT_Unref (hash_table);
}

/**
 * g_hash_table_insert:
 * @hash_table: a #GHashTable.
 * @key: a key to insert.
 * @value: the value to associate with the key.
 *
 * Inserts a new key and value into a #GHashTable.
 *
 * If the key already exists in the #GHashTable its current value is replaced
 * with the new value. If you supplied a @value_destroy_func when creating the
 * #GHashTable, the old value is freed using that function. If you supplied
 * a @key_destroy_func when creating the #GHashTable, the passed key is freed
 * using that function.
 **/
void NDHT_Insert( HANDLE hash_table, void *	key, void * value )
{
	__insert_internal( hash_table, key, value, /*FALSE*/false );
}

/**
 * NDHT_Replace:
 * @hash_table: a #ND_HashTable.
 * @key: a key to insert.
 * @value: the value to associate with the key.
 *
 * Inserts a new key and value into a #ND_HashTable similar to
 * NDHT_Insert(). The difference is that if the key already exists
 * in the #ND_HashTable, it gets replaced by the new key. If you supplied a
 * @value_destroy_func when creating the #ND_HashTable, the old value is freed
 * using that function. If you supplied a @key_destroy_func when creating the
 * #ND_HashTable, the old key is freed using that function.
 **/
void NDHT_Replace (HANDLE hash_table, void * key, void * value )
{
	__insert_internal( hash_table, key, value, /*TRUE*/true );
}

/**
 * NDHT_RemoveAll:
 * @hash_table: a #ND_HashTable
 *
 * Removes all keys and their associated values from a #ND_HashTable.
 *
 * If the #ND_HashTable was created using NDHT_NewFull(), the keys
 * and values are freed using the supplied destroy functions, otherwise you
 * have to make sure that any dynamically allocated values are freed
 * yourself.
 *
 * Since: 2.12
 **/
void NDHT_RemoveAll (HANDLE hash_table)
{
	assert (hash_table != NULL);

	__remove_all_nodes (hash_table, /*TRUE*/true);
	__maybe_resize (hash_table);
}

/**
 * NDHT_Steal:
 * @hash_table: a #ND_HashTable.
 * @key: the key to remove.
 *
 * Removes a key and its associated value from a #ND_HashTable without
 * calling the key and value destroy functions.
 *
 * Return value: %TRUE if the key was found and removed from the #ND_HashTable.
 **/
ND_BOOL NDHT_Steal (HANDLE hash_table, void *  key)
{
	return __remove_internal (hash_table, key, /*FALSE*/false);
}

/**
 * NDHT_StealAll:
 * @hash_table: a #ND_HashTable.
 *
 * Removes all keys and their associated values from a #ND_HashTable
 * without calling the key and value destroy functions.
 *
 * Since: 2.12
 **/
void NDHT_StealAll( HANDLE hash_table )
{
	assert (hash_table != NULL);

	__remove_all_nodes (hash_table, /*FALSE*/false);
	__maybe_resize (hash_table);
}

/**
 * NDHT_Lookup:
 * @hash_table: a #ND_HashTable.
 * @key: the key to look up.
 *
 * Looks up a key in a #ND_HashTable. Note that this function cannot
 * distinguish between a key that is not present and one which is present
 * and has the value %NULL. If you need this distinction, use
 * NDHT_LookupExtended().
 *
 * Return value: the associated value, or %NULL if the key is not found.
 **/
void * NDHT_Lookup( HANDLE hash_table, void * key )
{
	ND_UINT32 node_index;
	ND_UINT32 node_hash;
	ND_HashTable * pRec;

	if( hash_table == NULL ) return NULL;
	pRec = (ND_HashTable*)hash_table;
	node_index = __lookup_node( pRec, key, &node_hash );
	return HASH_IS_REAL(pRec->hashes[node_index])?pRec->values[node_index]:NULL;
}

/**
 * NDHT_LookupExtended:
 * @hash_table: a #ND_HashTable
 * @lookup_key: the key to look up
 * @orig_key: return location for the original key, or %NULL
 * @value: return location for the value associated with the key, or %NULL
 *
 * Looks up a key in the #ND_HashTable, returning the original key and the
 * associated value and a #ND_BOOL which is %TRUE if the key was found. This
 * is useful if you need to free the memory allocated for the original key,
 * for example before calling NDHT_Remove().
 *
 * You can actually pass %NULL for @lookup_key to test
 * whether the %NULL key exists, provided the hash and equal functions
 * of @hash_table are %NULL-safe.
 *
 * Return value: %TRUE if the key was found in the #ND_HashTable.
 **/
ND_BOOL NDHT_LookupExtended( HANDLE hash_table, void * lookup_key, void **orig_key, void **value )
{
	ND_UINT32 node_index;
	ND_UINT32 node_hash;
	ND_HashTable * pRec;

	if( hash_table == NULL ) return /*FALSE*/false;
	pRec = (ND_HashTable*)hash_table;
	node_index = __lookup_node( pRec, lookup_key, &node_hash );
	if( !HASH_IS_REAL( pRec->hashes[node_index] ) )
		return /*FALSE*/false;
	if( orig_key )
		*orig_key = pRec->keys[node_index];
	if( value )
		*value = pRec->values[node_index];
	return /*TRUE*/true;
}

/**
 * NDHT_Foreach:
 * @hash_table: a #ND_HashTable.
 * @func: the function to call for each key/value pair.
 * @user_data: user data to pass to the function.
 *
 * Calls the given function for each of the key/value pairs in the
 * #ND_HashTable.  The function is passed the key and value of each
 * pair, and the given @user_data parameter.  The hash table may not
 * be modified while iterating over it (you can't add/remove
 * items). To remove all items matching a predicate, use
 * NDHT_ForeachRemove().
 *
 * See NDHT_Find() for performance caveats for linear
 * order searches in contrast to NDHT_Lookup().
 **/
void NDHT_Foreach( HANDLE hash_table, ND_HFunc_CB func, void * user_data )
{
	ND_INT32 i;
	ND_HashTable * pRec;

	if( hash_table == NULL ) return;
	if( func == NULL ) return;
	pRec = (ND_HashTable*)hash_table;
	for( i = 0; i < pRec->size; i++ )
    {
		ND_UINT32 node_hash = pRec->hashes[i];
		void * node_key = pRec->keys[i];
		void * node_value = pRec->values[i];

		if( HASH_IS_REAL(node_hash) )
			(*func)( node_key, node_value, user_data );
    }
}

/**
 * NDHT_Find:
 * @hash_table: a #ND_HashTable.
 * @predicate:  function to test the key/value pairs for a certain property.
 * @user_data:  user data to pass to the function.
 *
 * Calls the given function for key/value pairs in the #ND_HashTable until
 * @predicate returns %TRUE.  The function is passed the key and value of
 * each pair, and the given @user_data parameter. The hash table may not
 * be modified while iterating over it (you can't add/remove items).
 *
 * Note, that hash tables are really only optimized for forward lookups,
 * i.e. NDHT_Lookup().
 * So code that frequently issues NDHT_Find() or
 * NDHT_Foreach() (e.g. in the order of once per every entry in a
 * hash table) should probably be reworked to use additional or different
 * data structures for reverse lookups (keep in mind that an O(n) find/foreach
 * operation issued for all n values in a hash table ends up needing O(n*n)
 * operations).
 *
 * Return value: The value of the first key/value pair is returned,
 *     for which @predicate evaluates to %TRUE. If no pair with the
 *     requested property is found, %NULL is returned.
 *
 * Since: 2.4
 **/
void * NDHT_Find( HANDLE hash_table, ND_HRFunc_CB predicate, void * user_data )
{
	ND_INT32 i;
	ND_BOOL match;
	ND_HashTable * pRec;

	if( hash_table == NULL ) return NULL;
	if( predicate == NULL ) return NULL;
 
	pRec = (ND_HashTable*)hash_table;
	match = /*FALSE*/false;
	for( i = 0; i < pRec->size; i++ )
    {
		ND_UINT32 node_hash = pRec->hashes[i];
		void * node_key = pRec->keys[i];
		void * node_value = pRec->values[i];
		if( HASH_IS_REAL(node_hash) )
			match = predicate( node_key, node_value, user_data );
		if( match )
			return node_value;
    }
	return NULL;
}

/**
 * NDHT_ForeachRemove:
 * @hash_table: a #ND_HashTable.
 * @func: the function to call for each key/value pair.
 * @user_data: user data to pass to the function.
 *
 * Calls the given function for each key/value pair in the #ND_HashTable.
 * If the function returns %TRUE, then the key/value pair is removed from the
 * #ND_HashTable. If you supplied key or value destroy functions when creating
 * the #ND_HashTable, they are used to free the memory allocated for the removed
 * keys and values.
 *
 * See #ND_HashTableIter for an alternative way to loop over the 
 * key/value pairs in the hash table.
 *
 * Return value: the number of key/value pairs removed.
 **/
ND_UINT32 NDHT_ForeachRemove( HANDLE hash_table, ND_HRFunc_CB func, void * user_data )
{
	if( hash_table == NULL ) return 0;
	if( func == NULL ) return 0;
	return __foreach_remove_or_steal( hash_table, func, user_data, TRUE );
}

/**
 * NDHT_ForeachSteal:
 * @hash_table: a #ND_HashTable.
 * @func: the function to call for each key/value pair.
 * @user_data: user data to pass to the function.
 *
 * Calls the given function for each key/value pair in the #ND_HashTable.
 * If the function returns %TRUE, then the key/value pair is removed from the
 * #ND_HashTable, but no key or value destroy functions are called.
 *
 * See #ND_HashTableIter for an alternative way to loop over the
 * key/value pairs in the hash table.
 *
 * Return value: the number of key/value pairs removed.
 **/
ND_UINT32 NDHT_ForeachSteal( HANDLE hash_table, ND_HRFunc_CB func, void * user_data )
{
	if( hash_table == NULL ) return 0;
	if( func == NULL ) return 0;
	return __foreach_remove_or_steal( hash_table, func, user_data, /*FALSE*/false );
}

/**
 * NDHT_Size:
 * @hash_table: a #ND_HashTable.
 *
 * Returns the number of elements contained in the #ND_HashTable.
 *
 * Return value: the number of key/value pairs in the #ND_HashTable.
 **/
ND_UINT32 NDHT_Size( HANDLE hash_table )
{
	if( hash_table != NULL ) return 0;
	return ((ND_HashTable*)hash_table)->nnodes;
}

/**
 * NDHT_GetKeys:
 * @hash_table: a #ND_HashTable
 *
 * Retrieves every key inside @hash_table. The returned data is valid
 * until @hash_table is modified.
 *
 * Return value: a #GList containing all the keys inside the hash
 *   table. The content of the list is owned by the hash table and
 *   should not be modified or freed. Use g_list_free() when done
 *   using the list.
 *
 * Since: 2.14
 */
HANDLE NDHT_GetKeys( HANDLE hash_table )
{
	ND_INT32 i;
	GList *retval;
	ND_HashTable * pRec;

	if( hash_table != NULL ) return NULL;

	retval = NULL;
	pRec = (ND_HashTable*)hash_table;
	for( i = 0; i < pRec->size; i++ )
    {
		if( HASH_IS_REAL( pRec->hashes[i] ) )
			retval = __list_prepend( retval, pRec->keys[i] );
    }
	return retval;
}

/**
 * NDHT_GetValues:
 * @hash_table: a #ND_HashTable
 *
 * Retrieves every value inside @hash_table. The returned data is
 * valid until @hash_table is modified.
 *
 * Return value: a #GList containing all the values inside the hash
 *   table. The content of the list is owned by the hash table and
 *   should not be modified or freed. Use g_list_free() when done
 *   using the list.
 *
 * Since: 2.14
 */
HANDLE NDHT_GetValues( HANDLE hash_table )
{
	ND_INT32 i;
	GList *retval;
	ND_HashTable * pRec;

	if( hash_table == NULL ) return NULL;

	pRec = (ND_HashTable*)hash_table;
	retval = NULL;
	for( i = 0; i < pRec->size; i++ )
    {
		if( HASH_IS_REAL( pRec->hashes[i] ) )
			retval = __list_prepend( retval, pRec->values[i] );
    }
	return retval;
}

/**
 * NDHT_IterInit:
 * @iter: an uninitialized #ND_HashTableIter.
 * @hash_table: a #ND_HashTable.
 *
 * Initializes a key/value pair iterator and associates it with
 * @hash_table. Modifying the hash table after calling this function
 * invalidates the returned iterator.
 * |[
 * ND_HashTableIter iter;
 * void * key, value;
 *
 * NDHT_IterInit (&iter, hash_table);
 * while (NDHT_IterNext (&iter, &key, &value)) 
 *   {
 *     /&ast; do something with key and value &ast;/
 *   }
 * ]|
 *
 * Since: 2.16
 **/
void NDHT_IterInit( HANDLE hIter, HANDLE hash_table )
{
	RealIter * ri = (RealIter *)hIter;

	if( hIter == NULL ) return;
	if( hash_table == NULL ) return;

	ri->hash_table = hash_table;
	ri->position = -1;
}

/**
 * NDHT_IterNext:
 * @iter: an initialized #ND_HashTableIter.
 * @key: a location to store the key, or %NULL.
 * @value: a location to store the value, or %NULL.
 *
 * Advances @iter and retrieves the key and/or value that are now
 * pointed to as a result of this advancement. If %FALSE is returned,
 * @key and @value are not set, and the iterator becomes invalid.
 *
 * Return value: %FALSE if the end of the #ND_HashTable has been reached.
 *
 * Since: 2.16
 **/
ND_BOOL NDHT_IterNext( HANDLE hIter, void ** key, void ** value )
{
	RealIter *ri = (RealIter*)hIter;
	ND_INT32 position;

	if( !hIter ) return /*FALSE*/false;
	if( ri->position >= ri->hash_table->size ) return /*FALSE*/false;

	position = ri->position;

	do
    {
		position++;
		if( position >= ri->hash_table->size )
		{
			ri->position = position;
			return /*FALSE*/false;
		}
	}
	while( !HASH_IS_REAL( ri->hash_table->hashes[position] ) );

	if( key != NULL )
		*key = ri->hash_table->keys[position];
	if( value != NULL )
		*value = ri->hash_table->values[position];

	ri->position = position;
	return /*TRUE*/true;
}

/**
 * NDHT_IterGetHashTable:
 * @iter: an initialized #ND_HashTableIter.
 *
 * Returns the #ND_HashTable associated with @iter.
 *
 * Return value: the #ND_HashTable associated with @iter.
 *
 * Since: 2.16
 **/
HANDLE NDHT_IterGetHashTable( HANDLE hIter )
{
	if( hIter ) return NULL;
	return ((RealIter*)hIter)->hash_table;
}

/**
 * NDHT_IterRemove:
 * @iter: an initialized #ND_HashTableIter.
 *
 * Removes the key/value pair currently pointed to by the iterator
 * from its associated #ND_HashTable. Can only be called after
 * NDHT_IterNext() returned %TRUE, and cannot be called more
 * than once for the same key/value pair.
 *
 * If the #ND_HashTable was created using NDHT_NewFull(), the
 * key and value are freed using the supplied destroy functions, otherwise
 * you have to make sure that any dynamically allocated values are freed 
 * yourself.
 *
 * Since: 2.16
 **/
void NDHT_IterRemove( HANDLE hIter )
{
	__iter_remove_or_steal( (RealIter*)hIter, /*TRUE*/true );
}

/**
 * NDHT_IterReplace:
 * @iter: an initialized #ND_HashTableIter.
 * @value: the value to replace with
 *
 * Replaces the value currently pointed to by the iterator
 * from its associated #ND_HashTable. Can only be called after
 * NDHT_IterNext() returned %TRUE.
 *
 * If you supplied a @value_destroy_func when creating the #ND_HashTable,
 * the old value is freed using that function.
 *
 * Since: 2.29.9
 **/
void NDHT_IterReplace( HANDLE hIter, void * value )
{
	RealIter *ri;
	ND_UINT32 node_hash;
	void * key;

	ri = (RealIter*)hIter;

	assert( ri != NULL );
	assert( ri->position >= 0 );
	assert( ri->position < ri->hash_table->size );

	node_hash = ri->hash_table->hashes[ri->position];
	key = ri->hash_table->keys[ri->position];

	__insert_node( ri->hash_table, ri->position, node_hash, key, value, TRUE );
}

/**
 * NDHT_IterSteal:
 * @iter: an initialized #ND_HashTableIter.
 *
 * Removes the key/value pair currently pointed to by the iterator
 * from its associated #ND_HashTable, without calling the key and value
 * destroy functions. Can only be called after
 * NDHT_IterNext() returned %TRUE, and cannot be called more
 * than once for the same key/value pair.
 *
 * Since: 2.16
 **/
void NDHT_IterSteal( HANDLE hIter )
{
	__iter_remove_or_steal( (RealIter*)hIter, /*FALSE*/false );
}

/**
 * NDHT_Ref:
 * @hash_table: a valid #ND_HashTable.
 *
 * Atomically increments the reference count of @hash_table by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Return value: the passed in #ND_HashTable.
 *
 * Since: 2.10
 **/
HANDLE NDHT_Ref ( HANDLE hHashTable )
{
	if( hHashTable == NULL )
		return NULL;

	((ND_HashTable*)hHashTable)->ref_count++;
	return hHashTable;
}

/**
 * NDHT_Unref:
 * @hash_table: a valid #ND_HashTable.
 *
 * Atomically decrements the reference count of @hash_table by one.
 * If the reference count drops to 0, all keys and values will be
 * destroyed, and all memory allocated by the hash table is released.
 * This function is MT-safe and may be called from any thread.
 *
 * Since: 2.10
 **/
void NDHT_Unref( HANDLE hHashTable )
{
	ND_HashTable * p;

	assert( hHashTable != NULL );

	p = (ND_HashTable*)hHashTable;

	if( p->ref_count == 0 )
	{
		__remove_all_nodes( hHashTable, /*TRUE*/true );
		if( p->keys != p->values )
			free( p->values );
		free( p->keys );
		free( p->hashes );
		free( p );
    }
}

/**
 * ND_StrHash:
 * @v: a string key
 *
 * Converts a string to a hash value.
 *
 * This function implements the widely used "djb" hash apparently posted
 * by Daniel Bernstein to comp.lang.c some time ago.  The 32 bit
 * unsigned hash value starts at 5381 and for each byte 'c' in the
 * string, is updated: <literal>hash = hash * 33 + c</literal>.  This
 * function uses the signed value of each byte.
 *
 * It can be passed to NDHT_New() as the @hash_func parameter,
 * when using strings as keys in a #ND_HashTable.
 *
 * Returns: a hash value corresponding to the key
 **/
ND_UINT32 ND_StrHash( void * v )
{
	const signed char *p;
	ND_UINT32 h = 5381;

	for( p = v; *p != '\0'; p++ )
		h = (h << 5) + h + *p;
	return h;
}

/**
 * ND_IntHash:
 * @v: a pointer to a #ND_INT32 key
 *
 * Converts a pointer to a #ND_INT32 to a hash value.
 * It can be passed to NDHT_New() as the @hash_func parameter, 
 * when using pointers to integers values as keys in a #ND_HashTable.
 *
 * Returns: a hash value corresponding to the key.
 */
ND_UINT32 ND_IntHash( void * v )
{
	return *(const ND_INT32*) v;
}

/**
 * ND_Int64Hash:
 * @v: a pointer to a #ND_INT64 key
 *
 * Converts a pointer to a #ND_INT64 to a hash value.
 * It can be passed to NDHT_New() as the @hash_func parameter, 
 * when using pointers to 64-bit integers values as keys in a #ND_HashTable.
 *
 * Returns: a hash value corresponding to the key.
 *
 * Since: 2.22
 */
ND_UINT32 ND_Int64Hash (void * v)
{
	return (ND_UINT32) *(const ND_INT64*)v;
}

/**
 * ND_DoubleHash:
 * @v: a pointer to a #ND_DOUBLE key
 *
 * Converts a pointer to a #ND_DOUBLE to a hash value.
 * It can be passed to NDHT_New() as the @hash_func parameter, 
 * when using pointers to doubles as keys in a #ND_HashTable.
 *
 * Returns: a hash value corresponding to the key.
 *
 * Since: 2.22
 */
ND_UINT32 ND_DoubleHash( void * v )
{
	return (ND_UINT32)*(const ND_DOUBLE*)v;
}

/**
 * ND_DirectHash:
 * @v: a #void * key
 *
 * Converts a void * to a hash value.
 * It can be passed to g_hash_table_new() as the @hash_func parameter, 
 * when using pointers as keys in a #ND_HashTable.
 *
 * Returns: a hash value corresponding to the key.
 */
ND_UINT32 ND_DirectHash( void * v )
{
	return (ND_UINT32)(v);
}

//#################################################################################################
//##################################### 外部接口结束 ##############################################
//#################################################################################################
