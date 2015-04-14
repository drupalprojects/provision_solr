<?php

/**
 * Base configuration class for server level Solr configs.

 * Each server has it's own configuration, and this class will
 * automatically generate a symlink to the correct file for each
 * server.
 */
class Provision_Config_Solr_Server extends Provision_Config_Solr {
  public $template = '';
  public $description = 'Link to app localhost folder.';

  function write() {

    // Symlink to application directory to the correct server config folder
    if (isset($this->data['application_name'])) {
      $filename = $this->filename();
      // We link the app_name file on the remote server to the right version.
      $cmd = sprintf('ln -sf %s %s',
        escapeshellarg($filename),
        escapeshellarg($this->data['server']->aegir_root . '/config/')
      );
      if ($this->data['server']->shell_exec($cmd)) {
        drush_log(dt("Created symlink for !file on !server, for !application", array(
          '!file' => $filename,
          '!server' => $this->data['server']->remote_host,
          '!application' => $this->data['application_name'],
        )), 'success');
      };
    }
    parent::write();
  }

  /**
   * Returns the path to the service implementation config folder.
   */
  function filename() {
    if (isset($this->data['application_name'])) {
      return  $this->data['server']->config_path . '/' . $this->data['application_name'];
    }
    else {
      return FALSE;
    }
  }
}