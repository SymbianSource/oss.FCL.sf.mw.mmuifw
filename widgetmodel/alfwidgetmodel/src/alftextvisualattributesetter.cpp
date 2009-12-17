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
* Description:   Implements attributesetters for text.
*
*/


//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alftextvisualattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>
#include <alf/alfwidgetenvextension.h>


//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alftextvisual.h>
#include <alf/alftexture.h>
#include <alf/alfeventhandler.h>
#include <alf/alfevent.h>

//dui includes
#include "alf/attrproperty.h"

//other includes
#include <libc/string.h>
#include <utf.h>


//namespaces
using namespace osncore;
using namespace duiuimodel::textvisualattributes;
using namespace duiuimodel::commonvisualattributes;
using namespace duiuimodel::layoutattributes;

namespace Alf
    {


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfTextVisualAttributeSetter::AlfTextVisualAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfTextVisualAttributeSetter::~AlfTextVisualAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfTextVisualAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    CAlfTextVisual* textVisual = dynamic_cast<CAlfTextVisual*> ( &aVisual );

    if ( !textVisual )
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfTextVisualAttributeSetter" )
        }

    AlfCommonVisualAttributeSetter::setAttributeValue(aVisual,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfTextVisualAttributeSetter::createCommand (
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/ )
    {
    //deprecated
    TAlfCommand* cmd = NULL;
    return cmd;
    }

// ---------------------------------------------------------------------------
// Sends a command to Env
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfTextVisualAttributeSetter::createAndSendCommands (
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    //ShadowOpacity command can be sent as a TAlfCustomEventCommand

    AlfCommonVisualAttributeSetter::createAndSendCommands (
        aVisual, aContainer, aRefVisual );
    }


// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual
// ---------------------------------------------------------------------------
//
void AlfTextVisualAttributeSetter::handleDynamicAttribute ( CAlfVisual &aVisual, 
	                                                        AlfAttribute& aAttr, 
	                                                        AlfAttributeContainer& aContainer)
    {
    const char* attrName = aAttr.name();
    CAlfTextVisual* textVisual = dynamic_cast<CAlfTextVisual*> ( &aVisual );
    if(textVisual == 0)
        {
        return;
        }

    if ( !strcmp ( attrName, KShadowOpacity ) )
        {
        TAlfTimedValue tVal ( ( TReal32 ) aAttr.getSourceValue()->realValue() );
        tVal.SetTarget ( ( TReal32 ) aAttr.getTargetValue()->realValue(), ( TInt ) aAttr.getTime() );
        tVal.SetStyle ( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier ( aAttr.getMappingFunctionId());
        textVisual->SetShadowOpacity ( tVal );
        }
    else
        {
        AlfCommonVisualAttributeSetter::handleDynamicAttribute( aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//
void AlfTextVisualAttributeSetter::handleStaticAttribute( CAlfVisual& aVisual, 
                                                          AlfAttribute& aAttr, 
                                                          AlfAttributeContainer& aContainer)
    {
    const char* attrName = aAttr.name();
    CAlfTextVisual* textVisual = dynamic_cast<CAlfTextVisual*> ( &aVisual );
    if (!textVisual)
        {
        ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
        }

    if ( !strcmp ( attrName, KLineWrap ) )
        {
        textVisual->SetWrapping ( (CAlfTextVisual::TLineWrap)aAttr.intValue() );
        }

    else if ( !strcmp ( attrName, KLineSpacing ) )
    	{
        try
            {
            AlfAttribute* linespacingunitsAttr = &aContainer.getAttributeByName ( KLineSpacingUnits );

            //a line spacing units attribute was found
            textVisual->SetLineSpacing ( aAttr.intValue(), 
            	(CAlfTextVisual::TLineSpacingUnits)linespacingunitsAttr->intValue() );
            }

        catch ( AlfDataException& e )
            {
            //a line spacing units attribute was not found or it was of invalid data type... Use default...
            textVisual->SetLineSpacing ( aAttr.intValue() );
            }
        }

    else if ( !strcmp ( attrName, KPreconfiguredStyle ) )
        {
        try
            {
            AlfAttribute* backgroundtypeAttr = &aContainer.getAttributeByName ( KBackgroundType );

            //a background type attribute was found
            textVisual->SetStyle ( ( TAlfPreconfiguredTextStyle ) aAttr.intValue(), 
            	(TAlfBackgroundType)backgroundtypeAttr->intValue() );
            }
        catch ( AlfDataException& e )
            {
            //no background type attribute found or it was of invalid data type... Use default...
            textVisual->SetStyle ( ( TAlfPreconfiguredTextStyle ) aAttr.intValue() );
            }
        }
    else if ( !strcmp ( attrName, KText ) )
        {
        TPtrC8 src;
        src.Set ( ( TUint8* ) aAttr.stringValue().getUtf8() );
        HBufC* desC = NULL;
        
        TRAPD(err1, desC = CnvUtfConverter::ConvertToUnicodeFromUtf8L ( src ));
        if (err1 != KErrNone)
            {
            ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
            }

        if ( desC )
            {
            TRAPD(err2, textVisual->SetTextL ( *desC ));
            delete desC;
            if (err2 != KErrNone)
                {
                ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
                }
            }
        }
    else if ( !strcmp ( attrName, KTextSkinColorId ) )
        {
	     
	        
	    // Retrieve Skin Color Table index
	    try
	    	{
	    	// colorIndex will not be null. Throws if attribute not found.
	    	AlfAttribute* colorIndex = &aContainer.getAttributeByName ( KTextColorSkinIndex );
	    	setTextSkinColor(*textVisual, aAttr.stringValue().getUtf8(), colorIndex->stringValue().getUtf8()); 
	    	}
	    catch(AlfDataException& e)
	    	{
	    	// Error in getting the color index. use default value.	  
	    	setTextSkinColor(*textVisual, aAttr.stringValue().getUtf8());	
	    	}
	    	
        }
    else if (!strcmp (attrName, KLocalisationText) ) // Support for Localised text
        {
        
        AlfAttribute* langAttr=NULL;
        AlfAttribute* varArgsAttr=NULL;
	   
        // Get the language attribute
	    try
	    	{
	    	// Retrieve the language attribute.	
	    	langAttr = &aContainer.getAttributeByName ( KLang );
	    	}
	    catch(AlfDataException& e)
	    	{
	    	// Nothing to do here. just continue.
	    	}
	    
	    // Get the varargs attribute.
	    try
	    	{
	    	varArgsAttr = &aContainer.getAttributeByName ( KVarArgs );
	    	}
	    catch(AlfDataException& e)
	    	{
	    	// Nothing to do here. just continue.
	    	}
	    setLocalisedText(*textVisual,aAttr.stringValue().getUtf8(),langAttr,varArgsAttr);	
	    
        }
    else if ( !strcmp ( attrName, KTextColor ) )
        {
        if ( 4 == aAttr.getTargetValueCount() && AlfAttributeValueType::EInt == aAttr.type(0) && 
             AlfAttributeValueType::EInt == aAttr.type(1) && AlfAttributeValueType::EInt == aAttr.type(2) && 
             AlfAttributeValueType::EInt == aAttr.type(3))
            {
            TRgb color ( aAttr.intValue(0), aAttr.intValue(1), aAttr.intValue(2), aAttr.intValue(3) );
            textVisual->SetColor( color );
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute, "AlfTextVisualAttributeSetter" );
            }
        }
    else if ( !strcmp ( attrName, KTextColorSkinIndex ) )
        {
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KTextSkinColorId
        }
    else if ( !strcmp ( attrName, KBackgroundType ) )
        {
        // Can not do anything with background type alone.
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KPreconfiguredStyle
        }
    else if ( !strcmp ( attrName, KLineSpacingUnits ) )
        {
        // Can not do anything with line spacing units alone.
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KLineSpacing
        }
    else if ( !strcmp ( attrName, KStyle ) )
        {
        textVisual->SetTextStyle ( aAttr.intValue() );
        }
    else if ( !strcmp ( attrName, KFont ) )
        {
        //not supported, no API in textvisual
        }
    else if ( !strcmp ( attrName, KVerticalAlign ) )
        {
        AlfAttribute* horzalignAttr = &aContainer.getAttributeByName ( KHorizontalAlign );
        //will throw if attribute is not found

        textVisual->SetAlign ( ( TAlfAlignHorizontal ) horzalignAttr->intValue(),
                               ( TAlfAlignVertical ) aAttr.intValue() );
        }
    else if ( !strcmp ( attrName, KHorizontalAlign ) )
        {
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KVerticalAlign
        }
    else if ( !strcmp ( attrName, KMaxLineCount ) )
        {
        if (aAttr.type(0) == AlfAttributeValueType::EInt)
            {
            textVisual->SetMaxLineCount ( aAttr.intValue() );
            }
        else if (aAttr.type(0) == AlfAttributeValueType::EFloat)
            {
            textVisual->SetMaxLineCount ( aAttr.realValue() );
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute, "AlfTextVisualAttributeSetter" );            
            }
        
        }
    else if ( !strcmp ( attrName, KEnableShadow ) )
        {
        int enableShadow = aAttr.intValue();
        TBool flag = ETrue;                      //Assume true for any non-zero value.
        if ( 0 == enableShadow )
        	{
        	flag = EFalse;
        	}
        textVisual->EnableShadow ( flag );
        }
    else
        {
        AlfCommonVisualAttributeSetter::handleStaticAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual from data
// ---------------------------------------------------------------------------
//
void AlfTextVisualAttributeSetter::handleDynamicDataAttribute (
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData )
    {
    //Shadow opacity can be supported
    AlfCommonVisualAttributeSetter::handleDynamicDataAttribute(aVisual, aAttr,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual  from data
// ---------------------------------------------------------------------------
//
void AlfTextVisualAttributeSetter::handleStaticDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData )
    {
    const char* dataField = aAttr.getDataField();

	// throw if datafield name or data is not set
    if ( !dataField )
    	{
    	ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
    	}

    if (!aData)
    	{
    	ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
    	}

    IAlfVariantType* data = aData->item ( UString(dataField));
    const char* attrName = aAttr.name();

    CAlfTextVisual* textVisual = dynamic_cast<CAlfTextVisual*> ( &aVisual );
    if(textVisual == 0)
        {
        ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
        }

    if ( !strcmp ( attrName, KLineWrap ) )
        {
        if ( data && data->type() == IAlfVariantType::EInt )
            {
            textVisual->SetWrapping ( ( CAlfTextVisual::TLineWrap ) data->integer() );
            }
        }
    else if ( !strcmp ( attrName, KText ) )
        {
        if ( data && data->type() == IAlfVariantType::EString )
            {
            TPtrC8 src;
            src.Set ( ( TUint8* ) data->string().getUtf8() );
            HBufC* desC = NULL;
            TRAPD(err1, desC = CnvUtfConverter::ConvertToUnicodeFromUtf8L ( src ));
            if (err1 != KErrNone)
            	{
            	ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
            	}

            if ( desC )
                {
                TRAPD(err2, textVisual->SetTextL ( *desC ));
                delete desC;
                if (err2 != KErrNone) 
                	{
                	ALF_THROW ( AlfDataException, ECommonError, "AlfTextVisualAttributeSetter" )
                	}
                }
            }
        }
    else if (!strcmp (attrName, KLocalisationText) ) // Support for Localised text
        {           
            
        AlfAttribute* langAttr=NULL;
        AlfAttribute* varArgsAttr=NULL;            
	        
	    	// Retrieve the language attribute.
	    	if ( data && data->type() == IAlfVariantType::EString )
	    		{   		
	    	
	    		try
	    			{
	    			langAttr = &aContainer.getAttributeByName ( KLang );
	    			}
	    		catch(AlfDataException& e)
	    			{
	    			// Nothing to do here .Just continue.
	    			}
	    		try
	    			{
	    			varArgsAttr = &aContainer.getAttributeByName ( KVarArgs );	    			
	    			}
	    		catch(AlfDataException& e)
	    			{
	    			// Nothing to do here .Just continue.
	    			}
	    		setLocalisedText(*textVisual, data->string().getUtf8(),langAttr,varArgsAttr); 
	    		}	    		
	    		
        }

    else if ( !strcmp ( attrName, KTextColor ) )
        {
        if ( data && data->type() == IAlfVariantType::EContainer )
            {
            IAlfContainer* dataContainer = data->container();

            if ( 4 != dataContainer -> count() )
            	{
            	ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfTextVisualAttributeSetter" )
            	}

            IAlfVariantType* red = dataContainer->item ( 0 ); //red
            IAlfVariantType* green = dataContainer->item ( 1 ); //green
            IAlfVariantType* blue = dataContainer->item ( 2 ); //blue
            IAlfVariantType* alpha = dataContainer->item ( 3 ); //alpha

            if ( red && IAlfVariantType::EInt == red->type() && 
                 green && IAlfVariantType::EInt == green->type() && 
                 blue && IAlfVariantType::EInt == blue->type() && 
                 alpha && IAlfVariantType::EInt == alpha->type() )
                {
                TRgb color ( red->integer(), green->integer(), blue->integer(), alpha->integer() );
                textVisual->SetColor ( color );
                }
            }
        }
    else if ( !strcmp ( attrName, KTextSkinColorId ) )
        {
        if ( data && data->type() == IAlfVariantType::EString )
            {
            
            try
    			{
		 		// Retrieve skin color table index			
    			AlfAttribute* colorIndex = &aContainer.getAttributeByName(
					KTextColorSkinIndex); // colorIndex will not be null. Throws if not found.
					
				const char* colorIndexDataField = colorIndex->getDataField();
				IAlfVariantType* colorIndexData =
					aData->item(UString(colorIndexDataField));
				if(colorIndexData && colorIndexData->type() == IAlfVariantType::EString)
					{
    				setTextSkinColor(*textVisual, data->string().getUtf8(),
    								 colorIndexData->string().getUtf8());
    				}
    			}
    		catch(AlfDataException& e)
    			{
    			// No color index found. Use default value. 	
    			setTextSkinColor(*textVisual, data->string().getUtf8());	    				
    			}
    			
            }
        }
    else if ( !strcmp ( attrName, KTextColorSkinIndex ) )
        {
        // Handled in KTextSkinColorId case above.
        }
    else if ( !strcmp ( attrName, KPreconfiguredStyle ) )
        {
        try
            {
            AlfAttribute* backgroundtypeAttribute = &aContainer.getAttributeByName ( KBackgroundType );
            const char* backgroundDataField = backgroundtypeAttribute->getDataField();
            IAlfVariantType* backgroundData = aData->item ( UString(backgroundDataField) );

            if ( data && data->type() == IAlfVariantType::EInt && backgroundData && backgroundData->type() == IAlfVariantType::EInt )
                {
                //a valid background type attribute was found
                textVisual->SetStyle ( ( TAlfPreconfiguredTextStyle ) data->integer(), ( TAlfBackgroundType ) data->integer() );
                }
            }
        catch ( AlfDataException& e )
            {
            if ( data && data->type() == IAlfVariantType::EInt )
                {
                //no valid background type attribute found... Use default...
                textVisual->SetStyle ( ( TAlfPreconfiguredTextStyle ) data->integer() );
                }
            }
        }
    else if ( !strcmp ( attrName, KBackgroundType ) )
        {
        // Can not do anything with background type alone.
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KPreconfiguredStyle
        }
    else if ( !strcmp ( attrName, KLineSpacing ) )
        {
        try
            {
            AlfAttribute* linespacingunitsAttribute = &aContainer.getAttributeByName ( KBackgroundType );
            const char* linespacingunitsDataField = linespacingunitsAttribute->getDataField();
            IAlfVariantType* linespacingunitsData = aData->item ( UString(linespacingunitsDataField) );

            if ( data && data->type() == IAlfVariantType::EInt && linespacingunitsData && linespacingunitsData->type() == IAlfVariantType::EInt )
                {
                //a valid line spacing units attribute was found
                textVisual->SetLineSpacing ( data->integer(), ( CAlfTextVisual::TLineSpacingUnits ) linespacingunitsData->integer() );
                }
            }
        catch ( AlfDataException& e )
            {
            if ( data && data->type() == IAlfVariantType::EInt )
                {
                //a valid line spacing units attribute was not found... Use default...
                textVisual->SetLineSpacing ( data->integer() );
                }
            }
        }
    else if ( !strcmp ( attrName, KLineSpacingUnits ) )
        {
        // Can not do anything with line spacing units alone.
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KLineSpacing
        }
    else if ( !strcmp ( attrName, KStyle ) )
        {
        if ( data && data->type() == IAlfVariantType::EInt )
            {
            textVisual->SetTextStyle ( data->integer() );
            }
        }
    else if ( !strcmp ( attrName, KFont ) )
        {
        //not supported, no API in textvisual
        }
    else if ( !strcmp ( attrName, KVerticalAlign ) )
        {
        //will throw if horizontal align is not found
        AlfAttribute* horzAlignAttribute = &aContainer.getAttributeByName ( KHorizontalAlign );
        const char* horzAlignDataField = horzAlignAttribute->getDataField();
        IAlfVariantType* horzAlignData = aData->item ( UString(horzAlignDataField) );

        if ( data && data->type() == IAlfVariantType::EInt && horzAlignData && horzAlignData->type() == IAlfVariantType::EInt )
            {
            textVisual->SetAlign ( ( TAlfAlignHorizontal ) horzAlignData->integer(),
                                   ( TAlfAlignVertical ) data->integer() );
            }
        }
    else if ( !strcmp ( attrName, KHorizontalAlign ) )
        {
        // Do nothing.
        // This attribute will be used in some other iteration in the
        // else-if block that uses KVerticalAlign
        }
    else if ( !strcmp ( attrName, KMaxLineCount ) )
        {
        if ( data && data->type() == IAlfVariantType::EInt )
            {
            textVisual->SetMaxLineCount ( data->integer() );
            }

        }
    else if ( !strcmp ( attrName, KEnableShadow ) )
        {
        if ( data && data->type() == IAlfVariantType::EBool )
            {
            textVisual->EnableShadow ( data->boolean() );
            }
        }
    else
        {
        AlfCommonVisualAttributeSetter::handleStaticDataAttribute (
            aVisual, aAttr, aContainer, aData );
        }
    }
    

// ---------------------------------------------------------------------------
// Localisation attribute setter helper method. Helper Method.
// ---------------------------------------------------------------------------
//    
void AlfTextVisualAttributeSetter::setLocalisedText(CAlfVisual& /*aVisual*/,const char* /*aLocId*/,AlfAttribute* /*aLangAttr*/,AlfAttribute* /*aVarArgsAttr*/)
	{
 	}



// ---------------------------------------------------------------------------
// Helper Method for setting skin color.
// ---------------------------------------------------------------------------
//
void AlfTextVisualAttributeSetter::setTextSkinColor(CAlfTextVisual& /*aTextVisual*/, const Utf8* /*aColorTable*/, 
																const char* /*aColorIndex*/)
	{
	}

    } // namespace Alf
    
// End of file
