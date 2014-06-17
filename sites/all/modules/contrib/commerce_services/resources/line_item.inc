<?php

/**
 * @file
 * Contains the resource callbacks for orders.
 */


/**
 * Returns a collection of line items.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_line_item_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
   return commerce_services_index_entities('commerce_line_item', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index line items.
 */
function commerce_services_line_item_index_access() {
  // Because the entity access control system will filter any result sets,
  // there's no reason to limit a particular user's access. Users with the
  // administer line item permission will have access to view any line item but
  // other users will only have access to line items on orders they can view.
  return TRUE;
}

/**
 * Returns a single line item.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_line_item_retrieve($line_item_id, $expand_entities, $flatten_fields) {
  return commerce_services_retrieve_entity('commerce_line_item', $line_item_id, $expand_entities, $flatten_fields);
}

/**
 * Determines an API user's access to retrieve a given line item.
 *
 * @param $line_item_id
 *   The ID of the line item to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the line item.
 */
function commerce_services_line_item_retrieve_access($line_item_id) {
  // Attempt to load the line item.
  if ($line_item = commerce_line_item_load($line_item_id)) {
    // And perform the view access check.
    if (commerce_line_item_access('view', $line_item)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Line item not found'), 404);
  }
}
/**
 * Creates a new cart order for the current API user. (IMPORTED from cart.inc)
 */
function commerce_services_cart_create($data, $flatten_fields) {
    global $user;
    $uid = !empty($user->uid) ? $user->uid : 0;

    // Even though the data array could technically accept a uid value, since the
    // resource is designed to work for the current API user this value will be
    // ignored and result in an error.
    if (isset($data['uid'])) {
        return services_error(t('Creating a cart does not permit you to set a uid value'), 400);
    }

    // Create the new order with the customer's uid and the cart order status.
    // Instead of using commerce_cart_order_new() directly, its contents have been
    // copied here to allow for setting field and property data before saving so
    // any errors in the data prevent saving a new order.
    $order = commerce_order_new($uid, 'cart');
    $order->log = t('Created as a shopping cart order.');

    // Set the field and property data and save the new order.
    commerce_services_set_field_values('commerce_order', $order, $data, $flatten_fields);
    commerce_order_save($order);

    // Reset the cart cache
    commerce_cart_order_ids_reset();

    // If the user is not logged in, ensure the order ID is stored in the session.
    if (!$uid && empty($user->uid)) {
        commerce_cart_order_session_save($order->order_id);
    }

    // Add simplified fields to the order object for certain field types.
    commerce_services_decorate_entity('commerce_order', $order);

    // Flatten field value arrays if specified. This must be the last operation
    // performed as it breaks the standard field data model. An entity whose
    // fields have thus been flattened is no longer wrappable or writable.
    if ($flatten_fields == 'true') {
        $order = clone($order);
        commerce_services_flatten_fields('commerce_order', $order);
    }

    return $order;
}


/**
 * Creates a new line item on an order.
 */
function commerce_services_line_item_create($data, $flatten_fields) {


    global $user;
//====check&create order
    $uid = !empty($user->uid) ? $user->uid : 0;
    if ($uid == 0) {
        return services_error(t('Access to this operation not granted'), 401);
    }
    $order_id = db_query('SELECT order_id FROM {commerce_order} WHERE uid = :uid AND status IN (:status_ids) ORDER BY order_id DESC', array(':uid' => $uid, ':status_ids' => 'cart'))->fetchField();

    if (empty($order_id)) {
      $order =  commerce_services_cart_create(array(), $flatten_fields);
      $order_id = $order->order_id;
    }
  commerce_services_line_item_create_access($data);

  if (!empty($data['callback'])){
     $_SESSION['callback'] = $data['callback'];
     unset($data['callback']);
  }

  $order = commerce_order_load($order_id);

  if ($data['type'] != 'shipping') {

      $product = commerce_product_load($data['commerce_product']);
      foreach ($order->commerce_line_items['und'] as $line) {
          $line_item = commerce_line_item_load($line['line_item_id']);
          if ($line_item->type != 'shipping') {
              if($product->product_id != $line_item->commerce_product['und'][0]['product_id']){
                  commerce_cart_order_product_line_item_delete($order, $line['line_item_id'], FALSE);
              }
          }
      }

      foreach ($order->commerce_line_items['und'] as $key=>$value) {
          $line_item =  $entity = entity_load_single('commerce_line_item', $value['line_item_id']);
          if ($data['commerce_product'] == $line_item->commerce_product['und'][0]['product_id']) {
                $data['quantity'] = $line_item->quantity + $data['quantity'];
                $line = commerce_services_line_item_update($value['line_item_id'], $data,false);
                return $line;
                break;
          }
      }
      add_manual_shipping_line_item($order, $data);
  }
  $data['order_id'] = $order_id;
  if ($data['type'] == 'shipping' and !empty($data['shipping_service_name'])) {
        $order = commerce_order_load($data['order_id']);
        return add_shipping_line_item($order, $data['shipping_service_name']);
  }

  if (empty($data['type']) || !in_array($data['type'], array_keys(commerce_line_item_types()))) {
    return services_error(t('You must specify a valid line item type'), 400);
  }
  // Default the quantity to 1.
  if (empty($data['quantity'])) {
    $data['quantity'] = 1;
  }

  // Create the new line item.
  $line_item = commerce_line_item_new($data['type'], $data['order_id']);

  // Remove the type and order_id from the data array since they've been set.
  unset($data['type'], $data['order_id']);

  // If the line item is a product line item, ensure a commerce_product value
  // was passed referencing a valid product ID and populate the line item.
  if (in_array($line_item->type, commerce_product_line_item_types())) {
    $product = NULL;

    if (!empty($data['commerce_product'])) {
      if ($flatten_fields == 'true') {
        $product = commerce_product_load($data['commerce_product']);
      }
      else {
        $product = commerce_product_load($data['commerce_product']['und'][0]);
      }
    }

    // Bail now if the product wasn't found.
    if (empty($product)) {
      return services_error(t('You must specify a valid commerce_product value'), 400);
    }

    // Populate the line item.
    commerce_product_line_item_populate($line_item, $product);
  }
  // Set the field and property data and save the line item.
  commerce_services_set_field_values('commerce_line_item', $line_item, $data, $flatten_fields);
  commerce_line_item_save($line_item);

  // Add the line item to its order.
  $order = commerce_order_load($line_item->order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_wrapper->commerce_line_items[] = $line_item;
  commerce_order_save($order);

  // Add simplified fields to the line item object for certain field types.
  commerce_services_decorate_entity('commerce_line_item', $line_item);

  // Flatten field value arrays if specified. This must be the last operation
  // performed as it breaks the standard field data model. An entity whose
  // fields have thus been flattened is no longer wrappable or writable.
  if ($flatten_fields == 'true') {
    $line_item = clone($line_item);
    commerce_services_flatten_fields('commerce_line_item', $line_item);
  }

  return $line_item;
}
/**
 * FUNCTION FOR save image
 */

function migrate_billboard_get_managed_file($uri) {
    $fid = db_query('SELECT fid FROM {file_managed} WHERE uri = :uri', array(':uri' => $uri))->fetchField();

    if (!empty($fid)) {
        $file_object = file_load($fid);
        return $file_object;
    }
    return FALSE;
}




/**
 * FUNCTION delete lineitem_seller
 */

function commerce_lineitem_seller_delete($line_id) {
   return db_delete('commerce_lineitem_x_seller')->condition('line_id', $line_id)->execute();
}

/**
 * Creates a new shipping line item on an order.
 */
function add_shipping_line_item($order, $shipping_service_name, $skip_order_save = FALSE) {
    $shipping_service = commerce_shipping_service_load($shipping_service_name);
    if (empty($shipping_service)) {
        return;
    }

    // Collect shipping rates for the order.
    commerce_shipping_collect_rates($order);
    if (empty($order->shipping_rates) || empty($order->shipping_rates[$shipping_service_name])) {
        return;
    }

    // Extract the unit price from the calculated rate.
    $rate_line_item = $order->shipping_rates[$shipping_service_name];
    $rate_line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $rate_line_item);
    $unit_price = $rate_line_item_wrapper->commerce_unit_price->value();

    // Create a new shipping line item with the calculated rate.
    $line_item = commerce_shipping_line_item_new($shipping_service_name, $unit_price, $order->order_id, $rate_line_item->data, $rate_line_item->type);

    // Delete any existing shipping line items from the order.
    commerce_shipping_delete_shipping_line_items($order, $skip_order_save);

    // Save and add the line item to the order.
    return commerce_shipping_add_shipping_line_item($line_item, $order, $skip_order_save);
}

/**
 * Creates a new shipping line item on an order.
 */
function add_manual_shipping_line_item($order, $data) {
    $product = commerce_services_retrieve_entity('commerce_product', $data['commerce_product'], false, false);

    $shipping_types_array=array();

    //====OR MAY BE STORE ARRAY IN SETVAR ?
    $types = commerce_product_reference_node_types();
    foreach ($types as $type => $info) {
        $shipping_types_array[$info->type]=variable_get($info->type."_fee", "");
    }
    if ($shipping_types_array[$product->type] == 0) {
        return false;
    }


    $unit_price = array(
        "amount"=>100,
        "currency_code"=>"USD",
        "data"=>array(
            "components"=>array(
                "0"=>array(
                    "name"=>"shipping",
                    "price"=>array(
                        "amount"=>100,
                        "currency_code"=>"USD",
                        "data"=>array(),
                    ),
                    "included"=>true,
                ),
            ),
        ),
    );

    $shipping_service = array(
        "name"=>"example_shipping_service",
        "base"=>"example_shipping_service",
        "display_title" => "Shipping",
        "description"=>"Set shipping fy by product type",
        "shipping_method"=>"example_shipping_method",
        "rules_component"=>true,
        "price_component"=>"shipping",
        "weight"=>2,
        "callbacks"=>array(
            "rate"=>"commerce_shipping_example_service_rate",
            "details_form"=>"commerce_shipping_example_service_details_form",
            "details_form_validate"=>"commerce_shipping_example_service_details_form_validate",
            "details_form_submit"=>"commerce_shipping_example_service_details_form_submit"
        ),
        "module"=>"commerce_shipping_example",
        "title"=>"Example shipping service",
        "admin_list"=>true,
        "base_rate"=>array("amount"=>100)
    );

    $temp = new stdClass();
    $temp->type="shipping";
    $temp->order_id=$order->order_id;
    $temp->quantity = 1;
    $temp->data=array("shipping_service"=>$shipping_service);
    $temp->line_item_id=null;
    $temp->line_item_label="Shipping";
    $temp->created="";
    $temp->changed="";
    $temp->is_new=true;
    $temp->commerce_shipping_service=array("und"=>array("0"=>array("value"=>"example_shipping_service")));
    $temp->commerce_unit_price = array("und"=>array("0"=>$unit_price));
    $temp->weight=2;
    $order->shipping_rates['example_shipping_service']=$temp;


//    $shipping_service_name= 'example_shipping_service';
//
//    $shipping_service = commerce_shipping_service_load($shipping_service_name);
//    if (empty($shipping_service)) {
//        return;
//    }
//    // Collect shipping rates for the order.
//    commerce_shipping_collect_rates($order);
//    if (empty($order->shipping_rates) || empty($order->shipping_rates[$shipping_service_name])) {
//        return;
//    }

    $order->shipping_rates['example_shipping_service']->data['shipping_service']['base_rate']['amount'] = $shipping_types_array[$product->type]*100;
    $order->shipping_rates['example_shipping_service']->commerce_unit_price['und'][0]['amount'] = $shipping_types_array[$product->type]*100;
    $order->shipping_rates['example_shipping_service']->commerce_unit_price['und'][0]['data']['components'][0]['price']['amount'] = $shipping_types_array[$product->type]*100;
    // Extract the unit price from the calculated rate.
    $rate_line_item = $order->shipping_rates['example_shipping_service'];
    $rate_line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $rate_line_item);
    $unit_price = $rate_line_item_wrapper->commerce_unit_price->value();

    // Create a new shipping line item with the calculated rate.
    $line_item = commerce_shipping_line_item_new('example_shipping_service', $unit_price, $order->order_id, $rate_line_item->data, $rate_line_item->type);


    // Delete any existing shipping line items from the order.
    commerce_shipping_delete_shipping_line_items($order, false);

    // Save and add the line item to the order.
    return commerce_shipping_add_shipping_line_item($line_item, $order, false);
}


