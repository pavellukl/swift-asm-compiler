/**
 * @name Implementation of IFJ23 language compiler
 * @file StackTokenData.h
 * @brief Contains the expansion of stack declaration macro
 *
 * @author Jan Klanica (xklani00)
 * @author Pavel Lukl (xlukl00)
 */

#ifndef STACK_TOKEN_DATA_H
#define STACK_TOKEN_DATA_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../helpers/stack.h"
#include "../token.h"

STACK_DEC(TokenData, TokenData, token_data)

#endif /* STACK_TOKEN_DATA_H */