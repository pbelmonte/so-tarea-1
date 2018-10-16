#include <stdlib.h>
#include <stdio.h>
#include <nSystem.h>
#include "transbordo.h"

int *enPargua; // arreglo de barcos en pargua
int *enChacao; // arreglo de barcos en chacao
int buff_size; // tamaño de los arreglos
int barcosEnPargua, barcosEnChacao; // cantidad de barcos en cada lado
int nextEmptyPargua, nextFullPargua, nextEmptyChacao, nextFullChacao; // punteros para recorrer los arreglos
nMonitor m;

void inicializar(int p) {
	enPargua = nMalloc(sizeof(int) * p);
	enChacao = nMalloc(sizeof(int) * p);
	buff_size = p;
	barcosEnPargua = p;
	barcosEnChacao = 0;
	nextEmptyPargua = 0;
	nextFullPargua = 0;
	nextEmptyChacao = 0;
	nextFullChacao = 0;
	m = nMakeMonitor();
	for (int i = 0; i < p; ++i)	{
		enPargua[i] = i;
	}
}

void finalizar() {
	nFree(enPargua);
	nFree(enChacao);
	nDestroyMonitor(m);
}

// obtener un transbordador en Pargua
int getPargua() {
	int x;
	nEnter(m);
	while (barcosEnPargua == 0)
		nWait(m);
	x = enPargua[nextFullPargua];
	nextFullPargua = (nextFullPargua + 1) % buff_size;
	barcosEnPargua--;
	nNotifyAll(m);
	nExit(m);
	return x;
}

// estacionar un transbordador en Pargua
void putPargua(int x) {
	nEnter(m);
	while (barcosEnPargua == buff_size)
		nWait(m);
	enPargua[nextEmptyPargua] = x;
	nextEmptyPargua = (nextEmptyPargua + 1) % buff_size;
	barcosEnPargua++;
	nNotifyAll(m);
	nExit(m);
}

// obtener un transbordador en Chacao
int getChacao() {
	int x;
	nEnter(m);
	while (barcosEnChacao == 0)
		nWait(m);
	x = enChacao[nextFullChacao];
	nextFullChacao = (nextFullChacao + 1) % buff_size;
	barcosEnChacao--;
	nNotifyAll(m);
	nExit(m);
	return x;
}

// estacionar un transbordador en Chacao
void putChacao(int x) {
	nEnter(m);
	while (barcosEnChacao == buff_size)
		nWait(m);
	enChacao[nextEmptyChacao] = x;
	nextEmptyChacao = (nextEmptyChacao + 1) % buff_size;
	barcosEnChacao++;
	nNotifyAll(m);
	nExit(m);
}

void transbordoAChacao(int v) {
	if (barcosEnChacao == buff_size) // si estan todos los barcos en Chacao
		transbordoAPargua(-1); // pido un transbordador
	int transbordador = getPargua();
	haciaChacao(transbordador, v);
	putChacao(transbordador);
}

void transbordoAPargua(int v) {
	if (barcosEnPargua == buff_size) // si estan todos los barcos en Pargua
		transbordoAChacao(-1); // pido un transbordador
	int transbordador = getChacao();
	haciaPargua(transbordador, v);
	putPargua(transbordador);
}