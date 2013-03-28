<Context docBase="<?php print $path_to_solr_war; ?>" debug="0" privileged="true" allowLinking="true" crossContext="true">
  <Environment name="solr/home" type="java.lang.String" value="<?php print $home_directory; ?>" override="true" />
  </Context>
