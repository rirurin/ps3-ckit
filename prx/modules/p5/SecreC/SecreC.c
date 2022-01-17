#ifdef GAME_P5
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

#include "modules/p5/p5.h"
#include "SecreC.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )


// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, FUN_004eae40, u16* param_1, int param_2, int param_3 );
//SHK_HOOK( void, FUN_005528f8, undefined4* param_1 );

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
static void FUN_004eae40Hook( u16* param_1, int param_2, int param_3 )
{	
	if ( GetBitflagState( 0x2130 ) == 0 )
	{
		//char bufferThing[64];
		u8 bVar1;
		SHK_CALL_HOOK( FUN_004eae40, param_1, param_2, param_3 );
		bVar1 = *(u8 *)((int)param_1 + 9);
		//bVar1 = (bVar1) / ((int)param_1 + 9);
		printf( "titties %d\n", bVar1 );
	}
	else
	{
		SHK_CALL_HOOK( FUN_004eae40, param_1, param_2, param_3 );
	}
	return;
}

/*static void FUN_005528f8Hook(undefined4* param_1)

{
	undefined4* puVar4;
	SHK_CALL_HOOK( FUN_005528f8, param_1 );
	if ( GetTotalDays() >= 214 )
    {
		puVar4 = sprintf("field/panel/wipe/wipeParallelTwistPack.pac", 0);
	}
	else
	{
		puVar4 = sprintf("field/panel/wipe/wipeParallelPack.pac", 0);
	}
  *param_1 = puVar4;
  *(undefined *)(param_1 + 0x45) = 1;
  return;
} */

void SecreCInit( void )
{
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  
  SHK_BIND_HOOK( FUN_004eae40, FUN_004eae40Hook );
  //SHK_BIND_HOOK( FUN_005528f8, FUN_005528f8Hook );
}

void SecreCShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif