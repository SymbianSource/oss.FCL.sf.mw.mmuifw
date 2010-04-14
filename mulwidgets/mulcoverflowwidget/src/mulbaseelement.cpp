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
* Description:  base element Implementation
 *
*/

//  Include Files
// Class Headers
#include "mulbaseelement.h"

#include <AknUtils.h>
// Alf Headers
#include <alf/alfbatchbuffer.h>
#include <alf/alfbrusharray.h>
#include <alf/alfgradientbrush.h>
#include <alf/alfviewportlayout.h>
#include <alf/alftextvisual.h>
#include <alf/alfimagevisual.h>
#include <alf/alfutil.h>
#include <alf/alftransformation.h>

// Mul Headers
#include "mul/mulmodelutility.h"
#include "imulmodelaccessor.h"
#include <mul/imulsliderwidget.h>
#include <mul/mulevent.h>
#include "mulassert.h"
#include "mulleave.h"
#include "mullog.h" //for logs

// Local Cover Flow Headers
#include "mulcoverflowcontrol.h"
#include "mulcoverflowtemplate.h"

namespace Alf
    {
//Internal base element structure implementation
struct TMulBaseElementImpl
    {        
    
    UString mEmptyText;       // empty text to be displayed when number of items is 0.
	
	int mPadding;            //Padding between item 
    
  	TMulCoverFlowItem mHighlightItemStruct;
	TMulCoverFlowItem mItemStruct;
	int mVisibleCount;
    int	mScrollAnimationTime; // animation time for highlight transition set by the application.
	bool mUiOn;    // true if in UiOnMode
    
    bool mTextVisible;  // not suppose to be displayed in template 4

	
	// To keep a pointer of iconFlow layout.Not owned, created at construction. 
	// The below 4 layouts will never be null. No need to validate before using	
	CAlfFlowLayout* mIconFlowLayout; 
	CAlfLayout* mTextLayout;
    CAlfFlowLayout* mVerticalFlow;
    CAlfLayout* mSliderLayout ; // stores visual of the slider widget
    
    // Can be null when the template doesnt have counter.
    CAlfTextVisual* mCounterVisual ;
    CAlfTextVisual* mEmptyTextVisual;
    
    //Stores current slider opacity. 
    float mSliderOpacity;
    TItemScroll mScrollDir;
    float mEmptyTextOpacity;
    // whether to apply default fit mode(fitinside) or not. 
    // if false apply default fit mode , else based on the image size.
    bool mFitMode;
    
    // different visuals color properties
    TMulVisualColorProperty mIconBgColor;
    TMulVisualColorProperty mCounterColor;
    TMulVisualColorProperty mTextColor;
    TMulRotationProperty mRotation;

	bool mChangeSize;
	// whether user has swiped multiple times.
	TNumberofSwipes mNumSwipes;
	// total number of digits in counter visual at every highlight.
	int mNumOfDigitsInCounter;
	// marquee animation time
	int mMarqueeAnimTime;
	// Titletext extents
	TSize mTitleExt;
	// Detailtext extents
	TSize mDetailExt;
	//Dummy text visual for getting text extents
	CAlfTextVisual* mDummyVisual;
	bool mIsMirrored;
	
	//Constructor of the structure
	TMulBaseElementImpl()
		{
		//Intialisation
		mPadding                 = -1;   
		mNumOfDigitsInCounter    = -1;
		mVisibleCount            = 0;
		mScrollAnimationTime     = 200;
		mEmptyText 	= "";
		mCounterVisual = NULL;
		mIconFlowLayout = NULL;
		mTextLayout = NULL;
	    mVerticalFlow = NULL;
	    mSliderLayout = NULL;
	    mEmptyTextVisual = NULL;
	    mSliderOpacity = 0.0f;
	    mTextVisible   = true;
	    mScrollDir = EItemNoDirection;
	    mEmptyTextOpacity = 0.0f;
	    mFitMode = false;
	    mUiOn = false;
	    mChangeSize = false;
	    mNumSwipes = ESingleSwipe;
	    mMarqueeAnimTime = 0;
	    mDummyVisual = NULL;
	    mIsMirrored = false;
		}
	~TMulBaseElementImpl()
		{		
		}
    }; //end of the structure


// ---------------------------------------------------------------------------
// MulBaseElement
// ---------------------------------------------------------------------------
//
MulBaseElement::MulBaseElement( CAlfWidgetControl& aControl,
			                    const char* aName )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::MulBaseElement");
    mData.reset( new (EMM)TMulBaseElementImpl );
    //Call the base class method      
    construct( aControl,aName );
    CreateBaseLayoutStructure();
    (static_cast<MulCoverFlowControl&>(aControl)).UpdateBaseElement(this);
    if (AknLayoutUtils::LayoutMirrored())
    	{
    	mData->mIsMirrored = true;
    	}
    else
		{
		mData->mIsMirrored = false;
		}
    }

// ---------------------------------------------------------------------------
// ~MulBaseElement
// ---------------------------------------------------------------------------
//	
MulBaseElement::~MulBaseElement()
    {
    // Nothing to delete .
    }

// ---------------------------------------------------------------------------
// accept
// ---------------------------------------------------------------------------
//	
bool MulBaseElement::accept( CAlfWidgetControl& /*aControl*/,
		                     const TAlfEvent& aEvent ) const
     {
     MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::accept");
     uint eventID; 
     if(!aEvent.IsCustomEvent())
       {
       return false;
       }
    eventID = aEvent.CustomParameter();

    //Events handled by the base element
	if( eventID == ETypeHighlight 
			|| eventID == EEventWidgetInitialized 
			|| eventID == ETypePrimaryValueChange )
        {
        return true;
        }
    else
        {
        return false;	
        }
    }

// ---------------------------------------------------------------------------
// offerEvent
// ---------------------------------------------------------------------------
//	
AlfEventStatus MulBaseElement::offerEvent( CAlfWidgetControl& aControl,
			const TAlfEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::offerEvent");
    if ( !accept( aControl, aEvent ) )
        {
        //return false;
		return EEventNotHandled;
        }
    
    uint eventID = aEvent.CustomParameter();
    
    MulCoverFlowControl* coverflowControl = static_cast<MulCoverFlowControl*>(&control());
      
    if( eventID == EEventWidgetInitialized )
        {
        // some of the member variable needs to be reset
        // to support dynamic template change with model change.
        mData->mNumOfDigitsInCounter = -1;
        
        UpdateTextAndCounter();
        CalculatePadding();
        ArrangeItems();
		// Show empty text if coverflow is empty
	    int totalModelCount = coverflowControl->TotalModelCount();
	    IMulModelAccessor* model = coverflowControl->ModelAccessor(); 
	    if( (totalModelCount <= 0) || (model == NULL))
	        {
	        ShowEmptyText( true );
	        return EEventConsumed;
	        }
	    else
	    	{
	    	ShowEmptyText( false );
	    	} 		
 		return EEventConsumed;
        }
        
    else if( eventID == ETypeHighlight )
        {
        // Update the text and counter values according to the new highlight
        int newHighlight = coverflowControl->HighlightIndex(); 
        int animationTime = aEvent.CustomEventData();
        animationTime = animationTime > 0 ? animationTime : mData->mScrollAnimationTime;

        if(mData->mScrollDir != EItemNoDirection)
            {
            if(coverflowControl->IsFastScrollMode())
	            {
	            animationTime = coverflowControl->FastScrollTransitionTime();
	            SetHighlight(newHighlight,animationTime);   	
	            }
            else
	            {
	            SetHighlight(newHighlight,animationTime );   	
	            }
            }
        else
            {
            //@TODO: update all the items 
            animationTime = KZeroAnimation;
            SetHighlight(newHighlight,animationTime);    
            }
        
        UpdateTextAndCounter(animationTime);
            
		return EEventConsumed;
        }
    else if( eventID == ETypePrimaryValueChange )
    	{
 		IMulSliderModel* mulSliderModel = coverflowControl->GetSliderModel();
 		if( mulSliderModel )
	        {
	    	int primaryValue = mulSliderModel->PrimaryValue();
	  	    coverflowControl->SetHighlightIndex( primaryValue , false);
    	    }
		return EEventConsumed;
    	}
    	 	
	return EEventNotHandled;    
    }

// ---------------------------------------------------------------------------
// setActiveStates
// ---------------------------------------------------------------------------
//	
void MulBaseElement::setActiveStates( unsigned int /*aStates*/ )
    {
    //No implementation required
    }

//----------------- Creating the visuals and layout structure ----------------


// ---------------------------------------------------------------------------
// CreateBaseLayoutStructure
// ---------------------------------------------------------------------------
//	
void  MulBaseElement::CreateBaseLayoutStructure()
	{
	THROW_IF_LEAVES
	( 
	 MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateBaseLayoutStructure");
	   
	 CAlfLayout* main = CAlfLayout::AddNewL(control()); 
    
     // set this flag, to recieve layout change events ,that are handled 
	 // by the ovverriden VisualLayoutUpdated method of the alfwidgetcontrol.
     main->SetFlags( EAlfVisualFlagLayoutUpdateNotification );
     main->SetTagL( mainlayout );
     main->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
	 main->SetClipping(ETrue); 
     
    CAlfDeckLayout* deckLayout = CAlfDeckLayout::AddNewL( (CAlfControl &)control(),main);
    deckLayout->SetTagL( decklayout );
    deckLayout->SetFlag( EAlfVisualFlagManualLayout );
    
    CAlfGradientBrush* deckBrush = CAlfGradientBrush::NewL( control().Env() );
    // Set the brush colour
    deckBrush->SetColor( KRgbBlack );
    deckLayout->EnableBrushesL( ETrue );
    // Apply brush and set the opacity .
    deckLayout->Brushes()->AppendL( deckBrush, EAlfHasOwnership ); 
    deckLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
	deckLayout->SetClipping(ETrue);
    
    
    // Create a vertical flow layout which has two horizontal layouts 
    mData->mVerticalFlow = CAlfFlowLayout::AddNewL((CAlfControl&)control(), deckLayout );

    // Set the Layout Direction
    mData->mVerticalFlow->SetFlowDirection( CAlfFlowLayout::EFlowVertical );
    mData->mVerticalFlow->SetFlag( EAlfVisualFlagManualLayout );
    mData->mVerticalFlow->SetTagL( verticalflowlayout );
    mData->mVerticalFlow->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);

    // Create flow layout,all the item visuals are added to this layout.
    // make this layout as the child to the vertical flow layout at index 1.
    mData->mIconFlowLayout = CAlfFlowLayout::AddNewL( (CAlfControl&)control(), mData->mVerticalFlow );
   
    // Set the Layout Direction
    mData->mIconFlowLayout->SetFlowDirection( CAlfFlowLayout::EFlowHorizontal );
    mData->mIconFlowLayout->SetTagL( iconflowlayout );
    mData->mIconFlowLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
    
    // stores visuals of all the items(text visuals) 
    mData->mTextLayout = CAlfLayout::AddNewL((CAlfControl&)control(),mData->mVerticalFlow);
    mData->mTextLayout->SetTagL( textflowlayout );
    mData->mTextLayout->SetFlag(EAlfVisualFlagIgnorePointer);
    mData->mTextLayout->SetFlag( EAlfVisualFlagManualLayout );    
    mData->mTextLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);
    //Add main layout to the base visual tree
    addVisualTree( main, KInitialvalueZero );
    
     );       
	}

// ---------------------------------------------------------------------------
// SetDefaultSize
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetDefaultSize( TSize aSize )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetDefaultSize");
	CAlfLayout* main  = (CAlfLayout*)findVisual( KMainLayoutIndex );
	main->SetSize(aSize );
	CAlfDeckLayout& deckLayout = static_cast<CAlfDeckLayout&> (main->Visual(KMainLayoutIndex) );
	deckLayout.SetSize(aSize);
	CAlfVisual* backgroundVisual = deckLayout.FindTag(backgroundvisual);
    if(backgroundVisual)
		{
		backgroundVisual->SetSize(aSize);				
		}	
	if( mData->mEmptyTextVisual && mData->mEmptyTextOpacity == 1 )
		{
		mData->mEmptyTextVisual->SetSize(aSize);		
		}
	}

// ---------------------------------------------------------------------------
// TextAnchorLayout
// ---------------------------------------------------------------------------
//	        
CAlfLayout& MulBaseElement::TextLayout()
	{
	__MUL_ASSERT( mData->mTextLayout != 0,KNullPointer);
	return *mData->mTextLayout;
	}

  
