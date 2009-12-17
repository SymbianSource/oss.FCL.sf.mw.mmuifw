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
* Description:  Defines all inline functions of the osn types.
 *
*/


#ifndef OSNTYPES_INL
#define OSNTYPES_INL

// ======== GLOBAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSize operator+
// -----------------------------------------------------------------------------
//
inline const CSize operator+( const CSize& aLeftHandSide,
                              const CSize& aRightHandSide )
    {
    return CSize( aLeftHandSide ) += aRightHandSide;
    }

// -----------------------------------------------------------------------------
// CSize operator-
// -----------------------------------------------------------------------------
//
inline const CSize operator-( const CSize& aLeftHandSide,
                              const CSize& aRightHandSide )
    {
    return CSize( aLeftHandSide ) -= aRightHandSide;
    }
    
// -----------------------------------------------------------------------------
// CPoint operator+
// -----------------------------------------------------------------------------
//
inline const CPoint operator+( const CPoint& aLeftHandSide,
                               const CPoint& aRightHandSide )
    {
    return CPoint( aLeftHandSide ) += aRightHandSide;
    }

// -----------------------------------------------------------------------------
// CSize operator-
// -----------------------------------------------------------------------------
//
inline const CPoint operator-( const CPoint& aLeftHandSide,
                               const CPoint& aRightHandSide )
    {
    return CPoint( aLeftHandSide ) -= aRightHandSide;
    }

// -----------------------------------------------------------------------------
// CRect + CPoint
// Point addition.
// -----------------------------------------------------------------------------
//
inline const CRect operator+( const CRect& aRect,
                              const CPoint& aPoint )
    {
    return CRect( aRect ) += aPoint;
    }

// -----------------------------------------------------------------------------
// CRect - CPoint
// Point substraction.
// -----------------------------------------------------------------------------
//
inline const CRect operator-( const CRect& aRect,
                              const CPoint& aPoint )
    {
    return CRect( aRect ) -= aPoint;
    }

// -----------------------------------------------------------------------------
// CRect + CSize
// Size addition.
// -----------------------------------------------------------------------------
//
inline const CRect operator+( const CRect& aRect,
                              const CSize& aSize )
    {
    return CRect( aRect ) += aSize;
    }

