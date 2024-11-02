#!/usr/sbin/dtrace -s


syscall::write:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called write() on fd %d\n", execname, arg0);
    printf("Data: %s\n", copyinstr(arg1));
}


syscall::fork:entry
/execname == "parent"/
{
    printf("Process %s called fork()\n", execname);
}


syscall::close:entry
/execname == "parent" || execname == "child"/
{
    if (arg0 == 4 || arg0 == 5){
        printf("Process %s called close() on fd %d\n", execname, arg0);
    }
}


syscall::mmap:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called mmap()\n", execname);
}


syscall::munmap:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called munmap()\n", execname);
}


syscall::shm_open:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called shm_open()\n", execname);
}


syscall::shm_unlink:entry
/execname == "parent" || execname == "child"/
{
    printf("Process %s called shm_unlink()\n", execname);
}


syscall::ftruncate:entry
/execname == "parent"/
{
    printf("Process %s called ftruncate()\n", execname);
}


syscall::open:entry
/execname == "child"/
{
    printf("Process %s called open()\n", execname);
}