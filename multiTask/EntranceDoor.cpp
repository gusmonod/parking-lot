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
	struct sigaction sa;
	sa.sa_handler = stop;

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
} //----- End of EntranceDoor


