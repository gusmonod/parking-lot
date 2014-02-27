/*************************************************************************
   TestPartage  -  The mother task creating the context and launching the app
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---------- Realization of the <TestPartage> task (file TestPartage.cpp) ----------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------------- System include
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>

//------------------------------------------------------- Personal include
#include "Information.h"
#include "Display.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables
static int  shmId;
static struct ParkingLot *shmParkingLot;

//------------------------------------------------------ Private functions
static void init    ( );
// How to use:

static void destroy ( );
// How to use:

static void handle  ( int signal );

static void init ( )
// Algorithm:
// Initializes app, and creates the IPC objects used to communicate.
{
	struct sigaction action;
	action.sa_handler = handle;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR2, &action, NULL);
	sigaction(SIGINT,  &action, NULL);
	// Getting the shared memory
	// (characteristics can be found in Information module)
	std::cout << "name: " << PROGRAM_NAME << " ftok: " << FTOK_CHAR << std::endl;
	std::cout << "key: " << ftok( PROGRAM_NAME, FTOK_CHAR ) << std::endl;
	shmId = shmget( ftok( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE, RIGHTS );
	std::cout << "shmId: " << shmId << std::endl;

	// Attaching the shared memory
	shmParkingLot = (struct ParkingLot *) shmat( shmId, NULL, 0 );
}

static void destroy ( )
// Algorithm:
// Deletes the IPC objects used to communicate and destroys the app.
{
	// Detaches the shared memory
	shmdt( shmParkingLot );
	shmParkingLot = NULL;
	exit( 0 );
}

static void handle ( int signal )
{
	if ( SIGUSR2 == signal || SIGINT == signal )
	{
		destroy( );
	}
}

static void printEntranceDoor( TypeBarriere type )
{
	switch ( type )
	{
		case PROF_BLAISE_PASCAL:
			std::cout << "P BP";
			break;
		case AUTRE_BLAISE_PASCAL:
			std::cout << "A BP";
			break;
		case ENTREE_GASTON_BERGER:
			std::cout << "E GB";
			break;
		default:
			perror( "Error entrance door print" );
	}
}

static void printUserType( TypeUsager userType )
{
	switch ( userType )
	{
		case AUCUN:
			std::cout << "Vide ";
		case PROF:
			std::cout << "Prof ";
			break;
		case AUTRE:
			std::cout << "Autre";
			break;
	}
}

static void printWaitingCar( struct WaitingCar * pCar )
{
	std::cout << "User: ";
	printUserType( pCar->userType );
	if ( AUCUN != pCar->userType )
	{
		std::cout << " Door: ";
		printEntranceDoor( pCar->entranceType );
		std::cout << " Arrived at: " << pCar->arrivalTime;
	}
}

static void printParkingLot( struct ParkingLot * pParkingLot )
{
	std::cout << "Size: " << NB_PLACES << " Available: "
	<< pParkingLot->fullSpots << std::endl;
	for ( unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i )
	{
		printWaitingCar( &( pParkingLot->waitingCars[i] ) );
		std::cout << std::endl;
	}

	if ( 0 == pParkingLot->fullSpots )
	{
		std::cout << "Empty" << std::endl;
	}
	else
	{
		for ( unsigned int i = 0; i < NB_PLACES; ++i )
		{
			if ( AUCUN != ( pParkingLot->parkedCars[i] ).userType )
			{
				struct ParkedCar *pCar = &( pParkingLot->parkedCars[i] );
				std::cout << "Spot #" << i << ": ";
				printUserType( pCar->userType );
				std::cout << " #" << pCar->carNumber
				<< "Parked since: " << pCar->parkedSince << std::endl;
			}
		}
	}
}

//static type name ( parameter list )
// How to use:
//
// Contract:
//
// Algorithm:
//
//{
//} //----- End of name

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
// type Name ( parameter list )
// Algorithm:
//
//{
//} //----- End of Name

int main ( )
{
	init( );
	for (;;)
	{
		printParkingLot( shmParkingLot );
		sleep( 1 );
	}
}
