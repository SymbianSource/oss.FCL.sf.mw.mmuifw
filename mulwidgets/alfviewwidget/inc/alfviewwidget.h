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
* Description:   Widget class header.
*
*/


#ifndef ALFVIEWWIDGET_H
#define ALFVIEWWIDGET_H

#include "alf/alfwidget.h"
#include <alf/ialfviewwidget.h>
#include <AknsItemID.h>
#include <alf/alfeventhandler.h>
#include <alf/alfenv.h>

#include <osn/ustring.h>
#include <osn/alfptrvector.h>
// Forward declarations
class CAlfControlGroup;

namespace dui
    {
    class DuiNode;
    }

namespace Alf
{

class AlfWidget;
class AlfViewWidgetZOrderChangedObserver;

/**
 * View widget implementation.
 *
 * View widget can be used to contain other widgets in an arbitrary layout.
 *
 * View widget creates a control group with ID passed in its constructor and
 * stores it's control in it.
 */
class AlfViewWidget : public IAlfViewWidget, public MAlfActionObserver,MAlfEventHandler
{
public:

    /**
     * Default constructor.
     * 
     * Use AlfViewWidgetFactoryPlugin::CreateProduct() to instantiate a view
     * widget. View widget will create a new control group from the ID passed to this
     * constructor and append the view control into the control group.
     *
     * @exception AlfException Error code EInvalidArgument is thrown if a control group already
     *            exists with the given ID.
     * @exception AlfViewWidgetException with Symbian Error code if the view 
     *            is unable to register itself as an action observer in the 
     *            Alfred Environment.
     * 
     * @param aWidgetName Name of the constructed view widget.
     * @param aControlGroupId ID of the control group to append the view widget control into.     
     * @param aEnv UI Accelerator Toolkit environment in which this widget is defined.
     * @param aDisplay Display in which this widget is defined.
     */
	AlfViewWidget(const char* aWidgetName, int aControlGroupId, CAlfEnv& aEnv, 
            CAlfDisplay& aDisplay, DuiNode* aNode=NULL, const char *aFilePath=NULL);
    
    /**
     * Default destructor.
     */
    virtual ~AlfViewWidget();

    // From Alf::IAlfWidget base class

    /**
     * Getter for the control. The ownership is not passed.
     *
     * @since S60 5.0
     * @return The control of this widget, or NULL if the control is not specified.
     */
    virtual CAlfWidgetControl* control() const;
    
    /**
     * getter for the parent container. The ownership is not passed.
     * @return the parent container or NULL if the widget is the root-widget.
     */
    virtual IAlfContainerWidget* parent() const;

    /**
     * Setter for the control. The control is owned by the Alfred environment.
     * The previously set control can be destroyed when new control is set.
     *
     * The set control is added to the control group of this view widget.
     * The set control is removed from it's previous control group.
     *
     * @since S60 5.0
     * @param aControl The control for this widget.
     * @param aDeletePreviousControl A boolean to indicate whether the previously set control
     *          is to be destroyed when this method is called.
     */
    virtual void setControl(CAlfWidgetControl* aControl, bool aDeletePreviousControl = true);

    /**
     * Getter for the model. The ownership is not passed.
     *
     * @since S60 5.0
     * @return The model of this widget, or NULL if the model is not specified.
     */
    virtual IAlfModel* model();

    /**
     * Setter for the model. Ownership is passed and the old model is released.
     *
     * @since S60 5.0
     * @param aModel The model for this widget.
     * @param aTakeOwnership Whether takes ownership of model or not
     */
    virtual void setModel(IAlfModel* aModel, bool aTakeOwnership = true);

    /**
     * Get the name of the widget instance.
     *
     * @since S60 5.0
     * @return Widget name
     */
    virtual const char* widgetName() const;

    /**
     * Sets/Releases the Focus from child widget of view.
     * Does not set the actual focused status in roster.
     *
     * @since S60 5.0
     * @param aFocus boolean value for focused status
     */
    virtual void setChildFocus(bool aFocus);

    // From Alf::IAlfContainerWidget base class

    /**
     * Returns total number of widgets contained by this view.
     *
     * @return int Total number of widgets.
     */
    virtual int widgetCount() const;

    /**
     * Adds a child widget to the view.
     *
     * Ownership of the widget is transferred to this view
     * object.
     *
     * @param  aWidget Child widget to be added to view.
     */
    virtual void addWidget(IAlfWidget& aWidget);

    /**
     * Returns child widget at given index.
     *
     * If index is not found returns null.
     *
     * @param  aIndex Index of widget to be returned.
     * @return Widget at given index or null if not found.
     */
    virtual IAlfWidget* getWidget(int aIndex) const;
    
