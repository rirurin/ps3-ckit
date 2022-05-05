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
#include "rirurin.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FUNC_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( functest ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FILE_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( enableFileAccessLogging ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.

//SHK_HOOK( void, FUN_00475e10, float* a1, int a2, int a3 );
//SHK_HOOK( u64, FUN_0047de54, int a1, u64 a2, u64 a3 );
//SHK_HOOK( u64, FUN_00449c6c, int a1, u64 a2, u64 a3 );
/*
static void SetPartyTextPosition(float* a1, int a2, int a3)
{
  f32* baseAddress = 0xd9ef4c;
  u16* partyMembers = a3 + 0x32;
  u16 adjustedPartyMembers = (*partyMembers == 10) ? *partyMembers - 1 : *partyMembers;

  f32 xpos = a2 * -12.0 + *(baseAddress + (adjustedPartyMembers << 1));
  f32 ypos = a2 * 38.0 + *((baseAddress + 1) + (adjustedPartyMembers << 1));
  //printf("%x, %x\n", (baseAddress + (adjustedPartyMembers << 1)), ((baseAddress + 1) + (adjustedPartyMembers << 1)));
  a1[0] = xpos;
  a1[1] = ypos;
}

static u64 ScrollThroughEquipPartyList(int a1, u64 a2, u64 a3)
{
  
  u16* partyMembers = a1 + 0x46;
  u32* selectedPartyMember = a1 + 0x20;
  u16* pad_val = 0x1166b10;
  if (a2 == 0) // a2 specifies the type of action happening on the equip menu
  // 0 - switching between party members
  // 1 - switching between gear
  // 2 - exiting a submenu
  {
    if (*pad_val & 0x40 || *pad_val & 0x800)
    {
      if (*selectedPartyMember == *partyMembers - 1)
      {
        a3 = 0;
      } else 
      {
        a3 = *selectedPartyMember + 1;
      }
    } else if (*pad_val & 0x10 || *pad_val & 0x400)
    {
      if (*selectedPartyMember == 0)
      {
        a3 = *partyMembers - 1;
      } else 
      {
        a3 = *selectedPartyMember - 1;
      }
    }
  }
  printf("%x, %x, %x\n", a1, a2, a3);
  return SHK_CALL_HOOK( FUN_0047de54, a1, a2, a3 );
}

static u64 ScrollThroughSkillPartyList(int a1, u64 a2, u64 a3)
{
  printf("%x, %x, %x\n", a1, a2, a3);
  BuildPartyMemberItemsMenu(0);
  return SHK_CALL_HOOK( FUN_00449c6c, a1, a2, a3 );
}
*/
// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void rirurinInit( void )
{
  //SHK_BIND_HOOK( FUN_00475e10, SetPartyTextPosition );
  //SHK_BIND_HOOK( FUN_0047de54, ScrollThroughEquipPartyList );
  //SHK_BIND_HOOK( FUN_00449c6c, ScrollThroughSkillPartyList );
}

void rirurinShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif