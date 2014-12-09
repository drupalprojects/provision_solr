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
    $path_to_solr_xml =  realpath(__DIR__ . '/../../../solr/solr.xml');

    // @TODO: Make solr version a server property.
    $path_to_default_conf = realpath(__DIR__ . '/../../../solr/conf/3.x');

    // Save some translation strings for messages.
    $t = array();
    $t['%home'] = $solr_home;
    $t['%default'] = $path_to_default_conf;
    $t['%solr_xml'] = $path_to_solr_xml;
    $t['%drupal'] = $path_to_drupal_conf;

    // Create the solr home dir if its not there yet. (~/config/SERVER/solr/SITE)
    if (!provision_file()->exists($solr_home)->status()) {
      drush_mkdir($solr_home);
      drush_mkdir($solr_home . '/conf');
      drush_log(dt('Created a new SOLR_HOME at %home', $t));
    }

    // Copy the config files in.
    if (!file_exists($path_to_solr_xml)) {
      return drush_set_error(DRUSH_FRAMEWORK_ERROR, 'solr.xml file not found at ' . $path_to_solr_xml);
    }
    drush_log(dt('Copying %solr_xml to SOLR_HOME', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_solr_xml $solr_home");

    if (!file_exists($path_to_default_conf)) {
      return drush_set_error(DRUSH_FRAMEWORK_ERROR, 'default solr config files not found at ' . $path_to_default_conf);
    }
    drush_log(dt('Copying default solr conf files in %default to SOLR_HOME at %home', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_default_conf/* $solr_home/conf");

    // Now copy this site's config over that.
    drush_log(dt('Copying site solr conf files in %drupal to SOLR_HOME at %home', $t), 'ok');
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
