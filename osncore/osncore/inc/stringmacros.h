/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  string Macro include.
*
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


#define QS_INTERNAL_BUFFER_SIZE 20
#define QS_INTERNAL_BUFFER_CHARS QS_INTERNAL_BUFFER_SIZE-1


#define malloc_good_size(size) size

//#define ALLOC_QCHAR_GOOD_SIZE(X) (malloc_good_size(X*sizeof(AlfChar))/sizeof(AlfChar))
#define ALLOC_CHAR_GOOD_SIZE(X) (malloc_good_size(X))

//#ifndef __OOM__
#define ALLOC_CHAR( N ) (char*) malloc(N)
#define REALLOC_CHAR( P, N ) (char *) realloc(P,N)
//#define ALLOC_QCHAR( N ) (AlfChar*) malloc(sizeof(AlfChar)*( N ))
//#define REALLOC_QCHAR( P, N ) (AlfChar *) realloc(P,sizeof(AlfChar)*( N ))
#define DELETE_CHAR( P ) free(P)
#define DELETE_QCHAR( P ) free( P )


#define QS_INTERNAL_BUFFER_UCHARS QS_INTERNAL_BUFFER_SIZE/2


