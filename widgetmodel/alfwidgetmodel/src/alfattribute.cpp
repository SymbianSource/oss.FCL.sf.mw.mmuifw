/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation for creating different types of attributes.
*
*/


#include <osn/osnnew.h>
#include <osn/ustring.h>
#include <osn/alfptrvector.h>
#include "alf/alfattribute.h"
#include <alf/alfdataexception.h>
#include <stdexcept>
#include <alf/alftimedvalue.h>
#include <alf/alfattributeexception.h>

using namespace osncore;

namespace Alf
    {


class AlfAttributeImpl
    {
public:
    AlfAttributeImpl():mCategory(AlfAttribute::EStatic),
            mInterpolationStyle(EAlfTimedValueStyleUseSystemDefault),
            mTime(0),
            mDelay(0),
            mMappingFunctionId(0),
            mDirtyFlag(true)
        {
        }

    ~AlfAttributeImpl()
        {
        }

    UString mName;
    AlfAttribute::attributecategory mCategory;
    TAlfInterpolationStyle mInterpolationStyle;
    int mTime;
    int mDelay;
    int mMappingFunctionId;
    UString mDataField;
    AlfPtrVector<AlfAttributeValueType> mTargetValueContainer;
    AlfPtrVector<AlfAttributeValueType> mSourceValueContainer;
    bool mDirtyFlag;
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::AlfAttribute()
    {
    reset("", 0, attributecategory(0));
    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::AlfAttribute(const char* aName,
                                      attributecategory aCategory)
    {
    reset(aName, 0, aCategory);
    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::AlfAttribute(const char* aName, int aValue,
                                      TAlfUnit aUnit, attributecategory aCategory)
    {
    auto_ptr<AlfAttributeValueType> value(new (EMM)
                                          AlfAttributeValueType(aValue, aUnit));
    reset(aName, value.get(), aCategory);
    value.release();
    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::AlfAttribute(const char* aName, float aValue,
                                      TAlfUnit aUnit, attributecategory aCategory)
    {
    auto_ptr<AlfAttributeValueType> value(
        new( EMM ) AlfAttributeValueType( aValue, aUnit ) );
    reset(aName, value.get(), aCategory);
    value.release();
    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::AlfAttribute(const char* aName,
                                      const UString& aValue, attributecategory aCategory)
    {
    if (aCategory == EStaticData)
        {
        reset(aName, 0, aCategory);
        setDataField(aValue.getUtf8());
        }
    else
        {
        auto_ptr<AlfAttributeValueType> value(
            new( EMM ) AlfAttributeValueType( aValue ) );
        reset(aName, value.get(), aCategory);
        value.release();
        }
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::~AlfAttribute()
    {
    }

// ---------------------------------------------------------------------------
// Assigment operation.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute& AlfAttribute::operator=(const AlfAttribute& aAttribute)
    {
    if ( this == &aAttribute )
        {
        return *this;
        }

    mData.reset( new( EMM ) AlfAttributeImpl() );

    mData->mCategory = aAttribute.mData->mCategory;
    mData->mName = UString(aAttribute.name());
    mData->mInterpolationStyle = aAttribute.mData->mInterpolationStyle;

    mData->mTime = aAttribute.mData->mTime;
    mData->mDelay = aAttribute.mData->mDelay;
    mData->mMappingFunctionId = aAttribute.mData->mMappingFunctionId;
    mData->mDataField = UString(aAttribute.getDataField());

    int count = aAttribute.mData->mTargetValueContainer.count();
    mData->mTargetValueContainer.resize( count );
    for ( int i = 0 ; i < count ; i++ )
        {
        mData->mTargetValueContainer.insert(
            i, cloneValueType( aAttribute.mData->mTargetValueContainer[i] ) );
        }

    count = aAttribute.mData->mSourceValueContainer.count();
    mData->mSourceValueContainer.resize( count );
    for ( int i = 0 ; i < count ; i++ )
        {
        mData->mSourceValueContainer.insert(
            i, cloneValueType( aAttribute.mData->mSourceValueContainer[i] ) );
        }

    return *this;
    }

// ---------------------------------------------------------------------------
// Clones the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute* AlfAttribute::clone()
    {
    auto_ptr<AlfAttribute> clone( new( EMM ) AlfAttribute() );
    *clone.get() = *this;
    return clone.release();
    }

// ---------------------------------------------------------------------------
// Gets the name of the Attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfAttribute::name() const
    {
    return mData->mName.getUtf8();
    }

// ---------------------------------------------------------------------------
// Gets the category of the atrribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute::attributecategory AlfAttribute::category() const
    {
    return mData->mCategory;
    }

// ---------------------------------------------------------------------------
// Gets the type of the target atrribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType::Type AlfAttribute::type(
    unsigned int aIndex) const
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mTargetValueContainer[aIndex]->type();
    }

// ---------------------------------------------------------------------------
// Gets the int value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfAttribute::intValue(unsigned int aIndex) const
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mTargetValueContainer[aIndex]->intValue();
    }

// ---------------------------------------------------------------------------
// Gets the real value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT float AlfAttribute::realValue(unsigned int aIndex) const
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mTargetValueContainer[aIndex]->realValue();
    }

// ---------------------------------------------------------------------------
// Gets the string value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfAttribute::stringValue(unsigned int aIndex) const
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mTargetValueContainer[aIndex]->stringValue();
    }

// ---------------------------------------------------------------------------
// Returns true if attribute is set.
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool AlfAttribute::isValueSet() const
    {
    return mData->mTargetValueContainer.count() > 0;
    }

// ---------------------------------------------------------------------------
// Gets the unit of the value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfUnit AlfAttribute::unit(unsigned int aIndex) const
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mTargetValueContainer[aIndex]->unit();
    }

// ---------------------------------------------------------------------------
// Adds a new target value to the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT  void AlfAttribute::addTargetValue(AlfAttributeValueType* aValue)
    {
    mData->mTargetValueContainer.resize(mData->mTargetValueContainer.count() +1 );
    mData->mTargetValueContainer.insert(mData->mTargetValueContainer.count(), aValue);
    }

// ---------------------------------------------------------------------------
// Gets target value count.
// ---------------------------------------------------------------------------
//
OSN_EXPORT  unsigned int AlfAttribute::getTargetValueCount() const
    {
    return mData->mTargetValueContainer.count();
    }

// ---------------------------------------------------------------------------
// Sets the target value of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setTargetValue(AlfAttributeValueType* aValue,
        unsigned int aIndex)
    {
    if (aIndex < mData->mTargetValueContainer.count())
        {
        mData->mTargetValueContainer.remove(aIndex);
        }
    mData->mTargetValueContainer.insert(aIndex, aValue);
    mData->mDirtyFlag=true;
    }

// ---------------------------------------------------------------------------
// Gets the target value of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType* AlfAttribute::getTargetValue(
    unsigned int aIndex) const
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mTargetValueContainer[aIndex];
    }

// ---------------------------------------------------------------------------
// Removes the target value of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::removeTargetValue(unsigned int aIndex)
    {
    if (aIndex >= mData->mTargetValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    mData->mTargetValueContainer.remove(aIndex);
    }

// ---------------------------------------------------------------------------
// Adds a new source value to the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::addSourceValue(AlfAttributeValueType* aValue)
    {
    mData->mSourceValueContainer.resize(mData->mSourceValueContainer.count() +1 );
    mData->mSourceValueContainer.insert(mData->mSourceValueContainer.count(), aValue);
    }

// ---------------------------------------------------------------------------
// Gets source value count.
// ---------------------------------------------------------------------------
//
OSN_EXPORT unsigned int AlfAttribute::getSourceValueCount() const
    {
    return mData->mSourceValueContainer.count();
    }

// ---------------------------------------------------------------------------
// Adds a new source value to the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setSourceValue(AlfAttributeValueType* aValue,
        unsigned int aIndex)
    {
    if (aIndex < mData->mSourceValueContainer.count())
        {
        mData->mSourceValueContainer.remove(aIndex);
        }
    mData->mSourceValueContainer.insert(aIndex, aValue);
    mData->mDirtyFlag=true;
    }

// ---------------------------------------------------------------------------
//  Gets the source value of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType* AlfAttribute::getSourceValue(
    unsigned int aIndex) const
    {
    if (aIndex >= mData->mSourceValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    return mData->mSourceValueContainer[aIndex];
    }

// ---------------------------------------------------------------------------
// Removes the source value of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::removeSourceValue(unsigned int aIndex)
    {
    if (aIndex >= mData->mSourceValueContainer.count())
        {
        ALF_THROW(AlfAttributeException,EInvalidAttribute,"AlfAttribute")
        }
    mData->mSourceValueContainer.remove(aIndex);
    }

// ---------------------------------------------------------------------------
// Sets the interpolation style of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setInterpolationStyle(
    TAlfInterpolationStyle aInterpolationStyle)
    {
    mData->mInterpolationStyle = aInterpolationStyle;
    mData->mDirtyFlag=true;
    }

// ---------------------------------------------------------------------------
// Gets the interpolation style of the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfInterpolationStyle AlfAttribute::getInterpolationStyle()const
    {
    return mData->mInterpolationStyle;
    }

// ---------------------------------------------------------------------------
// Sets the transition time for the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setTime(int aTime)
    {
    mData->mTime = aTime;
    mData->mDirtyFlag=true;
    }

// ---------------------------------------------------------------------------
// Gets the transition time for the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfAttribute::getTime()const
    {
    return mData->mTime;
    }

// ---------------------------------------------------------------------------
// Sets the mapping function id for the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setMappingFunctionId(int aId)
    {
    mData->mMappingFunctionId = aId;
    mData->mDirtyFlag=true;
    }

// ---------------------------------------------------------------------------
// Gets the mapping function id for the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfAttribute::getMappingFunctionId()const
    {
    return mData->mMappingFunctionId;
    }

// ---------------------------------------------------------------------------
// Sets the datafield for the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setDataField(const char* aDataField)
    {
    mData->mDataField = UString(aDataField);
    mData->mDirtyFlag = true;
    }

// ---------------------------------------------------------------------------
// Gets the datafield for the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfAttribute::getDataField() const
    {
    return mData->mDataField.getUtf8();
    }

// ---------------------------------------------------------------------------
// Sets the delay before the transition.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttribute::setDelay(int aDelay)
    {
    mData->mDelay = aDelay;
    mData->mDirtyFlag = true;
    }

// ---------------------------------------------------------------------------
// Gets the delay before the transition.
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfAttribute::getDelay() const
    {
    return mData->mDelay;
    }

// ---------------------------------------------------------------------------
// Resets the data.
// ---------------------------------------------------------------------------
//
void AlfAttribute::reset( const char* aName, AlfAttributeValueType* aValue,
                          attributecategory aCategory)
    {
    mData.reset(new (EMM) AlfAttributeImpl());

    mData->mCategory = aCategory;
    mData->mName = UString(aName);
    mData->mTargetValueContainer.remove(0);
    mData->mTargetValueContainer.insert(0, aValue);
    }

// ---------------------------------------------------------------------------
// Helper function for cloning value type.
// ---------------------------------------------------------------------------
//
AlfAttributeValueType* AlfAttribute::cloneValueType(
    AlfAttributeValueType* aValue) const
    {
    AlfAttributeValueType* ret = 0;

    if (aValue)
        {
        switch (aValue->type())
            {
            case AlfAttributeValueType::EInt:
                {
                ret = new (EMM) AlfAttributeValueType(
                    aValue->intValue(), aValue->unit());
                break;
                }
            case AlfAttributeValueType::EFloat:
                {
                ret = new (EMM) AlfAttributeValueType(
                    aValue->realValue(), aValue->unit());
                break;
                }
            case AlfAttributeValueType::EString:
                {
                ret = new (EMM) AlfAttributeValueType(
                    aValue->stringValue());
                break;
                }
            default:
                break;
            }
        }

    return ret;
    }
OSN_EXPORT bool AlfAttribute::isDirty( ) const
    {
    return mData->mDirtyFlag;
    }

OSN_EXPORT void AlfAttribute::setDirty(bool aFlag )
    {
    mData->mDirtyFlag = aFlag;
    }

    } // Alf
