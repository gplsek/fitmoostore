<?php

/**
 * @file
 * Contains the resource callbacks for redemptions.
 */


/**
 * Returns a collection of redemptions.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_redemption_index($fields)
{
    global $user;
    if (empty($user->uid)) {
        return services_error(t('user not found'), 404);
    }
    $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
    if (!empty($payment_method['settings']['cardonfile'])) {
        $stored_cards = commerce_cardonfile_load_multiple_by_uid($user->uid, $payment_method['instance_id']);
    }
    return $stored_cards;
}

/**
 * Determines an API user's access to index redemptions.
 */
function commerce_services_redemption_index_access()
{
    // Because the entity access control system will filter any result sets,
    // there's no reason to limit a particular user's access.
    return TRUE;
}

/**
 * Returns a single redemption.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_redemption_retrieve($user_id)
{
    global $user;
    if (empty($user->uid)) {
        return services_error(t('user not found'), 404);
    }
    $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
    if (!empty($payment_method['settings']['cardonfile'])) {
        $stored_cards = commerce_cardonfile_load_multiple_by_uid($user->uid, $payment_method['instance_id']);
    }
    return $stored_cards;
}

/**
 * Determines an API user's access to retrieve a given redemption.
 *
 * @param $redemption_id
 *   The ID of the redemption to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the redemption.
 */
function commerce_services_redemption_retrieve_access($redemption_id)
{
    return TRUE;
}

/**
 * Creates a new redemption.
 */
function commerce_services_redemption_create($data, $flatten_fields)
{
    //    $form, &$form_state, $order, $payment_method

    global $user;
    if (empty($user->uid)) {
        return services_error(t('Access to this operation not granted'), 401);
    }
    $user_fields = user_load($data['userID']);
    if (empty($user_fields)) {
        return services_error(t('User not found'), 401);
    }
    if (!empty($data['check'])) {
        $transfer_type = 'check';
        if (empty($data['mail']) and (empty($user_fields->mail))) {
            return services_error(t('Mail field is empty'), 401);
        } elseif (!empty($data['mail'])) {
            if (valid_email_address($data['mail'])) {
                user_save($user_fields, $edit = array('mail' => $data['mail']));
            } else {
                return services_error(t('It\'s not Email'), 401);
            }

        }



    } elseif (!empty($data['ach'])) {
        $transfer_type = 'ach';
        if ((empty($data['bank_rounting_number'])) and (empty($user_fields->field_bank_rounting_number))) {
            return services_error(t('field_bank_rounting_number is empty'), 401);
        }
        if ((empty($data['bank_account_number'])) and (empty($user_fields->field_bank_account_number))) {
            return services_error(t('field_bank_account_number is empty'), 401);
        }
        if ((empty($data['bank_account_name'])) and (empty($user_fields->field_bank_account_name))) {
            return services_error(t('field_bank_account_name is empty'), 401);
        }
        if ((empty($data['bank_name'])) and (empty($user_fields->field_bank_name))) {
            return services_error(t('field_bank_name is empty'), 401);
        }
        //$save = false;
        if (!empty($data['bank_account_name'])) {
            $edit = array('field_bank_rounting_number' => array('und' => array(0 => array('value' => $data['bank_rounting_number'],),),),);
            user_save($user_fields, $edit);
            //$save = true;
        }
        if (!empty($data['bank_account_number'])) {
            $edit = array('field_bank_account_number' => array('und' => array(0 => array('value' => $data['bank_account_number'],),),),);
            user_save($user_fields, $edit);
            //$save = true;
        }
        if (!empty($data['bank_account_name'])) {
            $edit = array('field_bank_account_name' => array('und' => array(0 => array('value' => $data['bank_account_name'],),),),);
            user_save($user_fields, $edit);
            //$save = true;
        }
        if (!empty($data['bank_name'])) {
            $edit = array('field_bank_name' => array('und' => array(0 => array('value' => $data['bank_name'],),),),);
            user_save($user_fields, $edit);
            //$save = true;
        }

//        if ($save ==true) {
//            user_save($user_fields, $edit);
//        }
    }

    $id = db_insert('fitmoo_user_accounting') // Table name no longer needs {}
        ->fields(array(
            'uid' => $data['userID'], //user ID
            'ammount' => -$data['ammount'],
            'type' => '',
            'status' => 'redemption',
            'created' => REQUEST_TIME,
            'updated' => REQUEST_TIME,
        ))
        ->execute();


    $redid =  db_insert('fitmoo_user_redemptions') // Table name no longer needs {}
        ->fields(array(
            'uid' => $data['userID'], //user ID
            'tid' => $id, //from the accounting table
            'ammount' => $data['ammount'],
            'status' => 'pending',
            'transfer_type' => $transfer_type, //ach or check
            'created' => REQUEST_TIME,
            'updated' => REQUEST_TIME,
    ))
        ->execute();

    return array('fitmoo_user_accounting_id'=>$id, 'fitmoo_user_redemptions_id'=>$redid);
}


