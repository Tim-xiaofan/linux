#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

const char* program_name;
const struct option long_opts[] = 
{
	{"help", 0, NULL, 'h'},
	{"output", 1, NULL, 'o'},
	{"version", 0, NULL, 'v'},
	{NULL, 0, NULL, 0}
};

const char *short_opts = "ho:v";

/*输出提示信息*/
void print_usage(FILE* stream, int exit_code)
{
	fprintf(stream, "Usage: %s options {intputfie...}\n", program_name);
	fprintf(stream, " -h --help.\n -o --output filename.\n -v version.\n");
	exit(exit_code);
}

int main(int argc, char *argv[])
{
	//printf("hello getopt_long\n");
	program_name = argv[0];
	int next_option = 0;
	const char *output_filename = NULL;
	do
	{
		next_option = getopt_long(argc, argv, short_opts, long_opts, NULL);
		switch(next_option)
		{
			case 'h':
				print_usage(stdout, 0);
				break;
			case 'o':
				output_filename = optarg;
				execl("/bin/cat", "cat", output_filename, NULL);
				break;
			case 'v':
				printf("the version is 1.0\n");
				break;
			case ':':
				printf("too few argment.\n");//参数不足
			case '?':
				print_usage(stderr, 1);//其他错误
				break;
			default:
				print_usage(stderr, 1);
				break;
		}
	}while(next_option != -1);
	return 0;	
}
