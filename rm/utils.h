#ifndef __RICK_AND_MORTY_UTILS_H__
#define __RICK_AND_MORTY_UTILS_H__

#include <stddef.h>
#include <stdint.h>
#include <json-c/json.h>
#include "types.h"

#define CURL_ERR(f) if (f != CURLE_OK)

#define JSON_GET_OBJ_STR(O, S,  N)                                  \
    do {                                                            \
        json_object *_tmp = NULL;                                   \
        if (json_object_object_get_ex(O, #N, &_tmp)) {              \
            if (json_object_get_type(_tmp) == json_type_string) {   \
                S->N = strdup(json_object_get_string(_tmp));        \
            }                                                       \
        }                                                           \
    } while (0)

#define JSON_GET_OBJ_UINT(O, S, T, N)                           \
    do {                                                        \
        json_object *_tmp = NULL;                               \
        if (json_object_object_get_ex(O, #N, &_tmp)) {          \
            if (json_object_get_type(_tmp) == json_type_int) {  \
                S->N = json_object_get_##T(_tmp);               \
            }                                                   \
        }                                                       \
    } while (0)


#define JSON_GET_OBJ_DICT(O, S, N)                                                                  \
    do {                                                                                            \
        json_object *_tmp = NULL;                                                                   \
        if (json_object_object_get_ex(O, #N, &_tmp)) {                                              \
            if (json_object_get_type(_tmp) == json_type_object) {                                   \
                S->N.name = strdup(json_object_get_string(json_object_object_get(_tmp, "name")));   \
                S->N.url = strdup(json_object_get_string(json_object_object_get(_tmp, "url")));     \
            }                                                                                       \
        }                                                                                           \
    } while (0)                                                                                                 

#define NULLED_FREE(x) \
    do { free((x)); (x) = NULL; } while (0)
    
#define FREE_IF_NONULL(x) \
    do { if ((x)) NULLED_FREE((x)); } while (0)
    
#define APPEND_FILTER(f, u, n)                  \
    do {                                        \
        if (f->n)                               \
            u = url_add_filter(u, #n, f->n);    \
    } while (0)

json_object *put_inside_json_array(json_object *jobj);
char *url_add_filter(char *restrict url, const char *restrict key, const char *restrict value);
char *make_url_page(const char *restrict endpoint, size_t page);
char *make_url_id_list(const char *restrict endpoint, uint64_t array_id[], size_t array_len);
char *make_url_id(const char *restrict endpoint, uint64_t id);
uint64_t count_digit_in_number(uint64_t i);
char *num_to_str(uint64_t n);
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

#endif /* __RICK_AND_MORTY_UTILS_H__ */
