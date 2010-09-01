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
* Description:  Attribute container.
*
*/


#ifndef ALFATTRIBUTECONTAINER_H
#define ALFATTRIBUTECONTAINER_H

#include <osn/osndefines.h>
#include <memory>

namespace Alf
    {

class AlfAttribute;
class AlfAttributeContainerImpl;

/**
 *  @class AlfAttributeContainer alfattributecontainer.h "alf/alfattributecontainer.h"
 *  AlfAttributeContainer is a collection of attributes. It is used to bunch up a set 
 *  of attributes. Attribute setters and implementations of the IAlfAttributeOwner interface
 *  can work with attribute containers.
 *  @see IAlfAttributeSetter
 *  @see IAlfAttributeOwner
 *  @see AlfAttribute 
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */ 
class AlfAttributeContainer
    {
public:

    /** Type enumeration. */
    enum Type
        {
        EAlfCommonAttributeContainer = 0,

        EAlfSetFlagAttributeContainer,
        EAlfClearFlagAttributeContainer,

        EAlfOpacityAttributeContainer,
        EAlfXPosAttributeContainer,
        EAlfYPosAttributeContainer,
        EAlfWidthAttributeContainer,
        EAlfHeightAttributeContainer,
        EAlfDepthOffsetAttributeContainer,
        EAlfScrollAttributeContainer, //not used yet
        EAlfExpandAttributeContainer,
        EAlfRowsAttributeContainer,
        EAlfColumnsAttributeContainer,
        EAlfAnchorPointAttributeContainer,
        EAlfScalingAttributeContainer,
        EAlfImageSecondaryAlphaAttributeContainer,

        EAlfPositionAttributeContainer,
        EAlfSizeAttributeContainer,
        EAlfScrollOffsetAttributeContainer,
        EAlfTranslationAttributeContainer,//not implemented still
        EAlfRotationXYZAAttributeContainer,//not implemented still

        EAlfPrimaryImageAttributeContainer,
        EAlfSecondaryImageAttributeContainer,
        EAlfTextAttributeContainer,

        EAlfViewportSizeAttributeContainer,
        EAlfViewportVirtualSizeAttributeContainer,
        EAlfViewportPosAttributeContainer,
        EAlfSkinImageIDAttributeContainer,
        EAlfSkinTextColorAttributeContainer,
        };

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aType a type of container.
     */
    OSN_IMPORT AlfAttributeContainer(Type aType = EAlfCommonAttributeContainer);

    /**
    * Destructor.
    */
    OSN_IMPORT ~AlfAttributeContainer();

    /**
     * Gets the type of the container.
     *
     * @return The type of the container.
     */
    OSN_IMPORT Type type() const;

    /**
     * Adds a new attribute to the container.
     * @exception std::bad_alloc
     *
     * @param aAttribute The new attribute, takes ownership.
     */
    OSN_IMPORT void addAttribute( AlfAttribute* aAttribute);

    /**
     * Returns the attribute count.
     *
     * @return the attribute count.
     */
    OSN_IMPORT unsigned int attributeCount() const;

    /**
     * Returns reference to the attribute.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex index of the attribute.
     * @return The attribute. Throws CAlfDataException if index is invalid.
     *         Ownership is not transferred.
     */
    OSN_IMPORT AlfAttribute& getAttribute(unsigned int aIndex) const;

    /**
     * Returns the attribute.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttribute if aName is not found among attributes contained in the container. 
     *
     * @param aName name of the attribute.
     * @return the attribute. Throws CAlfDataException if attribute is not found.
     *         Ownership is not transferred.
     */
    OSN_IMPORT AlfAttribute& getAttributeByName(const char* aName) const;

    /**
     * Removes the attribute. Throws CAlfDataException if index is invalid.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex index of the attribute.
     */
    OSN_IMPORT void removeAttribute(unsigned int aIndex);

    /**
     * Sets the animation time of the attribute container.
     *
     * @param The time value (milliseconds).
     */
    OSN_IMPORT void setTime(unsigned int aTime);

    /**
     * Returns the animation time of the attribute container.
     *
     * @return The time value (milliseconds).
     */
    OSN_IMPORT unsigned int getTime() const;

private:

    /**
     * Copy construction, not implemented.
     */
    AlfAttributeContainer(const AlfAttributeContainer&);

    /**
     * Assigment operation, not implemented.
     */
    AlfAttributeContainer& operator=(const AlfAttributeContainer&);

private:

    // Implementation data
    std::auto_ptr<AlfAttributeContainerImpl> mData;
    };

    }   // namespace Alf

#endif // ALFATTRIBUTECONTAINER_H