/**
 * Determines an API user's access to create new line items.
 */
function commerce_services_line_item_create_access($data) {
  // Load the order from the data array to determine access to create line items
  // on it.
  $order = NULL;

//  if (!empty($data['order_id'])) {
//    $order = commerce_order_load($data['order_id']);
//  }
//
//  if (empty($order)) {
//    return services_error(t('You must specify a valid order ID', 400));
//  }
//
//  // If the user has access to update the order...
//  if (commerce_order_access('update', $order)) {
    return TRUE;
//  }
//  else {
//    return services_error(t('Access to this operation not granted'), 401);
//  }
}

/**
 * Updates a line item.
 *
 * This callback does not use commerce_services_update_entity(), because we must
 * ensure that the unit price is rebased if its amount or currency code change
 * but the data array is not updated to include the requisite price component.
 */
function commerce_services_line_item_update($line_item_id, $data, $flatten_fields) {
  // Load the line item.
  $line_item = commerce_line_item_load($line_item_id);

  // Set field values using the data provided.
  commerce_services_set_field_values('commerce_line_item', $line_item, $data, $flatten_fields);

  // Loop over the update data to determine if the unit price amount or
  // currency_code is updated without the data array being updated to include
  // price component updates...
  $rebase = FALSE;

  foreach (array_keys($data) as $key) {
    if (in_array($key, array('commerce_unit_price_amount', 'commerce_unit_price_currency_code')) &&
      !in_array('commerce_unit_price_data', array(array_keys($data)))) {
      $rebase = TRUE;
    }
    elseif ($key == 'commerce_unit_price' && empty($data[$key]['data']['components'])) {
      $rebase = TRUE;
    }
  }

  // Rebase the unit price if necessary.
  if ($rebase) {
    commerce_line_item_rebase_unit_price($line_item);
  }

  // Save the updates to the line item.
  commerce_line_item_save($line_item);

  // Add simplified fields to the line item for certain field types.
  commerce_services_decorate_entity('commerce_line_item', $line_item);

  // Flatten field value arrays if specified. This must be the last operation
  // performed as it breaks the standard field data model. An entity whose
  // fields have thus been flattened is no longer wrappable or writable.
  if ($flatten_fields == 'true') {
    $line_item = clone($line_item);
    commerce_services_flatten_fields('commerce_line_item', $line_item);
  }

  // If the line item quantity or unit price was updated, load and save the
  // order to recalculate the order total.
  foreach (array_keys($data) as $key) {
    if (in_array($key, array('quantity', 'commerce_unit_price')) || strpos($key, 'commerce_unit_price_') === 0) {
      $order = commerce_order_load($line_item->order_id);
      commerce_order_save($order);
      break;
    }
  }

  return $line_item;
}

/**
 * Determines an API user's access to update a given line item.
 *
 * @param $line_item_id
 *   The ID of the line item to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the line item.
 */
function commerce_services_line_item_update_access($line_item_id) {
  // Attempt to load the line item.
  if ($line_item = commerce_line_item_load($line_item_id)) {
    // If the user has access to perform the operation...
    if (commerce_line_item_access('update', $line_item)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Line item not found'), 404);
  }
}

/**
 * Deletes a line item.
 *
 * @param $line_item_id
 *   The ID of the line item to be deleted.
 */
function commerce_services_line_item_delete($line_item_id) {
  commerce_line_item_delete($line_item_id);
}

/**
 * Determines an API user's access to delete a given line item.
 *
 * @param $line_item_id
 *   The ID of the line item to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the line item.
 */
function commerce_services_line_item_delete_access($line_item_id) {
  // Attempt to load the line item.
  if ($line_item = commerce_line_item_load($line_item_id)) {
    // If the user has access to perform the operation...
    if (commerce_line_item_access('delete', $line_item)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Line item not found'), 404);
  }
}
