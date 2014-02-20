/*************************************************************************
   EntranceDoor  -  The EntranceDoor task managing the Entrance
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
<<<<<<< HEAD
#include <signal.h>
#include <errno.h>
=======
>>>>>>> 4f0923bb9212ca6e6f0f15672adfff9ed66ef7eb

//------------------------------------------------------- Personal include
#include "Heure.h"
#include "Outils.h"
#include "Menu.h"

#include "Keyboard.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//------------------------------------------------------- Static variables

//------------------------------------------------------ Private functions
<<<<<<< HEAD
static void end();

=======
>>>>>>> 4f0923bb9212ca6e6f0f15672adfff9ed66ef7eb
//static type name ( parameter list )
// How to use:
//
// Contract:
//
// Algorithm:
//
//{
//} //----- End of name

static void stop ( int signal )
// How to use:
//
// Contract:
//
// Algorithm:
//
{
	end();
} //----- End of stop

static void end ( )
// How to use:
//
// Contract:
//
// Algorithm:
//
{
	exit(0);
} //----- End of end

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
	struct sigaction action;
	action.sa_handler = stop;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR2, &action, NULL);

	switch(type)
	{
		case AUCUNE:
		case SORTIE_GASTON_BERGER:
			end();
			break;
		case PROF_BLAISE_PASCAL:
			break;
		case AUTRE_BLAISE_PASCAL:
			break;
		case ENTREE_GASTON_BERGER:
			break;
	}

	for (;;) {
		sleep(20);
		if (EINTR == errno) {
			continue;
		}
	}
} //----- End of EntranceDoor


