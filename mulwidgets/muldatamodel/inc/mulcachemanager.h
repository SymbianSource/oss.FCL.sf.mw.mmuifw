/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Cache Manager class
*
*/


#ifndef MULCACHEMANAGER_H
#define MULCACHEMANAGER_H

#include <mul/mulvisualitem.h>

#include <memory>
#include <vector>
#include <map>

#include <mul/imulwidgetdef.h>
#include <mul/muldatapath.h>

#include "multree.h"
#include "mulvectoradaptor.h"

namespace osncore
    {
    class UString;  
    }
using namespace osncore;

namespace Alf
	{

//Forward Declaration
class MulVisualItem;
class IAlfMap;
class IAlfContainer;

/**
 *  Implementation class for Cache Manager.
 *  @lib 
 */

class MulCacheManager
    {
public: //structure

	/**
	 * Comparation structure
	 * Required by stl map to compare MulDataPath
	 */
	struct TCompPath
	{
	  bool operator()( MulDataPath& path1, MulDataPath& path2 ) const
	  {
	  return ( path1 < path2 ) ;
	  }
	};
	
	typedef std::map< int,MulVisualItem* > MulVisualItemMap;
	typedef std::map< int, int > MulSelectionMap;

public: //Constructor and destructor
    
    /**
     * C++ constructor.
     */
     MulCacheManager();
     
     /**
 	 * Destructor.
 	 */
	~MulCacheManager();
	
public: //New method	
	
	/**
	 * Create Node for visual items in tree. 
	 * this function must be called before adding VisualItem
	 * 
	 * @param aStartIndex Index of first item.
	 * @param aCount Number of items to be added.
	 * @param aPath
	 */
	void CreateNodes(int aStartIndex, int aCount, const MulDataPath& aPath );
	
	/**
     * Adds Visual Item in CacheManager.
     * 
     * @param aVisualItem Visual items to be added.
     * @param aItemIndex Index where item need to be added
     * 
     * @return index where item is added
     */ 
	//void AddVisualItem( std::auto_ptr<MulVisualItem> aVisualItem, int aItemIndex );
	
	/**
	 * Add Visual Item to cache manager
	 * 
	 * @param aVisualItem  Visual item to be added
	 * @param aPath Path of visual item in  tree
	 * @param aIndex Index of visual item in path
	 */
	void AddVisualItem( std::auto_ptr<MulVisualItem> aVisualItem, const MulDataPath& aPath , int aIndex );
	
	/**
     * Removes Visual Item in CacheManager.
     * 
     * @param aIndex index of item to be removed.
     * 
     * @throws invalid_argument exception
     */ 
	
	void RemoveVisualItem( int aIndex  );
	
	/**
	 * Remove Visual Item from cache manager
	 * 
	 * @param aPath Path of visual item in  tree
	 * @param aIndex Index of visual item in path
	 */
	void RemoveVisualItem( const MulDataPath& aPath , int aIndex );
	
	
	/**
	 * Remove Visual item form cache manager
	 *
	 * @param aIndex Start index of item to delete
	 * @param aCount No of items to be removed
	 * @param aPath Path from items to be removed
	 */
	void RemoveVisualItem(int aIndex, int aCount, const MulDataPath& aPath);
	
	/**
     * Updates Visual Item in CacheManager.
     * 
     * @param aVisualItem Visual items to be updated.
     * @param aItemIndex Index where item need to be added
     * @throws invalid_argument exception
     */ 
	
	void UpdateVisualItem( MulVisualItem& aVisualItem, int aItemIndex );
	
	
	/**
     * Updates Visual Item in CacheManager.
     * 
     * @param aVisualItem Visual items to be updated.
     * @param aPath Path of visual item in tree
     * @param aIndex Index of visual item in path
     * @throws invalid_argument exception
     */ 
	void UpdateVisualItem( MulVisualItem& aVisualItem, const MulDataPath& aPath, int aIndex );
		
	/**
     * Returns Visual Items to Model from CacheManager.
     * 
     * @param aIndex  Index of item to be retrieved.
     */
	
	MulVisualItem* Data( int aIndex ) const;
	
	/**
     * Returns Visual Items to Model from CacheManager.
     * 
     * @param aPath Path of visual item in tree
     * @param aIndex Index of visual item in path
     */
	MulVisualItem* Data( const MulDataPath& aPath, int aIndex ) const;
	
	/**
     * Gets Number Of items stored in the Cache Manager .
     * 
     * @return number of items in the Cache Manager
     */
	int Count() const;
	
	/**
     * Get Number of items in mentiond path
     * 
     * @param aPath Path of node
     * @return number of items 
     */
	//int Count( const MulDataPath& aPath ) const;
	
	/**
     * Get Number of childs for the given node
     * 
     * @param aPath Path of node
     * @return number of items 
     */
	//int ChildCount( const MulDataPath& aPath ) const;	
	
	/**
	 * Retrun number of items,
	 * Checks that node is expanded or not , 
	 * if expanded then add it couts child count else ignore child count.
	 * 
	 * @return Node count
	 */
	int ExpandedNodeCount( ) const;
	
	/**
	 * Return path of node of specified absoulte index
	 * 
	 * @param aAbsoluteIndex Absoulte index of node in tree
	 * @return Path to node.
	 */
	//MulDataPath Path( int aAbsoluteIndex ) const;
	
	/**
	 * Expand Node with given absolute index
	 * This Api will be called by widget to expand node.
	 * 
	 * @param aAbsoluteIndex Absolute index of node to be expanded
	 * @return Number of items affected
	 */
	//int ExpandNode( int aAbsoluteIndex );
	
	
	/**
	 * Returns absolute index
	 * 
	 * @param aPath Path to node
	 * @param aIndex index of node in path
	 * @return Absolute index
	 */
	//int PathToAbsoluteIndex( const MulDataPath& aPath, int aIndex ) const;
	
	/**
	 * Mark or Unmark item
	 * 
	 * @param aType Marking type
	 * @param aItemsIndex vector of index of items
	 * @param aPath Path/depth at wich items are to be marked/unmark
	 */
	void SetMarkedIndices( MulWidgetDef::TMulWidgetMarkingType aType ,
						   const std::vector<int>& aItemsIndex,
						   const MulDataPath& aPath );

	/**
	 * Mark or Unmark or unmark all type
	 * 
	 * @param aType Marking type
	 */
	void SetMarkedIndices( MulWidgetDef::TMulWidgetMarkingType aType = MulWidgetDef::EMulMarkAll );

	/**
	 * Return mared index for mentioned path
	 * 
	 * @param aPath Path
	 * @return vector of index
	 */
	const std::vector<int>& MarkedIndices( const MulDataPath& aPath ) ;

	/**
	* Check weather item at specified index is marked or not.
	* 
	* @param aIndex Absolute index of item
	* @return true if seleccted else otherwise
	*/	
	bool IsItemMarked( int aIndex ) const;
	
    /**
	 * Function for setting the item Marked or Unmarked. For mark and
	 * unmark operations, index of item should be provided.
	 * 
	 * @param aType Type of marking operation to be performed. 
	 * @param aItemsIndex Indices of the items to be changed
	 * @throw Invalid Argument, if type is specified as mark-all and unmark-all. 
	 */ 
    void SetMarkedIndex( MulWidgetDef::TMulWidgetMarkingType aType, int aIndex );
	
private: //New Method

	/**
	 * Remove visual Item data from map and update other keys for map
	 * 
	 * @param aPath Path of visual item in  tree
	 * @param aIndex Index of visual item in path
	 * @param aCount No of items to be removed
	 */
	void RemoveItemFromMap(const MulDataPath& aPath , int aIndex );

	/**
	 * Remove child visual Item from map
	 * 
	 * @param aPath Path of visual item in  tree
	 * @param aIndex Index of visual item in path
	 */	
	//void RemoveChildItemsFromMap(MulDataPath aPath , int aIndex );

	
	/**
	 * Update index in Map.
	 * 
	 * Once we remove any item from tree.
	 * Index of other items will be changed so the path that points
	 * to Data of Item. We now need to update path that to item.
	 * 
	 * @param aMap Map wich need to be updated
	 * @param aIndex Index from item removed
	 */
	template <typename T> void UpdataPathInMapAfterRemove( std::map<int,T >& aMap, int aIndex, int aCount );

	/**
	 * Update Path in Map.
	 * 
	 * Update the path (modify thh depth) for the child items after remove.
	 * 
	 * @param aMap Map wich need to be updated
	 * @param aParentNode, parent node
	 * @param aOldPath Old path
	 * @param aNewPath New path	 
	 * @param aIndex, index of the node in that parent node
	 */							 						 
//	template <typename T> void UpdataChildDepthInMap( 
//											std::map<MulDataPath,T,TCompPath>& aMap,
//											MulTreeNode* aParentNode,MulDataPath& aOldPath,
//								 			MulDataPath& aNewPath,int aIndex);	
	/**
	 * Update index in Map.
	 * 
	 * Once we insert any item from tree.
	 * Index of other items will be changed so the path that points
	 * to Data of Item. We now need to update path that to item.
	 * 
	 * @param aMap Map wich need to be updated
	 * @param aIndex Index at which new item inserted
	 */
	template <typename T> void UpdataPathInMapAfterInsert( std::map<int,T >& aMap, int aIndex, int aCount );
																								
	
private: //data
		
    int                         mTotalCount; // total no of items in cache
	MulVisualItemMap 		    mVisualItemMap; //store visual item in map using path as key	
	MulVectorAdaptor<int>       mSelectionVector;//store maked indices to ,used to return marking information to client
	MulSelectionMap 			mSelectionMap; //store marking information using path as key
  };
  
} // namespace Alf

#endif //MULCACHEMANAGER_H
    
//End of file
