<?php

/**
 * Returns a single line item.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_score_retrieve($user_id) {
  return commerce_services_retrieve_entity('commerce_score', $score_id, $expand_entities, $flatten_fields);
}

/**
 * RETURN TRUE
 */
function commerce_services_score_retrieve_access($score_id) {
   return TRUE;
}
