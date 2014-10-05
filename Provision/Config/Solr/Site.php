<?php

/**
 * Base class for a site's Solr configuration files.
 */
class Provision_Config_Solr_Site extends Provision_Config_Solr {

  // @TODO: REMOVE HACK!  For some reason, provisionConfig_tomcat_site isn't used.
  public $template = 'site.tpl.php';
  public $description = 'Tomcat webapp configuration file';

  function filename() {
    return $this->data['server']->solr_app_path . '/' . $this->uri . '.xml';
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
