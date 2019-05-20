#include <stdio.h>
#include <regex.h>
#include <SherkMechanism/src/module/administrator/administrator.h>
#include <memory.h>
#include <SherkMechanism/src/include/define/const.h>
#include <SherkSQL/src/module/interviewer/interviewer.h>
#include <SherkMechanism/src/module/grocery/grocery.h>
#include <SherkMechanism/src/module/variable_master/variable_master.h>

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

            result = 1;
        }
    }
    regfree(&regex);
    return result;
}

/**
 * 分析SQL分类
 */
int parser_analysis_sql_category(char *sql) {

    char *pattern_command = "^\\s*command=.*$";
    char *pattern_sql = "^\\s*sql=.*$";

    if (parser_match_regex(pattern_command, sql)) {

        return SQL_CATEGORY_IS_SHERK_COMMAND;
    } else if (parser_match_regex(pattern_sql, sql)) {

        return SQL_CATEGORY_IS_SQL;
    } else {

        return -1;
    }
}

/**
 * 解析匹配SQL
 * @param sql
 * @return
 */
char *parser_match_sql(char *sql) {

    return "";
}

/**
 * 解析匹配Sherk命令
 * @param command
 * @return
 */
char *parser_match_command(char *command) {

    // 解析 sherk command 命令
    char *s = grocery_string_cutwords(command,8,13);

    if( 0 == strcmp("login", s) ){


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
    int sql_category = parser_analysis_sql_category(sql);

    // 让 variable_master 去记忆SQL
    interviewer_call_variable_master_memory_sql(sql, sql_category);

    if (SQL_CATEGORY_IS_SHERK_COMMAND == sql_category) {

        return parser_match_command(sql);
    } else if (SQL_CATEGORY_IS_SQL == sql_category) {

        return parser_match_sql(sql);
    } else {

        return "Illegal SQL, Parse Error !\n";
    }
}

/**
 * 生成执行计划
 */
void parser_make_execution_plan() {}
