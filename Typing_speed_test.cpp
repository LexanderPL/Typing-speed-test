#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <ctime>
using namespace std;

struct statystyki
{
	int czas_poczatkowy;
	int czas_koncowy;
	int liczba_znakow = 0; //��czna liczba znak�w w pliku wej�ciowym
	int liczba_bledow_ort = 0;
	int liczba_bledow_interpkt = 0;
	int czas_znaku_w_linii[100];
	int czas_znaku_min;
	int czas_znaku_max=0;
};

bool sprawdz_dostepnosc_pliku(string nazwa_pliku) //sprawdzanie czy mo�na otworzy� plik
{
	fstream plik;
	plik.open(nazwa_pliku.c_str(), ios::in); //otwieranie pliku
	if (!plik.good())
	{
		cout << "Nie udalo sie odczytac pliku!" << endl;
		system("pause");
		return false; //jesli sie nie udalo
	}
	plik.close();

	return true; //jesli sie udalo
}

void utworz_plik(string nazwa_pliku)
{
	fstream plik;
	plik.open(nazwa_pliku.c_str(), ios::out); //tworzenie pustego pliku wyj�ciowego. Je�li istania�, nadpisujemy go pustym plikiem
	plik.close();
}


void zapisz_czas_znaku(statystyki & staty, string plik_out, int ilosc) //zapisywanie czas�w, w jakich zapisano ka�dy znak danej linijki
{
	fstream plik;
	plik.open(plik_out.c_str(), ios::app); //otwieranie pliku
	for (int i = 0; i < ilosc; i++) plik << staty.czas_znaku_w_linii[i]<< endl; //zapisywanie danych jedna pod drug�
	plik.close();
}

void zapisz_linie_do_pliku(string linia_wpisana, string nazwa_pliku)
{
	fstream plik;
	plik.open(nazwa_pliku.c_str(), ios::app); //otwieranie pliku
	plik << linia_wpisana << endl; //zapisywanie do pliku linii wpisanej przez u�ytkownika
	plik.close();
}

void testuj_linie(string linia[], statystyki & staty, string nazwa_pliku_out, string plik_do_statystyk)
{
	int czas_start_znaku;
	int czas_stop_znaku;
	string linia_wpisana; //linia wpisana przez u�ytkownika
	int i; //i jest zadeklarowane osobno, �eby by� do niego dost�p poza p�tl� for

	for (i = 0; i < linia[0].size(); i++)
	{
		system("CLS"); //czy�cimy ekran
		cout << linia[0] << endl; //wypisujemy pierwsz� lini�
		for (int j = 0; j < i; j++) cout << " "; //przesuwamy kursor do miejsca, w kt�rym piszemy, �eby narysowa� tam gwiazdk�
		cout << "*"; //rysujemy gwiazdk�
		cout << endl << endl << linia[1] << endl << endl << endl << linia[2] << endl; //wypisujemy pozosta�e linie z odst�pami]

		char znak;
		czas_start_znaku = clock(); //start stopera
		znak = _getch(); //pobieramy wpisany znak od u�ytkownika
		czas_stop_znaku = clock(); //stop stopera
		linia_wpisana += znak; //dopisujemy nowy znak u�ytkownika do linii przez niego wpisanej
		staty.czas_znaku_w_linii[i] = czas_stop_znaku - czas_start_znaku; //liczymy czas wpisywanego znaku
	}
	zapisz_linie_do_pliku(linia_wpisana, nazwa_pliku_out);
	zapisz_czas_znaku(staty, plik_do_statystyk, i); //zapisujemy czasy wpisywania ka�dego znaku z linijki
}


void analizuj_statystyki(statystyki & staty, string nazwa_pliku_in, string nazwa_pliku_out, string plik_statystyki)
{
	int czas_znaku;
	string linia_usera;
	string linia_oryginalna;
	fstream plik1; //plik z oryginalnym tekstem
	fstream plik2; //plik z tekstem wpisanym przez u�ytkownika
	plik1.open(nazwa_pliku_in.c_str(), ios::in); //otwieranie pliku
	plik2.open(nazwa_pliku_out.c_str(), ios::in); //otwieranie pliku

	while (getline(plik1, linia_oryginalna)) //bierzemy po kolei ka�d� lini� z pliku oryginalnego, �eby j� por�wna� z ka�d� lini� pliku u�ytkownika
	{
		staty.liczba_znakow += linia_oryginalna.size(); //liczymy znaki w danej linii, �eby policzy� sum� znak�w w tek�cie

		getline(plik2, linia_usera); //bierzemy lini� z pliku u�ytkownika, analogiczn� do tej oryginalnej

		for (int i = 0; i < linia_oryginalna.size(); i++) //przechodzimy po kolei przez ka�dy znak w danej linii
		{
			if (linia_usera[i] != linia_oryginalna[i]) //je�li u�ytkownik nie wpisa� tego samego, co jest w oryginale
			{
			//sprawdzamy rodzaj b��du i zwi�kszamy liczb� b��d�w tego rodzaju
			if (linia_oryginalna[i] == '.' || linia_oryginalna[i] == ',' || linia_oryginalna[i] == '?' || linia_oryginalna[i] == '!') staty.liczba_bledow_interpkt++;
			else staty.liczba_bledow_ort++;
			}
		}
	}
	plik1.close(); //zamykamy oba pliki
	plik2.close();

	plik1.open(plik_statystyki.c_str(), ios::in); //otwieramy nowy plik (pod istniej�c� ju� nazw�, dla oszcz�dzania pami�ci)
	while (getline(plik1, linia_oryginalna)) //bierzemy po kolei ka�d� lini�
	{
		czas_znaku=atoi(linia_oryginalna.c_str()); //konwertujemy tekst na liczb�
		if (czas_znaku < staty.czas_znaku_min) staty.czas_znaku_min = czas_znaku; //sprawdzamy czy u�ytkownik pobi� rekord maks. lub min. czasu wpisywania znaku
		else if (czas_znaku > staty.czas_znaku_max) staty.czas_znaku_max = czas_znaku;  
	}
	plik1.close();
}

