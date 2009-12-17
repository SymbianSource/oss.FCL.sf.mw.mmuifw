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
* Description:  template header for CoverFlow
*
*/

// This file defines the API for MulCoverFlowWidget.dll

#ifndef MULCOVERFLOWENHANCEDTEMPLATE_H_
#define MULCOVERFLOWENHANCEDTEMPLATE_H_

#include "mulcoverflowbasetemplate.h"

namespace osncore
	{
// Forward declarations
class UString;
	}

namespace Alf
    {
// Forward declarations
class MulCoverFlowControl;
class MulXmlUtility;

struct TMulCoverFlowEnhancedTemplateImpl;
    
class MulCoverFlowEnhancedTemplate :public MulCoverFlowBaseTemplate
    {
    
public: // Constructor and Destructor 
        
    /**
     * Constructor
     */
	MulCoverFlowEnhancedTemplate(MulCoverFlowControl &aControl,UString &aTemplateID, MulXmlUtility& aMulXmlUtility ,DuiNode* aRootNode);
    
    /**
     * Destructor
     */  
    ~MulCoverFlowEnhancedTemplate();

public: // New Methods
    
    /**
     * Returns the enhanced max size in TMulCoverFlowItem structure. 
     * 
     * @return Returns reference to TMulCoverFlowItem structure. 
     */
    const TMulCoverFlowItem& EnhancedMaxSize();
   
    /**
     * Returns the enhanced min size in TMulCoverFlowItem structure. 
     * 
     * @return Returns reference to TMulCoverFlowItem structure.
     */
    const TMulCoverFlowItem& EnhancedMinSize();
    
    /**
     * Returns the counter position and size in TMulCoverFlowItem structure. 
     * 
     * @return Returns reference to TMulCoverFlowItem structure.
     */
    const TMulCoverFlowItem& EnhacedCounterPos();
 
    /**
     * Returns the path of the image that should be used as ecf shadow border. 
     * 
     * @return Returns path of the image that should be used as ecf shadow border.
     */   
    const UString& EnhancedBorderImagePath();
       
private:
        
    /**
     * Parses xml tags of type <element> and in turn creates AlfElements 
     * which in turn are added in the control.
     *
     * All tags with <element> will be parsed here
     *
     * @param aNode the orientation node depending on the orientation
     * being used . 
     */
    void ParseNode( DuiNode& aNode );
    
    /**
     * Parses the enhanced size tag in the xml .
     * 
     * @param aNode the node at which the tag is <enhancedsize>.
     */
    void ParseEnhancedTag( DuiNode& aNode );

    /**
     * Parses the counter size tag in the xml .
     * 
     * @param aNode the node at which the tag is <counter>.
     */
    void ParseCounterTag( DuiNode& aNode );
    	
    /**
     * Parses the sizes tag and stores those values in corresponding variables.
     * 
     * @param aNode the node at which sizes are to be parsed from.
     */
	void ProcessSizeTag( DuiNode& aNode );
	
    /**
     * Parses the imagepath tag and stores those values in corresponding variables.
     * 
     * @param aNode the node at which sizes are to be parsed from.
     * @param aPath, Reference to Ustring in which the parsed value should be stored
     */	
	bool GetStaticImagePath(DuiNode& aNode, UString& aPath);
	
private:
    auto_ptr<TMulCoverFlowEnhancedTemplateImpl> mData;    // owns it
    
    }; // class 
    
    }//namespace Alf

#endif /*MULCOVERFLOWENHANCEDTEMPLATE_H_*/

//End of file
