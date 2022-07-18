#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h> 
#include <wctype.h>
#include <locale.h>

int choisir_un_mot(wchar_t le_mot[]);
int compare(wchar_t chaine1[], wchar_t chaine2[]);
wchar_t* saisie(wchar_t chaine[], int longueur);
void vide(wchar_t chaine[]);
int controle(wchar_t lettre[], wchar_t chaine[]);
void ajoute_accents(wchar_t chaine[]);
void viderBuffer();
void passe_en_minuscule(wchar_t chaine[], int longueur);
int saisie_valide (wchar_t chaine[]);

int main (void)
{
    srand(time (NULL));
    setlocale(LC_ALL, "");
    wprintf(L"Bonjour et bienvenue dans le super-pendu-de-la-mort !\n");
    
    int caractere = 0;
    int nb_coups = 10;
    int etat = 0;
    int comp = 0;
    int lettre_valide = 0;
    int lettre_alpha = 0;
    int nombre_de_lettres = 0;
    unsigned int Nb_de_mots = 0;
    wchar_t choix_lettre[5] = L"";
    wchar_t lettres_jouées[30] = L"";
    wchar_t rejouer[1] = L"";
    wchar_t mot_mystere[30] = L"";
    wchar_t mot_cache[30] = L"";
    
    do
    {
        nb_coups = 10;
        nombre_de_lettres = choisir_un_mot(mot_mystere);

        wprintf(L"Devine le mot mystère ! : il contient %d lettres. \n", nombre_de_lettres);

        for (int i = 0 ; i < nombre_de_lettres ; i++)
            mot_cache[i]='*';
        for (int i = 0 ; i < nombre_de_lettres ; i++)
            {    
                if (mot_mystere[i] == L'-')
                {   
                    mot_cache[i] = mot_mystere[i];
                    etat = 1;
                }
            }
        int j = 0;

        do
        {
            etat = 0;
            do
            {
                lettre_alpha = 1;
                lettre_valide = 1;
                wprintf (L"%ls", mot_cache);
                wprintf (L" || Nb de coups restant : %d\n", nb_coups);
                saisie(choix_lettre, 2);
                passe_en_minuscule(choix_lettre, 1);
                ajoute_accents(choix_lettre);
                lettre_alpha = saisie_valide(choix_lettre);
                lettre_valide = controle(choix_lettre, lettres_jouées);
                    if (lettre_valide == 0)
                    {
                        wprintf (L"Tu as déjà saisi : %lc\n", choix_lettre[0]);
                        vide(choix_lettre);
                    }
                    if (lettre_alpha == 0)
                    {
                        wprintf (L"Le caratère saisi n'est pas valide.\n");
                        vide(choix_lettre);
                    }
            } while (lettre_valide == 0 || lettre_alpha == 0);

            lettres_jouées[j] = choix_lettre[0];
            wprintf (L"Tu as saisi : %ls\n", lettres_jouées);
            for (int i = 0 ; i < nombre_de_lettres ; i++)
            {
                
                if (mot_mystere[i] == choix_lettre[0] || mot_mystere[i] == choix_lettre[1] || mot_mystere[i] == choix_lettre[2] || mot_mystere[i] == choix_lettre[3] || mot_mystere[i] == choix_lettre[4])
                {   
                    mot_cache[i] = mot_mystere[i];
                    etat = 1;
                }
            }

            if (etat == 0)
            nb_coups --;
        comp = compare(mot_mystere, mot_cache);
        j++;
        vide(choix_lettre);
        } while (nb_coups != 0 && comp != 0);

        if (comp == 0)
            wprintf (L"Bravo ! ! Le mot mystère était bien %ls\n", mot_mystere);
        else wprintf (L"Domage, tu n'as pas trouvé le mot mystère. C'était %ls\n", mot_mystere);
    
        vide(lettres_jouées);
        vide(mot_cache);
        vide(mot_mystere);

        do
        {
            wprintf(L"Rejouer (O/N) ? ");
            saisie(rejouer, 2);
            passe_en_minuscule(rejouer, 1);
            if (rejouer[0] != L'o' && rejouer[0] != L'n')
                wprintf(L"La saisie n'est pas valide, merci de taper \"o\" ou \"n\".\n");
        } while (rejouer[0] != L'o' && rejouer[0] != L'n');
    } while (rejouer[0] == L'o');

    return EXIT_FAILURE;
}


