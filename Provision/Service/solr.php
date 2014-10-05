<?php

/**
 * The solr service class.
 */
class Provision_Service_solr extends Provision_Service {
  public $service = 'solr';

  /**
   * Add the civicrm_version property to the site context.
   */
  static function subscribe_site($context) {
    $context->setProperty('solr_server', '@server_master');
    $context->setProperty('solr_config_path', '');
    $context->setProperty('solr_war_path', '');
    $context->is_oid('solr_server');
    $context->service_subscribe('solr', $context->solr_server->name);
  }
}
