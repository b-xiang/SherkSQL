#ifndef SERVER_SHERK_SQL_MODULE_ANALYST_H
#define SERVER_SHERK_SQL_MODULE_ANALYST_H

extern int analyst_analysis_sql_category(char *sql);

extern char *analyst_analysis_command_login_get_name(char *command);

extern char *analyst_analysis_command_login_get_password(char *command);

extern char *analyst_analysis_sql_create_database_get_database_name(char *command);

#endif
