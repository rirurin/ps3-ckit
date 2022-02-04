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
SHK_HOOK( undefined8, FUN_004eaca4, int param_1 );
SHK_HOOK( void, FUN_005528f8, ParallelWipe *a1 );
SHK_HOOK( u64, FUN_00244250, CmmStruct *a1, double a2 );
SHK_HOOK( undefined8, FUN_0023c070, undefined8 a1, u32 a2, int a3 );

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.

undefined8 SelPortraitFromModelID(int param_1)
{
	undefined8 result = SHK_CALL_HOOK( FUN_004eaca4, param_1 );
	if ( GetBitflagState( 2815 ) == 1 && GetTotalDays() == 231 )
	{
		*(undefined *)(param_1 + 9) = 5;
	}
	return result;
}

void WipeParallelHook( ParallelWipe *a1 ) // needs - [ be32, 0x552960, 0x60000000 ] #removes og metaverse wipe call
{
	SHK_CALL_HOOK( FUN_005528f8, a1 );
	/*if (GetTotalDays() >= 275 )
	{
	a1->Wipe_Pac = open_file("field/panel/wipe/wipeParallelTwistPack.pac",1);
	}
	else
	{
	a1->Wipe_Pac = open_file("field/panel/wipe/wipeParallelPack.pac",1);
	} */
	return; 
}

u64 RankupCardHook( CmmStruct *a1, double a2 ) //changes Kasumi card to Sumire's if the date is past 1/1 or later
 {
	if (GetTotalDays() >= 275 && a1->Field52->cardId == 21)
	{
		a1->Field52->cardId = 23;
	}
	return SHK_CALL_HOOK( FUN_00244250, a1, a2 );
 }

undefined8 ConfRankupHook( undefined8 a1, u32 a2, int a3 ) //makes kasumi rankup trigger sumire rankup animation if the date is 1/1 or later
{
	if ( a2 == 33 && GetTotalDays() >= 275 )
	{
		a2 = 36;
	}
	SHK_CALL_HOOK( FUN_0023c070, a1, a2, a3 );
}

void SecreCInit( void )
{
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  
  SHK_BIND_HOOK( FUN_004eaca4, SelPortraitFromModelID);
  SHK_BIND_HOOK( FUN_005528f8, WipeParallelHook );
  SHK_BIND_HOOK( FUN_00244250, RankupCardHook );
  SHK_BIND_HOOK( FUN_0023c070, ConfRankupHook );
}

void SecreCShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif