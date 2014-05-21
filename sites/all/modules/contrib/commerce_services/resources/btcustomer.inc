<?php

/**
 * @file
 * Contains the resource callbacks for btcustomers.
 */


/**
 * Returns a collection of btcustomers.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_btcustomer_index($fields)
{
    global $user;
    if (empty($user->uid)) {
        return services_error(t('user not found'), 404);
    }
    if (is_array($user->roles) && in_array('administrator', $user->roles)) {$access = true;} else {$access = false;}
    $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
    commerce_braintree_initialize($payment_method);

    if ($_GET['token'] and !empty($_GET['card'])) {
        $creditCard = Braintree_CreditCard::find($_GET['token']);
        if (($creditCard->customerId == $user->uid) or ($access == true)) {
            return $creditCard;
        } else {
            return services_error(t('Access to this operation not granted'), 401);
        }

    } elseif (!empty($_GET['customer'])) {
        $result = Braintree_Customer::find($_GET['id']);
        if (($result->id == $user->uid) or ($access == true)) {

            $customer['id'] = $result->id;
            $customer['merchantId'] = $result->merchantId;
            $customer['firstName'] = $result->firstName;
            $customer['lastName'] = $result->lastName;
            $customer['company'] = $result->company;
            $customer['email'] = $result->email;
            $customer['phone'] = $result->phone;
            $customer['fax'] = $result->fax;
            $customer['website'] = $result->website;
            $customer['createdAt'] = $result->createdAt;
            $customer['updatedAt'] = $result->updatedAt;

            $to_return = array('success'=>$result->success,
                'customer'=>$customer,
                'cc'=>$result->creditCards,
                'addresses'=>$result->addresses);

            return $to_return;
        } else {
            return services_error(t('Access to this operation not granted'), 401);
        }
    }
}

/**
 * Determines an API user's access to index btcustomers.
 */
function commerce_services_btcustomer_index_access()
{
    // Because the entity access control system will filter any result sets,
    // there's no reason to limit a particular user's access.
    return TRUE;
}

/**
 * Returns a single btcustomer.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_btcustomer_retrieve($user_id)
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
 * Determines an API user's access to retrieve a given btcustomer.
 *
 * @param $btcustomer_id
 *   The ID of the btcustomer to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the btcustomer.
 */
function commerce_services_btcustomer_retrieve_access($btcustomer_id)
{
    return TRUE;
}

/**
 * Creates a new btcustomer.
 */