// -----------------------------------------------------------------------------
// CRect - CSize
// Size substraction.
// -----------------------------------------------------------------------------
//
inline const CRect operator-( const CRect& aRect,
                              const CSize& aSize )
    {
    return CRect( aRect ) -= aSize;
    }

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSize::CSize
// Default Constructor.
// -----------------------------------------------------------------------------
//
inline CSize::CSize() :
    mX( 0 ),
    mY( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CSize::CSize
// Parameterized constructor.
// -----------------------------------------------------------------------------
//
inline CSize::CSize( int32 aX, int32 aY ) :
    mX( aX ),
    mY( aY )
    {
    }

// -----------------------------------------------------------------------------
// CSize::CSize
// Copy Constructor.
// -----------------------------------------------------------------------------
//
inline CSize::CSize( const CSize& aSize ) :
    mX( aSize.mX ),
    mY( aSize.mY )
    {
    }

// -----------------------------------------------------------------------------
// CSize::AsPoint
// Assignment substraction.
// -----------------------------------------------------------------------------
//
inline CPoint CSize::AsPoint()
    {
    return CPoint( mX, mY );
    }

// -----------------------------------------------------------------------------
// CSize::operator==
// Logical comparison.
// -----------------------------------------------------------------------------
//
inline bool32 CSize::operator==( const CSize& aSize ) const
    {
    return ( mX == aSize.mX ) && ( mY == aSize.mY );
    }

// -----------------------------------------------------------------------------
// CSize::operator!=
// Logical comparison.
// -----------------------------------------------------------------------------
//
inline bool32 CSize::operator!=( const CSize& aSize ) const
    {
    // call operator== and negate it (logically)
    return !( *this == aSize );
    }

// -----------------------------------------------------------------------------
// CSize::operator+=
// Assignment addition.
// -----------------------------------------------------------------------------
//
inline CSize& CSize::operator+=( const CSize& aSize )
    {
    mX += aSize.mX;
    mY += aSize.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CSize::operator-=
// Assignment substraction.
// -----------------------------------------------------------------------------
//
inline CSize& CSize::operator-=( const CSize& aSize )
    {
    mX -= aSize.mX;
    mY -= aSize.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CPoint::CPoint
// Default Constructor.
// -----------------------------------------------------------------------------
//
inline CPoint::CPoint() :
    mX( 0 ),
    mY( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CPoint::CPoint
// Parameterized Constructor.
// -----------------------------------------------------------------------------
//
inline CPoint::CPoint( int32 aX, int32 aY ) :
    mX( aX ),
    mY( aY )
    {
    }

// -----------------------------------------------------------------------------
// CPoint::CPoint
// Copy Constructor.
// -----------------------------------------------------------------------------
//
inline CPoint::CPoint( const CPoint& aPoint ) :
    mX( aPoint.mX ),
    mY( aPoint.mY )
    {
    }

// -----------------------------------------------------------------------------
// CPoint::AsSize
// Assignment substraction.
// -----------------------------------------------------------------------------
//
inline CSize CPoint::AsSize()
    {
    return CSize( mX, mY );
    }

// -----------------------------------------------------------------------------
// CPoint::operator==
// Logical comparison.
// -----------------------------------------------------------------------------
//
inline bool32 CPoint::operator==( const CPoint& aPoint ) const
    {
    return ( mX == aPoint.mX ) && ( mY == aPoint.mY );
    }

// -----------------------------------------------------------------------------
// CPoint::operator!=
// Logical comparison.
// -----------------------------------------------------------------------------
//
inline bool32 CPoint::operator!=( const CPoint& aPoint ) const
    {
    // call operator== and negate it (logically)
    return !( *this == aPoint );
    }

// -----------------------------------------------------------------------------
// CPoint::operator+=
// Assignment addition.
// -----------------------------------------------------------------------------
//
inline CPoint& CPoint::operator+=( const CPoint& aPoint )
    {
    mX += aPoint.mX;
    mY += aPoint.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CPoint::operator-=
// Assignment substraction.
// -----------------------------------------------------------------------------
//
inline CPoint& CPoint::operator-=( const CPoint& aPoint )
    {
    mX -= aPoint.mX;
    mY -= aPoint.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CRect::CRect
// Default Constructor.
// -----------------------------------------------------------------------------
//
inline CRect::CRect() :
    mX( 0 ),
    mY( 0 ),
    mWidth( 0 ),
    mHeight( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CRect::CRect
// Parameterized Constructor.
// -----------------------------------------------------------------------------
//
inline CRect::CRect( int32 aX, int32 aY, int32 aWidth, int32 aHeight ) :
    mX( aX ),
    mY( aY ),
    mWidth( aWidth ),
    mHeight( aHeight )
    {
    }

// -----------------------------------------------------------------------------
// CRect::CRect
// Parameterized Constructor.
// -----------------------------------------------------------------------------
//
inline CRect::CRect( const CPoint& aTopLeft, const CSize& aSize ) :
    mX( aTopLeft.mX ),
    mY( aTopLeft.mY ),
    mWidth( aSize.mX ),
    mHeight( aSize.mY )
    {
    }

// -----------------------------------------------------------------------------
// CRect::CRect
// Parameterized Constructor.
// -----------------------------------------------------------------------------
//
inline CRect::CRect( const CSize& aSize ) :
    mX( 0 ),
    mY( 0 ),
    mWidth( aSize.mX ),
    mHeight( aSize.mY )
    {
    }

// -----------------------------------------------------------------------------
// CRect::CRect
// Copy Constructor.
// -----------------------------------------------------------------------------
//
inline CRect::CRect( const CRect& aRect ) :
    mX( aRect.mX ),
    mY( aRect.mY ),
    mWidth( aRect.mWidth ),
    mHeight( aRect.mHeight )
    {
    }
    
// -----------------------------------------------------------------------------
// CRect::IsEmpty
// Check for emptyness.
// -----------------------------------------------------------------------------
//
inline bool32 CRect::IsEmpty() const
    {
    // is empty if either width or heigth are zero
    return ( !mHeight ) || ( !mWidth );
    }

// -----------------------------------------------------------------------------
// CRect::Intersects
// Checks whether two rects are intersecting.
// -----------------------------------------------------------------------------
//
inline bool32 CRect::Intersects( CRect aRect ) const
    {
    if( IsEmpty() || aRect.IsEmpty() )
        {
        return false;
        }
    
    aRect.Normalize();
    
    if( ( mWidth >= 0 ) && ( mHeight >= 0 ) )
        {
        return FastIntersects( aRect );
        }
    else if ( ( mWidth < 0 ) && ( mHeight < 0 ) )
        {
        return ( mX > aRect.mX ) &&
               ( mY > aRect.mY ) &&
               ( ( mX + mWidth ) < ( aRect.mX + aRect.mWidth ) ) &&
               ( ( mY + mHeight ) < ( aRect.mY + aRect.mHeight ) );
        }
    else if ( ( mWidth >= 0 ) && ( mHeight < 0 ) )
        {
        return ( ( mX + mWidth ) > aRect.mX ) &&
               ( mY > aRect.mY ) &&
               ( mX < ( aRect.mX + aRect.mWidth ) ) &&
               ( ( mY + mHeight ) < ( aRect.mY + aRect.mHeight ) );
        }
    else // ( mWidth < 0 ) && ( mHeight >= 0 )
        {
        return ( mX > aRect.mX ) &&
               ( ( mY + mHeight ) > aRect.mY ) &&
               ( ( mX + mWidth ) < ( aRect.mX + aRect.mWidth ) ) &&
               ( mY < ( aRect.mY + aRect.mHeight ) );
        }
    }

// -----------------------------------------------------------------------------
// CRect::FastIntersects
// Checks whether two normalized rects are intersecting.
// -----------------------------------------------------------------------------
//
inline bool32 CRect::FastIntersects( const CRect& aRect ) const
    {
    return !( IsEmpty() ||                          // Empty rects do not
              aRect.IsEmpty() ||                    // intersect.
              !IsNormalized() ||                    // Non-normalized rects are
              !aRect.IsNormalized() ||              // ignored.
              ( mX + mWidth ) <= aRect.mX ||        // Top or left side of aRect
              ( mY + mHeight ) <= aRect.mY ||       // inside of this rect?
              mX >= ( aRect.mX + aRect.mWidth ) ||  // Or the other way
              mY >= ( aRect.mY + aRect.mHeight ) ); // around?
    }

// -----------------------------------------------------------------------------
// CRect::Intersection
// Changes this rect to become the intersection rect of this rect and aRect.
// -----------------------------------------------------------------------------
//
inline void CRect::Intersection( CRect aRect )
    {
    Normalize();
    aRect.Normalize();

    FastIntersection( aRect );
    }

// -----------------------------------------------------------------------------
// CRect::FastIntersection
// Changes this rect to become the intersection rect of this rect and aRect.
// Works only for normalized rectangles.
// -----------------------------------------------------------------------------
//
inline void CRect::FastIntersection( const CRect& aRect )
    {
    // if not intersecting at all, the intersection area is zero.
    if( !FastIntersects( aRect ) )
        {
        mX = 0;
        mY = 0;
        mWidth = 0;
        mHeight = 0;
        return;
        }
    // this rect's left side lies left of aRect's left side
    if( mX < aRect.mX )
	    {
		mWidth -= ( aRect.mX - mX ); // adjust this rect's width
        mX = aRect.mX;               // move this rect's left side
        }
    // this rect's top side lies above aRect's top side
	if( mY < aRect.mY )
	    {
		mHeight -= ( aRect.mY - mY ); // adjust this rect's height
        mY = aRect.mY;                // move this rect's top side
        }
    // this rect's right side lies right of aRect's right side
	if( ( mX + mWidth ) > ( aRect.mX + aRect.mWidth ) )
	    {
		mWidth = ( aRect.mX + aRect.mWidth ) - mX; // adjust this rect's width
        }
    // this rect's bottom side lies below aRect's bottom side
	if( ( mY + mHeight ) > ( aRect.mY + aRect.mHeight ) )
	    {
		mHeight = ( aRect.mY + aRect.mHeight ) - mY; // adjust this rect's
                                                     // height
        }
    }

// -----------------------------------------------------------------------------
// CRect::Normalize
// Makes sure the rect has positive width and height.
// -----------------------------------------------------------------------------
//
inline void CRect::Normalize()
    {
	if( mWidth < 0 )
	    {
		mX += mWidth;
		mWidth = -mWidth;
	    }
	if( mHeight < 0 )
	    {
		mY += mHeight;
		mHeight = -mHeight;
	    }
    }

// -----------------------------------------------------------------------------
// CRect::IsNormalized
// Tests whether is rectangle is normalized.
// -----------------------------------------------------------------------------
//
inline bool32 CRect::IsNormalized() const
    {
	return ( mWidth >= 0 ) && ( mHeight >= 0 );
    }

// -----------------------------------------------------------------------------
// CRect::operator==
// Logical comparison.
// -----------------------------------------------------------------------------
//
inline bool32 CRect::operator==( const CRect& aRect ) const
    {
    return ( mY == aRect.mY ) &&
           ( mX == aRect.mX ) &&
           ( mHeight == aRect.mHeight ) &&
           ( mWidth == aRect.mWidth );
    }

// -----------------------------------------------------------------------------
// CRect::operator!=
// Logical comparison.
// -----------------------------------------------------------------------------
//
inline bool32 CRect::operator!=( const CRect& aRect ) const
    {
    // is empty, if either width or height are zero
    return !( *this == aRect );
    }

// -----------------------------------------------------------------------------
// CPoint::operator+=
// Point addition.
// -----------------------------------------------------------------------------
//
inline CRect& CRect::operator+=( const CPoint& aPoint )
    {
    mX += aPoint.mX;
    mY += aPoint.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CPoint::operator-=
// Point substraction.
// -----------------------------------------------------------------------------
//
inline CRect& CRect::operator-=( const CPoint& aPoint )
    {
    mX -= aPoint.mX;
    mY -= aPoint.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CPoint::operator+=
// Size addition.
// -----------------------------------------------------------------------------
//
inline CRect& CRect::operator+=( const CSize& aSize )
    {
    mWidth += aSize.mX;
    mHeight += aSize.mY;
    return *this;
    }

// -----------------------------------------------------------------------------
// CPoint::operator-=
// Size substraction.
// -----------------------------------------------------------------------------
//
inline CRect& CRect::operator-=( const CSize& aSize )
    {
    mWidth -= aSize.mX;
    mHeight -= aSize.mY;
    return *this;
    }

#endif // OSNTYPES_INL__
