#ifndef __RICK_AND_MORTY_LOCATION_H__
#define __RICK_AND_MORTY_LOCATION_H__

/*
 * \file location.h
 * \author ownesis
 * \date 9 janv. 2022
 * \brief Functions and structures for 'location' API
 *
 * \see https://rickandmortyapi.com/documentation/#location
 *
 * */

#include <stddef.h>
#include <stdint.h>

#include "types.h"
#include "api.h"

/*
 * \def RM_API_URL_LOCATION
 * */
#define RM_API_URL_LOCATION RM_API_ENDPOINT "/location/"

/*
 * \struct RM_location location.h
 * \brief Structure for location attributes.
 *
 * RM_location have typedef : rm_location_t
 *
 * \see https://rickandmortyapi.com/documentation/#location-schema
 * */
struct RM_location {
    uint64_t id; /**< ID (uint64_t) of the location */
    char *name; /**< Name (string) of the location */
    char *type; /**< Type (string) of the location */
    char *dimension; /**< Dimension (string) of the location */
    size_t residents_len; /** Len (size_t) of residents array */
    da_array_str_t residents; /**< Residents list (dynamyc array of string url) of location who have been last seen in location */
    char *url; /**< Link (string) of the location */
    char *created; /**< Dte time (string) at which the location was created in the database. */
};
typedef struct RM_location rm_location_t;

/* \struct RM_location_array location.h
 * \brief Structure for an array of RM_location struct and its length.
 * */
struct RM_location_array {
    rm_location_t **da_location; /**< An "Array" of struct RM_location */
    size_t len; /**< Len of the "Array" */
};
typedef struct RM_location_array rm_location_arr_t;


rm_location_t *parse_location(json_object *jobj);
char *location_parse_filter(char *url, rm_location_t *filter);
void rm_location_clear(rm_location_t *location);

/**
 * \fn rm_location_t rm_location_get(uint64_t id)
 * \brief This function get all attributs of the location with the specific ID.
 * \param[in] id (uint64_t) The ID of the location to get this attributs.
 * \return rm_location_t (struct RM_location) with field filled.
 *
 * */
void rm_location_clear(rm_location_t *location);

/**
 * \fn rm_location_t rm_location_get(uint64_t id)
 * \brief This function get all attributs of the location with the specific ID.
 * \param[in] id (uint64_t) The ID of the location to get this attributs.
 * \return rm_location_t (struct RM_location) with field filled.
 *
 * */
void  rm_da_location_clear(struct RM_location_array *location);

/**
 * \fn rm_location_t rm_location_get(uint64_t id)
 * \brief This function get all attributs of the location with the specific ID.
 * \param[in] id (uint64_t) The ID of the location to get this attributs.
 * \return rm_location_t (struct RM_location) with field filled.
 *
 * */
rm_location_t *rm_location_get(uint64_t id);

/**
 * \fn struct RM_location_array RM_location_get_all(void)
 * \brief This function get all locations and their attributes.
 * \return struct RM_location_array.
 *
 * */
struct RM_location_array *rm_location_get_all(rm_location_t *filter);

/**
 * \fn struct RM_location_array rm_location_get_list(uint64_t array_id[], size_t array_len)
 * \brief This function get all attributes of location with id inside array_id.
 * \param[in] array_id (uint64_t[]) An array with the number of ID of characeters to get. 
 * \param[in] array_len (size_t) The len of array. 
 * \return struct RM_location_array.
 *
 * */
struct RM_location_array *rm_location_get_list(uint64_t array_id[], size_t array_len);

/**
 * \fn struct RM_character_array rm_character_get_filter(rm_info_t *info, rm_character_t *filter)
 * \brief This function get an array of all character found in the specified page.
 * \param[out] ctx (rm_info_t *) Address of rm_info_t structure
 * \param[in] page (size_t) the page to get. 
 * \param[in] filter (rm_character_t *) Addres of a RM_character structure to use like filter (only the following field are verifed: name, status, species, type, gender). 
 * \return address of struct RM_character_array.
 *
 * */
struct RM_location_array *rm_location_get_page(rm_info_t *info, uint64_t page, rm_location_t *filter);

#endif /* __RICK_AND_MORTY_LOCATION_H__ */
