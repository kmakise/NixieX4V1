#ifndef __NIXIE_H
#define __NIXIE_H

#include "stdint.h"

void NixieTubeControl(uint8_t cmd);
void NixieTubeDisplay(uint8_t * num);
void NixieTubeTest(uint8_t delay);
void NixieTubeFlash(uint8_t delay);


#endif /*__NIXIE_H*/

