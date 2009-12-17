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
* Description:   Implements attributesetters for anchorlayout.
*
*/



//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alfanchorlayoutattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>


//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alfanchorlayout.h>
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

using namespace duiuimodel::anchorlayoutinput;

using namespace duiuimodel::layoutattributes;

using namespace duiuimodel::anchor;

using namespace duiuimodel::metric;

namespace Alf
	{

bool isMultivalueDataAchment ( TAlfAnchorAttachmentOrigin val )
	{
	return ( val == EAlfAnchorAttachmentOriginNone||
			 val == EAlfAnchorAttachmentOriginTopLeft ||
	         val == EAlfAnchorAttachmentOriginTopCenter ||
	         val == EAlfAnchorAttachmentOriginTopRight ||
	         val == EAlfAnchorAttachmentOriginCenterLeft ||
	         val == EAlfAnchorAttachmentOriginCenter ||
	         val == EAlfAnchorAttachmentOriginCenterRight ||
	         val == EAlfAnchorAttachmentOriginBottomLeft ||
	         val == EAlfAnchorAttachmentOriginBottomCenter ||
	         val == EAlfAnchorAttachmentOriginBottomRight );
	}

typedef void ( *pFunc ) ( AlfContainer* aCont, AlfAttribute* aAttrib, IAlfMap* aData );

void addToAnchortypecont ( AlfContainer* aCont, AlfAttribute* aAttrib, IAlfMap* aData )
	{

	if ( aData )
		{
		IAlfVariantType* dataX = aData->item ( UString(aAttrib->getDataField() ));

		if ( dataX && dataX->type() == IAlfVariantType::EInt )
			{
			aCont->addItem ( new ( EMM ) AlfVariantType ( dataX->integer() ) );
			}
		}
	else
		{
		aCont->addItem ( new ( EMM ) AlfVariantType ( aAttrib->intValue() ) );
		}

	}

void addToAttachmentcont ( AlfContainer* aCont, AlfAttribute* aAttrib, IAlfMap* aData )
	{

	if ( aData )
		{
		IAlfVariantType* dataX = aData->item ( UString(aAttrib->getDataField()));

		if ( dataX && dataX->type() == IAlfVariantType::EInt )
			{
			aCont->addItem ( new ( EMM ) AlfVariantType ( dataX->integer() ) );
			}
		}
	else
		{
		aCont->addItem ( new ( EMM ) AlfVariantType ( aAttrib->intValue() ) );
		}
	}

void addToMagnitudecont ( AlfContainer* aCont, AlfAttribute* aAttrib, IAlfMap* aData )
	{

	if ( aData )
		{
		IAlfVariantType* dataX = aData->item ( UString(aAttrib->getDataField()) );

		if ( dataX != NULL && dataX->type() == IAlfVariantType::EMap )
			{
			IAlfVariantType* valueX = dataX->map()->item ( UString("value"));
			IAlfVariantType* unitX = dataX->map()->item ( UString("unit"));
			auto_ptr<AlfContainer> value ( new ( EMM ) AlfContainer() );
			auto_ptr<AlfVariantType> vtype1(new ( EMM ) AlfVariantType ( valueX->real() ));
			value->addItem ( vtype1.get() );
			vtype1.release();
			auto_ptr<AlfVariantType> vtype2(new ( EMM ) AlfVariantType ( unitX->integer() ));
			value->addItem ( vtype2.get() );
			vtype2.release();
			aCont->addItem ( value.release() );

			}
		else if ( dataX != NULL && dataX->type() == IAlfVariantType::EReal )
			{
			auto_ptr<AlfContainer> value ( new ( EMM ) AlfContainer() );
			auto_ptr<AlfVariantType> vtype1(new ( EMM ) AlfVariantType ( dataX->real() ));
			value->addItem ( vtype1.get() );
			vtype1.release();
			auto_ptr<AlfVariantType> vtype2( new ( EMM ) AlfVariantType ( aAttrib->unit() ) );
			
			value->addItem (vtype2.get());
			vtype2.release();
			aCont->addItem ( value.release() );
			}
		}
	else
		{
		auto_ptr<AlfContainer> value ( new ( EMM ) AlfContainer() );
		auto_ptr<AlfVariantType> vtype1( new ( EMM ) AlfVariantType ( aAttrib->realValue() ) );
		value->addItem (vtype1.get());
		vtype1.release();
		auto_ptr<AlfVariantType> vtype2( new ( EMM ) AlfVariantType ( aAttrib->unit() ) );
		
		value->addItem (vtype2.get());
		vtype2.release();
		aCont->addItem ( value.release() );

		}

	}

void addToTextstylecont ( AlfContainer* /*aCont*/, AlfAttribute* /*aAttrib*/, IAlfMap* /*aData*/ )
	{
	}

struct anchormap
	{
	const char* name;
	pFunc addToContainer;
	AlfContainer* pData;
	};

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAnchorLayoutAttributeSetter::AlfAnchorLayoutAttributeSetter()
	{
	}

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAnchorLayoutAttributeSetter::~AlfAnchorLayoutAttributeSetter()
	{
	}
	
// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAnchorLayoutAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
	{
	
	CAlfAnchorLayout* layout = dynamic_cast<CAlfAnchorLayout*> ( &aVisual );

	if ( !layout )
		{
		ALF_THROW ( AlfVisualException, EInvalidVisual, 
		    "AlfAnchorLayoutAttributeSetter" )
		}
		
    AlfCommonLayoutAttributeSetter::setAttributeValue(aVisual, 
        aContainer, aData);
	}
	

// ---------------------------------------------------------------------------
// Deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfAnchorLayoutAttributeSetter::createCommand ( CAlfVisual& /*aVisual*/,
        AlfAttributeContainer* /*aContainer*/, IAlfMap* /*aData*/,
        int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/ )
	{
	//deprecated
	TAlfCommand* cmd = NULL;
	return cmd;
	}

