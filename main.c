volatile char *uart = (volatile char *)0x10009000;
const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	const char *p = (const char *)&logo[0];
	while (*p) {
		*uart = *p++;
	}
	for(;;);
	return 0;
}
