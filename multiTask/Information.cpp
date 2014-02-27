/*************************************************************************
                        Information -  description
                             -------------------
    date                 : Feb. 19 2014
    copyright            : (C) 2014 Yannick Marion & Gustave Monod
    e-mail               : yannick.marion@insa-lyon.fr
                           gustave.monod@insa-lyon.fr
*************************************************************************/

//---- Realization of the <Information> module (file Information.cpp) ----

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------------- System include
#include <sys/sem.h>

//------------------------------------------------------- Personal include
#include "Information.h"

/////////////////////////////////////////////////////////////////  PRIVATE
//-------------------------------------------------------------- Constants
int  const RIGHTS = 0660;
char const PROGRAM_NAME[] = "./Parking";
char const FTOK_CHAR = '3';

// Used for the shared memory:
int  const SHM_SIZE = sizeof(struct ParkingLot);

// Mutex for the shared memory
struct sembuf const MUTEX_ACCESS = { 0, -1, 0 };
struct sembuf const MUTEX_FREE   = { 0,  1, 0 };
int  const MUTEX_NB = 1;
int  const MUTEX_OK = 1;
int  const MUTEX_KO = 0;

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
