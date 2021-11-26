/* This code executes at compile time
 * It will then encapsulated output as a string.
 */
#define D #exe{ echo Test }

"%s", D;