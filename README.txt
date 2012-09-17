 Guide of installation on linux
 
 1. Install tomcat6
 ==============================
 aptitude install tomcat6 tomcat6-admin tomcat6-common tomcat6-user
 
 2.- Configure tomcat6
 ==============================
 Now is need add two user for use tomcat. Is need edit the file /etc/tomcat6/tomcat-users.xml. One example is:
 <tomcat-users>
   <role rolemane="admin"/>
   <role rolename="tomcat"/>
   <role rolename="manager-gui"/>
   <role rolename="manager-script"/>
   <user username="tomcat" password="s3cret" roles="manager-gui,admin"/>
   <user username="normal_user" password="normal_user" roles="manager-script"/>
 </tomcat-users>
 
 NOTE: When we change this file is need reload tomcat with /etc/init.d/tomcat6 restart

3.- Edit file tomcat for read configuration aegir
==================================================
Edit this file /etc/tomcat6/server.xml and replace this line:
<Host name="localhost"  appBase="webapps">
for this
<Host name="localhost"  appBase="/var/aegir/config/solr">

 
 4.- Copy the commands to drush folder
 ====================================
 Copy the folder provision/solr to drush folder  
 
 5.- Enable module
 =====================================
 Go to admin/build/modules for enable module and on the sites node you can see one check for provide solr server. If you enable this checkbox in the page node you can see the information for configure solr in your drupal.                  
