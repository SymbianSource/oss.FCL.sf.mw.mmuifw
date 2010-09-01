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
* Description:  Visual class for alfred widget model.
      This class queries the visual by name and ID.
*
*/


#ifndef ALFREFERENCETOVISUAL_H
#define ALFREFERENCETOVISUAL_H

//INCLUDES
#include <osn/osndefines.h>
#include <osn/ustring.h>
// FORWARD DECLARATIONS
class CAlfControl;
class CAlfVisual;


using namespace osncore;

namespace Alf
    {
class CAlfWidgetControl;

/** @class AlfReferenceToVisual alfreferencetovisual.h "alf/alfreferencetovisual.h"
 * A class for finding a specific visual according to
 * the name of the visual, name of the parent element,
 * and the associated data ID. The number of visual
 * trees created by an element may depend on the data.
 * Therefore, the visual structure is not always known
 * beforehand. CAlfControl::FindElement() and MAlfElement::FindVisual
 * can be used to find elements and visuals even in dynamic
 * conditions.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfReferenceToVisual
    {
public:

    /**
     * constructor.
     *
	 * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aElementName The name of the element that is responsible
    *      for creating the visual.
     * @param aVisualName The name of the visual.
     * @param aUseDataID Deprecated and not used. The given data ID is always
     * used when resolving the visual. Only if the visual is not found with
     * the given data ID, then the data ID zero is tested for global visuals.
     * @return New object.
     */
    OSN_IMPORT static AlfReferenceToVisual* create(
        const char* aElementName,const char* aVisualName, bool aUseDataID);

    /** Destructor. No derived classes, therefore not virtual. */
    OSN_IMPORT ~AlfReferenceToVisual();


    /**
     * Find the referenced visual according to the given data ID.
     *
     * @exception osncore::AlfVisualException Thrown with the error code osncore::ECommonError\n 
     *										  when some operation performed on visual fails.
     * @since S60 ?S60_version
     * @param aControl  The control containing the associated element.
     * @param aDataID  The data ID used to create the visual. The caller
     *      is responsible for determining what data ID
     *      is related to this reference. If the visual is not found with
     * the given data ID, then the data ID zero is tried for global visuals.
     * @return The resolved visual.
     */
    OSN_IMPORT CAlfVisual* resolve(CAlfWidgetControl& aControl, unsigned int aDataID);

    /**
     * Finds the referenced visual according to given pointer to visual
     *
        * @since S60 ?S60_version
        * @param aControl      The control containing the associated element.
        * @param aSourceVisual  Pointer to visual that needs to be queried.
        */
    OSN_IMPORT CAlfVisual* resolve(CAlfWidgetControl &aControl, CAlfVisual* aSourceVisual);
private:
    /**
     * constructor
     */
    AlfReferenceToVisual(const char* aElementName,const char* aVisualName, bool aUseDataID);

private:
    // The element name. Owned.
    UString mElementName;
    // The visual name. Owned.
    UString mVisualName;
    // The associated data ID.
    bool mUseDataID;
    };

    } // namespace Alf

#endif //ALFREFERENCETOVISUAL_H

