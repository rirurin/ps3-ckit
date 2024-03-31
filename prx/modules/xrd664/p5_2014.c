#ifdef GAME_XRD664
// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>
// Pre-prepared libraries exist in lib
// Common includes things like printf for printing, strlen, etc.
// PRX dont have access to the usual C libraries, so any functionality that you need from it
// will have to be reimplemented in there.
#include "lib/common.h"

// SHK (Static Hook library)
#include "lib/shk.h"

// Include this to use config variables
#include "lib/config.h"

#include "p5_2014.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.

SHK_HOOK( u32, initReadUpdate, gfdTask* id, float deltaTime );
SHK_HOOK( u32, fldMainUpdate, gfdTask* id, float deltaTime );
SHK_HOOK( u32, gfdAnimationStreamRead, gfdStream* pStream);
SHK_HOOK( void, shdPersonaModelUpdate, _shd_persona_disp_control_type* work);
SHK_HOOK( char, fldCrowdLayoutReadSync, fldCrowdLayoutHeader* pData, char await );
//SHK_HOOK( int, btlActionParticipateStart, Participate* this, Package* pkg );
SHK_HOOK( char, syncRoadmapFixedData, roadmap* pWork );
SHK_HOOK( int, fldPanelRoadMapGetLayerNum, gfdTask* id );
SHK_HOOK( char, datGetFlag, int flagNo );
SHK_HOOK( char, btlAiXlsTablebtlAICheckConditional, void* this, void* spAction, u32 chara, u32 conditional );

// persona 5
// releasing winter 2014 for playstation 3

shdPMDD shdPersonaStats[464] = {0, 0, 0, 0, 0, 0, 0.5};
u32 pRoadmapCount = 0;
u32 pTextureCount = 0;

btlXlsTableEntry* btlAIConditionalTable = (btlXlsTableEntry*)0x11cdb64; // 92 entries
btlXlsTableEntry* btlAIConditionalTableUnique = (btlXlsTableEntry*)0x11cdfb4; // 29 entries

s8 gfdStreamReadInt8(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x87853c, s8, gfdStream*, pStream);
}

u8 gfdStreamReadUInt8(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x878588, u8, gfdStream*, pStream);
}

s16 gfdStreamReadInt16(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x87853c, s16, gfdStream*, pStream);
}

u16 gfdStreamReadUInt16(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x878588, u16, gfdStream*, pStream);
}

s32 gfdStreamReadInt32(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x8785d0, s32, gfdStream*, pStream);
}

u32 gfdStreamReadUInt32(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x87861c, u32, gfdStream*, pStream);
}

float gfdStreamReadFloat(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1( 0x878664, float, gfdStream*, pStream);
}

u32 gfdPropertyStreamRead(gfdStream* pStream) {
    SHK_FUNCTION_CALL_1(0x876480, u32, gfdStream*, pStream);
}

void* gfdMemAllocCore(u32 size, u32 hint, char* file, u32 line) {
    SHK_FUNCTION_CALL_4(0x884414, void*, u32, size, u32, hint, char*, file, u32, line);
}

int readEndPackFile(PAK_FILE* file) {
    SHK_FUNCTION_CALL_1(0x1627e4, int, PAK_FILE*, file);
}

int readCheckPakFile(PAK_FILE* file) {
    SHK_FUNCTION_CALL_1(0x162790, int, PAK_FILE*, file);
}

int16_t ririReadShort(uint8_t* buffer, u32* cursor) {
    int16_t ret = *(int16_t*)(buffer + *cursor);
    *cursor += 2;
    return ret;
}

uint16_t ririReadUShort(uint8_t* buffer, u32* cursor) {
    uint16_t ret = *(uint16_t*)(buffer + *cursor);
    *cursor += 2;
    return ret;
}

int ririReadInt(uint8_t* buffer, u32* cursor) {
    int ret = *(int*)(buffer + *cursor);
    *cursor += 4;
    return ret;
}

u32 ririReadUInt(uint8_t* buffer, u32* cursor) {
    u32 ret = *(u32*)(buffer + *cursor);
    *cursor += 4;
    return ret;
}

