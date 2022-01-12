#ifndef __RICK_AND_MORTY_API_H__
#define __RICK_AND_MORTY_API_H__

/* 
 * \file rm_api.h
 * \author ownesis
 * \date 9 janv. 2022
 * \brief Header file for Functions and structure for the context
 * */

#include <json-c/json.h>
#include "types.h"

/*
 * \def RM_API_ENDPOINT 
 * */
#define RM_API_ENDPOINT "https://rickandmortyapi.com/api"

/*
 * \def RM_API_ARG_PAGE
 * */
#define RM_API_ARG_PAGE "?page="

/*
 * \struct RM_ctx rm_api.h
 * \brief structure for the context.
 *
 * struct RM_ctx have typedef : rm_ctx_t
 * */
struct Info {
    size_t count;
    size_t pages;
    char *next;
    char *prev;
};
typedef struct Info rm_info_t;

mem_t rm_api_request(const char *restrict endpoint);
_Bool parse_all_info(rm_info_t *info, json_object *jobj);

void clear_all_info(rm_info_t *info);

#endif /* __RICK_AND_MORTY_API_H__ */
