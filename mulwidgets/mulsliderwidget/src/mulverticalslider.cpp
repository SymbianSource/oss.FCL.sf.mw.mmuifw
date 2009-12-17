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
* Description: Implementation for presentation element - vertical.
*
*/

//Toolkit Includes
#include <avkon.mbg> 
#include <mulsliderwidget.mbg>

// Alfred Client includes
#include <alf/alfenv.h>
#include <alf/alfroster.h>
#include <alf/alfdisplay.h>
#include <alf/alfbrusharray.h>
#include <alf/alftextvisual.h>
#include <alf/alfevent.h>
//#include <osn/alfptrvector.h>
#include <alf/alfframebrush.h>

//Widget Model Includes
#include <alf/alfwidgetevents.h>
#include <alf/alfwidgetenvextension.h>
#include <alf/ialfmodel.h>

//Avkon Includes
#include <AknUtils.h>
#include <AknsConstants.h>
#include <utf.h>
#include <aknlayoutscalable_uiaccel.cdl.h>
#include <avkon.hrh>
//#include <math.h>

//slider widget includes
#include "mulslidervertical.h"
#include <mul/mulevent.h>
#include "mulsliderwidget.h"
#include "mulslidercontrol.h"
#include "mulslidermodel.h"
#include "mulsliderdefinitions.h"
#include "mulsliderutils.h"

//Timer function includes
#include "mulsliderlongtaptimer.h"
#include <alf/alfgradientbrush.h>
namespace Alf
    {    
// Structure to store data for the slider element class
struct MulVerticalSliderDataImpl
    {
    bool mOperation;// Drag state
    bool mSliderHit;// button down has happened on slider visual
    float mTickInPixels;// Tick size in Pixel
    int mRangeInPixels;// Range in Pixel
    int mCurrTickInPixels;// Current Tick posion in pixels
    CAlfVisual * mVisual;// Stores the current visual hit 
    TPoint mDragPoint;// Point at which drag started 
    TPoint mLongTapPos;// Point at which button down happened 
    int mDragStartStep;// Tick positon of Drag Start
    int mCurrTick;// Stores current tick
    int mTick;// Stores Tick Size
    int mTrackStartPoint;// Slider Track Start Point in pixel
    int mHandleGhostHieghtDelta;
    bool mOrientationLandScape;
    bool mHasBackground; // for background only created once 
    int mdirection;// 0 - nomovement,1 up movement 2 down
    bool mLayoutMirrored;// RTL if True else LTR
    int mImageTextureId; // to differtiate between textures 
    int mTrackTopTextureId ;// to store the auto generated texture id's
    int mTrackBottomTextureId;
    int mTrackMiddleTextureId;
    int mMarkerTextureId;
    int mZoomInTextureId;
    int mZoomOutTextureId;
    
    CAlfLayout* mMainLayout;
    CAlfLayout* mBaseSliderLayout;// Slider Background
    CAlfTexture* mTexture;
    CAlfLayout* mSliderCentre;// Track layout
    CAlfLayout* mSliderCentreGhost;// Extended touch are for track
    CAlfImageVisual* mTrackTopImage;
    CAlfImageVisual* mTrackMiddleImage;
    CAlfImageVisual* mTrackEndImage;
    CAlfImageVisual* mSliderHandle;// Slider Handle
    CAlfLayout* mSliderHandleGhost;//Extended touch are for handle
    CAlfVisual* mSliderPercentAudio;//Visula showing percentage
    CAlfImageVisual* mImagePlus;// Zoom in
    CAlfImageVisual* mImageMinus;// Zoom out


    MulVerticalSliderDataImpl()
        {
        mOperation = false;
        mSliderHit = false;
        mTickInPixels = 0;
        mRangeInPixels = 0;
        mCurrTickInPixels = 0;
        mVisual = NULL;
        mDragPoint.SetXY(0,0);
        mLongTapPos.SetXY(0,0);
        mDragStartStep = 0;
        mCurrTick = 0;
        mTick = 0;
        mTrackStartPoint = 0;
        mImageTextureId = 0;
        mTrackTopTextureId = 0;
        mTrackBottomTextureId = 0;
        mTrackMiddleTextureId = 0;
        mMarkerTextureId = 0;
        mZoomInTextureId = 0;
        mZoomOutTextureId = 0;
        mMainLayout = NULL;
        mBaseSliderLayout = NULL;
        mSliderCentre = NULL;
        mTrackTopImage = NULL;
        mTrackMiddleImage = NULL;
        mTrackEndImage = NULL;
        mSliderCentreGhost = NULL;
        mSliderHandleGhost = NULL;
        mSliderHandle = NULL;
        mSliderPercentAudio = NULL;
        mImagePlus = NULL;
        mImageMinus = NULL;
        mHandleGhostHieghtDelta = 0;   
        mOrientationLandScape = false;
        mdirection = 0;
        mLayoutMirrored = false;
        mHasBackground = false;
        } 

    };
    
// bitmap file path
_LIT(KAvkonBitmapFile,"\\resource\\apps\\avkon2.mbm");
_LIT(KImageViewerBitmapFile,"\\resource\\apps\\ImageViewer.mif");

const TInt KAlfTrackTopTextureId = 1;
const TInt KAlfTrackBottomTextureId = 2;
const TInt KAlfTrackMiddleTextureId = 3;
const TInt KAlfMarkerTextureId = 4;
const TInt KAlfZoomInTextureId = 5;
const TInt KAlfZoomOutTextureId = 6;
        
// ---------------------------------------------------------------------------
//  C++ default constructor.
// ---------------------------------------------------------------------------
//
MulSliderVertical::MulSliderVertical(
                                    CAlfWidgetControl& aControl,
                                    const char* aName, 
                                    AlfCustomInitDataBase* /*aCustomData*/)
    : mData(NULL)
        {
        // Base class (AlfElement) construct
        construct(aControl, aName);
        mSliderModel = NULL;
        mSliderWidget = NULL;
        mData = new (EMM) MulVerticalSliderDataImpl;
        mPosData.reset(new(EMM)MulSliderPos);

        // Timer for generating tap and hold repeat events  
        mLongTapTimer.reset(new (EMM) MulSliderLongTapTimer(this)); 
        initializeSliderData();
        }

// ---------------------------------------------------------------------------
//  Destructor
// ---------------------------------------------------------------------------
//
MulSliderVertical::~MulSliderVertical()
    {
    // Visualization Data
    if(mData)
        {
        (&control().Env())->TextureManager().UnloadTexture(mData->mTrackTopTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mTrackBottomTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mTrackTopTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mMarkerTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mZoomInTextureId);
        (&control().Env())->TextureManager().UnloadTexture(mData->mZoomOutTextureId);
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
IAlfInterfaceBase* MulSliderVertical::makeInterface(
                        const IfId& aType)
    {
    IAlfInterfaceBase* interface = 0;
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
        interface = AlfElement::makeInterface(aType);   
        }
    return interface;
    }

//--------------------------------------------------------------------------
// APIs from the IMulSliderBaseElement 
//--------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Gets called when the parent layout is relayouted.
// resets all the anchor points and updates the visualization
// ---------------------------------------------------------------------------
//
void MulSliderVertical::baseLayoutSizeChanged()
    {
    mData->mMainLayout->ClearFlag(EAlfVisualFlagLayoutUpdateNotification);
    if(mData->mMainLayout)
        {
        //mData->mOrientationLandScape = mSliderModel->IsLandscape();
        // This is a hack to resolve screen refresh issue
        control().Env().RefreshCallBack(&(control().Env()));
        // Layout the visualization with correct LCT values 
        layoutVisuals(mSliderModel->GetTemplate());
        // Convert all the user defined data to pixels
        ConvertDataToPixels();
        // Set the thumb position to the current tick
        SetThumbPosition();
        mData->mMainLayout->SetOpacity(mSliderWidget->GetOpacity());
        }
    mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);   
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ---------------------------------------------------------------------------
//  
void MulSliderVertical::setActiveStates(
    unsigned int /*aStates*/)
    {
    //do nothing
    }

// ---------------------------------------------------------------------------
//  initializes Model and Widget Pointer
// ---------------------------------------------------------------------------
//
void  MulSliderVertical::initializeSliderData()
    {
    // initialize the Slider Widget Pointer;
    IAlfWidgetFactory& widgetFactory = 
        AlfWidgetEnvExtension::widgetFactory(control().Env());
    mSliderWidget = static_cast<MulSliderWidget *>(widgetFactory.findWidget(
                    control().widget()->widgetName()));
            
    // initialize the Slider Model Pointer;
    mSliderModel =static_cast<MulSliderModel*>(mSliderWidget->model());
    }
    
// ---------------------------------------------------------------------------
//  set the positions and images for the visual hierarchy
// ---------------------------------------------------------------------------
//
void MulSliderVertical::layoutVisuals( sliderTemplate /*aTemplateId*/)
    {
    bool relayout = false;
    if(mData->mLayoutMirrored != AknLayoutUtils::LayoutMirrored())
        {
        mData->mOrientationLandScape = !mSliderModel->IsLandscape();
        mData->mLayoutMirrored = AknLayoutUtils::LayoutMirrored();
        }
        
    if( mSliderModel->IsLandscape() != mData->mOrientationLandScape ) 
        {
        relayout = true;
        mData->mOrientationLandScape = mSliderModel->IsLandscape();
        }  
    if(relayout)
        {
    
    CAlfControl* ctrl = (CAlfControl*)&control();  
    TAknLayoutRect layoutRect;
    //get the lct rect for mBaseSliderLayout and set it            
    // aaslider_pane(2)
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderPane,mData->mMainLayout,KVariety2);
    mData->mBaseSliderLayout->SetRect(
            TRect( TPoint(layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY  ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ;
   if(!mData->mHasBackground)
            SetBackgroundBrush();
    // Set Position and Size for Extended touch layout for track
    mData->mSliderCentreGhost->SetRect(
            TRect( TPoint(0,0 ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight )));


    //get the lct rect for mImageMinus and set it            
    // aid_touch_size_slider_min(2) 
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderMinus,mData->mBaseSliderLayout,KVariety2);
    mData->mImageMinus->SetRect(
            TRect( TPoint( layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ;

    //get the lct rect for mImagePlus and set it            
    // aid_touch_size_slider_max(1) 
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderPlus,
            mData->mBaseSliderLayout,
            KVariety1);
    mData->mImagePlus->SetRect(
            TRect( TPoint( layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ;

    //get the lct rect for mSliderHandle and set it     
    //aid_touch_size_slider_marker(5)
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderMarker,
            mData->mBaseSliderLayout,
            KVariety5);
    mData->mSliderHandle->SetRect(
            TRect( TPoint( layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY ),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight ))) ;
    
    layoutRect = MulSliderUtils::GetComponentRect(
                EVSliderMarkerExtended,
                mData->mBaseSliderLayout,
                KVariety5);
    mData->mHandleGhostHieghtDelta = 
        (layoutRect.Rect().Size().iHeight - mData->mSliderHandle->Size().iY.ValueNow()  );
            
    // Set Position and Size for Extended touch layout for handle
    mData->mSliderHandleGhost->SetRect(
            TRect( TPoint( mData->mBaseSliderLayout->Pos().iX.ValueNow(),
                    mData->mSliderHandle->Pos().iY.ValueNow()- 
                    mData->mHandleGhostHieghtDelta/2),
                    TSize( mData->mBaseSliderLayout->Size().iX.ValueNow(), 
                            layoutRect.Rect().Size().iHeight)));

    //get the lct rect for mSliderCentre and set it     
    //aaslider_bg_pane_cp001(6)
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderBgPane,
            mData->mBaseSliderLayout,
            KVariety6);
    mData->mSliderCentre->SetRect(
            TRect( TPoint( layoutRect.Rect().iTl.iX ,
                    layoutRect.Rect().iTl.iY),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight )));

    //get the lct rect for mTrackTopImage and set it     
    //aaslider_bg_pane_cp001_g1(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderTop,
            mData->mSliderCentre,
            KVariety0);
    mData->mTrackTopImage->SetRect(
            TRect( TPoint(0,layoutRect.Rect().iTl.iY),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight )));

    //get the lct rect for mTrackMiddleImage and set it     
    //aaslider_bg_pane_cp001_g3(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderMiddle,
            mData->mSliderCentre,
            KVariety0);
    mData->mTrackMiddleImage->SetRect(
            TRect( TPoint( 0,layoutRect.Rect().iTl.iY),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight )));

    //get the lct rect for mTrackEndImage and set it     
    //aaslider_bg_pane_cp001_g2(0)
    layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderEnd,
            mData->mSliderCentre,
            KVariety0);
    mData->mTrackEndImage->SetRect(
            TRect( TPoint(0,layoutRect.Rect().iTl.iY),
                    TSize( layoutRect.Rect().Size().iWidth, 
                            layoutRect.Rect().Size().iHeight )));
  
    if(mData->mTrackTopImage && 
                    mData->mTrackMiddleImage && 
                    mData->mTrackEndImage)
                {
                setTrackImage();
                }
    if(mData->mSliderHandle)
        {
        mData->mImageTextureId = KAlfMarkerTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfMarkerTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|
                                   EAlfTextureFlagSkinContent)));
        TAlfImage aImage(*mData->mTexture );                                                
        mData->mSliderHandle->SetImage(aImage);

        }

    if(mData->mImagePlus)
        {
        mData->mImageTextureId = KAlfZoomInTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfZoomInTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|EAlfTextureFlagSkinContent)));
        TAlfImage aImage(*mData->mTexture );
        mData->mImagePlus->SetImage(aImage);
        }

    if(mData->mImageMinus)
        {
        mData->mImageTextureId = KAlfZoomOutTextureId;
        // Create the texture from bitmap provider
        mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
                KAlfAutoGeneratedTextureId/*KAlfZoomOutTextureId*/,
                (MAlfBitmapProvider *)this,
                (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|EAlfTextureFlagSkinContent)));
        TAlfImage aImage(*mData->mTexture );
        mData->mImageMinus->SetImage(aImage);

        }
    }
    }
// ---------------------------------------------------------------------------
// Used for setting images for the track 
// ---------------------------------------------------------------------------
//

void MulSliderVertical::setTrackImage()
    {
    //Review
    TInt flags = EAlfTextureFlagSkinContent;
    flags |= EAlfTextureFlagAutoSize;

    CAlfControl* ctrl = (CAlfControl*)&control();
    //creating texture for topimage
    mData->mImageTextureId = KAlfTrackTopTextureId;
    mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
        KAlfAutoGeneratedTextureId/*KAlfTrackTopTextureId*/,
        (MAlfBitmapProvider *)this,
        (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|EAlfTextureFlagSkinContent)));

    TAlfImage trackTopImage(*mData->mTexture );
    //creating texture for middleimage
    mData->mImageTextureId = KAlfTrackMiddleTextureId;
    mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
        KAlfAutoGeneratedTextureId/*KAlfTrackMiddleTextureId*/,
        (MAlfBitmapProvider *)this,
        (TAlfTextureFlags)(EAlfTextureFlagRetainResolution|EAlfTextureFlagSkinContent)));

    TAlfImage trackMiddleImage(*mData->mTexture );

    //creating texture for bottom image
    mData->mImageTextureId = KAlfTrackBottomTextureId;
    mData->mTexture = &((ctrl->Env()).TextureManager().CreateTextureL(
        KAlfAutoGeneratedTextureId/*KAlfTrackBottomTextureId*/,
        (MAlfBitmapProvider *)this,
        (TAlfTextureFlags)(EAlfTextureFlagDefault)));


    TAlfImage trackEndImage(*mData->mTexture );
    // Set Image on visuals
    mData->mTrackTopImage->SetImage(trackTopImage);
    mData->mTrackMiddleImage->SetImage(trackMiddleImage);                    
    mData->mTrackEndImage->SetImage(trackEndImage);

    }//End of setImage
 
// ---------------------------------------------------------------------------
// ProvideBitmapL()
// ---------------------------------------------------------------------------
//
void MulSliderVertical::ProvideBitmapL (TInt aId, CFbsBitmap *& aBitmap, CFbsBitmap *& aMaskBitmap)
    {
    TAknLayoutRect layoutRect;
    switch(mData->mImageTextureId)
        {
        case KAlfTrackTopTextureId:
            {
            AknIconUtils::CreateIconL(
                   aBitmap,
                   aMaskBitmap,
                   KAvkonBitmapFile,
                   EMbmAvkonQgn_graf_nslider_vertical_top,
                   EMbmAvkonQgn_graf_nslider_vertical_top_mask );
                    
           layoutRect = MulSliderUtils::GetComponentRect(
                   EVSliderTop,
                   mData->mSliderCentre,
                   KVariety0); 
           
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
                    EMbmAvkonQgn_graf_nslider_vertical_bottom,
                    EMbmAvkonQgn_graf_nslider_vertical_bottom_mask );

            layoutRect = MulSliderUtils::GetComponentRect(
                    EVSliderEnd,
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
                    EMbmAvkonQgn_graf_nslider_vertical_middle,
                    EMbmAvkonQgn_graf_nslider_vertical_middle_mask );

            layoutRect = MulSliderUtils::GetComponentRect(
                    EVSliderMiddle,
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
                    EMbmAvkonQgn_graf_nslider_vertical_marker,
                    EMbmAvkonQgn_graf_nslider_vertical_marker_mask );    
            
            layoutRect = MulSliderUtils::GetComponentRect(
                    EVSliderMarker,
                    mData->mBaseSliderLayout,
                    KVariety5);   

             AknIconUtils::SetSize( aBitmap,
                                     TSize(layoutRect.Rect().Size().iWidth,
                                     layoutRect.Rect().Size().iHeight),
                                     EAspectRatioNotPreserved);

            mData->mImageTextureId = 0;
            mData->mMarkerTextureId = aId;
            }
            break;
        case KAlfZoomInTextureId:
            {
            AknIconUtils::CreateIconL(
                   aBitmap,
                   aMaskBitmap,
                   KImageViewerBitmapFile,
                   EMbmMulsliderwidgetQgn_indi_nslider_zoom_in,
                   EMbmMulsliderwidgetQgn_indi_nslider_zoom_in_mask );    

           layoutRect = MulSliderUtils::GetComponentRect(
                   EVSliderPlus,mData->mBaseSliderLayout,KVariety1);    

           AknIconUtils::SetSize( aBitmap,
                                  TSize(layoutRect.Rect().Size().iWidth,
                                  layoutRect.Rect().Size().iHeight),
                                  EAspectRatioNotPreserved);
            mData->mImageTextureId = 0;
            mData->mZoomInTextureId = aId;
            }
            break;
         case KAlfZoomOutTextureId:
             {
             AknIconUtils::CreateIconL(
                    aBitmap,
                    aMaskBitmap,
                    KImageViewerBitmapFile,
                    EMbmMulsliderwidgetQgn_indi_nslider_zoom_out,
                    EMbmMulsliderwidgetQgn_indi_nslider_zoom_out_mask ); 

            layoutRect = MulSliderUtils::GetComponentRect(
                    EVSliderMinus,mData->mBaseSliderLayout,KVariety2);       

            AknIconUtils::SetSize( aBitmap,
                                     TSize(layoutRect.Rect().Size().iWidth,
                                     layoutRect.Rect().Size().iHeight),
                                     EAspectRatioNotPreserved);

           mData->mImageTextureId = 0;
           mData->mZoomOutTextureId = aId;
           }
             break;
        default:
            break;
               
        }
    
    }

// ---------------------------------------------------------------------------
//  setBackgroundBrush()
// ---------------------------------------------------------------------------
//
void MulSliderVertical::SetBackgroundBrush()
    {

    CAlfControl* ctrl = (CAlfControl*)&control();
    // Get Slider Pane Rect (aaslider_pane(2)) and apply on mBaseSliderLayout
    TAknLayoutRect layoutRect = MulSliderUtils::GetComponentRect(
            EVSliderPane,mData->mMainLayout,2);


    TAknWindowLineLayout LayoutHandle = 
    AknLayoutScalable_UiAccel::
    aid_touch_size_slider_max(0).LayoutLine(); 


    int x = LayoutHandle.il;                               
    TAknsItemID itemid;

    itemid.Set(EAknsMajorSkin, EAknsMinorQgnGrafPopupTrans);
    CAlfFrameBrush* brush = CAlfFrameBrush::NewLC((*ctrl).Env(),itemid, 0, 0);

    if(brush!=NULL)
        {
        TRect dispRect(layoutRect.Rect().iTl.iX,
                layoutRect.Rect().iTl.iY,
                layoutRect.Rect().Size().iWidth,
                layoutRect.Rect().Size().iHeight );
        TRect innerRect(layoutRect.Rect().iTl.iX + x,
                layoutRect.Rect().iTl.iY + x,
                layoutRect.Rect().Size().iWidth - x ,
                layoutRect.Rect().Size().iHeight - x);

        brush->SetFrameRectsL(innerRect,dispRect); 
        brush->SetOpacity(KMinopacity);
        mData->mBaseSliderLayout->EnableBrushesL();
        mData->mBaseSliderLayout->Brushes()->AppendL(brush, EAlfHasOwnership); 
        CleanupStack::Pop(brush);
        }
    mData->mHasBackground = true;
    }    
    
// ---------------------------------------------------------------------------
//  MakeTransparent()
// ---------------------------------------------------------------------------
//
void MulSliderVertical::MakeTransparent(bool aVal)
    {
    CAlfBrushArray * brushArray = mData->mBaseSliderLayout->Brushes();
    if(aVal)
        {
        TAlfTimedValue opacity(KMinopacity);
        brushArray->At(0).SetOpacity(opacity);      
        }
    else
        {
        TAlfTimedValue opacity(KMaxopacity);
        brushArray->At(0).SetOpacity(opacity);
        }
    }    
// ---------------------------------------------------------------------------
//  ConvertDataToPixels
// ---------------------------------------------------------------------------
//
void MulSliderVertical::ConvertDataToPixels()
    {
    // Calculate the range in pixel values
    mData->mRangeInPixels = 
    (mData->mSliderCentre->Size().iY.ValueNow() - 
            mData->mSliderHandle->Size().iY.ValueNow());

    if(mSliderModel->MaxRange() - mSliderModel->MinRange() != 0)
        {
        // Calculate Tick Size in Pixels
        mData->mTickInPixels = 
        (mData->mSliderCentre->Size().iY.ValueNow() - 
                mData->mSliderHandle->Size().iY.ValueNow()) /
                (mSliderModel->MaxRange() - mSliderModel->MinRange()) ;
        }
    // Get the track start pixel value    
    mData->mTrackStartPoint = mData->mSliderCentre->Pos().iY.ValueNow() +
    mData->mSliderCentre->Size().iY.ValueNow() -   
    mData->mSliderHandle->Size().iY.ValueNow()  ;  
    // Store current tick
    mData->mCurrTick =  mSliderModel->PrimaryValue() ; 
    if(mSliderModel->MaxRange()== mSliderModel->MinRange())
        {
        mData->mTrackStartPoint= mData->mSliderCentre->Pos().iY.ValueNow();
        }
    mData->mTick = mSliderModel->Tick();
    }
    
    
// ---------------------------------------------------------------------------
//  createVisualization
// ---------------------------------------------------------------------------
//    
void MulSliderVertical::createVisualization(sliderTemplate /*aTemplateId*/)
    {
    mData->mHasBackground = false;
    if(AknLayoutUtils::LayoutMirrored())
        mData->mLayoutMirrored = true;
    else
        mData->mLayoutMirrored = false;
    // Create the visualization
    CAlfControl* ctrl = (CAlfControl*)&control();
    mData->mOrientationLandScape = !mSliderModel->IsLandscape();
        
    // Visual Hierarchy creation
    // create the mMainLayout
    mData->mMainLayout = CAlfLayout::AddNewL(*ctrl,NULL);
    // set the flag for Notifications 
    mData->mMainLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);
    //create the mBaseSliderLayout  
    mData->mBaseSliderLayout = 
    CAlfLayout::AddNewL(*ctrl,mData->mMainLayout);
    // create the dummy layout for slider track 
    mData->mSliderCentreGhost =
    CAlfLayout::AddNewL(*ctrl,mData->mBaseSliderLayout);
    // create the dummy layout for slider handle
    mData->mSliderHandleGhost = 
    CAlfLayout::AddNewL(*ctrl,mData->mBaseSliderLayout);
    
    // create the imagevisual for slider track
    mData->mSliderCentre = 
    CAlfLayout::AddNewL(*ctrl,mData->mBaseSliderLayout);
    // create the imagevisual for slider handle
    mData->mSliderHandle = 
    CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);
    // create the imagevisual for plus icon 
    mData->mImagePlus= 
    CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);
    // create the imagevisual for minus icon
    mData->mImageMinus = 
    CAlfImageVisual::AddNewL(*ctrl,mData->mBaseSliderLayout);
    if(mData->mSliderCentre)
        {
        mData->mTrackTopImage = CAlfImageVisual::AddNewL(*ctrl,mData->mSliderCentre);
        mData->mTrackMiddleImage = CAlfImageVisual::AddNewL(*ctrl,mData->mSliderCentre);
        mData->mTrackEndImage = CAlfImageVisual::AddNewL(*ctrl,mData->mSliderCentre);
        }

    // Get the widget opacity and apply on root visual
    mSliderWidget->ShowWidget(mSliderWidget->GetOpacity(),0);
    }
    

// ---------------------------------------------------------------------------
//  eventHandlerType
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerType MulSliderVertical::eventHandlerType()
    {
    return IAlfWidgetEventHandler::EPresentationEventHandler;
    }


// ---------------------------------------------------------------------------
//  eventExecutionPhase
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase MulSliderVertical::
    eventExecutionPhase()
    {
    return IAlfWidgetEventHandler::ETunnellingPhaseEventHandler;
    }


// ---------------------------------------------------------------------------
//  offerEvent
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderVertical::offerEvent( CAlfWidgetControl& /*aControl*/, 
                                              const TAlfEvent& aEvent )
    {   
    AlfEventStatus ret=EEventNotHandled;
    if(!mSliderWidget->IsHidden())
        {
        if(aEvent.IsCustomEvent())
            {
            if(aEvent.CustomParameter() == EEventMissedPointerUp)
                {
                 ret=EEventHandled;
                 mData->mSliderHit = false;
                 handlePointerUpEvent();
                 }
            }
        
            // handle key events    
        else if(aEvent.IsKeyEvent() && mSliderWidget->IsKeyEnabled() )
            {       
            ret = HandleKeyEvents(aEvent);               
            }
                
            // Pointer drag events and single tap events      
        else if(aEvent.IsPointerEvent())
            {
            if(mSliderWidget->GetOpacity())
                {
                ret = HandlePointerEvents((TAlfEvent *)&aEvent);        
                }
            }
           
        }
    
 	return ret;
    }  
    
 
