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

#include "rm_types.h"
#include "rm_api.h"

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
    da_array_t residents; /**< Residents list (dynamyc array of string url) of location who have been last seen in location */
    char *url; /**< Link (string) of the location */
    char *created /**< Dte time (string) at which the location was created in the database. */
};
typedef struct RM_location rm_location_t;

/* \struct RM_location_array location.h
 * \brief Structure for an array of RM_location struct and its length.
 * */
struct RM_location_array {
    rm_location_t *da_location; /**< An "Array" of struct RM_location */
    size_t len; /**< Len of the "Array" */
};

/**
 * \fn rm_location_t rm_location_get(uint64_t id)
 * \brief This function get all attributs of the location with the specific ID.
 * \param[in] id (uint64_t) The ID of the location to get this attributs.
 * \return rm_location_t (struct RM_location) with field filled.
 *
 * */
rm_location_t rm_location_get(uint64_t);

/**
 * \fn struct RM_location_array RM_location_get_all(void)
 * \brief This function get all locations and their attributes.
 * \return struct RM_location_array.
 *
 * */
struct RM_location_array rm_location_get_all(void);

/**
 * \fn struct RM_location_array rm_location_get_list(uint64_t array_id[], size_t array_len)
 * \brief This function get all attributes of location with id inside array_id.
 * \param[in] array_id (uint64_t[]) An array with the number of ID of characeters to get. 
 * \param[in] array_len (size_t) The len of array. 
 * \return struct RM_location_array.
 *
 * */
struct RM_location_array rm_location_get_list(uint64_t array_id[], size_t array_len);

/**
 * \fn struct RM_location_array rm_location_get_filter(rm_location_t filter)
 * \brief This function get all attributes of location match with given filter.
 * \param[in] filter (rm_location_t) RM_location structure to use like filter (only the following field are verifed: name, type, dimension). 
 * \return struct RM_location_array.
 *
 * */
struct RM_location_array rm_location_get_filter(rm_location_t filter);

#endif /* __RICK_AND_MORTY_LOCATION_H__ */
