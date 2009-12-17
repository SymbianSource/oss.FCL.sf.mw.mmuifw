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
* Description:   View widget interface.
*
*/


#ifndef IALF_VIEWWIDGET_H
#define IALF_VIEWWIDGET_H

#include <e32base.h>
#include <alf/ialfcontainerwidget.h>

//Forward Declaration
class TAknsItemID;
class CAlfDisplay;

namespace Alf
    {

namespace alfviewwidget
    {
static const IfId ident =
    {
    0, "viewwidget"
    };

    }

/**
 * An interface for the view widget.
 *
 *  @since S60 ?S60_version
 */

class IAlfViewWidget : public IAlfContainerWidget
    {

public:
    /**
     * Getter for the type identifier of this interface.
     * @return A descriptor to identify the type of this interface.
     *
     * @since S60 ?S60_version
     */
    static inline const IfId& type()
        {
        return alfviewwidget::ident;
        }


    /**
     * This will show the view widget. The view widget will be
     * shown on the top of the view stack. If the view was already
     * shown it will be moved on top of the view stack.
     *
     * @see getViewStackPosition()
     *
     * @param aShow true if the view is to be shown, false if
     *              the view will be hidden. The view is not shown, 
     *              till this api is called with true parameter.
     * @since S60 ?S60_version
     */
    virtual void show(bool aShow = true) = 0 ;
	
    /**
     * API to make the view widget accept events.
     *
     * @param aAccept true if the view is to accept inputs, else false
     *     default value is true
     * @since S60 ?S60_version
     */
    virtual void acceptEvents(bool aAccept = true) = 0 ;

    /**
     * API to enable/disable Avkon status pane
     *
     * @param aEnable true to enable the status pane, else false
     *     the default value is true.
     * @since S60 ?S60_version
     */
    virtual void enableStatusPane(bool aEnable = true) = 0;

    /**
     * API to hide/show Avkon control pane
     * If the AlfDisplay bound to the view widget does not occupy the 
     * entire client rectangle, this API has no effect.
     *
     * @param aEnable true to enable the control pane, else false
     *     the default value is true.
     * @since S60 ?S60_version
     */
    virtual void enableControlPane(bool aEnable = true) = 0;

    /**
     * API to enable/disable Avkon Skin to be used as view widget's background.
     *
     * @param aSkinBackground true if the view has to use Avkon Skin as background.
     *                        false otherwise.
     * @since S60 ?S60_version
     */
    virtual void useSkinBackground(bool aSkinBackground = true) = 0;

    /**
     * API to use Skin with the given ID to be used as view widget's background.
     * Also enables skin background. See IAlfViewWidget::useSkinBackground()     
     *
     * @param aID Skin id of the graphics to be used in the texture.
     *
     * @since S60 ?S60_version
     */
    virtual void setSkinBackground(TAknsItemID aSkinID) = 0;

    /**
     * API to check whether the view is shown
     *
     * @return true if the view is shown, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool shown() = 0;
    
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
     * @see show()
     * 
     * @return Position of the view, -1 if view is hidden.
     */    
    virtual int getViewStackPosition() const = 0;

    /**
     * API to check whether the view accepts events
     *
     * @return true if the view accepts events, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool eventsAccepted() = 0;

    /**
     * API to check whether the status pane is enabled
     *
     * @return true if the status pane is enabled, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool statusPaneEnabled() = 0;

    /**
     * API to check whether the control pane is enabled
     *
     * @return true if the control pane is enabled, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool controlPaneEnabled() = 0;

    /**
     * API to check whether skin info is being used for background
     * or not
     *
     * @return true if the skin info is being used, else false
     *
     * @since S60 ?S60_version
     */
    virtual bool usingSkinBackground() = 0;

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
    virtual TAknsItemID skinBackground() = 0;
    
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
    virtual void setRect(const TRect& aDisplayRect) = 0;

    /**
     * Virtual destructor.
     */
    virtual ~IAlfViewWidget () {}

    };
	
/**
 *  Structure to store information required to instantiate a view widget
 *  via the widget factory mechanism.
 *  A pointer to this structure is casted to a void pointer and sent to the
 *  factory plugin during the view widget construction.
 */
struct AlfViewWidgetInitData
    {
    /**
     * Owner environment for the widget
     */
    CAlfEnv* mEnv;

    /**
     * Display pointer
     */
    CAlfDisplay* mDisplay;

    /**
     * Widget instance ID.This uniquely identifies every widget instance
     * and is its name.
     * @see AlfWidget::Widgetname()
     */
    const char* mWidgetId;
    
    /**
     * Control group ID that will be used to create a new control group
     * and append the view widget into. 
     */
    int mControlGroupId;

    /**
     * Pointer to node in declaration containing information for the widget.
     */
    DuiNode* mNode;
    
    /**
     * XML file name containing the declaration for the presention of the widget. 
     */
    const char* mFilePath;

    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };	

	}//end of namespace

#endif // IALF_VIEWWIDGET_H
//End Of File
