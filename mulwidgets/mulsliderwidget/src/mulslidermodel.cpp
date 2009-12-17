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
* Description:  Implementation of slider model interface
 *
*/

//Class headers
#include "mulslidermodel.h"

// Alf headers

#include <alf/alfmodel.h>
#include <alf/alfutil.h>
#include <mul/mulvarianttype.h>

// Osn Headers
#include <osn/osnnew.h>  //for new(EMM)
#include <osn/ustring.h>
#include <mul/mulvisualitem.h>
#include <mul/imulsliderwidget.h>
#include <string.h>

// For Common strings

#include "mulslidervertical.h"
#include "mulsliderwidget.h"

const int KMaxrange = 100;
const int KTick = 1;
const int KSize = 50; 
namespace Alf
{

struct SliderModelDataImpl
	{
	int mSldMaxRange;// Slider Maximum Value
	int mSldMinRange;// Slider Maximum Value
	int mSldTick;//  Size of single click movement
	int mSldPrimaryTick;// Current Handle Position
	int mSldSecondaryTick;// Secondary progress position
	char* mSldLeftText;// Left text
	char* mSldRightText;// Right text
	char* mSldPercentText;// Percentage text for zoom slider
	MulSliderWidget* mSliderWidget;
    IMulSliderBaseElementInternal * mSliderElement;// Element pointer 
    sliderTemplate mTemplateId; // template ID 
	std::auto_ptr<MulVisualItem> mItem; 
	SliderModelDataImpl()
    	{
    	mItem = ( new (EMM) MulVisualItem() );	
    	mSldMaxRange = KMaxrange;
    	mSldMinRange = 0;
    	mSldTick = KTick;
    	mSldPrimaryTick = 0;
    	mSldSecondaryTick = 0;
    	mSldLeftText = NULL;
    	mSldRightText =NULL;
    	mSldPercentText = NULL;
    	mSliderWidget = NULL;
    	mSliderElement = NULL;
    	mTemplateId = ESliderTemplateNone;
    	}
	};


// ---------------------------------------------------------------------------
// Constructor 
// ---------------------------------------------------------------------------
//
MulSliderModel::MulSliderModel():
mModel ( NULL )
	{
	mSldrModelData = new(EMM)SliderModelDataImpl();
	}

// ---------------------------------------------------------------------------
// Destructor 
// ---------------------------------------------------------------------------
//
MulSliderModel::~MulSliderModel()
	{
	if(mSldrModelData)	  
	    {
	    delete mSldrModelData;
	    }
	else
	    {
	    
	    }
	}		
	
// ---------------------------------------------------------------------------
// SetData 
// ---------------------------------------------------------------------------
// 
void MulSliderModel::SetData(const MulVisualItem& aSliderItem)
    {
    
    UString str;    
     
   
    if(aSliderItem.Attribute(mulvisualitem::KMulMaxRange))
        {
        SetMaxRange(
            aSliderItem.AttributeAsInt(mulvisualitem::KMulMaxRange));
        }
    if(aSliderItem.Attribute(mulvisualitem::KMulMinRange))
        {
        SetMinRange(
            aSliderItem.AttributeAsInt(mulvisualitem::KMulMinRange));
        }
    if(aSliderItem.Attribute(mulvisualitem::KMulTick))
           {
           SetTick(aSliderItem.
               AttributeAsInt(mulvisualitem::KMulTick));
           }    
    if(aSliderItem.Attribute(mulvisualitem::KMulPrimaryTick))
        {
        SetPrimaryValue(aSliderItem.
            AttributeAsInt(mulvisualitem::KMulPrimaryTick));
        }
    if(aSliderItem.Attribute(mulvisualitem::KMulSecondaryTick))
        {
        SetSecondaryValue(aSliderItem.
            AttributeAsInt(mulvisualitem::KMulSecondaryTick));
        }
            
    if(aSliderItem.Attribute(mulvisualitem::KMulPercent1))
            {
            
            int percentValue = aSliderItem.
                AttributeAsInt(mulvisualitem::KMulPercent1);
            
            _LIT8(KPercent,"");
         	TBuf8<KSize> PercentText(KPercent);
         	PercentText.AppendNum (percentValue);
         	PercentText.Append(_L("%"));
         	mSldrModelData->mSldPercentText = (char*)PercentText.PtrZ();
         	
            }
  
     UpdateElement();
    }     

   
// ---------------------------------------------------------------------------
// Return MulVisual Item  
// ---------------------------------------------------------------------------
//
const MulVisualItem& MulSliderModel::Data() const
	{
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulMaxRange,
	                (int)mSldrModelData->mSldMaxRange);
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulMinRange, 
	                (int)mSldrModelData->mSldMinRange);
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulTick, 
	                (int)mSldrModelData->mSldTick);
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulPrimaryTick, 
	                (int)mSldrModelData->mSldPrimaryTick);
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulSecondaryTick, 
	                (int)mSldrModelData->mSldSecondaryTick);	
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulCounter1,
	                (int)mSldrModelData->mSldLeftText );
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulCounter2, 
	                (int)mSldrModelData->mSldRightText );
	mSldrModelData->mItem->SetAttribute ( mulvisualitem::KMulPercent1, 
	                (int)mSldrModelData->mSldPercentText );
	return *(mSldrModelData->mItem);
   }

