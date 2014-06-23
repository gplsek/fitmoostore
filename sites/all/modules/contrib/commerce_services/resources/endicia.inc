<?php

/**
 * @file
 * Contains the resource callbacks for endicia.
 */


/**
 * Returns a collection of endicia.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_endicia_index($fields) {
    global $user;

//    $file = 'endicia_labels/111.txt';
//    $post = file_get_contents('php://input');
//    $res = $post."   REQUEST =". $_SERVER['REQUEST_URI'];
//    file_put_contents($file, $res, FILE_APPEND);

    //$uid =  $user->uid;
    if (empty($_REQUEST['mail_class'])) {
        return services_error(t('Error Mail class is empty'));
    }

    //and !empty($uid)
    if (!empty($_SESSION['endicia'])) {
       module_load_include('module', 'endicia',drupal_get_path('module', 'endicia'));
       module_load_include('module', 'endicia_ui',drupal_get_path('module', 'endicia_ui'));
       $result = _endicia_ui_print_label_confirm_form($_SESSION['endicia']);
       if ($result) {
            $result['values']['mail_class'] = $_REQUEST['mail_class'];
            $result['values']['submit'] = 'Confirm shipment';
            $result['values']['op'] = 'Confirm shipment';
            return _endicia_ui_print_label_confirm_form_submit($result);
       } else {
            return services_error(t('Some error'));
       }

    }

}

/**
 * Determines an API user's access to index endicia.
 */
function commerce_services_endicia_index_access() {
    return TRUE;
}

/**
 * Returns a single endicia.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_endicia_retrieve($pid) {
    global $user;
    $uid =  $user->uid;
    if (!empty($uid)) {
        $result = endicia_ui_print_label_confirm_form($_SESSION['endicia']);
    }
    return  commerce_customer_profile_load($pid) ;
}

/**
 * Determines an API user's access to retrieve a given endicia.
 *
 * @param $endicia_id
 *   The ID of the endicia to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the endicia.
 */
function commerce_services_endicia_retrieve_access() {
    return TRUE;
}

/**
 * Creates a new endicia.
 */
function commerce_services_endicia_create($data) {
    $result = _endicia_print_label_data_validate($data);
    return $_SESSION['endicia'];
}

/**
 * Determines an API user's access to create a new endicia.
 */
function commerce_services_endicia_create_access() {
    return TRUE;
}


/**
 * Submit handler for the generic "cloud" print label form.
 */
