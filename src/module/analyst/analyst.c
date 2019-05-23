#include <stdio.h>
#include <string.h>
#include <SherkSQL/src/module/parser/parser.h>
#include <SherkService/mechanism/module/grocery/grocery.h>

// 分析家

/**
 * 分析SQL分类
 */
int analyst_analysis_sql_category(char *sql) {

    char *pattern_command = "^\\s*command=.*$";
    char *pattern_sql = "^\\s*sql=.*$";

    if (parser_match_regex(pattern_command, sql)) {

        // 传来的SQL是command
        return SQL_CATEGORY_IS_SHERK_COMMAND;
    } else if (parser_match_regex(pattern_sql, sql)) {

        // 传来的SQL是SQL命令
        return SQL_CATEGORY_IS_SQL;
    } else {

        return -1;
    }
}


char *analyst_analysis_command_login_get_name(char *command) {

    int left = grocery_string_get_str_last_char_index(command, "name=", 1) + 1;
    int right = grocery_string_get_str_first_char_index(command, ", password", 1) - 1;
    char *name = grocery_string_cutwords(command, left, right);

    return name;
}

char *analyst_analysis_command_login_get_password(char *command) {

    char *pos = grocery_string_get_str_next_char_address(command, ", password=", 1);

    return pos;
}

char *analyst_analysis_sql_use_database_get_database_name(char *command) {

    char *pos = grocery_string_get_str_next_char_address(command, "database ", 1);

    return pos;
}

char *analyst_analysis_sql_create_database_get_database_name(char *command) {

    char *pos = grocery_string_get_str_next_char_address(command, "database ", 1);

    return pos;
}

char *analyst_analysis_sql_drop_table_get_table_name(char *command) {

    char *pos = grocery_string_get_str_next_char_address(command, "table ", 1);

    return pos;
}

char *analyst_analysis_sql_create_table_get_table_name(char *command) {

    char *pos = grocery_string_get_str_next_char_address(command, "table ", 1);

    return pos;
}

char **analyst_analysis_sql_create_table_get_field_name_list(char *command) {

}

int **analyst_analysis_sql_create_table_get_field_type_list(char *command) {

}
