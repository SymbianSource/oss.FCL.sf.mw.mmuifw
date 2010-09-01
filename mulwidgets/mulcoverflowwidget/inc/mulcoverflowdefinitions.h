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
* Description:  header for defining all the common variables
*
*/

// This file defines the variables for MulCoverFlowWidget.dll

#ifndef __MULCOVERFLOWDEFINITIONS_H__
#define __MULCOVERFLOWDEFINITIONS_H__

/// @bug minor:avanhata:7/7/2008 The constants need to be documented. Overall,
/// if you are only using a constant in a single cpp file, then define it there.

namespace Alf
    {
    /// @bug minor:avanhata:7/7/2008: unclear variable name - is this duration 
    /// is milliseconds?
    /// @bug minor:avanhata:7/7/2008: KAnimation200 add absolutely 
    /// no abstraction over just the number 200.
    /// Define these as higher-level abstractions (
    /// e.g., KEntryAnimationDuration, KBounceAnimationDuration, etc.)
    //static const int KMaxVisibleItems       = 3;
    static const int KAnimation200         = 200;

    // Minimum differnece in Speed percentage after which the strip size 
    // and speed will be modified
	static const int KMainLayoutIndex = 0;  
	static const int KHorizontalLeftAlign = 0;
	static const int KHorizontalRightAlign = 2;
	static const int KMinNumItemForFastScroll = 4;
	static const int KNumberOfRepeatsForMediumSpeed = 10;
	static const int KNumberOfRepeatsForFastSpeed = 20;

	static const int KZeroAnimation        = 0;
  /// @bug minor:avanhata:7/7/2008 remove hard coded constant
  /// - use sizeof( TUint16 ) instead
	static const int KSlashSize            = 2;

	static const float KHighlightOpacity   = 1.0f;
	static const float KBackGroundOpacity  = 0.0f;

	static const int KIconFlowLayoutIndex  = 0;
	static const int KTextLayoutIndex  = 1;
	static const int KTextLayoutViewPortIndex  = 2;

	static const int KTime = 500;
	static const int KBounceTime = 150;
	static const int KMarqueeTime25 = 40; // marquee animation time
	static const int KMarqueeTime1000 = 1000; // after marquee end wait time 
	static const int KBounceRight= -1;
	static const int KBounceLeft= 1;
	static const int KVisibleWindow        = 3;
	static const int KContainerIndex = 1;
	static const int KGroupId        = 21;
	static const int KInitialvalueZero        = 0;
	static const int KInitialvalueOne        = 1;
	/// @bug minor:avanhata:7/7/2008 use a meaningful variable name
	static const int KTextRowsz = 6;
	/// @bug minor:avanhata:7/7/2008 Capitalisation
	static const int KTextFontindex = 9;
	/// @bug minor:avanhata:7/7/2008 move to the cpp file in which this is used
	static const int KTempBufferSize = 50;
	static const char* const KNameSliderWidget = "mulsliderwidget";
	static const char* const KNameSliderModel = "mulslidermodel";
	static const char* const KMulTemplate = "mul_template";
	
    static const char* const KBase = "base";
    static const char* const KItem = "item";
    static const char* const KText = "text";
    static const char* const KLandscape = "landscape";
    static const char* const KPortrait = "portrait";
    _LIT8( mainlayout,"mainlayout" );
    _LIT8( backgroundvisual,"backgroundvisual" );
    _LIT8( verticalflowlayout,"verticalflowlayout" );
    _LIT8( iconflowlayout,"iconflowlayout" );
    _LIT8( textflowlayout, "textflowlayout");
    _LIT8( counterflowlayout,"counterflowlayout" );
    _LIT8( decklayout,"deck" );
    _LIT8( KEmptyText,"emptytext" );
    _LIT8( KMulEcfShadowTop,"mul_ecf_shadow_top");
    _LIT8( KMulEcfShadowBottom,"mul_ecf_shadow_bottom");
    _LIT( KSlash,"/" );
    _LIT(KOutOfBound,"Out of Bound Index");
	_LIT(KLogicError,"Logic error");
	
	_LIT8( KDetailViewPortLayout,"detailviewportlayout");
	_LIT8( KTitleViewPortLayout,"titleviewportlayout");
    _LIT8( text,"text" );
    _LIT8( KLitMulBackGround , "mul_background" );
    _LIT8( KTextAnchor , "anchor" );
	_LIT(KTemplateNotFound, "Template Not Found");
	_LIT(KOrientationNotFound, "Orientation Node not found");
	_LIT(KVisualNotFound, "Visual Not Found");
	_LIT(KIteratorNotCreated, "Iterator could not be created");
	_LIT(KNullPointer,"Null Pointer");
	_LIT(KInvalidArgument,"Invalid Argument");	
    _LIT(KInvalidTemplate,"Invalid Template");
    _LIT(KInvalidAttribute,"Invalid Attribute");
    
	_LIT8( KCoverflowTitle,"mul_title");
	_LIT8( KCoverflowDetail,"mul_detail");    
	_LIT8( KCoverflowIcon,"mul_icon");
	_LIT8( KCoverflowIndicator,"mul_indicator");
	_LIT8( KCoverflowCounter,"mul_count");
	_LIT8(KInvalidID,"");
	
	static const char* const KOrientation = "orientation";
	static const char* const KCategory =  "category";
	static const char* const KElement = "element";
	static const char* const KHighlight = "highlight";
	static const char* const KLayout = "layout";
	static const char* const KFlow = "flow";
	static const char* const KBackground = "background";
	static const char* const KAnchor = "anchor";
	static const char* const KDeck = "deck";
	static const char* const KImageVisual = "imagevisual";
	static const char* const KTextVisual = "textvisual";
	static const char* const KBaseTemplate = "templatebase";
	static const char* const KCounter = "counter";
	static const char* const KNonHighlight = "nohighlight";
	static const char* const KXml = "xml";
	static const char* const KSize = "size";
	static const char* const KUiOnOff = "uiOnOff";
	static const char* const KEnhancedMode = "enhancedmode";
	static const char* const KSlider = "slider";
	static const char* const KSliderWidget = "widget";
	static const char* const KEnhancedSize = "enhancedsize";
	static const char* const KMinSize = "min_size";
	static const char* const KMaxSize = "max_size";
	static const char* const KCounterSize = "counter_size";
	static const char* const KMulBackGround = "mul_background";
	static const char* const KMulTitle = "mul_title";
	static const char* const KMulDetail = "mul_detail";
	static const char* const KOpacity = "opacity";
	static const char* const KColour = "colour";
	static const char* const KHeight = "height";
	static const char* const KWidth = "width";
	static const char* const KPosX = "positionx";
	static const char* const KPosY = "positiony";
	static const char* const KTemplateId = "template";
	static const char*  const KLogicalTemplateDrive  = "\\resource\\coverflow\\";
	static const char* const KTemplatePrefix = "logical_template"; //for slider
	static const char*  const KUnderscore = "_";
	static const char*  const KExtension = ".xml";
	static const char*  const KCentreItem = "item_center";
	static const char*  const KRightItem = "item_right";
	static const char* const KId = "id";
    static const char* const KMulCount = "mul_count";
    static const char* const KCounterBgColor = "backgroundcolor";
    static const char* const KCounterBgOpacity = "backgroundopacity";
    static const char* const KFitMode = "fitmode";
    static const char* const KItemCentre = "item_center";
    static const char* const KItemRight = "item_right";
    static const char* const KMainLayout = "mainlayout";
    static const char* const KMarquee = "marquee";
    static const char* const KMarqueeOn = "yes";
    static const char* const KRedColorAttrib = "red";
    static const char* const KGreenColorAttrib = "green";
    static const char* const KBlueColorAttrib = "blue";
    static const char* const KAlphaColorAttrib = "alpha";
    static const char* const KOn = "on";
    static const char* const KOff = "off";
    static const char* const KTextStyle = "style";
    static const char* const KHorzAlign = "horizontalalign";
    static const char* const KVertAlign = "verticalalign";
    static const char* const KColor = "color";
    static const char* const KFontThemeColor = "fontthemecolor";
    static const char* const KTextSkinColorId = "textskincolorid";
    static const char* const KTextColorSkinIndex = "textcolorskinindex";
    static const char* const KTempMaxCount = "templatemaxcount";
    static const char* const KFastScroll = "fastscroll";
	static const char* const KIndicator = "mul_indicator";
	
	enum TMulCoverFlowEventType
		{
		ECustomEventBounceBack = 300,/*!< A message regarding coverflow bounce */
		ECustomEventScroll,
		ECustomEventBounceCompleted
		};
        
    enum TItemScroll
			{
			EItemNoDirection,
			EItemScrollLeft,
			EItemScrollRight
			};  
    
    enum TTextVisualType
			{
			ECounterVisual = 1,
			ETilteDetailTextVisual    
			};  
			
    enum TNumberofSwipes
			{
			ESingleSwipe,
			EDoubleSwipe,
			ETripleSwipe
			};			    

struct TMulVisualColorProperty
    { 
    TRgb mColor;
    bool mIsColorSet;
    TMulVisualColorProperty():mIsColorSet(false){}
    }; //end of the structure
    
struct TMulRotationProperty
    { 
    int mIndex;
    bool mIsApplied;
	int mImageAngle;
	TSize mTextureSize;
    
    TMulRotationProperty():mIsApplied(false),mIndex(-1),mImageAngle(0)
    	{
    	}
    }; //end of the structure
	
    } //namespace Alf
    
#endif