function _endicia_print_label_data_validate($data) {
    global $user;
//    $file = 'endicia_labels/111.txt';
//    $post = file_get_contents('php://input');
//    $res = $post."   REQUEST =". $_SERVER['REQUEST_URI'];
//    file_put_contents($file, $res, FILE_APPEND);
    //if ($data['order_id']) {}
    $order = _commerce_services_order_retrieve($data['order_id'], 1, false);
    //<!-- if ($order->uid != $user->uid) {
        //return services_error(t("User have no access to this order"));
    //} -->
    // WE MUST REFACTOR HERE AFTER ORDER CAN INCLUDE MORE 1 PRODUCT
    foreach ($order->commerce_line_items['und'] as $key=>$value) {
       $line_item =  commerce_services_retrieve_entity('commerce_line_item', $value['line_item_id'], 1, false);
       if ($line_item->type == 'shipping') {continue;}
       $temp = field_get_items('commerce_line_item', $line_item, 'commerce_product');
        $types = array("event","service","membership");
        $product = commerce_product_load($temp[0]['product_id']);

        if (in_array($product->type, $types)) {
            $order->status = "Pending:AcceptEvent";
            commerce_order_save($order);
            return services_error(t("Non shippable Item, status was set to Pending:AcceptEvent"));
	   }
    }


    if (empty($data['package_info']['weight'])) {
        $data['package_info']['weight'] = variable_get($line_item->commerce_product_entities[$line_item->commerce_product['und'][0]['product_id']]->type."_w", "0");
    }
    if (empty($data['package_info']["mail_shape"])) {
            $data['package_info']["mail_shape"] = 'SmallFlatRateBox';
    }
    if (empty($data['package_info']['description'])) {
        $data['package_info']['description'] = $line_item->commerce_product_entities[$line_item->commerce_product['und'][0]['product_id']]->type;
    }
    if (empty($data['package_info']['declared_value'])) {
        $data['package_info']['declared_value'] = $line_item->commerce_total['und'][0]['amount']/100;
    }
    if (empty($data['package_info']['length'])) {
        $data['package_info']['length'] = variable_get("endicia_length", "0");
    }
    if (empty($data['package_info']['height'])) {
        $data['package_info']['height'] = variable_get("endicia_height", "0");
    }
    if (empty($data['package_info']['width'])) {
        $data['package_info']['width'] = variable_get("endicia_width", "0");
    }

    if (empty($data["customs_info"])) {
        $data["customs_info"] = array(
            "CustomsItem1" => array(
                "Description"=>$line_item->commerce_product_entities[$line_item->commerce_product['und'][0]['product_id']]->type,
                "Quantity"=>$line_item->quantity,
                "Weight"=>$data['package_info']['weight'],
                "Value"=>$line_item->commerce_total['und'][0]['ammount']/100,
            )
        );
    }

    if (empty($data["delivery_address"])) {
        foreach ($order->commerce_customer_shipping_entities as $key=>$value) {
            if (!empty($value)) {
                $data["delivery_address"] = $value->commerce_customer_address['und'][0];
            }
            break;
        }
    }

    if (!empty($data["delivery_address"]['full_name'])) {
        $name_array = _split_name($data["delivery_address"]['full_name']);
        unset($data["delivery_address"]['full_name']);
        if (!empty($name_array['first_name'])) {
            $data["delivery_address"]['first_name'] = $name_array['first_name'];
        }
        if (!empty($name_array['last_name'])) {
            $data["delivery_address"]['last_name'] = $name_array['last_name'];
        }
    }

    if ($data['package_info']['weight'] == 0) {
        return services_error(t("The package's entered weight must not be 0"));
    }

//    foreach (array('origin' => 'pickup', 'destination' => 'delivery') as $parent_key => $type) {
//        $digitsonly = preg_replace("/[^0-9]/", "", $data[$type . '_phone']);
//        if (strlen($digitsonly) > 10) {
//            return services_error(t('Phone numbers must not contain more than 10 digits.'));
//        }
//    }
    // Prepare for API call and create session data
    $_SESSION['endicia'] = array();
    $_SESSION['endicia']['package'] = array();
    $_SESSION['endicia']['package'] += $data['package_info'];
    $_SESSION['endicia']['package'] += $data['ship_options'];
    $_SESSION['endicia']['package'] += $data['customs_info'];
    $_SESSION['endicia']['order_id'] = $data['order_id'];

    foreach (array('pickup', 'delivery') as $type) {
        foreach ($data[$type . '_address'] as $key => $value) {
            $data[$type . '_' . $key] = $value;
        }
        unset($data[$type . '_address']);
    }
    $_SESSION['endicia']['pickup_address'] = _endicia_get_address($data, 'pickup');
    $_SESSION['endicia']['delivery_address'] = _endicia_get_address($data, 'delivery');

    // Make API call, storing any results in session
    $destination = $_SESSION['endicia']['delivery_address'];
    $origin = $_SESSION['endicia']['pickup_address'];
    $package_info = $_SESSION['endicia']['package'];
    $is_international = ($destination->country != 'US');

    // Set request data for API call to calculate postage rates
    $countries = _endicia_country_get_predefined_list();
    $data = array(
        'AccountID' => variable_get('endicia_account_id', ''),
        'Password' => variable_get('endicia_passphrase', ''),
        'MailClass' => ($is_international ? "International" : "Domestic"),
        'MailpieceShape' => $package_info['mail_shape'],
        'Weight' => $package_info['weight'],
        'Sender' => array(
            'PostalCode' => $origin->postal_code,
        ),
        'Recipient' => array(
            'PostalCode' => $destination->postal_code,
            'CountryCode' => $destination->country,
            'Country' => $countries[$destination->country],
        ),
        'CODAmount' => 0,
        'InsuredValue' => $package_info['insurance'] == 'None' ? 0 : $package_info['declared_value'], // Same here.
        'RegisteredMailValue' => 0, // You guessed it, here also.
        'DateAdvance' => $package_info['date_advance'],
        'Services' => array(),
    );
    foreach($package_info['options'] as $option => $value) {
        if ($value) {
            switch ($option) {
                case 'signature_confirmation':
                    $data['Services']['SignatureConfirmation'] = 'ON';
                    break;
                case 'delivery_confirmation':
                    $data['Services']['DeliveryConfirmation'] = 'ON';
                    break;
                case 'hold_for_pickup':
                    $data['Services']['HoldForPickup'] = 'ON';
                    break;
            }
        }
    }
    // Send only the ZIP5 for US postal codes
    if (!$is_international && preg_match('/\d+-\d+/', $destination->postal_code)) {
        list($zip5, $zip4) = explode('-', $destination->postal_code, 2);
        $data['Recipient']['PostalCode'] = $zip5;
    }
    // Make the API request
    $uri = _endicia_in_test_mode() ? ENDICIA_TEST_URI : ENDICIA_PRODUCTION_URI;
    $response_node = 'PostageRatesResponse';
    $response = endicia_api_request($uri, 'CalculatePostageRates', $data, $response_node);
    // Check for failures
    if ($response === FALSE) {
        form_error($form);
        _endicia_communication_error_redirect();
        return;
    }
    // Check for errors
    if ($error = endicia_api_request_error_check($response, $response_node)) {
        $message = t('Could not retrieve postage rate calculations. Please ensure that a valid destination address was entered (check the site log for additional details).');
        form_error($form, $message);
        return;
    }
    // Keep tallies of rates by mail class
    $mail_class_rates = array();
    if (is_array($response->PostageRatesResponse->PostagePrice)) {
        foreach ($response->PostageRatesResponse->PostagePrice as $rate) {
            // Avoid a PHP undefined index warnings; initialize or add to the existing index if necessary
            $mail_class_rates[$rate->MailClass] = isset($mail_class_rates[$rate->MailClass]) ? $mail_class_rates[$rate->MailClass] + $rate->TotalAmount : $rate->TotalAmount;
        }
    }
    else {
        $rate = &$response->PostageRatesResponse->PostagePrice;
        $mail_class_rates[$rate->MailClass] = isset($mail_class_rates[$rate->MailClass]) ? $mail_class_rates[$rate->MailClass] + $rate->TotalAmount : $rate->TotalAmount;
    }
    $_SESSION['endicia']['mail_class_rates'] = &$mail_class_rates;

    // Get their account balance
    $account_info = endicia_get_account_status();
    if ($account_info === FALSE || !$account_info) {
        $_SESSION['endicia']['account_balance'] = t('Unknown');
    }
    else {
        $_SESSION['endicia']['account_balance'] = sprintf('$%.2f', $account_info['PostageBalance']);
    }

    if (_endicia_in_test_mode()) {
        drupal_set_message(t('Reminder: Endicia is operating in test mode. Your account balance will not be deducted.'), 'warning');
    }
}

