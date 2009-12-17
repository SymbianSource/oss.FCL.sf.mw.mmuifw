/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Gesture class
*
*/

#include "gesture.h"

#include <e32math.h>

#include "gesturedefs.h"
#include "utils.h"

using namespace GestureHelper;

/** 
 * Point array for which only x axis is relevant
 */
class TXAxisPointArray : public TPointArray
    {
public:
    TXAxisPointArray( const RArray< TPointEntry >& aPoints )
            : TPointArray( aPoints ) {}
        
    // from TPointArray
    TPoint operator[]( TInt aIndex ) const 
        {
        return TPoint( Raw( aIndex ).iX, 0 );
        }
    };

/** 
 * Point array for which only y axis is relevant
 */
class TYAxisPointArray : public TPointArray
    {
public:
    TYAxisPointArray( const RArray< TPointEntry >& aPoints )
            : TPointArray( aPoints ) {}
    
    // from TPointArray
    TPoint operator[]( TInt aIndex ) const 
        {
        return TPoint( 0, Raw( aIndex ).iY );
        }
    };

namespace 
    {
    /** @return the current time */
    TTime CurrentTime()
        {
        TTime time;
        time.UniversalTime();
        return time;
        }

    /**
     * @param aRelevantAxis See @ref MGestureEvent::Code
     * @return gesture code by analysing the sequence of points
     */
    TGestureCode CodeFromPoints( const RArray< TPointEntry >& aPoints, 
            MGestureEvent::TAxis aRelevantAxis ) 
        {
        // select the correct filter based on aRelevantAxis
        // these filter_ objects are array decorators that will eliminate either 
        // x, y or neither coordinate of each point
        TXAxisPointArray filterY( aPoints );
        TYAxisPointArray filterX( aPoints );
        TPointArray filterNone( aPoints );
        TPointArray& filter = 
            aRelevantAxis == MGestureEvent::EAxisHorizontal ? static_cast< TPointArray& >( filterY ) : 
            aRelevantAxis == MGestureEvent::EAxisVertical   ? static_cast< TPointArray& >( filterX ) :
            /* otherwise EAxisBoth */                         filterNone;
                            
        // currently the gesture recogniser does not have any state, so it is fast
        // to instantiate. The call is not static however, to allow the recogniser
        // to be replaced by a more complicated implementation that has state.
        // then it may make sense to make the recogniser a member variable.
        return TGestureRecogniser().GestureCode( filter, aRelevantAxis );
        }

    /**
     * @param aPoints Sequence of points representing 1st pointer movement
     * @param aSecondaryPoints Sequence of points representing 2nd pointer movement
     * @param aPreviousDistance contains the previous distance aftre this function execution 
     * @return gesture code by analysing the sequence of points
     */
    TGestureCode MultiTouchCode( const RArray< TPointEntry >& aPoints, 
            const RArray< TPointEntry >& aSecondaryPoints, TInt aPreviousDistance, TBool aIsFirstPinch ) 
        {
        TPointArray filter(aPoints);
        TPointArray secondaryFilter(aSecondaryPoints);
        return TGestureRecogniser().MultiTouchGestureCode( filter, secondaryFilter, aPreviousDistance, aIsFirstPinch );
        }
    } // unnamed namespace

// ----------------------------------------------------------------------------
// constructor
// ----------------------------------------------------------------------------
//
CGesture::CGesture()
    {
    iPinchStartDistance = -1;
    iPinchEndDistance = -1;
    iPinchDetected = EFalse ; 
    }

// ----------------------------------------------------------------------------
// destructor
// ----------------------------------------------------------------------------
//
CGesture::~CGesture()
    {
    iPoints.Close();
    iSecondaryPoints.Close();
    }

// ----------------------------------------------------------------------------
// AsStartEventL
// ----------------------------------------------------------------------------
//
CGesture* CGesture::AsStartEventLC() const
    {
    __ASSERT_DEBUG( 0 < iPoints.Count(), Panic( EGesturePanicIllegalLogic ) );
    CGesture* gesture = new ( ELeave ) CGesture;
    CleanupStack::PushL( gesture );
    User::LeaveIfError( gesture->AddPoint( iPoints[0].iPos ) );
    return gesture;
    }

