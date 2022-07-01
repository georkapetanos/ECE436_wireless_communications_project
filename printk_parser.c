#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INITIAL_SIZE 65536
#define CPU_BASE_FREQUENCY 3400000000

typedef struct reading {
	unsigned short sequence;
	unsigned long long int rdtsc;
} readingT;

typedef struct array_data{
	readingT **rx;
	readingT **tx;
	readingT **diff;
	unsigned int rx_array_size;
	unsigned int tx_array_size;
	unsigned int diff_size;
	unsigned int rx_last_used;
	unsigned int tx_last_used;
} arrayT;

/* initial implementation of searching ICMP reply frame,
ICMP sequence number is a 2 byte integer, 2^16 = 65536 
maximum number, after this sequence becomes again 0 (or
possible 1).
*/
int search_rx(arrayT array, unsigned short sequence) {
	int i;
	
	for(i = 0; i < array.rx_last_used; i++) {
		if(array.rx[i]->sequence == sequence) {
			return i;
		}
	}
	
	return -1;
}

arrayT import(char filename[], arrayT array) {
	char line[256];
	char *occur = NULL;
	char sequence[4];
	
	FILE *filestream = fopen(filename, "r");
	
	if (filestream == 0) { //check if file doesn't exist
		printf("File %s doesn't exist.\n", filename);
		return array;
	}
	if (ferror(filestream)) { //ferror returns zero when no error
		printf("\nError opening %s.\n", filename);
		clearerr(filestream);
		return array;
	}
	
	while(1) {
		if(fgets(line, 256, filestream) == NULL) { //read file line by line
			break;
		}
		//printf("%s", line);
		occur = strchr(line, ']');
		if(occur == NULL) { //check if line with no data
			break;
		}
		if(strncmp(occur + 2, "mac80211", 8) == 0) {
			//tx packet
			occur = strchr(line, '=');
			occur = strchr(occur + 1, '=');
			if(strncmp(occur + 1, "01", 2) == 0) {
				printf("ICMP request packet ");
				
				if(array.tx_array_size == array.tx_last_used) {	//doubling array when it fills up
						array.tx = (readingT **) realloc(array.tx, 2*array.tx_array_size*sizeof(readingT));
						array.tx_array_size = 2*array.tx_array_size;
				}

				array.tx[array.tx_last_used] = (readingT * ) malloc(sizeof(readingT));
				occur = strchr(occur + 1, '=');
				strncpy(sequence, occur + 1, 4);
				array.tx[array.tx_last_used]->sequence = (unsigned short) strtol(sequence, NULL, 16);
				printf("sequence = %d ", array.tx[array.tx_last_used]->sequence);
				occur = strchr(occur + 1, '=');
				array.tx[array.tx_last_used]->rdtsc = strtoull(occur + 1, NULL, 10);
				printf("rdtsc = %llu\n", array.tx[array.tx_last_used]->rdtsc);
				array.tx_last_used++;
			}
		} 
		else if(strncmp(occur + 2, "ath9k", 5) == 0) {
			//rx packet
			occur = strchr(line, '=');
			occur = strchr(occur + 1, '=');
			if(strncmp(occur + 1, "01", 2) == 0) {
				printf("ICMP reply packet ");
				
				if(array.rx_array_size == array.rx_last_used) {	//doubling array when it fills up
						array.rx = (readingT **) realloc(array.rx, 2*array.rx_array_size*sizeof(readingT));
						array.rx_array_size = 2*array.rx_array_size;
				}

				array.rx[array.rx_last_used] = (readingT * ) malloc(sizeof(readingT));
				occur = strchr(occur + 1, '=');
				strncpy(sequence, occur + 1, 4);
				array.rx[array.rx_last_used]->sequence = (unsigned short) strtol(sequence, NULL, 16);
				printf("sequence = %d ", array.rx[array.rx_last_used]->sequence);
				occur = strchr(occur + 1, '=');
				array.rx[array.rx_last_used]->rdtsc = strtoull(occur + 1, NULL, 10);
				printf("rdtsc = %llu\n", array.rx[array.rx_last_used]->rdtsc);
				array.rx_last_used++;
			}
		}
	}
	
	fclose(filestream);
	
	return array;
}

