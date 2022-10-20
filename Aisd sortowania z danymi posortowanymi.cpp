#include <iostream>
#include <chrono>
#include <random>
#include <limits>
#include <algorithm>
#include <string>
#include <iomanip>
#include <fstream>
#include <clocale>

using std::swap;
using std::sort;


int losowa_liczba(int min, int max)
{
	static std::default_random_engine gen(std::random_device{}());
	static std::uniform_int_distribution<int> dist;
	return dist(gen, std::uniform_int_distribution<int>::param_type{ min, max });
}

void wypelnij(int* tablica, int rozmiar, int min = 0, int max = std::numeric_limits<int>::max())
{
	for (int i = 0; i < rozmiar; ++i)
		tablica[i] = i;
}
bool jest_posortowane(int* tablica, int rozmiar)
{
	return std::is_sorted(tablica, tablica + rozmiar);
}

double mierz_czas(int* tablica, int rozmiar, void(*funkcja_sortujaca)(int*, int))
{
	auto start = std::chrono::high_resolution_clock::now();
	funkcja_sortujaca(tablica, rozmiar);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	return duration.count();
}

/*

	parametry:
	 - funkcja_sortujaca - wskanik na funkcjącą sortowanie, musi przyjmować dwa parametry: adres początku tablicy (int*) oraz jej rozmiar (int)
	 - nazwa - nazwa testowanej funkcji, tylko w celach wypisania
	 - output - strumien do ktorego beda zapisane wyniki, domyslnie std::cerr, przy ostatnim uruchomieniu warto nadpisac otwartym strumieniem plikowym, aby sobie zebrac wyniki do pliku
	 - dodatkowe_miejsce - liczba dodatkowych elementow tablicy zaalokowanych PRZED poczatkiem tablicy, przykladowo gdy =1, pierwszym indeksem tablicy jest -1, ale dane rozpoczynaja sie od indeksu 0, moze sie przydac do sortowania przez wstawianie z wartownikiem
*/
void eksperyment(void(*funkcja_sortujaca)(int*, int), const std::string& nazwa, std::ostream& output = std::cerr, int dodatkowe_miejsce = 0)
{
	//ustawienia
	const double limit_czasu = 5; //sekund
	const int powtorzen = 3;
	const int rozmiar_poczatkowy = 1 << 10;
	/////////////////////////////////////////
	const int szerokosc = 100;
	int gwiazdek = szerokosc - nazwa.length() - 2;
	if (gwiazdek < 0)
		gwiazdek = 0;
	int i = 0;
	output << " ";
	for (; i < gwiazdek / 2; ++i)
		output << '*';
	output << " " << nazwa << " ";
	for (; i < gwiazdek; ++i)
		output << '*';
	output << "\n\n";
	output.flush();

	output << std::setw(9) << "N";
	output << std::setw(1) << "";
	for (int nr = 0; nr < powtorzen; ++nr)
		output << std::setw(9) << nr + 1 << " ";
	output << std::setw(12) << "średna" << " ";
	output << "\n";
	for (int rozmiar = rozmiar_poczatkowy; ; rozmiar *= 2)
	{
		output << std::setw(9) << rozmiar << ": ";
		output.flush();
		int* pamiec = new int[dodatkowe_miejsce + rozmiar];
		int* tablica = pamiec + dodatkowe_miejsce;
		double czas = 0.0;

		int* pattern = new int[rozmiar];


		for (int nr = 0; nr < powtorzen; ++nr)
		{
			wypelnij(tablica, rozmiar);
			for (int i = 0; i < rozmiar; ++i)
				pattern[i] = tablica[i];
			std::sort(pattern, pattern + rozmiar);
			double c = mierz_czas(tablica, rozmiar, funkcja_sortujaca);
			if (!jest_posortowane(tablica, rozmiar))
			{
				output << "Tablica nieposortowana!!\n";
				if (&output != &std::cerr)
					std::cerr << "Tablica nieposortowana!!\n";
				return;
			}
			if (!std::equal(pattern, pattern + rozmiar, tablica, tablica + rozmiar))
			{
				output << "Tablica zawiera inne wartosci niz powinna!!\n";
				if (&output != &std::cerr)
					std::cerr << "Tablica zawiera inne wartosci niz powinna!!\n";
				return;
			}
			czas += c;
			output.precision(6);
			output << std::fixed << c << "  ";
			output.flush();
		}
		czas /= powtorzen;
		output << std::setw(12) << std::fixed << czas << "\n";
		output.flush();
		delete[] pamiec;
		delete[] pattern;
		if (czas > limit_czasu || rozmiar >= 67108864)
			break;
	}
	output << "\n";
	output.flush();
}
void sortowanie_przez_Wstawienie(int* tablica, int rozmiar)
{
	int pom;
	int j;
	for (int i = 1; i < rozmiar; i++)
	{
		pom = tablica[i];
		j = i - 1;

		while (j >= 0 && tablica[j] > pom)
		{
			tablica[j + 1] = tablica[j];
			--j;
		}
		tablica[j + 1] = pom;
	}
}