    /**
     * Returns the child index, when given the child widget.
     *
     * @param  aWidget child widget, which index is returned 
     * @return the index of the child widget, or -1, if widget is not child of this container.
     */
    virtual int getWidgetIndex(IAlfWidget& aWidget) const;

    /**
     * Removes the child widget from given index.
     *
     * This will destroy the widget object at the given index.
     *
     * @param aIndex Index from which widget has to be removed.
     */
    virtual void removeWidget(int aIndex);

    /**
     * Sets the base layout of view widget.
     *
     * @param aLayout New base layout of view widegt.
     */
    virtual void applyLayout(IAlfLayoutManager& aLayout);

    /** 
     * Sets the presentation for the widget using presentation XML file.
     * Destroys any existing presentation.
     * @param aFilePath Path to XML file describing the presentation 
     *                  of the widget. Not Owned.
     * @exception AlfWidgetException if no visualization node available 
     *                               in Presentation XML or if no control
     *                               is associated with the widget.
     * @exception bad_alloc 
     * @since S60 ?S60_version
     * @return void.
     */
    virtual void setPresentation(const char* aFilePath);    


    //--------------------------------------------------------------------------------------//
    /* From IAlfViewWidget*/
    //--------------------------------------------------------------------------------------//


	/**
	 * API to show the view widget.
	 * @param aShow	true if the view is to be shown, else false
	 *				The view is not shown, till this api is called 
	 *              with true parameter.    
	 * @since S60 ?S60_version
	 */
	virtual void show(bool aShow = true);


    /**
     * API to make the view widget accept events.
     *
     * @param aAccept true if the view is to accept inputs, else false
     *     default value is true
     * @since S60 ?S60_version
     */
    virtual void acceptEvents(bool aAccept = true);

    /**
     * API to hide/show Avkon status pane.
     * If the AlfDisplay bound to the view widget does not occupy the 
     * entire client rectangle, this API has no effect. 
     *
     * @param aEnable true to enable the status pane, else false
     *        the default value is true.
     * @since S60 ?S60_version
     */
    virtual void enableStatusPane(bool aEnable = true);

    /**
     * API to hide/show Avkon control pane
     * If the AlfDisplay bound to the view widget does not occupy the 
     * entire client rectangle, this API has no effect.
     *
     * @param aEnable true to enable the control pane, else false
     *     the default value is true.
     * @since S60 ?S60_version
     */
    virtual void enableControlPane(bool aEnable = true);

    /**
     * API to enable/disable Avkon Skin to be used as view widget's background.
     *
     * @param aSkinBackground true if the view has to use Avkon Skin as background.
     *                        false otherwise.
     * @since S60 ?S60_version
     */
    virtual void useSkinBackground(bool aSkinBackground = true);

    /**
     * API to use Skin with the given ID to be used as view widget's background.
     * Also enables skin background. See IAlfViewWidget::useSkinBackground()
     *
     * @param aID Skin id of the graphics to be used in the texture.
     *
     * @since S60 ?S60_version
     */
    virtual void setSkinBackground(TAknsItemID aSkinID);

    /**
     * API to check whether the view is shown
     *
     * @return true if the view is shown, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool shown();
    
    /**
     * Returns the position of the view on the view stack.
     * This tells the view Z-coordinate position relative to other views.
     * Top-most view returns zero. Views under that have a growing
     * number of position. If the view is hidden -1 is returned.
     * Only view widgets are taken into consideration in the position calculation.
     * Other control groups in the display roster are ignored. For example,
     * position zero doesn't mean that the control group of this view would be
     * top-most.
     *
     * Only top-most view is activate while all other views are being deactivate.
     * 
     * @return Position of the view, -1 if view is hidden.
     */
    virtual int getViewStackPosition() const;

    /**
     * API to check whether the view accepts events
     *
     * @return true if the view accepts events, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool eventsAccepted();

    /**
     * API to check whether the status pane is enabled
     *
     * @return true if the status pane is enabled, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool statusPaneEnabled();

    /**
     * API to check whether the control pane is enabled
     *
     * @return true if the control pane is enabled, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool controlPaneEnabled();
    
	/**
	 * API to check whether skin info is being used for background
	 * or not
	 *
	 * @return true if the skin info is being used, else false 
	 *
	 * @since S60 ?S60_version
	 */
	virtual bool usingSkinBackground();

	/**
     * API to get the Avkon Skin ID being used as view widget's background.
     * Use IAlfViewWidget::usingSkinBackground() to check if this is 
     * being used.       
     * 
     * @return Avkon Skin ID being used for view's background.
     *         Returns an ID with major and minor fields=-1 if 
     *         this info was never set. 
     *         See IAlfViewWidget::setSkinBackground()
     *
     * @since S60 ?S60_version
	 */
	virtual TAknsItemID skinBackground();

    // From Alf::IAlfInterfaceBase base class

    /**
     * Interface getter.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 5.0
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface(const IfId& aType);
    
    // New methods
    
    /**
     * Called to notify the view of view activation (view becoming top most in
     * z-order) or deactivation. The view can be deactivated multiple times
     * in a row without being reactivated.
     *
     * This method is called internally by the view widget itself.
     * 
     * @see getViewStackPosition()
     * 
     * @param aIsActivated Indicates whether view widget is activated or not
     */
    void notifyViewActivated(bool aIsActivated);
    
    //From MAlfActionObserver
    /**
     * @see MAlfActionObserver
     */ 
    void HandleActionL(const TAlfActionCommand& aActionCommand);
    
    /**
     * API to Set the display area of the view widget.
     * This will set the alfdisplay area with param passed
     * Client need to call this api with screen/apprect to make 
     * view full screen or application rect
     *
     * @param aDisplayRect: the area in which view widget should be
     *                      displayed
     *
     * @since S60 ?S60_version
     */
    void setRect(const TRect& aDisplayRect);
    
   /**
    * Updates the array which contains the name of all the children widgets
    * 
    * @param aChildName the name of a child widget
    */ 
    void setChildNameToDelete( UString aChildName );
    
private:

    /**
     * Constructs widget from declaration.
     *
     * @param aEnv The environment for the widget.
     * @param aNode declaration node.
     */
    void constructComponentsFromNode(CAlfEnv& aEnv);
    
    /**
     * Constructs widget using the presentation XML.
     *
     * @param aEnv The environment for the widget.
     * @param aFilePath Path to XML file describing the presentation of the widget.
     */
    void constructFromPresentationXML(CAlfEnv& aEnv, const char *aFilePath);

    /**
     * Constructs default widget
     *
     * @param aEnv The environment for the widget.
     */
    void constructDefault(CAlfEnv& aEnv, bool aCreateLM = true);

    /**
     * Sets default layout mgr.
     */
    void setDefaultLayoutManager();
    
    /**
     * Utility function
     */
    void updateBackGroundImage();
    
    /**
     * Updates the status pane according to the view settings.
     */
    void updateStatusPane();
    
    /**
     * Updates the control pane according to the view settings.
     */
    void updateControlPane();
    
    /**
     * Finds the focused control of this view.
     */
    CAlfControl* focusedControl();
    
    /**
     * from MAlfEventHandler
     */
    TBool OfferEventL(const TAlfEvent& aEvent);
    
private:

    /**
     * AlfWidget instance, owned.
     */
    auto_ptr<AlfWidget> mWidget;

    /**
     * UI Accelerator Environment where this widget is used.
     * Not owned.
     */
    CAlfEnv* mEnv;

    /**
     * Display to be associated with the view.
     * Not owned.
     */
    CAlfDisplay* mDisplay;
    
    /**
	 * Control group to contain the view widget control in.
	 * Not owned.
	 */
	CAlfControlGroup* mControlGroup;
	
	/**
	 * Control group order changed observer object used to activate
	 * this view when necessary.
	 * Owned.
	 */
	auto_ptr<AlfViewWidgetZOrderChangedObserver> mZOrderChangedObserver;
	
    /**
	 * Control group resource ID.
	 */
	TInt mControlGroupResourceId;
	
    /**
     * Flag to indicate if the view is being shown or not.
     */
    bool mShown;
     
    /**
     * Flag to indicate if the Avkon Control Pane is shown or not.
     */
    bool mControlPaneEnabled;

    /**
     * Flag to indicate if the Avkon Status Pane is shown or not.
     */
    bool mStatusPaneEnabled;

    /**
     * Flag to indicate if the view uses skin background or not.
     */
    bool mSkinEnabled;

    /**
     * ID for skin related information to be used for background.
     */
    TAknsItemID mSkinId;   

    /**
     * Cache for ID of the child widget that is currently focused.
     * This is used to restore focus to the correct child when the view
     * is shown and to remember the focused child when the view  is hidden.
     */
    std::string mFocusedChildWidgetID;
    
    TRect mDisplayRect;
    
    /**
     * Stores the naame of all the children widgets
     */
    AlfPtrVector<UString> mChildWidgetName;
    
    /**
     * Flag to indicate whether AcquireFocus() has been called on one
     * of the controls.
     */
    TBool mHasFocus;
    };

    }

#endif /*ALFVIEWWIDGET_H*/
