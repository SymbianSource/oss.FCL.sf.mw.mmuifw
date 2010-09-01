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
* Description:  Header for  Pull Mode
 *
*/


#ifndef MULPULLMODE_H
#define MULPULLMODE_H

//Includes
#include "mulpushmode.h"

namespace Alf
    {
    
//Forward Declaration

class IMulModelProvider;
class MulAsyncDataProvider;

/**
 *  Implementation for Pull Mode
 */
class MulPullMode : public MulPushMode
    {
    
public:
	
	/**
	 * C++ constructor.
	 * 
	 * @param aDataWindow DataWindow for model
	 * @param aModelProvider Data Provider for model
	 */
    MulPullMode( MulPageDataWindow& aDataWindow,
				 const IMulModelProvider& aModelProvider);
	
	/**
	 * Destructor.
	 */
	~MulPullMode();
    
public: //from MulPushMode
   	
	
	void Update( int aIndex, int aCount, 
                         const MulDataPath& aPath);
            
	void Refresh(int aCount, const MulDataPath& aPath);				             
	     
	void SetData( int aIndex, auto_ptr<MulVisualItem> aVisualItem, 
                         const MulDataPath& aPath);

	void SetTemplate( mulwidget::TLogicalTemplate aTemplateId );               
	                    
	void WindowSiftedDown();

	void WindowSiftedUp();	
	
	void ModelCountChanged(int aOldCount, int aNewCount);
	
	void DataWindowUpdated();
	
	const MulVisualItem& Item( int aAbsoluteIndex );
	                    	
private:

	/**
	 * Create list of indices and call ProvideData method
	 *
	 * @param aStartIndex, Index form where the data should be fetched
	 * @param aRange, Number of items for which data should be fetched
	 */
	void ProvideData(int aStartIndex, int aRange);
	
	/**
	 * Update Loop buffer because of data window updation
	 * 
	 */
	void UpdateLoopBuffer();
	  				
private: //data

	//IMulModelProvider* mModelProvider; // Not own
	auto_ptr<MulAsyncDataProvider> mModelProvider; // own
    };
    
    } // namespace Alf

#endif // MULPULLMODE_H
    
//End of file
    
