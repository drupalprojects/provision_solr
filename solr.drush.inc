<?php
// $Id$

require_once('Provision/solr_service.inc');

/**
 * @file
 *   An solr of the provision service API.
 *
 *  Declares a new service type and a basic implementation of it.
 *  It matches the same service definition in the hosting front end.
 */

function solr_provision_services() {
  return array('solr' => 'lucene');
}

/**
 * Implements hook_drush_init()
 *
 * Ensures provision.drush.inc and provision.service.inc are included before this file.
 */
function solr_drush_init(){
  $list = drush_commandfile_list();
  $provision_drush_inc = $list['provision'];
  include_once($provision_drush_inc);
  
  $service_provision = str_replace('provision.drush.inc', 'provision.service.inc', $provision_drush_inc);
  include_once($service_provision);
}

/**
 * Implements hook_provision_apache_vhost_config()
 * Adds the $_ENV['solr_url'] variable to the VHOST
 */
function solr_provision_apache_vhost_config($uri, $data) {
  drush_log('Adding server variable for solr_url to vhost config.');

  $host = $data['server']->remote_host;
  $port = $data['server']->solr_port;
  $path = "/" . $uri;

  return array(
    "SetEnv solr_host $host",
    "SetEnv solr_port $port",
    "SetEnv solr_path $path",
  );
}

/**
 * Append PHP code to Drupal's settings.php file.
 * 
 * @param $uri
 *   URI for the site.
 * @param $data
 *   Associative array of data from provisionConfig_drupal_settings::data.
 *
 * @return
 *   Lines to add to the site's settings.php file.
 *
 * @see provisionConfig_drupal_settings
 */
function solr_provision_drupal_config($uri, $data, $config = array()) {
  $output =<<<php
/**
 * Solr Server Config
 * We are mimicking aegir database settings here for Solr
 */
if (isset(\$_SERVER['solr_host'])) {

  /**
   * Implements hook_default_search_api_server_alter().
   * This only works for search_api.
   * 
   * Yes, I am using system.module's good name to implement this hook.  
   * How else can I define a hook in settings.php?  Would love suggestions.
   */
  function system_default_search_api_server_alter(array &\$defaults) {
    if (isset(\$defaults['localhost'])){
      \$defaults['localhost']->options['host'] = \$_SERVER['solr_host'];
      \$defaults['localhost']->options['port'] = \$_SERVER['solr_port'];
      \$defaults['localhost']->options['path'] = \$_SERVER['solr_path'];
    }
  }
}
php;

  return $output;
}
