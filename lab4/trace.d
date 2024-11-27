#!/usr/sbin/dtrace -s


syscall::dlopen:entry
/execname == "main_1" || execname == "main_2"/
{
    printf("Process %s called dlopen() on fd %d\n", execname, arg0);
    printf("Data: %s\n", copyinstr(arg1));
}


syscall::E_Series:entry
/execname == "main_1" || execname == "main_2"/
{
    printf("Process %s called E_Series on fd %d\n", execname, arg0);
   
}


syscall::Pi_Leibniz:entry
/execname == "main_1" || execname == "main_2"/
{
    printf("Process %s called Pi_Leibniz\n", execname);
}


