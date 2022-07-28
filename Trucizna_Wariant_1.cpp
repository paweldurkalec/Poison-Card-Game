#define LSIZE 1500
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

struct parametry_t {
    int n = 0;
    int k = 0;
    int g = 0;
    int gv = 0;
    int o = 0;
    int e = 0;
    int r = 0;
};

struct karta_t {
    int wartosc;
    char tab[10];
};

struct gracz_t2 {
    karta_t* karty;
    karta_t* stos;
    int liczbaKart;
    int liczbaKartStos;
};

struct gracz_t {
    karta_t karty[130];
    karta_t stos[130];
    int liczbaKart = 0;
    int liczbaKartStos = 0;
};

struct kociolek_t2 {
    karta_t* karty;
    int liczbaKart;
    int sumaWartosci;
    char kolor[10];
};

struct kociolek_t {
    karta_t karty[130];
    char kolor[10] = { "brak" };
    int liczbaKart = 0;
    int sumaWartosci = 0;
};

struct stanGry_t {
    gracz_t2* gracze;
    kociolek_t2* kociolki;
    int iloscGraczy;
    int iloscKociolkow;
    int progEksplozji;
    int aktGracz;
    int czyDalejGramy;
    int numerRundy;
};

struct tablicaWartosci_t {
    char kolor[10];
    int wartosci[200];
    int licznik = 0;
};

void inicjalizujGraczyiKociolki(gracz_t2 gracze[], kociolek_t2 kociolki[], const parametry_t& parametry) {
    int wielkoscTalii = parametry.g + parametry.k * parametry.o;
    for (int i = 0; i < parametry.n; i++) {
        gracze[i].karty = (karta_t*)malloc(wielkoscTalii * sizeof(karta_t));
        gracze[i].stos = (karta_t*)malloc(wielkoscTalii * sizeof(karta_t));
        gracze[i].liczbaKart = 0;
        gracze[i].liczbaKartStos = 0;
    }
    for (int i = 0; i < parametry.k; i++) {
        kociolki[i].karty = (karta_t*)malloc(wielkoscTalii * sizeof(karta_t));
        kociolki[i].liczbaKart = 0;
        kociolki[i].sumaWartosci = 0;
        strcpy_s(kociolki[i].kolor, 10, "brak");
    }
}

void usunGraczyiKociolki(gracz_t2 gracze[], kociolek_t2 kociolki[], const parametry_t& parametry) {
    for (int i = 0; i < parametry.n; i++) {
        free(gracze[i].karty);
        free(gracze[i].stos);
    }
    for (int i = 0; i < parametry.k; i++) {
        free(kociolki[i].karty);
    }
}

void inicjalizujTalie(const parametry_t& parametry, const int ov[], const char tabelaKolorow[][10], karta_t talia[]) {
    int wielkoscTalii = parametry.g + parametry.k * parametry.o;
    //uzupelniamy talie o karty zielone
    for (int i = 0; i < parametry.g; i++) {
        talia[i].wartosc = parametry.gv;
        strcpy_s(talia[i].tab, 10, "green");
    }
    //uzupelniamy talie o reszte kolorow
    int aktualnyKolor = 0;
    int liczbaKartAktKoloru = 0;
    for (int i = parametry.g; i < (wielkoscTalii); i++) {
        talia[i].wartosc = ov[liczbaKartAktKoloru];
        strcpy_s(talia[i].tab, 10, tabelaKolorow[aktualnyKolor]);
        liczbaKartAktKoloru++;
        if (liczbaKartAktKoloru == parametry.o) {
            aktualnyKolor++;
            liczbaKartAktKoloru = 0;
        }
    }
}

void wypiszString(const char str[], const int dlugosc) {
    for (int i = 0; i < dlugosc; i++) {
        if (str[i] != '\0') cout << str[i];
        else break;
    }
}

void wypiszKarty(const karta_t karty[], const int dlugosc) {
    for (int i = 0; i < dlugosc; i++) {
        cout << karty[i].wartosc << " ";
        wypiszString(karty[i].tab, strlen(karty[i].tab));
        cout << " ";
    }
}

void rozdajKarty(const parametry_t& parametry, const karta_t talia[], gracz_t2 gracze[], const int aktGracz) {
    int wielkoscTalii = parametry.g + parametry.k * parametry.o;
    int aktualnygracz = aktGracz - 1;
    for (int i = 0; i < wielkoscTalii; i++) {
        gracze[aktualnygracz].karty[gracze[aktualnygracz].liczbaKart].wartosc = talia[i].wartosc;
        strcpy_s(gracze[aktualnygracz].karty[gracze[aktualnygracz].liczbaKart].tab, 10, talia[i].tab);
        gracze[aktualnygracz].liczbaKart++;
        aktualnygracz++;
        if (aktualnygracz == parametry.n) {
            aktualnygracz = 0;
        }
    }
}

void potasujTalie(karta_t talia[], int iloscKart) {
    srand(time(NULL));
    karta_t temp;
    int nr1, nr2;
    for (int i = 0; i < 1000; i++) {
        nr1 = rand() % iloscKart;
        nr2 = rand() % iloscKart;
        strcpy_s(temp.tab, 10, talia[nr1].tab);
        temp.wartosc = talia[nr1].wartosc;
        strcpy_s(talia[nr1].tab, 10, talia[nr2].tab);
        talia[nr1].wartosc = talia[nr2].wartosc;
        strcpy_s(talia[nr2].tab, 10, temp.tab);
        talia[nr2].wartosc = temp.wartosc;
    }

}

void zrestartujRozgrywke(gracz_t2 gracze[], const int iloscGraczy, kociolek_t2 kociolki[], const int iloscKociolkow) {
    for (int i = 0; i < iloscGraczy; i++) {
        gracze[i].liczbaKart = 0;
        gracze[i].liczbaKartStos = 0;
    }
    for (int i = 0; i < iloscKociolkow; i++) {
        kociolki[i].liczbaKart = 0;
        kociolki[i].sumaWartosci = 0;
    }
}

int podajIloscKart(gracz_t gracze[], const int iloscGraczy, kociolek_t kociolki[], const int iloscKociolkow, const int tryb) {
    // gdy tryb=1 zlicza wszystkie karty
    // gdy tryb=0 zlicza zagrane karty
    int suma = 0;
    for (int i = 0; i < iloscGraczy; i++) {
        if (tryb == 1) {
            suma += gracze[i].liczbaKart;
        }
        suma += gracze[i].liczbaKartStos;
    }
    for (int i = 0; i < iloscKociolkow; i++) {
        suma += kociolki[i].liczbaKart;
    }
    return suma;
}

int sprawdzStanGry(const int aktGracz, gracz_t gracze[], const int iloscGraczy, kociolek_t kociolki[], const int iloscKociolkow, const int progEksplozji) {
    int wszystkieKarty = podajIloscKart(gracze, iloscGraczy, kociolki, iloscKociolkow, 1);
    int zagraneKarty = podajIloscKart(gracze, iloscGraczy, kociolki, iloscKociolkow, 0);
    int ktoryGraczZaczal = aktGracz;
    int* tymczasowaTablicaIlosciKart = new int[iloscGraczy];
    for (int i = 0; i < iloscGraczy; i++) {
        tymczasowaTablicaIlosciKart[i] = 0;
    }
    //sprawdzenie ktory gracz zaczal rozgrywke
    for (int i = 0; i < zagraneKarty; i++) {
        ktoryGraczZaczal--;
        if (ktoryGraczZaczal == 0) {
            ktoryGraczZaczal = iloscGraczy;
        }
    }
    //symulacja poprawnego rozdania kart
    int temp = ktoryGraczZaczal - 1;
    for (int i = 0; i < wszystkieKarty; i++) {
        tymczasowaTablicaIlosciKart[temp]++;
        temp++;
        if (temp == iloscGraczy) {
            temp = 0;
        }
    }
    //symulacja poprawnych ruchow
    temp = ktoryGraczZaczal - 1;
    for (int i = 0; i < zagraneKarty; i++) {
        tymczasowaTablicaIlosciKart[temp]--;
        temp++;
        if (temp == iloscGraczy) {
            temp = 0;
        }
    }
    //porownanie prawidlowych i rzeczywistych wartosci
    int prawidlowyStanKartGraczy = 1;
    for (int i = 0; i < iloscGraczy; i++) {
        if (gracze[i].liczbaKart != tymczasowaTablicaIlosciKart[i]) prawidlowyStanKartGraczy = 0;
    }
    delete[] tymczasowaTablicaIlosciKart;
    //sprawdzenie prawidlowosci kart w kociolkach
    int* prawidloweKartyWKociolkach = new int[iloscKociolkow];
    for (int i = 0; i < iloscKociolkow; i++) {
        prawidloweKartyWKociolkach[i] = 1;
    }
    char kolor[10] = "brak";
    int czyWszystkieKociolkiOk = 1;
    for (int i = 0; i < iloscKociolkow; i++) {
        for (int j = 0; j < kociolki[i].liczbaKart; j++) {
            if (strcmp(kociolki[i].karty[j].tab, "green") != 0) {
                if (strcmp(kolor, "brak") == 0) strcpy_s(kolor, 10, kociolki[i].karty[j].tab);
                else if (strcmp(kolor, kociolki[i].karty[j].tab) != 0) {
                    prawidloweKartyWKociolkach[i] = 0;
                    czyWszystkieKociolkiOk = 0;
                }
            }
        }
        strcpy_s(kolor, 10, "brak");
    }
    //sprawdzenie czy kociolki powinny juz wybuchnac
    int* niepowinienWybuchnac = new int[iloscKociolkow];
    int czyWybuchyOk = 1;
    for (int i = 0; i < iloscKociolkow; i++) {
        niepowinienWybuchnac[i] = 1;
        if (kociolki[i].sumaWartosci > progEksplozji) {
            niepowinienWybuchnac[i] = 0;
            czyWybuchyOk = 0;
        }
    }
    //wypisanie statusu
    if ((czyWybuchyOk == 1) && (czyWszystkieKociolkiOk == 1) && (prawidlowyStanKartGraczy == 1)) {
        return 1;
    }
    else {
        if (prawidlowyStanKartGraczy == 0) cout << "The number of players cards on hand is wrong" << '\n';
        for (int i = 0; i < iloscKociolkow; i++) {
            if (prawidloweKartyWKociolkach[i] == 0) cout << "Two different colors were found on the " << i + 1 << " pile" << '\n';
        }
        for (int i = 0; i < iloscKociolkow; i++) {
            if (niepowinienWybuchnac[i] == 0) cout << "Pile number " << i + 1 << " should explode earlier" << '\n';
        }
        return 0;
    }
    delete[] prawidloweKartyWKociolkach;
    delete[] niepowinienWybuchnac;
}

void wypiszStanRozgrywki(const int iloscGraczy, const gracz_t gracze[], const int iloscKociolkow, const kociolek_t kociolki[], const int aktGracz) {
    cout << "active player = 1";
    cout << '\n';
    cout << "players number = " << iloscGraczy;
    cout << '\n';
    for (int i = 0; i < iloscGraczy; i++) {
        cout << i + 1 << " player hand cards: ";
        wypiszKarty(gracze[i].karty, gracze[i].liczbaKart);
        cout << '\n';
        cout << i + 1 << " player deck cards: ";
        wypiszKarty(gracze[i].stos, gracze[i].liczbaKartStos);
        cout << '\n';
    }
    for (int i = 0; i < iloscKociolkow; i++) {
        cout << i + 1 << " pile cards: ";
        wypiszKarty(kociolki[i].karty, kociolki[i].liczbaKart);
        cout << '\n';
    }
}

void zagrajPierwszaKarte(const int aktGracz, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow) {
    int zagrywanaKarta = 0;
    int nrGrwTablicy = aktGracz - 1;
    int czyZagranoKarte = 0;
    //zagranie karty jezeli jest juz kociolek z danym kolorem lub karta jest zielona
    for (int i = 0; i < iloscKociolkow; i++) {
        if ((strcmp(kociolki[i].kolor, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab) == 0) || (strcmp("green", gracze[nrGrwTablicy].karty[zagrywanaKarta].tab) == 0)) {
            strcpy_s(kociolki[i].karty[kociolki[i].liczbaKart].tab, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
            kociolki[i].karty[kociolki[i].liczbaKart].wartosc = gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
            kociolki[i].sumaWartosci += gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
            kociolki[i].liczbaKart++;
            czyZagranoKarte = 1;
            break;
        }
    }
    //zagranie karty jezeli nie znaleziono kociolka z danym kolorem, ani karta nie jest zielona
    if (czyZagranoKarte == 0) {
        for (int i = 0; i < iloscKociolkow; i++) {
            if (strcmp(kociolki[i].kolor, "brak") == 0) {
                strcpy_s(kociolki[i].karty[kociolki[i].liczbaKart].tab, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
                strcpy_s(kociolki[i].kolor, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
                kociolki[i].karty[kociolki[i].liczbaKart].wartosc = gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
                kociolki[i].sumaWartosci += gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
                kociolki[i].liczbaKart++;
                czyZagranoKarte = 1;
                break;
            }
        }
    }
    //przesuniecie kart gracza o 1 w lewo
    for (int i = zagrywanaKarta + 1; i < gracze[nrGrwTablicy].liczbaKart; i++) {
        strcpy_s(gracze[nrGrwTablicy].karty[i - 1].tab, 10, gracze[nrGrwTablicy].karty[i].tab);
        gracze[nrGrwTablicy].karty[i - 1].wartosc = gracze[nrGrwTablicy].karty[i].wartosc;
    }
    gracze[nrGrwTablicy].liczbaKart--;
}

void wybierzNajmniejszaKarte(const int aktGracz, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow) {
    int zagrywanaKarta;
    int najmniejszaWartosc;
    int nrGrwTablicy = aktGracz - 1;
    //znalezienie karty z najmniejsza wartoscia
    for (int i = 0; i < gracze[nrGrwTablicy].liczbaKart; i++) {
        if (i == 0) {
            najmniejszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
            zagrywanaKarta = i;
        }
        else if (gracze[nrGrwTablicy].karty[i].wartosc < najmniejszaWartosc) {
            najmniejszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
            zagrywanaKarta = i;
        }
    }
    int czyWczesniejZnalazl = 0;
    int wybranyKociolek = -1;
    //sprawdzenie czy ta karta nie musi isc do konkretnego kociolka
    for (int i = 0; i < iloscKociolkow; i++) {
        if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, kociolki[i].kolor) == 0)) {
            wybranyKociolek = i;
            czyWczesniejZnalazl = 1;
        }
    }
    //znalezienie kociolka z najmniejsza suma wartosci do ktorego mozna zagrac te karte, jezeli nie ma ona jeszcze swojego dedykowanego kociolka
    if (czyWczesniejZnalazl == 0) {
        for (int i = 0; i < iloscKociolkow; i++) {
            if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") == 0) || (strcmp(kociolki[i].kolor, "brak") == 0)) {
                if (czyWczesniejZnalazl == 0) {
                    wybranyKociolek = i;
                    najmniejszaWartosc = kociolki[i].sumaWartosci;
                    czyWczesniejZnalazl = 1;
                }
                else {
                    if (kociolki[i].sumaWartosci < najmniejszaWartosc) {
                        wybranyKociolek = i;
                        najmniejszaWartosc = kociolki[i].sumaWartosci;
                    }
                }
            }
        }
    }
    //zagranie karty
    if (wybranyKociolek == -1) {
        cout << "nie znaleziono kociolka" << '\n';
        wypiszStanRozgrywki(2, gracze, iloscKociolkow, kociolki, aktGracz);
    }
    else {
        if ((strcmp(kociolki[wybranyKociolek].kolor, "brak") == 0) && (strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") != 0)) {
            strcpy_s(kociolki[wybranyKociolek].kolor, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
        }
        strcpy_s(kociolki[wybranyKociolek].karty[kociolki[wybranyKociolek].liczbaKart].tab, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
        kociolki[wybranyKociolek].karty[kociolki[wybranyKociolek].liczbaKart].wartosc = gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
        kociolki[wybranyKociolek].sumaWartosci += gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
        kociolki[wybranyKociolek].liczbaKart++;

        //przesuniecie kart gracza o 1 w lewo
        for (int i = zagrywanaKarta + 1; i < gracze[nrGrwTablicy].liczbaKart; i++) {
            strcpy_s(gracze[nrGrwTablicy].karty[i - 1].tab, 10, gracze[nrGrwTablicy].karty[i].tab);
            gracze[nrGrwTablicy].karty[i - 1].wartosc = gracze[nrGrwTablicy].karty[i].wartosc;
        }
        gracze[nrGrwTablicy].liczbaKart--;
    }
}

void pozbadzSieNajwiekszej(const int aktGracz, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow, int progEksplozji) {
    int zagrywanaKarta;
    int najmniejszaWartosc;
    int nrGrwTablicy = aktGracz - 1;
    //znalezienie karty z najmniejsza wartoscia
    for (int i = 0; i < gracze[nrGrwTablicy].liczbaKart; i++) {
        if (i == 0) {
            najmniejszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
            zagrywanaKarta = i;
        }
        else if (gracze[nrGrwTablicy].karty[i].wartosc < najmniejszaWartosc) {
            najmniejszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
            zagrywanaKarta = i;
        }
    }
    int czyWczesniejZnalazl = 0;
    int wybranyKociolek = -1;
    //sprawdzenie czy ta karta nie musi isc do konkretnego kociolka
    for (int i = 0; i < iloscKociolkow; i++) {
        if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, kociolki[i].kolor) == 0)) {
            wybranyKociolek = i;
            czyWczesniejZnalazl = 1;
        }
    }
    //znalezienie kociolka z najmniejsza suma wartosci do ktorego mozna zagrac te karte, jezeli nie ma ona jeszcze swojego dedykowanego kociolka
    if (czyWczesniejZnalazl == 0) {
        for (int i = 0; i < iloscKociolkow; i++) {
            if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") == 0) || (strcmp(kociolki[i].kolor, "brak") == 0)) {
                if (czyWczesniejZnalazl == 0) {
                    wybranyKociolek = i;
                    najmniejszaWartosc = kociolki[i].sumaWartosci;
                    czyWczesniejZnalazl = 1;
                }
                else {
                    if (kociolki[i].sumaWartosci < najmniejszaWartosc) {
                        wybranyKociolek = i;
                        najmniejszaWartosc = kociolki[i].sumaWartosci;
                    }
                }
            }
        }
    }
    //sprawdzenie czy wybor najmniejszej karty spowoduje eksplozje, jestli tak to pozbywamy sie najwiekszej, jesli nie to zagrywamy wczesniej znaleziona najmniejsza
    if ((kociolki[wybranyKociolek].sumaWartosci + gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc) > progEksplozji) {
        int najwiekszaWartosc;
        //znalezienie karty z najwieksza wartoscia
        for (int i = 0; i < gracze[nrGrwTablicy].liczbaKart; i++) {
            if (i == 0) {
                najwiekszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
                zagrywanaKarta = i;
            }
            else if (gracze[nrGrwTablicy].karty[i].wartosc > najwiekszaWartosc) {
                najwiekszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
                zagrywanaKarta = i;
            }
        }
        czyWczesniejZnalazl = 0;
        wybranyKociolek = -1;
        //sprawdzenie czy ta karta nie musi isc do konkretnego kociolka      
        for (int i = 0; i < iloscKociolkow; i++) {
            if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, kociolki[i].kolor) == 0)) {
                wybranyKociolek = i;
                czyWczesniejZnalazl = 1;
            }
        }
        //znalezienie kociolka z najmniejsza suma wartosci do ktorego mozna zagrac te karte, jezeli nie ma ona jeszcze swojego dedykowanego kociolka
        if (czyWczesniejZnalazl == 0) {
            for (int i = 0; i < iloscKociolkow; i++) {
                if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") == 0) || (strcmp(kociolki[i].kolor, "brak") == 0)) {
                    if (czyWczesniejZnalazl == 0) {
                        wybranyKociolek = i;
                        najmniejszaWartosc = kociolki[i].sumaWartosci;
                        czyWczesniejZnalazl = 1;
                    }
                    else {
                        if (kociolki[i].sumaWartosci < najmniejszaWartosc) {
                            wybranyKociolek = i;
                            najmniejszaWartosc = kociolki[i].sumaWartosci;
                        }
                    }
                }
            }
        }
    }
    if (wybranyKociolek == -1) {
        cout << "nie znaleziono kociolka" << '\n';
        wypiszStanRozgrywki(2, gracze, iloscKociolkow, kociolki, aktGracz);
    }
    else {
        if ((strcmp(kociolki[wybranyKociolek].kolor, "brak") == 0) && (strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") != 0)) {
            strcpy_s(kociolki[wybranyKociolek].kolor, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
        }
        strcpy_s(kociolki[wybranyKociolek].karty[kociolki[wybranyKociolek].liczbaKart].tab, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
        kociolki[wybranyKociolek].karty[kociolki[wybranyKociolek].liczbaKart].wartosc = gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
        kociolki[wybranyKociolek].sumaWartosci += gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
        kociolki[wybranyKociolek].liczbaKart++;

        //przesuniecie kart gracza o 1 w lewo
        for (int i = zagrywanaKarta + 1; i < gracze[nrGrwTablicy].liczbaKart; i++) {
            strcpy_s(gracze[nrGrwTablicy].karty[i - 1].tab, 10, gracze[nrGrwTablicy].karty[i].tab);
            gracze[nrGrwTablicy].karty[i - 1].wartosc = gracze[nrGrwTablicy].karty[i].wartosc;
        }
        gracze[nrGrwTablicy].liczbaKart--;
    }
}

int czyWybuchnie(karta_t zagrywanaKarta, kociolek_t kociolki[], const int iloscKociolkow, const int progEksplozji, int* wybranyKociolek) {
    int czyWczesniejZnalazl = 0;
    //sprawdzenie czy ta karta musi isc do konkretnego kociolka
    for (int i = 0; i < iloscKociolkow; i++) {
        if ((strcmp(zagrywanaKarta.tab, kociolki[i].kolor) == 0)) {
            *wybranyKociolek = i;
            czyWczesniejZnalazl = 1;
        }
    }
    if (czyWczesniejZnalazl == 0) {
        if (kociolki[*wybranyKociolek].sumaWartosci + zagrywanaKarta.wartosc > progEksplozji) {
            return 1;
        }
    }
    //znalezienie kociolka z najmniejsza suma wartosci do ktorego mozna zagrac te karte, zeby sprawdzic czy tam wywola eksplozje
    int najmniejszaWartosc;
    if (czyWczesniejZnalazl == 0) {
        for (int i = 0; i < iloscKociolkow; i++) {
            if ((strcmp(zagrywanaKarta.tab, "green") == 0) || (strcmp(kociolki[i].kolor, "brak") == 0)) {
                if (czyWczesniejZnalazl == 0) {
                    *wybranyKociolek = i;
                    najmniejszaWartosc = kociolki[i].sumaWartosci;
                    czyWczesniejZnalazl = 1;
                }
                else {
                    if (kociolki[i].sumaWartosci < najmniejszaWartosc) {
                        *wybranyKociolek = i;
                        najmniejszaWartosc = kociolki[i].sumaWartosci;
                    }
                }
            }
        }
    }
    if (kociolki[*wybranyKociolek].sumaWartosci + zagrywanaKarta.wartosc > progEksplozji) {
        return 1;
    }
    return 0;
}

void wybierzOptymalnie(const int aktGracz, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow, int progEksplozji) {
    int nrGrwTablicy = aktGracz - 1;
    int zagrywanaKarta = -1;
    int mozliwyWybranyKociolek = -1;
    int wybranyKociolek = -1;
    //szukamy karty z najwieksza wartoscia, ktora nie spowoduje wybuchu w kociolku
    for (int i = 0; i < gracze[nrGrwTablicy].liczbaKart; i++) {
        if ((czyWybuchnie(gracze[nrGrwTablicy].karty[i], kociolki, iloscKociolkow, progEksplozji, &mozliwyWybranyKociolek) == 0) && (zagrywanaKarta == -1)) {
            zagrywanaKarta = i;
            wybranyKociolek = mozliwyWybranyKociolek;
        }
        else if ((czyWybuchnie(gracze[nrGrwTablicy].karty[i], kociolki, iloscKociolkow, progEksplozji, &mozliwyWybranyKociolek) == 0) && gracze[nrGrwTablicy].karty[i].wartosc > gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc) {
            zagrywanaKarta = i;
            wybranyKociolek = mozliwyWybranyKociolek;
        }
    }
    //jezeli wszystkie karty powoduja wybuch, zagrywamy najwieksza karte
    if (zagrywanaKarta == -1) {
        int najwiekszaWartosc;
        //znalezienie karty z najwieksza wartoscia
        for (int i = 0; i < gracze[nrGrwTablicy].liczbaKart; i++) {
            if (i == 0) {
                najwiekszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
                zagrywanaKarta = i;
            }
            else if (gracze[nrGrwTablicy].karty[i].wartosc > najwiekszaWartosc) {
                najwiekszaWartosc = gracze[nrGrwTablicy].karty[i].wartosc;
                zagrywanaKarta = i;
            }
        }
        int czyWczesniejZnalazl = 0;
        //sprawdzenie czy ta karta nie musi isc do konkretnego kociolka      
        for (int i = 0; i < iloscKociolkow; i++) {
            if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, kociolki[i].kolor) == 0)) {
                wybranyKociolek = i;
                czyWczesniejZnalazl = 1;
            }
        }
        int najmniejszaWartosc;
        //znalezienie kociolka z najmniejsza suma wartosci do ktorego mozna zagrac te karte, jezeli nie ma ona jeszcze swojego dedykowanego kociolka
        if (czyWczesniejZnalazl == 0) {
            for (int i = 0; i < iloscKociolkow; i++) {
                if ((strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") == 0) || (strcmp(kociolki[i].kolor, "brak") == 0)) {
                    if (czyWczesniejZnalazl == 0) {
                        wybranyKociolek = i;
                        najmniejszaWartosc = kociolki[i].sumaWartosci;
                        czyWczesniejZnalazl = 1;
                    }
                    else {
                        if (kociolki[i].sumaWartosci < najmniejszaWartosc) {
                            wybranyKociolek = i;
                            najmniejszaWartosc = kociolki[i].sumaWartosci;
                        }
                    }
                }
            }
        }
    }
    if (wybranyKociolek == -1) {
        cout << "nie znaleziono kociolka" << '\n';
        wypiszStanRozgrywki(2, gracze, iloscKociolkow, kociolki, aktGracz);
    }
    else {
        if ((strcmp(kociolki[wybranyKociolek].kolor, "brak") == 0) && (strcmp(gracze[nrGrwTablicy].karty[zagrywanaKarta].tab, "green") != 0)) {
            strcpy_s(kociolki[wybranyKociolek].kolor, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
        }
        strcpy_s(kociolki[wybranyKociolek].karty[kociolki[wybranyKociolek].liczbaKart].tab, 10, gracze[nrGrwTablicy].karty[zagrywanaKarta].tab);
        kociolki[wybranyKociolek].karty[kociolki[wybranyKociolek].liczbaKart].wartosc = gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
        kociolki[wybranyKociolek].sumaWartosci += gracze[nrGrwTablicy].karty[zagrywanaKarta].wartosc;
        kociolki[wybranyKociolek].liczbaKart++;

        //przesuniecie kart gracza o 1 w lewo
        for (int i = zagrywanaKarta + 1; i < gracze[nrGrwTablicy].liczbaKart; i++) {
            strcpy_s(gracze[nrGrwTablicy].karty[i - 1].tab, 10, gracze[nrGrwTablicy].karty[i].tab);
            gracze[nrGrwTablicy].karty[i - 1].wartosc = gracze[nrGrwTablicy].karty[i].wartosc;
        }
        gracze[nrGrwTablicy].liczbaKart--;
    }
}

void obsluzWybuchKociolkow(const int aktGracz, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow, const int progEksplozji) {
    int nrGrwTablicy = aktGracz - 1;
    for (int i = 0; i < iloscKociolkow; i++) {
        if (kociolki[i].sumaWartosci > progEksplozji) {
            for (int j = 0; j < kociolki[i].liczbaKart; j++) {
                strcpy_s(gracze[nrGrwTablicy].stos[gracze[nrGrwTablicy].liczbaKartStos].tab, 10, kociolki[i].karty[j].tab);
                gracze[nrGrwTablicy].stos[gracze[nrGrwTablicy].liczbaKartStos].wartosc = kociolki[i].karty[j].wartosc;
                gracze[nrGrwTablicy].liczbaKartStos++;
            }
            kociolki[i].liczbaKart = 0;
            kociolki[i].sumaWartosci = 0;
            strcpy_s(kociolki[i].kolor, 10, "brak");
        }
    }
}

void obsluzKoniecRundy(const int iloscGraczy, gracz_t gracze[], const char tabelaKolorow[][10], int punkty[], const int nrRundy) {
    if (nrRundy == 1) {
        for (int i = 0; i < iloscGraczy; i++) {
            punkty[i] = 0;
        }
    }
    //inicjacja tymczasowej tablicy do ktorej beda zapisane ilosci kart roznych kolorow
    int** iloscKolorowUGraczy = new int* [iloscGraczy];
    for (int i = 0; i < iloscGraczy; ++i) {
        iloscKolorowUGraczy[i] = new int[6];
        for (int j = 0; j < 6; j++) {
            iloscKolorowUGraczy[i][j] = 0;
        }
    }
    //inicjacja pomocniczych tablic, pierwsza zapisuje gracza, ktory ma najwiecej kart danego koloru, druga czy dwoch lub wiecej graczy ma najwiecej kart danego koloru
    int ktoryGracz[6] = { -1, -1, -1, -1, -1, -1 };
    int czyJedenGracz[6] = { 1, 1, 1, 1, 1, 1 };
    //zliczenie ilosci kart poszczegolnych kolorow u graczy oraz podliczenie punktow za zielone karty
    for (int i = 0; i < iloscGraczy; i++) {
        for (int j = 0; j < gracze[i].liczbaKartStos; j++) {
            for (int k = 0; k < 6; k++) {
                if (strcmp(gracze[i].stos[j].tab, tabelaKolorow[k]) == 0) {
                    iloscKolorowUGraczy[i][k]++;
                }
            }
            if (strcmp(gracze[i].stos[j].tab, "green") == 0) punkty[i] += 2;
        }
    }
    //sprawdzenie ktory gracz ma najwiecej kart danego koloru
    int max = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < iloscGraczy; j++) {
            if (iloscKolorowUGraczy[j][i] > max) {
                max = iloscKolorowUGraczy[j][i];
                ktoryGracz[i] = j;
                czyJedenGracz[i] = 1;
            }
            else if ((iloscKolorowUGraczy[j][i] == max) && (max != 0)) {
                czyJedenGracz[i] = 0;
            }
        }
        max = 0;
    }
    //wypisanie na jaki kolor kto jest odporny
    for (int i = 0; i < 6; i++) {
        if ((czyJedenGracz[i] == 1) && (ktoryGracz[i] != -1)) {
            //cout << "Na kolor " << tabelaKolorow[i] << " odporny jest gracz " << ktoryGracz[i] + 1 << '\n';
        }
    }
    //naliczenie punktow za karty inne niz zielone
    for (int i = 0; i < iloscGraczy; i++) {
        for (int j = 0; j < 6; j++) {
            if (!((ktoryGracz[j] == i) && (czyJedenGracz[j] == 1))) {
                punkty[i] += iloscKolorowUGraczy[i][j];
            }
        }
        //wypisanie punktow gracza
        cout << "Wynik gracza " << i + 1 << " = " << punkty[i] << '\n';
    }

    for (int i = 0; i < iloscGraczy; i++) {
        delete[] iloscKolorowUGraczy[i];
    }
    delete[] iloscKolorowUGraczy;
}

void ustalNastepnegoGracza(int* aktGracz, const int iloscGraczy) {
    (*aktGracz)++;
    if (*aktGracz > iloscGraczy) {
        *aktGracz = 1;
    }
}

void ustalKtoZaczynaNastRunde(int numerRundy, int iloscGraczy, int* aktGracz) {
    if (numerRundy % iloscGraczy == 1) {
        *aktGracz = 2;
    }
    else {
        *aktGracz = 1;
    }
}

void wczytajSlowo(const char line[], char temp[], int* lchar) {
    int znak = 0;
    while ((line[*lchar] != '\0') && (line[*lchar] != ' ') && (line[*lchar] != '\n')) {
        temp[znak] = line[*lchar];
        znak++;
        (*lchar)++;
    }
    if (line[*lchar] != '\0') {
        (*lchar)++;
    }
    while (line[*lchar] == ' ') {
        (*lchar)++;
    }
    temp[znak] = '\0';
}

int wczytajIlosc(FILE* plik) {
    char temp1[LSIZE];
    char* pointer;
    fgets(temp1, LSIZE, plik);
    pointer = strchr(temp1, '=');
    pointer += 2;
    return atoi(pointer);
}

void wczytajGraczy(gracz_t gracze[], const int iloscGraczy, FILE* plik) {
    char line[LSIZE];
    char temp1[10];
    char temp2[10];
    int lchar;
    for (int i = 0; i < iloscGraczy; i++) {
        fgets(line, LSIZE, plik);
        lchar = 21;
        while (unsigned(lchar) < strlen(line)) {
            wczytajSlowo(line, temp1, &lchar);
            wczytajSlowo(line, temp2, &lchar);
            gracze[i].karty[gracze[i].liczbaKart].wartosc = atoi(temp1);
            strcpy_s(gracze[i].karty[gracze[i].liczbaKart].tab, 10, temp2);
            gracze[i].liczbaKart++;
        }
        fgets(line, LSIZE, plik);
        lchar = 21;
        while (unsigned(lchar) < strlen(line)) {
            wczytajSlowo(line, temp1, &lchar);
            wczytajSlowo(line, temp2, &lchar);
            gracze[i].stos[gracze[i].liczbaKartStos].wartosc = atoi(temp1);
            strcpy_s(gracze[i].stos[gracze[i].liczbaKartStos].tab, 10, temp2);
            gracze[i].liczbaKartStos++;
        }
    }
}

int wczytajIloscKociolkow(gracz_t gracze[], const int iloscGraczy, const char tabelaKolorow[][10]) {
    int czy[6];
    czy[0] = czy[1] = czy[2] = czy[3] = czy[4] = czy[5] = 0;
    int ileKociolkow = 0;
    for (int i = 0; i < iloscGraczy; i++) {
        for (int j = 0; j < gracze[i].liczbaKart; j++) {
            for (int k = 0; k < 6; k++) {
                if (strcmp(gracze[i].karty[j].tab, tabelaKolorow[k]) == 0) czy[k]++;
                /*cout << endl;
                cout << "start" << endl;
                cout <<"tabelakolorow: "<< tabelaKolorow[k];
                cout << endl;
                cout <<"kartagracza: "<< gracze[i].karty[j].tab;
                cout << endl;
                cout << "stop" << endl;*/
            }
        }
        for (int j = 0; j < gracze[i].liczbaKartStos; j++) {
            for (int k = 0; k < 6; k++) {
                if (strcmp(gracze[i].stos[j].tab, tabelaKolorow[k]) == 0) czy[k]++;
            }
        }
    }
    for (int i = 0; i < 6; i++) {
        if (czy[i] > 0) ileKociolkow++;
    }
    return ileKociolkow;
}

void wczytajKociolki(kociolek_t kociolki[], int iloscKociolkow, FILE* plik) {
    char line[LSIZE];
    char temp1[10];
    char temp2[10];
    int lchar;
    for (int i = 0; i < iloscKociolkow; i++) {
        fgets(line, LSIZE, plik);
        lchar = 14;
        while (unsigned(lchar) < strlen(line)) {
            wczytajSlowo(line, temp1, &lchar);
            wczytajSlowo(line, temp2, &lchar);
            kociolki[i].karty[kociolki[i].liczbaKart].wartosc = atoi(temp1);
            kociolki[i].sumaWartosci += kociolki[i].karty[kociolki[i].liczbaKart].wartosc;
            strcpy_s(kociolki[i].karty[kociolki[i].liczbaKart].tab, 10, temp2);
            if ((strcmp(temp2, "green") != 0) && (strcmp(kociolki[i].kolor, "brak") == 0)) strcpy_s(kociolki[i].kolor, 10, temp2);
            kociolki[i].liczbaKart++;
        }
    }
}

void wypiszWszystkieInformacjeDoPliku(const int aktGracz, const int iloscGraczy, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow, const int progEksplozji, FILE* zapis) {
    //cout << "active player = " << aktGracz << endl;
    fprintf(zapis, "active player = %d\n", aktGracz);
    //cout << "players number = " << iloscGraczy << endl;
    fprintf(zapis, "players number = %d\n", iloscGraczy);
    //cout << "explosion threshold = " << progEksplozji << endl;
    fprintf(zapis, "explosion threshold = %d\n", progEksplozji);
    for (int i = 0; i < iloscGraczy; i++) {
        //cout << i + 1 << " player hand cards:";
        fprintf(zapis, "%d player hand cards:", i + 1);
        for (int j = 0; j < gracze[i].liczbaKart; j++) {
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].karty[j].wartosc;
            fprintf(zapis, "%d", gracze[i].karty[j].wartosc);
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].karty[j].tab;
            fprintf(zapis, "%s", gracze[i].karty[j].tab);
        }
        //cout << endl << i + 1 << " player deck cards:";
        fprintf(zapis, "\n%d player deck cards:", i + 1);
        for (int j = 0; j < gracze[i].liczbaKartStos; j++) {
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].stos[j].wartosc;
            fprintf(zapis, "%d", gracze[i].stos[j].wartosc);
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].stos[j].tab;
            fprintf(zapis, "%s", gracze[i].stos[j].tab);
        }
        //cout << endl;
        fprintf(zapis, "\n");
    }
    for (int i = 0; i < iloscKociolkow; i++) {
        //cout << i + 1 << " pile cards:";
        fprintf(zapis, "%d pile cards:", i + 1);
        for (int j = 0; j < kociolki[i].liczbaKart; j++) {
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << kociolki[i].karty[j].wartosc;
            fprintf(zapis, "%d", kociolki[i].karty[j].wartosc);
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << kociolki[i].karty[j].tab;
            fprintf(zapis, "%s", kociolki[i].karty[j].tab);
        }
        //cout << endl;
        fprintf(zapis, "\n");
    }
}

void wypiszWszystkieInformacje(const int aktGracz, const int iloscGraczy, gracz_t gracze[], kociolek_t kociolki[], const int iloscKociolkow, const int progEksplozji) {
    //cout << "active player = " << aktGracz << endl;
    printf("active player = %d\n", aktGracz);
    //cout << "players number = " << iloscGraczy << endl;
    printf("players number = %d\n", iloscGraczy);
    //cout << "explosion threshold = " << progEksplozji << endl;
    printf("explosion threshold = %d\n", progEksplozji);
    for (int i = 0; i < iloscGraczy; i++) {
        //cout << i + 1 << " player hand cards:";
        printf("%d player hand cards:", i + 1);
        for (int j = 0; j < gracze[i].liczbaKart; j++) {
            //cout << ' ';
            printf(" ");
            //cout << gracze[i].karty[j].wartosc;
            printf("%d", gracze[i].karty[j].wartosc);
            //cout << ' ';
            printf(" ");
            //cout << gracze[i].karty[j].tab;
            printf("%s", gracze[i].karty[j].tab);
        }
        //cout << endl << i + 1 << " player deck cards:";
        printf("\n%d player deck cards:", i + 1);
        for (int j = 0; j < gracze[i].liczbaKartStos; j++) {
            //cout << ' ';
            printf(" ");
            //cout << gracze[i].stos[j].wartosc;
            printf("%d", gracze[i].stos[j].wartosc);
            //cout << ' ';
            printf(" ");
            //cout << gracze[i].stos[j].tab;
            printf("%s", gracze[i].stos[j].tab);
        }
        //cout << endl;
        printf("\n");
    }
    for (int i = 0; i < iloscKociolkow; i++) {
        //cout << i + 1 << " pile cards:";
        printf("%d pile cards:", i + 1);
        for (int j = 0; j < kociolki[i].liczbaKart; j++) {
            //cout << ' ';
            printf(" ");
            //cout << kociolki[i].karty[j].wartosc;
            printf("%d", kociolki[i].karty[j].wartosc);
            //cout << ' ';
            printf(" ");
            //cout << kociolki[i].karty[j].tab;
            printf("%s", kociolki[i].karty[j].tab);
        }
        //cout << endl;
        printf("\n");
    }
}

void wypiszWszystkieInformacjeDoPliku2(const int aktGracz, const int iloscGraczy, gracz_t2 gracze[], kociolek_t2 kociolki[], const int iloscKociolkow, const int progEksplozji, FILE* zapis) {
    //cout << "active player = " << aktGracz << endl;
    fprintf(zapis, "active player = %d\n", aktGracz);
    //cout << "players number = " << iloscGraczy << endl;
    fprintf(zapis, "players number = %d\n", iloscGraczy);
    //cout << "explosion threshold = " << progEksplozji << endl;
    fprintf(zapis, "explosion threshold = %d\n", progEksplozji);
    for (int i = 0; i < iloscGraczy; i++) {
        //cout << i + 1 << " player hand cards:";
        fprintf(zapis, "%d player hand cards:", i + 1);
        for (int j = 0; j < gracze[i].liczbaKart; j++) {
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].karty[j].wartosc;
            fprintf(zapis, "%d", gracze[i].karty[j].wartosc);
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].karty[j].tab;
            fprintf(zapis, "%s", gracze[i].karty[j].tab);
        }
        //cout << endl << i + 1 << " player deck cards:";
        fprintf(zapis, "\n%d player deck cards:", i + 1);
        for (int j = 0; j < gracze[i].liczbaKartStos; j++) {
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].stos[j].wartosc;
            fprintf(zapis, "%d", gracze[i].stos[j].wartosc);
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << gracze[i].stos[j].tab;
            fprintf(zapis, "%s", gracze[i].stos[j].tab);
        }
        //cout << endl;
        fprintf(zapis, "\n");
    }
    for (int i = 0; i < iloscKociolkow; i++) {
        //cout << i + 1 << " pile cards:";
        fprintf(zapis, "%d pile cards:", i + 1);
        for (int j = 0; j < kociolki[i].liczbaKart; j++) {
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << kociolki[i].karty[j].wartosc;
            fprintf(zapis, "%d", kociolki[i].karty[j].wartosc);
            //cout << ' ';
            fprintf(zapis, " ");
            //cout << kociolki[i].karty[j].tab;
            fprintf(zapis, "%s", kociolki[i].karty[j].tab);
        }
        //cout << endl;
        fprintf(zapis, "\n");
    }
}

int gracz1() {
    FILE* zapis;
    fopen_s(&zapis, "zapis.txt", "r+");
    if (zapis == NULL) {
        printf("Otwarcie zapisu nie powiodlo sie\n");
        return 0;
    }

    const char tabelaKolorow[6][10] = { "blue", "red", "violet", "yellow", "white", "black" };
    int aktGracz = wczytajIlosc(zapis);

    if (aktGracz != 1) {
        fclose(zapis);
        printf("Nie moja kolej\n");
        return 0;
    }

    int iloscGraczy = wczytajIlosc(zapis);
    int progEksplozji = wczytajIlosc(zapis);
    gracz_t* gracze = new gracz_t[iloscGraczy];
    wczytajGraczy(gracze, iloscGraczy, zapis);

    if (gracze[aktGracz - 1].liczbaKart == 0) {
        fclose(zapis);
        delete[] gracze;
        return 0;
    }

    int iloscKociolkow = wczytajIloscKociolkow(gracze, iloscGraczy, tabelaKolorow);
    kociolek_t* kociolki = new kociolek_t[iloscKociolkow];
    wczytajKociolki(kociolki, iloscKociolkow, zapis);
    //wypiszWszystkieInformacje(aktGracz, iloscGraczy, gracze, kociolki, iloscKociolkow, progEksplozji);
    fclose(zapis);
    sprawdzStanGry(aktGracz, gracze, iloscGraczy, kociolki, iloscKociolkow, progEksplozji);

    FILE* przebieg;
    fopen_s(&przebieg, "przebieg_rund.txt", "a");
    if (przebieg == NULL) {
        printf("Otwarcie pliku z przebiegiem rund nie powiodlo sie\n");
        return 0;
    }
    wypiszWszystkieInformacjeDoPliku(aktGracz, iloscGraczy, gracze, kociolki, iloscKociolkow, progEksplozji, przebieg);
    fclose(przebieg);

    //strategia
    zagrajPierwszaKarte(aktGracz, gracze, kociolki, iloscKociolkow);
    //wybierzNajmniejszaKarte(aktGracz, gracze, kociolki, iloscKociolkow);
    //pozbadzSieNajwiekszej(aktGracz, gracze, kociolki, iloscKociolkow, progEksplozji);
    //wybierzOptymalnie(aktGracz, gracze, kociolki, iloscKociolkow, progEksplozji);

    obsluzWybuchKociolkow(aktGracz, gracze, kociolki, iloscKociolkow, progEksplozji);
    ustalNastepnegoGracza(&aktGracz, iloscGraczy);

    fopen_s(&zapis, "zapis.txt", "w");
    if (zapis == NULL) {
        printf("Otwarcie zapisu nie powiodlo sie\n");
        return 0;
    }
    wypiszWszystkieInformacjeDoPliku(aktGracz, iloscGraczy, gracze, kociolki, iloscKociolkow, progEksplozji, zapis);
    fclose(zapis);

    delete[] gracze;
    delete[] kociolki;
    return 1;
}