// ---------------------------------------------------------------------------
// HandleKeyEvents
// ---------------------------------------------------------------------------
// 
 AlfEventStatus MulSliderVertical::HandleKeyEvents(const TAlfEvent& aEvent)
    {
    AlfEventStatus ret = EEventNotHandled;

    
    if(aEvent.KeyEvent().iScanCode == EStdKeyUpArrow)
        {
        if(aEvent.Code() == EEventKey)
            {
            ret = EEventHandled;
            int newPos = mSliderModel->PrimaryValue() + mData->mTick;
            updateModelPrimaryValue(newPos);          
            }       
        }
    else if(aEvent.KeyEvent().iScanCode == EStdKeyDownArrow)
        {
        if(aEvent.Code() == EEventKey)
            {
            ret = EEventHandled;
            int newPos = mSliderModel->PrimaryValue() - mData->mTick;
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
AlfEventStatus MulSliderVertical::trackVisualHit(CAlfVisual * /*aHitVisual*/)
    {
    //Find the thumb Anchor Layout.
    // Zoom Plus icon rectangle
    TAlfRealRect zoomPlus = mData->mBaseSliderLayout->DisplayRect();
    // Slider Handle position
    TAlfTimedPoint thumbVisPos = mData->mSliderHandle->Pos();
    TAlfTimedPoint thumbVisSize = mData->mSliderHandle->Size();
    // Slider track display rect
    TAlfRealRect rct = mData->mSliderCentre->DisplayRect() ;
    int diff = rct.iTl.iY - zoomPlus.iTl.iY;
    //Check if click was above/to left or below/to right of thumb
        if (mData->mLongTapPos.iY - rct.iTl.iY < thumbVisPos.iY.ValueNow() + (thumbVisSize.iY.ValueNow()/2) - diff)
            {
			// Pointer down happened above Handle 
            int newPos = mSliderModel->PrimaryValue() + (((thumbVisPos.iY.ValueNow() +(thumbVisSize.iY.ValueNow()/2) - diff) -(mData->mLongTapPos.iY - rct.iTl.iY))/mData->mTickInPixels);
           
               if( rct.iTl.iY > mData->mLongTapPos.iY )
                   {
                   mData->mLongTapPos.iY = rct.iTl.iY + (thumbVisSize.iY.ValueNow()/2);
                   }
            // Move Handle 
            if(mData->mdirection !=2)
                {
                // checks if the thumb was moving up 
                updateModelPrimaryValue(newPos);
                mData->mdirection = 1;
                }
   
            if(!mData->mOperation)
                {
                mData->mVisual  =  mData->mSliderHandle;
                initializeThumbDragData(
                        mData->mLongTapPos);

                }
            }
        else if (mData->mLongTapPos.iY - rct.iTl.iY > (thumbVisPos.iY.ValueNow() + 
            (thumbVisSize.iY.ValueNow()/2)- diff) )
            {
            if( (rct.iTl.iY + rct.Size().iHeight < mData->mLongTapPos.iY))
                {
                mData->mLongTapPos.iY = rct.iTl.iY+ rct.Size().iHeight - (thumbVisSize.iY.ValueNow()/2);
                }
            // Pointer down happened below Handle
            int newPos = mSliderModel->PrimaryValue() - (((mData->mLongTapPos.iY - rct.iTl.iY) -(thumbVisPos.iY.ValueNow() + (thumbVisSize.iY.ValueNow()/2)-diff))/mData->mTickInPixels);
             // Move Handle 
             if(mData->mdirection !=1)
                {
                    // checks if the thumb was moving down
                updateModelPrimaryValue(newPos);
                mData->mdirection = 2;
                }
            
            //mData->mLongTapStarted = false;
            if(!mData->mOperation)
                    {
                    mData->mVisual  =  mData->mSliderHandle;
                    initializeThumbDragData(  mData->mLongTapPos);

                }
            }
        else 
            {
              
            }
    return EEventHandled;
    }   
    
// ---------------------------------------------------------------------------
//  handles the pointerDown event
// 
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderVertical::handlePointerDownEvent(TAlfEvent * aPntrEvent)
    {
    
    CAlfVisual * focusvisual = aPntrEvent->Visual();
    AlfEventStatus result = EEventNotHandled;
    if (focusvisual)
        {
        mData->mVisual = focusvisual;
        if (IsSliderVisual(focusvisual))
            {
            control().processEvent(TAlfEvent(ECustomEventIconClick));   
            mData->mSliderHit = true;
            MakeTransparent(false);
           
            // Register with the Roster to receive Long Tap Event    
            control().Display()->Roster().SetPointerEventObservers( 
                EAlfPointerEventReportDrag,control());
                 
            if(!mData->mOperation)
                {
                mData->mLongTapPos = aPntrEvent->PointerEvent().iParentPosition;        
                //mData->mLongTapStarted = true;  
                }
           
            //Page Movement
            if (focusvisual == mData->mSliderCentre ||
                focusvisual == mData->mSliderCentreGhost)
                {
                result =  trackVisualHit(focusvisual);
                }
        
            // Drag 
            else if (focusvisual == mData->mSliderHandle 
                 ||  focusvisual == mData->mSliderHandleGhost )
                {
                if(focusvisual)
                    {
                    // Initialize the drag variables
                    initializeThumbDragData(aPntrEvent->PointerEvent().iParentPosition);
                    //Tactile Feedback For Thumb touch
                    MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
                    sldrcntrl.TactileEffectOnTouchandHold();
                    }
                result = EEventHandled;
                }
             // Zoom Icon Plus Clicked
            else if (focusvisual == mData->mImagePlus || focusvisual == mData->mImageMinus )
                 {
                     // Update the newTick
                    int newPos =0;
                    if( mData->mVisual == mData->mImagePlus)
                        {
                        newPos = mSliderModel->MaxRange();
                        }
                    else
                        {
                        newPos = mSliderModel->MinRange();
                        //newPos = mSliderModel->PrimaryValue() - mData->mTick;
                        }
                        updateModelPrimaryValue(newPos);
                //return ETrue;
                result = EEventHandled;
                }
               
            }//END OF if (isSliderVisual()) 
        }
    return result;
    }// End Of handlePointerDownEvent

    
// ---------------------------------------------------------------------------
// HandlePointerEvents
// ---------------------------------------------------------------------------
// 
 AlfEventStatus MulSliderVertical::HandlePointerEvents(TAlfEvent* aEvent)
    {
    AlfEventStatus ret = EEventNotHandled;
    if (aEvent->PointerEvent().iType == TPointerEvent::EButton1Down)
        {   
        mData->mdirection = 0;
        return handlePointerDownEvent(aEvent);
        }
    // Drag Events
    else if (aEvent->PointerEvent().iType == TPointerEvent::EDrag)
        {
            // Drag on Handle
        TAlfRealRect rct = mData->mSliderCentre->DisplayRect() ;
        TAlfTimedPoint thumbVisSize = mData->mSliderHandle->Size();
                    // Drag on Handle                
                if(mData->mVisual && 
                   mData->mSliderHandle && mData->mSliderHandleGhost)
                    {
                    // Store the new pointer position
                    mData->mLongTapPos= aEvent->PointerEvent().iParentPosition ;
                    }
                
            // Calculate the distance moved from the drag  start point
            int dist = mData->mDragPoint.iY - aEvent->PointerEvent().iParentPosition.iY;
            int precision = KRoundupvalue;
            if(dist<0)
                {
                precision = -KRoundupvalue;
                }
            // Calculate the new tick position
            int newPos = (int)(((float)dist / mData->mTickInPixels ) + precision) + 
                mData->mDragStartStep;
            // Update the model hence update visualization
            updateModelPrimaryValue(newPos); 
            //Tactile Feedback For Thumb Drag
            MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
            sldrcntrl.TactileEffectOnDrag(*aEvent);
            ret = EEventHandled;    
        }
    
    else if(aEvent->PointerUp()  )
        {
        
            // Long tap Timer not running
            //mData->mLongTapStarted = false;
            // Handle events for pointer up
		if(mData->mSliderHit)
			{
			// checing if any of slider visual got a button down event 
			// Otherwise its a stray pointerup event , so dont handle it
			mData->mSliderHit = false;
			ret = EEventHandled;
			handlePointerUpEvent();
			}
         
         }
    
    return ret;
    }
    
    
// ---------------------------------------------------------------------------
//  updateModelPrimaryValue
// ---------------------------------------------------------------------------
//    
void MulSliderVertical::updateModelPrimaryValue(int aNewPosValue)
    {
    // Validate the new tick value
    if(aNewPosValue < mSliderModel->MinRange())
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
        mSliderModel->SetPrimaryValue(aNewPosValue);
        mPosData.reset(new(EMM)MulSliderPos);
        mPosData->mPreviousValue = mData->mCurrTick  ;  
        mPosData->mCurrentValue= aNewPosValue;
        mData->mCurrTick = aNewPosValue;
        // Send event to the scrollable widget
        TAlfEvent customevent(ETypePrimaryValueChange,
            (uint)mPosData.get());
        control().processEvent(customevent);
        }
    }

// ---------------------------------------------------------------------------
//  snapPrimaryValueToTicks
// ---------------------------------------------------------------------------
//    
void MulSliderVertical::snapPrimaryValueToTicks()
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
//  handles the pointerUp event
// 
// ---------------------------------------------------------------------------
//
void MulSliderVertical::handlePointerUpEvent()
    {
    // if thumb or track is clicked, change the graphics
    // Send event if single click has hapened 
  
        snapPrimaryValueToTicks();
        MakeTransparent(true);
    if(mData->mOperation)
        {
        //Somehow missed the Up event?
        stopDrag();
        //Tactile Feedback For Thumb Release
        MulSliderControl& sldrcntrl = static_cast<MulSliderControl&>(control());
        sldrcntrl.TactileEffectOnRelease();
        }
    
       
        // Un-register with the roster for long tap events 
        control().Display()->Roster().SetPointerEventObservers(0,
            control());
         
    // send the event for release/poiter up  
     control().processEvent(TAlfEvent(ECustomEventIconRelease));
    }

// ---------------------------------------------------------------------------
//  Stops event handling
// 
// ---------------------------------------------------------------------------
//
void MulSliderVertical::stopEvents()
{
    if(mData->mSliderHit)
		{
		// checking if any of slider visual got a button down event 
		mData->mSliderHit = false;
		handlePointerUpEvent();
		}
}
 	
// ---------------------------------------------------------------------------
 
// UpdateVisualization
// ---------------------------------------------------------------------------
//	 
	 
void MulSliderVertical::updateVisualization()
    {
    ConvertDataToPixels();
    SetThumbPosition();
    }
// ---------------------------------------------------------------------------
// SetThumbPosition
// ---------------------------------------------------------------------------
//
void MulSliderVertical::SetThumbPosition()
    {
    // Current primary value in pixels
    int deltaY = (mSliderModel->PrimaryValue() - mSliderModel->MinRange()) * 
                mData->mTickInPixels;
    // Handle 
    int handleX = mData->mSliderHandle->Pos().iX.ValueNow();
    int ghostHandleX = mData->mSliderHandleGhost->Pos().iX.ValueNow();
    // Handle new pos
    TAlfRealPoint pnt(handleX,mData->mTrackStartPoint - deltaY);
    // Ghost Handle ( Extended) new pos 
    TAlfRealPoint pnt1(ghostHandleX,mData->mTrackStartPoint - deltaY - 
                mData->mHandleGhostHieghtDelta/2); 
    // Set the position
    mData->mSliderHandle->SetPos(pnt,0);
    mData->mSliderHandleGhost->SetPos(pnt1,0);
    // Update visualization variables
    mData->mCurrTick = mSliderModel->PrimaryValue();
    //mData->mMainLayout->UpdateChildrenLayout(0);
    
    }
// ---------------------------------------------------------------------------
// SetTextLabels
// ---------------------------------------------------------------------------
// 
void MulSliderVertical::SetTextLabels(const char* /*aStr*/)
    {
    }

// ---------------------------------------------------------------------------
// accept
// ---------------------------------------------------------------------------
// 
bool MulSliderVertical::accept( CAlfWidgetControl& /*aControl*/, const TAlfEvent& aEvent ) const
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
// initializeThumbDragData
// ---------------------------------------------------------------------------
//    
bool MulSliderVertical::initializeThumbDragData(const TPoint& aPointerPos)
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
        
        }    
    return ETrue;  
    }
    
// ---------------------------------------------------------------------------
// stopDrag
// Stop Dragging 
// ---------------------------------------------------------------------------
//
bool MulSliderVertical::stopDrag()
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
// ---------------------------------------------------------------------------
//  
bool MulSliderVertical::IsSliderVisual(CAlfVisual *aVisual)
	{
	if(aVisual == mData->mSliderCentre||
	   aVisual == mData->mSliderCentreGhost||
	   aVisual == mData->mSliderHandle||  
	   aVisual == mData->mSliderHandleGhost||
	   aVisual == mData->mImagePlus||
	   aVisual == mData->mImageMinus)
	    {
	    return true;
	    }
	 return false;   
	}
	
// ---------------------------------------------------------------------------
// updateTextVisualization
// ---------------------------------------------------------------------------
//  
void MulSliderVertical::updateTextVisualization()
    {/*
    MulSliderModel* sliderModel = mSliderModel; 
    const char* perctext =  sliderModel->GetPercentText();
    SetTextLabels(perctext);*/
    }
    }//namespace Alf
    
//End Of File
