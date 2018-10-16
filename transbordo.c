#include <stdlib.h>
#include <nSystem.h>

nMonitor m;
int *enPargua;

void inicializar(int p) {
	enPargua = malloc(sizeof(int) * p);
	for (int i = 0; i < p; ++i)	{
		enPargua[i] = TRUE;
	}
	m = nMakeMonitor();
}

void finalizar() {
	free(enPargua);
	nDestroyMonitor(m);
}

void transbordoAChacao(int v) {

}

void transbordoAPargua(int v) {

}