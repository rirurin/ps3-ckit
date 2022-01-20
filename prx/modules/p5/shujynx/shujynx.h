#ifdef GAME_P5
#ifndef SHUJYNX_H
#define SHUJYNX_H

void ShujynxInit( void );
void ShujynxShutdown( void );


// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

#include "modules/p5/p5.h"

typedef struct
{
    int flags;
    short Field_02;
    short Field_04;
    short EnemyID[5];
    short Field_MajorID;
    short Field_MinorID;
    short MusicID;
} EncounterStruct;

#endif
#endif