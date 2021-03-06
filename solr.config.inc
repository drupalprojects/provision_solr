<?php
/**
 * @file solr.config.inc
 *    Configuration classes for the SOLR service.
 */

 /**
  *
  * @TODO:  For some reason, $this->data is never set.
  *
  * I cannot figure out where it is supposed to happen.
  * This might be the cause of the inability to use the templating system
  * properly, and other weird quirks.
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
    $this->data['server']->sync($this->filename());
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
        drush_log(dt("Created symlink for %file on %server", array(
          '%file' => $filename,
          '%server' => $this->data['server']->remote_host,
        )));  
      };
    }
    parent::write();
  }
  
  /**
   * Returns the path to the service implementation config folder.
   */
  function filename() {
    if (isset($this->data['application_name'])) {
      $filename = $this->data['server']->config_path . '/' . $this->data['application_name'];
      return $filename;
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
  
  // @TODO: REMOVE HACK!  For some reason, provisionConfig_tomcat_site isn't used.
  public $template = 'site.tpl.php';
  public $description = 'Tomcat webapp configuration file';
  
  // Needs to return the filename of the template file?
  function filename() {
    $filename = $this->data['server']->solr_app_path . '/' . $this->uri . '.xml';
    return $filename;
  }

  function write() {
    
    // Writes application xml file.
    parent::write();

    // Prep paths
    $solr_home = $this->data['solr_home'];
    $path_to_drupal_conf = $this->data['solr_config_path'];
    $path_to_default_conf = __DIR__ . '/solr';
    
    // Save some translation strings for messages.
    $t = array();
    $t['%home'] = $solr_home;
    $t['%default'] = $path_to_default_conf;
    $t['%drupal'] = $path_to_drupal_conf;
    
    // Create the solr home dir if its not there yet. (~/config/SERVER/solr/SITE)
    if (!provision_file()->exists($solr_home)->status()) {
      drush_mkdir($solr_home);
      drush_mkdir($solr_home . '/conf');
      drush_log(dt('Created a new SOLR_HOME at %home', $t));
    }
    
    // Copy the config files in.
    drush_log(dt('Copying %default/solr.xml to SOLR_HOME', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_default_conf/solr.xml $solr_home");

    drush_log(dt('Copying files in %default/conf to SOLR_HOME at %home', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_default_conf/conf/* $solr_home/conf");
      
    // Now copy this site's config over that.
    drush_log(dt('Copying files in %drupal to SOLR_HOME at %home', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_drupal_conf/* $solr_home/conf");
    
    // Ensure chmod of the solr home folders.
    provision_file()->chmod($this->data['server']->solr_homes_path, 0775);
  }

  function unlink() {
    parent::unlink();
  }

  function process() {
    parent::process();
  }
}
