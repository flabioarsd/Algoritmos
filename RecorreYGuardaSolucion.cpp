/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Flavio
 *
 * Created on 6 de septiembre de 2024, 07:18 PM
 */

#include <cstdlib>
#include<iostream>
#include<iomanip>
#define N 5
using namespace std;
int pedidoPesos[N]{5,1,50,8};
char pedidoTipo[N]{'A','V','M','A'};

bool seAtendioTodo(int resulPesos[N][N],char resulProducto[N][N]){
    int sum=0,sum2=0;
    for(int i=0;i<N;i++)sum+=pedidoPesos[i];
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)sum2+=resulPesos[i][j];
    if(sum==sum2)return true;
    else return false;
}
bool esElProducto(char tipo,int peso){
    for(int i=0;pedidoPesos[i];i++){
        if(tipo==pedidoTipo[i] and peso==pedidoPesos[i]){
            return true;
        }
    }
    return false;
}
bool    resuelvePedido(int x,int y,int pesos[N][N],char productos[N][N],
                       int resulPesos[][N],char resulProducto[][N]){
    
    if(x==N-1 and y==N-1){
        if(seAtendioTodo(resulPesos,resulProducto))return true;
        else return false;
    }
    //movimientos: (1,0) o (0,1)
    int mov[2][2]{{1,0},{0,1}};
    for(int i=0;i<2;i++){
        int newX=x+mov[i][0];
        int newY=y+mov[i][1];
        if(newX<N and newX>=0 and newY<N and newY>=0){
            cout<<productos[newX][newY]<<'-'<<pesos[newX][newY]<<endl;
            if(esElProducto(productos[newX][newY],pesos[newX][newY])){
                resulPesos[newX][newY]=pesos[newX][newY];
                resulProducto[newX][newY]=productos[newX][newY];
            }
                if(resuelvePedido(newX,newY,pesos,productos,resulPesos,resulProducto)){
                    return true;
                }else{
                    resulPesos[newX][newY]=0;
                    resulProducto[newX][newY]=0;
                }
        }
    }
    return false;
}
void imprimePedido(int resulPesos[N][N],char resulProducto[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout<<setw(4)<<resulPesos[i][j]<<resulProducto[i][j];
        }
        cout<<endl;
    }
}
int main(int argc, char** argv) {
    int pesos[N][N]{{20,10,10,25,10},
                     {10,1,10,5,15},
                     {1,20,50,5,1},
                     {25,10,8,5,15},
                     {10,2,8,10,10}};
    int resulPesos[N][N]{};
    char resulProducto[N][N]{};
    
    char productos[N][N]{{'A','V','A','M','M'},
                        {'A','V','M','A','V'},
                        {'V','M','M','M','A'},
                        {'V','A','A','A','V'},
                        {'M','M','M','M','A'}};
    
    if(resuelvePedido(0,0,pesos,productos,resulPesos,resulProducto)){
        cout<<"Es posible resolver el pedido"<<endl;
        imprimePedido(resulPesos,resulProducto);
    }else{
        cout<<"No fue posible resolver el pedido"<<endl;
        imprimePedido(resulPesos,resulProducto);
    }
    
    return 0;
}

