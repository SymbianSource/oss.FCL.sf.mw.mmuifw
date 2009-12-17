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
* Description:  The implementation for presentation elements.
*
*/

#include <avkon.hrh>
#include <avkon.mbg> 
//Toolkit Includes
// Alfred Client includes
#include <alf/alfenv.h>
#include <alf/alfroster.h>
#include <alf/alfdisplay.h>
#include <alf/alfbitmapprovider.h>
#include <alf/alftexture.h> 
#include <alf/alfbrusharray.h>
#include <alf/alftextvisual.h>
#include <alf/alfevent.h>

//Widget Model Includes
#include <alf/alfwidgetevents.h>
#include <alf/alfwidgetenvextension.h>
#include <alf/ialfmodel.h>

//Avkon Includes
#include <AknUtils.h>
#include <AknsConstants.h>
#include <utf.h> 
#include <math.h>

//slider widget includes
#include <mul/mulevent.h>
#include "mulsliderhorizontal.h"
#include "mulsliderwidget.h"
#include "mulslidercontrol.h"
#include "mulslidermodel.h"
#include "mulsliderdefinitions.h"

#include "mulsliderutils.h"

//Timer function includes
#include "mulsliderlongtaptimer.h"


namespace Alf
    {
    
// Structure to store data for the slider element class    

 struct MulHorizontalSliderDataImpl
     {

     bool mOperation;// Drag state	
     float mTickInPixels;// Tick size in Pixel
     bool mIsLongTapObserverSet;// Is LongTap Event observer set
     CAlfVisual * mVisual;// Stores thje current visual hit 
     TPoint mDragPoint;// Point at which drag started 
     TPoint mLongTapPos;// Point at which button down happened 
     bool mLongTapStarted;// Long tap timer started (true)
     int mDragStartStep;// Tick positon of Drag Start
     int mCurrTick;// Stores current tick
     int mTrackStartPoint;// Slider Track Start Point in pixel
     int mTick;// Stores Tick Size
     bool mLayoutMirrored;// 
     int mdirection;// 0 - nomovement,1 left movement 2 right
     int mImageTextureId; // to differtiate between textures 
     int mTrackTopTextureId;// to store the auto generated texture id's
     int mTrackBottomTextureId;
     int mTrackMiddleTextureId;
     int mMarkerTextureId;
     // Visualization Layouts
     CAlfLayout* mMainLayout;
     CAlfLayout* mBaseSliderLayout;
     CAlfLayout* mSliderCentre;
     CAlfImageVisual* mTrackLeftImage;
     CAlfImageVisual* mTrackMiddleImage;
     CAlfImageVisual* mTrackRightImage;
     CAlfLayout* mSliderCentreGhost;
     CAlfImageVisual* mSliderHandle;
     CAlfLayout* mSliderHandleGhost;
     CAlfImageVisual* mSliderProgressDownload;
     CAlfImageVisual* mSliderProgressCentre;
     CAlfTexture* mTexture;

     MulHorizontalSliderDataImpl()
         {
         mOperation = 0;	
         mTickInPixels = 0;
         mIsLongTapObserverSet = 0;
         mDragPoint.SetXY(0,0) ;
         mLongTapPos.SetXY(0,0);
         mLongTapStarted = 0;
         mDragStartStep = 0;
         mCurrTick = 0;
         mTrackStartPoint = 0;
         mImageTextureId = 0;
         mTrackTopTextureId = 0 ;
         mTrackBottomTextureId = 0;
         mTrackMiddleTextureId = 0;
         mMarkerTextureId = 0;
         mVisual = NULL;
         mMainLayout = NULL;
         mBaseSliderLayout = NULL;  
         mSliderCentre = NULL;
         mSliderHandle = NULL;
         mSliderProgressCentre = NULL;
         mBaseSliderLayout     = NULL;       
         mSliderCentreGhost = NULL;
         mSliderHandleGhost = NULL;   
         mLayoutMirrored = false;  
         mdirection = 0;
         } 

     };
 
 // Bitmaps for slider
_LIT(KAvkonBitmapFile,"\\resource\\apps\\avkon2.mbm"); 
//_LIT(KImageViewerBitmapFile,"\\resource\\apps\\ImageViewer.mif");
// Bitmap Texture ID
const TInt KAlfTrackTopTextureId = 1;
const TInt KAlfTrackBottomTextureId = 2;
const TInt KAlfTrackMiddleTextureId = 3;
const TInt KAlfMarkerTextureId = 4;
//const TInt KAlfMarkerSelectedTextureId = 5;
        
// ---------------------------------------------------------------------------
//  C++ default constructor.
// ---------------------------------------------------------------------------
//
MulSliderHorizontal::MulSliderHorizontal(
                                    CAlfWidgetControl& aControl,
                                    const char* aName, 
                                    AlfCustomInitDataBase* /*aCustomData*/)
    : mData(NULL)
    {
    // Base class (AlfElement) construct
    construct(aControl, aName);
    mData = new (EMM) MulHorizontalSliderDataImpl;
	mPosData.reset(new(EMM)MulSliderPos);
	// Timer for generating tap and hold repeat events  
    mLongTapTimer.reset(new (EMM) MulSliderLongTapTimer(this)); 
    mDragVisual = NULL; 
    mSliderModel = NULL;
    mSliderWidget = NULL; 
    // Store the widget and model pointer
    initializeSliderData();  
    }

// ---------------------------------------------------------------------------
//  Destructor
// ---------------------------------------------------------------------------
//
MulSliderHorizontal::~MulSliderHorizontal()
    {
    if(mData)
        {
       
        (&control().Env())->TextureManager().UnloadTexture(mData->mTrackTopTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mTrackBottomTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mTrackMiddleTextureId);
        // (&control().Env())->TextureManager().UnloadTexture(KAlfMarkerSelectedTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mMarkerTextureId);
        // Visualization Data
        delete mData;
        }
    else
        {

        }

    }//End of Destructor

// ---------------------------------------------------------------------------
// From class MAlfInterfaceBase.
// Getter for interfaces provided by the slider widget.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulSliderHorizontal::makeInterface(
                        const IfId& aType)
    {
    UString param (aType.mImplementationId);
     if (param == IAlfWidgetEventHandler::type().mImplementationId)
        {
        return static_cast<IAlfWidgetEventHandler*>(this);
        }

    else if (param == 
        IMulSliderBaseElementInternal::type().mImplementationId)
        {
        return static_cast<IMulSliderBaseElementInternal*>(this);
        }  
    else
        {
        return AlfElement ::makeInterface(aType);
        }  
   
    }


