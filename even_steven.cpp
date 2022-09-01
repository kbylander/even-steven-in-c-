//---------------------------------------------
// Filnamn: even_steven.cpp
// Syfte:   Skapa en evensteven med dynamiska arrays och klasser.
// Indata:  Transaktioner (textfil) samt skrivbordsinput.
// Utdata:  Information ang�ende transaktionerna, skulder osv.
//
// Programmerare: Karl Bylander
// Datum: 2022-08-16
//---------------------------------------------
//

#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

//globala konstanter

const int MAX_PERSONER=10;
const int MAX_TRANSAKTIONER=30;

//-----------------------------------------------------------------------------------------------

//klassdeklarationer

class Person {
 private:
   string namn;
   double betalat_andras;
   double skyldig;

 public:
   Person();
   Person( const string &namn_in, double betalat_andras_in, double skyldig_in);
   string hamtaNamn();
   double hamtaBetalat();
   double hamtaSkyldig();
   void   skrivUt(ostream &os);
};

 //-----------------------------------------------------------------------------------------------

 class Transaktion
 {
 private:
   string  datum;
   string  typ;
   string  namn;
   double  belopp;
   int     antal_kompisar;
   string *kompisar;

  public:
   Transaktion();
   ~Transaktion();
   Transaktion& operator=( const Transaktion& t);
   string hamtaNamn();
   double hamtaBelopp();
   int    hamtaAntalKompisar();
   bool   finnsKompis(const string &namn);
   bool   lasIn(istream &is);
   void   skrivUt(ostream &os);
   void   skrivTitel(ostream &os);
 };

//-----------------------------------------------------------------------------------------------

class PersonLista
{
private:
    int antal_personer;
    Person *personer;

public:
    PersonLista();
    PersonLista(const PersonLista &p);
    void laggTill(Person & person);
    void skrivUtOchFixa(ostream & os);
    double summaSkyldig();
    double summaBetalat();
    bool finnsPerson(const string& namn);
    };

//-----------------------------------------------------------------------------------------------

  class TransaktionsLista
 {
 private:
   int          antal_transaktioner=0;
   Transaktion *transaktioner;

 public:
    TransaktionsLista();
   ~TransaktionsLista();
   TransaktionsLista( const TransaktionsLista & t );
    void   lasIn(istream & is);
    void   skrivUt(ostream & os);
    void   laggTill(Transaktion & t);
    double totalKostnad() const;
    double liggerUteMed(const string &namn);
    double arSkyldig(const string &namn);
    PersonLista FixaPersoner();
 };

 //-----------------------------------------------------------------------------------------------

 //huvudskriptet