// ---------------------------------------------------------------------------
// SetValue 
// ---------------------------------------------------------------------------
//
void MulSliderModel::SetPrimaryValue(int aValue)
    { 
	int maxRange = mSldrModelData->mSldMaxRange;	
    int minRange = mSldrModelData->mSldMinRange;	
    // Validate aValue 
    if(aValue != mSldrModelData->mSldPrimaryTick)
        {
        if( (aValue >= minRange) && (aValue <= maxRange))
    		{
    		mSldrModelData->mSldPrimaryTick = aValue;
    		}
        else if(aValue <= minRange)
            {
    		mSldrModelData->mSldPrimaryTick = minRange;
    		}
    	else if(aValue >= maxRange)
    	    {
    	    mSldrModelData->mSldPrimaryTick = maxRange;
    	    }
    	// Update the visualization	
    	if(GetElement())
    	    {
            GetElement()->updateVisualization();	
    	    }        
	    }
    }
    
// ---------------------------------------------------------------------------
// Gets the Slider Element 
// ---------------------------------------------------------------------------
//
IMulSliderBaseElementInternal* MulSliderModel::GetElement() 
    {
    if(mSldrModelData->mSliderWidget)
        {
        return mSldrModelData->mSliderWidget->GetSliderElement();  
        }
    else
        {
        
        }
    return NULL;  
    }
    
    
 // ---------------------------------------------------------------------------
// update element visualization 
// ---------------------------------------------------------------------------
//
void MulSliderModel::UpdateElement() 
    {
    if(mSldrModelData->mSliderWidget)
        {
   		if(mSldrModelData->mSliderWidget->GetSliderElement())
	   		{
		    mSldrModelData->mSliderWidget->GetSliderElement()->updateVisualization();
	   		}
	    }
    }
       
// ---------------------------------------------------------------------------
// Value 
// ---------------------------------------------------------------------------
//
int MulSliderModel::PrimaryValue() const
    {
    return mSldrModelData->mSldPrimaryTick;
    }

// ---------------------------------------------------------------------------
// SetValue 
// ---------------------------------------------------------------------------
//
void MulSliderModel::SetSecondaryValue(int aValue)
    { 
  	int maxRange = mSldrModelData->mSldMaxRange;	
    int minRange = mSldrModelData->mSldMinRange;	
    if(mSldrModelData->mSldSecondaryTick != aValue)
        {
        // Validate aValue
        if( (aValue >= minRange) && (aValue <= maxRange))
    		{
    		mSldrModelData->mSldSecondaryTick = aValue;
    		}
    		
    	UpdateElement();
        }
    }

// ---------------------------------------------------------------------------
// Value 
// ---------------------------------------------------------------------------
//
int MulSliderModel::SecondaryValue() const
    {
    return mSldrModelData->mSldSecondaryTick;
    }
    

// ---------------------------------------------------------------------------
// TotalTicks 
// ---------------------------------------------------------------------------
//
int MulSliderModel::TotalTicks() const
    {
    //MUL_LOG_INFO("CMulSliderModel::TotalTicks");
    
    int maxRange = mSldrModelData->mSldMaxRange;	
    int minRange = mSldrModelData->mSldMinRange;	
    int tick     = mSldrModelData->mSldTick;
    if(tick == 0)
        {
        tick = 1;
        }
    return ((maxRange-minRange)/tick);
    }

// ---------------------------------------------------------------------------
// SetTemplate 
// ---------------------------------------------------------------------------
//
void MulSliderModel::SetTemplate( sliderTemplate aTemplateId)
    {
    if(mSldrModelData->mTemplateId != aTemplateId)
        {
    	mSldrModelData->mTemplateId = aTemplateId; 
    	if(mSldrModelData->mSliderWidget)
            {
            // Change the tempate 
            mSldrModelData->mSliderWidget->changeTemplate(aTemplateId);
            }
        else 
            {
            //do nothing
            }	
        }
    }

// ---------------------------------------------------------------------------
// makeInterface
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulSliderModel::makeInterface( const IfId& aType)
    {
    //MUL_LOG_INFO("CMulSliderModel::makeInterface");
    IAlfInterfaceBase* ret(0);
      UString param(aType.mImplementationId);
      if ( param == IMulSliderModel::type().mImplementationId )
          {
          ret = static_cast<IMulSliderModel*>(this);
          }
      else if(param == IAlfModel::type().mImplementationId)
          {
          ret = static_cast<IAlfModel*>(this);
          }
      return ret;      
          
    }

