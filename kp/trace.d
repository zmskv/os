#pragma D option quiet

pid$target::allocBuddy:entry
{
    printf("allocBuddy called with block_size = %d\n", arg1);
}

pid$target::freeBuddy:entry
{
    printf("freeBuddy called with address = %p\n", arg1);
}

pid$target::freeBuddy:return
{
    printf("freeBuddy returned\n");
}

pid$target::alloc:entry
{
    printf("alloc called with block_size = %d\n", arg1);
}

pid$target::freeBlock:entry
{
    printf("freeBlock called with address = %p\n", arg1);
}


