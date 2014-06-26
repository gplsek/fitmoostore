<?php
global $base_url;
$path = $base_url.'/'.drupal_get_path('module', 'fitmoo_checkout');

$image = array(
  'path' => $uri,
  'alt' => 'product image',
  'style_name' => 'thumbnail',
);

$redirect_base = variable_get('anonymous_redirect_base', '');

$fitfeeperc = $buyperc + $fitperc;

//print theme('image_style', $image);
	
?>

<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    <meta charset="utf-8">
    <!--[if IE]>
    <meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1'><![endif]-->
    <title>FITMOO</title>
    <meta name="description" content="">
    <meta name="viewport" content="width=device-width">

    <style>
    @font-face {
        font-family: 'bentonsansregular';
        src: url('<?php print $path;?>/templates/fonts/bentonsans-regular.eot');
        src: url('<?php print $path;?>/templates/fonts/bentonsans-regular.eot?#iefix') format('embedded-opentype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-regular.woff') format('woff'),
        url('<?php print $path;?>/templates/fonts/bentonsans-regular.ttf') format('truetype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-regular.svg#bentonsansregular') format('svg');
        font-weight: normal;
        font-style: normal;
    }
    @font-face {
        font-family: 'bentonsansblack';
        src: url('<?php print $path;?>/templates/fonts/bentonsans-black.eot');
        src: url('<?php print $path;?>/templates/fonts/bentonsans-black.eot?#iefix') format('embedded-opentype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-black.woff') format('woff'),
        url('<?php print $path;?>/templates/fonts/bentonsans-black.ttf') format('truetype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-black.svg#bentonsansblack') format('svg');
        font-weight: normal;
        font-style: normal;
    }
    @font-face {
        font-family: 'bentonsansbook';
        src: url('<?php print $path;?>/templates/fonts/bentonsans-book.eot');
        src: url('<?php print $path;?>/templates/fonts/bentonsans-book.eot?#iefix') format('embedded-opentype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-book.woff') format('woff'),
        url('<?php print $path;?>/templates/fonts/bentonsans-book.ttf') format('truetype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-book.svg#bentonsansbook') format('svg');
        font-weight: normal;
        font-style: normal;
    }
    @font-face {
        font-family: 'bentonsansmedium';
        src: url('<?php print $path;?>/templates/fonts/bentonsans-medium.eot');
        src: url('<?php print $path;?>/templates/fonts/bentonsans-medium.eot?#iefix') format('embedded-opentype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-medium.woff') format('woff'),
        url('<?php print $path;?>/templates/fonts/bentonsans-medium.ttf') format('truetype'),
        url('<?php print $path;?>/templates/fonts/bentonsans-medium.svg#bentonsansmedium') format('svg');
        font-weight: normal;
        font-style: normal;
    }

    /*-------------------------------------*/


    ::-webkit-input-placeholder {
        color:    #999999;
    }
    :-moz-placeholder {
        color:    #999999;
    }
    ::-moz-placeholder {
        color:    #999999;
    }
    :-ms-input-placeholder {
        color:    #999999;
    }


    /*-------------------------------------*/
    fieldset {
        border:0;
    }
    input,
    a,
    img{
        outline:none;
        border:none;
    }
    body{
        font-size:12px;
        color:#333333;
    }
    a {
        color:#38a0dc;
        text-decoration: none;

    }
    h1,h2,h3,h4,h5,
    p{
        margin:0;
        font-weight:normal;
    }
    h1 {
        color: #333;
        font-size: 21px;
        text-decoration: none;
        font-family: 'bentonsansmedium';
        font-weight:bold;
    }

    .promo-desc {
        font-size: 18px;
        line-height: 30px;
        font-family: 'bentonsansbook';
        margin: 0px 3px 0 0;
    }

    :focus {
        outline: none;
    }

    html,
    body{
        height:100%;
        margin:0;
        padding:0;
    }
    .main-site_container{

        min-width:1000px;
    }

    .section-wrapper{
        width:1000px;
        margin: 0 auto;
        border-bottom: 2px solid #a9a9a9;
        padding-top: 20px;
        padding-bottom: 20px;
    }
    .clear-ul{
        padding:0;
        margin:0;
        list-style:none;
    }
    .clearfix:before,
    .clearfix:after {
        content: " ";
        display: table;
    }
    .clearfix:after {
        clear: both;
    }
    .clearfix {
        *zoom: 1;
    }

    .content{
        min-width: 1000px;
    }
    .store-text {
        font-family: 'bentonsansregular';
        font-size:12px;
        line-height:18px;
        padding: 3px 0 0 0;
    }

    .mailtext{
        background:#FFF;
    }

    .views-formh2 {
        margin-left: 174px;
    }
    .rcolumn {
        float: right;
        width: 50%;
        text-align: right;
    }
    .lcolumn {
        float: left;
        width: 50%;
    }


    .views-formh2 a {
        color: #333;
        font-size: 18px;
        line-height: 24px;
        text-decoration: none;
        font-family: 'bentonsansmedium';
    }

    .field-label {
        color: #333;
        font-size: 18px;
        line-height: 24px;
        text-decoration: none;
        font-family: 'bentonsansregular';
    }

    .cart_contents_form .views-form {
        display: table;
        margin-bottom: 20px;
        width: 96%;
    }
    .content-div {
        font-size: 18px;
        line-height: 30px;
        font-family: 'bentonsansbook';
        float: left;
        width: 95%;
    }

    #commerce_product_product_node {
        position: relative;
        top: -36px;
        width: 130px;
        float: left;
        border:0;
    }
    </style>

</head>
<body>
<div class="main-site_container">
    <div class="section-wrapper clearfix" style="border-bottom-width: 5px;">
       <a href="<?php print $redirect_base;?>" class="header-logo">
           <img src="<?php print $path;?>/templates/img/head-logo.png" alt="FITMOO" height="33" width="133">
       </a>
    </div>


    <div class="content clearfix otherPeople-store">

        <section class="mailtext">
            <div class="section-wrapper clearfix">
                <div class="content-div">
                    <p>Thank you for shopping with us. This is to inform you that your order has been received and we have already notified the seller. 
		The seller will have 5 days to prepare the shipment and ship your item. 
		The estimated delivery time is 5-10 business days. To view the status of your order, please visit 
		<a href="<?php print $redirect_base;?>/dashboard/purchases">Your order</a> on Fitmoo.com </p>
                </div>
                <div class="content-div" style="padding-top: 20px;">
                <h1>Order Details</h1>
                    <div style="float: left;">Order #: <a href="<?php print $redirect_base;?>/dashboard/purchases"><?php print $orderID;?></a></div>
                    <div style="float: right;">Placed on: <?php print date("M jS, Y", $orderDate);?></div>
                </div>
                <div class="content-div" style="padding-top: 20px;">
                    <div class="lcolumn">
                        <div style="font-weight: bold;">Shipped to:</div>
                        <div><?php print $shipto['name_line'];?></div>
                        <div><?php print $shipto['thoroughfare'];?> <?php print $shipto['premise'];?></div>
                        <div><?php print $shipto['locality'];?>, <?php print $shipto['administrative_area'];?> <?php print $shipto['postal_code'];?> <?php print $shipto['country'];?></div>
                    </div>
                    <div class="rcolumn">
                        <!-- <div style="font-weight: bold;">Payment info:</div>
                        <div>Americaan Express</div>
                        <div>Last4: 5252</div> -->
                    </div>
                </div>
            </div>

        </section>
        <section class="products">
            <div class="section-wrapper clearfix">
                    <div class="views-form">
                        <div class="views-row views-row-1 views-row-odd views-row-first views-row-last">

                            <div class="content-div" style="height: 100px;">
                                        <div class="views-formh2">
                                            <a href="#"><?php print $product;?></a>
                                            <a href="#" style="float: right;">$<?php print $total;?></a>
                                        </div>
                                        <div class="content">
                                            <fieldset id="commerce_product_product_node">
                                                 <div class="fieldset-wrapper">
                                                    <div class="field-type-image">
                                                        <div class="field-items">
                                                            <div class="field-item even"><?php print theme('image_style', $image);?></div>
                                                        </div>
                                                    </div>
                                                </div>
                                            </fieldset>
                                            <fieldset id="commerce_product_product_node_teaser_group_product_details">
                                                <div class="fieldset-wrapper">
                                                    <div class="field-type-details">
                                                        <div class="field-label">Size:&nbsp;<?php print $size;?></div>
                                                        <div class="field-label">Qty:&nbsp;<?php print number_format(floor($qty));?></div>
                                                    </div>
                                                </div>
                                            </fieldset>
                                        </div>
                            </div>
                        </div>
                    </div>

            </div>
        </section>
        <section class="total">
            <div class="section-wrapper clearfix">
                <div class="content-div">
                    <div class="rcolumn">
                        <div>
                            <div class="lcolumn">Price</div>
                            <div class="rcolumn">$<?php print $total*$qty;?></div>
                        </div>
                        <div>
                            <div class="lcolumn">Estimated Tax</div>
                            <div class="rcolumn">$0.00</div>
                        </div>
                        <div>
                            <div class="lcolumn">Shipping</div>
                            <div class="rcolumn">$<?php print $shipping;?></div>
                        </div>
                        <div>
                            <div class="lcolumn">Order total</div>
                            <div class="rcolumn">$<?php print $ordertotal;?></div>
                        </div>
                    </div>
                </div>

            </div>
        </section>


    </div>
    <!--END content-->

</div>
<!--END main-site_container-->


<footer class="siteFooter">

</footer>


</body>
</html>