function _split_name($name) {
    $name_array = explode(' ',$name);
    $result = array();
    if (count($name_array) == 1) {
        $result['first_name'] = '';
        $result['last_name'] = '';
    } else {
        $result['first_name'] = $name_array[0];
        unset($name_array[0]);
        $result['last_name'] = implode(' ', $name_array);
    }
    return $result;
}

function _commerce_services_order_retrieve($order_id, $expand_entities, $flatten_fields) {
    $item = commerce_services_retrieve_entity('commerce_order', $order_id, $expand_entities, $flatten_fields);
    foreach ($item->commerce_line_items as $itemkey=>$itemvalue) {
        $item->commerce_line_items_data[$itemvalue] = commerce_services_retrieve_entity('commerce_line_item', $itemvalue, 1, true);
    }
    return $item;
}

function _endicia_ui_print_label_confirm_form($form_state) {
    // Check to make sure request is valid
    if (!isset($_SESSION['endicia'])) {
        drupal_goto('admin/endicia/label/print');
    }

    // Setup some stuff for use later
    $destination = $_SESSION['endicia']['delivery_address'];
    $origin = $_SESSION['endicia']['pickup_address'];
    $package_info = $_SESSION['endicia']['package'];
    $mail_class_rates = $_SESSION['endicia']['mail_class_rates'];
    $balance = $_SESSION['endicia']['account_balance'];
    $is_international = ($destination->country != 'US');

    // Process pricing information returned from API request and add it to the form.
    $mail_classes = _endicia_mail_classes($is_international ? 'International' : 'Domestic');
    $mail_class_estimates = array();
    foreach ($mail_class_rates as $class => $total) {
        $class_text = isset($mail_classes[$class]) ? $mail_classes[$class] : t('[Title missing] @name', array('@name' => $class));
        $mail_class_estimates[$class] = sprintf('%s - $%.2f', $class_text, $total);
    }

    // Now that we have all the information we need, setup the form
    $form['instructions'] = array(
        '#value' => t('Select a shipping class and click on Confirm Shipment to complete the shipment. Your account balance will be deducted the amount indicated next to the shipping class.'),
    );

    $form['account_balance'] = array(
        '#title' => t('Account Balance'),
        '#type' => 'item',
        '#markup' => $balance,
    );

    $form['mail_class'] = array(
        '#type' => 'select',
        '#title' => t('Shipping Class'),
        '#description' => t('Select the mail class to use for this shipment.'),
        '#options' => $mail_class_estimates,
        '#default_value' => _endicia_get_service_type($is_international), // FIXME default value not preserved upon error
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Confirm shipment'),
        '#suffix' => l(t('Go Back'), 'admin/endicia/label/print'),
    );

    return $form;
}


