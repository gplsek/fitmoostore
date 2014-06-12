<?php

/**
 * @file
 * Contains the resource callbacks for orders.
 */


/**
 * Returns a collection of orders.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_order_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  if (!empty($_REQUEST['seller'])) {
      $order_ids = commerce_services_order_seller();
      global $user;
      foreach ($order_ids as $key=>$value) {
          $item = commerce_services_order_retrieve($value, 1, true);
          foreach ($item->commerce_line_items_data as $keyitem=>$valueitem) {
              if ($valueitem->commerce_product_entities[$valueitem->commerce_product]->uid != $user->uid) {
                  $delkey = array_search($keyitem,$item->commerce_line_items);
                  unset($item->commerce_line_items_data[$keyitem]);
                  unset($item->commerce_line_items[$delkey]);

              }
          }
          $string = array();
          foreach ($item->commerce_line_items_data as $keyitem=>$valueitem) {
             $string[] = $keyitem;
          }
          $item->commerce_line_items = $string;
          $temp[] = $item;
      }
      return $temp;
  } elseif (!empty($_REQUEST['buyer'])) {
      $order_ids = commerce_services_order_buyer();
      foreach ($order_ids as $key=>$value) {
          $item = commerce_services_order_retrieve($value, 1, true);
          $temp[] = $item;
      }
      return $temp;
  } else {
    return commerce_services_index_entities('commerce_order', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
  }
}
function commerce_services_order_seller() {
    global $user;

    $query = db_select('commerce_lineitem_x_seller', 'c');
    $query->condition('seller_id', $user->uid);
    $order = $query
        ->fields('c', array('order_id'))
        ->execute();

    foreach ($order as $key=>$row) {
        $result[$row->order_id] = $row->order_id;
    }
    return $result;
}


function commerce_services_order_buyer() {
    global $user;

    $query = db_select('commerce_lineitem_x_seller', 'c');
    $query->condition('buyer_id', $user->uid);
    $order = $query
        ->fields('c', array('order_id'))
        ->execute();

    foreach ($order as $key=>$row) {
        $result[$row->order_id] = $row->order_id;
    }
    return $result;
}


/**
 * Determines an API user's access to index orders.
 */
function commerce_services_order_index_access() {
  // Because the entity access control system will filter any result sets,
  // there's no reason to limit a particular user's access.
  return TRUE;
}

/**
 * Returns a single order.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_order_retrieve($order_id, $expand_entities, $flatten_fields) {
   $item = commerce_services_retrieve_entity('commerce_order', $order_id, $expand_entities, $flatten_fields);
   foreach ($item->commerce_line_items as $itemkey=>$itemvalue) {
        $item->commerce_line_items_data[$itemvalue] = commerce_services_retrieve_entity('commerce_line_item', $itemvalue, 1, true);
   }

   return $item;
}

/**
 * Determines an API user's access to retrieve a given order.
 *
 * @param $order_id
 *   The ID of the order to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the order.
 */
