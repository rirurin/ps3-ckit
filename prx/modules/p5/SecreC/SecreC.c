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
SHK_HOOK( void, FUN_0049eb90, int a1, u16 a2, undefined4 a3, char a4 );
SHK_HOOK( void, FUN_0049ee38, int a1, char a2, u32 a3, char a4 );
SHK_HOOK( int, FUN_003e8ff8, int a1 );
SHK_HOOK( void, FUN_005a4584, int a1, int a2 );

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

u64 RankupCardHook( CmmStruct *a1, double a2 ) //changes Kasumi card to Sumire's if the date is past 1/10 or later
 {
	if (GetTotalDays() >= 284 && a1->Field52->cardId == 21 && GetBitflagState( 2162 ) == 1)
	{
		a1->Field52->cardId = 23;
	}
	return SHK_CALL_HOOK( FUN_00244250, a1, a2 );
 }

undefined8 ConfRankupHook( undefined8 a1, u32 a2, int a3 ) //makes kasumi rankup trigger sumire rankup animation if the date is 1/10 or later
{
	if ( a2 == 33 && GetTotalDays() >= 284 && GetBitflagState( 2162 ) == 1)
	{
		a2 = 36;
	}
	return SHK_CALL_HOOK( FUN_0023c070, a1, a2, a3 );
}

void CharaTexHook( int a1, u16 a2, undefined4 a3, char a4 ) // makes Kasumi CharaTex in confidant menu show Sumire past 1/10
{
	if ( a2 == 10 && GetTotalDays() >= 284 && GetBitflagState( 2162 ) == 1)
	{
		a2 = 32;
	}
	return SHK_CALL_HOOK( FUN_0049eb90, a1, a2, a3, a4 );
}

void CommuCardHook( int a1, char a2, u32 a3, char a4 ) // makes Kasumi CardTex in confidant menu show Sumire's past 1/10
{
	if ( a2 == 22 && GetTotalDays() >= 284 && GetBitflagState( 2162 ) == 1)
	{
		a2 = 24;
	}
	return SHK_CALL_HOOK( FUN_0049ee38, a1, a2, a3, a4 );
}

int FUN_003e8ff8Hook( int a1 ) // increase offset to pointer by 4 bytes when fldMajor == 5
{
	int result = SHK_CALL_HOOK( FUN_003e8ff8, a1 );
	u32 *puVar1 = FUN_0031f35c();
    u32 fldMajor = (u32)*(u16 *)(puVar1[0xd] + 0x144);
	if (fldMajor == 5)
	{
		result = 6;
	}
	return result;
}

void FUN_005a4584Hook( int a1, int a2 )
{
	SHK_CALL_HOOK( FUN_005a4584, a1, a2 );
	int iVar4;
	short ShopId = *(short *)(a2 + 0xc2);
	u16 uVar2 = *(u16 *)(0xdf237c + (int)((s64)ShopId << 2)) & 0xff;
	switch (ShopId){

		case 77:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_POWERSTONE.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 81:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_STATIONERY.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 82:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_FRIEDFOOD.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 83:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_JIDOU.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 84:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_JIDOU.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 85:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_HURUGI.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 87:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_FOOD.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

		case 88:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_CHINESE.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;
	}
	
	return;
}
void SecreCInit( void )
{
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  
  SHK_BIND_HOOK( FUN_004eaca4, SelPortraitFromModelID);
  SHK_BIND_HOOK( FUN_005528f8, WipeParallelHook );
  SHK_BIND_HOOK( FUN_00244250, RankupCardHook );
  SHK_BIND_HOOK( FUN_0023c070, ConfRankupHook );
  SHK_BIND_HOOK( FUN_0049eb90, CharaTexHook );
  SHK_BIND_HOOK( FUN_0049ee38, CommuCardHook );
  SHK_BIND_HOOK( FUN_003e8ff8, FUN_003e8ff8Hook );
  SHK_BIND_HOOK( FUN_005a4584, FUN_005a4584Hook );
}

void SecreCShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif