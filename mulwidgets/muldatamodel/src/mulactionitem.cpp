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
* Description:   Action item class implementation.
 *
*/


// includes
#include "mulactionitem.h"

#include <stdexcept>

namespace Alf
	{

static const char* const KNotImplemented = "Not Implemented";
static const char* const KInvalidArgument = "Invalid Parameter";

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//		
OSN_EXPORT MulActionItem::MulActionItem(TMulActionType aType, MulActionData* aData):
	mType(aType)	
	{       
	if( !aData )
	    {
	    throw std::invalid_argument(std::string(KInvalidArgument));
	    }
	mData = aData;    
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//		
OSN_EXPORT MulActionItem::MulActionItem( const MulActionItem& aActionItem )
	{
	mType = aActionItem.mType;
	mData.reset( new (EMM) MulActionData(*aActionItem.mData.get()));
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//		
OSN_EXPORT MulActionItem::~MulActionItem()
	{
	// No implemnetation.    	
	}

// ---------------------------------------------------------------------------
// SetActionType
// ---------------------------------------------------------------------------
//		
OSN_EXPORT void MulActionItem::SetActionType( TMulActionType aType)
	{
	mType = aType;		
	}

// ---------------------------------------------------------------------------
// ActionType
// ---------------------------------------------------------------------------
//		
OSN_EXPORT MulActionItem::TMulActionType MulActionItem::ActionType() const
	{
	return mType;
	}

// ---------------------------------------------------------------------------
// SetActionData
// ---------------------------------------------------------------------------
//		
OSN_EXPORT void MulActionItem::SetActionData( MulActionData* aData )
	{
	if( !aData )
	    {
	    throw std::invalid_argument(std::string(KInvalidArgument));    
	    }
	    
	mData = aData;
	}

// ---------------------------------------------------------------------------
// ActionData
// ---------------------------------------------------------------------------
//		
OSN_EXPORT const MulActionData& MulActionItem::ActionData() const
	{
	return *mData.get();
	}

// ---------------------------------------------------------------------------
// Type
// ---------------------------------------------------------------------------
//	
//const UString& MulActionItem::Type()
//	{
//	mActionString.reset(new UString(mulactionitem::KActionString));
//    return *mActionString.get();
//	}

// ---------------------------------------------------------------------------
// Clone
// ---------------------------------------------------------------------------
//
OSN_EXPORT std::auto_ptr< IMulVariantType > MulActionItem::Clone()
	{
	std::auto_ptr<MulActionData> actionData( new (EMM) MulActionData(*mData));
	std::auto_ptr<IMulVariantType> clone ( new (EMM) MulActionItem( ActionType(), actionData.get()));
	actionData.release();
	return clone;
	}

// ---------------------------------------------------------------------------
// set
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulActionItem::set(IAlfVariantType& /*aValue*/)
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// type
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType::Type MulActionItem::type() const
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// boolean
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool MulActionItem::boolean() const
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// integer
// ---------------------------------------------------------------------------
//
OSN_EXPORT int MulActionItem::integer() const
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// uinteger
// ---------------------------------------------------------------------------
//
OSN_EXPORT uint MulActionItem::uinteger() const
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// real
// ---------------------------------------------------------------------------
//
OSN_EXPORT double MulActionItem::real() const
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// string
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& MulActionItem::string() const 
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// container
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainer* MulActionItem::container() 
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap* MulActionItem::map() 
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// branch
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfBranch* MulActionItem::branch() 
	{
	throw std::logic_error(KNotImplemented);
	}

// ---------------------------------------------------------------------------
// customData
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfModelBase* MulActionItem::customData() 
	{
	throw std::logic_error(KNotImplemented);
	}
		
	} // namespace ends 
	
// End of file
