#include <stdio.h>
#include <stdlib.h>
#include <SherkMechanism/src/module/variable_master/variable_master.h>


char *executor_handle_command_login(char *name, char *password) {

    VariableMaster_SQLContainer sql_container;
    variable_master_init_sql(&sql_container);
    sql_container.command = "login";
    variable_master_memory_sql(sql_container);

}