// ---------------------------------------------------------------------------
// SetMinRange 
// ---------------------------------------------------------------------------
//

void MulSliderModel::SetMinRange(int aMinRange)
	{
	// Validate aMinRange
	if(aMinRange != mSldrModelData->mSldMinRange)
	    {
	    mSldrModelData->mSldMinRange = aMinRange;
	    
	   if(aMinRange > mSldrModelData->mSldMaxRange)
	       {
	       mSldrModelData->mSldMaxRange = aMinRange;
	       mSldrModelData->mSldTick = 0;
	       }
	   else
	       {
	       if(mSldrModelData->mSldTick > mSldrModelData->mSldMaxRange - mSldrModelData->mSldMinRange || mSldrModelData->mSldTick == 0)
              {
              mSldrModelData->mSldTick = mSldrModelData->mSldMaxRange - mSldrModelData->mSldMinRange; 
              }
	       }
    	
	       mSldrModelData->mSldPrimaryTick = aMinRange;
        UpdateElement();	  
        }

	}

// ---------------------------------------------------------------------------
// SetMaxRange 
// ---------------------------------------------------------------------------
//

void MulSliderModel::SetMaxRange(int aMaxRange)
	{
	if(aMaxRange != mSldrModelData->mSldMaxRange)
	    {
	    mSldrModelData->mSldMaxRange = aMaxRange;
	   
	    if(aMaxRange < mSldrModelData->mSldMinRange)
	        {
	        mSldrModelData->mSldMinRange = aMaxRange;
	        mSldrModelData->mSldTick = 0;
	        }
	    else
	        {
	        if(mSldrModelData->mSldTick > mSldrModelData->mSldMaxRange - mSldrModelData->mSldMinRange || mSldrModelData->mSldTick == 0 )
                {
                mSldrModelData->mSldTick = mSldrModelData->mSldMaxRange - mSldrModelData->mSldMinRange; 
                }
	        }
    	
	    mSldrModelData->mSldPrimaryTick = mSldrModelData->mSldMinRange;
        UpdateElement();	    
        }
    }

// ---------------------------------------------------------------------------
// MinRange 
// ---------------------------------------------------------------------------
//

int MulSliderModel::MinRange() const
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");		
	return mSldrModelData->mSldMinRange;	
	}
		
// ---------------------------------------------------------------------------
// MaxRange 
// ---------------------------------------------------------------------------
//

int MulSliderModel::MaxRange() const
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");		
	return mSldrModelData->mSldMaxRange;	
	}
// ---------------------------------------------------------------------------
// SetTick 
// ---------------------------------------------------------------------------
//

void MulSliderModel::SetTick(int aTick)
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");
	if(aTick != mSldrModelData->mSldTick && aTick > 0) 
	    {
        if(aTick > mSldrModelData->mSldMaxRange - mSldrModelData->mSldMinRange)
    	    {
    	    mSldrModelData->mSldTick = mSldrModelData->mSldMaxRange - mSldrModelData->mSldMinRange;	
    	    }
    	else
    	    {
    	    mSldrModelData->mSldTick = aTick;
    	    }
        mSldrModelData->mSldPrimaryTick = mSldrModelData->mSldMinRange;
    	if(GetElement())
    	    {
    	    GetElement()->updateVisualization();
    	    }	    
	    }
	}

// ---------------------------------------------------------------------------
// Tick 
// ---------------------------------------------------------------------------
//

int MulSliderModel::Tick() const
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");		
	return mSldrModelData->mSldTick;	
	}


// ---------------------------------------------------------------------------
// SetRightText 
// ---------------------------------------------------------------------------
//

void MulSliderModel::SetRightText(char* aRightText)
	{
	if(aRightText)
	    {
	    mSldrModelData->mSldRightText = aRightText;
	    UpdateElement();
	    }
	    	
	}

// ---------------------------------------------------------------------------
// SetLeftText 
// ---------------------------------------------------------------------------
//

void MulSliderModel::SetLeftText(char* aLeftText)
	{
    if(aLeftText)
	    {
	    mSldrModelData->mSldLeftText = aLeftText;
	    UpdateElement();
	    }
	}

// ---------------------------------------------------------------------------
// SetPercentText 
// ---------------------------------------------------------------------------
//

void MulSliderModel::SetPercentText(char* aPercText)
	{
	if(aPercText)
	    {
	    mSldrModelData->mSldPercentText = aPercText;
	    if(GetElement())
	    {
	    GetElement()->updateTextVisualization();	
	    }
	    }
	
	}

