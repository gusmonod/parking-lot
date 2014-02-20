/*************************************************************************
   Mother  -  The mother task creating the context and launching the app
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

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"

#include "Mother.h"
#include "Keyboard.h"
#include "EntranceDoor.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables

//------------------------------------------------------ Private functions
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
	// First thing to do: Initialize app
	InitialiserApplication( XTERM );

	// The pid of the multiple tasks
	pid_t noKeyboard;
	pid_t noHour = ActiverHeure();
	pid_t noEntranceDoors[NB_BARRIERES_ENTREE];
	
	for (unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i) {
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
		for (unsigned int i = 0; i < NB_BARRIERES_ENTREE; ++i) {
			kill(noEntranceDoors[i], SIGUSR2);
			waitpid(noEntranceDoors[i], NULL, 0);
		}

		// Killing the Hour task
		kill(noHour, SIGUSR2);
		waitpid( noHour, NULL, 0 );

		// And finally, terminating the application
		TerminerApplication();
	}
	
	return 0;
}
