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
* Description:  Template interface getter for CAlfElement class.
*
*/


// -----------------------------------------------------------------------------
// Template getter for interfaces provided by the the element classes.
// The type of the queried interface is specified by the
// template parameter.
// -----------------------------------------------------------------------------
//
template <class T>
T* AlfElement::makeInterface( AlfElement* aElement )
    {
    if (aElement)
        {
        return static_cast< T* >( aElement->makeInterface( T::type() ) );
        }
    return 0;
    }
