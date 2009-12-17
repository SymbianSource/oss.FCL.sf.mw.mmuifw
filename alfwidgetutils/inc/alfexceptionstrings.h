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
* Description:  error codes and descrptions for erros
*
*/



#ifndef ALFEXCEPTIONSTRINGS_H
#define ALFEXCEPTIONSTRINGS_H

namespace osncore
    {
static const char* const KErrorUndefinedException 
                            = "Undefined Exception";
static const char* const KErrorInvalidAttribute 
                            = "Invalid Attribute";
static const char* const KErrorInvalidAttributeValue 
                            = "Invalid Attribute Value";
static const char* const KErrorIncompleteAttributeValues 
                            = "Incomplete Attribute Values";
static const char* const KErrorInvalidVisual 
                            = "Invalid Visual";
static const char* const KErrorCanNotCreateVisual 
                            = "Can Not Create Visual";
static const char* const KErrorInvalidVisualValue 
                            = "Invalid Visual Value";
static const char* const KErrorInvalidElement 
                            = "Invalid Element";

//Data Type Exception Strings
static const char* const KErrorInvalidVariantDataType 
                            = "Invalid Variant Data Type";
static const char* const KErrorInvalidArrayIndex 
                            = "Invalid Array Index";
static const char* const KErrorInvalidContainerOperation 
                            = "Invalid Conatiner Operation";
static const char* const KErrorInvalidMapOperation 
                            = "Invalid Map Operation";
static const char* const KErrorInvalidBranchOperation 
                            = "Invalid Branch Operation";
static const char* const KErrorInvalidModelOperation 
                            = "Invalid Model Operation";
static const char* const KErrorInvalidWidget 
                            = "Invalid Widget";

    } //namespace

#endif //ALFEXCEPTIONSTRINGS_H

// End of File
