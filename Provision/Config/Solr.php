<?php

/**
 * Base class for Solr config files.
 *
 * This class will publish the config files to remote
 * servers automatically.
 */
class Provision_Config_Solr extends Provision_Config {
  function write() {
    parent::write();
    $this->data['server']->sync($this->data['solr_home']);
  }

  function unlink() {
    parent::unlink();
    $this->data['server']->sync($this->data['solr_home']);
  }
}
