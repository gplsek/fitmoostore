<?php
/**
 * @file
 * fitmoo_store.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function fitmoo_store_user_default_roles() {
  $roles = array();

  // Exported role: Store Owner.
  $roles['Store Owner'] = array(
    'name' => 'Store Owner',
    'weight' => 3,
  );

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
  );

  return $roles;
}
