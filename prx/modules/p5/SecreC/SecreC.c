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
SHK_HOOK( undefined8, FUN_004eaca4, int a1 );
SHK_HOOK( void, FUN_005528f8, ParallelWipe *a1 );
SHK_HOOK( u64, FUN_00244250, CmmStruct *a1, double a2 );
SHK_HOOK( undefined8, FUN_0023c070, undefined8 a1, u32 a2, int a3 );
SHK_HOOK( void, FUN_0049eb90, int a1, u16 a2, undefined4 a3, char a4 );
SHK_HOOK( void, FUN_0049ee38, int a1, char a2, u32 a3, char a4 );
SHK_HOOK( int, FUN_003e8ff8, int a1 );
SHK_HOOK( void, FUN_005a4584, int a1, int a2 );
SHK_HOOK( undefined8, FUN_004e392c, short a1 );
SHK_HOOK( u64, FUN_00548c8c, short a1 );
SHK_HOOK( u64, FUN_00397f4c, int a1 );
SHK_HOOK( void, FUN_0059bb98, int a1 );
SHK_HOOK( void, FUN_005a7098, int a1, int a2);
SHK_HOOK( void, FUN_00597740, int a1 );
SHK_HOOK( void, FUN_00260340, u32 a1, u32 a2 );
SHK_HOOK( u64, FUN_000503d0, int a1 );
SHK_HOOK( undefined8, FUN_005a6b78, a1, a2, a3 );
SHK_HOOK( void, FUN_005a5130, int a1 );

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.

undefined8 InverseItemPurchaseLimit( int a1 )
{
	if 	( a1 == ( 0x1000 + 14 )
		 || a1	== ( 0x1000 + 15 )
		 || a1	== ( 0x1000 + 48 )
		 || a1	== ( 0x1000 + 49 )
		 || a1	== ( 0x1000 + 68 )
		 || a1	== ( 0x1000 + 69 )
		 || a1	== ( 0x1000 + 88 )
		 || a1	== ( 0x1000 + 89 )
		 || a1	== ( 0x1000 + 108 )
		 || a1	== ( 0x1000 + 109 )
		 || a1	== ( 0x1000 + 0x80 )
		 || a1	== ( 0x1000 + 129 )
		 || a1	== ( 0x1000 + 148 )
		 || a1	== ( 0x1000 + 149 )
		 || a1	== ( 0x1000 + 168 )
		 || a1	== ( 0x1000 + 169 )
		 || a1	== ( 0x1000 + 188 )
		 || a1	== ( 0x1000 + 189 ) )
	{
		return 99 - GET_ITEM_NUM( a1 ); //reverses item purchase cap to be the amount of items you have
	}
	return GET_ITEM_NUM( a1 );	
}

void SubtractCurrencyForPurchase( int a1, int a2 )
{
	if (a1 == 72)
	{
		ActualSetCount( 90, a2 );
	}
	else if (a1 == 75 || a1 == 76)
	{
		ActualSetCount( 162, a2 );
	}
	else if (a1 == 85)
	{
		ActualSetCount( 8, a2 );
	}
	else
	{
		SetMoney(a2);
	}
	return;
}

int ReturnSpendableCurrency( int a1 )
{
	if (a1 == 72)
	{
		return ActualGetCount( 90 );
	}
	else if (a1 == 75 || a1 == 76)
	{
		return ActualGetCount( 162 );
	}
	else if (a1 == 85)
	{
		return ActualGetCount( 8 );
	}
	else
	{
		return GetMoneyCount();
	}
}

undefined8 GetLmapBitFlag( int a1 )
{
	if( a1 == 2704 ) // Kasumi
	{
		if ( GetBitflagState (3583) == 1 ) return 1;
		return GetBitflagState( 2694 );
	}
	else if( a1 == 2706 ) //Maruki
	{
		if ( GetBitflagState (3583) == 1 ) return 1;
		return GetBitflagState( 2695 );
	}
	else if( a1 == 2680 ) //Akechi
	{
		if ( GetBitflagState (3583) == 1 ) return 1;
		return GetBitflagState( 2680 );
	}
	else
	{
		return GetBitflagState( a1 );
	}
}

