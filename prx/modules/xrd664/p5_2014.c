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

static u32 initReadUpdateHook( gfdTask* id, float deltaTime ) {
    u32 ret = SHK_CALL_HOOK( initReadUpdate, id, deltaTime );
    //char* initReadText = "CKIT TEST: initReadUpdate work at %x\n";
    //u32 updateTaskAddr = (u32)(id->workData);
    //SHK_FUNCTION_CALL_2( 0x824cf8, int, char*, initReadText, u32, updateTaskAddr );
    return ret;
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p52014Init( void ) {
    SHK_BIND_HOOK( initReadUpdate, initReadUpdateHook );
}

void p52014Shutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif