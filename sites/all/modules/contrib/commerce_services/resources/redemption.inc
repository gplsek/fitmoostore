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
    if (empty($data['amount'])) {
        return services_error(t('Enter amount pls'), 401);
    }
	
	if($data['amount'] > $user->field_balance_available['und'][0]['value']){
	   return services_error(t('Requested amount is not available balance is: $'.field_balance_available['und'][0]['value']), 401);
	}
	
    if (!empty($data['check'])) {
        $transfer_type = 'check';

        if (empty($data['first_name']) or empty($data['last_name'])  or empty($data['thoroughfare']) or empty($data['country']) or empty($data['locality']) or empty($data['administrative_area']) or empty($data['postal_code'])) {
            return services_error(t('One of profile field is empty'), 401);
        }
        if (empty($data['premise'])) {
            $data['premise'] = '';
        }


        $query = db_select('commerce_customer_profile', 'cp');
        $query->join('field_data_commerce_customer_address', 'fd', 'cp.profile_id = fd.entity_id');
        $query->fields('cp',array('profile_id'))
            ->fields('fd')
            ->orderBy('cp.profile_id')//ORDER BY created
            ->condition('cp.uid', $user->uid,'=')
            ->condition('cp.type', 'shipping','=')
            ->condition('fd.commerce_customer_address_country', $data["country"],'=')
            ->condition('fd.commerce_customer_address_administrative_area', $data["administrative_area"],'=')
            ->condition('fd.commerce_customer_address_postal_code', $data["postal_code"],'=')
            ->condition('fd.commerce_customer_address_thoroughfare', $data["thoroughfare"],'=')
            ->condition('fd.commerce_customer_address_locality', $data["locality"],'=')
            ->range(0,1);//LIMIT to 1 record
        $result = $query->execute();
        $profile_list = $result->fetchAssoc();

        if ($profile_list == false) {
            $address = implode(',', array($data["thoroughfare"], $data["locality"], $data["postal_code"], $data["country"]));

            $libpath = libraries_get_path('Geocoder');
            require_once DRUPAL_ROOT . '/' . $libpath . '/' . 'autoload.php';
            $geocoder = new \Geocoder\Geocoder();
            $adapter = new \Geocoder\HttpAdapter\CurlHttpAdapter();
            $chain = new \Geocoder\Provider\ChainProvider(array(
                new \Geocoder\Provider\FreeGeoIpProvider($adapter),
                new \Geocoder\Provider\HostIpProvider($adapter),
                new \Geocoder\Provider\GoogleMapsProvider($adapter, 'en_US', 'US', true),
            ));
            $geocoder->registerProvider($chain);

            try {
                $geocode = $geocoder->geocode($address);
                $res = $geocode->toArray();
                $thoroughfare = implode(' ',array($res['streetNumber'], $res['streetName']));

                $query1 = db_select('commerce_customer_profile', 'cp');
                $query1->join('field_data_commerce_customer_address', 'fd', 'cp.profile_id = fd.entity_id');
                $query1->fields('cp',array('profile_id'))
                    ->fields('fd')
                    ->orderBy('cp.profile_id')
                    ->condition('cp.uid', $user->uid,'=')
                    ->condition('cp.type', 'shipping','=')
                    ->condition('fd.commerce_customer_address_country', $res["countryCode"],'=')
                    ->condition('fd.commerce_customer_address_administrative_area', $res["regionCode"],'=')
                    ->condition('fd.commerce_customer_address_postal_code', $res["zipcode"],'=')
                    ->condition('fd.commerce_customer_address_thoroughfare', $thoroughfare,'=')
                    ->condition('fd.commerce_customer_address_locality', $res["city"],'=')
                    ->range(0,1);
                $result1 = $query1->execute();
                $profile_list1 = $result1->fetchAssoc();
                if ($profile_list1 == false) {
                    $thoroughfare = implode(' ',array($res['streetNumber'], $res['streetName']));
                    $profilenew = commerce_customer_profile_new('shipping', $user->uid);
                    $profilenew->commerce_customer_address['und'][0]['thoroughfare'] = $thoroughfare;
                    $profilenew->commerce_customer_address['und'][0]['country'] = $res["countryCode"];
                    $profilenew->commerce_customer_address['und'][0]['premise'] = $data["premise"];
                    $profilenew->commerce_customer_address['und'][0]['locality'] = $res["city"];
                    $profilenew->commerce_customer_address['und'][0]['administrative_area'] = $res["regionCode"];
                    $profilenew->commerce_customer_address['und'][0]['postal_code'] = $res["zipcode"];
                    $profilenew->commerce_customer_address['und'][0]['first_name'] = $data["first_name"];
                    $profilenew->commerce_customer_address['und'][0]['last_name'] = $data["last_name"];
                    commerce_customer_profile_save($profilenew);
                    $profileId = $profilenew->profile_id;
                } else {
                    $profileId = $profile_list1['profile_id'];
                }
            } catch (Exception $e) {
                if (empty($res['countryCode'])) {
                    $profilenew = commerce_customer_profile_new('shipping', $user->uid);
                    $profilenew->commerce_customer_address['und'][0]['thoroughfare'] = $data['thoroughfare'];
                    $profilenew->commerce_customer_address['und'][0]['country'] = $data["country"];
                    $profilenew->commerce_customer_address['und'][0]['premise'] = $data["premise"];
                    $profilenew->commerce_customer_address['und'][0]['locality'] = $data["locality"];
                    $profilenew->commerce_customer_address['und'][0]['administrative_area'] = $data['administrative_area'];
                    $profilenew->commerce_customer_address['und'][0]['postal_code'] = $data["postal_code"];
                    $profilenew->commerce_customer_address['und'][0]['first_name'] = $data["first_name"];
                    $profilenew->commerce_customer_address['und'][0]['last_name'] = $data["last_name"];
                    commerce_customer_profile_save($profilenew);
                    $profileId = $profilenew->profile_id;
                } else {
                    return services_error(t($e), 401);
                }

            }
        } else {
            $profileId = $profile_list->profile_id;
        }


    } elseif (!empty($data['ach'])) {
        $profileId = '0';
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
            'ammount' => -$data['amount'],
            'type' => '',
            'status' => 'redemption',
            'created' => REQUEST_TIME,
            'updated' => REQUEST_TIME,
        ))
        ->execute();

    //to logs===
     db_insert('fitmoo_user_accounting_log')
        ->fields(array(
            'uaID' => $id,
            'uid' => $data['userID'],
            'ammount' => -$data['amount'],
            'type' => '',
            'status' => 'redemption',
            'created' => REQUEST_TIME,
        ))
        ->execute();


    $redid = db_insert('fitmoo_user_redemptions') // Table name no longer needs {}
        ->fields(array(
            'uid' => $data['userID'], //user ID
            'tid' => $id, //from the accounting table
            'ammount' => $data['amount'],
            'status' => 'pending',
            'transfer_type' => $transfer_type, //ach or check
            'profile_id' => $profileId, //ach or check
            'created' => REQUEST_TIME,
            'updated' => REQUEST_TIME,
        ))
        ->execute();
		
    fitmoo_admintools_notify_redemption($data['userID'],$data['amount'],$transfer_type);
	_update_user_balance($data['userID']);
    return array('fitmoo_user_accounting_id' => $id, 'fitmoo_user_redemptions_id' => $redid);
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
