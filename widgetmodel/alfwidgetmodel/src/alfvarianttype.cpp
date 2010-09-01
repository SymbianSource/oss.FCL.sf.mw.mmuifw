/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Impelements Variants for widgetmodel data.
*
*/



//INCLUDES


#include <alf/ialfmodelbase.h>
#include <osn/ustring.h>
#include <osn/alfptrvector.h>
#include <alf/alfvarianttype.h>
#include <osn/osncommon.h>
#include <osn/ustring.h>
#include <alf/alfexceptions.h>
//#include "alf/alfperf.h"
#include <osn/osnnew.h>
#include "alfsort.h"

using osncore::UString;
using osncore::AlfPtrVector;

using std::auto_ptr;


// Forward declared inside the Alf namespace
namespace Alf
    {

    union UData
        {
        bool    mBool;                                // Boolean
        int     mInt;                                // 32-bit integer
        uint    mUint;                                // 32-bit unsigned integer
        double  mReal;                                // 64-bit real
        UString*  mString;        
        Alf::IAlfModelBase* mBasePtr;               
        };



//Internal class used to store the VariantData type
//These classes are not exposed.
class AlfVariantDataImpl
{
public:
    //Default Constructor
    AlfVariantDataImpl()
    {
        mData.mString = NULL;
    }
    //Destructor
    ~AlfVariantDataImpl()
    {
        if(mType == IAlfVariantType::EString)
        {
            if(mData.mString)
            {
                delete mData.mString;
                mData.mString = NULL;
            }
        }
        if(mType == IAlfVariantType::ECustomData)
        {
            if(mData.mBasePtr)
            {
                delete mData.mBasePtr;
                mData.mBasePtr = NULL;
            }
        }        

    }
    //Union that actually stores the variant Data
    uint8 mType;
    UData mData;    
};    


//Internal class used to store the Conatiner Data type
//These classes are not exposed.
class AlfContainerDataImpl 
{
public:    
    //Default Constructor
    AlfContainerDataImpl()
    {
        mArray.setAutoDelete();
    }
    //Destructor
    ~AlfContainerDataImpl()
    {
        mArray.clear();
    }
    AlfPtrVector<IAlfVariantType> mArray;
    
};



//Internal class used to store the Conatiner Data type
//These classes are not exposed.
class MapItem
{
public:
    //Default  Constructor
    MapItem():mData(0),mName(0)
    {
        
    }
    //Destructor
    ~MapItem()
    {
        if(mData)
        {
            delete mData;
            mData = NULL;
        }
        if(mName)
        {
            delete mName;
            mName = NULL;
        }
    }
    IAlfVariantType*    mData;
    UString*         mName;
};
    
//Internal class used to store the Branch Data type
//These classes are not exposed.
class AlfMapDataImpl
{
public:
    AlfMapDataImpl()
    {
        
    }
    ~AlfMapDataImpl()
    {
        
        mArray.clear();
    }
    //Array to hold the Map Item
    AlfPtrVector<MapItem> mArray;
};    

//owned
class AlfBranchDataImpl 
{
public:
    AlfBranchDataImpl():mCurrentData(0),mChildData(0)
    {
        
    }
    ~AlfBranchDataImpl()
    {
        if(mCurrentData)
        {    
            delete mCurrentData;
            mCurrentData=NULL;    
        }
        
        if(mChildData)
        {
            delete mChildData;
            mChildData=NULL;    
        }    
    }    
    IAlfMap* mCurrentData;
    IAlfMap* mChildData;    
};


////////////////////// AlfVariantType //////////////////////

// ---------------------------------------------------------------------------
// Description :Initialize the VarianType with an Unsigned 8 bit Integer Value 
// ---------------------------------------------------------------------------
//

void AlfVariantType::construct(Type aType)
    {   
    mVarData.reset( new (EMM) AlfVariantDataImpl() );
    mVarData->mType = aType;    
    }

OSN_EXPORT AlfVariantType::AlfVariantType() 
    {    
    mVarData.reset( new (EMM) AlfVariantDataImpl() );
    }

// ---------------------------------------------------------------------------
// Description : Parameterized Constructor for an Integer
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfVariantType::AlfVariantType(IAlfModelBase* aValue)
    {
    construct(ECustomData);
    mVarData->mData.mBasePtr = aValue;
    }


// ---------------------------------------------------------------------------
// Description : Parameterized Constructor for an Integer
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfVariantType::AlfVariantType(const UString& aValue)
    {
    construct(EString);
    mVarData->mData.mString = new(EMM) UString(aValue);
    }



// ---------------------------------------------------------------------------
// Description : Parameterized Constructor for an Integer value
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfVariantType::AlfVariantType(const int& aValue)
    {
    construct(EInt);
    mVarData->mData.mInt  = aValue;    
    }


// ---------------------------------------------------------------------------
// Description : Parameterized Constructor for an Unsigned Int  value
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfVariantType::AlfVariantType(const uint& aValue)
    {
    construct(EUint);
    mVarData->mData.mUint = aValue;      
    }

// ---------------------------------------------------------------------------
// Description : Parameterized Constructor for an Double value
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfVariantType::AlfVariantType(const double& aValue)
    {
    construct(EReal);
    mVarData->mData.mReal  = aValue;
    }

// ---------------------------------------------------------------------------
// Description : Parameterized Constructor for an Bool  value
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfVariantType::AlfVariantType(const bool& aValue)
    {
    construct(EBool);
    mVarData->mData.mBool  = aValue;        
    }


// ---------------------------------------------------------------------------
// Description : Class Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfVariantType::~AlfVariantType()
    {
    }
  
// ---------------------------------------------------------------------------
// Description : Sets  Data to be stored in the Variant Data Type
// ---------------------------------------------------------------------------
// 
OSN_EXPORT void AlfVariantType::set(IAlfVariantType& aValue)
    {
	if( mVarData->mType == EString)
		{
		delete mVarData->mData.mString;
        mVarData->mData.mString = 0;
		}
		
    // Set the type
    mVarData->mType = aValue.type();

    // Copy/create the new data
    if (aValue.type() == EBool)
        {
        mVarData->mData.mBool = aValue.boolean();
        }
    else if (aValue.type() <= EInt)
        {
        mVarData->mData.mInt = aValue.integer();
        }
    else if (aValue.type() <= EUint)
        {
        mVarData->mData.mUint = aValue.uinteger();
        }
    else if (aValue.type() <= EReal)
        {
        mVarData->mData.mReal = aValue.real();
        }
    else if (aValue.type() == EString)
        {
        mVarData->mData.mString = new (EMM) UString(aValue.string());
        }
    else if (aValue.type() == ECustomData)
        {
        mVarData->mData.mBasePtr = aValue.customData();
        }        
    else
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }         
    }

// ---------------------------------------------------------------------------
// Description : Gets the Type of the Variant Data Type
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType::Type AlfVariantType::type() const
    {
    return (Type)mVarData->mType;
    }
    

// ---------------------------------------------------------------------------
// Description : Returns the Boolean value if the data type in the variant 
//                 structure  is a bool 
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool AlfVariantType::boolean() const
    {
    if (type() != EBool)
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }
    return mVarData->mData.mBool;
    }
    
    
// ---------------------------------------------------------------------------
// Description : Returns the int value if the data type in the variant 
//                 structure  is a int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfVariantType::integer() const
    {
    if (type() != EInt && type() != EUint) // Signed-unsigned conversion allowed
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }
    return mVarData->mData.mInt;
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the Unsigned value if the data type in the variant 
//                 structure  is a unsigned int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT uint AlfVariantType::uinteger() const
    {
    if (type() != EInt && type() != EUint) // Signed-unsigned conversion allowed
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }
    return mVarData->mData.mUint;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Real value if the data type in the variant 
//                 structure  is a Real data
// ---------------------------------------------------------------------------
//
OSN_EXPORT double AlfVariantType::real() const
    {
    if (type() != EReal)
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }
    return mVarData->mData.mReal;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Descriptor value if the data type in the variant 
//                 structure  is a string data
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfVariantType::string() const
    {
    if (type() != EString)
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }
    return *mVarData->mData.mString;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Container if the data type in the variant 
//                 structure  is a collection
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainer*    AlfVariantType::container() 
    {
    ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
    }