/**
 * Determines an API user's access to create a new redemption.
 */
function commerce_services_redemption_create_access($data)
{
    return TRUE;
}

/**
 * Updates a redemption.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_redemption_update($redemption_id, $data, $flatten_fields)
{
    global $user;
    if (is_array($data['items'][0])) {
        $array = array();
        foreach ($data['items'][0] as $key => $items) {
            $redemption = commerce_redemption_load($items[0]);
            if (!commerce_redemption_access('update', $redemption)) {
                return services_error(t('Access to this operation not granted'), 401);
            }
            $array[] = _cs_redemption_update($key, $items[0], $flatten_fields);
        }
        return $array;
    } else if ($data['node'] == "true") {
        unset($data['node']);
        $redemption_node = node_load($redemption_id);
        $uid = $user->uid;
        $access = false;
        if ($uid == $redemption_node->uid) {
            $access = true;
        }
        if (is_array($user->roles) && in_array('administrator', $user->roles)) {
            $access = true;
        }
        if ($access == false) {
            return services_error(t('Access to this operation not granted'), 401);
        }

        if (!empty($data['type'])) {
            $redemption_node->type = $data['type'];
            unset($data['type']);
        }
        if (!empty($data['title'])) {
            $redemption_node->title = $data['title'];
            unset($data['title']);
        }
        if (!empty($data['sku'])) {
            $redemption_node->sku = $data['sku'];
            unset($data['sku']);
        }
        if (!empty($data['description'])) {
            $redemption_node->body['und'][0]['value'] = $data['description'];
            unset($data['description']);
        }
        if (is_array($data['redemptions'][0])) {
            $redemptions = $data['redemptions'][0];
        }
        $redemptions = $data['redemptions'];
        unset($data['redemptions']);
        foreach ($data as $key => $value) {
            $redemption_node->{$key} = $value;
        }
        node_save($redemption_node);
        if (is_array($redemptions)) {
            foreach ($redemptions as $key => $items) {
                $array[] = _cs_redemption_update($key, $items, false);
            }
            return array('node_redemption' => $redemption_node, 'items' => $array);

        }
        return $redemption_node;
    } else {
        $redemption = commerce_redemption_load($redemption_id);
        if (!commerce_redemption_access('update', $redemption)) {
            return services_error(t('Access to this operation not granted'), 401);
        }
        return _cs_redemption_update($redemption_id, $data, $flatten_fields);
    }
}

function _cs_redemption_update($redemption_id, $data, $flatten_fields)
{

    if (!empty($data['size'])) {
        $res = field_info_instances('commerce_redemption', $data['type']);
        foreach ($res as $key1 => $value1) {
            if (preg_match('/(.*)(field)(.*)(size)(.*)/ism', $key1, $matches)) {
                if ($data['type'] == 'redemption') {
                    $size_id = commerce_services_get_term_from_name($data['size'], 'redemption_size', $parent_tid = null);
                    if ($size_id) {
                        $data[$matches[0]] = $size_id;
                    }
                } else {
                    $data[$matches[0]] = $data['size'];
                }
            }
        }
        unset($data['size']);
    }
    return commerce_services_update_entity('commerce_redemption', $redemption_id, $data, $flatten_fields);
}

/**
 * Determines an API user's access to update a given redemption.
 *
 * @param $redemption_id
 *   The ID of the redemption to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the redemption.
 */
function commerce_services_redemption_update_access($redemption_id)
{
    return TRUE;
}

/**
 * Deletes a redemption.
 *
 * @param $redemption_id
 *   The ID of the redemption to be deleted.
 */
function commerce_services_redemption_delete($redemption_id)
{
    commerce_redemption_delete($redemption_id);
}

/**
 * Determines an API user's access to delete a given redemption.
 *
 * @param $redemption_id
 *   The ID of the redemption to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the redemption.
 */
function commerce_services_redemption_delete_access($redemption_id)
{
    // Attempt to load the redemption.
    if ($redemption = commerce_redemption_load($redemption_id)) {
        // If the user has access to perform the operation...
        if (commerce_redemption_access('delete', $redemption)) {
            // And the redemption can safely be deleted...
            if (commerce_redemption_can_delete($redemption)) {
                return TRUE;
            } else {
                return services_error(t('redemption cannot be deleted'), 403);
            }
        } else {
            return services_error(t('Access to this operation not granted'), 401);
        }
    } else {
        return services_error(t('redemption not found'), 404);
    }
}
