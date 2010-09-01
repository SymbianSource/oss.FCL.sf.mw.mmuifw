/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This interface defines a generic data type
*
*/


#ifndef I_ALFMODELCHANGEOBSERVER_H
#define I_ALFMODELCHANGEOBSERVER_H

//INCLUDES

namespace osncore
    {
template <class T> class AlfPtrVector;
    }
using namespace osncore;

namespace Alf
    {

/* Forward declarations*/
class IAlfModel;
class AlfModelOperation;

/**
*  An interface for observing the changes in the model.
*  @lib hitchcockwidgetmodel.lib
*  @since S60 ?S60_version
* @status Draft
*/
class IAlfModelChangeObserver
    {
public:
    /** Virtual destructor.*/
    virtual ~IAlfModelChangeObserver() {}

    /**
     * Notify the observer that the model has changed.
     *
     * @param aModel    The new model.
     * @since S60 ?S60_version
     */
    virtual void modelChanged( IAlfModel& aModel ) = 0;

    /**
     * Notify the observers that a number of change operations
     * are about to be performed on the model.
     *
     * @param aArrayOfOperation An Array containing the operations performed on the model.
     * @since S60 ?S60_version
     */
    virtual void dataChanging( const AlfPtrVector<AlfModelOperation>& aArrayOfOperation ) = 0;

    /**
     * Notify the observers that a change operation
     * is about to be performed on the model.
     *
     * @param aOperation    An operation performed on the model.
     * @since S60 ?S60_version
     */
    virtual void dataChanging( const AlfModelOperation& aOperation ) = 0;

    /**
     * Notify the observers that the data of the model has been changed.
     * This is called automatically after any DataChangingL() call.
     *
     * @since S60 ?S60_version
     */
    virtual void dataChanged() = 0;
    };


    } // namespace Alf

#endif // I_ALFMODELCHANGEOBSERVER_H

// End of File
