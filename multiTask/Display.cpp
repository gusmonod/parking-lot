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
static struct Parking *shm;

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
	// Getting the shared memory
	// (characteristics can be found in Information module)
	std::cout << "name: " << PROGRAM_NAME << " ftok: " << FTOK_CHAR << std::endl;
	std::cout << "key: " << ftok( PROGRAM_NAME, FTOK_CHAR ) << std::endl;
	shmId = shmget( ftok( PROGRAM_NAME, FTOK_CHAR ), SHM_SIZE, RIGHTS );
	std::cout << "shmId: " << shmId << std::endl;

	// Attaching the shared memory
	shm = (struct Parking *) shmat( shmId, NULL, 0 );
}

static void destroy ( )
// Algorithm:
// Deletes the IPC objects used to communicate and destroys the app.
{
	// Detaches the shared memory
	shmdt( shm );
	shm = NULL;
	system( "clear" );
	exit( 0 );
}

static void handle ( int signal )
{
	if ( SIGUSR2 == signal )
	{
		destroy( );
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
		std::cout << "Memory: shmid='" << shmId << "' pid= '"
				  << getpid() << "'" << std::endl;
		sleep( 1 );
	}
}
