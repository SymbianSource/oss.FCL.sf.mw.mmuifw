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
* Description:  template header for CoverFlow
*
*/

// This file defines the API for MulCoverFlowWidget.dll

#ifndef __MULCOVERFLOWTEMPLATE_H__
#define __MULCOVERFLOWTEMPLATE_H__

// Avkon Includes
#include <aknlayoutscalable_uiaccel.cdl.h>

#include "mulbaseelement.h" // for TMulCoverflowItem

namespace osncore
	{
// Forward declarations
class UString;
	}

namespace Alf
    {
// Forward declarations
class MulCoverFlowControl;

struct TMulCoverFlowTemplateImpl
    {
    
    TMulCoverFlowTemplateImpl()
        {
		mEnhancedSupportFlag = false;
		mSliderEnabled       = false;
		mUiOnOffFlag         = false;
		mOrientationChange   = false;
		mControl             = NULL; 
		mBaseElement         = NULL;
		mMaxVisibleCount     = 0;
		mTitleFontMaxCharLength = 0;
		mDetailFontMaxCharLength = 0;
		mCounterFontMaxCharLength = 0;
        }

    //Structure used to store the position and sizes of the counter.
    TMulCoverFlowItem mCounterDimensions;
    TMulCoverFlowItem mSliderDimensions;    
    TMulCoverFlowItem mIndicatorDimensions; 
    bool mEnhancedSupportFlag;
    bool mSliderEnabled;
    bool mUiOnOffFlag ;
    int mMaxVisibleCount;
    MulBaseElement* mBaseElement;
    bool mOrientationChange;
    MulCoverFlowControl* mControl;          // doesn't own it .
    mulwidget::TLogicalTemplate mTemplateID; // to store the template name
    TSize mTitleParentSize;
    TSize mDetailParentSize;
    int mTitleFontMaxCharLength;
    int mDetailFontMaxCharLength;
	int mCounterFontMaxCharLength;    

    ~TMulCoverFlowTemplateImpl()
      {
      }    
    };


    
class MulCoverFlowTemplate 
    {
    
public: // Constructor and Destructor 
        
    /**
     * Constructor
     */
    MulCoverFlowTemplate(MulCoverFlowControl &aControl, MulBaseElement &aBaseElement);
    
    /**
     * Destructor
     */  
    virtual ~MulCoverFlowTemplate();

public: // New Methods
    
    /**
     * Returns the counter element structure. 
     * 
     * @return Returns a refrence to the counter element structure.
     */
    const TMulCoverFlowItem& CounterDimensions() const
    	{return mData->mCounterDimensions;}
    
    /*
     * Returns whether the current template and orientation supports enhanced mode or not.
     *
     * @Returns true/false depending on whether the template supports or not.
     */
    bool EnhancedTagParsed() const {return mData->mEnhancedSupportFlag;}
    
    /*
     * Returns whether the slider is enable or not for the current template and
     * orientation.
     *
     * @Returns true/false depending on whether enabled or not.
     */
    bool IsSliderVisible() const {return mData->mSliderEnabled;}
      
    /*
     * Returns  the maxvisible items to be in the respective template .
     *
     * @Returns int depending on the no of visble item count.
     */
    int MaxVisibleCount() const {return mData->mMaxVisibleCount;}
    
    /**
     * Returns whether the template is UI on/off or not .
     * That is UI on/off flag is enabled in the template or not . 
     * 
     * @returns true if flag is enabled else false .
     */
    bool IsUiOnOffFlagEnabled() const {return mData->mUiOnOffFlag;}
    
    /**
     * Returns the slider dimensions . 
     *
     * @Returns TMulCoverFlowItem dimension of the slider
     */
    const TMulCoverFlowItem& SliderDimension() {return mData->mSliderDimensions;}
    
    /**
     * Returns the video indicator dimension. 
     * 
     * @return Returns a refrence to the indicator dimension.
     */
     const TMulCoverFlowItem& IndicatorDimension() const {return mData->mIndicatorDimensions;}
    
    /**
     *  Returnt the size of the title window size.
     *
     */
    const TSize& TitleWindowSize() {return mData->mTitleParentSize;}
    
    /**
     *  Return the size of the detail window size.
     *
     */
    const TSize& DetailWindowSize() {return mData->mDetailParentSize; }
    
    /**
     * Return the title maximum character font Width
     *
     */
    int TitleMaxCharWidth() {return mData->mTitleFontMaxCharLength;}
    
    /**
     * Return the detail maximum character font Width
     */
    int DetailMaxCharWidth() {return mData->mDetailFontMaxCharLength;}
    
    /**
     * Return the counter maximum character font Width
     */
    int CounterMaxCharWidth() {return mData->mCounterFontMaxCharLength;}
    
    
    /**
     * Virtual function must be overriden by derived class.
     * Creates the visualisation based on the Orientation
     * also updates the visualisation on orientation change
     *
     * @param aIsLandscape, current orientation
     * @param aIsOrientationChange, based on flag decide to
     *			create or update the visualisation
     */
	virtual void CreateVisualisation(bool aIsLandscape,bool aIsOrientationChange = false);


protected:
	
    /**
     * Get the window rect of the Imagevisual or layouts .
     * @Param aLayout layout which gives all the LCT data of
     *                the visual(image or layout). 
     * @Param aParentRect rect of the parent to the visual.
     */
    TRect IconWindowRect(const TAknWindowLineLayout &aLayout, const TRect& aParentRect);
	
    /**
     * Get the window rect of the textvisual .
     * @Param aLayout layout which gives all the LCT data of
     *                 the textvisual. 
     * @Param aParentRect rect of the parent to the visual 
     */
    TRect TextWindowRect(const TAknTextLineLayout& aLayout, const TRect& aParentRect);
    
    /**
     * Set the style to the textvisual
     * 
     * @param aTextFontId  the fontId to be set on textvisual.
     * @param textVisual refernce to textvisual.
     */
    void  SetTextStyle(int aTextFontId, CAlfTextVisual& textVisual);
    
    /**
     *  Update the maximum character width of text font of textvisual 
     *  @Param textVisual , alftextvisual
     */
    int UpdateMaxFontWidth(CAlfTextVisual* textVisual);
    
    /**
     * Applies brush on the specified visual .
     * The colour of the brush is aBrushColour.
     * @param aVisual visual on which brush has to be applied.
     * @param aBrushColour The brush colour.
     * @param aBrushOpacity The opacity to be applied on the brush.
     */
    void ApplyBrush( CAlfTextVisual& aVisual,const TRgb& aBrushColour,
    		 float aBrushOpacity );
     
private:
         
    /**
     * Returns a font id for the specified style.
     * 
     * @Param aFontStyle font style.
     * @Param aManager textstyle manager.
     */		
	TInt GetFontId(const UString &aFontStyle, CAlfTextStyleManager& aManager)const;
    		
        
protected:
    auto_ptr<TMulCoverFlowTemplateImpl> mData;  
    
    };

    
// class Template 4
class MulCoverFlowTemplate4 : public MulCoverFlowTemplate
    {
public:
	
    /**
     * Constructor
     */
    MulCoverFlowTemplate4(MulCoverFlowControl &aControl, MulBaseElement &aBaseElement);
    
    /**
     * destructor
     */
    ~MulCoverFlowTemplate4(); 
    
// From MulCoverFlowTemplate       

    /**
     * @See CreateVisualisation of MulCoverFlowTemplate
     */
	void CreateVisualisation(bool aIsLandscape,bool aIsOrientationChange=false);
	
// new method	
	
    /**
     * provides the dimension of the visual attribute (icon & video indicator)
     * based on the orientation.
     *
     * @param aAttribute, type of visual attribue 
     * @param aIslandscape, current orientation
     */	
	static void GetIconSize(mulvisualitem::TVisualAttribute aAttribute,bool aIslandscape,
		int& aHeight, int& aWidth);
		
private:

    /**
     * create visualisation for potrait orientation
     *
     */	
	void CreatePrtStructure();
	
    /**
     * create visualisation for landscape orientation
     *
     */		
	void CreateLscStructure();
	
    /**
     * create counter visual and apply attribute
     *
     * @param acounterPaneVerityId, lct verity of counter visual.
     * @param aMainRect, parent rect for counter visual.
     */		
	void ApplyCounterAttribute(int acounterPaneVerityId,const TRect& aMainRect);
	
    };
    
// class Template 1
class MulCoverFlowTemplate1 : public MulCoverFlowTemplate
    {
public:
	
    /**
     * Constructor
     */
    MulCoverFlowTemplate1(MulCoverFlowControl &aControl, MulBaseElement &aBaseElement);
    
    /**
     * destructor
     */
    ~MulCoverFlowTemplate1();
    
// From MulCoverFlowTemplate           
	
    /**
     * @See CreateVisualisation of MulCoverFlowTemplate
     */		
	void CreateVisualisation(bool aIsLandscape,bool aIsOrientationChange=false);

// new method	
	
    /**
     * provides the dimension of the visual attribute (icon & video indicator)
     * based on the orientation.
     *
     * @param aAttribute, type of visual attribue 
     * @param aIslandscape, current orientation
     */	
	static void GetIconSize(mulvisualitem::TVisualAttribute aAttribute,bool aIslandscape,
		int& aHeight, int& aWidth);
	
private:

    /**
     * create visualisation for potrait orientation
     */	
	void CreatePrtStructure();
	
    /**
     * create visualisation for landscape orientation
     */		
	void CreateLscStructure();	
	
    /**
     * create counter visual and apply attribute
     *
     * @param acounterPaneVerityId, lct verity of counter visual.
     * @param aMainRect, parent rect for counter visual.
     */		
	void ApplyCounterAttribute(int acounterPaneVerityId,const TRect& aMainRect);
	
    /**
     * apply attribute on main text layout
     *
     * @param aMainRect, parent rect for main text layout
     */		
	void ApplyTextAttribute(const TRect& aMainRect);
	
    /**
     * create title text visual and apply attribute
     *
     * @param aVerityId, lct verity of title text.
     * @param aTextLayoutRect, parent rect for title text
     */		
	void ApplyTitleTextAttribute(int aVerityId,const TRect& aTextLayoutRect);
	
    /**
     * create detail text visual and apply attribute
     *
     * @param aVerityId, lct verity of detail text.
     * @param aTextLayoutRect, parent rect for detail text
     */		
	void ApplyDetailTextAttribute(int aVerityId,const TRect& aTextLayoutRect);
	
    /**
     * parse and apply the slider attribute
     *
     * @param aMainRect, parent rect for slider
     */		
	void ApplySliderAttribute(const TRect& aMainRect);
    };

    
    }//namespace Alf
#endif //__MULCOVERFLOWTEMPLATE_H

//End of file