function commerce_services_btcustomer_create($data, $flatten_fields)
{
    //    $form, &$form_state, $order, $payment_method

    global $user;
    if (empty($user->uid)) {
        return services_error(t('Access to this operation not granted'), 401);
    }

    if (is_array($user->roles) && in_array('administrator', $user->roles)) {$access = true;} else {$access = false;}

    if (!empty($data['customer_data']['id'])) {
        $result_array['id'] = $data['customer_data']['id'];
        if (($result_array['id'] != $user->uid) and !$access) {
            return services_error(t('Access to this operation not granted'), 401);
        }
    } else {
        $result_array['id'] = $user->uid;
    }


    // Initialize the Braintree client.
    $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
    commerce_braintree_initialize($payment_method);

    //==============CREATE_CUSTOMER================
    if (!empty($data['customer'])) {

//        $result_array = array(
//            'id' => 'customer_123',
//            'firstName' => $data['firstName'],
//            'lastName' => $data['lastName'],
//            'company' => $data['company'],
//            'email' => $data['email'],
//            'phone' => $data['phone'],
//            'fax' => $data['fax'],
//            'website' => $data['website'],
//        );

        foreach($data['customer_data'] as $key=>$value) {
            $result_array[$key] = $value;
        }


//            'creditCard' => array(
//                'cardholderName' => 'Fred Jones',
//                'number' => '5105105105105100',
//                'cvv' => '123',
//                'expirationDate' => '05/2020',
//                'options' => array(
//                    'verifyCard' => true
//                ),
//               'billingAddress' => $billingaddress,
//            )

        $cc = array();
        foreach($data['credit_card'] as $key=>$value) {
            $cc[$key] = $value;
        }


//        $billingaddress = array(
//            'firstName' => 'Drew',
//            'lastName' => 'Smith',
//            'company' => 'Smith Co.',
//            'streetAddress' => '1 E Main St',
//            'extendedAddress' => 'Suite 101',
//            'locality' => 'Chicago',
//            'region' => 'IL',
//            'postalCode' => '60622',
//            'countryCodeAlpha2' => 'US'
//        );

        $billingaddress = array();
        foreach($data['billing_address'] as $key=>$value) {
            $billingaddress[$key] = $value;
        }

        if (count($billingaddress)>0) {
            $cc['billingAddress'] = $billingaddress;
        }

        if (count($cc)>0) {
            $result_array['creditCard'] = $cc;
        }

        $result = Braintree_Customer::create($result_array);
        $customer['id'] = $result->id;
        $customer['merchantId'] = $result->merchantId;
        $customer['firstName'] = $result->firstName;
        $customer['lastName'] = $result->lastName;
        $customer['company'] = $result->company;
        $customer['email'] = $result->email;
        $customer['phone'] = $result->phone;
        $customer['fax'] = $result->fax;
        $customer['website'] = $result->website;
        $customer['createdAt'] = $result->createdAt;
        $customer['updatedAt'] = $result->updatedAt;

        $to_return = array('success'=>$result->success,
          'customer'=>$customer,
            'cc'=>$result->creditCards);

        return $to_return;

    } elseif ($data['card']) {


//        array(
//            'customerId' => 'the_customer_id',
//            'cardholderName' => 'John Doe',
//            'cvv' => '123',
//            'number' => '4111111111111111',
//            'expirationDate' => '05/2020',
//            'options' => array(
//                'verifyCard' => true
//            )
//        )

        $cc = array();
        $cc['customerId'] = $result_array['id'];
        foreach($data['credit_card'] as $key=>$value) {
            $cc[$key] = $value;
        }

        $result = Braintree_CreditCard::create($cc);
        return $result;

    }

}

/**
 * Determines an API user's access to create a new btcustomer.
 */
function commerce_services_btcustomer_create_access($data)
{
    return TRUE;
}

