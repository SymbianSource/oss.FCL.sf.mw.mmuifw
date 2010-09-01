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
* Description:  CoverFlow templates Implementation
 *
*/

//  Include Files

// Class Headers 
#include "mulcoverflowcontrol.h"

// DUI headers for xml parser
#include <uimodel/duinode.h>
#include <dui/duiproperty.h>
#include <uimodel/duixmlattributelist.h>

// Local Headers
#include "mulutility.h"
#include "mulbaseelement.h"
#include "mulcoverflowenhancedtemplate.h"
#include "mulcoverflowdefinitions.h"     //defines all the strings 
#include "mullog.h"

using namespace duiuimodel;

namespace Alf
    {
	
struct TMulCoverFlowEnhancedTemplateImpl
    {
    
    TMulCoverFlowEnhancedTemplateImpl()
    	{
    	}

    //Structure used to store the position and sizes of the counter.
    TMulCoverFlowItem mEnhancedMinSize;  // minimum size of images in enhanced mode
    TMulCoverFlowItem mEnhancedMaxSize;  // maximum size of images in enhanced mode
    TMulCoverFlowItem mEnhacedCounterPos; // The position and size of the counter in enhaced mode
    UString mEnhancedBorderImagePath;
    
    ~TMulCoverFlowEnhancedTemplateImpl()
    	{
    	}    
    };

// ---------------------------------------------------------------------------
// MulCoverFlowEnhancedTemplate
// ---------------------------------------------------------------------------
//
MulCoverFlowEnhancedTemplate::MulCoverFlowEnhancedTemplate(MulCoverFlowControl &aControl,
			UString &aTemplateID, MulXmlUtility& aMulXmlUtility,DuiNode* aRootNode):MulCoverFlowBaseTemplate( 
				    aControl, aTemplateID, aMulXmlUtility,aRootNode )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::MulCoverFlowEnhancedTemplate");
    mData.reset(new (EMM)TMulCoverFlowEnhancedTemplateImpl);
    
    // Parses the xml from the oreientation node.
    ParseNode( *GetOrientationNode() );
    }
    
// ---------------------------------------------------------------------------
// ~MulCoverFlowWidget
// ---------------------------------------------------------------------------
//
MulCoverFlowEnhancedTemplate::~MulCoverFlowEnhancedTemplate()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::~MulCoverFlowEnhancedTemplate");       
    //Nothing to delete .
    }

// ---------------------------------------------------------------------------
// EnhancedMaxSize
// ---------------------------------------------------------------------------
//
const TMulCoverFlowItem& MulCoverFlowEnhancedTemplate::EnhancedMaxSize()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::EnhancedMaxSize");
    return mData->mEnhancedMaxSize;
    }

// ---------------------------------------------------------------------------
// EnhancedMinSize
// ---------------------------------------------------------------------------
//
const TMulCoverFlowItem& MulCoverFlowEnhancedTemplate::EnhancedMinSize()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::EnhancedMinSize");
    return mData->mEnhancedMinSize;
    }

// ---------------------------------------------------------------------------
// EnhacedCounterPos
// ---------------------------------------------------------------------------
//
const TMulCoverFlowItem& MulCoverFlowEnhancedTemplate::EnhacedCounterPos()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::EnhacedCounterPos");
    return mData->mEnhacedCounterPos;
    }
    
// ---------------------------------------------------------------------------
// EnhancedBorderImagePath
// ---------------------------------------------------------------------------
//
const UString& MulCoverFlowEnhancedTemplate::EnhancedBorderImagePath()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::EnhancedBorderImagePath");
    return mData->mEnhancedBorderImagePath; 
    }

// ---------------------------------------------------------------------------
// ParseNode
// ---------------------------------------------------------------------------
//
void MulCoverFlowEnhancedTemplate::ParseNode( DuiNode& aNode )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::ParseNode");
    int nChildCount = aNode.childCount ();
	for (int index = 0; index < nChildCount; index++)
		{
		DuiNode& item = aNode.child (index);
		
        //parse the elements 
		if ( !item.type().compare(KElement) )
			{
		    if( !item.nodeId().compare(KEnhancedSize) )
				{
				ParseEnhancedTag( item );
				}
			else if( !item.nodeId().compare(KCounter) )
				{
				ParseCounterTag( item );	
				}
			}
		}
	}

