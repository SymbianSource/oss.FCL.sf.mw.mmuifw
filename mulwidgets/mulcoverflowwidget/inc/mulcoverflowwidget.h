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
* Description:  widget header for CoverFlow
*
*/

// This file defines the API for MulCoverFlowWidget.dll

#ifndef __MULCOVERFLOWWIDGET_H__
#define __MULCOVERFLOWWIDGET_H__


//  Include Files
// Mul headers
#include <mul/imulcoverflowwidget.h>

// Alf headers
#include "alf/alfwidget.h"

//Gesture Helper namespace 
namespace GestureHelper
    {
    class CGestureHelper;
    }

namespace Alf
    {
    
//Forward Class Definitions  
class MulBaseElement;
class MulCoverFlowControl;

/// @bug comment:avanhata:3/7/2008 It would be good to describe the responsibilities and 
/// design of the class here. The comment about method is incorrect, as it is the abstract
/// interface that the application uses
/**
 * Widget Implementation class. The Widget Class is responsible for the creation of the control and elements.
 */

class MulCoverFlowWidget : public AlfWidget, 
                           public IMulCoverFlowWidget
    {
public:    //Constructor and destructor
    
   /**
    * C++ constructor.
    */
    MulCoverFlowWidget(  CAlfEnv& aEnv, const char* aWidgetId, IAlfContainerWidget& aContainerWidget );
    
    /**
     * Destructor.
     */
    ~MulCoverFlowWidget();

public: // From AlfWidget Common Property related apis
    
    IAlfContainerWidget* parent() const;
    
    CAlfWidgetControl* control() const;
    
    void setControl( CAlfWidgetControl* aControl,bool aDeletePreviousControl  );
    
    IAlfModel* model();
    
    void setModel( IAlfModel* aModel,bool aTakeOwnership );
    
    const char* widgetName() const;
    
    void setChildFocus( bool aFocus );
    
    void setPresentation( const char* aFilePath );

public: //From MAlfInterfaceBase.
    
    IAlfInterfaceBase* makeInterface( const IfId& aType );
    
public: //From IMulCoverFlowWidget.
    
    void SetBackground( const TRgb& aColor );
    
    void SetBackground(const TAknsItemID& aItemID );
    
    void SetEmptyText( const UString& aDefaultText );
    
    void SetUIMode(bool UIOn=false, int aTransition=0 );
    
    void SetDefaultImage(int aTextureId);
    
	void RotateImage(TMulRotation aDirection,TSize aImageSize,int aAnimationTime);
	
	GestureHelper::CGestureHelper* Gesturehelper();   
    
public: //From IMulWidget

 	void SetFlags( unsigned int aAnyFlag );

 	void ClearFlags( unsigned int aAnyFlag ) ;
 	
 	unsigned int GetFlags() const;

 	bool IsFlagSet( TMulWidgetPropertiesFlag aAnyFlag ) const;
 	
 	void SetModel( IMulModelAccessor* aAccessor, bool aFlag=true );

 	void SetRect( int aLeftMargin, int aTopmargin, int aHeight, int aWidth, int aTransitionTime = 0 );
    
	void ShowWidget( bool aShow, int aTransitionTime = 0 ) ;
	
	void GetIconSize( mulwidget::TLogicalTemplate aTemplateId, 
		    mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth);
	    
	void GetIconSize(mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth);

	void AddEventHandler( IAlfWidgetEventHandler& aObserver );
    
    void RemoveEventHandler( IAlfWidgetEventHandler& aObserver );

	const CAlfLayout& ContainerLayout(); 
	
	void SetMarkMode( bool aFlag );
		
	bool IsMarkMode()const ;

	void SetReorderMode( TMulReOrderMode /*aReorderMode*/);
	
	bool IsReorderModeOn() const;
	
	
	void SetOverlay( TMulAttributeDataType aType, const UString& aAttribute, const UString& aFormat ) ;
	
	void SetAnimationTime( TMulVisualEffect aVisualEffect, int aTransitionTime = 0 ) ;
	
	void SetVisualColor( TMulVisualType aVisualType , const TRgb& aColor );
         
    
private:    // Class Data
    
    unsigned int mWidgetPropertyFlag;	
    IMulModelAccessor* mMulModelAccessor; // doesn't won it.
    MulBaseElement* mBaseElement; // doesn't own it.
    MulCoverFlowControl* mCoverflowControl; //// doesn't own it.
    bool mTakesModelOwnership;
	
    };

    } // namespace Alf
    
#endif  // __MULCOVERFLOWWIDGET_H__

//End of file
