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

// Used for the shared memory:
struct mParking
{
	pid_t pid;
	char c;
	short unsigned int NB_PLACES_MAX;
	short unsigned int NbPlacesDisponibles;
	waitingCar WaitingCars[3];
	parkedCar ParkedCars[8];
};

// Represents a waiting car in front of an entrance
struct waitingCar
{
	TypeBarriere entranceType;
	time_t arrivalTime;
}

// Represents a car in the parking
struct parkedCar
{
	TypeUsager userType;
	short unsigned int carNumber;
	time_t parkedSince;
}

extern int  const SHM_SIZE; // sizeof(struct m)

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
