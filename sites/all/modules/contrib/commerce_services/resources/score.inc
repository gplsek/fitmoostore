<?php

/**
 *!!!! MUST REFACTOR TO MAKE MORE SECURE FOR SQL INJECTIONS
 */
function commerce_services_score_create($data) {
    global $user;
    $uid =  $user->uid;

    if (is_array($user->roles) && in_array('administrator', $user->roles)) {
        if (!empty($data['uid'])) {$uid = $data['uid'];}
    }

    if (!empty($data['group'])) {$group = " GROUP BY `{$data['group']}` ";  } else {$group = '';}
    if (!empty($data['status'])) {$status = " and status = '{$data['status']}' ";  } else {$status = '';}
    if (!empty($data['type'])) {$type = " and type = '{$data['type']}' ";  } else {$type = '';}
    if (!empty($data['orderID'])) {$orderID = " and orderID = '{$data['orderID']}' ";  } else {$orderID = '';}
    if (!empty($data['productID'])) {$productID = " and productID = `{$data['productID']}` ";  } else {$productID = '';}
    if (!empty($data['from'])) {$from = " and created >= '{$data['from']}' ";  } else {$from = '';}
    if (!empty($data['till'])) {$till = " and created <= '{$data['till']}' ";  } else {$till = '';}

    $result = db_query("SELECT SUM( ammount ) , type, orderID, productID FROM `fitmoo_user_accounting` WHERE uid = $uid $status $type $orderID $productID $from $till $group");
    foreach($result as $key=>$value){
        $temp[] = $value;
    }
    return $temp;
//    SELECT
//FROM  `fitmoo_user_accounting`
//WHERE uid =1
//    AND TYPE =  'seller'
//GROUP BY orderID
//LIMIT 0 , 30

//
//    $result = db_select('fitmoo_user_accounting', 'fua')
//        ->fields('fua', array('orderID', 'productID',))
//        ->addExpression('SUM( ammount )', 'ammount')
//        ->condition('router_path', $router_paths)
//        ->execute()
//        ->fetchAll(PDO::FETCH_ASSOC);

}

/**
 * RETURN TRUE
 */
function commerce_services_score_create_access($data){
   return TRUE;
}
