#ifndef PARSING_H
#define PARSING_H

#include "predictive_parsing.h"
#include "precedent_parsing.h"

/**
 * @brief Type for parsing parameters.
 */
typedef struct {
    /// @brief Simple LL-table containing numbers of rules.
    LLTable ll_table;

    /// @brief length of LL-table in n direction.
    int ll_table_n;

    /// @brief Length of LL-table in m direction.
    int ll_table_m;

    /// @brief Simple precedent table containing numbers of rules.
    PrecedentTable precedent_table;

    /// @brief Length of precedent table in n direction.
    int precedent_table_n;

    /// @brief Length of precedent table in m direction.
    int precedent_table_m;
} ParsingParams;

#endif /* PARSING_H */