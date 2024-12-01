/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Flavio
 *
 * Created on 25 de noviembre de 2024, 10:13 AM
 */

#include <cstdlib>
#include <vector>
#include<iostream>
#include<ctime>
#include<algorithm>
#include <cmath>
#include <limits.h>
#include<fstream>
#define ITERACIONES 99999
#define alfa 0.5 // 0.7 
using namespace std;

typedef struct{
    int id_cliente;
    int posicion;
    int tIni;
    int tFin;
    int cantCajas;
    int distancia;
    bool emHora;
}Cliente;
typedef struct{
    int id_auto;
    int posicion;
    int velocidad;
    int maxCapacidad;
    int horaIngreso;
    int horaSalida;
    double tiempoServicio;
    int distanciaTotalRecorrida;
    vector<Cliente>listaPendientes;
    vector<Cliente>listaAtendidos;
}Auto;

typedef struct{
    Cliente cliente;
    int distancia;
}Nodo;

double fitCliente(Cliente c){
    /* Brindamos un 75% de prioridad en el tiempo de inicio
    y un 25% a la amplitud del tiempo en el que puede aceptar el pedido*/
    return (c.tIni*0.75 + (c.tFin-c.tIni)*0.25);
}
int fitAuto(Auto a){
    /* Lo ordenamos segun la velocidad*/
    return a.velocidad;
}

int verificaCliente(int c_rcl,vector<Cliente> it_clientes){
    int cont=0; 
    for(int i=0;i<it_clientes.size();i++){
        if(fitCliente(it_clientes[i])<=c_rcl) cont++;
        else break;
    }
    return cont;
}
int verificaAuto(int a_rcl,vector<Auto> it_auto){
    int cont=0; 
    for(int i=0;i<it_auto.size();i++){
        if(fitAuto(it_auto[i])>=a_rcl) cont++;
        else break;
    }
    return cont;
}
int verificaVecinos(int rcl,vector<Nodo>vecinos){
    int cont=0; 
    for(int i=0;i<vecinos.size();i++){
        if(vecinos[i].distancia<=rcl) cont++;
        else break;
    }
    return cont;
}

bool cmpCliente(Cliente a,Cliente b){
    /* Ordenamos los clientes por 
       en el que resiven el servicio. */
    return fitCliente(a)<fitCliente(b);
}
bool cmpAuto(Auto a, Auto b){
    /* Ordenamos de mayor a menor velocidad*/
    return fitAuto(a)>fitAuto(b);
}



bool   noHayEspacios(vector<Auto>it_autos,int cantCajas){
     for (int i = 0; i < it_autos.size(); i++) {
        if (cantCajas <= it_autos[i].maxCapacidad) {
            return false; // Hay al menos un auto con capacidad suficiente
        }
    }
    return true; // Ningún auto tiene capacidad suficiente
}
bool autoCompleto(int maxCap,vector<Cliente> cli){
    for(int i=0;i<cli.size();i++){
        if(maxCap>=cli[i].cantCajas){
            return false; //hay al menos un cliente que puede ser atendido por este auto
        }
    }
    return true; //El auto ya no tiene espacio para ningun cliente mas
}



void imprimeClientes(vector<Cliente>it_clietes){
    for(Cliente&cli : it_clietes){
        cout<<cli.id_cliente<<'-'<<cli.cantCajas<<endl;
    }
    cout<<"============"<<endl;
}
void imprimeAutos(vector<Auto>it_clietes){
    for(Auto&cli : it_clietes){
        cout<<"     "<<cli.id_auto<<'-'<<cli.maxCapacidad<<endl;
    }
    cout<<"============"<<endl;
}

