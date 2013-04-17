<?php

/**
 * Implementation of hook_provision_enable
 */
function drush_solr_provision_enable() {
  d()->service('solr')->create_config('site');
  d()->service('solr')->parse_configs();
}

