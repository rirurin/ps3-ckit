#ifdef GAME_XRD664
#ifndef P5_2014_H
#define P5_2014_H
#pragma pack(1)

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

// Globals

typedef u32 gfdUInt32;
typedef int gfdInt32;

typedef gfdUInt32 gfdTaskState;

typedef gfdInt32 gfdTaskPrio;

typedef gfdUInt32 gfdNameFlag;

typedef char gfdChar;

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

// Functions
void p52014Init(void);
void p52014Shutdown(void);
#pragma pop
#endif
#endif