void mostrarSolucion(vector<Auto>autos){
    for(Auto &au : autos){
        cout<<"Codigo del vehiculo: "<<au.id_auto<<"    distancia total recorrida: "<<au.distanciaTotalRecorrida<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"Lista de clientes Pendientes:"<<endl;
        for(Cliente &cli : au.listaPendientes){
            cout<<"Codigo de cliente: "<<cli.id_cliente<<endl;
        }
        if(au.listaPendientes.empty())cout<<"No hay clientes pendientes"<<endl;
        cout<<"------------------------------------------------------------------"<<endl;
       // cout<<"___________________________________________________________________"<<endl;
        cout<<"Lista de clientes Atendidos:"<<endl;
        for(Cliente &cli : au.listaAtendidos){
            cout<<"Codigo de cliente: "<<cli.id_cliente<<"     -";
            if(cli.emHora)cout<<"Cliente satisfecho"<<endl;
            else cout<<"Cliente insatisfecho"<<endl;
        }
        if(au.listaAtendidos.empty())cout<<"No se atendieron clientes"<<endl;
        cout<<"==================================================================="<<endl;
    }
}
vector<Auto> grasp(vector<Auto> autos, vector<Cliente> clientes,vector<vector<int> > mapa){
    vector<Auto> resultado;
        /* Ponemos la semilla */
        srand(time(NULL));
        /* Guardamos los vehiculos */
        vector<Auto> it_autos=autos;
        /* Guardamos y ordenamiento clientes a atender en el dia*/
        vector<Cliente> it_clietes=clientes;
        
        sort(it_clietes.begin(),it_clietes.end(),cmpCliente);
        sort(it_autos.begin(),it_autos.end(),cmpAuto);
        while(!it_clietes.empty() and !it_autos.empty()){
            /* Elejimos el cliente */
            int c_beta,c_tau,c_rcl,c_indrcl;
           // imprimeClientes(it_clietes);
            c_beta=fitCliente(it_clietes[0]);
            c_tau=fitCliente(it_clietes[it_clietes.size()-1]);
            c_rcl=round(c_beta+alfa*(c_tau-c_beta));
            int posRand=verificaCliente(c_rcl,it_clietes);
            if(posRand!=0)c_indrcl=rand()%posRand;
            else c_indrcl=0;
            /* Elejimos al auto */
            //imprimeAutos(it_autos);
            int a_beta,a_tau,a_rcl,a_indrcl;
            a_beta=fitAuto(it_autos[0]);
            a_tau=fitAuto(it_autos[it_autos.size()-1]);
            a_rcl=round(a_beta-alfa*(a_beta-a_tau));
            a_indrcl=rand()%verificaAuto(a_rcl,it_autos);
            /* Asignamos el cliente al auto */
            if(it_autos[a_indrcl].maxCapacidad>=it_clietes[c_indrcl].cantCajas){
                /* Eliminamos el cliente y actualizamos el auto */
                it_autos[a_indrcl].listaPendientes.push_back(it_clietes[c_indrcl]); //Agregamos el cliente al auto
                it_autos[a_indrcl].maxCapacidad-=it_clietes[c_indrcl].cantCajas; //reducimos la capacidad del auto
                it_clietes.erase(it_clietes.begin()+c_indrcl); //eliminamos el cliente elegido
            }    
            /*Cuando el auto ya esta lleno, lo guardamos en el resultado*/
            if(autoCompleto(it_autos[a_indrcl].maxCapacidad,it_clietes)){
                resultado.push_back(it_autos[a_indrcl]);
                it_autos.erase(it_autos.begin()+a_indrcl);
            }
        }
        /*Condicion para elegir al mejor (no hay manera de elegir la mejor solucion, hasta donde vi)*/
    return resultado;
}