//--------------------------------------------------------------------------
// APIs from the IMulSliderBaseElement 
//--------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Gets called when the parent layout is relayouted.
// resets all the anchor points and updates the visualization
// ---------------------------------------------------------------------------
//
void MulSliderHorizontal::baseLayoutSizeChanged()
    {
    mData->mMainLayout->ClearFlag(EAlfVisualFlagLayoutUpdateNotification); 
    if(mData->mMainLayout)
        {
        // This is a hack to resolve screen refresh issue
        control().Env().RefreshCallBack(&(control().Env()));
        // Layout the visualization with correct LCT values 
        layoutVisuals(mSliderModel->GetTemplate());
        // Convert all the user defined data to pixels
        ConvertDataToPixels();
        // Set the thumb position to the current tick
        SetThumbPosition();
        if(!mSliderModel->IsLandscape())
            {
            if(mSliderWidget->GetOpacity())
                {
                TAlfTimedValue opacity(1,0);
                mData->mMainLayout->SetOpacity(opacity);
                }
            }
        else
            {
            TAlfTimedValue opacity(0,0);
            mData->mMainLayout->SetOpacity(opacity);
            } 
        } 

    mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);   
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ---------------------------------------------------------------------------
//  


// ---------------------------------------------------------------------------
//  Destructor
// ---------------------------------------------------------------------------
//
void MulSliderHorizontal::setActiveStates(
    unsigned int /*aStates*/)
    {
    //do nothing
    }



// ---------------------------------------------------------------------------
//  initializes Model and Widget Pointer
// ---------------------------------------------------------------------------
//

void  MulSliderHorizontal::initializeSliderData() 
    {
    // initialize the Slider Widget Pointer;
    IAlfWidgetFactory& widgetFactory = 
    AlfWidgetEnvExtension::widgetFactory(control().Env());
    mSliderWidget = static_cast<MulSliderWidget*>(widgetFactory.findWidget(
            control().widget()->widgetName()));

    // initialize the Slider Model Pointer;
    mSliderModel = static_cast<MulSliderModel*>(mSliderWidget->model());
    }
    
// ---------------------------------------------------------------------------
// layoutVisuals
// ---------------------------------------------------------------------------
// 
void MulSliderHorizontal::layoutVisuals(sliderTemplate /*aTemplateId*/)
    {
    // TAknLayoutRect to store the Lct rect 
    TAknLayoutRect layoutRect;
    CAlfControl* ctrl = (CAlfControl*)&control();

    //get the lct rect for mBaseSliderLayout and set it 
    //aacf_slider_pane(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EHSliderPane, 
            mData->mMainLayout,
            KVariety0);
    mData->mBaseSliderLayout->SetRect(
            TRect( TPoint( 0,0 ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ;

    mData->mSliderCentreGhost->SetRect(
            TRect( TPoint( 0,0 ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ;

    //get the lct rect for mSliderCentre and set it 
    //aaslider_bg_pane(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EHSliderCenter,
            mData->mBaseSliderLayout,
            KVariety0);


    mData->mSliderCentre->SetRect(
            TRect( TPoint(layoutRect.Rect().iTl.iX,
                    layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight))) ; 


   
    //TAknLayoutRect handleGostRect = layoutRect;    
    //get the lct rect for mSliderHandle and set it 
    //aacf_slider_pane_g1(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EHSliderHandle,mData->mBaseSliderLayout,KVariety1);

    // This is a correction since the handle LCT data is not correct
   /* mData->mSliderHandle->SetRect(
            TRect( TPoint(handleGostRect.Rect().iTl.iX + 
                    (handleGostRect.Rect().Size().iWidth - 
                            layoutRect.Rect().Size().iWidth)/2    ,
                            handleGostRect.Rect().iTl.iY + 
                            (handleGostRect.Rect().Size().iHeight -
                                    layoutRect.Rect().Size().iHeight)/2 ),
                                    TSize( layoutRect.Rect().Size().iWidth, 
                                            layoutRect.Rect().Size().iHeight ))) ; */
    mData->mSliderHandle->SetRect(
                TRect( TPoint(layoutRect.Rect().iTl.iX,
                        layoutRect.Rect().iTl.iY ),
                                        TSize( layoutRect.Rect().Size().iWidth, 
                                                layoutRect.Rect().Size().iHeight ))) ;
    mData->mSliderHandleGhost->SetRect(
               TRect( TPoint(layoutRect.Rect().iTl.iX,
                       mData->mBaseSliderLayout->Pos().iX.ValueNow() ),
                       TSize( layoutRect.Rect().Size().iWidth, 
                               mData->mBaseSliderLayout->Size().iY.ValueNow() ))) ;
    //get the lct rect for mTrackLeftImage and set it 
    //aaslider_bg_pane_g1(0)  
    layoutRect = MulSliderUtils::GetComponentRect(
            EHSliderLeft,mData->mSliderCentre,0);

    mData->mTrackLeftImage->SetRect(
            TRect( TPoint(layoutRect.Rect().iTl.iX,
                    layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ; 

    //get the lct rect for mTrackMiddleImage and set it 
    //aaslider_bg_pane_g2_copy1(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EHSliderCentre,mData->mSliderCentre,0);

    mData->mTrackMiddleImage->SetRect(
            TRect( TPoint(layoutRect.Rect().iTl.iX,
                    layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth,
                            layoutRect.Rect().Size().iHeight ))) ; 

    //get the lct rect for mTrackRightImage and set it 
    //aaslider_bg_pane_g2(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EHSliderRight,mData->mSliderCentre,0);

    mData->mTrackRightImage->SetRect(
            TRect( TPoint(layoutRect.Rect().iTl.iX,
                    layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ; 


    if(mData->mTrackLeftImage && 
            mData->mTrackMiddleImage && 
            mData->mTrackRightImage)
        {
        setTrackImage();
        }

    if(mData->mSliderHandle)
        {
        mData->mImageTextureId = KAlfMarkerTextureId;
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfMarkerTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|EAlfTextureFlagSkinContent)));

        TAlfImage markerImage(*mData->mTexture );
        mData->mSliderHandle->SetImage(markerImage);

        }

    }

// ---------------------------------------------------------------------------
// Used for setting images for the track 
// ---------------------------------------------------------------------------
//

void MulSliderHorizontal::setTrackImage()
    {
    TInt flags = EAlfTextureFlagSkinContent;
    flags |= EAlfTextureFlagAutoSize;
    CAlfControl* ctrl = (CAlfControl*)&control();
    //create texture for leftimage
    mData->mImageTextureId = KAlfTrackTopTextureId;
    mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
            KAlfAutoGeneratedTextureId/*KAlfTrackTopTextureId*/,
            (MAlfBitmapProvider *)this,
            (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|
                    EAlfTextureFlagSkinContent)));

    TAlfImage trackLeftImage(*mData->mTexture );

    //create texture for middleimage
    mData->mImageTextureId = KAlfTrackMiddleTextureId;
    mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
            KAlfAutoGeneratedTextureId/*KAlfTrackMiddleTextureId*/,
            (MAlfBitmapProvider *)this,
            (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|
                    EAlfTextureFlagSkinContent)));
    TAlfImage trackMiddleImage(*mData->mTexture );
    //create texture for right image
    mData->mImageTextureId = KAlfTrackBottomTextureId;
    mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
            KAlfAutoGeneratedTextureId/*KAlfTrackBottomTextureId*/,
            (MAlfBitmapProvider *)this,
            (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|
                    EAlfTextureFlagSkinContent)));

    TAlfImage trackRightImage(*mData->mTexture );
    // Set Image on visuals
    if(AknLayoutUtils::LayoutMirrored())
        {
        mData->mTrackLeftImage->SetImage(trackRightImage);
        mData->mTrackRightImage->SetImage(trackLeftImage);
        }
    else
        {
        mData->mTrackLeftImage->SetImage(trackLeftImage);
        mData->mTrackRightImage->SetImage(trackRightImage);
        }

    mData->mTrackMiddleImage->SetImage(trackMiddleImage); 

    }//End of setImage
        
// ---------------------------------------------------------------------------
// ProvideBitmapL()
// ---------------------------------------------------------------------------
//
void MulSliderHorizontal::ProvideBitmapL (TInt aId, 
                                         CFbsBitmap *& aBitmap, 
                                         CFbsBitmap *& aMaskBitmap)
    {
    TAknLayoutRect layoutRect;
    TRect rect;
    TSize size;
    switch(mData->mImageTextureId)
        {
        case KAlfTrackTopTextureId:
            {
            AknIconUtils::CreateIconL(
                   aBitmap,
                   aMaskBitmap,
                   KAvkonBitmapFile,
                   EMbmAvkonQgn_graf_nslider_end_left,
                   EMbmAvkonQgn_graf_nslider_end_left_mask );


            layoutRect = MulSliderUtils::GetComponentRect(
                   EHSliderLeft,mData->mSliderCentre,KVariety0);

            AknIconUtils::SetSize( aBitmap,
                   TSize(layoutRect.Rect().Size().iWidth,
                           layoutRect.Rect().Size().iHeight),
                           EAspectRatioNotPreserved);
            mData->mImageTextureId = 0;
            mData->mTrackTopTextureId = aId;
            }
            break;
        case KAlfTrackBottomTextureId:
            {
            AknIconUtils::CreateIconL(
                    aBitmap,
                    aMaskBitmap,
                    KAvkonBitmapFile,
                    EMbmAvkonQgn_graf_nslider_end_right,
                    EMbmAvkonQgn_graf_nslider_end_right_mask );
            layoutRect = MulSliderUtils::GetComponentRect(
                    EHSliderRight,
                    mData->mSliderCentre,
                    KVariety0);

            AknIconUtils::SetSize( aBitmap,
                    TSize(layoutRect.Rect().Size().iWidth,
                            layoutRect.Rect().Size().iHeight),
                            EAspectRatioNotPreserved);

    
            mData->mImageTextureId = 0;
            mData->mTrackBottomTextureId = aId;
            }
            break;
        case KAlfTrackMiddleTextureId:
            {
            AknIconUtils::CreateIconL(
                    aBitmap,
                    aMaskBitmap,
                    KAvkonBitmapFile,
                    EMbmAvkonQgn_graf_nslider_middle,
                    EMbmAvkonQgn_graf_nslider_middle_mask );

            //AknIconUtils::SetSize( aBitmap,TSize(304,14),EAspectRatioNotPreserved);    
            layoutRect = MulSliderUtils::GetComponentRect(
                    EPSliderCentre,
                    mData->mSliderCentre,
                    KVariety0);
            AknIconUtils::SetSize( aBitmap,
                    TSize(layoutRect.Rect().Size().iWidth,
                            layoutRect.Rect().Size().iHeight),
                            EAspectRatioNotPreserved);
            mData->mImageTextureId = 0;
            mData->mTrackMiddleTextureId = aId;
            }
            break;
        case KAlfMarkerTextureId:
            {
            AknIconUtils::CreateIconL(
                    aBitmap,
                    aMaskBitmap,
                    KAvkonBitmapFile,
                    EMbmAvkonQgn_graf_nslider_marker,
                    EMbmAvkonQgn_graf_nslider_marker_mask );            

            layoutRect = MulSliderUtils::GetComponentRect(
                    EHSliderHandle,
                    mData->mBaseSliderLayout,
                    KVariety1);

            AknIconUtils::SetSize( aBitmap,
                    TSize(layoutRect.Rect().Size().iWidth,
                            layoutRect.Rect().Size().iHeight),
                            EAspectRatioNotPreserved);
            mData->mImageTextureId = 0;
            mData->mMarkerTextureId = aId;
            }
        default:
            break;
               
        }
    }
 
    
// ---------------------------------------------------------------------------
//  ConvertDataToPixels
// ---------------------------------------------------------------------------
// 
void MulSliderHorizontal::ConvertDataToPixels()   
    {
    // Calculate the range in pixel values
    int distance = (mData->mSliderCentre->Size().iX.ValueNow() - 
            mData->mSliderHandle->Size().iX.ValueNow());
    if((mSliderModel->MaxRange() - mSliderModel->MinRange()) != 0)
        {
        // Calculate Tick Size in Pixels
        mData->mTickInPixels = 
        (float)distance /(mSliderModel->MaxRange() 
                - mSliderModel->MinRange()) ;                             
        }
    // Get the track start pixel value  
    mData->mTrackStartPoint = mData->mSliderCentre->Pos().iX.ValueNow()   ;  
    // Store current tick
    mData->mCurrTick =  mSliderModel->PrimaryValue() ; 
    mData->mTick = mSliderModel->Tick();
    }
    
    
// ---------------------------------------------------------------------------
//  createVisualization
// ---------------------------------------------------------------------------
// 
void MulSliderHorizontal::createVisualization(sliderTemplate /*aTemplateId*/)
    {
    // Create the visualization
    CAlfControl* ctrl = (CAlfControl*)&control();
    // Visual Hierarchy creation
    //creating mMainLayout 
    mData->mMainLayout = CAlfLayout::AddNewL(*ctrl,NULL); 
    // set the flag for Notifications and mirroring  
    mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);
    mData->mMainLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
    // create mBaseSliderLayout 
    mData->mBaseSliderLayout = CAlfLayout::AddNewL(*ctrl,mData->mMainLayout);
    mData->mBaseSliderLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
    //create dummy layout for slider track
    mData->mSliderCentreGhost = 
    CAlfLayout::AddNewL(*ctrl,mData->mBaseSliderLayout);
    //create dummy layout for slider Thumb
    // create imagevisual for slider track
    mData->mSliderCentre = 
    CAlfLayout::AddNewL(*ctrl,mData->mBaseSliderLayout);
    mData->mTrackLeftImage = CAlfImageVisual::AddNewL(*ctrl,mData->mSliderCentre);
    mData->mTrackMiddleImage = CAlfImageVisual::AddNewL(*ctrl,mData->mSliderCentre);
    mData->mTrackRightImage = CAlfImageVisual::AddNewL(*ctrl,mData->mSliderCentre);

    // create imagevisual for slider thumb 
    mData->mSliderHandle = 
    CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);
    // Attach brush and transfer its ownership to the visual.
    mData->mSliderHandleGhost = 
    CAlfLayout::AddNewL(*ctrl,mData->mBaseSliderLayout);                           

    // Get the widget opacity and apply on root visual
    mSliderWidget->ShowWidget(mSliderWidget->GetOpacity(),0);
    }
    

