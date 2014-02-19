<?php
/**
 * @file
 * fitmoo_store.features.inc
 */

/**
 * Implements hook_commerce_product_default_types().
 */
function fitmoo_store_commerce_product_default_types() {
  $items = array(
    'event' => array(
      'type' => 'event',
      'name' => 'event',
      'description' => 'Event based product',
      'help' => '',
      'revision' => 1,
    ),
    'tops' => array(
      'type' => 'tops',
      'name' => 'tops',
      'description' => 'tops, shirts, jackets and etc',
      'help' => '',
      'revision' => 1,
    ),
  );
  return $items;
}

/**
 * Implements hook_node_info().
 */
function fitmoo_store_node_info() {
  $items = array(
    'product' => array(
      'name' => t('Product'),
      'base' => 'node_content',
      'description' => t('Product Display'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'tops' => array(
      'name' => t('Tops'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
