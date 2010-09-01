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
* Description:  Animation class for alfred widget model.
      This class creates the animation and commands.
*
*
*/




#ifndef ALF_ANIMATIONOUTPUT_H
#define ALF_ANIMATIONOUTPUT_H

#include <osn/osndefines.h>
// FORWARD DECLARATIONS
class CAlfControl;
class CAlfVisual;
namespace Alf
    {
// FORWARD DECLARATIONS
class IAlfAttributeSetter;
class AlfReferenceToVisual;
class AlfAttributeContainer;

/**
 *  @class AlfAnimationOutput alfanimationoutput.h "alfanimationoutput.h"
 *  Class for creating a command for setting or animating
 *  visual attribute values. Animation sequences can be created
 *  by specifying delay parameters. The target attribute value
 *  can be specified relatively to a reference visual.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfAnimationOutput
    {
public:
    /**
     * Constructor.
     *
     * @since S60 ?S60_version
     *
     * @param aTargetVisual  Reference to the animated visual.
     * @param aReferenceVisual Reference to the visual with the reference value,
     *       or NULL if no reference value is used.
     * @param aAttributeSetter The attribute setter for animating the attribute
     *       using Hitchcock commands. Ownership is transferred.
     *
     * @param aAttributeContainer The attribute container holding the attribute
     * value(s) used by the attribute setter. Ownership is transferred, if the method does not
     * throw an exception. The container can hold new values for multiple different
     * attributes, in which case all of them are applied when the animation output
     * is triggered.
     *
     * @param aAnimationTime The duration of the animation. Note that this parameter
     * is used only if the transition time parameter is not defined in the
     * corresponding attribute in the attribute container.
     *
     * @param aDelay The delay in microseconds before starting the animation.
     * Note that this parameter is used only if the delay parameter is not defined in the
     * corresponding attribute in the attribute container.
     */
    OSN_IMPORT AlfAnimationOutput(AlfReferenceToVisual* aTargetVisual,
                                  AlfReferenceToVisual* aReferenceVisual,
                                  IAlfAttributeSetter* aAttributeSetter,
                                  AlfAttributeContainer* aAttributeContainer,
                                  unsigned int aAnimationTime, unsigned int aDelay);

    /** Destructor. No derived classes, therefore not virtual. */
    OSN_IMPORT ~AlfAnimationOutput();


    /**
     * Create and send the animation command.
     * @exception osncore::AlfVisualException Thrown with EInvalidVisual if reference visual or \n
     *                                        target visual is invalid .
     * 
     * @since S60 ?S60_version
     * @param aControl  The control to find the animated visual and
     *      the reference visual.
     * @param aDataID  The data ID used to create the animated visual
     *      and the reference visual. The caller
     *      is responsible for determining what data ID
     *      is related to the visuals. The visual references
     *      may or may not use the given data ID.
     * @return The visual, or NULL if the visual was not found.
     */
    OSN_IMPORT void sendCommand(CAlfWidgetControl& aControl, unsigned int aDataID);

private:
    // Reference to the animated visual. Owned.
    AlfReferenceToVisual* mTargetVisual;
    // Reference to the visual with the reference value. Owned.
    AlfReferenceToVisual* mReferenceVisual;
    // The attribute setter. Owned.
    IAlfAttributeSetter* mAttributeSetter;
    // The attribute value container. Owned.
    AlfAttributeContainer* mAttributeContainer;
    // The duration of the animation.
    unsigned int mAnimationTime;
    // The delay before sending the animation command.
    unsigned int mDelay;
    };

    } // namespace Alf

#endif //ALF_ANIMATIONOUTPUT_H
