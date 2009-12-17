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
* Description:  Implementation for utility class
*
*/

// Includes

// Class header
#include "mulutility.h"

#include "mul/mulmodelutility.h"
#include "mulassert.h"
#include <osn/ustring.h>
#include <f32file.h>
#include <eikenv.h>
#include <aknappui.h>
#include <AknUtils.h>

// Local includes
#include "mulassert.h"
//for logs
#include "mullog.h"
namespace Alf
    {
        
//-----------------------------------------------------------------------------
// AbsolutePath
//-----------------------------------------------------------------------------
//
OSN_EXPORT bool MulUtility::AbsolutePath(UString& aRelativePath)
    {
    MUL_LOG_INFO("aakash::MulUtility::AbsolutePath");
    bool result = false;
    aRelativePath.insert(0,"c:");
    HBufC* relBuffer =NULL;
        
    TRAPD(err, relBuffer = HBufC::NewL(aRelativePath.getCharLength()))
    __MUL_ASSERT(err == KErrNone ,_L("Bad Alloc")); 
    
    try
	    {  
	    TPtr ptr = relBuffer->Des()              ;
	    MulModelUtility::ConvertUStringToTDes(aRelativePath,ptr);    
			
	    if(IsFileExists(*relBuffer))
	    	{
	    	result = true;
	    	}
	   	else
		   	{		   	
		   	aRelativePath.replace(0,"z");
		    MulModelUtility::ConvertUStringToTDes(aRelativePath,ptr);    
		    if(IsFileExists(*relBuffer))
		    	{
		    	result = true;
		    	}
		    else
		    	{
		    	aRelativePath.erase(0,2);	
		    	}		
		   	}
	   	   	    
	    }
	catch(...)
		{
		
		}
	delete relBuffer;	
    return result;
    }

//-----------------------------------------------------------------------------
// IsLandscape
//-----------------------------------------------------------------------------
//    
OSN_EXPORT  bool MulUtility::IsLandscape()
	{
	MUL_LOG_INFO("aakash::MulUtility::IsLandscape");
	CAknAppUiBase::TAppUiOrientation 
        orientation = static_cast<CAknAppUi*>((CEikonEnv::Static()->EikAppUi()))->Orientation();
        
                            
	if (orientation == CAknAppUiBase::EAppUiOrientationLandscape)
    {
  	 return true;
   
    }
    
     else if (orientation == CAknAppUiBase::EAppUiOrientationUnspecified )
     {
        AknLayoutUtils::TAknCbaLocation cbaLocation = AknLayoutUtils::CbaLocation();
        if ( (cbaLocation == AknLayoutUtils::EAknCbaLocationRight) ||
             (cbaLocation == AknLayoutUtils::EAknCbaLocationLeft) )
            {
            //landscape
            return true;
            }
        else
            {
            // Portrait
            return false;
            }
    }
    return false;
    
	}

//-----------------------------------------------------------------------------
// IsFileExists
//-----------------------------------------------------------------------------
//  
bool MulUtility::IsFileExists(/*const char* afilePath*/const TDesC& aFilePath)
    {
    MUL_LOG_INFO("aakash::MulUtility::IsFileExists");
    bool result(false);
    
  	  
    RFs fSession;
    TInt fsError = fSession.Connect();    
    __MUL_ASSERT(fsError == KErrNone,_L("File Session unable to connect"));    
    
    RFile chkFile;    
    TInt errCode = chkFile.Open(fSession,aFilePath,EFileRead);
    if(KErrPathNotFound != errCode)
    	{
    	result = true;	
    	}    
    chkFile.Close();	
    fSession.Close();		
    return result;        
    }
    
// ---------------------------------------------------------------------------
// IsPoolResource   
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool MulUtility::IsPoolResource(const UString& aImageTag)
    {    
    TPtrC8 src((TUint8*)aImageTag.getUtf8());
    _LIT8(KMulTag,"MUL_RP");

    return ( KErrNotFound != src.FindC(KMulTag));    
    }
//----------------------------------------------------------------------------
//IsQwerty
//----------------------------------------------------------------------------
OSN_EXPORT bool MulUtility::IsQwerty()
	{
		CKeyBoardUtility* iKBUtility = CKeyBoardUtility::NewL();
		if(iKBUtility)
		{
			return iKBUtility->IsQwerty();
		}
		return false;
	}

    } // namespace ends

// End of file
