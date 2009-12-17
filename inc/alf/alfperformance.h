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
* Description: Perf log Header.
*
*/

#ifndef ALFPERFORMANCE_H
#define ALFPERFORMANCE_H

//  INCLUDES
#include <e32debug.h>
#include <e32base.h>
#include <e32std.h>
#include <hal.h>
#include <e32cmn.h>
#include <flogger.h>
#include <f32file.h>

_LIT(KLogFileName,"alfperf.log");
_LIT(KLogFileDir,"alflogs");
_LIT(KLogFileRootDir,"c:\\logs\\");

class CAlfPerformance : public CBase
    {
    public:
        struct TPerfData
            {
            TTime iStartTime;
            TInt iStartMemory;
            };

    public:  // Constructors and destructor
        static CAlfPerformance* NewL();
        virtual ~CAlfPerformance();

    public: // New functions
        inline static void PrintUserMem(const TDesC& aMsg);
        inline static TPerfData* StartTestCase(const TDesC& aMsg = KNullDesC);
        inline static void StopTestCase(TPerfData* aData, const TDesC& aMsg = KNullDesC);
        //creates log file if it doesn't exist already
        inline static void CreateLogFile();
    private:
        CAlfPerformance();
        void ConstructL();
    };

inline void CAlfPerformance::PrintUserMem(const TDesC& aMsg)
    {
    TBuf<512> buffer;
    TInt freeRAM;
    HAL::Get(HALData::EMemoryRAMFree, freeRAM);
    RHeap heap = User::Heap();
    heap.Open();
    TInt _size = heap.Size();
    TInt largest = 0;
    TInt available = heap.Available(largest);
    heap.Close();
    _LIT( KMemoryFormat, "FreeRAM: %d kB, User: - heap %d kB, available %d kB, largest block %d kB" );
    buffer.Format(KMemoryFormat, freeRAM / 1024, _size / 1024, available / 1024, largest / 1024);
    CreateLogFile();
    //Write logs into file
    RFileLogger::WriteFormat( KLogFileDir,KLogFileName,EFileLoggingModeAppend,_L("RAM - %S - %S"), &aMsg, &buffer);
    }

inline CAlfPerformance::TPerfData* CAlfPerformance::StartTestCase(const TDesC& aMsg)
    {
    CreateLogFile();
    //Write logs into file
    RFileLogger::WriteFormat(KLogFileDir,KLogFileName,EFileLoggingModeAppend,_L("Test case %S starts"), &aMsg);
    CAlfPerformance::TPerfData* data = new (ELeave) TPerfData;
    TTime readyTime;
    data->iStartTime.HomeTime();
    User::AllocSize(data->iStartMemory);
    return data;
    }

inline void CAlfPerformance::StopTestCase(CAlfPerformance::TPerfData* aData, const TDesC& aMsg)
    {
    if(!aData)
        {
        return;
        }
    TTime readyTime;
    readyTime.HomeTime();
    TTimeIntervalMicroSeconds delay = readyTime.MicroSecondsFrom(aData->iStartTime);
    TTime transferTime(delay.Int64());
    TBuf<64> timeString;
    transferTime.FormatL(timeString, _L("- Elapsed time: %S%C microseconds"));
    TBuf<256> tmp;
    tmp.Append(timeString);

    RFileLogger::Write(KLogFileDir,KLogFileName,EFileLoggingModeAppend,tmp);
    // Memory consumption
    TInt endMemory;
    User::AllocSize(endMemory); 

    CreateLogFile();
    //Write logs into file
    RFileLogger::WriteFormat(KLogFileDir,KLogFileName,EFileLoggingModeAppend,_L("- Allocated memory: %d kB"), (endMemory - aData->iStartMemory) / 1024);
    delete aData;
    aData = NULL;
    RFileLogger::WriteFormat(KLogFileDir,KLogFileName,EFileLoggingModeAppend,_L("Test case %S ends"), &aMsg);
    }
inline void CAlfPerformance::CreateLogFile()
    {
        RFs fsSession;
        RFile file;
        User::LeaveIfError(fsSession.Connect());
        TFileName filename;
        filename.Append(KLogFileRootDir);
        filename.Append(KLogFileDir);
        filename.Append(_L("\\"));
        //create logs directory if it doesn't exist
        fsSession.MkDir(filename);
        filename.Append(KLogFileName);
        //check if log file already exists
        TInt retStatus = file.Open(fsSession,filename,EFileRead);
        if(retStatus == KErrNotFound)
            file.Create(fsSession,filename,EFileWrite);         //create a new log file
        file.Close();
        fsSession.Close();
    }
#endif // ALFPERFORMANCE_H
            
// End of File
