#Union-Find-implementation

This project provide a direct view on how algorithom  Weighted-Quick-Union Find with Path Compression (WQUPC) will work.

Description of the app:
In a imaginary view, there is a big square which consist of N*N little square.
Every little square has a state opened or not opened,separately indicated by 1 and 0 in <Array gridOpenSet>
Every little square has a root,this can changes when it uinon with other little square.
Having the same root with other square means they are connected to each other.

at the begining the top edge of the big square are not connect to the bottom, with the amount of little square increaseing, they will connect to each other, and that's when the demo view stop to generate new square. 
  
<img src="https://github.com/BensonLaur/image-storage/raw/master/Union-Find-implementation%20project/demo1.png" width="70%" height="70%">
<img src="https://github.com/BensonLaur/image-storage/raw/master/Union-Find-implementation%20project/demo2.png" width="70%" height="70%">