// ---------------------------------------------------------------------------
//  eventHandlerType          
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerType MulSliderHorizontal::eventHandlerType()
    {
    return IAlfWidgetEventHandler::EPresentationEventHandler;
    }


// ---------------------------------------------------------------------------
//  eventExecutionPhase         
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase MulSliderHorizontal::eventExecutionPhase()
    {
    return IAlfWidgetEventHandler::ETunnellingPhaseEventHandler;
    }

// ---------------------------------------------------------------------------
//  offerEvent
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderHorizontal::offerEvent( CAlfWidgetControl& /*aControl*/, 
                                              const TAlfEvent& aEvent )
    {    
    AlfEventStatus ret = EEventNotHandled;	
    if(!mSliderWidget->IsHidden())
        {	    
        if(aEvent.IsCustomEvent())
            {
            if(aEvent.CustomParameter() == EEventMissedPointerUp)
                {
                mData->mLongTapStarted = false;
                ret=EEventHandled;
                handlePointerUpEvent();
                }
            }
        // handle key events    
        else if(aEvent.IsKeyEvent() &&  mSliderWidget->IsKeyEnabled())
            { 		
            ret = HandleKeyEvents(aEvent);	             
            }    		    
        // Pointer drag events and single tap events      
        else if(aEvent.IsPointerEvent())
            {
            ret = HandlePointerEvents((TAlfEvent *)&aEvent);	    
            }     
        }
    return ret;
    } 
    
    