// ----------------------------------------------------------------------------
// ResetToLastPoint
// ----------------------------------------------------------------------------
//
void CGesture::ResetToLastPoint(TBool aSetPointerZero,TBool aSetToZero)
    {
    TPointEntry lastEntry(TPoint(0,0),TTime());
    if( aSetToZero )
        {
        __ASSERT_DEBUG( 0 < iPoints.Count(), Panic( EGesturePanicIllegalLogic ) );
        lastEntry = LastPoint( iPoints);
        }
    else
        {
        __ASSERT_DEBUG( 0 < iSecondaryPoints.Count(), Panic( EGesturePanicIllegalLogic ) );
        lastEntry = LastPoint( iSecondaryPoints);
        }
    Reset();
    if( aSetPointerZero )
        {
        iPoints.Append(lastEntry);
        }
    else
        {
        iSecondaryPoints.Append(lastEntry);  
        }     
    }

// ----------------------------------------------------------------------------
// LastPoint
// ----------------------------------------------------------------------------
//
inline const TPointEntry CGesture::LastPoint( const RArray< TPointEntry >& aPoints ) const
    {
    return aPoints[aPoints.Count() - 1];
    }

// ----------------------------------------------------------------------------
// IsPinchToleranceHigh
// ----------------------------------------------------------------------------
//
inline TBool CGesture::IsHighPinchTolerance() const
    {
  
    if( iPinchEndDistance == -1 )
        {
        return ETrue;
        }
    else
        {
        TInt currentDistance = TGestureRecogniser().Length( LastPoint(iPoints).iPos, LastPoint(iSecondaryPoints).iPos );
        // if previously zooming out and current distance corresponds to zooming in or viceversa
        if( ( iZoomState == EZoomOut && currentDistance < iPinchStartDistance) || 
                ( iZoomState == EZoomIn && currentDistance > iPinchStartDistance))
            {
            return ETrue;
            }
        }
    return EFalse;
    }
// ----------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------
//
void CGesture::Reset()
    {
    iPoints.Reset();
    iSecondaryPoints.Reset();
    iHoldingState = ENotHolding;
    iState = ENotActive;
    iHoldingPointIndex = 0;
    iVisual = NULL;
    iIsDoubleTap = EFalse;
    iPinchStartDistance = -1;
    iPinchEndDistance = -1;
    iPinchDetected = EFalse;
    iZoomState = ENoZoom;
    }
    
// ----------------------------------------------------------------------------
// IsEmpty
// ----------------------------------------------------------------------------
//
TBool CGesture::IsEmpty() const
    {
    return iPoints.Count() == 0;
    }

// ----------------------------------------------------------------------------
// IsMultiTouch
// ----------------------------------------------------------------------------
//
TBool CGesture::IsMultiTouch() const
    {
    return iSecondaryPoints.Count() == 0;
    }

