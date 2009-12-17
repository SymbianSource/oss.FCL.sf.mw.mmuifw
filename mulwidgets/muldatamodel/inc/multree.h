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
* Description:  Header for MulTree
*
*/


#ifndef MULTREE_H_
#define MULTREE_H_

#include <memory>
#include <vector>
#include <osn/osnnew.h>

namespace Alf
    {
//Forward declaration
class MulDataPath;
	
class MulTreeNode
	{
public:

	/**
	 * Constuctor
	 * 
	 * @param aIndex Index of this node
	 */
	MulTreeNode( int aIndex )
		{
		mParent = NULL;
		mExpandedState = false;
		mIndex = aIndex;
		}
	
	/**
	 * Constuctor
	 * 
	 * @param aParent Parent node for this node. Node dosent take ownership of aParent
	 * @param aIndex Index of this node
	 */
	MulTreeNode( MulTreeNode* aParent, int aIndex )
		{
		mParent = aParent;
		mExpandedState = false;
		mIndex = aIndex;
		}

	/**
	 * Destructor
	 */
	~MulTreeNode()
		{
		int count = mChilds.size();
		for(int i = 0 ; i < count ; ++i )
			{
			delete Child(i);
			}
		mChilds.clear();
		}
	
	/**
	 * Insert child node in this node at given index
	 * 
	 * @param aChild Child node.Takes ownership of aChild
	 * @param aIndex index at which child node to be inserted
	 */
	void InsertChild( std::auto_ptr<MulTreeNode> aChild, int aIndex )
		{
		aChild->mParent = this;
		mChilds.insert( mChilds.begin() + aIndex, aChild.get() );
		aChild.release();
		}
	
	/**
	 * Remove child node from this node.
	 * 
	 * @param aIndex Index of child that need to be removed
	 */
	void RemoveChild( int aIndex )
		{
		MulTreeNode* childNode = Child(aIndex);
		delete childNode;
		childNode = NULL;
		mChilds.erase( mChilds.begin() + aIndex );
		}
	
	/**
	 * Number of child node that this node contains
	 * 
	 * @return Number of childs
	 */
	int ChildCount() const
		{
		return mChilds.size();
		}
	
	/**
	 * Return child node at given index
	 * 
	 * @param aIndex index of child node
	 * @return Child node at given index
	 */
	MulTreeNode* Child( int aIndex )
		{
		//At checks for out of bound condition we dont need to check again
		return mChilds.at(aIndex);
		}
	
	/**
	 * Check that weather this node is parent node and has any childs
	 * 
	 * @retrun true if node has cilds false otherwise
	 */
	bool HasChild()	const		
		{	
		return ( ChildCount() > 0 );	
		}
	
	/**
	 * Return parent of this node
	 * 
	 * @return Parent of node
	 */
	MulTreeNode* Parent() const
		{
		return mParent;
		}
	
	/**
	 * Set Index of node to current path
	 * 
	 * @param aIndex index of node
	 */
	void SetIndex( int aIndex )
		{
		mIndex = aIndex;
		}

	/**
	 * Return index of node
	 * 
	 * @return index of node.
	 */
	int Index() const
		{
		return mIndex;
		}
	
	/**
	 * Expand or unexpand node.
	 * 
	 * @param aExpendedState New state of node
	 */
	void SetExpanded( bool aExpandedState )
		{
		mExpandedState = aExpandedState;
		}
	
	/**
	 * Return weather this node is expended or not
	 * 
	 * @return true if expended, false otherwise
	 */
	bool IsExpanded() const
		{
		return mExpandedState;
		}
	
private:

	MulTreeNode* mParent; //not own
	int mIndex;
	bool mExpandedState;
	std::vector<MulTreeNode*> mChilds; //owns all children	
	};

class MulTree 
	{
public: //Constructor and Destructor

	/**
	 * C++ Constructor
	 */
	MulTree()
		{
		//create root node for tree
		mRootNode.reset( new (EMM) MulTreeNode(0) );
		mRootNode->SetExpanded( true );
		mRootNode->SetIndex(0);
		}

	/**
	 * Destructor
	 */
	~MulTree()
		{
		}
	
public: //New method
	
	/**
	 * Create new node at specified index
	 * 
	 * @param aPath Path to parent node
	 * @param aIndex Position at which node to be inserted in parent node
	 */
	void AddNode( const MulDataPath& aPath, int aIndex );
	
	/**
	 * Remove node from specified index
	 * 
	 * @param aPath Path to parent node
	 * @param aIndex Posiotin at which node to be remove from parent node
	 */
	void RemoveNode( const MulDataPath& aPath, int aIndex );
	
	/**
	 * Finds node with specified path
	 * 
	 * @param aPath path of node to be find
	 * @return Node does not return ownership
	 */
    MulTreeNode* FindNode( const MulDataPath& aPath ) const;

	/**
	 * Finds node with specified index in specified node
	 * 
	 * @param aParentNode Parent node in which search required to be made
	 * @param aIndex Index of node to be find
	 * @return Node does not return ownership
	 */
	MulTreeNode* FindChildNode( MulTreeNode* aParentNode, int aIndex ) const;
	
	/**
	 * Find node with specified absolute index and return path to that node.
	 * 
	 * @param aIndex Index of node
	 * @return Path to node
	 */
	MulDataPath FindNode( int aIndex ) const;
		
	/**
	 * Create and return path for given node
	 * 
	 * @param aNode Node of which path is required
	 * @return Path of node
	 */
	MulDataPath Path( MulTreeNode& aNode ) const;
	
	/**
	 * Return number of node this tree contains
	 * 
	 * @return Number of nodes
	 */
	int NodeCount() const;
	
	/**
	 * Return number of node this node branch
	 * 
	 * @return Number of nodes
	 */
	int NodeCount(MulTreeNode* aCurrentNode ) const;
	
	/**
	 * Return number of items,
	 * Checks that node is expanded or not, 
	 * if expanded then add it counts child count else ignore child count.
	 * 
	 * @return Node count
	 */
	int ExpandedNodeCount() const;
	
	/**
	 * Return number of items,
	 * Checks that node is expanded or not, 
	 * if expanded then add it couts child count else ignore child count.
	 * 
	 * @param aCurrentNode Node of which count is needed
	 * 
	 * @return Node count
	 */
	int ExpandedNodeCount(MulTreeNode* aCurrentNode ) const;
	
	/**
	 * Returns node's absolute index
	 * 
	 * @param aPath Path to node
	 * @param aIndex index of node in path
	 * @return position of node in whole tree
	 */
	int NodeIndex( const MulDataPath& aPath, int aIndex ) const;
		
private:	

	/**
	 * Find node with specified absolute index and return path to that node.
	 * 
	 * @param aNode Node in which index to be find
	 * @param aIndex Index of node
	 * @return Path to node
	 */
	MulDataPath FindNode( MulTreeNode* aNode, int& aAbsoluteIndex, int aIndex ) const;
	
	/**
	 * Returns node's absolute index
	 * 
	 * @param aCurrentNode Node to compare path with
	 * @param aPath Path of node which absolute index is required
	 * @param aAbsoluteIndex Counter variable
	 * @return position of node in whole tree
	 */
	int NodeIndex( MulTreeNode* aCurrentNode, const MulDataPath& aPath,int& aAbsoluteIndex ) const;
	
private: //data

	std::auto_ptr<MulTreeNode> mRootNode; //root of tree - owned
	};
	
	} //namespace Alf

#endif /*MULTREE_H_*/
