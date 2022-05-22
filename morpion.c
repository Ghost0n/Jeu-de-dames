/******************************************
 * Morpion
 ******************************************/
#include "graphics.h"

typedef struct numCase {
	int l, c;
} numCase;


int tabJeu [3][3]; // 0: vide; 1: croix et 2: rond


/*****************************************
 * Interface Graphique 
 ****************************************/
 
void affiche_plateau ()
{
	POINT P1, P2;
	
	P1.x = 20;  P1.y = 200;
	P2.x = 580; P2.y = 200;
	draw_line (P1, P2, blanc);
	
	P1.x = 20;  P1.y = 400;
	P2.x = 580; P2.y = 400;
	draw_line (P1, P2, blanc);
	
	P1.x = 200;  P1.y = 20;
	P2.x = 200; P2.y = 580;
	draw_line (P1, P2, blanc);

	P1.x = 400;  P1.y = 20;
	P2.x = 400; P2.y = 580;
	draw_line (P1, P2, blanc);

	P1.x = 40;  P1.y = 660;
	aff_pol("A qui de jouer :", 26, P1, blanc);
}

void affiche_croix (POINT centre, int taille)
{
	POINT P1, P2;
	
	P1.x = centre.x - taille;
	P1.y = centre.y - taille;

	P2.x = centre.x + taille;
	P2.y = centre.y + taille;
	
	draw_line (P1, P2, blanc);

	P1.x = centre.x - taille;
	P1.y = centre.y + taille;

	P2.x = centre.x + taille;
	P2.y = centre.y - taille;
	
	draw_line (P1, P2, blanc);
}

void affiche_rond (POINT centre, int taille)
{
	draw_circle (centre, taille, blanc);
}

void efface_tour () // point central du carré : (300,645)
{
	POINT P1, P2;
	P1.x = 280; P1.y = 625;
	P2.x = 320; P2.y = 665;
	draw_fill_rectangle (P1, P2, noir);
}

void affiche_tour_croix ()
{
	POINT centre ;   
	
	efface_tour ();

	centre.x = 300 ; centre.y = 645;
	affiche_croix (centre, 20);
}

void affiche_tour_rond ()
{
	POINT centre ; 
	
	efface_tour ();

	centre.x = 300 ; centre.y = 645;
	affiche_rond (centre, 20);
}

/*****************************************
 * Moteur du Jeu 
 ****************************************/

numCase transforme_clic_en_numCase (POINT clic)
{
	numCase nc;
	
	nc.l = clic.y/200;
	nc.c = clic.x/200;
	
	return nc;
}

POINT transforme_numCase_en_pointCentral (numCase nc)
{
	POINT centre;
	
	centre.x = 100 + nc.c*200;
	centre.y = 100 + nc.l*200;
	
	return centre;
}

void init_tabJeu ()
{
	int l,c;
	
	for (l=0; l<3; l++)
	{
		for (c=0; c<3; c++)
		{
			tabJeu[l][c] = 0;
		}		
	}
}

/*****************************************
 * Main 
 ****************************************/
 
int main ()
{
	POINT centre, clic;
	numCase nc;
	int joueur; // 1: croix ; 2: rond
	int estDansJeu;
	
	init_graphics (600, 700);
	
	affiche_plateau ();
	affiche_tour_croix ();
	joueur = 1;
	init_tabJeu ();	
	
	while (1)
	{
		do {
			clic = wait_clic();
			if (clic.y < 600)
			{
				estDansJeu = 1;
				nc = transforme_clic_en_numCase (clic);
			}
			else
			{
				estDansJeu = 0;
			}
		}
		while (estDansJeu == 0 || tabJeu[nc.l][nc.c] != 0);

		tabJeu[nc.l][nc.c] = joueur;
		
		centre = transforme_numCase_en_pointCentral (nc);
		
		if (joueur == 1)
		{
			affiche_croix (centre, 80);
			joueur = 2;
			affiche_tour_rond ();
		}
		else
		{
			affiche_rond (centre, 80);
			joueur = 1;
			affiche_tour_croix ();
		}
	}

	wait_escape ();
	return (0);	 
}
 

