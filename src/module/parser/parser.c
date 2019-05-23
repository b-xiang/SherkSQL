#include <stdio.h>
#include <regex.h>
#include <memory.h>
#include <SherkSQL/src/module/interviewer/interviewer.h>
#include <SherkSQL/src/module/analyst/analyst.h>
#include <stdlib.h>
#include <SherkEngine/src/module/executor/executor.h>
#include <SherkService/mechanism/module/grocery/grocery.h>
#include <SherkService/test/module/test_variable/test_variable.h>
#include <SherkService/mechanism/include/define/const.h>

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

    // use 数据库名;
    char *use_database_pattern = "^\\s*use\\s+.+$";

    // show variables;
    char *show_variables_pattern = "^\\s*show variables.*$";

    // create database 数据库名
    char *create_database_pattern = "^\\s*create database .*$";
    // drop database 数据库名
    char *drop_database_pattern = "^\\s*drop database .*$";

    // create table 数据表名(
    //
    //     id   INT,
    //     name INT,
    //     age  INT
    // );
    char *create_table_pattern = "";

    // drop table 数据表名
    char *drop_table_pattern = "^\\s*drop table .*$";

    char *res = (char *) malloc(sizeof(char) * CONST_RESPONSE_SIZE);
    memset(res, '\0', sizeof(char) * CONST_RESPONSE_SIZE);

    if(parser_match_regex(use_database_pattern, sql)){

        char *database_name = analyst_analysis_sql_use_database_get_database_name(sql);
        executor_handle_sql_use_database(database_name);
        sprintf(res, "Database Change to %s Success.\n", database_name);
    }else if (parser_match_regex(show_variables_pattern, sql)) {

        test_variable_print_variable();
        strcpy(res, "Show Variables Success.\n");
    } else if (parser_match_regex(create_database_pattern, sql)) {

        char *database_name = analyst_analysis_sql_create_database_get_database_name(sql);
        executor_handle_sql_create_database(database_name);
        sprintf(res, "Create %s Database Success.\n", database_name);
    } else if (parser_match_regex(drop_database_pattern, sql)) {

        char *database_name = analyst_analysis_sql_create_database_get_database_name(sql);
        executor_handle_sql_drop_database(database_name);
        sprintf(res, "Drop %s Database Success.\n", database_name);
    } else if (parser_match_regex(create_table_pattern, sql)) {

        char *table_name = analyst_analysis_sql_create_table_get_table_name(sql);

        char **field_name_list = analyst_analysis_sql_create_table_get_field_name_list(sql);
        int **field_type_list = analyst_analysis_sql_create_table_get_field_type_list(sql);
        executor_handle_sql_create_table(table_name, field_name_list, field_type_list);
        sprintf(res, "Create %s Table Success.\n", table_name);
    } else if (parser_match_regex(drop_table_pattern, sql)) {

        char *table_name = analyst_analysis_sql_drop_table_get_table_name(sql);
        executor_handle_sql_drop_table(table_name);
        sprintf(res, "Drop %s Table Success.\n", table_name);
    }

    return res;
}

/**
 * 解析匹配Sherk命令
 * @param command
 * @return
 */
char *parser_match_command(char *command) {

    // 解析 sherk command 命令
    char *s = grocery_string_cutwords(command, 0, 4);

    if (0 == strcmp("login", s)) {

        char *name = analyst_analysis_command_login_get_name(command);
        char *password = analyst_analysis_command_login_get_password(command);
        executor_handle_command_login(name, password);
        return "Login Success !"; // 必须有响应, 否则network不会send, 程序就会死循环
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
