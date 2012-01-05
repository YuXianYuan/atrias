/**
 * @file
 * @author Devin Koepl
 * @brief does all the includes for controller_wrapper_states.c
 *
 *
 */

/*****************************************************************************/

#ifndef FUNCS_H_CONTROL_WRAPPER_STATES
#define FUNCS_H_CONTROL_WRAPPER_STATES

// ATRIAS
#include <atrias/ucontroller.h>

// ATRIAS Controllers
#include <atrias_controllers/controller.h>
#include <atrias_controllers/control_switcher_state_machine.h>
#include <atrias_controllers/uspace_kern_shm.h>

// DRL Library
#include <drl_library/discretize.h>
#include <drl_library/drl_math.h>

#define NUM_OF_MEDULLAS_ON_ROBOT 			2 
#define NUM_OF_SLAVES_IN_SIMULATION_MACHINE 1

/** @brief Reference to Motor A. */
#define	A_INDEX								1

/** @brief Reference to Motor B. */
#define B_INDEX								0

// Horizontal velocity windowing filter parameters.
#define HOR_VEL_FILTER_EPS					0.003
#define HOR_VEL_WINDOW						100

/*****************************************************************************/

unsigned char initialize_shm( void );
void takedown_shm( void );

void control_wrapper_state_machine( uControllerInput **, uControllerOutput ** );


unsigned char state_wakeup( uControllerInput **, uControllerOutput **, unsigned char );
unsigned char state_restart( uControllerInput **, uControllerOutput **, unsigned char );
unsigned char state_check( uControllerInput **, uControllerOutput **, unsigned char );
unsigned char state_initialize(  uControllerInput **, uControllerOutput **, unsigned char );
unsigned char state_run( uControllerInput **, uControllerOutput **, unsigned char );
unsigned char state_error( uControllerInput **, uControllerOutput **, unsigned char );

/*****************************************************************************/

#endif // FUNCS_H_CONTROL_WRAPPER_STATES