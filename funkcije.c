#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Header.h"

#define DATHRANA "Hrana.bin"
#define DATKALORIJA "Kalorije.bin"

void izbornik() {
	int izbor;
	char buffer[10];

	do {
		system("cls");
		printf("1. Dodaj namirnicu\n");
		printf("2. Izbrisi namirnicu\n");
		printf("3. Promijeni namirnicu\n");
		printf("4. Iscitaj namirnice\n");
		printf("5. Unos kalorija\n");
		printf("6. Broj unesenih kalorija\n");
		printf("7. Savrsen broj kalorija\n");
		printf("8. Zavrsi program\n");
		printf("Izaberite opciju: ");
		fgets(buffer, 10, stdin);
		if (sscanf(buffer, "%d", &izbor) == 1) {
			switch ((OpcijaIzbornika)izbor) {
			case DODAJ_HRANU:
				dodajHranu();
				break;
			case IZBRISI_HRANU:
				izbrisiHranu();
				break;
			case PROMIJENI_HRANU:
				promijeniHranu();
				break;
			case ISCITAJ_HRANU:
				iscitavanjeHrane();
				break;
			case UNOS_KALORIJA:
				unosKalorija();
				break;
			case PRIKAZI_UKUPNE_KALORIJE:
				prikaziUkupneKalorije();
				break;
			case PRIKAZI_IDEALNE_KALORIJE:
				prikaziIdealneKalorije();
				break;
			case ZAVRSI_PROGRAM:
				printf("Zavrsetak programa.\n");
				break;
			default:
				printf("Pogresan unos, pokusajte ponovno.\n");
			}
		}
		else {
			printf("Pogresan unos, unesite samo jednu opciju.\n");
		}
	} while (izbor != ZAVRSI_PROGRAM);
}

void dodajHranu() {
	FILE* datoteka = fopen(DATHRANA, "rb+");
	if (!datoteka) {
		// If the file doesn't exist, create it
		datoteka = fopen(DATHRANA, "wb+");
		if (!datoteka) {
			perror("Greska pri kreiranju datoteke");
			return;
		}
	}

	Hrana hrana;
	int maxID = 0;

	// Determine the highest existing ID
	while (fread(&hrana, sizeof(Hrana), 1, datoteka)) {
		if (hrana.id > maxID) {
			maxID = hrana.id;
		}
	}

	hrana.id = maxID + 1;

	printf("Unesite ime namirnice: ");
	fgets(hrana.ime, sizeof(hrana.ime), stdin);
	hrana.ime[strcspn(hrana.ime, "\n")] = '\0';

	printf("Unesite broj kalorija: ");
	while (scanf("%d", &hrana.kalorije) != 1) {
		printf("Neispravan unos. Molimo unesite broj kalorija: ");
		while (getchar() != '\n'); 
	}
	while (getchar() != '\n'); 

	fseek(datoteka, 0, SEEK_END);
	fwrite(&hrana, sizeof(Hrana), 1, datoteka);
	fclose(datoteka);

	printf("Namirnica dodana.\n");
}





void izbrisiHranu() {
	int id;
	printf("Unesite ID namirnice za brisanje: ");
	scanf("%d", &id);

	FILE* datoteka = fopen(DATHRANA, "rb");
	if (!datoteka) {
		perror("Greska pri otvaranju datoteke");
		return;
	}

	FILE* privremenaDatoteka = fopen("privremena.bin", "wb");
	if (!privremenaDatoteka) {
		perror("Greska pri otvaranju privremene datoteke");
		fclose(datoteka);
		return;
	}

	Hrana hrana;
	int pronadeno = 0;

	while (fread(&hrana, sizeof(Hrana), 1, datoteka)) {
		if (hrana.id != id) {
			fwrite(&hrana, sizeof(Hrana), 1, privremenaDatoteka);
		}
		else {
			pronadeno = 1;
		}
	}

	fclose(datoteka);
	fclose(privremenaDatoteka);

	remove(DATHRANA);
	rename("privremena.bin", DATHRANA);

	if (pronadeno) {
		printf("Namirnica izbrisana.\n");
	}
	else {
		printf("Namirnica s ID %d nije pronaðena.\n", id);
	}
}

void promijeniHranu() {
	int id;
	printf("Unesite ID namirnice za promjenu: ");
	scanf("%d", &id);

	FILE* datoteka = fopen(DATHRANA, "rb+");
	if (!datoteka) {
		perror("Greska pri otvaranju datoteke");
		return;
	}

	FILE* privremenaDatoteka = fopen("privremena.bin", "wb");
	if (!privremenaDatoteka) {
		perror("Greska pri otvaranju privremene datoteke");
		fclose(datoteka);
		return;
	}

	Hrana hrana;
	int pronadeno = 0;

	while (fread(&hrana, sizeof(Hrana), 1, datoteka)) {
		if (hrana.id == id) {
			printf("Unesite novo ime namirnice: ");
			scanf("%s", hrana.ime);
			printf("Unesite novi broj kalorija: ");
			scanf("%d", &hrana.kalorije);

			pronadeno = 1;
		}
		fwrite(&hrana, sizeof(Hrana), 1, privremenaDatoteka);
	}

	fclose(datoteka);
	fclose(privremenaDatoteka);

	remove(DATHRANA);
	rename("privremena.bin", DATHRANA);

	if (pronadeno) {
		printf("Namirnica ažurirana.\n");
	}
	else {
		printf("Namirnica s ID %d nije pronaðena.\n", id);
	}
}

void iscitavanjeHrane() {
	int izbor;
	char buffer[10];

	do {
		printf("1. Sortiraj po imenu uzlazno\n");
		printf("2. Sortiraj po imenu silazno\n");
		printf("3. Sortiraj po broju kalorija uzlazno\n");
		printf("4. Sortiraj po broju kalorija silazno\n");
		printf("0. Povratak na izbornik\n");
		printf("Izaberite opciju: ");
		fgets(buffer, 10, stdin);
		if (sscanf(buffer, "%d", &izbor) == 1) {

			if (izbor >= 1 && izbor <= 4) {
				sortirajHranu(izbor);
			}
			else if (izbor != 0) {
				printf("Pogresan unos, pokusajte ponovno.\n");
			}
		}
		else {
			printf("Pogresan unos, unesite samo jednu opciju.\n");
		}
	} while (izbor != 0);
}

void sortirajHranu(int opcija) {
	FILE* datoteka = fopen(DATHRANA, "rb");
	if (!datoteka) {
		perror("Greska pri otvaranju datoteke");
		return;
	}

	fseek(datoteka, 0, SEEK_END);
	long velicinaDatoteke = ftell(datoteka);
	rewind(datoteka);

	int brojNamirnica = velicinaDatoteke / sizeof(Hrana);
	Hrana* namirnice = (Hrana*)malloc(brojNamirnica * sizeof(Hrana));
	if (!namirnice) {
		perror("Greska pri alociranju memorije");
		fclose(datoteka);
		return;
	}

	fread(namirnice, sizeof(Hrana), brojNamirnica, datoteka);
	fclose(datoteka);

	switch (opcija) {
	case 1:
		qsort(namirnice, brojNamirnica, sizeof(Hrana), usporediImeUzlazno);
		break;
	case 2:
		qsort(namirnice, brojNamirnica, sizeof(Hrana), usporediImeSilazno);
		break;
	case 3:
		qsort(namirnice, brojNamirnica, sizeof(Hrana), usporediKalorijeUzlazno);
		break;
	case 4:
		qsort(namirnice, brojNamirnica, sizeof(Hrana), usporediKalorijeSilazno);
		break;
	}

	printf("%-5s %-20s %-10s\n", "ID", "Ime", "Kalorije");
	for (int i = 0; i < brojNamirnica; i++) {
		printf("%-5d %-20s %-10d\n", namirnice[i].id, namirnice[i].ime, namirnice[i].kalorije);
	}

	free(namirnice);
}

int usporediImeUzlazno(const void* a, const void* b) {
	return strcmp(((Hrana*)a)->ime, ((Hrana*)b)->ime);
}

int usporediImeSilazno(const void* a, const void* b) {
	return strcmp(((Hrana*)b)->ime, ((Hrana*)a)->ime);
}

int usporediKalorijeUzlazno(const void* a, const void* b) {
	return ((Hrana*)a)->kalorije - ((Hrana*)b)->kalorije;
}

int usporediKalorijeSilazno(const void* a, const void* b) {
	return ((Hrana*)b)->kalorije - ((Hrana*)a)->kalorije;
}

void unosKalorija() {
	int izbor;
	printf("1. Pretrazi namirnicu preko ID\n");
	printf("2. Pretrazi namirnicu preko imena\n");
	printf("Izaberite opciju: ");
	scanf("%d", &izbor);

	int id;
	char ime[50];
	Hrana hrana;
	int pronadeno = 0;

	FILE* datoteka = fopen(DATHRANA, "rb");
	if (!datoteka) {
		perror("Greska pri otvaranju datoteke");
		return;
	}

	switch (izbor) {
	case 1:
		printf("Unesite ID namirnice: ");
		scanf("%d", &id);
		while (fread(&hrana, sizeof(Hrana), 1, datoteka)) {
			if (hrana.id == id) {
				pronadeno = 1;
				break;
			}
		}
		break;
	case 2:
		printf("Unesite ime namirnice: ");
		scanf("%s", ime);
		while (fread(&hrana, sizeof(Hrana), 1, datoteka)) {
			if (strcmp(hrana.ime, ime) == 0) {
				pronadeno = 1;
				break;
			}
		}
		break;
	default:
		printf("Pogresan unos, pokusajte ponovno.\n");
		fclose(datoteka);
		return;
	}

	fclose(datoteka);

	if (pronadeno) {
		UnosKalorija unos;
		unos.idHrane = hrana.id;
		unos.kalorije = hrana.kalorije;

		FILE* datotekaKalorija = fopen(DATKALORIJA, "ab");
		if (!datotekaKalorija) {
			perror("Greska pri otvaranju datoteke");
			return;
		}

		fwrite(&unos, sizeof(UnosKalorija), 1, datotekaKalorija);
		fclose(datotekaKalorija);
		printf("Uneseno %d kalorija za namirnicu %s.\n", hrana.kalorije, hrana.ime);
	}
	else {
		printf("Namirnica nije pronaðena.\n");
	}
}

void prikaziUkupneKalorije() {
	FILE* datoteka;
	UnosKalorija unos;
	int ukupnoKalorija;
	int unosBroj;
	char buffer[10];

	do {
		datoteka = fopen(DATKALORIJA, "rb");
		if (!datoteka) {
			perror("Greska pri otvaranju datoteke");
			return;
		}

		ukupnoKalorija = 0;

		while (fread(&unos, sizeof(UnosKalorija), 1, datoteka)) {
			ukupnoKalorija += unos.kalorije;
		}

		fclose(datoteka);

		printf("Unijeli ste %d kalorija tijekom dana.\n", ukupnoKalorija);
		printf("Unesite 0 za izlazak, 1 za resetiranje kalorija: ");

		fgets(buffer, sizeof(buffer), stdin);
		if (sscanf(buffer, "%d", &unosBroj) != 1) {
			printf("Neispravan unos, pokusajte ponovno.\n");
			continue;
		}

		if (unosBroj == 1) {
			datoteka = fopen(DATKALORIJA, "wb");
			if (!datoteka) {
				perror("Greska pri otvaranju datoteke");
				return;
			}
			fclose(datoteka);
			printf("Kalorije su resetirane.\n");
		}

	} while (unosBroj != 0);
}

void prikaziIdealneKalorije() {
	float visina, tezina;
	int izbor;
	char buffer[10];

	do {
		printf("Unesite visinu (u cm) ili 0 za izlaz: ");
		fgets(buffer, sizeof(buffer), stdin);
		if (sscanf(buffer, "%f", &visina) != 1 || visina == 0) {
			printf("Izlaz iz funkcije.\n");
			return;
		}

		printf("Unesite tezinu (u kg): ");
		fgets(buffer, sizeof(buffer), stdin);
		if (sscanf(buffer, "%f", &tezina) != 1) {
			printf("Neispravan unos, pokusajte ponovno.\n");
			continue;
		}


		float bmr = 10 * tezina + 6.25 * visina - 5 * 30 + 5;
		float donjiRaspon = bmr * 1.2;
		float gornjiRaspon = bmr * 1.5;

		printf("Idealan unos kalorija je izmeðu %.0f i %.0f kalorija dnevno.\n", donjiRaspon, gornjiRaspon);

	} while (1);
}

