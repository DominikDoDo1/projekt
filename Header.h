#pragma once

typedef struct {
	int id;
	char ime[50];
	int kalorije;
} Hrana;

typedef struct {
	int idHrane;
	int kalorije;
} UnosKalorija;

typedef enum {
	DODAJ_HRANU = 1,
	IZBRISI_HRANU,
	PROMIJENI_HRANU,
	ISCITAJ_HRANU,
	UNOS_KALORIJA,
	PRIKAZI_UKUPNE_KALORIJE,
	PRIKAZI_IDEALNE_KALORIJE,
	ZAVRSI_PROGRAM,
	SORTIRAJ_PO_IMENU_UZLAZNO = 101,
	SORTIRAJ_PO_IMENU_SILAZNO,
	SORTIRAJ_PO_KALORIJAMA_UZLAZNO,
	SORTIRAJ_PO_KALORIJAMA_SILAZNO
} OpcijaIzbornika;

void izbornik();
void dodajHranu();
void izbrisiHranu();
void promijeniHranu();
void iscitavanjeHrane();
void unosKalorija();
void prikaziUkupneKalorije();
void prikaziIdealneKalorije();
void sortirajHranu(OpcijaIzbornika opcija);
int usporediImeUzlazno(const void* a, const void* b);
int usporediImeSilazno(const void* a, const void* b);
int usporediKalorijeUzlazno(const void* a, const void* b);
int usporediKalorijeSilazno(const void* a, const void* b);

