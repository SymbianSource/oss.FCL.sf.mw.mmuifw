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
* Description:  Interface for embedabld widget
*
*/


#ifndef I_MULEMBEDABLEWIDGET_H
#define I_MULEMBEDABLEWIDGET_H
#include <mul/mulvisualitem.h>
#include <alf/ialfwidgeteventhandler.h>
namespace Alf
    {

/** @class IMulEmbedableWidget
 *  @brief Interface for Embedable widget.
 *
 *  Every widget that can be embeded need to implement this interface.
 *  Parent widget will use this interface to access embeded widget.
 */
class IMulEmbedableWidget
    {
public:

    /**
     * Set/Update data of Embeded widget.
	 * Parent widget will provides its own visual item as parameter
	 * embeded widget will have to construct new visual item from provided visual item
	 * and remove unwanted attributes and call update on singel item model.
	 *
	 * @param aItem Parent widget's data in visual item format
	 */ 
   	virtual void SetData(const MulVisualItem& aItem) = 0;
	
	
	/**
     * Return data in format of visual item.
	 * Parent will used this api to update its own data model.
	 *
	 * @return  Data of embeded widget
	 */
	virtual const MulVisualItem& Data() = 0;
	
	/**
	 * Add as eventhandler for embeded widget.
	 * Parent will use this api to add itself and client as event handler of child widget.
	 * 
	 * @param aHandler Event handler
	 */
	virtual void AddEventHandler( IAlfWidgetEventHandler& aHandler ) = 0;

	/**
	 * Remove as eventhandler form embeded widget.
	 * Parent can remvoe itself and client as event handler using this api
	 * 
 	 * @param aHandler Event handler
	 */
	virtual void RemoveEventHandler( IAlfWidgetEventHandler& aHandler ) = 0;
	
	/**
	 * Return container layout of embeded widget
	 * Parent can use this api to show or hide widget and position child widget.
	 * 
	 * @return Layout of embeded widget.
	 */
	virtual const CAlfLayout& ContainerLayout() = 0;
	
    };


    } // namespace Alf

#endif // I_MULEMBEDABLEWIDGET_H

//End of file

