#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t dev_random(uint32_t);
void generate_passphrase(char*);

#endif