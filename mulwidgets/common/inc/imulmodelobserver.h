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
* Description:  Implementation of model interface
 *
*/


#ifndef	I_MULMODEOBSERVER_H
#define I_MULMODEOBSERVER_H

#include <alf/ialfmodelchangeobserver.h> 

namespace Alf
	{
	
//Forward Declaration
class IMulVariantType;

/**
 * Observer interface to provide notification about model state change.
 * Widget needs to implement this interface to get highlight or marking change event.
 */
class IMulModelObserver //: public IAlfModelChangeObserver
	{
	
public: //enum

	/**
	 *Changed state of model 
	 */
	enum TMulChangedState 
	    { 
	    EHighlightChanged = 1, 
	    EMarkingChanged,
	    EDataWindowUpdated, 
	    EItemsInserted,
	    EItemsRemoved,
	    EItemUpdated,
	    ETemplateChanged,
	    EModelRefreshed
	    };

public: //method

	/**
     * Notify observer about IMulModel state change.
	 * Observer will be notified when highlight or marking is changed from model.
	 *
	 * @param aState Highlight or making changed
	 * @param aData Index of new highlight or Index of marked or unmarked item.
	 *				( Item is marked or not should be checked using IMulModelAccessor interface)
	 */
	virtual void ModelStateChanged( TMulChangedState aState, IMulVariantType& aData ) = 0;

	};
	
	} // Namespace Alf
	
#endif //I_MULMODEOBSERVER_H	
	
//End of file
	