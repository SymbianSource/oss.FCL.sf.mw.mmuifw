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
* Description:  List Template Interface
 *
*/

#ifndef	I_MULLOGICALTEMPLATE_H
#define I_MULLOGICALTEMPLATE_H

#include "mulvisualutility.h"
#include "mulitemdefinitions.h"

namespace Alf
	{

	
class IMulLogicalTemplate
    {

public:
    virtual ~IMulLogicalTemplate() {}
    //fills up the data structure pertaining to specific templates
    virtual void PopulateData() = 0;	
    //Return the structure of the specified element Id   
    virtual const LCTElementStructure* GetElementStructure(
        TMulCurrentOrientation aCurOrient,
        const char* aElemId) = 0;
	
    };
    
	}


#endif //I_MULLOGICALTEMPLATE_H	
	
//End of file
