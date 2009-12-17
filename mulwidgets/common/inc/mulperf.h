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


#ifndef MULPERF_H
#define MULPERF_H
#include "mului.hrh"
#ifdef MUL_DEBUG_PERFORMANCE
#include "MULperformance.h"
#define MUL_PERF_START(p1,p2) CMulPerf::TPerfData* p1 = CMulPerf::StartTestCase(_L(p2));
#define MUL_PERF_STOP(p1,p2) CMulPerf::StopTestCase(p1,_L(p2));
#else
#define MUL_PERF_START(p1,p2)
#define MUL_PERF_STOP(p1,p2)
#endif

#endif //MULPERF_H