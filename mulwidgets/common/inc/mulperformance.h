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
* Description: Perf log header.
*
*/

#ifndef CMULPERF_H
#define CMULPERF_H

//  INCLUDES
#include <e32debug.h>
#include <e32base.h>
#include <e32std.h>
#include <hal.h>
#include <e32cmn.h>
#include <flogger.h>
#include <f32file.h>

_LIT(KLogFileName,"mulperf.log");
_LIT(KLogFileDir,"mullogs");
_LIT(KLogFileRootDir,"c:\\logs\\");

/**
 *  @class CMulPerf
 *
 *  Provides API to measure performance (memory and time). Results are saved to a file.
 *
 *  @lib duiengine.lib
 *  @since Series 60 5.0
 */
class CMulPerf : public CBase
    {
public:
    /**
     * Struct which holds data before measuring is started.
     */
    struct TPerfData
        {
        /**
         * Time when measure starts
         */
        TTime iStartTime;

        /**
         * Memory when measure starts
         */
        TInt iStartMemory;
        };

public:  // Constructors and destructor
    /**
     * Constructor.
     */
    static CMulPerf* NewL();

    /**
     * Destructor.
     */
    virtual ~CMulPerf();

public: // New functions
    /**
     * Prints current amount of memory to file.
     *
     * @param aMsg User defined message which is printed before amount of memory
     */
    inline static void PrintUserMem(const TDesC& aMsg);

    /**
     * Saves current amount of memory and time and returns those to user.
     *
     * @param aMsg User defined message.
     * @return TPerfData which holds current memory consumption and time.
     */
    inline static TPerfData* StartTestCase(const TDesC& aMsg = KNullDesC);

    /**
     * Reads current amount of memory and time and calculates performance using
     * values got from StartTestCase.
     *
     * @param aData Data which has got when calling StartTestCase.
     * @param aMsg User defined message.
     */
    inline static void StopTestCase(TPerfData* aData, const TDesC& aMsg = KNullDesC);

    /**
     * Create file where results are written to.
     */
    inline static void CreateLogFile();
private:
    /**
     * Internal constructor.
     */
    CMulPerf();

    /**
     * Internal constructor.
     */
    void ConstructL();
    };

inline void CMulPerf::PrintUserMem(const TDesC& aMsg)
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

inline CMulPerf::TPerfData* CMulPerf::StartTestCase(const TDesC& aMsg)
    {
    CreateLogFile();
    //Write logs into file
    RFileLogger::WriteFormat(KLogFileDir,KLogFileName,EFileLoggingModeAppend,_L("Test case %S starts"), &aMsg);
    CMulPerf::TPerfData* data = new (ELeave) TPerfData;
    TTime readyTime;
    data->iStartTime.HomeTime();
    User::AllocSize(data->iStartMemory);
    return data;
    }

inline void CMulPerf::StopTestCase(CMulPerf::TPerfData* aData, const TDesC& aMsg)
    {
    if (!aData)
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
inline void CMulPerf::CreateLogFile()
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
    if (retStatus == KErrNotFound)
        file.Create(fsSession,filename,EFileWrite);         //create a new log file
    file.Close();
    fsSession.Close();
    }
#endif // CMULPERF_H

// End of File
