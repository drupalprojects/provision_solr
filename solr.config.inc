<?php
/**
 * @file solr.config.inc
 *    Configuration classes for the SOLR service.
 */


/**
 * Base class for Solr config files.
 *
 * This class will publish the config files to remote
 * servers automatically.
 */
class provisionConfig_solr extends provisionConfig {
  function write() {
    parent::write();
    $filename = $this->filename();
    $this->context->sync($filename);
  }

  function unlink() {
    parent::unlink();
    $this->data['server']->sync($this->filename());
  }
}

/**
 * Base configuration class for server level http configs.
 *
 * This class uses the service defined application_name, to generate
 * a top level $app_name.conf for the service.
 *
 * Each server has it's own configuration, and this class will
 * automatically generate a symlink to the correct file for each
 * server.
 */
class provisionConfig_solr_server extends provisionConfig_solr {
  public $template = 'server.tpl.php';
  public $description = 'Link to app localhost folder.';

  function write() {
    parent::write();
    
    drush_log('CALLING: provisionConfig_solr_server::write()', 'warning');
    
    // $file is tomcat or jetty
    if (isset($this->context->solr_service_type)) {
      $file = $this->context->solr_service_type;
      // We link the app_name file on the remote server to the right version.
      $cmd = sprintf('ln -sf %s %s', 
        escapeshellarg($this->context->config_path . '/' . $file), 
        escapeshellarg($this->context->aegir_root . '/config/')
      );
      
      if ($this->context->shell_exec($cmd)) {
        drush_log(dt("Created symlink for %file on %server", array(
          '%file' => $file,
          '%server' => $this->context->remote_host,
        )));  
       
      };
    }
  }
  
  /**
   * Generates a foldername for solr service implementation (tomcat, jetty, etc.)
   */
  function filename() {
    if (isset($this->context->solr_service_type)) {
      $file = $this->context->solr_service_type;
      return $this->context->config_path . '/' . $file;
    }
    else {
      return '';
    }
  }
}

/**
 * Base class for virtual host configuration files.
 */
class provisionConfig_solr_site extends provisionConfig_solr {
  public $template = 'site.tpl.php';
  public $description = 'Tomcat webapp configuration file';

  function filename() {
    return $this->context->solr_homes_path . '/' . $this->uri;
  }

  function write() {
    // Creates solr_home directory and application xml file
    parent::write();
    
    drush_log('CALLING: provisionConfig_solr_server::write()', 'warning');

    
  }

  function unlink() {
    parent::unlink();
  }

  function process() {
    parent::process();
  }
}