int main()
{
  cout << "Startar med att l�sa fr�n en fil." << endl;

  TransaktionsLista transaktioner;
  std::ifstream     is("resa.txt");
  transaktioner.lasIn(is);

  int operation = 1;
  while (operation != 0)
    {
      cout << endl;
      cout << "V�lj i menyn nedan:" << endl;
      cout << "0. Avsluta. Alla transaktioner sparas p� fil." << endl;
      cout << "1. Skriv ut information om alla transaktioner." << endl;
      cout << "2. L�s in en transaktion fr�n tangentbordet." << endl;
      cout << "3. Ber�kna totala kostnaden." << endl;
      cout << "4. Hur mycket �rr en viss person skyldig?" << endl;
      cout << "5. Hur mycket ligger en viss person ute med?" << endl;
      cout << "6. Lista alla personer mm och FIXA" << endl;

      cin >> operation;
      cout << endl;

      switch (operation)
        {
          case 1:
            {
              transaktioner.skrivUt(cout);
              break;
            }
          case 2:
            {
              Transaktion transaktion;
              cout << "Ange transaktion i f�ljande format" << endl;
              transaktion.skrivTitel(cout);
              transaktion.lasIn(cin);
              transaktioner.laggTill(transaktion);
              break;
            }
          case 3:
            {
              cout << "Den totala kostnanden f�r resan var "
                   << transaktioner.totalKostnad() << endl;
              break;
            }
          case 4:
            {
              cout << "Ange personen: ";
              string namn;
              cin >> namn;
              double ar_skyldig = transaktioner.arSkyldig(namn);
              if (ar_skyldig == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
              else
                cout << namn << " �r skyldig " << ar_skyldig << endl;
              break;
            }
          case 5:
            {
              cout << "Ange personen: ";
              string namn;
              cin >> namn;
              double ligger_ute_med = transaktioner.liggerUteMed(namn);
              if (ligger_ute_med == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
              else
                cout << namn << " ligger ute med " << ligger_ute_med << endl;
              break;
            }
          case 6:
            {
              cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
              PersonLista lista;
              lista = transaktioner.FixaPersoner();
              lista.skrivUtOchFixa(cout);
              break;
            }
        }
    }

  std::ofstream os("transaktioner.txt");
  transaktioner.skrivUt(os);

  return 0;
}

//-----------------------------------------------------------------------------------------------

//klassimplementationen

//konstruktor
Person::Person()
{
    string namn="";
    double betalat_andras=0;
    double skyldig=0;
}

//�verlagrad konstruktor

Person::Person(const string &namn_in, double betalat_andras_in, double skyldig_in): namn(namn_in),
betalat_andras(betalat_andras_in), skyldig(skyldig_in)
{}

//selektorer

string Person::hamtaNamn(){return namn;}

double Person::hamtaBetalat(){return betalat_andras;}

double Person::hamtaSkyldig(){return skyldig;}

//utskriftsfunktion
void Person::skrivUt(ostream &os)
{
    os << hamtaNamn() << " har betalat " << hamtaBetalat() << ", �r skyldig " << hamtaSkyldig();
    if (hamtaBetalat() > hamtaSkyldig())
    {
        os << ", och ska d�rf�r ha " << abs(hamtaSkyldig()-hamtaBetalat())<< " fr�n potten!" << endl;
    }
    else
    {
        os << ", och �r d�rf�r skyldig potten " << abs(hamtaSkyldig()-hamtaBetalat()) << endl;
    }
}
//-----------------------------------------------------------------------------------------------

//konstruktorn
PersonLista::PersonLista(): antal_personer(0), personer(0)
{}

//kopieringskonstruktor
PersonLista::PersonLista(const PersonLista & PL):
antal_personer(PL.antal_personer)
{
    if (antal_personer > 0)
    {
        personer = new Person[antal_personer];
        for (int i=0; i<antal_personer; i++)
        {
            personer[i] = PL.personer[i];
        }
    }
    else
    {
        personer=0;
    }
}

//L�gger till en person i den dynamiska listan
void PersonLista::laggTill(Person & person)
{
    if (antal_personer < MAX_PERSONER)
    {
        Person *temp=0;
        temp = new Person[antal_personer+1];
        for (int i=0; i< antal_personer; i++)
        {
            temp[i] = personer[i];
        }

        delete [] personer;
        antal_personer++;
        personer = temp;
        personer[antal_personer-1] = person;
    }
}

//utskriftsfunktion, anv�nder sig av funktionen i Person klassen
void PersonLista::skrivUtOchFixa(ostream & os)
{

    for (int i=0; i<antal_personer; i++)
    {
        personer[i].skrivUt(os);
    }
    if (summaBetalat() == summaSkyldig())
    {
        cout << endl <<"det gick j�mnt ut.";
    }
    else
    {
        cout << endl << "det gick inte j�mnt ut.";
    }
}

//summar totala skulden, anv�nder sig av en selektor i Person klassen.
double PersonLista::summaSkyldig()
{
    double sum_skyldig=0.;

    for (int i=0; i<antal_personer; i++)
    {
        sum_skyldig+=personer[i].hamtaSkyldig();
    }
    return sum_skyldig;
}

//summar totala kostnaden, anv�nder sig av en selektor i Person klassen.
double PersonLista::summaBetalat()
{
    double sum_betalat=0.;
    for (int i=0; i<antal_personer; i++)
    {
        sum_betalat+=personer[i].hamtaBetalat();
    }

    return sum_betalat;
}

//Hitta r�tt namn i personlistan.
bool PersonLista::finnsPerson(const string& namn)
{
    for (int i=0; i<antal_personer; i++)
    {
        if (personer[i].hamtaNamn()==namn)
        {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------------------------

//selektorn
 Transaktion::Transaktion()
{
    string datum="";
    string typ="";
    string namn="";
    double belopp=0.;
    int    antal_kompisar=0;
    kompisar=0;
}

//destruktorn
Transaktion::~Transaktion()
{
    delete [] kompisar;
    kompisar=0;
}

//Tilldelningsoperatorn
Transaktion& Transaktion::operator=( const Transaktion& t)
{
    if (this != &t)
    {
        delete[] kompisar;
        datum          = t.datum;
        typ            = t.typ;
        namn           = t.namn;
        belopp         = t.belopp;
        antal_kompisar = t.antal_kompisar;
        kompisar       = new string[antal_kompisar];
        for (int i=0; i < antal_kompisar; i++)
        {
            kompisar[i] = t.kompisar[i];
        }
    }
    return *this;
}

//Selektorer

string Transaktion::hamtaNamn()
{
    return namn;
}

int Transaktion::hamtaAntalKompisar()
{
    return antal_kompisar;
}

double Transaktion::hamtaBelopp()
{
    return belopp;
}

//Finns kompisens namn i bland de objektet betalat f�r.
bool Transaktion::finnsKompis(const string &namn)
{
    for (int i=0; i<antal_kompisar; i++)
    {
        if (kompisar[i]==namn)
        {
            return true;
        }
    }
    return false;
}

//l�ser in ny Transaktion
bool Transaktion::lasIn(istream &is)
{
    is >> datum >> typ >> namn >> belopp >> antal_kompisar;

    kompisar = new string[antal_kompisar];

    for (int i=0; i< antal_kompisar; i++)
    {
        is >> kompisar[i];
    }
    return !is.eof();
}

//utskriftsfunktioner

void Transaktion::skrivTitel(ostream &os)
{
    os << "Datum:\t Typ:\t Namn:\t Belopp:  Antal:  Lista av namn:  " << endl;
}

void Transaktion::skrivUt(ostream &os)
{
    os << datum << "\t" << typ << "\t" << hamtaNamn() << "\t" << hamtaBelopp() << "\t" << hamtaAntalKompisar() << "\t";
    for (int i=0; i<antal_kompisar; i++)
    {
        os << kompisar[i] << "\t";
    }
    os << endl;
}
//-----------------------------------------------------------------------------------------------

//selektorn
TransaktionsLista::TransaktionsLista()
{
    antal_transaktioner = 0;
    transaktioner = 0;
}

//destruktorn
TransaktionsLista::~TransaktionsLista()
{
    delete[] transaktioner;
    transaktioner = 0;
}

//Kopieringskonstruktor.
TransaktionsLista::TransaktionsLista( const TransaktionsLista & t )
  : antal_transaktioner(t.antal_transaktioner)
{
  if ( antal_transaktioner > 0 )
    {
      transaktioner = new Transaktion[antal_transaktioner];

      for ( int i = 0; i < antal_transaktioner; i++ )
	transaktioner[i] = t.transaktioner[i];
    }
  else
    transaktioner = 0;
}

//L�ser in en ny Transaktion
void   TransaktionsLista::lasIn(istream & is)
{
    Transaktion t;

    while (t.lasIn(is))
    {
        laggTill( t );
    }
}

//l�gger till ett Transaktion's objekt i dynamiska arrayn
void TransaktionsLista::laggTill(Transaktion &t)
{
    Transaktion *temp = 0;
    temp = new Transaktion[antal_transaktioner+1];

    for(int i=0; i<antal_transaktioner; i++)
    {
        temp[i]=transaktioner[i];
    }

    delete[] transaktioner;

    transaktioner = temp;
    antal_transaktioner++;
    transaktioner[antal_transaktioner-1] = t;
}

//skriv ut funktion, anv�nder sig av Transaktion klassens.
void   TransaktionsLista::skrivUt(ostream & os)
{
    transaktioner[1].skrivTitel(os);
    for (int i=0; i<antal_transaktioner; i++)
    {
        transaktioner[i].skrivUt(os);
    }
}

//ber�knar totala kostnaden f�r resan.
double TransaktionsLista::totalKostnad() const
{
    double tot_kost=0.;
    for (int i=0; i<antal_transaktioner; i++)
    {
        tot_kost+=transaktioner[i].hamtaBelopp();
    }
    return tot_kost;
}

//Ber�knar hur mycket "namn" ligger ute med.
double TransaktionsLista::liggerUteMed(const string &namn)
{
    double summa = 0.;
    for (int i = 0; i < antal_transaktioner; i++)
    {
    if (transaktioner[i].hamtaNamn() == namn){
        summa += transaktioner[i].hamtaBelopp() *
            (1.0 - 1.0 / (transaktioner[i].hamtaAntalKompisar() + 1));
    }
    }
    return summa;
 }

 //ber�knar hur mycket "namn" �r skyldig
double TransaktionsLista::arSkyldig(const string &namn)
{
    double summa=0.0;
    for (int i=0; i<antal_transaktioner; i++)
    {
        if (transaktioner[i].finnsKompis(namn))
        {
            summa += transaktioner[i].hamtaBelopp() *
            (1.0 / (transaktioner[i].hamtaAntalKompisar() + 1));

        }
    }

    return summa;
}

//funktionen ordnar s� att alla personer bara finns med en g�ng samt summerar samtligas utgifter och skulder.
PersonLista TransaktionsLista::FixaPersoner()
{
    PersonLista lista;
    for (int i=0; i<antal_transaktioner; i++)
    {
        if (!lista.finnsPerson(transaktioner[i].hamtaNamn()))
        {
            string namn=transaktioner[i].hamtaNamn();
            Person person(namn, liggerUteMed(namn), arSkyldig(namn));
            lista.laggTill(person);
        }
    }
    return lista;
}

//-----------------------------------------------------------------------------------------------
