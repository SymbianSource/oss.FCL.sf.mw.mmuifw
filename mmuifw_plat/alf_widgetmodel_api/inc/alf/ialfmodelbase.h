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




#ifndef I_ALFMODELBASE_H
#define I_ALFMODELBASE_H

namespace osncore
    {
class UString;
    }

using osncore::UString;

namespace Alf
    {
// CLASS DECLARATIONS

/**
 * Base class for setting custom data to a variant type.
 * Derive from this class if you want to store custom data in
 * a variant type.
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfModelBase
    {
public:
    virtual ~IAlfModelBase() {}
    virtual const UString& Type() = 0;
    };

    }// namespace Alf

#endif // I_ALFMODEL_H

// End of File
