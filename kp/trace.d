#pragma D option quiet


pid$target::freeBuddy:entry
{
    printf("freeBuddy called with address = %p\n", arg1);
}

pid$target::freeBuddy:return
{
    printf("freeBuddy returned\n");
}

pid$target::freeBlock:entry
{
    printf("freeBlock called with address = %p\n", arg1);
}


