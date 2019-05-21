#include <stdio.h>
#include <regex.h>
#include <memory.h>
#include <SherkSQL/src/module/interviewer/interviewer.h>
#include <SherkSQL/src/module/analyst/analyst.h>
#include <stdlib.h>
#include <SherkEngine/src/module/executor/executor.h>
#include <SherkService/mechanism/module/grocery/grocery.h>
#include <SherkService/test/module/test_variable/test_variable.h>

// 解析原理 : 正则 + 解析树

const int SQL_MAX_PATTRENS = 100;

const int SQL_CATEGORY_IS_SHERK_COMMAND = 1;
const int SQL_CATEGORY_IS_SQL = 2;

const int SQL_TYPE_IS_CREATE_DATABASE = 3;
const int SQL_TYPE_IS_CREATE_TABLE = 4;

const int SQL_TYPE_IS_DROP_DATABASE = 5;


/**
 * 匹配正则
 * @param pattern
 * @param sql
 * @return
 */
int parser_match_regex(const char *pattern, const char *sql) {
    int result = 0;

    regex_t regex;
    int regexInit = regcomp(&regex, pattern, REG_EXTENDED);
    if (!regexInit) {

        int reti = regexec(&regex, sql, 0, NULL, 0);

        if (REG_NOMATCH != reti) {

            result = 1; // 匹配到
        }
    }
    // printf("匹配, patter:%s, sql:%s, result:%d\n", pattern, sql, result);
    regfree(&regex);
    return result;
}

/**
 * 解析匹配SQL
 * @param sql
 * @return
 */
char *parser_match_sql(char *sql) {

    char *show_variables_pattern = "^\\s*show variables.*$";
    char *create_database_pattern = "^\\s*create database .*$";
    char *res = (char*)malloc(sizeof(char)*100);

    if(parser_match_regex(show_variables_pattern, sql)){

        test_variable_print_variable();
    }else if( parser_match_regex(create_database_pattern, sql) ){

        char *database_name = analyst_analysis_sql_create_database_get_database_name(sql);
        executor_handle_sql_create_database(database_name);
        sprintf(res, "Create %s Database Success.\n", database_name);
        return res;
    }

    return "";
}

/**
 * 解析匹配Sherk命令
 * @param command
 * @return
 */
char *parser_match_command(char *command) {

    // 解析 sherk command 命令
    char *s = grocery_string_cutwords(command,0,4);

    if( 0 == strcmp("login", s) ){

        char *name = analyst_analysis_command_login_get_name(command);
        char *password = analyst_analysis_command_login_get_password(command);
        executor_handle_command_login(name, password);
    }

    return "";
}

/**
 * 执行解析
 * @param sql
 * @return
 */
char *parser_exec(char *sql) {

    grocery_string_tolower(sql);

    // 分析SQL的类型
    int sql_category = analyst_analysis_sql_category(sql);

    // 让 variable_master 去记忆SQL
    interviewer_call_variable_master_memory_sql(sql, sql_category);

    // 分类不同的SQL
    if (SQL_CATEGORY_IS_SHERK_COMMAND == sql_category) {

        // SQL 是 Sherk Command: command=
        return parser_match_command(&sql[8]);
    } else if (SQL_CATEGORY_IS_SQL == sql_category) {

        // SQL 是 SQL 命令: sql=
        return parser_match_sql(&sql[4]);
    } else {

        return "Illegal SQL, Parse Error !\n";
    }
}

/**
 * 生成执行计划
 */
void parser_make_execution_plan() {}
