#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

int	fsize(char *file) { 
  struct stat	stats;

  if (stat(file, &stats) == 0)
    return (stats.st_size);
  return (0);
}

int	fexist(char *path) {
    struct stat stats;
    return (!stat(path, &stats));
}

int getchmod(char *fil) {
    int oct;
    struct stat stats;

    stat(fil, &stats);
    oct = stats.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    return (oct);
}

int copyx(char *input, char *output, long long dec) {
    int c;
    long long i;
    long long sz = fsize(input);
    FILE *inp = fopen(input, "r");
    FILE *out = fopen(output, "a");

    if (inp && out) {
        while ((c = fgetc(inp)) != EOF && i < sz) {
            if (!(i % 1000000)) {
                printf(">  %lldMb / %lldMb\r", i / 1000000, sz / 1000000);
                fflush(stdout);
            }
            char comp = c + dec;
            fputc((int)comp, out);
            i++;
        }
    }
    fclose(out);
    fclose(inp);
    printf("\n");
    return (fsize(output));
}

int launch(char *output, char *file, long long key) {
    int perms;
    FILE *try = fopen(file, "r");;

    if (!try || !fexist(file)) {
        printf("\nCannot open %s try sudo\n", file);
        return (0);
    }
    if (fexist(output)) {
        printf("\n%s already exists\n", output);
        return (0);
    }
    perms = getchmod(file);
    if (!copyx(file, output, key)) {
        printf("\nError copying try sudo\n");
        return (0);
    }
    if (chmod(output, perms) == -1) {
        printf("\nCannot apply permission\n");
        return (0);
    }
    return (42);
}

int main(int ac, char **av) {
    if (ac < 3 || ac > 4) {
        printf("\nUsage: %s file output\n", av[0]);
        return (-1);
    }
    if ((ac == 4 && !launch(av[2], av[1], atoll(av[3]))) || (ac == 3 && !launch(av[2], av[1], 0)))
        return (-1);
    return (0);
}
