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
* Description:   Implements attributesetters for grid layout..
*
*/


//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alfgridlayoutattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>

//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alfgridlayout.h>

//dui includes
#include "alf/attrproperty.h"

//other includes
#include <libc/string.h>

using osncore::UString;

using namespace duiuimodel::commonvisualattributes;

using namespace duiuimodel::layoutattributes;

//variable inside ifdef to prevent compiler warning.
#ifdef ALF_DEBUG_EXCEPTIONS
static const char* const ClassName = "AlfGridLayoutAttributeSetter";
#endif

namespace Alf
    {

// ---------------------------------------------------------------------------
// returns int or realvalue as float.
// ---------------------------------------------------------------------------
//
float realOrInt(const AlfAttribute& aAttr, int aIndex)
    {
    switch(aAttr.type(aIndex))
        {
        case AlfAttributeValueType::EInt:
            return aAttr.intValue(aIndex);
        case AlfAttributeValueType::EFloat:            
            return aAttr.realValue(aIndex);
        default:
            ALF_THROW ( AlfDataException, ECommonError, ClassName);
        }
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfGridLayoutAttributeSetter::AlfGridLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfGridLayoutAttributeSetter::~AlfGridLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfGridLayoutAttributeSetter::setAttributeValue(
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    CAlfGridLayout* gridlayout = dynamic_cast<CAlfGridLayout*>(&aVisual);

    if (!gridlayout)
        {
        ALF_THROW(AlfVisualException, EInvalidVisual, ClassName)
        }

    AlfCommonLayoutAttributeSetter::setAttributeValue(aVisual,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfGridLayoutAttributeSetter::createCommand(
        CAlfVisual& /*aVisual*/,
        AlfAttributeContainer* /*aContainer*/, 
        IAlfMap* /*aData*/,
        int /*aTransitionTime*/, 
        CAlfVisual* /*aRefVisual*/ )
    {
    //deprecated
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// Sends a command to Env
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfGridLayoutAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    //no animatable attributes in grid layout.
    AlfCommonLayoutAttributeSetter::createAndSendCommands( 
        aVisual, aContainer, aRefVisual);
    }


// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual
// ---------------------------------------------------------------------------
//
void AlfGridLayoutAttributeSetter::handleDynamicAttribute(
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer)
    {
    //no dynamic attributes in grid layout.
    AlfCommonLayoutAttributeSetter::handleDynamicAttribute(
        aVisual, aAttr, aContainer);
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//
void AlfGridLayoutAttributeSetter::handleStaticAttribute ( 
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer)
    {
    const char* attrName = aAttr.name();
    CAlfGridLayout* gridlayout = dynamic_cast<CAlfGridLayout*>(&aVisual);
    if (!gridlayout)
        {
        return;
        }     

    if (!strcmp(attrName, grid::KRows))
        {
        int rows = realOrInt(aAttr, 0);     //this can throw
        TRAPD(err1, gridlayout->SetRowsL(rows));
        if (err1 != KErrNone) 
            {
            ALF_THROW(AlfException, err1, ClassName)
            }
        }
    else if (!strcmp(attrName, grid::KColumns))
        {
        int columns = realOrInt(aAttr, 0);    //this can throw
        TRAPD(err1, gridlayout->SetColumnsL(columns));
        if (err1 != KErrNone) 
            {
            ALF_THROW(AlfException, err1, ClassName)
            }
        }
    else if (!strcmp( attrName, grid::KColumnWeight))
        {
        unsigned int valueCount = aAttr.getTargetValueCount();

        for (int i = 0; i < valueCount; ++i)
            {
            TAlfUnit unitType = aAttr.unit(i);

            float val = realOrInt(aAttr, i);
            TAlfMetric metric( val, unitType );

            TRAPD(err, gridlayout->ReplaceWeightL(EAlfGridColumn, metric, i));

            int err2 = KErrNone;
            if (err == KErrArgument)
                {
                TRAP(err2, gridlayout->AppendWeightL(EAlfGridColumn, metric));
                if (err2 != KErrNone)
                    {
                    ALF_THROW(AlfException, err2, ClassName)
                    }
                }
            else if (err != KErrNone)
                {
                ALF_THROW(AlfException, ECommonError, ClassName)
                }
            }
        }
    else if ( !strcmp ( attrName, grid::KRowWeight ) )
        {
        unsigned int valueCount = aAttr.getTargetValueCount();

        for ( int i = 0; i < valueCount; ++i )
            {

            TAlfUnit unitType = aAttr.unit(i);
            float val = realOrInt(aAttr, i);
            TAlfMetric metric ( val, unitType );

            TRAPD ( err, gridlayout->ReplaceWeightL ( EAlfGridRow, metric, i ) );

            int err2 = KErrNone;
            if ( err == KErrArgument )
                {
                TRAP ( err2, gridlayout->AppendWeightL ( EAlfGridRow, metric ) );
                if (err2 != KErrNone)
                    {
                    ALF_THROW(AlfException, err2, ClassName)
                    }
                }
            else if (err != KErrNone)
                {
                ALF_THROW(AlfException, ECommonError, ClassName)
                }
            }
        }
    else if (!strcmp( attrName, KExpansionFlag ))
        {
        gridlayout->SetExpanding ( aAttr.intValue() );
        }
    else if (!strcmp( attrName, grid::KSetLayoutModeFlag ))
        {
        if (aAttr.getTargetValueCount() == 2 && 
            aAttr.type(0) == AlfAttributeValueType::EInt && 
            aAttr.type(1) == AlfAttributeValueType::EInt)
            {
            gridlayout->SetLayoutModeFlags(TAlfGridDimension( 
                aAttr.intValue(0)), uint(aAttr.intValue(1)));
            }
        else
            {
            ALF_THROW(AlfAttributeException, EInvalidAttribute, ClassName)
            }
        }
    else if (!strcmp( attrName, grid::KClearLayoutModeFlag ))
        {
        if ( aAttr.getTargetValueCount() == 2 && 
             aAttr.type(0) == AlfAttributeValueType::EInt && 
             aAttr.type(1) == AlfAttributeValueType::EInt)
            {
            gridlayout->ClearLayoutModeFlags(TAlfGridDimension( 
                aAttr.intValue(0)), uint(aAttr.intValue(1)));
            }
        else
            {
            ALF_THROW( AlfAttributeException, EInvalidAttribute, ClassName)
            }
        }
    else
        {
        AlfCommonLayoutAttributeSetter::handleStaticAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual from data
// ---------------------------------------------------------------------------
//
void AlfGridLayoutAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer, 
    IAlfMap* aData )
    {
    //no dynamic attributes in grid layout.
    AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute(aVisual,
            aAttr, aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual  from data
// ---------------------------------------------------------------------------
//
void AlfGridLayoutAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer, 
    IAlfMap* aData)
    {
    CAlfGridLayout* gridlayout = dynamic_cast<CAlfGridLayout*> ( &aVisual );
    if (!gridlayout)
        {
        ALF_THROW(AlfDataException, ECommonError, ClassName)
        }    

    const char* attrName = aAttr.name();
    const char* dataField = aAttr.getDataField();

    if (!dataField)
        {
        ALF_THROW(AlfDataException, ECommonError, ClassName)
        }

    if (!aData) 
        {
        ALF_THROW(AlfDataException, ECommonError, ClassName)
        }

    IAlfVariantType* data = aData->item(UString(dataField));

    if (data)
        {
        if (!strcmp(attrName, grid::KRows))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int rows = data->integer() ;
                TRAPD(err1, gridlayout->SetRowsL ( rows ));
                if (err1 != KErrNone) 
                    {
                    ALF_THROW(AlfException, err1, ClassName)
                    }
                }
            }
        else if (!strcmp( attrName, grid::KColumns))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int columns = data->integer();
                TRAPD(err1, gridlayout->SetColumnsL (columns));
                if (err1 != KErrNone)
                    {
                    ALF_THROW ( AlfException, ECommonError, ClassName )
                    }
                }
            }
        else if (!strcmp(attrName, KExpansionFlag))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                gridlayout->SetExpanding(data->integer());
                }
            }
        else if (!strcmp(attrName, grid::KSetLayoutModeFlag))
            {
            if (data->type() == IAlfVariantType::EContainer)
                {
                IAlfContainer* dataContainer = data->container();

                if ( dataContainer->count() != 2)
                    {
                    ALF_THROW(AlfDataException, EInvalidContainerOperation, 
                        ClassName)
                    }

                IAlfVariantType* dimensionData = dataContainer->item(0);
                IAlfVariantType* flags = dataContainer->item(1);

                if (dimensionData && flags &&
                    dimensionData->type() == IAlfVariantType::EInt && 
                    flags->type() == IAlfVariantType::EInt)
                    {
                    gridlayout->SetLayoutModeFlags(TAlfGridDimension( 
                        dimensionData->integer()) , uint(flags->integer()));
                    }
                }
            }
        else if (!strcmp(attrName, grid::KClearLayoutModeFlag))
            {
            if (data->type() == IAlfVariantType::EContainer)
                {
                IAlfContainer* dataContainer = data->container();

                if (dataContainer->count() != 2)
                    {
                    ALF_THROW(AlfDataException, EInvalidContainerOperation, 
                        ClassName)
                    }

                IAlfVariantType* dimensionData = dataContainer->item(0);
                IAlfVariantType* flags = dataContainer->item(1);

                if (dimensionData && flags && 
                    dimensionData->type() == IAlfVariantType::EInt && 
                    flags->type() == IAlfVariantType::EInt)
                    {
                    gridlayout->ClearLayoutModeFlags(TAlfGridDimension(
                        dimensionData->integer()), uint(flags->integer()));
                    }
                }
            }
        else if ( !strcmp ( attrName, grid::KColumnWeight ) )
            {
            }
        else if ( !strcmp ( attrName, grid::KRowWeight ) )
            {
            }
        else
            {
            AlfCommonAttributeSetter::handleStaticDataAttribute(aVisual, 
                aAttr, aContainer, aData);
            }
        }
    }

    } // namespace Alf
// End of file


