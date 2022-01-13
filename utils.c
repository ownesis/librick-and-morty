#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include "utils.h"
#include "api.h"

char *url_add_filter(char *restrict url, const char *restrict key, const char *restrict value) {
    size_t url_len = strlen(url);
    size_t key_len = strlen(key);
    char *value_encoded = NULL;
    size_t value_enc_len = 0;
    size_t total_len = 0;
    char *new_url = NULL;

    value_encoded = curl_easy_escape(NULL, value, strlen(value));

    if (!value_encoded) 
        return NULL;

    value_enc_len = strlen(value_encoded);

    /* 1: '&'
     * 1: '='
     * */
    total_len = (url_len
            + key_len
            + value_enc_len
            + 2);

    new_url = malloc(total_len+1);

    if (!new_url) {
        free(url);
        free(value_encoded);
        return NULL;
    }

    if (!memset(new_url, 0, total_len+1)) {
        free(url);
        free(value_encoded);
        free(new_url);
        return NULL;
    }

    if (!strcpy(new_url, url)) {
        free(url);
        free(value_encoded);
        free(new_url);
        return NULL;
    }

    new_url[url_len] = '&';

    if (!strcat(new_url, key)) {
        free(url);
        free(value_encoded);
        free(new_url);
        return NULL;
    }

    new_url[url_len+1+key_len] = '=';

    if (!strcat(new_url, value_encoded)) {
        free(url);
        free(value_encoded);
        free(new_url);
        return NULL;
    }
    
    free(value_encoded);
    free(url);

    return new_url;
}

json_object *put_inside_json_array(json_object *jobj) {
    json_object *arr_obj = NULL;

    if (!jobj)
        return NULL;

    /* Create an json array and add jobj inside */
    arr_obj = json_object_new_array_ext(1);

    if (!arr_obj)
        return NULL;

    json_object_array_add(arr_obj, jobj);

    return arr_obj;
}

char *make_url_page(const char *restrict endpoint, size_t page) {
    char *url = NULL;
    char *page_str = NULL;
    size_t page_strlen = 0;
    size_t endpoint_len = 0;
    size_t total_len = 0;


    endpoint_len = strlen(endpoint);
    page_str = num_to_str(page);
    page_strlen = strlen(page_str);
    
    total_len = (endpoint_len
            + page_strlen
            + sizeof(RM_API_ARG_PAGE)
            + 1);

    url = malloc(total_len);
    
    if (!url) {
        free(page_str);
        return NULL;
    }

    if (!memset(url, 0, total_len)) {
        free(page_str);
        free(url);
        return NULL;
    }

    if (!strcat(url, endpoint)) {
        free(page_str);
        free(url);
        return NULL;
    }
 
    if (!strcat(url, RM_API_ARG_PAGE)) {
        free(page_str);
        free(url);
        return NULL;
    }  

    if (!strcat(url, page_str)) {
        free(page_str);
        free(url);
        return NULL;
    }

    free(page_str);

    return url;
}

char *make_url_id_list(const char *restrict endpoint, uint64_t array_id[], size_t array_len) {
    char *url = NULL;
    char *tmp = NULL;
    char *id_str = NULL;
    size_t id_strlen = 0;
    size_t url_len = 0;

    if (array_len <= 0 || !array_id || !endpoint)
        return NULL;
    
    url = make_url_id(endpoint, array_id[0]);

    if (!url)
        return NULL;

    for (size_t i = 1; i < array_len; i++) {
        id_str = num_to_str(array_id[i]);
        url_len = strlen(url);
        id_strlen = strlen(id_str);

        tmp = realloc(url, url_len+id_strlen+2);

        if (!tmp) {
            free(url);
            free(id_str);
            return NULL;
        }

        tmp[url_len] = ',';

        if (!memcpy(&tmp[url_len]+1, id_str, id_strlen)) {
            free(tmp);
            free(id_str);
            return NULL;
        }
    
        tmp[url_len+id_strlen+1] = '\0';

        url = tmp;

        free(id_str);
    }

    return url;
}

char *make_url_id(const char *restrict endpoint, uint64_t id) {
    char *id_str = NULL;
    char *url = NULL;
    size_t id_len = 0;
    size_t endpoint_len = 0;
    
    if (!endpoint || id <= 0)
        return NULL;

    endpoint_len = strlen(endpoint);

    id_str = num_to_str(id);
    
    if (!id_str) 
        return NULL;

    id_len = strlen(id_str);
    
    url = malloc(endpoint_len+id_len+1);

    if (!url) {
        free(id_str);
        return NULL;
    }
    
    if (!memset(url, '\0', endpoint_len+id_len+1)) {
        free(id_str);
        free(url);
        return NULL;
    }

    if (!strcpy(url, endpoint)) {
        free(id_str);
        free(url);
        return NULL;
    }

    if (!strcat(url, id_str)) {
        free(id_str);
        free(url);
        return NULL;
    }

    free(id_str);
    
    return url;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = (size * nmemb);
    struct Mem *chunk = (struct Mem *)userdata;
    char *tmp = realloc(chunk->ptr, chunk->size+total_size+1);

    if (!tmp)
        return 0;

    chunk->ptr = tmp;
    memcpy(chunk->ptr+chunk->size, ptr, total_size);
    chunk->size += total_size;

    return total_size;
}

uint64_t count_digit_in_number(uint64_t i) {
    uint64_t ret = 1;

    while (i /= 10) ret++;

    return ret;
}

char *num_to_str(uint64_t n) {
    size_t buf_size = 0;
    char *buf = NULL;   
    int ret;

    buf_size = count_digit_in_number(n);
    
    buf = calloc(buf_size+1, sizeof(char));
    
    if (!buf) 
        return NULL;

    ret = snprintf(buf, buf_size+1, "%lu", n);

    if (ret < 0) {
        free(buf);
        return NULL;
    }

    return buf;
}