function commerce_services_order_retrieve_access($order_id) {

  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
  $result = false;
  foreach ($order->commerce_line_items['und'] as $key=>$value) {
        $line_item =  $entity = entity_load_single('commerce_line_item', $value['line_item_id']);
		//print_r($line_item);die(0)
        //commerce_services_retrieve_entity('commerce_line_item', $value->line_item_id);
        //commerce_services_line_item_retrieve($value->line_item_id);
		if($line_item->line_item_label){ // check on this, beore it was returning product not founf for orderID 404
          $product = commerce_product_load_by_sku($line_item->line_item_label);
          $result = commerce_services_product_retrieve_access($product->product_id);
          if ($result == true) {return TRUE;}
		}
  }


    // And perform the view access check.
    if (commerce_order_access('view', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}

/**
 * COPY
 *
 * @return
 *   Boolean indicating the user's access to retrieve the product.
 */
function commerce_services_product_retrieve_access($product_id) {
    // Attempt to load the product.
    if ($product = commerce_product_load($product_id)) {
        // And perform the view access check.
        if (commerce_product_access('view', $product)) {
            return TRUE;
        }
        else {
            return services_error(t('Access to this operation not granted'), 401);
        }
    }
    else {
        return services_error(t('Product not found'), 404);
    }
}

/**
 * Updates an order.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_order_update($order_id, $data, $flatten_fields) {
    global $user;


    if(!empty($data['commerce_customer_shipping'])) {
      //  $profiles = commerce_customer_profile_load_multiple(array(), array('type' => 'shipping','uid' => $user->uid,'status'=> TRUE));
//        if ($profiles) {
//// Prepare the options for supplying to a select box
//            $shipping_options = array();
//            $find = false;
//            foreach ($profiles as $id => $profile) {
//                $field_values = field_get_items('commerce_customer_profile', $profile, 'commerce_customer_address');
//                $current_find = false;
//                foreach ($field_values as $key=>$value) {
//                    if($data['commerce_customer_shipping']['key'] != $value) { break;}
//                }
          //  }
          $profilenew = commerce_customer_profile_new('shipping', $user->uid);
          foreach ($data['commerce_customer_shipping'] as $key=>$value) {
              $profilenew->commerce_customer_address['und'][0][$key] = $value;
          }
          $result = commerce_customer_profile_save($profilenew);

        unset($data['commerce_customer_shipping']);
        if ($result) {
            $data['commerce_customer_shipping'] = $profilenew->profile_id;
        }
    }
    if(!empty($data['commerce_customer_billing'])) {
        $profilenew2 = commerce_customer_profile_new('billing', $user->uid);
        foreach ($data['commerce_customer_billing'] as $key=>$value) {
            $profilenew2->commerce_customer_address['und'][0][$key] = $value;
        }
        $result2 = commerce_customer_profile_save($profilenew2);

        unset($data['commerce_customer_billing']);
        if ($result2) {
            $data['commerce_customer_billing'] = $profilenew2->profile_id;
        }
    }
  $order = commerce_services_update_entity('commerce_order', $order_id, $data, $flatten_fields);
  if ($result or $result2) {
     $order->commerce_customer_shipping_entities[$profilenew->profile_id] = $profilenew;
     commerce_order_save($order);
  }
  if ($result2) {
     $order->commerce_customer_billing_entities[$profilenew2->profile_id] = $profilenew2;
     commerce_order_save($order);
  }
  
  //move this to firmoo checkout and use order_save_hook to process

 // if ($order->status == 'Shipping: Delivered') {
 //     $payment_method = commerce_payment_method_instance_load('braintree_tr|commerce_payment_braintree_tr');
 //     commerce_braintree_initialize($payment_method);
 //     $transactions = commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id));
//      foreach ($transactions as $transaction) {
 //         if ($transaction->remote_status == 'authorized') {
 //             //$result = Braintree_Transaction::submitForSettlement('the_transaction_id', '35.00');
 //             $result = Braintree_Transaction::submitForSettlement($transaction->remote_id);
//              if ($result->success) {
//                  # Success
//              } else {
//                  print_r($result->errors);
//              }
         // }
      //}
	  //}

  return $order;
}


/**
 * Determines an API user's access to update a given order.
 *
 * @param $order_id
 *   The ID of the order to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the order.
 */
function commerce_services_order_update_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // If the user has access to perform the operation...
    if (commerce_order_access('update', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}

/**
 * Deletes an order.
 *
 * @param $order_id
 *   The ID of the order to be deleted.
 */
function commerce_services_order_delete($order_id) {
  commerce_order_delete($order_id);
}

/**
 * Determines an API user's access to delete a given order.
 *
 * @param $order_id
 *   The ID of the order to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the order.
 */
function commerce_services_order_delete_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // If the user has access to perform the operation...
    if (commerce_order_access('delete', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}

/**
 * Returns a collection of line items on an order.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_order_line_item_index($order_id, $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  // Add the order ID to the filter list.
  $filter['order_id'] = $order_id;
  $filter_op['order_id'] = '=';

  return commerce_services_index_entities('commerce_line_item', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index line items on an order.
 */
function commerce_services_order_line_item_index_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // And perform the view access check.
    if (commerce_order_access('view', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}
