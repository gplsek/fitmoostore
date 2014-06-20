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
                <p style="font-size: 18px; line-height: 30px; font-family: 'bentonsansbook'; width: 95%;">Your package is ready to be shipped. Please use the attached shipping label or click the following link to generate a shipping label:<BR>
                    <a style="color:#38a0dc; text-decoration: none;" href="<?php print $LabelLink; ?>"><?php print $LabelLink; ?></a></p>
                <h1 style="color: #333; font-size: 21px; text-decoration: none; font-family: 'bentonsansmedium'; font-weight:bold;">Step to ship the following item:</h1>
            </div>
        </section>
        <section class="products">
            <div style="width:1000px; margin: 0 auto; border-bottom: 2px solid #a9a9a9; padding-top: 29px; padding-bottom: 31px;">
                <div style="width:96%">
                    <div style="color: #333; font-size: 18px; padding-left: 132px; line-height: 24px; text-decoration: none; font-family: 'bentonsansmedium';"><b><?php print $ProductName; ?></b></div>
                    <a style="color: #333; float:right; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansmedium';" href="<?php print $ProductLink; ?>" style="float: right;">$<?php print $total; ?></a>
                </div>
                <div  style="min-width: 1000px;">
                    <fieldset style="border:0; margin: 0; padding: 0; position: relative; top: -38px; width: 130px; float: left; border:0;">
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
                                <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';">Size:&nbsp;<?php print $size;?></div>
                                <div style="color: #333; font-size: 18px; line-height: 24px; text-decoration: none; font-family: 'bentonsansregular';">Qty:&nbsp;<?php print $qty;?></div>
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
                        1. Print the <a style="color:#38a0dc; text-decoration: none;" href="<?php print $LabelLink; ?>">shipping label</a> that has been generated for you.<BR><BR>
                        2. Pack the item in a box or envelope and ensure that it is properly sealed..<BR><BR>
                        3. Attach the printed shipping label on this package.<BR><BR>
						4. Drop off at your near USPS post off or schedule a pickup with USPS.
                    </p>
                    <h1 style="color: #333; font-size: 21px; text-decoration: none; font-family: 'bentonsansmedium'; font-weight:bold;">What next:</h1>
                    <p style="line-height: 25px;">72 hours after the order has been delivered, your earnings will be available for redemption from the Fitmoo dashboard. Note: You have 7 days to ship the item. If the item is not delivered within 7 days, the buyer has option to cancel the order.
</p>
                </div>
            </div>
        </section>
    </div>
</div>
</body>
</html>
