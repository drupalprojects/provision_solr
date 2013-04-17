Hosting Solr
============

Provides a Solr service with a Tomcat implementation for the Aegir Hosting system.

Create a server and enable Solr.  Then create a site and enter the path to Solr config
files for that site's solr instance.

Requirements
------------

Hosting Solr is built for Aegir 1.x only.  Upgrade to 2.x coming soon.

Hosting Solr requires provision_solr.  For more info see http://drupal.org/project/provision_solr
...or just download it:

    $ drush dl provision_solr
    

Installation
------------

To install, follow these instructions:

1. $ sudo apt-get install tomcat7
2. Download apachesolr-3.6.2 (or your desired version) from ...
3. Copy examples/webapp/solr.war from the apachesolr package ...

Coming soon! 




Sponsored by thinkdrop.net

Based on based on the initial code: https://github.com/EugenMayer/aegir_solr_service
