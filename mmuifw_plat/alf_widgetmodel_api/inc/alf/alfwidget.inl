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
* Description:  Template interface getter for CAlfWidget class.
*
*/


// -----------------------------------------------------------------------------
// Template getter for interfaces provided by the widget, the control,
// or the model. The type of the queried interface is specified by the
// template parameter.
// -----------------------------------------------------------------------------
//
template <class T>
T* AlfWidget::makeInterface( AlfWidget* aWidget )
    {
    return static_cast< T* >( AlfWidget::makeInterface( aWidget, T::type() ) );
    }
