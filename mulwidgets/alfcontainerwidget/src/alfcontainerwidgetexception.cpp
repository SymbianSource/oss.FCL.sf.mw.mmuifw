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
* Description:  Container widget exception class.
*
*/


#include "alfcontainerwidgetexception.h"

using namespace Alf;

AlfContainerWidgetException::AlfContainerWidgetException(int aError) throw()
	: osncore::AlfException(aError)
	{
	}

AlfContainerWidgetException::AlfContainerWidgetException(int aError, const char* aInfo) throw()
	: osncore::AlfException(aError, aInfo)
	{
	mDescription = osncore::UString(aInfo);
	}

AlfContainerWidgetException::AlfContainerWidgetException(int aError, const char* aInfo, const char* aFileAndLine) throw()
	: osncore::AlfException(aError, aInfo, aFileAndLine)
	{
	mDescription = osncore::UString(aInfo);
	mDescription.append(aFileAndLine);
	}

AlfContainerWidgetException::~AlfContainerWidgetException() throw()
	{
	}

const char* AlfContainerWidgetException::what() const throw()
	{
	return mDescription.getUtf8();
	}
