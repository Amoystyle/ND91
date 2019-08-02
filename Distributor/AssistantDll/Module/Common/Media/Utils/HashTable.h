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

#ifndef __HASHTABLE_H
#define __HASHTABLE_H
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include "iTunesMp3Lib_Type.h"
#ifdef WIN32
	#include <malloc.h>
#else
	typedef void* 				HANDLE;
	#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef ND_UINT32 ( *ND_HashFunc_CB)( void * key );
typedef ND_BOOL (*ND_EqualFunc_CB)( void * a, void * b );
typedef void (*ND_DestroyNotify_CB)( void * data );
typedef void (*ND_HFunc_CB)( void * key, void * value, void * user_data );
typedef ND_BOOL (*ND_HRFunc_CB)( void * key, void * value, void * user_data );

HANDLE NDHT_New( ND_HashFunc_CB	hash_func, ND_EqualFunc_CB key_equal_func );
HANDLE NDHT_NewFull( ND_HashFunc_CB hash_func, ND_EqualFunc_CB key_equal_func, ND_DestroyNotify_CB key_destroy_func, ND_DestroyNotify_CB  value_destroy_func);
void NDHT_Destroy( HANDLE hash_table);
void NDHT_Insert( HANDLE hash_table, void *	key, void * value );
void NDHT_Replace( HANDLE hash_table, void * key, void * value );
ND_BOOL NDHT_Remove( HANDLE hash_table,void * key );
void NDHT_RemoveAll( HANDLE hash_table );
ND_BOOL NDHT_Steal( HANDLE hash_table, void * key );
void NDHT_StealAll( HANDLE hash_table );
void * NDHT_Lookup( HANDLE hash_table, void * key );
ND_BOOL NDHT_LookupExtended( HANDLE hash_table, void * lookup_key, void ** orig_key, void ** value );
void NDHT_Foreach( HANDLE hash_table, ND_HFunc_CB func, void * user_data );
void * NDHT_Find( HANDLE hash_table, ND_HRFunc_CB predicate, void * user_data );
ND_UINT32 NDHT_ForeachRemove( HANDLE hash_table, ND_HRFunc_CB func, void * user_data );
ND_UINT32 NDHT_ForeachSteal( HANDLE hash_table, ND_HRFunc_CB func, void * user_data );
ND_UINT32 NDHT_Size( HANDLE hash_table );
HANDLE NDHT_GetKeys( HANDLE hash_table );
HANDLE NDHT_GetValues( HANDLE hash_table );
void NDHT_IterInit( HANDLE hIter, HANDLE hash_table );
ND_BOOL NDHT_IterNext( HANDLE hIter, void **key, void **value );
HANDLE NDHT_IterGetHashTable( HANDLE hIter );
void NDHT_IterRemove( HANDLE hIter );
void NDHT_IterReplace( HANDLE hIter, void *	value );
void NDHT_IterSteal( HANDLE hIter );
/* keeping hash tables alive */
HANDLE NDHT_Ref( HANDLE hash_table );
void NDHT_Unref( HANDLE hash_table );

ND_UINT32 ND_StrHash( void * v );

ND_UINT32 ND_IntHash( void * v );

ND_UINT32 ND_Int64Hash( void * v );

ND_UINT32 ND_DoubleHash( void * v );

ND_UINT32 ND_DirectHash( void * v );

#ifdef __cplusplus
}
#endif

#endif