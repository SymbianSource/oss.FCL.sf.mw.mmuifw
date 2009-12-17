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
* Description:  Visual template class for alfred widget model.
*      This class creates and updates visual tree.
*
*/


#ifndef ALF_VISUALTEMPLATE_H
#define ALF_VISUALTEMPLATE_H

#include <osn/osndefines.h>
#include <alf/alflayout.h>
#include "alf/ialfvisualtemplate.h"
#include <osn/ustring.h>
#include <osn/alfptrvector.h>
#include <alf/ialfinterfacebase.h>
#include <vector>

class CAlfControl;
class CAlfBrush;
class CAlfVisual;

using namespace osncore;

namespace Alf
    {
// FORWARD DECLARATIONS
class IAlfMap;
class IAlfAttributeSetter;
class AlfAttributeContainer;

/**
 * @class AlfVisualTemplate alfvisualtemplate.h "alf/alfvisualtemplate.h"
 * A generic visual template for creating or updating a visual tree.
 * The visual templates may form a tree structure that matches
 * the visual tree to be created or updated. Each template is
 * responsible for creating and initializing a single visual.
 * Visual templates contain a number of attribute setters
 * to initialize or update the visuals. Attribute setters use values
 * from the corresponding attribute containers.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 * @interfaces IAlfVisualTemplate
 */
class AlfVisualTemplate : public IAlfVisualTemplate
    {
public:

    /**
     * Constructor for a template for creating a visual.
     * @exception std::bad_alloc
     *            
     * @since S60 ?S60_version
     *
     * @param aType  The type of the visual to create.
     */
    OSN_IMPORT static AlfVisualTemplate* create(TAlfVisualType aType);

    /**
     * Constructor for a template for creating a layout.
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     *
     * @param aType  The type of the layout to create.
     */
    OSN_IMPORT static AlfVisualTemplate* create(TAlfLayoutType aType);

    /**
     * Constructor for an empty template.
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     *
     */
    OSN_IMPORT static AlfVisualTemplate* create();

    // Destructor.
    OSN_IMPORT virtual ~AlfVisualTemplate();

	//From IAlfVisualTemplate
         
    /**
     * Sets the owner element. User should not call this method directly. It will
     * be called implicitly by Alf::IAlfElement::setVisualTemplate() method.
     * Call to this method cannot throw an exception.
     *
     * @see Alf::IAlfElement::setVisualTemplate()
     *
     * @since S60 ?S60_version
     *
     * @param aOwner Element that owns this root visual template.
     */
    OSN_IMPORT void setOwner(IAlfElement* aElement) throw();
    
    /**
     * Returns the owner element. Call to this method cannot
     * throw an exception.
     *
     * @see Alf::IAlfElement::setVisualTemplate()
     *
     * @since S60 ?S60_version
     *
     * @return The owner element of a visual template, if this visual template is owned
     *         by an element. NULL if visual template is not owned by an element.
     */
    OSN_IMPORT IAlfElement* owner() const throw();

    /**
     * Set name of the visual. This name is used when creating
     * concrete visual trees. Name is assigned to each of the produced concrete
     * visuals as a tag. Note thus that all concrete visual
     * trees produced from this visual template share the same tag.
     *
     * @see name()
     *
     * @exception std::bad_alloc thrown if heap memory allocation fails.
     *
     * @since S60 ?S60_version
     *
     * @param aName The name of the visual.
     */
    OSN_IMPORT void setName(const char* aName);
    
    /**
     * Returns the name of the visual. Call to this method
     * cannot throw an exception.
     *
     * @see setName()
     *
     * @since S60 ?S60_version
     *
     * @return aName The name of the visual.
     */
    OSN_IMPORT const char* name() const throw();

    /**
     * Get the number of child visual templates.
     * This returns the number of child visual templates directly
     * under this visual template in the visual template
     * hierarchy. Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     *
     * @return The number of direct child visual templates
     *         under this visual template in the visual template hierarchy.
     */
    OSN_IMPORT int numChildTemplates() const throw();

    /**
     * Returns a child visual template at a given index.
     *
     * @exception osncore::AlfException thrown with error code osncore::EInvalidArrayIndex if
     *            the index is out of bounds.
     *
     * @since S60 ?S60_version
     *
     * @param aIndex The index of the child visual template.
     *
     * @return Reference to the child visual template object.
     */
    OSN_IMPORT IAlfVisualTemplate& childTemplate(int aIndex) const;

    /**
     * Add a child visual template. The visual templates may form
     * a tree structure to create and update a visual tree. The visuals
     * created by children are added to the layout created by this
     * visual template. This visual template acquires the ownership for the
     * child visual template passed as an argument.
     *
     * If a visual template added as a child to this visual template is owned
     * by an Alf::IAlfElement the connection between the child visual template
     * and its owner element is implicitly removed in this call when a 
     * parent-child connection is created. Effectively having a parent visual
     * template or being owned by an element are mutually exclusive properties
     * for a visual template. Elements can own only root visual templates and
     * if an owned root visual template becomes a child of another visual 
     * template the visual template will be removed from the associated owner
     * element.
     *
     * @see insertChildTemplate()
     *
     * @exception std::bad_alloc Thrown if memory allocation fails in appending the child
     *            visual template to child array of this visual template.
     * @exception osncore::AlfException thrown with error code osncore::EInvalidHierarchy if
     *            the call to this method would result in an invalid visual template hierarchy.
     *            This can occur if a child is added to a non-layout visual template or if a
     *            call would result in a parent becoming a descendant of it's child (a recursive
     *            tree).
     *
     * @since S60 ?S60_version
     *
     * @param aChild Child visual template to be appended to the children of this visual template.
     */
    OSN_IMPORT void addChildTemplate(IAlfVisualTemplate& aChild);
    
    /**
     * Inserts a child visual template at given index. The visual templates may form
     * a tree structure to create and update a concrete visual tree. The concrete visuals
     * created by children are added to the layout created by this
     * visual template. This visual template acquires the ownership of the inserted
     * child visual template.
     *
     * @see addChildTemplate()
     *     
     * @exception std::bad_alloc thrown if memory allocation fails in inserting the child visual
     *            template to the child list of this visual template.
     * @exception osncore::AlfException thrown with error code osncore::EInvalidArrayIndex when
     *            index supplied is out of range for array of visual templates.
     * @exception osncore::AlfException thrown with error code osncore::EInvalidHierarchy if
     *            the call to this method would result in an invalid visual template hierarchy.
     *            This can occur if a child is added to a non-layout visual template or if a
     *            call would result in a parent becoming a descendant of it's child (a recursive
     *            tree) or if the same child visual template is added to the hierarchy twice.
     *
     * @since S60 ?S60_version
     *
     * @param aChild  The child visual template.
     * @param aIndex  Position at which child template is to be added.
     */
    OSN_IMPORT void insertChildTemplate(IAlfVisualTemplate& aChild, int aIndex);

    /**
     * Removes and destroyes a child visual template from an index in the child list
     * of this visual template. This will recursively also destroy all child visual templates
     * in the hierarchy below the destroyed child visual template. This call is silently
     * ignored if the index is out of bounds. Call to this method cannot throw an
     * exception.
     *
     * @since S60 ?S60_version
     *
     * @param aIndex Position from which child visual template is to be removed.
     */
    OSN_IMPORT void destroyChildTemplate(int aIndex) throw();

    /**
     * Destroyes a child visual template with the given name and removes it from the children
     * of this visual template. This will recursively also destroy all child visual templates
     * in the hierarchy below the destroyed child visual template. This call is silently
     * ignored if a child visual template with the given name is not found. Call
     * to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     *
     * @param aName Name of child visual template to be destroyed.
     */
    OSN_IMPORT void destroyChildTemplate(const char* aName) throw();

    /**
     * Add a new attribute setter and associated attribute container to set visual attributes
     * of the concrete visual hierarchies created or updated with this visual template hierarchy.
     * This visual template object acquires ownership of both the passed attribute setter as well
     * as the attribute container. Attribute containers in a visual template are iterated through
     * whenever concrete visual trees are created or updated. Concrete visual tree attributes
     * are set from the data passed in through the createVisualTree() and updateVisualTree() methods
     * using the attribute setters and containers added into the visual template using this method.
     *
     * @see createVisualTree()
     * @see updateVisualTree()
     *
     * @exception std::bad_alloc thrown if heap memory allocation fails.
     *
     * @since S60 ?S60_version
     *
     * @param aSetter Attribute setter that will be used to set the attributes in the passed in
     *                container to concrete visuals.
     * @param aContainer Container holding the attribute value used by the attribute setter.
     */
    OSN_IMPORT void addAttributeSetter(IAlfAttributeSetter* aSetter,
                                       AlfAttributeContainer* aContainer);

    /**
     * Get the number of the attribute setters and the corresponding attribute containers.
     * Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     *
     * @return The number of the attribute setters and containers.
     */
    OSN_IMPORT int numAttributeSetters() const throw();

    /**
     * Returns an attribute setter maintained in this visual template from the given index.
     *
     * @exception osncore::AlfException thrown with error code osncore::EInvalidArrayIndex if
     *            the given index is out of bounds.
     *
     * @since S60 ?S60_version
     *
     * @param aIndex The index of the visual attribute setter.
     * @return Reference to the attribute setter object.
     */
    OSN_IMPORT IAlfAttributeSetter& attributeSetter( int aIndex ) const;

    /**
     * Returns an attribute container maintained in this visual template from the given index.
     *
     * @exception osncore::AlfException thrown with error code osncore::EInvalidArrayIndex if
     *            the given index is out of bounds.
     *
     * @since S60 ?S60_version
     *
     * @param aIndex The index of the visual attribute container.
     * @return Reference to the attribute container object.
     */
    OSN_IMPORT AlfAttributeContainer& attributeContainer( int aIndex ) const;

    /**
     * Update an existing concrete visual tree with new values. The existing visual tree
     * should be produced from this visual template hierarchy using the createVisualTree() -
     * method.
     * 
     * @see createVisualTree()
     * @see setSelectChildMode()
     * @see addAttributeSetter()
     *
     * @since S60 ?S60_version
     *
     * @param aData A map containing new data for the visuals.
     * @param aOldData Old data, that will be replaced. This is used to for instance tear down
     *                 the old selected child branch before creating a new branch in the selected
     *                 child mode.
     * @param aVisual The root of the concrete visual tree that will be updated. This root visual
     *                needs to match this visual template and should have been created using this
     *                visual template.
     * @return Root visual of the updated concrete visual tree.
     */
    OSN_IMPORT CAlfVisual* updateVisualTree(IAlfMap* aData,
                                            IAlfMap* aOldData, CAlfVisual& aVisual);

    /**
     * Creates a new concrete visual tree from this visual template hierarchy and initializes the visuals.
     * Created visual tree matches the visual template hierarchy under this visual template.
     *
     * @see updateVisualTree()
     * @see setSelectChildMode()
     * @see addAttributeSetter()
     *
     * @since S60 ?S60_version
     *
     * @param aControl The owner control of the new concrete visual hierarchy.
     * @param aData A map containing data for the visuals. This is used to set the attributes of the
     *              produced concrete visuals using the attribute containers and attribute setters
     *              added through the addAttributeSetter() - method.
     * @param aParentLayout Concrete layout visual to insert the created concrete visual tree into.
     *                      Parent layout for a created visual.
     * @param aLayoutIndex Ordinal index, into where the created visual should be placed in the parent layout.
     *
     * @return Pointer to the root visual object of the concrete visual hierarchy that was constructed.
     */
    OSN_IMPORT CAlfVisual* createVisualTree(CAlfControl& aControl,
                                            IAlfMap* aData, CAlfLayout* aParentLayout, int aLayoutIndex);

    /**
     * Set the visual type. Every visual template maps to
     * one concrete visual or layout type. Each visual template
     * node in the visual template hierarchy can produce
     * only visuals that match its type. Container nodes
     * in the visual template hierarchy map to layouts
     * and leaf visual template nodes map to visuals.
     *
     * Call to this method will make this visual template 
     * a visual type. The type can thus change from layout
     * type to a visual type.
     *
     * @see setLayoutType()
     * @see visualType()
     * @see layoutType()
     *
     * @since S60 ?S60_version
     *
     * @param aType Type of visual
     */
    OSN_IMPORT void setVisualType(TAlfVisualType aType);

    /**
     * Get the visual type
     *
     * @since S60 ?S60_version
     *
     * @return aType Type of visual
     */
    OSN_IMPORT int visualType() const throw();

    /**
     * Set the layout type. Every visual template maps to
     * one concrete visual or layout type. Each visual template
     * node in the visual template hierarchy can produce
     * only visuals that match its type. Container nodes
     * in the visual template hierarchy map to layouts
     * and leaf visual template nodes map to visuals.
     *
     * Call to this method will make this visual template 
     * a layout type. The type can thus change from visual
     * producer to a layout producer.
     *
     * @see setVisualType()
     * @see visualType()
     * @see layoutType()
     *
     * @since S60 ?S60_version
     *
     * @param aType Type of layout
     */
    OSN_IMPORT void setLayoutType(TAlfLayoutType aType);

    /**
     * Get the layout type or a negative integer
     * if this visual template is of visual type.
     * Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     *
     * @return Type of layout that this visual template produces
     *         or a negative integer if this visual template
     *         produces visuals.
     */
    OSN_IMPORT int layoutType() const throw();

    /**
     * Adds brush to the brush array.
     *
     * All brushes added to a visual template will be applied to
     * the produced concrete visual when one is produced. The same
     * brush array is shared across all concrete visual instancies
     * created from same visual template.
     * 
     * @see brush()
     * @see numBrushes()
     *
     * @exception std::bad_alloc thrown if a heap memory allocation fails.
     *
     * @since S60 ?S60_version
     *
     * @param aBrush Brush to be added
     */
    OSN_IMPORT void addBrush(CAlfBrush& aBrush);

    /**
     * Get the munber of brushes
     *
     * @since S60 ?S60_version
     *
     * @return The number of brushes
     */
    OSN_IMPORT int numBrushes() const;

    /**
     * Returns a brush instance from the array of brushes.
     *
     * @see addBrush()
     * @see numBrushes()
     *
     * @exception osncore::AlfException thrown with error code osncore::EInvalidArrayIndex if
     *            the given index is out of bounds.
     *     
     * @since S60 ?S60_version
     *
     * @param aIndex The index into the array of brushes.
     *
     * @return Reference to the brush object instance at the given array index.
     */
    OSN_IMPORT CAlfBrush& brush(int aIndex) const;

    /**
     * Enables or disables selected child mode. By default
     * this is disabled and thus visual template creates or updates
     * all its children when createVisualTree() or updateVisualTree()
     * is called. If selected child mode is enabled the visual template 
     * will create or update only the selected child. The child which 
     * is affected in selected child mode is determined in the data passed
     * in the createVisualTree() or updateVisualTree() method. In the 
     * passed data map a search is done to match the child index 
     * field name with the keys in the map. If an entry is found its value
     * is used as an index to determine the child branch that is to be 
     * created or updated.
     *
     * @see createVisualTree()
     * @see updateVisualTree()
     *
     * @since S60 ?S60_version
     *
     * @param aSelectChild Set to true to enable the child mode or false to disable the child mode.
     * @param aChildIndFieldName Key in data map, from which entry the index of
     *                           the selected child is fetched.
     */
    OSN_IMPORT void setSelectChildMode(bool aSelectChild,
                                       const UString& aChildIndFieldName );

    //From IAlfInterfaceBase
    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the visual template.
     * Derived classes should always call the base class method
     * from the overridden makeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT virtual IAlfInterfaceBase* makeInterface( const IfId& aType );

    /**
     * Removes the given child visual template from this visual template.
     * Call to this method is silently ignored if the given visual template
     * is not found from children. The parent template of the removed child
     * is set to null. Ownership of the removed child visual template is
     * transferred to the caller of this method. Call to this method cannot
     * throw an exception.
     *
     * @see setParent()
     * @see parent()
     *
     * @param aChild Reference to the child visual template that is located
     *               and removed from this visual template.
     */
    void removeChildTemplate(IAlfVisualTemplate& aChild) throw();
    
    /**
     * Retrieves pointer to the parent visual template.
     * Returns NULL if this is a root visual template of a visual template
     * hierarchy. Call to this method cannot throw an exception.
     *
     * @see addChildTemplate()
     * @see insertChildTemplate()
     * @see Alf::IAlfElement::setVisualTemplate()
     *
     * @return Pointer to a Alf::IAlfVisualTemplate object that this visual template
     *         is a child of.
     */
    OSN_IMPORT IAlfVisualTemplate* parent() const throw();
        
    /**
     * Sets the parent visual template.
     *
     * User should not call this method directly. It will be called implicitly
     * by addChildTemplate() or insertChildTemplate() methods of
     * the parent visual template into which this visual template is
     * added to.
     *
     * Parent visual template and owner element are mutually exclusive.
     * If a visual template is added under another visual template
     * in a visual template hierarchy the connection to owner element
     * is implicitly removed. Alf::IAlfElement objects can thus own only
     * roots of visual template hierarchies.
     *
     * Call to this method cannot throw an exception.
     * 
     * @see addChildTemplate()
     * @see insertChildTemplate()
     * @see parent()
     * @see Alf::IAlfElement::setVisualTemplate()
     *
     * @param aParent Pointer to a parent visual template under which this
     *                visual template is located in in the visual template
     *                hierarchy or null, if the visual template is removed from
     *                a visual template hierarchy.
     */
    OSN_IMPORT void setParent(IAlfVisualTemplate* aParent) throw();

protected:
    /**
     * c++-constructor for a template for creating a visual.
     *
     * @since S60 ?S60_version
     *
     * @param aType  The type of the visual to create.
     */
    OSN_IMPORT AlfVisualTemplate(TAlfVisualType aType);

    /**
     * c++-constructor for a template for creating a layout.
     *
     * @since S60 ?S60_version
     *
     * @param aType  The type of the layout to create.
     */
    OSN_IMPORT AlfVisualTemplate(TAlfLayoutType aType);

    /**
     * c++-constructor for an empty template.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfVisualTemplate();

private:
    /**
     * returns the index to child, that should be created. The index is
     * fetched from the data.
     *
     * @since S60 ?S60_version
     *
     * @param aData data-map, where the index is fetched.
     * @return  index to child, that should be created.
     */
    int selectedChildInd(IAlfMap* aData);
    
private:

    // Type of the visual to create.
    // Negative values are layout types, positive values visual types.
    int mVisualType;

    // The array containing child visual templates. Owned.
    std::vector<IAlfVisualTemplate*> mChildren;

    // The tag of the visual to create.
    UString mVisualName;

    // The array containing visual attribute setters. Owned.
    AlfPtrVector<IAlfAttributeSetter> mAttributeArray;

    // The array containing attribute containers.
    AlfPtrVector<AlfAttributeContainer> mContainerArray;

    //The array containing brushes. Owned.
    AlfPtrVector<CAlfBrush> mBrushArray;

    //if true, creates only one child visual tree.
    bool mSelectOneChild;

    //field name in data where child index is read.
    //if iSelectOneChild is true, index points to child in childarray, to create.
    UString mChildIndFieldName;
    
    // Owner element of this visual template. Not owned.
	IAlfElement* mOwner;
	
	// Pointer to the parent visual template.
	IAlfVisualTemplate* mParent;
    };

    } // namespace Alf

#endif //ALF_VISUALTEMPLATE_H