static u32 initReadUpdateHook( gfdTask* id, float deltaTime ) {
    u32 ret = SHK_CALL_HOOK( initReadUpdate, id, deltaTime );
    return ret;
}

static u32 fldMainUpdateHook( gfdTask* id, float deltaTime ) {
    fldMainWork* fmwk = (fldMainWork*)id->workData;
    if (fmwk != NULL) {
        //printf("field work: addr 0x%x, state %d", (u32)fmwk, fmwk->state);
    }
    return SHK_CALL_HOOK( fldMainUpdate, id, deltaTime );
}

// serialize animations with user properties
static u32 gfdAnimationStreamReadHook(gfdStream* pStream) {
    if (pStream != NULL && pStream->accessType & 1 != 0 && pStream->type == 3) {
        // gfd doesn't like us invoking their stream functions in the hook,
        // so we'll just read the byte stream manually
        u32 animFlags = 0;
        if (pStream->header.version > 0x1104110) {
            animFlags = *(u32*)(pStream->pBuffer + pStream->memoryPosition);
        }
        u32 animationOut = SHK_CALL_HOOK(gfdAnimationStreamRead, pStream);
        if (animFlags >> 0x17 & 1 != 0) {
            gfdPropertyStreamRead(pStream);
        }
        return animationOut;
    } else {
        return SHK_CALL_HOOK(gfdAnimationStreamRead, pStream);
    }
}

// fix crash for camp persona menu
static void shdPersonaModelUpdateHook(_shd_persona_disp_control_type* work) {
    if (work->pResPerModel != NULL) {
        //int32_t persona = work->PersonaID;
        if (work->pPMMDHandle == NULL) {
            printf("pPMMDHandle is null, setting to new value 0x%x\n", shdPersonaStats);
            work->pPMMDHandle = shdPersonaStats;
        }
        SHK_CALL_HOOK(shdPersonaModelUpdate, work);
    }
}

/*
// clt deserialization CLT
static char fldCrowdLayoutReadSyncHook(fldCrowdLayoutHeader* pData, char await) {
    if (pData != NULL && pData->hFile != NULL) {
        PAK_FILE* file = pData->hFile;
        if (await == 1) {
            readEndPackFile(pData->hFile);
            file = pData->hFile;
        }
        int status = readCheckPakFile(file);
        if (status == 0) {
            return 0;
        }
        uint8_t* buffer = pData->hFile->info.buff;
        u32 vCursor = 0;
        pData->version = ririReadUInt(buffer, &vCursor);
        pData->num = ririReadUInt(buffer, &vCursor);
        if (pData->num != 0) {
            void* alloc = gfdMemAllocCore(pData->num * 0x44, 0, __FILE__, __LINE__);
            pData->pLayout = alloc;
        }
    }
    return 1;
    // if (pData != NULL) {
    //     printf("fldCrowdLayoutRead: Data at 0x%x, version %d, %d entries\n", pData, pData->version, pData->num);
    // }
    // return SHK_CALL_HOOK(fldCrowdLayoutReadSync, pData, await);
    
}
*/

static char fldCrowdLayoutReadSyncHook(fldCrowdLayoutHeader* pData, char await) {
    return SHK_CALL_HOOK(fldCrowdLayoutReadSync, pData, await);
}

static char syncRoadmapFixedDataHook(roadmap* pWork) {
    char ret = SHK_CALL_HOOK(syncRoadmapFixedData, pWork);
    /*
    if (pWork->hRoadmapTbl != NULL) {
        u32 bufAddr = (u32)pWork->hRoadmapTbl->info.buff;
        pRoadmapCount = pWork->hRoadmapTbl->info.size / sizeof(fldPanelRoadmap);
        printf("Roadmap TBL: %s, size 0x%x @ 0x%x\n", pWork->hRoadmapTbl->info.filename, pWork->hRoadmapTbl->info.size, bufAddr);
    }
    if (pWork->hTexPackTbl != NULL) {
        pTextureCount = pWork->hTexPackTbl->info.size / sizeof(fldPanelRoadmapTex);
    }
    if (pWork->pRoadmap != NULL) {
        for (int i = 0; i < pRoadmapCount; i++) {
            fldPanelRoadmap* currMap = &pWork->pRoadmap[i];
            printf("Roadmap %d: major %d, minor %d, origin (%d, %d), id %d\n", i, currMap->major, currMap->minor, currMap->origin_x, currMap->origin_y, currMap->pack_id);
        }
    }
    if (pWork->pTexPack != NULL) {
        for (int i = 0; i < pTextureCount; i++) {
            fldPanelRoadmapTex* currTex = &pWork->pTexPack[i];
            u32 currTexAddr = (u32)currTex;
            printf("Texpack %d @ 0x%x: major %d, minor %d, layer %d pxl length %f flag %d\n", i, currTexAddr, currTex->major, currTex->id, currTex->layer_num, currTex->pxl_length, currTex->flag);
        }
    }
    */
    /*
    if (pWork->hTexPackTbl != NULL) {
        //pWork->hRoadmapTbl->info.buff
        u32 bufAddr = (u32)pWork->hTexPackTbl->info.buff;
        printf("TexPack TBL: %s, size 0x%x @ 0x%x\n", pWork->hTexPackTbl->info.filename, pWork->hTexPackTbl->info.size, bufAddr);
    }*/
    // roadmap tbl - 0xc per entry
    // texpack tbl - 0x10 per emtry
    return ret;
}

// it tries to access roadmap->pTexPack but that's nullptr for some reason idk
// it's just a copy of roadmap->pTexPackTbl (see syncRoadmapFixedData) so use that as a fallback
static int fldPanelRoadMapGetLayerNumHook(gfdTask* id) {
    if (id == NULL) {
        printf("WARNING: roadmap task is nullptr, returning 0 for roadmap layer\n");
        return 0;
    }
    roadmap* pWork = (roadmap*)id->workData;
    if (pWork == NULL) {
        printf("WARNING: roadmap task work returned nullptr, returning 0 for roadmap layer\n");
        return 0;
    }
    fldPanelRoadmapTex* firstTex = pWork->pTexPack;
    if (firstTex == NULL) {
        firstTex = pWork->pTexPackTbl;
    }
    return firstTex->layer_num;
    //printf("fldPanelRoadMap Layer: Task %s, work @ 0x%x\n", id->name.string, workAddr);
    //return SHK_CALL_HOOK(fldPanelRoadMapGetLayerNum, id);
}

// force zero when receiving an erroneus flag. roadmap stuff is broken
static char datGetFlagHook(int flagNo) {
    //printf("Getting flag 0x%x\n", flagNo);
    if (flagNo < 0 || flagNo > 0x22ff) {
        return 0;
    }
    return SHK_CALL_HOOK(datGetFlag, flagNo);
}

// if an xls table entry has a null function, the original function won't stop, it'll just try to deref it lol
static char btlAiXlsTablebtlAICheckConditionalHook(void* this, void* spAction, u32 chara, u32 conditional) {
    u32 condTblId = conditional >> 0x18;
    u32 condArg = conditional & 0xffffff;
    printf("AIXLSTABLE: character %d, cond table id %x, arg %x\n", chara, condTblId, condArg);
    return SHK_CALL_HOOK(btlAiXlsTablebtlAICheckConditional, this, spAction, chara, conditional);
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p52014Init( void ) {
    SHK_BIND_HOOK( initReadUpdate, initReadUpdateHook );
    SHK_BIND_HOOK( fldMainUpdate, fldMainUpdateHook );
    SHK_BIND_HOOK( gfdAnimationStreamRead, gfdAnimationStreamReadHook );
    SHK_BIND_HOOK( shdPersonaModelUpdate, shdPersonaModelUpdateHook );
    SHK_BIND_HOOK( fldCrowdLayoutReadSync, fldCrowdLayoutReadSyncHook );
    SHK_BIND_HOOK( syncRoadmapFixedData, syncRoadmapFixedDataHook );
    SHK_BIND_HOOK( fldPanelRoadMapGetLayerNum, fldPanelRoadMapGetLayerNumHook );
    SHK_BIND_HOOK( datGetFlag, datGetFlagHook );
    SHK_BIND_HOOK( btlAiXlsTablebtlAICheckConditional, btlAiXlsTablebtlAICheckConditionalHook );
}

void p52014Shutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif