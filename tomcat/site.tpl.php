<!-- <?php print $comment; ?>-->
<Context docBase="<?php print $solr_war_path; ?>" debug="0" privileged="true" allowLinking="true" crossContext="true">
  <Environment name="solr/home" type="java.lang.String" value="<?php print $solr_home_path; ?>" override="true" />
</Context>
