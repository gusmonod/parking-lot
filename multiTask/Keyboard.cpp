/*************************************************************************
   Keyboard  -  The keyboard task managing the inputs
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---------- Realization of the <Keyboard> task (file Keyboard.cpp) ----------

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

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
// type Name ( parameter list )
// Algorithm:
//
//{
//} //----- End of Name

void Keyboard ( )
{
	Menu();
}

void Commande ( char code, unsigned int valeur )
{
	switch(code)
	{
		case 'Q' :
			exit(0);
		break;
	}
}

