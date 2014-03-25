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
      foreach ($order_ids as $key=>$value) {
          $temp[]= commerce_services_order_retrieve($value->order_id, false, false);
      }
      return $temp;
  } elseif (!empty($_REQUEST['buyer'])) {
      $order_ids = commerce_services_order_buyer();
      foreach ($order_ids as $key=>$value) {
          $temp[]= commerce_services_order_retrieve($value->order_id, false, false);
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

    foreach ($order as $row) {
        $result[] = $row;
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

    foreach ($order as $row) {
        $result[] = $row;
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
  return commerce_services_retrieve_entity('commerce_order', $order_id, $expand_entities, $flatten_fields);
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
        //commerce_services_retrieve_entity('commerce_line_item', $value->line_item_id);
            //commerce_services_line_item_retrieve($value->line_item_id);
        $product = commerce_product_load_by_sku($line_item->line_item_label);
        $result = commerce_services_product_retrieve_access($product->product_id);
        if ($result == true) {return TRUE;}
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
  return commerce_services_update_entity('commerce_order', $order_id, $data, $flatten_fields);
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
