<?php

/**
 * Implementation of hook_provision_disable
 */
function drush_solr_provision_disable() {
  d()->service('solr')->create_config('site');
  d()->service('solr')->parse_configs();
}

