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
* Description: Implementation for presentation element- progressbar.
*
*/

//Toolkit Includes
#include <alf/alfimagevisual.h>
#include <alf/alfevent.h>

// Alfred Client includes
#include <alf/alfenv.h>
#include <alf/alftexture.h> 
#include <alf/alftextvisual.h>
#include <alf/alflayout.h>
#include <aknconsts.h>
#include <avkon.mbg>

//Widget Model Includes
#include <alf/alfwidgetenvextension.h>

//Avkon Includes
#include <AknUtils.h>
#include <AknsConstants.h>
#include <utf.h>

//OpenC Includes

#include <math.h>

//slider widget includes
#include "mulprogressbarslider.h"
#include "mulutility.h"
#include "mulsliderutils.h"
#include "mulsliderwidget.h"
#include "mulslidermodel.h"
#include "mulsliderdefinitions.h"

// Textureid's used for comparison 
const TInt KAlfProgressBarTextureId = 1;
const TInt KAlfFrameLeftTextureId = 2;
const TInt KAlfFrameCentreTextureId = 3;
const TInt KAlfFrameRightTextureId = 4;
namespace Alf
    {
// Structure to store data for the progressbar element class  
struct MulSliderProgressBarImpl
	{		
	int mMaxRange; //Store Max range for progressbar from model   
    int mMinRange;  //Store Min range for progressbar from model
    int mTempMin; //Temporary stores the min range 
    int mTempMax; //Temporary stores the max range  
    int mTick;    // Store Tick value for progressbar from model 
    int mImageTextureId; // to differtiate between textures
    bool mOrientationLandScape; // check for landscape 
  	sliderTemplate mTemplateId; //slider template id
  	CAlfLayout* mMainLayout; //slider main layout 
  	int mProgressBarTextureId;// to store the auto generated texture id's
  	int mFrameLeftTextureId ;
  	int mFrameCentreTextureId;
  	int mFrameRightTextureId ;
  	CAlfLayout* mBaseSliderLayout;
  	CAlfImageVisual* mSliderProgressCentre;
	CAlfImageVisual* mSliderSecProgressCentre;
    CAlfImageVisual* mFrameLeftImage;
    CAlfImageVisual* mFrameRightImage;
    CAlfImageVisual* mFrameCentreImage;
    CAlfTexture* mTexture;
    MulSliderProgressBarImpl()
    {
    mTick = 0;    
    mMaxRange = 0;
    mMinRange = 0; 
    mTempMax = 0;
    mTempMin = 0; 
    mImageTextureId = 0;
    mTemplateId = ESliderTemplateNone;
    mProgressBarTextureId = 0;
    mFrameLeftTextureId = 0;
    mFrameCentreTextureId = 0;
    mFrameRightTextureId = 0;
    mBaseSliderLayout = NULL;
    mSliderProgressCentre = NULL;
    mSliderSecProgressCentre = NULL;
    mOrientationLandScape = false;
	}	    
	};  
// ---------------------------------------------------------------------------
//  MulSliderProgressive default constructor.
// ---------------------------------------------------------------------------
//
MulSliderProgressive::MulSliderProgressive(
                                    CAlfWidgetControl& aControl,
                                    const char* aName ,
                                    AlfCustomInitDataBase* /*aCustomData*/)
    : mData(NULL)
    {
    // Base class (AlfElement) construct
    construct(aControl, aName);
    mData = new (EMM) MulSliderProgressBarImpl;
    iBitmap = new (EMM) CFbsBitmap;
    // initializes the mSlliderModel and mSliderWidget  
    initializeSliderData();
    
    }
// ---------------------------------------------------------------------------
//  Destructor
// ---------------------------------------------------------------------------
//
MulSliderProgressive::~MulSliderProgressive()
    {
    // Visualization Data
    if(mData)
        {
        (&control().Env())->TextureManager().UnloadTexture(mData->mProgressBarTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mFrameLeftTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mFrameRightTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mFrameCentreTextureId);

        delete mData;
        delete iBitmap;
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
IAlfInterfaceBase* MulSliderProgressive::makeInterface(
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
        
    IAlfInterfaceBase* interface = 0;
    interface = AlfElement::makeInterface(aType);
    return interface;
    }

// ---------------------------------------------------------------------------
// Gets called when the parent layout is relayouted.
// resets all the anchor points and updates the visualization
// ---------------------------------------------------------------------------
//
void MulSliderProgressive::baseLayoutSizeChanged()
    {
	mData->mMainLayout->ClearFlag(EAlfVisualFlagLayoutUpdateNotification);
	control().Env().RefreshCallBack(&(control().Env()));
	// Layout the visualization w.r.t. mainlayout 
	layoutVisuals(mSliderModel->GetTemplate());
	if(mSliderModel->IsLandscape())
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
    mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification); 
    updateVisualization();
    }    
// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// setActiveStates
// ---------------------------------------------------------------------------
void MulSliderProgressive::setActiveStates(
    unsigned int /*aStates*/)
    {
  		  //do nothing
    }

// ---------------------------------------------------------------------------
//  initializes Model and Widget Pointer
// ---------------------------------------------------------------------------
//
void  MulSliderProgressive::initializeSliderData()
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
//  Bitmap provider  
// ---------------------------------------------------------------------------
//
void MulSliderProgressive::ProvideBitmapL (TInt aId, 
                                           CFbsBitmap *& aBitmap, 
                                           CFbsBitmap *& aMaskBitmap)
    {
    TAknLayoutRect layoutRect;
   
    switch(mData->mImageTextureId)
        {
        case KAlfProgressBarTextureId:
            {
            AknIconUtils::CreateIconL(
                    aBitmap, aMaskBitmap, KAvkonBitmapFile,
                    EMbmAvkonQgn_graf_bar_progress,
                    EMbmAvkonQgn_graf_bar_progress_mask ); 

            TInt aHandle = aBitmap->Handle();

            iBitmap->Duplicate(aHandle);

            AknIconUtils::SetSize( aBitmap,TSize(50,14),EAspectRatioNotPreserved);  
            mData->mImageTextureId = 0;
            mData->mProgressBarTextureId = aId;
            }
            break;
        case KAlfFrameLeftTextureId:
            {
            AknIconUtils::CreateIconL( 
                    aBitmap, aMaskBitmap, KAvkonBitmapFile,
                    EMbmAvkonQgn_graf_bar_frame_side_l,
                    EMbmAvkonQgn_graf_bar_frame_side_l_mask );

            layoutRect = MulSliderUtils::GetComponentRect(
                    EPSliderLeft,
                    mData->mMainLayout,
                    KVariety0);

            AknIconUtils::SetSize( aBitmap,TSize(layoutRect.Rect().Size().iWidth,
                    layoutRect.Rect().Size().iWidth),
                    EAspectRatioNotPreserved);
           
            //  AknIconUtils::SetSize( aBitmap,TSize(7,17),EAspectRatioNotPreserved);
    
            mData->mImageTextureId = 0;
            mData->mFrameLeftTextureId = aId;
            }
            break;
        case KAlfFrameRightTextureId:
            {
            AknIconUtils::CreateIconL( 
                            aBitmap, aMaskBitmap, KAvkonBitmapFile,
                    EMbmAvkonQgn_graf_bar_frame_side_r,
                    EMbmAvkonQgn_graf_bar_frame_side_r_mask );

            layoutRect = MulSliderUtils::GetComponentRect(
                    EPSliderRight,
                    mData->mMainLayout,
                    KVariety0);    

            AknIconUtils::SetSize( aBitmap,TSize(layoutRect.Rect().Size().iWidth,
                    layoutRect.Rect().Size().iWidth),
                    EAspectRatioNotPreserved);

            //  AknIconUtils::SetSize( aBitmap,TSize(7,17),EAspectRatioNotPreserved);
            mData->mImageTextureId = 0;
            mData->mFrameRightTextureId = aId;
            }
            break;
        case KAlfFrameCentreTextureId:
            {
            AknIconUtils::CreateIconL( 
                   aBitmap, aMaskBitmap, KAvkonBitmapFile,
                   EMbmAvkonQgn_graf_bar_frame_center,
                   EMbmAvkonQgn_graf_bar_frame_center_mask );

           layoutRect = MulSliderUtils::GetComponentRect(
                   EPSliderCentre,
                   mData->mMainLayout,
                   KVariety0);

            AknIconUtils::SetSize( aBitmap,TSize(layoutRect.Rect().Size().iWidth,
                   layoutRect.Rect().Size().iWidth),
                   EAspectRatioNotPreserved);

            //  AknIconUtils::SetSize( aBitmap,TSize(308,17),EAspectRatioNotPreserved);
            mData->mImageTextureId = 0;
            mData->mFrameCentreTextureId = aId;
            }
        default:
            break;
               
        }
   
    }
//----------------------------------------------------------------------------
// layoutVisuals
//----------------------------------------------------------------------------
void MulSliderProgressive::layoutVisuals(sliderTemplate aTemplateid)
    {
      
       TAknLayoutRect layoutRect;
       mData->mTemplateId = aTemplateid;
       //Get the mainlayout rect 
       int mainX = mData->mMainLayout->Pos().iX.ValueNow();
       int mainY = mData->mMainLayout->Pos().iY.ValueNow();
       int mainWidth = mData->mMainLayout->Size().iX.ValueNow();
       int mainHeight = mData->mMainLayout->Size().iY.ValueNow();
       if (mData->mTemplateId == ESliderTemplate7 ) 
           {
    
       
        mData->mBaseSliderLayout->SetRect(
                TRect( TPoint(0,0 ),
                        TSize( mainWidth, mainHeight ))) ; 

        layoutRect = MulSliderUtils::GetComponentRect(
                EPSliderLeft,
                mData->mMainLayout,
                KVariety0);	   

        mData->mSliderProgressCentre->SetRect(
                TRect( TPoint(0,0),
                        TSize( 0, layoutRect.Rect().Size().iHeight ))) ; 

        mData->mFrameLeftImage->SetRect(
                TRect( TPoint(layoutRect.Rect().iTl.iX,
                        layoutRect.Rect().iTl.iY ),
                        TSize( layoutRect.Rect().Size().iWidth, 
                                layoutRect.Rect().Size().iHeight ))) ; 


        layoutRect = MulSliderUtils::GetComponentRect(
                EPSliderCentre,
                mData->mMainLayout,
                KVariety0);

        mData->mFrameCentreImage->SetRect(
                TRect( TPoint(layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY ),
                        TSize( layoutRect.Rect().Size().iWidth, 
                                layoutRect.Rect().Size().iHeight ))) ; 


        layoutRect = MulSliderUtils::GetComponentRect(
                EPSliderRight,
                mData->mMainLayout,
                KVariety0);
        mData->mFrameRightImage->SetRect(
                TRect( TPoint(layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY ),
                        TSize( layoutRect.Rect().Size().iWidth,
                                layoutRect.Rect().Size().iHeight ))) ; 
           }
       
      
    }

//----------------------------------------------------------------------------
// createVisualization
//----------------------------------------------------------------------------
void MulSliderProgressive::createVisualization(sliderTemplate templateid)
    {
    mData->mTemplateId = templateid;
    CAlfControl  *ctrl = &control();

    // Visual Hierarchy creation
    mData->mMainLayout = CAlfLayout::AddNewL(*ctrl,NULL);
    mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);
    mData->mMainLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
    mData->mBaseSliderLayout = CAlfLayout::AddNewL(*ctrl, mData->mMainLayout);
    mData->mBaseSliderLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
    mData->mSliderProgressCentre = CAlfImageVisual::AddNewL(
            *ctrl,mData->mBaseSliderLayout);
    mData->mFrameLeftImage = CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);
    mData->mFrameCentreImage = CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);
    mData->mFrameRightImage = CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);

    if(mData->mFrameLeftImage)
        {
        mData->mImageTextureId = KAlfFrameLeftTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfFrameLeftTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagAutoSize|
                        EAlfTextureFlagSkinContent)));

        int height = mData->mTexture->Size().iHeight;
        int width = mData->mTexture->Size().iWidth;

        TAlfImage aImageLeft(*mData->mTexture );
        mData->mFrameLeftImage->SetImage(aImageLeft);
        //mData->mFrameLeftImage->SetOpacity(0.5);
        }
    if(mData->mFrameCentreImage)
        {
        mData->mImageTextureId = KAlfFrameCentreTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfFrameCentreTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagAutoSize|EAlfTextureFlagSkinContent)));

        int height = mData->mTexture->Size().iHeight;
        int width = mData->mTexture->Size().iWidth;
        TAlfImage aImageCentre(*mData->mTexture );
        mData->mFrameCentreImage->SetImage(aImageCentre);
        }

    if(mData->mFrameRightImage)
        {
        mData->mImageTextureId = KAlfFrameRightTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfFrameRightTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagAutoSize|EAlfTextureFlagSkinContent)));

        int height = mData->mTexture->Size().iHeight;
        int width = mData->mTexture->Size().iWidth;


        TAlfImage aImageRight(*mData->mTexture );

        mData->mFrameRightImage->SetImage(aImageRight);
        }

    if(mData->mSliderProgressCentre)
        {
        mData->mImageTextureId = KAlfProgressBarTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfProgressBarTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagAutoSize|EAlfTextureFlagSkinContent)));
        int height = mData->mTexture->Size().iHeight;
        int width = mData->mTexture->Size().iWidth;
        TAlfImage aImage(*mData->mTexture );
        mData->mSliderProgressCentre->SetImage(aImage);
        }

    }
	
