<?php

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function fitmoo_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];
  $last = null;

  if(!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to screen-reader users. Make the heading invisible with .element-invisible.
    if(count($breadcrumb) > 1) $last = array_pop($breadcrumb);
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<div class="breadcrumb">' . implode('<span class="arrow">></span>', $breadcrumb);
    if($last) $output .= '<span class="arrow">></span><span class="current">'.$last.'</span>';
    $output .= '</div>';
    return $output;
  }
}

/**
 * Override or insert variables into the maintenance page template.
 */
function fitmoo_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // fitmoo_preprocess_html() also happen on the maintenance page, it has to be
  // called here.
  fitmoo_preprocess_html($vars);
}



/**
 * Override or insert variables into the html template.
 */
function fitmoo_preprocess_html(&$vars) {
 $node = menu_get_object();

  if ($node && $node->nid) {
    $vars['theme_hook_suggestions'][] = 'html__' . $node->type;
  }
    // Classes for body element. Allows advanced theming based on context
    // (home page, node of certain type, etc.)
    if (!$vars['is_front']) {
      // Add unique class for each page.
      $path = drupal_get_path_alias($_GET['q']);
      // Add unique class for each website page
      $section = (strpos($path, '/')) ? substr($path, 0, strpos($path, '/')) : $path;
      $page = preg_split('/\//', $path);
      $page = $page[count($page) - 1];
      if (arg(0) == 'node') {
          if (arg(1) == 'add') {
              $page = 'node-add';
          } elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
              $page = 'node-' . arg(2);
          }
      }
      $vars['classes_array'][] = drupal_html_class('path-' . $page);
      $vars['classes_array'][] = drupal_html_class('section-' . $section);
      /* add  js
      if($section == 'sectionhook') {
        drupal_add_js(base_path().path_to_theme().'/includes/file.js', array('type'=>'file', 'every_page'=>FALSE));
      }*/
    }
}

/**
 * Override or insert variables into the page template.
 */
function fitmoo_preprocess_page(&$vars) {
	 global $base_url;
	 //print $base_url;die();
	if(arg(1) == 'register'){
		
	    // Make sure there isn't a redirect loop.
	    $redirect_base = variable_get('anonymous_redirect_base', '');
	    if ($redirect_base == $base_url) {
	      return;
	    }

	    // Redirect.
	    drupal_goto($redirect_base);
		
	}
	
    if(arg(0) == 'cart'){
	  if(isset($_GET['checkout'])){
 	     $_SESSION['callback'] = $_GET['checkout'];
        }else{
         $_SESSION['callback'] = $_SERVER['HTTP_REFERER'];
        }
    }
	
    if(arg(0) == 'checkout'){
        //drupal_add_js('misc/checkout.js');
		
 	   if(isset($_GET['checkout'])){
 	     $_SESSION['callback'] = $_GET['checkout'];
        }else{
         $_SESSION['callback'] = $_SERVER['HTTP_REFERER'];
        }
    }
	
	if($vars['user']->uid != 1){
	 $url = array('checkout','cart','user','endicia');
	
	 if (!(in_array(arg(0), $url))){
 	    // Make sure there isn't a redirect loop.
 	    $redirect_base = variable_get('anonymous_redirect_base', '');
 	    if ($redirect_base == $base_url) {
 	      return;
 	    }

 	    // Redirect. 
 	    drupal_goto($redirect_base);
	 }
	}
	
	
    // global $base_url;
// 
//     // Only continue if the current request should be redirected.
//     if (!variable_get('anonymous_redirect_enable', FALSE)
//      || trim(variable_get('anonymous_redirect_base', '')) == ''
//      || user_is_logged_in()
//      || arg(0) == 'user'
//      || drupal_is_cli()) {
//       return;
//     }
// 

	
	

   
  
   
  // Prepare header.
  $site_fields = array();
  if (!empty($vars['site_name'])) {
    $site_fields[] = $vars['site_name'];
  }
  if (!empty($vars['site_slogan'])) {
    $site_fields[] = $vars['site_slogan'];
  }
  $vars['site_title'] = implode(' ', $site_fields);
  if (!empty($site_fields)) {
    $site_fields[0] = '<span>' . $site_fields[0] . '</span>';
  }
  $vars['site_html'] = implode(' ', $site_fields);

  // Set a variable for the site name title and logo alt attributes text.
  $slogan_text = $vars['site_slogan'];
  $site_name_text = $vars['site_name'];
  $vars['site_name_and_slogan'] = $site_name_text . ' ' . $slogan_text;

  // Set ajax and page-node tpls
  if(isset($_GET['ajax']) && $_GET['ajax'] == 1) {
    $vars['theme_hook_suggestions'] = 'page__ajax';
  }
  if(!empty($vars['node'])) {
    if($vars['node']->type != '') {
      $vars['theme_hook_suggestions'][] = 'page__'.str_replace('_', '--', $vars['node']->type);
    }
  }
}

/**
 * Override or insert variables into the node template.
 */
function fitmoo_preprocess_node(&$vars) {
  $vars['submitted'] = $vars['date'] . ' � ' . $vars['name'];
}

/**
 * Override or insert variables into the comment template.
 */
function fitmoo_preprocess_comment(&$vars) {
    $vars['submitted'] = $vars['author'].'<span class="pipe">|</span><span class="date">'.$vars['created'].'</span>';
    if($vars['id']%2 == 0) $vars['comment_stripe'] = 'comment-even';
    else $vars['comment_stripe'] = 'comment-odd';
}

/*
* Override filter.module's theme_filter_tips() function to disable tips display.
*/
function fitmoo_form_comment_form_alter(&$form, &$form_state, $form_id) {
  //$form['comment_body']['#after_build'][] = 'remove_tips';
}


/**
* Add unique class (mlid) to all menu items.
* with Menu title as class
*/
function fitmoo_menu_link(&$vars) {
  $element = $vars['element'];
  $sub_menu = '';
  $name_id = strtolower(strip_tags($element['#title']));
// remove colons and anything past colons
  if (strpos($name_id, ':')) $name_id = substr ($name_id, 0, strpos($name_id, ':'));
//Preserve alphanumerics, everything else goes away
  $pattern = '/[^a-z]+/ ';
  $name_id = preg_replace($pattern, '', $name_id);
  $element['#attributes']['class'][] = 'menu-' . $name_id;
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
