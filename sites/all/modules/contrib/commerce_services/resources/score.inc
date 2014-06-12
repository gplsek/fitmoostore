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

    $result = db_query("SELECT SUM( ammount ) as ammount, type, orderID, productID FROM `fitmoo_user_accounting` WHERE uid = $uid $status $type $orderID $productID $from $till $group");
    foreach($result as $key=>$value){
        $temp[] = $value;
    }
    return $temp;

}

/**
 * RETURN TRUE
 */
function commerce_services_score_create_access($data){
   return TRUE;
}


function commerce_services_score_retrieve($uid) {
    global $user;
    $uid =  $user->uid;
    $user_object = user_load($uid);
    if (is_array($user->roles) && in_array('administrator', $user->roles)) {
        if (!empty($data['uid'])) {$uid = $data['uid'];}
    }

    //$pending_sell_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='pending' and type='seller'")->fetchField();
    //$pending_buy_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='pending' and type='buyer'")->fetchField();
    //$pending = $pending_sell_income + $pending_buy_income;

    //$complete_sell_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='cleared' and type='seller'")->fetchField();
    //$complete_buy_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='cleared' and type='buyer'")->fetchField();
    //$complete = $complete_sell_income+$complete_buy_income;
	
    //$result['ballance'] = $complete + $pending;
    //$result['pending'] = $pending;
    //$result['redeemable'] = $complete;
	
    $result['ballance'] = $user_object->field_balance_total['und'][0]['value'];
    $result['pending'] = $user_object->field_balance_pending['und'][0]['value'];
    $result['redeemable'] = $user_object->field_balance_available['und'][0]['value'];
	
	$results = db_select('fitmoo_user_redemptions', 'm')
	    ->fields('m')
		->condition('uid', $uid,'=')
	    ->execute();
	
	$redemption = array();	
	while($trans = $results->fetchAssoc()){
			
		$redemption['id'] = $trans['id'];
		$redemption['amount'] = $trans['ammount'];
		$redemption['type'] = $trans['transfer_type'];
		$redemption['status'] = $trans['status'];
		$redemption['created'] = $trans['created'];
		$redemption['updated'] = $trans['updated'];
		$result['redemptions'][] = $redemption;
	}
	
	unset($user_object);
	
    return $result;
}

function commerce_services_score_retrieve_access() {
    global $user;
    if (!empty($user->uid)) {return TRUE;} else {return FALSE;}
}

function commerce_services_score_index($uid) {
    global $user;
    $uid =  $user->uid;
    $user_object = user_load($uid);
    if (is_array($user->roles) && in_array('administrator', $user->roles)) {
        if (!empty($data['uid'])) {$uid = $data['uid'];}
    }

    //$pending_sell_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='pending' and type='seller'")->fetchField();
    //$pending_buy_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='pending' and type='buyer'")->fetchField();
    //$pending = round($pending_sell_income + $pending_buy_income,2);

    //$complete_sell_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='cleared' and type='seller'")->fetchField();
    //$complete_buy_income = db_query("SELECT SUM( ammount ) as ammount FROM `fitmoo_user_accounting` WHERE uid = $uid and status ='cleared' and type='buyer'")->fetchField();
    ///$complete = round($complete_sell_income+$complete_buy_income,2);
    //$result['ballance'] = $complete + $pending;
    //$result['pending'] = $pending;
    //$result['redeemable'] = $complete;
	
    $result['ballance'] = $user_object->field_balance_total['und'][0]['value'];
    $result['pending'] = $user_object->field_balance_pending['und'][0]['value'];
    $result['redeemable'] = $user_object->field_balance_available['und'][0]['value'];
	
	$results = db_select('fitmoo_user_redemptions', 'm')
	    ->fields('m')
		->condition('uid', $user_object->uid,'=')
	    ->execute();
		
	$redemption = array();	
	while($trans = $results->fetchAssoc()){
		
		$redemption['id'] = $trans['id'];
		$redemption['amount'] = $trans['ammount'];
		$redemption['type'] = $trans['transfer_type'];
		$redemption['status'] = $trans['status'];
		$redemption['created'] = $trans['created'];
		$redemption['updated'] = $trans['updated'];
		$result['redemptions'][] = $redemption;
	}
	
	unset($user_object);
	
    return $result;
}

function commerce_services_score_index_access() {
    global $user;
    if (!empty($user->uid)) {return TRUE;} else {return FALSE;}
}