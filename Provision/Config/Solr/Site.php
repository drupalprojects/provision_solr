<?php

/**
 * Base configuration class for site level Solr configs.
 */
class Provision_Config_Solr_Site extends Provision_Config_Solr {

  function write() {
    parent::write();

    // Prep paths
    $solr_home = $this->data['solr_home'];
    $path_to_drupal_conf = $this->data['solr_config_path'];

    // @TODO: Make solr version a server property.
    $path_to_default_conf = realpath(__DIR__ . '/../../../solr/conf/3.x');

    // Save some translation strings for messages.
    $t = array();
    $t['!home'] = $solr_home;
    $t['!default'] = $path_to_default_conf;
    $t['!drupal'] = $path_to_drupal_conf;

    // Create the solr home dir if its not there yet. (~/config/SERVER/solr/SITE)
    if (!provision_file()->exists($solr_home)->status()) {
      provision_file()->mkdir($solr_home)
        ->succeed('Created directory @path.')
        ->fail('Created a new SOLR_HOME at @path.');

      provision_file()->mkdir($solr_home . '/conf')
        ->succeed('Created directory @path.')
        ->fail('Could not created a new SOLR_HOME at @path.');
    }

    if (!file_exists($path_to_default_conf)) {
      return drush_set_error(DRUSH_FRAMEWORK_ERROR, 'default solr config files not found at ' . $path_to_default_conf);
    }

    // Copying default solr config from this module.
    // NOTE: Cannot use provision_file() because it uses copy() which only allows files.
    drush_log(dt('Copying default solr conf files in !default to SOLR_HOME at !home', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_default_conf $solr_home/conf");

    // Now copy this site's config over that.
    drush_log(dt('Copying site solr conf files in !drupal to SOLR_HOME at !home', $t), 'ok');
    drush_shell_exec("cp -rf $path_to_drupal_conf/* $solr_home/conf");

    // Ensure chmod of the solr home folders.
    provision_file()->chmod($this->data['server']->solr_homes_path, 0775);
  }

  function unlink() {
    parent::unlink();

    $home_dir = $this->data['solr_home'];

    // Remove solr home folder.
    provision_file()->rmdir($home_dir)
      ->succeed('Solr Home folder was removed: @path.')
      ->fail('Could not remove Solr Home folder: @path.');
  }

  function process() {
    parent::process();
  }
}
