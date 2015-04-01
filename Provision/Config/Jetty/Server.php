<?php

/**
 * Jetty server level configuration file class
 */
class Provision_Config_Jetty_Server extends Provision_Config_Solr_Server {
  protected $mode = 0755;

  function process() {
    parent::process();
  }

  function write() {
    parent::write();

    // Ensure chmod of the solr home folder
    provision_file()->chmod($this->filename(), $this->mode);
  }
}
