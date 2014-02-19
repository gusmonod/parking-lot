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
	pid_t noKeyboard;
	pid_t noHeure = ActiverHeure();
	
	InitialiserApplication( XTERM );
	
	if( ( noKeyboard = fork ( ) ) == 0 )
	{
		Keyboard( );
	}
	else
	{
		waitpid( noKeyboard, NULL, 0 );	
	}
	kill(noHeure, SIGUSR2);
	TerminerApplication();
	
	return 0;
}
