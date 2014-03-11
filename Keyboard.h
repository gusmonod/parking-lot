/*************************************************************************
                          Keyboard  -  description
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---------- Interface of the <Keyboard> task (file Keyboard.h) --------------
#if !defined ( KEYBOARD_H )
#define KEYBOARD_H

//------------------------------------------------------------------------
// Role of the <Keyboard> task
//
// Creates:
// * Keyboard listener
//
// Terminates task and destroys everything when:
// * The input to quit is received
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Used interfaces

//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
void Keyboard ( );
// How to use:
//
// Contract:

void Commande ( char code, unsigned int valeur );
// How to use:
//
// Contract:
//

// type Name ( parameter list );
// How to use:
//
// Contract:
//

#endif // KEYBOARD_H


