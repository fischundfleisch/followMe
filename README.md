# followMe
# followMe

Roboter folgt einem Ultraschallsignal. 

Benötigt werden 4 Ultraschallsensoren und zwei Arduinos. 3 Sensoren werden an Bord des Roboters verbaut: Front, Links und Rechts. Bei ihnen wird neben Vin und GRD nur der Echo - Pin verlötet. Im Abstand von derzeit je einer Sekunde lauschen die dem Sendesignal. Trifft dieses zuerst an der Front und danach etwa gleich schnell links und rechts ein, ist der Sender geradeaus. 
Trifft das Signal jedoch zuerst Front und Rechts ein und erst später oder gar nicht links, dann ist der Sender vorne rechts, umgekehrt ist er vorne links. Entsprechend soll der Roboter lenken. 
