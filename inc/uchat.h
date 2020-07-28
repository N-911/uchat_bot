#ifndef UCHAT_H
#define UCHAT_H

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sqlite3.h>
#include <stdarg.h> 
#include <stdint.h>
#include <syslog.h>
#include <sys/event.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
//#include "mail.h"
//#include "audio.h"
//#include "../libportaudio/include/portaudio.h"
//#include "../libmx/inc/libmx.h"
#include "../libressl/include/tls.h"
#include "../libressl/include/openssl/evp.h"
#include "../libressl/include/openssl/sha.h"
#include "../libressl/include/openssl/aes.h"
#include "../libressl/include/openssl/conf.h"
#include "../libjson/json.h"



#define MAXSLEEP 24 
#define MAX_CLIENT_INPUT 1024
#define REPORT_FILENAME "server_log.txt"
#define BUFLEN 128
#define QLEN 10
#define HOST_NAME_MAX 256
#define MX_OK 0
#define MX_SAVE_FOLDER_IN_CLIENT "./Uchat_downloads/"
#define MX_SAVE_FOLDER_IN_SERVER "./files/"

#define KEY10 "rooms"

#define MX_MAX_FILE_SIZE 300000000
#define MX_MAX_USERS_IN_ROOM 1024

#define MX_EMAIL_POS 8613
#define MX_EMAIL_POS_2 348

#define MX_MSG_TYPE 1
#define MX_FILE_SEND_TYPE 2
#define MX_AUTH_TYPE 3
#define MX_AUTH_TYPE_V 4
#define MX_AUTH_TYPE_NV 5
#define MX_REG_TYPE 6
#define MX_REG_TYPE_V 7
#define MX_REG_TYPE_NV 8
#define MX_LOGOUT_TYPE 9
#define MX_LOAD_MORE_TYPE 10
#define MX_DELETE_MESSAGE_TYPE 11
#define MX_FILE_DOWNLOAD_TYPE 12
#define MX_EDIT_MESSAGE_TYPE 13
#define MX_LOAD_PROFILE_TYPE 14
#define MX_EDIT_PROFILE_TYPE 15
#define MX_LEAVE_ROOM_TYPE 16
#define MX_SEARCH_ALL_TYPE 17
#define MX_JOIN_ROOM_TYPE 18
#define MX_CREATE_ROOM_TYPE 19
#define MX_DIRECT_MESSAGE_TYPE 20
#define MX_DELETE_ACCOUNT_TYPE 21
#define MX_RECONNECTION_TYPE 22
#define MX_EMPTY_JSON 23
#define MX_PACKAGE_SIZE sizeof(t_package)
#define MX_MAX_MSG_SIZE 200




typedef struct  s_client_info {
    char *login;
    char *login_nik;
    char *password;
    int id;
    int visual;
    int audio;
    int argc;
    char **argv;
    char *ip;
    uint16_t port;
    int socket;
    struct tls *tls_client;
    int auth_client;
    int current_room;
    pthread_mutex_t mutex;
    int responce;
    int can_load;
    int editing;
    int editing_room;
    char *record_file;
    struct s_file_list *input_files;
    struct json_object *rooms;
    int input;
    int can_record;
}               t_client_info;


#define MX_PATH_TO_DB "./server_db.bin"

typedef struct  s_clients {
    pthread_t tid;
    int fd;
    char *login;
    char *password;
    char *err_msg;
    int channel;
    struct s_clients *next;
}               t_clients;

typedef struct s_work {
    int i;
    int *user_id;
    int *user_sock;
}               t_work;

typedef struct  s_server_info {  
    int argc;
    char **argv;
    char *ip;
    int kq;
    uint16_t port;
    struct s_clients *clients;
    struct s_socket_list *socket_list;
    struct s_file_list *input_files;
    sqlite3 *db;
    pthread_mutex_t mutex;
    struct s_work *wdb;
}               t_server_info;


typedef struct  s_file_tmp {
    pthread_mutex_t *mutex;
    struct tls *tls;
    char *file_name;
    int size;
    int file_id;
    int room_id;
}               t_file_tmp;

typedef struct  s_server_room {
    int room_id;
    char *name;
    struct  s_server_room *next;
}               t_server_room;

typedef struct  s_socket_list {
    int socket;
    struct tls *tls_socket;
    struct json_object *obj;
	struct json_tokener *tok;
    pthread_mutex_t mutex;
    struct s_socket_list *left;
    struct s_socket_list *right;
    struct s_socket_list *parent;
}               t_socket_list;

typedef struct  s_file_list {
    int id; 
    int file_size;
    FILE *file;
    char *file_name;
    struct s_file_list *next;
}               t_file_list;




/*Client*/
bool mx_pick_file_to_send(t_client_info *info, FILE **file, json_object **js);
int mx_start_client(t_client_info *info);
int mx_reconnect_client(t_client_info *info);
int mx_connect_client(t_client_info *info);
int mx_tls_config_client(t_client_info *info);
int mx_make_tls_connect_client(t_client_info *info);
int mx_authorization_client(t_client_info *info, char **login_for_exit);
void mx_process_message_in_client(t_client_info *info);
void *mx_process_input_from_server(void *i);
void mx_sleep_ms (int milliseconds);
void mx_load_profile_client(t_client_info *info, int id);
void mx_load_user_profile(t_client_info *info, json_object *new_json);
void mx_search_all_client(t_client_info *info, json_object *new_json);



/*Functions*/
int mx_detect_file_extention(char *filename);
int tls_send(struct tls *tls_socket, const char *content, int size);
void mx_print_curr_time(void);
char *mx_curr_time(void);
void mx_print_tid(const char *s);
void mx_err_exit(const char *err_msg);  // вивести помилку
int mx_err_return2(const char *err_msg, const char *err_msg2);
int mx_err_return3(const char *err_msg, const char *err_msg2, int value);
int mx_err_return(const char *err_msg);
void mx_sha_hash_password(char *password);
void mx_get_input(char *buffer);
int mx_get_input2(char *buffer);
void mx_report_tls(struct tls * tls_ctx, char * host);
void mx_print_client_address(struct sockaddr_storage client_address, 
                             socklen_t client_len);
char *mx_date_to_char(void);
/*Json*/
int mx_js_o_o_add(struct json_object *obj, const char *key,
                  struct json_object *val);
struct json_object *mx_js_o_o_get(const struct json_object *obj,
                                  const char *key);
json_bool mx_js_o_o_get_ex(const struct json_object *obj, const char *key,
                           struct json_object **value);
const char *mx_js_g_str(json_object *jso);
int mx_js_g_str_len(const struct json_object *obj);
int32_t mx_js_g_int(const struct json_object *obj);
struct json_object *mx_js_n_int(int32_t i);
int mx_js_s_int(struct json_object *obj, int new_value);
struct json_object *mx_js_n_o();
struct json_object *mx_js_n_str(const char *s);
const char *mx_js_o_to_js_str(struct json_object *obj);
char *mx_strhash(const char *to_hash);
char *mx_encrypt(char *str);
char *mx_decrypt(char *str);
json_object *mx_create_basic_json_object(int type);
void mx_print_json_object(struct json_object *jobj, const char *msg);
void mx_send_format(int socket, const char *text);
void mx_send_format_tls(struct tls *tls, char *arg1, char *arg2,
                        char *arg3);
int mx_connect_to_server(const char *hostname, const char *port);
struct tls *mx_create_tls(void);
int mx_check_response(const char *response);
int mx_wait_on_response(int socket, struct tls *tls, int reply_code);
void mx_printerr(const char *s);
int mx_get_char_index(const char *str, char c);

#endif
