#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;

int licznik = 0;

void wypełnianie(int* tablica, int m)
{
    for (int i = 0; i <= (m - 1); i++)
    {
        tablica[i] = (-1);
    }
}

int h1(int x, int m)
{
    return x % m;
}
int h2(int x, int m) 
{
    return(((x / m) % (m / 2)) * 2) + 1;
}

int h(int x, int i, int m) 
{
    return (h1(x, m) + i * h2(x, m)) % m;
}

bool hash_Wstaw_podwojnie(int* tablica, int m, int x)
{
    
    int k;
    int h1;
    int h2;
    for (int i = 0; i < m; i++)
    {
        h1 = x % m;
        h2 = (((x / m) % (m / 2)) * 2) + 1;
        k = (h1 + i * h2) % m;
        if (tablica[k] == -1)
        {
            tablica[k] = x;
            licznik++;
            return true;
        }
        licznik++;
    }
    return false;
}


bool hash_al_wstaw(int* tablica, int m, int x)
{
    int k;
    int h;
    for (int i = 0; i < m; i++)
    {
        h = x % m;
        k = (h + i) % m;
        if (tablica[k] == -1)
        {
            tablica[k] = x;
            licznik++;
            return true;
      }
        licznik++;
    }

    return false;
}


bool hash_al_szukaj(int* tablica, int m, int x)
{
    for (int i = 0; i < m - 1; i++)
    {
        int k = h(x,i,m);
        licznik++;


        if (tablica[k] == x)
        {
            licznik++;
            return true;
            
        }

        if (tablica[k] == -1)
        {
            licznik++;
            return false;

        }

    }

    return false;
}
bool hash_al_szukaj_mieszaj(int* tablica, int m, int x)
{
    int k;
    int h1;
    int h2;
    for (int i = 0; i < m - 1; i++) {
        h1 = x % m;
        h2 = (((x / m) % (m / 2)) * 2) + 1;
        k = (h1 + i * h2) % m;
        if (tablica[k] = x) 
        {
            licznik++;
            return true;
        }
        else if (tablica[k] == -2) 
        {
            licznik++;
            return false;
        }
    }
    return false;
}

void miszanie(int* tablica, int l)
{
    for (int i = 0; i < l; i++)
    {
        tablica[i] = i;
    }
    random_shuffle(tablica, tablica + l);

}

void czas_mierz(int* tablica, int rozmiar, int* tab_wym, int element) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = element; i < element + 10000; i++)
    {
       //hash_al_szukaj(tablica, rozmiar, tab_wym[i]);
        //hash_Wstaw_podwojnie(tablica, rozmiar, tab_wym[i]);
        //hash_wstaw(tablica, rozmiar, tab_wym[i]);
        hash_al_szukaj_mieszaj(tablica, rozmiar, tab_wym[i]);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    std::cerr << "Uplynelo: " << duration.count() << "us\n" << "wykonano " << licznik << " razy";
}
int main()
{
    int element10000;
    int m = 1048576;
    int l = 12000000;
    int dzp = 0;
    int* tablica = new int[m]; // tablica mieszająca
    int* tablica_miesz = new int[l]; // tablica z danymi
    wypełnianie(tablica, m);
    miszanie(tablica_miesz, l);


    for (int a = 0; a < m; a++)
    {
        //hash_wstaw(tablica, m, tablica_miesz[a]);
        //hash_al_szukaj(tablica, m, tablica_miesz[a]);
        hash_al_szukaj_mieszaj(tablica, m, tablica_miesz[a]);
        //hash_Wstaw_podwojnie(tablica, m, tablica_miesz[a]);
        if (a >= m * 0)
        {
            czas_mierz(tablica, m, tablica_miesz, a);
            break;
        }
    }
}