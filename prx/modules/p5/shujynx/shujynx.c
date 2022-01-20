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
#include "Shujynx.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )


// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, GetFieldIDsFromEncountTable, int param_1, u32 param_2, short *param_3, short *param_4);
SHK_HOOK( int, FUN_00305468, u32 param_1, u16 param_2);
SHK_HOOK( EncounterStruct *, GetDataFromEncountTBL, u32 param_1);
// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.

static void GetFieldIDsFromEncountTableHook( int param_1, u32 param_2, short *param_3, short *param_4 )
{
  EncounterStruct *pEVar1;
  int iVar2;
  short sVar3;
  
  if (-1 < (int)param_2) {
    pEVar1 = SHK_CALL_HOOK(GetDataFromEncountTBL, param_2 & 0xffff);
    *param_3 = 0;
    *param_4 = 100;
    return;
  }
  *param_3 = *(short *)(param_1 + 0x144) + 100;
  //iVar2 = FUN_00305468((u32)*(u16 *)(param_1 + 0x144),*(u16 *)(param_1 + 0x146));
  iVar2 = SHK_CALL_HOOK(FUN_00305468, (u32)*(u16 *)(param_1 + 0x144),*(u16 *)(param_1 + 0x146));
  sVar3 = 1;
  if (iVar2 != 0) {
    sVar3 = *(short *)(iVar2 + 2);
  }
  *param_4 = sVar3;
  /*if ((-1 < sVar3) && ((*(u64 *)(param_1 + 0x138) & 1) != 0)) {
                    /* WARNING: Subroutine does not return */
    /*FUN_002d03b0((int *)(param_1 + 0x19b0),ZEXT48(register0x0000000c) - 0x60);
  }*/
  return;
}

void ShujynxInit( void )
{
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  printf("Shujynx Module Enabled. CBT has begun.\n");
  SHK_BIND_HOOK( GetFieldIDsFromEncountTable, GetFieldIDsFromEncountTableHook);
}

void ShujynxShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif