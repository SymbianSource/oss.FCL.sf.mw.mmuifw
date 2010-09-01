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
* Description:  The base class for Multimedia multi-item widgets.
 *
*/


#ifndef I_MULMULTIITEMWIDGET_H
#define I_MULMULTIITEMWIDGET_H

// Includes
#include <mul/imulwidget.h>

// Namespaces used
using namespace osncore;

namespace Alf
	{
	    
    // Forward declaration
	class IMulModelAccessor;
	
	namespace mulmultiitemwidget
		{
		static const IfId Ident=
			{
			0, "mulmultiitemwidget"
			};
		}

	/*! @class IMulMultiItemWidget
	 *  @brief An interface for all Multimedia multi-item widgets.
	 */
	class IMulMultiItemWidget : public IMulWidget
		{
	public:

		/*! @enum TMulWidgetPropertiesFlag
		 * This enumeration defines some properties for the widgets. These can
		 * be enabled or disabled for individual widget instances.
		 */
		enum TMulWidgetPropertiesFlag
			{
			EMulWidgetFlagFastScroll = 0x0001, /*!< Enable fast scroll.*/ 
            EMulWidgetFlagCyclic = 0x0002, /*!< Enable cyclic/looping effect on widget.*/
            EMulWidgetFlagNaviScroll = 0x0004,  /*!< Enable navi scroll support.*/
            EMulWidgetFlagCenterFocus = 0x0008, /*!<Deprecated.*/
            EMulWidgetFlagMultiSelect = 0x0010, /*!< Enable multiple marking/selection in widget. EMulWidgetMarkingMode must be enabled*/
            EMulWidgetFlagVisible = 0x0020,  /*!< Attribute is of type String.*/
            EMulWidgetMarkingMode = 0x0040, /*!< Enable marking/selection in widget.Only one item can be marked with this flag*/
            EMulWidgetFlagDrag = 0x0080, /*!< Enable draging support.*/
            EMulWidgetImageScaleMode = 0x0100, /*!< Deprecated.*/
            EMulWidgetFlagSwap = 0x0200, /*!< Applicable only for grid widget for swaping item instead of reorder.*/
            EMulWidgetFlagSingleTap = 0x0800, /*!< Enable single tap item selection.*/
            EMulWidgetTitleMarqueeOn = 0x1000, /*!< Enable title marquee for list widget.*/
            EMulWidgetDoubleTap = 0x0400/*!< Enable double tap .*/ 
			};
			
		/*! @enum TMulAttributeDataType
	     * This enumeration defines the attribute data types
	     * 
	     */
		enum TMulAttributeDataType
	    	{
			EString = 1,    /*!< Attribute is of type String.*/
			EDate,          /*!< Attribute is of type Date.*/
			ENumeric        /*!< Attribute is of type Numeric.*/
    		};
    	/*! @enum TMulReOrderMode
    	 *
    	 *	This enumeration defines the Reorder mode
    	 */
    	enum TMulReOrderMode
    	    {
    	    EReorderShiftOn =1,   /*!< Switch On the Reorder Mode, and Type will be Shift */
    	    EReorderSwapOn,		  /*!< Switch On the Reorder Mode, and Type will be Swap */
  	  		EReorderDone,       /*!< Switch Off the Reorder Mode after committing  the reorder changes.*/
    	    EReorderCancel        /*!< Switch Off the Reorder Mode, with Last Operation Cancel*/    		    		
          };	
    		
		/**
		 * Returns the type identifier of this interface.
		 *
		 * @return A descriptor to identify the type of this interface.
		 */
		static inline const IfId& Type()
			{
			return mulmultiitemwidget::Ident;
		}

		/**
		 * Set a property or a group of properties. Properties not specified are not changed.
		 *
		 * @param aAnyFlag Property flag(s) to be set. To set more than one property,  
		 *                  flags can be ORed.
		 * @throw Invalid argument, if unknown/undefined flag is passed.
		 * @throw Invalid argument, if the property is not supported by the widget.
		 *
		 * <B>Examples:</b>
		 * @code
		 * Set the flag for selection.
		 *
		 *      mListWidget->SetFlags( IMulMultiItemWidget::EMulWidgetFlagMultiSelect );
		 * @endcode
		 */
		virtual void SetFlags(unsigned int aAnyFlag) = 0;

		/**
		 * Clear a property or a group of properties. Properties not specified are not changed.
		 *
		 * @param aAnyFlag  Property flag(s) to be cleared. To clear more than one property,
		 *                  the flags can be ORed.
		 * @throw Invalid argument, if unknown/undefined flag is passed.
		 * @throw Invalid argument, if the property is not supported by the widget.
		 *
		 * <B>Examples:</b>
		 * @code
		 * Clear the flag for selection.
		 *
		 *  	mListWidget->ClearFlags( IMulMultiItemWidget::EMulWidgetFlagMultiSelect );
		 * @endcode		 
		 */
		virtual void ClearFlags(unsigned int aAnyFlag) = 0;

		/**
		 * Return the status of all the flags. The flags that are set have the 
		 * corresponding bit ON.
		 *
		 * @return All enabled flag(s). The corresponding bit for enabled flag(s) are set to 1.
		 *
		 * <B>Examples:</b>
		 * @code
		 * Get all flags status.
		 *
		 *  	int allFlags = mListWidget->GetFlags();
		 * @endcode		 
		 */
		virtual unsigned int GetFlags() const = 0;

		/**
		 * Returns the property state, whether flag is on or off for widget.
		 *
		 * @param aAnyFlag  Flag to be cross-checked
		 * @return True if set, False if not set
		 *
		 * <B>Examples:</b>
		 * @code
		 * Check if the cyclic flag is set
		 *  
		 *      if( mListWidget->IsFlagSet( IMulMultiItemWidget::EMulWidgetFlagCyclic ) )
		 *          {
		 *          ...
		 *          }
		 * @endcode		 
		 */
		virtual bool IsFlagSet(TMulWidgetPropertiesFlag aAnyFlag) const = 0;
		
        /**
		 * Provide an empty text for the widget. This text is shown if there is no
		 * data to be displayed in the widget. Widget makes a copy of the variable
		 * and it is safe to call it many times.
		 *
		 * @param aDefaultText Text to be displayed in empty widget
		 *
		 * <B>Examples:</b>
		 * @code
		 * Set the empty text to "No Images"
		 *
		 *      mListWidget->SetEmptyText( UString("No Images") );
		 * @endcode
		 */
		virtual void SetEmptyText( const UString& aDefaultText ) = 0;

		/**
		 * Set the data model for this widget. Widget uses the data model via 
		 * IMulModelBase interface because it needs to know only limited functionality 
		 * about the data model.
		 *
		 * @param aModel  Model to be set for widget.
		 * @param aOwnership Flag which conveys the widget whether it should take 
		 * the ownersip of the model or not.
		 * @throw Logic error, if same accessor object passed again.
		 *
		 * <B>Examples:</b>
		 * @code
		 * Create the data model using widget factory and assign it to a widget.
		 *
		 *      mWidgetModel = widgetFactory.createModel<IMulModel> (KLoadNameModel);
		 *
         * Set the default visualization for the list items. If the visualization is not 
         * specified there will be a panic when the model is assigned to the widget.
         *
	     *      mWidgetModel->SetTemplate( UString ( mulwidget::KTemplate6));
	     *
	     * Setting the data model to the widget. 
         * 
	     *      AddItems(10);
	     *      mListWidget->SetModel( mWidgetModel);
		 * @endcode
		 */
	//	virtual void SetModel(IMulModelBase* aModel, bool aOwnership) = 0;		
		
    	/**
    	 * Set the state of the mark mode.
    	 *
    	 * @param aFlag True means mode is On, False means mode is Off.
    	 *
		 * <B>Examples:</b>
		 * @code
		 * Switch on the mark mode.
		 *
		 *      mListWidget->SetMarkMode(true);
		 * @endcode
    	 */
        virtual void SetMarkMode(bool aFlag) = 0;
    
        /**
    	 * Returns the state of the mark mode.
    	 *
    	 * @return  Status of the mark mode.
    	 *
		 * <B>Examples:</b>
		 * @code
		 * Check if the mark mode is On.
		 *
		 *      if( mListWidget->IsMarkMode() )
		 *          {
		 *          ...
		 *          }
		 * @endcode
    	 */
    	virtual bool IsMarkMode()const = 0;

        /**
    	 * Set the state of the reorder mode.
    	 *
    	 * @param aFlag EReorderShiftOn    : Switch on reorder mode in type shift.
    	 *				EReorderSwapOn     : Switch on reorder mode in type swap.
    	 *			      EReorderDone       : Switch Off the reorder with operatoin done
    	 *              EReorderCancel     : Switch off the reorder mode with cancel last operation.
    	 *
		 * <B>Examples:</b>
		 * @code
		 * Switch on the reorder mode.
		 *
		 *      mListWidget->SetReorderMode(IMulMultiItemWidget::EReorderShiftOn);
		 * @endcode
    	 */    	
    	virtual void SetReorderMode(IMulMultiItemWidget::TMulReOrderMode aReorderMode ) = 0;
		
		
		/**
		 * Returns whether reorder mode on/off.
		 *
		 * @return  Reorder Mode On/Off Status.
		 *
		 * <B>Examples:</b>
		 * @code
		 * Check if the reorder mode is On.
		 *
		 *      if( mListWidget->IsReorderModeOn() )
		 *          {
		 *          ...
		 *          }
		 * @endcode
		 */    	 
    	virtual bool IsReorderModeOn()const = 0 ;
    	
    		
    	/**
    	 * During fast scroll mode, the widget displays an overlay with more info.
    	 * Setter API for the overlay strip.
    	 *
    	 * @param aType Attribute data type
    	 * @param aAttribute Attribute whose value will be shown in the overlay
    	 * @param aFormat Format string for formatting the value of the attribute
    	 *
    	 * <B>Examples:</b>
		 * @code
		 * Set the overlay to display (Month, Year) as more info.
		 *
		 *      mListWidget->SetOverlay( IMulMultiItemWidget::EDate, 
		 *          UString(mulvisualitem::KMulDetail),
		 *          UString("") );
		 * @endcode
    	 */
    	virtual void SetOverlay(TMulAttributeDataType aType, const UString& aAttribute, const UString& aFormat) = 0 ;
    	
		/**
		 * Sets the default image of the widget.
		 * This image will be shown when the actual image data is not available
		 *
		 * @param aTextureId texture id to be used for default image.
		 *
		 * <B>Examples:</b>
		 * @code
		 * Set default image in coverflow widget. 
		 *
		 *     mCoverFlowWidget->SetDefaultImage( 1000 );
		 * 
		 * @endcode
		 */
       virtual  void SetDefaultImage(int aTextureId) = 0;          	
	
		/** 
		 * Virtual destructor. 
		 */
		virtual ~IMulMultiItemWidget()
			{
			}

		};

	} // namespace Alf

#endif // I_MULMULTIITEMWIDGET_H

