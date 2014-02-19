/*************************************************************************
   EntranceDoor  -  The EntranceDoor task managing the Entrance
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---------- Interface of the <EntranceDoor> task (file EntranceDoor.h) --------------
#if !defined ( ENTRANCEDOOR_H )
#define ENTRANCEDOOR_H

//------------------------------------------------------------------------
// Role of the <EntranceDoor> task
//
// Creates and destroys:
// * Keyboard
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Used interfaces

//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
void EntranceDoor ( TypeBarriere type );
// How to use:
//
// Contract:

// type Name ( parameter list );
// How to use:
//
// Contract:
//

#endif // ENTRANCEDOOR_H