// ---------------------------------------------------------------------------
// ParseEnhancedTag
// ---------------------------------------------------------------------------
//
void MulCoverFlowEnhancedTemplate::ParseEnhancedTag( DuiNode& aNode )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::ParseEnhancedTag");
	int totalitems = aNode.childCount();
	for( int index = 0; index < totalitems; index++ )
		{
	     DuiNode& childItem = aNode.child(index);
		 if( !childItem.type().compare(KSize) )
		 	{
		 	// for enhanced mode xmls.
		 	ProcessSizeTag( childItem );
		 	} 
		 else
		 	{
			bool pathFound = GetStaticImagePath(childItem, mData->mEnhancedBorderImagePath);
			TPtrC8 src((TUint8*)mData->mEnhancedBorderImagePath.getUtf8());
			}
		}
	}
  
// ---------------------------------------------------------------------------
// ParseCounterTag
// ---------------------------------------------------------------------------
//
void MulCoverFlowEnhancedTemplate::ParseCounterTag( DuiNode& aNode )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::ParseCounterTag");
	int totalItems = aNode.childCount();
	for( int index = 0; index < totalItems; index++ )
		{
	     DuiNode& childItem = aNode.child(index);
		 if( !childItem.type().compare(KSize) )
		 	{
		 	// for enhanced mode xmls.
		 	ProcessSizeTag( childItem );
		 	} 
		}
	}
	    
// ---------------------------------------------------------------------------
// ProcessSizeTag
// ---------------------------------------------------------------------------
//   				
void MulCoverFlowEnhancedTemplate::ProcessSizeTag( DuiNode& aNode )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowEnhancedTemplate::ProcessSizeTag");
	int totalitems = aNode.childCount();

	auto_ptr<TMulCoverFlowItem> itemstruct = ProcessItemPos( aNode );
	if( !aNode.nodeId().compare(KMinSize) )
		{
		// Store the maximum and minimum sizes for enhanced mode.
		mData->mEnhancedMinSize = *itemstruct;
	 	}
 	else if( !aNode.nodeId().compare(KMaxSize) )
		{
		// Store the maximum and minimum sizes for enhanced mode.
	    mData->mEnhancedMaxSize = *itemstruct;
	 	}
	else if( !aNode.nodeId().compare(KCounterSize) )
		{
		// Store the position and sizes for the counter in enhanced mode.
	    mData->mEnhacedCounterPos = *itemstruct;
	 	}
 	//delete itemstruct;
	}
//-----------------------------------------------------------------------------
// GetStaticImagePath
//-----------------------------------------------------------------------------
//
bool MulCoverFlowEnhancedTemplate::GetStaticImagePath(DuiNode& aNode, UString& aPath)
	{
	 MUL_LOG_ENTRY_EXIT("aakash::MulXmlTemplate::GetStaticImagePath");
	 
	int aAttrSetterCount = aNode.childCount ();
	
	for (int index = 0; index < aAttrSetterCount; index++)
	    {
	    DuiNode& attrSetterNode = aNode.child (index);
		const UString& aNodeType = attrSetterNode.type ();
		
		DuiXmlAttributeList& attSetterList = attrSetterNode.attributeList ();
		const UString& attrName = attSetterList.attributeValue (UString (attributesetter::KName));
		const UString& category = attSetterList.attributeValue (UString (KCategory));
		
	    if ( aNodeType == nodetypes::KAttributeSetter )
		    {
		    if ( attrName == duiuimodel::imagevisualattributes::KImagePath && category == attributesetter::typevalues::KStatic )
		    	{
		    	aPath = attSetterList.attributeValue(UString(attributesetter::KTargetValue));
		    	MulUtility::AbsolutePath(aPath);
		    	return true;
		    	}
		    }
	    }
	return false;
	}
	
	}//namespace Alf

// End of file.