int choisir_un_mot(wchar_t le_mot[])
{   
    FILE* File_dico = NULL;

    int caractere = 0;
    int Nb_de_mots = 0;
    wchar_t buffer[30] = L"";
    
    File_dico = fopen("dicobin.bin", "rb");
    if (File_dico == NULL)
    EXIT_FAILURE;

    while (caractere != EOF)
    {
        caractere = fgetwc(File_dico); 
        if (caractere == '\n')
        Nb_de_mots++;  // On compte les mots
    }
    
    wprintf(L"\nLe dictionnaire compte %d mots\n", Nb_de_mots);

    rewind(File_dico);          // on rembobine.

    int Numero_de_mot = (rand() % (Nb_de_mots + 1));
    int curseur = 0;

    do {
        caractere = fgetwc(File_dico); 
        if (caractere == '\n')
            {
            curseur++;
            if (curseur == Numero_de_mot)
                caractere = fgetwc(File_dico); 
            }
    }
    while (curseur != Numero_de_mot);

    curseur = 0;
    
    while (caractere != '\n' && caractere != EOF)
    {   
        le_mot[curseur] = caractere;
        caractere = fgetwc(File_dico);
        curseur++;
    };

    fclose (File_dico);
return (curseur);
}

int compare(wchar_t chaine1[], wchar_t chaine2[])
{
    int etat = 0;
    for(int i = 0 ; i < 30 ; i++)
        {
            if (chaine1[i] != chaine2[i])  
            etat = 1;
        }

    return etat; // renvoie 0 si les 2 chaînes sont identiques
}

wchar_t* saisie(wchar_t chaine[], int longueur)
{
    wchar_t *data = NULL;
 
    data = fgetws(chaine, longueur, stdin);
    chaine[longueur - 1] = '\0';
    viderBuffer();

    return data;
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getwchar();
    }
}

void passe_en_minuscule(wchar_t chaine[], int longueur)
{
    for (int i = 0 ; i < longueur ; i++)
    {
        wint_t buff = towlower(chaine[i]);
        chaine[i] = buff;
    }
}

void vide(wchar_t chaine[])
{
    for (int i = 0 ; chaine[i]!='\0' ; i++)
        chaine[i]='\0';
}

int controle(wchar_t lettre[], wchar_t chaine[])
{
    int ok = 1 ;
    for (int i = 0 ; i < wcslen(chaine) ; i++)
    {
        if (lettre[0] == chaine[i])
        { 
            ok = 0 ;
            break;
        }
    }
    return ok;
}

void ajoute_accents(wchar_t chaine[])
{
    if (chaine[0] == L'a' || chaine[0] == L'à' || chaine[0] == L'â' || chaine[0] == L'ä')
    {
        chaine[0] = L'a';
        chaine[1] = L'à';
        chaine[2] = L'â';
        chaine[3] = L'ä';
    }
    if (chaine[0] == L'c' || chaine[0] == L'ç')
        chaine[0] = L'c';
        chaine[1] = L'ç';

    if (chaine[0] == L'e' || chaine[0] == L'ê' || chaine[0] == L'ë' || chaine[0] == L'é' || chaine[0] == L'è')
    {
        chaine[0] = L'e';
        chaine[1] = L'é';
        chaine[2] = L'ê';
        chaine[3] = L'è';
        chaine[4] = L'ë';
    }
    if (chaine[0] == L'i' || chaine[0] == L'î' || chaine[0] == L'ï')
    {
        chaine[0] = L'i';
        chaine[1] = L'î';
        chaine[2] = L'ï';
    }    
    if (chaine[0] == L'o' || chaine[0] == L'ô' || chaine[0] == L'ö')
    {
        chaine[0] = L'o';
        chaine[1] = L'ö';
        chaine[2] = L'ô';
    }   
    if (chaine[0] == L'u' || chaine[0] == L'û' || chaine[0] == L'ü' || chaine[0] == L'ù')
    {
        chaine[0] = L'u';
        chaine[1] = L'ù';
        chaine[2] = L'û';
        chaine[3] = L'ü';
    }

}

int saisie_valide (wchar_t chaine[])
{
    int lettre_valide = 1;

        if (chaine[0] != L'a' && chaine[0] != L'b' && chaine[0] != L'c' && chaine[0] != L'd' && chaine[0] != L'e' && chaine[0] != L'f' && chaine[0] != L'g' && chaine[0] != L'h' && chaine[0] != L'i' && chaine[0] != L'j' && chaine[0] != L'k' && chaine[0] != L'l' && chaine[0] != L'm' && chaine[0] != L'n' && chaine[0] != L'o' && chaine[0] != L'p' && chaine[0] != L'q' && chaine[0] != L'r' && chaine[0] != L's' && chaine[0] != L't' && chaine[0] != L'u' && chaine[0] != L'v' && chaine[0] != L'w' && chaine[0] != L'x' && chaine[0] != L'y' && chaine[0] != L'z')
        lettre_valide = 0;

    return lettre_valide;
}