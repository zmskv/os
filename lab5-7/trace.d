#pragma D option quiet


pid$target::create_node:entry
{
    printf("create_node called with ID = %d, Parent ID = %d\n", arg0, arg1);
}

pid$target::add_node:entry
{
    printf("add_node called with ID = %d, Parent ID = %d\n", arg0, arg1);
}

pid$target::find_node:entry
{
    printf("find_node called with ID = %d\n", arg0);
}

pid$target::handle_create_command:entry
{
    printf("handle_create_command called with command: %s\n", stringof(arg0));
}

pid$target::handle_exec_command:entry
{
    printf("handle_exec_command called with command: %s\n", stringof(arg0));
}

pid$target::handle_ping_command:entry
{
    printf("handle_ping_command called with command: %s\n", stringof(arg0));
}

pid$target::create_node:return
{
    printf("create_node returned\n");
}

pid$target::add_node:return
{
    printf("add_node returned\n");
}

pid$target::find_node:return
{
    printf("find_node returned\n");
}
