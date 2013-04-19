Aegir Apache Solr
=================

This tool gives Aegir the power to manage Solr servers and provision a Solr web
app for any Drupal site. It includes a Tomcat implementation but can be extended
to work with Jetty, etc. Patches welcome.

Provision Solr is the backend, Hosting Solr is the front-end.

Architecture
============

Solr is the service, Tomcat and Jetty are the servers. Just like HTTP is the
service, and Apache and NGINX are the servers.

All of the Solr-specific code is in solr.service.inc and solr.config.inc. All of
the tomcat code (and config template) is separated into the folder tomcat.

Same goes with the configuration. Solr home directories are in
~/config/server_SERVER/solr/SITE and the Tomcat application XML files go in
~/config/server_SERVER/tomcat/SITE.xml.

Then, once the server and site are verified, there will be a solr server
available at http://SERVER_NAME:8080/SITENAME

*NOTE: These Solr webapps have no security on them by default, and we don't yet
have automatic security enforcement. You can still customize your Tomcat or
Jetty as normal. Please use caution.*

**TODO: It shouldn't be too hard to build an SSL version of this
ProvisionService. We modeled this on the HTTP portion of Provision, creating a
provisionService_solr_public, so theoretically, a provisionService_solr_private
should be possible. PATCHES ENCOURAGED**

Aegir Servers
-------------

Aegir server entities now have solr_port, solr_restart_cmd, solr_app_path,
solr_war_path, and solr_homes_path properties.

When you install Tomcat and Solr, you will symlink Tomcat's Catalina/localhost
to ~/config/tomcat.

Then, on verification of server_NAME, Provision symlinks ~/config/tomcat to
~/config/server_NAME/tomcat.

Everything else is Aegir Site specific.

Aegir Sites
-----------

After tons of research I've determined that the Multiple Solr Webapps method is
the best and easiest way to get "many solr databases" on one server.

This has many benefits: each site gets a completely independent solr home
folder, each server can use a different WAR file, enabling multiple versions of
solr on one physical server. And, you could create more Solr server types,
adding Jetty for instance, and easily choose which server runs what.

Each site gets it's own independent Tomcat application, defined by an XML file,
which is written to ~/config/tomcat as a provisionConfig template on site
verification.

Each site gets a SOLR_HOME folder of its own located at
~/config/server_SERVER/solr/SITE. So for your site mysolrpowr.com, on
server_master, you get a SOLR_HOME folder at
/var/aegir/config/server_master/solr/mysolrpower.com.

This Solr home directory contains solr.xml, and folders for conf and data.
@TODO: We should probably move data out of there. Tomcat7 writes to this folder,
and then the aegir user cannot delete that data.

Each site entity stores a new property: solr_config_path. This is the relative
path within the site to the Solr conf files.

For example: sites/all/modules/apachesolr/solr-conf/solr-3.x. The files in
solr_config_path are copied to the SOLR_HOME/conf folder.

This allows you to store the Solrconfig.xml, schema.xml, and other Solr config
files in your Drupal codebase. This means each site on your Aegir instance can
manipulate Solr config however they want.

When a site is verified, the files in the Drupal tree's solr_config_path are
copied to the solr_home path

Installation
============

See INSTALL.txt

Credits
=======

Hosting & Provision Solr was built by the team at ThinkDrop Consulting.

It was inspired by https://github.com/EugenMayer/aegir_solr_service.