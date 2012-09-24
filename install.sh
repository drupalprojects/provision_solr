#!/bin/bash

#Install tomcat6
apt-get install tomcat6 tomcat6-admin tomcat6-common tomcat6-user

#Download and extract latest tested solr instance
cd /tmp
wget http://apache.osuosl.org/lucene/solr/3.6.1/apache-solr-3.6.1.tgz
tar -xvzf apache-solr-3.6.1.tgz

#Create a shared folder for solr and move solr.war into it.
mkdir /usr/share/tomcat6/solr
mkdir /usr/share/tomcat6/webapps
cp apache-solr-3.6.1/example/webapps/solr.war /usr/share/webapps/solr.war
cp -r apache-solr.3.6.1/example/solr /usr/share/tomcat6/solr

#Change own
chown -R tomcat6 /usr/share/tomcat6

#Add tomcat6 to aegir group, so she can read aegir owned files
usermod tomcat6 -G tomcat6,aegir

#Add aegir to tomcat6 group.
usermod aegir -G aegir,tomcat6,www-data

#Give aegir ownership of Catalina's localhost folder, so aegir can later write symlinks
chown aegir /etc/tomcat6/Catalina/localhost

#Allow aegir to restart tomcat6 without a password
#@TODO: Is this OPTIONAL? If conifgured to autopull, no need to restart tomcat6
echo 'aegir ALL=NOPASSWD: /etc/init.d/tomcat6' >> /etc/sudoers.d/aegir