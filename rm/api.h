#ifndef __RICK_AND_MORTY_API_H__
#define __RICK_AND_MORTY_API_H__

/** 
 * \file rm_api.h
 * \author ownesis
 * \date 9 janv. 2022
 * \brief Header file for Functions and structure for the context
 * */

#include <json-c/json.h>
#include "types.h"

/**
 * \def RM_API_ENDPOINT 
 * */
#define RM_API_ENDPOINT "https://rickandmortyapi.com/api"

/**
 * \def RM_API_ARG_PAGE
 * */
#define RM_API_ARG_PAGE "?page="

#define rm_entity_da_clear(p, e)                    \
    do {                                            \
        if ((p) && (p)->da_##e) {                   \
            for (size_t i = 0; i < (p)->len; i++)   \
                rm_##e##_clear((p)->da_##e[i]);     \
            free((p)->da_##e);                      \
            free((p));                              \
        }                                           \
    } while (0)

/**
 * \struct Info rm_api.h
 * \brief structure for Info page.
 *
 * struct Info have typedef : rm_info_t
 * */
struct Info {
    size_t count;
    size_t pages;
    char *next;
    char *prev;
};
typedef struct Info rm_info_t;

void *rm_entity_get(uint64_t id, rm_entity_t e_entity);
void *rm_entity_get_page(rm_info_t *info, uint64_t page, void *filter, rm_entity_t e_entity);
void *rm_entity_get_all(void *filter, rm_entity_t e_entity);
void *rm_entity_get_list(uint64_t array_id[], size_t array_len, rm_entity_t e_entity);

void *parse_entity_array(json_object *jobj, int e_entity);
void *parse_entity_list(json_object **jobj, rm_entity_t e_entity);
void *parse_entity_all(rm_info_t *info, json_object *jobj, rm_entity_t e_entity);

mem_t rm_api_request(const char *restrict endpoint);

_Bool parse_all_info(rm_info_t *info, json_object *jobj);

/**
 * \fn void rm_all_info_clear(rm_info_t *info)
 * \param[in] info (rm_info_t *) Address of rm_info_t structure to free.
 * */
void rm_all_info_clear(rm_info_t *info);

#endif /* __RICK_AND_MORTY_API_H__ */
