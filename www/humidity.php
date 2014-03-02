<?php
require_once('db.php');
require_once('jpgraph/jpgraph.php');
require_once('jpgraph/jpgraph_scatter.php');
require_once('jpgraph/jpgraph_mgraph.php');
require_once('jpgraph/jpgraph_plotline.php');
require_once('jpgraph/jpgraph_log.php');

$tlim=24;

$dbh=dbConnect();
try {
  //
  $qry="SELECT (UNIX_TIMESTAMP(tstamp)-UNIX_TIMESTAMP(UTC_TIMESTAMP()))/3600.0 AS trel, humidity FROM sensor_log HAVING trel>-$tlim"; 
  $res=$dbh->query($qry);
  while(($row=$res->fetch(PDO::FETCH_ASSOC))!==false) {
    $trel[]=$row["trel"];
    $humidity[]=$row["humidity"];
  }

  // Width and height of the graph
  $timer=new JpgTimer;
  $timer->Push();
  $width=1200;
  $height=300;
  $mgraph=new MGraph();

  //
  $graph=new Graph($width, $height);
  $graph->SetScale("linlin",0,100,-$tlim,0);
  $graph->xaxis->SetPos('min');
  $graph->yaxis->SetPos('min');

  $aplot=new ScatterPlot($humidity, $trel);
  $aplot->mark->SetType(MARK_CROSS);
  $aplot->mark->SetSize(1);
  $aplot->mark->SetWidth(0);
  $aplot->mark->SetWeight(1);
  $aplot->mark->SetColor('black');
  $aplot->mark->SetFillColor('black');
  $graph->Add($aplot);

  $line=new PlotLine(VERTICAL, 880, "black", 1);

  //
  $mgraph->Add($graph, 0, 0);
  $mgraph->Stroke();
}
catch(PDOException $e) {
  print "Database error: ".$e->getMessage()."<br/>";
  die();
}

?>
