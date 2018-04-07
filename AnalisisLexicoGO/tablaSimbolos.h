////////////////////////////////////////////////////////////////
typedef char* tipoelem;
typedef char* tipoclave;
////////////////////////////////////////////////////////////////
		
typedef void * tabla; //tipo opaco

void crea(tabla *A);
void destruye(tabla *A);

int busca(tabla *A, tipoelem nombre);
void creaNodo(tabla *A, tipoelem nombre, int codigo);

tipoelem getLexema(tabla A);
int getCodigo(tabla A);

void imprimir(tabla A);

