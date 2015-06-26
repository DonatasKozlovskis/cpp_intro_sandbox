#include <vector>
#include <iostream>
#include <string>
using namespace std;

/*******************************************
 * Completez le programme à partir d'ici.
 *******************************************/

class Affichable {
public:
	// pure virtual method must be provided in subclass
	virtual void affiche(ostream& sortie) const = 0;
};
//----------------------------------------------------------------------
ostream& operator<<(ostream& sortie, Affichable const& obj) {
	obj.affiche(sortie);
	return sortie;
}

class OptionVoyage : public Affichable {
public:
	OptionVoyage(string name, double price)
		: name(name), price(price)
	{}

	virtual ~OptionVoyage() {}

	virtual string nom() const {
		return name;
	}
	virtual double prix() const {
		return price;
	}

	virtual void affiche(ostream& sortie) const {
		// Par défaut, un produit affiche son nom et prix.
		sortie << nom() << " -> " << prix() << " CHF";
	}

private: // Encapsulons un maximum....
	string name;
	double price;
};

//======================================================================
class Sejour : public virtual OptionVoyage {
public:
	Sejour(string name, double price, int numOfNights, double pxPerNight)
		: OptionVoyage(name, price), numberOfNights(numOfNights), priceForNight(pxPerNight)
	{}

	virtual double prix() const override {
		return OptionVoyage::prix() + numberOfNights*priceForNight;
	}
private:
	int numberOfNights;
	double priceForNight;
};

//======================================================================
class Transport : public virtual OptionVoyage {
	static const double  TARIF_LONG;
	static const double  TARIF_BASE;
public:
	Transport(string name, double price, bool longTrip = false)
		: OptionVoyage(name, price), longTrip(longTrip)
	{}

	virtual double prix() const override {
		return OptionVoyage::prix() + (longTrip ? TARIF_LONG : TARIF_BASE);
	}
private:
	bool longTrip;

};
const double Transport::TARIF_LONG = 1500.0;
const double Transport::TARIF_BASE = 200.0;

//======================================================================
class OptionCombinee : public Transport, public Sejour {
public:
	OptionCombinee(string name, double price, int numOfNights, double pxPerNight, bool longTrip = false)
		: OptionVoyage(name, price)
		, Transport(name, price, longTrip)
		, Sejour(name, price, numOfNights, pxPerNight)
	{}
	virtual double prix() const override {
		return 0.75* (Sejour::prix() + Transport::prix());
	}
private:
	bool longTrip;
};
//======================================================================
class KitVoyage : public Affichable {
public:
	KitVoyage(string departure, string destination)
		: departure(departure), destination(destination)
	{}
	double prix() const {
		double prix_final = 0;
		for (auto const& part : voyages) {
			prix_final += part->prix();
		}
		return prix_final;
	}

	void ajouter_option(const OptionVoyage & option) {
		voyages.push_back(&option);
	}
	void annuler() {
		voyages.clear();
	}
	virtual void affiche(ostream& out) const {
		out << "Voyage de " << departure << " à " << destination;
		if (voyages.empty()) {
			out << ":  vous n'avez rien réservé !" << endl;
		}
		else {
			out << ", avec pour options : " << endl;
			for (auto const& part : voyages) {
				out << "   - " << *part << endl;
			}
			out << "Prix total : " << prix() << " CHF" << endl;
		}

	}
private:
	vector<const OptionVoyage *> voyages;
	string departure;
	string destination;
};



/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

int main()
{
  Transport transp1("Trajet en train", 50.0);
  Transport transp2("Trajet en avion", 50.0, true);

  Sejour sejour1("Hotel 3* : Les amandiers ", 40.0, 5, 100.0);
  Sejour sejour2("Hotel 4* : Ambassador Plazza  ", 100.0, 2, 250.0);

  OptionCombinee combi("Hotel 4* : Ambassador Plazza et trajet en avion",
                       100.0, 2, 250.0, true);

  KitVoyage kit1("Zurich", "Paris");
  kit1.ajouter_option(transp1);
  kit1.ajouter_option(sejour1);
  cout << kit1 << endl;

  KitVoyage kit2("Zurich", "New York");
  kit2.ajouter_option(transp2);
  kit2.ajouter_option(sejour2);
  cout << kit2 << endl;

  KitVoyage kit3("Zurich", "New York");
  kit3.ajouter_option(combi);
  cout << kit3 << endl;

  kit1.annuler();
  kit2.annuler();
  cout << kit1 << endl;
  cout << kit2 << endl;

  return 0;

}
