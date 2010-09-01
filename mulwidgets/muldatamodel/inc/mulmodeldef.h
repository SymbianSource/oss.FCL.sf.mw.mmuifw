/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains const character string for model
*
*/


#ifndef __MULMODELDEF_H
#define __MULMODELDEF_H

#include <e32base.h>

namespace Alf
	{
	
static const int KDefaultBufferSize = 50;    
static const int KDefaultWindowSize = 20;  
static const int KNotInitialized = -1;

static const char* const KDefaultTemplate = "template";
static const char* const KItem = "item";
static const char* const KContainerIndex = "container";
static const char* const KItemIndex = "itemindex";
static const char* const KInvalidArgument = "invalid argument";
static const char* const KIndex = "index";
static const char* const KStartIndex = "index";
static const char* const KCount = "count";

_LIT(KLInvalidArgument,"Invalid Argument");
_LIT(KLNotImplemented,"Not implemented");
	
	} // namespace Alf

#endif // __MULMODELDEF_H

// End of file
