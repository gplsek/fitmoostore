<div id="siteWrapper">
  <div id="pageWrapper">

    <!-- header top -->
      <div id="header-top">
        <div id="themeHeader">
          <div id="logoWrap" class="headerBlock">
            <?php if($is_front && $title): ?>
              <h1 id="page-title" class="title" title="<?php print $site_name; ?>"><a href="/" title="<?php print $site_name; ?>"><img src="<?php print base_path().path_to_theme(); ?>/logo.png" alt="<?php print $site_name; ?>" /></a></h1>
            <?php else: ?>
              <div id="logo" title="<?php print $site_name; ?>"><a href="/" title="<?php print $site_name; ?>"><img src="<?php print base_path().path_to_theme(); ?>/logo.png" alt="<?php print $site_name; ?>" /></a></div>
            <?php endif; ?>
          </div>
        </div>
        <?php if ($page['header_top']): ?>
          <?php print render($page['header_top']); ?>
        <?php endif; ?>
      </div>
  
    <!-- header -->
    <div id="header">
      <?php if ($page['header']): ?>
        <?php print render($page['header']); ?>
      <?php endif; ?>
    </div>

    <!-- messages -->
    <?php if ($messages): ?>
      <div id="messages">
        <?php print $messages; ?>
      </div>
    <?php endif; ?>

    <!-- tabs -->
    <?php if ($tabs): ?>
      <div class="tabs">
        <?php print render($tabs); ?>
      </div>
    <?php endif; ?>

    <!-- action links -->
    <?php if ($action_links): ?>
      <ul class="action-links">
        <?php print render($action_links); ?>
      </ul>
    <?php endif; ?>

    <!-- content top -->
    <?php if ($page['content_top']): ?>
      <div id="content-top">
        <?php print render($page['content_top']); ?>
      </div>
    <?php endif; ?>

    <div id="contentWrapper">

      <!-- sidebar first -->
      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first">
          <?php print render($page['sidebar_first']); ?>
        </div>
      <?php endif; ?>

      <!-- content -->
      <div id="content">
        <?php print render($title_prefix); ?>
        <?php print render($title_suffix); ?>
        <?php print render($page['help']); ?>
        <?php print render($page['content']); ?>
      </div><!-- /#content -->

      <!-- sidebar first -->
      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-second">
          <?php print render($page['sidebar_second']); ?>
        </div>
      <?php endif; ?>
    </div><!--contentWrapper-->

    <!-- content bottom -->
    <?php if ($page['content_bottom']): ?>
      <div id="content-bottom">
        <?php print render($page['content_bottom']); ?>
      </div>
    <?php endif; ?>

    <!-- footer -->
    <?php if ($page['footer']): ?>
      <div id="footer">
        <?php print render($page['footer']); ?>
      </div> <!-- /#footer -->
    <?php endif; ?>

  </div> <!-- /#page -->
</div>
