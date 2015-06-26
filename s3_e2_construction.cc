#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Pour simplifier
typedef string Forme   ;
typedef string Couleur ;

class Brique
{
private:
  Forme   forme   ;
  Couleur couleur ;

public:
  /*****************************************************
    Compléter le code à partir d'ici
  *******************************************************/
	Brique() = delete;
	Brique(Forme forme, Couleur couler)
		: forme(forme), couleur(couler) {};
	ostream& afficher(ostream& sortie) const;
};
ostream& Brique::afficher(ostream& sortie) const 
{
	if (!couleur.empty())
	{
		sortie << "(" << forme << ", " << couleur << ")";
	}
	else
	{
		sortie << forme;
	}
	return sortie;
}
ostream& operator<<(ostream& sortie, Brique const& brique)
{
	brique.afficher(sortie);
	return sortie;
}

class Construction
{
private:
	
public:
	vector< vector< vector< Brique > > > contenu;
	Construction(Brique const brique)
	{
		contenu.resize(1);
		contenu[0].resize(1);
		contenu[0][0].push_back(brique);
	};


	friend class Grader;
	ostream& afficher(ostream& sortie) const;

	Construction& operator^=(Construction const& c2)
	{
		contenu.insert(contenu.end(), c2.contenu.begin(), c2.contenu.end());
		return *this;
	};

	Construction& operator-=(Construction const& other)
	{
		int thisheight = contenu.size();
		int otherheight = other.contenu.size();
		if (otherheight >= thisheight)
		{
			for (std::size_t plane = 0; plane != contenu.size(); ++plane)
			{
				contenu[plane].insert(contenu[plane].end(), other.contenu[plane].begin(), other.contenu[plane].end());
			}
			//for (auto & plane = contenu.begin(); plane != contenu.end(); ++plane)
			//{
			//}
		}
		else
		{
			//ignore
		}
		return *this;
	};
	Construction& operator+=(Construction const& other)
	{
		int thisheight	= contenu.size();
		int thisDepth;

		int otherheight = other.contenu.size();
		int otherDepth;

		if (otherheight >= thisheight)
		{
			for (std::size_t plane = 0; plane != contenu.size(); ++plane)
			{
				thisDepth = contenu[plane].size();
				otherDepth = other.contenu[plane].size();
				if (otherDepth >= thisDepth)
				{
					for (std::size_t row = 0; row != contenu[plane].size(); ++row)
					{
						contenu[plane][row].insert(contenu[plane][row].end(), other.contenu[plane][row].begin(), other.contenu[plane][row].end());
					}
				}
				else
				{
					//ignore;
				}
			}
		}
		else
		{
			//ignore
		}
		return *this;
	};

};

ostream& Construction::afficher(ostream& sortie) const
{
	if (contenu.empty())
	{

	}
	else
	{
		
		for (auto plane = contenu.rbegin(); plane != contenu.rend(); ++plane)
		{
			int index = contenu.size() - 1 - distance(contenu.rbegin(), plane);
			sortie << "Couche " << index << " :" << endl;

			for (auto row = plane->begin(); row != plane->end(); ++row)
			{
				for (auto col = row->begin(); col != row->end(); ++col)
				{
					sortie << *col;
				}
				sortie << endl;
			}
			
			//for (auto & row : (*(plane.operator->))) {
			//	for (auto & col : row) {
			//		sortie << col;
			//	}
			//	sortie << endl;
			//}
		}
	}
	return sortie;
}

ostream& operator<<(ostream& sortie, Construction const& construction)
{
	construction.afficher(sortie);
	return sortie;
}


const Construction  operator^(Construction c1, Construction const& c2)
{
	return c1 ^= c2;
}

const Construction  operator+(Construction c1, Construction const& c2)
{
	return c1 += c2;
}


const Construction  operator-(Construction c1, Construction const& c2)
{
	return c1 -= c2;
}

const Construction operator*(unsigned int n, Construction const& a)
{
	Construction b(a);
	for (unsigned int i = 0; i != n - 1; ++i)
	{
		b += a;
	}
	return b;
}

const Construction operator/(unsigned int n, Construction const& a)
{
	Construction b(a);
	for (unsigned int i = 0; i != n - 1; ++i)
	{
		b ^= a;
	}
	return b;
}

const Construction operator%(unsigned int n, Construction const& a)
{
	Construction b(a);
	for (unsigned int i = 0; i != n - 1; ++i)
	{
		b -= a;
	}
	return b;
}

/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  // Modèles de briques
  Brique toitD("obliqueD", "rouge");
  Brique toitG("obliqueG", "rouge");
  Brique toitM(" pleine ", "rouge");
  Brique mur  (" pleine ", "blanc");
  Brique vide ("                 ", "");

  unsigned int largeur(4);
  unsigned int profondeur(3);
  unsigned int hauteur(3); // sans le toit

  // on construit les murs
  Construction maison( hauteur / ( profondeur % (largeur * mur) ) );

  // on construit le toit
  Construction toit(profondeur % ( toitG + 2*toitM + toitD ));
  toit ^= profondeur % (vide + toitG + toitD);

  // on pose le toit sur les murs
  maison ^= toit;

  // on admire notre construction
  cout << maison << endl;

  return 0;
}