int SetSpritetoYenorPoints( int a1 )
{
	if (a1 == 72 || a1 == 70 || a1 == 85)
	{
		return 75;
	}
	else
	{
		return a1;
	}
}

undefined8 SelPortraitFromModelID(SelPrompt *a1)
{
	//pseudo function replacing the ones at 0x5a6c84 and 0x5a526c
	if (a1 < 0x9000)
	{
		return InverseItemPurchaseLimit( a1 );
	}	
	//end pseudo function
	undefined8 result = SHK_CALL_HOOK( FUN_004eaca4, a1 );
	if ( GetBitflagState( 2815 ) == 1 && GetTotalDays() == 231 ) //If In-game day is 11/18 and flag 2815 is enabled
	{
		a1->PortraitID = 5; //cutin_005_000.dds
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
		//a1->Field52->cardId = 23;
	}
	return SHK_CALL_HOOK( FUN_00244250, a1, a2 );
 }

undefined8 ConfRankupHook( undefined8 a1, u32 a2, int a3 ) //makes kasumi rankup trigger sumire rankup animation if the date is 1/10 or later
{
	if ( a2 == 33 && GetTotalDays() >= 284 && GetBitflagState( 2162 ) == 1)
	{
		//a2 = 36;
	}
	return SHK_CALL_HOOK( FUN_0023c070, a1, a2, a3 );
}

void CharaTexHook( int a1, u16 a2, undefined4 a3, char a4 ) // makes Kasumi CharaTex in confidant menu show Sumire past 1/10
{
	if ( a2 == 10 && GetTotalDays() >= 284 && GetBitflagState( 2162 ) == 1)
	{
		//a2 = 32;
	}
	return SHK_CALL_HOOK( FUN_0049eb90, a1, a2, a3, a4 );
}

void CommuCardHook( int a1, char a2, u32 a3, char a4 ) // makes Kasumi CardTex in confidant menu show Sumire's past 1/10
{
	if ( a2 == 22 && GetTotalDays() >= 284 && GetBitflagState( 2162 ) == 1)
	{
		//a2 = 24;
	}
	return SHK_CALL_HOOK( FUN_0049ee38, a1, a2, a3, a4 );
}

int FUN_003e8ff8Hook( int a1 ) // increase offset to pointer by 4 bytes when fldMajor == 5
{
	//pseudo function for Getting Money Values from counts at 0x5a8f4c
	if (a1 > -1 && a1 < 90)
	{
		return ReturnSpendableCurrency( a1 );
	}
	//end pseudo function
	int result = SHK_CALL_HOOK( FUN_003e8ff8, a1 );
	if (result != -1)
	{
		u32 *puVar1 = FUN_0031f35c();
    	u32 fldMajor = (u32)*(u16 *)(puVar1[0xd] + 0x144);
		if (fldMajor == 5)
		{
			result = 6;
		}
	}
	return result;
}

