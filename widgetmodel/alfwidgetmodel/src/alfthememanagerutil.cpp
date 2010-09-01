/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of AlfThemeManagerUtil class, the class
*                which is an interface to the ThemeManager component
*
*/

#include "alf/alfthememanagerutil.h"


#include "tm/themeitemmapper.h"
#include "tm/thememanager.h"
#include "tm/themeexception.h"
using namespace Alf;

#include <UTF.h>

namespace Alf
    {
   
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfThemeManagerUtil::AlfThemeManagerUtil()
    {
    mThemeManager=NULL;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfThemeManagerUtil::~AlfThemeManagerUtil()
    {
	if(mThemeManager)
		{
    	ThemeManager::deleteThemeManager();
    	mThemeManager = NULL;
		}
    }

// ---------------------------------------------------------------------------
// Get Skin Image parameters.  Given the logical name this method returns the skinid.
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAknsItemID AlfThemeManagerUtil::getSkinId(const Utf8* aSkinLogicalName)
	{

	TAknsItemID skinid;
	skinid.Set(0,0);
	
	// Create ThemeManager instance
	if(!mThemeManager)	
		{			
		mThemeManager = ThemeManager::createThemeManager();
		}
		
	// Query ThemeManager for an ItemMapper interface
	IThemeItemMapper* itemMapper=NULL;	
	// Get Image data	
	ThemeItemDef item;
	 
	try
		{		
		mThemeManager->getItemMapperInterface(&itemMapper);
		UString alfSkinLogicalname(aSkinLogicalName);
		if(itemMapper)
			{		
			item= itemMapper->findItem(alfSkinLogicalname);
			}				
		}              
	catch(ThemeException& ex)
		{	
		if(itemMapper)
			mThemeManager->releaseItemMapperInterface();
		throw ex;
		}
		
	skinid.iMajor= item.getMajorId(); 
	skinid.iMinor= item.getMinorId();
	
	// Done with ItemMapper interface, release it
	if(itemMapper)
		mThemeManager->releaseItemMapperInterface();
	return skinid;
	}

    }
