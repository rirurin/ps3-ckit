#ifdef GAME_P5
// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>
#include "lib/file.h"
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
#include "kasumi.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FUNC_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( functest ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( bool, FUN_00425b0c, u16 unitID );
SHK_HOOK( void, FUN_0025b888, btlUnit_Unit* unitID );
SHK_HOOK( void, FUN_0025b7b8, btlUnit_Unit* unitID, int bullets );
SHK_HOOK( int, FUN_0025b740, btlUnit_Unit* unitID );
SHK_HOOK( void, FUN_0024bb54, void );
SHK_HOOK( void, FUN_00661220, u64 a1, u64 a2, u64 a3 );
SHK_HOOK( void, FUN_00b20618, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1c1a0, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1c398, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1c610, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b20ec8, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1ca18, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b21980, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b21bf8, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b21e28, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b22138, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b22f60, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1bf38, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( int, FUN_00b1bd50, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2 );
SHK_HOOK( void, FUN_00b18368, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b190a8, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b192e8, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( int, FUN_00425de0, partyMemberMenu* a1 );
SHK_HOOK( int, FUN_00425c80, partyMemberMenu* a1 );
SHK_HOOK( int, FUN_00425d20, partyMemberMenu* a1 );
SHK_HOOK( int, FUN_0034e128, int a1 );
SHK_HOOK( int, FUN_0060dd98, int a1, char* a2, int a3, char* a4 );
SHK_HOOK( void, FUN_00475e10, float* a1, int a2, int a3 );
SHK_HOOK( u64, FUN_0047de54, int a1, u64 a2, u64 a3 );
SHK_HOOK( u64, FUN_00449c6c, int a1, u64 a2, u64 a3 );
SHK_HOOK( u64, FUN_0047f91c, int a1, u64 a2, u64 a3 );
SHK_HOOK( void, FUN_00480cc0, u64* a1 );
SHK_HOOK( void, FUN_0044a5d8, u64 a1 );
SHK_HOOK( u64, FUN_0044f7ac, u32* a1, s16 a2, u64 a3, int a4 );
SHK_HOOK( u64, FUN_0044f8e0, short a1, short a2, int a3, int a4 );
SHK_HOOK( u64, FUN_00487c58, short a1, short a2, short* a3 );

static bool isPartyMemberUnlocked( u16 unitID )
{
  FUNC_LOG("Loading isPartyMemberUnlocked\n");
  if ( unitID == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    return GetSavedataBitflagAlt( 0x2138 );
  }
  else return SHK_CALL_HOOK( FUN_00425b0c, unitID );
}

static int BuildPartyMemberStatsMenu ( partyMemberMenu* partyMenu )
{
  int count = 1;
  
  partyMenu->partyMemberID[0] = 1;
  for ( int i = 2; i <= 10; i++)
  {
    if (isPartyMemberUnlocked(i))
    //if (true)
    {
      partyMenu->partyMemberID[count] = i;
      //DEBUG_LOG("Party Member %d added to the menu\n", i);
      count += 1;
    }
  }
  return count;
}

static int BuildPartyMemberItemsMenu ( partyMemberMenu* partyMenu )
{
  u16* pad_val = 0x1166b10;
  partyMemberListAddress = partyMenu;
  scrollMax = 0;
  for (int i = 1; i <= 10; i++)
  {
    partyMembers[i-1] = false;
    //if (true && i != 8)
    if (isPartyMemberUnlocked(i) && i != 8)
    {
      partyMembers[i-1] = true;
      //printf("player %d is available\n", i);
      scrollMax++;
    }
  }
  if (scrollMax < 7) scrollMax = 0;
  else scrollMax = scrollMax - 7;
  int count = 0;
  int index = partyListOffset;
  for (int i = partyListOffset; i < 10; i++) {
    //printf("%d\n", i);
    if (partyMembers[i]) {
      //printf("adding player %d to slot %d\n", i + 1, count);
      partyMenu->partyMemberID[count] = i + 1;
      count++;
    }
    if (count == 7) {
      break;
    }
  }
  return count;
}

static int BuildPartyMemberEquipMenu ( partyMemberMenu* partyMenu )
{
  partyMenu->partyMemberID[9] = 10; // this fixes it for *some* reason

  int count = 1;
  
  partyMenu->partyMemberID[0] = 1;
  
  for ( int i = 2; i <= 10; i++)
  {
    if (isPartyMemberUnlocked(i))
    //if ( i != 9 )
    {
      partyMenu->partyMemberID[count] = i;
      DEBUG_LOG("Party Member %d added to the menu\n", i);
      count += 1;
    }
  }
  return count;
}

// Fixes Party Text position in equip menu with 10 members

static void SetPartyTextPosition(float* a1, int a2, int a3)
{
  f32* baseAddress = 0xd9ef4c;
  u16* partyMembers = a3 + 0x30;
  u16 adjustedPartyMembers = (*partyMembers == 10) ? *partyMembers - 1 : *partyMembers;

  f32 xpos = a2 * -12.0 + *(baseAddress + (adjustedPartyMembers << 1));
  f32 ypos = a2 * 38.0 + *((baseAddress + 1) + (adjustedPartyMembers << 1));
  
  a1[0] = xpos;
  a1[1] = ypos;
}

static u64 ScrollThroughEquipPartyList(int a1, u64 a2, u64 a3)
{
  
  u16* partyMembers = a1 + 0x44;
  u32* selectedPartyMember = a1 + 0x20;
  u16* pad_val = 0x1166b10;
  printf("%d, %d\n", *partyMembers, *selectedPartyMember + 1);
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
  //printf("%x, %x, %x\n", a1, a2, a3);
  return SHK_CALL_HOOK( FUN_0047de54, a1, a2, a3 );
}

static u64 ScrollThroughSkillPartyList(int a1, u64 a2, u64 a3)
{
  u16* pad_val = 0x1166b10;
  
  if (scrollMax > 0 && (a2 == 0 || a2 == 3)) {
   if (*pad_val & 0x10) { // going up
      if (a3 == 0 && partyListOffset > 0) { // can keep scrolling up
        a3 = 1;
        partyListOffset--;
      }
      if (a3 == 6 && partyListOffset == 0) { // top of list, go to bottom
        partyListOffset = scrollMax;
      }
   }
   else if (*pad_val & 0x40) { // going down
      if (a3 == 6 && partyListOffset < scrollMax) { // can keep scrolling down
        a3 = 5;
        partyListOffset++;
      }
      if (a3 == 0 && partyListOffset == scrollMax) { // bottom of list, go to top
        partyListOffset = 0;
      }
   }
    BuildPartyMemberItemsMenu(partyMemberListAddress);
  }
  int skillActivePartyMember = a3 + partyListOffset;
  /*
  int skillActivePartyMember = 0;
  for (int i = 0; i < a3 + partyListOffset;) {
    if (partyMembers[skillActivePartyMember]) i++;
    skillActivePartyMember++;
  }
  */
  if (a2 == 0 || a2 == 3) {
    //printf("%x, %x, %x\n", a3, partyListOffset, scrollMax);
    /*
    printf("%d, %d\n", GetBtlPlayerUnitFromID(skillActivePartyMember)->currentHP
    , GetBtlPlayerUnitFromID(skillActivePartyMember)->currentSP);
    */
   /*
   printf("Player %d: %d, %d\n", skillActivePartyMember + 1, GetBtlPlayerUnitFromID(skillActivePartyMember + 1)->currentHP
    , GetBtlPlayerUnitFromID(skillActivePartyMember + 1)->currentSP);
    */
  }

  return SHK_CALL_HOOK( FUN_00449c6c, a1, a2, a3 );
}

static u64 SkillMenuHealingSkillInit( u32* a1, s16 a2, u64 a3, int a4 ) {
  u64 uVar1;
  u64 uVar2;
  int iVar3;
  //printf("%x, %d, %d, %d\n", a1, a2, a3, a4);
  // this is most likely a struct of some kind but idc lol
  iVar3 = (int)*(short*)(a4 + 0x26) + (int)*(short*)(a4 + 0x24);
  printf("FUN_0044f6a8: %d, %d, %d\n", a2, iVar3, a4);
  skillMenuPartyMemberSource = a2;
  // a2 is the character ID
  // iVar3 determines if the skill affects all players
  // a4 points towards the address 0x10de178
  uVar1 = FUN_0044f6a8(a2,iVar3,a4);
  if ((int)uVar1 == 0) { // if can't use healing item
    PlayFromSystemACB(4);
    printf("Cannot use healing item\n");
    uVar2 = 0;
  }
  else { // if can use healing item
    iVar3 = FUN_0042f5c4((u32)*(u16*)(a4 + iVar3 * 0xc + 0x1ea));
    printf("Skill selected: %d\n", (u32)*(u16*)(a4 + iVar3 * 0xc + 0x1ea));
    if (iVar3 == 0) {
      ScrollThroughSkillPartyList(a4, 3, 0);
      FUN_004495c0(a4,5);
      printf("Healing item for one player\n");
      if (a1 != (u32*)0x0) {
        *a1 = 7;
      }
    }
    else if ((iVar3 == 2) || (iVar3 == 1)) {
      ScrollThroughSkillPartyList(a4, 3, -1);
      FUN_004495c0(a4,7);
      printf("Healing item for all players\n");
      if (a1 != (u32*)0x0) {
        *a1 = 9;
      }
    }
    PlayFromSystemACB(1);
    uVar2 = 1;
  }
  return uVar2;
}

static u64 SkillMenuUseHealingSkill(short srcPlayer,short tgtPlayer,int param_3,int param_4)

{
  short sVar1;
  u16 skillID;
  short sVar3;
  bool bVar4;
  short sVar9;
  int iVar7;
  int iVar8;
  u64 uVar5;
  u64 uVar6;
  int iVar10;
  u32 uVar11;
  int iVar13;
  bool bVar14;
  u64 uVar15;
  u32 uVar16;
  
  bVar14 = false;
  uVar15 = 1;
  iVar10 = param_4 + param_3 * 0xc;
  sVar9 = -1;
  sVar1 = *(short *)(iVar10 + 0x1e8);
  uVar11 = 0;
  iVar8 = *(int *)(iVar10 + 0x1ec);
  skillID = *(u16 *)(iVar10 + 0x1ea);
  uVar16 = (int)srcPlayer & 0xffff;
  iVar10 = *(int *)(iVar10 + 0x1f0);
  printf("%d, %d, %d, %d\n", srcPlayer, tgtPlayer, param_3, param_4);
  srcPlayer = skillMenuPartyMemberSource;
  printf("Adjusted source player: %d\n", srcPlayer);
  if (sVar1 != -1) {
    //sVar9 = FUN_0025dcfc(uVar16);
    sVar9 = GetBtlPlayerUnitFromID(uVar16)->EquippedPersonaIndex;
    FUN_0025d624(uVar16,sVar1);
  }
  iVar7 = FUN_0042f344((int)srcPlayer,(u32)skillID);
  if (iVar7 != 1) {
    uVar15 = 0;
    goto LAB_0044fcec;
  }
  if (skillID == 0x187) { // Traesto
    uVar15 = 0;
    iVar7 = FUN_004260c4();
    if (iVar7 != 0) {
      uVar15 = 3;
    }
    if ((int)uVar15 == 0) goto LAB_0044fcec;
  }
  else {
    if (tgtPlayer == -1) {
      // heal all players
      iVar13 = 0;
      iVar7 = param_4;
      for (int i = 0; i < 10; i++) {
        if (partyMembers[i]) {
          printf("Healing player %d using skill %d\n", i + 1, (u32)skillID);
          uVar6 = FUN_0042f5ec(srcPlayer, i + 1, (u32)skillID, 0); 
          if (uVar6 == 0) {
              FUN_0042f3ec((int)srcPlayer, i + 1, (u32)skillID, 0);
              iVar13++;
          }
        }
      }
      printf("Characters that can heal: %d\n", iVar13);
      if (iVar13 != 0) {
        // now that we've healed some players, take some health away from the source player
        uVar5 = FUN_0042f994((u32)skillID);
        uVar11 = (u32)uVar5;
        goto LAB_0044fb20;
      }
      uVar15 = 0;
      goto LAB_0044fcec;
    } else {
      // heal one player
      uVar6 = FUN_0042f5ec((int)srcPlayer,(int)tgtPlayer,(u32)skillID,0);
      printf("Health left to heal: %d\n", uVar6);
      if ((int)uVar6 != 0) {
        uVar15 = 0;
        goto LAB_0044fcec;
      }
      FUN_0042f3ec((int)srcPlayer,(int)tgtPlayer,(u32)skillID,0);
      uVar5 = FUN_0042f994((u32)skillID);
      uVar11 = (u32)uVar5;
    }
  }
LAB_0044fb20:
  sVar1 = (short)iVar8;
  printf("iVar8: %d\n", iVar8);
  if (iVar8 > 0) {
    iVar8 = ActualGetCount(0xfe);
    // remove HP from character
    if (iVar8 < 1) {
      btlUnit_Unit* pUnit = GetBtlPlayerUnitFromID(srcPlayer); // get player
      int health = uVar6 - sVar1;
      printf("health taken away: %d\n", health);
      if (health > 999 && pUnit->unitID != 2) health = 999;
      pUnit->currentHP += health; // change health
    }
    else {
      iVar8 = ActualGetCount(0xfe);
      ActualSetCount(0xfe,iVar8 + -1);
      bVar14 = true;
    }
  }
  sVar3 = (short)iVar10;
  printf("SP change: %d\n", iVar10);
  bVar4 = false;
  if (iVar10 > 0) {
    // remove SP from character
    iVar8 = ActualGetCount(0xff);
    if (iVar8 < 1) {
      btlUnit_Unit* pUnit = GetBtlPlayerUnitFromID(srcPlayer); // get current player
      int sp = uVar6 - sVar3;
      printf("SP change of %d to player %d, %d SP remaining\n", sp, srcPlayer, pUnit->currentSP + sp);
      if (sp > 999 && pUnit->unitID != 2) sp = 999;
      pUnit->currentSP += sp; // change sp

      bVar4 = false;
    }
    else {
      iVar8 = ActualGetCount(0xff);
      ActualSetCount(0xff,iVar8 + -1);
      bVar4 = true;
    }
  }
  if ((int)uVar15 == 3) {
      if (bVar14) {
        iVar8 = ActualGetCount(0xfe);
        ActualSetCount(0xfe,iVar8 + 1);
      }
      // increase health
      else {
        btlUnit_Unit* pUnit = GetBtlPlayerUnitFromID(srcPlayer); // get player
        int health = uVar6 + sVar1;
        printf("health added: %d\n", health);
        if (health > 999 && pUnit->unitID != 2) health = 999;
        pUnit->currentHP += health; // change health
        
      }
      if (bVar4) {
        iVar8 = ActualGetCount(0xff);
        ActualSetCount(0xff,iVar8 + 1);
      }
      // increase SP
      else {
        btlUnit_Unit* pUnit = GetBtlPlayerUnitFromID(srcPlayer); // get current player
        int sp = uVar6 + sVar3;
        printf("sp added: %d\n", sp);
        if (sp > 999 && pUnit->unitID != 2) sp = 999;
        pUnit->currentSP += sp; // change SP
      }
      PlayFromSystemACB(7);
    FUN_00116ad0(0x2213);
  }
  else {
    uVar6 = FUN_0044f6a8(srcPlayer,param_3,param_4);
    if ((int)uVar6 == 0) {
      uVar15 = 2;
    }
    else {
      FUN_0044f448(param_4,uVar11);
    }
  }
LAB_0044fcec:
  if (sVar9 != -1) {
    FUN_0025d624(uVar16,sVar9);
  }
  uVar16 = (u32)uVar15;
  if (uVar16 == 0) {
    PlayFromSystemACB(4);
  } else if (uVar16 < 4) {
    PlayFromSystemACB(7);
  }
  return uVar15;
}

static u64 ScrollThroughItemPartyList(int a1, u64 a2, u64 a3)
{
  u16* pad_val = 0x1166b10;
  //printf("%x, %x, %x\n", a1, a2, a3);
  if (a2 == 3 && scrollMax > 0) { // a2 = 3 to check up/down in party list
    
    if ((a3 == 0) && *pad_val & 0x10) // if going to the top of the list and pressing up
    {
      if (partyListOffset > 0)
      {
        a3 = 1;
        partyListOffset--;
      }
    }
    if (a3 == 6 && *pad_val & 0x10) // if going to the bottom of the list and pressing up
    {
      partyListOffset = scrollMax;
    }
    if (a3 == 6 && *pad_val & 0x40) // if going to the bottom of the list and pressing down
    {
      if (partyListOffset < scrollMax)
      {
        a3 = 5;
        partyListOffset++;
      }
    }
    if (a3 == 0 && *pad_val & 0x40) // if going to the top of the list and pressing up
    {
      partyListOffset = 0;
    }
    BuildPartyMemberItemsMenu(partyMemberListAddress);
  }
  return SHK_CALL_HOOK( FUN_0047f91c, a1, a2, a3 );
}

static void ItemMenuInit(u64 a1)
{
  partyListOffset = 0;
  //printf("Reset party list offset\n");
  return SHK_CALL_HOOK(FUN_00480cc0, a1);
}

static void SkillMenuInit(u64 a1)
{
  partyListOffset = 0;
  ///printf("Reset party list offset\n");
  return SHK_CALL_HOOK(FUN_0044a5d8, a1);
}

static int JokerDied_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2 )
{
  FUNC_LOG("Loading JokerDied_NaviDialogue\n");
  int result = SHK_CALL_HOOK( FUN_00b1bd50, param_1, param_2 );
  if ( param_2 != 0 && param_2->EncounterID >= 792 )
  {
    int messageIndex = 0x3b;
    int emotionID = 1;
    switch ( param_2->EncounterID )
    {
      case 831:
      case 832:
      case 833:
      case 834:
      case 835:
      case 898:
        if ( GetBitflagState( 8603 ) ) 
        {
          CALL_NAVI_DIALOGUE( 50, 1, 914, 1 );
        }
        else
        {
          if ( ActualGetCount( 511 ) < 50 )
          {
            CALL_NAVI_DIALOGUE( 51, 1, 912, 1 );
          }
          else 
          {
            CALL_NAVI_DIALOGUE( 52, 1, 913, 1 );
          }
        }
        result = 1;
        break;
      default:
        if ( GetBitflagState( 0x21c3 ) ) 
        {
          messageIndex = randomIntBetween( 0, 2 );
          messageIndex = messageIndex % 3 + 0x31;
        }
        if ( 0x31 >= messageIndex )
        {
          messageIndex = 0x31;
        }
        if ((0x30 < messageIndex) && (messageIndex < 0x33)) 
        {
          emotionID = 4;
        }
        if (messageIndex == 0xffffffff) 
        {
          return result;
        }
        CallNaviDialogue( param_1, messageIndex, emotionID, 0, -1, 0x00b1bd48, 1, 2, 0.0 );
        result = 1;
      break;
    }
    return result;
  }
  return result;
}

static void PartyMemberFinishedLastEnemy_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{

  FUNC_LOG("Loading PartyMemberFinishedLastEnemy_NaviDialogue\n");
  if ( GetBtlPlayerUnitFromID( 1 )->currentHP == 0 )
  {
    return;
  }
  undefined8 uVar1;
  u8 bVar4;
  u64 uVar2;
  int iVar3;
  int messageIndex;
  
  uVar1 = FUN_0074805c((u64)(int)param_1,(int)param_2);
  if (((((int)uVar1 == 0) && ((param_2->field118 >> 0x16 & 1) != 0)) && (param_4 != 0)) &&
     ((param_3 == (struct_2_pointers *)0x0 || (bVar4 = FUN_002588b4(param_3->pointer_2), bVar4 == 0)
      ))) {
    uVar2 = FUN_007489a8((int)param_1,(int)param_2);
    if ((int)uVar2 == 0) {
      uVar2 = param_3->pointer_2->unitID;
      if ((uVar2) < 5) {
        if ((uVar2) < 2) {
          if ((int)uVar2 == 0) {
            return;
          }
          iVar3 = GetBitflagState(0x21c3);
          messageIndex = 299;
          if (iVar3 != 0) {
            messageIndex = 0x123;
          }
        }
        else {
          if ((uVar2) < 3) {
            iVar3 = GetBitflagState(0x21c4);
            messageIndex = 299;
            if (iVar3 != 0) {
              messageIndex = 0x124;
            }
          }
          else {
            if ((uVar2) < 4) {
              iVar3 = GetBitflagState(0x21c5);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x125;
              }
            }
            else {
              iVar3 = GetBitflagState(0x21c6);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x126;
              }
            }
          }
        }
      }
      else {
        if ((uVar2) < 8) {
          if ((uVar2) < 6) {
            iVar3 = GetBitflagState(0x21c7);
            messageIndex = 299;
            if (iVar3 != 0) {
              messageIndex = 0x127;
            }
          }
          else {
            if ((uVar2) < 7) {
              iVar3 = GetBitflagState(0x21c8);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x128;
              }
            }
            else {
              iVar3 = GetBitflagState(0x21c9);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x129;
              }
            }
          }
        }
        else {
          if ( uVar2 == 10 )
          {
            messageIndex = 894;
          }
          else if ( uVar2 == 9 )
          {
            iVar3 = GetBitflagState(0x21cb);
            messageIndex = 299;
            if (iVar3 != 0) 
            {
              messageIndex = 0x12a;
            }
          }
          else
          {
            return;
          }
        }
      }
      if (((messageIndex != -1) &&
          (uVar2 = FUN_0091da04(), (uVar2 + ((uVar2 & 0xffffffff) / 100) * -100 & 0xffffffff) < 0x32
          )) && (uVar2 = FUN_748ff0((int)param_1), (int)uVar2 == -1)) {
        CallNaviDialogue(param_1,messageIndex,1,0,-1,param_6,'\0',1,0.0);
      }
    }
    else {
      if (param_4 < 3) {
        if (param_4 == 0) {
          return;
        }
        if (param_4 < 2) {
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x14f;
              if (iVar3 != 0) {
                messageIndex = 0x147;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x14f;
                if (iVar3 != 0) {
                  messageIndex = 0x148;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x149;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x14a;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x14f;
                if (iVar3 != 0) {
                  messageIndex = 0x14b;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x14c;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x14d;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 && CONFIG_ENABLED( enableKasumi ) )
              {
                messageIndex = 898;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x14f;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x14e;
                }
              }
              else
              {
                return;
              }
            }
          }
        }
        else {
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x146;
              if (iVar3 != 0) {
                messageIndex = 0x13e;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x146;
                if (iVar3 != 0) {
                  messageIndex = 0x13f;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x140;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x141;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x146;
                if (iVar3 != 0) {
                  messageIndex = 0x142;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x143;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x144;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 )
              {
                messageIndex = 897;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x146;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x145;
                }
              }
              else
              {
                return;
              }
            }
          }
        }
      }
      else {
        if (param_4 < 4) {
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x13d;
              if (iVar3 != 0) {
                messageIndex = 0x135;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x13d;
                if (iVar3 != 0) {
                  messageIndex = 0x136;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x137;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x138;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x13d;
                if (iVar3 != 0) {
                  messageIndex = 0x139;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x13a;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x13b;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 )
              {
                messageIndex = 896;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x13d;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x13c;
                }
              }
            }
          }
        }
        else {
          if (4 < param_4) {
            return;
          }
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x134;
              if (iVar3 != 0) {
                messageIndex = 300;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x134;
                if (iVar3 != 0) {
                  messageIndex = 0x12d;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x12e;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x12f;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x134;
                if (iVar3 != 0) {
                  messageIndex = 0x130;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x131;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x132;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 )
              {
                messageIndex = 895;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x134;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x133;
                }
              }
              else
              {
                return;
              }
            }
          }
        }
      }
      if ((messageIndex != -1) && (uVar2 = FUN_748ff0((int)param_1), (int)uVar2 == -1)) {
        CallNaviDialogue( param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
      }
    }
  }
  return;
}

