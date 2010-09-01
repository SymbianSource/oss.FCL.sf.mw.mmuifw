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
* Description:  The private implementation of CAlfWidget.
*
*/


#ifndef ALFWIDGETIMPL_H
#define ALFWIDGETIMPL_H

#include <osn/ustring.h>
#include <osn/alfptrvector.h>
using osncore::UString;
using osncore::AlfPtrVector;

namespace Alf
    {

class CAlfWidgetControl;
class IAlfModel;
class AlfWidget;
class AlfAttribute;
class AlfAttributeValueType;
class AlfWidgetAttributeOwnerImpl;

/**
 *  The private implementation of AlfWidget.
 *  The methods are documented in the class AlfWidget.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
class AlfWidgetImpl
    {
public:

    /**
     * Constructor.
     *
     * @since S60 ?S60_version
     */
    AlfWidgetImpl();

    /**
     * Destructor.
     *
     * @since S60 ?S60_version
     */
    ~AlfWidgetImpl();
    /**
     * control.
     *
     * @since S60 ?S60_version
     */
    CAlfWidgetControl* control();
    /**
     * setControl.
     *
     * @since S60 ?S60_version
     */
    void setControl( CAlfWidgetControl* aControl,bool aDeletePreviousControl);
    /**
     * model.
     *
     * @since S60 ?S60_version
     */
    IAlfModel* model();
    /**
     * setModel.
     *
     * @since S60 ?S60_version
     */
    void setModel( IAlfModel* aModel ,bool aTakeOwnership);

    /**
     * Set the id of the widget.
     *
     * @since S60 ?S60_version
     * @param aWidgetName The name of the widget instance.
     */
    void setWidgetName( const UString& aWidgetName );
    /**
     * widgetName.
     *
     * @since S60 ?S60_version
     */
    const UString& widgetName() const;
    
    /**
     * Add a widget to the widgets array.
     *
     * @param aWidget The widget to be appended to the array of widgets.
     */
    void addWidget(AlfWidget *aWidget);
    
    /**
     * Return total number of widgets in the list.
     */
    int  widgetCount();
    
    /**
     * Return widget at the index
     */
    AlfWidget* getWidget(int aIndex);
    
    /**
     * Returns the index of the widget in this array.
     * If the given widget is not found from the array -1
     * is returned.
     *
     * @param aWidget Widget to be searched from the array.
     * @return Index of the widget in the internal array or -1 if the widget
     * is not found.
     */
    int findWidget(AlfWidget* aWidget);
        
    /*
     *Removes a widget at given index
     */
    void removeWidget(int aIndex); 
    
    /*
     * Returns attribute owner interface.Ownership is not transferred.
     *
     * @return attribute owner interface.
     */
    AlfWidgetAttributeOwnerImpl* getAttributeOwner();

private: // data

    /**
     * The control. Not own.
     */
    CAlfWidgetControl* mControl;

    /**
     * The model. Own.
     */
    IAlfModel* mModel;

    /**
     * The widget id. Own.
     */
    UString mWidgetName;

    AlfPtrVector<AlfAttribute> mAttributeList;

    AlfPtrVector<AlfWidget> mWidgets;
    bool mTakesModelOwnership;
    
    /*
     * IAlfAttributeOwner implementation. Own.
     */
    auto_ptr<AlfWidgetAttributeOwnerImpl> mAttributeOwnerImpl;

private:

    friend class AlfWidget;
    };

    } // namespace Alf

#endif // C_ALFWIDGETIMPL_H