function _endicia_ui_print_label_confirm_form_submit($form_state) {
    global $user;
    $origin = $_SESSION['endicia']['pickup_address'];
    $destination = $_SESSION['endicia']['delivery_address'];
    $package_info = $_SESSION['endicia']['package'];
    $is_international = ($destination->country != 'US');

    $countries = _endicia_country_get_predefined_list();
    $data = array(
        'AccountID' => variable_get('endicia_account_id', ''),
        'Password' => variable_get('endicia_passphrase', ''),
        'Class' => $form_state['values']['mail_class'],
        'Packaging' => $package_info['mail_shape'],
        'Weight' => $package_info['weight'], // ounces
        'Description' => $package_info['description'],
        'DeclaredValue' => $package_info['declared_value'], // not needed?
        'CustomerID' => $user->uid,
        'TransactionID' => sprintf('c-%s', time()),
        'Recipient' => array(
            'Name' => $destination->first_name . ' ' . $destination->last_name,
            'Company' => $destination->company,
            'Address1' => $destination->street1,
            'Address2' => $destination->street2,
            'City' => $destination->city,
            'State' => $destination->zone,
            'CountryCode' => $destination->country,
            'Country' => $countries[$destination->country],
            // 'PostalCode' added later
            'Phone' => preg_replace("/[^0-9]/", "", $destination->phone),
            'Email' => $destination->email,
        ),
        'Sender' => array(
            'Name' => $origin->first_name . ' ' . $origin->last_name,
            'Address1' => $origin->street1,
            'Address2' => $origin->street2,
            'City' => $origin->city,
            'State' => $origin->zone,
            'PostalCode' => $origin->postal_code,
            'Phone' => preg_replace("/[^0-9]/", "", $origin->phone),
            'Email' => $origin->email,
        ),
        'Test' => _endicia_in_test_mode() ? 'YES' : 'NO',
        'CostCenter' => 0,
        'LabelFormat' => variable_get('endicia_label_image_format', 'PNG'),
        'Services' => array(),
    );
    // Figure out if we are using a ZIP5 or international postal code.
    if ($is_international || !preg_match('/\d+-\d+/', $destination->postal_code)) {
        $data['Recipient']['PostalCode'] = $destination->postal_code;
    }
    else {
        list($zip5, $zip4) = explode('-', $destination->postal_code, 2);
        $data['Recipient']['PostalCode'] = $zip5;
        $data['Recipient']['PostalCodeZip4'] = $zip4;
    }
    // Determine shipping options
    foreach($package_info['options'] as $option => $value) {
        if ($value) {
            switch ($option) {
                case 'signature_confirmation':
                    $data['Services']['SignatureConfirmation'] = 'ON';
                    break;
                case 'delivery_confirmation':
                    $data['Services']['DeliveryConfirmation'] = 'ON';
                    break;
                case 'hold_for_pickup':
                    $data['Services']['HoldForPickup'] = 'ON';
                    break;
                case 'usps_form_2976':
                    $data['CustomsInfo'] = array();
                    break;
            }
        }
    }

    // Outdated but requests are refused without them.
    $data += array(
        'CustomsQuantity1' => 0, 'CustomsValue1' => 0, 'CustomsWeight1' => 0,
        'CustomsQuantity2' => 0, 'CustomsValue2' => 0, 'CustomsWeight2' => 0,
        'CustomsQuantity3' => 0, 'CustomsValue3' => 0, 'CustomsWeight3' => 0,
        'CustomsQuantity4' => 0, 'CustomsValue4' => 0, 'CustomsWeight4' => 0,
        'CustomsQuantity5' => 0, 'CustomsValue5' => 0, 'CustomsWeight5' => 0,
    );
    if (isset($data['CustomsInfo'])) {
        // Customs integrated forms, if applicable
        $customs_items = array();
        foreach (range(1, 5) as $i) {
            if (!empty($package_info['CustomsItem' . $i]['Description'])) {
                $customs_items[] = $package_info['CustomsItem' . $i];
            }
        }
        $label_type = $is_international ? 'International' : 'Domestic';
        $form_type = 'Form2976';
        // This is ugly, but there isn't much we can do.
        $general_case = ( $label_type == 'International' && in_array($data['Class'], array('PriorityMailInternational', 'ExpressMailInternational')) );
        $exception_int_flenv_prio = $label_type == 'International' && $data['Packaging'] == 'SmallFlatRateBox' && $data['Class'] == 'PriorityMailInternational';
        if ($general_case && !$exception_int_flenv_prio) {
            $form_type .= 'A';
        }
        if (count($customs_items)) {
            if ($form_type == 'Form2976' && count($customs_items) > 5) {
                return services_error(t('Only 5 item descriptors are supported on the USPS Form 2976 due to 4x6 label size constraints.'));
            }
            $data += array(
                'LabelType' => $label_type,
                'LabelSubtype' => 'Integrated',
                'IntegratedFormType' => $form_type,
            );
            $data['CustomsInfo'] += array(
                'ContentsType' => 'Merchandise',
                'CustomsItems' => $customs_items,
            );
        }
    }

    // Determine label size: Endicia uses a WxH notation, we want a 6x4 in all cases
    if ($is_international) {
        $data['LabelSize'] = '4x6c'; // this returns a 6x4 label
    }
    else {
        $data['LabelSize'] = '4x6';
        if (!in_array($data['LabelFormat'], array('EPL2', 'ZPLII'))) {
            $data['LabelRotation'] = 'Rotate90';
        }
    }

    $uri = _endicia_in_test_mode() ? ENDICIA_TEST_URI : ENDICIA_PRODUCTION_URI;
    $response_node = 'LabelRequestResponse';
    $response = endicia_api_request($uri, 'GetPostageLabel', $data, $response_node);
    // Check for failures
    if ($response === FALSE) {
        // watchdog already notified.
        return services_error(t('Error response from endicia'));
    }
    // Check for errors
    if ($error = endicia_api_request_error_check($response, $response_node)) {
        $message = 'Could not generate label due to error #@errornum: %errormsg (check the site log for additional details).';
        $variables = array(
            '@errornum' => $error['status'],
            '%errormsg' => $error['message'],
        );
        return services_error(t($error['status'].' '. $error['message']));
    }

    // If this node is here, only one label was returned
    $image_format = variable_get('endicia_label_image_format', 'PNG');
    $extension = '.' . _endicia_get_file_extension($image_format);

    $result = FALSE;
    $tracking_num = $response->LabelRequestResponse->TrackingNumber;
    if (isset($response->LabelRequestResponse->Base64LabelImage)) {
        // Single image returned as base64
        $filename = 'label-cloud-' . $tracking_num . $extension;
        $data = base64_decode($response->LabelRequestResponse->Base64LabelImage);
        $result = endicia_save_label($filename, $data, 'endicia_ui', $tracking_num);
    }
    else {
        if (is_array($response->LabelRequestResponse->Label->Image)) {
            // Two+ labels returned as base64 under their own nodes
            foreach($response->LabelRequestResponse->Label->Image as $labelpart) {
                $filename = 'label-cloud-' . $tracking_num . '-' . $labelpart->PartNumber . $extension;
                $data = base64_decode($labelpart->_);
                $result = endicia_save_label($filename, $data, 'endicia_ui', $tracking_num);
                if (!$result) {
                    break;
                }
            }
        }
        else {
            // One label returned  as base64 under its own node
            $labelpart = &$response->LabelRequestResponse->Label->Image;
            $filename = 'label-cloud-' . $tracking_num . '-' . $labelpart->PartNumber . $extension;
            $data = base64_decode($labelpart->_);
            $result = endicia_save_label($filename, $data, 'endicia_ui', $tracking_num);
        }
    }
    if (!$result) {
        services_error(t("Could not save the label image due to insufficient permissions. Please verify that Drupal's private files directory exists and is writable."), 'error');
    }
    /*
    $ship_date['year'] = substr($response->LabelRequestResponse->TransactionDateTime, 0, 4);
    $ship_date['month'] = substr($response->LabelRequestResponse->TransactionDateTime, 4, 2);
    $ship_date['day'] = substr($response->LabelRequestResponse->TransactionDateTime, 6, 2);
    $ship_date['hour'] = substr($response->LabelRequestResponse->TransactionDateTime, 8, 2);
    $ship_date['min'] = substr($response->LabelRequestResponse->TransactionDateTime, 10, 2);
    $ship_date['sec'] = substr($response->LabelRequestResponse->TransactionDateTime, 12, 2);
    $timestamp = gmmktime($ship_date['hour'], $ship_date['min'], $ship_date['sec'], $ship_date['month'], $ship_date['day'], $ship_date['year']);
    */

    $order = _commerce_services_order_retrieve($_SESSION['endicia']['order_id'], 1, false);

    unset($_SESSION['endicia']);
    // FIXME: What about multiple labels returned?
    $form_state['label_url'] = 'endicia/print/endicia_labels/' . $filename;
    $order->field_tracking['und'][0]['value'] = $tracking_num;
    $order->status = 'Shipping: shipped';
    $order->field_shipping_label_url['und'][0]['value'] = 'http://'.$_SERVER['HTTP_HOST'].'/'.$form_state['label_url'];
    commerce_order_save($order);
    fitmoo_checkout_endiciaSendLabelTOSellerAndBuyer($order);
    return array('track_number' => $tracking_num, 'url' => $order->field_shipping_label_url['und'][0]['value']);
}




/**
 * Updates a endicia.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_endicia_update($number, $dat, $flatten_fields) {
    $data = array();
    $account_id = variable_get('endicia_account_id', '');
    $passphrase = variable_get('endicia_passphrase', '');



    $URL = "https://www.endicia.com/ELS/ELSServices.cfc?wsdl";
    $xmlRequest = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
  <StatusRequest>
    <AccountID>$account_id</AccountID>
    <Test>Y</Test>
    <Password>$passphrase</Password>
    <FullStatus>Yes</FullStatus>
    <StatusList>
       <PICNumber>$number</PICNumber>
    </StatusList>
  </StatusRequest>
XML;

    $requestData = '&method=StatusRequest&XMLInput=' . $xmlRequest;

    $curl_handle = curl_init ();
    curl_setopt ($curl_handle, CURLOPT_URL, $URL);
    curl_setopt ($curl_handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt ($curl_handle, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt ($curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt ($curl_handle, CURLOPT_POST, 1);
    curl_setopt ($curl_handle, CURLOPT_POSTFIELDS, $requestData);

    $curl_result = curl_exec ($curl_handle) or die ("There has been a CURL_EXEC error");




    curl_close ($curl_handle);
    if ($curl_result === FALSE) {
        return false;
    }



    $result = xml2array($curl_result);
    return $result['STATUSBREAKDOWN'];


    //$response_node = 'StatusResponse';
//    //$response = endicia_api_request(ENDICIA_ELS_SERVICES_URI, 'StatusRequest', $data, $response_node);
//    // Check for errors
//    if ($error = endicia_api_request_error_check($response, $response_node)) {
//        $message = 'Could not get shipping status status';
//        $variables = array(
//            '@identifier' => $data['number'],
//            '%errormsg' => $error['message'],
//        );
//        drupal_set_message(t($message, $variables), 'error');
//    }

}

function xml2array($xml){
    $opened = array();
    $xml_parser = xml_parser_create();
    xml_parse_into_struct($xml_parser, $xml, $xmlarray);
    $array = array();
    $arrsize = sizeof($xmlarray);
    for($j=0;$j<$arrsize;$j++){
        $val = $xmlarray[$j];
        switch($val["type"]){
            case "open":
                $opened[$val["tag"]] = $array;
                unset($array);
                break;
            case "complete":
                $array[$val["tag"]][] = $val["value"];
                break;
            case "close":
                $opened[$val["tag"]] = $array;
                $array = $opened;
                break;
        }
    }
    return $array;
}

/**
 * Determines an API user's access to update a given endicia.
 *
 * @param $endicia_id
 *   The ID of the endicia to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the endicia.
 */
function commerce_services_endicia_update_access() {
  return TRUE;
}

/**
 * Deletes a endicia.
 *
 * @param $endicia_id
 *   The ID of the endicia to be deleted.
 */
function commerce_services_endicia_delete($pid) {
//    global $user;
//    if (is_array($user->roles) && in_array('administrator', $user->roles)) {
      return _endicia_ui_refund_postage_form_submit($pid);
//    } else {
//        return services_error(t('Access to this operation not granted'), 401);
//    }
}


/**
 * Submit handler for postage refund form.
 *
 * @see endicia_ui_refund_postage_form().
 */
function _endicia_ui_refund_postage_form_submit($pid) {
    $data = array();
    $account_id = variable_get('endicia_account_id', '');
    $passphrase = variable_get('endicia_passphrase', '');
    if (!$account_id) {
        drupal_set_message(t('Your account ID has not been set! Please configure your Endicia account information first.'), 'error');
        drupal_goto('admin/endicia/settings/credentials');
    }

    $valid = array();

        $number = $pid;
        $data = array(
            'AccountID' => $account_id,
            'Password' => $passphrase,
            'Test' => _endicia_in_test_mode() ? 'Y' : 'N',
            'StatusList' => array(
                'PICNumber' => $number,
            ),
        );

        $response_node = 'RefundResponse';
        $response = endicia_api_request(ENDICIA_ELS_SERVICES_URI, 'RefundRequest', $data, $response_node);
        // Check for failures
        if ($response === FALSE) {
            _endicia_communication_error_redirect('admin/endicia/account/refund');
        }
        // Check for errors
        if ($error = endicia_api_request_error_check($response, $response_node)) {
            $message = 'Could not process refund for package #@identifier: %errormsg (check the site log for additional details).';
            $variables = array(
                '@identifier' => $number,
                '%errormsg' => $error['message'],
            );
            drupal_set_message(t($message, $variables), 'error');
        }


    if ($response) {
        return 'Your refund request(s) were processed successfully.';
    }
}

/**
 * Determines an API user's access to delete a given endicia.
 *
 * @param $endicia_id
 *   The ID of the endicia to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the endicia.
 */
function commerce_services_endicia_delete_access($endicia_id) {
   return TRUE;
}
