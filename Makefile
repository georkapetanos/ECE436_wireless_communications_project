all: parser tsc_example

parser:
	gcc -Wall -g printk_parser.c -o printk_parser

tsc_example:
	gcc -Wall -g rdtsc_example.c -o rdtsc_example

clean:
	rm rdtsc_example printk_parser