static void PartyMemberWarnHealthLow_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{

  FUNC_LOG("Loading PartyMemberWarnHealthLow_NaviDialogue\n");
  if ( GetBtlPlayerUnitFromID( 1 )->currentHP == 0 )
  {
    return;
  }
  u64 uVar1;
  int iVar2;
  int messageIndex;
  
  if ((param_2->field118 >> 0x16 & 1) != 0) {
    if (param_4 == 1) {
      uVar1 = param_3->pointer_2->unitID;
      if ((uVar1 & 0xffffffff) < 5) {
        if ((uVar1 & 0xffffffff) < 2) {
          if ((int)uVar1 == 0) {
            return;
          }
          iVar2 = GetBitflagState(0x21c3);
          messageIndex = 0x45;
          if (iVar2 != 0) {
            messageIndex = 0x3d;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 3) {
            iVar2 = GetBitflagState(0x21c4);
            messageIndex = 0x45;
            if (iVar2 != 0) {
              messageIndex = 0x3e;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 4) {
              iVar2 = GetBitflagState(0x21c5);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x3f;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c6);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x40;
              }
            }
          }
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 8) {
          if ((uVar1 & 0xffffffff) < 6) {
            iVar2 = GetBitflagState(0x21c7);
            messageIndex = 0x45;
            if (iVar2 != 0) {
              messageIndex = 0x41;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 7) {
              iVar2 = GetBitflagState(0x21c8);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x42;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c9);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x43;
              }
            }
          }
        }
        else {
          if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
          {
            messageIndex = 874;
          }
          else if ( uVar1 == 9 )
          {
            iVar2 = GetBitflagState(0x21cb);
            messageIndex = 0x45;
            if (iVar2 != 0) 
            {
              messageIndex = 0x44;
            }
          }
          else
          {
            return;
          }
        }
      }
      if (messageIndex == -1) {
        return;
      }
    }
    else {
      messageIndex = 0x3c;
    }
    CallNaviDialogue(param_1, messageIndex, 3, 0, -1, param_6, '\0', 1, 0.0);
  }
  return;
}