// ---------------------------------------------------------------------------
// OfferEvent
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderProgressive::offerEvent( 
    CAlfWidgetControl&/* aControl*/, 
    const TAlfEvent& /*aEvent*/ )
    {	
     // progress bar doesnt have any event handling 
	 return EEventNotHandled;
    }  

// ---------------------------------------------------------------------------
// UpdateVisualization
// ---------------------------------------------------------------------------
void MulSliderProgressive::updateVisualization()
    {
    // Call to function to set relative 
	//values for ranges if min range is not zero
    NormalizeRange();
    int currTick = mSliderModel->PrimaryValue();
    // Calculating the relative currtick if the MinRange is not zero.
    if (currTick >0 )
    	{
    	currTick = currTick - mData->mTempMin;	
    	}
    // Set the new thumbimage position.
    SetTickPosition(currTick);
    }
// ---------------------------------------------------------------------------
// NormalizeRange
// ---------------------------------------------------------------------------
 void MulSliderProgressive::NormalizeRange()   
	{
 	
	mData->mMaxRange = mSliderModel->MaxRange();
	mData->mMinRange = mSliderModel->MinRange();
	mData->mTick = mSliderModel->Tick();
	mData->mTempMax = mData->mMaxRange;
	mData->mTempMin = mData->mMinRange;
	mData->mMaxRange = mData->mMaxRange - mData->mMinRange;
	mData->mMinRange = 0;		 	
    }

// ---------------------------------------------------------------------------
// accept
// ---------------------------------------------------------------------------
// 
bool MulSliderProgressive::accept(
    CAlfWidgetControl& /*aControl*/, 
    const TAlfEvent& /*aEvent*/) const
	{
		 return false;	
	}
//---------------------------------------------------------------------------
//MulSliderProgressive eventHandlerType
//-------------------------------------------------------------------------
IAlfWidgetEventHandler::AlfEventHandlerType MulSliderProgressive::
    eventHandlerType()
    {
    return IAlfWidgetEventHandler::EPresentationEventHandler;
    }

//---------------------------------------------------------------------------
//MulSliderProgressive eventExecutionPhase
//-------------------------------------------------------------------------
IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase MulSliderProgressive::
    eventExecutionPhase()
    {
    return IAlfWidgetEventHandler::ETunnellingPhaseEventHandler;
    }
// ---------------------------------------------------------------------------
// SetTickPosition
// ---------------------------------------------------------------------------
void MulSliderProgressive::SetTickPosition(int aCurrTick)
    { 
    //Getting slider width 
    int sliderWidth = mData->mBaseSliderLayout->Size().Target().iX;
    //Getting the slider progress height 
    int sliderProgressHeight = mData->mSliderProgressCentre->Size().Target().iY;
    //calculating the current postion of slider progress width
    int curpos =(sliderWidth * aCurrTick)/(mData->mMaxRange - mData->mMinRange);
    //setting the slider progress width
    mData->mSliderProgressCentre->SetSize(
            TAlfRealPoint(curpos,sliderProgressHeight),0);
    int sliderProgressWidth = mData->mSliderProgressCentre->Size().Target().iX;
    AknIconUtils::SetSize(iBitmap,TSize(sliderProgressWidth,sliderProgressHeight),EAspectRatioNotPreserved);  
    }


//---------------------------------------------------------------------------
//updateTextVisualization
//-------------------------------------------------------------------------
   void MulSliderProgressive::updateTextVisualization()	
    {
    // do nothing 
    }	
 }//namespace Alf

//End Of File
