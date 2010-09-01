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
* Description:  MulTree Implementaiton
*
*/


#include "multree.h"

#include <stdexcept>

#include "mulassert.h"
#include "mulmodeldef.h"
#include "muldatapath.h"

namespace Alf
	{

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// AddNode
// -----------------------------------------------------------------------------
//
void MulTree::AddNode(const MulDataPath& aPath,int aIndex)
	{
	//Find parent node
	MulTreeNode* node = FindNode( aPath );
	
	//update index of other child
	for( int i = 0 ; i < node->ChildCount() ; i++ )
		{
		MulTreeNode* childNode = node->Child(i);
		int index = childNode->Index();
		if( childNode->Index() >= aIndex )
			{
			childNode->SetIndex( childNode->Index()+1 );
			}
		}	
	
	//insert node in to parent node
	std::auto_ptr<MulTreeNode> childNode( new (EMM) MulTreeNode(aIndex));
	node->InsertChild(childNode,aIndex);
	}

// -----------------------------------------------------------------------------
// RemoveNode
// -----------------------------------------------------------------------------
//
void MulTree::RemoveNode(const MulDataPath& aPath, int aIndex )
	{
	MulTreeNode* node = FindNode( aPath );
	node->RemoveChild(aIndex);
	
	//update index of other child
	for( int i = 0 ; i < node->ChildCount() ; i++ )
		{
		MulTreeNode* childNode = node->Child(i);
		int index = childNode->Index();
		if( childNode->Index() > aIndex )
			{
			childNode->SetIndex( childNode->Index()-1 );
			}
		}	
	}

// -----------------------------------------------------------------------------
// FindNode
// -----------------------------------------------------------------------------
//
MulTreeNode* MulTree::FindNode( const MulDataPath& aPath ) const
	{	
	MulTreeNode* currentNode = mRootNode.get();
	for( int i = 0; i < aPath.Depth() ;i++ )
		{
		int index = aPath[i];
		if( currentNode->HasChild() )
			{
			currentNode = FindChildNode( currentNode,index);
			}
		else
			{
			__MUL_ASSERT_DEBUG( false , KLInvalidArgument );
			}
		}
	return currentNode;
	}

// -----------------------------------------------------------------------------
// FindNode
// -----------------------------------------------------------------------------
//
MulTreeNode* MulTree::FindChildNode(MulTreeNode* aParentNode,int aIndex) const
	{
	MulTreeNode* currentNode = aParentNode;
	MulTreeNode* foundNode = NULL;
	int index = 0; 
	bool found(false);
	int childCount = currentNode->ChildCount();
	while( !found && index < childCount )
		{	
		MulTreeNode* childNode = currentNode->Child(index);
		if( aIndex == childNode->Index() )
			{
			found = true;
			foundNode = childNode;
			}
		index++;
		}
		
	if( found == false )
		{
		__MUL_ASSERT_DEBUG( false , KLInvalidArgument );
		}
		
	return foundNode;
	}

// -----------------------------------------------------------------------------
// FindNode
// -----------------------------------------------------------------------------
//
MulDataPath MulTree::FindNode( int aIndex ) const
	{	
	int absoluteIndex = -1;
	return FindNode( mRootNode.get(), absoluteIndex, aIndex );	
	}

// -----------------------------------------------------------------------------
// FindNode
// -----------------------------------------------------------------------------
//
MulDataPath MulTree::FindNode( MulTreeNode* aNode, int& aAbsoluteIndex, int aIndex ) const
	{	
	MulTreeNode* currentNode = aNode;
	if( currentNode->HasChild() )
		{
		for(int i = 0 ; i < currentNode->ChildCount() ; i++ )
			{
			aAbsoluteIndex++;
			MulTreeNode* childNode = currentNode->Child(i);
			
#ifdef _DEBUG
			bool hasChild = childNode->HasChild();
			int count = childNode->ChildCount();
			bool isExpanded = childNode->IsExpanded();
#endif //_DEBUG
			
			if( aAbsoluteIndex == aIndex )
				{
				MulDataPath path= Path(*childNode);
				path.SetIndex(i);
				return path;
				}
			 else if( childNode->HasChild() && childNode->IsExpanded() )
				{
				MulDataPath path = FindNode( childNode, aAbsoluteIndex , aIndex );
				if(path.Index() != KNotInitialized )
					{
					return path;
					}
				}			
			}
		}
	//__MUL_ASSERT_DEBUG( false , KLInvalidArgument );
	MulDataPath path;
	path.SetIndex(KNotInitialized);
	return path; 
	}

// -----------------------------------------------------------------------------
// Path
// -----------------------------------------------------------------------------
//
MulDataPath MulTree::Path(MulTreeNode& aNode) const
	{
	MulTreeNode* parentNode = aNode.Parent();
	std::vector<int> pathArray;
	while( parentNode && parentNode != mRootNode.get() )
		{
		int index = parentNode->Index();
		pathArray.push_back( parentNode->Index() );
		parentNode = parentNode->Parent();
		}
	
	int count = pathArray.size();
	
	MulDataPath path;
	for( int i= pathArray.size()- 1 ; i >=0 ; i-- )
		{
		path.Append( pathArray[i] );
		}
	pathArray.clear();
	return path;
	}

// -----------------------------------------------------------------------------
// NodeCount
// -----------------------------------------------------------------------------
//
int MulTree::NodeCount() const
	{
	int nodeCount =NodeCount( mRootNode.get() );
	return nodeCount;
	}

// -----------------------------------------------------------------------------
// NodeCount
// -----------------------------------------------------------------------------
//
int MulTree::NodeCount(MulTreeNode* aCurrentNode ) const
	{
	int nodeCount = 0;
	if(aCurrentNode->HasChild())
		{
		nodeCount = aCurrentNode->ChildCount();
		for(int i=0;i< aCurrentNode->ChildCount();i++)
			{
			nodeCount+= NodeCount(aCurrentNode->Child(i));
			}
		}
	return nodeCount;
	}

// -----------------------------------------------------------------------------
// ExpandedNodeCount
// -----------------------------------------------------------------------------
//
int MulTree::ExpandedNodeCount() const
	{
	int nodeCount = ExpandedNodeCount( mRootNode.get() );
	return nodeCount;
	}

// -----------------------------------------------------------------------------
// ExpandedNodeCount
// -----------------------------------------------------------------------------
//
int MulTree::ExpandedNodeCount(MulTreeNode* aCurrentNode ) const
	{
	int nodeCount = 0;
	if( aCurrentNode->HasChild() && aCurrentNode->IsExpanded() )
		{
		nodeCount = aCurrentNode->ChildCount();
		for( int i=0; i< aCurrentNode->ChildCount() ; i++ )
			{
			nodeCount+= ExpandedNodeCount( aCurrentNode->Child(i) );
			}
		}
	return nodeCount;
	}

// -----------------------------------------------------------------------------
// NodeIndex
// -----------------------------------------------------------------------------
//
int MulTree::NodeIndex( const MulDataPath& aPath, int aIndex ) const
	{
	MulDataPath path( aPath );
	path.SetIndex( aIndex );
	
	int absoluteIndex = -1;
	int index = NodeIndex( mRootNode.get(), path, absoluteIndex );
	
	return index;
	}

// -----------------------------------------------------------------------------
// NodeIndex
// -----------------------------------------------------------------------------
//
int MulTree::NodeIndex( MulTreeNode* aCurrentNode, const MulDataPath& aPath, int& aAbsoluteIndex ) const
	{	
	MulTreeNode* currentNode = aCurrentNode;
	if( currentNode->HasChild() )
		{
		for(int i = 0 ; i < currentNode->ChildCount() ; i++ )
			{
			aAbsoluteIndex++;
			MulTreeNode* childNode = currentNode->Child(i);
			
#ifdef _DEBUG
			bool hasChild = childNode->HasChild();
			int count = childNode->ChildCount();
			bool isExpanded = childNode->IsExpanded();
#endif //_DEBUG
			
			MulDataPath path = Path(*childNode);
			path.SetIndex(i);
			
			if( path.IsEqual(aPath) )
				{
				return aAbsoluteIndex;
				}
			 else if( childNode->HasChild() && childNode->IsExpanded() )
				{
				int index = NodeIndex( childNode, aPath, aAbsoluteIndex );
				if( index != KNotInitialized )
					{
					return index;
					}
				}			
			}
		}
	return KNotInitialized; 
	}
		
	} //namespace Alf
	
	//End of file
