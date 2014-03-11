/*************************************************************************
                           Mother  -  description
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---------- Interface of the <Mother> task (file Mother.h) --------------
#if !defined ( MOTHER_H )
#define MOTHER_H

//------------------------------------------------------------------------
// Role of the <Mother> task
//
// Creates:
// * Shared memory
// * Mutex for the above shared memory
// * Mailbox for communitcating Keyboard commands
//
// * <Keyboard> task
// * <Hour> task
// * <EntranceDoor> tasks (NB_BARRIERES_ENTREE instances)
//
// Terminates all tasks and destroys everything when:
// * The <Keyboard> task is done (meaning the app should quit)
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Used interfaces

//-------------------------------------------------------------- Constants

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//------------------------------------------------------- Public functions
int main ( );
// How to use:
//
// Contract:

// type Name ( parameter list );
// How to use:
//
// Contract:
//

#endif // MOTHER_H

