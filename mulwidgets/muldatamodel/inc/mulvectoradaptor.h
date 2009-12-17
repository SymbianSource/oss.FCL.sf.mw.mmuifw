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
* Description:  Adaptor class for AlfPtrVector and Stt vector
*
*/


#ifndef MULVECTORADAPTOR_H_
#define MULVECTORADAPTOR_H_

#include <vector>
#include <algorithm> //for find algorithm

using namespace std;

namespace Alf
	{
	
//Forward Declaration

/**
 * Adaptor class for stl vector with interface similar to AlfPtrVector
 */
template <class T> class MulVectorAdaptor 
    {         
public: //new method

	/**
	 * Return number of element in  vector
	 * 
	 * @return no of element in vector
	 */
	int count()
		{
		return mVector.size();
		}
	
	/**
	 * Insert item in vector
	 * 
	 * @param aPosition Position of item 
	 * @param aItem item to be inserted
	 */
	void insert( uint aPosition, T aItem )
		{		
		mVector.insert( mVector.begin() + aPosition, aItem );
		}
	
	/**
	 * Insert item in vector at last
	 * 
	 * @param aItem item to be inserted
	 */
	void insert( T aItem )
		{
		mVector.push_back( aItem );
		}
	
	/**
	 * Return item at specified index
	 * 
	 * @param aPosition position of item 
	 * @return itmem at specified index
	 */
	T operator[] ( uint aPosition ) const
		{
		return mVector[aPosition];
		}
	
	/**
	 * Remove item from specified position
	 * 
	 * @param aPosition Position of item to be removed
	 */
	void remove( uint aPosition  )
		{
		mVector.erase( mVector.begin() + aPosition );
		}
	
	/**
	 * Remove specified item
	 * 
	 * @param aItem Item to be removed
	 */
	void remove( T aItem )
		{
//		vector<T>::iterator iter = std::find( mVector.begin(), mVector.end(), aItem) ;
//		if( iter !=  mVector.end())
//			{
//			mVector.erase( iter );
//			}
		
		if(  std::find( mVector.begin(), mVector.end(), aItem) !=  mVector.end())
			{
			mVector.erase(  std::find( mVector.begin(), mVector.end(), aItem) );
			}
		}
	
	/**
	 * Return actual std vector of this adaptor class
	 * 
	 * @return Return std vector.
	 */
	const vector<T>& actualVector()
		{
		return mVector;
		}
	
	/**
	 * Find specifed item in vecotr
	 * 
	 * @return return true if item found false otherwise
	 */
	bool find( T aItem )
		{
//		vector<T>::iterator iter = std::find(mVector.begin(), mVector.end(), aItem) ;
//		return (iter != mVector.end());
		
		return (std::find(mVector.begin(), mVector.end(), aItem) != mVector.end());		
		}
	
	/**
	 * Clear the content of vector
	 */
	void clear()
		{
		mVector.clear();
		}
	
private: //data

	vector<T> mVector;	
    };
	    
    } //namespace Alf

#endif /*MULVECTORADAPTOR_H_*/

//End of file


