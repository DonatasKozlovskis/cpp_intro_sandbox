#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*****************************************************
 * Compléter le code à partir d'ici
 *****************************************************/

class Piece {
protected:
	string name;
public:
	// constructeur
	Piece(string name)
		: name(name) {};
	//otres methodes
	void afficher(ostream& out) const {
		out << "\"" << name << "\"";
	}

};
ostream& operator<<(ostream& out, Piece const& piece) {
	piece.afficher(out);
	return out;
}


class Simple : public Piece {
protected:
	string orientation;
public:
	Simple(string name)
		: Piece(name) {};
	Simple(string name, string orient)
		: Piece(name), orientation(orient) {}
	//otres methodes
	ostream& afficher(ostream& out) const {
		Piece::afficher(out);
		if (!orientation.empty()) {
			out << " orienté vers la " << orientation;
		}
		out << endl;
		return out;
	}

};
ostream& operator<<(ostream& out, Simple const& simple) {
	out << "pièce simple ";
	simple.afficher(out);
	return out;
}

class Composee : public Piece {
private:
	vector<Simple> pieceList;
public:
	Composee(string newName)
		: Piece(newName) {};
	// other methods
	void construire(Simple const simplePiece) {
		pieceList.push_back(simplePiece);
	}
	ostream& afficher(ostream& out) const {
		Piece::afficher(out);
		out << " composée de :" << endl;
		for (auto piece : pieceList) {
			out << "    * " << piece;
		}
		return out;
	}
};

ostream& operator<<(ostream& out, Composee const& composee) {
	out << "pièce ";
	composee.afficher(out);
	return out;
}


/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

// Sans polymorphisme, c'est difficile de faire autrement :-(...
template <typename T>
void ajouterComposant(T composant, unsigned int nb)
{
  cout << ' ' << nb << " exemplaire";
  if (nb > 1) cout << 's';
  cout << " de " << composant;
}

int main()
{
  cout << "Nous fabriquons une boîte de jeu contenant :" << endl;
  ajouterComposant(Simple("brique standard"), 59);

  // Déclare une pièce composée dont le nom est «porte»
  Composee porte("porte");

  // Une porte est constituée...
  //  ...d'un cadran de porte orienté à gauche
  porte.construire(Simple("cadran porte", "gauche"));
  //  ...et d'un battant orienté à gauche
  porte.construire(Simple("battant", "gauche"));

  // La boite de jeu a pour 2e composant la porte
  ajouterComposant(porte, 1);

  // Déclare une pièce composée dont le nom est «fenêtre»
  Composee fenetre("fenetre");

  // Une fenêtre est constituée...
  //  ...d'un cadran de fenêtre (sans orientation)
  fenetre.construire(Simple("cadran fenetre"));
  //  ...d'un volet orienté à gauche
  fenetre.construire(Simple("volet", "gauche"));
  //  ...d'un volet orienté à droite
  fenetre.construire(Simple("volet", "droite"));

  // La boite de jeu a en plus deux fenêtres
  ajouterComposant(fenetre, 2);

  return 0;
}
