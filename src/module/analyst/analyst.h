#ifndef SERVER_SHERK_SQL_MODULE_ANALYST_H
#define SERVER_SHERK_SQL_MODULE_ANALYST_H

extern int analyst_analysis_sql_category(char *sql);

extern char *analyst_analysis_command_login_get_name(char *command);

extern char *analyst_analysis_command_login_get_password(char *command);

extern char *analyst_analysis_sql_use_database_get_database_name(char *command);

extern char *analyst_analysis_sql_create_database_get_database_name(char *command);

extern char *analyst_analysis_sql_drop_table_get_table_name(char *command);

extern char *analyst_analysis_sql_create_table_get_table_name(char *command);

extern char **analyst_analysis_sql_create_table_get_field_name_list(char *command);

extern int **analyst_analysis_sql_create_table_get_field_type_list(char *command);

#endif
