#!/usr/sbin/dtrace -s

pid$target::dlopen:entry
/execname == "main_1" || execname == "main_2"/
{
    printf("Process %s called dlopen() on fd %d\n", execname, arg0);
    ustack();
}

pid$target::dlsym:entry
/execname == "main_1" || execname == "main_2"/
{
    printf("Process %s called dlsym() on fd %d\n", execname, arg0);
    ustack();
}

pid$target::dlclose:entry
/execname == "main_1" || execname == "main_2"/
{
    printf("Process %s called dlclose() on fd %d\n", execname, arg0);
    ustack();
}
