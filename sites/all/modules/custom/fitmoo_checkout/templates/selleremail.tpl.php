<p>Good news! Someone purchased <?php print $product;?>. For details please visit <a href="https://beta.fitmoo.com/dashboard/sales">Your Sales</a> at Fitmoo.com</p>
<h1>Order Details</h1>

Order #: <?php print $orderID;?> 
Placed on: <?php print gmdate("Y-m-d\TH:i:s\Z", $orderDate);?> 


<?php print views_embed_view('commerce_cart_summary','block_3', 65); ?>