/**
 * Updates a btcustomer.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_btcustomer_update($btcustomer_id, $data, $flatten_fields)
{
    global $user;
    if (empty($user->uid)) {
        return services_error(t('Access to this operation not granted'), 401);
    }

    if (is_array($user->roles) && in_array('administrator', $user->roles)) {$access = true;} else {$access = false;}
    if (($btcustomer_id != $user->uid) and !$access) {
          return services_error(t('Access to this operation not granted'), 401);
    }

    // Initialize the Braintree client.
    $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
    commerce_braintree_initialize($payment_method);


    //==============UPDATE_CUSTOMER================
    if (!empty($data['customer'])) {

//        $result_array = array(
//            'firstName' => $data['firstName'],
//            'lastName' => $data['lastName'],
//            'company' => $data['company'],
//            'email' => $data['email'],
//            'phone' => $data['phone'],
//            'fax' => $data['fax'],
//            'website' => $data['website'],
//        );

        foreach($data['customer_data'] as $key=>$value) {
            $result_array[$key] = $value;
        }

//            'creditCard' => array(
//                'cardholderName' => 'Fred Jones',
//                'number' => '5105105105105100',
//                'cvv' => '123',
//                'expirationDate' => '05/2020',
//                'options' => array(
//                    'verifyCard' => true
//                ),
//               'billingAddress' => $billingaddress,
//            )

        $cc = array();
        foreach($data['credit_card'] as $key=>$value) {
            $cc[$key] = $value;
        }


//        $billingaddress = array(
//            'firstName' => 'Drew',
//            'lastName' => 'Smith',
//            'company' => 'Smith Co.',
//            'streetAddress' => '1 E Main St',
//            'extendedAddress' => 'Suite 101',
//            'locality' => 'Chicago',
//            'region' => 'IL',
//            'postalCode' => '60622',
//            'countryCodeAlpha2' => 'US'
//        );

        $billingaddress = array();
        foreach($data['billing_address'] as $key=>$value) {
            $billingaddress[$key] = $value;
        }

        if (count($billingaddress)>0) {
            $cc['billingAddress'] = $billingaddress;
        }

        if (count($cc)>0) {
            $result_array['creditCard'] = $cc;
        }

        $result = Braintree_Customer::update($btcustomer_id,$result_array);
        $customer['id'] = $result->id;
        $customer['merchantId'] = $result->merchantId;
        $customer['firstName'] = $result->firstName;
        $customer['lastName'] = $result->lastName;
        $customer['company'] = $result->company;
        $customer['email'] = $result->email;
        $customer['phone'] = $result->phone;
        $customer['fax'] = $result->fax;
        $customer['website'] = $result->website;
        $customer['createdAt'] = $result->createdAt;
        $customer['updatedAt'] = $result->updatedAt;

        $to_return = array('success'=>$result->success,
            'customer'=>$customer,
            'cc'=>$result->creditCards);

        return $to_return;

//
//
//        $output = array();
//        $output['customer'] = $customer;
//        $customer = $result->;
//        $creditCard = $customer->creditCards[0];
//        $creditCard->bin;
//# '510510'
//        $creditCard->last4;
//# '5100'
//
//# The same address, two different ways
//        $address = $customer->addresses[0];
//        $creditCard->billingAddress;
//
//        $address->firstName;
//# 'Drew'
//        $address->lastName;
//# 'Smith'
    } elseif ($data['card']) {


//        array(
//            'cardholderName' => 'Jane Jones',
//            'cvv' => '321',
//            'number' => '4111111111111111',
//            'expirationDate' => '05/2020',
//            'options' => array(
//                'verifyCard' => true
//            )
//        )

        $cc = array();
        foreach($data['credit_card'] as $key=>$value) {
            $cc[$key] = $value;
        }

        $result = Braintree_CreditCard::update($data['token'], array($cc));
        return $result;

    }


}

function _cs_btcustomer_update($btcustomer_id, $data, $flatten_fields)
{

    if (!empty($data['size'])) {
        $res = field_info_instances('commerce_btcustomer', $data['type']);
        foreach ($res as $key1 => $value1) {
            if (preg_match('/(.*)(field)(.*)(size)(.*)/ism', $key1, $matches)) {
                if ($data['type'] == 'btcustomer') {
                    $size_id = commerce_services_get_term_from_name($data['size'], 'btcustomer_size', $parent_tid = null);
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
    return commerce_services_update_entity('commerce_btcustomer', $btcustomer_id, $data, $flatten_fields);
}

/**
 * Determines an API user's access to update a given btcustomer.
 *
 * @param $btcustomer_id
 *   The ID of the btcustomer to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the btcustomer.
 */
function commerce_services_btcustomer_update_access($btcustomer_id)
{
    return TRUE;
}

/**
 * Deletes a btcustomer.
 *
 * @param $btcustomer_id
 *   The ID of the btcustomer to be deleted.
 */
function commerce_services_btcustomer_delete($btcustomer_id)
{
    commerce_btcustomer_delete($btcustomer_id);
}

/**
 * Determines an API user's access to delete a given btcustomer.
 *
 * @param $btcustomer_id
 *   The ID of the btcustomer to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the btcustomer.
 */
function commerce_services_btcustomer_delete_access($btcustomer_id)
{
    // Attempt to load the btcustomer.
    if ($btcustomer = commerce_btcustomer_load($btcustomer_id)) {
        // If the user has access to perform the operation...
        if (commerce_btcustomer_access('delete', $btcustomer)) {
            // And the btcustomer can safely be deleted...
            if (commerce_btcustomer_can_delete($btcustomer)) {
                return TRUE;
            } else {
                return services_error(t('btcustomer cannot be deleted'), 403);
            }
        } else {
            return services_error(t('Access to this operation not granted'), 401);
        }
    } else {
        return services_error(t('btcustomer not found'), 404);
    }
}
