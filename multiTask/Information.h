/*************************************************************************
                        Information  -  description
                             -------------------
    date                 : Feb. 25 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//------ Interface of the <Information> module (file Information.h) ------
#if !defined ( INFORMATION_H )
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
#include <sys/sem.h> // for struct sembuf

#include <time.h> // for time()

#include "Outils.h"

//-------------------------------------------------------------- Constants
extern int const RIGHTS; // 0666
extern char const PROGRAM_NAME[]; // ./Parking
extern char const FTOK_CHAR; // '3'

extern unsigned int NB_CHAR_STATE;

// Represents a waiting car in front of an entrance
struct WaitingCar
{
	TypeUsager userType;
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


extern size_t const SHM_SIZE; // sizeof( struct ParkingLot )

extern struct sembuf const MUTEX_ACCESS; // Mutex for the shared memory
extern struct sembuf const MUTEX_FREE;
extern int const MUTEX_NB;
extern int const MUTEX_OK;
extern int const MUTEX_KO;

struct EnterCommand
// Sent from keyboard to the appropriate door
{
	long doorType; // Should always be an entrance door
	enum TypeUsager userType; // Should never be AUCUN
};

struct ExitCommand
// Sent from keyboard to the appropriate door
{
	long doorType; // Should always be an exit door
	unsigned int position; // Position in the parking lot
};

extern int const ENTER_CMD_SIZE; // sizeof( enum TypeUsager )
extern int const EXIT_CMD_SIZE; // sizeof( unsigned int )

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions

struct sembuf SemP ( short unsigned int noSem );
// How to use:
//     Returns the correct sembuf structure for a P operation

struct sembuf SemV ( short unsigned int noSem );
// How to use:
//     Returns the correct sembuf structure for a V operation

// type Name ( parameter list );
// How to use:
//
// Contract:
//

#endif // INFORMATION_H