void CallShopBannerHook( int a1, ShopStruct *a2 )
{
	//pseudo function for Getting Money/Point Values at 0x5a6fd8
	if (a1 > -1 && a1 < 90)
	{		
		SubtractCurrencyForPurchase( a1, a2 );
		return;
	}
	//end psuedo function
	SHK_CALL_HOOK( FUN_005a4584, a1, a2 );
	int iVar4;
	switch (a2->ShopID){

		case 70:
		iVar4 = FUN_00967abc("facility/fcl_ps_title/h_name_HURUGI.dds", 1);
		*(int *)(a1 + 0x924) = iVar4;
		break;

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

undefined8 BuildGroupChatIconListHook( short a1 )
{	
	//pseudo function replacing GetBitflagState at 0xaf81c for Lmap Confidant Indicators
	if ( a1 >= 2671 ) 
	{
		return GetLmapBitFlag( a1 );
	}
	//pseudo function end
	
	undefined8 result = SHK_CALL_HOOK( FUN_004e392c, a1 );
	if (a1 == 10 && GetBitflagState(1175) == 1 ) // Check for flag 1175 before adding Kasumi to the PT group chat
	{
		result = 1; //add Kasumi to the Phantom Thieves Group chat
	}
	return result;
}

u64 BuildConfidantListHook( short a1 )
{
	u8 confCount = GetActiveConfidantAmount(35); //returns how many confidants are active
	printf("%d Active Confidants\n", confCount);
	u64 result = SHK_CALL_HOOK( FUN_00548c8c, a1 );
	u16* pad_val = 0x1166b10;
	if (((*pad_val) & 0x200) && ( a1 == 21 && confCount == 23 )) //Before Adding Sae to the Confidant list Check if all Confidants are active, and if R2 is being held down
	{
		return 0; //remove Sae from the Confidant list
	}
	else if ((((*pad_val) & 0x200) == 0) && a1 == 24 && confCount == 23 ) //Before Adding Maruki to the Confidant list Check if all Confidants are active, and if R2 is not being held down
	{
		return 0; //remove Maruki from the confidant list
	}
	return result;
}

u64 LmapIdtoPointerList( int a1 )
{

	int result = SHK_CALL_HOOK( FUN_00397f4c, a1 );
	if ( a1 == 7 ) //Lmap Id 7 = Kichijoji
	{
		return 7; //pointer to field Id list at 0xd010b4 + 7 * 4
	}
	return result; // :hee_shake:
}

u64 FUN_000503d0Hook( int a1 )
{
	//pseudo function for 0x5a2d68
	if (a1 < 100)
	{
		return SetSpritetoYenorPoints(a1);
	}
	//end pseudo function
	u64 result = SHK_CALL_HOOK( FUN_000503d0, a1 );
	if (CONFIG_ENABLED(announceWeekday))
	{
		if (*(undefined4 *)(a1 + 0x3314) == 2) ActualSetCount( 321, 0 );
		if (*(undefined4 *)(a1 + 0x3314) == 3 && ActualGetCount(321) == 0)
		{
			if ( GetBitflagState( 4182 ) == 1 && ( GetTotalDays() < 233 || GetTotalDays() > 237 ))
			{
				PlayFromSinglewordACB( 2, 51 );
			}
			else
			{
				int Variation = randomIntBetween( 0, 1 ); //Randomize between two voice line variations
				int DayOfWeek = GetTotalDays() % 7;
				switch(DayOfWeek){
				
				case 0: //Friday
				DayOfWeek = 4;
				break;
				
				case 1: //Saturday
				DayOfWeek = 5;
				break;

				case 2: //Sunday
				DayOfWeek = 6;
				break;

				case 3: //Monday
				DayOfWeek = 0;
				break;

				case 4: //Tuesday
				DayOfWeek = 1;
				break;

				case 5: //Wednesday
				DayOfWeek = 2;
				break;

				case 6: //Thursday
				DayOfWeek = 3;
				break;
				
				}
				int DayCue = 120 + Variation + (DayOfWeek * 2);
				PlayFromSinglewordACB( 2, DayCue );
			}
			ActualSetCount( 321, 1 ); //Prevents the voice clip from playing 84759207594560879 times
		}
	}
	return result;
}

void ShopBannerColorHook( ShopStruct *a1 )
{
	if ( CONFIG_ENABLED( enableRandomShopBannerColors ) ) //DC Copypasta :hee_cat:
  	{
      // randomize color
	  FrameTiming = randomIntBetween( 0, 360 * 4 );
      FrameTiming += 1;
      hsv c;
      //c.h = ((P5_SIN((FrameTiming/3.0)) + 1.0f) / 2.0f) * 360;
      c.h = ( FrameTiming % ( 360 * 4 ) ) / 4;
      c.s = 1.0;
      c.v = 1.0;

      rgb cc = hsv2rgb(c);
      cc.r *= 255; 
      cc.g *= 255; 
      cc.b *= 255;
	  a1->BannerColor = ((int)cc.r << 24) | ((int)cc.g << 16) | ((int)cc.b << 8) | 0xFF;;
    }

	if (a1->ShopID == 70)
	{
		a1->BannerColor = 0x20dff8ff;
	}
	SHK_CALL_HOOK( FUN_0059bb98, a1 );
}

void FUN_005a7098Hook( ShopStruct *a1, int a2 )
{
	if (a1->ShopID == 70)
	{
		a2 *= -1; //reverses the cost of items in shop 70
	}
	SHK_CALL_HOOK( FUN_005a7098, a1, a2 );
}

void FUN_00597740Hook( ShopStruct *a1 )
{
	if (a1->ShopID == 70)
	{
		a1->PurchasePrice *= -1; //reverses the cost of items in shop 70
	}
	SHK_CALL_HOOK( FUN_00597740, a1 );
}

void SET_ITEM_NUM_Hook( u32 a1, u32 a2 )
{
	if (a1 > 0xa000) //runs when a1 is a pointer, making it outside the normal item range
	{
		char itemPosition = *(char *)(a1 + 0xf); //commence ghidra copypasta
    	s64 lVar9 = (s64)
            *(short *)(a1 + ((int)itemPosition + (int)*(short *)(a1 + 0x7f8)) * 2 + 0x84);
    	int iVar11 = a1 + (int)(lVar9 << 5) + (int)(lVar9 << 3);
		u16 purchaseQuantity = *(u16 *)(a1 + 0x18);
      	u16 itemID = *(u16 *)(iVar11 + 0x322);
      	s8 itemsPerUnit = *(s8 *)(iVar11 + 0x324);
		u16 itemPCInventory = GET_ITEM_NUM((u32)itemID);
		if (*(short *)(a1 + 0xc2) == 70)
		{
        	SET_ITEM_NUM((u32)itemID,itemPCInventory - ((u32)itemsPerUnit * (u32)purchaseQuantity & 0xff)); //reverses the amount of items you acquire when purchasing an item from shop 70
		}
		else
		{
			SET_ITEM_NUM((u32)itemID,itemPCInventory + ((u32)itemsPerUnit * (u32)purchaseQuantity & 0xff));
		}
		return;
	}
	SHK_CALL_HOOK( FUN_00260340, a1, a2 );
}

undefined8 FUN_005a6b78Hook( ShopStruct *a1, int a2, int a3 )
{
	if (a1->ShopID == 70)
	{
		a1->SpendableMoney = 9999999; //Make the game think your money is maxed out when calculating how many items you can buy
	}
	SHK_CALL_HOOK( FUN_005a6b78, a1, a2, a3 );

}

void FUN_005a5130Hook( ShopStruct *a1 )
{
	if (a1->ShopID == 70)
	{
		a1->SpendableMoney = 9999999; //Make the game think your money is maxed out when calculating how many items you can buy
	}
	SHK_CALL_HOOK( FUN_005a5130, a1 );
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
  SHK_BIND_HOOK( FUN_005a4584, CallShopBannerHook );
  SHK_BIND_HOOK( FUN_004e392c, BuildGroupChatIconListHook );
  SHK_BIND_HOOK( FUN_00548c8c, BuildConfidantListHook );
  SHK_BIND_HOOK( FUN_00397f4c, LmapIdtoPointerList );
  SHK_BIND_HOOK( FUN_0059bb98, ShopBannerColorHook );
  SHK_BIND_HOOK( FUN_005a7098, FUN_005a7098Hook );
  SHK_BIND_HOOK( FUN_00597740, FUN_00597740Hook );
  SHK_BIND_HOOK( FUN_00260340, SET_ITEM_NUM_Hook );
  SHK_BIND_HOOK( FUN_000503d0, FUN_000503d0Hook );
  SHK_BIND_HOOK( FUN_005a6b78, FUN_005a6b78Hook );
  SHK_BIND_HOOK( FUN_005a5130, FUN_005a5130Hook );
}

void SecreCShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif