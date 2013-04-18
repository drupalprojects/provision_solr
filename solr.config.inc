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
  public $template = '';
  public $description = 'Link to app localhost folder.';

  function write() {
    
    // $file is tomcat or jetty
    if (isset($this->context->solr_service_type)) {
      $filename = $this->filename();
      // We link the app_name file on the remote server to the right version.
      $cmd = sprintf('ln -sf %s %s', 
        escapeshellarg($filename), 
        escapeshellarg($this->context->aegir_root . '/config/')
      );
      
      if ($this->context->shell_exec($cmd)) {
        drush_log(dt("Created symlink for %file on %server", array(
          '%file' => $filename,
          '%server' => $this->context->remote_host,
        )));  
       
      };
    }
    parent::write();
  }
  
  /**
   * Returns the path to the service implementation config folder.
   */
  function filename() {
    if (isset($this->context->solr_service_type)) {
      $filename = $this->context->config_path . '/' .$this->context->solr_service_type;
      drush_log("filename = $filename");
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
  public $template = 'site.tpl.php';
  public $description = 'Tomcat webapp configuration file';

  function filename() {
    $filename = $this->server->solr_homes_path . '/' . $this->uri;
    drush_log("filename = $filename");
    return $filename;
  }

  function write() {
    // Creates solr_home directory and application xml file
    
    // Prep paths
    $solr_home = $this->filename();
    
    // CONTEXT is from the server node in hostmaster
    $path_to_drupal_conf = $this->context->root .'/'. $this->context->solr_config_path;
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
    drush_log(dt('Copying files in %default/conf to SOLR_HOME at %home', $t));
    drush_shell_exec("cp -rf $path_to_default_conf/conf/* $solr_home/conf");
      
    // Now copy this site's config over that.
    drush_log(dt('Copying files in %drupal to SOLR_HOME at %home', $t));
    // @TODO: Gotta fix path_to_drupal_conf, has to load from hostmaster, until
    // we figure out how to save it in the site.
    //drush_shell_exec("cp -rf $path_to_drupal_conf/* $solr_home/conf");
    
    parent::write();

    //// @TODO: This was supposed to be automatic, i thought.  http.config.inc
    // Now save our server file
    // doesn't do this...
    $path_to_solr_server_config = $this->server->solr_app_path . '/' . $this->uri . '.xml';
    $path_to_template = __DIR__ . '/' . $this->server->solr_service_type . '/' . $this->template;
    
    $template = file_get_contents($path_to_template);
    $data = array(
      'solr_home_path' => $solr_home,
      'solr_war_path' => $this->solr_server->solr_war_path
    );
    $contents = $this->render_template($template, $data);
    
    // @TODO WTF?!?  It's writing an empty file 
    $status = provision_file()->file_put_contents($path_to_solr_server_config, $contents);
    
    drush_log("SAVING TO: $path_to_solr_server_config " . $contents, "ok");
    if (!$status) {
      return drush_set_error('DRUSH_CREATE_SOLR', dt('Problem creating !dest.', array('!dest' => $xml_config)));
    }
  }

  function unlink() {
    parent::unlink();
  }

  function process() {
    parent::process();
  }
  
  /**
   * @TODO: This shouldn't be needed for us too, right?
   */
  private function render_template($template, $variables) {
    drush_errors_off();
    extract($variables, EXTR_SKIP); // Extract the variables to a local namespace
    ob_start(); // Start output buffering
    eval('?>' . $template); // Generate content
    $contents = ob_get_contents(); // Get the contents of the buffer
    ob_end_clean(); // End buffering and discard
    drush_errors_on();
    return $contents; // Return the contents
  }
}
