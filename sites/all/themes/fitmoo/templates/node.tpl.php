<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if (!$is_front): ?>
    <?php print render($title_prefix); ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
    <?php print render($title_suffix); ?>
  <?php endif; ?>
  
  <div class="content clearfix"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <?php if($teaser):?>
	  <div class="comments"><a href="<?php print $node_url; ?>#comments">Comments</a></div>
  <?php endif;?>
  <?php print render($content['comments']); ?>

</div>
