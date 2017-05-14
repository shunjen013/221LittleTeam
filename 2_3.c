#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define PAGE_SIZE 4096
#define RAM_SIZE 8589934592

int main(int argc, char ** argv) {
  int fd = open(argv[1], O_RDONLY);
  int fd2 = open(argv[2], O_RDONLY);
  struct stat stats;
  fstat(fd, &stats);
  posix_fadvise(fd, 0, stats.st_size, POSIX_FADV_DONTNEED);
  int j;
  int result = 0;
  char * map = (char *) mmap(NULL, 2*RAM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED) {
    perror("Failed to mmap");
    return 1;
  }
  else
    printf("mmap success!\n");
  unsigned long long int i;
  for (i = 0; i < 2*RAM_SIZE; i=i+RAM_SIZE/2) {
    printf("%llu\n", i);
    result += map[i];
  }
  munmap(map, 2*RAM_SIZE);

  close(fd);
  //printf("Size:%zu\n", stats.st_size);
  //printf("Result:%d\n", result);
  return result;
}