int dijkstra(vector<vector<int>>mapa,int ini,int fin){
    int n = mapa.size(); // Número de nodos
    vector<int> dist(n, INT_MAX); // Distancias mínimas
    vector<bool> visitado(n, false); // Nodos visitados

    // Inicializamos la distancia al nodo inicial como 0
    dist[ini] = 0;
    for (int i = 0; i < n; i++) {
        // Encontrar el nodo no visitado con la menor distancia
        int nodoActual = -1;
        for (int j = 0; j < n; j++) {
            if (!visitado[j] && (nodoActual == -1 || dist[j] < dist[nodoActual])) {
                nodoActual = j;
            }
        }
        // Si no hay nodo accesible, terminamos
        if (dist[nodoActual]==INT_MAX) break;

        // Marcamos el nodo como visitado
        visitado[nodoActual]=true;

        // Actualizamos las distancias a los nodos vecinos
        for (int vecino = 0; vecino < n; vecino++) {
            if (mapa[nodoActual][vecino] != 0 && !visitado[vecino]) {
                int nuevaDistancia = dist[nodoActual] + mapa[nodoActual][vecino];
                if (nuevaDistancia < dist[vecino]) {
                    dist[vecino] = nuevaDistancia;
                }
            }
        }
    }
    // Devolver la distancia al nodo destino, o -1 si es inaccesible
    return (dist[fin] == INT_MAX) ? -1 : dist[fin];
}
bool    ordenaVecinos(Nodo&a,Nodo&b){
    return a.distancia<b.distancia;
}
void procesaCliente(Auto &autos,Cliente cli,double &hora){
    for(int i=0;i<autos.listaPendientes.size();i++){
        if(cli.id_cliente==autos.listaPendientes[i].id_cliente){
            if(hora<=cli.tIni)hora=(double)cli.tIni+autos.tiempoServicio;
            if(cli.tFin>=hora)autos.listaPendientes[i].emHora=true;
            else autos.listaPendientes[i].emHora=false;
            autos.listaAtendidos.push_back(autos.listaPendientes[i]);
            autos.listaPendientes.erase(autos.listaPendientes.begin()+i);
            break;
        }
    }
}
void graspRutaMinima(Auto &autos,vector<vector<int>>mapa){
    //para seleccionar los vecinos uso dijktra
    Auto mejorRespuesta;
    int id=autos.id_auto;
    double horadeldia=(double)autos.horaIngreso;
    int mejordistanciaRecorrida=INT_MAX;
    for(int i=0;i<ITERACIONES;i++){
        Auto auxAuto=autos;
        int distanciaRecorrida;
        while(not auxAuto.listaPendientes.empty() and horadeldia<=auxAuto.horaSalida){
            vector<Nodo>vecinos;
            for(int i=0;i<auxAuto.listaPendientes.size();i++){
                Nodo aux;
                int minimadistancia=dijkstra(mapa,auxAuto.posicion,auxAuto.listaPendientes[i].posicion);
                aux.cliente=auxAuto.listaPendientes[i];
                aux.distancia=minimadistancia;
                vecinos.push_back(aux);
            }
            if(vecinos.empty())break; //no hay mas a donde ir
            sort(vecinos.begin(),vecinos.end(),ordenaVecinos);
            int beta,tao,indrcl,posRand,rcl;
            beta=vecinos[0].distancia;
            tao=vecinos[vecinos.size()-1].distancia;
            rcl=round(beta+alfa*(tao-beta));
            posRand=verificaVecinos(rcl,vecinos);
            indrcl=rand()%posRand;
            //Procesas el vecino osea un cliente, luego con este procesado lo verificas en la lista y lo quitas
            distanciaRecorrida=vecinos[indrcl].distancia*2; //ida y vuelta
            horadeldia+=(double)vecinos[indrcl].distancia/(double)auxAuto.velocidad;
            auxAuto.distanciaTotalRecorrida+=distanciaRecorrida;
            auxAuto.posicion=vecinos[indrcl].cliente.posicion;
            procesaCliente(auxAuto,vecinos[indrcl].cliente,horadeldia);
        }
        if(distanciaRecorrida<mejordistanciaRecorrida){
            mejordistanciaRecorrida=distanciaRecorrida;
            mejorRespuesta=auxAuto;
        }
    }
    autos=mejorRespuesta;
}
void resolucionRutas(vector<Auto>&autos,vector<vector<int>>mapa){
    for(Auto&recorrido : autos){
        graspRutaMinima(recorrido,mapa);
    }
}
void lectura(vector<Auto>&autos,vector<Cliente>&clientes,vector<vector<int>>&mapa){
    ifstream archAutos("auto.txt",ios::in);
    ifstream archClientes("cliente.txt",ios::in);
    ifstream archMapa("mapa.txt",ios::in);
    int id,pos,tIni,tFin,cantCajas,dist;
    char car;
    while(true){
        Cliente cli;
        archClientes>>id;
        if(archClientes.eof())break;
        archClientes>>car>>pos>>car>>tIni>>car>>tFin>>car>>cantCajas;
        cli.id_cliente=id;
        cli.cantCajas=cantCajas;
        cli.posicion=pos;
        cli.tIni=tIni;
        cli.tFin=tFin;
        clientes.push_back(cli);
    }
    int velocidad,maxCapacidad,hIng,hSal;
    double serv;
    while(true){
        Auto aut;
        archAutos>>id;
        if(archAutos.eof())break;
        archAutos>>car>>pos>>car>>velocidad>>car>>maxCapacidad>>car>>hIng>>car>>hSal>>car>>serv;
        aut.distanciaTotalRecorrida=0;
        aut.horaIngreso=hIng;
        aut.horaSalida=hSal;
        aut.id_auto=id;
        aut.maxCapacidad=maxCapacidad;
        aut.posicion=pos;
        aut.velocidad=velocidad;
        aut.tiempoServicio=serv;
        autos.push_back(aut);
    }
    int a,b,c,d,e,f,g,h;
    char aux;
    while(true){
        vector<int> line;
        archMapa>>a;
        if(archMapa.eof())break;
        archMapa>>car>>b>>car>>c>>car>>d>>car>>e>>car>>f>>car>>g>>car>>h;
        line.push_back(a);
        line.push_back(b);
        line.push_back(c);
        line.push_back(d);
        line.push_back(e);
        line.push_back(f);
        line.push_back(g);
        line.push_back(h);
        mapa.push_back(line);
    }
}
int main(void){
    /* Problema */
    /* Definimos que A: El lugar del almacen en donde salen los autos.
        Los distintos lugares seran A=0,B=1,C=2, ... .*/
    vector<Auto> autos;
    vector<Cliente> clientes;
    vector<vector<int>> mapa;
    
    lectura(autos,clientes,mapa);
    
    /* Se muestra el algoritmo grasp */
    autos=grasp(autos,clientes,mapa);
    resolucionRutas(autos,mapa);
    mostrarSolucion(autos);
    return 0;
}