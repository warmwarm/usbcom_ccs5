#ifndef SETLUT_H_
#define SETLUT_H_


tBoolean writePageToEEPROM (unsigned char* rev_buf);

tBoolean writeLUTToEEPROM (tBoolean set_default);

tBoolean readLUTFromEEPROM ();

#endif /*SETLUT_H_*/
