<?php

// $Id$

/**
 * @file
 *   Provision hooks for the install command
 */
function drush_solr_provision_install() {
  d()->service('solr')->create_config('site');
}

/**
 * Implementation of hook_provision_post_install
 */
function drush_solr_post_provision_install() {
  d()->service('solr')->parse_configs();
}

/**
 * Implementation of hook_provision_install_rollback()
 *
 * Remove the apache configs if we fail to install the site.
 */
function drush_solr_provision_install_rollback() {
  d()->service('solr')->delete_config('site');
}