void stdsort(int* tablica, int rozmiar)
{
	std::sort(tablica, tablica + rozmiar);
}



void sortowanie_przezWybieranie(int* tablica, int rozmiar)
{


	for (int i = 0; i <= (rozmiar - 1); i++)
	{
		int k = i;
		int x = tablica[i];

		for (int j = i + 1; j <= (rozmiar - 1); j++)
		{
			if (tablica[j] < x)
			{
				k = j;
				x = tablica[j];
			}
		}
		tablica[k] = tablica[i];
		tablica[i] = x;
	}

}
void sortowanie_babelkowe(int* tablica, int rozmiar)
{
	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 1; j < rozmiar - i; j++)
		{
			if (tablica[j - 1] > tablica[j])
			{
				swap(tablica[j - 1], tablica[j]);
			}
		}
	}
}
void sortowanie_shella(int* tablica, int rozmiar)
{
	int h = 1;
	while (h < (rozmiar / 9))
	{
		h = 3 * h + 1;
	}
	while (h > 0)
	{
		for (int i = h; i < rozmiar; i++)
		{
			int x = tablica[i]; int j = i;
			while ((j >= h) && (x < tablica[j - h]))
			{
				tablica[j] = tablica[j - h]; j = j - h;
			}
			tablica[j] = x;
		}
		h = h / 3;
	}
}
void sortowanie_szybkie(int* tablica, int d, int g) {
	if (d < g) {
		int t = tablica[d];
		int s = d;
		for (int i = d; i <= g; i++) {
			if (tablica[i] < t) {
				s = s + 1;
				swap(tablica[s], tablica[i]);
			}
		}
		swap(tablica[d], tablica[s]);
		sortowanie_szybkie(tablica, d, s - 1);
		sortowanie_szybkie(tablica, s + 1, g);
	}
}
void sortowanie_szybkie_start(int* tablica, int rozmiar) {
	sortowanie_szybkie(tablica, 0, rozmiar - 1);
}
void sortowanie_szybkie_modyf(int* tablica, int d, int g)
{
	if (d < (g - 10))
	{
		int t = tablica[d];
		int s = d;
		for (int i = d; i <= g; i++)
		{
			if (tablica[i] < t)
			{
				s = s + 1;
				swap(tablica[s], tablica[i]);
			}
		}
		swap(tablica[d], tablica[s]);
		sortowanie_szybkie_modyf(tablica, d, s - 1);
		sortowanie_szybkie_modyf(tablica, s + 1, g);
	}
}
void sortowanie_szybkie_start_modyf(int* tablica, int rozmiar)
{
	sortowanie_szybkie_modyf(tablica, 0, rozmiar - 1);

	 sortowanie_przez_Wstawienie( tablica,  rozmiar);
}
int main()
{
	setlocale(LC_ALL, "");
	std::ofstream wyniki("wyniki11111.txt");

	std::ostream& output = std::cerr; //zmienic na = wyniki aby zapisywalo do pliku
	eksperyment(sortowanie_szybkie_start, "Sortowanie Szybkie ", output);
	eksperyment(sortowanie_szybkie_start_modyf, "Sortowanie Szybkie modyfikacja", output);
	eksperyment(sortowanie_shella, "Sortowanie Shella", output);
	eksperyment(sortowanie_babelkowe, "Sortowanie babelkowe", output);
	eksperyment(sortowanie_przezWybieranie, "Sortowanie przez wybieranie", output);
	eksperyment(sortowanie_przez_Wstawienie, "Sortowanie przez wstawianie", output);
	eksperyment(stdsort, "Sortowanie std::sort", output);
	return 0;
}