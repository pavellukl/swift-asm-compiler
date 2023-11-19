#ifndef RECURSIVE_PARSER_H
#define RECURSIVE_PARSER_H

#include "../parser.h"
#include "../precedence_parser/precedence_parser.h"

void add_inbuilt_functions_to_symtable(ListST *symtable);
void parse_check_optimize_generate(ParserOptions *parser_opt);
void parse_function_definition(ParserOptions *parser_opt);

bool _program(ParserOptions *parser_opt);
bool _function_definition(ParserOptions *parser_opt);
bool _function_head(ParserOptions *parser_opt);
bool __func_identif_lbracket_arglist_rbracket(ParserOptions *parser_opt);
bool _param_list(ParserOptions *parser_opt);
bool _comma_param(ParserOptions *parser_opt);
bool _param(ParserOptions *parser_opt);
bool __param_name(ParserOptions *parser_opt);
bool _scope_body(ParserOptions *parser_opt);
bool _command_sequence(ParserOptions *parser_opt);
bool _command(ParserOptions *parser_opt);
bool __identif(ParserOptions *parser_opt, char *identif);
bool _data_type(ParserOptions *parser_opt);
bool _return_command(ParserOptions *parser_opt);
bool __return(ParserOptions *parser_opt);
bool _variable_def(ParserOptions *parser_opt);
bool __varlet_identif(ParserOptions *parser_opt);
bool __varlet_identif_colon_type(ParserOptions *parser_opt);
bool _conditional_command(ParserOptions *parser_opt);
bool __if(ParserOptions *parser_opt);
bool __if_let_identif_body(ParserOptions *parser_opt);
bool __if_let_identif_body_else(ParserOptions *parser_opt);
bool _while_command(ParserOptions *parser_opt);
bool _function_call(ParserOptions *parser_opt);
bool _arg_list(ParserOptions *parser_opt, Parameters *args);
bool _comma_arg(ParserOptions *parser_opt, Parameters *args);
bool _arg(ParserOptions *parser_opt, Parameters *args);
bool __arg_name(ParserOptions *parser_opt, Parameter *arg);
bool __arg_name_colon(ParserOptions *parser_opt, Parameter *arg);
bool _arg_val(ParserOptions *parser_opt, Parameter *arg);

#endif /* RECURSIVE_PARSER_H */