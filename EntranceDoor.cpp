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
#include <cstdio> // for perror

#include <unistd.h> // for exit
#include <signal.h> // for sigaction
#include <errno.h>  // for EINTR

#include <map> // for child processes management

using namespace std;

#include <sys/ipc.h> // for all IPCS
#include <sys/shm.h> // for shmget
#include <sys/sem.h> // for semget
#include <sys/msg.h> // for msgget

#include <sys/wait.h>

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"

#include "Information.h"

#include "EntranceDoor.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables
static const unsigned int ENTRANCE_TEMPO = 1;

static TypeBarriere doorType;

//IPC
static struct ParkingLot *shmParkingLot;
static int shmMutexId;

static int mbCommandId;

static struct sembuf mutexAccess = MUTEX_ACCESS;
static struct sembuf mutexFree = MUTEX_FREE;

static int waitSemSetId;

static map<pid_t, TypeUsager> childrenPid;

static int const TEMPO = 0;

//------------------------------------------------------ Private functions

//------------------------------------------------------------- Init phase
static int init ( TypeBarriere type );
// How to use:
// Initialization process of the <EntranceDoor> task

//---------------------------------------------------------- Destroy phase
static void destroy ( );
// How to use:
// Destruction phase of the <EntranceDoor> task

static void endTask ( int signal );
// How to use:
// Handles the SIGUSR2 signal, by destroying the task and its children

//------------------------------------------------------------ Motor phase
static void carParked ( int signal );
// How to use:
// Handles the SIGCHLD signal, received when a car is parked

static void savePark ( unsigned int noParkingSpot, TypeUsager userType );
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
	sigemptyset( &action.sa_mask );
	action.sa_flags = 0;

	action.sa_handler = endTask;
	sigaction( SIGUSR2, &action, NULL );

	action.sa_handler = carParked;
	sigaction( SIGCHLD, &action, NULL );

	// Getting the command mailbox
	mbCommandId = msgget( ftok( PROGRAM_NAME, FTOK_CHAR ), RIGHTS );

	// Getting the shared memory
	// (characteristics can be found in Information module)
	int shmId = shmget( ftok( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE, RIGHTS );

	// Attaching shared memory
	shmParkingLot = (struct ParkingLot *) shmat( shmId, NULL, 0 );

	// Getting the shared memory mutex
	shmMutexId = semget( ftok( PROGRAM_NAME, FTOK_CHAR ), MUTEX_NB, RIGHTS );

	// Getting the semaphore set for the entrance doors to wait
	waitSemSetId = semget( ftok( PROGRAM_NAME, FTOK_CHAR + 1 ),
			NB_BARRIERES_ENTREE, RIGHTS );

	return shmId;
} //----- End of init

static void destroy ( )
// Algorithm:
// Detaches the shared memory
{
	// Detaching the shared memory
	shmdt( shmParkingLot );
	shmParkingLot = NULL;

	_exit( EXIT_SUCCESS );
} //----- End of destroy

static void endTask ( int signal )
// Algorithm:
//
{
	if ( SIGUSR2 == signal )
	{
		map<pid_t, TypeUsager>::iterator it;

		for ( it = childrenPid.begin( ); it != childrenPid.end( ); ++it )
		{
			//Killing every child pid
			kill( it->first, SIGUSR2 );
			waitpid( it->first, NULL, 0 );
		}

		destroy( );
	}
} //----- End of endTask

static void carParked ( int signal )
// Algorithm:
//
{
	if ( SIGCHLD == signal )
	{
		pid_t child;
		int status;
		child = waitpid( -1, &status, WNOHANG );

		if ( child > 0 && WIFEXITED( status ) )
		{
			savePark( WEXITSTATUS( status ), childrenPid[child] );

			// Removes the pid from the "to be deleted" pid map
			childrenPid.erase( child );
		}
	}
}

static void savePark ( unsigned int noParkingSpot, TypeUsager userType )
// Contract:
//
// Algorithm:
//
{
	int status;
	struct ParkedCar *pParkedCar =
			&( shmParkingLot->parkedCars[noParkingSpot - 1] );

	/* BEGIN shared memory exclusion */
	do
	{
		status = semop( shmMutexId, &mutexAccess, 1 );
	} while ( -1 == status && EINTR == errno );
		pParkedCar->userType = userType; // parameter
		pParkedCar->carNumber = ( shmParkingLot->nextCarNo )++;
		pParkedCar->parkedSince = time( NULL );
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */

	AfficherPlace( noParkingSpot, pParkedCar->userType,
			pParkedCar->carNumber, pParkedCar->parkedSince );
} //----- End of savePark

static int waitForEmptySpot ( )
{
	struct sembuf P = SemP( doorType - 1 );
	// Waiting for a car to exit
	return semop( waitSemSetId, &P, 1 );
} //----- End of waitForEmptySpot

static void request ( TypeUsager userType, time_t timeOfRequest )
{
	int status;
	struct WaitingCar *pWaitingCar =
			&( shmParkingLot->waitingCars[doorType - 1] );

	/* BEGIN shared memory exclusion */
	do
	{
		status = semop( shmMutexId, &mutexAccess, 1 );
	} while ( -1 == status && EINTR == errno );
		pWaitingCar->userType = userType;
		pWaitingCar->arrivalTime = timeOfRequest;
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */

	AfficherRequete( doorType, userType, timeOfRequest );
} //----- End of request

static void clearRequest ( )
{
	struct WaitingCar *pWaitingCar =
			&( shmParkingLot->waitingCars[doorType - 1] );

	/* BEGIN shared memory exclusion */
	semop( shmMutexId, &mutexAccess, 1 );
		pWaitingCar->userType = AUCUN;
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */

	Effacer( ( TypeZone )( REQUETE_R1 + doorType - 1 ) );
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

	for ( ; ; )
	{
		pid_t childPid;
		struct EnterCommand command;
		int status;

		do
		{
			status = msgrcv( mbCommandId, &command, ENTER_CMD_SIZE,
					doorType, 0 );
		} while ( -1 == status && EINTR == errno );

		DessinerVoitureBarriere( doorType, command.userType );

		if ( shmParkingLot->fullSpots == NB_PLACES )
		{
			request( command.userType, time( NULL ) );

			// Waiting for a car to exit
			do
			{
				status = waitForEmptySpot( );
			} while ( -1 == status && EINTR == errno );

			clearRequest( );
		}

		/* BEGIN shared memory exclusion */
		do
		{
			status = semop( shmMutexId, &mutexAccess, 1 );
		} while ( -1 == status && EINTR == errno );

			// Parking a car
			if ( -1 == ( childPid = GarerVoiture( type ) ) )
			{
				perror( "Error trying to park a car" );
				_exit( EXIT_FAILURE );
			}

			++( shmParkingLot->fullSpots );

		semop( shmMutexId, &mutexFree, 1 );
		/* END   shared memory exclusion */

		childrenPid.insert( make_pair( childPid, command.userType ) );

		sleep( ENTRANCE_TEMPO ); // So as to avoid entrance collision
	}

	perror( "Error: exited the EntranceDoor loop" );
	_exit( EXIT_FAILURE );
} //----- End of EntranceDoor