//----------------------Empty text related visualisation---------------------
// ---------------------------------------------------------------------------
// SetEmptyText
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetEmptyText( const UString& aDefaultText )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetEmptyText");

	mData->mEmptyText = aDefaultText;
	int totalModelCount = ((MulCoverFlowControl*)&control())->TotalModelCount();

	if( mData->mEmptyTextVisual )
		{
		THROW_IF_LEAVES
		 ( 
		  TBuf<KTempBufferSize> desc; 
		   
		  MulModelUtility::ConvertUStringToTDes(mData->mEmptyText, desc);   
		  mData->mEmptyTextVisual->SetTextL(desc);
		 );
		}
		
    if( totalModelCount <= KInitialvalueZero )
        {
        ShowEmptyText( true );
        }
	}

// ---------------------------------------------------------------------------
// ShowEmptyText
// ---------------------------------------------------------------------------
//
void MulBaseElement::ShowEmptyText( bool aShowText  )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ShowEmptyText");
    	
     // If the application has not set any empty text , then no need to do anything 
    if( mData->mEmptyText.isEmpty() )
    	{
    	if( aShowText )
    		{
    		ShowHideVisualsForEmptyText(false);
    		HandleSizeChangeOnMain();
    		}
    	else
    		{
    		ShowHideVisualsForEmptyText(true);
    		}
    	return;
    	}
    	
    if( aShowText )
    	{
    	if( !mData->mEmptyTextVisual )
    		{
    		CreateEmptyTextVisualisation();		
    		}
    		    	
    	// This function is called twice only when an empty model is set and the app sets the empty text twice .	
    	ShowHideVisualsForEmptyText(false);	
    	if( mData->mEmptyTextOpacity == KBackGroundOpacity && mData->mEmptyTextVisual )
    		{
	    	mData->mEmptyTextVisual->SetOpacity(KHighlightOpacity);
	    	mData->mEmptyTextOpacity = KHighlightOpacity;
	    	}
    	HandleSizeChangeOnMain();
	    	
    	}
    else
    	{
    	ShowHideVisualsForEmptyText(true);
    	if( mData->mEmptyTextVisual && mData->mEmptyTextOpacity == 1 )
    		{
    		mData->mEmptyTextVisual->SetOpacity(KBackGroundOpacity);
    		mData->mEmptyTextOpacity = KBackGroundOpacity;
    		}
    	}
    }
 
// ---------------------------------------------------------------------------
// HandleSizeChangeOnMain()
// ---------------------------------------------------------------------------
// 
void  MulBaseElement::HandleSizeChangeOnMain()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::HandleSizeChangeOnMain");

	IMulModelAccessor* model = ((MulCoverFlowControl*)&control())->ModelAccessor(); 
	CAlfLayout* main  = (CAlfLayout*)findVisual( KMainLayoutIndex );
	TSize topLayoutSize = main->Size().Target().AsSize(); ;
	
	if( !model )
		{
		if (topLayoutSize != TSize(0,0))
			{
			SetDefaultSize(topLayoutSize);
			}
		else
			{
			SetDefaultSize( control().DisplayArea().Size() );
			}
			
		}
	else
	    {
	    SetDefaultSize(topLayoutSize);		
	    }
	}

// ---------------------------------------------------------------------------
// IsEmptyText
// ---------------------------------------------------------------------------
// 
bool  MulBaseElement::IsEmptyText()
	{
	return !(mData->mEmptyText.isEmpty());
	}  
// ---------------------------------------------------------------------------
// ShowHideVisualsForEmptyText
// ---------------------------------------------------------------------------
// 
void  MulBaseElement::ShowHideVisualsForEmptyText( bool aShow )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ShowHideVisualsForEmptyText");

	if( !aShow )
		{
		SetTextVisualOpacity( KBackGroundOpacity );
		SetCounterTextOpacity( KBackGroundOpacity);   
		}
	else 
		{
		if(mData->mTextVisible)
			{
			SetTextVisualOpacity( KHighlightOpacity );	
			}		
		// if ui on off template is enabled then reset the counter 
		// opacity depending on the current ui on/off value
		if( Template2D()->IsUiOnOffFlagEnabled() )
			{
			ResetUiOnOff();	
			}
		else
			{
			SetCounterTextOpacity( KHighlightOpacity);   
			}	
		}
	}
	
// ---------------------------------------------------------------------------
// CreateEmptyTextVisualisation
// ---------------------------------------------------------------------------
//	
void MulBaseElement::CreateEmptyTextVisualisation() 
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateEmptyTextVisualisation");
 
	if( !mData->mEmptyTextVisual )
		{
		THROW_IF_LEAVES
		 ( 
		 	CAlfLayout* main = static_cast<CAlfLayout*>(findVisual( KMainLayoutIndex ));
		    mData->mEmptyTextVisual = CAlfTextVisual::AddNewL(control(),main);
		    
		    TBuf<KTempBufferSize> desc; 
		   
		    MulModelUtility::ConvertUStringToTDes(mData->mEmptyText, desc);   
		    mData->mEmptyTextVisual->SetTextL(desc);
		    mData->mEmptyTextVisual->SetTagL(KEmptyText);
		    mData->mEmptyTextVisual->SetWrapping(CAlfTextVisual::ELineWrapBreak);
		       
		    TInt fontId =control().Env().TextStyleManager()
				.CreatePlatformTextStyleL(EAknLogicalFontSecondaryFont,-1);	
		    // check if user has set any different color for text.
			if (mData->mTextColor.mIsColorSet)
				{
				// apply the color set by the application
				mData->mEmptyTextVisual->SetColor(mData->mTextColor.mColor);
				}
			else
				{
				// apply the default color
				mData->mEmptyTextVisual->SetColor(KRgbWhite);
				}
			
			mData->mEmptyTextVisual->SetLineSpacing(KTextRowsz);
			mData->mEmptyTextVisual->SetTextStyle(fontId);
		    mData->mEmptyTextVisual->SetFlag(EAlfVisualFlagIgnorePointer);
		 );
		
		}
    }



// ---------------------------------------------------------------------------
// DisplayIndicatorIcon
// ---------------------------------------------------------------------------
// 
void MulBaseElement::DisplayIndicatorIcon(const MulVisualItem & aVisualItem, int aIndex)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::DisplayIndicatorIcon");
    CAlfDeckLayout& layout = static_cast<CAlfDeckLayout&>(mData->mIconFlowLayout->Visual(aIndex));
    CAlfImageVisual* visual =static_cast<CAlfImageVisual*>(layout.FindTag(KCoverflowIndicator));    
    IMulVariantType* varData = aVisualItem.Attribute(mulvisualitem::KMulIndicator2); 
    
    if(varData)
        {
        if(!visual)
            {
            visual = CreateIndicatorIconVisual(layout);   
            }
        ((MulCoverFlowControl*)&control())->DoSetImage(varData,visual);
        visual->ClearFlag(EAlfVisualFlagIgnorePointer);
        }

    else if(visual)
        {
        CAlfTextureManager& textureMgr = control().Env().TextureManager();
        TAlfImage image = TAlfImage(textureMgr.BlankTexture()); 
        visual->SetImage(image); 
        visual->SetFlag(EAlfVisualFlagIgnorePointer);
        }

    }
 
// ---------------------------------------------------------------------------
// CreateIndicatorIconVisual
// ---------------------------------------------------------------------------
//    
CAlfImageVisual*  MulBaseElement::CreateIndicatorIconVisual(CAlfDeckLayout& aLayout) 
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateIndicatorIconVisual");
    CAlfImageVisual* indicatorVisual = CAlfImageVisual::AddNewL(control(), &aLayout);
    indicatorVisual->SetTagL(KCoverflowIndicator); 
    SetIndicatorDimensions(*indicatorVisual);
    indicatorVisual->SetScaleMode(CAlfImageVisual::EScaleNormal);
    
    return indicatorVisual;
    }

// ---------------------------------------------------------------------------
// SetIndicatorDimensions
// ---------------------------------------------------------------------------
//  
void  MulBaseElement::SetIndicatorDimensions(CAlfImageVisual& aIndicatorVisual)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetIndicatorDimensions");
    TMulCoverFlowItem indicatorDimensions = Template2D()->IndicatorDimension();
    aIndicatorVisual.SetPos(TAlfRealPoint(indicatorDimensions.posx,indicatorDimensions.posy));
    aIndicatorVisual.SetSize(TAlfRealSize(indicatorDimensions.width ,indicatorDimensions.height));    
    }
    
// ---------------------------------------------------------------------------
// StoreVisualDimensions
// ---------------------------------------------------------------------------
// 
void MulBaseElement::StoreVisualDimensions( TMulCoverFlowItem aHighlightItemDimensions, TMulCoverFlowItem aNonHighlightItemDimensions,int aVisibleIconCount )
 	{
 	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::StoreVisualDimensions");
 	mData->mHighlightItemStruct = aHighlightItemDimensions;
	mData->mItemStruct = aNonHighlightItemDimensions;
	mData->mVisibleCount = aVisibleIconCount;
	((MulCoverFlowControl*)&control())->StoreVisibleItemCount(mData->mVisibleCount);
	if (mData->mVisibleCount > 1)
		{
		mData->mChangeSize = true;	
		}
	else
		{
		mData->mChangeSize = false;	
		}
 	}

// ---------------------------------------------------------------------------
// StoreTextVisibility
// ---------------------------------------------------------------------------
//	
void MulBaseElement::StoreTextVisibility( bool aIsTextVisible )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::StoreTextVisibility");
	mData->mTextVisible = aIsTextVisible;
	}


// ---------------------------------------------------------------------------
// IsTextVisibile
// ---------------------------------------------------------------------------
//  
bool MulBaseElement::IsTextVisibile( )const
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::IsTextVisible");
    return mData->mTextVisible ;
    }


// ---------------------------------------------------------------------------
// OrientationChange
// ---------------------------------------------------------------------------
//
void MulBaseElement::OrientationChange()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::OrientationChange");

	mData->mVerticalFlow->SetOpacity(0);
	if (mData->mDummyVisual)
		{
		// deleting since the textextents being zero.
		mData->mDummyVisual->RemoveAndDestroyAllD();
		mData->mDummyVisual = NULL;
		}

	// update the attribute of visuals
	Template2D()->CreateVisualisation(((MulCoverFlowControl*)&control())->IsLandscape(),true);
    
    CalculatePadding();    
    CreateAndInitializeVisuals(true);
	
	// Redraw the visuals after orientation changes
	int totalModelCount = ((MulCoverFlowControl*)&control())->TotalModelCount();    		
	if( totalModelCount == 0 )
		{
		CAlfLayout* main  = (CAlfLayout*)findVisual( KMainLayoutIndex );
		SetDefaultSize(main->Size().ValueNow().AsSize());		
		}
	ArrangeItems();
	SetNewCounterPosition(Template2D()->CounterDimensions(),true);
	UpdateRotation();
	mData->mVerticalFlow->SetOpacity(1);   
	}


//------------------Counter and Text related----------------------------------

// ---------------------------------------------------------------------------
// CreateCounterVisual
// ---------------------------------------------------------------------------
// 
CAlfTextVisual& MulBaseElement::CounterVisual()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CounterVisual");
   	__MUL_ASSERT( mData->mVerticalFlow != 0,KNullPointer);

    mData->mCounterVisual = &CreateTextVisual(KCoverflowCounter,*mData->mVerticalFlow);
    mData->mCounterVisual->SetAlign(EAlfAlignHCenter,EAlfAlignVCenter);
	return *mData->mCounterVisual;
    }

// ---------------------------------------------------------------------------
// CreateVisual
// ---------------------------------------------------------------------------
// 
CAlfTextVisual& MulBaseElement::CreateTextVisual(const TDesC8& aName, 
        CAlfLayout& aLayout) 
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateTextVisual");
	
	CAlfTextVisual* textVisual =static_cast<CAlfTextVisual*>(aLayout.FindTag(aName));

	if(textVisual == NULL)
	   {
	   THROW_IF_LEAVES
	        (
	   		textVisual = CAlfTextVisual::AddNewL( (CAlfControl &)control(),&aLayout); 
	        textVisual->SetTagL(aName);
	        ); 
	    textVisual->SetFlag(EAlfVisualFlagIgnorePointer);
	    textVisual->SetFlag(EAlfVisualFlagClipping);
	   }
	return *textVisual;  
	}
    
    
// ---------------------------------------------------------------------------
// UpdateTextAndCounter
// ---------------------------------------------------------------------------
//	
void MulBaseElement::UpdateTextAndCounter(int animationTime)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::UpdateTextAndCounter");
    
	UpdateTextValue(animationTime);	
    
    //After highlight is changed then the counter text should be updated
    SetCounterText();
    } 
    
// ---------------------------------------------------------------------------
// SetCounterTextOpacity
// ---------------------------------------------------------------------------
//  
void MulBaseElement::SetCounterTextOpacity( int aOpacity)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetCounterTextOpacity");
    if(mData->mCounterVisual)
        {
        mData->mCounterVisual->SetOpacity( aOpacity ); 		
        }	    
	}

    
