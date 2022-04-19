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

#include "vector.h"

Vector
Vector_Create( size_t elem_size )
{
    return (Vector)
    {
        .length = 0,
        .capacity = 0,
        .elem_size = elem_size,
        .ptr = NULL
    };
}

void *
Vector_PtrAt( const Vector *vec, size_t index )
{
    return (void *)((char *)(vec->ptr) + vec->elem_size * index);
}

void *
Vector_Find( const Vector *vec, const void *elem, VectorCmpFunc cmp )
{
    if (!vec->length)
        return NULL;

    char *ptr = vec->ptr;
    for (size_t i = 0; i < vec->length; i++, ptr += vec->elem_size)
        if (cmp((void *)ptr, elem))
            return ptr;

    return NULL;
}

void
Vector_Expand( Vector *vec )
{
    if (!vec->capacity)
    {
        vec->capacity = 1ULL;
        vec->ptr = HeapAlloc(GetProcessHeap(), 0, vec->elem_size);
    }
    else
    {
        vec->capacity *= 2ULL;
        vec->ptr = HeapReAlloc(GetProcessHeap(), 0, vec->ptr, vec->elem_size * vec->capacity);
    }
}

void
Vector_ExpandUntil( Vector *vec, size_t size )
{
    // This can be optimized, but usually we won't need this repeatedly
    while (vec->capacity < size)
        Vector_Expand(vec);
}

void
Vector_Push( Vector *vec, const void *elem )
{
    Vector_ExpandUntil(vec, vec->length + 1ULL);
    CopyMemory(Vector_PtrAt(vec, vec->length), elem, vec->elem_size);
    vec->length++;
}

void
Vector_Destroy( Vector *vec )
{
    if (vec->capacity)
        HeapFree(GetProcessHeap(), 0, vec->ptr);
}
