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
* Description:  Defines macros to log performance related details
*
*/


#ifndef ALFPERF_H
#define ALFPERF_H
#include "alf/alfwidgetmodeldefines.hrh"
#ifdef ALF_DEBUG_PERFORMANCE
#include "alf/alfperformance.h"
  #define ALF_PERF_START(p1,p2) CAlfPerformance::TPerfData* p1=CAlfPerformance::StartTestCase(_L(p2));
  #define ALF_PERF_STOP(p1,p2) CAlfPerformance::StopTestCase(p1,_L(p2));
#else
  #define ALF_PERF_START(p1,p2)
  #define ALF_PERF_STOP(p1,p2)
#endif

#endif //ALFPERF_H