// ---------------------------------------------------------------------------
// ResetUiOnOff
// ---------------------------------------------------------------------------
//	
void MulBaseElement::ResetUiOnOff(int aTransitionTime)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ResetUiOnOff");
    
	int totalModelCount = ((MulCoverFlowControl*)&control())->TotalModelCount();
    if(mData->mCounterVisual && Template2D() && Template2D()->IsUiOnOffFlagEnabled() && totalModelCount>0 )
        {
        int opacity = mData->mUiOn? KHighlightOpacity: KBackGroundOpacity;
        mData->mCounterVisual->SetOpacity( TAlfTimedValue(opacity,aTransitionTime)  ); 
        // Every time In template4 Ui on off mode toggles we should set the enable holding flag 
		// to gesture helper to recieve the hold events.
		((MulCoverFlowControl*)&control())->SetHoldingEnabled(); 
        }
    } 
    
// ---------------------------------------------------------------------------
// UIOnOffMode
// ---------------------------------------------------------------------------
//
void MulBaseElement::UIOnOffMode(bool aUiOn, int aTransitionTime)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::UIOnOffMode");
      
    mData->mUiOn = aUiOn;
    ResetUiOnOff( aTransitionTime );
    }
    
// ---------------------------------------------------------------------------
// IsUiOnMode
// ---------------------------------------------------------------------------
//  
bool MulBaseElement::IsUiOnMode()
	{
    return mData->mUiOn;	
	}


// ---------------------------------------------------------------------------
// SetTextVisualOpacity
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetTextVisualOpacity( int aOpacity , int aTransitionTime )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetTextVisualOpacity"); 
	if(mData->mTextLayout)
		{
    	mData->mTextLayout->SetOpacity(TAlfTimedValue (aOpacity,aTransitionTime));
    	}	
	}

// ---------------------------------------------------------------------------
// UpdateTextValue
// ---------------------------------------------------------------------------
//
void MulBaseElement::UpdateTextValue(int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("Mul::MulBaseElement::UpdateTextValue");
	if( !mData->mTextVisible )
    	{
    	return;
    	}
    	
	IMulModelAccessor* modelAccessor = ((MulCoverFlowControl*)&control())->ModelAccessor();
	int highlightindex = static_cast<MulCoverFlowControl*>(&control())->HighlightIndex();
    
	if(highlightindex>=0  )
        {
        //set text to title
        try
            {
			const MulVisualItem& item = modelAccessor->Item(highlightindex);
			IMulVariantType* varData = item.Attribute(mulvisualitem::KMulTitle);
            MUL_LOG_INFO("MUL::UpdateTextValue::UpdateTiltle");
            SetTextToVisual( mulvisualitem::KMulTitle, varData  ); 
            
            // SetVirtualSize if required.
            SetVirtualViewPortSize(mulvisualitem::KMulTitle);  
            }
        catch(...)
            {
            IMulVariantType* varData = NULL;
            SetTextToVisual( mulvisualitem::KMulTitle, varData  );
            }
         
         //set text to detail
         try
            {
			const MulVisualItem& item = modelAccessor->Item(highlightindex);
			IMulVariantType* varData = item.Attribute(mulvisualitem::KMulDetail);
            MUL_LOG_INFO("MUL::UpdateTextValue::UpdateDetail");
            SetTextToVisual( mulvisualitem::KMulDetail, varData );
            
            // SetVirtualSize if required.
            SetVirtualViewPortSize(mulvisualitem::KMulDetail);  
            }
        catch(...)
            {
            IMulVariantType* varData = NULL;
            SetTextToVisual( mulvisualitem::KMulDetail,varData );
            } 
        
        // Cancel if any command is in process before start marquee.
        CancelAllCommands();
        
        // Before orientaion change stop marquee on the text
        if(!((static_cast<MulCoverFlowControl&>(control())).IsFastScrollMode())) 
            {            
            control().Env().Send(
                TAlfCustomEventCommand(ECustomEventMarqueeStart, 
                &control()), KMarqueeTime1000 + aAnimationTime);
            
            }
        }
	}
// ---------------------------------------------------------------------------
// SetTextToVisual
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetTextToVisual( mulvisualitem::TVisualAttribute aName,IMulVariantType* aText )
	{
	MUL_LOG_ENTRY_EXIT("Mul::MulBaseElement::SetTextToVisual");

	if(mData->mTextLayout)
		{
		CAlfTextVisual* textVisual = 
			static_cast<CAlfTextVisual*> (mData->mTextLayout->FindTag( IdToTag(aName) ));

		if(textVisual)
			{
			if ( aText )
				{  
				// set text to visual
				THROW_IF_LEAVES
					(
					MUL_LOG_INFO("MUL::SetTextToVisual::Set New Data");
					textVisual->SetTextL(aText->DesC());
					textVisual->SetWrapping(CAlfTextVisual::ELineWrapTruncate);
					);
				    // A workaround till the text visual starts giving the correct text extents value.
		            if(!mData->mDummyVisual)
		                {
		                mData->mDummyVisual = CAlfTextVisual::AddNewL(control(), NULL);
		                mData->mDummyVisual->SetWrapping(CAlfTextVisual::ELineWrapManual);
		                mData->mDummyVisual->SetOpacity(0);
		                mData->mDummyVisual->SetFlags(EAlfVisualFlagIgnorePointer);
		                }
		            
		            // Setting the style same as our text visual.
		            mData->mDummyVisual->SetTextStyle(textVisual->TextStyle()); 
		            mData->mDummyVisual->SetTextL(aText->DesC()); // Setting the text
		            	        
		            if(aName == mulvisualitem::KMulDetail)
		                {
		                mData->mDetailExt = mData->mDummyVisual->TextExtents();
		                }
		            else
		                {
		                mData->mTitleExt = mData->mDummyVisual->TextExtents();
		                }
				}
			else
				{
				// reset to null value .
				THROW_IF_LEAVES
					(
					textVisual->SetTextL(_L(""));
					);
				}
			}
		else
			{
			mData->mTextLayout->SetOpacity(TAlfTimedValue(KBackGroundOpacity,0));   
			}   
		}
	}

// ---------------------------------------------------------------------------
// SetVirtualViewPortSize()
// ---------------------------------------------------------------------------
//    
void MulBaseElement::SetVirtualViewPortSize(mulvisualitem::TVisualAttribute aName)
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulBaseElement::SetVirtualViewPortSize");
	CAlfViewportLayout* viewportLayout = NULL ;
	CAlfTextVisual* text = NULL ;
	TSize textExtent ;
	TSize parentSize;
	if (aName == mulvisualitem::KMulDetail)
	    {
	    viewportLayout = static_cast<CAlfViewportLayout*>(
	            mData->mTextLayout->FindTag(KDetailViewPortLayout));
	    if(!viewportLayout)
	        {
	        return; // return if no viewport
	        }
        textExtent = mData->mDetailExt;
        parentSize = Template2D()->DetailWindowSize();
	          
	    }
	else
	    {
	    viewportLayout = static_cast<CAlfViewportLayout*>(
	            mData->mTextLayout->FindTag(KTitleViewPortLayout));
	    if(!viewportLayout)
	        {
	        return; // return if no viewport
	        }
       textExtent = mData->mTitleExt;
       parentSize = Template2D()->TitleWindowSize();
	        
	    }
	text = static_cast<CAlfTextVisual*>(
	        viewportLayout->FindTag( IdToTag(aName) ));
	if(!text)
	    {
	    return; // if text visual is not found we will not set viewportsize.
	    }
	
	viewportLayout->SetViewportPos((TAlfRealPoint(0, 0)), 0);
    viewportLayout->SetVirtualSize(
            (TAlfRealPoint(parentSize.iWidth, parentSize.iHeight)),0);   
	
	//Tollerance is 15 pixel, it might happen that
	// the text is big but while wrapping it not filling the whole
	// width of the layout because the remaining space is less to
	// accomodate a character.
	// As for QFN_PRIMARY the max character width is 15 pixel
	// so I am keeping tollerance as 15 
	// @TODO: Insted of hard coding to 15 pixel use max charecter 
	// width for QFN_PRIMARY.
	if ((parentSize.iWidth - textExtent.iWidth) > 15)
		{
		text->SetAlign(EAlfAlignHCenter, EAlfAlignVCenter); 	
		}
	else
		{
		text->SetAlign(EAlfAlignHLocale,EAlfAlignVCenter);
		}
    }

// ---------------------------------------------------------------------------
// StartMarquee()
// ---------------------------------------------------------------------------
//
void MulBaseElement::StartMarquee(mulvisualitem::TVisualAttribute aName)
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulBaseElement::StartMarquee");
    // start marquee.
    int widthOffScreen = 0;
    CAlfViewportLayout* viewportLayout = NULL ;
    CAlfTextVisual* text =  NULL ;
	TSize textExtent ;
	TSize parentSize;
	int maxCharWidth ;
	if (aName == mulvisualitem::KMulDetail)
        {
        viewportLayout =static_cast<CAlfViewportLayout*>(
                mData->mTextLayout->FindTag(KDetailViewPortLayout));
        if(!viewportLayout)
            {
            return;// return if no viewport
            }                  
       textExtent = mData->mDetailExt;
       parentSize = Template2D()->DetailWindowSize();
       maxCharWidth = Template2D()->DetailMaxCharWidth();         
        }
    else
        {
        viewportLayout = static_cast<CAlfViewportLayout*>(
                mData->mTextLayout->FindTag(KTitleViewPortLayout));
        if(!viewportLayout)
            {
            return;// return if no viewport
            }
       textExtent = mData->mTitleExt;
       parentSize = Template2D()->TitleWindowSize();
       maxCharWidth = Template2D()->TitleMaxCharWidth();
        }
	text = static_cast<CAlfTextVisual*>(
	        viewportLayout->FindTag( IdToTag(aName) ));
	if(!text)
	    {
	    return; // if no text.
	    }
	
    text->SetWrapping(CAlfTextVisual::ELineWrapManual);
   
    widthOffScreen= textExtent.iWidth - parentSize.iWidth;
    int nDbMaxChars = (widthOffScreen/(2*maxCharWidth)) + 1;
	          	        
    // Schedule a update event, when the visual has finished marquee text scrolling.
    if (aName == mulvisualitem::KMulDetail)
        {
        if (nDbMaxChars > 0 )
            {
			viewportLayout->SetVirtualSize(
			        (TAlfRealPoint(textExtent.iWidth, parentSize.iHeight)),0);                  
            // to avoid negative time value
            mData->mMarqueeAnimTime = (nDbMaxChars*KMarqueeTime1000);
            viewportLayout->SetViewportPos((TAlfRealPoint(widthOffScreen , 0)), 
                    nDbMaxChars * KMarqueeTime1000);
            // delay after completion of the marquee for titlte.
            mData->mMarqueeAnimTime += KMarqueeTime1000;
            control().Env().Send(
                TAlfCustomEventCommand(ECustomEventMarqueeFinished, 
                static_cast<CAlfControl*>(&(control()))), 
                mData->mMarqueeAnimTime);
            }
        }
    else
        {
        // reset the marquee animation time
        mData->mMarqueeAnimTime = 0;  
        if(nDbMaxChars > 0)
            {
			viewportLayout->SetVirtualSize(
			        (TAlfRealPoint(textExtent.iWidth, parentSize.iHeight)),0);  
            
            // to avoid negative time value
            mData->mMarqueeAnimTime = (nDbMaxChars*KMarqueeTime1000);  
            viewportLayout->SetViewportPos((TAlfRealPoint(widthOffScreen , 0)), 
                    nDbMaxChars * KMarqueeTime1000);
            // delay after completion of the marquee for titlte.
            mData->mMarqueeAnimTime += KMarqueeTime1000;
            control().Env().Send(
                TAlfCustomEventCommand(ECustomEventTitleMarqueeFinished, 
                static_cast<CAlfControl*>(&(control()))), 
                mData->mMarqueeAnimTime );
            // delay before start the marquee for detail.
            mData->mMarqueeAnimTime += KMarqueeTime1000;
            }
        control().Env().Send(
            TAlfCustomEventCommand(ECustomEventDetailMarqueeStart, 
            (&(control()))), mData->mMarqueeAnimTime );
        }
    } 	
    
