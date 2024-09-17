
#include<iostream>
#include<iomanip>
#include <cstdlib>
#include<vector>
#include<list>
using namespace std;
#define N 8

/*
 * 
 */

 

void imprimeTablero(vector<vector<char>>tablero){
    for(int i=0;i<tablero.size();i++){
        for(int j=0;j<tablero.size();j++){
            cout<<tablero[i][j]<<' ';
        }
        cout<<endl;
    }
}
bool recorridoCaballo(int x,int y,int numMovimientos,int movimientosPosibles[N][2],
                        vector<vector<char>>&tablero){
    if(numMovimientos==N*N)return true;
    for(int i=0;i<8;++i){
        int newX=x+movimientosPosibles[i][0];
        int newY=y+movimientosPosibles[i][1];
        if(newX>=0 and newY>=0 and newX<8 and newY<8 and tablero[newX][newY]=='_'){
            tablero[newX][newY]='C';
//            numMovimientos++;
            if(recorridoCaballo(newX,newY,numMovimientos+1,movimientosPosibles,tablero)){
                cout<<"Recorrido: "<<newX<<'-'<<newY<<endl;
                return true;
            }else{
                tablero[newX][newY]='_';
                //numMovimientos--;
            }
        }
    }
    return false;
    
}
int main(int argc, char** argv) {
    int movimientosPosibles[N][2]={{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2}
                                  ,{1,-2},{2,-1}};
    vector<vector<char>>tablero(N,vector<char>(N,'_'));
    cout<<"Tablero inicial: "<<endl;
    imprimeTablero(tablero);
    tablero[0][0]='C';
    if(recorridoCaballo(0,0,1,movimientosPosibles,tablero)){
        cout<<"Se logro recorrer todo"<<endl;
        imprimeTablero(tablero);
    }else{
        cout<<"No se logro recorrer todo"<<endl;
        imprimeTablero(tablero);
    }
    return 0;
}

