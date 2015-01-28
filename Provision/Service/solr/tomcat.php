<?php

/**
 * @file
 *  Solr service implemented in tomcat.
 */

/**
 * A class containing the 'tomcat' implementation of the 'solr' service.
 *
 * This class is conditionally loaded when the "--solr_service_type=tomcat"
 * option is passed to provision-save commands run on servers.
 *
 * The above flag is generated by the hosting counterpart of this class, which
 * provides the front end to configure all these fields.
 *
 * The responsibilities of this class include responding and saving any
 * values that are passed to it, and also to override the portions of
 * the public API for this service that are necessary.
 */
class Provision_Service_solr_tomcat extends Provision_Service_solr_public {
  protected $application_name = 'tomcat';
  protected $has_restart_cmd = TRUE;

  protected $has_port = TRUE;

  function default_restart_cmd() {
    return Provision_Service_solr_tomcat::tomcat_restart_cmd();
  }

  function default_port() {
    return 8080;
  }

  function init_server() {
    $this->configs['server'][] = 'Provision_Config_Tomcat_Server';
    $this->configs['site'][] = 'Provision_Config_Tomcat_Site';
    parent::init_server();
  }
//
//  function config_data($config = null, $class = null) {
//    $data = parent::config_data($config, $class);
//    return $data;
//  }

  /**
   * Guess at the likely value of the http_restart_cmd.
   *
   * This method is a static so that it can be re-used by the apache_ssl
   * service, even though it does not inherit this class.
   */
  public static function tomcat_restart_cmd() {

    // Try to detect the apache restart command.
    $options[] = '/etc/init.d/tomcat7';
    $options[] = '/etc/init.d/tomcat6';

    foreach ($options as $test) {
      if (is_executable($test)) {
        $command = $test;
        break;
      }
    }
    return "sudo $command restart";
  }

  /**
   * Restart apache to pick up the new config files.
   */
  function parse_configs() {
    return $this->restart();
  }
}