#include <iostream>
#include <string>
#include "armadillo"
#include <utility>
#include <vector>
using namespace std;
using namespace arma;
struct component{
    string name;
    int n1,n2;
    double val,init_val;
}temp;
vector<vector<component> > circuit(100);// Graph Representation of the circuit
//vector<pair<int,int>>capacitors;// Locations of capacitors in Graph 'circuit'
//vector<pair<int,int>>inductors;// Locations of capacitors in Graph 'circuit'

void SolveX(mat& C,mat& D,mat& G,mat& B,mat& A,mat& X,vec& Z){//Solve vector of unknowns
    A.zeros(0,0);
    C = trans(B);
    A.insert_rows(0,G);
    A.insert_cols(A.n_cols,B);
    C.insert_cols(C.n_cols,D);
    A.insert_rows(A.n_rows,C);
    X = solve( A, Z );
}
void printRow(vec X , int n,int v,int L,float t){//print a row in the table of output
    printf("\n%.1f\t ",t);
    for(int i =1;i<=n;i++)
        printf("| %f ",X(i-1));
    for(int i =1;i<=v+L;i++)
        printf("| %f ",X(n+i-1));
}
int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    /********************Read input & create the circuit Graph********************/
    int n=0,m=0,v=0,cur=0,L=0,cap=0,l=0,cp=0,cpj=0,vv=0;//# of nodes & # of voltage source & # of current source
    while(cin>>temp.name>>temp.n1>>temp.n2>>temp.val>>temp.init_val){
        circuit[temp.n1].push_back(temp);// Creating the circuit Graph
        if(temp.name.compare("Vsrc")==0)
            m++;    //# of voltage source
        else if(temp.name.compare("I")==0)
            L++;    //# of inductors
        else if(temp.name.compare("C")==0)
            cap++;  //# of capacitors
        n = max(n,max(temp.n1,temp.n2));//# of nodes
    }

    /******************************Create Matrix A********************************/

    mat G(n,n),B(n,m+L),C(m+L,n),D(m+L,m+L),X,A(0,0);
    vec Z=zeros(n+m+L);
    G.zeros(n,n);
    B.zeros(n,m+L);
    C.zeros(m+L,n);
    D.zeros(m+L,m+L);
    // Traversing the circuit Graph
    printf("The circuit Graph representation (node,value):\n");
    for(int i=0;i<=n;i++){
        printf("node%d",i);//node # i
        for(int j=0;j<circuit[i].size();j++){
            if(circuit[i][j].name.compare("R")==0 ){//Case Resistance
                if(i!=0)
                    G(i-1,i-1)+=(1/circuit[i][j].val);
                if(circuit[i][j].n2!=0){
                    if(i!=0)
                        G(i-1,circuit[i][j].n2-1)=G(circuit[i][j].n2-1,i-1)-=(1/circuit[i][j].val);
                    G(circuit[i][j].n2-1,circuit[i][j].n2-1)+=(1/circuit[i][j].val);
                }
            }
            else if(circuit[i][j].name.compare("Vsrc")==0){//Case Voltage source
                if( i!=0)
                    B(i-1,v)=1;
                if(circuit[i][j].n2!=0)
                    B(circuit[i][j].n2-1,v)=-1;
                Z(n+v)=circuit[i][j].val;
                v++;
            }
            else if(circuit[i][j].name.compare("Isrc")==0){//Case Current source
                if(i!=0)
                    Z(i-1)+=circuit[i][j].val;//enters
                if(circuit[i][j].n2!=0)
                    Z(circuit[i][j].n2-1)-=circuit[i][j].val;//leave
                cur++;
            }
            else if(circuit[i][j].name.compare("C")==0){//Case Capacitor
                if(i!=0){//Source node
                    G(i-1,i-1)+=(circuit[i][j].val/0.1); //Resitance part for diagonals
                    Z(i-1)+=((circuit[i][j].val)/0.1)*circuit[i][j].init_val;//Current part: enters
                }
                if(circuit[i][j].n2!=0){//Destination node
                    if(i!=0)
                        G(i-1,circuit[i][j].n2-1)=G(circuit[i][j].n2-1,i-1)-=(circuit[i][j].val/0.1);
                    G(circuit[i][j].n2-1,circuit[i][j].n2-1)+=(circuit[i][j].val/0.1);//Resitance part for diagonals
                    Z(circuit[i][j].n2-1)-=((circuit[i][j].val)/0.1)*circuit[i][j].init_val;//leave
                }
                //capacitors.push_back(make_pair(i,j));

            }
            else if(circuit[i][j].name.compare("I")==0){//Case Inductor
                //Assume no resistance
                //voltage part in B
                if( i!=0)
                    B(i-1,m+l)=1;

                if(circuit[i][j].n2!=0)
                    B(circuit[i][j].n2-1,m+l)=-1;
                // Diagonal in D = -L/h
                D(m+l,m+l)=circuit[i][j].val/-0.1;
                //voltage part in Z
                Z(n+m+l)= ((circuit[i][j].val)/0.1)*circuit[i][j].init_val;
                //inductors.push_back(make_pair(i,j));
                l++;
            }
            printf(" -> (%d,%.2f)",circuit[i][j].n2,circuit[i][j].val);
        }
    printf("\n");
    }

    SolveX(C,D,G, B, A, X,Z);
    //A.print("A:");
    //Z.print("Z:");
    //X.print("X:");
    //printf("\n");

    /*********Output table format***********/

    //Print table columns' names e.g. V0 V1 Ivsrc
    printf("\nt\t ");
    for(int i =1;i<=n;i++)
        printf("|     V%d   ",i);
    for(int i =1;i<=v+L;i++)
        printf("|   I%d  ",i);

    float t=0.0;
    //print first row (initial value at t=0)
    printRow(X,n,v,L,t);

    if(L||cap){//if there exist any inductors or any capacitors check effect of time
        for(t =0.1;t<2.1;t+=0.1){//iterate through time
            Z=zeros(n+m+L);
            for(int i=0;i<=n;i++){//iterate the graph
                v=cur=0,l=0;
                for(int j=0;j<circuit[i].size();j++){
                    if(circuit[i][j].name.compare("Vsrc")==0){//Case Voltage source
                        Z(n+v)=circuit[i][j].val;
                        v++;
                    }
                    else if(circuit[i][j].name.compare("Isrc")==0){//Case Current source
                        if(i!=0)
                            Z(i-1)+=circuit[i][j].val;//enters
                        if(circuit[i][j].n2!=0)
                            Z(circuit[i][j].n2-1)-=circuit[i][j].val;//leave
                        cur++;
                    }
                    else if(circuit[i][j].name.compare("C")==0){//Case Capacitor
                        if(i!=0 && circuit[i][j].n2!=0){//Source node
                            //Z(i-1)+=(circuit[i][j].val/0.1)*(X(n+m+l));
                            Z(i-1)+=((circuit[i][j].val)/0.1)*(X(circuit[i][j].n1-1)-X(circuit[i][j].n2-1));//enters
                            Z(circuit[i][j].n2-1)-=((circuit[i][j].val)/0.1)*(X(circuit[i][j].n1-1)-X(circuit[i][j].n2-1));//leaves
                        }
                        if(i==0)
                            Z(circuit[i][j].n2-1)-=((circuit[i][j].val)/0.1)*(0-X(circuit[i][j].n2-1));//leaves
                        else if(circuit[i][j].n2==0)
                            Z(i-1)+=(circuit[i][j].val/0.1)*(X(circuit[i][j].n1-1)-0);
                    }
                    else if(circuit[i][j].name.compare("I")==0){//Case Inductor
                        //Assume no resistance
                        //voltage part in B
                        if( i!=0)
                            B(i-1,v+l)=1;

                        if(circuit[i][j].n2!=0)
                            B(circuit[i][j].n2-1,v+l)=-1;
                        // Diagonal in D = -L/h
                        D(m+l,m+l)=circuit[i][j].val/-0.1;
                        //voltage part in Z
                        Z(n+m+l)-= ((circuit[i][j].val)/0.1)*X(n+m+l);
                        l++;

                    }
                }
            }
            SolveX(C,D,G, B, A, X,Z);//Solve New matrix
            printRow(X,n,v,L,t);

        }
        return 0;
    }

    return 0;
}
