/* Here is where we try to write*/
void __attribute__ ((noinline))  trywrite(size_t pagesize) {
/*printf will write a little of the stack, but maybe not enough */
printf("Trying to write on stack\n");
/*So we allocate two pages size
void* ptr = alloca(2*pagesize);
/*And we try to use memset to write to it */
/*memset(ptr,1,2*pagesize);*/
/*If it works, we print success, otherwise the program crashes */
printf("Success\n");
}

/*There it is */
/* I marked it as noinline to make sure that the compiler to not optimize the function away */
/* that's more predictable */
void __attribute__ ((noinline)) protect(size_t pagesize) {
    /* Here we allocate two page size on the stack */
    /* The reason I use two pagesizes is that alloca is not aligned on a page */
    /*But mprotect works on a page by page basis */
    void* ptr = alloca(2*pagesize);
    /* Here I go to the immediate next page */
    /* Since I allocated two pages size, I am sure to be on a page completely allocated by alloca */
    void* pboundary = ptr+pagesize-(((unsigned long)ptr)%pagesize);
    /* Here we protect it, specifying only READ make it read only */
    /* If you specified EXEC, you would get the ability to execute code that is on the stack */
    mprotect(pboundary, pagesize, PROT_READ);
    /* Here we print the memory range that we just protected */
    printf("0x%lx-0x%lx is not read only\n",(unsigned long)pboundary,(unsigned long)(pboundary+pagesize-1));
    /*Then we try to write on the stack*/
    trywrite(pagesize);
    /*Here we are writing on new memory so that part succeeds */
}
int main(int argc, char**argv){
    /*Here we ger the page size from sysconf*/
    size_t pagesize=sysconf(_SC_PAGESIZE);

    protect(pagesize);
    /*this will cause a segmentation fault*/
    trywrite(pagesize);
    return 0;
}