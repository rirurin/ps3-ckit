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

shdPMDD shdPersonaStats[464] = {0, 0, 0, 0, 0, 0, 0.5};

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

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p52014Init( void ) {
    SHK_BIND_HOOK( initReadUpdate, initReadUpdateHook );
    SHK_BIND_HOOK( fldMainUpdate, fldMainUpdateHook );
    SHK_BIND_HOOK( gfdAnimationStreamRead, gfdAnimationStreamReadHook );
    SHK_BIND_HOOK( shdPersonaModelUpdate, shdPersonaModelUpdateHook );
}

void p52014Shutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif