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
* Description:  An interface for querying and setting attributes.
*
*/


#ifndef I_ALFATTRIBUTEOWNER_H
#define I_ALFATTRIBUTEOWNER_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

namespace osncore
    {
class UString;
    }

using namespace osncore;

namespace Alf
    {

class AlfAttribute;
class AlfAttributeValueType;
class AlfAttributeContainer;

namespace alfattributeowner
    {
static const IfId ident =
    {
    0, "alfattributeowner"
    };
    }

/**
 *  An interface for querying and setting attributes.
 *  The interface defines a generic functionality for setting
 *  any attributes of the implementing class. It can be used
 *  also to query the available set of attributes.
 *
 *  The interface should by implemented by all widget classes.
 *  It can be also used by element classes.
 *  Widget attributes have often corresponding data fields in the
 *  model. In this case, setting new values should also update
 *  the model. If an attribute is related to the presentation of
 *  the widget, the presentation is updated when a new value is set
 *  for the attribute.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfAttributeOwner : public IAlfInterfaceBase
    {
public:
    /**
     * Virtual destructor.
     */
    virtual ~IAlfAttributeOwner() {}

    /**
     * Get the number of available attributes.
     *
     * @return The number of attributes.
     */
    virtual int attributeCount() const = 0;

    /**
     * Gets the attribute with the given name.
     * The returned attribute instance is owned by the widget and
     * should not be deleted.
     * Also the returned attribute instance should not be modified.
     * Use SetAttribute for that.
     *
     * @param aAttribName The name of the attribute.
     * @return The attribute, or 0 if the attribute was not found.
     */
    virtual const AlfAttribute* getAttribute( const UString& aAttribName ) const = 0;

    /**
     * Set a value of an attribute.
     *
     * If no value is present in passed attribute parameter,
     * an AlfAttributeException-exception
     * is thrown with EInvalidAttribute error code.
     *
     * @param aAttribute Reference attribute, the data is copied from it.
     * The ownership is not transferred.
     * @return True if the attribute was supported, false if not.
     *         Attribute is added to container even if it not supported.
     */
    virtual bool setAttribute( const AlfAttribute& aAttribute ) = 0; 

    /**
     * Set a value of an attribute.
     * This SetAttribute variant is for simple non animated attributes 
     * that only have name and value.
     *
     * If no value is present in passed attributevalue parameter,
     * an AlfAttributeException-exception
     * is thrown with EInvalidAttribute error code.
     *
     * @param aAttribName Name of the attribute.
     * @param aValue Value of the attribute. Ownership is transferred 
     * @return True if the attribute was supported, false if not.
     *         Attribute is added to container even if it not supported.
     */    
    virtual bool setAttribute( const UString& aAttribName, 
        AlfAttributeValueType* aValue ) = 0; 
    /**
     * Sets values of each attribute in given container 
     *
     * If no value is present in an attribute in the container,
     * an AlfAttributeException-exception
     * is thrown with EInvalidAttribute error code.
     *
     * @param aAttributeContainer Reference attributeContainer, All attributes
     *        in the container are handled. The ownership is not transferred.
     * @return True if all attributes were supported, false if at least 1 attribute was not supported
     */    
    virtual bool setAttributeContainer( AlfAttributeContainer& aAttributeContainer ) = 0; 

   /**
     * Sets a value of an attribute to a specific target. 
     * The target is identified by using aTargetId parameter.
     *
     * If no value is present in passed attribute parameter,
     * an AlfAttributeException-exception
     * is thrown with EInvalidAttribute error code.
     *
     * @param aTargetId  Name of the target widget part, e.g element class tag.
     * @param aAttribute Reference attribute, the data is copied from it.
     *                   The ownership is not transferred.
     * @return True if the attribute was supported, false if not.
     */    
    virtual bool setAttribute( const UString& aTargetId , 
                               AlfAttribute& aAttribute ) = 0; 


 
  /**
     * Sets a value of an static attribute.
     * This SetAttribute variant is for simple non animated attributes 
     * that only have name and value.
     *
     * If no value is present in passed attributevalue parameter,
     * an AlfAttributeException-exception
     * is thrown with EInvalidAttribute error code.
     *
     * @param aTargetId   The name of the target widget part, e.g. element class
     * @param aAttribName Name of the attribute.
     * @param aValue      Value of the attribute. Ownership is transferred
     * @return True if the attribute was supported, false if not.
     */
    virtual bool setAttribute( const UString& aTargetId, 
                               const UString& aAttribName, 
                               AlfAttributeValueType* aValue ) = 0;
 
   /**
     * Sets values of each attribute in given container to a specific target. 
     * The target is identified by using aTargetId parameter.
     *
     * If no value is present in an attribute in the container,
     * an AlfAttributeException-exception
     * is thrown with EInvalidAttribute error code.
     *
     * @param aTargetId  Name of the target widget part, e.g element class tag.
     * @param aAttributeContainer Reference attributeContainer, All attributes
     *        in the container are handled. The ownership is not transferred.
     * @return True if all attributes were supported, false if at least 1 attribute was not supported
     */    
    virtual bool setAttributeContainer( const UString& aTargetId , 
                               AlfAttributeContainer& aAttributeContainer ) = 0; 

		/**
     * Gets the attribute with the given name from specific target.
     * The returned attribute instance is owned by the widget and
     * should not be deleted.
     * Also the returned attribute instance should not be modified.
     * Use SetAttribute for that.
     *
     * @param aTargetId   The name of the target widget part, e.g. element class
     * @param aAttribName The name of the attribute.
     * @return A pointer to the attribute, or 0 if the attribute was not found.
     */
    virtual const AlfAttribute* getAttribute( const UString& aTargetId ,
                                              const UString& aAttribName ) const = 0;



    /**
     * Getter for the type identifier of this interface.
     *
     * @since S60 ?S60_versio
     * @return Identifier of this interface.
     */
    static inline const IfId& type()
        {
        return alfattributeowner::ident;
        }
    };

    } // namespace Alf

#endif // I_ALFATTRIBUTEOWNER_H