// ----------------------------------------------------------------------------
// Add a point to the sequence of points that together make up the gesture
// ----------------------------------------------------------------------------
//
TInt CGesture::AddPoint( const TPoint& aPoint )
    {
    if ( !IsLatestPoint( aPoint ) )
        {
        return iPoints.Append( TPointEntry( aPoint, CurrentTime() ) );
        }
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// AddSecondaryPoint
// ----------------------------------------------------------------------------
//
TInt CGesture::AddSecondaryPoint( const TPoint& aPoint )
    {
    if ( !IsLatestSecondaryPoint( aPoint ) )
        {
        return iSecondaryPoints.Append( TPointEntry( aPoint, CurrentTime() ) );
        }
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// SetVisual
// ----------------------------------------------------------------------------
//
void CGesture::SetVisual( CAlfVisual* aVisual )
    {
    iVisual = aVisual;
    }

// ----------------------------------------------------------------------------
// IsNearHoldingPoint
// ----------------------------------------------------------------------------
//
TBool CGesture::IsNearHoldingPoint( const TPoint& aPoint ) const
    {
    return ToleranceRect( iPoints[ iHoldingPointIndex ].iPos ).Contains( aPoint );
    }

// ----------------------------------------------------------------------------
// IsLatestPoint
// ----------------------------------------------------------------------------
//
TBool CGesture::IsLatestPoint( const TPoint& aPoint ) const
    {
    if ( iPoints.Count() > 0 )
        {
        return aPoint == CurrentPos();
        }
    return EFalse;
    }
// ----------------------------------------------------------------------------
// IsLatestSecondaryPoint
// ----------------------------------------------------------------------------
//
TBool CGesture::IsLatestSecondaryPoint( const TPoint& aPoint ) const
    {
    if ( iSecondaryPoints.Count() > 0 )
        {
        return aPoint == iSecondaryPoints[ iSecondaryPoints.Count() - 1 ].iPos;
        }
    return EFalse;
    }

// ----------------------------------------------------------------------------
// StartHolding
// ----------------------------------------------------------------------------
//
void CGesture::StartHolding()
    {
    iHoldingState = EHoldStarting;
    
    // remove all points that were introduced after holding started
    for ( TInt i = iPoints.Count() - 1; i > iHoldingPointIndex; i-- )
        {
        iPoints.Remove( i );
        }
    }

// ----------------------------------------------------------------------------
// SetHoldingPoint
// ----------------------------------------------------------------------------
//
void CGesture::SetHoldingPoint()
    {
    iHoldingPointIndex = iPoints.Count() - 1;
    }

// ----------------------------------------------------------------------------
// ContinueHolding
// ----------------------------------------------------------------------------
//
void CGesture::ContinueHolding()
    {
    iHoldingState = EHolding;
    }

// ----------------------------------------------------------------------------
// SetSingleTouchActive
// ----------------------------------------------------------------------------
//
void CGesture::SetSingleTouchActive()
    {
    __ASSERT_DEBUG( ENotActive == iState, Panic( EGesturePanicIllegalLogic ) );
    iState = ESingleTouchActive;
    }
// ----------------------------------------------------------------------------
// SetMultiTouchActive
// ----------------------------------------------------------------------------
//
void CGesture::SetMultiTouchActive()
    {
    iState = EMultiTouchActive;
    iIsMultiTouched = ETrue;
    }
// ----------------------------------------------------------------------------
// SetSingleTouchReleased
// ----------------------------------------------------------------------------
//
void CGesture::SetSingleTouchReleased()
    {
    // IsMovementStopped expects corresponding SetComplete to be called before SetRelease
    __ASSERT_DEBUG( ESingleTouchComplete == iState, Panic( EGesturePanicIllegalLogic ) );
    iState = ESingleTouchReleased;
    iIsMultiTouched = EFalse;
    }
// ----------------------------------------------------------------------------
// SetMultiTouchReleased
// ----------------------------------------------------------------------------
//
void CGesture::SetMultiTouchReleased()
    {
    // IsMovementStopped expects corresponding SetComplete to be called before SetRelease
    __ASSERT_DEBUG( EMultiTouchComplete == iState, Panic( EGesturePanicIllegalLogic ) );
    iState = EMultiTouchReleased;
    }

/**
 * @return elapsed time between aStartTime and aEndTime
 */
inline TTimeIntervalMicroSeconds32 Elapsed( const TTime& aStartTime, 
                                            const TTime& aEndTime )
    {
    return aEndTime.MicroSecondsFrom( aStartTime ).Int64();
    }

// ----------------------------------------------------------------------------
// SetSingleTouchComplete
// ----------------------------------------------------------------------------
//
void CGesture::SetSingleTouchComplete()
    {
    __ASSERT_DEBUG( iPoints.Count() > 0, Panic( EGesturePanicIllegalLogic ) );
    iState = ESingleTouchComplete;
    iCompletionTime = CurrentTime();
    }

// ----------------------------------------------------------------------------
// SetMultiTouchComplete
// ----------------------------------------------------------------------------
//
void CGesture::SetMultiTouchComplete()
    {
    __ASSERT_DEBUG( iSecondaryPoints.Count() > 0, Panic( EGesturePanicIllegalLogic ) );
    iState = EMultiTouchComplete;
    }

// ----------------------------------------------------------------------------
// SetCancelled
// ----------------------------------------------------------------------------
//
void CGesture::SetCancelled()
    {
    iState = ECancelled;
    }

// ----------------------------------------------------------------------------
// SetDoubleTap
// ----------------------------------------------------------------------------
//
void CGesture::SetDoubleTap() 
    { 
    iIsDoubleTap = ETrue; 
    }

// ----------------------------------------------------------------------------
// IsTap
// ----------------------------------------------------------------------------
//
TBool CGesture::IsTap() const
    {
    if(iIsMultiTouched)
        {
        return EFalse;
        }
    return CodeFromPoints( iPoints, EAxisBoth ) == EGestureTap;
    }

// ----------------------------------------------------------------------------
// IsPinch
// ----------------------------------------------------------------------------
//
TBool CGesture::IsPinch()
    {
    if (iPinchStartDistance  == -1 )
        {
        iPinchStartDistance = TGestureRecogniser().Length( iPoints[0].iPos, iSecondaryPoints[0].iPos ) ;
        }
    // if there is a pinch detected in the last attempt then update the start distance.
    // henceforth this new distance is used as reference for calculating the pinch.
    
    if( iPinchDetected )
        {
        iPinchStartDistance = iPinchEndDistance;
        }
    iPinchDetected = MultiTouchCode( iPoints, iSecondaryPoints, iPinchStartDistance,IsHighPinchTolerance()) == EGesturePinch;
    if( iPinchDetected )
        {
        // This end distance is updated the first time the pinch is detected.
        // This is done the save the value of pinch end distnce for further refernce to
        // update the pinch start distance next time any pointer position changes.
        iPinchEndDistance = TGestureRecogniser().Length( 
                LastPoint(iPoints).iPos,LastPoint(iSecondaryPoints).iPos );
        iZoomState = iPinchEndDistance > iPinchStartDistance ? EZoomOut : EZoomIn;
        }
    return iPinchDetected;
    }

/**
 * Translates a non-holding code into a holding code
 * @param aCode original gesture code
 * @return a gesture code with hold flag applied
 */
inline TGestureCode Hold( TGestureCode aCode )
    {
    if ( aCode != EGestureStart && 
         aCode != EGestureDrag && 
         aCode != EGestureReleased && 
         aCode != EGestureUnknown )
        {
        return static_cast< TGestureCode >( aCode | EFlagHold );
        }
    return aCode;
    }
    
// ----------------------------------------------------------------------------
// Code
// ----------------------------------------------------------------------------
//
TGestureCode CGesture::Code( TAxis aRelevantAxis ) const
    {
    switch ( iState )
        {
        case ESingleTouchActive:           
            // "start" event if only first point received
            // need to check that not holding, in case user pressed stylus
            // down, and activated holding without moving the stylus
            if ( iPoints.Count() == 1 && !IsHolding() )
                {
                return EGestureStart;
                }
            // "drag" event if holding not started or holding started earlier
            else if ( iHoldingState != EHoldStarting )
                {
                // select the correct filter based on aRelevantAxis
                // these filter_ objects are array decorators that will eliminate either 
                // x, y or neither coordinate of each point
                TXAxisPointArray filterY( iPoints );
                TYAxisPointArray filterX( iPoints );
                TPointArray filterNone( iPoints );
                TPointArray& filter = 
                    aRelevantAxis == MGestureEvent::EAxisHorizontal ? static_cast< TPointArray& >( filterY ) : 
                    aRelevantAxis == MGestureEvent::EAxisVertical   ? static_cast< TPointArray& >( filterX ) :
                    /* otherwise EAxisBoth */                         filterNone;
            
                return TGestureRecogniser().ValidateDrag( filter, aRelevantAxis );
                }
            // holding was just started
            else 
                {
                return Hold( CodeFromPoints( iPoints, aRelevantAxis ) );
                }
            
        case EMultiTouchActive:
            // Only if there are some points in secondary array
            if ( iSecondaryPoints.Count() == 1 && iPoints.Count() == 1 )
                {
                return EGestureMultiTouchStart;
                }
            else
                {
                return MultiTouchCode( iPoints, iSecondaryPoints,  iPinchStartDistance,IsHighPinchTolerance() );
                }

        case ESingleTouchComplete:
			{
            if ( iIsDoubleTap )
                {
                return EGestureDoubleTap;
                }
            
            // If there was a mulitouch within the last gesture then ignore the tap
			TGestureCode gestureCode = CodeFromPoints( iPoints, aRelevantAxis );
            if( gestureCode == EGestureTap && iIsMultiTouched)
                {
                return EGestureUnknown;
                }
            return gestureCode;
            }
        case EMultiTouchComplete:
            return MultiTouchCode( iPoints, iSecondaryPoints, iPinchStartDistance,IsHighPinchTolerance());
            
        case ESingleTouchReleased:
            return EGestureReleased;
        case EMultiTouchReleased:
            return EGestureMultiTouchReleased;         
            
        case ECancelled: // fallthrough
        case ENotActive:
        default: 
            return EGestureUnknown;
        }
    }

// ----------------------------------------------------------------------------
// IsHolding
// ----------------------------------------------------------------------------
//
TBool CGesture::IsHolding() const
    {
    return iHoldingState >= EHoldStarting;
    }

// ----------------------------------------------------------------------------
// StartPos
// ----------------------------------------------------------------------------
//
TPoint CGesture::StartPos() const
    {
    // at least one point will be in the array during callback (pointer down pos)
    return iPoints[ 0 ].iPos;
    }

// ----------------------------------------------------------------------------
// CurrentPos
// ----------------------------------------------------------------------------
//
TPoint CGesture::CurrentPos() const
    {
    // at least on point will be in the array during callback (pointer down pos)
    return iPoints[ iPoints.Count() - 1 ].iPos;
    }

// ----------------------------------------------------------------------------
// IsMovementStopped
// ----------------------------------------------------------------------------
//
inline TBool CGesture::IsMovementStopped() const
    {
    // iCompletionTime is only only valid if client has called SetComplete 
    if ( iState >= ESingleTouchComplete )
        {
        return Elapsed( NthLastEntry( 1 ).iTime, iCompletionTime ).Int() > KSpeedStopTime;
        }
    return EFalse;
    }

namespace 
    {
    const TInt KFloatingPointAccuracy = 0.000001;
    
    /** @return percentage (0.0-1.0) how far aPos is from aEdge1 towards aEdge2 */
    inline TReal32 Proportion( TReal32 aPos, TReal32 aEdge1, TReal32 aEdge2 )
        {
        if ( Abs( aEdge2 - aEdge1 ) > KFloatingPointAccuracy )
            {
            return ( aPos - aEdge1 ) / ( aEdge2 - aEdge1 );
            }
        return 0; // avoid division by zero 
        }
    
    /** Edges (pixels) at which speed should be -100% or 100% */
    NONSHARABLE_STRUCT( TEdges )
        {
        TReal32 iMin;
        TReal32 iMax;
        };
        
    /** 
     * scale which allows different (coordinate -> percentage) mapping
     * between -100% to 0% and 0 and 100%
     */
    NONSHARABLE_STRUCT( TScale )
        {
        TScale( TInt aZero, const TEdges& aEdges )
                : iMin( aEdges.iMin ), iZero( aZero ), iMax( aEdges.iMax )
            {
            }
        
        /** @return aPos as a percentage between -100% and 100% in aScale */
        TReal32 Percent( TReal32 aPos ) const;

        /// coordinate where speed is -100%
        TReal32 iMin;
        /// coordinate where speed is 0%
        TReal32 iZero;
        /// coordinate where speed is 100%
        TReal32 iMax;
        };
        
    /** @convert aPos into a percentage between -100% and 100% in aScale */
    TReal32 TScale::Percent( TReal32 aPos ) const
        {
        TReal32 percent;
        if ( aPos < iZero )
            {
            // return negative percentages on the lower side of zero point
            percent = -1 * Proportion( aPos, iZero, iMin );
            }
        else 
            {
            percent = Proportion( aPos, iZero, iMax );
            }
        // constrain between -100% and 100%
        return Min( Max( percent, -1.0F ), 1.0F );
        }
    
    /** Scale in x and y dimensions */
    NONSHARABLE_STRUCT( TScale2D )
        {
        TRealPoint Percent( const TPoint& aPos ) const
            {
            return TRealPoint( iX.Percent( aPos.iX ),
                               iY.Percent( aPos.iY ) );
            }
                
        TScale iX;
        TScale iY;
        };
            
    enum TDirection { ESmaller, ELarger };
    
    /** @return the direction of pos compared to the previous pos */
    inline TDirection Direction( TInt aPos, TInt aPreviousPos )
        {
        return aPos < aPreviousPos ? ESmaller : ELarger;    
        }

    /** Direction in x and y dimensions */
    NONSHARABLE_STRUCT( TDirection2D )
        {
        TDirection iX;
        TDirection iY;
        };

    /** Return the direction (up/down) of signal at aIndex */
    inline TDirection2D Direction( TInt aIndex, const RArray< TPointEntry >& aPoints )
        {
        const TPoint& pos = aPoints[ aIndex ].iPos;
        const TPoint& prevPos = aPoints[ aIndex - 1 ].iPos;
        TDirection2D dir = { Direction( pos.iX, prevPos.iX ),
                             Direction( pos.iY, prevPos.iY ) };
        return dir;
        }   
    /** 
     * @return a position in the aLow and aHigh, so that it aProportion of
     *         of length is above the pos 
     */
    TReal32 ProportionalLength( TReal32 aLow, TReal32 aHigh, TReal32 aProportion )
        {
        return ( aHigh - aLow ) * aProportion / ( 1 + aProportion );
        }
    
    /** 
     * @return aVariableEdge scaled to new position, when the other edge changes
     *         from aOldEdge to aNewEdge, so that aOrigin maintains the *same relative
     *         position* between aVariableEdge and the other edge 
     */
    inline TReal32 ScaledEdge( TReal32 aOrigin, TReal32 aVariableEdge, 
            TReal32 aOldEdge, TReal aNewEdge )
        {
        TReal32 proportion = Proportion( aOrigin, aVariableEdge, aOldEdge );
        return ( proportion * aNewEdge - aOrigin ) / ( proportion - 1 );
        }
    
    TScale Rescale( TReal32 aPos, TDirection aDir, TDirection aPrevDir, 
            const TScale& aPrevScale, const TEdges& aEdges )
        {
        TScale scale( aPrevScale );
        if ( aPrevDir != aDir )
            {
            // the code duplication is accepted here, since it is difficult to factor out
            // while maintaining the understandability of this anyway complex algorithm
            if ( aDir == ESmaller )
                {
                scale.iMin = aEdges.iMin;
                if ( aPrevScale.iZero < aPos )
                    {
                    TReal32 proportionAboveZero = Proportion( aPos, aPrevScale.iZero, aPrevScale.iMax );
                    scale.iZero = aPos - ProportionalLength( aEdges.iMin, aPos, proportionAboveZero );
                    }
                else 
                    {
                    // adjust zero pos so that proportion between aPos, Min, and Zero pos 
                    // stay the same (Min will move to 0, aPos stays the same)
                    scale.iZero = ScaledEdge( aPos, aPrevScale.iZero, 
                        aPrevScale.iMin, aEdges.iMin );
                    }
                // adjust the upper edge to take into account the movement of zero pos
                scale.iMax = ScaledEdge( aPos, aPrevScale.iMax, 
                    aPrevScale.iZero, scale.iZero );
                }
            else // ELarger
                {
                scale.iMax = aEdges.iMax;
                if ( aPos < aPrevScale.iZero )
                    {
                    TReal32 proportionBelowZero = Proportion( aPos, aPrevScale.iZero, aPrevScale.iMin );
                    scale.iZero = aPos + ProportionalLength( aPos, aEdges.iMax, proportionBelowZero );
                    }
                else
                    {
                    // adjust zero pos so that proportion between aPos, Max, and Zero pos 
                    // stay the same (Max will move edge, aPos stays the same)
                    scale.iZero = ScaledEdge( aPos, aPrevScale.iZero, 
                        aPrevScale.iMax, aEdges.iMax );
                    }
                // adjust the lower edge to take into account the movement of zero pos
                scale.iMin = ScaledEdge( aPos, aPrevScale.iMin, 
                    aPrevScale.iZero, scale.iZero );
                }
            }
        return scale;
        }
     
    /** Edges in x and y dimensions */
    NONSHARABLE_STRUCT( TEdges2D )
        {
        TEdges iX;
        TEdges iY;
        };
    
    /** 
     * @param aEdges edges of the area in which gesture points are accepted
     * @return the scale of latest point in the list of points 
     */
    TScale2D Scale( const RArray< TPointEntry >& aPoints, const TEdges2D& aEdges )
        {
        TScale2D scale = { TScale( aPoints[0].iPos.iX, aEdges.iX ),
                           TScale( aPoints[0].iPos.iY, aEdges.iY ) };
        TInt count = aPoints.Count();
        if ( count > 1 )
            {
            // iterate the whole point list to arrive to the current scale
            TDirection2D dir( Direction( 1, aPoints ) );
            for ( TInt i = 1; i < count; i++ )
                {
                // get direction at i
                TDirection2D newDir( Direction( i, aPoints ) );
                // get new scale at i
                scale.iX = Rescale( aPoints[i - 1].iPos.iX, newDir.iX, dir.iX, scale.iX, aEdges.iX );
                scale.iY = Rescale( aPoints[i - 1].iPos.iY, newDir.iY, dir.iY, scale.iY, aEdges.iY );
                dir = newDir;
                }
            }
        return scale;
        }
    } // unnamed namespace

TRealPoint CGesture::SpeedPercent( const TRect& aEdges ) const
    {
    // x and y coordinates are easier to handle separately, extract from TRect:
    // ((iMinX, iMinY), (iMaxX, iMaxY)) -> ((iMinX, iMaxX), (iMinY, iMaxY))
    TEdges2D edges = { { aEdges.iTl.iX, aEdges.iBr.iX },
                       { aEdges.iTl.iY, aEdges.iBr.iY } };
    // work out the current scale (coordinate -> percentage mapping) from 
    // the history of points (i.e., points of current gesture). Then
    // calculate the percentage of the current position.
    return Scale( iPoints, edges ).Percent( CurrentPos() );
    }
    
// ----------------------------------------------------------------------------
// Speed
// ----------------------------------------------------------------------------
//
TRealPoint CGesture::Speed() const
    {
    const TReal32 KMicroSecondsInSecond = 1000000;
    
    // Speed is only evaluated at the end of the swipe
    // if user stops at the end of the swipe before lifting stylus,
    // speed is zero. If time is zero, return 0 speed (infinite does 
    // not make sense either). Will need to consider also earlier points 
    // and their times or start time, if this zero-speed behavior is a problem
    TRealPoint speed;
    TReal32 time = static_cast<TReal32>( TimeFromPreviousPoint().Int() ) 
        / KMicroSecondsInSecond;
    if ( !IsMovementStopped() && time > 0 )
        {
        TPoint distance = CurrentPos() - PreviousPos();
        speed.iX = static_cast<TReal32>( distance.iX ) / time;
        speed.iY = static_cast<TReal32>( distance.iY ) / time;
        }
    return speed;
    }

// ----------------------------------------------------------------------------
// Distance
// ----------------------------------------------------------------------------
//
TPoint CGesture::Distance() const
    {
    return CurrentPos() - StartPos();
    }

// ----------------------------------------------------------------------------
// Visual
// ----------------------------------------------------------------------------
//
CAlfVisual* CGesture::Visual() const
    {
    return iVisual;
    }

// ----------------------------------------------------------------------------
// TimeFromPreviousPoint
// ----------------------------------------------------------------------------
//
inline TTimeIntervalMicroSeconds32 CGesture::TimeFromPreviousPoint() const
    {
    const TInt KLatestEntryOffset = 1;
    return Elapsed( PreviousEntry().iTime, NthLastEntry( KLatestEntryOffset ).iTime );
    }

// ----------------------------------------------------------------------------
// return nth point from the end of the points array
// ----------------------------------------------------------------------------
//
inline const TPointEntry& CGesture::NthLastEntry( TInt aOffset ) const
    {
    return iPoints[ Max( iPoints.Count() - aOffset, 0 ) ];
    }

// ----------------------------------------------------------------------------
// PreviousEntry
// ----------------------------------------------------------------------------
//
inline const TPointEntry& CGesture::PreviousEntry() const
    {
    return NthLastEntry( KPreviousPointOffset );
    }

// ----------------------------------------------------------------------------
// PreviousPos
// ----------------------------------------------------------------------------
//
inline TPoint CGesture::PreviousPos() const
    {
    return PreviousEntry().iPos;
    }

// ----------------------------------------------------------------------------
// PinchPercent
// ----------------------------------------------------------------------------
//
TInt CGesture::PinchPercent() const
    {
    return (iPinchEndDistance*100/iPinchStartDistance);
    }

// ----------------------------------------------------------------------------
// PinchCentrePoint
// ----------------------------------------------------------------------------
//
TPoint CGesture::PinchCentrePoint() const
    {
    if( iPoints.Count() <= 0 || iSecondaryPoints.Count() <= 0 )
        {
        return TPoint(0,0);
        }
    TInt greaterX = iPoints[0].iPos.iX > iSecondaryPoints[0].iPos.iX ? iPoints[0].iPos.iX : iSecondaryPoints[0].iPos.iX;
    TInt greaterY = iPoints[0].iPos.iY > iSecondaryPoints[0].iPos.iY ? iPoints[0].iPos.iY : iSecondaryPoints[0].iPos.iY; 
    return TPoint( (Abs(iPoints[0].iPos.iX - iSecondaryPoints[0].iPos.iX)/2 + greaterX), (Abs(iPoints[0].iPos.iY - iSecondaryPoints[0].iPos.iY)/2 + greaterY));
    }

// end of file

