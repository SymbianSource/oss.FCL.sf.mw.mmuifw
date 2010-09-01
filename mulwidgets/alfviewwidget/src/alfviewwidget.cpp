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
* Description:  The implementation	for	presentation elements.
*
*/



#include "alfviewwidget.h"
#include "alfviewcontrol.h"
#include <e32std.h>
#include <assert.h>
#include <memory>
#include <alf/alfcontrolgroup.h>
#include <alf/alfenv.h>
#include <alf/alfdisplay.h>
#include <alf/alfroster.h>
#include <alf/alfmetric.h>
#include "alf/alfwidget.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/alfanchorlayoutmanager.h>
#include <alf/alfgridlayout.h>
#include <alf/ialfgridlayoutpolicy.h>
#include <alf/alfwidgetenvextension.h>
#include <alf/ialfwidgetfactory.h>

#include <alf/alfutil.h>

#include "alfviewwidgetexception.h"

#include <eikbtgpc.h>
#include <eikspane.h>
#include <eikmenub.h>

namespace Alf
    {

/**
 * Observer class to observe changes in the relative order of control groups in associated
 * roster object and to notify view object about possible view activation (view coming
 * top-most) and deactivation.
 *
 * This class is a Symbian - specific implementation class.
 */
class AlfViewWidgetZOrderChangedObserver : public MAlfControlGroupOrderChangedObserver
    {
    public:
    
        /**
         * Default constructor
         */
        AlfViewWidgetZOrderChangedObserver(CAlfDisplay& aDisplay, AlfViewWidget& aView)
            : iDisplay(aDisplay), iView(aView)
            {
            aDisplay.Roster().AddControlGroupOrderChangedObserverL(*this);
            }
        
        /**
         * Default destructor
         */
        ~AlfViewWidgetZOrderChangedObserver()
            {
            int index = CAlfEnv::Static()->FindDisplayIndex(iDisplay);
            if(index != -1)
	            {
	            iDisplay.Roster().RemoveControlGroupOrderChangedObserver(*this);	
	            }            
            }
        
        /**
         * Callback method called by roster when control group order changes.
         */
        void NotifyControlGroupOrderChanged()
            {
            // If the associated view is on top, notify it about view activation
            // and the view below it about view deactivation.
            if(iView.getViewStackPosition() == 0)
                {               
                AlfViewWidget* viewBelow = getViewFromStack(1);                
                if(viewBelow)
                    {
                    viewBelow->notifyViewActivated(false);
                    }
                
                iView.notifyViewActivated(true);
                }
            }
                
    private:
    
        /**
         * Utility method to return view from specified z-position on the view stack.
         * Returns 0 if the index is out of bounds.
         */
        AlfViewWidget* getViewFromStack(int index)
            {
            int zPos = 0;
            
            // Iterate through the control groups in the roster
            for(int i = (iDisplay.Roster().Count() - 1); i >= 0; --i)
            	{
            	CAlfControlGroup& group = iDisplay.Roster().ControlGroup(i);            	    	
            	
            	// Investigate whether this is a control group of a view widget
            	if(group.Count() > 0)
            		{
            		CAlfControl& control = group.Control(0);
            		CAlfViewControl* viewControl = dynamic_cast<CAlfViewControl*>(&control);
            		if(viewControl != 0)
            			{
            			// View widget found from the roster
            			if(zPos == index)
            			    {
            			    return &viewControl->viewWidget();
            			    }
            			zPos++;
            			}
            		}
            	}
            
            return 0;
            }
    
        /** Roster to which this callback object is added to. */
        CAlfDisplay& iDisplay;
        
        /** View widget that this observer notifies. */
        AlfViewWidget& iView;
            
    };


AlfViewWidget::AlfViewWidget(const char* aWidgetName, int aControlGroupId, CAlfEnv& aEnv, 
                             CAlfDisplay& aDisplay, DuiNode* aNode, const char *aFilePath) : 
    mEnv(&aEnv), mDisplay(&aDisplay), mControlGroup(0),
    mZOrderChangedObserver(0),
    mControlGroupResourceId(aControlGroupId),
    mShown(false), mControlPaneEnabled(true), mStatusPaneEnabled(true),
    mSkinEnabled(false), mSkinId(KAknsIIDQsnBgScreen), mHasFocus(false)
	{
	mChildWidgetName.setAutoDelete(true);
	mWidget.reset(new (EMM) AlfWidget(aWidgetName));
	
	TRAPD(err,aEnv.AddActionObserverL(this));
	if(err!=KErrNone)
	    {
	    ALF_THROW(AlfViewWidgetException, err, 
	        "CAlfEnv::AddActionObserverL() - failed.");
	    }
	
	// By default, we use ClientRect() as the area of the view widget    
	TRect rect = ((CAknAppUi*)CEikonEnv::Static()->AppUi())->ClientRect();
	mDisplayRect =  TRect(rect);
	
	mZOrderChangedObserver.reset(new (EMM) AlfViewWidgetZOrderChangedObserver(aDisplay, *this));
	   
        constructDefault(aEnv);
    
	}

AlfViewWidget::~AlfViewWidget()
	{	
	//If the view widget is deleted before the AlfEnv is deleted,
	//the control group needs to be destroyed.
	if(mEnv->FindControlGroup(mControlGroupResourceId))
    	{
    	mEnv->DeleteControlGroup(mControlGroup->ResourceId());		
    	}
    
    mEnv->RemoveActionObserver(this);
    
    IAlfWidget* childWidget = NULL;
    
    IAlfWidgetFactory& factory = AlfWidgetEnvExtension::widgetFactory(*(CAlfEnv::Static()));
    for(int i =0 ;i < mChildWidgetName.count();i++)
	    {
	    childWidget = factory.findWidget(mChildWidgetName.at(i)->getUtf8());
	    if(childWidget)
		    {
			factory.destroyWidget(childWidget);
			childWidget = NULL;
		    }
	    }
    mChildWidgetName.clear();
	}

CAlfWidgetControl* AlfViewWidget::control() const
	{
	return mWidget->control();
	}
	
IAlfContainerWidget* AlfViewWidget::parent() const
    {
    return mWidget->parent();
    }	

void AlfViewWidget::setControl(CAlfWidgetControl* aControl, bool aDeletePreviousControl)
	{
	CAlfWidgetControl* prevControl = mWidget->control();
		
	// Set new control to the widget implementation.
	// Never delete the previous control in this phase, so that we can
	// still remove it from the control group if this call is succesfull.
	mWidget->setControl(aControl, false);		
	
	// Remove the previous control from control group.
	if(prevControl != 0)
	    {
	    assert(prevControl->ControlGroup() == mControlGroup);
	    if(aDeletePreviousControl)
	        {
	        mControlGroup->Remove(prevControl);
	        delete prevControl;
	        }
	    }	
	
	// Append the new control into the control group.
    if(aControl)
    	{
	    TRAPD(err, mControlGroup->AppendL(aControl));
	    if(err != KErrNone)
	    	{
	    	mWidget->setControl(0, false);
	    	ALF_THROW(AlfViewWidgetException, err, "CAlfControlGroup::AppendL() - failed.");
	    	}
    	}	
	}
	
void AlfViewWidget::setChildNameToDelete( UString aChildName )
	{
	UString* childName = new(EMM)UString(aChildName.getUtf8());
	mChildWidgetName.resize(mChildWidgetName.count()+1);
	mChildWidgetName.insert(mChildWidgetName.count(),childName);
	}

IAlfModel* AlfViewWidget::model()
    {
    return 0;
    }

void AlfViewWidget::setModel(IAlfModel* /*aModel*/, bool /*aTakeOwnership*/)
    {
    //Do nothing.
    }

const char* AlfViewWidget::widgetName() const
    {
    return mWidget->widgetName();
    }

void AlfViewWidget::setChildFocus(bool /*aFocus*/)
    {

    }

//---------------------------------------------------------------------------
// Creates the presentation for the widget from XML. Destroys any existing
// presentation.
//---------------------------------------------------------------------------
//    
void AlfViewWidget::setPresentation (const char* aFilePath)
    {
    if(mWidget.get())
    	{
        mWidget->setPresentation(aFilePath);    	
    	}
    }

int AlfViewWidget::widgetCount() const
    {
    return mWidget->widgetCount();
    }

void AlfViewWidget::addWidget(IAlfWidget& aWidget)
    {
    mWidget->addWidget(aWidget);
    }

IAlfWidget* AlfViewWidget::getWidget(int aIndex) const
    {
    return mWidget->getWidget(aIndex);
    }
    
int AlfViewWidget::getWidgetIndex(IAlfWidget& aWidget) const
    {
    return mWidget->getWidgetIndex(aWidget);
    }

void AlfViewWidget::removeWidget(int aIndex)
    {
    mWidget->removeWidget(aIndex);
    }

void AlfViewWidget::applyLayout(IAlfLayoutManager& aLayout)
    {
    mWidget->applyLayout(aLayout);
    }

IAlfInterfaceBase* AlfViewWidget::makeInterface(const IfId& aType)
	{
    // Type cast to IAlfWidget
    if(!strcmp(aType.mImplementationId, IAlfWidget::type().mImplementationId))
        {
        return static_cast<IAlfWidget*>(this);
        }
        
    // Type cast to IAlfContainerWidget
    if (!strcmp(aType.mImplementationId, IAlfContainerWidget::type().mImplementationId))
        {
        return static_cast<IAlfContainerWidget*>(this);
        }

    // Type cast to IAlfViewWidget        
    if(!strcmp(aType.mImplementationId, IAlfViewWidget::type().mImplementationId))
        {
        return static_cast<IAlfViewWidget*>(this);
        }    
       
    return mWidget->makeInterface(aType);
    }

void AlfViewWidget::notifyViewActivated(bool aIsActivated)
	{
	if(aIsActivated)
		{
        // Update control pane, status pane and background according to state of the view.
        updateStatusPane();
        updateControlPane();
        // When a view is activated, it sets the size of the display to full screen
        // This is done so that the view widget gets pointer events in the entire screen
        TRect screenRect(TPoint(0,0), TSize(AlfUtil::ScreenSize()));
        control()->Env().PrimaryDisplay().SetVisibleArea(screenRect);
        setRect(mDisplayRect);
        
        // Restore the focus here
        IAlfWidget* focusedWidget = 0;
        focusedWidget = AlfWidgetEnvExtension::widgetFactory(*mEnv).findWidget(mFocusedChildWidgetID.c_str());
        if(focusedWidget == 0 && widgetCount() > 0)
            {
            focusedWidget = getWidget(0);
            }
        if(!mHasFocus)
	        {
	        mHasFocus = true;
	        if(focusedWidget != 0)
	            {
	            focusedWidget->control()->AcquireFocus();
	            }
	        else
	            {
	            control()->AcquireFocus();
	            }	
	        }
        }
    else
        {
        // Cache and release the focus here
        // FocusedControl function is not exported from the toolkit
        CAlfControl* focusedCtrl = focusedControl();
        if(focusedCtrl)
            {
            CAlfWidgetControl* focusedWidgetControl = dynamic_cast<CAlfWidgetControl*>(focusedCtrl);
            if(focusedWidgetControl != 0)
                {
                mFocusedChildWidgetID = focusedWidgetControl->widget()->widgetName();
                }                        
            focusedCtrl->RelinquishFocus();
            }
        mHasFocus = false;    
        }
    }
        
CAlfControl* AlfViewWidget::focusedControl()
    {
    CAlfControl* focusedconnection = control();
    
    // Iterate through the focused connections and return the control
    // that has the focus.
    while(focusedconnection != 0)
        {
        if(focusedconnection->Focus())
            {
            // There should be no connections leading outside from the control
            // group of the view.
            assert(focusedconnection->ControlGroup() == mControlGroup);
            return focusedconnection;
            }
        focusedconnection = focusedconnection->FocusedConnection();        
        }
        
    return 0;
    }
    
void AlfViewWidget::show(bool aShow)
    {
    mShown = aShow;
    if (aShow)
        {   
        TRAPD(err, mDisplay->Roster().ShowL(*mControlGroup));
        if (err != KErrNone)
            {
            ALF_THROW(AlfViewWidgetException, err, "AlfViewWidget::show(): View Widget Show failed");   
            }
        }
    else
        {
        // Notify this view that it is being deactivated.
        notifyViewActivated(false);
        
        // Hide the control group
        mDisplay->Roster().Hide(*mControlGroup);
        }
    }

void AlfViewWidget::acceptEvents(bool aAccept)
    {
    assert(mControlGroup);
    mControlGroup->SetAcceptInput(aAccept);
    }

void AlfViewWidget::enableStatusPane(bool aEnable)
    {
	if(aEnable != mStatusPaneEnabled)
		{
	    mStatusPaneEnabled = aEnable;
	    
	    // Update status pane and background image state if this view is active.
	    if(getViewStackPosition() == 0)
	        {
	        updateStatusPane();
	        }
		}
    }

void AlfViewWidget::enableControlPane(bool aEnable)
    {
    if (aEnable != mControlPaneEnabled)
        {
        mControlPaneEnabled=aEnable;
        
	    // Update control pane and background image state if this view is active.
	    if(getViewStackPosition() == 0)
	        {        
            updateControlPane();
	        }
        }
    }

void AlfViewWidget::useSkinBackground(bool aSkinBackground)
    {
    if(mSkinEnabled != aSkinBackground)
    	{
    	 mSkinEnabled = aSkinBackground;
    	 updateBackGroundImage();	
    	}
    
    }

void AlfViewWidget::setSkinBackground(TAknsItemID aSkinID)
    {
    mSkinEnabled = true;
    mSkinId = aSkinID;
    updateBackGroundImage();
    }

bool AlfViewWidget::shown()
    {
	return mShown;
	}

int AlfViewWidget::getViewStackPosition() const
	{
	int ret = 0;
    const CAlfRoster& roster = mDisplay->Roster();
    
    // Iterate through the control groups in the roster
    for(int i = (roster.Count() - 1); i >= 0; --i)
    	{
    	CAlfControlGroup& group = roster.ControlGroup(i);
    	    	
    	// Compare the control group with this view's control group
    	if(mControlGroup == &group)
    		{
    		return ret;
    		}
    	
    	// Investigate whether this is a control group of a view widget
    	if(group.Count() > 0)
    		{
    		CAlfControl& control = group.Control(0);    		
    		if(dynamic_cast<CAlfViewControl*>(&control) != 0)
    			{
    			ret++;
    			}
    		}
    	}
    
    return -1;   
	}

bool AlfViewWidget::eventsAccepted()
    {
    assert(mControlGroup);
    return mControlGroup->AcceptInput();
    }

bool AlfViewWidget::statusPaneEnabled()
    {
    return mStatusPaneEnabled;
    }

bool AlfViewWidget::controlPaneEnabled()
    {
    return mControlPaneEnabled;
    }
    
bool AlfViewWidget::usingSkinBackground()
	{
	return mSkinEnabled;	
	}

TAknsItemID AlfViewWidget::skinBackground()
	{
	return mSkinId;	
	}

// ---------------------------------------------------------------------------
// Constructs components from node
// ---------------------------------------------------------------------------
//  
void AlfViewWidget::constructComponentsFromNode(CAlfEnv& aEnv)
    {
    constructDefault(aEnv, false);
    
    //check, if the layoutmanager is already set.
    IAlfLayoutManager* layoutManager = 
        IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(control());
    if (!layoutManager)
        {
        setDefaultLayoutManager();
        }
    }

//---------------------------------------------------------------------------
// Constructs widget using the presentation XML. Other widget parts
// are default.
//---------------------------------------------------------------------------
//    
void AlfViewWidget::constructFromPresentationXML(CAlfEnv& aEnv, const char* /**aFilePath*/)
    {
    constructDefault(aEnv, true);
    }

// ---------------------------------------------------------------------------
// Imperative construction
// ---------------------------------------------------------------------------
//    
void AlfViewWidget::constructDefault(CAlfEnv& aEnv, bool aCreateLM)
    {
    TRAPD( err, 
         mControlGroup = &aEnv.NewControlGroupL(mControlGroupResourceId);
         );
    
    if(err != KErrNone)
        {
        if (err == KErrAlreadyExists)
            {
            ALF_THROW(AlfViewWidgetException, 
                EControlGroupAlreadyExists, "AlfViewWidget::AlfViewWidget() - Tried to create a control group with already existing ID.");
            }
        else
            {
            ALF_THROW(AlfViewWidgetException, 
                err, "AlfViewWidget::AlfViewWidget() - Control group creation failed.");
            }
        }
    
    // Create control for the view widget
    auto_ptr<CAlfViewControl> control( new (EMM) CAlfViewControl(aEnv, *this) );
    setControl(control.get());
    control.release();
    
    if (aCreateLM)
        {
        setDefaultLayoutManager();
        }
    }

// ---------------------------------------------------------------------------
// Imperative createDefaultLayoutManager
// ---------------------------------------------------------------------------
//
void AlfViewWidget::setDefaultLayoutManager()
    {
  	// A root layout covering the entire display is created to grab all pointer events in the screen
  	// Note that this layout is just dummy and does not do any actual layouting
    CAlfControl* ctrl = (CAlfControl*)control();
    CAlfLayout* fullDisplayLayout = CAlfLayout::AddNewL(*ctrl);
    
    // Default layout manager provided by the view widget
    auto_ptr<AlfAnchorLayoutManager> layoutManager(new (EMM) AlfAnchorLayoutManager());
    this->applyLayout(*layoutManager.get());
    
    layoutManager.release();
    }

void AlfViewWidget::updateStatusPane()
	{
    /*CEikStatusPane *sp = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    if(sp)  
        {
        sp->MakeVisible(mStatusPaneEnabled);
//        mDisplay->SetVisibleArea(CEikonEnv::Static()->AppUiFactory()->ClientRect());
        }*/	
	}

void AlfViewWidget::updateControlPane()
	{	
    /*CEikButtonGroupContainer* cba= CEikonEnv::Static()->AppUiFactory()->Cba();
    if(cba)  
        {
        cba->MakeVisible(mControlPaneEnabled);
//        mDisplay->SetVisibleArea(CEikonEnv::Static()->AppUiFactory()->ClientRect());
        }*/

    }
    
void AlfViewWidget::updateBackGroundImage()
    {
    TAknsItemID skinId=KAknsIIDQsnBgScreen;
    // Do background related stuff only if the view is showing
    if(mSkinEnabled )
        {
        skinId=mSkinId;
        }
    if(mShown)
        {
        TAlfDisplayBackgroundItem bgItem(mDisplay->VisibleArea(),skinId);
        RArray<TAlfDisplayBackgroundItem> bgItemArray;
        bgItemArray.Append(bgItem);
        TRAPD(err,mDisplay->SetBackgroundItemsL(bgItemArray));
        if(err!=KErrNone)
            {
            ALF_THROW(AlfViewWidgetException, err,"Skin Background could not be set");
            }
        bgItemArray.Reset();
        }
    
    }
    

void AlfViewWidget::HandleActionL(const TAlfActionCommand& aActionCommand)
    {
    if(KAlfActionIdDeviceLayoutChanged == aActionCommand.Id())
        {
        TAlfCustomEventCommand command(KAlfActionIdDeviceLayoutChanged);
        control()->Env().Send(command,0);
        if(mShown && getViewStackPosition() == 0)
              {
              TRect screenRect(TPoint(0,0), TSize(AlfUtil::ScreenSize()));

              // SetVisibleArea() only have an effect if the application is on foreground!
              control()->Env().PrimaryDisplay().SetVisibleArea(screenRect);
              }
       
        }
    
    else if(KAlfActionIdForegroundGained == aActionCommand.Id())
        {
        
            TRect screenRect(TPoint(0,0), TSize(AlfUtil::ScreenSize()));
            
            // SetVisibleArea() only have an effect if the application is on foreground!
            
            control()->Env().PrimaryDisplay().SetVisibleArea(screenRect);
            
        
        } 

    }

TBool AlfViewWidget::OfferEventL(const TAlfEvent& /*aEvent*/)
    {
/*    if(aEvent.IsCustomEvent() && aEvent.CustomParameter() ==  KAlfActionIdDeviceLayoutChanged)
        {
        if(mShown && getViewStackPosition() == 0)
              {
              //need to do this to overwrite the displayrect done by menuApp
             // TRect rect= control()->Env().PrimaryDisplay().VisibleArea();
             // control()->Env().PrimaryDisplay().SetVisibleArea(mDisplayRect);
              updateBackGroundImage();            
              }
        return ETrue;
        }
  */  return EFalse;
    }
  
void AlfViewWidget::setRect(const TRect& aRect)
    {
    mDisplayRect = aRect;
    
    IAlfLayoutManager* layoutManager  = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(control());
    if (layoutManager)
    	{
    	// Set the area of the root layout of the view widget to the one specified
        CAlfLayout& layout = layoutManager->getLayout();
        layout.SetFlag(EAlfVisualFlagManualLayout);
        TAlfRealRect rect(mDisplayRect);
        layout.SetRect(rect);
    	}
    
    TRect visibleRect = control()->Env().PrimaryDisplay().VisibleArea();
	TRect screenRect(TPoint(0,0), TSize(AlfUtil::ScreenSize()));
    
    if(visibleRect != screenRect && getViewStackPosition() == 0)
        {
    	// the display area is required to be full screen, for the pointer grab to work
    	// check here whether the display area is full screen. If not, set it to full screen
        control()->Env().PrimaryDisplay().SetVisibleArea(screenRect);
        }
    }

    } //Alf


//End of file
 