static void PartyMemberGotDowned_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{

  FUNC_LOG("Loading PartyMemberGotDowned_NaviDialogue\n");
  if ( GetBtlPlayerUnitFromID( 1 )->currentHP == 0 )
  {
    return;
  }
  u64 uVar1;
  int iVar2;
  int messageIndex;
  int iVar4;
  
  if ((param_2->field118 >> 0x16 & 1) != 0) {
    iVar4 = 3;
    if (param_4 == 1) {
      uVar1 = param_3->pointer_2->unitID;
      if ((uVar1 & 0xffffffff) < 5) {
        if ((uVar1 & 0xffffffff) < 2) {
          if ((int)uVar1 == 0) {
            return;
          }
          iVar2 = GetBitflagState(0x21c3);
          messageIndex = 0x4f;
          if (iVar2 != 0) {
            messageIndex = 0x47;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 3) {
            iVar2 = GetBitflagState(0x21c4);
            messageIndex = 0x4f;
            if (iVar2 != 0) {
              messageIndex = 0x48;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 4) {
              iVar2 = GetBitflagState(0x21c5);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x49;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c6);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x4a;
              }
            }
          }
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 8) {
          if ((uVar1 & 0xffffffff) < 6) {
            iVar2 = GetBitflagState(0x21c7);
            messageIndex = 0x4f;
            if (iVar2 != 0) {
              messageIndex = 0x4b;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 7) {
              iVar2 = GetBitflagState(0x21c8);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x4c;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c9);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x4d;
              }
            }
          }
        }
        else {
          if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
          {
            messageIndex = 875;
          }
          else if ( uVar1 == 9 )
          {
            iVar2 = GetBitflagState(0x21cb);
            messageIndex = 0x4f;
            if (iVar2 != 0) 
            {
              messageIndex = 0x4e;
            }
          }
          else
          {
            return;
          }
        }
      }
      if (messageIndex == 0x47) {
        iVar4 = 5;
      }
      if (messageIndex == -1) {
        return;
      }
    }
    else {
      messageIndex = 0x46;
    }
    CallNaviDialogue( param_1, messageIndex, iVar4, 0, -1, param_6, '\0', 1, 0.0);
  }
  return;
}

static void PartyMemberDodgedAttack_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{

  FUNC_LOG("Loading PartyMemberDodgedAttack_NaviDialogue\n");
  if ( GetBtlPlayerUnitFromID( 1 )->currentHP == 0 )
  {
    return;
  }
  u64 uVar1;
  int iVar2;
  int messageIndex;
  
  if ((param_2->field118 >> 0x16 & 1) != 0) 
  {
    uVar1 = param_3->pointer_2->unitID;
    if ((uVar1 & 0xffffffff) < 5) 
    {
      if ((uVar1 & 0xffffffff) < 2) 
      {
        if ((int)uVar1 == 0) 
        {
          return;
        }
        iVar2 = GetBitflagState(0x21c3);
        messageIndex = 0x59;
        if (iVar2 != 0) 
        {
          messageIndex = 0x51;
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 3) 
        {
          iVar2 = GetBitflagState(0x21c4);
          messageIndex = 0x59;
          if (iVar2 != 0) 
          {
            messageIndex = 0x52;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 4) 
          {
            iVar2 = GetBitflagState(0x21c5);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x53;
            }
          }
          else 
          {
            iVar2 = GetBitflagState(0x21c6);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x54;
            }
          }
        }
      }
    }
    else 
    {
      if ((uVar1 & 0xffffffff) < 8) 
      {
        if ((uVar1 & 0xffffffff) < 6) 
        {
          iVar2 = GetBitflagState(0x21c7);
          messageIndex = 0x59;
          if (iVar2 != 0) 
          {
            messageIndex = 0x55;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 7) 
          {
            iVar2 = GetBitflagState(0x21c8);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x56;
            }
          }
          else 
          {
            iVar2 = GetBitflagState(0x21c9);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x57;
            }
          }
        }
      }
      else 
      {
        if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
        {
          messageIndex = 876;
        }
        else if ( uVar1 == 9 )
        {
          iVar2 = GetBitflagState(0x21cb);
          messageIndex = 0x59;
          if (iVar2 != 0) 
          {
            messageIndex = 0x58;
          }
        }
        else
        {
          return;
        }
      }
    }
    if ((messageIndex != -1) && (uVar1 = FUN_748ff0((int)param_1), (int)uVar1 == -1)) {
      CallNaviDialogue(param_1,messageIndex,1,0,-1,param_6,'\0',1,0.0);
    }
  }
  return;
}

static void PartyMemberKnockEnemyDown_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberKnockEnemyDown_NaviDialogue\n");
  //printf("Party Member Knocked and Enemy down! Character ID -> %d\nFUN_007489a8 result -> %d, a4 is -> %d\n", param_3->pointer_2->unitID, FUN_007489a8(param_1,(int)param_2), param_4);
  int knockdown = param_4;
  int numberOfExistingEnemies = FUN_007489a8(param_1,(int)param_2);

  if ( param_3->pointer_2->unitID != 10 ) // not Kasumi, return
  {
    if ( randomIntBetween( 1, 100 ) >= 50 ) // half chance of likelyhood
    {
      int PartyMemberID = GetUnitIDFromPartySlot( randomIntBetween( 1, 3 ) ); //grab random party member

      if( PartyMemberID != 0 && 
          !isPlayerUnitDead(PartyMemberID) && 
          !btlUnitHasAilment( GetBtlPlayerUnitFromID(PartyMemberID), 0x400000 ) && 
          PartyMemberCombatVoiceInstance[PartyMemberID] != 0 )
      {
        LoadSoundByCueIDCombatVoice( PartyMemberCombatVoiceInstance[PartyMemberID], PartyMemberCombatVoiceInstanceB[PartyMemberID], 69 + param_3->pointer_2->unitID, 1 );
        return;
      }
    }
    
    SHK_CALL_HOOK( FUN_00b20ec8, param_1, param_2, param_3, param_4, param_5, param_6 );
    return;
  }

  int messageIndex = -1;
  if ( param_3->pointer_2->unitID == 10 && knockdown != 0 && CONFIG_ENABLED( enableKasumi ) )
  {
    if ( knockdown ==  numberOfExistingEnemies )
    {
      messageIndex = 899;
    }
    else if ( knockdown == 4 )
    {
      messageIndex = 900;
    }
    else if ( knockdown == 3 )
    {
      messageIndex = 901;
    }
    else if ( knockdown == 2 )
    {
      messageIndex = 902;
    }
    else if ( knockdown == 1 )
    {
      messageIndex = 903;
    }
  }
  if (messageIndex != -1) 
  {
    numberOfExistingEnemies = FUN_748ff0((int)param_1);
    if ((int)numberOfExistingEnemies == -1) 
    {
      CallNaviDialogue( param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0 );
    }
  }
  return;
}

static void PartyMemberWarnAilment_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{

  FUNC_LOG("Loading PartyMemberWarnAilment_NaviDialogue\n");
  
  if ( GetBtlPlayerUnitFromID( 1 )->currentHP == 0 )
  {
    return;
  }
  btlUnit_Unit* Kasumi = GetBtlUnitInCombat( 1, 10 );

  if ( Kasumi == 0x0 )
  {
    DEBUG_LOG("Kasumi not in Combat\n");
    return SHK_CALL_HOOK( FUN_00b1ca18, param_1, param_2, param_3, param_4, param_5, param_6 );
  }
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;
  
  if ( !CONFIG_ENABLED( enableKasumi ) )
  {
    DEBUG_LOG("Kasumi Config disabled\n");
    return SHK_CALL_HOOK( FUN_00b1ca18, param_1, param_2, param_3, param_4, param_5, param_6 );
  }

  if ( Kasumi->StatusAilments > 0 )
  {
    //printf("Kasumi Ailment is 0x%x\n", Kasumi->StatusAilments);
    for ( int i = 0; i <= 11; i++)
    {
      if ( Kasumi->StatusAilments & ( 1 << i ) && hasUzukiAilmentAnnounce[i] == false )
      {
        messageIndex = 881 + i;
        hasUzukiAilmentAnnounce[i] = true;
      }
    }
  }
  else if ( Kasumi->currentHP < GetBtlUnitMaxHP( Kasumi ) / 3 && !UzukiLowHPWarn )
  {
    messageIndex = 880;
    UzukiLowHPWarn = true;
  }
  else if ( Kasumi->Buffs.BuffStatus_ATK && 
          Kasumi->BuffsDirDur.BuffDir_ATK == 14 && 
          Kasumi->BuffsDirDur.BuffDur_ATK >= 1 && 
          !UzukiDebuffAttackWarn )
  {
    messageIndex = 877;
    UzukiDebuffAttackWarn = true;
  }
  else if ( Kasumi->Buffs.BuffStatus_DEF && 
          Kasumi->BuffsDirDur.BuffDir_DEF == 14 && 
          Kasumi->BuffsDirDur.BuffDur_DEF >= 1 && 
          !UzukiDebuffDeffenseWarn )
  {
    messageIndex = 878;
    UzukiDebuffDeffenseWarn = true;
  }
  else if ( Kasumi->Buffs.BuffStatus_EVA && 
          Kasumi->BuffsDirDur.BuffDir_EVA == 14 && 
          Kasumi->BuffsDirDur.BuffDur_EVA >= 1 && 
          !UzukiDebuffSpeedWarn )
  {
    messageIndex = 879;
    UzukiDebuffSpeedWarn = true;
  }
  if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)) &&
  ((uVar9 = FUN_007490a4(param_1, messageIndex), (int)uVar9 == 0 &&
  (param_1->field2c != messageIndex)))) 
  {
    CallNaviDialogue(param_1, messageIndex, 5, 0, -1, param_6, '\0', 1, 0.0);
    param_1->field2c = messageIndex;
  }
  else SHK_CALL_HOOK( FUN_00b1ca18, param_1, param_2, param_3, param_4, param_5, param_6 );
  /*printf("Kasumi->BuffsDirDur.BuffDir_ATK -> %d\n", Kasumi->BuffsDirDur.BuffDir_ATK);
  printf("Kasumi->BuffsDirDur.BuffDir_ACC -> %d\n", Kasumi->BuffsDirDur.BuffDir_ACC);
  printf("Kasumi->BuffsDirDur.BuffDir_DEF -> %d\n", Kasumi->BuffsDirDur.BuffDir_DEF);
  printf("Kasumi->BuffsDirDur.BuffDir_EVA -> %d\n", Kasumi->BuffsDirDur.BuffDir_EVA);
  printf("Kasumi->BuffsDirDur.BuffDur_ATK -> %d\n", Kasumi->BuffsDirDur.BuffDur_ATK);
  printf("Kasumi->BuffsDirDur.BuffDur_ACC -> %d\n", Kasumi->BuffsDirDur.BuffDur_ACC);
  printf("Kasumi->BuffsDirDur.BuffDur_DEF -> %d\n", Kasumi->BuffsDirDur.BuffDur_DEF);
  printf("Kasumi->BuffsDirDur.BuffDur_EVA -> %d\n", Kasumi->BuffsDirDur.BuffDur_EVA);
  printf("Kasumi->BuffsDirDur.BuffDir_ -> %d\n", Kasumi->BuffsDirDur.BuffDir_);*/
  return;
}

static void PartyMemberMissedEnemy_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberMissedEnemy_NaviDialogue\n");
  u8 bVar4;
  u64 uVar1;
  int iVar3;
  undefined8 uVar2;
  int messageIndex;
  int iVar5;
  
  if ((param_2->field118 >> 0x16 & 1) == 0) {
    return;
  }
  if ((param_3 != 0) && (bVar4 = FUN_002588b4(param_3->pointer_2), bVar4 == 0)) {
    return;
  }
  if ((0x304 < param_2->EncounterID && param_2->EncounterID < 0x307)) {
    return;
  }
  iVar5 = 3;
  uVar1 = param_3->pointer_2->unitID;
  if ((uVar1 & 0xffffffff) < 5) {
    if ((uVar1 & 0xffffffff) < 2) {
      if ((int)uVar1 == 0) {
        return;
      }
      iVar3 = GetBitflagState(0x21c3);
      messageIndex = 0x197;
      if (iVar3 != 0) {
        messageIndex = 399;
      }
    }
    else {
      if ((uVar1 & 0xffffffff) < 3) {
        iVar3 = GetBitflagState(0x21c4);
        messageIndex = 0x197;
        if (iVar3 != 0) {
          messageIndex = 400;
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 4) {
          iVar3 = GetBitflagState(0x21c5);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x191;
          }
        }
        else {
          iVar3 = GetBitflagState(0x21c6);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x192;
          }
        }
      }
    }
  }
  else {
    if ((uVar1 & 0xffffffff) < 8) {
      if ((uVar1 & 0xffffffff) < 6) {
        iVar3 = GetBitflagState(0x21c7);
        messageIndex = 0x197;
        if (iVar3 != 0) {
          messageIndex = 0x193;
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 7) {
          iVar3 = GetBitflagState(0x21c8);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x194;
          }
        }
        else {
          iVar3 = GetBitflagState(0x21c9);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x195;
          }
        }
      }
    }
    else {
      if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
      {
        messageIndex = 906;
      }
      else if ( uVar1 == 9 )
      {
        iVar3 = GetBitflagState(0x21cb);
        messageIndex = 0x197;
        if (iVar3 != 0) 
        {
          messageIndex = 0x196;
        }
      }
      else
      {
        return;
      }
    }
  }
  
  iVar5 = 4;
  
  if (((messageIndex != -1) &&
      (uVar2 = FUN_007490a4((int)param_1,messageIndex), (int)uVar2 == 0)) &&
     (uVar1 = FUN_748ff0((int)param_1), (int)uVar1 == -1)) 
  {
    CallNaviDialogue(param_1,messageIndex,iVar5,0,-1,param_6,'\0',1,0.0);
  }
  return;
}

static void PartyMemberLandedCrit_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberLandedCrit_NaviDialogue\n");

  if ( param_3->pointer_2->unitID != 10 ) // not Kasumi, return
  {
    if ( randomIntBetween( 1, 100 ) >= 50 ) // half chance of likelyhood
    {
      int PartyMemberID = GetUnitIDFromPartySlot( randomIntBetween( 1, 3 ) ); //grab random party member

      if( PartyMemberID != 0 && !isPlayerUnitDead(PartyMemberID) && !btlUnitHasAilment( GetBtlPlayerUnitFromID(PartyMemberID), 0x400000 ) && PartyMemberCombatVoiceInstance[PartyMemberID] != 0 )
      {
        LoadSoundByCueIDCombatVoice( PartyMemberCombatVoiceInstance[PartyMemberID], PartyMemberCombatVoiceInstanceB[PartyMemberID], 69 + param_3->pointer_2->unitID, 1 );
        return;
      }
    }
    
    SHK_CALL_HOOK( FUN_00b21e28, param_1, param_2, param_3, param_4, param_5, param_6 );
    return;
  }

  
  SHK_CALL_HOOK( FUN_00b21e28, param_1, param_2, param_3, param_4, param_5, param_6 );
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 905;

    if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)))
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void PartyMemberGetUp_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberGetUp_NaviDialogue\n");
  SHK_CALL_HOOK( FUN_00b22138, param_1, param_2, param_3, param_4, param_5, param_6 );
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 893;

    if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)))
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void PartyMemberExploitedWeakness_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberExploitedWeakness_NaviDialogue\n");

  if ( param_3->pointer_2->unitID != 10 ) // not Kasumi, return
  {
    if ( randomIntBetween( 1, 100 ) >= 50 ) // half chance of likelyhood
    {
      int PartyMemberID = GetUnitIDFromPartySlot( randomIntBetween( 1, 3 ) ); //grab random party member

      if( PartyMemberID != 0 && !isPlayerUnitDead(PartyMemberID) && !btlUnitHasAilment( GetBtlPlayerUnitFromID(PartyMemberID), 0x400000 ) && PartyMemberCombatVoiceInstance[PartyMemberID] != 0 )
      {
        LoadSoundByCueIDCombatVoice( PartyMemberCombatVoiceInstance[PartyMemberID], PartyMemberCombatVoiceInstanceB[PartyMemberID], 69 + param_3->pointer_2->unitID, 1 );
        return;
      }
    }
    
    SHK_CALL_HOOK( FUN_00b21bf8, param_1, param_2, param_3, param_4, param_5, param_6 );
    return;
  }
  
  SHK_CALL_HOOK( FUN_00b21bf8, param_1, param_2, param_3, param_4, param_5, param_6 );
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 904;

    if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)) &&
    (param_1->field2c != messageIndex))
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
      param_1->field2c = messageIndex;
    }
  }
  return;
}

static void PartyMemberRunAway_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberRunAway_NaviDialogue\n");
  SHK_CALL_HOOK( FUN_00b22f60, param_1, param_2, param_3, param_4, param_5, param_6 );
  bool bVar4;
  if ((param_2->field118 >> 0x16 & 1) == 0) 
  {
    return;
  }
  if ((param_3 != 0) && (bVar4 = FUN_002588b4(param_3->pointer_2), bVar4 == 0)) {
    return;
  }
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 907;

    if ( messageIndex != -1 )
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void PartyMemberDied_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  FUNC_LOG("Loading PartyMemberDied_NaviDialogue\n");
  if ( GetBtlPlayerUnitFromID( 1 )->currentHP == 0 )
  {
    return;
  }
  SHK_CALL_HOOK( FUN_00b1bf38, param_1, param_2, param_3, param_4, param_5, param_6 );
  bool bVar4;
  if ((param_2->field118 >> 0x16 & 1) == 0) 
  {
    return;
  }
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;

  if ( Kasumi == 10 )
  {
    messageIndex = 873;

    if ( messageIndex != -1 )
    {
      CallNaviDialogue(param_1, messageIndex, 4, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void NewGameSetup( void )
{
  FUNC_LOG("Loading NewGameSetup\n");
  SHK_CALL_HOOK( FUN_0024bb54 );
  CheckKasumiEquipment( GetBtlPlayerUnitFromID( 10 ) );
  return;
}

int BulletAmountList[]                = { 0, 8,  4,  5, 12, 12,  6, 2, 0,  6,  5 };
int BulletAmountList_ConfidantBoost[] = { 0, 16, 8, 10, 24, 24, 12, 4, 0, 12, 10 };

static void SetBulletsToMax(btlUnit_Unit* a1)
{
  FUNC_LOG("Loading SetBulletsToMax\n");
  u32 bullets;
  u32 unitID;
  
  unitID = a1->unitID;
  if ((a1->unitType == 2) || (10 < unitID)) 
  {
    bullets = 0;
  }
  else 
  {
    if (!IsConfidantBonusObtained(165)) 
    {
      bullets = BulletAmountList[unitID];
    }
    else 
    {
      bullets = BulletAmountList_ConfidantBoost[unitID];
    }
  }
  a1->numOfBullets = bullets;

  return;
}

static int GetMaxBullets(btlUnit_Unit* a1)
{
  FUNC_LOG("Loading GetMaxBullets\n");
  int bullets;
  u32 unitID;
  
  unitID = a1->unitID;
  if ((a1->unitType == 2) || (10 < unitID)) 
  {
    bullets = 0;
  }
  else 
  {
    if (!IsConfidantBonusObtained(165)) 
    {
      bullets = BulletAmountList[unitID];
    }
    else 
    {
      bullets = BulletAmountList_ConfidantBoost[unitID];
    }
  }
  return bullets;
}

static void SetBullets( btlUnit_Unit* a1, int targetBullets)
{
  FUNC_LOG("Loading SetBullets\n");
  u32 bullets;
  u32 unitID;
  
  unitID = a1->unitID;
  if ((a1->unitType == 2) || (10 < unitID)) 
  {
    bullets = 0;
  }
  else 
  {
    if (!IsConfidantBonusObtained(165)) 
    {
      bullets = BulletAmountList[unitID];
    }
    else 
    {
      bullets = BulletAmountList_ConfidantBoost[unitID];
    }
  }

  u32 newBullets = a1->numOfBullets +  targetBullets;

  if (newBullets < bullets) // prevent going over max
  {
    bullets = newBullets;
  }
  a1->numOfBullets = newBullets;
  return;
}

void CheckKasumiEquipment(btlUnit_Unit* Kasumi)
{
  if ( Kasumi->meleeID == 1 )
  {
    Kasumi->meleeID = 248;
    Kasumi->rangedWeaponID = 0x8000 + 223;
    Kasumi->protectorID = 0x1000 + 101;
    Kasumi->Flags = GetBtlPlayerUnitFromID( 6 )->Flags; //Copy unit flags from existing female party member
  }
  return;
}

static void BattleEndSkillChecks( u64 a1, u64 a2, u64 a3 )
{
  FUNC_LOG("Loading BattleEndSkillChecks\n");
  SHK_CALL_HOOK( FUN_00661220, a1, a2, a3 );
  for ( int i = 1; i <= 10; i++ )
  {
    btlUnit_Unit* PlayerUnit = GetBtlPlayerUnitFromID(i);
    SetBulletsToMax( PlayerUnit );
    PlayerUnit->Flags = (PlayerUnit->Flags & ~(1UL << 5)) | (0 << 5); // remove infinite endure flag
    PartyMemberCombatVoiceInstance[i] = 0;
    PartyMemberCombatVoiceInstanceB[i] = 0;
  }
  hasAkechiEndured = false;
  hasSumiEndured = false;
  for ( int i = 0; i <= 11; i++) // navi stuff
  {
    hasUzukiAilmentAnnounce[i] = false;
  }
  UzukiLowHPWarn = false;
  UzukiDebuffAttackWarn = false;
  UzukiDebuffDeffenseWarn = false;
  UzukiDebuffSpeedWarn = false;
  randomizedCombatOutfit = true;
  currentActingUnit = 0;

  btlUnit_Unit* Joker = GetBtlPlayerUnitFromID(1);
  if ( Joker->currentHP == 0 && isPreventGameOver )
  {
    Joker->currentHP = 1;
  }

  SetBitflagState( 0x209C, 0 ); // flag checked for ending twins encounter early

  return;
}

static void MonaNavi_KnockEnemyDown_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  if ( param_3->pointer_2->unitID != 10 ) // not Kasumi, return
  {
    if ( randomIntBetween( 1, 100 ) >= 50 ) // half chance of likelyhood
    {
      int PartyMemberID = GetUnitIDFromPartySlot( randomIntBetween( 1, 3 ) ); //grab random party member

      if( PartyMemberID != 0 && 
          !isPlayerUnitDead(PartyMemberID) && 
          !btlUnitHasAilment( GetBtlPlayerUnitFromID(PartyMemberID), 0x400000 ) && 
          PartyMemberCombatVoiceInstance[PartyMemberID] != 0 )
      {
        LoadSoundByCueIDCombatVoice( PartyMemberCombatVoiceInstance[PartyMemberID], PartyMemberCombatVoiceInstanceB[PartyMemberID], 69 + param_3->pointer_2->unitID, 1 );
        return;
      }
    }
  }
    
  SHK_CALL_HOOK( FUN_00b18368, param_1, param_2, param_3, param_4, param_5, param_6 );
  return;
}

static void MonaNavi_ExploitedWeakness_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  if ( param_3->pointer_2->unitID != 10 ) // not Kasumi, return
  {
    if ( randomIntBetween( 1, 100 ) >= 50 ) // half chance of likelyhood
    {
      int PartyMemberID = GetUnitIDFromPartySlot( randomIntBetween( 1, 3 ) ); //grab random party member

      if( PartyMemberID != 0 && 
          !isPlayerUnitDead(PartyMemberID) && 
          !btlUnitHasAilment( GetBtlPlayerUnitFromID(PartyMemberID), 0x400000 ) && 
          PartyMemberCombatVoiceInstance[PartyMemberID] != 0 )
      {
        LoadSoundByCueIDCombatVoice( PartyMemberCombatVoiceInstance[PartyMemberID], PartyMemberCombatVoiceInstanceB[PartyMemberID], 69 + param_3->pointer_2->unitID, 1 );
        return;
      }
    }
  }
    
  SHK_CALL_HOOK( FUN_00b190a8, param_1, param_2, param_3, param_4, param_5, param_6 );
  return;
}

static void MonaNavi_LandedCrit_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  if ( param_3->pointer_2->unitID != 10 ) // not Kasumi, return
  {
    if ( randomIntBetween( 1, 100 ) >= 50 ) // half chance of likelyhood
    {
      int PartyMemberID = GetUnitIDFromPartySlot( randomIntBetween( 1, 3 ) ); //grab random party member

      if( PartyMemberID != 0 && 
          !isPlayerUnitDead(PartyMemberID) && 
          !btlUnitHasAilment( GetBtlPlayerUnitFromID(PartyMemberID), 0x400000 ) && 
          PartyMemberCombatVoiceInstance[PartyMemberID] != 0 )
      {
        LoadSoundByCueIDCombatVoice( PartyMemberCombatVoiceInstance[PartyMemberID], PartyMemberCombatVoiceInstanceB[PartyMemberID], 69 + param_3->pointer_2->unitID, 1 );
        return;
      }
    }
  }
    
  SHK_CALL_HOOK( FUN_00b192e8, param_1, param_2, param_3, param_4, param_5, param_6 );
  return;
}