// ---------------------------------------------------------------------------
// StopMarquee()
// ---------------------------------------------------------------------------
//
void MulBaseElement::StopMarquee(mulvisualitem::TVisualAttribute aName)
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulBaseElement::StopMarquee");
    CAlfViewportLayout* viewPortLayout = NULL; 
    CAlfTextVisual* text = NULL;
    TSize parentSize;
    if (aName == mulvisualitem::KMulDetail)
        {
        viewPortLayout =
            static_cast<CAlfViewportLayout*>(mData->mTextLayout->FindTag(KDetailViewPortLayout));
        parentSize = Template2D()->DetailWindowSize();
        }
    else
        {
        viewPortLayout =
            static_cast<CAlfViewportLayout*>(mData->mTextLayout->FindTag(KTitleViewPortLayout));
        parentSize = Template2D()->TitleWindowSize();
        }
   if(!viewPortLayout)
       {
       return;
       }
   text = static_cast<CAlfTextVisual*>(viewPortLayout->FindTag( IdToTag(aName) ));
   if(!text)
       {
       return;
       }
   const TDesC textDesc = text->Text();
   if(!textDesc.Length())
       {
       return;
       }
  
    viewPortLayout->SetViewportPos((TAlfRealPoint(0, 0)), 0);
    viewPortLayout->SetVirtualSize((TAlfRealPoint(parentSize.iWidth, parentSize.iHeight)),0);   
    text->SetWrapping(CAlfTextVisual::ELineWrapTruncate);
               
    }

// ---------------------------------------------------------------------------
// CancelAllCommands
// ---------------------------------------------------------------------------
//  
void MulBaseElement::CancelAllCommands( )
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulBaseElement::CancelAllCommands");
    CAlfWidgetControl * cntrl = &control();
    control().Env().CancelCustomCommands( cntrl, ECustomEventMarqueeFinished);
    control().Env().CancelCustomCommands( cntrl, ECustomEventMarqueeStart);
    control().Env().CancelCustomCommands( cntrl, ECustomEventTitleMarqueeFinished);
    control().Env().CancelCustomCommands( cntrl, ECustomEventDetailMarqueeStart); 
    }

// ---------------------------------------------------------------------------
// SetCounterText
// ---------------------------------------------------------------------------
//	
void MulBaseElement::SetCounterText( int /*aTransitionTime*/ )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetCounterText");
	   
    if( mData->mCounterVisual )
    	{
    	SetNewCounterPosition(Template2D()->CounterDimensions());
    	int totalModelCount = ((MulCoverFlowControl*)&control())->TotalModelCount();
    	int currHighlightIndex = ((MulCoverFlowControl*)&control())->HighlightIndex();
		
		// this code will work for model count upto 99999.
		// done for performance improvement , no need to calculate 
		// no of digits in highlight index and total model count.
    	auto_ptr<HBufC> countbuffer(HBufC::NewL (12)); 
      
        //settin value for counter text visual
        /// @bug critical:avanhata:7/7/2008 in arabic/hebrew the text should flow
        /// from right to left (totalCount / highlight) => the string format needs
        /// to come from a resource file, and use stringloader (and arabic indic number
        /// conversion if stringloader does not do it automatically)
        countbuffer->Des().AppendNum(currHighlightIndex+1);
        countbuffer->Des().Append(KSlash);
        countbuffer->Des().AppendNum(totalModelCount);

        mData->mCounterVisual->SetTextL(*countbuffer);
    	}    
	}

//--------------------------------------------------------------------------
// SetNewCounterPosition
//--------------------------------------------------------------------------
//
void MulBaseElement::SetNewCounterPosition( const TMulCoverFlowItem& aCounterLctDimension, bool aOrientationChanged)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetNewCounterPosition");
	MulCoverFlowControl* coverflowControl = ((MulCoverFlowControl*)&control());
	int totalModelCount = coverflowControl->TotalModelCount();
	int currHighlightIndex = coverflowControl->HighlightIndex();
    
    if( totalModelCount > 0 && currHighlightIndex >= 0 )
    	{
    	int numOfDigits = NumberOfDigits(totalModelCount);
    	numOfDigits += NumberOfDigits(currHighlightIndex);
   		
   		if( mData->mNumOfDigitsInCounter!= numOfDigits || aOrientationChanged )
   			{
        	mData->mNumOfDigitsInCounter = numOfDigits;
        	int maxCharWidth = coverflowControl->Template2D()->CounterMaxCharWidth();
        	int numOfPixels = numOfDigits*maxCharWidth + 20; // 10 is the tolerance.
  
        	int rightPosx =0; 
        	int leftPosx = 0;
        	// if mirroring is enabled(ie for RTL Language)
        	if (mData->mIsMirrored)
        		{
        		CAlfLayout* main  = (CAlfLayout*)findVisual( KMainLayoutIndex );
						TSize topLayoutSize = main->Size().Target().AsSize();
        		rightPosx = topLayoutSize.iWidth - aCounterLctDimension.posx ;
        		leftPosx = rightPosx - numOfPixels;
        		}
        	else
        		{
        		//rightPosx = aCounterLctDimension.posx + aCounterLctDimension.width; 
        		//leftPosx = rightPosx - numOfPixels;
        		leftPosx = aCounterLctDimension.posx;
        		}
	   		mData->mCounterVisual->SetPos(TAlfRealPoint(leftPosx,aCounterLctDimension.posy));
		    mData->mCounterVisual->SetSize(TAlfRealPoint(numOfPixels ,aCounterLctDimension.height));
   			}
    	}
	}
// ---------------------------------------------------------------------------
// NumberOfDigits
// ---------------------------------------------------------------------------
//
int MulBaseElement::NumberOfDigits(int aNumber)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::NumberOfDigits");
    int num_digits = 0;
    int number = aNumber;
    do 
    	{
    	num_digits++;
		number/=10;	
    	}
    while ( number > 0 );
    
    return 	num_digits;
    }
    	
// ---------------------------------------------------------------------------
// ApplyScaleMode
// ---------------------------------------------------------------------------
//
void MulBaseElement::ApplyScaleMode( CAlfImageVisual& aImageVisual)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ApplyScaleMode");
    CAlfImageVisual::TScaleMode scaleMode; 

    if (mData->mFitMode)
	    {
	    if(aImageVisual.Image().HasTexture())
	        {
	        CAlfTexture* texture = const_cast<CAlfTexture*>(&aImageVisual.Image().Texture());
	        TInt imageWidth = texture->Size().iWidth;
	        TInt imageHeight = texture->Size().iHeight;
	        // when animation is not complete sizes will not be correct.
	        // so sizes are taken from template   
	        if(imageWidth == 0 || imageHeight == 0) 
		        {
		        scaleMode = CAlfImageVisual::EScaleFitInside;
		        }
	        else if( imageWidth <= mData->mHighlightItemStruct.width && 
	        	imageHeight <= mData->mHighlightItemStruct.height )
	            {
	            scaleMode = CAlfImageVisual::EScaleNormal;
	            }
	        else
	            {
		        scaleMode = CAlfImageVisual::EScaleFitInside;
	            }
	        }
	    else
	        {
		    scaleMode = CAlfImageVisual::EScaleFitInside;
	        }
	    }
	else
		{
		// Apply default scale mode
		scaleMode = CAlfImageVisual::EScaleFitInside;
		}
     
	if (aImageVisual.ScaleMode() != scaleMode) 
		{
	    aImageVisual.SetScaleMode( scaleMode ); 	
		}
	 ApplyBrushOnIcon( aImageVisual );	
    }
    
// ---------------------------------------------------------------------------
// ApplyBrushOnIcon
// ---------------------------------------------------------------------------
//
void MulBaseElement::ApplyBrushOnIcon( CAlfImageVisual& aImageVisual )
    { 
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ApplyBrushOnIcon");  
    if( mData->mIconBgColor.mIsColorSet )
    	{
    	aImageVisual.EnableBrushesL(ETrue); 
    	CAlfBrushArray* brushArray = aImageVisual.Brushes();
	    if( (brushArray->Count()==0))
	        {
	        CAlfGradientBrush* brush = CAlfGradientBrush::NewL(aImageVisual.Env());     
	    	brush->SetColor( mData->mIconBgColor.mColor );         
	    	aImageVisual.Brushes()->AppendL( brush, EAlfHasOwnership );
	        }
	     else
	     	{
	     	CAlfGradientBrush* brush = static_cast<CAlfGradientBrush*>(&brushArray->At(0));
	     	if ( brush->Color(0) != mData->mIconBgColor.mColor )
		     	{
		     	brush->SetColor( mData->mIconBgColor.mColor );  	
		     	}
	     	}
    	}
    }
    
// ---------------------------------------------------------------------------
// RemoveBrushOnIcon
// ---------------------------------------------------------------------------
//
void MulBaseElement::RemoveBrushOnIcon( CAlfImageVisual& aImageVisual )
    {  
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::RemoveBrushOnIcon"); 
	CAlfBrushArray* brushArray = aImageVisual.Brushes();
    if(brushArray && (brushArray->Count()!=0))
        {
     	brushArray->Remove(0);
        }
    }    

// ---------------------------------------------------------------------------
// Template2D
// ---------------------------------------------------------------------------
//
MulCoverFlowTemplate*  MulBaseElement::Template2D()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::Template2D");
	return ((MulCoverFlowControl*)&control())->Template2D(); 
	}

// ---------------------------------------------------------------------------
// SetScrollAnimationTime
// ---------------------------------------------------------------------------
//	    
void MulBaseElement::SetScrollAnimationTime( int aScrollAnimationTime )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetScrollAnimationTime");
    mData->mScrollAnimationTime = aScrollAnimationTime;    
    }
   
// ---------------------------------------------------------------------------
// ArrangeItems
// ---------------------------------------------------------------------------
//	    
void MulBaseElement::ArrangeItems()
    { 
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ArrangeItems");
		
	control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushOnlyForced); 

	int totalVisual = 2*mData->mVisibleCount + 1;
	int highlightIndex = mData->mVisibleCount;
	CAlfImageVisual* imageVisual = NULL;	


	int posX = mData->mHighlightItemStruct.posx - mData->mVisibleCount*(mData->mItemStruct.width + mData->mPadding);
	
	TAlfRealPoint highlightItemSize(mData->mHighlightItemStruct.width, mData->mHighlightItemStruct.height);
	TAlfRealPoint nonHighlightItemSize(mData->mItemStruct.width, mData->mItemStruct.height);

	for (int i=0; i < totalVisual ; i++)  
	    {
	    CAlfVisual& deckLayout = mData->mIconFlowLayout->Visual(i);		
		imageVisual = (CAlfImageVisual*)deckLayout.FindTag(KCoverflowIcon);
	    
		if (i != highlightIndex)
			{
			deckLayout.SetPos( TAlfRealPoint(posX,mData->mItemStruct.posy));
		 	deckLayout.SetSize(nonHighlightItemSize);
		   	imageVisual->SetSize(nonHighlightItemSize);
		   	posX = posX + mData->mItemStruct.width + mData->mPadding;
			}
		else
			{
		    deckLayout.SetPos( TAlfRealPoint(mData->mHighlightItemStruct.posx,mData->mHighlightItemStruct.posy));
		 	deckLayout.SetSize(highlightItemSize);
		   	imageVisual->SetSize(highlightItemSize);
		   	posX = posX + mData->mHighlightItemStruct.width + mData->mPadding;
			}
		if (mData->mFitMode)
			{
			ApplyScaleMode(*imageVisual);
			}
	   	CAlfImageVisual* indicatorVisual =static_cast<CAlfImageVisual*>(deckLayout.FindTag(KCoverflowIndicator));    
        if (indicatorVisual)
            {
            SetIndicatorDimensions(*indicatorVisual);    
            }
	
	    }
	    
	control().Env().BatchBufferHandler().FlushBatchBuffer();            
	control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushDeferred);           
	        
    }

// ---------------------------------------------------------------------------
// SetScrollDir
// ---------------------------------------------------------------------------
//    
void MulBaseElement::SetScrollDir( TItemScroll aScrollDir )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetScrollDir");
    mData->mScrollDir = aScrollDir;   
    }

// ---------------------------------------------------------------------------
// SetScrollDir
// ---------------------------------------------------------------------------
//    
TItemScroll MulBaseElement::ScrollDir()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ScrollDir");
    return mData->mScrollDir;   
    }    

// ---------------------------------------------------------------------------
// CalculatePadding
// ---------------------------------------------------------------------------
//	
void MulBaseElement::CalculatePadding()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CalculatePadding");
    
    // The vector has already been created when the template was set. 
     if( mData->mVisibleCount == 1)
        {
        if( mData->mHighlightItemStruct.posx > KInitialvalueZero )
            {
            mData->mPadding = mData->mHighlightItemStruct.posx;
            }
        else
            {
            // full screen template ..photos
            // padding between the highlight and non highlight item is considered to be 2 pixel for 
            // full screen template as only one visual is visible.
            mData->mPadding = 2;
            }
        }
   else
        {
        int startx = mData->mHighlightItemStruct.posx+ mData->mHighlightItemStruct.width;
        mData->mPadding = mData->mItemStruct.posx - startx;
        } 
    }


// ---------------------------------------------------------------------------
// GetIconSize
// ---------------------------------------------------------------------------
//
void MulBaseElement::GetIconSize( mulwidget::TLogicalTemplate aTemplateId, 
		    mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::GetIconSize");

	bool isLandscape = ((MulCoverFlowControl*)&control())->IsLandscape();
	switch(aTemplateId)
		{
		case mulwidget::KTemplate1:
			{
			MulCoverFlowTemplate1::GetIconSize(aAttribute,isLandscape,aHeight,aWidth);
			}
		break;
		case mulwidget::KTemplate4:
			{
			MulCoverFlowTemplate4::GetIconSize(aAttribute,isLandscape,aHeight,aWidth);
			}
		break;
		default:
			{
			// Invalid template id
			// raise exception
			__MUL_ASSERT(false,KInvalidTemplate);
			}
		break;
		}
	}


// ---------------------------------------------------------------------------
// ShowWidget
// ---------------------------------------------------------------------------
//		
void MulBaseElement::ShowWidget( bool aShow, int aTransitionTime ) 
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ShowWidget");
	float opacity ;
	CAlfLayout* main = (CAlfLayout*) findVisual( KMainLayoutIndex );

    if( aShow )
		{
		opacity =  KHighlightOpacity;
		}
	else
		{
		opacity =  KBackGroundOpacity;
		}
	main->SetOpacity(TAlfTimedValue(opacity,aTransitionTime));
	}

// ---------------------------------------------------------------------------
// makeInterface
// ---------------------------------------------------------------------------
//	        
IAlfInterfaceBase* MulBaseElement::makeInterface( const IfId& aType )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::makeInterface");
    UString param(aType.mImplementationId);
     
    if(param == MulBaseElement::Type().mImplementationId )
        {
        return static_cast<IAlfElement*>(this);  
        }    
   else if(param == IAlfWidgetEventHandler::type().mImplementationId )
        {
        return static_cast<IAlfWidgetEventHandler*>(this);  
        }
    else 
        {
        return AlfElement::makeInterface(aType);    
        }
    }

// ---------------------------------------------------------------------------
// setEventHandlerData
// ---------------------------------------------------------------------------
//
void MulBaseElement::setEventHandlerData( const AlfWidgetEventHandlerInitData& /*aData*/ )
	{
	//No implementation required		
	}

// ---------------------------------------------------------------------------
// eventHandlerData
// ---------------------------------------------------------------------------
//
AlfWidgetEventHandlerInitData* MulBaseElement::eventHandlerData()
	{
    return NULL;		
	}
	
// ----------------------------------------------------------------------------
// eventHandlerType
// ----------------------------------------------------------------------------
//
 IAlfWidgetEventHandler::AlfEventHandlerType MulBaseElement::eventHandlerType() 
	{
	return IAlfWidgetEventHandler::ELogicalEventHandler ;
	}
// ----------------------------------------------------------------------------
// eventExecutionPhase
// ----------------------------------------------------------------------------
//

IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase MulBaseElement::eventExecutionPhase()
	{
	return EBubblingPhaseEventHandler;
	}   
	
//------------------------Slider related Api's -------------------------	

//--------------------------------------------------------------------------
// UpdateSliderTick
//--------------------------------------------------------------------------
//
void MulBaseElement::UpdateSliderTick(int aUpdateTick)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::MulBaseElement");
	IMulSliderModel* mulSliderModel = ((MulCoverFlowControl*)&control())->GetSliderModel();
    if( mulSliderModel && Template2D()->IsSliderVisible() )
		{
		 // Creating a reference Visual Item
    	//@todo remove const from Data api in slider model
		const MulVisualItem& item = mulSliderModel->Data();
		//auto_ptr<MulVisualItem> tempItem( new (EMM) MulVisualItem());
		MulVisualItem* tempItem  = const_cast<MulVisualItem*> (&item);
		  
		tempItem->SetAttribute( mulvisualitem::KMulMaxRange, --aUpdateTick );
		tempItem->SetAttribute (mulvisualitem::KMulMinRange, 0);
		tempItem->SetAttribute ( mulvisualitem::KMulTick,1);
		
		// Add the newly created visual item to the model
		// Model takes the ownership of the visual item
		mulSliderModel->SetData(*tempItem);   
		}
		
	// Set the opacity to the slider widget after verifyiny all the conditions.
	SetSliderOpacity();
	}
//--------------------------------------------------------------------------
// SetSliderOpacity
//--------------------------------------------------------------------------
//	
void MulBaseElement::SetSliderOpacity()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetSliderOpacity");
	
	if (mData->mSliderLayout)
		{
		if ( Template2D()->IsSliderVisible() )
	    	{
	    	if( static_cast<MulCoverFlowControl&>(control()).TotalModelCount() > 1 && mData->mSliderOpacity == KBackGroundOpacity)
		    	{
		     	ShowSlider(true);	    	
		    	}
		    else if((static_cast<MulCoverFlowControl&>(control()).TotalModelCount() <= 1) && mData->mSliderOpacity == KHighlightOpacity)
		    	{
		    	ShowSlider(false);	    	
		    	}
	    	}
	     else if(mData->mSliderOpacity == KHighlightOpacity)
	     	{
	     	 ShowSlider(false);	
	     	}
		}
	}

//--------------------------------------------------------------------------
//CreateSlider
//--------------------------------------------------------------------------
//	
void MulBaseElement::CreateSlider()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateSlider");
	if ( Template2D()->IsSliderVisible() )
    	{
    	MulCoverFlowControl* coverflowControl = static_cast<MulCoverFlowControl*>(&control());
    	IAlfWidget* sliderwidget = coverflowControl->GetSliderWidget();
    	if(!sliderwidget)
			{
			ConstructSliderWidget();
			}

		const TMulCoverFlowItem sliderStruct = Template2D()->SliderDimension();
		mData->mSliderLayout->SetPos(TAlfRealPoint(sliderStruct.posx,sliderStruct.posy));
		mData->mSliderLayout->SetSize(TAlfRealPoint(sliderStruct.width,sliderStruct.height));
		int totalModelCount = coverflowControl->TotalModelCount();
		if( totalModelCount > 1 )
			{
			UpdateSliderTick(totalModelCount);	
			coverflowControl->SetSliderTickPosition();
			ShowSlider(true);
			}
		else
			{
			ShowSlider(false);
			}
	    mData->mSliderLayout->UpdateChildrenLayout();
    	}
    else
    	{
    	if (!mData->mSliderLayout)
	    	{
	    	return;	
	    	}
	    ShowSlider(false);
    	}
	}

//--------------------------------------------------------------------------
//ConstructSliderWidget
//--------------------------------------------------------------------------	
void MulBaseElement::ConstructSliderWidget()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ConstructSliderWidget");
	IAlfWidget* alfSliderWidget = ((MulCoverFlowControl*)&control())->CreateSliderWidget();
	IMulSliderWidget* mulSliderWidget = static_cast<IMulSliderWidget*>(alfSliderWidget);
	mData->mSliderLayout = const_cast<CAlfLayout*>(&mulSliderWidget->ContainerLayout());
	mData->mVerticalFlow->Insert(mData->mSliderLayout, mData->mVerticalFlow->Count());
	}
	

//--------------------------------------------------------------------------
//ShowSlider
//--------------------------------------------------------------------------
void MulBaseElement::ShowSlider(bool aFlag)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::ShowSlider");
	if(mData->mSliderLayout)
		{
		IAlfWidget* alfSliderWidget = ((MulCoverFlowControl*)&control())->GetSliderWidget();
		IMulSliderWidget* mulSliderWidget = static_cast<IMulSliderWidget*>(alfSliderWidget);
		mulSliderWidget->ShowWidget(aFlag);
		if(aFlag)
			{
			mData->mSliderLayout->SetOpacity(TAlfTimedValue(KHighlightOpacity));
			mData->mSliderOpacity = KHighlightOpacity;							
			}
		else
			{
    		mData->mSliderLayout->SetOpacity(TAlfTimedValue(KBackGroundOpacity));
    		mData->mSliderOpacity = KBackGroundOpacity;			
			}
		}
	}	
	
    
//--------------------------------------------------------------------------
// StartBounce
//--------------------------------------------------------------------------
void MulBaseElement::StartBounce(int aBounceDirection) 
    {
    int bounceDistance = (mData->mHighlightItemStruct.width/4) * aBounceDirection; 	
    control().Env().Send(TAlfCustomEventCommand(ECustomEventBounceBack,&control()),KBounceTime); 
 	StartDoodling(bounceDistance,KBounceTime);
    }


// ---------------------------------------------------------------------------
// CalculateDistanceChangeFactor
// ---------------------------------------------------------------------------
//
int MulBaseElement::CalculateDistanceChangeFactor()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CalculateDistanceChangeFactor");
   
    int firstVisualMidPoint = mData->mHighlightItemStruct.posx + (mData->mHighlightItemStruct.width/2);
    int secVisualMidPoint =  mData->mItemStruct.posx + (mData->mItemStruct.width/2);
   
    return Abs(secVisualMidPoint-firstVisualMidPoint);
    }


// ---------------------------------------------------------------------------
// SetBackground
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetBackground(const TRgb& aColor)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetBackground");
	
	CAlfLayout* main = static_cast<CAlfLayout*>(findVisual( KMainLayoutIndex ));
	CAlfDeckLayout& deckLayout = static_cast<CAlfDeckLayout&> (main->Visual(KMainLayoutIndex) );
	
	CAlfVisual* oldVisual = deckLayout.FindTag(backgroundvisual);
	if(oldVisual)
		{
		CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(oldVisual);
		if(imageVisual->Image().HasTexture())
			{
			const CAlfTexture& textureToUnload = imageVisual->Image().Texture();
			CAlfTextureManager& textureMgr = control().Env().TextureManager();    
			textureMgr.UnloadTexture(textureToUnload.Id());		
			}
					
		deckLayout.Remove( oldVisual );
		oldVisual->RemoveAndDestroyAllD();
		}
		
    deckLayout.EnableBrushesL( ETrue );
    CAlfBrushArray* brushArray = deckLayout.Brushes();
    if(brushArray)
        {
		if( brushArray->Count()!=0)
		    {
			CAlfGradientBrush& brush = static_cast<CAlfGradientBrush&>(brushArray->At(0));		    
		    brush.SetColor( aColor ); 
		    }
		else
			{
			THROW_IF_LEAVES
				(
				CAlfGradientBrush* brush = CAlfGradientBrush::NewL( control().Env() );
				// Set the brush colour
				brush->SetColor( aColor );
				// Apply brush and set the opacity .
				deckLayout.Brushes()->AppendL( brush, EAlfHasOwnership ); 
				); 	
			}
        }
	}

// ---------------------------------------------------------------------------
// SetBackground
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetBackground(const TAknsItemID& aIID)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetBackground");
	
	CAlfLayout* main = static_cast<CAlfLayout*>(findVisual( KMainLayoutIndex ));
	CAlfDeckLayout& deckLayout = static_cast<CAlfDeckLayout&> (main->Visual(KMainLayoutIndex) );

	CAlfBrushArray* brushArray = deckLayout.Brushes();
    if(brushArray)
        {     
		if( brushArray->Count()!=0)
		    {
		    brushArray->Reset();            
		    }
        }
		    
	TAlfRealSize layoutSize = deckLayout.Size().ValueNow();
	const TAlfImage image(aIID,TSize(layoutSize),EAspectRatioPreserved,NULL,-1,-1,0.f,0.f,1.f,
					1.f, TAlfTextureFlags(EAlfTextureFlagAutoSize | EAlfTextureFlagSkinContent),
					KAlfAutoGeneratedTextureId,NULL);							 
                    
	CAlfVisual* oldVisual = deckLayout.FindTag(backgroundvisual);
	if(oldVisual)
		{
		CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(oldVisual);
		if(imageVisual->Image().HasTexture())
			{
			const CAlfTexture& textureToUnload = imageVisual->Image().Texture();
			CAlfTextureManager& textureMgr = control().Env().TextureManager();    
			textureMgr.UnloadTexture(textureToUnload.Id());		
			}		
		imageVisual->SetImage(image);	
		}
	else
		{
		THROW_IF_LEAVES
			(
			CAlfImageVisual* imagevisual(CAlfImageVisual::AddNewL((CAlfControl&)control(), &deckLayout));
			deckLayout.MoveVisualToBack(*imagevisual);
			imagevisual->SetImage(image);
			imagevisual->SetFlag(EAlfVisualFlagIgnorePointer);
			imagevisual->SetPos(TPoint(0,0));
		    imagevisual->SetSize(layoutSize);
		    imagevisual->SetOpacity( KHighlightOpacity );
		    imagevisual->SetTagL( backgroundvisual );
			);	
		}	
	}

// ---------------------------------------------------------------------------
// SetItemBrush
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetItemBrush(const TRgb& aColor)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetItemBrush");
	mData->mIconBgColor.mIsColorSet = true;
	mData->mIconBgColor.mColor = aColor;
	
	int itemCount = 2*mData->mVisibleCount + 1;
	
	itemCount = itemCount > mData->mIconFlowLayout->Count() ? mData->mIconFlowLayout->Count() : itemCount;
	
	for(int i =0 ;i<itemCount;i++)
    	{
    	CAlfVisual& visual = mData->mIconFlowLayout->Visual( i );
        CAlfImageVisual* imageVisual = ( CAlfImageVisual* )visual.FindTag(KCoverflowIcon);
        ApplyBrushOnIcon( *imageVisual);    
    	}
    }

	
// ---------------------------------------------------------------------------
// SetTextColor
// ---------------------------------------------------------------------------
//
void MulBaseElement::SetTextColor(TTextVisualType aVisualType, const TRgb& aColor)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetTextColor");
    
    switch(aVisualType)
	    {
	    case ECounterVisual:
		    {
		    // save the values set by the application
		    mData->mCounterColor.mIsColorSet = true;
		    mData->mCounterColor.mColor = aColor;
		    
		    // apply the property if the visuals are already created
		    if(mData->mCounterVisual)
			    {
			    mData->mCounterVisual->SetColor(aColor);	
			    }
		    break;	
		    }
	    case ETilteDetailTextVisual:
		    {
		    // save the values set by the application
		    mData->mTextColor.mIsColorSet = true;
		    mData->mTextColor.mColor = aColor;

		    // apply the property if the visuals are already created
		    CAlfTextVisual* titleTextVisual =static_cast<CAlfTextVisual*>(mData->mTextLayout->FindTag(KCoverflowTitle));
	        if(titleTextVisual)
	            {
	            titleTextVisual->SetColor(aColor);
	            }
	        CAlfViewportLayout* viewPortLayout =static_cast<CAlfViewportLayout*>(mData->mTextLayout->FindTag(KDetailViewPortLayout));
	        if(viewPortLayout)
	            {
	            CAlfTextVisual* detailTextVisual =static_cast<CAlfTextVisual*>(viewPortLayout->FindTag(KCoverflowDetail));
	            if(detailTextVisual)
	                {
	                detailTextVisual->SetColor(aColor);
	                }
	            }
            // Apply the same color for empty text also
            if(mData->mEmptyTextVisual)
	            {
	            mData->mEmptyTextVisual->SetColor(aColor);	
	            }
		    break;	
		    }
		default:
			break;
	    }
    }

// ---------------------------------------------------------------------------
// TextColor
// ---------------------------------------------------------------------------
//
TMulVisualColorProperty MulBaseElement::TextColor(TTextVisualType aVisualType)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::TextColor");
    TMulVisualColorProperty colorProperty;
    
    switch(aVisualType)
	    {
	    case ECounterVisual:
		    {
		    colorProperty = mData->mCounterColor;
		    break;
		    }
	    case ETilteDetailTextVisual:
		    {
		    colorProperty = mData->mTextColor;
		    break;
		    }
		default:
			break;
	    }
	return colorProperty;
    }

// ---------------------------------------------------------------------------
// FlowLayout
// ---------------------------------------------------------------------------
//
CAlfLayout& MulBaseElement::FlowLayout( int aLayoutIndex )
      {
      MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::FlowLayout");
      
      return static_cast<CAlfLayout&>(mData->mVerticalFlow->Visual( aLayoutIndex ));
      }

// ---------------------------------------------------------------------------
// SetFitMode
// ---------------------------------------------------------------------------
//      
void MulBaseElement::SetFitMode(bool aFlag)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetFitMode");
	mData->mFitMode = aFlag;
	}
	
// ----------------------------------------------------------------------------
// CreateAndInitializeVisuals
// ----------------------------------------------------------------------------
//
void MulBaseElement::CreateAndInitializeVisuals(bool aIsOrientationChange)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateAndInitializeVisuals");
	CreateVisuals();

	// Specific to template 4 don't update the visuals
	// during orientation change
	// done for performance improvement.
	if (!(aIsOrientationChange &&mData->mFitMode))
		{
		UpdateVisuals();
		CreateSlider();
		}
	
	// For template 4 set model case
	// cancel the rotation if any.
	if (!aIsOrientationChange && mData->mFitMode)
		{
		if (mData->mRotation.mIsApplied)
			{
			if (mData->mRotation.mIndex >= 0)
				{
				CancelRotation(mData->mRotation.mIndex);
				}
			else
				{
				// rotation applied on highlight index.
				// so cancel it.
				mData->mRotation.mIndex = 1;
				CancelRotation(mData->mRotation.mIndex);
				}
			}
		}
	}
	
// ----------------------------------------------------------------------------
// CreateVisuals
// ----------------------------------------------------------------------------
//
void MulBaseElement::CreateVisuals()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateVisuals");
	int totalVisual = 2*mData->mVisibleCount + 1;
	int visualIndex = 0;
	
	// if there are less visuals than required tehn create the extra visauls and
	// set opacity 1 for already existing visuals
	if(totalVisual > mData->mIconFlowLayout->Count())
    	{
    	visualIndex = mData->mIconFlowLayout->Count(); 	
        for(; visualIndex < totalVisual;visualIndex++) 
            {
            CreateIconStructure(mData->mIconFlowLayout); 
            }
    	}
    // if the number of visuals are more, then set opacity 0 for the extra visuals
    else if (totalVisual < mData->mIconFlowLayout->Count())
        {
        IMulModelAccessor* accessor = ((MulCoverFlowControl*)&control())->ModelAccessor();
        if(accessor)
            {
            if(accessor->CurrentItemCount() < totalVisual)
	            {
	            for(; visualIndex < totalVisual;visualIndex++) 
	                {
	                CAlfVisual& visual = mData->mIconFlowLayout->Visual(visualIndex);
        			static_cast<MulCoverFlowControl&>(control()).UpdateItemAtIndex(visualIndex);	
	                }
	            }
	            
    	    CAlfTextureManager& textureMgr = control().Env().TextureManager();
    	    TAlfImage image = TAlfImage(textureMgr.BlankTexture());
    	    // position those visuals to some other location.
    	    int posX = mData->mHighlightItemStruct.posx + totalVisual * mData->mHighlightItemStruct.width;
            for(; visualIndex < mData->mIconFlowLayout->Count();visualIndex++) 
                {
	            CAlfVisual& visual = mData->mIconFlowLayout->Visual(visualIndex);
	            visual.SetPos(TAlfRealPoint(posX,mData->mHighlightItemStruct.posy));
	            CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(visual.FindTag(KCoverflowIcon));
		        // remove the icon brush if any.
		        RemoveBrushOnIcon(*imageVisual);        
		        imageVisual->SetImage(image); 
                CAlfImageVisual* indicatorVisual =static_cast<CAlfImageVisual*>(visual.FindTag(KCoverflowIndicator));    
                if(indicatorVisual)
                    {
                    indicatorVisual->SetImage(image);   
                    }
	            }
            }
        }
	}

// ----------------------------------------------------------------------------
// UpdateVisuals
// ----------------------------------------------------------------------------
//
void MulBaseElement::UpdateVisuals()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::UpdateVisuals");
	int totalVisual = 2*mData->mVisibleCount + 1;
	MulCoverFlowControl* coverflowControl = static_cast<MulCoverFlowControl*>(&control());
	IMulModelAccessor* accessor = coverflowControl->ModelAccessor();
	if(accessor)
    	{
    	for(int i = 0; i < totalVisual; i++) 
            {
            CAlfVisual& visual = mData->mIconFlowLayout->Visual(i);
        	coverflowControl->UpdateItemAtIndex(i);	          
            }
    	}
	}
	
// ----------------------------------------------------------------------------
// CreateIconStructure
// ----------------------------------------------------------------------------
//
CAlfVisual* MulBaseElement::CreateIconStructure(CAlfLayout* aParentLayout)
    { 
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CreateIconStructure");       
    //create the deck layout.
    CAlfDeckLayout* deck =CAlfDeckLayout::AddNewL((CAlfControl &)control(),aParentLayout);
    CAlfImageVisual* imageVisual =CAlfImageVisual::AddNewL((CAlfControl &)control(),deck);
    imageVisual->SetTagL(KCoverflowIcon);
	deck->SetFlag( EAlfVisualFlagManualLayout );
    return deck;
    }
	
	
// ----------------------------------------------------------------------------
// SetHighlight
// This function is used to change the highlight by 1 with animation.
// This function also takes care whether we need to change the size or only the position.
// it takes into consideration the current direction and the current highlight index.
// The logic is , we are always keeping the highlight visual index fixed (its equal to
// the no of visible item in the screen), Depending on the scroll direction we reorder
// the last or first visual and then position all visuals keeping the highlight visual
// in its position. Then update the last or 1st visual with appropriate data.
// ----------------------------------------------------------------------------
//
void MulBaseElement::SetHighlight(int /*aHighlightIndex*/, int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::SetHighlight");
	int totalVisual = 2*mData->mVisibleCount + 1;
	int highlightIndex = mData->mVisibleCount;
	CAlfVisual* deckLayout = NULL;
	CAlfVisual* imageVisual = NULL;	
	
	int highlightPosX = mData->mHighlightItemStruct.posx;

    if(mData->mScrollDir == EItemScrollLeft)
    	{
    	CancelRotation(totalVisual - 1);
		control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushOnlyForced);     	
		deckLayout = (CAlfVisual*)&mData->mIconFlowLayout->Visual(totalVisual - 1);
    	mData->mIconFlowLayout->Reorder(*deckLayout,0);
    	
		int posX = highlightPosX - mData->mVisibleCount*(mData->mItemStruct.width + mData->mPadding);
		deckLayout->SetPos( TAlfRealPoint(posX,mData->mItemStruct.posy));
		posX = posX + mData->mItemStruct.width + mData->mPadding;
		for (int i=1; i < totalVisual ; i++)  
		    {
			deckLayout = (CAlfVisual*)&mData->mIconFlowLayout->Visual(i);
			imageVisual = deckLayout->FindTag(KCoverflowIcon);
			if (i != highlightIndex)
				{
				deckLayout->SetPos( TAlfTimedPoint(posX,mData->mItemStruct.posy,aAnimationTime));
			   	posX = posX + mData->mItemStruct.width + mData->mPadding;
			   
			   	// Change the size if required
				if((i == highlightIndex+1 || i == highlightIndex-1) && mData->mChangeSize)
					{
					deckLayout->SetSize(TAlfTimedPoint(mData->mItemStruct.width,mData->mItemStruct.height,aAnimationTime));
					imageVisual->SetSize(TAlfTimedPoint(mData->mItemStruct.width,mData->mItemStruct.height,aAnimationTime));
					}
				}
			else
				{
			    deckLayout->SetPos( TAlfTimedPoint(highlightPosX,mData->mHighlightItemStruct.posy,aAnimationTime));
			   	posX = posX + mData->mHighlightItemStruct.width + mData->mPadding;
			   	
			   	// Change the size if required
			   	if(mData->mChangeSize)
				   	{
					deckLayout->SetSize(TAlfTimedPoint(mData->mHighlightItemStruct.width,
														mData->mHighlightItemStruct.height,
														aAnimationTime));
					imageVisual->SetSize(TAlfTimedPoint(mData->mHighlightItemStruct.width,
														mData->mHighlightItemStruct.height,
														aAnimationTime));
				   	}
				}
		    }
	    control().Env().BatchBufferHandler().FlushBatchBuffer();            
	  	control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushDeferred);           
		((MulCoverFlowControl*)&control())->UpdateItemAtIndex(0, aAnimationTime);

    	}
    else if (mData->mScrollDir == EItemScrollRight)
	    {
	    CancelRotation(0);
		control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushOnlyForced); 	    
		deckLayout = (CAlfVisual*)&mData->mIconFlowLayout->Visual(0);
    	mData->mIconFlowLayout->Reorder(*deckLayout,totalVisual-1);
    	
		int posX = highlightPosX - mData->mVisibleCount*(mData->mItemStruct.width + mData->mPadding);

		for (int i=0; i < totalVisual-1 ; i++)  
		    {
			deckLayout = (CAlfVisual*)&mData->mIconFlowLayout->Visual(i);
			imageVisual = deckLayout->FindTag(KCoverflowIcon);
		    
			if (i != highlightIndex)
				{
				deckLayout->SetPos( TAlfTimedPoint(posX,mData->mItemStruct.posy,aAnimationTime));
			   	posX = posX + mData->mItemStruct.width + mData->mPadding;
			   	// Change the size if required
				if((i == highlightIndex+1 || i == highlightIndex-1) && mData->mChangeSize)
					{
					deckLayout->SetSize(TAlfTimedPoint(mData->mItemStruct.width,mData->mItemStruct.height,aAnimationTime));
					imageVisual->SetSize(TAlfTimedPoint(mData->mItemStruct.width,mData->mItemStruct.height,aAnimationTime));
					}			   	
				}
			else
				{
			    deckLayout->SetPos( TAlfTimedPoint(highlightPosX,mData->mHighlightItemStruct.posy,aAnimationTime));
			   	posX = posX + mData->mHighlightItemStruct.width + mData->mPadding;
			   	// Change the size if required
			   	if(mData->mChangeSize)
				   	{
					deckLayout->SetSize(TAlfTimedPoint(mData->mHighlightItemStruct.width,
														mData->mHighlightItemStruct.height,
														aAnimationTime));
					imageVisual->SetSize(TAlfTimedPoint(mData->mHighlightItemStruct.width,
														mData->mHighlightItemStruct.height,
														aAnimationTime));
				   	}
				}
		    }
		deckLayout = (CAlfVisual*)&mData->mIconFlowLayout->Visual(totalVisual-1);
		deckLayout->SetPos( TAlfRealPoint(posX,mData->mItemStruct.posy));
	    control().Env().BatchBufferHandler().FlushBatchBuffer();            
	    control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushDeferred);           
		((MulCoverFlowControl*)&control())->UpdateItemAtIndex(totalVisual-1, aAnimationTime);
	    }
	    
	}
	
// ----------------------------------------------------------------------------
// StartDoodling
// This function is used in doodling case. it calls the appropriate function to
// handle doodling based on whether we have to only change the position or both
// position and size.
// ----------------------------------------------------------------------------
//
void MulBaseElement::StartDoodling(int aDistance, int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::StartDoodling");
	if (mData->mChangeSize)
		{
		MoveVisualsWithSizeChange(aDistance,aAnimationTime);
		}
	else
		{
		MoveVisuals(aDistance,aAnimationTime);
		}
	}
	
// ----------------------------------------------------------------------------
// StopDoodling
// it calls when we get a gestureunknown or to do a bounce back.
// it basically repositions the visuals based on current highlight.
// it simply calls a startdoodling function with 0 distance and with
//  animation time if required.
// ----------------------------------------------------------------------------
//
void MulBaseElement::StopDoodling(int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::StopDoodling");
	StartDoodling(0,aAnimationTime);
	}
	
	
// ----------------------------------------------------------------------------
// MoveVisuals
// This function is used in doodling when only position needs to be change, usefull
// in potrait where 1 visible item is in the screen. 
// The logic is to change the position of all visuals by doodling distance.
// ----------------------------------------------------------------------------
//
void MulBaseElement::MoveVisuals(int aDistance, int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::MoveVisuals");
	CancelRotation();
		
	control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushOnlyForced); 
	int totalVisual = 2*mData->mVisibleCount + 1;
	int highlightIndex = mData->mVisibleCount;
	
	int highlightPosX = mData->mHighlightItemStruct.posx;
	
	highlightPosX += aDistance;

	int posX = highlightPosX - mData->mVisibleCount*(mData->mItemStruct.width + mData->mPadding);
	for (int i=0; i < totalVisual ; i++)  
	    {
		CAlfVisual& deckLayout = mData->mIconFlowLayout->Visual(i);
		
		if (i != mData->mVisibleCount)
			{
			deckLayout.SetPos( TAlfTimedPoint(posX,mData->mItemStruct.posy,aAnimationTime));
		   	posX = posX + mData->mItemStruct.width + mData->mPadding;
			}
		else
			{
		    deckLayout.SetPos( TAlfTimedPoint(highlightPosX,mData->mHighlightItemStruct.posy,aAnimationTime));
		   	posX = posX + mData->mHighlightItemStruct.width + mData->mPadding;
			}
	    }
  	control().Env().BatchBufferHandler().FlushBatchBuffer();            
    control().Env().BatchBufferHandler().SetAutoFlushMode(EAlfAutoFlushDeferred);           
	}

// ----------------------------------------------------------------------------
// NumberOfSwipe
// ----------------------------------------------------------------------------
//	
TNumberofSwipes MulBaseElement::NumberOfSwipes()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::NumberOfSwipes");
	return mData->mNumSwipes;
	}
	
// ----------------------------------------------------------------------------
// FinalSwipeDirection
// ----------------------------------------------------------------------------
//
int MulBaseElement::FinalSwipeDirection(int aDistance, int aLastSwipeDirection)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::FinalSwipeDirection");
	
	int NewHighlightPos = NearestPoint(FinalMidPoints(ReferenceMidPoints(),aDistance),aLastSwipeDirection);
	
	int scrollDir = EItemNoDirection;
	mData->mNumSwipes = ESingleSwipe;
	
	// now depending on the new highlight position make a decission
	// whether to double swipe or single swipe and in which direction.
	if(mData->mVisibleCount == 1)
		{
		switch(NewHighlightPos)
			{
			case 0:
				{
				scrollDir = EItemScrollLeft;	
				}
			break;
			case 2:
				{
				scrollDir = EItemScrollRight;	
				}
			break;
			case 1:
			default:
			break;				
			}
		}
	else
		{
		switch(NewHighlightPos)
			{
			case 0:
				{
				mData->mNumSwipes = ETripleSwipe; // triple swipe
				scrollDir = EItemScrollLeft;	
				}
			break;
			case 1:
				{
				mData->mNumSwipes = EDoubleSwipe;
				scrollDir = EItemScrollLeft;	
				}
			break;	
			case 2:
				{
				scrollDir = EItemScrollLeft;	
				}
			break;					
			case 4:
				{
				scrollDir = EItemScrollRight;	
				}
			break;
			case 5:
				{
				mData->mNumSwipes = EDoubleSwipe;
				scrollDir = EItemScrollRight;
				}
			break;
			case 6:
				{
				mData->mNumSwipes = ETripleSwipe; // triple swipe
				scrollDir = EItemScrollRight;
				}
			break;									
			case 3:
			default:
			break;				
			}
		}
	return scrollDir;							
	}	

// ----------------------------------------------------------------------------
// NearestPoint
// ----------------------------------------------------------------------------
//
int MulBaseElement::NearestPoint(std::vector<int> aFinalMidPoints,int aLastSwipeDirection)
	{
	int referencePoint = mData->mHighlightItemStruct.posx + mData->mHighlightItemStruct.width/2;
	int i = 0;
	while(i < aFinalMidPoints.size())
		{
		int temp = 	aFinalMidPoints[i];
		if(aFinalMidPoints[i] > referencePoint)
			{
			break;	
			}
		i++;
		}
      
	if(aLastSwipeDirection == EEventScrollLeft)
		{
		return --i;
		}
	return i;
	}
	
// ----------------------------------------------------------------------------
// ReferenceMidPoints
// ----------------------------------------------------------------------------
//
std::vector<int> MulBaseElement::ReferenceMidPoints()
	{
	std::vector<int> referenceMidPoints;
	int highlightMidPoint = mData->mHighlightItemStruct.posx + mData->mHighlightItemStruct.width/2;
	int differnce = mData->mHighlightItemStruct.width/2
			+ mData->mPadding + mData->mItemStruct.width/2 ;
			
	int doubleDiff = mData->mPadding + mData->mItemStruct.width;
	if(mData->mVisibleCount != 1)
		{
		referenceMidPoints.push_back(highlightMidPoint - (differnce
			+ 2 * doubleDiff));
			
		referenceMidPoints.push_back(highlightMidPoint - (differnce
			+ doubleDiff));
		}
	referenceMidPoints.push_back(highlightMidPoint - differnce);
	referenceMidPoints.push_back(highlightMidPoint);
	referenceMidPoints.push_back(highlightMidPoint + differnce);

	if(mData->mVisibleCount != 1)
		{
		referenceMidPoints.push_back(highlightMidPoint + (differnce
			+ 2 * doubleDiff));	
			
		referenceMidPoints.push_back(highlightMidPoint + (differnce
			+ doubleDiff));						
		}	
	return referenceMidPoints;
	}

// ----------------------------------------------------------------------------
// FinalMidPoints
// ----------------------------------------------------------------------------
//
std::vector<int> MulBaseElement::FinalMidPoints(std::vector<int> aReferenceMidPoints,int aDistance)
	{
	std::vector<int> newMidPoints;
	for(int i = 0; i < aReferenceMidPoints.size(); i++)
		{
		newMidPoints.push_back(aReferenceMidPoints[i] + aDistance);
		int temp = 	newMidPoints[i];
		}
	return newMidPoints;	
	}

// ----------------------------------------------------------------------------
// MoveVisualsWithSizeChange
// This function used when during doodling size changes also required.
// ie in case of landscape and visible items are 3 during doodling position as well
// as size change requird.
// The basic logic is we are keeping a reference distance for a complete highlight 
// change (that means the highlight becomes nonhighlight and vice versa). so at 
// any point of time there will be only two visuals whose size needs to be change.
// one to grow and other to shrink. so we  calculate the % of grow and shrink of
// visual keeping the doodling distance and reference distance into consideration.
// ----------------------------------------------------------------------------
//
void MulBaseElement::MoveVisualsWithSizeChange(int aDistance, int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::MoveVisualsWithSizeChange");
	int totalVisual = 2*mData->mVisibleCount + 1;
	int highlightIndex = mData->mVisibleCount;
	CAlfVisual* imageVisual = NULL;	
	int visualIndexToShrink = -1;
	int visualIndexToGrow = -1;  
	int updateSpecialIndex = -1;  
	int distancefactor = CalculateDistanceChangeFactor ();
	
	int highlightPosX = mData->mHighlightItemStruct.posx;
	highlightPosX += aDistance;
	
	
	if (Abs(aDistance) < distancefactor)
		{
		// user has doodled to change the highlight by 1.Single swipe.
		if (aDistance < 0)
			{
			visualIndexToGrow = highlightIndex +1;
			}
		else if (aDistance > 0)
			{
			visualIndexToGrow = highlightIndex -1;
			}
		else
			{
			// for stop doodling case
			visualIndexToGrow = highlightIndex - 1;
			updateSpecialIndex = highlightIndex + 1;
			}
		visualIndexToShrink = highlightIndex;
		}
	else
		{
		// user has doodled to change the highlight by 2. Double swipe.
		if (aDistance < 0)
			{
			visualIndexToShrink = highlightIndex +1;
			visualIndexToGrow = highlightIndex +2;
			aDistance +=distancefactor;
			}
		else
			{
			visualIndexToShrink = highlightIndex -1;
			visualIndexToGrow = highlightIndex -2;
			aDistance -=distancefactor;
			}
		}
	int changeInPosY = (((mData->mItemStruct.posy - mData->mHighlightItemStruct.posy)*Abs(aDistance))/distancefactor);
	int changeInWidth = (((mData->mHighlightItemStruct.width - mData->mItemStruct.width)*Abs(aDistance))/distancefactor);
	int changeInHeight = (((mData->mHighlightItemStruct.height - mData->mItemStruct.height)*Abs(aDistance))/distancefactor);


	int posX = highlightPosX - mData->mVisibleCount*(mData->mItemStruct.width + mData->mPadding);
	for (int i=0; i < totalVisual && i < mData->mIconFlowLayout->Count(); i++)  
	    {
		CAlfVisual& deckLayout = mData->mIconFlowLayout->Visual(i);
		if (i == visualIndexToShrink)
			{
			int posY = mData->mHighlightItemStruct.posy + changeInPosY;
			int width =mData->mHighlightItemStruct.width - changeInWidth;
			int height =mData->mHighlightItemStruct.height - changeInHeight;
			deckLayout.SetPos( TAlfTimedPoint(posX,posY,aAnimationTime));
			imageVisual = deckLayout.FindTag(KCoverflowIcon);
			deckLayout.SetSize(TAlfTimedPoint(width,height,aAnimationTime));
			imageVisual->SetSize(TAlfTimedPoint(width,height,aAnimationTime));
			posX = posX + width + mData->mPadding;
			}
		else if (i == visualIndexToGrow)
			{
			int posY = mData->mItemStruct.posy - changeInPosY;
			int width = mData->mItemStruct.width + changeInWidth;
			int height = mData->mItemStruct.height + changeInHeight;
			deckLayout.SetPos( TAlfTimedPoint(posX,posY,aAnimationTime));
			imageVisual = deckLayout.FindTag(KCoverflowIcon);
			deckLayout.SetSize(TAlfTimedPoint(width,height,aAnimationTime));
			imageVisual->SetSize(TAlfTimedPoint(width,height,aAnimationTime));
			posX = posX + width + mData->mPadding;
			}
		else if (i == updateSpecialIndex)
			{
			deckLayout.SetPos( TAlfTimedPoint(posX,mData->mItemStruct.posy,aAnimationTime));
			imageVisual = deckLayout.FindTag(KCoverflowIcon);
			deckLayout.SetSize(TAlfTimedPoint(mData->mItemStruct.width,mData->mItemStruct.height,aAnimationTime));
			imageVisual->SetSize(TAlfTimedPoint(mData->mItemStruct.width,mData->mItemStruct.height,aAnimationTime));
		   	posX = posX + mData->mItemStruct.width + mData->mPadding;
			}
		else
			{
			deckLayout.SetPos( TAlfTimedPoint(posX,mData->mItemStruct.posy,aAnimationTime));
		   	posX = posX + mData->mItemStruct.width + mData->mPadding;
			}
	    }
	}

// ----------------------------------------------------------------------------
//RecycleIconVisuals
// ----------------------------------------------------------------------------
//
void MulBaseElement::RecycleIconVisuals()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::RecycleIconVisuals");
    CAlfTextureManager& textureMgr = control().Env().TextureManager();
    TAlfImage image = TAlfImage(textureMgr.BlankTexture());
    
    for(int i=0; i<mData->mIconFlowLayout->Count(); i++)
        {
        CAlfVisual& visual = mData->mIconFlowLayout->Visual(i);
        CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(visual.FindTag(KCoverflowIcon)); 
        // remove the icon brush if any.
        RemoveBrushOnIcon(*imageVisual);        
        imageVisual->SetImage(image);
        
        CAlfImageVisual* indicatorVisual =static_cast<CAlfImageVisual*>(visual.FindTag(KCoverflowIndicator));    
        if(indicatorVisual)
            {
            indicatorVisual->SetImage(image);   
            }
        }
	// cancel the rotation if any.
	if (mData->mRotation.mIsApplied)
		{
		if (mData->mRotation.mIndex >= 0)
			{
			CancelRotation(mData->mRotation.mIndex);
			}
		else
			{
			// rotation applied on highlight index.
			// so cancel it.
			mData->mRotation.mIndex = 1;
			CancelRotation(mData->mRotation.mIndex);
			}
		}
    }

// ----------------------------------------------------------------------------
//IdToTag
// ----------------------------------------------------------------------------
//
const TDesC8& MulBaseElement::IdToTag( int aId )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::IdToTag");
    switch( aId )
        {
        case mulvisualitem::KMulIcon1 :
            {
            return KCoverflowIcon;
            }
        case mulvisualitem::KMulTitle :
            {
            return KCoverflowTitle;
            }    
        case mulvisualitem::KMulDetail :
            {
            return KCoverflowDetail;
            }  
        case mulvisualitem::KMulIndicator2 :
            {
            return KCoverflowIndicator;
            }                               
        default:
            {
            return KInvalidID;
            }
        }
    }
 
// ---------------------------------------------------------------------------
// RemoveSliderFromLayout
// ---------------------------------------------------------------------------
//
void MulBaseElement::RemoveSliderFromLayout()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::RemoveSliderFromLayout");
    IAlfWidget* slider = (static_cast<MulCoverFlowControl&>(control())).GetSliderWidget();
    if(slider && slider->control())
        {
        IMulSliderWidget* sliderwidget = static_cast<IMulSliderWidget*>(slider);
        CAlfLayout* sliderContlyt = const_cast<CAlfLayout*>(sliderwidget->control()->ContainerLayout(NULL));
        mData->mVerticalFlow->Remove(sliderContlyt);
        }
    
    }    

// ---------------------------------------------------------------------------
// RotateImage
// ---------------------------------------------------------------------------
//
void MulBaseElement::RotateImage(IMulWidget::TMulRotation aDirection,TSize aImageSize,int aAnimationTime,bool aAdjustSize)
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::RotateImage");
	
	__MUL_ASSERT( mData->mIconFlowLayout->Count() != 0,KNullPointer);
		
	if (Template2D()->IsUiOnOffFlagEnabled())
		{
		CAlfVisual& visual = mData->mIconFlowLayout->Visual(1);
		CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(visual.FindTag(KCoverflowIcon)); 
		
		if (!aAdjustSize)
			{
			// Take care the case where rotation was
			// applied to the previous highlight index
			// cancel the rotation on that visual  
			CancelRotation(mData->mRotation.mIndex);

			switch(aDirection)
				{
				case IMulWidget::ERotateLeft:
					{
					mData->mRotation.mImageAngle = (mData->mRotation.mImageAngle + 90)%360;
					imageVisual->EnableTransformationL();
					imageVisual->Transformation().Rotate(TAlfTimedValue(90,aAnimationTime));
					mData->mRotation.mIsApplied = true;
					break;
					}
				case IMulWidget::ERotateRight:
					{
					mData->mRotation.mImageAngle = (mData->mRotation.mImageAngle - 90)%360;
					imageVisual->EnableTransformationL();
					imageVisual->Transformation().Rotate(TAlfTimedValue(-90,aAnimationTime));
					mData->mRotation.mIsApplied = true;
					break;
					}
				default:
					break;
				}
			// store the actual texture size of image to be rotated.
			mData->mRotation.mTextureSize = aImageSize;
			}
			
		AdjustVisualSizeInRotation(imageVisual, aAnimationTime);
		}
	}

// ---------------------------------------------------------------------------
// AdjustVisualSizeInRotation
// ---------------------------------------------------------------------------
//
void MulBaseElement::AdjustVisualSizeInRotation(CAlfImageVisual* aVisual,int aAnimationTime)
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::AdjustVisualSizeInRotation");

	bool isLandScape = static_cast<MulCoverFlowControl&>(control()).IsLandscape();
	int shortEdge = !isLandScape ? mData->mHighlightItemStruct.width : mData->mHighlightItemStruct.height;
		 			 			
	if( mData->mRotation.mTextureSize.iWidth <= shortEdge && mData->mRotation.mTextureSize.iHeight <= shortEdge)
	    {
	    return;
	    }
	else
	    {
	    switch(Abs(mData->mRotation.mImageAngle))
	    	{
	    	case 90:
	    	case 270:
	        	{
	        	HandleRotationOnVerticalAxis(aVisual,aAnimationTime);
	        	}
	    	break;
	    	case 0:
	    	case 180:
	    		{
	    		HandleRotationOnHorizontalAxis(aVisual,aAnimationTime);
	    		}
	        break;
	    	default:
	    		break;
	    	}
	    }
	}

// ---------------------------------------------------------------------------
// HandleRotationOnVerticalAxis
// ---------------------------------------------------------------------------
//
void MulBaseElement::HandleRotationOnVerticalAxis(CAlfImageVisual* aVisual,int aAnimationTime)
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::HandleRotationOnVerticalAxis");

	bool isLandScape = static_cast<MulCoverFlowControl&>(control()).IsLandscape();
	int longEdge = isLandScape ? mData->mHighlightItemStruct.width : mData->mHighlightItemStruct.height;
	int shortEdge = !isLandScape ? mData->mHighlightItemStruct.width : mData->mHighlightItemStruct.height;

	if (isLandScape)
		{
		if(mData->mRotation.mTextureSize.iWidth > shortEdge) 
			{
			// make the visual of shortEdge size and fit the image by width.
			aVisual->SetPos(TAlfRealPoint(longEdge/2 - shortEdge/2,0),aAnimationTime);
			aVisual->SetSize(TAlfRealSize(shortEdge,shortEdge),aAnimationTime);
			aVisual->SetScaleMode(CAlfImageVisual::EScaleFitWidth); 
			}
		else 
			{
			//imageHeight > shortEdge and imageWidth < shortEdge
			if (mData->mRotation.mTextureSize.iHeight > longEdge)
				{
				aVisual->SetPos(TAlfRealPoint(longEdge/2 - shortEdge/2,shortEdge/2 -longEdge/2),aAnimationTime);
				aVisual->SetSize(TAlfRealSize(shortEdge,longEdge),aAnimationTime);
				}
			else
				{
				aVisual->SetPos(TAlfRealPoint(longEdge/2 - shortEdge/2,shortEdge/2 - mData->mRotation.mTextureSize.iHeight/2),aAnimationTime);
				aVisual->SetSize(TAlfRealSize(shortEdge,mData->mRotation.mTextureSize.iHeight),aAnimationTime);
				}
			aVisual->SetScaleMode(CAlfImageVisual::EScaleFitHeight);            				
			}
		}
	else
		{
		if(mData->mRotation.mTextureSize.iHeight > shortEdge) 
			{
			// make the visual of shortEdge size and fit the image by height.
			aVisual->SetPos(TAlfRealPoint(0,longEdge/2 - shortEdge/2),aAnimationTime);
			aVisual->SetSize(TAlfRealSize(shortEdge,shortEdge),aAnimationTime);
			aVisual->SetScaleMode(CAlfImageVisual::EScaleFitHeight);
			}
		else  
			{
			//imageWidth > shortEdge and imageHeight < shortEdge
			if (mData->mRotation.mTextureSize.iWidth > longEdge)
				{
				aVisual->SetPos(TAlfRealPoint(shortEdge/2 - longEdge/2,longEdge/2 - shortEdge/2),aAnimationTime);
				aVisual->SetSize(TAlfRealSize(longEdge,shortEdge),aAnimationTime);
				}
			else
				{
				aVisual->SetPos(TAlfRealPoint(shortEdge/2 - mData->mRotation.mTextureSize.iWidth/2,longEdge/2 - shortEdge/2),aAnimationTime);
				aVisual->SetSize(TAlfRealSize(mData->mRotation.mTextureSize.iWidth,shortEdge),aAnimationTime);
				}
			// @todo check if this effective height after fit by width can ever be more than
			// the shorter edge. If yes then this fit mode should be chnged to fit inside(check this)
			aVisual->SetScaleMode(CAlfImageVisual::EScaleFitWidth);
			}
		}
	}

// ---------------------------------------------------------------------------
// HandleRotationOnHorizontalAxis
// ---------------------------------------------------------------------------
//	
void MulBaseElement::HandleRotationOnHorizontalAxis(CAlfImageVisual* aVisual,int aAnimationTime)
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::HandleRotationOnHorizontalAxis");
	
	aVisual->SetPos(TAlfRealPoint(0,0),aAnimationTime);
	aVisual->SetSize(TAlfRealSize(mData->mHighlightItemStruct.width,mData->mHighlightItemStruct.height),aAnimationTime);
	ApplyScaleMode(*aVisual);
	}
		
// ---------------------------------------------------------------------------
// CancelRotation
// ---------------------------------------------------------------------------
//
void MulBaseElement::CancelRotation(int aIndex)
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CancelRotation");
	
	if (mData->mRotation.mIsApplied)
		{
		if (mData->mRotation.mIndex >= 0)
			{
			// revert it back
			mData->mRotation.mImageAngle = 0;
			CAlfVisual& deckLayout = mData->mIconFlowLayout->Visual(mData->mRotation.mIndex);
			CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(deckLayout.FindTag(KCoverflowIcon));
			imageVisual->EnableTransformationL();
			imageVisual->Transformation().LoadIdentity();
			// reset the rotation property
			mData->mRotation.mIsApplied = false;
			mData->mRotation.mIndex = -1;   
        	imageVisual->SetPos(TAlfRealPoint(0,0));
			imageVisual->SetSize(TAlfRealSize(mData->mHighlightItemStruct.width,mData->mHighlightItemStruct.height));
			ApplyScaleMode(*imageVisual);			
			}
		else
			{
			mData->mRotation.mIndex = aIndex;
			}
		}	    
	}

// ---------------------------------------------------------------------------
// CancelRotationOnUpdate
// ---------------------------------------------------------------------------
//
void MulBaseElement::CancelRotationOnUpdate(int aIndex)
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::CancelRotationOnUpdate");
	
	if (mData->mRotation.mIsApplied)
		{
		if (mData->mRotation.mIndex < 0 || mData->mRotation.mIndex == aIndex)
			{
			// revert it back
			mData->mRotation.mImageAngle = 0;
			CAlfVisual& deckLayout = mData->mIconFlowLayout->Visual(aIndex);
			CAlfImageVisual* imageVisual = static_cast<CAlfImageVisual*>(deckLayout.FindTag(KCoverflowIcon));
			imageVisual->EnableTransformationL();
			imageVisual->Transformation().LoadIdentity();
			// reset the rotation property
			mData->mRotation.mIsApplied = false;
			mData->mRotation.mIndex = -1;   
        	imageVisual->SetPos(TAlfRealPoint(0,0));
			imageVisual->SetSize(TAlfRealSize(mData->mHighlightItemStruct.width,mData->mHighlightItemStruct.height));
			ApplyScaleMode(*imageVisual);			
			}
		}	    
	}
		
// ---------------------------------------------------------------------------
// UpdateRotation
// ---------------------------------------------------------------------------
//
void MulBaseElement::UpdateRotation()
	{
    MUL_LOG_ENTRY_EXIT("aakash::MulBaseElement::UpdateRotation");
	
	if (mData->mRotation.mIsApplied)
		{
		// rotation applied on the highlight item.
		if (mData->mRotation.mIndex < 0)
			{
			RotateImage(IMulWidget::ERotateLeft,TSize(0,0),0,true);
			}
		}	    
	}	

    } //namespace Alf
    
//End of file
