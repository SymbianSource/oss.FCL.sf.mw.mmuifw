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
* Description:  Description of various grid templates
 *
*/

 
#include "mulcoverlogicaltemplatebase.h"

namespace Alf
	{
	
	MulCoverLogicalTemplateBase::~MulCoverLogicalTemplateBase()
		{	   
	    for(int i =0; i < mTemplatePrtStruct.size(); ++i)
	        {
	        delete mTemplatePrtStruct[i];            
	        }
	        
	      for(int i =0; i < mTemplateLscStruct.size(); ++i)
	        {
	        delete mTemplateLscStruct[i];            
	        }      
		}
	//----------------------------------------------------------------
	// GetElementStructure
	//----------------------------------------------------------------
	// 
	const LCTElementStructure* MulCoverLogicalTemplateBase::GetElementStructure(
	    TMulCurrentOrientation aCurOrient,
	    const char* aElemId) 
	    {

	    LCTElementStructure* returnElem = NULL;
	    
	    if(aCurOrient == EMulPortrait)
	        {
	        
	        for (vector<LCTElementStructure*>::const_iterator itP = mTemplatePrtStruct.begin();
	             itP!=mTemplatePrtStruct.end(); ++itP) 
	            {
	            if(!strcmp(((*itP)->GetId()).getUtf8(),aElemId))
	                returnElem =  (*itP);
	            }  
	        }
	    else if(aCurOrient == EMulLandScape)
	        {
	        for (vector<LCTElementStructure*>::const_iterator itL = mTemplateLscStruct.begin();
	             itL!=mTemplateLscStruct.end(); ++itL) 
	            {
	            if(!strcmp(((*itL)->GetId()).getUtf8(),aElemId))
	                returnElem = (*itL);
	            }  
	        }
	    return returnElem;
	    
	    }
    
	void  MulCoverLogicalTemplateBase::PopulateData ()
		{
		}
	}// End of Alf

