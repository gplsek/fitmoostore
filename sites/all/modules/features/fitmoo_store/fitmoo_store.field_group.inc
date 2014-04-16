<?php
/**
 * @file
 * fitmoo_store.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function fitmoo_store_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_details|commerce_product|clothing|node_teaser';
  $field_group->group_name = 'group_product_details';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'clothing';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_size',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-details field-group-fieldset',
      ),
    ),
  );
  $export['group_product_details|commerce_product|clothing|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_details|commerce_product|product|form';
  $field_group->group_name = 'group_product_details';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'product';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Details',
    'weight' => '4',
    'children' => array(
      0 => 'commerce_price',
      1 => 'commerce_stock',
      2 => 'field_product_size',
      3 => 'title',
      4 => 'status',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_product_details|commerce_product|product|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_details|commerce_product|product|node_teaser';
  $field_group->group_name = 'group_product_details';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'product';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_product_size',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_product_details|commerce_product|product|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_details|commerce_product|shoes|node_teaser';
  $field_group->group_name = 'group_product_details';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'shoes';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_shoe_size',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-details field-group-fieldset',
      ),
    ),
  );
  $export['group_product_details|commerce_product|shoes|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_details|node|product|default';
  $field_group->group_name = 'group_product_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'product';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Details',
    'weight' => '1',
    'children' => array(
      0 => 'body',
      1 => 'field_product',
      2 => 'field_weight',
      3 => 'field_brand',
      4 => 'field_gender',
      5 => 'product:commerce_price',
      6 => 'product:field_product_size',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_product_details|node|product|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_details|node|product|form';
  $field_group->group_name = 'group_product_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'product';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Details',
    'weight' => '3',
    'children' => array(
      0 => 'body',
      1 => 'field_weight',
      2 => 'field_brand',
      3 => 'field_gender',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_product_details|node|product|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|commerce_product|clothing|node_teaser';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'clothing';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '0',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-media field-group-fieldset',
      ),
    ),
  );
  $export['group_product_media|commerce_product|clothing|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|commerce_product|membership|node_teaser';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'membership';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '0',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-media field-group-fieldset',
      ),
    ),
  );
  $export['group_product_media|commerce_product|membership|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|commerce_product|product|form';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'product';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '1',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-media field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_product_media|commerce_product|product|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|commerce_product|product|node_teaser';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'product';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '0',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_product_media|commerce_product|product|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|commerce_product|service|node_teaser';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'service';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '0',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-media field-group-fieldset',
      ),
    ),
  );
  $export['group_product_media|commerce_product|service|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|commerce_product|shoes|node_teaser';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'commerce_product';
  $field_group->bundle = 'shoes';
  $field_group->mode = 'node_teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '0',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-media field-group-fieldset',
      ),
    ),
  );
  $export['group_product_media|commerce_product|shoes|node_teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_product_media|node|product|form';
  $field_group->group_name = 'group_product_media';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'product';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Product Media',
    'weight' => '1',
    'children' => array(
      0 => 'field_product',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-product-media field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_product_media|node|product|form'] = $field_group;

  return $export;
}
