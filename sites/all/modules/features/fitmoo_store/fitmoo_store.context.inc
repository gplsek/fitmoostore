<?php
/**
 * @file
 * fitmoo_store.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fitmoo_store_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'checkout';
  $context->description = '';
  $context->tag = 'checkout';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'checkout' => 'checkout',
        'checkout/*' => 'checkout/*',
        '~checkout/*/review' => '~checkout/*/review',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-commerce_cart_summary-block_1' => array(
          'module' => 'views',
          'delta' => 'commerce_cart_summary-block_1',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('checkout');
  $export['checkout'] = $context;

  return $export;
}
