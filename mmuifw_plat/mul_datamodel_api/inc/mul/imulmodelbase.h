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
* Description:  Interface for model base
*
*/


#ifndef I_MULMODELBASE_H
#define I_MULMODELBASE_H

//Includes

namespace Alf
    {
    // Forward Declaration    
    class IMulModelAccessor; 

   
    /*! @class IMulModelBase
     *  @brief An interface for common data model's base class.  
     *
     *  Client can query Accessor() interface from this class
     *  and can set Accessor to Multi-item Widgets.
     *
     * <b>Note:</b> <b><i>There has been a change in the way multi-item widget accesses 
	 * the data model (for other widgets continue to set the model to the widget in the same way). 
	 * The widget gets a restrictive access to the data model via IMulModelAccessor
	 * interface. The new way of assigning model to the widget is shown in the
	 * example given below. The client applications can't access the IMulModelAccessor
	 * interface anymore as its not export from now on.</i></b>
     *
     */
    class IMulModelBase 
        {    
    public:

    	/**
    	 * Return pointer of accessor interface.
    	 * This pointer is required to call SetModel Api IMulWidget.
    	 * Ownership of Accessor is not transferred.
    	 * 
    	 * @return IMulModelAccessor Accessor interface.
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
    	virtual IMulModelAccessor* Accessor() = 0 ;      
	
    }; //End of class

    } // namespace Alf

#endif // I_MULMODELBASE_H

//End of file