void wyswietl_statystyki(statystyki staty)
{
	system("cls");
	cout <<"STATYSTYKI: "<< endl;

	cout << "Czas trwania testu: " << (staty.czas_koncowy - staty.czas_poczatkowy)/1000 << "," << (staty.czas_koncowy - staty.czas_poczatkowy)%1000 << " s" << endl;
	cout << "Ilosc wpisanych znakow: " << staty.liczba_znakow << endl;
	cout << "Ilosc bledow ortograficznych: " << staty.liczba_bledow_ort << endl;
	cout << "Ilosc bledow interpunkcyjnych: " << staty.liczba_bledow_interpkt << endl;
	cout << "Minimalny czas nacisniecia klawisza: " << staty.czas_znaku_min << " ms" << endl;
	cout << "Maksymalny czas nacisniecia klawisza: " << staty.czas_znaku_max << " ms" << endl;
	cout << "Sredni czas nacisniecia klawisza: " << (staty.czas_koncowy - staty.czas_poczatkowy)/staty.liczba_znakow << " ms" << endl;
}


//====================================================================================

int main()
{
	statystyki staty; //tworzymy struktur�
	string nazwa_pliku_in; //nazwa pliku wej�ciowego
	string nazwa_pliku_out = "tekstOUT.txt"; //nazwa pliku wyj�ciowego dla wpisanego tekstu
	string plik_do_statystyk = "statystyki.txt"; //nazwa pliku do zapisu statystyk
	string linijka[3]; //b�dziemy przechowywa� 3 linie tekstu

	fstream plik_in;
	utworz_plik(nazwa_pliku_out); //funkcja tworzy pusty plik, �eby m�c go p�niej nadpisywa�
	utworz_plik(plik_do_statystyk);

	//Obs�uga pliku wej�ciowego
	cout << "Podaj sciezke do pliku wejsciowego: ";
	cin >> nazwa_pliku_in;
	if (!sprawdz_dostepnosc_pliku(nazwa_pliku_in)) return 0; //je�li pliku wej�ciowego nie da si� otworzy�, program ko�czy swoje dzia�anie

	plik_in.open(nazwa_pliku_in.c_str(), ios::in); //otwieranie pliku

	cout << "Plik wczytano pomyslnie." << endl << "Przygotuj sie!" << endl;
	system("pause");

	//bierzemy pierwsze dwie linijki
	getline(plik_in, linijka[0]);
	getline(plik_in, linijka[1]);

	staty.czas_znaku_min = 100000; //zak�adamy, �e minimalny czas to 100 sekund, �eby u�ytkownik m�g� go �atwo pobi� i uzyska� realny czas

	staty.czas_poczatkowy = clock(); //zaczynamy liczy� czas

	while (getline(plik_in, linijka[2])) //p�tla od trzeciej linii tekstu do ko�ca pliku
	{
		testuj_linie(linijka, staty, nazwa_pliku_out, plik_do_statystyk); //u�ytkownik wpisuje pierwsz� linijk�

		//przesuwamy linijki do g�ry
		linijka[0] = linijka[1];
		linijka[1] = linijka[2];
	}

	linijka[2] = ""; //plik si� nam sko�czy� (wyszli�my z p�tli), wi�c ostatnie linie staj� si� puste
	testuj_linie(linijka, staty, nazwa_pliku_out, plik_do_statystyk); //testujemy przedostatni� lini�
	linijka[0] = linijka[1]; //przesuwamy linijki w g�r�
	linijka[1] = "";
	testuj_linie(linijka, staty, nazwa_pliku_out, plik_do_statystyk); //testujemy ostatni� lini�

	staty.czas_koncowy = clock(); //ko�czymy przeliczanie czasu

	plik_in.close(); //zamykamy plik

	analizuj_statystyki(staty, nazwa_pliku_in, nazwa_pliku_out,  plik_do_statystyk);
	wyswietl_statystyki(staty);

	system("pause");
}
