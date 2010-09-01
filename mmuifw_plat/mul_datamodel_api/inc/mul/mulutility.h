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


#ifndef MULUTILITY_H_
#define MULUTILITY_H_

#include <stdexcept>
#include <string>
#include <e32cmn.h>
#include <osn/osndefines.h>
#include "mulkeyboardutility.h"

namespace osncore
    {
    class UString;    
    }

using namespace osncore;
    
namespace Alf
    {
    	
static void MUL_ASSERT(bool aExpression, const char* aExceptionString)
   {
   if( aExpression )
       {
       throw std::logic_error(aExceptionString);
       }
   }
                               
class MulUtility
    {
public:
    /**
     * Converts the relative path to the absolute path.
     * 
     * @param aRelativePath Which needs to be changed to the absolutepath.
     * @return true , if aRelativepath changed to Absoulte path or else false
     */
	OSN_IMPORT static bool AbsolutePath(UString& aRelativePath);
    
   /**
    * Checks which orienatation the application is in and returns true if it is in 
    * Landscape orientation otherwise returns false
    *
    */ 
  OSN_IMPORT static bool IsLandscape();
  
     /**
      * Checks if the given string tag is a pool resource or not
      *
      * @param aImageTag Path of the image file,which needs to be checked for resource tag format. 
      */
     OSN_IMPORT static bool IsPoolResource(const UString& aImageTag);

     /**
      * Checks if current KeyBoard is QWERTY
      * @return true if KeyBoard is QWERTY else false.
      */
     OSN_IMPORT static bool IsQwerty();
     
private: 
    /**
     * Checks the file for its existence
     *
     * @param afilePath File to be checked.
     * @return true, In case file exists or else false. 
     */
    static bool IsFileExists(/*const char* afilePath*/const TDesC& aFilePath);
    
    
    
    };
    
    } // namespace Alf

#endif  // MULUTILITY_H_

// End of file

