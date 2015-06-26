#include <iostream>
using namespace std;

class Rectange{
public:
	Rectange();
	Rectange(double h, double l);

	double surface() const;
	double getHauter() const { return hauteur; }
	double getLargeur() const { return largeur; }
	void setHauter(double h)  { hauteur = h; }
	void setLargeur(double l) { largeur = l; }
	
private:
	double hauteur;
	double largeur;
	
};

Rectange::Rectange()
{
	this->hauteur = 0.0;
	this->largeur = 0.0;
}
Rectange::Rectange(double h, double l)
{
	this->hauteur = h;
	this->largeur = l;
}

double Rectange::surface() const
{
	return hauteur * largeur;
}


class RectangleColore {
public:
	RectangleColore(double h, double l, int x)
	: rectangle(h, l), a(x)
	{}
	
private:
	Rectange rectangle;
	int a;	
};



int main()
{
	Rectange rect1;
	RectangleColore aa(1, 2, 3);

	rect1.setHauter(4.0);
	rect1.setLargeur(2.0);
	
	cout << "hauteur: " << rect1.getHauter() << endl;
	cout << "largeur: " << rect1.getLargeur() << endl;
	cout << "surface: " << rect1.surface() << endl;
	return 0;
}
