/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   AutoPtr class for array.
*
*/


/**
 *  Holds reference to an auto_arrayptr. Prevents unsafe copying
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */  
      
    template<typename Y>
    struct auto_arrayptr_ref
        {
       Y* iPtr;
       
       explicit
       auto_arrayptr_ref(Y* aPtr): iPtr(aPtr) { }
        };

/**
 *  Auto pointer template implementation for C++ arrays
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */  
   template<typename X>
     class auto_arrayptr
     {
     private:
        X* iPtr;
       
     public:
       
       typedef X element_type;
       
   /**
     * Constructor.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */       
       explicit
       auto_arrayptr(X* p = 0) throw() : iPtr(p) { }
    /**
     * Copy argument a to this object. Owned pointer ownership is transferred to this.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   
       
       auto_arrayptr(auto_arrayptr& a) throw() : iPtr(a.release()) { }
    /**
     * Copy argument a to this object. Owned pointer ownership is transferred to this.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   
       
       template<typename Y>
         auto_arrayptr(auto_arrayptr<Y>& a) throw() : iPtr(a.release()) { }
    
    /**
     * Assign argument a to this object. If this already owned pointer that pointer is deleted.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   
       auto_arrayptr& operator=(auto_arrayptr& a) throw()
            {
            reset(a.release());
            return *this;
            }
    /**
     * Assign argument a to this object. If this already owned pointer that pointer is deleted.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   

       template<typename Y>
       auto_arrayptr& operator=(auto_arrayptr<Y>& a) throw()
            {
            reset(a.release());
            return *this;
        }
    /**
     * Deletes the underlying pointer.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   
     ~auto_arrayptr() { delete [] iPtr; }

    /**
     * Return owned pointer
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
        
    X* get() const throw() { return iPtr; }
    /**
     * Releases ownership of the managed pointer and returns that pointer. 
     * The *this object is left holding a null pointer.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
    
    X* release() throw()
            {
             X* tmp = iPtr;
             iPtr = 0;
             return tmp;
            }
            
    /**
     * Reset managed pointer. Deletes previously owned pointer. 
     * The *this object is holding given pointer.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */       
       void reset(X* aPtr = 0) throw()
            {
            if (aPtr != iPtr)
               {
                 delete [] iPtr;
                 iPtr = aPtr;
               }
            }
        
       
    /**
     * Constructor for value object.Uses type conversion function.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */     
       auto_arrayptr(auto_arrayptr_ref<X> aRef) throw()
        : iPtr(aRef.iPtr) { }
     
    /**
     * Assign argument a (value object) to this object. If this already owned pointer that pointer is deleted.
     * Uses type conversion function.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */      
       auto_arrayptr& operator=(auto_arrayptr_ref<X> aRef) throw()
            {
            if (aRef.iPtr != this->get())
                {
                delete [] iPtr;
                iPtr = aRef.iPtr;
                }
            return *this;
            }
       
    /**
     * Type conversion. Constructs an auto_arrayptr_ref from *this and returns it.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   
       template<typename Y>
       operator auto_arrayptr_ref<Y>() throw()
         { return auto_arrayptr_ref<Y>(this->release()); }
 
    /**
     * Type conversion. Constructs a new auto_arrayptr using the underlying pointer held by *this. 
     * Calls release() on *this, so *this no longer possesses the pointer.
     * Returns the new auto_arrayptr.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */   
       template<typename Y>
       operator auto_arrayptr<Y>() throw()
         { return auto_arrayptr<Y>(this->release()); }
     };