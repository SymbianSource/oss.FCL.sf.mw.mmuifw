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
* Description:  Utility class for data model
*
*/


//Includes
#include "mul/mulmodelutility.h"

#include <e32cmn.h>
#include <mul/mulvisualitem.h>
#include <mul/imulvarianttype.h>
#include <mul/mulvarianttype.h>
#include <stdexcept> // for standard exceptions
#include <osn/ustring.h>
#include <utf.h>

//#include "mulactionitem.h"

namespace Alf
    {
        
// ---------------------------------------------------------------------------
// CreateVariantType
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType* MulModelUtility::CreateVariantType( IAlfVariantType& aValue )
	{
/*	IAlfVariantType* data = new (EMM) AlfVariantType();
	data->set(aValue);
	return data;*/
	switch( aValue.type() )
		{
		case IAlfVariantType::EBool:
			{
			return new AlfVariantType( aValue.boolean() );
			}
		case IAlfVariantType::EInt:
			{
			return new AlfVariantType( aValue.integer() );
			}
		case IAlfVariantType::EUint:
			{
			return new AlfVariantType( aValue.uinteger() );
			}
		case IAlfVariantType::EReal:
			{
			return new AlfVariantType( aValue.real() );
			}
		case IAlfVariantType::EString:
			{
			return new AlfVariantType( aValue.string() );
			}
		case IAlfVariantType::EContainer:
			{
			return new AlfVariantType( aValue.container() );
			}
		case IAlfVariantType::EMap:
			{
			return new AlfVariantType( aValue.map() );
			}
		case IAlfVariantType::EBranch:
			{
			return new AlfVariantType( aValue.branch() );
			}
		case IAlfVariantType::ECustomData:
		default :
			{
			return NULL;
			}
		}
	}

// ---------------------------------------------------------------------------
// ConvertVisualItemToMap
// ---------------------------------------------------------------------------
//
OSN_EXPORT std::auto_ptr<IAlfMap> MulModelUtility::ConvertVisualItemToMap( MulVisualItem& /*aVisualItem*/ )
	{
	auto_ptr<IAlfMap> map( new ( EMM ) AlfMap());	
	
	return map;
	}

// ---------------------------------------------------------------------------
// ConvertMapToVisualItem
// ---------------------------------------------------------------------------
//
OSN_EXPORT std::auto_ptr<MulVisualItem> MulModelUtility::ConvertMapToVisualItem( IAlfMap& /*aMap*/ )
	{
	auto_ptr<MulVisualItem> visualItem( new ( EMM ) MulVisualItem());	
//	for( int i = 0 ; i < aMap.count() ; ++i )
//		{
//		const UString& attributeName = aMap.name(i);	
//		IAlfVariantType* origValue = aMap.item( attributeName );
//		std::auto_ptr< IMulVariantType> value ( new (EMM) MulVariantType() );
//		value->set(*origValue);
//		visualItem->SetAttribute( attributeName , value.get() );
//		value.release();
//		}
	return visualItem;
	}

// ---------------------------------------------------------------------------
// CheckNamePrefix
// ---------------------------------------------------------------------------
//
bool MulModelUtility::CheckNamePrefix( UString aName ) 
	{
	_LIT8(KMulPrefix,"mul_");
	TPtrC8 debugString1 = ((TUint8*)aName.getUtf8());
	int index = debugString1.Find(KMulPrefix);
	if( index == KErrNotFound )
		{
		return false;
		}
	else 
	    {
	    return true;
	    }
	}



// ---------------------------------------------------------------------------
// UStringToAlfString
// ---------------------------------------------------------------------------
//
OSN_EXPORT UString MulModelUtility::UStringToAlfString( const UString& aUString )
	{
	if(aUString.isEmpty())
		{
		return UString("");
		}
	else
		{
		return UString( aUString.getUtf8() );
		}
	}

// ---------------------------------------------------------------------------
// AlfStringToUString
// ---------------------------------------------------------------------------
//
OSN_EXPORT UString MulModelUtility::AlfStringToUString( const UString& aAlfString )
	{
	return UString( aAlfString.getUtf8() );
	}

// ---------------------------------------------------------------------------
// IntToAlfString
// ---------------------------------------------------------------------------
//
OSN_EXPORT UString MulModelUtility::IntToUString( int aNumber )
	{
	TBuf8<20> buf;
	buf.AppendNum(aNumber);
	return UString( (char*)buf.PtrZ());
	}
	
// ---------------------------------------------------------------------------
// ConvertTDesToUString
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulModelUtility::ConvertTDesToUString (const TDesC& aDes, UString& aString)
	{
	HBufC8* utf8Buffer( NULL );
    TRAPD( error, utf8Buffer = CnvUtfConverter::ConvertFromUnicodeToUtf8L( aDes ) );
    if ( !utf8Buffer || error != KErrNone )
        {
        delete utf8Buffer;
        throw std::exception();
        }
    
    TPtrC8 ptr = utf8Buffer->Des();
    std::auto_ptr<osncore::UString> ret;
    try
        {
        if ( ptr.Length() )
            {
            ret.reset( new osncore::UString( ( osncore::Utf8* ) &ptr[0], ptr.Length() ));
            }
        else
            {
            ret.reset( new osncore::UString( "" ) );
            }
        }
    catch ( std::exception e )
        {
        delete utf8Buffer;
        throw e;        
        }
    
    delete utf8Buffer;
    utf8Buffer = NULL;
    aString = UString(*ret);       
	}
	
// ---------------------------------------------------------------------------
// ConvertTDes8ToUString
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulModelUtility::ConvertTDes8ToUString (const TDesC8& aDes, UString& aString)
	{
    TPtrC8 ptr = aDes;
    std::auto_ptr<osncore::UString> ret;
    try
        {
        if ( ptr.Length() )
            {
            ret.reset( new osncore::UString( ( osncore::Utf8* ) &ptr[0], ptr.Length() ));
            }
        else
            {
            ret.reset( new osncore::UString( "" ) );
            }
        }
    catch ( std::exception e )
        {
        throw e;        
        }
    
    aString = UString(*ret);       
	}

// ---------------------------------------------------------------------------
// ConvertUStringToTDes
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulModelUtility::ConvertUStringToTDes (const UString& aString, TDes& aDes)
    {
    HBufC* unicodeBuffer = NULL;
    if ( !aString.isEmpty() )
        {
        TRAPD( error, unicodeBuffer = CnvUtfConverter::ConvertToUnicodeFromUtf8L(TPtrC8((TUint8*)aString.getUtf8())) );
        if ( !unicodeBuffer || error != KErrNone )
            {
            delete unicodeBuffer;
            throw std::exception();
            }
        aDes.Copy(*unicodeBuffer);
        delete unicodeBuffer;
        unicodeBuffer = NULL;
        }
    else
    	{
    	aDes.Zero(); 
    	}
    }
    
// ---------------------------------------------------------------------------
// ConvertUStringToTDes8
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulModelUtility::ConvertUStringToTDes8(const UString& aString, TDes8& aDes)
    {
    /// @see comments in MulModelUtility::ConvertUStringToTDes
    HBufC* unicodeBuffer = NULL;
    if ( !aString.isEmpty() )
        {
        TRAPD( error, unicodeBuffer = CnvUtfConverter::ConvertToUnicodeFromUtf8L(TPtrC8((TUint8*)aString.getUtf8())) );
        if ( !unicodeBuffer || error != KErrNone )
            {
            delete unicodeBuffer;
            throw std::exception();
            }
        aDes.Copy(*unicodeBuffer);
        delete unicodeBuffer;
        unicodeBuffer = NULL;
        }
    else
    	{
    	aDes.Zero(); 
    	}
    }

// ---------------------------------------------------------------------------
// CreateVisualItem
// ---------------------------------------------------------------------------
//    
OSN_EXPORT std::auto_ptr<MulVisualItem> MulModelUtility::CreateVisualItem( mulwidget::TLogicalTemplate aDefaultTemplate )
	{
	std::auto_ptr<MulVisualItem> visualItem( new (EMM) MulVisualItem() );
	visualItem->SetAttribute( mulvisualitem::KAttributeTemplate , aDefaultTemplate);
	return visualItem;
	}

} // namespace Alf
    
//End of file
