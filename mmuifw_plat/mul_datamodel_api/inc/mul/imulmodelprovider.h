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
* Description:  Interface for requesting data from the provider
*
*/


#ifndef I_MULMODELPROVIDER_H
#define I_MULMODELPROVIDER_H

namespace Alf
    {

// Forward Declarations
class MulVisualItem;
class MulDataPath;

/*! @class IMulModelProvider
 *  @brief An interface for requesting the data provider for data.
 *
 *  Application should implement this interface inorder to become a data provider. 
 *  Data model uses this interface to ask the application for data. 
 *
 */
class IMulModelProvider
    {
public:

    /**
     * Request the data provider for updating data in the model.
     *
     * @param aIndex Index at which the item request has been initiated.
     * @param aCount Number of items requested.
     * @param aPath  Path of parent item in the data hierarchy.
     */ 
    virtual void ProvideData( int aIndex, int aCount, MulDataPath aPath ) = 0;
            
    };


} // namespace Alf

#endif // I_MULMODELPROVIDER_H

//End of file

