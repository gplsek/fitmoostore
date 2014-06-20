<!-- <p style="margin:0; font-weight:normal;">Good news! Someone purchased <?php //print $product;?>. For details please visit <a href="https://beta.fitmoo.com/dashboard/sales">Your Sales</a> at Fitmoo.com. We will cancel this transaction if the item is not shipped within <strong>5days</strong></p>
<h1>Order Details</h1>

Order #: <?php //print $orderID;?> 
Placed on: <?php //print gmdate("Y-m-d\TH:i:s\Z", $orderDate);?>  -->
<?php
global $base_url;
$path = $base_url.'/'.drupal_get_path('module', 'fitmoo_checkout');

$image = array(
  'path' => $uri,
  'alt' => 'product image',
  'style_name' => 'thumbnail',
);

$redirect_base = variable_get('anonymous_redirect_base', '');

//print theme('image_style', $image);
	
?>

<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    <meta charset="utf-8">
    <!--[if IE]>
    <meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1'><![endif]-->
    <title>FITMOO Your shipping label</title>
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

    </style>

</head>
<body style="height:100%; margin:0; padding:0; font-size: 18px; ">
<div class="main-site_container">
    <div style="width:1000px; margin: 0 auto; border-bottom: 2px solid #a9a9a9; padding-top: 20px; padding-bottom: 20px; border-bottom-width: 5px;">
        <a href="<?php print $redirect_base;?>" class="header-logo">
            <img src="<?php print $path;?>/templates/img/head-logo.png" alt="FITMOO" height="33" width="133">
        </a>
    </div>


    <div style="min-width: 1000px;">

        <section>
            <div style="width:1000px; margin: 0 auto; border-bottom: 2px solid #a9a9a9; padding-top: 20px; padding-bottom: 20px;">
                <p style="font-size: 18px; line-height: 30px; font-family: 'bentonsansbook'; width: 95%;">Thank you for shopping us. We would like to let you know that your item is on its way. To view details or to make any changes to this shipment or manage other orders, please visit <a style="color:#38a0dc; text-decoration: none;" href="<?php print $redirect_base;?>/dashboard/sales/<?php print $OrderId;?>"">Your Orders</a> on Fitmoo.com</p>
            </div>
        </section>
        <section class="products">
            <div style="width:1000px; margin: 0 auto; border-bottom: 2px solid #a9a9a9; padding-top: 29px; padding-bottom: 31px;">
                <div  style="min-width: 1000px;">
                    <fieldset style="border:0; margin: 0; padding: 0; position: relative; top: -3px; width: 130px; float: left; border:0;">
                        <div class="fieldset-wrapper">
                            <div class="field-type-image">
                                <div class="field-items">
                                    <div class="field-item even"><?php print theme('image_style', $image);?></div>
                                </div>
                            </div>
                        </div>
                    </fieldset>
                    <fieldset style="border:0; margin: 0; padding: 0;" id="commerce_product_product_node_teaser_group_product_details">
                        <div class="fieldset-wrapper">
                            <div class="field-type-details">
                                <div style="float: left; width: 50%;">
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansmedium';"><b><?php print $ProductName; ?></b></div>
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';">Order #:&nbsp;<?php print $orderId;?></div>
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';">Status:&nbsp;<?php print $ShippingStatus;?></div>
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';">Tracking #:&nbsp;<?php print $TrackingNum; ?></a></div>
                                </div>
                                <div style="float: right; width: 50%;">
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';">Send to:</div>
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';"><b><?php print $ShipToName;?></b></div>
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';"><b><?php print $ShipToAdress; ?></b></div>
                                    <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';"><b><?php print $ShipToAdress2; ?></b></div>
                                </div>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </section>
        <section class="total">
            <div style="width:1000px; margin: 0 auto; border-bottom: 2px solid #a9a9a9; padding-top: 20px; padding-bottom: 20px;">
                <div style="font-size: 18px; font-family: 'bentonsansbook'; width: 95%;">
                    <p>
                        Please allow 5 - 7 business days for delivery. If you need further assistance, <a style="color:#38a0dc; text-decoration: none;" href="<?php print $LabelLink; ?>">contact the seller using your dashboard</a>.
                    </p>
                </div>
            </div>
        </section>
    </div>
</div>
</body>