int gracz2() {
    FILE* zapis;
    fopen_s(&zapis, "zapis.txt", "r+");

    if (zapis == NULL) {
        printf("Otwarcie zapisu nie powiodlo sie\n");
        return 0;
    }

    const char tabelaKolorow[6][10] = { "blue", "red", "violet", "yellow", "white", "black" };
    int aktGracz = wczytajIlosc(zapis);

    if (aktGracz != 2) {
        fclose(zapis);
        printf("Nie moja kolej\n");
        return 0;
    }

    int iloscGraczy = wczytajIlosc(zapis);
    int progEksplozji = wczytajIlosc(zapis);
    gracz_t* gracze = new gracz_t[iloscGraczy];
    wczytajGraczy(gracze, iloscGraczy, zapis);

    if (gracze[aktGracz - 1].liczbaKart == 0) {
        fclose(zapis);
        delete[] gracze;
        return 0;
    }

    int iloscKociolkow = wczytajIloscKociolkow(gracze, iloscGraczy, tabelaKolorow);
    kociolek_t* kociolki = new kociolek_t[iloscKociolkow];
    wczytajKociolki(kociolki, iloscKociolkow, zapis);
    //wypiszWszystkieInformacje(aktGracz, iloscGraczy, gracze, kociolki, iloscKociolkow, progEksplozji);
    fclose(zapis);
    sprawdzStanGry(aktGracz, gracze, iloscGraczy, kociolki, iloscKociolkow, progEksplozji);

    FILE* przebieg;
    fopen_s(&przebieg, "przebieg_rund.txt", "a");
    if (przebieg == NULL) {
        printf("Otwarcie pliku z przebiegiem rund nie powiodlo sie\n");
        return 0;
    }
    wypiszWszystkieInformacjeDoPliku(aktGracz, iloscGraczy, gracze, kociolki, iloscKociolkow, progEksplozji, przebieg);
    fclose(przebieg);

    //strategia
    //zagrajPierwszaKarte(aktGracz, gracze, kociolki, iloscKociolkow);
    wybierzNajmniejszaKarte(aktGracz, gracze, kociolki, iloscKociolkow);
    //pozbadzSieNajwiekszej(aktGracz, gracze, kociolki, iloscKociolkow, progEksplozji);
    //wybierzOptymalnie(aktGracz, gracze, kociolki, iloscKociolkow, progEksplozji);

    obsluzWybuchKociolkow(aktGracz, gracze, kociolki, iloscKociolkow, progEksplozji);
    ustalNastepnegoGracza(&aktGracz, iloscGraczy);

    fopen_s(&zapis, "zapis.txt", "w");
    if (zapis == NULL) {
        printf("Otwarcie zapisu nie powiodlo sie\n");
        return 0;
    }
    wypiszWszystkieInformacjeDoPliku(aktGracz, iloscGraczy, gracze, kociolki, iloscKociolkow, progEksplozji, zapis);
    fclose(zapis);

    delete[] gracze;
    delete[] kociolki;
    return 1;
}

int main()
{
    //przyjmujemy parametry rozgrywki z wejscia i inicjalizujemy potrzebne tablice
    FILE* zrodlo;
    FILE* zapis;
    FILE* przebieg;

    fopen_s(&przebieg, "przebieg_rund.txt", "w");
    if (przebieg == NULL) {
        printf("Otwarcie pliku z przebiegiem rund nie powiodlo sie\n");
        return 0;
    }
    fclose(przebieg);

    fopen_s(&zrodlo, "zrodlo.txt", "r");
    if (zrodlo == NULL) {
        printf("Otwarcie zrodla nie powiodlo sie\n");
        return 0;
    }
    stanGry_t stanGry;
    parametry_t parametry;
    const char tabelaKolorow[6][10] = { "blue", "red", "violet", "yellow", "white", "black" };
    fscanf_s(zrodlo, "%d %d %d %d %d %d %d\n", &parametry.n, &parametry.k, &parametry.g, &parametry.gv, &parametry.o, &parametry.e, &parametry.r);
    stanGry.progEksplozji = parametry.e;
    stanGry.aktGracz = 1;
    stanGry.iloscGraczy = parametry.n;
    stanGry.iloscKociolkow = parametry.k;
    int iloscRund = parametry.r;
    int* punkty = new int[stanGry.iloscGraczy];
    int iloscKart = parametry.g + parametry.k * parametry.o;
    stanGry.gracze = new gracz_t2[parametry.n];
    int* ov = new int[parametry.o];
    karta_t* talia = new karta_t[iloscKart];
    stanGry.kociolki = new kociolek_t2[parametry.k];
    int aktGracz = 1;
    int iloscGraczy;
    int progEksplozji;
    int iloscKociolkow;
    for (int i = 0; i < parametry.o; i++) {
        fscanf_s(zrodlo, " %d", &ov[i]);
    }

    inicjalizujTalie(parametry, ov, tabelaKolorow, talia);
    inicjalizujGraczyiKociolki(stanGry.gracze, stanGry.kociolki, parametry);

    //petla obslugujaca cala rozgrywke
    for (int i = 1; i <= iloscRund; i++) {
        cout << "Runda nr: " << i << '\n';
        cout << "Runde zaczyna gracz:" << aktGracz << '\n';
        stanGry.czyDalejGramy = 1;
        stanGry.numerRundy = i;
        potasujTalie(talia, iloscKart);
        rozdajKarty(parametry, talia, stanGry.gracze, aktGracz);
        fopen_s(&zapis, "zapis.txt", "w");
        if (zapis == NULL) {
            printf("Otwarcie zapisu nie powiodlo sie\n");
            return 0;
        }
        wypiszWszystkieInformacjeDoPliku2(aktGracz, stanGry.iloscGraczy, stanGry.gracze, stanGry.kociolki, stanGry.iloscKociolkow, stanGry.progEksplozji, zapis);
        fclose(zapis);

        //petla obslugujaca aktualna runde
        while (stanGry.czyDalejGramy) {
            if (aktGracz == 1) {
                stanGry.czyDalejGramy = gracz1();
            }
            else if (aktGracz == 2) {
                stanGry.czyDalejGramy = gracz2();
            }
            ustalNastepnegoGracza(&aktGracz, stanGry.iloscGraczy);
        }

        //wczytanie informacji z pliku po zakonczeniu rundy w celu podliczenia punktow
        fopen_s(&zapis, "zapis.txt", "r");
        if (zapis == NULL) {
            printf("Otwarcie zapisu nie powiodlo sie\n");
            return 0;
        }
        aktGracz = wczytajIlosc(zapis);
        iloscGraczy = wczytajIlosc(zapis);
        progEksplozji = wczytajIlosc(zapis);
        gracz_t* gracze = new gracz_t[iloscGraczy];
        wczytajGraczy(gracze, iloscGraczy, zapis);
        iloscKociolkow = wczytajIloscKociolkow(gracze, iloscGraczy, tabelaKolorow);
        kociolek_t* kociolki = new kociolek_t[iloscKociolkow];
        wczytajKociolki(kociolki, iloscKociolkow, zapis);
        fclose(zapis);
        ustalKtoZaczynaNastRunde(stanGry.numerRundy, iloscGraczy, &aktGracz);
        obsluzKoniecRundy(iloscGraczy, gracze, tabelaKolorow, punkty, i);
        delete[] gracze;
        delete[] kociolki;
        zrestartujRozgrywke(stanGry.gracze, stanGry.iloscGraczy, stanGry.kociolki, stanGry.iloscKociolkow);
    }

    usunGraczyiKociolki(stanGry.gracze, stanGry.kociolki, parametry);
    delete[] punkty;
    delete[] ov;
    delete[] talia;
    delete[] stanGry.gracze;
    delete[] stanGry.kociolki;
    fclose(zrodlo);
    cout << "Koniec rozgrywki.\n";
    return 0;
}

