<?php

/**
 * @file
 * Contains the resource callbacks for address.
 */


/**
 * Returns a collection of address.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_address_index($fields) {
    global $user;
    $uid =  $user->uid;
    $profile_list = commerce_customer_profile_load_multiple($profile_ids = array(), array('uid'=>$uid), $reset = FALSE);
    return $profile_list;
}

/**
 * Determines an API user's access to index address.
 */
function commerce_services_address_index_access() {
    return TRUE;
}

/**
 * Returns a single address.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_address_retrieve($pid) {
    global $user;
    $uid =  $user->uid;
    return  commerce_customer_profile_load($pid) ;
}

/**
 * Determines an API user's access to retrieve a given address.
 *
 * @param $address_id
 *   The ID of the address to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the address.
 */
function commerce_services_address_retrieve_access() {
    return TRUE;
}

/**
 * Creates a new address.
 */
function commerce_services_address_create($data) {
    // Ensure the request specifies a unique, valid SKU.
    global $user;
    $profilenew = commerce_customer_profile_new($data['type'], $user->uid);
    unset($data['type']);
    foreach ($data as $key=>$value) {
        $profilenew->commerce_customer_address['und'][0][$key] = $value;
    }
    $result = commerce_customer_profile_save($profilenew);
    return $profilenew;
}

/**
 * Determines an API user's access to create a new address.
 */
function commerce_services_address_create_access() {
    return TRUE;
}

/**
 * Updates a address.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_address_update($pid, $data) {
    // Ensure the request specifies a unique, valid SKU.
    global $user;
    $profile = commerce_customer_profile_load($pid);
    if ($profile->uid != $user->uid) {
        return services_error(t('Access to this operation not granted'), 401);
    }
    if (!empty($data['type'])) {
        $profile->type =  $data['type'];
    }

    unset($data['type']);
    foreach ($data as $key=>$value) {
        $profile->commerce_customer_address['und'][0][$key] = $value;
    }
    $result = commerce_customer_profile_save($profile);
    return $profile;
}

/**
 * Determines an API user's access to update a given address.
 *
 * @param $address_id
 *   The ID of the address to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the address.
 */
function commerce_services_address_update_access() {
  return TRUE;
}

/**
 * Deletes a address.
 *
 * @param $address_id
 *   The ID of the address to be deleted.
 */
function commerce_services_address_delete($pid) {
    global $user;
    $profile = commerce_customer_profile_load($pid);
    if ($profile->uid != $user->uid) {
        return services_error(t('Access to this operation not granted'), 401);
    }
    return commerce_customer_profile_delete($pid, array());
}

/**
 * Determines an API user's access to delete a given address.
 *
 * @param $address_id
 *   The ID of the address to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the address.
 */
function commerce_services_address_delete_access($address_id) {
   return TRUE;
}
