#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	long long unsigned int tick1, tick2; //unsigned 64 bit quantity
	unsigned int c,d;

	asm volatile("rdtsc" : "=a" (c), "=d" (d)); //assembly code running the instruction rdtsc
	tick1 = (((uint64_t)c) | (((uint64_t)d) << 32)); // calculating the tick value.

	//printf("time %llu\n",tick1);
	usleep(1000000);

	asm volatile("rdtsc" : "=a" (c), "=d" (d)); //assembly code running the instruction rdtsc
	tick2 = (((uint64_t)c) | (((uint64_t)d) << 32)); // calculating the tick value.

	printf("time %llu\n",tick2);
	printf("diff = %llu\n", tick2 - tick1);
}
