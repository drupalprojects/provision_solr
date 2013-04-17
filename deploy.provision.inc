<?php

function drush_solr_post_provision_deploy() {
  d()->service('solr')->create_config('site');
  d()->service('solr')->parse_configs();
}

/**
 * Delete the config file created.
 */
function drush_solr_post_provision_deploy_rollback() {
  d()->service('solr')->create_config('site');
  d()->service('solr')->parse_configs();
}