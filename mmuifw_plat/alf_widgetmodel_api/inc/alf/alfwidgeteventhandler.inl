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
* Description:   Alfwidgethandler inline file.
*
*/


template <class T>
    T* AlfWidgetEventHandler::makeInterface( IAlfInterfaceBase* aThis )
    {        
    if ( aThis && aThis->type() == T::type() )
        {
        return static_cast<T*>( this );
        }
    return NULL;
    }

// End of File