// ---------------------------------------------------------------------------
//  handleLongTap ( Long TapTimer calls  inside its RUNL)
// ---------------------------------------------------------------------------
// 
 AlfEventStatus MulSliderHorizontal::handleLongTap()
     {
     mData->mMainLayout->ClearFlag(EAlfVisualFlagLayoutUpdateNotification);
     //CAlfVisual * focusvisual = aEvent->Visual();
     AlfEventStatus ret = EEventNotHandled;
     if (mData->mVisual == mData->mSliderCentre || mData->mVisual == mData->mSliderCentreGhost)
         {
         mData->mLongTapStarted = true;
         ret =  trackVisualHit(mData->mVisual);
         // Handle Long tap timer tactile feedback 
         MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
         sldrcntrl.TactileEffectOnTouchandHold();
         }

     //  ret = HandlePointerEvents(aEvent);
     mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);
     return ret;
     }
    
    
// ---------------------------------------------------------------------------
// HandleKeyEvents
// ---------------------------------------------------------------------------
// 
 AlfEventStatus MulSliderHorizontal::HandleKeyEvents(const TAlfEvent& aEvent)
     {
     AlfEventStatus ret = EEventNotHandled;
     int newPos = 0;
     int data = 0;
     if(AknLayoutUtils::LayoutMirrored())
         {
         data= - mData->mTick;
         }
     else
         {
         data= mData->mTick;
         }
     if(aEvent.Code() == EEventKey  )
         {
         if(aEvent.KeyEvent().iScanCode == EStdKeyRightArrow)
             {
             ret = EEventHandled;
             newPos = mSliderModel->PrimaryValue() + data;
             updateModelPrimaryValue(newPos);          
             }
         else if(aEvent.KeyEvent().iScanCode == EStdKeyLeftArrow)
             {
             ret = EEventHandled;
             newPos = mSliderModel->PrimaryValue() - data;
             updateModelPrimaryValue(newPos);
             }
         }
     return ret;
     }
	
	
// ---------------------------------------------------------------------------
//  Sends drag Up or Down event, according to the hit position
//  on track
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderHorizontal::trackVisualHit(CAlfVisual * /*aHitVisual*/)
    {

    //Find the thumb Anchor Layout.
    TAlfTimedPoint thumbVisPos = mData->mSliderHandle->Pos();
    TAlfTimedPoint thumbVisSize = mData->mSliderHandle->Size();
    //Check if click was above/to left or below/to right of 
    //thumb
    if (mData->mLongTapPos.iX  < thumbVisPos.iX.ValueNow())
        {// Pointer down happened left side of Handle 

        int newPos = mSliderModel->PrimaryValue() - mData->mTick;
        if(mData->mLongTapStarted)
            {
            if(mData->mdirection !=2)
                  {
            	// Move Handle 
            	updateModelPrimaryValue(newPos);
            	mData->mdirection = 1;
            	}
            }
        }
    else if (mData->mLongTapPos.iX  > (thumbVisPos.iX.ValueNow() + 
            thumbVisSize.iX.ValueNow()) )
        {
        // Pointer down happened right side of Handle
        int newPos = mSliderModel->PrimaryValue() + mData->mTick;
        if(mData->mLongTapStarted)
            {
            if(mData->mdirection !=1)
                {
            	// Move Handle
            	updateModelPrimaryValue(newPos);
            	mData->mdirection = 2;
            	}
            }  
        }
    else 
        {
        if(mData->mIsLongTapObserverSet)
            {
            // Stop the long tap timer
            mLongTapTimer->Stop();
            mData->mLongTapStarted = false;
            if(AknLayoutUtils::LayoutMirrored() )
                {
                mData->mLongTapPos.iX = mData->mMainLayout->Size().iX.ValueNow() - 
                mData->mLongTapPos.iX;
                }
            mData->mIsLongTapObserverSet = EFalse;  
            if(!mData->mOperation)
                {
                mData->mVisual  =  mData->mSliderHandle;
                initializeThumbDragData(
                        mData->mLongTapPos);
                }  
            }
        else
            {
            //Do Nothing.
            }                            
        }
    return EEventHandled;
    }	

	
// ---------------------------------------------------------------------------
//  handles the pointerDown event
// 
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderHorizontal::handlePointerDownEvent(TAlfEvent * aPntrEvent)
    {
    CAlfVisual * focusvisual = aPntrEvent->Visual();
    AlfEventStatus result = EEventNotHandled;
    if (focusvisual)
        {
        mData->mVisual = focusvisual; 

        if (IsSliderVisual(focusvisual))
            {
            control().processEvent(TAlfEvent(ECustomEventIconClick));  
            control().Display()->Roster().SetPointerEventObservers( 
                    EAlfPointerEventReportDrag ,control());
            if(!mData->mOperation)   
                {  
                mData->mIsLongTapObserverSet = ETrue;
                //mData->mLongTapPos = aPntrEvent->PointerEvent().iParentPosition;
                mirrorPointerPosition(aPntrEvent->PointerEvent().iParentPosition);
                mData->mLongTapStarted = true;
                mLongTapTimer->Start();
                // Dont send custom event at pointer up

                }
            //Tick Movement
            if (focusvisual == mData->mSliderCentre ||
                    focusvisual == mData->mSliderCentreGhost)
                {
                mData->mLongTapStarted = true;
                return trackVisualHit(focusvisual);
                }

            // Drag 
            else if (focusvisual == mData->mSliderHandle ||
                    focusvisual == mData->mSliderHandleGhost )
                {
                // Initialize the drag variables
                if(!mData->mOperation)
                    {
                    initializeThumbDragData(
                            aPntrEvent->PointerEvent().iParentPosition);
                    //Tactile Feedback for thumb touch
                    MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
                    sldrcntrl.TactileEffectOnTouchandHold();
                    }

                result = EEventHandled;
                }
            }

        }
    return result;
    }

	
// ---------------------------------------------------------------------------
// HandlePointerEvents
// ---------------------------------------------------------------------------
// 
 AlfEventStatus MulSliderHorizontal::HandlePointerEvents(TAlfEvent* aEvent)
     {
     AlfEventStatus ret = EEventHandled;
     if (aEvent->PointerEvent().iType == TPointerEvent::EButton1Down)
         {
         mData->mdirection = 0;
         ret = handlePointerDownEvent(aEvent); 
         }
     // Drag Events
     else if (aEvent->PointerEvent().iType == TPointerEvent::EDrag)
         {

         if(!mData->mOperation) 
             {// Drag during tap and hold on track . 
             if(mData->mIsLongTapObserverSet)
                 {// Store the new pointer position
                 //mData->mLongTapPos=aEvent->PointerEvent().iParentPosition;
                 mirrorPointerPosition(aEvent->PointerEvent().iParentPosition);
                 }
             else
                 {
                 //Do Nothing.
                 }    
             }
         else
             {
             // Drag on Handle
             if(mData->mIsLongTapObserverSet &&
                     mData->mVisual && 
                     mData->mSliderHandle &&  
                     mData->mSliderHandleGhost) 
                 {                
                 // Store the new pointer position
                 mirrorPointerPosition(aEvent->PointerEvent().iParentPosition);                    
                 }
             else
                 {
                 // Do nothing
                 }
             int dist;
             // Calculate the distance moved from the drag  start point
             if(AknLayoutUtils::LayoutMirrored())
                 {
                 dist = mData->mDragPoint.iX - 
                 aEvent->PointerEvent().iParentPosition.iX  ;
                 }
             else
                 {
                 dist = aEvent->PointerEvent().iParentPosition.iX - 
                 mData->mDragPoint.iX ;
                 }
             // Calculate the new tick position
             int precision = KRoundupvalue;
             if(dist<0)
                 {
                 precision = -KRoundupvalue;
                 }
             int newPos= (int)(((float)dist / mData->mTickInPixels ) + precision) + 
             mData->mDragStartStep;
             // send tactile sensitive event
             // Update the model hence update visualization
             updateModelPrimaryValue(newPos);
             }
         // Tactile Feedback for Drag
         //Tactile Feedback for thumb drag
         MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
         sldrcntrl.TactileEffectOnDrag(*aEvent);
         }
     else if(aEvent->PointerUp()  )
         {
         mData->mLongTapStarted = false;
         handlePointerUpEvent();
         
         }
     else
         {
         //Do Nothing.
         }

     return ret;
     }
    
    
// ---------------------------------------------------------------------------
//  updateModelPrimaryValue
// ---------------------------------------------------------------------------
//    
void MulSliderHorizontal::updateModelPrimaryValue( int aNewPosValue) 
    {
    // Validate the new tick value
    if(aNewPosValue< mSliderModel->MinRange())
        {
        aNewPosValue = mSliderModel->MinRange();	
        }
    else if(aNewPosValue > mSliderModel->MaxRange())
        {
        aNewPosValue = mSliderModel->MaxRange();	
        }
    if(mSliderModel->PrimaryValue() != aNewPosValue && mData->mTick != 0)
        {
        // Update visualization and model only if data is changed
        mPosData.reset(new(EMM)MulSliderPos);
        mPosData->mPreviousValue = mSliderModel->PrimaryValue()  ;
        mSliderModel->SetPrimaryValue(aNewPosValue);
        mPosData->mCurrentValue =aNewPosValue;
        mData->mCurrTick = aNewPosValue; 

        // Send event to the slider widget        
        TAlfEvent customevent(ETypePrimaryValueChange,
                (uint)mPosData.get());
        control().processEvent(customevent);
        }
    }
    

// ---------------------------------------------------------------------------
//  snapPrimaryValueToTicks
// ---------------------------------------------------------------------------
//    
void MulSliderHorizontal::snapPrimaryValueToTicks()
    {
    // If the position is not a multiple of ticks then 
    //snap to the nearest tick multiple

    int currHandlePos = mSliderModel->PrimaryValue() - mSliderModel->MinRange();
    int tick = mSliderModel->Tick();
    int newPos = currHandlePos + mSliderModel->MinRange();
    if(mSliderModel->PrimaryValue() < mSliderModel->MaxRange())
        {         
        int diff = 0;
        if(tick > 0) //model
            {
            diff = currHandlePos - ((currHandlePos /tick) * tick);
            }
        if(diff !=0)
            {            
            newPos = currHandlePos + tick - diff + mSliderModel->MinRange();            
            }        
        }

    // Update model
    updateModelPrimaryValue(newPos);
    } 
    
      
// ---------------------------------------------------------------------------
//  handlePointerUpEvent
// 
// ---------------------------------------------------------------------------
//
void MulSliderHorizontal::handlePointerUpEvent()
    {
    // if thumb or track is clicked, change the graphics
    // Send event if single click has hapened 

    snapPrimaryValueToTicks();
    
    if(mData->mOperation)
        {
        //Somehow missed the Up event?
        stopDrag();
        //Tactile Feedback For Thumb Release
        MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
        sldrcntrl.TactileEffectOnRelease();
        }
    if(mData->mIsLongTapObserverSet)
        {
        // Stop the long tap timer
        mLongTapTimer->Stop();
        // Un-register with the roster for long tap events 
        control().Display()->Roster().SetPointerEventObservers(0,
                control());
        mData->mIsLongTapObserverSet = EFalse;    
        }
    else
        {
        //Do Nothing.
        }    
    }
	
	
