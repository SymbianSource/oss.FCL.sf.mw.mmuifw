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
* Description:  Contains common properties used in DUI UI Model
*
*/


#ifndef DUIPROPERTY_H
#define DUIPROPERTY_H

namespace duiuimodel
    {

// DUI XML namespace.
const char KDefaultNameSpaceUri[] = "http://www.series60.com/xml/dhuiml/1";

namespace nodetypes
    {
    static const char KImageVisual[] =      "imagevisual";
    static const char KTextVisual[] =       "textvisual";
    static const char KMeshVisual[] =       "meshvisual";
    static const char KLineVisual[] =       "linevisual";
    static const char KLayout[] =           "layout";
    static const char KGridLayout[] =       "gridlayout";
    static const char KFlowLayout[] =  "flowlayout";
    static const char KDeckLayout[] =  "decklayout";
    static const char KCurvePathLayout[] =  "curvepathlayout";
    static const char KCurvePath[] =        "curvepath";
    static const char KLine[] =             "line";
    static const char KArc[] =              "arc";
    static const char KColor[] =            "color";
    static const char KViewportLayout[] =   "viewportlayout";
    static const char KAttributeSetter[] =  "attributesetter";
    static const char KAttributeCreator[] =  "attributecreator";
    static const char KDataAttributeSetter[] =  "dataattributesetter";
    static const char KAnchorLayout[] =     "anchorlayout";
    static const char KAnchorLayoutInput[] ="anchorlayoutinput";
    static const char KEventHandler[] =     "eventhandler";
    static const char KEventInput[] =       "eventinput";
    static const char KEventOutput[] =      "eventoutput";
    static const char KCondition[] =        "condition";
    static const char KConditionGroup[] =        "conditiongroup";
    static const char KProperty[] =         "property";
    static const char KVisualConnection[] = "visualconnection";
    static const char KListFlowElement[] =  "listflowelement";
    static const char KAnchor[] =           "anchor";
    static const char KMetric[] =           "metric";
    static const char KRect[] =             "rect";
    static const char KLCTImageVisual[] =      "lctimagevisual";
    static const char KLCTTextVisual[] =       "lcttextvisual";
    static const char KLCTAnchorLayout[] =     "lctanchorlayout";
    static const char KLCTGridLayout[] =     "lctgridlayout";
    static const char KPadding[] =          "padding";
    static const char KFrameBrush[] =    "framebrush";
    static const char KBorderBrush[] =   "borderbrush";
    static const char KDropShadowBrush[] =  "dropshadowbrush";
    static const char KGradientBrush[] =  "gradientbrush";
    static const char KImageBrush[] =   "imagebrush";
    static const char KShadowBorderBrush[] ="shadowborderbrush";
    static const char KVisualization[] = "visualization";
    static const char KImageResource[] = "imageresource";
    static const char KResources[] = "resources";
    static const char KElement[] = "element";
    static const char KTactile[] = "tactile";
    static const char KButtonControl[] = "buttoncontrol";
    static const char KButtonWidgetControl[] = "buttonwidgetcontrol";
    static const char KListControl[] = "listcontrol";
    static const char KGridControl[] = "gridcontrol";
    static const char KGridContainerControl[] = "gridcontainercontrol";
    static const char KGridDefaultBaseElement[] = "griddefaultbaseelement";
    static const char KGridDefaultFocusElement[] = "griddefaultfocuselement";
    static const char KGridDefaultTextItemElement[] = "griddefaulttextitemelement";
    static const char KGridDefaultImageElement[] = "griddefaultimageelement";
    static const char KGridDefaultImagetextElement[] = "griddefaultimagetextitemelement";
    static const char KLayoutManager[] = "layoutmanager";
    static const char KConnection[] = "connection";
    static const char KMapToEvent[] = "maptoevent";
    static const char KAppEvents[] = "appevents";
    static const char KButtonWidget[] = "buttonwidget";
    static const char KListWidget[] = "listwidget";
    static const char KGridWidget[] = "gridwidget";
    static const char KScrollbarWidget[] = "scrollbarwidget";
    static const char KWidget[] = "widget";
    static const char KAppData[] = "appdata";
    static const char KApplication[] = "application";
    static const char KContainer[] = "container";
    static const char KView[] = "view";
    static const char KDhuiml[] = "dhuiml";
    static const char KVersion[] = "version";
    }

namespace commonattributevalues
    {
    static const char KTrue[] = "true";
    static const char KFalse[] = "false";
    }

namespace commonattributes
    {
    static const char KId[] = "id";
    static const char KRef[] = "ref";
    static const char KClass[] = "class";
    }

namespace commonvisualattributes
    {
    // deprecated
    static const char KFocusable[] = "focusable";
    static const char KDisabled[] = "disabled";
    static const char KParentVisual[] = "parentvisual";
    static const char KOpacity[] = "opacity";
    static const char KTurnAngle[] = "turnangle";
    // end deprecated

    
    static const char KSetFlags[] = "setflags";
    static const char KClearFlags[] = "clearflags";

    namespace visualflags
        {
        static const char KManualSize[] = "ManualSize";
        static const char KAlfVisualFlagManualPosition[] = "AlfVisualFlagManualPosition";
        static const char KAlfVisualFlagManualLayout[] = "AlfVisualFlagManualLayout";
        static const char KAlfVisualFlagLayoutUpdateNotification[] = "AlfVisualFlagLayoutUpdateNotification";
        static const char KAlfVisualFlag3DProjection[] = "AlfVisualFlag3DProjection";
        static const char KAlfVisualFlagNoDepthOcclusion[] = "AlfVisualFlagNoDepthOcclusion";
        static const char KAlfVisualFlagLocalHorizon[] = "AlfVisualFlagLocalHorizon";
        static const char KAlfVisualFlagClipping[] = "AlfVisualFlagClipping";
        static const char KAlfVisualFlagHOriginCenter[] = "AlfVisualFlagHOriginCenter";
        static const char KAlfVisualFlagHOriginRight[] = "AlfVisualFlagHOriginRight";
        static const char KAlfVisualFlagVOriginCenter[] = "AlfVisualFlagVOriginCenter";
        static const char KAlfVisualFlagVOriginBottom[] = "AlfVisualFlagVOriginBottom";
        static const char KAlfVisualChanged[] = "AlfVisualChanged";
        static const char KAlfVisualFlagNoDepthUpdate[] = "AlfVisualFlagNoDepthUpdate";
        static const char KAlfVisualFlagManualTransitionTime[] = "AlfVisualFlagManualTransitionTime";
        static const char KAlfVisualFlagDrawAfterOthers[] = "AlfVisualFlagDrawAfterOthers";
        static const char KAlfVisualFlagFreezeLayout[] = "AlfVisualFlagFreezeLayout";
        static const char KAlfVisualFlagIgnorePointer[] = "AlfVisualFlagIgnorePointer";
        static const char KAlfVisualFlagAutomaticLocaleMirroringEnabled[] = "AlfVisualFlagAutomaticLocaleMirroringEnabled";
        static const char KAlfVisualFlagLast[] = "AlfVisualFlagLast";
        }
    }

namespace commonbrushattributes
    {
    static const char KOwnerVisual[] = "ownervisual";
    static const char KOpacity[] = "opacity";
    static const char KSecondaryAlpha[] = "secondaryalpha";
    static const char KLayer[] = "layer";

    namespace layertypes
        {
        static const char KBrushLayerBackGround[] = "brushlayerbackground";
        static const char KBrushLayerForeGround[] = "brushlayerforeground";
        }
    }

namespace borderbrushattributes
    {
    static const char KImage[] = "image";
    static const char KThicknessWidth[] = "thicknesswidth";
    static const char KThicknessHeight[] = "thicknessheight";
    static const char KImageOffsetX[] = "imageoffsetx";
    static const char KImageOffsetY[] = "imageoffsety";
    static const char KEdgeOffsetX[] = "edgeoffsetx";
    static const char KEdgeOffsetY[] = "edgeoffsety";
    }

namespace dropshadowbrushattributes
    {
    static const char KSoftness[] = "softness";
    static const char KDepth[] = "depth";
    }

namespace framebrushattributes
    {
    static const char KSkinID[] = "skinid";
    }

namespace gradientbrushattributes
    {
    static const char KImage[] = "image";
    static const char KDirection[] = "direction";

    namespace directiontypes
        {
        static const char KUp[] = "up";
        static const char KDown[] = "down";
        static const char KLeft[] = "left";
        static const char KRight[] = "right";
        }
    }

namespace imagebrushattributes
    {
    static const char KImage[] = "image";
    static const char KImageScaleMode[] = "imagescalemode";
    static const char KOffsetX[] = "offsetx";
    static const char KOffsetY[] = "offsety";
    static const char KScale[] = "scale";
    }

namespace shadowborderbrushattributes
    {
    static const char KBorderWitdh[] = "borderwitdh";
    }

namespace rect
    {
    static const char KTopLeftX[] = "topleftx";
    static const char KTopLeftY[] = "toplefty";
    static const char KBottomRightX[] = "bottomrightx";
    static const char KBottomRightY[] = "bottomrighty";
    }

namespace padding
    {
    static const char KPaddingTop[] = "padding-top";
    static const char KPaddingRight[] = "padding-right";
    static const char KPaddingBottom[] = "padding-bottom";
    static const char KPaddingLeft[] = "padding-left";
    }

namespace color
    {
    static const char KAlpha[] = "sca";
    static const char KBlue[] = "scb";
    static const char KGreen[] = "scg";
    static const char KRed[] = "scr";
    }

namespace layoutattributes
    {
    static const char KParentLayout[] = "parentlayout";
    static const char KHeight[] = "height";
    static const char KWidth[] = "width";
    static const char KMaxHeight[] = "maxheight";
    static const char KMaxWidth[] = "maxwidth";
    static const char KMinHeight[] = "minheight";
    static const char KMinWidth[] = "minwidth";
    
    static const char KUpdateLayoutTime[] = "updatelayouttime";
    static const char KTransitionTime[] = "transitiontime"; // will replace updatelayouttime
    static const char KScrolling[] = "scrolling";
    static const char KAutomaticLocaleMirroring[] = "automirroring";
    static const char KBaseUnitX[] = "baseunitx";
    static const char KBaseUnitY[] = "baseunity";
    static const char KPositionX[] = "positionx";
    static const char KPositionY[] = "positiony";
    static const char KInnerPaddingVertical[]="innerpadding-vertical";
    static const char KInnerPaddingHoriz[]="innerpadding-horizontal";
    static const char KLayoutScrollOffsetX[]="scrolloffsetx";
    static const char KLayoutScrollOffsetY[]="scrolloffsety";
    static const char KExpansionFlag[] = "expansion flag";

    namespace grid
        {
        static const char KRows[] = "rows";
        static const char KColumns[] = "columns";
        static const char KRowWeight[] = "rowweight";
        static const char KColumnWeight[] = "columnweight";
        static const char KSetLayoutModeFlag[] = "setlayoutmodeflag";
        static const char KClearLayoutModeFlag[] = "clearlayoutmodeflag";
        }

    namespace flow
        {
        static const char KFlowDirection[] = "flowdirection";
        static const char KLayoutMode[] = "layoutmode";
        static const char KLayoutCentering[] = "layoutcentering";

        namespace flowdirectionvalues
            {
            static const char KHorizontal[] = "horizontal";
            static const char KVertical[] = "vertical";
            }

        namespace layoutmodevalues
            {
            static const char KCenterperpendicular[] = "centerperpendicular";
            static const char KFitperpendicular[] = "fitperpendicular";
            }
        }

    namespace viewport
        {
        static const char KVirtualHeight[] = "virtualheight";
        static const char KVirtualWidth[] = "virtualwidth";
        static const char KViewportHeight[] = "viewportheight";
        static const char KViewportWidth[] = "viewportwidth";
        static const char KViewportPositionX[] = "viewportpositionx";
        static const char KViewportPositionY[] = "viewportpositiony";
        }
    }

namespace property
    {
    static const char KName[] = "name";
    static const char KValue[] = "value";
    }
    
namespace textvisualattributes
    {
    static const char KLineWrap[] = "linewrap";
    static const char KText[] = "text";
    static const char KBackgroundType[] = "backgroundtype";
    static const char KLineSpacing[] = "linespacing";
    static const char KLineSpacingUnits[] = "linespacingunits";
    static const char KFont[] = "font";
    static const char KHorizontalAlign[] = "horizontalalign";
    static const char KVerticalAlign[] = "verticalalign";
    static const char KMaxLineCount[] = "maxlinecount";
    static const char KStyle[] = "style";
    static const char KPreconfiguredStyle[] = "preconfiguredstyle";
    static const char KShadowOpacity[] = "shadowopacity";
    static const char KTextSkinColorId[] = "textskincolorid";
    static const char KTextColorSkinIndex[] = "textcolorskinindex";
    static const char KEnableShadow[]= "enableshadow";
    static const char KTextColor[]= "textcolor";
    static const char KLocalisationText[] = "localisedtext"; //for localisation
    static const char KLang[] = "localisedlanguage";    //for localisation
    static const char KVarArgs[] = "varargs";    //for localisation

    namespace linewrapvalue
        {
        static const char KManual[] = "manual";
        static const char KTruncate[] = "truncate";
        static const char KBreak[] = "break";
        }

    namespace backgroundtypevalue
        {
        static const char KDark[] = "dark";
        static const char KLight[] = "light";
        }

    namespace linespacingvalue
        {
        static const char KPixel[] = "pixel";
        static const char KTwips[] = "twips";
        }

    namespace horizontalalignvalue
        {
        static const char KLeft[] = "left";
        static const char KCenter[] = "center";
        static const char KRight[] = "right";
        static const char KLocale[] = "locale";
        static const char KLocaleMirrored[] = "localemirrored";
        }

    namespace verticalalignvalue
        {
        static const char KTop[] = "top";
        static const char KCenter[] = "center";
        static const char KBottom[] = "bottom";
        }

    namespace preconfiguredstylevalue
        {
        static const char KNormal[] = "normal";
        static const char KNormalItalic[] = "normalitalic";
        static const char KMenuItem[] = "menuitem";
        static const char KSmall[] = "small";
        static const char KMenuSmall[] = "menusmall";
        static const char KMenuTitle[] = "menutitle";
        static const char KLarge[] = "large";
        static const char KSoftkey[] = "softkey";
        static const char KSupplement[] = "supplement";
        static const char KTitle[] = "title";
        }
    }

namespace lctvisualattributes
    {
    static const char KLCT[] = "lct";
    static const char KLCTNoSize[] = "lctnosize";
    static const char KVariety[] = "variety";
    static const char KAdditionalIcon[] = "additionalicon";
    static const char KAdditionalIconCount[] = "additionaliconcount";
    }

namespace linevisualattributes
    {
    static const char KImagePath[] = "imagepath";
    static const char KThickness[] = "thickness";
    static const char KShadowThickness[] = "shadowthickness";
    static const char KStartPos[] = "startposition";
    static const char KEndPos[] = "endposition";
    static const char KColor[] = "color";
    static const char KAlphaMappingFunction[] = "alphamappingfunction";
    static const char KWidthMappingFunction[] = "widthmappingfunction";
    }

namespace tactileattributes
    {
    static const char KEventInput[] = "eventinput";
    static const char KFeedbackType[] = "feedbacktype";

    namespace eventinputvalues
        {
        static const char KStylusDown[] = "stylusdown";
        static const char KStylusUp[] = "stylysup";
        }

    namespace feedbacktypevalues
        {
        static const char KBasic[] = "basic";
        static const char KSensitive[] = "sensitive";
        }
    }

namespace mappingfunctions
    {
    static const char KConst[] = "constantmappingfunction";
    static const char KLinear[] = "linearmappingfunction";
    static const char KSine[] = "sinemappingfunction";
    static const char KCosine[] = "cosinemappingfunction";
    }


namespace imagevisualattributes
    {
    static const char KImageScaleMode[] = "imagescalemode";
    static const char KImageColorMode[] = "imagecolormode";
    static const char KSecondaryAlpha[] = "secondaryalpha";
    static const char KDropShadow[] = "dropshadow";
    static const char KImagePath[] = "imagepath";
    static const char KSecondaryImagePath[] = "secondaryimagepath";
    static const char KScale[] = "scale";
    static const char KOffset[] = "offset";
    static const char KStretchMode[] = "stretchmode";
    static const char KOffsetX[] = "offsetx";
    static const char KOffsetY[] = "offsety";
    static const char KSkinImagePath[] = "skinimagepath";
    static const char KSecondarySkinImagePath[] = "skinsecondaryimage";
    static const char KImageResourceRef[] = "imageresourceref";
    static const char KSecondaryImageResourceRef[] = "secondaryimageresourceref";
    static const char KImageWidth[] = "imagewidth";
    static const char KImageHeight[] = "imageheight";
    static const char KColor[] = "color";
    static const char KSkinFrameImage[] = "skinframeimage";
    static const char KAnimatedSkinImage[] = "animationimage";
    static const char KFrameOuterRectTLX[] = "frameouterrecttlx";
    static const char KFrameOuterRectTLY[] = "frameouterrecttly";
    static const char KFrameOuterRectBRX[] = "frameouterrectbrx";
    static const char KFrameOuterRectBRY[] = "frameouterrectbry";
    static const char KFrameInnerRectTLX[] = "frameinnerrecttlx";
    static const char KFrameInnerRectTLY[] = "frameinnerrecttly";
    static const char KFrameInnerRectBRX[] = "frameinnerrectbrx";
    static const char KFrameInnerRectBRY[] = "frameinnerrectbry";
    static const char KLctFrameInnerrectId[] = "lctframeinnerrectid";

    namespace imagescalemodevalue
        {
        static const char KNormal[] = "normal";
        static const char KFitWidth[] = "fitwidth";
        static const char KFitHeight[] = "fitheight";
        static const char KCover[] = "cover";
        static const char KFit[] = "fit";
        static const char KFitInside[] = "fitinside";
        }

    namespace imagecolormodevalue
        {
        static const char KColorModulate[] = "colormodulate";
        static const char KColorDimmed[] = "colordimmed";
        }

    namespace stretchmodevalue
        {
        static const char KNone[] = "none";
        static const char KHorizontal[] = "horizontal";
        static const char KVertical[] = "vertical";
        static const char KFull[] = "full";
        }

    }

namespace meshvisualattributes
    {
    static const char KMeshType[] = "meshtype";
    static const char KImagePath[] = "imagepath";
    static const char KSecondaryImagePath[] = "secondaryimagepath";
    static const char KSpecularImagePath[] = "specularimagepath";
    static const char KSecondaryAlpha[] = "secondaryalpha";
    static const char KYawAngle[] = "yawangle";
    static const char KPitchAngle[] = "pitchangle";
    static const char KScale[] = "scale";

    namespace meshtypevalue
        {
        static const char KProcedural[] = "procedural";
        static const char KM3g[] = "m3g";
        }
    }

namespace attributesetter
    {
    static const char KName[] = "name";
    static const char KTargetValue[] = "targetvalue";
    static const char KSourceValue[] = "sourcevalue";
    static const char KInterpolationStyle[] = "interpolationstyle";
    static const char KTime[] = "time";
    static const char KTimeToTarget[] = "timetotarget";
    static const char KMappingFunctionId[] = "mappingfunctionid";
    static const char KType[] = "type";
    static const char KDataField[] = "datafield";

    namespace interpolationstylevalues
        {
        static const char KStyleSystemDefault[] = "stylesystemdefault";
        static const char KStyleNone[] = "stylenone";
        static const char KStyleLinear[] = "stylelinear";
        static const char KSineWave[] = "sinewave";
        static const char KDecelerate[] = "decelerate";
        static const char KStyleAccelerate[] = "styleaccelerate";
        static const char KStyleAdaptive[] = "styleadaptive";
        static const char KStyleImpulse[] = "styleimpulse";
        static const char KStyleLast[] = "stylelast";
        }

    namespace categoryvalues
        {
        static const char KData[] = "data";
        }

    namespace typevalues
        {
        static const char KStatic[] = "static";
        static const char KDynamic[] = "dynamic";
        }
    }

namespace curvepathattributes
    {
    static const char KEnableLoop[] = "enableloop";
    static const char KOrigin[] = "origin";
    static const char KOffsetX[] = "offsetx";
    static const char KOffsetY[] = "offsety";
    static const char KLine[] = "line";
    static const char KArc[] = "arc";

    namespace line
        {
        static const char KStartX[] = "startx";
        static const char KStartY[] = "starty";
        static const char KEndX[] = "endx";
        static const char KEndY[] = "endy";
        static const char KLength[] = "length";
        }

    namespace arc
        {
        static const char KOriginX[] = "originx";
        static const char KOriginY[] = "originy";
        static const char KHorizontalRadius[] = "horizontalradius";
        static const char KVerticalRadius[] = "verticalradius";
        static const char KStartAngle[] = "startangle";
        static const char KEndAngle[] = "endangle";
        static const char KLength[] = "length";
        }
    }

namespace anchorlayoutinput
    {
    static const char KChildOrdinal[] = "childordinal";
    static const char KAttachmentOrdinal[] = "attachmentordinal";
    }

namespace eventhandler
    {
    static const char KPriority[] =     "priority";
    static const char KType[] =         "type";
    static const char KState[] =        "state";
    static const char KCapture[] =      "capture";
    static const char KOwner[] =        "owner";
    }

namespace eventinput
    {
    static const char KName[] = "name";
    static const char KElementName[] = "elementname";

    namespace namevalue
        {
        static const char KKeyEvent[] =     "keyevent";
        static const char KStylus[] =       "stylus";
        static const char KCustomEvent[] =  "customevent";
        static const char KEventPasserInput[] = "eventpasserinput";
        static const char KPropertyInput[] =    "attributeinput";

        namespace keyeventvalue
            {
            static const char KScanCode[] = "scancode";
            static const char KKeyCode[] =  "keycode";
            static const char KModifier[] = "modifier";
            static const char KType[] = "type";
            }

        namespace keyeventtype
            {
            static const char KKeyUp[] = "keyup";
            static const char KKeyDown[] = "keydown";
            }

        namespace stylusvalue
            {
            static const char KElementName[] =  "elementname";
            static const char KVisualName[] =   "visualname";
            }

        namespace customeventvalue
            {
            static const char KEventId[] =  "eventid";
            static const char KUserData[] = "userdata";
            }

        namespace propertyinput
            {
            static const char KPropertyName[] =     "propertyname";
            }
        }
    }

namespace eventoutput
    {
    static const char KName[] = "name";

    namespace namevalue
        {
        static const char KCustomEvent[] =      "customevent";
        static const char KCancelEvent[] =      "cancelevent";
        static const char KAnimationCancel[] =  "animationcancel";
        static const char KAnimation[] =        "animation";
        static const char KPropertyOutput[] =   "attributeoutput";
        static const char KTactile[] =     "tactile";
        static const char KEventPasserOutput[] = "eventpasseroutput";
        static const char KAppEvent[] =         "appevent";

        namespace customevent
            {
            static const char KEventId[] =      "eventid";
            static const char KEventType[] =    "eventtype";
            static const char KWidgetName[] =   "widgetname";
            static const char KDelay[] =        "delay";
            }

        namespace cancelevent
            {
            static const char KEventId[] =      "eventid";
            static const char KWidgetName[] =   "widgetname";
            static const char KCancelAll[] = "cancelall";
            }

        namespace animationcancel
            {
            static const char KElementName[] =      "elementname";
            static const char KVisualName[] =       "visualname";
            static const char KOperationType[] =    "operationtype";
            static const char KCancelAll[] =  "cancelall";
            }

        namespace animation
            {
            static const char KAnimationTime[] =    "animationtime";
            static const char KDelay[] =            "delay";
            }
            
        namespace propertyoutput
            {
            static const char KTargetWidget[] =     "targetwidget";
            }

        namespace tactile
            {
            static const char KFeedbackType[] =     "feedbacktype";
            }

        namespace eventpasseroutput
            {
            static const char KSourceWidgetId[] = "sourcewidget";
            static const char KTargetWidgetId[] = "targetwidget";
            static const char KMapsTo[] = "mapsto";
            static const char KMapToEventCustomData[] = "customdata";
            }
        }
    }

namespace operationname
    {
    static const char KNone[] = "none";
    static const char KHide[] = "hide";
    static const char KShow[] = "show";
    static const char KEnableInput[] = "enableinput";
    static const char KDisableInput[] = "disableinput";
    static const char KRemoveAndDestroyAll[] = "removeanddestroyall";
    static const char KModifyFlags[] = "modifyflags";
    static const char KSetPrimaryImage[] = "setprimaryimage";
    static const char KSetSecondaryImage[] = "setsecondaryimage";
    static const char KSetText[] = "settext";
    static const char KEnableTransformation[] = "enabletransformation";
    static const char KDisableTransformation[] = "disabletransformation";
    static const char KSet[] = "set";
    static const char KSetWithSpeed[] = "setwithspeed";
    static const char KSetStyle[] = "setstyle";
    static const char KLoadIdentity[] = "loadidentity";
    static const char KTranslate[] = "translate";
    static const char KScale[] = "scale";
    static const char KRotate[] = "rotate";
    static const char KReplaceStep[] = "replacestep";
    }

namespace keyeventmodifier
    {
    static const char KAlt[] = "alt";
    static const char KCtrl[] = "ctrl";
    static const char KShift[] = "shift";
    static const char KFunc[] = "func";
    static const char KCapsLock[] = "capslock";
    static const char KNumLock[] = "numlock";
    static const char KScrollLock[] = "scrolllock";
    }

namespace visualconnection
    {
    static const char KTargetWidget[] =     "targetwidget";
    static const char KTargetElement[] =    "targetelement";
    static const char KTargetVisual[] =     "targetvisual";
    static const char KSourceWidget[] =     "sourcewidget";
    static const char KSourceElement[] =    "sourceelement";
    static const char KSourceVisual[] =     "sourcevisual";
    }


namespace listcontrolattributes
    {
    static const char KFocusloop[] = "focusloop";
    static const char KPagescroll[] = "pagescroll";
    static const char KExpandItemDelay[] = "expanditemdelay";
    static const char KMarkable[] = "markable";
    static const char KTactile[] = "tactile";
    static const char KScrollbarMode[] = "scrollbarmode";

    namespace markablevalue
        {
        static const char KNone[] = "none";
        static const char KSingle[] = "single";
        static const char KMulti[] = "multi";
        }

    namespace tactilevalue
        {
        static const char KEnabled[] = "enabled";
        static const char KDisabled[] = "disabled";
        }

    namespace scrollbarmodevalue
        {
        static const char KHide[] = "hide";
        static const char KShow[] = "show";
        static const char KAuto[] = "auto";
        }
    }

namespace listflowelement
    {
    static const char KParentElement[] = "parentelement";
    static const char KDefaultParentLayout[] = "defaultparentlayout";
    static const char KScrollingTime[] = "scrollingtime";
    static const char KPageScrollingTime[] = "pagescrollingtime";
    static const char KInsertingTime[] = "insertingtime";
    static const char KRemovingTime[] = "removingtime";
    static const char KChangingTime[] = "changingtime";
    static const char KFadeInTime[] = "fadeintime";
    static const char KFadeInDelay[] = "fadeindelay";
    static const char KFadeOutDelay[] = "fadeoutdelay";
    static const char KFadeOutTime[] = "fadeouttime";
    static const char KPartialsOpacity[] = "partialsopacity";
    static const char KItemHeight[] = "itemheight";
    static const char KItemWidth[] = "itemwidth";
    static const char KHorizontalAlign[] = "horizontalalign";
    static const char KScrollOffsetY[] = "scrolloffsety";
    static const char KVerticalOffset[] = "verticaloffset";
    static const char KFocusInFront[] = "focusinfront";
    }

namespace gridcontrolattributes
    {
    static const char KTactile[] = "tactile";

    namespace tactile
        {
        static const char KTactileOn[] = "on";
        static const char KTactileOff[] = "off";
        }
    }

namespace griddefaultbaseelement
    {
    static const char KLooping[] = "looping";
    static const char KFadingEffect[] = "fadingeffect";
    static const char KDefaultAnimationTime[] = "defaultanimationtime";
    static const char KSingleRow[] = "singlerow";
    static const char KSingleTapSelect[] = "singletapselect";
    static const char KFocussedIndex[] = "focussedindex";
    static const char KHorizScrollbarMode[] = "horizontalscrollbarmode";
    static const char KVertScrollbarMode[] = "verticalscrollbarmode";
    static const char KCellWidth[] = "cellwidth";
    static const char KCellHeight[] = "cellheight";
    static const char KMaxColumns[] = "maxcolumns";   

    namespace looping
        {
        static const char KLoopingOn[] = "on";
        static const char KLoopingOff[] = "off";
        }

    namespace singletapselect
        {
        static const char KSingleTapSelectOn[] = "on";
        static const char KSingleTapSelectOff[] = "off";
        }

    namespace fading
        {
        static const char KFadingOn[] = "on";
        static const char KFadingOff[] = "off";
        }

    namespace scrollbarmodevalue
        {
        static const char KHide[] = "hide";
        static const char KShow[] = "show";
        static const char KAuto[] = "auto";
        }
    }

namespace griddefaultfocuselement
    {
    static const char KImagePath[] = "imagepath";
    static const char KSkinImagePath[] = "skinimagepath";
    static const char KSkinFrameImage[] = "skinframeimage";
    }

namespace griddefaultitemelement
    {
    static const char KState[] = "state";
    static const char KSkinEnabled[] = "skinenabled";

    namespace itemstate
        {
        static const char KEnabled[] = "enabled";
        static const char KDisabled[] = "disabled";
        }
    }

namespace gridpinningbaseelement
    {
    static const char KOrientation[] = "orientation";
    static const char KFocussedIndex[] = "focussedindex";
    static const char KPinnedPosition[] = "pinnedposition";
    static const char KPinnedItemSize[] = "pinneditemsize";
    static const char KDefaultAnimationTime[] = "defaultanimationtime";
    static const char KCellWidth[] = "cellwidth";
    static const char KCellHeight[] = "cellheight";
    static const char KArrow[] = "arrow";

    namespace orientation
        {
        static const char KHorizontal[] = "horizontal";
        static const char KVertical[] = "vertical";
        }
    namespace arrow
    	{
        static const char KEnableArrow[] = "enable";
        static const char KDisableArrow[] = "disable";    	
    	}
    }

namespace scrollbarcontrolattributes
    {
    static const char KTactile[] = "tactile";

    namespace tactile
        {
        static const char KTactileOn[] = "on";
        static const char KTactileOff[] = "off";
        }
    }
    
namespace scrollbardefaultbaseelement
    {
    static const char KEnableArrows[] = "enablearrows";
    static const char KAlignment[] = "alignment";
    static const char KOrientation[] = "orientation";

    // Thumb specific Attributes
    static const char KThumbWidth[] = "thumbwidth";
    static const char KThumbOpacity[] = "thumbopacity";
    static const char KThumbMinimumSize[] = "thumbminimumsize";
    static const char KThumbAnimationTime[] = "thumbanimationtime";

    // image attributes
    static const char KTrackStartImage[] = "trackstartimage";
    static const char KTrackMiddleImage[] = "trackmiddleimage";
    static const char KTrackEndImage[] = "trackendimage";
    static const char KThumbStartImage[] = "thumbstartimage";
    static const char KThumbMiddleImage[] = "thumbmiddleimage";
    static const char KThumbEndImage[] = "thumbendimage";
    static const char KTrackStartImagePressed[] = "trackstartimagepressed";
    static const char KTrackMiddleImagePressed[] = "trackmiddleimagepressed";
    static const char KTrackEndImagePressed[] = "trackendimagepressed";
    static const char KThumbStartImagePressed[] = "thumbstartimagepressed";
    static const char KThumbMiddleImagePressed[] = "thumbmiddleimagepressed";
    static const char KThumbEndImagePressed[] = "thumbendimagepressed";
    static const char KStartArrowImage[] = "startarrowimage";
    static const char KEndArrowImage[] = "endarrowimage";

    namespace alignment
        {
        static const char KLeft[] = "left";
        static const char KRight[] = "right";
        static const char KTop[] = "top";
        static const char KBottom[] = "bottom";
        }

    namespace orientation
        {
        static const char KHorizontal[] = "horizontal";
        static const char KVertical[] = "vertical";
        }
    }

namespace anchor
    {
    static const char KAnchorType[] = "anchortype";
    static const char KAttachmentOrigin[] = "attachmentorigin";

    namespace anchortypevalue
        {
        static const char KAnchorTypeNone[] = "anchortypenone";
        static const char KAnchorTypeWidth[] = "anchortypewidth";
        static const char KAnchorTypeLeft[] = "anchortypeleft";
        static const char KAnchorTypeHCenter[] = "anchortypehcenter";
        static const char KAnchorTypeRight[] = "anchortyperight";
        static const char KAnchorTypeHeight[] = "anchortypeheight";
        static const char KAnchorTypeTop[] = "anchortypetop";
        static const char KAnchorTypeVCenter[] = "anchortypevcenter";
        static const char KAnchorTypeBottom[] = "anchortypebottom";
        static const char KAnchorTypeSize[] = "anchortypesize";
        static const char KAnchorTypeTopLeft[] = "anchortypetopleft";
        static const char KAnchorTypeTopCenter[] = "anchortypetopcenter";
        static const char KAnchorTypeTopRight[] = "anchortypetopright";
        static const char KAnchorTypeCenterLeft[] = "anchortypecenterleft";
        static const char KAnchorTypeCenter[] = "anchortypecenter";
        static const char KAnchorTypeCenterRight[] = "anchortypecenterright";
        static const char KAnchorTypeBottomLeft[] = "anchortypebottomleft";
        static const char KAnchorTypeBottomCenter[] = "anchortypebottomcenter";
        static const char KAnchorTypeBottomRight[] = "anchortypebottomright";
        }

    namespace attachmentoriginvalue
        {
        static const char KAnchorAttachmentOriginNone[] = "anchorattachmentoriginnone";
        static const char KAnchorAttachmentOriginLeft[] = "anchorattachmentoriginleft";
        static const char KAnchorAttachmentOriginHCenter[] = "anchorattachmentoriginhcenter";
        static const char KAnchorAttachmentOriginRight[] = "anchorattachmentoriginright";
        static const char KAnchorAttachmentOriginTop[] = "anchorattachmentorigintop";
        static const char KAnchorAttachmentOriginVCenter[] = "anchorattachmentoriginvcenter";
        static const char KAnchorAttachmentOriginBottom[] = "anchorattachmentoriginbottom";
        static const char KAnchorAttachmentOriginTopLeft[] = "anchorattachmentorigintopleft";
        static const char KAnchorAttachmentOriginTopCenter[] = "anchorattachmentorigintopcenter";
        static const char KAnchorAttachmentOriginTopRight[] = "anchorattachmentorigintopright";
        static const char KAnchorAttachmentOriginCenterLeft[] = "anchorattachmentorigincenterleft";
        static const char KAnchorAttachmentOriginCenter[] = "anchorattachmentorigincenter";
        static const char KAnchorAttachmentOriginCenterRight[] = "anchorattachmentorigincenterright";
        static const char KAnchorAttachmentOriginBottomLeft[] = "anchorattachmentoriginbottomleft";
        static const char KAnchorAttachmentOriginBottomCenter[] = "anchorattachmentoriginbottomcenter";
        static const char KAnchorAttachmentOriginBottomRight[] = "anchorattachmentoriginbottomright";
        }
    }

namespace metric
    {
    static const char KMagnitude[] = "magnitude";
    static const char KTextStyleId[] = "textstyleid";
    }

namespace imageresource
    {
    static const char KLogicalName[] = "logicalname";
    static const char KResourceName[] = "resourcename";
    static const char KResourceType[] = "resourcetype";

    namespace resourcetypevalue
        {
        static const char KTheme[] = "theme";
        static const char KFile[] = "file";
        }
    }

namespace controlattributes
    {
    static const char KType[] =         "type";
    static const char KScrollType[] =   "scrolltype";
    static const char KItemType[] = "itemtype";
    static const char KFocusLoop[] = "focusloop";
    static const char KPageScroll[] = "pagescroll";
    static const char KSelectionType[] = "selectiontype";
    static const char KNumModes[] = "nummodes";
    static const char KEnable[] = "enable";
    static const char KFocus[] = "focus";
    static const char KMarkable[] = "markable";
    }

namespace layoutmanager
    {
    static const char KType[] = "type";
    static const char KId[] = "id";

    namespace lctanchorlayout
        {
        static const char KWidgetAnchor[] = "widgetanchor";
        }
    }

    } // namespace duiuimodel
#endif // DUIPROPERTY_H
