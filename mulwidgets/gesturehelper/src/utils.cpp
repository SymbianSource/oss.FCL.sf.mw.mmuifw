/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Gesture recognition utilities
*
*/

#include "utils.h"

#include <coemain.h> // for CCoeEnv
#include <eikappui.h>  // for CEikAppUi

#include "gesturedefs.h"

namespace GestureHelper
    {
    /** @return the longer edge of the size */
    inline TInt LongerEdge( const TSize& aSize ) 
        {
        return Max( aSize.iHeight, aSize.iWidth );
        }
    
    // for documentation, see header file
    TRect ToleranceRect( const TPoint& aCenterPoint ) 
        {
        TSize screenSize = static_cast<CEikAppUi*>( CCoeEnv::Static()->AppUi() )
            ->ApplicationRect().Size(); 
        // multiplication has to be done first, to avoid rounding integer to 0 with division
        int toleranceLength = ( KGestureTolerancePercent * LongerEdge( screenSize ) ) / 100; 
        TRect toleranceRect( aCenterPoint, TSize() );
        // grow by the tolerance length, while keeping the center point
        toleranceRect.Shrink( -toleranceLength, -toleranceLength );
        return toleranceRect;
        }
 
     // for documentation, see header file
    TRect ToleranceRect( const TPoint& aCenterPoint, MGestureEvent::TAxis aRelevantAxis ) 
        {
        TRect toleranceRect = ToleranceRect(aCenterPoint);

		TSize screenSize = static_cast<CEikAppUi*>( CCoeEnv::Static()->AppUi() )
            ->ApplicationRect().Size();
            
		if(aRelevantAxis == MGestureEvent::EAxisHorizontal)
			{
			toleranceRect.iTl.iY = 0;
        	toleranceRect.iBr.iY = screenSize.iHeight;	
			}
		else if(aRelevantAxis == MGestureEvent::EAxisVertical)
			{
			toleranceRect.iTl.iX = 0;
        	toleranceRect.iBr.iX = screenSize.iWidth;	
			}
		else // EAxisBoth
			{
			// nothing to be done	
			}                   

        return toleranceRect;
        }
           
    } // namespace GestureHelper
