/*************************************************************************
                          Keyboard  -  description
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//-------- Realization of the <Keyboard> task (file Keyboard.cpp) --------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------------- System include
#include <unistd.h> // for _exit
#include <cstdio> // for perror

#include <sys/ipc.h> // for all IPCS
#include <sys/msg.h> // for msgget

//------------------------------------------------------- Personal include
#include "Outils.h"
#include "Menu.h"

#include "Information.h"
#include "Keyboard.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables
static int mbCommandId;

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

void Keyboard ( )
// Algorithm:
// Infinitely asks the Menu for input
{
	// Getting the mailbox for the commands
	mbCommandId = msgget( ftok( PROGRAM_NAME, FTOK_CHAR ), RIGHTS );

	for ( ; ; )
	{
		Menu( );
	}
	perror( "Error: exited the Keyboard loop" );
	_exit( EXIT_FAILURE );
} //----- End of Keyboard

void Commande ( char code, unsigned int valeur )
// Algorithm:
// Checks the code and does the appropiate action
{
	switch ( code )
	{
		case 'Q' :
			_exit( EXIT_SUCCESS );
			break;
		case 'P' :
		{
			// The command to send:
			struct EnterCommand command;

			command.doorType = 1 == valeur ?
					PROF_BLAISE_PASCAL : ENTREE_GASTON_BERGER;
			command.userType = PROF;

			msgsnd( mbCommandId, &command, ENTER_CMD_SIZE, IPC_NOWAIT );
			break;
		}
		case 'A' :
		{
			// The command to send:
			struct EnterCommand command;

			command.doorType = 1 == valeur ?
					AUTRE_BLAISE_PASCAL : ENTREE_GASTON_BERGER;
			command.userType = AUTRE;

			msgsnd( mbCommandId, &command, ENTER_CMD_SIZE, IPC_NOWAIT );
			break;
		}
		case 'S' :
		{
			// The command to send:
			struct ExitCommand command;

			command.doorType = SORTIE_GASTON_BERGER;
			command.position = valeur;

			msgsnd( mbCommandId, &command, EXIT_CMD_SIZE, IPC_NOWAIT );
			break;
		}
	}
} //----- End of Commande