// ---------------------------------------------------------------------------
// Description : Returns the Map if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap*    AlfVariantType::map() 
    {
    ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Branch if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfBranch*    AlfVariantType::branch() 
    {
    ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
    }

// ---------------------------------------------------------------------------
// Description : Returns the User Defined Data Pointer if the data type in the variant 
//                 structure  is IAlfModelBase
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfModelBase*    AlfVariantType::customData() 
    {
    if (type() != ECustomData)
        {
        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
        }
    return mVarData->mData.mBasePtr;
    }
    
// ---------------------------------------------------------------------------
// Description : Returns true, if objects are equal, false otherwise.
// ---------------------------------------------------------------------------
//    

OSN_EXPORT bool AlfVariantType::operator==(const IAlfVariantType &aOther) const
    {
    // Set the type
    bool equals = false;
    if (type() == aOther.type() )
        {
        switch( type() )
            {
            case EBool:
                equals = ( bool() == aOther.boolean() );
                break;
            case EInt:
                equals = ( int() == aOther.integer() );
                break;
            case EUint:
                equals = ( uint() == aOther.uinteger() );
                break;
            case EReal:
                equals = ( real() == aOther.real() );
                break;
            case EString:
                equals = ( string() == aOther.string() );
                break;
            case ECustomData: //not known how to compare.
                break;
            default:
                ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfVariantType")
                 
            }
        }
        
    return equals;
    }

////////////////////// AlfContainer //////////////////////



// ============================ MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------------------------
// Description : Default Constructor for the Container Data Type
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfContainer::AlfContainer()
    {
    construct();
    }


// ---------------------------------------------------------------------------
// Description : Default Construtor
// ---------------------------------------------------------------------------
//
void AlfContainer::construct() 
    {
    mContainerData.reset( new( EMM ) AlfContainerDataImpl() );
    }
        
// ---------------------------------------------------------------------------
// Description : Class Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfContainer::~AlfContainer()
    {
    }
    
// ---------------------------------------------------------------------------
// Description : Adds an AlfVariantType into the container
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfContainer::addItem(IAlfVariantType* aData)
    {
    mContainerData->mArray.resize(mContainerData->mArray.count()+1);
    mContainerData->mArray.insert(mContainerData->mArray.count(),aData);    
    }
    
// ---------------------------------------------------------------------------
// Description : Adds an AlfVariantType into the container
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfContainer::addItem(uint aIndex, IAlfVariantType* aData)
    {
    if( aIndex > mContainerData->mArray.count() )
        {
        ALF_THROW(AlfDataException,EInvalidArrayIndex,"AlfContainer")
        }
    else
        {
        mContainerData->mArray.resize(mContainerData->mArray.count()+1);
        mContainerData->mArray.insert(aIndex, aData);
        }
    }    

// ---------------------------------------------------------------------------
// Description : Gets the Size of the container
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfContainer::count()
    {
    return mContainerData->mArray.count();
    }

// ---------------------------------------------------------------------------
// Description : Gets the Item at a given index in the container
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType*    AlfContainer::item(uint aIndex)
    {
    uint cmpIndex =0; //To remove RVCT compiler warning
    if(aIndex < mContainerData->mArray.count() && aIndex >= cmpIndex )
        {
        return mContainerData->mArray[aIndex];    
        }
    else
        {
        return NULL;    
        }
    }

// ---------------------------------------------------------------------------
// Description :  Clones the existing conatiner and returns a new COntainer
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainer* AlfContainer::clone()
    {
    try
    {
        //ALF_PERF_START( perfdata, "AlfContainer-Clone-Cloning Container")
        auto_ptr<AlfContainer> clone( new( EMM ) AlfContainer() );
        auto_ptr<IAlfVariantType> childData;
        
        for ( int i = 0; i < mContainerData->mArray.count(); ++i )
            {                
            if( mContainerData->mArray[i]->type()== IAlfVariantType::EInt )
                {
                childData.reset( new(EMM) AlfVariantType(mContainerData->mArray[i]->integer()) );
                }
            else if( mContainerData->mArray[i]->type()== IAlfVariantType::EUint )
                {
                childData.reset( new(EMM) AlfVariantType(mContainerData->mArray[i]->uinteger()) );
                }
            else if(mContainerData->mArray[i]->type()== IAlfVariantType::EBool)
                {
                childData.reset( new (EMM) AlfVariantType(mContainerData->mArray[i]->boolean()) );
                }
            else if( mContainerData->mArray[i]->type()== IAlfVariantType::EReal )
                {
                childData.reset( new(EMM) AlfVariantType(mContainerData->mArray[i]->real()) );
                }
            else if( mContainerData->mArray[i]->type()== IAlfVariantType::EString )
                {
                childData.reset( new(EMM) AlfVariantType(mContainerData->mArray[i]->string()) );
                }
            else if( mContainerData->mArray[i]->type()== IAlfVariantType::EMap )
                {
                childData.reset( (IAlfMap*)(mContainerData->mArray[i]->map())->clone() );
                }
            else if( mContainerData->mArray[i]->type()== IAlfVariantType::EContainer )
                {
                childData.reset( (IAlfContainer*)(mContainerData->mArray[i]->container())->clone() );
                }
               else if ( mContainerData->mArray[i]->type()== IAlfVariantType::EBranch )
                {
                childData.reset( mContainerData->mArray[i]->branch()->clone() );
                }
                    else
                    {
                        ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfContainer")
                    }
            clone->mContainerData->mArray.resize( clone->mContainerData->mArray.count() + 1 );
            clone->mContainerData->mArray.insert( clone->mContainerData->mArray.count(), childData.get() );
            childData.release(); // ownership was transferred.
            }
        //ALF_PERF_STOP( perfdata, "AlfContainer-Clone-Cloning Container")
        return clone.release();
      }
      catch(...)
      {
          ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
      }
    }

// ---------------------------------------------------------------------------
// Description : Sorts the container using user defined callback method
// ---------------------------------------------------------------------------
// 
OSN_EXPORT void AlfContainer::sort( const IAlfSortFunction& aSortFunction )
    {
    IAlfVariantType** d = mContainerData->mArray.data();
    AlfSort::sort( d, mContainerData->mArray.count(), aSortFunction );
    }

// ---------------------------------------------------------------------------
// Description : Sets  Data to be stored in the Variant Data Type
// ---------------------------------------------------------------------------
// 
OSN_EXPORT void AlfContainer::set(IAlfVariantType& /*aValue*/)
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }

// ---------------------------------------------------------------------------
// Description : Gets the Type of the Variant Data Type
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType::Type AlfContainer::type() const
    {
    return IAlfVariantType::EContainer;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Boolean value if the data type in the variant 
//                 structure  is a bool 
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool AlfContainer::boolean() const
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the int value if the data type in the variant 
//                 structure  is a int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfContainer::integer() const
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the Unsigned value if the data type in the variant 
//                 structure  is a unsigned int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT uint AlfContainer::uinteger() const
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Real value if the data type in the variant 
//                 structure  is a Real data
// ---------------------------------------------------------------------------
//
OSN_EXPORT double AlfContainer::real() const
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Descriptor value if the data type in the variant 
//                 structure  is a string data
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfContainer::string() const
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer");
    }

// ---------------------------------------------------------------------------
// Description : Returns the Container if the data type in the variant 
//                 structure  is a collection
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainer*  AlfContainer::container() 
    {
    return this;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Map if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap*    AlfContainer::map() 
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Branch if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfBranch*    AlfContainer::branch() 
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }
    
    
// ---------------------------------------------------------------------------
// Description : Returns the User Defined Data Pointer if the data type in the variant 
//                 structure  is IAlfModelBase
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfModelBase*    AlfContainer::customData() 
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }
////////////////////// AlfContainer //////////////////////




// ---------------------------------------------------------------------------
// Description :  Remove  a  item from the Container
// ---------------------------------------------------------------------------
//
OSN_EXPORT     void AlfContainer::removeItem(uint aIndex)
    {
    uint cmpIndex=0;
    if(aIndex >= mContainerData->mArray.count() || (aIndex < cmpIndex))
        {
            ALF_THROW(AlfDataException,EInvalidArrayIndex,"AlfContainer")
        }

    mContainerData->mArray.remove(aIndex);    
    }
    

// ---------------------------------------------------------------------------
// Description :  Clears the contents of the Container
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfContainer::clear()
{
    mContainerData->mArray.clear();     
}
    

// ---------------------------------------------------------------------------
// Description :  Replaces the items of the container at a given index with the new data
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfContainer::replaceItem(uint aIndex, IAlfVariantType* aNewData )
    {
    if((aIndex < mContainerData->mArray.count()) && aNewData)
       {
       try
           {
           mContainerData->mArray.remove(aIndex);
           mContainerData->mArray.insert(aIndex,aNewData);
           }
       catch(...)
           {
           ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
           }
       }
    else
        ALF_THROW(AlfDataException,EInvalidArrayIndex,"AlfContainer")
    }

// ---------------------------------------------------------------------------
// Description : Returns true, if objects are equal, false otherwise.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT bool AlfContainer::operator==(const IAlfVariantType &/*aOther*/) const
    {
    ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfContainer")
    }


////////////////////// AlfMap //////////////////////

// ============================ MEMBER FUNCTIONS ===============================



// ---------------------------------------------------------------------------
// Description : 1st Phase Constructor. Leaves the object onto the cleanup stack
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfMap::AlfMap()
    {
    construct();
    }

// ---------------------------------------------------------------------------
// Description : Map Constructor
// ---------------------------------------------------------------------------
//
void AlfMap::construct()
    {
    mMapDataImpl.reset( new (EMM) AlfMapDataImpl() );
    }

// ---------------------------------------------------------------------------
// Description : Sets  Data to be stored in the Variant Data Type
// ---------------------------------------------------------------------------
// 
OSN_EXPORT void AlfMap::set(IAlfVariantType& aValue)
    {
    (void)aValue;
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }

// ---------------------------------------------------------------------------
// Description : Gets the Type of the Variant Data Type
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType::Type AlfMap::type() const
    {
    return IAlfVariantType::EMap;
    }
    

// ---------------------------------------------------------------------------
// Description : Returns the Boolean value if the data type in the variant 
//                 structure  is a bool 
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool AlfMap::boolean() const
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }
    
    
// ---------------------------------------------------------------------------
// Description : Returns the int value if the data type in the variant 
//                 structure  is a int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfMap::integer() const
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")    
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the Unsigned value if the data type in the variant 
//                 structure  is a unsigned int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT uint AlfMap::uinteger() const
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")    
    }

// ---------------------------------------------------------------------------
// Description : Returns the Real value if the data type in the variant 
//                 structure  is a Real data
// ---------------------------------------------------------------------------
//
OSN_EXPORT double AlfMap::real() const
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Descriptor value if the data type in the variant 
//                 structure  is a string data
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfMap::string() const
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Container if the data type in the variant 
//                 structure  is a collection
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainer*  AlfMap::container() 
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }


// ---------------------------------------------------------------------------
// Description : Returns the Map if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap*    AlfMap::map() 
    {
    return this;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Branch if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfBranch*    AlfMap::branch() 
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the User Defined Data Pointer if the data type in the variant 
//                 structure  is IAlfModelBase
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfModelBase*    AlfMap::customData() 
    {
    ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
    }    

////////////////////// AlfContainer //////////////////////





// ---------------------------------------------------------------------------
// Description : Map Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfMap::~AlfMap()
    {
    }


// ---------------------------------------------------------------------------
// Description : APi to get the name of the data item in the map
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfMap::name(uint aIndex) const
    {
    uint cmpIndex=0;
    if( aIndex >= mMapDataImpl->mArray.count() || (aIndex < cmpIndex)) 
        {
        ALF_THROW(AlfDataException,EInvalidArrayIndex,"AlfMap")
        }
    return *mMapDataImpl->mArray[aIndex]->mName;       
    }
    

// ---------------------------------------------------------------------------
// Description : APi to add an VariantData into the map
// ---------------------------------------------------------------------------
//    
void AlfMap::addItem(IAlfVariantType* aData, const UString &aName)
    {
    auto_ptr<MapItem> item( new( EMM ) MapItem() );
    
    item->mName =  new( EMM ) UString( aName );
    
    mMapDataImpl->mArray.resize( mMapDataImpl->mArray.count() + 1 );
    mMapDataImpl->mArray.insert( mMapDataImpl->mArray.count(), item.get() );

    // Take the ownership only if there was no exception.
    // Otherwise the client is resp. of cleaning up the object.
    item->mData = aData;
    item.release();
    }
    
// ---------------------------------------------------------------------------
// Description : APi to add an VariantData into the map
// ---------------------------------------------------------------------------
//        
void AlfMap::addItem(uint aIndex, IAlfVariantType* aData, const UString& aName)
    {
    if( aIndex > mMapDataImpl->mArray.count() )
        {
        ALF_THROW(AlfDataException,EInvalidArrayIndex,"AlfMap")
        }
    else
        {
        auto_ptr<MapItem> item( new( EMM ) MapItem() );

        item->mName =  new( EMM ) UString( aName );

        mMapDataImpl->mArray.resize( mMapDataImpl->mArray.count() + 1 );
        mMapDataImpl->mArray.insert( aIndex, item.get() );

        // Take the ownership only if there was no exception.
        // Otherwise the client is resp. of cleaning up the object.
        item->mData = aData;
        item.release();
        }
    }


// ---------------------------------------------------------------------------
// Description : Gets the size of the map
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfMap::count() const
    {
    return mMapDataImpl->mArray.count();
    }


// ---------------------------------------------------------------------------
// Description : Gets the Element at a given index
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType*  AlfMap::item(uint aIndex)
    {
    uint cmpIndex=0;
    if( aIndex >= mMapDataImpl->mArray.count() || aIndex <cmpIndex ) 
        {
        return NULL;
        }
    else
        {
        return mMapDataImpl->mArray[aIndex]->mData;    
        }
    }


// ---------------------------------------------------------------------------
// Description : Clones the map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap* AlfMap::clone()
    {
    try
        {
        //ALF_PERF_START( perfdata, "AlfContainer-Clone-Cloning Map")
        auto_ptr<AlfMap> clone( new(EMM) AlfMap() );
        auto_ptr<MapItem> cloneItem;

        for (int i = 0; i < mMapDataImpl->mArray.count(); ++i)
            {
            cloneItem.reset( new(EMM) MapItem() );
            if( !(mMapDataImpl->mArray[i]->mName->getUtf8() == ""))
                {
                cloneItem->mName = new(EMM) UString(*(mMapDataImpl->mArray[i]->mName));
                }        
            if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EInt)
                {
                cloneItem->mData = new(EMM) AlfVariantType(mMapDataImpl->mArray[i]->mData->integer());
                }
            else if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EUint)
                {
                cloneItem->mData = new(EMM) AlfVariantType(mMapDataImpl->mArray[i]->mData->uinteger());
                }
            else if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EBool)
                {
                cloneItem->mData = new (EMM)AlfVariantType(mMapDataImpl->mArray[i]->mData->boolean());
                }
            else if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EReal)
                {
                cloneItem->mData  = new(EMM) AlfVariantType(mMapDataImpl->mArray[i]->mData->real());
                }
            else if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EString)
                {
                cloneItem->mData  = new(EMM) AlfVariantType(mMapDataImpl->mArray[i]->mData->string());
                }
            else if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EMap)
                {
                cloneItem->mData   = (IAlfMap*)(mMapDataImpl->mArray[i]->mData->map())->clone();
                }
            else if(mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EContainer)
                {
                cloneItem->mData   = (IAlfContainer*)(mMapDataImpl->mArray[i]->mData->container())->clone();
                }
            else if (mMapDataImpl->mArray[i]->mData->type()== IAlfVariantType::EBranch)
                {
                cloneItem->mData   = mMapDataImpl->mArray[i]->mData->branch()->clone();
                }
            else
                {
                ALF_THROW(AlfDataException,EInvalidVariantDataType,"AlfMap")
                }

            clone->mMapDataImpl->mArray.resize(clone->mMapDataImpl->mArray.count()+1);
            clone->mMapDataImpl->mArray.insert(clone->mMapDataImpl->mArray.count(),cloneItem.get() );
            cloneItem.release();
            }
        //ALF_PERF_STOP( perfdata, "AlfContainer-Clone-Cloning Map")

        return clone.release();
        }
    catch(...)
        {
        ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
        }
    }


// ---------------------------------------------------------------------------
// Description : Gets the map item with the given name
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType*    AlfMap::item(const UString &aName)
    {
    for (int i = 0; i < mMapDataImpl->mArray.count(); ++i)
        {
        if ( *mMapDataImpl->mArray[i]->mName == aName)
            {
            return mMapDataImpl->mArray[i]->mData;
            }
        }
    return NULL;    
    }

// ---------------------------------------------------------------------------
// Description : Gets the index of the map item with the given name
// ---------------------------------------------------------------------------
//
OSN_EXPORT int    AlfMap::itemIndex(const UString &aName)
    {
    for (int i = 0; i < mMapDataImpl->mArray.count(); ++i)
        {
        if ( *mMapDataImpl->mArray[i]->mName == aName)
            {
            return i;
            }
        }
    return -1;
    }


// ---------------------------------------------------------------------------
// Description :  Remove  a  item from the Map at a given index
// ---------------------------------------------------------------------------
//
OSN_EXPORT     void AlfMap::removeItem(uint aIndex)
    {
    try
        {
        uint cmpIndex =0;
        if(aIndex < mMapDataImpl->mArray.count() && aIndex>= cmpIndex )
            {
            mMapDataImpl->mArray.remove(aIndex);                
            }
        }
    catch(...)
        {
        ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
        }    
    }
    

// ---------------------------------------------------------------------------
// Description :  Clears the contents of the Map 
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfMap::clear()
    {
    mMapDataImpl->mArray.clear();    
    }
    

// ---------------------------------------------------------------------------
// Description :  Replaces the items of the container at a given index with the new data
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfMap::replaceItem(uint aIndex, IAlfVariantType* aNewData )
    {
    try
        {
        uint cmpIndex =0;
        if ( aIndex < mMapDataImpl->mArray.count()&& aIndex >= cmpIndex  )
            {
            auto_ptr<MapItem> newData( new( EMM ) MapItem() );
            newData.get()->mData = aNewData;
            newData.get()->mName = new( EMM ) UString(
                *( mMapDataImpl->mArray[aIndex]->mName ) );

            mMapDataImpl->mArray.remove(aIndex);
            mMapDataImpl->mArray.insert(aIndex,newData.get());

            newData.release(); // ownership transferred away
            }
        }
    catch(...)
        {
        ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
        }
    }


// ---------------------------------------------------------------------------
// Description :  Replaces the items of the container at a given index with the Given name 
// ---------------------------------------------------------------------------
//
OSN_EXPORT   void AlfMap::replaceItem(const UString &aName,IAlfVariantType* aNewData)
    {

    try
        {
        for (int i = 0; i < mMapDataImpl->mArray.count(); ++i)
            {
            if ( *mMapDataImpl->mArray[i]->mName == aName)
                {
                auto_ptr<MapItem> newData( new( EMM ) MapItem() );

                newData.get()->mData = aNewData;
                newData.get()->mName = new( EMM ) UString(*(mMapDataImpl->mArray[i]->mName));

                mMapDataImpl->mArray.remove(i);
                mMapDataImpl->mArray.insert(i,newData.get());

                newData.release(); // ownership transferred away
                break;        
                }
            }
        }
    catch(...)
        {
        ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
        //aNewData might not have been deleted. In case of this exception the user should delete the variant data passed
        }

    }


// ---------------------------------------------------------------------------
// Description :  Remove  a  item from the Map with a given name
// ---------------------------------------------------------------------------
//
OSN_EXPORT     void AlfMap::removeItem(const UString &aName)
    {
    try
        {
        for(int i=0; i< mMapDataImpl->mArray.count();i++)
            {
            if ( *mMapDataImpl->mArray[i]->mName == aName)
                {
                mMapDataImpl->mArray.remove(i);                
                break;
                }        
            }
        }
    catch(...)
        {
        ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
        } 

    }

////////////////////// AlfBranch //////////////////////

// ---------------------------------------------------------------------------
// Description : 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void AlfBranch::construct( IAlfMap* aData, IAlfMap* aChildData) 
    {
    mBranchData.reset( new( EMM ) AlfBranchDataImpl() );

    // Take the ownership of the parameter objects
    mBranchData->mCurrentData = aData;
    mBranchData->mChildData = aChildData;
    // No throwing code after the ownership has been transferred,
    // it would result to double deletion crash.

    }
    
// ---------------------------------------------------------------------------
// Description : 1st Phase Constructor. Leaves the object onto the cleanup stack
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfBranch::AlfBranch(IAlfMap* aData, IAlfMap* aChildData)
    {
    construct( aData, aChildData );
    }
    
// ---------------------------------------------------------------------------
// Description : Default Construtor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfBranch::AlfBranch() 
    {
    mBranchData.reset( new(EMM) AlfBranchDataImpl() );
    }
    
// ---------------------------------------------------------------------------
// Description : Map Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfBranch::~AlfBranch()
    {
    }

// ---------------------------------------------------------------------------
// Description : Sorts the branch using user defined callback method
// ---------------------------------------------------------------------------
// 
OSN_EXPORT void AlfBranch::sort( const IAlfSortFunction& aSortFunction )
    {
        try
        {
            for (int i = 0; i < mBranchData->mChildData->count(); ++i)
                {
                if (mBranchData->mChildData->item(i)->type() == EContainer)
                    {
                    IAlfContainer* container = 
                        mBranchData->mChildData->item(i)->container();
                    container->sort(aSortFunction);
                    }
                }
         }
         catch(...)
         {
             ALF_THROW(AlfDataException,EInvalidMapOperation,"AlfMap")
         }
    }
// ---------------------------------------------------------------------------
// Description : returns the data for this branch as a IAlfMap interface
// ---------------------------------------------------------------------------
//
OSN_EXPORT   IAlfMap* AlfBranch::data()
    {
    return mBranchData->mCurrentData;
    }
    

// ---------------------------------------------------------------------------
// Description : Set the data for this branch
// ---------------------------------------------------------------------------
//
OSN_EXPORT   void AlfBranch::setData( IAlfMap* aData )
    {
    delete mBranchData->mCurrentData;
    mBranchData->mCurrentData = aData;
    }

// ---------------------------------------------------------------------------
// Description : returns the child data at the given index
// ---------------------------------------------------------------------------
//
OSN_EXPORT   IAlfVariantType* AlfBranch::childData(int aIndex)
    {
    if(aIndex <0 || mBranchData->mChildData->count() <=0) 
        return NULL;

    int count = 0;

    // Start counting from the first child (item number 1)
    for (int i = 0; i < mBranchData->mChildData->count(); ++i)
        {

        if ( mBranchData->mChildData->item(i)->type() == IAlfVariantType::EMap || mBranchData->mChildData->item(i)->type() == IAlfVariantType::EBranch )
            {
            // Map contains the data for a single child

            if(count == aIndex)
                {
                return mBranchData->mChildData->item(aIndex);
                }
            count++;
            }

        if (mBranchData->mChildData->item(i)->type() == IAlfVariantType::EContainer)
            {
            IAlfContainer* container = mBranchData->mChildData->item(i)->container();
            for ( int j = 0; j < container->count(); ++j )        
                {
                // Container contains data for multiple children

                if(count == aIndex)
                    {                    
                    return mBranchData->mChildData->item(i)->container()->item(aIndex); 
                    }   
                count ++;
                }
            }
        }

    return NULL;
    }



// ---------------------------------------------------------------------------
// Description : returns the child name at the given index
// ---------------------------------------------------------------------------
//
OSN_EXPORT  const UString& AlfBranch::childName(int aIndex)
    {

    int count = 0;

    // Start counting from the first child (item number 1)
    for (int i = 0; i < mBranchData->mChildData->count(); ++i)
        {

        if ( mBranchData->mChildData->item(i)->type() == IAlfVariantType::EMap || mBranchData->mChildData->item(i)->type() == IAlfVariantType::EBranch )
            {
            // Map contains the data for a single child

            if(count == aIndex)
                {
                return mBranchData->mChildData->name(i);
                }
            count++;
            }
        if (mBranchData->mChildData->item(i)->type() == IAlfVariantType::EContainer)
            {
            IAlfContainer* container = mBranchData->mChildData->item(i)->container();

            if(aIndex >= count && aIndex < (count + container->count()) )
                {
                return mBranchData->mChildData->name(i);
                }
            count+= container->count();

            }
        }
    UString* ret(0);
    return *ret; 
    }



// ---------------------------------------------------------------------------
// Description : returns the Data for all childs 
// ---------------------------------------------------------------------------
//
OSN_EXPORT   IAlfMap* AlfBranch::childData()
    {
    return mBranchData->mChildData;
    }

// ---------------------------------------------------------------------------
// Description : Set the data for children of this branch
// ---------------------------------------------------------------------------
//
OSN_EXPORT   void AlfBranch::setChildData( IAlfMap* aChildData )
    {
    delete mBranchData->mChildData;
    mBranchData->mChildData = aChildData;
    }

// ---------------------------------------------------------------------------
// Description : Api to get the Number of children of a branch
// ---------------------------------------------------------------------------
//
OSN_EXPORT   uint AlfBranch::childrenCount()
    {

    uint count = 0;

    // Start counting from the first child (item number 1)
    for (int i = 0; i < mBranchData->mChildData->count(); ++i)
        {
        if ( mBranchData->mChildData->item(i)->type() == IAlfVariantType::EMap ||
            mBranchData->mChildData->item(i)->type() == IAlfVariantType::EBranch )
            {
            // Map contains the data for a single child
            count++;
            }
        if (mBranchData->mChildData->item(i)->type() == IAlfVariantType::EContainer)
            {
            // Container contains data for multiple children
            count += mBranchData->mChildData->item(i)->container()->count();
            }
        }

    return count;
    }        

// ---------------------------------------------------------------------------
// Description : Sets  Data to be stored in the Variant Data Type
// ---------------------------------------------------------------------------
// 
OSN_EXPORT void AlfBranch::set(IAlfVariantType& aValue)
    {
    (void)aValue;
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }

// ---------------------------------------------------------------------------
// Description : Gets the Type of the Variant Data Type
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType::Type AlfBranch::type() const
    {
    return IAlfVariantType::EBranch;
    }

// ---------------------------------------------------------------------------
// Description : Returns the Boolean value if the data type in the variant 
//                 structure  is a bool 
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool AlfBranch::boolean() const
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the int value if the data type in the variant 
//                 structure  is a int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfBranch::integer() const
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }
    
// ---------------------------------------------------------------------------
// Description : Returns the Unsigned value if the data type in the variant 
//                 structure  is a unsigned int 
// ---------------------------------------------------------------------------
//
OSN_EXPORT uint AlfBranch::uinteger() const
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Real value if the data type in the variant 
//                 structure  is a Real data
// ---------------------------------------------------------------------------
//
OSN_EXPORT double AlfBranch::real() const
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Descriptor value if the data type in the variant 
//                 structure  is a string data
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfBranch::string() const
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Container if the data type in the variant 
//                 structure  is a collection
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainer*  AlfBranch::container() 
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }


// ---------------------------------------------------------------------------
// Description : Returns the Map if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfMap*    AlfBranch::map() 
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }

// ---------------------------------------------------------------------------
// Description : Returns the Branch if the data type in the variant 
//                 structure  is a map
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfBranch*    AlfBranch::branch() 
    {
    return this;
    }


// ---------------------------------------------------------------------------
// Description : Returns the User Defined Data Pointer if the data type in the variant 
//                 structure  is IAlfModelBase
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfModelBase*    AlfBranch::customData() 
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }
// ---------------------------------------------------------------------------
// Description : Clones the branch
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfBranch*    AlfBranch::clone()
    {
    try
        {
        auto_ptr<IAlfMap> currentData( mBranchData->mCurrentData->clone() );
        auto_ptr<IAlfMap> childData( mBranchData->mChildData->clone() );
        IAlfBranch* clone = new(EMM) AlfBranch( currentData.get(), childData.get() );
        currentData.release(); // ownership transferred away
        childData.release(); // ownership transferred away

        return clone;
        }
    catch(...)
        {
        ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
        }
    }

// ---------------------------------------------------------------------------
// Description : Returns true, if objects are equal, false otherwise.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT bool AlfBranch::operator==(const IAlfVariantType &/*aOther*/) const
    {
    ALF_THROW(AlfDataException,EInvalidBranchOperation,"AlfBranch")
    }

} // namespace Alf
