<?php

$dbHost="bismuth.local";
$dbUser="pi";
$dbPasswd="";
$dbName="weathermon";

function dbConnect() {
  global $dbHost, $dbUser, $dbName, $dbPasswd;
  try {
    $dbh=new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPasswd,
		 array(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY => false));
    $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    return $dbh;
  } catch(PDOException $e) {
    print "Error connecting to database: ".$e->getMessage()."<br/>";
    die();
  }
}

?>
