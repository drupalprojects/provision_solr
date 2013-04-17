<?php

/**
 * Implementation of hook_provision_pre_restore
 */
function drush_solr_pre_provision_restore() {
  d()->service('solr')->delete_config('site'); 
}

function drush_solr_pre_provision_restore_rollback() {
  d()->service('solr')->create_config('site');
  d()->service('solr')->parse_configs();
}

