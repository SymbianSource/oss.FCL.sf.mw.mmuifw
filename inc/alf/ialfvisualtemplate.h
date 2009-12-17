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


#ifndef I_ALFVISUALTEMPLATE_H
#define I_ALFVISUALTEMPLATE_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>
#include <osn/ustring.h>
#include <alf/alflayout.h>

class CAlfVisual;
class CAlfControl;
class CAlfBrush;
class CAlfLayout;

namespace duiuimodel
    {
    //FORWARD DECLARATIONS
    class DuiNode;	
    }	
using namespace duiuimodel;
using namespace osncore;
    
namespace Alf
    {
// FORWARD DECLARATIONS
class IAlfMap;
class IAlfAttributeSetter;
class IAlfElement;
class AlfAttributeContainer;
class AlfCustomInitDataBase;  

namespace alfvisualtemplate
    {
static const IfId ident =
    {
    0, "alfvisualtemplate"
    };
    }
    
//INTERFACE DECLARATIONS

/**
 * An interface for visual templates.
 *
 * A visual template hierarchy can be formed using visual template objects. From a visual template
 * hierarchy a concrete visual tree can be constructed that composes of toolkit visuals.
 * Produced concrete visual tree will match the visual template hierarchy constructed by
 * adding visual templates as children of each other. User can create any number of identical 
 * visual trees from one visual template hierarchy. Each visual template object creates 
 * one visual in the produced concrete visual tree.
 *
 * A visual template can be owned by a widget element (Alf::IAlfElement).
 * Only root visual templates of a visual template hierarchy can be owned by an element.
 * As a consequence of this an owner element of a visual template and a parent visual template
 * are mutually exclusive properties. That is, if a visual template is a child of
 * another visual template it cannot be owned by an element. Also if an element
 * owns a visual template it cannot be a child of another visual template.
 *
 * If a child visual template is set to an element the connection between child visual template
 * and its parent is removed thus making the visual template a root of the visual template hierarchy
 * below it. Similarly if a visual template owned by an element is set as a child to a visual template
 * the ownership of that visual template by its element is removed leaving the element with no visual
 * template. 
 *
 * Concrete visual trees can be produced using createVisualTree() method. Existing concrete visual
 * trees can be updated using the updateVisualTree() method. Attributes of the produced concrete 
 * visuals can be manipulated through attribute setters and containers assigned to visual template
 * using method addAttributeSetter(). Data that is passed to the createVisualTree() and updateVisualTree()
 * methods contain the attribute values that are set to the concrete visuals giving users a 
 * possibility to customize the produced concrete visual trees.
 *
 * @see Alf::IAlfElement
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfVisualTemplate : public IAlfInterfaceBase
    {
public:
    /**
     * Getter for the type identifier of this interface.
     *
     * @since S60 ?S60_version
     * @return Identifier of this interface.
     */
    static inline const IfId& type()
        {
        return alfvisualtemplate::ident;
        }

    /** Virtual destructor. */
    virtual ~IAlfVisualTemplate() {}
    
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
    virtual void setOwner(IAlfElement* aElement) throw() = 0;
    
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
    virtual IAlfElement* owner() const throw() = 0;
    
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
    virtual void setName(const char* aName) = 0;
    
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
    virtual const char* name() const throw() = 0;

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
    virtual int numChildTemplates() const throw() = 0;

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
    virtual IAlfVisualTemplate& childTemplate(int aIndex) const = 0;
    
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
    virtual void setVisualType(TAlfVisualType aType) = 0;

    /**
     * Get the visual type or a negative integer
     * if this visual template is of layout type.
     * Call to this method cannot throw an exception.
     *
     * @see setVisualType()
     *
     * @since S60 ?S60_version
     *
     * @return Type of visual that this visual template produces
     *         or a negative integer if this visual template
     *         produces layouts.
     */
    virtual int visualType() const throw() = 0;

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
    virtual void setLayoutType(TAlfLayoutType aType) = 0;

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
    virtual int layoutType() const throw() = 0;

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
    virtual void addBrush(CAlfBrush& aBrush) = 0;

    /**
     * Returns the number of brushes in the brush array.
     *
     * @since S60 ?S60_version
     *
     * @return The number of brushes
     */
    virtual int numBrushes() const = 0;

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
    virtual CAlfBrush& brush(int aIndex) const = 0;

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
    virtual void setSelectChildMode(bool aSelectChild, const UString& aChildIndFieldName) = 0;

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
    virtual void addChildTemplate(IAlfVisualTemplate& aChild) = 0;

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
    virtual void insertChildTemplate(IAlfVisualTemplate& aChild, int aIndex) = 0;

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
    virtual void destroyChildTemplate(int aIndex) throw() = 0;

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
    virtual void destroyChildTemplate(const char* aName) throw() = 0;

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
    virtual CAlfVisual* updateVisualTree(IAlfMap* aData, IAlfMap* aOldData, CAlfVisual& aVisual) = 0;

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
    virtual CAlfVisual* createVisualTree(CAlfControl& aControl,
                                         IAlfMap* aData, CAlfLayout* aParentLayout, int aLayoutIndex) = 0;

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
    virtual void addAttributeSetter(IAlfAttributeSetter* aSetter, AlfAttributeContainer* aContainer) = 0;

    /**
     * Get the number of the attribute setters and the corresponding attribute containers.
     * Call to this method cannot throw an exception.
     *
     * @since S60 ?S60_version
     *
     * @return The number of the attribute setters and containers.
     */
    virtual int numAttributeSetters() const throw() = 0;

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
    virtual IAlfAttributeSetter& attributeSetter(int aIndex) const = 0;

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
    virtual AlfAttributeContainer& attributeContainer(int aIndex) const = 0;
         
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
    virtual IAlfVisualTemplate* parent() const throw() = 0;
    
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
    virtual void removeChildTemplate(IAlfVisualTemplate& aChild) throw() = 0;    

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
    virtual void setParent(IAlfVisualTemplate* aParent) throw() = 0;
    
    };
    
/**
 * Placeholder for information required to instantiate a visual template
 * via the widget factory mechanism.
 * A pointer to this structure is casted to a void pointer and sent to the
 * factory plugin.
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
struct AlfVisualTemplateInitData
    {
    /**
     * Visual template instance ID.This uniquely identifies every visual template instance
     */
    char* mVisualTemplateId;

    /**
     * Pointer to node in declaration containing information for the widget.
     */
    DuiNode* mNode;

    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };

    } // namespace Alf

#endif // I_ALFVISUALTEMPLATE_H

// End of File
