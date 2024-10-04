#!/usr/sbin/dtrace -s

syscall::write:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called write() on fd %d\n", execname, arg0);
    printf("Data: %s\n", copyinstr(arg1));
}

syscall::read:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called read() on fd %d\n", execname, arg0);
}

syscall::fork:entry
/execname == "parent"/
{
    printf("Process %s called fork()\n", execname);
}

syscall::execve:entry
/execname == "child"/
{
    printf("Process %s called execve()\n", execname);
}

syscall::pipe:entry
/execname == "parent"/
{
    printf("Process %s called pipe()\n", execname);
}

syscall::close:entry
/execname == "parent" || execname == "child"/
{
    if (arg0 == 4 || arg0 == 5){
        printf("Process %s called close() on fd %d\n", execname, arg0);
    }
}
