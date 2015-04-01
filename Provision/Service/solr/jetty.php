<?php

/**
 * @file
 *  Solr service hosted by Jetty.
 *
 */

/**
 * A class containing the 'jetty' implementation of the 'solr' service.
 */
class Provision_Service_solr_jetty extends Provision_Service_solr_public {
  protected $application_name = 'jetty';
  protected $has_restart_cmd = TRUE;

  protected $has_port = TRUE;


  function default_restart_cmd() {
    // Try to detect the solr restart command.
    $options[] = '/etc/init.d/solr';

    foreach ($options as $test) {
      if (is_executable($test)) {
        $command = $test;
        break;
      }
    }
    return "sudo $command restart";
  }

  function default_port() {
    return 8983;
  }

  function init_server() {
    $this->configs['server'][] = 'Provision_Config_Jetty_Server';
    $this->configs['site'][] = 'Provision_Config_Jetty_Site';
    parent::init_server();
  }

  /**
   * Restart solr to pick up the new config files.
   */
  function parse_configs() {
    return $this->restart();
  }

  function config_data($config = null, $class = null) {
    return parent::config_data($config, $class);
  }
}
