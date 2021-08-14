#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int fd;

	fd = open("/dev/chrdev0", O_RDWR, 0666);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	sleep(5);

	close(fd);

	return 0;
}
