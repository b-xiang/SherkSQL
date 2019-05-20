#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <SherkMechanism/src/module/variable_master/variable_master.h>
#include <SherkSystem/src/module/logclerk/logclerk.h>
#include <SherkSystem/src/include/const/log_level.h>


extern Variable_Master_Session_Variables variable_master_session_variables;

void interviewer_call_variable_master_memory_sql(char *sql, int sql_category){

    VariableMaster_SQLContainer sql_container;
    variable_master_init_sql(&sql_container);
    size_t sql_len = strlen(sql);

    sql_container.sql = sql;
    sql_container.sql_len = sql_len;
    sql_container.sql_category = sql_category;

    variable_master_memory_sql(sql_container);

    char log[1000];memset(log,'\0', sizeof(char)*1000);// 开100的时候有点小，导致sprintf出错
    sprintf(log, "sql:%s, sql_len:%d, sql_category:%d \n", sql, (int)sql_len, sql_category);
    logclerk_write(LOGCLERK_LOG_LEVEL_SQL_QUERY, log);
}