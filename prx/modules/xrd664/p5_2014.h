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
    gfdFloat x;
    gfdFloat y;
} gfdCommonVec2;

typedef struct {
    gfdFloat x;
    gfdFloat y;
    gfdFloat z;
} gfdCommonVec3;

typedef struct {
    gfdFloat x;
    gfdFloat y;
    gfdFloat z;
    gfdFloat w;
} gfdCommonVec4;

typedef struct {
    gfdFloat x;
    gfdFloat y;
    gfdFloat w;
    gfdFloat h;
} gfdRect;

typedef struct {
    gfdNameFlag flags;
    gfdChar *string;
    gfdInt32 length;
    gfdUInt32 hash;
} gfdName;

typedef struct gfdTask gfdTask;

struct gfdTask {
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
    gfdTask* link_parent;
    gfdTask* link_child;
    gfdTask* link_next;
    gfdTask* idle_prev;
    gfdTask* idle_next;
    gfdTask* update_prev;
    gfdTask* update_next;
    gfdTask* render_prev;
    gfdTask* render_next;
    gfdTask* pad_prev;
    gfdTask* pad_next;
    gfdTask* detach;
    gfdUID uid;
};

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

typedef struct {
    gfdInt32 type;
    gfdInt32 mode;
    gfdChar filename[128];
    gfdUInt32 size;
    gfdInt16 maxIdx;
    gfdInt16 freeCnt;
    gfdInt16 refCnt;
    gfdInt16 field92; // (padding)
    void *buff;
    gfdInt32 fd;
    //CriFsLoaderHn loader;
    void* loader;
} PAK_FILE_INFO;

typedef struct {
    PAK_FILE_INFO info;
    void *prv;
    void *next;
} PAK_FILE;

typedef struct {
    gfdUInt32 version;
    gfdUInt32 num;
    void* pLayout;
    PAK_FILE* hFile;
} fldCrowdLayoutHeader;

typedef struct {
    gfdUInt16 major;
    gfdUInt16 minor;
    gfdUInt16 envMajor;
    gfdUInt16 envMinor;
    gfdUInt16 envSub;
    gfdUInt16 divIndex;
    gfdUInt16 posIndex;
    gfdUInt16 atDngIndex;
    gfdUInt16 buildID;
    gfdUInt16 clTotalDay;
    gfdUInt16 clTime;
    gfdUInt16 clWeather;
} fldMainParam;

typedef enum {
    roadmapState_TEX_READY=1,
    roadmapState_TEX_READ=2,
    roadmapState_WHOLE=4,
    roadmapState_WHOLE_FRAME=8,
    roadmapState_VISIBLE=16,
    roadmapState_INDEPENDENT=32,
    roadmapState_MASKDRAW=64,
    roadmapState_FRAMEIN=128,
    roadmapState_FRAMEIN_ANIM=256,
    roadmapState_FRAMEOUT_ANIM=512,
    roadmapState_TEX_READ_PAUSE=1024,
    roadmapState_TEX_CHANGE=2048,
    roadmapState_ALPHA_INC=4096,
    roadmapState_ALPHA_DEC=8192,
    roadmapState_DEBUG=2147483648
} roadmapState;

typedef struct {
    gfdInt16 major;
    gfdInt16 id;
    gfdUInt32 layer_num;
    gfdFloat pxl_length;
    gfdUInt32 flag;
} fldPanelRoadmapTex;

typedef struct {
    gfdInt16 major;
    gfdInt16 minor;
    gfdInt16 origin_x;
    gfdInt16 origin_y;
    gfdInt32 pack_id;
} fldPanelRoadmap;

typedef struct {
    gfdFloat u;
    gfdFloat v;
    gfdFloat w;
    gfdFloat h;
    gfdFloat x;
    gfdFloat y;
    gfdUInt32 flag;
    gfdFloat anim_time;
} fldPanelRoadmapParts;

typedef struct {
    gfdUInt32 step;
    roadmapState state;
    PAK_FILE* hRoadmapTbl;
    //fldPanelRoadmap_t *pRoadmapTbl;
    fldPanelRoadmap* pRoadmapTbl;
    PAK_FILE* hTexPackTbl;
    //fldPanelRoadmapTex_t *pTexPackTbl;
    fldPanelRoadmapTex* pTexPackTbl;
    PAK_FILE* hPartsTbl;
    //fldPanelRoadmapParts_t *pPartsTbl;
    void* pPartsTbl;
    PAK_FILE* hDispTbl;
    //fldPanelRoadmapParts_t *pDispTbl;
    void* pDispTbl;
    //fldPanelRoadmap_t *pRoadmap;
    //fldPanelRoadmapTex_t *pTexPack;
    //fldPanelRoadmapParts_t *pParts;
    //fldPanelRoadmapParts_t *pDisp;
    //fldPanelRoadmapParts_t *pCurrentParts;
    //fldPanelRoadmapParts_t *pCurrentDisp;
    fldPanelRoadmap *pRoadmap;
    fldPanelRoadmapTex *pTexPack;
    void *pParts;
    void *pDisp;
    void *pCurrentParts;
    void *pCurrentDisp;
    //gfdTexture *pTex;
    void* pTex;
    gfdFloat alpha_ratio;
    gfdFloat alpha_2_ratio;
    uint32_t field4c;
    VmathVector3 playerSymbolGeom[6];
    gfdFloat pxlLength;
    gfdInt32 currentLayer;
    gfdRect baseRC;
    gfdRect dispRC;
    gfdRect clipRC;
    gfdInt32 layer_order;
    gfdCommonVec2 maskCircleCenter;
    gfdFloat maskCircleRadius;
    gfdCommonVec2 anim_rotCenter;
    gfdFloat anim_rot;
    gfdFloat anim_time;
    //fldMainWork_t *pParentWork;
    void* pParentWork;
    fldMainParam pFieldParam;
    //gfdTaskID pPadLog;
    gfdTask* pPadLog;
    gfdInt32 debug_parts_cursor;
    uint32_t field12c;
} roadmap;

typedef struct {
    void* func;
    char* name;
    void* field08;
} btlXlsTableEntry;

typedef struct  {
    int* vtable;
    struct SmartPointer *m_pPrev;
    struct SmartPointer *m_pNext;
} SmartPointer;

// Functions
void p52014Init(void);
void p52014Shutdown(void);

#endif
#endif