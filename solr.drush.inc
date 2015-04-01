<?php
/**
 * @file solr.drush.inc
 *
 * Provides the "solr" service type to AEgir.
 *
 * This file only cares about the SOLR side of things.  Regardless of implementation,
 * Solr needs its SOLR_HOME, conf and data directories, so this file handles that part.
 *
 */

/**
 * Implements hook_drush_init()
 */
function solr_drush_init(){
  solr_provision_register_autoload();
}

/**
 * Register our directory as a place to find Provision classes.
 *
 * This allows Provision to autoload our classes, so that we don't need to
 * specifically include the files before we use the class.
 */
function solr_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_provision_services()
 *
 *  Declares a new service type and a basic implementation of it.
 *  It matches the same service definition in the hosting front end.
 */
function solr_provision_services() {
  solr_provision_register_autoload();
  return array('solr' => NULL);
}

/**
 * Implements hook_provision_apache_vhost_config()
 * Adds the $_ENV['solr_url'] variable to the VHOST
 */
function solr_provision_apache_vhost_config($uri, $data) {
  drush_log('Adding server variable for solr_url to vhost config.');

  $host = d($uri)->solr_server->remote_host;
  $port = d($uri)->solr_server->solr_port;

  if (d($uri)->solr_server->solr_service_type == 'jetty') {
    $path = "/solr/" . $uri;
  }
  else {
    $path = "/" . $uri;
  }

  return array(
    "SetEnv aegir_solr_host $host",
    "SetEnv aegir_solr_port $port",
    "SetEnv aegir_solr_path $path",
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
 *   @TODO: THIS IS FORCED UPON USERS at the moment.... we should detect if
 *   the site is using apachesolr or searchapi!
 *
 * @see provisionConfig_drupal_settings
 */
function solr_provision_drupal_config($uri, $data, $config = array()) {
  drush_log('Adding solr_url to settings.php');

  $host = d()->solr_server->remote_host;
  $port = d()->solr_server->solr_port;
  $path = "/" . $uri;

  return <<<PHP
  // Hosting Solr
  // If \$_SERVER isn't available, set it here
  if (empty(\$_SERVER['aegir_solr_host'])) {
    \$_SERVER['aegir_solr_host'] = "$host";
    \$_SERVER['aegir_solr_port'] = "$port";
    \$_SERVER['aegir_solr_path'] = "$path";
  }
PHP;
}

