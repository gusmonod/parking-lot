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
#include <sys/sem.h>

#include <time.h>

#include "Outils.h"

//-------------------------------------------------------------- Constants
extern int  const RIGHTS; // 0666
extern char const PROGRAM_NAME[]; // ./Parking
extern char const FTOK_CHAR; // '3'

extern unsigned int NB_CHAR_STATE;

// Represents a waiting car in front of an entrance
struct WaitingCar
{
	TypeUsager userType;
	TypeBarriere entranceType;
	time_t arrivalTime;
};

// Represents a car in the parking
struct ParkedCar
{
	TypeUsager userType;
	unsigned int carNumber;
	time_t parkedSince;
};

// Used for the shared memory:
struct ParkingLot
{
	unsigned int fullSpots;
	unsigned int nextCarNo;
	struct WaitingCar waitingCars[NB_BARRIERES_ENTREE];
	struct ParkedCar parkedCars[NB_PLACES];
};


extern int  const SHM_SIZE; // sizeof(struct ParkingLot)

extern struct sembuf const MUTEX_ACCESS; // Mutex for the shared memory
extern struct sembuf const MUTEX_FREE;
extern int  const MUTEX_NB;
extern int  const MUTEX_OK;
extern int  const MUTEX_KO;


//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
// type Name ( parameter list );
// How to use:
//
// Contract:
//

#endif // INFORMATION_H
