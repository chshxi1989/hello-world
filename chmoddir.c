#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h> 

static int
change_mod_and_owner(char* dir_name, mode_t mode, uid_t owner, gid_t group) {
    if (dir_name == NULL) {
        printf("error, dir_name is NULL\n");
        return -1;
    }
    DIR * dir = NULL;
    struct dirent * ptr = NULL;
    dir = opendir(dir_name);
    char filepath[PATH_MAX] = "\0";
    if (dir == NULL) {
        printf("error, open %s failed\n", dir_name);
        return -1;
    }
    while((ptr = readdir(dir)) != NULL) {
        if( ptr->d_type & DT_DIR) {
            // directory
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
                 sprintf(filepath, "%s/%s", dir_name, ptr->d_name);
                 change_mod_and_owner(filepath, mode, owner, group);
            }
        } else {
            // file
            sprintf(filepath, "%s/%s", dir_name, ptr->d_name);
            if (chmod(filepath, mode) != 0) {
                printf("error, chmod %s failed\n", filepath);
            }
            if (chown(filepath, owner, group) != 0) {
                printf("error, chown %s failed\n", filepath);
            }
        }
    }
    return 0;
}

int
main(int argc, char** args) {
    change_mod_and_owner(args[1], S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH, 1000, 1000);
    return 0;
}