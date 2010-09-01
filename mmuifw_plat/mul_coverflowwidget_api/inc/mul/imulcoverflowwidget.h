/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The interface class for all Multimedia UI Library Cover Flow Widgets.
 *
*/

#ifndef I_MULCOVERFLOWWIDGET_H
#define I_MULCOVERFLOWWIDGET_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>
#include <mul/imulmultiitemwidget.h>
#include <gdi.h>
#include <AknsItemID.h>

//Gesture Helper namespace 
namespace GestureHelper
    {
    class CGestureHelper;
    }
    
namespace Alf
	{
	namespace mulcoverflowwidget
	    {
	    static const IfId Ident=
	    	{
	    	0, "mulcoverflowwidget"
	        };      
		}

	/**
	 *  An interface for Multimedia coverflow Widget.
	 *  Widgets consist of control and visualization elements.
	 * 	
	 *	Interface paradigm would be used in a restrictive way.
	 *  Controls are owned by Alfred environment.
	 *  Widgets can be constructed using the widget factory.
	 *  Widgets are owned and accessed using the Alfred
	 *  environment.
	 *
	 *  @lib mulcoverflowwidget.lib
	 */
    class IMulCoverFlowWidget : public IMulMultiItemWidget
    	{
    public:

		/**
		 * Getter for the type identifier of this interface.
		 *
		 * @return A descriptor to identify the type of this interface.
		 **/
        static inline const IfId& Type()
        	{
        	return mulcoverflowwidget::Ident;
        	}
		
        /**
         * Setting color as background of the coverflow.
         * The API should be used in the case when the background type doesn't 
         * comes along with any extra data. This API should be used when the 
         * background has to set to Black, White or any other color
         *
         * @param aColor RGB color
         *
         * <B>Examples:</b>
         * @code
         * Set background of the coverflow as red color.
         *
         *     mCoverFlowWidget->SetBackground( KRgbRed );
         * @endcode
         **/ 
        virtual void SetBackground( const TRgb& aColor ) = 0;

        /**
         * Setting Skin id as background of the coverflow.
         * The API should be used in the case when the background of
         * Coverflow should come from skin data.
         *
         * @param aItemID Skin ID.
         *
         * <B>Examples:</b>
         * @code
         * Set background of the coverflow as a texture.
         *
         *     mCoverFlowWidget->SetBackground( KAknsIIDQsnBgScreenIdle );
         * 
         * @endcode
         **/ 
        virtual  void SetBackground(const TAknsItemID& aItemID ) = 0;
        
		/**
		 * Setting UIOnOff mode in template4 of the coverflow.
		 * The API should be in template 4 to hide or show counter as per UI ON/OFF mode.
		 *
		 * @param aUIOn true if UIOn mode and counter to be displayed, else false for UIOff.
		 * @param aTransition transition time to show or hide counter.

		 * <B>Examples:</b>
		 * @code
		 * Set UiOnOff mode in coverflow widget. 
		 *
		 *     mCoverFlowWidget->SetUIOnOFF( true, 100 );
		 * 
		 * @endcode
		 **/
        virtual  void SetUIMode(bool aUIOn = false, int aTransition = 0 ) = 0;  
        
		/**
		 * Rotate the highlight image
		 * The API is supported in template 4(fullscreen templet) to rotate the highlight image as per the 
		 * direction specified in the param.
		 * This will rotate the image 90 degree on left or right side.
		 *
		 * @param aDirection, Direction of the rotation (ERotateLeft/ERotateRight).
		 * @param aImageSize, The actual size of the current highlighted image.
		 * @param aTransition transition time to rotate the image.

		 * <B>Examples:</b>
		 * @code
		 * Rotate the highlight image of the coverflow widget. 
		 *
		 *     mCoverFlowWidget->RotateImage( IMulWidget::ERotateLeft,TSize(500,400),100 );
		 * 
		 * @endcode
		 **/
        virtual  void RotateImage(TMulRotation aDirection ,TSize aImageSize,int aAnimationTime) = 0;     
        
		/**
		 * Returns the gesture helper used in coverflow.
		 * This API is required for sharing the gesture helper between the widget and application.
		 * The ownership of the gesture helper lies with the widget.
		 *
		 * @return CGestureHelper Instance of gesture helper created by the widget.

		 * <B>Examples:</b>
		 * @code
		 * Query the gesture helper and use it in application
		 *
		 *     CGestureHelper* gestureInstance = mCoverFlowWidget->Gesturehelper();
		 *     gestureInstance->addObserver(*this);
		 * 
		 * @endcode
		 **/
        virtual  GestureHelper::CGestureHelper* Gesturehelper() = 0;   
                     
     
		/** 
		 * Virtual destructor. 
		 **/
		virtual ~IMulCoverFlowWidget()
			{
			}
		};

	} // namespace Alf

#endif // I_MULCOVERFLOWWIDGET_H
