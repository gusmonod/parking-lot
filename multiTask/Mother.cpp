/*************************************************************************
                           Mother  -  description
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---------- Realization of the <Mother> task (file Mother.cpp) ----------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------------- System include
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"

#include "Information.h"
#include "Mother.h"
#include "Keyboard.h"
#include "EntranceDoor.h"
#include "ExitDoor.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables
static int shmId;
static int shmMutexId;

static int mbCommandId;

static int waitSemSetId;

//------------------------------------------------------ Private functions
static void init    ( );
// How to use:

static void destroy ( );
// How to use:

static void init ( )
// Algorithm:
// Initializes app, and creates the IPC objects used to communicate.
{
	// First thing to do: Initialize app
	InitialiserApplication( XTERM );

	// Creating the shared memory
	// (characteristics can be found in Information module)
	shmId = shmget( ftok( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE,
					IPC_CREAT | RIGHTS );

	struct ParkingLot * shmParkingLot;

	// Initialization of the shared memory
	shmParkingLot = ( struct ParkingLot * ) shmat( shmId, NULL, 0 );

	shmParkingLot->fullSpots = 0;
	shmParkingLot->nextCarNo = 1;

	for ( unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i )
	{
		( shmParkingLot->waitingCars[i] ).userType = AUCUN;
	}

	for ( unsigned int i = 0; i < NB_PLACES; ++i )
	{
		( shmParkingLot->parkedCars[i] ).userType = AUCUN;
	}

	// Initialization done, detaching memory
	shmdt( shmParkingLot );
	shmParkingLot = NULL;

	// Creating the shared memory mutex
	shmMutexId = semget( ftok( PROGRAM_NAME, FTOK_CHAR ), MUTEX_NB,
					IPC_CREAT | RIGHTS );
	// The shared memory is accessible, so the mutex is set to MUTEX_OK
	semctl( shmMutexId, 0, SETVAL, MUTEX_OK );

	// Creating the mailbox for the commands
	mbCommandId = msgget( ftok( PROGRAM_NAME, FTOK_CHAR ),
						  IPC_CREAT | RIGHTS );

	// Creating the semaphore set for the entrance doors to wait
	waitSemSetId = semget( ftok( PROGRAM_NAME, FTOK_CHAR + 1 ),
					NB_BARRIERES_ENTREE, IPC_CREAT | RIGHTS );

	for ( unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i )
	// For all semaphores in the set:
	{
		// By default, the entrance door should not let anyone in
		semctl( waitSemSetId, i, SETVAL, MUTEX_KO );
	}
}

static void destroy ( )
// Algorithm:
// Deletes the IPC objects used to communicate and destroys the app.
{
	// Deletes all of the waiting semaphores
	for ( unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i )
	{
		semctl( waitSemSetId, i, IPC_RMID, 0 );
	}

	// Deletes the command mailbox
	msgctl( mbCommandId, IPC_RMID, 0 );

	// Deletes the shared memory mutex
	semctl( shmMutexId, 0, IPC_RMID, 0 );

	// Deletes the shared memory
	shmctl( shmId, 0, IPC_RMID );

	// Terminates the app
	TerminerApplication();

	_exit( EXIT_SUCCESS );
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

	// The pid of the multiple tasks
	pid_t nbHour = ActiverHeure();
	pid_t nbExitDoor;
	pid_t nbEntranceDoors[NB_BARRIERES_ENTREE];
	pid_t nbKeyboard;

	if ( 0 == ( nbExitDoor = fork( ) ) )
	{
		ExitDoor( );
		perror( "exit" );
//		_exit( EXIT_FAILURE );
	}
	else
	{
		for (unsigned int i = 0; i < NB_BARRIERES_ENTREE ; ++i)
		{
			nbEntranceDoors[i] = fork();
			if (0 == nbEntranceDoors[i]) // Child process
			{
				// Giving the correct type of door as argument
				EntranceDoor((TypeBarriere)(i+ 1));
				perror( "entrance" );
//				_exit( EXIT_FAILURE );
			}
		}
		if ( 0 == ( nbKeyboard = fork( ) ) )
		{
			Keyboard( );
			perror( "keyboard" );
//			_exit( EXIT_FAILURE );
		}
		else
		{
			// Waiting for end synchronization from Keyboard
			waitpid( nbKeyboard, NULL, 0 );

			// Killing the exit door
			kill( nbExitDoor, SIGUSR2 );
			waitpid( nbExitDoor, NULL, 0 );

			// Killing the NB_BARRIERES_ENTREE entrance doors
			for (unsigned int i = 0; i < NB_BARRIERES_ENTREE ; ++i)
			{
				kill( nbEntranceDoors[i], SIGUSR2 );
				waitpid( nbEntranceDoors[i], NULL, 0 );
			}

			// Killing the Hour task
			kill( nbHour, SIGUSR2 );
			waitpid( nbHour, NULL, 0 );

			// And finally, terminating the application
			destroy( );
		}
	}

	_exit( EXIT_FAILURE );
}
