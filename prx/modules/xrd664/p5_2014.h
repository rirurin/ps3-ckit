#ifdef GAME_XRD664
#ifndef P5_2014_H
#define P5_2014_H

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

// Globals

typedef char gfdBool;
typedef int16_t gfdInt16;
typedef uint16_t gfdUInt16;
typedef int32_t gfdInt32;
typedef uint32_t gfdUInt32;
typedef float gfdFloat;

typedef gfdUInt32 gfdTaskState;

typedef gfdInt32 gfdTaskPrio;

typedef gfdUInt32 gfdNameFlag;

typedef char gfdChar;

typedef u64 gfdUID;

typedef u64 gfdUInt64;

typedef gfdUInt64 fldMainFlags;

typedef gfdUInt32 gfdPlatformType;

typedef gfdUInt16 gfdStreamType;

typedef gfdUInt16 gfdStreamAccessType;

typedef struct  {
    float r;
    float i;
    float j;
    float k;
} VmathQuat;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} VmathVector3;

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

typedef struct {
    gfdUInt32 magic;
    gfdUInt32 version;
    gfdPlatformType platform;
    gfdUInt32 reserve;
} gfdStreamHeader;

typedef struct {
    gfdStreamHeader header;
    gfdStreamType type;
    gfdStreamAccessType accessType;
    gfdInt32 position;
    gfdUInt32 memoryPosition;
    gfdUInt32 memorySize;
    uint8_t* pBuffer;
} gfdStream;

typedef struct {
    gfdFloat posX;
    gfdFloat posY;
    gfdFloat posZ;
    gfdFloat rotX;
    gfdFloat rotY;
    gfdFloat rotZ;
    gfdFloat scale;
} shdPMDD;

typedef struct {
    gfdInt32 personaID;
    gfdUInt32 rgba;
    gfdUInt32 sdwRgba;
    gfdInt32 cnt;
    gfdBool sdwFlag;
    undefined field5_0x11;
    undefined field6_0x12;
    undefined field7_0x13;
    gfdFloat depth;
    gfdFloat dlt;
    gfdBool texFlag;
    undefined field11_0x1d;
    undefined field12_0x1e;
    undefined field13_0x1f;
    gfdInt32 flag;
    //Resrc_t *pResPerModel;
    void* pResPerModel;
    undefined field16_0x28;
    undefined field17_0x29;
    undefined field18_0x2a;
    undefined field19_0x2b;
    undefined field20_0x2c;
    undefined field21_0x2d;
    undefined field22_0x2e;
    undefined field23_0x2f;
    VmathVector3 pos;
    VmathVector3 fromPos;
    VmathVector3 toPos;
    VmathVector3 rot;
    VmathVector3 fromRot;
    VmathVector3 toRot;
    shdPMDD* pPMMDHandle;
    undefined field31_0x94;
    undefined field32_0x95;
    undefined field33_0x96;
    undefined field34_0x97;
    undefined field35_0x98;
    undefined field36_0x99;
    undefined field37_0x9a;
    undefined field38_0x9b;
    undefined field39_0x9c;
    undefined field40_0x9d;
    undefined field41_0x9e;
    undefined field42_0x9f;
} _shd_persona_disp_control_type;

// Functions
void p52014Init(void);
void p52014Shutdown(void);

#endif
#endif