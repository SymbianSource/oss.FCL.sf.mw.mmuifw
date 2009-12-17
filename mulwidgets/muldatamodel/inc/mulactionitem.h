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
* Description:  Action Data class
 *
*/


#ifndef MULACTIONITEM_H
#define MULACTIONITEM_H

#include <mul/imulvarianttype.h>
#include <alf/alfvarianttype.h>
#include <memory>

#include <osn/osndefines.h>
#include <osn/ustring.h>

namespace osncore
    {
    class UString;
    }
using namespace osncore;
    

namespace Alf
    {
    
    namespace mulactionitem
        {
        static const char* const KActionString = "actionItem";   
        }
    /*! @struct MulActionData
	 *  @brief Class holds the Action Data. Data varies on the basis of the type of 
	 *  the action. Individual data items are public and can be accessed 
	 *  once ref is obtained to the MulActionData structure. 
	 */  
	struct MulActionData
		{		
		/*! @var Contains command id in case of simple command. */
		int mCmdId;
		};
		
	/*! @struct MulActionData
	 *  @brief Client need to use this class to add action item to data model.
	 *  Action can be of type simple command or a service api. 
	 *  More data about the action is also encapsulated within the same class
	 *  Action item will represent one attribute within the visual item.
	 * 
	 */  
	class MulActionItem : public IMulVariantType//public IAlfModelBase
		{
		public:

			/*! @enum TMulActionType
			 * Action Type Definition.
			 */
			enum TMulActionType
				{
				EActionSimple	/*!< Type specifies that the action is a simple command. */
				};
			
		public:	//Constructor and Destructor
			
			/**
			 * C++ constructor
			 *
			 * @param aType Action type. Possible options are defined by TMulActionType.
     		 * @param aData Action data based on type of the action. Ownership gets transfered.
     		 * @throw Invalid_argument, In case aData is NULL.
			 */
			OSN_IMPORT MulActionItem(TMulActionType aType, MulActionData* aData);
			
			/**
			 * Copy constructor.
			 */
			 OSN_IMPORT MulActionItem( const MulActionItem& aActionItem );
			
			 /**
		 	 * Destructor.
		 	 */
			OSN_IMPORT ~MulActionItem();
	   
		public: // New methods
            
            /**
			 * Set/Change the action type of an action item.
			 * Default type of the action item is EActionSimple.
			 *
			 * @param aType Action type. Possible options are defined by TMulActionType.
			 */
			OSN_IMPORT void SetActionType( TMulActionType aType = EActionSimple );
			
			/**
			 * Returns the action type of the action item.
			 *
			 * @return Action type. Possible options are defined by TMulActionType.
			 */
			OSN_IMPORT MulActionItem::TMulActionType ActionType() const;
			
		    /**
			 * Set/Change the action data of an action item.
			 * Ownership gets transfered.
			 *
			 * @param aData Action data based on type of the action. Ownership gets transfered.
			 * @throw Invalid_argument, In case aData is NULL.
			 */
			OSN_IMPORT void SetActionData( MulActionData* aData );
			
			/**
			 * Returns the action data of the action item.
			 *
			 * @return Action data of the action item. A const reference is returned.
			 */
			OSN_IMPORT const MulActionData& ActionData() const;

		public: // from IAlfModelBase
		
		    //virtual const UString& Type();	
		    
		public: // from IMulVariantType
		
			OSN_IMPORT std::auto_ptr< IMulVariantType > Clone();
			
		private: // form IAlfVAriantType
			
			void set(IAlfVariantType& aValue) ;

			Type type() const ;

			bool boolean() const ;

			int integer() const ;

			uint uinteger() const ;
		 
			double real() const ;

			const UString& string() const ;

			IAlfContainer* container() ;

			IAlfMap* map() ;
		    
			IAlfBranch* branch() ;

			IAlfModelBase* customData() ;
			
		private: // form IAlfVariantType
	
		private:	// Class data

			/*! @var Type of the action data the class instance holds. */
			TMulActionType mType;		
			
			/*! @var Pointer to the action data. Data varies on the basis of the action type
			 *  Class owns the action data. */
			auto_ptr<MulActionData> mData; 
			
			/*! @var Pointer to the ActionString data. */
			auto_ptr<UString> mActionString; 
		};
	
    } // namespace Alf	

#endif //MULACTIONITEM_H

//End of file
