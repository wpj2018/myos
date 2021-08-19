extern int printk(const char *, ...);
const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	printk("%s", &logo[0]);
	for(;;);
	return 0;
}
