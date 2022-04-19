/*************************************************************************************************
*                                                                                                *
*                                           [ Til-mi ]                                           *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                     A tool for creating State of War's til and tmi files.                      *
*               (ɔ) 2017 - 2022 State of War Baidu PostBar, some rights reserved.                *
*                                                                                                *
*             Tilmi is a free software. You can freely do whatever you want with it              *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*               Tilmi is created, intended to be useful, but without any warranty.               *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*               <https://github.com/State-of-War-PostBar/Tilmi/blob/main/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/Tilmi>.                        *
*                                                                                                *
**************************************************************************************************/

#ifndef TILMI_VECTOR_H
#define TILMI_VECTOR_H

#include <pch.h>

// Ripped straight from the Blue Alert project. Very limited edition...

typedef bool (*VectorCmpFunc)( const void *, const void * );

typedef struct Vector
{
    size_t elem_size;
    size_t capacity;
    size_t length;
    void *ptr;
} Vector;

Vector
Vector_Create( size_t elem_size );

void *
Vector_PtrAt( const Vector *vec, size_t index );

void *
Vector_Find( const Vector *vec, const void *data, VectorCmpFunc cmp );

void
Vector_Expand( Vector *vec );

void
Vector_ExpandUntil( Vector *vec, size_t size );

void
Vector_Push( Vector *vec, const void *elem );

void
Vector_Destroy( Vector *vec );

#endif // !TILMI_VECTOR_H
