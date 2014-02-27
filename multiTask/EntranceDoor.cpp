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
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <sys/wait.h>

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"
#include "Menu.h"

#include "Information.h"
#include "Keyboard.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables

static TypeBarriere doorType;

//IPC
static struct ParkingLot *shmParkingLot;
static int shmMutexId;

static struct sembuf mutexAccess = MUTEX_ACCESS;
static struct sembuf mutexFree  = MUTEX_FREE;

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
// Sets the signal handler, attaches the shared memory to the shm pointer,
// and returns its id
{
	doorType = type;

	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	action.sa_handler = endTask;
	sigaction(SIGUSR2, &action, NULL);

	action.sa_handler = carParked;
	sigaction(SIGCHLD, &action, NULL);

	// Getting the shared memory
	// (characteristics can be found in Information module)
	int shmId = shmget( ftok( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE, RIGHTS );

	// Attaching shared memory
	shmParkingLot = (struct ParkingLot *) shmat( shmId, NULL, 0 );

	// Getting the shared memory mutex
	shmMutexId = semget( ftok( PROGRAM_NAME, FTOK_CHAR), MUTEX_NB, RIGHTS);

	char msg[1024];
	sprintf(msg, "%d: Je suis en phase d'initialisation", getpid());
	Afficher( MESSAGE, msg );
	sleep( 3 );

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

static void carParked( int signal )
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

	char state[NB_CHAR_STATE + 1];
	int carNo;

	/* BEGIN shared memory exclusion */
	semop( shmMutexId, &mutexAccess, 1 );
		carNo = ( shmParkingLot->nextCarNo )++;
	semop( shmMutexId, &mutexFree, 1 );
	/* END   shared memory exclusion */

	// TODO get real values
	sprintf( state, "%c  %03d  %09d", userTypeCh( PROF ), carNo, 38914799 );

	Afficher( ( TypeZone )( ETAT_P1 + noParkingSpot - 1 ), state );
} //----- End of saveParked

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

	// Parking a car
	if ( -1 == GarerVoiture( type ) )
	{
		perror( "Error while trying to park a car" );
		destroy( );
	}

	for (;;)
	{
		sleep( 5 );
	}
	destroy( );
} //----- End of EntranceDoor


