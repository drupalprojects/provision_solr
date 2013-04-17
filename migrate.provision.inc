<?php

function drush_solr_pre_provision_migrate() {
  d()->service('solr')->delete_config('site'); 
}

function drush_http_pre_provision_migrate_rollback() {
  d()->service('solr')->create_config('site');
}