// ---------------------------------------------------------------------------
// GetRightText 
// ---------------------------------------------------------------------------
//

const char* MulSliderModel::GetRightText()
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");		
	return mSldrModelData->mSldRightText;	
	}

// ---------------------------------------------------------------------------
// GetLeftText 
// ---------------------------------------------------------------------------
//

const char* MulSliderModel::GetLeftText()
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");		
	return mSldrModelData->mSldLeftText;	
	}

// ---------------------------------------------------------------------------
// GetPercentText 
// ---------------------------------------------------------------------------
//

const char* MulSliderModel::GetPercentText()
	{
	//MUL_LOG_INFO("CMulSliderModel::makeInterface");		
	return mSldrModelData->mSldPercentText;	
	}


// ---------------------------------------------------------------------------
// addModelChangeObserver 
// ---------------------------------------------------------------------------
//
void MulSliderModel::addModelChangeObserver( IAlfModelChangeObserver& /*aObserver*/ )
    {    
    //MUL_LOG_INFO("CMulSliderModel::addModelChangeObserver");
    }

// ---------------------------------------------------------------------------
// removeModelChangeObserver 
// ---------------------------------------------------------------------------
//
void MulSliderModel::removeModelChangeObserver( IAlfModelChangeObserver& /*aObserver*/ )
    {
    //MUL_LOG_INFO("CMulSliderModel::removeModelChangeObserver");
    }

// ---------------------------------------------------------------------------
// setData 
// ---------------------------------------------------------------------------
//
void MulSliderModel::setData( IAlfVariantType* /*aData*/ )
    {
    //MUL_LOG_INFO("CMulSliderModel::setData");
    }

// ---------------------------------------------------------------------------
// updateData 
// ---------------------------------------------------------------------------
//
void MulSliderModel::updateData(int /*aNumContainerIndices*/,int* /*aContainerIndices*/,
                                IAlfVariantType* /*aData*/ )
    {
    //MUL_LOG_INFO("CMulSliderModel::updateData");
    }

// ---------------------------------------------------------------------------
// addData 
// ---------------------------------------------------------------------------
//
void MulSliderModel::addData(int /*aNumContainerIndices*/,int* /*aContainerIndices*/, 
                             IAlfVariantType* /*aData*/ )
    {
    //MUL_LOG_INFO("CMulSliderModel::addData");
    }

// ---------------------------------------------------------------------------
// removeData 
// ---------------------------------------------------------------------------
//
void MulSliderModel::removeData(int /*aNumContainerIndices*/,int* /*aContainerIndices*/ )
    {
    //MUL_LOG_INFO("CMulSliderModel::removeData");
    }

// ---------------------------------------------------------------------------
// executeOperations 
// ---------------------------------------------------------------------------
//
void MulSliderModel::executeOperations(AlfPtrVector<AlfModelOperation>&  /*aOperationsArray*/)
    {
    //MUL_LOG_INFO("CMulSliderModel::executeOperations");
    

    }

// ---------------------------------------------------------------------------
// executeOperation 
// ---------------------------------------------------------------------------
//
void MulSliderModel::executeOperation(AlfModelOperation* /*aOperation*/)
    {
    //MUL_LOG_INFO("CMulSliderModel::executeOperation");
    }

// ---------------------------------------------------------------------------
// clearModel 
// ---------------------------------------------------------------------------
//
void MulSliderModel::clearModel()
    {
    //MUL_LOG_INFO("CMulSliderModel::clearModel");
    }

// ---------------------------------------------------------------------------
// data 
// ---------------------------------------------------------------------------
//
IAlfVariantType* MulSliderModel::data() const 
    {
    //MUL_LOG_INFO("CMulSliderModel::data");
    return NULL;
    }    
	
// ---------------------------------------------------------------------------
// StoreWidget
// ---------------------------------------------------------------------------
//    
void MulSliderModel::storeWidget(MulSliderWidget* aWidget)
    {
    mSldrModelData->mSliderWidget = aWidget;
    }
// ---------------------------------------------------------------------------
// GetTemplate 
// ---------------------------------------------------------------------------
//
sliderTemplate MulSliderModel::GetTemplate()
    {
    return mSldrModelData->mTemplateId ;
    }
  
// ---------------------------------------------------------------------------
// IsLandscape
// 
// ---------------------------------------------------------------------------
//  
 bool MulSliderModel::IsLandscape()   
    {
	
	bool landscape = false;
	if(mSldrModelData->mSliderWidget)
    	{
    	TSize size = AlfUtil::ScreenSize();        	
    	if(size.iHeight < size.iWidth)
        	{
            landscape = true;
        	}
    	else if(size.iWidth < size.iHeight)
        	{
        	landscape = false;
        	}
    	}
    return landscape;
    }
 
	} // Namespace Alf

//End of file
