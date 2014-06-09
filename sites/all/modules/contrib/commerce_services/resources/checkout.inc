<?php

/**
 * @file
 * Contains the resource callbacks for checkouts.
 */


/**
 * Returns a collection of checkouts.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_checkout_index($fields) {
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
 * Determines an API user's access to index checkouts.
 */
function commerce_services_checkout_index_access() {
  // Because the entity access control system will filter any result sets,
  // there's no reason to limit a particular user's access.
  return TRUE;
}

/**
 * Returns a single checkout.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_checkout_retrieve($user_id) {
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
 * Determines an API user's access to retrieve a given checkout.
 *
 * @param $checkout_id
 *   The ID of the checkout to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the checkout.
 */
function commerce_services_checkout_retrieve_access($checkout_id) {
      return TRUE;
}

/**
 * Creates a new checkout.
 */
function commerce_services_checkout_create($data, $flatten_fields) {
        //    $form, &$form_state, $order, $payment_method

        global $user;
        if (empty($user->uid)) {
            return services_error(t('Access to this operation not granted'), 401);
        }

        // Initialize the Braintree client.
        $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
        commerce_braintree_initialize($payment_method);
        if (empty($data['order_id'])) {
            $order_id = commerce_cart_order_id($user->uid);
            $order = commerce_order_load($order_id);

        } else {
            $order = commerce_order_load($data['order_id']);
        }
        if (empty($order)) {
            return services_error(t('Order empty'), 400);
        }

        $context = commerce_braintree_payment_session_load($order->order_id);

        list($amount, $customer_name, $country, $thoroughfare, $locality, $postal_code, $administrative_area, $customer_mail) = _commerce_braintree_get_transaction_informations($order);

        // Retrieve the order balance instead of the order total, this allows you
        // to pay your order with multiple payment methods.
        $balance = commerce_payment_order_balance($order);
        $amount = commerce_braintree_price_amount($balance['amount'], $balance['currency_code']);
        $post_values = array();
        $post_values['op'] = 'Process payment';
        if (empty($data['card_id'])) {
            // Build the credit card form first.
            $form = commerce_braintree_credit_card_form($payment_method);
            $post_values['transaction']['credit_card']['number']=$data['cc_number'];
            $post_values['transaction']['credit_card']['cardholder_name']=$data['cardholder_name'];
            $post_values['transaction']['credit_card']['expiration_month']=$data['expiration_month'];
            $post_values['transaction']['credit_card']['expiration_year']=$data['expiration_year'];
            $post_values['transaction']['credit_card']['cvv']=$data['cvv'];

            // Create a transaction data string using the Braintree client.
            $trData = Braintree_TransparentRedirect::transactionData(array(
                // Add transaction related data.
                'transaction' => array(
                    'type' => Braintree_Transaction::SALE,
                    'amount' => $amount,
                    'customer' => array(
                        'firstName' => $customer_name,
                        'email' => $customer_mail,
                    ),
                    'billing' => array(
                        'countryCodeAlpha2' => $country,
                        'streetAddress' => $thoroughfare,
                        'firstName' => $customer_name,
                        'locality' => $locality,
                        'postalCode' => $postal_code,
                        'region' => $administrative_area,
                    ),
                    'options' => array(
                        'storeInVault' => TRUE,
                        'submitForSettlement' => TRUE,
                        //'submitForSettlement' => $payment_method['settings']['commerce_braintree_settle'] == 'settle_now' ? TRUE : FALSE,
                    ),
                ),
                'redirectUrl' => url('/fit_store/checkout/' . $order->order_id . '?payment_redirect_key=' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
            ));
        }
        elseif (module_exists('commerce_cardonfile')) {
            // Load the selected card on file.
            $card = commerce_cardonfile_load($data['card_id']);

            // Create a transaction data string using the Braintree client.
            $trData = Braintree_TransparentRedirect::transactionData(array(
                // Add transaction related data.
                'transaction' => array(
                    'type' => Braintree_Transaction::SALE,
                    'amount' => $amount,
                    'paymentMethodToken' => $card->remote_id,
                ),
                'redirectUrl' => url('/fit_store/checkout/' . $order->order_id . '?payment_redirect_key=' . $order->data['payment_redirect_key'], array('absolute' => TRUE))
            ));

            $form['card']['#markup'] = theme('card_data_overview', array('card_data' => $card));
        }

        // Store the Transparent Redirect request data in the form.
        $form['tr_data'] = array(
            '#type' => 'hidden',
            '#default_value' => $trData,
            '#name' => 'tr_data',
        );

        // Provide a submit button with a back link that returns to the payment
        // redirect back URL, which sends the customer to the previous page.
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Process payment'),
            '#suffix' => l(t('Cancel'), 'checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
        );

        // Set the action URL to submit directly to Braintree's server.
        $form['#action'] = Braintree_TransparentRedirect::url();


    $option = array();
    $post_values['tr_data'] = $trData;

    $option['method'] = 'POST';
    $option['data'] = drupal_http_build_query($post_values);
    $request = drupal_http_request($form['#action'], $option);
    $feedback = explode('?', $request->headers['location']);
    $feedback=$feedback[1];
    commerce_braintree_tr_process_transaction($order, $payment_method, $feedback, $redirect = TRUE);


}



/**
 * Determines an API user's access to create a new checkout.
 */
function commerce_services_checkout_create_access($data) {
    return TRUE;
}

/**
 * Updates a checkout.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_checkout_update($checkout_id, $data, $flatten_fields) {
 global $user;
 if(is_array($data['items'][0])) {
      $array = array();
      foreach($data['items'][0] as $key => $items){
          $checkout = commerce_checkout_load($items[0]);
          if (!commerce_checkout_access('update', $checkout)) {
              return services_error(t('Access to this operation not granted'), 401);
          }
          $array[] = _cs_checkout_update($key, $items[0], $flatten_fields);
      }
      return $array;
 } else if ($data['node'] == "true") {
     unset($data['node']);
     $checkout_node = node_load($checkout_id);
     $uid = $user->uid;
     $access = false;
     if ($uid == $checkout_node->uid) {$access = true;}
     if (is_array($user->roles) && in_array('administrator', $user->roles)) {$access = true;}
     if ($access == false) {return services_error(t('Access to this operation not granted'), 401);}

     if (!empty($data['type'])) {$checkout_node->type = $data['type']; unset($data['type']);}
     if (!empty($data['title'])) {$checkout_node->title = $data['title'];unset($data['title']);}
     if (!empty($data['sku'])) {$checkout_node->sku = $data['sku'];  unset($data['sku']);}
     if (!empty($data['description'])) {$checkout_node->body['und'][0]['value'] = $data['description']; unset($data['description']);}
     if (is_array($data['checkouts'][0])) {$checkouts = $data['checkouts'][0];}
     $checkouts = $data['checkouts'];
     unset($data['checkouts']);
     foreach($data as $key=>$value){
         $checkout_node->{$key}= $value;
     }
     node_save($checkout_node);
     if(is_array($checkouts)) {
         foreach($checkouts as $key => $items){
             $array[] = _cs_checkout_update($key, $items, false);
         }
         return array('node_checkout'=>$checkout_node, 'items'=>$array);

     }
     return $checkout_node;
 } else {
     $checkout = commerce_checkout_load($checkout_id);
     if (!commerce_checkout_access('update', $checkout)) {
         return services_error(t('Access to this operation not granted'), 401);
     }
     return _cs_checkout_update($checkout_id, $data, $flatten_fields);
 }
}

function _cs_checkout_update($checkout_id, $data, $flatten_fields) {

    if (!empty($data['size'])) {
        $res = field_info_instances('commerce_checkout', $data['type']);
        foreach ($res as $key1=>$value1){
            if (preg_match('/(.*)(field)(.*)(size)(.*)/ism',$key1,$matches)) {
                if ($data['type'] == 'checkout') {
                    $size_id = commerce_services_get_term_from_name($data['size'], 'checkout_size', $parent_tid = null);
                    if ($size_id) {
                        $data[$matches[0]] = $size_id;
                    }
                } else {
                    $data[$matches[0]]=$data['size'];
                }
            }
        }
        unset($data['size']);
    }
    return commerce_services_update_entity('commerce_checkout', $checkout_id, $data, $flatten_fields);
}

/**
 * Determines an API user's access to update a given checkout.
 *
 * @param $checkout_id
 *   The ID of the checkout to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the checkout.
 */
function commerce_services_checkout_update_access($checkout_id) {
  return TRUE;
}

/**
 * Deletes a checkout.
 *
 * @param $checkout_id
 *   The ID of the checkout to be deleted.
 */
function commerce_services_checkout_delete($checkout_id) {
  commerce_checkout_delete($checkout_id);
}

/**
 * Determines an API user's access to delete a given checkout.
 *
 * @param $checkout_id
 *   The ID of the checkout to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the checkout.
 */
function commerce_services_checkout_delete_access($checkout_id) {
  // Attempt to load the checkout.
  if ($checkout = commerce_checkout_load($checkout_id)) {
    // If the user has access to perform the operation...
    if (commerce_checkout_access('delete', $checkout)) {
      // And the checkout can safely be deleted...
      if (commerce_checkout_can_delete($checkout)) {
        return TRUE;
      }
      else {
        return services_error(t('checkout cannot be deleted'), 403);
      }
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('checkout not found'), 404);
  }
}
