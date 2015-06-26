#include <iostream>
#include <string>
#include <array>
#include <vector>
using namespace std;

/*******************************************
 * Completez le programme à partir d'ici.
 *******************************************/

 
//======================================================================
class Mise {
public:
	Mise(int mise)
		: mise_(mise)
	{}
	virtual ~Mise() {
	};

	virtual int mise() const final {
		return mise_;
	}
	// pure virtual method must be provided in subclass
	virtual int gain(int winning_number) const = 0;
private:
	int mise_;
};
//======================================================================
class Pleine : public Mise {
public:
	Pleine(int mise, int number)
		: Mise(mise), number(number)
	{}

	virtual ~Pleine() {}

	virtual int gain(int winning_number) const {
		return (winning_number == number ? multFactor*mise() : 0);
	}

private:
	int number;
	static int multFactor;
};
int Pleine::multFactor = 35;

//======================================================================
class Rouges : public Mise {
public:
	Rouges(int mise)
		: Mise(mise)
	{}
	virtual ~Rouges() {}

	virtual int gain(int winning_number) const {
		bool redWins = false;
		for (auto square : redSquares) {
			if (winning_number == square) {
				redWins = true;
				break;
			}
		}

		return (redWins ? mise() : 0);
	}

private:
	static array<int, 18> redSquares;
};
array<int, 18> Rouges::redSquares = { 1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36 };

//======================================================================
class Joueur {
public:
	// constructor
	Joueur(string name)
		: name(name)
	{}
	// delete copy constructor
	Joueur(const Joueur& ) = delete;
	// no assign
	Joueur& operator=(const Joueur&) = delete;

	// destructor
	~Joueur() {
		strategie = nullptr;
	}
	// methods
	string nom() const {
		return name;
	}
	// changes the player's strateg
	void set_strategie(Mise *newStrategy) {
		strategie = newStrategy;
	}
	int mise() const {
		return (strategie != nullptr ? strategie->mise() : 0);
	}
	int gain(int winning_number) const {
		return (strategie != nullptr ? strategie->gain(winning_number) : 0);
	}
private:
	const string name;
	Mise * strategie = nullptr;
};


//======================================================================
class Roulette {
public:
	Roulette(){}

	//force class to be pure virtual
	virtual ~Roulette() = 0;

	// delete copy constructor
	Roulette(const Roulette&) = delete;
	// no assign
	Roulette& operator=(const Roulette&) = delete;


	virtual void participe(const Joueur& newPlayer) {
		bool alreadyPlays = false;

		bool redWins = false;
		for (auto player : players) {
			if (player == &newPlayer) {
				alreadyPlays = true;
				break;
			}
		}
		if (!alreadyPlays) {
			players.push_back(&newPlayer);
		}
	}
	virtual int tirage() const final {
		return numberDrawn;
	}
	virtual int nombre_participants() const final {
		return players.size();
	}

	virtual void rien_ne_va_plus(int valeur) {
		numberDrawn = valeur;
	}
	// pure virtual
	//  calculating how much each player loses of his/her bet (which is retained by the casino)
	virtual int perte_mise(int player_bet) const = 0;

	virtual int gain_maison() const {
		int userGain = 0;
		int userLoss = 0;

		for (auto const& player : players) {
			if (player->gain(numberDrawn) > 0) {
				userGain += player->gain(numberDrawn);
			}
			else {
				userLoss += perte_mise(player->mise());
			}
		}
		return userLoss - userGain;
	}

	// pure virtual
	virtual void afficher(ostream& out) const = 0;

	virtual void annoncer() const {
		cout << "Croupier : le numéro du tirage est le " << numberDrawn << endl;
		for (auto const& player : players) {
			cout << "Le joueur " << player->nom();
			cout << " a misé " << player->mise() << " et ";
			if (player->gain(numberDrawn) > 0) {
				cout << "gagne " << player->gain(numberDrawn);
			}
			else {
				cout << "perd " << perte_mise(player->mise());
			}
			cout << endl;
		}
		cout << "Gain/perte du casino : " << gain_maison() << endl;
	}

private:
	vector<const Joueur*> players;
	int numberDrawn;
};
ostream& operator<<(ostream& out, Roulette const& game) {
	game.afficher(out);
	return out;
}
Roulette::~Roulette() {}

//======================================================================
class RouletteFrancaise : public Roulette {
public:
	virtual void afficher(ostream& out) const override {
		out << "Roulette française";
	}
	virtual int perte_mise(int player_bet) const override {
		return player_bet;
	}

private:
};

//======================================================================
class RouletteAnglaise : public Roulette {
public:
	virtual void afficher(ostream& out) const override {
		out << "Roulette anglaise";
	}
	virtual int perte_mise(int player_bet) const override {
		return 0.5 * player_bet;
	}
	void participe(const Joueur& player) override {
		if (nombre_participants() < 10) {
			Roulette::participe(player);
		}
	}

private:
};

/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

void simuler_jeu(Roulette& jeu)
{
  cout << jeu << " :" << endl;
  for (int tirage : array<int, 3>({ 12, 1, 31 })) {
    jeu.rien_ne_va_plus(tirage);
    jeu.annoncer();
    cout << endl;
  }
  cout << endl;
}

int main()
{
  Joueur joueur1("Dupond");
  Joueur joueur2("Dupont");
  Joueur joueur3("Dupond"); // un AUTRE joueur, du même nom

  Pleine p1(100, 1); // miser 100 jetons sur le 1
  Rouges p2(30);     // miser  30 jetons sur les rouges
  Pleine p3(20, 13); // miser  20 jetons sur le 13

  joueur1.set_strategie(&p1);
  joueur2.set_strategie(&p2);
  joueur3.set_strategie(&p3);

  RouletteAnglaise  jeu1;
  RouletteFrancaise jeu2;

  for (auto jeu : array<Roulette*, 2>({ &jeu1, &jeu2 })) {
    jeu->participe(joueur1);
    jeu->participe(joueur2);
    jeu->participe(joueur3); // c'est un autre joueur
    jeu->participe(joueur2); // c'est déjà un joueur du jeu
    simuler_jeu(*jeu);
  }

  return 0;
}
