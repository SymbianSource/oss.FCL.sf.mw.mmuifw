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
* Description:  The base class for all Multimedia widgets.
 *
*/


#ifndef I_MULWIDGET_H
#define I_MULWIDGET_H

#include <alf/alftypes.h>
#include <alf/ialfwidgeteventhandler.h>
#include <alf/ialfinterfacebase.h>
#include <alf/ialfwidget.h> 
#include <alf/alflayout.h> 
#include <mul/mulvisualitem.h>
#include <e32base.h>
#include <gdi.h>
#include <osn/ustring.h>

using namespace osncore;

// Include widget observer 

namespace Alf
	{
	class IMulModelAccessor;

	namespace mulwidget
		{
		static const IfId Ident=
			{
					0, "mulwidget"
			};
			
		enum TLogicalTemplate
			{
			KTemplateUnknown = 0,
			KTemplate1 ,
			KTemplate2,
			KTemplate3,
			KTemplate4,
			KTemplate5,
			KTemplate6,
			KTemplate7,
			KTemplate8,
			KTemplate9,
			KTemplate10,
			KTemplate11,
			KTemplate12,
			KTemplate13,
			KTemplate14
			};
		/*
		static const char* const KTemplate1 = "logical_template_1";
		static const char* const KTemplate2 = "logical_template_2";
		static const char* const KTemplate3 = "logical_template_3";
		static const char* const KTemplate4 = "logical_template_4";
		static const char* const KTemplate5 = "logical_template_5";
		static const char* const KTemplate6 = "logical_template_6";
		static const char* const KTemplate7 = "logical_template_7";
		static const char* const KTemplate8 = "logical_template_8";
		static const char* const KTemplate9 = "logical_template_9";
		static const char* const KTemplate10 = "logical_template_10";
		static const char* const KTemplate11 = "logical_template_11";
		static const char* const KTemplate12 = "logical_template_12";
		static const char* const KTemplate13 = "logical_template_13";
		static const char* const KTemplate14 = "logical_template_14";
		*/
		}

	/**
	 *  An interface for all Multimedia widgets.
	 *  Widgets consist of control and visualization elements.
	 *  Model owned by the application.
	 * 	
	 *	Interface paradigm would be used in a restrictive way.
	 *  Controls are owned by Alfred environment.
	 *  Widgets can be constructed using the widget factory.
	 *  Widgets are owned and accessed using the Alfred
	 *  environment.
	 */
	class IMulWidget : public IAlfWidget // base class 
		{
	public:
	
		/*! @enum TMulVisualEffect
		 * This enumeration defines Visual effect that widget support
		 * 
		 */
		enum TMulVisualEffect
		    {
			EHighlightTransition = 1    /*!< Highlight transition effect.*/
	    	};

    	enum TMulVisualType
			{
			ETextVisual = 1,    /*!< Color of the text in the widgets.*/
			ECoverflowCounterVisual,     /*!< Color of the Counter text in the Coverflow Widget.*/
			EItemBackgroundBrush,         /*!< Color of the Item brush in the Widget.*/  
			EThumbnailBackgroundBrush		/*!< Color for the background of the thumbnail image. Currently used in list widget only.*/
			};
			
		/*! @enum TMulRotation
		 * This enumeration defines Rotation effect that widget supports
		 * 
		 */		
		enum TMulRotation
			{
			ERotateLeft = 1, /*!< For left rotation*/
			ERotateRight	/*!< For right rotation*/
			};	
    	
		/**
		 * Getter for the type identifier of this interface.
		 *
		 * @return A descriptor to identify the type of this interface.
		 **/
		static inline const IfId& Type()
			{
			return mulwidget::Ident;
			}

		/**
		 * Set the data model for this widget.  Widget uses the data model via 
		 * IMulModelAccessor interface. Does not take ownership of the accessor
		 * and there can be only one accessor to the widget at any point. This 
		 * will remove the binding to previous accessor, if any. Before 
		 * deleting the accessor application should remove the binding to 
		 * widget by calling the api by passing NULL.
		 *
		 * @param aAccessor  model to be set for widget
		 */
		virtual void SetModel(IMulModelAccessor* aAccessor , bool aFlag = true) = 0;

		/**
		 * Set the size and position of the widget.All parameter values are in pixels and are relevant 
		 * to the parent of the widget.
		 *
		 * @param aLeftMargin  margin for the left border (top left x co-ordinate value)
		 * @param aTopmargin   margin for the top border (top left y co-ordinate value)
		 * @param aHeight      height of widget
		 * @param aWidth       width of widget
		 * @param aTransitionTime       Transition Time in MilliSeconds
		 */
		virtual void SetRect(int aLeftMargin, int aTopmargin, int aHeight,
				int aWidth, int aTransitionTime =0) = 0;

		/**
		 * To hide and unhide Widget.
		 *
		 * @param aShow                 value to hide/unhide the Widget 
		 * @param aTransitionTime       Transition Time in MilliSeconds
		 */
		virtual void ShowWidget(bool aShow, int aTransitionTime =0) = 0;
		
		/**
		 * Get the icon size from the template
		 *
		 * @param atemplateId           Logical Template Id 
		 * @param aAttribute            Attribute name of the Icon
		 * @param aHeight               Height of the Icon
		 * @param aWidth                Width of the Icon
		 */
		virtual void GetIconSize( mulwidget::TLogicalTemplate aTemplateId, 
		    mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth) = 0;

		/**
		 * Get the icon size from the template
		 *
		 * @param aAttribute            Attribute name of the Icon
		 * @param aHeight               Height of the Icon
		 * @param aWidth                Width of the Icon
		 */
		virtual void GetIconSize( 
		    mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth) = 0;
		    	
		    	
		    /**
		 * Add an observer to widget inorder to get event notifications
		 * from the widget. Widget can have multiple observersers.
		 *
		 * @param aObserver Observer of widget.
		 **/
		virtual void AddEventHandler( IAlfWidgetEventHandler& aObserver) = 0;

		/**
		 * Remove an observer to widget
		 *
		 * @param aObserver Observer of widget.
		 */
		virtual void RemoveEventHandler( IAlfWidgetEventHandler& aObserver) = 0;  
		
		/**
		* Get the Container Layout for widget.
		*
		* @return Container layout of the widget. This can be used to set the position of widget.
		*/
		virtual const CAlfLayout& ContainerLayout() = 0;
		
		
		/**
		 * Set Transaction/Animation time for variaous visual effect.
		 *
		 * @param aVisualEffect         Visual effect to which animation time/transation time to set
		 * @param aTransitionTime       Transition Time in MilliSeconds
		 *
		 * <B>Examples:</b>
		 * @code
		 * Set the animation time.
		 *
		 *      mListWidget->SetAnimationTime( TMulVisualEffect::EHighlightTransition, 1000 );
		 * @endcode
		 */
		virtual void SetAnimationTime( TMulVisualEffect aVisualEffect, int aTransitionTime = 0 ) = 0;
		 
		/**
		 * Set the color and opacity of the visual for various visuals as per the widget.
		 *  Also used to set the background transparent.
		 *
		 * @param aVisualType visual to which color is to be applied.
		 * @param aColor RGB color
		 *
		 * NOTE: TRgb with the alpha is mandatory .,if application doesnt sets the alpha results in the 0-opacity-Transparent.
		 *        so applications always should use TRgb with alpha constructor: like- TRgb(TUint32 color,TInt alpha);
		 * 
		 * <B>Examples: To Set the color and opacity of the thumbnail background </b>
		 * @code
		 * Set the color and alpha of the visual.0x008090 and 0 respectively.
		 *
		 * iMyWidget->SetVisualColor( TMulVisualColor::EThumbnailBackgroundColor, TRgb(0x008090,0) );
		 * Here 0(zero) implies opacity - 1. 
		 * @endcode
		 * 
         * <B>Examples: To make the background Transparent </b>
         * @code
         * Set the color and alpha of the visual.0x008090 and 255 respectively.
         *
         * iMyWidget->SetVisualColor( TMulVisualColor::EThumbnailBackgroundColor, TRgb(0x008090,255) );
         * Here 255 alpha implies Transparent Background.No matter what the color is.
         * @endcode
		 * 		 
		 */
		virtual void SetVisualColor( TMulVisualType aVisualType , const TRgb& aColor ) = 0;
 
		    
		/** 
		 *
		 Virtual destructor. 
		 **/
		virtual ~IMulWidget()
			{
			}

		};

	} // namespace Alf

#endif // I_MULWIDGET_H

