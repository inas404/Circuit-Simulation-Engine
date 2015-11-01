# Circuit-Simulation-Engine
Computing nodal voltages and voltage source currents.

![Image of Circuit-Simulation-Engine](https://github.com/inas404/Circuit-Simulation-Engine/blob/master/arch.png)

Takes input as netlist format:  
Component_Type | Node1 | Node2 | Value | Initial_Value  
<br />
*Component_Type stands for the type of the component. It can be one of the following:  
 Voltage Source "Vsrc".  
 Current Source "Isrc".  
 Resistance "R".  
 Inductor "I".  
 Capacitor "C".  
<br />
*Node1 and Node2 stands for the nodes‟ numbers to which the component is connected to.  
*Value is the physical value of the component.  
*Initial_Value is the initial current or voltage that is observed on the component at time=0.  
<br />
Sample input (no transient):  
<br />
Vsrc 1 0 4 0  
Vsrc 3 2 6 0  
R 1 2 1 0  
R 2 0 4 0  
R 3 0 2 0  
Isrc 1 2 1 0  

![Image of Circuit-Simulation-Engine](https://github.com/inas404/Circuit-Simulation-Engine/blob/master/circuit1.png)
<br />
Sample output:  
<br />
The circuit Graph representation (node,value):
node0
node1 -> (0,4.00) -> (2,1.00) -> (2,1.00)
node2 -> (0,4.00)
node3 -> (2,6.00) -> (0,2.00)

t	 |     V1   |     V2   |     V3   |   I1  |   I2  
0.0	 | 4.000000 | -0.000000 | 6.000000 | -3.000000 | -3.000000 


Sample input (with transient):
'''
R 1 0 1 0
C 1 0 1 0
R 1 2 1 0
I 2 0 1 0
Isrc 2 0 1 0
'''
Sample output:
'''
The circuit Graph representation (node,value):
node0
node1 -> (0,1.00) -> (0,1.00) -> (2,1.00)
node2 -> (0,1.00) -> (0,1.00)

t	 |     V1   |     V2   |   I1  
0.0	 | 0.081967 | 0.983607 | 0.098361 
0.1	 | 0.147810 | 0.954045 | 0.193765 
0.2	 | 0.199356 | 0.914173 | 0.285182 
0.3	 | 0.238339 | 0.866506 | 0.371833 
0.4	 | 0.266385 | 0.813229 | 0.453156 
0.5	 | 0.285006 | 0.756228 | 0.528779 
0.6	 | 0.295597 | 0.697108 | 0.598489 
0.7	 | 0.299433 | 0.637221 | 0.662212 
0.8	 | 0.297668 | 0.577688 | 0.719980 
0.9	 | 0.291342 | 0.519419 | 0.771922 
1.0	 | 0.281380 | 0.463143 | 0.818237 
1.1	 | 0.268602 | 0.409423 | 0.859179 
1.2	 | 0.253725 | 0.358678 | 0.895047 
1.3	 | 0.237371 | 0.311204 | 0.926167 
1.4	 | 0.220075 | 0.267189 | 0.952886 
1.5	 | 0.202290 | 0.226731 | 0.975559 
1.6	 | 0.184396 | 0.189852 | 0.994544 
1.7	 | 0.166706 | 0.156511 | 1.010195 
1.8	 | 0.149473 | 0.126616 | 1.022857 
1.9	 | 0.132897 | 0.100037 | 1.032861 
2.0	 | 0.117132 | 0.076610 | 1.040522 
'''
