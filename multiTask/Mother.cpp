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

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"

#include "Information.h"
#include "Mother.h"
#include "Keyboard.h"
#include "EntranceDoor.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables
static int shmId;
static int shmMutexId;

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

	// Initialisation of the shared memory
	shmParkingLot = ( struct ParkingLot * ) shmat( shmId, NULL, 0 );

	shmParkingLot->fullSpots = 0;

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
	shmMutexId = semget( ftok( PROGRAM_NAME, FTOK_CHAR), MUTEX_NB,
					IPC_CREAT | RIGHTS );
	// The shared memory is accessible, so the mutex is set to MUTEX_OK
	semctl( shmMutexId, 0, SETVAL, MUTEX_OK );
}

static void destroy ( )
// Algorithm:
// Deletes the IPC objects used to communicate and destroys the app.
{
	// Deletes the shared memory
	shmctl( shmId, 0, IPC_RMID );

	// Deletes the shared memory mutex
	semctl( shmMutexId, 0, IPC_RMID, 0 );

	// Terminates the app
	TerminerApplication();
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
	pid_t noKeyboard;
	pid_t noHour = ActiverHeure();
	pid_t noEntranceDoors[NB_BARRIERES_ENTREE];

	for (unsigned int i = 0; i < NB_BARRIERES_ENTREE ; ++i)
	{
		noEntranceDoors[i] = fork();
		if (0 == noEntranceDoors[i]) // Child process
		{
			// Giving the correct type of door as argument
			EntranceDoor((TypeBarriere)(i+ 1));
		}
	}

	if( ( noKeyboard = fork ( ) ) == 0 )
	{
		Keyboard( );
	}
	else
	{
		// Waiting for end synchronization from Keyboard
		waitpid( noKeyboard, NULL, 0 );

		// Killing the NB_BARRIERES_ENTREE entrance doors
		for (unsigned int i = 0; i < NB_BARRIERES_ENTREE ; ++i)
		{
			kill( noEntranceDoors[i], SIGUSR2 );
			waitpid( noEntranceDoors[i], NULL, 0 );
		}

		// Killing the Hour task
		kill( noHour, SIGUSR2 );
		waitpid( noHour, NULL, 0 );

		// And finally, terminating the application
		destroy( );
	}
	
	return 0;
}