// ---------------------------------------------------------------------------
// Sends a command to Env
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAnchorLayoutAttributeSetter::createAndSendCommands (
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
	{
	//no animatable attributes in grid layout.
	AlfCommonLayoutAttributeSetter::createAndSendCommands ( aVisual, aContainer, aRefVisual );
	}

// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual
// ---------------------------------------------------------------------------
//
void AlfAnchorLayoutAttributeSetter::handleDynamicAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer )
	{
	//no dynamic attributes in anchor layout.
	AlfCommonLayoutAttributeSetter::handleDynamicAttribute(aVisual, aAttr, aContainer);
	}
	
// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//		
void AlfAnchorLayoutAttributeSetter::handleStaticAttribute ( 
    CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer )
    {
    if (!doHandleStaticAttribute(aVisual, aAttr, aContainer, NULL))
        {
        AlfCommonLayoutAttributeSetter::handleStaticAttribute(aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//
bool AlfAnchorLayoutAttributeSetter::doHandleStaticAttribute( CAlfVisual &aVisual, 
    AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData )
	{
    bool handled = true;
	const char* attrName = aAttr.name();
	CAlfAnchorLayout* layout = dynamic_cast<CAlfAnchorLayout*> ( &aVisual );
	if(!layout)
	    {
	    return false;
	    }

	if ( !strcmp ( attrName, KChildOrdinal )||!strcmp ( attrName, KAttachmentOrdinal ) 
			|| !strcmp ( attrName, KAnchorType )||!strcmp ( attrName, KAttachmentOrigin )
			||!strcmp ( attrName, KMagnitude )||!strcmp ( attrName, KTextStyleId ) )
		{
		AlfAttribute& attachmentordinal = aContainer.getAttributeByName ( KAttachmentOrdinal );
		AlfAttribute& aAttrChildOrdinal = aContainer.getAttributeByName ( KChildOrdinal );
		float ordianal=0;
		float attachordinalval=-1;
		if(aData)
			{
			IAlfVariantType* data = aData->item ( UString(aAttrChildOrdinal.getDataField()) );

			if ( data && data->type() == IAlfVariantType::EReal )
				{
					ordianal=data->real();	
				}
			data = aData->item ( UString(attachmentordinal.getDataField()) );
			if ( data && data->type() == IAlfVariantType::EReal )
				{
					attachordinalval=data->real();	
				}
			}
		else
			{
			ordianal=aAttrChildOrdinal.realValue();
			attachordinalval=attachmentordinal.realValue();
			}
		auto_ptr<AlfContainer> anchortypecont ( new ( EMM ) AlfContainer() );
		auto_ptr<AlfContainer> attachmentorigincont ( new ( EMM ) AlfContainer() );
		auto_ptr<AlfContainer> magnitudecont ( new ( EMM ) AlfContainer() );
		auto_ptr<AlfContainer> textstyleidcont ( new ( EMM ) AlfContainer() );

		anchormap maptable[] =
			{
			{KAnchorType, addToAnchortypecont, NULL},
			{KAttachmentOrigin, addToAttachmentcont, NULL},
			{KMagnitude, addToMagnitudecont, NULL},
			{KTextStyleId, addToTextstylecont, NULL}
			};
		unsigned int tableCount = sizeof ( maptable ) / sizeof ( anchormap );
		maptable[0].pData = anchortypecont.get();
		maptable[1].pData = attachmentorigincont.get();
		maptable[2].pData = magnitudecont.get();
		maptable[3].pData = textstyleidcont.get();

		unsigned int attrCount = aContainer.attributeCount();

		for ( int i = 0;i < attrCount;i++ )
			{
			for ( int j = 0;j < tableCount;j++ )
				{
				if ( !strcmp ( maptable[j].name, aContainer.getAttribute ( i ).name() ) )
					{
					maptable[j].addToContainer ( maptable[j].pData, &aContainer.getAttribute ( i ), aData );
					aContainer.getAttribute ( i ).setDirty(false);
					}
				}
			}

		unsigned int totalApitocall = attachmentorigincont->count();

		unsigned int magnitudeindex = 0;

		for ( int counter = 0;counter < totalApitocall;counter++ )
			{
			IAlfVariantType* Origin = attachmentorigincont->item ( counter );
			TAlfAnchorAttachmentOrigin value = static_cast<TAlfAnchorAttachmentOrigin> ( Origin->integer() );

			if ( isMultivalueDataAchment ( value ) )
				{
				IAlfVariantType* aType = anchortypecont->item ( counter );
				TAlfAnchorType type = static_cast<TAlfAnchorType> ( aType->integer() );

				if ( type == EAlfAnchorTypeNone )
					{
					IAlfContainer* magCont1 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex ) );
					IAlfContainer* magCont2 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex + 1 ) );
					IAlfContainer* magCont3 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex + 2 ) );
					IAlfContainer* magCont4 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex + 3 ) );

					TAlfMetric metric1 ( magCont1->item ( 0 )->real(), magCont1->item ( 1 )->integer() );
					TAlfMetric metric2 ( magCont2->item ( 0 )->real(), magCont2->item ( 1 )->integer() );
					TAlfMetric metric3 ( magCont3->item ( 0 )->real(), magCont3->item ( 1 )->integer() );
					TAlfMetric metric4 ( magCont4->item ( 0 )->real(), magCont4->item ( 1 )->integer() );
					TAlfBoxMetric boxmetric ( metric1, metric2, metric3, metric4 );
					int err=layout->Attach ( ordianal, boxmetric, value, attachordinalval );
					if(err!=KErrNone)
						{
						ALF_THROW ( AlfVisualException, err, "Anchor Attach Failed" )
						}
					magnitudeindex += 4;
					}
				else
					{
					IAlfContainer* magCont1 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex ) );
					IAlfContainer* magCont2 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex + 1 ) );

					TAlfMetric metric1 ( magCont1->item ( 0 )->real(), magCont1->item ( 1 )->integer() );
					TAlfMetric metric2 ( magCont2->item ( 0 )->real(), magCont2->item ( 1 )->integer() );



					TAlfXYMetric xymetric ( metric1, metric2 );
				
					int err=layout->Attach ( ordianal, type, xymetric, value, attachordinalval );
					if(err!=KErrNone)
						{
						ALF_THROW ( AlfVisualException, err, "Anchor Attach Failed" )
						}

					magnitudeindex += 2;
					}
				}
			else
				{
				IAlfVariantType* typeVar = anchortypecont->item ( counter );
				TAlfAnchorType type = static_cast<TAlfAnchorType> ( typeVar->integer() );
				IAlfContainer* magCont1 = static_cast<IAlfContainer*> ( magnitudecont->item ( magnitudeindex ) );

				TAlfMetric metric ( magCont1->item ( 0 )->real(), magCont1->item ( 1 )->integer() );


				int err=layout->Attach ( ordianal, type, metric, value, attachordinalval );
				if(err!=KErrNone)
					{
					ALF_THROW ( AlfVisualException, err, "Anchor Attach Failed" )
					}

				magnitudeindex++;
				}
			}
			
		attachmentordinal.setDirty(false);
		aAttrChildOrdinal.setDirty(false);
		}
	else 
		{
		handled = false;
		}
		
    return handled;
	}


// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual from data
// ---------------------------------------------------------------------------
//
void AlfAnchorLayoutAttributeSetter::handleDynamicDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData )
	{
	AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute ( aVisual, aAttr, aContainer, aData );
	}

// ---------------------------------------------------------------------------
// Sets static attributes to visual  from data
// ---------------------------------------------------------------------------
//
void AlfAnchorLayoutAttributeSetter::handleStaticDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData )
	{
	if (!doHandleStaticAttribute(aVisual, aAttr, aContainer, aData))
	    {
	    AlfCommonLayoutAttributeSetter::handleStaticDataAttribute(aVisual, aAttr, aContainer, aData);
	    }
	}

	} // namespace Alf

// End of file