arrayT calculate_difference(arrayT array) {
	int i, found;
	
	if(array.diff != NULL) {
		for(i = 0; i < array.diff_size; i++) {
			free(array.diff[i]);
		}
		free(array.diff);
	}
	
	array.diff = (readingT **) malloc(array.tx_last_used * sizeof(readingT));
	array.diff_size = 0;
	for(i = 0; i < array.tx_last_used; i++) {
		array.diff[array.diff_size] = (readingT * ) malloc(sizeof(readingT));
		array.diff[i]->sequence = array.tx[i]->sequence;
		
		found = search_rx(array, array.tx[i]->sequence);
		if(found != -1) {
			array.diff[i]->rdtsc = array.rx[found]->rdtsc - array.tx[i]->rdtsc;
		}
		else {
			array.diff[i]->rdtsc = -1;
		}
		printf("diff = %llu\n", array.diff[i]->rdtsc);
		array.diff_size++;
	}

	return array;
}

void calculate_stats(arrayT array) {
	int i;
	unsigned long long int reduct = 0;

	if(array.diff == NULL) {
		printf("Difference array is empty, execute calculate_difference first\n");
	}
	else {
		for(i = 0; i < array.diff_size; i++) {
			if(array.diff[i]->rdtsc == -1) {
				printf("diff rdtsc is -1, skipping...\n");
				continue;
			}
			reduct += array.diff[i]->rdtsc;
		}
		printf("Mean RTT is %.15Lf seconds\n", (long double) reduct / array.diff_size / CPU_BASE_FREQUENCY);
	}
}

void export(arrayT array, char filename[]) {
	FILE *filestream = NULL;
	char destination[256];
	char str[18];
	int i;
	long double time = 0;
	
	strcpy(destination, filename);
	strcat(destination, ".difference");
	filestream = fopen(destination, "w+");
	
	if (filestream == 0) {
		printf("File %s already exists.\n", destination);
	}
	else {
		for(i = 0; i < array.diff_size; i++) {
			if(array.diff[i]->rdtsc == -1) {
				printf("diff rdtsc is -1, skipping...\n");
				continue;
			}
			time = ((long double) array.diff[i]->rdtsc) / CPU_BASE_FREQUENCY;
			printf("time = %.15Lf\n", time);
			sprintf(str, "%.15Lf", time);
			fwrite(str, 17, 1, filestream);
			if(i != (array.diff_size - 1)) {
				fwrite(",", 1, 1, filestream);
			}
		}
		fflush(filestream);
		fclose(filestream);
	}
}

int main(int argc, char *argv[]) {
	char command;
	arrayT array;
	
	if(argc < 2) {
		printf("Usage: ./printk_parser filename.txt\n");
		return 0;
	}

	array.rx = (readingT **) malloc(INITIAL_SIZE * sizeof(readingT));
	array.tx = (readingT **) malloc(INITIAL_SIZE * sizeof(readingT));
	array.rx_array_size = INITIAL_SIZE;
	array.tx_array_size = INITIAL_SIZE;
	array.rx_last_used = 0;
	array.tx_last_used = 0;
	array.diff = NULL;
	
	printf("Enter command: (i, c, e, g, p, h, q)\n");
	while(1) {
		scanf(" %c", &command);
		
		switch(command) {
			case 'i':
				array = import(argv[1], array);
				break;
			case 'c':
				array = calculate_difference(array);
				break;
			case 'g':
				printf("Tx size: %d\nRx size: %d\n", array.tx_last_used, array.rx_last_used);
				break;
			case 'e':
				export(array, argv[1]);
				break;
			case 's':
				calculate_stats(array);
				break;
			/*case 'p':
				plot_array(filename);
				break;*/
			case'h':
				printf("i -> import file\nc -> calculate timestamp difference\ng -> print import statistics\ne -> export to matlab readable file\ns -> calculate timestamp stats\nh -> help message\nq -> quit\n");
				break;
			case 'q':
				printf("Quitting...\n");
				return 0;
			default:
				printf("Invalid command\n");
				break;
		}
	}

	return 0;
}
