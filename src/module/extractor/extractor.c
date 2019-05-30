#include <stdio.h>
#include <stdlib.h>
#include <SherkService/mechanism/module/grocery/grocery.h>
#include <SherkSQL/src/module/analyst/analyst.h>
#include <SherkService/test/module/test_sql/test_sql.h>
#include <SherkSupport/src/include/serialize/tobe_serialized_structs.h>
#include <SherkSupport/src/module/skeleton/skeleton.h>


/**
 *
 * @param sql
 * @param count
 * @param field_name_list
 * @param field_type_list
 * @param field_value_int
 * @param field_value_float
 * @param field_value_char
 * @param field_value_string
 */
void
extractor_extract_update_sql(char *sql, int *count, char **field_name_list, int *field_type_list, int *field_value_int,
                             float *field_value_float,
                             char *field_value_char, char **field_value_string) {

}

/**
 * extractor_extract_insert_sql
 * @param sql
 * @param field_name_list
 * @param field_value_int
 * @param field_value_float
 * @param field_value_char
 * @param field_value_string
 */
void
extractor_extract_insert_sql(char *sql, int *count, char **field_name_list, int *field_type_list, int *field_value_int,
                             float *field_value_float,
                             char *field_value_char, char **field_value_string) {

    //char **field_name_list = (char **) malloc(sizeof(char **) * 10);
    //int  *field_type_list = (int *) malloc(sizeof(int*) * 10);
    //int *field_value_int = (int *) malloc(sizeof(int *) * 10);
    //float *field_value_float = (float *) malloc(sizeof(float *) * 10);
    //char *field_value_char = (char *) malloc(sizeof(char *) * 10);
    //char **field_value_string = (char **) malloc(sizeof(char **) * 10);
    //int count = 0;

    // SQL中的字段名列表和字段值列表
    char *field_name_list_sql = grocery_string_trim(
            grocery_string_delete_quote(grocery_string_crimp(sql, '(', 1, ')', 1)));
    char *field_value_list_sql = grocery_string_trim(grocery_string_crimp(sql, '(', 2, ')', 2));

    // 字段个数
    *count = grocery_char_in_string_appear_times(field_name_list_sql, ',') + 1;

    printf("sql:%s--------\n",sql);
    printf("field_name_list_sql:%s--------\n",field_name_list_sql);
    printf("field_value_list_sql:%s--------\n",field_value_list_sql);

    // printf("field_name_list_sql:%s\n", field_name_list_sql);
    // printf("field_value_list_sql:%s\n", field_value_list_sql);

    // 字段名
    int i, field_count = *count;
    char *field_name, *field_value;

    // 循环处理
    if (1 == field_count) {

    } else {
        for (i = 0; i <= field_count - 1; ++i) {

            if (0 == i) {

                int end = grocery_string_get_str_first_char_index(field_name_list_sql, ",", 0) - 1;
                field_name = grocery_string_trim(grocery_string_cutwords(field_name_list_sql, 0, end));

                end = grocery_string_get_str_first_char_index(field_value_list_sql, ",", 0) - 1;
                field_value = grocery_string_trim(grocery_string_cutwords(field_value_list_sql, 0, end));
            } else if (field_count - 1 == i) {

                int start = grocery_string_get_char_in_string_last_appear_index(field_name_list_sql, ',', 0) + 1;
                field_name = grocery_string_trim(&(field_name_list_sql[start]));

                start = grocery_string_get_char_in_string_last_appear_index(field_value_list_sql, ',', 0) + 1;
                field_value = grocery_string_trim(&(field_value_list_sql[start]));
            } else {

                field_name = grocery_string_trim(grocery_string_crimp(field_name_list_sql, ',', i, ',', i + 1));

                field_value = grocery_string_trim(grocery_string_crimp(field_value_list_sql, ',', i, ',', i + 1));
            }

            field_name_list[i] = field_name;
            field_type_list[i] = analyst_analysis_field_type_by_field_value_string(field_value);

            if (FIELD_TYPE_STRING == field_type_list[i]) {

                field_value_string[i] = grocery_string_trim(grocery_string_delete_quote(field_value));
            } else if (FIELD_TYPE_FLOAT == field_type_list[i]) {

                field_value_float[i] = atof(field_value);
            } else if (FIELD_TYPE_CHAR == field_type_list[i]) {

                field_value_char[i] = grocery_string_delete_quote(field_value);
            } else {

                // 整型处理
                field_value_int[i] = atoi(field_value);
            }
        }
    }

    // test_sql_print_field_name_list(field_name_list, field_count);
    // test_sql_print_field_type_list(field_type_list, field_count);
    // test_sql_print_field_value_list(field_count, field_name_list, field_type_list, field_value_int, field_value_float,
    //                                 field_value_char, field_value_string);
}

void extractor_extract_create_sql(char *sql, char **field_name_list, int *field_type_list, int *field_count_pointer) {

    // char *sql = "create table test(id int, name string, age string)";

    // 字段数
    *field_count_pointer = grocery_char_in_string_appear_times(sql, ',') + 1;

    int field_count = *field_count_pointer;

    // 字段名列表, 字段类型列表
    // char **field_name_list = (char **) malloc(sizeof(char **) * 100);
    // int *field_type_list = (int *) malloc(sizeof(int *) * 100);

    // 开始循环获取
    int i;
    char *field_definition; // 每一个字段的定义
    char *field_type; // 字段类型
    char *field_value;
    if (1 == field_count) {

        field_definition = grocery_string_crimp(sql, '(', 1, ')', 1);
        field_name_list[0] = grocery_string_split_by_whitespace_into_two_part(field_definition)[0];
        field_type = grocery_string_split_by_whitespace_into_two_part(field_definition)[1];
        field_type_list[0] = analyst_analysis_field_type_by_field_type_string(field_type);

    } else {

        for (i = 0; i <= field_count - 1; ++i) {

            if (0 == i) { // 获取第1个字段定义的时候

                field_definition = grocery_string_crimp(sql, '(', 1, ',', 1);
            } else if (field_count - 1 == i) { // 获取最后1个字段定义的时候

                field_definition = grocery_string_crimp(sql, ',', i, ')', 1);
            } else {

                field_definition = grocery_string_crimp(sql, ',', i, ',', i + 1);
            }
            field_definition = grocery_string_trim(field_definition);

            field_name_list[i] = grocery_string_split_by_whitespace_into_two_part(field_definition)[0];
            field_type_list[i] = analyst_analysis_field_type_by_field_type_string(
                    grocery_string_split_by_whitespace_into_two_part(field_definition)[1]);
        }
    }

    // test_sql_print_field_name_list(field_name_list, field_count);
    // test_sql_print_field_type_list(field_type_list, field_count);
}



