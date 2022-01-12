#include <string.h>
#include <curl/curl.h>
#include "api.h"
#include "types.h"
#include "utils.h"

mem_t rm_api_request(const char *restrict endpoint) {
    CURL *curl_hndl = NULL;
    mem_t chunk = {NULL};
    long response_code = 0;

    if (!endpoint)
        return (mem_t){NULL};

    chunk.ptr = malloc(1);

    if (!chunk.ptr)
        return (mem_t){NULL};

    chunk.ptr[0] = 0;

    curl_hndl = curl_easy_init();

    if (!curl_hndl) {
        free(chunk.ptr);
        return (mem_t){NULL};
    }

    CURL_ERR(curl_easy_setopt(curl_hndl, CURLOPT_URL, endpoint)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};
    }
    
    CURL_ERR(curl_easy_setopt(curl_hndl, CURLOPT_WRITEFUNCTION, write_callback)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};
    }
    
    CURL_ERR(curl_easy_setopt(curl_hndl, CURLOPT_WRITEDATA, (void *)&chunk)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};  
    }
   
    CURL_ERR(curl_easy_perform(curl_hndl)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL}; 
    }
    
    CURL_ERR(curl_easy_getinfo(curl_hndl, CURLINFO_RESPONSE_CODE, &response_code)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};
    }

    curl_easy_cleanup(curl_hndl);

    if (response_code != 200) {
        free(chunk.ptr);
        return (mem_t){NULL};
    }

    chunk.ptr[chunk.size] = '\0';

    return chunk;
}

_Bool parse_all_info(rm_info_t *info, json_object *jobj) {
    JSON_GET_OBJ_UINT(jobj, info, uint64, count);
    JSON_GET_OBJ_UINT(jobj, info, uint64, pages);
    JSON_GET_OBJ_STR(jobj, info, next);
    JSON_GET_OBJ_STR(jobj, info, prev);

    return 1;
}

void clear_all_info(rm_info_t *info) {
    if (!info)
        return;

    FREE_IF_NONULL(info->next);
    FREE_IF_NONULL(info->prev);
}