// ---------------------------------------------------------------------------
// UpdateVisualization
// ---------------------------------------------------------------------------
//	  
void MulSliderHorizontal::updateVisualization()
    {
    ConvertDataToPixels();
    SetThumbPosition();
    }
    
// ---------------------------------------------------------------------------
//Mirror pointer position
// ---------------------------------------------------------------------------
//	  
void MulSliderHorizontal::mirrorPointerPosition(const TPoint& aPointerPos )
    {    
    mData->mLongTapPos = aPointerPos;
    if(AknLayoutUtils::LayoutMirrored())
        {
        mData->mLongTapPos.iX  = mData->mMainLayout->Size().iX.ValueNow() -
        mData->mLongTapPos.iX  ;
        }
    }    
// ---------------------------------------------------------------------------
// SetThumbPosition
// ---------------------------------------------------------------------------
//

void MulSliderHorizontal::SetThumbPosition()
    {
    // Current primary value in pixels
    int newTickVal = (mSliderModel->PrimaryValue() - mSliderModel->MinRange()) * 
    mData->mTickInPixels;
    // Handle
    int handleY = mData->mSliderHandle->Pos().iY.ValueNow();
    int ghostHandleY = mData->mSliderHandleGhost->Pos().iY.ValueNow();
    // Handle new pos
    int widthDiff = (mData->mSliderHandleGhost->Size().iX.ValueNow() - 
            mData->mSliderHandle->Size().iX.ValueNow())/2;
    TAlfRealPoint pnt(mData->mTrackStartPoint + newTickVal,handleY); 
    // Ghost Handle ( Extended) new pos 
    TAlfRealPoint pnt1(mData->mTrackStartPoint - 
            widthDiff/2 + newTickVal,ghostHandleY); 
    // Set the position
    mData->mSliderHandle->SetPos(pnt,0);
    mData->mSliderHandleGhost->SetPos(pnt1,0);

    // Update visualization variables
    mData->mCurrTick = mSliderModel->PrimaryValue();
    }


// ---------------------------------------------------------------------------
// accept
// ---------------------------------------------------------------------------
// 
bool MulSliderHorizontal::accept( CAlfWidgetControl& /*aControl*/, 
                                 const TAlfEvent& aEvent ) const
	{
	 // Accept only key or pointer events.
	 if(aEvent.IsKeyEvent()||aEvent.IsPointerEvent())
		 {
		 return true;	
		 }
	 else
		 {
		 return false;
		 } 
	}


// ---------------------------------------------------------------------------
//initializeThumbDragData
// ---------------------------------------------------------------------------
//    
bool MulSliderHorizontal::initializeThumbDragData( const TPoint& aPointerPos )
    {       
    TRAPD(err,control().Display()->Roster().SetPointerDragThreshold(control(),
            TAlfXYMetric(TAlfMetric(KXToleranceInPixel,EAlfUnitPixel),
                    TAlfMetric(KYToleranceInPixel,EAlfUnitPixel))) );  

    if(mData->mVisual && err == KErrNone)
        {
        mData->mOperation = EMoveItem;
        mData->mDragPoint = aPointerPos;
        mData->mDragStartStep = mSliderModel->PrimaryValue();

        }
    else
        {
        // Do Nothing
        }   
    return ETrue;  
    }

    
// ---------------------------------------------------------------------------
// stopDrag
//  
// ---------------------------------------------------------------------------
//
bool MulSliderHorizontal::stopDrag()
    {
    TRAPD(err,
            control().Display()->Roster().SetPointerEventObservers(
                    0, 
                    control()));
    if(err == KErrNone)
        {
        mData->mVisual = NULL;
        mData->mOperation = ENop;
        return ETrue;
        }
    return EFalse; 
    }
 
    
        

// ---------------------------------------------------------------------------
// IsSliderVisual
// 
// ---------------------------------------------------------------------------
// 
bool MulSliderHorizontal::IsSliderVisual(CAlfVisual *aVisual)
    {
    if(aVisual == mData->mSliderCentre||
            aVisual == mData->mTrackLeftImage ||
            aVisual == mData->mTrackMiddleImage ||
            aVisual == mData->mTrackRightImage ||
            aVisual == mData->mSliderCentreGhost||
            aVisual == mData->mSliderHandle||
            aVisual == mData->mSliderHandleGhost)
        {
        return true;
        }
    return false;   
    }      	   	
      	   
    void MulSliderHorizontal::updateTextVisualization()	
    {
    	
    }
    }//namespace Alf
    
//End Of File
