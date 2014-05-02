<?php
/**
 * @file
 * fitmoo_admin.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function fitmoo_admin_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'usertransactions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'fitmoo_user_accounting';
  $view->human_name = 'User Transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User Transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'type' => 'type',
    'created' => 'created',
    'ammount' => 'ammount',
    'type_1' => 'type_1',
    'percentage' => 'percentage',
    'updated' => 'updated',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'ammount' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'percentage' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'updated' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: fitmoo_user_accounting: Order */
  $handler->display->display_options['relationships']['orderID']['id'] = 'orderID';
  $handler->display->display_options['relationships']['orderID']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['relationships']['orderID']['field'] = 'orderID';
  /* Relationship: fitmoo_user_accounting: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Field: fitmoo_user_accounting: User Type */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['label'] = 'Plain text field';
  /* Field: fitmoo_user_accounting: earnings ammount */
  $handler->display->display_options['fields']['ammount']['id'] = 'ammount';
  $handler->display->display_options['fields']['ammount']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['fields']['ammount']['field'] = 'ammount';
  $handler->display->display_options['fields']['ammount']['label'] = '$';
  $handler->display->display_options['fields']['ammount']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['ammount']['alter']['text'] = '$[ammount] ';
  /* Field: fitmoo_user_accounting: split percentage */
  $handler->display->display_options['fields']['percentage']['id'] = 'percentage';
  $handler->display->display_options['fields']['percentage']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['fields']['percentage']['field'] = 'percentage';
  $handler->display->display_options['fields']['percentage']['label'] = '%';
  $handler->display->display_options['fields']['percentage']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['percentage']['alter']['text'] = '[percentage]%';
  /* Field: fitmoo_user_accounting: created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: fitmoo_user_accounting: updated date */
  $handler->display->display_options['fields']['updated']['id'] = 'updated';
  $handler->display->display_options['fields']['updated']['table'] = 'fitmoo_user_accounting';
  $handler->display->display_options['fields']['updated']['field'] = 'updated';
  $handler->display->display_options['fields']['updated']['date_format'] = 'short';
  /* Field: Commerce Order: Link */
  $handler->display->display_options['fields']['view_order']['id'] = 'view_order';
  $handler->display->display_options['fields']['view_order']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['view_order']['field'] = 'view_order';
  $handler->display->display_options['fields']['view_order']['relationship'] = 'orderID';
  $handler->display->display_options['fields']['view_order']['label'] = 'view order';
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'orderID';
  /* Sort criterion: Commerce Order: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'commerce_order';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['relationship'] = 'orderID';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/user-transactions';
  $export['usertransactions'] = $view;

  return $export;
}
