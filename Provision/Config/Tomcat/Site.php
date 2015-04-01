<?php

/**
 * Solr site level config class.
 */
class Provision_Config_Tomcat_Site extends Provision_Config_Solr_Site {

  public $template = 'site.tpl.php';
  public $description = 'Tomcat webapp configuration file';

  function filename() {
    return $this->data['server']->solr_app_path . '/' . $this->uri . '.xml';
  }

  function write() {

    // Writes application xml file.
    parent::write();

    // Copy the tomcat solr.xml files in.
    $solr_home = $this->data['solr_home'];
    $path_to_solr_xml =  realpath(__DIR__ . '/../../../solr/solr.xml');
    if (!file_exists($path_to_solr_xml)) {
      return drush_set_error(DRUSH_FRAMEWORK_ERROR, 'solr.xml file not found at ' . $path_to_solr_xml);
    }
    drush_log(dt('Copying %solr_xml to SOLR_HOME', array('%solr_xml' => $path_to_solr_xml)), 'ok');
    drush_shell_exec("cp -rf $path_to_solr_xml $solr_home");
  }

  function unlink() {
    parent::unlink();
  }

  function process() {
    parent::process();
  }
}
