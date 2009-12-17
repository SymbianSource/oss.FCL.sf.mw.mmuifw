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
* Description:  MulVarianttype implementation
*
*/


#include <mul/mulvarianttype.h>

#include <mul/imulvarianttype.h>
#include <osn/osnnew.h>

#include "mulassert.h"

using namespace osncore;
using namespace std;

namespace Alf
    {
    
_LIT( KUnknownDataType, "Unknown data type" );

// ---------------------------------------------------------------------------
// Defination of class MulVariantTypeImpl
// ---------------------------------------------------------------------------
//

class MulVariantType::MulVariantTypeImpl
    {     
public:
    
    ~MulVariantTypeImpl()
        {
        if( mDataType == IMulVariantType::EDes )
            {
            if( mData.mDes )
                {
                delete mData.mDes;
                }
            }
        else if( mDataType == IMulVariantType::EMap )
            {
            if( mData.mMap )
                {
                delete mData.mMap;
                }
            }
      
        }
    
    MulVariantTypeImpl()
        {
        mData.mDes = NULL;
        mData.mMap = NULL;
        mDataType = IMulVariantType::EUnknown;
        }
    
public:
    
    union TData
        {
        bool     mBool;  // Boolean
        int      mInt;   // 32-bit integer
        uint     mUint;  // 32-bit unsigned integer
        double   mReal;  // 64-bit real
        HBufC*   mDes;   // 16 bit descriptor 
        IAlfMap* mMap;
        };
    
    TData mData;
    IMulVariantType::TMulType mDataType;
    
    };

void MulVariantType::Construct( TMulType aType )
    {
    mVarData.reset( new (EMM) MulVariantTypeImpl() );
    mVarData->mDataType = aType;
    }

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType()	
	{
	Construct(IMulVariantType::EUnknown );
	// No implementation required
	}

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType( const TDesC& aDes )
    {
    Construct(IMulVariantType::EDes );
    mVarData->mData.mDes = aDes.AllocL();
    }

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType(const int& aValue)
	{
	Construct(IMulVariantType::EInt );
	mVarData->mData.mInt = aValue;
	}

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType(const uint& aValue)	
	{
	Construct(IMulVariantType::EUint );
	mVarData->mData.mUint = aValue;
	}

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType(const double& aValue)	
	{
	Construct(IMulVariantType::EReal );
	mVarData->mData.mReal = aValue;
	}

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType(const bool& aValue)	
	{
	Construct(IMulVariantType::EBool );
	mVarData->mData.mBool = aValue;
	}

// ---------------------------------------------------------------------------
// MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::MulVariantType( IAlfMap& aData )  
    {
    Construct(IMulVariantType::EMap );
    mVarData->mData.mMap = aData.clone();
    }

// ---------------------------------------------------------------------------
// ~MulVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVariantType::~MulVariantType()    
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// MulType
// ---------------------------------------------------------------------------
//
OSN_EXPORT IMulVariantType::TMulType MulVariantType::Type() const
    {
    return mVarData->mDataType;
    }

// ---------------------------------------------------------------------------
// DesC
// ---------------------------------------------------------------------------
//
OSN_EXPORT const TDesC& MulVariantType::DesC() const
    {
    if ( Type() != IMulVariantType::EDes ) 
        {
        __MUL_ASSERT(false, KUnknownDataType );
        }
    return *mVarData->mData.mDes;
    }
	
// ---------------------------------------------------------------------------
// Clone
// ---------------------------------------------------------------------------
//
OSN_EXPORT std::auto_ptr< IMulVariantType > MulVariantType::Clone()
	{
	std::auto_ptr<IMulVariantType> clone;
	
	switch( Type() )
		{
		case IMulVariantType::EBool:
		    {
	        clone.reset( new (EMM) MulVariantType( boolean() ) );
	        return clone;
		    }
		case IMulVariantType::EInt:
		    {
		    clone.reset( new (EMM) MulVariantType( integer() ) ) ;
		    return clone;
		    }
		case IMulVariantType::EUint:
		    {
		    clone.reset( new (EMM) MulVariantType( uinteger() ) );
		    return clone;
		    }
		case IMulVariantType::EReal:
		    {
		    clone.reset( new (EMM) MulVariantType( real() ) );
		    return clone;
		    }
		case IMulVariantType::EDes:
			{
			clone.reset( new (EMM) MulVariantType( DesC() ) ) ;
			return clone;
			}
		case IAlfVariantType::EMap:
		    {
		    clone.reset( new (EMM) MulVariantType( Map() ) ) ;
		    return clone;
		    }
		case IAlfVariantType::ECustomData: //delebrate fallthrough
		default :
			{
			__MUL_ASSERT( false, KUnknownDataType );
			return clone;
			}
		}
	}

// ---------------------------------------------------------------------------
// boolean
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool MulVariantType::boolean() const
	{
    if ( Type() != IMulVariantType::EBool ) 
        {
        __MUL_ASSERT( false, KUnknownDataType );
        }
    return mVarData->mData.mBool;
	}

// ---------------------------------------------------------------------------
// integer
// ---------------------------------------------------------------------------
//
OSN_EXPORT int MulVariantType::integer() const
	{
	if ( Type() != IMulVariantType::EInt ) 
       {
       __MUL_ASSERT( false, KUnknownDataType );
       }
    return mVarData->mData.mInt;
	}

// ---------------------------------------------------------------------------
// uinteger
// ---------------------------------------------------------------------------
//
OSN_EXPORT uint MulVariantType::uinteger() const
	{
	if ( Type() != IMulVariantType::EUint ) 
       {
       __MUL_ASSERT( false, KUnknownDataType );
       }
	return mVarData->mData.mUint;
	}

// ---------------------------------------------------------------------------
// real
// ---------------------------------------------------------------------------
//
OSN_EXPORT double MulVariantType::real() const
	{
	if ( Type() != IMulVariantType::EUint ) 
       {
       __MUL_ASSERT( false, KUnknownDataType );
       }
	return mVarData->mData.mReal;
	}

// ---------------------------------------------------------------------------
// map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap& MulVariantType::Map() const
	{
	if ( Type() != IMulVariantType::EMap ) 
       {
       __MUL_ASSERT( false, KUnknownDataType );
       }
	return *mVarData->mData.mMap;
	}

    }// namespace Alf	

//End of file
