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


#include <mul/muldatapath.h>

#include <vector>
#include <osn/osnnew.h>
#include <osn/ustring.h>
#include <algorithm> //for equal algorithm

namespace Alf
    {
    
class MulDataPathImpl
	{
	
public:

     MulDataPathImpl():mIndex(-1)
         {
         }
     
	~MulDataPathImpl()
		{
		mPath.clear();
		}
	
public:
	std::vector<int> mPath; //store index of node for each depth
	int mIndex;
	};

// ---------------------------------------------------------------------------
// MulDataPath
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulDataPath::MulDataPath()
	{
	}

// ---------------------------------------------------------------------------
// MulDataPath
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulDataPath::MulDataPath( int aIndexForDepth1 )
	{
	Append( aIndexForDepth1 );
	}

// ---------------------------------------------------------------------------
// MulDataPath
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulDataPath::MulDataPath( int aIndexForDepth1, int aIndexForDepth2)
	{
	Append( aIndexForDepth1 );
	Append( aIndexForDepth2 );
	}

// ---------------------------------------------------------------------------
// MulDataPath
// ---------------------------------------------------------------------------
//	
OSN_EXPORT MulDataPath::MulDataPath( const MulDataPath& aPath )
	{
	if(aPath.mData.get())
		{
		SetIndex( aPath.Index() );
		for( int i = 0 ; i < aPath.Depth() ; ++i )
			{
			Append( aPath[i] );
			}
		}
	}
	
// ---------------------------------------------------------------------------
// ~MulDataPath
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulDataPath::~MulDataPath()
	{
	if( mData.get() )
		{
		mData->mPath.clear();
		}
	}
	
// ---------------------------------------------------------------------------
// Append
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulDataPath::Append(int aIndex)
	{
	if( !mData.get() )
		{
		mData.reset( new (EMM) MulDataPathImpl() );
		}
	mData->mPath.push_back(aIndex);
	}

// ---------------------------------------------------------------------------
// Depth
// ---------------------------------------------------------------------------
//
OSN_EXPORT int MulDataPath::Depth() const
	{
	if( mData.get() )
		{
		return 	mData->mPath.size();
		}
	else
		{
		return -1;
		}
	}
	
// ---------------------------------------------------------------------------
// operator[]
// ---------------------------------------------------------------------------
//	
OSN_EXPORT int MulDataPath::operator[]( int aDepth ) const
	{
	if( mData.get() )
		{
		return 	(mData->mPath)[aDepth];
		}
	else
		{
		return -1;
		}
	}

// ---------------------------------------------------------------------------
// operator[]
// ---------------------------------------------------------------------------
//
bool MulDataPath::operator< ( MulDataPath& aPath)
	{
	bool result(false);
	
    //i dont wanted to add Index to original path because it is not part of path
    //but index is quite necessary for comparition so creating temp vector 
	//and copying path to temp vector and also appending index to path
	std::vector<int> v1 = mData->mPath;
	v1.push_back(Index());
	
	std::vector<int> v2 = aPath.mData->mPath;
	v2.push_back(aPath.Index());
	
	result = lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end());
	
	return result;
	}

// ---------------------------------------------------------------------------
// Index
// ---------------------------------------------------------------------------
//
int MulDataPath::Index() const
	{
	if(mData.get())
		{
		return mData->mIndex;
		}
	return -1;
	}

// ---------------------------------------------------------------------------
// SetIndex
// ---------------------------------------------------------------------------
//
void MulDataPath::SetIndex(int aIndex)
	{
	if( !mData.get() )
		{
		mData.reset( new (EMM) MulDataPathImpl() );
		}
	mData->mIndex = aIndex;
	}

// ---------------------------------------------------------------------------
// IsEqual
// ---------------------------------------------------------------------------
//
bool MulDataPath::IsEqual( const MulDataPath& aPath ) 
	{
	if( (*this) == aPath )
		{
		if( Depth() == -1 && aPath.Depth() == -1 )
			{
			return true;
			}
		else
			{
			return Index() == aPath.Index();
			}
		}
	else
		{
		return false;
		}
	}

// ---------------------------------------------------------------------------
// Operator ==
// ---------------------------------------------------------------------------
//
bool MulDataPath::operator == ( const MulDataPath& aPath) 
	{
	#ifdef _DEBUG
	int depth1 = Depth();
	int depth2 = aPath.Depth();
	#endif
	
	//if depth is -1 , it mean that it points to root node
	if( Depth() == -1 && aPath.Depth() == -1 )
		{
		return true;
		}
	else if((Depth() == -1 && aPath.Depth() == 0 ) || (Depth() == 0 && aPath.Depth() == -1 ))
		{
		return true;	
		}
	else if( Depth() != aPath.Depth() )
		{
		return false;
		}
	else
		{
		return equal( mData->mPath.begin(), mData->mPath.end(), aPath.mData->mPath.begin());
		}
	}

// ---------------------------------------------------------------------------
// Operator =
// ---------------------------------------------------------------------------
//
void MulDataPath::operator= ( MulDataPath aPath )	
	{
	if( aPath.mData.get() )	
		{
		if( mData.get() )
			{
			mData->mPath.clear();		
			}
		
		SetIndex( aPath.Index() );
		for( int i = 0 ; i < aPath.Depth() ; ++i )
			{
			Append( aPath[i] );
			}		
		}
	}
	
	} //namespace Alf

//End of file
