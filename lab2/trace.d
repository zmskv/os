#!/usr/sbin/dtrace -s


syscall:::entry
/execname == "quicksort"/
{
    printf("Process %s (PID %d) called syscall %s with args: ", execname, pid, probefunc);
    printf("\n");
}

syscall:::return
/execname == "quicksort"/
{
    printf("Process %s (PID %d) returned from syscall %s with return value: %d\n", execname, pid, probefunc, arg1);
   
}