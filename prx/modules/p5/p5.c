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
#include "lib/shk.h"
#include "p5.h"
#include "math.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

int partyListOffset = 0;
int partyListTotal = 0;
bool partyMembers[10] = {false};
int skillMenuPartyMemberSource = 0;

btlEquipBgmTableEntry btlEquipBgmTable[] =
{
  { 159, 1 }, // DLC_001
  { 158, 2 }, // DLC_002
  { 161, 3 }, // DLC_003
  { 160, 4 }, // DLC_004
  { 163, 5 }, // DLC_005
  { 166, 6 }, // DLC_006
  { 167, 7 }, // DLC_007
  { 168, 8 }, // DLC_008
  { 169, 9 }, // DLC_009
  { 162, 10 }, // DLC_010
  { 170, 11 }, // DLC_011
  { 171, 12 }, // DLC_012
  { 172, 13 }, // DLC_013
  { 173, 14 }, // DLC_014
  { 174, 15 }, // DLC_015
  { 175, 16 }, // DLC_016
  { 176, 17 }, // DLC_017
  { 177, 18 }, // DLC_018
};

int RECOVERY_ALL( void )
{
    SHK_FUNCTION_CALL_0( 0x1edf9c, int );
}

int BULLET_RECOVERY( void )
{
    SHK_FUNCTION_CALL_0( 0x1f113c, int );
}

AI_UnitStruct* FLW_GetBattleUnitStructFromContext( void )
{
    SHK_FUNCTION_CALL_0( 0x1f2974, AI_UnitStruct* );
}

btlUnit_Unit* GetBtlPlayerUnitFromID( u32 id )
{
    SHK_FUNCTION_CALL_1( 0x24B200, btlUnit_Unit*, u32, id );
}

int GetUnitIDFromPartySlot( u32 id )
{
    SHK_FUNCTION_CALL_1( 0x24B21C, int, u32, id );
}

void SetPlayerBulletsToMax( btlUnit_Unit* id )
{
    SHK_FUNCTION_CALL_1( 0x25b888, void, btlUnit_Unit*, id );
}

int GetBtlUnitAilments( btlUnit_Unit* id )
{
    SHK_FUNCTION_CALL_1( 0x2590ac, int, btlUnit_Unit*, id );
}

u32 FUN_0025867c( u16 personaID )
{
    SHK_FUNCTION_CALL_1( 0x25867c, int, u16, personaID );
}

int FLW_GetIntArg( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f266c, int, u32, arg );
}

f64 FLW_GetFloatArg( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f2768, f64, u32, arg );
}

char* FLW_GetStringArg( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f2868, char*, u32, arg );
}

u64 GetBtlUnitPersona( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x25bdf8, u64, u32, arg );
}

u64 FUN_00263714( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x263714, u64, u32, arg );
}

int FLW_SetIntReturn( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f28d8, int, u32, arg );
}

u32 GetJokerTargetLvExp( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x2634cc, int, u32, arg );
}

u32 GetEquippedPersonaFunction( PersonaNames unitID )
{
    SHK_FUNCTION_CALL_1( 0x25be3c, u32, PersonaNames, unitID );
}

u32 ActualGetCount( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x24bf5c, u32, u32, arg );
}

u32 FUN_0025bdf8( u16 unitID )
{
    SHK_FUNCTION_CALL_1( 0x25bdf8, u32, u16, unitID );
}

void* MallocAndReturn( u32 size )
{
    SHK_FUNCTION_CALL_1( 0x625570, void*, u32, size );
}

void* malloc( u32 size )
{
    SHK_FUNCTION_CALL_1( 0x91edd8, void*, u32, size );
}

u64 GetSavedataBitflagAlt( u64 flag )
{
    SHK_FUNCTION_CALL_1( 0x116a94, u64, u64, flag );
}

void* SomethingAboutLoadingVoices( void* someAddressPointer )
{
    SHK_FUNCTION_CALL_1( 0x74a9d0, void*, void*, someAddressPointer );
}

void SetUnitLv( u32 unitID, u32 targetLv )
{
    SHK_FUNCTION_CALL_2( 0x26a678, void, u32, unitID, u32, targetLv );
}

int FUN_002584cc( u32 a1, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x2584cc, int, u32, a1, u32, a2 );
}

int sub_24B274( u32 a1, u16 a2 )
{
    SHK_FUNCTION_CALL_2( 0x24B274, int, u32, a1, u32, a2 );
}

void FUN_0025cb8c( u32 unitID, u16 personaID )
{
    SHK_FUNCTION_CALL_2( 0x25cb8c, void, u32, unitID, u16, personaID );
}

void ActualSetCount( u32 CountNumber, u32 Value )
{
    SHK_FUNCTION_CALL_2( 0x24ba4c, void, u32, CountNumber, u32, Value );
}

void SetUnitEXP( u32 ID, u16 Lv )
{
    SHK_FUNCTION_CALL_2( 0x26a6f4, void, u32, ID, u16, Lv );
}

bool btlUnitHasAilment( btlUnit_Unit* Unit, u32 ailmentID )
{
    SHK_FUNCTION_CALL_2( 0x2590d8, bool, btlUnit_Unit*, Unit, u32, ailmentID );
}

u32 unitGetEquipment( btlUnit_Unit* unitID, EquipSlot equipSlotID )
{
    SHK_FUNCTION_CALL_2( 0x258bbc, u32, btlUnit_Unit*, unitID, EquipSlot, equipSlotID );
}

u32 CalculateAddressWithPointer( void* address, structA* unk )
{
    SHK_FUNCTION_CALL_2( 0xb10eb8, u32, void*, address, structA*, unk );
}

u32 PlayerUnitGetModelMinorID( u32 playerID, u8 a2, u8 a3 )
{
    SHK_FUNCTION_CALL_3( 0x45d24, u32, u32, playerID, u8, a2, u8, a3 );
}

void LoadSoundByCueIDCombatVoice( CueIDThingy* a1, u32 a2, u32 cueID,  u8 idk )
{
    SHK_FUNCTION_CALL_4( 0x74aac0, void, CueIDThingy*, a1, u32, a2, u32, cueID, u8, idk );
}

s32 setSeq( s32 seqId, void* params, s32 paramsSize, s32 r6 ) 
{
  SHK_FUNCTION_CALL_4( 0x10DB4, s32, s32, seqId, void*, params, s32, paramsSize, s32, r6 );
}

void FUN_0074ae50( void* a1, char* acb_string, char* awb_string, u32 unitID )
{
  SHK_FUNCTION_CALL_4( 0x74ae50, void, void*, a1, char*, acb_string, char*, awb_string, u32, unitID );
}

s32 sndManPlaySfx( s32 a0, s32 a1, s32 cueId, s32 a3, s32 a4, s32 a5 ) 
{ 
    SHK_FUNCTION_CALL_6( 0x10F0D4, s32, s32, a0, s32, a1, s32, cueId, s32, a3, s32, a4, s32, a5 ); 
}

int FUN_0090053c( u32 a1, u64 a2, u64 a3 )
{
  SHK_FUNCTION_CALL_3( 0x90053c, int, u32, a1, u64, a2, u64, a3 );
}

fileHandleStruct* open_file( char* file_path, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x1144ac, fileHandleStruct*, char*, file_path, u32, a2 );
}

u64 FUN_00118280( char *param_1, char *param_2, char *param_3, u8 param_4 )
{
    SHK_FUNCTION_CALL_4( 0x118280, u64, char*, param_1, char*, param_2, char*, param_3, u8, param_4 );
}

void FUN_00747f48( int* a1, int a2, int a3 )
{
    SHK_FUNCTION_CALL_3( 0x747f48, void, int*, a1, int, a2, int, a3 );
}

void FadeInFunction( u32 a1, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x281918, void, u32, a1, u32, a2 );
}

void FadeOutFunction( u32 a1, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x281d4c, void, u32, a1, u32, a2 );
}

u64 fsSync( int a1 )
{
    SHK_FUNCTION_CALL_1(0x114720, u64, int, a1);
}

u64 scrRunScript( u32 a1, u32 a2, u32 a3, u32 a4 )
{
    SHK_FUNCTION_CALL_4( 0x1f1e2c, u64, u32, a1, u32, a2, u32, a3, u32, a4 );
}

int semaphore_WaitPost( int a1 )
{
    SHK_FUNCTION_CALL_1( 0x1141fc, int, int, a1 );
}

FieldGetIDStruct* FUN_00352f40( void )
{
    SHK_FUNCTION_CALL_0( 0x00352f40, FieldGetIDStruct* );
}

ResourceHandleStruct* FUN_00015c34( int a1 )
{
    SHK_FUNCTION_CALL_1( 0x15c34, ResourceHandleStruct*, int, a1 );
}

int FUN_00014a38(u32 param_1, u32 param_2, u32 param_3, float param_4, float param_5)
{
    SHK_FUNCTION_CALL_5( 0x14a38, int, u32, param_1, u32, param_2,  u32, param_3, float, param_4, float, param_5 ); 
}

void FUN_000148dc(double param_1, u64* param_2, int param_3)
{
    SHK_FUNCTION_CALL_3( 0x148dc, void, double, param_1, u64*, param_2, int, param_3 );
}

void FUN_00324b70(u64* param_1, u64* param_2, u64 param_3)
{
    SHK_FUNCTION_CALL_3( 0x324b70, void, u64*, param_1, u64*, param_2, u64, param_3 );
}

int FUN_003b9110( int a1, int a2, int a3, int a4, int a5 )
{
    SHK_FUNCTION_CALL_5( 0x3b9110, int, int, a1, int, a2, int, a3, int, a4, int, a5 );
}

u64 FUN_0032c3d4( int arg )
{
    SHK_FUNCTION_CALL_1( 0x32c3d4, u64, int, arg );
}

u64 FUN_002ab314( int arg )
{
    SHK_FUNCTION_CALL_1( 0x2ab314, u64, int, arg );
}

int FUN_00320b1c( int arg )
{
    SHK_FUNCTION_CALL_1( 0x320b1c, int, int, arg );
}

u64 FUN_002e1338(u64* param_1, u64* param_2, char param_3)
{
    SHK_FUNCTION_CALL_3( 0x2e1338, u64, u64*, param_1, u64*, param_2, char, param_3 );
}

bool FUN_002d128c(int param_1, u64 param_2)
{
    SHK_FUNCTION_CALL_2( 0x2d128c, bool, int, param_1, u64, param_2 );
}

u64 FUN_002d7b6c( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d7b6c, u64, int*, arg );
}

int FUN_003316e0( int arg )
{
    SHK_FUNCTION_CALL_1( 0x3316e0, int, int, arg );
}

int FUN_002d7b18( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d7b18, int, int*, arg );
}

undefined8 FUN_002d0870( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d0870, undefined8, int*, arg );
}

undefined8 FUN_002d08b4( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d08b4, undefined8, int*, arg );
}

void FUN_0003b4b8(int param_1, char param_2)
{
    SHK_FUNCTION_CALL_2( 0x3b4b8, void, int, param_1, char, param_2 );
}

void FUN_0003b510(int param_1, char param_2)
{
    SHK_FUNCTION_CALL_2( 0x3b510, void, int, param_1, char, param_2 );
}

void FUN_002d9d00(undefined4* param_1)
{
    SHK_FUNCTION_CALL_1( 0x2d9d00, void, undefined4*, param_1 );
}

char* FUN_00968be8( void )
{
    SHK_FUNCTION_CALL_0( 0x968be8, char* );
}

char* FUN_00968bf4( void )
{
    SHK_FUNCTION_CALL_0( 0x968bf4, char* );
}

char* FUN_001a5834( void )
{
    SHK_FUNCTION_CALL_0( 0x1a5834, char* );
}

/*int criFsBinder_BindCpk( char* arg )
{
    SHK_FUNCTION_CALL_1( 0x114b74, int, char*, arg );
}*/

int criFsBinder_SetPriority ( int cpkBinder, int priority )
{
   SHK_FUNCTION_CALL_2( 0xAB8ED4, int, int, cpkBinder, int, priority );
}

int FUN_00ab563c( int* arg )
{
    SHK_FUNCTION_CALL_1( 0xab563c, int, int*, arg );
}

int FUN_001a52f8( int arg )
{
    SHK_FUNCTION_CALL_1( 0x1a52f8, int, int, arg );
}

int FUN_2604C4( int arg )
{
    SHK_FUNCTION_CALL_1( 0x2604C4, int, int, arg );
}

int FUN_748ff0( int arg )
{
    SHK_FUNCTION_CALL_1( 0x748ff0, int, int, arg );
}

int FUN_0024b28c( int arg )
{
    SHK_FUNCTION_CALL_1( 0x24b28c, int, int, arg );
}

int FUN_0010fbbc( int arg )
{
    SHK_FUNCTION_CALL_1( 0x10fbbc, int, int, arg );
}

int FUN_0091da04( void )
{
    SHK_FUNCTION_CALL_0( 0x91da04, int );
}

int GetCurrentBGMCueID( void )
{
    SHK_FUNCTION_CALL_0( 0x6ccb8, int );
}

int FUN_007489a8( int a1, int a2 )
{
    SHK_FUNCTION_CALL_2( 0x7489a8, int, int, a1, int, a2 );
}

bool FUN_002588b4( btlUnit_Unit* a1 )
{
    SHK_FUNCTION_CALL_1( 0x2588b4, bool, btlUnit_Unit*, a1 );
}

int FUN_0074805c( int a1, int a2 )
{
    SHK_FUNCTION_CALL_2( 0x74805c, int, int, a1, int, a2 );
}

int isCharacterAssistExpressonValid( short a1, short a2 )
{
    SHK_FUNCTION_CALL_2( 0x3b9644, int, short, a1, short, a2 );
}

bool FUN_007490a4( struct_2_pointers* a1, int a2 )
{
    SHK_FUNCTION_CALL_2( 0x7490a4, bool, struct_2_pointers*, a1, int, a2 );
}

void LoadEncounterEventSoundbank( int encounterID )
{
    SHK_FUNCTION_CALL_1( 0x745b9c, void, int, encounterID );
}

encounterIDTBL* GetEncounterEntryFromTBL( int encounterID)
{
    SHK_FUNCTION_CALL_1(0x263b94, encounterIDTBL*, int, encounterID);
}

bool GetBitflagState ( int bitFlagID )
{
    SHK_FUNCTION_CALL_1(0x24beac, bool, int, bitFlagID);
}

bool SetBitflagState ( int bitFlagID, u8 state )
{
    SHK_FUNCTION_CALL_2(0x24bef8, bool, int, bitFlagID, u8, state);
}

int GetBtlUnitMaxHP(btlUnit_Unit* param_1)
{
    SHK_FUNCTION_CALL_1(0x258d1c, int, btlUnit_Unit*, param_1);
}

int GetBtlUnitMaxSP(btlUnit_Unit* param_1)
{
    SHK_FUNCTION_CALL_1(0x258e48, int, btlUnit_Unit*, param_1);
}

void CallNaviDialogue (struct_2_pointers* param_1, int param_2, int param_3, int param_4, int param_5, int param_6, char param_7, short param_8, double param_9)
{
    SHK_FUNCTION_CALL_9(0x748d78, void, struct_2_pointers*, param_1, int, param_2, int, param_3, int, param_4, int, param_5, int, param_6, char, param_7, short, param_8, double, param_9);
}

PartyMemberLvUpThresholdExp* GetPartyMemberLvUpThreshold( int unitID )
{
    SHK_FUNCTION_CALL_1(0x263634, PartyMemberLvUpThresholdExp*, int, unitID);
}

PartyMemberPersonaBlock* GetPartyMemberPersonaBlock( int personaID )
{
    SHK_FUNCTION_CALL_1(0x263618, PartyMemberPersonaBlock*, int, personaID);
}

fieldworkdataStruct* GetFieldWorkData( void )
{
    SHK_FUNCTION_CALL_0( 0x352f40, fieldworkdataStruct* );
}

u16 GetTotalDays( void )
{
    SHK_FUNCTION_CALL_0( 0x48e80, u16 );
}

bool AI_CHK_SLIP( void )
{
    SHK_FUNCTION_CALL_0( 0x7bf3b0, bool );
}

int AI_CHK_ENIDHP( void )
{
    SHK_FUNCTION_CALL_0( 0x7bde18, int );
}

bool FUN_0031f9cc( void )
{
    SHK_FUNCTION_CALL_0( 0x31f9cc, bool );
}

itemTBLAccessoryEntry* GetAccessoryTBLEntry( u16 accessoryID )
{
    SHK_FUNCTION_CALL_1(0x263830, itemTBLAccessoryEntry*, u16, accessoryID);
}

unitTBLStats* GetUnitStatsBlockTBLEntry( u16 outfitID )
{
    SHK_FUNCTION_CALL_1(0x2636dc, unitTBLStats*, u16, outfitID);
}

ItemTBL_Outfit* GetOutfitTBLEntry( u16 enemyID )
{
    SHK_FUNCTION_CALL_1(0x2638e8, ItemTBL_Outfit*, u16, enemyID);
}

ItemTBL_MeleeWeapon* GetMeleeWeaponTBLEntry( u16 WeaponID )
{
    SHK_FUNCTION_CALL_1(0x2637d0 , ItemTBL_MeleeWeapon*, u16, WeaponID);
}

ItemTBL_RangedWeapon* GetRangedWeaponTBLEntry( u16 WeaponID )
{
    SHK_FUNCTION_CALL_1(0x26393c , ItemTBL_RangedWeapon*, u16, WeaponID);
}

bool BtlUnitCheckHasSkill ( btlUnit_Unit* btlUnit, int SkillID ){
    SHK_FUNCTION_CALL_2( 0x259148, bool, btlUnit_Unit*, btlUnit, int, SkillID );
}

u32 FUN_000442cc ( int param_1, int param_2 ){
    SHK_FUNCTION_CALL_2( 0x442cc, u32, int, param_1, int, param_2 );
}

void FUN_0024c018 ( u64 param_1, char param_2 ){
    SHK_FUNCTION_CALL_2( 0x24c018, void, u64, param_1, char, param_2 );
}

double FUN_00250a10( btlUnit_Unit* btlUnit, u32 param_2, undefined8 param_3 ){
    SHK_FUNCTION_CALL_3( 0x250a10, double, btlUnit_Unit*, btlUnit, u32, param_2, undefined8, param_3 );
}

bool AilmentCheck(btlUnit_Unit* btlUnit, u32 param_2){
    SHK_FUNCTION_CALL_2( 0x2590d8, bool, btlUnit_Unit*, btlUnit, u32, param_2 );
}

int GetCountEquipHPBonus(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x258d1c, int, btlUnit_Unit*, param_1 );
}

int GetCountEquipSPBonus(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x258e48, int, btlUnit_Unit*, param_1 );
}

bool CheckAccessoryEffect(btlUnit_Unit* btlUnit, int EffectID){
    SHK_FUNCTION_CALL_2( 0x2594b4, bool, btlUnit_Unit*, btlUnit, int, EffectID );
}

int FUN_26AE14(u32 param_1){
    SHK_FUNCTION_CALL_1( 0x26ae14, int, u32, param_1 );
}

undefined4 FUN_25E5D8(int param_1, btlUnit_Unit* btlUnit_1, btlUnit_Unit* btlUnit_2, undefined2 param_4, undefined4 param_5){
    SHK_FUNCTION_CALL_5( 0x25e5d8, undefined4, int, param_1, btlUnit_Unit*, btlUnit_1, btlUnit_Unit*, btlUnit_2, undefined2, param_4, undefined4, param_5 );
}

short FUN_25893C(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25893C, short, btlUnit_Unit*, param_1 );
}

u64 GetSkillData(btlUnit_Unit* btlUnit, u32 SkillId){
    SHK_FUNCTION_CALL_2( 0x25007c, u64, btlUnit_Unit*, btlUnit, u32, SkillId );
}

u8 FUN_2588B4(btlUnit_Unit *param_1){
    SHK_FUNCTION_CALL_1( 0x2588B4, u8, btlUnit_Unit*, param_1 );
}

double FUN_250A10(btlUnit_Unit* param_1, u32 param_2){
    SHK_FUNCTION_CALL_2( 0x250A10, double, btlUnit_Unit*, param_1, u32, param_2 );
}

bool GetEquipmentEffect(u64 AccessoryID, int param_2){
    SHK_FUNCTION_CALL_2( 0x260d18, bool, u64, AccessoryID, int, param_2 );
}

double CalculateResistEffect(int AccId){
    SHK_FUNCTION_CALL_1( 0x260dc4, double, int, AccId );
}

u64 FUN_2595C4(btlUnit_Unit* btlUnit,u32 param_2){
    SHK_FUNCTION_CALL_2( 0x2595C4, u64, btlUnit_Unit*, btlUnit, u32, param_2 );
}

void FUN_25A328(btlUnit_Unit* param_1,u64 buff){
    SHK_FUNCTION_CALL_2( 0x25A328, void, btlUnit_Unit*, param_1, u64, buff );
}

bool FUN_25963C(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25963C, bool, btlUnit_Unit*, param_1 );
}

bool AICheckSlipfunction(structA_2* param_1){
    SHK_FUNCTION_CALL_1( 0x6c9c60, bool, structA_2*, param_1 );
}

bool FUN_0025b6ac(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25b6ac, bool, btlUnit_Unit*, param_1 );
}

bool FUN_00258888(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x258888, bool, btlUnit_Unit*, param_1 );
}

int GetNumberOfBullets(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25b880, int, btlUnit_Unit*, param_1 );
}

bool IsBtlUnitDead( btlUnit_Unit* Unit, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x256754, bool, btlUnit_Unit*, Unit, u32, a2 );
}

bool IsConfidantBonusObtained(int param_1){
    SHK_FUNCTION_CALL_1( 0x54af94, bool, int, param_1 );
}

u64 FUN_252980(u16 SkillId, btlUnit_Unit* btlUnit_1,btlUnit_Unit *btlUnit_2,u8 mode){
    SHK_FUNCTION_CALL_4( 0x252980, u64, u16, SkillId, btlUnit_Unit*, btlUnit_1, btlUnit_Unit*, btlUnit_2, u8, mode );
}

u64 FUN_25294C(u16 SkillId,btlUnit_Unit *btlUnit_1,btlUnit_Unit *btlUnit_2,u8 mode){
    SHK_FUNCTION_CALL_4( 0x25294C, u64, u16, SkillId, btlUnit_Unit*, btlUnit_1, btlUnit_Unit*, btlUnit_2, u8, mode );
}

int SetCombatAnimation( CueIDThingy* a1, int a2, double a3, double a4, double a5)
{
    SHK_FUNCTION_CALL_5( 0x73d218, int, CueIDThingy*, a1, int, a2, double, a3, double, a4, double, a5 );
}

int SetTwinsDodgeAnimation( CueIDThingy* a1, double a2, double a3)
{
    SHK_FUNCTION_CALL_3( 0x73d218, int, CueIDThingy*, a1, double, a2, double, a3 );
}

u32 GetCurrentHP(btlUnit_Unit *param_1){
    SHK_FUNCTION_CALL_1( 0x2589c0, u32, btlUnit_Unit*, param_1 );
}

u32 GetCurrentSP(btlUnit_Unit *param_1){
    SHK_FUNCTION_CALL_1( 0x2589cc, u32, btlUnit_Unit*, param_1 );
}

PersonaTBL_Segment0* PersonaTBL_GetPersonaStatsSegment0( int PersonaID )
{
    SHK_FUNCTION_CALL_1( 0x2635c8, PersonaTBL_Segment0*, int, PersonaID );
}

void PrepareDLCBGM( void ){
    SHK_FUNCTION_CALL_0( 0x6ccc8, void );
}

u32 FUN_0031f35c( void ){
    SHK_FUNCTION_CALL_0( 0x31f35c, u32 );
}

s64 FUN_00967abc( u32 a1, int a2 ){
    SHK_FUNCTION_CALL_2( 0x967abc, s64, u32, a1, int, a2 );
}

undefined8 FUN_00548bd0( short a1 ){
    SHK_FUNCTION_CALL_1( 0x548bd0, undefined8, short, a1 );
}

u64 GetMoneyCount( void ){
    SHK_FUNCTION_CALL_0( 0x24b340, u64 );
}

void SetMoney( u32 a1 ){
    SHK_FUNCTION_CALL_1( 0x24b350, void, u32, a1 );
}

