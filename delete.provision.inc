<?php

function drush_solr_provision_delete() {
  if (d()->type === 'site') {
    d()->service('solr')->delete_config('solr');
  }
  d()->service('solr')->parse_configs();
}
