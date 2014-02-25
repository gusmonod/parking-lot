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
static char *shm;

//------------------------------------------------------ Private functions
static int  init    ( );
// How to use:
// Initialization process of the <EntranceDoor> task

static void destroy ( );
// How to use:
// Destruction phase of the <EntranceDoor> task

static void handle  ( int signal );
// How to use:
// Handles the signals received

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
// Sets the signal handler, attaches the shared memory to the shm pointer,
// and returns its id
{
	struct sigaction action;
	action.sa_handler = handle;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR2, &action, NULL);

	// Getting the shared memory
	// (characteristics can be found in Information module)
	int shmId = shmget( ftok ( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE, RIGHTS );

	// Attaching shared memory
	shm = (char *) shmat( shmId, NULL, 0 );

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
	// Destruction of the shared memory
	// shmctl( shmId, 0, IPC_RMID, 0 );

	// Detaching the shared memory
	shmdt( shm );

	char msg[1024];
	sprintf(msg, "%d: Je suis en phase de destruction  ", getpid());
	Afficher( MESSAGE, msg );
	sleep( 3 );

	exit( 0 );
} //----- End of destroy

static void handle ( int signal )
// How to use:
//
// Contract:
//
// Algorithm:
//
{
	if ( SIGUSR2 == signal )
	{
		destroy( );
	}
} //----- End of handle

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
	init( );
	for (;;)
	{
		char msg[1024];
		sprintf(msg, "%d: Je suis en phase moteur          ", getpid());
		Afficher( MESSAGE, msg );
		sleep( 3 );
	}
	destroy( );
} //----- End of EntranceDoor


