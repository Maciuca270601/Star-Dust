#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/*Maciuca Alexandru Petru 314CA*/

/*
Alocam un vector care sa retina numarul de elemente
de pe fiecare linie.
*/

int *alloc_vector(int n)
{
	int *v = (int *)malloc(n * sizeof(int));
	if (!v) {
		printf("Nu s-a putut aloca memoria");
		return NULL;
	}
	return v;
}

/*
Aloc matricea in concordanta cu valorile din vector
si deoarece pentru a ajunge la urmatorul v[i] tb sa citesc si
elementele din matrice , voi realiza si citirea tot aici.
*/
int **alloc_matrix(int n, int *vector)
{
	int x, nr; // x = mi , nr = aij
	int **mat = (int **)malloc(n * sizeof(int *));
	if (!mat) {
		printf("Nu s-a putut aloca memoria");
		free(mat);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		scanf("%d", &x);
		mat[i] = (int *)malloc(x * sizeof(int *));
		if (!mat[i]) {
			printf("Nu s-a putut aloca memoria");
			while (--i > 0)
				free(mat[i]);
			free(mat);
			return NULL;
		}
/*
v[i] imi pastreaza numarul de elemente de pe linia i
mat[i][j] = nr pe care il citesc de la tastatura/
*/
		vector[i] = x;
		for (int j = 0; j < x; j++) {
			scanf("%X", &nr);
			mat[i][j] = nr;
		}
	}
	return mat;
}

double get_shield(int n, int **a, int *v)
{
	double m = 0;// media e de tip double pt a pastra zecimale.
	char char0, char1, char2, char3;
/*
Convertesc a[i][j] la char pt a imparti numarul in 4 grupuri.
ex: 00010203 -> 03 02 01 00
Aplicam o masca pt a pastra fix numerele de care avem nevoie restul devenind 0
,din cauza & si apoi shiftam pentru ca variabilele car0, car1, car2, car3
sa retina grupul0(03), grupul1(02), grupul2(01), grupul3(00).
*/
	for (int j = 0; j < v[0]; j++) {
		char0 = ((a[0][j] & 0xFF000000) >> (8 * 3));
		char1 = ((a[0][j] & 0x00FF0000) >> (8 * 2));
		char2 = ((a[0][j] & 0x0000FF00) >> (8 * 1));
		char3 = ((a[0][j] & 0x000000FF));
		m += (double)char0 + (double)char1 + (double)char2 + (double)char3;
		}
	for (int j = 0; j < v[n - 1]; j++) {
		char0 = ((a[n - 1][j] & 0xFF000000) >> (8 * 3));
		char1 = ((a[n - 1][j] & 0x00FF0000) >> (8 * 2));
		char2 = ((a[n - 1][j] & 0x0000FF00) >> (8 * 1));
		char3 = ((a[n - 1][j] & 0x000000FF));
		m += (double)char0 + (double)char1 + (double)char2 + (double)char3;
		}
/*
Pe linia 0 si n-1 tb adunate toate grupurile, insa pe restul avem nevoie
doar de primul si ultimul.
Citesc: 00000001 dar in harta el apare 01 00 00 00(pe dos).
Asadar pt 00000001: 0xFF000000->00(adica coada)
					0x000000FF->01(adica inceputul)
*/
	for (int i = 1; i < n - 1; i++) {
		char1 = ((a[i][v[i] - 1] & 0xFF000000) >> (8 * 3));//coada
		char2 = (a[i][0] & 0x000000FF);//inceput
		m += (double)char1 + (double)char2;
	}
/*
Pentru a numara suprafetele nu este nevoie sa ne complicam:
Primul si ultimul rand vor contine 4grupuri pentru fiecare numar =>
4*v[0] + 4*v[n-1]
,iar apoi fiecare linie va contine un grup de inceput
si un grup de final care pot proveni de la primul respectiv ultimul numar
sau daca este unul singur de la acelasi numar =>
2*(n-2).
*/
	int count = 0;
	count = count + 4 * v[0] + 4 * v[n - 1] + (n - 2) * 2;
	m = m / count;
	return m;
}

void print_matrix(int n, int **a, int *v)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < v[i]; j++)
			printf("%.8X ", a[i][j]);
		printf("\n");
	}
}

void swap_matrix(int **a)
{
	char data_type;
	int line, group;
	scanf(" %c", &data_type);
	scanf("%d", &line);
	scanf("%d", &group);

	unsigned char aux;
	unsigned char *bytes = (unsigned char *)malloc(4 * sizeof(unsigned char));
	if (data_type == 'I') {
		bytes[0] = ((a[line][group] & 0xFF000000) >> (8 * 3));//primul grup
		bytes[1] = ((a[line][group] & 0x00FF0000) >> (8 * 2));//al doilea grup
		bytes[2] = ((a[line][group] & 0x0000FF00) >> 8);//al treilea grup
		bytes[3] = (a[line][group] & 0x000000FF);//al patrulea grup

		a[line][group] = *(int *)bytes;
/*
	Imi salveaza de la bytes[3]->bytes[0] in ordinea inversa fata de cum
apar in vectorul bytes incepand cu bytes[3] pana la bytes[0].
	Din acest motiv la swap-ul pe short va tb sa gandesc pe "logica negativa".
Daca vreau sa inversez doar primele 2 valori va tb sa inversez de fapt ultimile
2 si apoi pe toate , iar cand voi da cast la int se vor schimba doar primele 2.
*/

	} else if (data_type == 'S') {
		if (group % 2 == 0) {
			// swap pe primele 2 grupuri
			bytes[0] = ((a[line][group / 2] & 0xFF000000) >> (8 * 3));//1st
			bytes[1] = ((a[line][group / 2] & 0x00FF0000) >> (8 * 2));//2nd
			bytes[2] = ((a[line][group / 2] & 0x0000FF00) >> 8);//3rd
			bytes[3] = a[line][group / 2] & 0x000000FF;//4th

			aux = bytes[2];
			bytes[2] = bytes[3];
			bytes[3] = aux;
			//din cauza cast ului de la final lucrez pe logica inversa

			aux = bytes[0];
			bytes[0] = bytes[3];
			bytes[3] = aux;

			aux = bytes[1];
			bytes[1] = bytes[2];
			bytes[2] = aux;

			a[line][group / 2] = *(int *)bytes;
		} else if (group % 2 != 0) {
			// swap pe ultimile 2 grupuri
			bytes[0] = ((a[line][group / 2] & 0xFF000000) >> (8 * 3));//1st
			bytes[1] = ((a[line][group / 2] & 0x00FF0000) >> (8 * 2));//2nd
			bytes[2] = ((a[line][group / 2] & 0x0000FF00) >> 8);//3rd
			bytes[3] = a[line][group / 2] & 0x000000FF;//4th

			aux = bytes[0];
			bytes[0] = bytes[1];
			bytes[1] = aux;

			aux = bytes[0];
			bytes[0] = bytes[3];
			bytes[3] = aux;

			aux = bytes[1];
			bytes[1] = bytes[2];
			bytes[2] = aux;

			a[line][group / 2] = *(int *)bytes;
		}
	}
	free(bytes);
}

/*
Rationamentul pentru stergere este asemanator cu cel de la swap.
Astfel vom popula elementele vectorului bytes[], iar apoi vom
seta valorile din vector la 0 in functie de tipul de data
char -> o singura valoare tb setata 0
short -> 2 valori/ int -> 4valori <=> a[linie][grup-1]=0;
Rest este o variabila care ma va ajuta sa tin minte unde
tb realizata o schimbare , astfel modificarile vor avea loc
pentru bytes[rest] pentru char , iar pentru short rest va indica daca primii
2 bytes tb modificati sau ultimii 2.
*/

void delete_matrix(int **a)
{
	char data_type;
	int line, group;
	scanf(" %c", &data_type);
	scanf("%d", &line);
	scanf("%d", &group);
	int rest;
	unsigned char *bytes = (unsigned char *)malloc(4 * sizeof(unsigned char));

	if (data_type == 'C') {
		bytes[3] = ((a[line][(group - 1) / 4] & 0xFF000000) >> (8 * 3));//1st
		bytes[2] = ((a[line][(group - 1) / 4] & 0x00FF0000) >> (8 * 2));//2nd
		bytes[1] = ((a[line][(group - 1) / 4] & 0x0000FF00) >> 8);//3rd
		bytes[0] = a[line][(group - 1) / 4] & 0x000000FF;//4th

		rest = (group - 1) % 4;
		bytes[rest] = 0x00;

		a[line][(group - 1) / 4] = *(int *)bytes;
	} else if (data_type == 'S') {
		bytes[3] = ((a[line][(group - 1) / 2] & 0xFF000000) >> (8 * 3));//1st
		bytes[2] = ((a[line][(group - 1) / 2] & 0x00FF0000) >> (8 * 2));//2nd
		bytes[1] = ((a[line][(group - 1) / 2] & 0x0000FF00) >> 8);//3rd
		bytes[0] = a[line][(group - 1) / 2] & 0x000000FF;//4th

		rest = (group - 1) % 2;
		if (rest == 0) {
			bytes[0] = 0x00;
			bytes[1] = 0x00;
		} else {
			bytes[2] = 0x00;
			bytes[3] = 0x00;
		}

		a[line][(group - 1) / 2] = *(int *)bytes;
	} else if (data_type == 'I') {
		bytes[3] = ((a[line][group - 1] & 0xFF000000) >> (8 * 3));//1st
		bytes[2] = ((a[line][group - 1] & 0x00FF0000) >> (8 * 2));//2nd
		bytes[1] = ((a[line][group - 1] & 0x0000FF00) >> 8);//3rd
		bytes[0] = a[line][group - 1] & 0x000000FF;//4th

		for (int i = 0; i < 4; i++)
			bytes[i] = 0x00;

		a[line][group - 1] = *(int *)bytes;
	}
	free(bytes);
}

/*
Addmem verifica pentru o anumita linie , cate elemente tb adaugate
si apoi realloc memoria si seteaza valorile noi cu 0. Apoi va efectua
modificarile care tb urmate. Aceasta se apeleaza doar daca matricea curenta
nu are destule elemente alocate in prealabil.
*/

void addmem(char data_type, int line, int group, int element, int **a, int *v)
{
	int count_add = 0;

	if (data_type == 'C') {
		count_add = group / 4;
		if (group % 4  != 0)
			count_add++;
	}
	if (data_type == 'S') {
		count_add = group / 2;
		if (group % 2 != 0)
			count_add++;
	}
	if (data_type == 'I')
		count_add = group;
/*
count_add stabileste cate elemente tb sa fie in final pe linie
apoi se realoca cate o linie din matrice si se seteaza 0.
*/
	int *aux = (int *)realloc(a[line], count_add * sizeof(int *));
	if (!aux) {
		printf("Nu s-a putut aloca memoria");
		free(aux);
	}
	a[line] = aux;
	for (int i = v[line]; i < count_add; i++)
		a[line][i] = 0;

	v[line] = count_add;
	a[line][v[line] - 1] = element;
//adaugam elementul pe care il dorim pe pozitia ceruta.
}

/*
Modify_matrix este scrisa si ea dupa acelasi schelete al functiei swap
si delete cu singura exceptie ca aceasta include si o functie separata
care va realoca memoria in cazul in care linia asupra careia efectuam
modificarile nu are destule elemente.
Verific daca am destula memorie pentru a modifica elementele din matrice
si pentru asta verific tipul de data Char/Short/Int si apoi verific numarul
de elemente din vector[linie]. Daca nu am destula memorie apelez functia
addmem(..) , in caz ca am efectuez modificarile necesare.
Addmem va consulta tipul de data pe care il primeste pentru a stabili
cate elemente tb adaugate.
Rest are acelasi scop ca un functia de delete.
*/

void modify_matrix(int **a, int *v)
{
	char data_type;
	int line, group, element, rest;
	unsigned char *bytes = (unsigned char *)malloc(4 * sizeof(unsigned char));
	scanf(" %c ", &data_type);
	scanf("%d", &line);
	scanf("%d", &group);
	scanf("%X", &element);

	if (data_type == 'C') {
		if (v[line] * 4 < group) {
			addmem(data_type, line, group, element, a, v);
		} else if (v[line] * 4 >= group) {
			bytes[3] = ((a[line][(group - 1) / 4] & 0xFF000000) >> (8 * 3));
			bytes[2] = ((a[line][(group - 1) / 4] & 0x00FF0000) >> (8 * 2));
			bytes[1] = ((a[line][(group - 1) / 4] & 0x0000FF00) >> 8);
			bytes[0] = a[line][(group - 1) / 4] & 0x000000FF;

			rest = (group - 1) % 4;
			bytes[rest] = (element & 0x000000FF);

			a[line][(group - 1) / 4] = *(int *)bytes;
		}
	}
	if (data_type == 'S') {
		if (v[line] * 2 < group) {
			addmem(data_type, line, group, element, a, v);
		} else if (v[line] * 2 >= group) {
			bytes[3] = ((a[line][(group - 1) / 2] & 0xFF000000) >> (8 * 3));
			bytes[2] = ((a[line][(group - 1) / 2] & 0x00FF0000) >> (8 * 2));
			bytes[1] = ((a[line][(group - 1) / 2] & 0x0000FF00) >> 8);
			bytes[0] = a[line][(group - 1) / 2] & 0x000000FF;

			rest = (group - 1) % 2;
			if (rest == 0) {
				bytes[0] = (element & 0x000000FF);
				bytes[1] = ((element & 0x0000FF00) >> 8);
			} else if (rest != 0) {
				bytes[2] = (element & 0x000000FF);
				bytes[3] = ((element & 0x0000FF00) >> 8);
			}

			a[line][(group - 1) / 2] = *(int *)bytes;
		}
	}
	if (data_type == 'I') {
		if (v[line] < group)
			addmem(data_type, line, group, element, a, v);
		else if (v[line] >= group)
			a[line][group - 1] = element;
	}
	free(bytes);
}

int main(void)
{
	int **matrix, *vector;
	int n, k;
	char command;
	double m_a;

	scanf("%d", &n);
	vector = alloc_vector(n);
	matrix = alloc_matrix(n, vector);
	m_a = get_shield(n, matrix, vector);
	scanf("%d", &k);
	for (int i = 0; i < k; i++) {
		scanf(" %c ", &command);
		if (command == 'S')
			swap_matrix(matrix);
		if (command == 'D')
			delete_matrix(matrix);
		if (command == 'M')
			modify_matrix(matrix, vector);
	}
	printf("task 1\n");
	printf("%.8lf\n", m_a);
	printf("task 2\n");
	print_matrix(n, matrix, vector);
	printf("task 3\n");
	printf("\n");

	free(vector);
	for (int i = 0; i < n; i++)
		free(matrix[i]);
	free(matrix);

	return 0;
}
