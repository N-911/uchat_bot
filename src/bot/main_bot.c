#include "uchat.h"


static void init_info(t_client_info *info, int argc, char *argv[]) {
    (*info).argc = argc;
    (*info).argv = argv;
    (*info).ip = argv[1];
    (*info).login = argv[3];
    (*info).login_nik = argv[4];
    (*info).port = (uint16_t) atoi(argv[2]);
    (*info).tls_client = NULL;
    (*info).responce = 0;
    (*info).can_record = 1;
}

int main(int argc, char **argv) {
    t_client_info *info = NULL;

//    if (argc != 3) {
//        mx_printerr("usage: uchat [ip_adress] [port]\n");
//        return -1;
//    }
    info = (t_client_info *)malloc(sizeof(t_client_info));
    memset(info, 0, sizeof(t_client_info));
    init_info(info, argc, argv);
//    create_download_folder();
    if (mx_start_client(info)) {
        printf("error = %s\n", strerror(errno));
        return -1;
    }
    printf("exit client\n");
    return 0;
}
