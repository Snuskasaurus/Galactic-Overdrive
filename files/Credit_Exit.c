#include "Credit.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Credit_Exit(MainData *_data)
{
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];

	// Delete the state from memory
	free(state);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //