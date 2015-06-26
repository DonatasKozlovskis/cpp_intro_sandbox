#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int distance(int x, int y)
{
  return abs(x - y);
}

class Creature
{
  /*****************************************************
   * Compléter le code à partir d'ici
   *****************************************************/
protected:
	const string nom_;
	int niveau_; //level
	int points_de_vie_;
	int force_;
	int position_;
private:
public:
	//constructor
	Creature(string name, int level, int health_points, int force, int position = 0)
		: nom_(name), niveau_(level), points_de_vie_(health_points), force_(force), position_(position) {}

	int position() const {
		return position_;
	}
	bool vivant() const {
		return (points_de_vie_ > 0) ? true : false;
	}
	// attack points
	int points_attaque() const {
		return (vivant()) ? (niveau_*force_) : (0);
	}
	void deplacer(int step) {
		if (vivant()) {
			position_ += step;
		}
	}
	void adieux() const {
		cout << nom_ << " n'est plus !" << endl;
	}


	void faiblir(int points) {
		if (vivant()) {
			points_de_vie_ -= points;
		}
		// after attack:
		if (!vivant()) {
			points_de_vie_ = 0;
			adieux();
		}
	}
	void  afficher() const {
		cout << nom_ << ", niveau: " << niveau_ << ", points de vie: " << points_de_vie_ << ", force: " << force_ << \
			", points d'attaque: " << points_attaque() << ", position: " << position_ << endl;
	}
};

class Dragon : public Creature {
private:
	int portee_flamme_;
public:
	Dragon(string name, int level, int health_points, int force, int range_flame, int position = 0)
		: Creature(name, level, health_points, force, position), portee_flamme_(range_flame) {}

	void voler(int pos) {
		deplacer(pos - position_);
	}
	// blow simulation
	void souffle_sur(Creature& bete) {
		int d = abs(position_ - bete.position());
		if (vivant() && bete.vivant() && d <= portee_flamme_) {
			bete.faiblir(points_attaque());
			faiblir(d);
			// after fight
			if (vivant() && !bete.vivant()) {
				niveau_ += 1;
			}
		}
	}
};

class Hydre : public Creature {
private:
	int longueur_cou_;
	int dose_poison_;
public:
	Hydre(string name, int level, int health_points, int force, int neckLenght, int dose, int position = 0)
		: Creature(name, level, health_points, force, position), longueur_cou_(neckLenght), dose_poison_(dose) {}

	void empoisonne(Creature& bete) {
		int d = abs(position_ - bete.position());
		if (vivant() && bete.vivant() && d <= longueur_cou_) {
			bete.faiblir(points_attaque() + dose_poison_);
			// after fight
			if (vivant() && !bete.vivant()) {
				niveau_ += 1;
			}
		}
	}
};

void combat(Dragon & dragon, Hydre & hydre) {
	hydre.empoisonne(dragon);
	dragon.souffle_sur(hydre);
}
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/
int main()
{
  Dragon dragon("Dragon rouge"   , 2, 10, 3, 20         );
  Hydre  hydre ("Hydre maléfique", 2, 10, 1, 10, 1,  42 );

  dragon.afficher();
  cout << "se prépare au combat avec :" << endl;
  hydre.afficher();

  cout << endl;
  cout << "1er combat :" << endl;
  cout << "    les créatures ne sont pas à portée, donc ne peuvent pas s'attaquer."
       << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  cout << endl;
  cout << "Le dragon vole à proximité de l'hydre :" << endl;
  dragon.voler(hydre.position() - 1);
  dragon.afficher();

  cout << endl;
  cout << "L'hydre recule d'un pas :" << endl;
  hydre.deplacer(1);
  hydre.afficher();

  cout << endl;
  cout << "2e combat :" << endl;
  cout << "\
  + l'hydre inflige au dragon une attaque de 3 points\n\
      [ niveau (2) * force (1) + poison (1) = 3 ] ;\n\
  + le dragon inflige à l'hydre une attaque de 6 points\n\
      [ niveau (2) * force (3) = 6 ] ;\n\
  + pendant son attaque, le dragon perd 2 points de vie supplémentaires\n\
       [ correspondant à la distance entre le dragon et l'hydre : 43 - 41 = 2 ].\
" << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  cout << endl;
  cout << "Le dragon avance d'un pas :" << endl;
  dragon.deplacer(1);
  dragon.afficher();

  cout << endl;
  cout << "3e combat :" << endl;
  cout << "\
  + l'hydre inflige au dragon une attaque de 3 points\n\
      [ niveau (2) * force (1) + poison (1) = 3 ] ;\n\
  + le dragon inflige à l'hydre une attaque de 6 points\n\
      [ niveau (2) * force (3) = 6 ] ;\n\
  + pendant son attaque, le dragon perd 1 point de vie supplémentaire\n\
       [ correspondant à la distance entre le dragon et l'hydre : 43 - 42 = 1 ] ;\n\
  + l'hydre est vaincue et le dragon monte au niveau 3.\
" << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  cout << endl;
  cout << "4e Combat :" << endl;
  cout << "    quand une créature est vaincue, rien ne se passe." << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  return 0;
}
