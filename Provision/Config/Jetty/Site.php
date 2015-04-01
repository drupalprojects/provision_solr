<?php

/**
 * Solr site level config class.
 */
class Provision_Config_Jetty_Site extends Provision_Config_Solr_Site {

  public $template = 'core.properties.tpl.php';
  public $description = 'Solr core properties file.';

  function filename() {
    return $this->data['solr_home'] . '/core.properties';
  }

  function write() {
    parent::write();
  }
}


