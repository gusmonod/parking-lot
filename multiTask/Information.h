/*************************************************************************
                        Information  -  description
                             -------------------
    date                 : Feb. 25 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//------ Interface of the <Information> module (file Information.h) ------
#if ! defined ( INFORMATION_H )
#define INFORMATION_H

//------------------------------------------------------------------------
// Role of the <Information> module
//
//    This module contains all of the necessary information to share
//    between all of the different tasks.
//
//    This includes:
//      · The shared memory "ParkingLot"
//------------------------------------------------------------------------

//-------------------------------------------------------- Used interfaces
#include <sys/types.h>

//-------------------------------------------------------------- Constants
extern int  const RIGHTS; // 0666
extern char const PROGRAM_NAME[]; // ./Parking
extern char const FTOK_CHAR; // '3'

// Used for the shared memory:
struct m
{
	pid_t pid;
	char c;
};

extern int  const SHM_SIZE; // sizeof(struct m)

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
// type Name ( parameter list );
// How to use:
//
// Contract:
//

#endif // INFORMATION_H
