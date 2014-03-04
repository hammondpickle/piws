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
  $qry="SELECT (UNIX_TIMESTAMP(tstamp)-UNIX_TIMESTAMP(UTC_TIMESTAMP()))/3600.0 AS trel, ambient_temperature, sky_temperature, dewpoint FROM sensor_log HAVING trel>-$tlim"; 
  $res=$dbh->query($qry);
  while(($row=$res->fetch(PDO::FETCH_ASSOC))!==false) {
    $trel[]=$row["trel"];
    $ambtemp[]=$row["ambient_temperature"];
    $skytemp[]=$row["sky_temperature"];
    $dewpoint[]=$row["dewpoint"];
  }

  // Width and height of the graph
  $timer=new JpgTimer;
  $timer->Push();
  $width=1200;
  $height=300;
  $mgraph=new MGraph();

  //
  $graph=new Graph($width, $height);
  $graph->SetScale("linlin",0,0,-$tlim,0);
  $graph->xaxis->SetPos('min');
  $graph->yaxis->SetPos('min');

  $aplot=new ScatterPlot($ambtemp, $trel);
  $aplot->mark->SetType(MARK_CROSS);
  $aplot->mark->SetSize(1);
  $aplot->mark->SetWidth(0);
  $aplot->mark->SetWeight(1);
  $aplot->mark->SetColor('red');
  $aplot->mark->SetFillColor('red');
  $graph->Add($aplot);

  $splot=new ScatterPlot($skytemp, $trel);
  $splot->mark->SetType(MARK_CROSS);
  $splot->mark->SetSize(1);
  $splot->mark->SetWidth(0);
  $splot->mark->SetWeight(1);
  $splot->mark->SetColor('blue');
  $splot->mark->SetFillColor('blue');
  $graph->Add($splot);

  $dplot=new ScatterPlot($dewpoint, $trel);
  $dplot->mark->SetType(MARK_CROSS);
  $dplot->mark->SetSize(1);
  $dplot->mark->SetWidth(0);
  $dplot->mark->SetWeight(1);
  $dplot->mark->SetColor('forestgreen');
  $dplot->mark->SetFillColor('forestgreen');
  $graph->Add($dplot);

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
