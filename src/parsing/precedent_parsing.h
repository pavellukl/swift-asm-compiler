#ifndef PRECEDENT_PARSING_H
#define PRECEDENT_PARSING_H

/// @brief Enumeration of operations that occur in precedent parsing.
typedef enum {PRC_PAR_REDUCE, PRC_PAR_SHIFT, PRC_PAR_EQUALS, PRC_PAR_ERROR} PrecedentOperation;

/**
 * @brief Precedent table type.
 *
 * The table is of size m*n, where m represents terminal and n represents
 * token.
 */
typedef PrecedentOperation **PrecedentTable;

#endif /* PRECEDENT_PARSING_H */