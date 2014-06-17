<?php

/**
 * Views for the shopping cart.
 */

/**
 * Implements hook_views_default_views().
 */

/**
 * Implements hook_views_default_views_alter().
 *
 * Add the cart state as a filter on the default commerce_orders (admin)
 * view.
 */
function commerce_cart_views_default_views_alter(&$views) {
  if (isset($views['commerce_orders'])) {
    $views['commerce_orders']->display['default']->display_options['filters']['state']['value']['cart'] = 'cart';
    $views['commerce_orders']->display['default']->display_options['filters']['state']['value']['checkout'] = 'checkout';

    /* Display: Shopping carts */
    $handler = $views['commerce_orders']->new_display('page', 'Shopping carts', 'shopping_carts');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Shopping carts';
    $handler->display->display_options['defaults']['empty'] = FALSE;
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'text';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'There are currently no shopping cart orders.';
    $handler->display->display_options['empty']['text']['format'] = 'plain_text';
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Order: Order state */
    $handler->display->display_options['filters']['state']['id'] = 'state';
    $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['state']['field'] = 'state';
    $handler->display->display_options['filters']['state']['value'] = array(
      'cart' => 'cart',
      'checkout' => 'checkout',
    );
    $handler->display->display_options['filters']['state']['expose']['label'] = 'Order state';
    $handler->display->display_options['filters']['state']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
    $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
    $handler->display->display_options['path'] = 'admin/commerce/orders/carts';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Shopping carts';
    $handler->display->display_options['menu']['weight'] = '0';
  }
}
