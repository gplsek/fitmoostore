<?php
/**
 * @file
 * fitmoo_store.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function fitmoo_store_taxonomy_default_vocabularies() {
  return array(
    'top_size' => array(
      'name' => 'top_size',
      'machine_name' => 'top_size',
      'description' => 'top size',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
