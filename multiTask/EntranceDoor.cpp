/*************************************************************************
                        EntranceDoor  -  description
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---- Realization of the <EntranceDoor> task (file EntranceDoor.cpp) ----

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------------- System include
#include <cstdio> // for sprintf

#include <unistd.h> // for exit
#include <signal.h> // for sigaction
#include <errno.h>  // for EINTR

#include <sys/ipc.h> // for all IPCS
#include <sys/shm.h> // for shmget
#include <sys/sem.h> // for semget
#include <sys/msg.h> // for msgget

#include <sys/wait.h> // for waitpid

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"
#include "Menu.h"

#include "Information.h"
#include "Keyboard.h"

#include "EntranceDoor.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables

static TypeBarriere doorType;

//IPC
static struct ParkingLot *shmParkingLot;
static int shmMutexId;

static int mbCommandId;

static struct sembuf mutexAccess = MUTEX_ACCESS;
static struct sembuf mutexFree  = MUTEX_FREE;

static int waitSemSetId;

//------------------------------------------------------ Private functions

//------------------------------------------------------------- Init phase
static int  init      ( TypeBarriere type );
// How to use:
// Initialization process of the <EntranceDoor> task

//---------------------------------------------------------- Destroy phase
static void destroy   ( );
// How to use:
// Destruction phase of the <EntranceDoor> task

static void endTask   ( int signal );
// How to use:
// Handles the SIGUSR2 signal, by destroying the task and its children

//------------------------------------------------------------ Motor phase
static void carParked ( int signal );
// How to use:
// Handles the SIGCHLD signal, received when a car is parked

static void savePark  ( unsigned int noParkingSpot );
// How to use:
// Saves a car into the parking
//   · Safe (mutex) write in the shared memory of the newly parked car
//   · Displays all of its information in the right spot

//static type name ( parameter list )
// How to use:
//
// Contract:
//
// Algorithm:
//
//{
//} //----- End of name

static int init ( TypeBarriere type )
// Algorithm:
// Sets the signal handler, gets the command mailbox, attaches the shared
// memory to the shmParkingLot pointer, and returns its id
{
	doorType = type;

	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	action.sa_handler = endTask;
	sigaction(SIGUSR2, &action, NULL);

	action.sa_handler = carParked;
	sigaction(SIGCHLD, &action, NULL);

	// Getting the command mailbox
	mbCommandId = msgget( ftok( PROGRAM_NAME, FTOK_CHAR ), RIGHTS );

	// Getting the shared memory
	// (characteristics can be found in Information module)
	int shmId = shmget( ftok( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE, RIGHTS );

	// Attaching shared memory
	shmParkingLot = (struct ParkingLot *) shmat( shmId, NULL, 0 );

	// Getting the shared memory mutex
	shmMutexId = semget( ftok( PROGRAM_NAME, FTOK_CHAR), MUTEX_NB, RIGHTS);

	// Getting the semaphore set for the entrance doors to wait
	waitSemSetId = semget( ftok( PROGRAM_NAME, FTOK_CHAR + 1 ),
					NB_BARRIERES_ENTREE, RIGHTS );

	return shmId;
} //----- End of init

static void destroy ( )
// Algorithm:
// Detaches the shared memory
{
	// Waiting for all of the children
	while ( -1 != waitpid( -1, NULL, 0 ) );

	// Detaching the shared memory
	shmdt( shmParkingLot );
	shmParkingLot = NULL;

	exit( 0 );
} //----- End of destroy

static void endTask ( int signal )
// Algorithm:
//
{
	if ( SIGUSR2 == signal )
	{
		destroy( );
	}
} //----- End of endTask

static void carParked ( int signal )
// Algorithm:
//
{
	if ( SIGCHLD == signal )
	{
		pid_t p;
		int status;
		p = waitpid( -1, &status, WNOHANG );
		if ( p > 0 &&  WIFEXITED( status ) )
		{
			savePark( WEXITSTATUS( status ) );
		}
	}
}

static char userTypeCh ( TypeUsager userType )
{
	switch ( userType )
	{
		case PROF:
			return 'P';
		case AUTRE:
			return 'A';
		case AUCUN:
			return ' ';
	}
	return -1;
}

static void savePark ( unsigned int noParkingSpot )
// Contract:
//
// Algorithm:
//
{
	char msg[1024];
	sprintf(msg, "%d: Une voiture est garee au %d       ",
			getpid(), noParkingSpot );
	Afficher( MESSAGE, msg );

	// TODO Get waiting car from map
	struct WaitingCar * pWaitingCar =
			&( shmParkingLot->waitingCars[AUCUNE + doorType] );

	struct ParkedCar * pParkedCar =
			&( shmParkingLot->parkedCars[noParkingSpot - 1] );

	/* BEGIN shared memory exclusion */
	semop( shmMutexId, &mutexAccess, 1 );
		pParkedCar->userType = pWaitingCar->userType;
		pParkedCar->carNumber = ( shmParkingLot->nextCarNo )++;
		pParkedCar->parkedSince = time( NULL );

		pWaitingCar->userType = AUCUN;
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */

	AfficherPlace( noParkingSpot, pParkedCar->userType,
				   pParkedCar->carNumber, pParkedCar->parkedSince );
} //----- End of savePark

static void waitForEmptySpot ( )
{
	int status;
	struct sembuf P = SemP( doorType - 1 );
	// Waiting for a car to exit
	status = semop( waitSemSetId, &P, 1 );

	if ( -1 == status && EINTR == errno )
	{
		waitForEmptySpot( );
	}
} //----- End of waitForEmptySpot

static void request ( TypeUsager userType, time_t timeOfRequest )
{
	/*
	struct WaitingCar * pWaitingCar =
			&( shmParkingLot->waitingCars[AUCUNE + doorType] );
	int carNb;

	/* BEGIN shared memory exclusion *
	semop( shmMutexId, &mutexAccess, 1 );
		pParkedCar->userType = pWaitingCar->userType;
		pParkedCar->carNumber = ( shmParkingLot->nextCarNo )++;
		pParkedCar->parkedSince = time( NULL );

		pWaitingCar->userType = AUCUN;
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */
	AfficherRequete( doorType, userType, timeOfRequest );
} //----- End of request

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
// type Name ( parameter list )
// Algorithm:
//
//{
//} //----- End of Name

void EntranceDoor ( TypeBarriere type )
// Algorithm:
//
{
	init( type );

	for (;;)
	{
		struct EnterCommand command;
		int status;
		status = msgrcv( mbCommandId, &command, ENTER_CMD_SIZE,
						 doorType , 0 );
		if ( -1 == status && EINTR == errno )
		{
			continue; // starts waiting again without parking anyone
		}
		DessinerVoitureBarriere( doorType, command.userType );

		char msg[1024];
		sprintf(msg, "EntranceDoor: je dois faire rentrer un %c",
				userTypeCh( command.userType ) );
		Afficher( MESSAGE, msg );

		// TODO put <pid_t, TypeUsager> into map
		struct WaitingCar * pWaitingCar =
				&( shmParkingLot->waitingCars[AUCUNE + doorType] );
		// TODO following line BAD
		pWaitingCar->userType = command.userType;

		if ( shmParkingLot->fullSpots == NB_PLACES )
		// The parking is full
		{
			request( command.userType, time( NULL ) );
			waitForEmptySpot( );
		}
		// Parking a car
		if ( -1 == GarerVoiture( doorType ) )
		{
			perror( "Error while trying to park a car" );
			destroy( );
		}
		sleep( 1 ); // To avoid collision during the drawing
	}

		destroy( );
} //----- End of EntranceDoor

