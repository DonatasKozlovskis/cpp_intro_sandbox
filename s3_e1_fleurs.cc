#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* Classe représentant les "fleurs".
 * Une instance de cette classe hors Bouquet est un exemple / une catégorie
 * un type de de fleurs.
 * Les fleurs "réelles" sont les instances présentes dans les bouquets.
 */
class Fleur
{
	/*******************************************
	* Complétez le programme à partir d'ici.
	*******************************************/
public:
	Fleur(string nom, string couleur, double prix_base, bool parfum = false, bool promotion = false)
		: nom(nom), couleur(couleur), prix_base(prix_base),
		parfum_(parfum), promotion(promotion){}

	double prix() const {
		return (promotion) ? (prix_base / 2) : (prix_base);
	}
	bool parfum() const {
		return parfum_;
	}
	void affiche(ostream& sortie) const {
		if (parfum_) {
			sortie << nom << " " << couleur << " parfumée, prix : " << prix() << " CHF";
		} else {
			sortie << nom << " " << couleur << ", prix : " << prix() << " CHF";
		}
	}
	bool operator==(const Fleur &other) const {
		// compare
		if (nom == other.nom && couleur == other.couleur && parfum_ == other.parfum_)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
private:
	string nom;
	string couleur;
	double prix_base;
	bool parfum_;
	bool promotion;
};
ostream& operator<<(ostream& os, const Fleur& fleur) {
	fleur.affiche(os);
	return os;
}

class Bouquet
{

public:
	Bouquet(){}

	double prix() const {
		double total = 0;
		for (auto&& fleur : bouquet) {
			total = total + fleur.prix();
		}
		return parfum() ? total * 2 : total;
	}
	bool parfum() const {
		bool parfum = false;
		for (auto&& fleur : bouquet) {
			parfum = parfum || fleur.parfum();
		}
		return parfum;
	}
	void affiche(ostream& sortie) const {
		if ( !bouquet.empty() ) {
			sortie << "Bouquet";
			parfum() ? sortie << " parfumé" : sortie << "";
			sortie << " composé de :" << endl;
			for (auto&& fleur : bouquet) {
				sortie << fleur << endl;
			}
			sortie << "Prix total : " << prix() << " CHF" << endl;
		}
		else {
			sortie << "Encore aucune fleur dans le bouquet !" << endl;
		}
	}
	Bouquet& operator+=(const Fleur& fleur) {
		bouquet.push_back(fleur);
		return *this;
	}
	Bouquet& operator-=(const Fleur& fleur) {
		auto it = bouquet.begin();

		while (it != bouquet.end()) {
			if (*it == fleur)
				it = bouquet.erase(it);
			else
				++it;
		}
		return *this;
	}
	Bouquet& operator+(const Fleur& fleur) {
		return operator+=(fleur);
	}
	Bouquet& operator-(const Fleur& fleur) {
		return operator-=(fleur);
	}
private:
	vector<Fleur> bouquet;
};
ostream& operator<<(ostream& os, const Bouquet& bouquet) {
	bouquet.affiche(os);
	return os;
}


/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  Fleur r1("Rose", "jaune", 1.5); // exemple de rose jaune inodore
  cout << r1 << endl;

  Fleur r2("Rose", "jaune", 3.0, true); // exemple de rose jaune parfumée
  Fleur r3("Rose", "rouge", 2.0, true, true); // exemple de rose rouge parfumée en promo
  Bouquet b1;
  b1 += r1; // ajoute une fleur de type r1
  b1 += r1; // ajoute aurte une fleur de type r1
  b1 += r2;
  b1 += r3;
  cout << b1 << endl;

  b1 = b1 - r1; // supprime toutes les fleurs de même type que r1
  cout << b1 << endl;

  Bouquet b2;
  b2 = b1 + r1; // remet une fleur de type r1
  cout << b2 << endl;

  // supprime toutes les fleurs parfumées
  b2 -= r2;
  b2 -= r3;
  cout << b2;

  return 0;
}
