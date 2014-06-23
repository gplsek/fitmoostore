<?php

/**
 * @file
 * Contains the resource callbacks for products.
 */


/**
 * Returns a collection of products.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_product_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  return commerce_services_index_entities('commerce_product', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index products.
 */
function commerce_services_product_index_access() {
  // Because the entity access control system will filter any result sets,
  // there's no reason to limit a particular user's access.
  return TRUE;
}

/**
 * Returns a single product.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_product_retrieve($product_id, $expand_entities, $flatten_fields) {

    return commerce_services_retrieve_entity('commerce_product', $product_id, $expand_entities, $flatten_fields);
}

/**
 * Determines an API user's access to retrieve a given product.
 *
 * @param $product_id
 *   The ID of the product to be retrieved.
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
 * Creates a new product.
 */
function commerce_services_product_create($data, $flatten_fields) {
  // Ensure the request specifies a unique, valid SKU.
  global $user;
  if (!empty($data['node']) and ($data['node'] == "true")) {
      unset($data['node']);
      $product_node = new stdClass();
      $product_node->type = $data['type']; //could be event at this test point
      unset($data['type']);
      $product_node->title = $data['title'];
      unset($data['title']);
      $product_node->sku = $data['sku'];
      unset($data['sku']);

      if (!empty($data['field_brand'])) {
          $brand_id = commerce_services_get_term_from_name($data['field_brand'], 'brand', $parent_tid = null);
          if ($brand_id){$product_node->field_brand['und'][0]['tid']= $brand_id;}
          unset($data['field_brand']);
      }
      if (!empty($data['field_gender'])) {
          $gender_id = commerce_services_get_term_from_name($data['field_gender'], 'gender', $parent_tid = null);
          if ($gender_id){$product_node->field_gender['und'][0]['tid']= $gender_id;}
          unset($data['field_gender']);
      }

      $product_node->uid = $user->uid;  //product owner drupal user id - should the the logged in user uid $user->uid;
      $product_node->status = 1;
      $product_node->promote = 1;
      $product_node->sticky = 1;
      $product_node->language = 'und';
      $product_node->body['und'][0]['value'] = $data['description'];
      unset($data['description']);
      $prod_array = $data['products'];
      unset($data['products']);
      foreach($data as $key=>$value){
          $product_node->{$key}= $value;
      }


      foreach($prod_array as $key=>$prod){
          //========CREATE_PRODUCT_ITEM
          if (!empty($prod['size'])) {
            $res = field_info_instances('commerce_product', $prod['type']);
            foreach ($res as $key1=>$value1){
               if (preg_match('/(.*)(field)(.*)(size)(.*)/ism',$key1,$matches)) {
                   if ($prod['type'] == 'product') {
                       $size_id = commerce_services_get_term_from_name($prod['size'], 'product_size', $parent_tid = null);
                       if ($size_id) {
                         $prod[$matches[0]] = $size_id;
                       }
                   } else {
                       $prod[$matches[0]]=$prod['size'];
                   }
               }
            }
          unset($prod['size']);
          }

          if (isset($prod['sku'])) {
              $prod['sku'] = trim($prod['sku']);
          }
          $prod['uid'] = $user->uid;
          if (empty($prod['sku'])) {
              return services_error(t('You must specify a sku value'), 400);
          }
          elseif (!commerce_product_validate_sku($prod['sku'])) {
              return services_error(t('Invalid sku value given'), 400);
          }
          elseif (!commerce_product_validate_sku_unique($prod['sku'], NULL)) {
              return services_error(t('You must specify a unique sku value'), 400);
          }

          // Ensure the request specifies a title.
          if (empty($prod['title'])) {
              return services_error(t('You must specify a title value'), 400);
          }

          // Ensure the request specifies a commerce_price amount and currency_code.
          $amount = NULL;
          $currency_code = NULL;

          // If the commerce_price field is set in the data array, examine its values
          // for a price amount and currency code.
          if (!empty($prod['commerce_price'])) {
              if ($flatten_fields == 'true') {
                  if (isset($prod['commerce_price']['amount']) && is_numeric($prod['commerce_price']['amount'])) {
                      $amount = $prod['commerce_price']['amount'];
                  }
                  if (!empty($prod['commerce_price']['currency_code']) && commerce_currency_load($prod['commerce_price']['currency_code'])) {
                      $currency_code = $prod['commerce_price']['currency_code'];
                  }
              }
              else {
                  $langcode = key($prod['commerce_price']);

                  if (isset($prod['commerce_price'][$langcode][0]['amount']) && is_numeric($prod['commerce_price'][$langcode][0]['amount'])) {
                      $amount = $prod['commerce_price'][$langcode][0]['amount'];
                  }
                  if (!empty($prod['commerce_price'][$langcode][0]['currency_code']) && commerce_currency_load($prod['commerce_price'][$langcode][0]['currency_code'])) {
                      $currency_code = $prod['commerce_price'][$langcode][0]['currency_code'];
                  }
              }
          }

          // If not, look also in the column specific properties.
          if (isset($prod['commerce_price_amount']) && is_numeric($prod['commerce_price_amount'])) {
              $amount = $prod['commerce_price_amount'];
          }
          if (!empty($prod['commerce_price_currency_code']) && commerce_currency_load($prod['commerce_price_currency_code'])) {
              $currency_code = $prod['commerce_price_currency_code'];
          }

          if (is_null($amount)) {
              return services_error(t('You must specify a numeric commerce_price_amount value'), 400);
          }
          elseif (is_null($currency_code)) {
              return services_error(t('You must specify a valid commerce_price_currency_code value'), 400);
          }

          // Create the new product.
          $product_item = commerce_product_new($prod['type']);

          // Remove the type from the data array since it's been set.
          unset($prod['type']);

          //images saving
          if ($prod['image']) {
              $filename = basename($prod['image']);
              // $filename = commerce_services_check_file('public://',$filename);
              $image_file = system_retrieve_file($prod['image'], 'public://'.$filename, TRUE);
              $prod['field_image']['und'][0] = (array) $image_file;
              $prod['field_image_url'] = file_create_url($image_file->uri);
              unset($prod['image-url']);
              unset($prod['image']);
          }

          // Set the field and property data and save the product.
          commerce_services_set_field_values('commerce_product', $product_item, $prod, $flatten_fields);
          commerce_product_save($product_item);

          // Add simplified fields to the product object for certain field types.
          commerce_services_decorate_entity('commerce_product', $product_item);

          // Flatten field value arrays if specified. This must be the last operation
          // performed as it breaks the standard field data model. An entity whose
          // fields have thus been flattened is no longer wrappable or writable.
          if ($flatten_fields == 'true') {
              $product_item = clone($product_item);
              commerce_services_flatten_fields('commerce_product', $product_item);
          }
          //========END_CREATE_PRODUCT_ITEM
          $product_node->field_product['und'][$key]['product_id'] = $product_item->product_id;
      }
      node_save($product_node);
      return $product_node;
  } else {

      if (!empty($data['size'])) {
          $res = field_info_instances('commerce_product', $data['type']);
          foreach ($res as $key1=>$value1){
              if (preg_match('/(.*)(field)(.*)(size)(.*)/ism',$key1,$matches)) {
                  if ($data['type'] == 'product') {
                      $size_id = commerce_services_get_term_from_name($data['size'], 'product_size', $parent_tid = null);
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

      if (isset($data['sku'])) {
          $data['sku'] = trim($data['sku']);
      }
      $data['uid'] = $user->uid;
      if (empty($data['sku'])) {
          return services_error(t('You must specify a sku value'), 400);
      }
      elseif (!commerce_product_validate_sku($data['sku'])) {
          return services_error(t('Invalid sku value given'), 400);
      }
      elseif (!commerce_product_validate_sku_unique($data['sku'], NULL)) {
          return services_error(t('You must specify a unique sku value'), 400);
      }

      // Ensure the request specifies a title.
      if (empty($data['title'])) {
          return services_error(t('You must specify a title value'), 400);
      }

      // Ensure the request specifies a commerce_price amount and currency_code.
      $amount = NULL;
      $currency_code = NULL;

      // If the commerce_price field is set in the data array, examine its values
      // for a price amount and currency code.
      if (!empty($data['commerce_price'])) {
          if ($flatten_fields == 'true') {
              if (isset($data['commerce_price']['amount']) && is_numeric($data['commerce_price']['amount'])) {
                  $amount = $data['commerce_price']['amount'];
              }
              if (!empty($data['commerce_price']['currency_code']) && commerce_currency_load($data['commerce_price']['currency_code'])) {
                  $currency_code = $data['commerce_price']['currency_code'];
              }
          }
          else {
              $langcode = key($data['commerce_price']);

              if (isset($data['commerce_price'][$langcode][0]['amount']) && is_numeric($data['commerce_price'][$langcode][0]['amount'])) {
                  $amount = $data['commerce_price'][$langcode][0]['amount'];
              }
              if (!empty($data['commerce_price'][$langcode][0]['currency_code']) && commerce_currency_load($data['commerce_price'][$langcode][0]['currency_code'])) {
                  $currency_code = $data['commerce_price'][$langcode][0]['currency_code'];
              }
          }
      }

      // If not, look also in the column specific properties.
      if (isset($data['commerce_price_amount']) && is_numeric($data['commerce_price_amount'])) {
          $amount = $data['commerce_price_amount'];
      }
      if (!empty($data['commerce_price_currency_code']) && commerce_currency_load($data['commerce_price_currency_code'])) {
          $currency_code = $data['commerce_price_currency_code'];
      }

      if (is_null($amount)) {
          return services_error(t('You must specify a numeric commerce_price_amount value'), 400);
      }
      elseif (is_null($currency_code)) {
          return services_error(t('You must specify a valid commerce_price_currency_code value'), 400);
      }

      // Create the new product.
      $product = commerce_product_new($data['type']);
	  
	  

      // Remove the type from the data array since it's been set.
      unset($data['type']);

      //images saving
      if ($data['image']) {
          $filename = basename($data['image']);
          // $filename = commerce_services_check_file('public://',$filename);
          $image_file = system_retrieve_file($data['image'], 'public://'.$filename, TRUE);
          $data['field_image']['und'][0] = (array) $image_file;
          $data['field_image_url'] = file_create_url($image_file->uri);
          unset($data['image-url']);
          unset($data['image']);
      }


      // Set the field and property data and save the product.
      commerce_services_set_field_values('commerce_product', $product, $data, $flatten_fields);
      commerce_product_save($product);

      // Add simplified fields to the product object for certain field types.
      commerce_services_decorate_entity('commerce_product', $product);

      // Flatten field value arrays if specified. This must be the last operation
      // performed as it breaks the standard field data model. An entity whose
      // fields have thus been flattened is no longer wrappable or writable.
      if ($flatten_fields == 'true') {
          $product = clone($product);
          commerce_services_flatten_fields('commerce_product', $product);
      }

      return $product;

  }


}

/**
 * Determines an API user's access to create a new product.
 */
function commerce_services_product_create_access($data) {
  // Ensure the request specifies a valid product type.
  if (empty($data['type']) || !in_array($data['type'], array_keys(commerce_product_types()))) {
    return services_error(t('You must specify a valid product type'), 400);
  }

  // Check the user's access to create products of the specified type.
  if (commerce_product_access('create', commerce_product_new($data['type']))) {
    return TRUE;
  }
  else {
    return services_error(t('Access to this operation not granted'), 401);
  }
}

//SESS8d3b4b16bda24dae7c6b16bc7fa6f067=9Uhv7o6-VJpv14si-S36Ex3IhEwRIS-KgTi9-fWDTZs
//i1Kthm7mnvtONtaWSMHsU_7CeOu0Vz0hMuD3xC4YhiY


/**
 * Updates a product.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_product_update($product_id, $data, $flatten_fields) {
 global $user;
 if(is_array($data['items'][0])) {
      $array = array();
      foreach($data['items'][0] as $key => $items){
          $product = commerce_product_load($items[0]);
          if (!commerce_product_access('update', $product)) {
              return services_error(t('Access to this operation not granted'), 401);
          }
          $array[] = _cs_product_update($key, $items[0], $flatten_fields);
      }
      return $array;
 } else if ($data['node'] == "true") {
     unset($data['node']);
     $product_node = node_load($product_id);
     $uid = $user->uid;
     $access = false;
     if ($uid == $product_node->uid) {$access = true;}
     if (is_array($user->roles) && in_array('administrator', $user->roles)) {$access = true;}
     if ($access == false) {return services_error(t('Access to this operation not granted'), 401);}

     if (!empty($data['type'])) {$product_node->type = $data['type']; unset($data['type']);}
     if (!empty($data['title'])) {$product_node->title = $data['title'];unset($data['title']);}
     if (!empty($data['sku'])) {$product_node->sku = $data['sku'];  unset($data['sku']);}
     if (!empty($data['description'])) {$product_node->body['und'][0]['value'] = $data['description']; unset($data['description']);}
     if (is_array($data['products'][0])) {$products = $data['products'][0];}
     $products = $data['products'];
     unset($data['products']);
     foreach($data as $key=>$value){
         $product_node->{$key}= $value;
     }
     node_save($product_node);
     if(is_array($products)) {
         foreach($products as $key => $items){
             $array[] = _cs_product_update($key, $items, false);
         }
         return array('node_product'=>$product_node, 'items'=>$array);

     }
     return $product_node;
 } else {
     $product = commerce_product_load($product_id);
     if (!commerce_product_access('update', $product)) {
         return services_error(t('Access to this operation not granted'), 401);
     }
     return _cs_product_update($product_id, $data, $flatten_fields);
 }
}

function _cs_product_update($product_id, $data, $flatten_fields) {

    if (!empty($data['size'])) {
        $res = field_info_instances('commerce_product', $data['type']);
        foreach ($res as $key1=>$value1){
            if (preg_match('/(.*)(field)(.*)(size)(.*)/ism',$key1,$matches)) {
                if ($data['type'] == 'product') {
                    $size_id = commerce_services_get_term_from_name($data['size'], 'product_size', $parent_tid = null);
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

    //images saving
    if ($data['image']) {
        $filename = basename($data['image']);
        // $filename = commerce_services_check_file('public://',$filename);
        $image_file = system_retrieve_file($data['image'], 'public://'.$filename, TRUE);
        $data['field_image']['und'][0] = (array) $image_file;
        $data['field_image_url'] = file_create_url($image_file->uri);
        unset($data['image-url']);
        unset($data['image']);
    }

    $product = commerce_services_update_entity('commerce_product', $product_id, $data, $flatten_fields);
    if (!empty($data['commerce_stock']))
    {
        //===========fitmoo call=============
        $data = '{"id" : "'.$product_id.'" , "stock" : "'.$data['commerce_stock'].'"}';
        $options = array(
            'method' => 'PUT',
            'data' => $data,
            'timeout' => 30,
            'headers' => array('Content-Type' => 'application/json'),
        );
        $redirect_base = variable_get('redirect_base', '');
        $result['stock'] = drupal_http_request('http://admin:fitmoo@'.$redirect_base.'/api/update_stock', $options);
    }

    return $product;
}

/**
 * Determines an API user's access to update a given product.
 *
 * @param $product_id
 *   The ID of the product to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the product.
 */
function commerce_services_product_update_access($product_id) {
  return TRUE;
}

/**
 * Deletes a product.
 *
 * @param $product_id
 *   The ID of the product to be deleted.
 */
function commerce_services_product_delete($product_id) {
  commerce_product_delete($product_id);
}

/**
 * Determines an API user's access to delete a given product.
 *
 * @param $product_id
 *   The ID of the product to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the product.
 */
function commerce_services_product_delete_access($product_id) {
  // Attempt to load the product.
  if ($product = commerce_product_load($product_id)) {
    // If the user has access to perform the operation...
    if (commerce_product_access('delete', $product)) {
      // And the product can safely be deleted...
      if (commerce_product_can_delete($product)) {
        return TRUE;
      }
      else {
        return services_error(t('Product cannot be deleted'), 403);
      }
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Product not found'), 404);
  }
}
