#ifndef LIST_PP_H
#define LIST_PP_H

#include "../../../helpers/list.h"
#include "../../../scanner/StackTokenData/StackTokenData.h"

/** 
 * @brief List item type. Some of those values are used for indexing in
 * the precedence table.
 */
typedef enum { 
    TERMINAL_ADD, TERMINAL_SUB, TERMINAL_MUL, TERMINAL_DIV,
    TERMINAL_EQUAL, TERMINAL_NOT_EQUAL, TERMINAL_LESSER, TERMINAL_LESSER_EQUAL,
    TERMINAL_GREATER, TERMINAL_GREATER_EQUAL, TERMINAL_AND, TERMINAL_OR,
    TERMINAL_EXCL_MARK, TERMINAL_NOT, TERMINAL_NIL_COALESCING,
    TERMINAL_KEYWORD_TRUE, TERMINAL_KEYWORD_FALSE, TERMINAL_IDENTIF,
    TERMINAL_NUMBER, TERMINAL_STRING, TERMINAL_KEYWORD_NIL, TERMINAL_L_BRACKET,
    TERMINAL_R_BRACKET, TERMINAL_EMPTY, NONTERMINAL_EXPRESSION, FLAG_HANDLE
} ASTItemType;

/** @brief Represents an item in the precedence list. */
typedef struct ASTNode {
    TokenData token;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef struct {
    ASTItemType type;
    ASTNode node;
} PPListItem;

LIST_DEC(PPListItem, PP, pp)

#endif /* LIST_PP_H */