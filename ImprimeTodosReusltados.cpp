/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Flavio
 *
 * Created on 5 de septiembre de 2024, 07:28 PM
 */
#include <iostream>
#include <vector>

using namespace std;

void empaca(int i,int n,int peso,int *paq,vector<int>combinacion){
    if(i==n or peso<0) 
        return;
    if(paq[i]==peso){
        cout << "Combinacion que da el peso exacto: ";
        for (int j = 0; j < combinacion.size(); ++j)
            cout << combinacion[j] << " ";
        cout<<paq[i]<< endl;
        return ;
    }
    combinacion.push_back(paq[i]);
    empaca(i+1,n,peso-paq[i],paq,combinacion);
    combinacion.pop_back();
    empaca(i+1,n,peso,paq,combinacion);
}
int main(int argc, char** argv) {
    int n=5,peso=9;
    int paq[]={1,3,12,5,9};
    vector<int>combinacion;
    empaca(0,n,peso,paq,combinacion);
    return 0;
}


