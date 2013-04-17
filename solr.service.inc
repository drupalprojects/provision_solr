<?php

// Ripped off from http.drush.inc

// Base solr service class. 
class provisionService_solr extends provisionService {
  public $service = 'solr';

  function verify_server_cmd() {
    $this->create_config($this->context->type);
    $this->parse_configs();
  }

  function verify_platform_cmd() {
    $this->create_config($this->context->type);
    $this->parse_configs();
  }

  function verify_site_cmd() {
    $this->create_config($this->context->type);
    $this->parse_configs();
  }


  /**
   * Register the solr handler for platforms, based on the solr_server option.
   */
  static function subscribe_site($context) {
    $context->setProperty('solr_server', '@server_master');
    $context->is_oid('solr_server');
    $context->service_subscribe('solr', $context->solr_server->name);
  }
}

// Public solr service , as in non-encrypted and listening on a port.
class provisionService_solr_public extends provisionService_solr {
  protected $has_port = TRUE;

  // The implementation chooses the port.   
  function default_port() {
    return FALSE;
  }

  function config_data($config = null, $class = null) {
    $data = parent::config_data($config, $class);
    
    if (!is_null($this->application_name)) {
      $data['solr_app_path'] = $this->server->solr_homes_path;
      $data['solr_homes_path'] = $this->server->solr_homes_path;
      $data['solr_war_path'] = $this->server->solr_war_path;
    }

    $data['solr_port'] = $this->server->solr_port;

    return $data;
  }

  function init_server() {
    parent::init_server();
    
    // application_name is tomcat, jetty, etc
    if (!is_null($this->application_name)) {
      $this->server->solr_app_path = "{$this->server->config_path}/{$this->application_name}";
      $this->server->solr_war_path = drush_get_option('solr_war_path', '/usr/share/solr.war');
      $this->server->solr_homes_path = "{$this->server->config_path}/solr";
    }
  }

  static function option_documentation() {
    return array(
      '--solr_war_path' => 'The path to the solr.war file on this server',
    );
  }

  function verify_server_cmd() {
    if (!is_null($this->application_name)) {
      
      // Create solr application folder (tomcat, etc)
      provision_file()->create_dir($this->server->solr_app_path, dt("Solr Application Folder"), 0700);
      $this->sync($this->server->solr_app_path);

      // Create Solr Homes folder
      provision_file()->create_dir($this->server->solr_homes_path, dt("Solr Homes Folder"), 0700);
      $this->sync($this->server->solr_homes_path);
    }

    parent::verify_server_cmd();
  }
  
  function verify_site_cmd() {
    parent::verify_site_cmd();
    //
    //// Create a SOLR_HOME for this site.
    //if (!is_null($this->application_name)) {
    //  
    //  // Create tomcat XML file
    //  $template = file_get_contents($this->server->solr_tpl);
    //  $xml_config = $this->server->tomcat6_conf_path .'/'. $solr_core_name .'.xml';
    //  $data = array(
    //    'home_directory' => $home,
    //    'solr_war_path' => $this->server->solr_war_path
    //  );
    //  
    //  $status = provision_file()->file_put_contents($xml_config, $this->render_template($template, $data));
    //  
    //  if (!$status) {
    //    return drush_set_error('DRUSH_CREATE_SOLR', dt('Problem creating !dest.', array('!dest' => $xml_config)));
    //  }
    //  
    //  //Change permissions
    //  //provision_file()->chmod($home, 0777, TRUE);
    //  
    //  //Sync if is different server_master
    //  if ($this->server->solr_server != '@server_master') {
    //    _drush_sitealias_load_alias($this->server->solr_server);
    //    $all_site_aliases = drush_get_context('site-aliases');
    //    $remote_server = $all_site_aliases[$this->server->solr_server];
    //  
    //   //sync to remote server if is need
    //   $this->sync_site_conf($this->server->solr_conf_path, $remote_server['remote_host']);
    //   $this->sync_site_conf($this->server->tomcat6_conf_path, $remote_server['remote_host']);
    //  }
    
  }
  

  /**
   * Ask the web server to check for and load configuration changes.
   */
  function parse_configs() {
    return TRUE;
  }
}


include_once('solr.config.inc');
