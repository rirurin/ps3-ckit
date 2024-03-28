#ifdef GAME_XRD664
#ifndef P5_2014_H
#define P5_2014_H

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

// Globals

typedef int32_t gfdInt32;
typedef uint32_t gfdUInt32;

typedef gfdUInt32 gfdTaskState;

typedef gfdInt32 gfdTaskPrio;

typedef gfdUInt32 gfdNameFlag;

typedef char gfdChar;

typedef u64 gfdUID;

typedef u64 gfdUInt64;

typedef gfdUInt64 fldMainFlags;

typedef struct {
    gfdNameFlag flags;
    gfdChar *string;
    gfdInt32 length;
    gfdUInt32 hash;
} gfdName;

typedef struct {
    gfdTaskState state;
    gfdUInt32 flags;
    gfdTaskPrio updatePrio;
    gfdTaskPrio renderPrio;
    gfdName name;
    gfdUInt32 startTimer;
    gfdUInt32 shutdownTimer;
    /*
    gfdTaskFunc (*updateFunc)(gfdTaskID, gfdFloat);
    gfdTaskFunc (*renderFunc)(gfdTaskID);
    void (*shutdownFunc)(gfdTaskID);
    */
    void* updateFunc;
    void* renderFunc;
    void* shutdownFunc;
    void *workData;
    /*
    gfdTaskLink link;
    gfdTaskList idle;
    gfdTaskList update;
    gfdTaskList render;
    gfdTaskList pad;
    gfdTask *detach;
    gfdUID uid;
    */
} gfdTask;

typedef struct {
    gfdUID procUID;
    gfdUID commandUID;
    gfdUID cmnScrUID;
    gfdUID rsdScrUID;
    gfdUID btlEndUID;
    gfdUID crsFadeUID;
    gfdUID psModelUID;
    gfdUID bossScrUID;
    gfdUID fridgeUID;
    gfdTask* hParent;
    gfdTask* hProc;
    gfdTask* hCamera;
    gfdTask* hSupport;
    gfdTask* hCommand;
    gfdTask* hCmnScr;
    gfdTask* hRsdScr;
    gfdTask* hBtlEnd;
    gfdTask* hCrsFade;
    gfdTask* hAlert;
    gfdTask* hBattle;
    gfdTask* hPSModel;
    gfdTask* hBossScr;
    gfdTask* hFridge;
    fldMainFlags flags;
    gfdInt32 state;
} fldMainWork;

// Functions
void p52014Init(void);
void p52014Shutdown(void);

#endif
#endif