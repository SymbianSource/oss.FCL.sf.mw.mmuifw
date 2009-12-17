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


#ifndef ALFELEMENT_H
#define ALFELEMENT_H

#include <osn/osndefines.h>
#include <memory>
#include <alf/ialfelement.h>
#include <osn/alfptrvector.h>

using osncore::AlfPtrVector;
using std::auto_ptr;

class CAlfVisual;
class CAlfLayout;

namespace Alf
    {
class CAlfWidgetControl;
class IAlfVariantType;
class AlfReferenceToVisual;
class IAlfVisualTemplate;
class AlfElementImpl;
class IfId;

/** @class AlfElement alfelement.h "alf/alfelement.h"
 *  The implementation of a generic element. Elements can be used,
 *  e.g., from declaration, to describe a specific part of
 *  the presentation of a component. The presentation may
 *  be predefined, or it may depend on the component data.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeOwner, IAlfElement
 *	@attributes duiuimodel::tactileattributes::KEventInput\n
 *				duiuimodel::tactileattributes::KFeedbackType\n
 *				duiuimodel::commonvisualattributes::KOpacity\n
 *				duiuimodel::layoutattributes::KPositionX\n
 *				duiuimodel::layoutattributes::KPositionY\n
 *				duiuimodel::layoutattributes::KWidth\n
 *				duiuimodel::layoutattributes::KHeight\n
 *				duiuimodel::layoutattributes::KMaxWidth\n
 *				duiuimodel::layoutattributes::KMaxHeight\n
 *				duiuimodel::layoutattributes::KMinWidth\n
 *				duiuimodel::layoutattributes::KMinHeight
 */
class AlfElement : public IAlfElement
    {
public:


    /**
     * Static cnstructor. The new element is left on the cleanup stack.
     *
     * @since S60 ?S60_version
     * @param aControl  The owner control.
     * @param aName     The name of the element.
     * @return New object. Should be added to the control.
     */
    OSN_IMPORT AlfElement(CAlfWidgetControl& aControl, const char* aName);

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfElement();

    /**
     * Returns name of element set via Constructor.
     *
     * @since S60 ?S60_version
     * @return name of element.
     */
    OSN_IMPORT const char* name() const;

    /**
     * Creates a visual tree in given element.
     *
     * @since S60 ?S60_version
     * @param aElement Element for which visual tree will be created
     * @param aChildData Data for visual tree to be created
     * @param aData Data for parent visual tree
     * @param aIndex Position in parent layout at which new visual tree will be added
     * @param aDataID Parent visual tree's data id
     */
    OSN_IMPORT virtual void createChildVisualTree( IAlfElement* aElement,
                                                  IAlfVariantType& aChildData,
                                                  IAlfBranch& aData,
                                                  int aIndex, uint aDataID );

    /**
     * Creates a visual tree with the data given for visuals.
     *
     * @exception osncore::AlfElementException Thrown with the error code osncore::EInvalidElement\n 
     *										  when creation of visual tree fails.
     * @since S60 ?S60_version
     * @param aChildData Data for visual tree to be created
     * @param aParentData Data for parent visual tree
     * @param aIndex Position in parent layout at which new visual tree will be added
     * @param aParentDataID Parent visual tree's data id
     * @param aParentLayout Parent layout to which newly created visual tree will be added
     * @param aLayoutIndex Position in parent layout at which new visual tree will be added
     */
    OSN_IMPORT virtual CAlfVisual* createVisualTree(
        IAlfVariantType& aChildData, IAlfBranch& aParentData, int aIndex,
        uint aParentDataID, CAlfLayout* aParentLayout, int aLayoutIndex );

    /**
     * Creates a visual tree with the data given for visuals.
     *
     * @exception std::bad_alloc 
     * @exception osncore::AlfVisualException Thrown with the error code osncore::ECanNotCreateVisual\n 
     *										  when creation of visual fails.
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *										  when wrond data id is supplied.
	 * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aData Data for visual tree to be created
     * @param aDataID Parent visual tree's data id
     * @param aParentLayout Parent layout to which newly created visual tree will be added
     * @param aLayoutIndex Position in parent layout at which new visual tree will be added
     */
    OSN_IMPORT virtual CAlfVisual* createVisualTree(
        IAlfVariantType& aData, uint aDataID,
        CAlfLayout* aParentLayout, int aLayoutIndex );

    /**
     * Removes visual tree from given element.
     *
     * @since S60 ?S60_version
     * @param aElement Element from which visual tree is to be removed
     * @param aData
     * @param aIndex 
     * @param aDataID 
     */
    OSN_IMPORT virtual void removeChildVisualTree( IAlfElement* aElement,
                                                  IAlfBranch& aData,
                                                  int aIndex, uint aDataID );

    /**
     * Removes visual tree.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *										  when removal of visual tree fails.
     * @since S60 ?S60_version
     * @param aParentData
     * @param aIndex 
     * @param aParentDataID 
     */
    OSN_IMPORT virtual void removeVisualTree( IAlfBranch& aParentData,
                                             int aIndex, uint aParentDataID );

    /**
     * Removes visual tree.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *										  when removal of visual tree fails.
     * @since S60 ?S60_version
     * @param aData
     * @param aDataID 
     */
    OSN_IMPORT virtual void removeVisualTree( IAlfVariantType& aData,
                                             uint aDataID );

    /**
     * Updates the visual tree.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *										  when a visual can not be created.
     * @since S60 ?S60_version
     * @param aNewData
     * @param aOldData 
     * @param aDataID 
     */
    OSN_IMPORT virtual void updateVisualTree( IAlfVariantType& aNewData,
                                                  IAlfVariantType& aOldData,
                                             uint aDataID );

    /**
     * Replaces the visual tree.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *										  when old data id is wrong.
     * @since S60 ?S60_version
     * @param aData
     * @param aDataID 
     * @param aOldDataID 
     */
    OSN_IMPORT void replaceVisualTree( IAlfVariantType& aData, 
                                       uint aDataID, uint aOldDataID );

    /**
     * Add a new visual tree to the element.
     * Used by derived classes.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *										  when either the visual is NULL or data id is wrong.
     * @exception std::bad_alloc 
     * @since S60 ?S60_version
     * @param aVisualTree visual to be appended.
     * @param aDataID   data id of the visual.
     */
    OSN_IMPORT void addVisualTree(CAlfVisual* aVisualTree, uint aDataID);

    /**
     * Updates the Data ids in an the element.
     *
     * @since S60 ?S60_version
     * @param aParentBranch Data in which IDs to be updated
     * @param aStartIndex Index from which IDs to be updated
     * @param aOffset Amount by which IDs are increased/decreased
     * @param aOldParentDataId Not used.
     * @param aParentDataID Parent data id of the element
     */
    OSN_IMPORT void updateDataIDs( IAlfBranch& aParentBranch, int aStartIndex,
                                 int aOffset, uint aOldParentDataId, uint aParentDataID );

    /**
     * Returns the default parent layout of visual tree in an element.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidVisual\n 
     *										  when wrong data id is supplied.
     * @since S60 ?S60_version
     * @param aParentDataIS
     * @return Parent layout
     */
    OSN_IMPORT CAlfLayout* defaultParentLayout(uint aParentDataID);

    /**
     * Set reference to the default parent layout.
     *
     * @since S60 ?S60_version
     * @param aReference The reference to the default parent layout.
     */
    OSN_IMPORT void setDefaultParentLayout(AlfReferenceToVisual* aReference);

    /**
     * Finds visual with the given name and id in visual tree of element.
     *
     * @since S60 ?S60_version
     * @param aName Name of the visual
     * @param aDataID Data id of the visual
     * @return Visual.Returns NULL if no matching visual is found.
     */
    OSN_IMPORT CAlfVisual* findVisual(const char* aName, uint aDataID);

    /**
     * Get visual from the data id.
     *
     * @since S60 ?S60_version
     * @param aDataID data id of the visual.
     */
    OSN_IMPORT CAlfVisual* findVisual(uint aDataID);
    
    /**
     * Returns the data id of given visual.
     *
     * @since S60 ?S60_version
     * @param aVisual Reference to visual whose data id is needed.
     * @return Data id of the visual
     */
    OSN_IMPORT uint dataID(const CAlfVisual& aVisual) const;

    /**
     * Returns parent element of the element.
     *
     * @since S60 ?S60_version
     * @return Parent element. NULL, if there is no parent element.
     */
    OSN_IMPORT const IAlfElement* parentElement() const;

    /**
     * Makes the element child of given element.
     *
     * @since S60 ?S60_version
     * @param aParent Parent element.
     */
    OSN_IMPORT void setParentElement(IAlfElement& aParent);
    
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
    OSN_IMPORT void setVisualTemplate(IAlfVisualTemplate& aTemplate) throw();
    
    /**
     * Returns the root visual template of the visual template hierarchy
     * owned by this element. Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT IAlfVisualTemplate* getVisualTemplate() const throw();
        
    /**
     * Destroyes the visual template object assigned to this element and the whole
     * visual template hierarchy under it.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT void destroyVisualTemplate() throw();

     /**
     * Removes the visual template from this element. Returns pointer to the removed visual
     * template object. The ownership of the visual template object is transferred to the
     * caller. Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfVisualTemplate* removeVisualTemplate() throw();
    
    /**
     * Getter for the type identifier of this class.
     *
     * @since S60 ?S60_version
     * @return A descriptor to identify the type of this class.
     */

    /**
     * Template getter for interfaces provided by the element classes.
     * The type of the queried interface is specified by the
     * template parameter.
     *
     * @since S60 ?S60_version
     * @param aElement The element to get the interface for.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    template <class T>
    static T* makeInterface( AlfElement* aElement );

// from base class IAlfInterfaceBase

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the element classes.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT virtual IAlfInterfaceBase* makeInterface( const IfId& aType );
    
    /**
     * removes and destroys all the visuals in the element.
     *
     * @since S60 ?S60_version
     * @param aTimeMilliseconds the animation time for remove.
     */    
    OSN_IMPORT void removeAndDestroyVisuals( int aTimeMilliseconds );

    /**
     * from IAlfElement
     */
    OSN_IMPORT bool contains(CAlfVisual& aVisual) const;
    
protected:

    /**
     * The protected constructor.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfElement();

    /**
     * The second phase constructor.
     * The element is added to the control.
     *
	 * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aControl  The owner control.
     * @param aName     The name of the element.
     */
    OSN_IMPORT void construct( CAlfWidgetControl& aControl, const char* aName );

    /**
     * APi to get the control containing the element.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT CAlfWidgetControl& control();

private:

    int findFromArray( uint aDataID ) const;
    
private: // data

    auto_ptr<AlfElementImpl> mData;
    };

#include "alf/alfelement.inl"

    } // namespace Alf
#endif // ALFELEMENT_H
