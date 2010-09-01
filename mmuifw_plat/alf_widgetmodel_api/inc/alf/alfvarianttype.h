/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This interface defines a generic data type
*
*/



#ifndef ALFVARIANTTYPE_H
#define ALFVARIANTTYPE_H


//  INCLUDES
#include <e32def.h>
#include <osn/osndefines.h>
#include <osn/osncommon.h>
#include <alf/ialfvarianttype.h>
#include <alf/ialfcontainer.h>
#include <alf/ialfmap.h>
#include <alf/ialfbranch.h>
#include <memory>
#include <alf/ialfsortfunction.h>


namespace osncore
    {
class UString;
    }
namespace std
    {
template <class T> class auto_ptr;
    }

using namespace osncore;
using std::auto_ptr;

namespace Alf
    {

// FORWARD DECLARATIONS
class IAlfContainer;
class IAlfMap;
class AlfBranch;
class AlfVariantDataImpl;
class AlfContainerDataImpl;
class AlfMapDataImpl;
class AlfBranchDataImpl;
class IAlfModelBase;
// CLASS DECLARATIONS

/**
 *  @class AlfVariantType alfvarianttype.h "alf/alfvarianttype.h"
 *  Concrete implementation of the variant data type interface. 
 *  AlfVariantType can hold data of integer,real,string and unsigned integer types.
 *  @see IAlfVariantType
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfVariantType: public IAlfVariantType
    {
public:

    /**
     * Constructor for variant data type creation for a descriprtor type of data
     * @exception std::bad_alloc
     *
     * @param aValue - Descriptor to be assigned for the variant data
     * @since S60 ?S60_version
     */
    OSN_IMPORT  AlfVariantType(const UString& aValue);


    /**
     * Constructor for variant data type creation for a integer type of data
     * @exception std::bad_alloc
     *
     * @param aValue - integer to be assigned for the variant data
     * @since S60 ?S60_version
     */
    OSN_IMPORT   AlfVariantType(const int& aValue);


    /**
     * Constructor for variant data type creation for a Unsigned integer type of data
     * @exception std::bad_alloc
     *
     * @param aValue - unsigned integer to be assigned for the variant data
     * @since S60 ?S60_version
     */
    OSN_IMPORT  AlfVariantType(const uint& aValue);


    /**
     * Constructor for variant data type creation for a real type of data
     * @exception std::bad_alloc
     *
     * @param aValue - Real value to be assigned for the variant data
     * @since S60 ?S60_version
     */
    OSN_IMPORT   AlfVariantType(const double& aValue);


    /**
      * Constructor for variant data type creation for a user defined type of data
      * @exception std::bad_alloc
      *
      * @param aValue - IAlfModelBase* to be assigned for the variant data
      * @since S60 ?S60_version
      */
    OSN_IMPORT  AlfVariantType(IAlfModelBase* aValue) ;

    /**
     * Constructor for variant data type creation for a bool type of data
     * @exception std::bad_alloc
     *
     * @param aValue - Boolean value to be assigned for the variant data
     * @since S60 ?S60_version
     */
    OSN_IMPORT  AlfVariantType(const bool& aValue);

    //from IAlfVariantType Interface
    /**
     * Set the Data the variantData. From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is of invalid type.  
     * @exception std::bad_alloc
     *
     * @param aValue    The new value.
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void set(IAlfVariantType& aValue);

    /**
     * Get the data type.From IAlfVariantType Interface
     *
     * @since S60 ?S60_version
     * @return The data type.
     */
    OSN_IMPORT    IAlfVariantType::Type type() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is not of boolean type.  
     *
     * @return bool data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    bool boolean() const;

    /**
     * Get the data value as an Integer .From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is not of integer type.  
     *
     * @return Integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    int integer() const;

    /**
     * Get the data value as unsigned  int.From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is not of unsigned integer type.  
     *
     * @return unsigned integer value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    uint uinteger() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is not of real type.  
     *
     * @return real data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    double real() const;

    /**
     * Get the data value as an Descriptor .From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is not of string type.  
     *
     * @return string data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    const UString& string() const;

    /**
     * Get the data value as an Container .From IAlfVariantType Interface. 
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfVariantType is not a container.   
     * 
     * @return AlfContainer .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfContainer* container();

    /**
     * Get the data value as Map .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfVariantType is not a map.   
     *
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* map() ;

    /**
     * Get the data value as Branch .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfVariantType is not a branch.   
     *
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfBranch* branch() ;

    /**
     * Get the data value as a User Defined Data Pointer.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is not of custom type.  
     *
     * @return IAlfModelBase interface.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfModelBase* customData();

    /**
     * Destructor
     *
     * @return
     * @since S60 ?S60_version
     */
    OSN_IMPORT ~AlfVariantType();


    /**
     * The default constructor.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfVariantType();

    /**
     * Equality comparison operator.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if aValue  
     *                                      is of invalid type.  
     *
     * @param aOther object to compare against.
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool operator==(const IAlfVariantType& aOther) const;

private:


    /**
     * 2nd Phase Constructor
     *
     * @since S60 ?S60_version
     * @param aType
     */
    void  construct(Type aType);


private:
    //data owned
    auto_ptr<AlfVariantDataImpl> mVarData;

    };


/**
 *  @class AlfVariantType alfvarianttype.h "alf/alfvarianttype.h"
 *  Concrete implementation of the variant data type interface. 
 *  Simple default container of variant types.
 *  @see IAlfContainer
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfContainer : public IAlfContainer
    {
public:

     /**
     * Constructor for creating a variant data type which can store a alfcontainer
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT  AlfContainer();

    /**
     * Destructor
     * @since S60 ?S60_version
     */
    OSN_IMPORT  ~AlfContainer();

    //IAlfContainer APIs

    /**
     * Adds a variantData item into the container .From IAlfContainer interface
     * @aData variant data to be added into the container. Ownership is transferred
     * if the method completes without exceptions. If there is an exception
     * (e.g. due to out of memory), the client code is responsible for cleaning up the object.
     * @exception std::bad_alloc
     *
     * @return void.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void addItem(IAlfVariantType* aData);

    /**
     * Adds a variantData item into the container .From IAlfContainer interface
     * @aIndex index where the data is added
     * @aData variant data to be added into the container. Ownership is transferred
     * if the method completes without exceptions. If there is an exception
     * (e.g. due to out of memory), the client code is responsible for cleaning up the object.
     * @exception std::bad_alloc
     *
     * @return void.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void addItem(uint aIndex, IAlfVariantType* aData);

    /**
     * Gets the Number of items in  the container . From IAlfContainer interface
     *
     * @return number of items in the container
     * @since S60 ?S60_version
     */
    OSN_IMPORT int count();

    /**
     * Gets the Item at a given index in  the container From IAlfContainer interface
     * @param aIndex - index at which the element is required
     * @return item as a MlfVariantType
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfVariantType* item(uint aIndex);

    /**
     * Clones the  entire container From IAlfContainer interface
     * @exception std::bad_alloc
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if 
     *                                      there is an invalid data type in the container.   
     *
     * @return the cloned container
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfContainer* clone();


    /**
     * Removes  an item from  given index in  the container. From IAlfContainer interface
     * @param aIndex - index at which the element is to be removed.
     * @return item as a IAlfVariantType
     * @since S60 ?S60_version
     */
    OSN_IMPORT void removeItem(uint aIndex);

    /**
     * Clear the Container. From IAlfContainer interface
     *
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT void clear() ;

    /**
     * Replace  an item from  given index in  the container.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidContainerOperation if
     *                                      the replaceItem fails.   
     *
     * @param aIndex - index at which the element is to be replaced
     * @param aNewData - new Data to be replaced
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT void replaceItem(uint aIndex, IAlfVariantType* aNewData );

    /**
     * sorts the container using user defined sort function.
     *
     * @param aSortFunction - sort function.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void sort( const IAlfSortFunction& aSortFunction );

    //from IAlfVariantType Interface
    /**
     * Set the Data the variantData. From IAlfVariantType Interface.
     * Always throws an exception always.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidContainerOperation always. 
     *                                      Use the addItem API.   
     *
     * @param aValue    The new value.
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT void set(IAlfVariantType& aValue);

    /**
     * Get the data type.From IAlfVariantType Interface
     *
     * @return The data type.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfVariantType::Type type() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not boolean.   
     *
     * @return bool data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool boolean() const;

    /**
     * Get the data value as an Integer .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not an integer.   
     *
     * @return Integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT  int integer() const;

    /**
     * Get the data value as unsigned  int.From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not an unsigned integer.   
     *
     * @return unsigned integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT  uint uinteger() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not a real value.   
     *
     * @return real data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT double real() const;

    /**
     * Get the data value as an Descriptor .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not a string.   
     *
     * @return string data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT  const UString& string() const;

    /**
     * Get the data value as an Conatiner .From IAlfVariantType Interface.
     *
     * @return AlfContainer .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfContainer* container();

    /**
     * Get the data value as Map .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not a map.   
     *
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* map();

    /**
     * Get the data value as Branch .From IAlfVariantType Interface.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not a map.   
     *
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfBranch* branch();

    /**
     * Get the data value as a User Defined Data Pointer.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfContainer is not a custom data type.   
     *
     * @return IAlfModelBase interface.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfModelBase* customData();

    /**
     * Equality comparison operator.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidContainerOperation always.
     *
     * @param aOther object to compare against.
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool operator==(const IAlfVariantType& aOther) const;
private:
    void construct();
    //Data owned
    auto_ptr<AlfContainerDataImpl> mContainerData;

    };

/**
 *  @class AlfMap alfvarianttype.h "alf/alfvarianttype.h"
 *  Concrete implementation of the variant data type interface. 
 *  Simple map of variant types.The map is contains data and a key.
 *  @see IAlfMap
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft 
 */
class AlfMap : public IAlfMap
    {
public:


    /**
     * 1st phase Constructor for creating a variant data type which can store a AlfMap
     * @exception std::bad_alloc
     * 
     * @since S60 ?S60_version
     */
    OSN_IMPORT  AlfMap();



    //IAlfContainer APIs

    /**
     * Gets the Number of items in  the container . From IAlfContainer interface
     *
     * @return number of items in the container
     * @since S60 ?S60_version
     */
    OSN_IMPORT    int count() const;

    /**
     * Gets the Item at a given index in  the container From IAlfContainer interface
     * @param aIndex - index at which the element is required
     * @return item as a MlfVariantType
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfVariantType* item(uint aIndex);

    /**
     * Clones the  entire container From IAlfContainer interface
     * @exception std::bad_alloc
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType if 
     *                                      there is an invalid data type in the container.   
     *
     * @return the cloned container
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* clone();


    /**
     * Removes  an item from  given index in  the container. From IAlfContainer interface.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidMapOperation if
     *                                      the removeItem fails.   
     *     
     * @param aIndex - index at which the element is to be removed
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void removeItem(uint aIndex);

    /**
     * Clear the Container. From IAlfContainer interface
     *
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void clear() ;

    /**
     * Replace  an item from  given index in  the container
     * @exception std::bad_alloc
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidMapOperation if
     *                                      the replaceItem fails.   
     * @param aIndex - index at which the element is to be replaced
     * @param aNewData - new Data to be replaced
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void replaceItem(uint aIndex, IAlfVariantType* aNewData );



    //IAlfMap Interface APis

    /**
     * Gets the Item for a given name  in  the map. From IAlfContainer interface
     * @param aName - name of the element that is required
     * @return item as a IAlfVariantType
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfVariantType* item(const UString& aName);

    /**
     * Gets the index of the item by the given name.
     * @param aName The name associated with requested item.
     * @return The index corresponding to the name of the item.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    int itemIndex(const UString& aName);

    /**
     * Removes  an item for a given Name in map. From IAlfContainer interface
     * @exception std::bad_alloc
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidMapOperation if
     *                                      the removeItem fails.   
     *
     * @param aName - name of the element that is to be removed
     * @return item as a MlfVariantType
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void removeItem(const UString& aName);

    /**
     * Replace  an item for a given name in  the container
     * @exception std::bad_alloc
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidMapOperation if
     *                                      the replaceItem fails.   
     *
     * @param aName - name  of the element that is to be replaced
     * @param aNewData - new Data to be replaced
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void replaceItem(const UString& aName,IAlfVariantType* aNewData);

    /**
     * Adds a variantData item into the container .From IAlfContainer interface
     * @exception std::bad_alloc
     *
     * @param aName - name of the data that needs to be added
     * @param aData- variant data to be added into the container. Ownership is transferred
     * if the method completes without exceptions. If there is an exception
     * (e.g. due to out of memory), the client code is responsible for cleaning up the object.
     * @return void.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void addItem(IAlfVariantType* aData, const UString& aName);

    /**
     * Adds a variantData item into the container .From IAlfContainer interface
     * @exception std::bad_alloc
     *
     * @aIndex index where the data is added
     * @param aName - name of the data that needs to be added
     * @param aData- variant data to be added into the container. Ownership is transferred
     * if the method completes without exceptions. If there is an exception
     * (e.g. due to out of memory), the client code is responsible for cleaning up the object.
     * @return void.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void addItem(uint aIndex, IAlfVariantType* aData, const UString& aName);

    /**
     * Gets the Name at a given index in  the container
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidArrayIndex if
     *                                      aIndex is invalid.   
     *
     * @param aIndex - index at which the name of the element in the map  is required
     * @return item name as a TPtr
     * @since S60 ?S60_version
     */
    const UString& name(uint aIndex) const;

    //class methods
    /**
     * Destructor
     * @since S60 ?S60_version
     */
    OSN_IMPORT ~AlfMap();


    //from IAlfVariantType Interface
    /**
     * Set the Data the variantData. From IAlfVariantType Interface.
     * Always throws an exception always.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidMapOperation always. 
     *                                      Use the addItem API.   
     * @param aValue    The new value.
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void set(IAlfVariantType& aValue);

    /**
     * Get the data type.From IAlfVariantType Interface
     *
     * @return The data type.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfVariantType::Type type() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not boolean.   
     *
     * @return bool data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    bool boolean() const;

    /**
     * Get the data value as an Integer .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not an integer.   
     *
     * @return Integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    int integer() const;

    /**
     * Get the data value as unsigned  int.From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not an unsigned integer.   
     *
     * @return unsigned integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    uint uinteger() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not a real value.   
     *
     * @return double data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    double real() const;

    /**
     * Get the data value as an Descriptor .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not a string value.   
     *
     * @return string data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    const UString& string() const;

    /**
     * Get the data value as an Conatiner .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not a container.   
     *
     * @return AlfContiner .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfContainer* container();

    /**
     * Get the data value as Map .From IAlfVariantType Interface
     *
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* map();

    /**
     * Get the data value as Branch .From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not a branch.   
     *
     * @return Branch
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfBranch* branch();

    /**
     * Get the data value as a User Defined Data Pointer.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfMap is not a custom data type.   
     *
     * @return IAlfModelBase interface.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfModelBase* customData();

    /**
     * Equality comparison operator.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool operator==(const IAlfVariantType& aOther) const;

private:
    void construct();
    //data
    auto_ptr<AlfMapDataImpl> mMapDataImpl;
    };

/**
 *  @class AlfBranch alfvarianttype.h "alf/alfvarianttype.h"
 *  Concrete implementation of the variant data type interface. 
 *  @see IAlfBranch
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft 
 */
class AlfBranch: public IAlfBranch
    {
public:


    /**
     * Parameterized Constructor
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT    AlfBranch( IAlfMap* aData, IAlfMap* aChildData);

    /**
     * Default Constructor
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT    AlfBranch();

    /**
     *
     * @return
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* data();

    /**
     *
     * @param  aData -
     * @return void.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void setData( IAlfMap* aData );

    /**
     *
     * @param  aIndex -
     * @return .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfVariantType* childData(int aIndex);

    /**
     *
     * @param  aIndex -
     * @return .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    const UString& childName(int aIndex);

    /**
     *
     * @return .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* childData();

    /**
     *
     * @param  aChildData -
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void setChildData( IAlfMap* aChildData );

    /**
     *
     * @return
     * @since S60 ?S60_version
     */
    OSN_IMPORT    uint childrenCount();

    /**
     * Destructor
     * @since S60 ?S60_version
     */
    OSN_IMPORT ~AlfBranch();

    /**
     * sorts the branch using user defined sort function.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidBranchOperation if
     *                                      sorting fails.   
     *
     * @param aSortFunction - sort function.
     * return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT void sort( const IAlfSortFunction& aSortFunction );

    /**
     * Function to Clone the Branch
     * @exception std::bad_alloc
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidBranchOperation if
     *                                      cloning fails.   
     * 
     * @return
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfBranch*  clone();


    //from IAlfVariantType Interface
    /**
     * Set the Data the variantData. From IAlfVariantType Interface
     * Always throws an exception always.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidBranchOperation always. 
     *                                      Use the setData API.   
     *
     * @param aValue    The new value.
     * @return void
     * @since S60 ?S60_version
     */
    OSN_IMPORT    void set(IAlfVariantType& aValue);

    /**
     * Get the data type.From IAlfVariantType Interface
     *
     * @return The data type.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfVariantType::Type type() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not boolean.   
     * @return bool data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    bool boolean() const;

    /**
     * Get the data value as an Integer .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not an integer.   
     * 
     * @return Integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    int integer() const;

    /**
     * Get the data value as unsigned  int.From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not an unsigned integer.   
     * 
     * @return unsigned integer data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    uint uinteger() const;

    /**
     * Get the data value as an bool .From IAlfVariantType Interface
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not a real value.   
     * 
     * @return real data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    double real() const;

    /**
     * Get the data value as an Descriptor .From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not a string value.   
     * 
     * @return string data value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT    const UString& string() const;

    /**
     * Get the data value as an Conatiner. From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not a container.   
     * 
     * @return AlfContiner .
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfContainer* container();

    /**
     * Get the data value as Map. From IAlfVariantType Interface
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not a map.   
     * 
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfMap* map();

    /**
     * Get the data value as Branch. From IAlfVariantType Interface
     * 
     * @return Map
     * @since S60 ?S60_version
     */
    OSN_IMPORT    IAlfBranch* branch();

    /**
     * Get the data value as a User Defined Data Pointer.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidVariantDataType always
     *                                      since AlfBranch is not a custom value.   
     * 
     * @return IAlfModelBase interface.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfModelBase* customData();

    /**
     * Equality comparison operator.
     * Always throws an exception.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidBranchOperation always.
     * 
     * @param aOther object to compare against.
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool operator==(const IAlfVariantType& aOther) const;
private:
    /**
     * 2nd Phase Constructor
     * @return
     * @since S60 ?S60_version
     */
    void construct( IAlfMap* aData, IAlfMap* aChildData);

private:

    //Data - owned
    auto_ptr<AlfBranchDataImpl> mBranchData;


    };

    } // namespace Alf

#endif // ALFVARIANTTYPE_H

// End of File
