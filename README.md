# followMe

Roboter folgt einem Ultraschallsignal. 

Benötigt werden 4 Ultraschallsensoren und zwei Arduinos. 3 Sensoren werden an Bord des Roboters verbaut: Front, Links und Rechts. Bei ihnen wird neben Vin und GRD nur der Echo - Pin verlötet. Im Abstand von derzeit je einer Sekunde lauschen die dem Sendesignal. Trifft dieses zuerst an der Front und danach etwa gleich schnell links und rechts ein, ist der Sender geradeaus. 
Trifft das Signal jedoch zuerst Front und Rechts ein und erst später oder gar nicht links, dann ist der Sender vorne rechts, umgekehrt ist er vorne links. Entsprechend soll der Roboter lenken. 
<p>
Nun, wir haben herausgefunden, dass am trigger eine Spannung anliegen muss, sonst schaltet der Echo nicht. Deshalb Trigger überkleben.
Wenn das Signal verloren wird, dann stoppt der Motor. 
Ein moving meridian sorgt für nicht ganz so abgehackte Bewegungen.