u64 AddMoneyAndReturn( int a1 ){
    SHK_FUNCTION_CALL_1( 0x24b374, u64, int, a1 );
}

void SET_ITEM_NUM( u32 a1, u32 a2 ){
    SHK_FUNCTION_CALL_2( 0x260340, void, u32, a1, u32, a2 );
}

u16 GET_ITEM_NUM( u32 a1 ){
    SHK_FUNCTION_CALL_1( 0x26028c, u16, u32, a1 );
}

u32 GetMessageScript(void){
    SHK_FUNCTION_CALL_0( 0x1f2954, u32 );
}

void PlayFromSystemACB( undefined8 a1 ){
    SHK_FUNCTION_CALL_1( 0x6d42c, void, undefined8, a1 );
}

void PlayFromSinglewordACB( u64 a1, undefined8 a2 ){
    SHK_FUNCTION_CALL_2( 0x6d0bc, void, u64, a1, undefined8, a2 ); //first arg needs to be 2
}
u8 GetActiveConfidantAmount( u8 a1 ) // gets the amount of active confidants
{
	u8 i = 1;
	u8 count;
	do
	{
		if ( FUN_00548bd0(i) == 1 )
		{
			count += 1;
		}
		i += 1;
	}while(i < a1 + 1); //will count confidant Ids through (inclusive) the Id specified by a1
	return count;
}

int GetRandom( int MaxValue )
{
  int v0; // r31
  u32 v1; // r3

  v0 = MaxValue;
  v1 = FUN_0091da04();
  return (v1 - (v1 / (v0 + 1) * v0 + v1 / (v0 + 1)));
}

void memzero( void* args, int size )
{
    SHK_FUNCTION_CALL_2(0x9006ac, void, void*, args, int, size);
}

bool isMidWinterValid( void )
{
    isMidwinter = false;

    if ( !CONFIG_ENABLED( enableMidWinterOutfits ) )
    {
        return isMidwinter;
    }
    
    if ( GetBitflagState( 2162 ) && GetTotalDays() >= 275 && GetTotalDays() < 334 )
    {
        isMidwinter = true;
        if ( sequenceIDGlobal == 6 )
        {
            fieldworkdataStruct* FWD = GetFieldWorkData();
            if ( FWD != 0 && FWD->FieldID_Major == 2 )
            {
                if ( FWD->FieldID_Minor == 1 || FWD->FieldID_Minor == 4 || FWD->FieldID_Minor == 5 || FWD->FieldID_Minor == 13 )
                {
                    isMidwinter = true;
                }
                else isMidwinter = false;
            }
        }
    }
    else isMidwinter = false;
    
    return isMidwinter;
}

void CALL_NAVI_DIALOGUE( int charID, int expressionID, int msgID, int dialogueBoxType )
{
  if ( FUN_0031f9cc() && (charID = isCharacterAssistExpressonValid( (short)charID, (short)expressionID ), -1 < charID))
  {
    printf("Calling Navi Dialogue; assist ID %d, msgID %d\n", charID, msgID);
    FUN_003b9110( charID, msgID, dialogueBoxType, 0, 0 );
  }
}

static s32 sys_time_get_current_time( u64* secs, u64* nsecs )
{
  system_call_2( 145, (u64)secs, (u64)nsecs );
  return_to_user_prog( s32 );
} 

u64 getTicks(void)
{
    u32 ticks_micro;
    u64 secs;
    u64 nsecs;

    sys_time_get_current_time(&secs, &nsecs);
    ticks_micro =  secs * 1000000UL + (nsecs / 1000);

    return ticks_micro;                                                                         
}

s32 sys_dbg_read_process_memory(s32 pid, u32 process_ea, u32 size, void* data)
{
  system_call_4( 904, (u64)pid, (u64)process_ea, (u64)size, (u32)data );
  return_to_user_prog( int );
}

static s32 pattern[] = { 0x00, 0xBA, 0x69, 0x98, -1, -1, -1, -1, -1, -1, -1, -1, 0x00, 0xBA, 0x23, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void* sigscan( void* start, void* end, s32* pattern, s32 patternLength )
{
  if ( start == NULL ) start = 0x100000; // should be elf start
  if ( end == NULL ) end = (void*)0x40000000 - start; // need to determine sane default

  printf("sigscan starting; range 0x%x to 0x%x\n", start, end);

  for ( void* cur = start; cur < end; ++cur )
  {
    u32 remainder = end - cur;
    
    if ( remainder % 0x100 == 0 )
    {
        //printf("sigscan in progress; cur addr 0x%x; remainder 0x%x\n", cur, remainder);
    }

    if ( remainder < patternLength )
      return NULL; 

    bool found = true;
    for ( u32 i = 0; i < patternLength; ++i )
    {
      if ( pattern[i] == -1 )
      {
        // wildcard
        continue;
      }

      u8 curVal;
      if ( sys_dbg_read_process_memory( 1, cur + i, 1, &curVal ) != CELL_OK )
      {
        // memory unreadable
        printf("Failed to read memory at 0x%x\n", cur + i);
        found = false;
        break;
      }

      if ( curVal != pattern[i] )
      {
        // not a match
        found = false;
        break;
      }
    }

    if ( found )
      return cur;
  }
}

#define UnitList1 (*((UnitList1**)0x1183C90))

btlUnit_Unit* GetBtlUnitInCombat( u32 a1, u32 a2 )
{
    for ( int i = 0; i <= 2; i++ )
    {
        //printf("===============================\n");
        //printf("Unit List #%d Size %d\n", i, UnitList1->field54->field34->UnitList[i].ListSize);
        //printf("-------------------------------\n");
        Node* btlUnitListLocal = UnitList1->field54->field34->UnitList[i].First;
        while ( btlUnitListLocal != 0 )
        {
            //printf("btlUnit at 0x%x\n", btlUnitListLocal->Field14->Field18->pointer_2);
            //printf("Current Node at 0x%x\n", btlUnitListLocal);
            //printf("Previous Node at 0x%x\n", btlUnitListLocal->Field04);
            btlUnit_Unit* localBtlUnit = btlUnitListLocal->Field14->Field18->pointer_2;
            if ( localBtlUnit->unitType == a1 && localBtlUnit->unitID == a2 )
            {
                return localBtlUnit;
            }
            btlUnitListLocal = btlUnitListLocal->next;
        }
        //printf("===============================\n");
    }
    return 0;
}

int GetTotalUnitsOfType( u32 TargetUnitType )
{
    u32 result = 0;
    for ( int i = 0; i <= 2; i++ )
    {
        //printf("===============================\n");
        //printf("Unit List #%d Size %d\n", i, UnitList1->field54->field34->UnitList[i].ListSize);
        //printf("-------------------------------\n");
        Node* btlUnitListLocal = UnitList1->field54->field34->UnitList[i].First;
        while ( btlUnitListLocal != 0 )
        {
            //printf("btlUnit at 0x%x\n", btlUnitListLocal->Field14->Field18->pointer_2);
            //printf("Current Node at 0x%x\n", btlUnitListLocal);
            //printf("Previous Node at 0x%x\n", btlUnitListLocal->Field04);
            btlUnit_Unit* localBtlUnit = btlUnitListLocal->Field14->Field18->pointer_2;
            if ( localBtlUnit->unitType == TargetUnitType && !IsBtlUnitDead( localBtlUnit, 0 ) )
            {
                result += 1;
            }
            btlUnitListLocal = btlUnitListLocal->next;
        }
        //printf("===============================\n");
    }
    return result;
}

bool isPlayerUnitDead( u32 unitID )
{
    bool result = false;
    
    btlUnit_Unit* PlayerUnit = GetBtlPlayerUnitFromID(unitID);

    if ( btlUnitHasAilment( PlayerUnit, 0x80000 ) ) // check for Dead ailment
    {
        result = true;
    }
    else if ( PlayerUnit->currentHP == 0 )
    {
        result = true;
    }

    return result;
}

bool isBtlUnitPlayerUnitDead( btlUnit_Unit* PlayerUnit )
{
    bool result = false;
    
    if ( btlUnitHasAilment( PlayerUnit, 0x80000 ) ) // check for Dead ailment
    {
        result = true;
    }
    else if ( PlayerUnit->currentHP == 0 )
    {
        result = true;
    }

    return result;
}

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}

rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}

float P5_SIN( float in )
{
    SHK_FUNCTION_CALL_1( 0xac9990, float, float, in );
}

float ckit_sin( float in )
{
    return P5_SIN( in * 0.01745329238474369 );
}

void WriteKasumiData( void )
{
    btlUnit_Unit* Futaba = GetBtlPlayerUnitFromID( 8 );
    btlUnit_Unit* Kasumi = GetBtlPlayerUnitFromID( 10 );

    DEBUG_LOG("Writing Kasumi Data to save\n");

    Futaba->StockPersona[1].personaID = Kasumi->Field06;
    Futaba->StockPersona[1]._x6 = Kasumi->Field1A;
    Futaba->StockPersona[1].personaExp = Kasumi->currentHP;
    Futaba->StockPersona[1]._x2C = Kasumi->currentSP;
    Futaba->StockPersona[1].SkillID[0] = Kasumi->Field42;
    Futaba->StockPersona[1].SkillID[1] = Kasumi->meleeID;
    Futaba->StockPersona[1].SkillID[2] = Kasumi->protectorID;
    Futaba->StockPersona[1].SkillID[3] = Kasumi->accessoryID;
    Futaba->StockPersona[1].SkillID[4] = Kasumi->outfitID;
    Futaba->StockPersona[1].SkillID[5] = Kasumi->rangedWeaponID;

    Futaba->StockPersona[2].SkillID[0] = Kasumi->TacticsState;
    Futaba->StockPersona[2].SkillID[1] = Kasumi->Field29A;
    Futaba->StockPersona[2].SkillID[2] = Kasumi->HPGainNextLv;
    Futaba->StockPersona[2].SkillID[3] = Kasumi->SPGainNextLv;
    Futaba->StockPersona[2].personaExp = Kasumi->Field28E;
    Futaba->StockPersona[2]._x2C = Kasumi->Field296;

    Futaba->StockPersona[3].Flags = Kasumi->StockPersona[0].Flags;
    Futaba->StockPersona[3].isUnlocked = Kasumi->StockPersona[0].isUnlocked;
    Futaba->StockPersona[3].personaID = Kasumi->StockPersona[0].personaID;
    Futaba->StockPersona[3].personaLv = Kasumi->StockPersona[0].personaLv;
    Futaba->StockPersona[3]._x5 = Kasumi->StockPersona[0]._x5;
    Futaba->StockPersona[3]._x6 = Kasumi->StockPersona[0]._x6;
    Futaba->StockPersona[3].personaExp = Kasumi->StockPersona[0].personaExp;
    Futaba->StockPersona[3].SkillID[0] = Kasumi->StockPersona[0].SkillID[0];
    Futaba->StockPersona[3].SkillID[1] = Kasumi->StockPersona[0].SkillID[1];
    Futaba->StockPersona[3].SkillID[2] = Kasumi->StockPersona[0].SkillID[2];
    Futaba->StockPersona[3].SkillID[3] = Kasumi->StockPersona[0].SkillID[3];
    Futaba->StockPersona[3].SkillID[4] = Kasumi->StockPersona[0].SkillID[4];
    Futaba->StockPersona[3].SkillID[5] = Kasumi->StockPersona[0].SkillID[5];
    Futaba->StockPersona[3].SkillID[6] = Kasumi->StockPersona[0].SkillID[6];
    Futaba->StockPersona[3].SkillID[7] = Kasumi->StockPersona[0].SkillID[7];
    Futaba->StockPersona[3].Stats[0] = Kasumi->StockPersona[0].Stats[0];
    Futaba->StockPersona[3].Stats[1] = Kasumi->StockPersona[0].Stats[1];
    Futaba->StockPersona[3].Stats[2] = Kasumi->StockPersona[0].Stats[2];
    Futaba->StockPersona[3].Stats[3] = Kasumi->StockPersona[0].Stats[3];
    Futaba->StockPersona[3].Stats[4] = Kasumi->StockPersona[0].Stats[4];
    Futaba->StockPersona[3].StatsEx[0] = Kasumi->StockPersona[0].StatsEx[0];
    Futaba->StockPersona[3].StatsEx[1] = Kasumi->StockPersona[0].StatsEx[1];
    Futaba->StockPersona[3].StatsEx[2] = Kasumi->StockPersona[0].StatsEx[2];
    Futaba->StockPersona[3].StatsEx[3] = Kasumi->StockPersona[0].StatsEx[3];
    Futaba->StockPersona[3].StatsEx[4] = Kasumi->StockPersona[0].StatsEx[4];
    Futaba->StockPersona[3].StatsExTemp[0] = Kasumi->StockPersona[0].StatsExTemp[0];
    Futaba->StockPersona[3].StatsExTemp[1] = Kasumi->StockPersona[0].StatsExTemp[1];
    Futaba->StockPersona[3].StatsExTemp[2] = Kasumi->StockPersona[0].StatsExTemp[2];
    Futaba->StockPersona[3].StatsExTemp[3] = Kasumi->StockPersona[0].StatsExTemp[3];
    Futaba->StockPersona[3].StatsExTemp[4] = Kasumi->StockPersona[0].StatsExTemp[4];
    Futaba->StockPersona[3]._x2B = Kasumi->StockPersona[0]._x2B;
    Futaba->StockPersona[3]._x2C = Kasumi->StockPersona[0]._x2C;
}

void ReadKasumiData( void )
{
    btlUnit_Unit* Futaba = GetBtlPlayerUnitFromID( 8 );
    btlUnit_Unit* Kasumi = GetBtlPlayerUnitFromID( 10 );

    DEBUG_LOG("Loading Kasumi Data from save\n");

    if ( Futaba->StockPersona[3].personaID == 0 )
    {
        return;
    }

    Kasumi->Field06 = Futaba->StockPersona[1].personaID;
    Kasumi->Field1A = Futaba->StockPersona[1]._x6;
    Kasumi->Field42 = Futaba->StockPersona[1].SkillID[0];
    Kasumi->meleeID = Futaba->StockPersona[1].SkillID[1];
    Kasumi->protectorID = Futaba->StockPersona[1].SkillID[2];
    Kasumi->accessoryID = Futaba->StockPersona[1].SkillID[3];
    Kasumi->outfitID = Futaba->StockPersona[1].SkillID[4];
    Kasumi->rangedWeaponID = Futaba->StockPersona[1].SkillID[5];

    Kasumi->TacticsState = Futaba->StockPersona[2].SkillID[0];
    Kasumi->Field29A = Futaba->StockPersona[2].SkillID[1];
    Kasumi->HPGainNextLv = Futaba->StockPersona[2].SkillID[2];
    Kasumi->SPGainNextLv = Futaba->StockPersona[2].SkillID[3];
    Kasumi->Field28E = Futaba->StockPersona[2].personaExp;
    Kasumi->Field296 = Futaba->StockPersona[2]._x2C;

    Kasumi->StockPersona[0].Flags = Futaba->StockPersona[3].Flags;
    Kasumi->StockPersona[0].isUnlocked = Futaba->StockPersona[3].isUnlocked;
    Kasumi->StockPersona[0].personaID = Futaba->StockPersona[3].personaID;
    Kasumi->StockPersona[0].personaLv = Futaba->StockPersona[3].personaLv;
    Kasumi->StockPersona[0]._x5 = Futaba->StockPersona[3]._x5;
    Kasumi->StockPersona[0]._x6 = Futaba->StockPersona[3]._x6;
    Kasumi->StockPersona[0].personaExp = Futaba->StockPersona[3].personaExp;
    Kasumi->StockPersona[0].SkillID[0] = Futaba->StockPersona[3].SkillID[0];
    Kasumi->StockPersona[0].SkillID[1] = Futaba->StockPersona[3].SkillID[1];
    Kasumi->StockPersona[0].SkillID[2] = Futaba->StockPersona[3].SkillID[2];
    Kasumi->StockPersona[0].SkillID[3] = Futaba->StockPersona[3].SkillID[3];
    Kasumi->StockPersona[0].SkillID[4] = Futaba->StockPersona[3].SkillID[4];
    Kasumi->StockPersona[0].SkillID[5] = Futaba->StockPersona[3].SkillID[5];
    Kasumi->StockPersona[0].SkillID[6] = Futaba->StockPersona[3].SkillID[6];
    Kasumi->StockPersona[0].SkillID[7] = Futaba->StockPersona[3].SkillID[7];
    Kasumi->StockPersona[0].Stats[0] = Futaba->StockPersona[3].Stats[0];
    Kasumi->StockPersona[0].Stats[1] = Futaba->StockPersona[3].Stats[1];
    Kasumi->StockPersona[0].Stats[2] = Futaba->StockPersona[3].Stats[2];
    Kasumi->StockPersona[0].Stats[3] = Futaba->StockPersona[3].Stats[3];
    Kasumi->StockPersona[0].Stats[4] = Futaba->StockPersona[3].Stats[4];
    Kasumi->StockPersona[0].StatsEx[0] = Futaba->StockPersona[3].StatsEx[0];
    Kasumi->StockPersona[0].StatsEx[1] = Futaba->StockPersona[3].StatsEx[1];
    Kasumi->StockPersona[0].StatsEx[2] = Futaba->StockPersona[3].StatsEx[2];
    Kasumi->StockPersona[0].StatsEx[3] = Futaba->StockPersona[3].StatsEx[3];
    Kasumi->StockPersona[0].StatsEx[4] = Futaba->StockPersona[3].StatsEx[4];
    Kasumi->StockPersona[0].StatsExTemp[0] = Futaba->StockPersona[3].StatsExTemp[0];
    Kasumi->StockPersona[0].StatsExTemp[1] = Futaba->StockPersona[3].StatsExTemp[1];
    Kasumi->StockPersona[0].StatsExTemp[2] = Futaba->StockPersona[3].StatsExTemp[2];
    Kasumi->StockPersona[0].StatsExTemp[3] = Futaba->StockPersona[3].StatsExTemp[3];
    Kasumi->StockPersona[0].StatsExTemp[4] = Futaba->StockPersona[3].StatsExTemp[4];
    Kasumi->StockPersona[0]._x2B = Futaba->StockPersona[3]._x2B;
    Kasumi->StockPersona[0]._x2C = Futaba->StockPersona[3]._x2C;
    Kasumi->currentHP = Futaba->StockPersona[1].personaExp;
    Kasumi->currentSP = Futaba->StockPersona[1]._x2C;
}

void setBit(s32 index, bool value) 
{
  if (value) 
  {
    NewBits->Bits[index/8] |= 1 << (index % 8);
  } 
  else 
  {
    NewBits->Bits[index/8] &= ~(1 << (index % 8));
  }
}

bool GetBit( s32 index )
{
  return (NewBits->Bits[index/8] & 1 << (index % 8)) != 0;
}

s32 ReturnConvertedFlag( s32 BitFlag )
{
    int FlagSection = BitFlag/0x10000000;
    
    if (FlagSection == 1)
    {
        BitFlag -= FlagSection * 0x10000000;
        if ( BitFlag > 2048 )
        {
            BitFlag -= 2048; // section size
            BitFlag += 8959; // p5 max
        }
        else BitFlag += 2048;
    }
    else if (FlagSection == 2)
    {
        BitFlag -= FlagSection * 0x10000000;
        if ( BitFlag > 4096 )
        {
            BitFlag -= 4096;  // section size
            BitFlag += 394; // section 1 overflow
            BitFlag += 8959; // p5 max
        }
        else BitFlag += 4096;
    }
    else if (FlagSection == 3)
    {
        BitFlag -= FlagSection * 0x10000000;
        if ( BitFlag > 256 )
        {
            BitFlag -= 256; // section size
            BitFlag += 394; // section 1 overflow
            BitFlag += 1024; // section 2 overflow
            BitFlag += 8959; // p5 max
        }
        else BitFlag += 8192;
    }
    else if (FlagSection == 4)
    {
        BitFlag -= FlagSection * 0x10000000;
        if ( BitFlag > 256 )
        {
            BitFlag -= 256; // section size
            BitFlag += 394; // section 1 overflow
            BitFlag += 1024; // section 2 overflow
            BitFlag += 256; // section 3 overflow
            BitFlag += 8959; // p5 max
        }
        else BitFlag += 8448;
    }
    else if (FlagSection == 5)
    {
        BitFlag -= FlagSection * 0x10000000;
        if ( BitFlag > 256 )
        {
            BitFlag -= 256; // section size
            BitFlag += 394; // section 1 overflow
            BitFlag += 1024; // section 2 overflow
            BitFlag += 256; // section 3 overflow
            BitFlag += 256; // section 4 overflow
            BitFlag += 8959; // p5 max
        }
        else BitFlag += 8704;
    }

    return BitFlag;
}

UnkHelperStruct* FUN_000af304( int a1, u16 a2, int a3, u16 a4, u16 a5, u16 a6, u16 a7 )
{ 
    SHK_FUNCTION_CALL_7( 0xaf304, UnkHelperStruct*, int, a1, u16, a2, int, a3, u16, a4, u16, a5, u16, a6, u16, a7 ); 
}

u64 FUN_0044f6a8( s16 a1, int a2, int a3 ) {
    SHK_FUNCTION_CALL_3( 0x44f6a8, u64, s16, a1, int, a2, int, a3 );
}
void FUN_0042f5c4( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x42f5c4, void, u32, a1 );
}

u64 FUN_0025d624( u32 a1, short a2 ) {
    SHK_FUNCTION_CALL_2( 0x25d624, u64, u32, a1, short, a2 );
}

int FUN_0042f344( u32 a1, u32 a2 ) {
    SHK_FUNCTION_CALL_2( 0x42f344, int, u32, a1, u32, a2 );
}

u64 FUN_004260c4() {
    SHK_FUNCTION_CALL_0( 0x4260c4, u64 );
}

u64 FUN_0042f5ec( u32 a1, u32 a2, u32 a3, u32 a4 ) {
    SHK_FUNCTION_CALL_4( 0x42f5ec, u64, u32, a1, u32, a2, u32, a3, u32, a4 );
}

u64 FUN_0042f3ec( u32 a1, u32 a2, u32 a3, u32 a4 ) {
    SHK_FUNCTION_CALL_4( 0x42f3ec, u64, u32, a1, u32, a2, u32, a3, u32, a4 );
}

u64 FUN_0042f994( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x42f994, u64, u32, a1 );
}

void FUN_00116ad0( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x116ad0, void, u32, a1 );
}

void FUN_0044f448( int a1, u32 a2 ) {
    SHK_FUNCTION_CALL_2( 0x44f448, void, int, a1, u32, a2 );
}

u64 FUN_004495c0( int a1, int a2 ) {
    SHK_FUNCTION_CALL_2( 0x4495c0, u64, int, a1, int, a2 );
}

void FUN_002604cc( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x2604cc, void, u32, a1 );
}

void FUN_00260780( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x260780, void, u32, a1 );
}

u64 FUN_00261400( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x261400, u64, u32, a1 );
}

void FUN_0042f24c( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x42f24c, void, u32, a1 );
}

void FUN_00260894( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x260894, void, u32, a1 );
}

u64 FUN_00296394( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x296394, u64, u32, a1 );
}

void FUN_0026090c( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x26090c, void, u32, a1 );
}

u64 FUN_002604bc( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x2604bc, u64, u32, a1 );
}

void FUN_0040f370( int a1, short a2 ) {
    SHK_FUNCTION_CALL_2( 0x40f370, void, int, a1, short, a2 );
}

void FUN_0026028c( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x26028c, void, u32, a1 );
}

void FUN_004873c4( int a1, int a2 ) {
    SHK_FUNCTION_CALL_2( 0x4873c4, void, int, a1, int, a2 );
}

void FUN_00480a20( int a1 ) {
    SHK_FUNCTION_CALL_1( 0x480a20, void, int, a1 );
}

void FUN_0029793c( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x29793c, void, u32, a1 );
}

void FUN_00480a28( int a1 ) {
    SHK_FUNCTION_CALL_1( 0x480a28, void, int, a1 );
}

void FUN_004e35e0( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x4e35e0, void, u32, a1 );
}

void FUN_004e3610( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x4e3610, void, u32, a1 );
}

void FUN_004e3640( u32 a1 ) {
    SHK_FUNCTION_CALL_1( 0x4e3640, void, u32, a1 );
}

#endif