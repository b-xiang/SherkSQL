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
#include <SherkService/mechanism/include/define/rescode.h>

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

    // show databases
    char *show_databases_pattern = "^\\s*show databases.*$";

    // show tables
    char *show_tables_pattern = "^\\s*show tables.*$";

    // use 数据库名;
    char *use_database_pattern = "^\\s*use.+$"; // "^\s*use\s+.+$" - 这样写匹配不到,  "^\\s*use.+$" - 这样写可以匹配到

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
    char *create_table_pattern = "^\\s*create table .*$";

    // desc table 数据表名
    char *desc_table_pattern = "^\\s*desc table .*$";

    // drop table 数据表名
    char *drop_table_pattern = "^\\s*drop table .*$";

    char *res = (char *) malloc(sizeof(char) * CONST_RESPONSE_SIZE);
    memset(res, '\0', sizeof(char) * CONST_RESPONSE_SIZE);

    // 匹配 SQL: show databases
    if (parser_match_regex(show_databases_pattern, sql)) {

        executor_handle_sql_show_databases();
        sprintf(res, "Show Databases Success.\n");
    }
        // 匹配 SQL: show tables
    else if (parser_match_regex(show_tables_pattern, sql)) {

        if (RES_OK != executor_handle_sql_show_tables()) {

            sprintf(res, "Please Use Database Firstly.\n");
        } else {
            sprintf(res, "Show Tables Success.\n");
        };
    }
        // 匹配 SQL: use database
    else if (parser_match_regex(use_database_pattern, sql)) {

        char *database_name = analyst_analysis_sql_use_database_get_database_name(sql);
        executor_handle_sql_use_database(database_name);
        sprintf(res, "Database Change to %s Success.\n", database_name);
    }
        // 匹配 SQL: show variables
    else if (parser_match_regex(show_variables_pattern, sql)) {

        test_variable_print_variable();
        sprintf(res, "Show Variables Success.\n");
    }
        // 匹配 SQL: create database
    else if (parser_match_regex(create_database_pattern, sql)) {

        char *database_name = analyst_analysis_sql_create_database_get_database_name(sql);
        executor_handle_sql_create_database(database_name);
        sprintf(res, "Create %s Database Success.\n", database_name);
    }
        // 匹配 SQL: drop database
    else if (parser_match_regex(drop_database_pattern, sql)) {

        char *database_name = analyst_analysis_sql_create_database_get_database_name(sql);
        executor_handle_sql_drop_database(database_name);
        sprintf(res, "Drop %s Database Success.\n", database_name);
    }

        // 匹配 SQL: desc table
    else if (parser_match_regex(desc_table_pattern, sql)) {

        char *table_name = analyst_analysis_sql_desc_table_get_table_name(sql);
        int desc_table_code = executor_handle_sql_desc_table(table_name);

        if (1 == desc_table_code) {

            sprintf(res, "Please Use Database Firstly.\n");
        } else if (2 == desc_table_code) {

            sprintf(res, "This Table Not Exists.\n");
        } else {
            sprintf(res, "Desc Table Success.\n");
        }
    }
        // 匹配 SQL: create table
    else if (parser_match_regex(create_table_pattern, sql)) {

        char *table_name = analyst_analysis_sql_create_table_get_table_name(sql);
        char **field_name_list = analyst_analysis_sql_create_table_get_field_name_list(sql);
        int *field_type_list = analyst_analysis_sql_create_table_get_field_type_list(sql);

        // for (int i = 0; i <= 2; ++i) { printf("%s----\n", field_name_list[i]); }
        // printf("-------------------------\n");
        // for (int i = 0; i <= 2; ++i) { printf("%d----\n", field_type_list[i]); }

        int create_table_code = executor_handle_sql_create_table(table_name, field_name_list, field_type_list, 3);

        if (1 == create_table_code) {

            // 请先选择数据库
            sprintf(res, "Please Use Database Firstly.\n");
        } else if (2 == create_table_code) {

            // 创表语句有错误
            sprintf(res, "Your Create Table SQL With a Error.\n");
        } else if (3 == create_table_code) {

            // 表已存在
            sprintf(res, "The Same Table Already Taken.\n");
        } else {

            sprintf(res, "Create %s Table Success.\n", table_name);
        }
    }
        // 匹配 SQL: drop table
    else if (parser_match_regex(drop_table_pattern, sql)) {

        char *table_name = analyst_analysis_sql_drop_table_get_table_name(sql);
        executor_handle_sql_drop_table(table_name);
        sprintf(res, "Drop %s Table Success.\n", table_name);
    } else {

        sprintf(res, "Illegal SQL, Parse Error !\n");
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
