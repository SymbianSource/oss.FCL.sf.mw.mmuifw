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
* Description:  Header for MulDataPath
*
*/


#ifndef MULDATAPATH_H
#define MULDATAPATH_H

#include <memory>
#include <osn/osndefines.h>

namespace osncore
    {
    class UString;  
    }
using namespace osncore;

namespace Alf
    {
    
//Forward Declaration
class MulDataPathImpl; //store data for MulDataPath class    
/**
 * @class MulDataPath
 * 
 * MulDataPath represent hierarchy in data model.
 * It stores index of node for each depth.
 */
class MulDataPath
	{
public:
	/**
	 * Default constructor
	 * 
	 * By default path will point to root node.
	 */
	OSN_IMPORT MulDataPath();
	
	/**
	 * Constrctor will create path with depth 1 and store specified index
	 * at depth1
	 * 
	 * @param aIndexForDepth1 Index of node at depth 1
	 */
	OSN_IMPORT MulDataPath( int aIndexForDepth1 );
	
	/**
	 * Constrctor will create path with depth 2 and store specified index
	 * at depth 1 and depth 2
	 * 
	 * @param aIndexForDepth1 Index of node at depth 1
 	 * @param aIndexForDepth2 Index of node at depth 2
	 */
	OSN_IMPORT MulDataPath( int aIndexForDepth1, int aIndexForDepth2);
	
	/**
	 * Copy constructor
	 * 
	 *  @param aPath Existing path object
	 */
	OSN_IMPORT MulDataPath(const MulDataPath& aPath);
			
	/**
	 * Destructor
	 */
	OSN_IMPORT ~MulDataPath();
			
	/**
	 * Create new depth and add specified index at created depth.
	 * 
	 * @param aIndex position of node at current depth
	 */
	OSN_IMPORT void Append(int aIndex);
		
	/**
	 * Depth of path
	 * 
	 * @return Depth
	 */
	OSN_IMPORT int Depth() const;
		
	/**
	 * Return position/index of node at given depth
	 * 
	 * @param aDepth Depth of path
	 * @return Position/index of node
	 */
	OSN_IMPORT int operator[]( int aDepth ) const;
	
	/**
	 * Comparisition operator for Path.
	 * Comapre that path is less that other path or not
	 * 
	 * @param aPath path to wich current path need to be compared
	 * @return true - if is less ,false otherwise
	 */
	bool operator<( MulDataPath& aPath);
	
	/**
	 * return position in current depth. For internal use only
	 * 
	 * @param position in current depth
	 */
	int Index() const;
	
	/**
	 * Set internal position in current depth
	 * 
	 * @param aIndex position in current depth
	 */
	void SetIndex( int aIndex );
	
	/**
	 * Check whather both path are equal or not,
	 *
	 * 
	 * @param aPath Path object to compare with
	 * @return true is equals, false other wise
	 */
	bool operator== ( const MulDataPath& aPath);
	
	/**
	 * Check whather both path are equal or not,For internal use only
	 * 
	 * @param aPath Path object to compare with
	 * @return true is equals, false other wise
	 */
	bool IsEqual( const MulDataPath& aPath );
	
	
	/**
	 * Assignment operator 
	 *
	 * @param aPath Existing path
	 */
	void operator= ( MulDataPath aPath );
				
				
private: //data

	std::auto_ptr<MulDataPathImpl> mData; //own
	};

	} //namespace Alf

#endif //MULDATAPATH_H

//End of file
