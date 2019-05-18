#include <stdio.h>
#include <regex.h>
#include <SherkSQL/src/include/const/sql_type.h>
#include <SherkEngine/src/module/executor/executor.h>
#include <SherkMechanism/src/module/administrator/administrator.h>
#include <memory.h>

// 解析原理 : 正则 + 解析树

/**
 * 匹配每一个正则
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
 * 解析SQL类型
 * @param sql_type
 */
void parser_analysis_sql_type(int sql_type){

    switch (sql_type) {

        case SQL_TYPE_IS_CREATE_DATABASE:
            break;
        case SQL_TYPE_IS_DROP_DATABASE:
            break;

        case SQL_TYPE_IS_CREATE_TABLE:
            break;

        default:
            break;
    }


}


/**
 * 生成执行计划
 */
void parser_make_execution_plan(){

}


/**
 * 解析器开始匹配
 * @param sql
 * @return
 */
int parser_match(char *sql) {

    char *pattrens = {

            "^\\s+CREATE\\s+DATABASE\\s+.+$", // 建库语句
            "^\\s+DROP\\s+DATABASE\\s+.+$", // 删库语句

            "", // 建表语句
            "^\\s+DROP\\s+TABLE\\s+.+$", // 删表语句

            "^\\s+DELETE\\s+FROM\\s+.+$", // 清空表数据

            "^\\s+TRUNCATE\\s+FROM\\s+.+$", // 截断表的全部数据
    };


    // 循环匹配 pattrens, 匹配成功则退出
    int sql_type;
    parser_analysis_sql_type(sql_type);

}

/**
 * 执行解析
 * @param sql
 * @return
 */
char *parser_exec(char *sql) {

    parser_match(sql);

    return "创建数据库成功";
}

/**
 * 解析器初始化
 * @return
 */
int parser_init() {

    /*例子: const char *pattern = "^3w.*cd$";
    const char *sql = "3w     cd";
    printf("匹配的结果是 : %d \n" , matchRegex(pattern, sql));*/
}