static u64 ItemMenuUseHealingSkill(short srcPlayer, short tgtPlayer, short* a3) {
  printf("FUN_00487c58: %d, %d, %x\n", srcPlayer, tgtPlayer, a3);
  short sVar1;
  short sVar2;
  u16 uVar3;
  u32 skillID;
  u32 uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  u64 uVar4;
  u64 uVar5;
  short sVar11;
  s64 lVar12;
  short *psVar13;
  u32 uVar14;
  u64 uVar15;
  int iVar16;
  
  uVar15 = 1;
  iVar10 = 0;
  uVar14 = 0;
  sVar1 = a3[0x11];
  sVar2 = a3[0x10];
  uVar3 = a3[((int)sVar1 + (int)sVar2) * 2 + 0xf8];
  skillID = FUN_0026090c((u32)uVar3);
  uVar7 = FUN_002604cc((u32)uVar3);
  if ((uVar7 & 0x8000000) == 0) {
    uVar5 = FUN_002604bc((u32)uVar3);
    if ((int)uVar5 == 6) {
      uVar15 = 4;
LAB_0048829c:
      iVar10 = FUN_0026028c((u32)uVar3);
      SET_ITEM_NUM_Hook((u32)uVar3,iVar10 - 1U & 0xff);
      iVar10 = (int)uVar15;
      if (iVar10 == 3) {
        uVar4 = 1;
        if ((int)uVar4 == 0) {
          iVar10 = FUN_0026028c((u32)uVar3);
          SET_ITEM_NUM_Hook((u32)uVar3,iVar10 + 1U & 0xff);
          PlayFromSystemACB(7);
        }
        FUN_00116ad0(0x2213);
        goto LAB_004883ec;
      }
      if (iVar10 == 5 || iVar10 == 6) goto LAB_004883ec;
      FUN_004873c4((int)a3,uVar14);
      sVar11 = a3[((int)sVar1 + (int)sVar2) * 2 + 0xf9] + -1;
      a3[((int)sVar1 + (int)sVar2) * 2 + 0xf9] = sVar11;
      if (sVar11 != 0) goto LAB_004883ec;
      FUN_00480a28((int)a3);
      if (a3[0x120f6] < 8) {
        ScrollThroughItemPartyList((int)a3,1,0);
        if ((int)((s64)a3[0x120f6] + -1) < (int)a3[0x10]) {
          ScrollThroughItemPartyList((int)a3,0,(s64)a3[0x120f6] + -1);
        }
      }
      else {
        lVar12 = (s64)a3[0x120f6] + -8;
        if ((int)lVar12 < (int)a3[0x11]) {
          ScrollThroughItemPartyList((int)a3,1,lVar12);
        }
      }
      uVar15 = 2;
    }
    else {
      if (skillID != 0x187) {
        uVar7 = FUN_00260780((u32)uVar3);
        if ((uVar7 & 8) != 0) {
          lVar12 = 0xc - 0x80;
          memzero((void *)lVar12,0x14);
          return lVar12;
        }
        if (tgtPlayer == -1) {
          iVar10 = 0;
          iVar16 = 0;
          psVar13 = a3;
          if (a3[0x12100] > 0) {
            printf("Healing all members\n");
            for (int i = 0; i < 10; i++) {
              if (partyMembers[i]) {
                btlUnit_Unit* pUnit = GetBtlPlayerUnitFromID(i + 1);
                uVar5 = FUN_0042f5ec(srcPlayer,i + 1,skillID,1);
                if ((int)uVar5 == 0) {
                  FUN_0042f3ec(srcPlayer,i + 1,skillID,1);
                  iVar16++;
                }
              }
            }
            if (iVar16 != 0) {
              uVar4 = FUN_0042f994(skillID);
              uVar14 = (u32)uVar4;
              goto LAB_0048829c;
            }
          }
          uVar15 = 0;
          PlayFromSystemACB(4);
          return uVar15;
        }
        printf("Healing for team member %d\n", tgtPlayer);
        uVar5 = FUN_0042f5ec(srcPlayer,tgtPlayer,skillID,1);
        if ((int)uVar5 != 0) {
          uVar15 = 0;
          PlayFromSystemACB(4);
          return uVar15;
        }
        FUN_0042f3ec(srcPlayer,tgtPlayer,skillID,1);
        uVar4 = FUN_0042f994(skillID);
        uVar14 = (u32)uVar4;
        goto LAB_0048829c;
      }
      uVar15 = 0;
      uVar5 = FUN_004260c4();
      if ((int)uVar5 != 0) {
        uVar15 = 3;
      }
      if ((int)uVar15 != 0) goto LAB_0048829c;
LAB_004883ec:
      skillID = (u32)uVar15;
      if (skillID == 4 || skillID > 6) return uVar15;
      else if (skillID == 5 || skillID > 2) {
        PlayFromSystemACB(7);
        return uVar15;
      } else if (skillID == 0) {
        PlayFromSystemACB(4);
        return uVar15;
      }
    }
    uVar5 = FUN_002604bc((u32)uVar3);
    if ((int)uVar5 != 6) PlayFromSystemACB(7);
  }
  else {
    if (uVar3 != 0x3152) {
      FUN_0029793c((u32)uVar3);
      uVar15 = 6;
      goto LAB_0048829c;
    }
    iVar16 = 0;
    psVar13 = a3;
    if (0 < a3[0x12100]) {
LAB_00487d40:
      iVar8 = FUN_004e35e0((int)psVar13[0x120f7]);
      iVar9 = FUN_004e3610((int)psVar13[0x120f7]);
      if (iVar9 <= iVar8) {
        iVar16 = iVar16 + 1;
        psVar13 = psVar13 + 1;
        if (a3[0x12100] <= iVar16) {
          uVar15 = 0;
          PlayFromSystemACB(4);
          return uVar15;
        }
        goto LAB_00487d40;
      }
      psVar13 = a3;
      if (0 < a3[0x12100]) {
        do {
          FUN_004e3640((int)psVar13[0x120f7]);
          iVar10 = iVar10 + 1;
          psVar13 = psVar13 + 1;
        } while (iVar10 < a3[0x12100]);
      }
      uVar14 = 4;
      goto LAB_0048829c;
    }
    uVar15 = 0;
    PlayFromSystemACB(4);
  }
  return uVar15;
}

/*static int FUN_00436730Hook( int a1 )
{
  int result = 0;

  if ( a1 == 1108082688 )
  {
    a1 = 52;
  }
  else if ( a1 == 1106247680 )
  {
    a1 = 53;
  }
  //printf( "FUN_00436730 called; a1 -> %d\n", a1 );
  result = SHK_CALL_HOOK( FUN_00436730, a1 );
  //printf( "FUN_00436730 result is -> 0x%x\n", result );
  return result;
}*/

static int FUN_0034e128Hook( int a1 )
{
  int result = SHK_CALL_HOOK( FUN_0034e128, a1 );

  ReadKasumiData();
  return result;
}

static int FUN_0060dd98Hook(int a1, char* a2, int a3, char* a4)
{
  WriteKasumiData();
  return SHK_CALL_HOOK( FUN_0060dd98, a1, a2, a3, a4 );
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void KasumiInit( void )
{
  randomSetSeed( getTicks() );
  //printf("Uzuki Fixes Module loaded.\n");
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( FUN_00425b0c, isPartyMemberUnlocked);
  SHK_BIND_HOOK( FUN_00b20618, PartyMemberFinishedLastEnemy_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1c1a0, PartyMemberWarnHealthLow_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1c398, PartyMemberGotDowned_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1c610, PartyMemberDodgedAttack_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b20ec8, PartyMemberKnockEnemyDown_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1ca18, PartyMemberWarnAilment_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b21980, PartyMemberMissedEnemy_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b21bf8, PartyMemberExploitedWeakness_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b21e28, PartyMemberLandedCrit_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b22138, PartyMemberGetUp_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b22f60, PartyMemberRunAway_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1bf38, PartyMemberDied_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1bd50, JokerDied_NaviDialogue );
  SHK_BIND_HOOK( FUN_0025b888, SetBulletsToMax );
  SHK_BIND_HOOK( FUN_0025b740, GetMaxBullets );
  SHK_BIND_HOOK( FUN_0025b7b8, SetBullets );
  SHK_BIND_HOOK( FUN_0024bb54, NewGameSetup );
  SHK_BIND_HOOK( FUN_00661220, BattleEndSkillChecks );
  SHK_BIND_HOOK( FUN_00b18368, MonaNavi_KnockEnemyDown_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b190a8, MonaNavi_ExploitedWeakness_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b192e8, MonaNavi_LandedCrit_NaviDialogue );
  SHK_BIND_HOOK( FUN_00425de0, BuildPartyMemberStatsMenu );
  SHK_BIND_HOOK( FUN_00425c80, BuildPartyMemberItemsMenu );
  SHK_BIND_HOOK( FUN_0034e128, FUN_0034e128Hook );
  SHK_BIND_HOOK( FUN_0060dd98, FUN_0060dd98Hook );
  SHK_BIND_HOOK( FUN_00425d20, BuildPartyMemberEquipMenu );
  SHK_BIND_HOOK( FUN_00475e10, SetPartyTextPosition );
  SHK_BIND_HOOK( FUN_0047de54, ScrollThroughEquipPartyList );
  SHK_BIND_HOOK( FUN_00449c6c, ScrollThroughSkillPartyList );
  SHK_BIND_HOOK( FUN_0047f91c, ScrollThroughItemPartyList );
  SHK_BIND_HOOK( FUN_00480cc0, ItemMenuInit );
  SHK_BIND_HOOK( FUN_0044a5d8, SkillMenuInit );
  SHK_BIND_HOOK( FUN_0044f7ac, SkillMenuHealingSkillInit );
  SHK_BIND_HOOK( FUN_0044f8e0, SkillMenuUseHealingSkill );
  SHK_BIND_HOOK( FUN_00487c58, ItemMenuUseHealingSkill );
}

void KasumiShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif