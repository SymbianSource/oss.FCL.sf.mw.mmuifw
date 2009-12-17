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
* Description:  The interface and the basic implementation for presentation elements.
*
*/


#ifndef I_ALFELEMENT_H
#define I_ALFELEMENT_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>
#include <osn/osntypes.h>

class CAlfVisual;
class CAlfLayout;

using namespace osncore;

namespace duiuimodel
    {
class DuiNode;
    }
using namespace duiuimodel;

namespace Alf
    {
//FORWARD DECLARATION    
class IAlfVariantType;
class IAlfBranch;
class IAlfWidgetControl;
class AlfCustomInitDataBase;  
class IAlfVisualTemplate;

namespace alfelement
    {
static const IfId ident=
    {
    0,"alfelement"
    };
    }

/**
 *  The interface for presentation elements.
 *  The presentation of a widget can be divided into parts by using
 *  elements. Each element is responsible for populating and maintaining
 *  a specific part of the presentation. The element creates and updates
 *  visual trees according  to the data provided by the control. Elements
 *  may also implement presentation logic, such as visual lifetime and
 *  resource management, scrolling, and animations.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfElement : public IAlfInterfaceBase
    {
public:
    static inline const IfId& type()
        {
        return alfelement::ident;
        }
    /**
     * Get the name of the element.
     *
     * @since S60 ?S60_version
     * @return The name of the element.
     */
    virtual const char* name() const = 0;

    /**
     * Create a new visual tree to the child element of this element.
     * Some elements, e.g., CAlfRecyclingGridElement and CAlfRecyclingCurvePathElement
     * implement visual lifetime management to control the creation of the
     * visual trees in the child elements. This method can be implemented to
     * control, if the visual tree should be created or not.
     *
     * @since S60 ?S60_version
     * @param aElement The child element.
     * @param aChildData The data for the new visual tree.
     * @param aData The parent branch of the added data. Used to determine the
     *              visual trees that have outdated data IDs.
     *
     * @param aIndex The index of the new data .
     */
    virtual void createChildVisualTree( IAlfElement* aElement,
                                        IAlfVariantType& aChildData,
                                        IAlfBranch& aData,
                                        int aIndex, uint aDataID ) = 0;

    /**
     *
     *
     * @since S60 ?S60_version
     * @param  aData
     * @param  aParentData
     * @param  aIndex
     * @param  aParentDataID
     * @param  aParentLayout
     * @param  aLayoutIndex
     * @return
     */
    virtual CAlfVisual* createVisualTree( IAlfVariantType& aData,
                                          IAlfBranch& aParentData,
                                          int aIndex, uint aParentDataID,
                                          CAlfLayout* aParentLayout,
                                          int aLayoutIndex ) = 0;

    /**
     *
     *
     * @since S60 ?S60_version
     * @param  aData
     * @param  aDataID
     * @param  aParentLayout
     * @param  aLayoutIndex
     * @return
     */
    virtual CAlfVisual* createVisualTree( IAlfVariantType& aData,
                                          uint aDataID,
                                          CAlfLayout* aParentLayout,
                                          int aLayoutIndex ) = 0;

    /**
     *
     *
     * @since S60 ?S60_version
     * @param  aElement
     * @param  aData
     * @param  aIndex
     * @param  aDataID
     * @return void
     */
    virtual void removeChildVisualTree( IAlfElement* aElement,
                                        IAlfBranch& aData,
                                        int aIndex, uint aDataID ) = 0;

    /**
     *
     *
     * @since S60 ?S60_version
     * @param  aParentData
     * @param  aIndex
     * @param  aParentDataID
     * @return void
     */
    virtual void removeVisualTree( IAlfBranch& aParentData, int aIndex,
                                   uint aParentDataID ) = 0;

    /**
     *
     *
     * @since S60 ?S60_version
     * @param  aData
     * @param  aDataID
     * @return void
     */
    virtual void removeVisualTree( IAlfVariantType& aData, uint aDataID ) = 0;

    /**
     * Create or update a visual tree according to the given data. A data id is assigned
     * to the created visual tree. If a visual tree with the given data id already
     * exists, the visuals are just updated. The caller is responsible for providing
     * the parent layout for the visual tree. The default parent layout may or may not
     * be used.
     *
     * @since S60 ?S60_version
     * @param aData         The data for the visuals.
     * @param aOldData      the old data for the visuals. The data, that is replaced.
     * @param aDataID       The data id for the visual tree.
     * @return The created or updated visual tree.
     */
    virtual void updateVisualTree( IAlfVariantType& aNewData,
                                   IAlfVariantType& aOldData,
                                   uint aDataID ) = 0;

    /**
     * Replace an existing visual tree with a new one. A data id is used
     * to find the visual tree to be replaced. A new data id is assigned
     * to the visual tree, and the visuals are updated according to the
     * given data. The method is used for recycling visual trees for
     * a large number of presentation instances (@see CAlfRecyclingGridElement).
     * The structure of the new visual tree should match the structure
     * of the replaced visual tree. Otherwise, the visuals structure
     * needs to be modified by creating and destroying visuals. The visual
     * tree remains in the same layout as it was before.
     *
     * @since S60 ?S60_version
     * @param aData         The data for the new visuals.
     * @param aDataID       The data id for the new visual tree.
     * @param aOldDataID    The data id of the visual tree to be replaced.
     */
    virtual void replaceVisualTree( IAlfVariantType& aData,
                                    uint aDataID, uint aOldDataID ) = 0;

    /**
     *
     *
     * @since S60 ?S60_version
     * @param  aParentBranch
     * @param  aStartIndex
     * @param  aOffset
     * @param  aOldParentDataId
     * @param  aParentDataID
     * @return void
     */
    virtual void updateDataIDs( IAlfBranch& aParentBranch, int aStartIndex,
                                int aOffset, uint aOldParentDataId,
                                uint aParentDataID ) = 0;

    /**
     * Get the default parent layout for the visual trees created by this element.
     * The default parent layout is used for all root level elements (@see RootElement()).
     *
     * @since S60 ?S60_version
     * @param aParentDataID The data id associated with the parent element instance.
     * @return The default layout for the visual trees created by this element, or NULL
     *         if the default layout does not exist.
     */
    virtual CAlfLayout* defaultParentLayout(uint aParentDataID) = 0;

    /**
     * Find a visual according to the name of the visual and the data id
     * used to create the visual tree.
     *
     * @param aName     The name of the visual.
     * @param aDataID   The data id associated with the visual tree.
     * @return The default layout for the visual trees created by this element.
     */
    virtual CAlfVisual* findVisual(const char* aName, uint aDataID) = 0;

    /**
     * Get the data id of a visual tree created by this element.
     *
     * @param aVisual   The root of the visual tree.
     * @return The data id associated with the visual tree.
     */
    virtual uint dataID(const CAlfVisual& aVisual) const = 0;

    /**
     * Get the parent element of this element. Some elements may have
     * child elements (@see CAlfRecyclingGridElement). In this case,
     * the parent element is responsible for populating the child elements.
     * Elements with no parent element are called root elements and
     * populated directly by the control.
     *
     * @since S60 ?S60_version
     * @return The parent element, or NULL if this is a root element.
     */
    virtual const IAlfElement* parentElement() const = 0 ;

    /**
     * Set the parent element for this element.
     *
     * @since S60 ?S60_version
     * @param aParent The parent element of this element.
     */
    virtual void setParentElement(IAlfElement& aParent) = 0;

    /**
     * removes and destroys all the visuals
     *
     * @since S60 ?S60_version
     * @param aTimeMilliseconds the animation time for remove.
     */
    virtual void removeAndDestroyVisuals( int aTimeMilliseconds ) = 0;

    /**
     * Checks if referred visual is part of this element.
     *
     * @since S60 ?S60_version
     * @param aVisual reference to visual under inspection.
     */
    virtual bool contains(CAlfVisual& aVisual) const = 0;

    /**
     * Sets the visual template for the element. This element
     * will acquire ownership of the visual template hierarchy
     * under the given visual template.
     * Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     * @param aTemplate Root of the visual template hierarchy to be set
     *                  into the widget element.
     */
    virtual void setVisualTemplate(IAlfVisualTemplate& aTemplate) throw() = 0;
    
    /**
     * Returns the root visual template of the visual template hierarchy
     * owned by this element. Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */    
    virtual IAlfVisualTemplate* getVisualTemplate() const throw() = 0;

    /**
     * Destroyes the visual template object assigned to this element and the whole
     * visual template hierarchy under it. Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     */
    virtual void destroyVisualTemplate() throw() = 0;

    /**
     * Removes the visual template from this element. Returns pointer to the removed visual
     * template object. The ownership of the visual template object is transferred to the
     * caller. Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     *
     * @return The visual template object that was owned by this element. NULL if no visual
     *         template hierarchy was owned by this element.
     */
    virtual IAlfVisualTemplate* removeVisualTemplate() throw() = 0;

    /**
     * Destructor
     */
    virtual ~IAlfElement(){}
    };


/**
 *  Placeholder for information required to instantiate an element
 *  via the widget factory mechanism.
 *  A pointer to this structure is casted to a void pointer and sent to the
 *  factory plugin.
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
struct AlfElementInitData
    {
    /**
     * Owner control for the element.
     */
    IAlfWidgetControl* mControl;
    /**
     * Element instance ID.This uniquely identifies every element instance
     * and is its name. Also see AlfElement::name().
     */
    char* mElementId;
    
    /**
     * Pointer to node in declaration containing information for the element.
     */
    DuiNode* mNode;

    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };


    } // namespace Alf
#endif // I_ALFELEMENT_H

// End of File
