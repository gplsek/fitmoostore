<?php
/**
 * @file
 * fitmoo_store.features.inc
 */

/**
 * Implements hook_commerce_flat_rate_default_services().
 */
function fitmoo_store_commerce_flat_rate_default_services() {
  $items = array(
    'manual_shipping_fee' => array(
      'name' => 'manual_shipping_fee',
      'base' => 'manual_shipping_fee',
      'display_title' => 'Shipping',
      'description' => 'Shipping',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate_manual_shipping_fee',
      'weight' => 0,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => 'manual_shipping_fee_details_form',
        'details_form_validate' => 'manual_shipping_fee_details_form_validate',
        'details_form_submit' => 'manual_shipping_fee_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => 'manual_shipping_fee',
      'base_rate' => array(
        'amount' => 799,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
    ),
  );
  return $items;
}

/**
 * Implements hook_commerce_product_default_types().
 */
function fitmoo_store_commerce_product_default_types() {
  $items = array(
    'clothing' => array(
      'type' => 'clothing',
      'name' => 'clothing',
      'description' => '',
      'help' => '',
      'revision' => 1,
    ),
    'commerce_sp_subscription' => array(
      'type' => 'commerce_sp_subscription',
      'name' => 'Membership SP',
      'description' => 'A product type for selling subscriptions.',
      'help' => '',
      'revision' => 1,
    ),
    'event' => array(
      'type' => 'event',
      'name' => 'event',
      'description' => 'Event based product',
      'help' => '',
      'revision' => 1,
    ),
    'membership' => array(
      'type' => 'membership',
      'name' => 'Membership',
      'description' => '',
      'help' => '',
      'revision' => 1,
    ),
    'pants_shorts' => array(
      'type' => 'pants_shorts',
      'name' => 'Pants_shorts',
      'description' => '',
      'help' => '',
      'revision' => 1,
    ),
    'product' => array(
      'type' => 'product',
      'name' => 'Product',
      'description' => 'A basic product type.',
      'help' => '',
      'revision' => 1,
    ),
    'service' => array(
      'type' => 'service',
      'name' => 'service',
      'description' => '',
      'help' => '',
      'revision' => 1,
    ),
    'shoes' => array(
      'type' => 'shoes',
      'name' => 'shoes',
      'description' => 'shoes',
      'help' => '',
      'revision' => 0,
    ),
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function fitmoo_store_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fitmoo_store_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function fitmoo_store_node_info() {
  $items = array(
    'article' => array(
      'name' => t('Article'),
      'base' => 'node_content',
      'description' => t('Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'clothing' => array(
      'name' => t('Clothing'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'membership' => array(
      'name' => t('Membership'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'page' => array(
      'name' => t('Basic page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'product' => array(
      'name' => t('Product'),
      'base' => 'node_content',
      'description' => t('Product Display'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'service' => array(
      'name' => t('service'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'shoes' => array(
      'name' => t('Shoes'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_search_api_index().
 */
function fitmoo_store_default_search_api_index() {
  $items = array();
  $items['default_node_index'] = entity_import('search_api_index', '{
    "name" : "Default node index",
    "machine_name" : "default_node_index",
    "description" : "An automatically created search index for indexing node data. Might be configured to specific needs.",
    "server" : null,
    "item_type" : "node",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "data_alter_callbacks" : { "search_api_alter_node_access" : { "status" : 1, "weight" : "0", "settings" : [] } },
      "processors" : {
        "search_api_case_ignore" : { "status" : 1, "weight" : "0", "settings" : { "strings" : 0 } },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\nh2 = 3\\nh3 = 2\\nstrong = 2\\nb = 2\\nem = 1.5\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : { "spaces" : "[^\\\\p{L}\\\\p{N}]", "ignorable" : "[-]" }
        }
      },
      "fields" : {
        "type" : { "type" : "string" },
        "title" : { "type" : "text", "boost" : "5.0" },
        "promote" : { "type" : "boolean" },
        "sticky" : { "type" : "boolean" },
        "created" : { "type" : "date" },
        "changed" : { "type" : "date" },
        "author" : { "type" : "integer", "entity_type" : "user" },
        "comment_count" : { "type" : "integer" },
        "search_api_language" : { "type" : "string" },
        "body:value" : { "type" : "text" }
      }
    },
    "enabled" : "0",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
