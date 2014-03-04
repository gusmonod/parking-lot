/*************************************************************************
                          ExitDoor  -  description
                             -------------------
    date                 : Mar. 04 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---- Realization of the <ExitDoor> task (file ExitDoor.cpp) ----

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

#include "ExitDoor.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables

//IPC
static struct ParkingLot *shmParkingLot;
static int shmMutexId;

static int mbCommandId;

static struct sembuf mutexAccess = MUTEX_ACCESS;
static struct sembuf mutexFree  = MUTEX_FREE;

static int waitSemSetId;

//------------------------------------------------------ Private functions

//------------------------------------------------------------- Init phase
static int  init      ( );
// How to use:
// Initialization process of the <ExitDoor> task

//---------------------------------------------------------- Destroy phase
static void destroy   ( );
// How to use:
// Destruction phase of the <ExitDoor> task

static void endTask   ( int signal );
// How to use:
// Handles the SIGUSR2 signal, by destroying the task and its children

//------------------------------------------------------------ Motor phase
static void carExited ( int signal );
// How to use:
// Handles the SIGCHLD signal, received when a car is parked

static void saveExit  ( unsigned int noParkingSpot );
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

static int init ( )
// Algorithm:
// Sets the signal handler, gets the command mailbox, attaches the shared
// memory to the shmParkingLot pointer, and returns its id
{
	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	action.sa_handler = endTask;
	sigaction(SIGUSR2, &action, NULL);

	action.sa_handler = carExited;
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

static void carExited ( int signal )
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
			saveExit( WEXITSTATUS( status ) );
		}
	}
}

static void saveExit ( unsigned int noParkingSpot )
// Contract:
//
// Algorithm:
//
{
	char msg[1024];
	sprintf(msg, "%d: Une voiture s'est en allee du %d  ",
			getpid(), noParkingSpot );
	Afficher( MESSAGE, msg );

	struct ParkedCar * pExitedCar =
			&( shmParkingLot->parkedCars[noParkingSpot - 1] );

	/* BEGIN shared memory exclusion */
	semop( shmMutexId, &mutexAccess, 1 );
		pExitedCar->userType = AUCUN;
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */

	int carChosen = -1;
	struct WaitingCar * pCars = shmParkingLot->waitingCars;
	for ( unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i )
	{
		if ( AUCUN != ( pCars[i] ).userType )
		{
			if (// No car wants to enter yet:
				-1 == carChosen ||

				// PROF is prioritary over AUTRE:
				( AUTRE == ( pCars[carChosen] ).userType &&
				  PROF ==  ( pCars[i] ).userType ) ||

				// Same user type means "first arrived, first served":
				( ( pCars[i] )        .arrivalTime
				< ( pCars[carChosen] ).arrivalTime ) )
			{
				carChosen = i;
			}
		}
	}

	if ( -1 != carChosen )
	{
		// Allow one entrance door to continue
		struct sembuf V = SemV( carChosen );
		semop( waitSemSetId, &V, 1 );
	}

	Effacer( ( TypeZone )( ETAT_P1 + noParkingSpot - 1 ) );
} //----- End of savePark

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
// type Name ( parameter list )
// Algorithm:
//
//{
//} //----- End of Name

void ExitDoor ( )
// Algorithm:
//
{
	init( );

	for (;;)
	{
		struct ExitCommand command;
		int status;
		status = msgrcv( mbCommandId, &command,
						 EXIT_CMD_SIZE, SORTIE_GASTON_BERGER, 0 );
		if ( -1 == status && EINTR == errno )
		{
			continue; // starts waiting again without making anyone leave
		}

		char msg[1024];
		sprintf(msg, "ExitDoor: je dois faire sortir le %d",
				command.position );
		Afficher( MESSAGE, msg );

		// Making a car exit
		if ( -1 == SortirVoiture( command.position ) )
		{
			perror( "Error while trying to exit a car" );
			destroy( );
		}
	}

		destroy( );
} //----- End of ExitDoor

