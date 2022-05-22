/******************************************
 * Jeu de dames
 ******************************************/
 
#include "graphics.h"

#define monBlue 0x006D9F
#define monVert 0x006D7F
#define monMarron 0x45331D

typedef enum INTERFACE {
	INTERFACE1,INTERFACE2
} INTERFACE;

typedef struct numCase {
	int l, c;
} numCase;


typedef enum TYPEP {
	VIDE,PION,DAME
} TYPEP;

typedef enum JOUEUR {
	J1,J2
} JOUEUR;
	

typedef struct PIECE {
	TYPEP typep;
	JOUEUR joueur;
	COULEUR couleur;
	
} PIECE;


PIECE plateau [10] [10];


 /*****************************************
 * Modèle
 ****************************************/
 
void init_plateau()
{
	int l,c;
	for(l=0;l<10;l++)
	{	
		for(c=0;c<10;c++)
		{	
			if (((l % 2 == 0) && (c % 2 != 0))
				||
				((l % 2 != 0) && (c % 2 == 0)))
			{
				if (c <= 3)
				{
					plateau[l][c].typep=PION;
					plateau[l][c].joueur=J1;
					plateau[l][c].couleur=rouge;
				}
				else if (c >= 6)
				{
					plateau[l][c].typep=PION;
					plateau[l][c].joueur=J2;
					plateau[l][c].couleur=monBlue;
				}		
			}
			else
			{
				plateau[l][c].typep=VIDE;
			}
	
		 }
	}
} 

numCase transforme_clic_en_numCase(POINT clic,INTERFACE interface)   
{
	numCase nc;
	
	if (interface == INTERFACE1) 
	{
		nc.l = ((clic.y - 60)/60);
		nc.c = ((clic.x - 60)/60);
	}
	else if (interface == INTERFACE2)
	{
		nc.l = 9 - ((clic.x - 60)/60);
		nc.c = ((clic.y - 60)/60);
	}
	return nc;
}


POINT transforme_numCase_en_pointCentral(numCase nc,INTERFACE interface)
{
	POINT centre;
	
	if (interface == INTERFACE1) 
	{
		centre.x = 90 + nc.c*60;
		centre.y = 90 + nc.l*60;
	}
	
	else if (interface == INTERFACE2)
	{
		centre.x = 90 + (9 - nc.l)*60;
		centre.y = 90 + nc.c*60;
	}	
		
	return centre;
}


BOOL est_case_vide(numCase nc)         
{
	if ((nc.l <= 9) && (nc.c <= 9))
	{
		return (plateau [nc.l][nc.c].typep == VIDE);
	}
	else
	{
		return TRUE;
	}
}


BOOL est_tour_valide(JOUEUR joueur ,numCase nc)
{
	if (!est_case_vide(nc))
	{
		return (plateau [nc.l][nc.c].joueur == joueur) ;   
	}
	else
	{
		return FALSE;
	}	   
	
}


BOOL est_pion(numCase nc)               
{
	return (plateau [nc.l][nc.c].typep == PION);
	
}

BOOL est_dame(numCase nc)               
{
	return (plateau [nc.l][nc.c].typep == DAME);
	
}

BOOL est_devenu_dame(numCase ncInitial,numCase ncFinal)               
{
	int colonneFinal;
	
	if(plateau[ncInitial.l][ncInitial.c].joueur == J1)
	{
		colonneFinal = 9;
	}
	else
	{
		colonneFinal = 0;
	}
		
	if(ncFinal.c == colonneFinal || plateau[ncInitial.l][ncInitial.c].typep == DAME)
	{
		return plateau[ncFinal.l][ncFinal.c].typep = DAME;
	}
	
	return FALSE;

}

numCase selectionner_case_a_sauter(numCase ncInitial,numCase ncFinal,JOUEUR joueur)
{
	numCase ncSautee;
	
	if (est_pion(ncInitial)) 
	{
		if (joueur == J1)
		{
			ncSautee.c = ncInitial.c + 1;
		}else if (joueur == J2)
		{
			ncSautee.c = ncInitial.c - 1;
		}
	}
	else if (est_dame(ncInitial))
	{
		if (ncFinal.c < ncInitial.c)
		{
			ncSautee.c = ncInitial.c - 1;		
		}else if (ncFinal.c > ncInitial.c)
		{
			ncSautee.c = ncInitial.c + 1;
		}	
	}	
		
	if (ncFinal.l < ncInitial.l)
	{
		ncSautee.l = ncInitial.l - 1;
	}else
	{
		ncSautee.l = ncInitial.l + 1;
	}	
	
	return ncSautee;
}


BOOL est_mouvement_sans_prise_valide(numCase ncInitial, numCase ncFinal,JOUEUR joueur,int pas)
{
	numCase basdroite,basgauche,hautdroite,hautgauche;
	
	basdroite.l = ncInitial.l - pas; basdroite.c = ncInitial.c + pas;
	basgauche.l = ncInitial.l - pas; basgauche.c = ncInitial.c - pas;
	hautdroite.l = ncInitial.l + pas; hautdroite.c = ncInitial.c + pas;
	hautgauche.l = ncInitial.l + pas; hautgauche.c = ncInitial.c - pas;
	
	if ((ncFinal.l <= 9) && (ncFinal.c <= 9))
	{
		if (est_pion(ncInitial)) 
		{
			if (joueur == J1)
			{
				return (((ncFinal.l == basdroite.l) && (ncFinal.c == basdroite.c))
						||
						((ncFinal.l == hautdroite.l) && (ncFinal.c == hautdroite.c)));
						
			}
			else 
			{
				return (((ncFinal.l == basgauche.l) && (ncFinal.c == basgauche.c))
						||
						((ncFinal.l == hautgauche.l) && (ncFinal.c == hautgauche.c)));
						
			}
		}	
		else if (est_dame(ncInitial)) 
		{
			return ((((ncFinal.l == basdroite.l) && (ncFinal.c == basdroite.c))
					||
					((ncFinal.l == hautdroite.l) && (ncFinal.c == hautdroite.c)))
					||
					(((ncFinal.l == basgauche.l) && (ncFinal.c == basgauche.c))
					||
					((ncFinal.l == hautgauche.l) && (ncFinal.c == hautgauche.c))));
		}
	}
	return FALSE;	
}

BOOL est_prise_valide(numCase ncInitial,numCase ncFinal,JOUEUR joueur)
{
	numCase ncSautee;
	ncSautee = selectionner_case_a_sauter(ncInitial,ncFinal,joueur);
	
	if ((ncFinal.l <= 9) && (ncFinal.c <= 9))
	{
		if ((est_mouvement_sans_prise_valide(ncInitial,ncFinal,joueur,2)) && (!(est_case_vide(ncSautee))))
		{
			return (plateau[ncSautee.l][ncSautee.c].joueur != joueur);
		}
		else
		{
			return FALSE;
		}
	}
	
	return FALSE;	
}


BOOL est_mouvement_valide(numCase ncInitial, numCase ncFinal,JOUEUR joueur)
{
	
	if ((ncFinal.l - ncInitial.l == 1) || (ncFinal.l - ncInitial.l == -1))
	{
		return est_mouvement_sans_prise_valide(ncInitial,ncFinal,joueur,1);
	}
	
	else if ((ncFinal.l - ncInitial.l == 2) || (ncFinal.l - ncInitial.l == -2))
	{
		return est_prise_valide(ncInitial,ncFinal,joueur);
	}
	
	return FALSE;
}

BOOL est_prise_possible(numCase ncInitial,JOUEUR joueur)
{
	numCase basdroite,basgauche,hautdroite,hautgauche;
	int pas = 2;
	
	basdroite.l = ncInitial.l - pas; basdroite.c = ncInitial.c + pas;
	basgauche.l = ncInitial.l - pas; basgauche.c = ncInitial.c - pas;
	hautdroite.l = ncInitial.l + pas; hautdroite.c = ncInitial.c + pas;
	hautgauche.l = ncInitial.l + pas; hautgauche.c = ncInitial.c - pas;    
	
	if (est_pion(ncInitial))
	{
		if (joueur == J1)  
		{
			return (((est_prise_valide(ncInitial,basdroite,joueur)) && (est_case_vide(basdroite)))
					||
					((est_prise_valide(ncInitial,hautdroite,joueur)) && (est_case_vide(hautdroite)))) ;
		}
		else 
		{		
			return	(((est_prise_valide(ncInitial,basgauche,joueur)) && (est_case_vide(basgauche)))
					||
					((est_prise_valide(ncInitial,hautgauche,joueur)) && (est_case_vide(hautgauche))));
		}
	}
	else
	{
		return (((est_prise_valide(ncInitial,basdroite,joueur)) && (est_case_vide(basdroite)))
				||
				((est_prise_valide(ncInitial,hautdroite,joueur)) && (est_case_vide(hautdroite)))
				||
				((est_prise_valide(ncInitial,basgauche,joueur)) && (est_case_vide(basgauche)))
				||
				((est_prise_valide(ncInitial,hautgauche,joueur)) && (est_case_vide(hautgauche))));
	}			
	
}

BOOL est_mouvement_sans_prise_possible(numCase ncInitial,JOUEUR joueur)
{
	numCase basdroite,basgauche,hautdroite,hautgauche;
	int pas = 1;
	
	basdroite.l = ncInitial.l - pas; basdroite.c = ncInitial.c + pas;
	basgauche.l = ncInitial.l - pas; basgauche.c = ncInitial.c - pas;
	hautdroite.l = ncInitial.l + pas; hautdroite.c = ncInitial.c + pas;
	hautgauche.l = ncInitial.l + pas; hautgauche.c = ncInitial.c - pas;
	
	if (est_pion(ncInitial))
	{
		if (joueur == J1)     
		{
			return 	(((est_mouvement_sans_prise_valide(ncInitial,basdroite,joueur,1)) && (est_case_vide(basdroite)))
					||
					((est_mouvement_sans_prise_valide(ncInitial,hautdroite,joueur,1)) && (est_case_vide(hautdroite))));
		}
		else
		{			
			return	(((est_mouvement_sans_prise_valide(ncInitial,basgauche,joueur,1)) && (est_case_vide(basgauche)))
					||
					(((est_mouvement_sans_prise_valide(ncInitial,hautgauche,joueur,1))) && (est_case_vide(hautgauche))));
		}
	}
	else 
	{
			return	(((est_mouvement_sans_prise_valide(ncInitial,basdroite,joueur,1)) && (est_case_vide(basdroite)))
					||
					((est_mouvement_sans_prise_valide(ncInitial,hautdroite,joueur,1)) && (est_case_vide(hautdroite)))
					||
					((est_mouvement_sans_prise_valide(ncInitial,basgauche,joueur,1)) && (est_case_vide(basgauche)))
					||
					(((est_mouvement_sans_prise_valide(ncInitial,hautgauche,joueur,1))) && (est_case_vide(hautgauche))));
	}
}

BOOL est_piece_bloquee(numCase ncInitial,JOUEUR joueur)
{
	return ((!est_mouvement_sans_prise_possible(ncInitial,joueur)) && (!est_prise_possible(ncInitial,joueur)));
}


BOOL est_bouton_stop_appuyee(POINT clicCoupMultiple)
{
	return (((clicCoupMultiple.x >= 775) && (clicCoupMultiple.x <= 1045))
			&&
			((clicCoupMultiple.y >= 210) && (clicCoupMultiple.y <= 290 )));
}


void appliquer_coup(numCase ncInitial,numCase ncFinal,JOUEUR joueur)
{
	numCase ncSautee;
	
	plateau [ncFinal.l][ncFinal.c].joueur = joueur;
	
	if (est_devenu_dame(ncInitial,ncFinal))
	{ 
		plateau [ncFinal.l][ncFinal.c].typep = DAME;
	}
	else
	{
		plateau [ncFinal.l][ncFinal.c].typep = plateau [ncInitial.l][ncInitial.c].typep; 
	}	
	
	plateau [ncFinal.l][ncFinal.c].couleur = plateau [ncInitial.l][ncInitial.c].couleur;
	
	plateau [ncInitial.l][ncInitial.c].typep = VIDE;
	
	if (est_prise_valide(ncInitial,ncFinal,joueur))
	{
		ncSautee = selectionner_case_a_sauter(ncInitial,ncFinal,joueur);
		plateau [ncSautee.l][ncSautee.c].typep = VIDE;
	}
}




 /*****************************************
 * Vue
 *****************************************/
  
 void affiche_losange(POINT centre, COULEUR couleur)
{
	POINT P1,P2,P3;

	P1.x=centre.x; P1.y=centre.y-20;
	P2.x=centre.x-20; P2.y=centre.y;
	P3.x=centre.x+20; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,couleur);

	P1.x=centre.x; P1.y=centre.y+20;
	P2.x=centre.x-20; P2.y=centre.y;
	P3.x=centre.x+20; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,couleur);
	
	affiche_all();
}


void efface_losange(POINT centre)   
{	
	draw_fill_circle(centre,30,noir);
	affiche_all();
}


void efface_rond(POINT centre)
{
	POINT P1,P2;
	
	P1.x = centre.x - 30 ; P1.y = centre.y - 30 ;
	P2.x = centre.x + 30 ; P2.y = centre.y + 30 ; 
	draw_fill_rectangle(P1,P2,noir);
	
	affiche_all();
}


void affiche_rond(POINT centre,COULEUR couleur)
{
	if (couleur == monBlue)
	{
		draw_fill_circle(centre,23,lightsteelblue);
		draw_fill_circle(centre,20,couleur);
		draw_circle(centre,15,bleumarine);	
	}
	else 
	{
		draw_fill_circle(centre,23,lightsalmon);
		draw_fill_circle(centre,20,couleur);
		draw_circle(centre,15,darkred);
	}
	
	affiche_all();
}

void affiche_dame(POINT p, COULEUR couleur)
{
	POINT p1, p2, p3;
	

	p1.x = p.x; p1.y = p.y + 8;
	p2.x = p.x - 10; p2.y = p.y - 10;
	p3.x = p.x + 10; p3.y = p.y - 10;

	draw_fill_triangle(p1,p2,p3,orange);
	draw_triangle(p1,p2,p3,couleur);

	p1.x -=10; p1.y +=2;
	p3.x -=10;

	draw_fill_triangle(p3,p1,p2,gold);
	draw_line(p1,p3,couleur);


	p1.x += 20;
	p2.x += 20;

	draw_fill_triangle(p3,p1,p2,gold);
	draw_line(p1,p3,couleur);

	
	affiche_all();
}

void affiche_bouton_stop()
{
	POINT p1, p2, p3;

	p1.x = 775; p1.y = 290;
	p2.x = 1045; p2.y = 210;
	p3.x = p1.x; p3.y = p2.y;
	draw_fill_triangle(p2,p1,p3,bleumarine);

	p3.x = p2.x; p3.y = p1.y;
	draw_fill_triangle(p2,p1,p3,darkblue);

	p1.x = 860; p1.y = 280;
	aff_pol("STOP",40,p1,floralwhite);
	
	affiche_all();
}

void efface_bouton_stop()
{
	POINT p1,p2;

	p1.x =775; p1.y = 210;

	p2.x = 1045; p2.y = 290;
	draw_fill_rectangle(p1,p2,monVert);
	
	affiche_all();
}

void affiche_coup(numCase ncInitial,numCase ncFinal,INTERFACE interface)    
{
	POINT P1,P2,P3;
	COULEUR couleur;
	JOUEUR joueur;
	numCase nc;
	
	joueur = plateau[ncInitial.l][ncInitial.c].joueur;
	P1 = transforme_numCase_en_pointCentral(ncInitial,interface);
	P2 = transforme_numCase_en_pointCentral(ncFinal,interface);
	P3 = transforme_numCase_en_pointCentral(selectionner_case_a_sauter(ncInitial,ncFinal,joueur),interface);


	couleur = plateau [ncInitial.l][ncInitial.c].couleur;
	
	if (interface == INTERFACE1)
	{
		efface_rond(P1);
		if ((est_dame(ncInitial)) || (est_devenu_dame(ncInitial,ncFinal)))
		{
			affiche_rond(P2,couleur);
			affiche_dame(P2,couleur);
			
		}
		else if(est_pion(ncInitial))
		{
			affiche_rond(P2,couleur);
		}
		
		if (est_prise_valide(ncInitial,ncFinal,joueur))
		{
			efface_rond(P3);
			nc = selectionner_case_a_sauter(ncInitial,ncFinal,joueur);
			plateau[nc.l][nc.c].typep = VIDE;
		}
	}
	
	else if (interface == INTERFACE2)
	{
		efface_losange(P1);
		affiche_losange(P2,couleur);
		if (est_prise_valide(ncInitial,ncFinal,joueur))
		{
			efface_losange(P3);
		}
	}
		affiche_all();
}

void coulorier_case(numCase nc,COULEUR couleur,JOUEUR joueur)
{
	POINT P1,P2; 
	
	if ((nc.l <=9) && (nc.c <=9))
	{
		if (!est_case_vide(nc))
		{
			POINT P1,P2,centre;
			
			P1.y= 60 + nc.l*60; P1.x= 60 + nc.c*60;
			P2.y=120 + nc.l*60; P2.x=120 + nc.c*60;
			draw_fill_rectangle(P1,P2,couleur);
			
			P1.x +=3; P1.y +=3;
			P2.x +=-3; P2.y +=-3;
			draw_fill_rectangle(P1,P2,noir);
			
			centre = transforme_numCase_en_pointCentral(nc,INTERFACE1);
			
			if (plateau[nc.l][nc.c].joueur == J1)
			{
				if (est_pion(nc))
				{
					affiche_rond(centre,rouge);
				}
				else
				{
					affiche_rond(centre,rouge);
					affiche_dame(centre,rouge);
				}
			}
			else
			{
				if (est_pion(nc))
				{
					affiche_rond(centre,monBlue);
				}
				else
				{
					affiche_rond(centre,monBlue);
					affiche_dame(centre,monBlue);
				}
			}	
		}
		else
		{
			if (((nc.l % 2 == 0) && (nc.c % 2 != 0))

					||
					
					((nc.l % 2 != 0) && (nc.c % 2 == 0)))
			{		
				P1.y= 60 + nc.l*60; P1.x= 60 + nc.c*60;
				P2.y=120 + nc.l*60; P2.x=120 + nc.c*60;
				draw_fill_rectangle(P1,P2,couleur);
				
				P1.x +=3; P1.y +=3;
				P2.x +=-3; P2.y +=-3;
				draw_fill_rectangle(P1,P2,noir);
			}
			else
			{
				P1.y= 60 + nc.l*60; P1.x= 60 + nc.c*60;
				P2.y=120 + nc.l*60; P2.x=120 + nc.c*60;
				draw_fill_rectangle(P1,P2,couleur);
				
				P1.x +=3; P1.y +=3;
				P2.x +=-3; P2.y +=-3;
				draw_fill_rectangle(P1,P2,floralwhite);
			}	
		}
	}
}	

void affiche_plateau1()
{
	POINT P1,P2,centre;
	int i;
	int j;
	
	P1.x=60; P1.y=60;
	P2.x=660; P2.y=660;
	
	draw_fill_rectangle(P1,P2,floralwhite);
	
	P2.x=120; P2.y=120;

	for(i=0;i<10;i++)
	{	
		
		for(j=0;j<10;j++)
		{	
			if (((i % 2 == 0) && (j % 2 != 0))

				||
				
				((i % 2 != 0) && (j % 2 == 0)))
			{
				centre.x = 90 + j*60;
				centre.y = 90 + i*60;	
				
				draw_fill_rectangle(P1,P2,noir);
				
				if (j <= 3)
				{
					affiche_rond(centre,rouge);
					
				}
				else if (j >= 6)
				{
					affiche_rond(centre,monBlue);
	
				}		
				
			}
			
			P1.x += 60; 
			P2.x += 60; 	
	   }
		 
		P1.x=60;   P1.y+=60; 
		P2.x=120;  P2.y+=60;		
	}
	
	affiche_all();
}


void affiche_plateau2()
{
	POINT centre;
	int i;
	int j;

	affiche_auto_off();  

	centre.x=90; centre.y=90;

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if (((i % 2 == 0) && (j % 2 != 0))

				||

				((i % 2 != 0) && (j % 2 == 0)))
			{
				draw_fill_circle(centre,30,noir);

				if (i <= 3)
				{
					affiche_losange(centre,rouge);
				}
				else if (i >= 6)
				{
					affiche_losange(centre,seagreen);
				}

			}
			else
            {
                draw_fill_circle(centre,30,blanc);
            }

			centre.x+=65;
	   }

		centre.x=90; centre.y+=65;
	}
	
	affiche_all();
}


void efface_tour () 
{
	POINT P1, P2;
	
	P1.x = 1000 ; P1.y = 570;
	P2.x = 1110; P2.y = 600;
	draw_fill_rectangle (P1, P2,monVert);
	
	affiche_all();
}


void affiche_tour_joueur1 (INTERFACE interface)   
{
	POINT P ;   
	if (interface == INTERFACE1)
	{
		efface_tour ();
		P.x = 1020 ; P.y = 580;
		affiche_rond(P,rouge);
	}
	else
	{
		efface_tour ();
		P.x = 1020 ; P.y = 580;
		affiche_losange(P,rouge);
	}
	
}


void affiche_tour_joueur2 (INTERFACE interface)    
{
	POINT P ;   
	
	if (interface == INTERFACE1)
	{
		efface_tour ();
		P.x = 1020 ; P.y = 580;
		affiche_rond(P,monBlue);
	}
	else
	{
		efface_tour ();
		P.x = 1020 ; P.y = 580;
		affiche_losange(P,seagreen);
	}
}


void affiche_tour(JOUEUR joueur, INTERFACE interface)
{
	POINT P;
	
	P.x = 780 ; P.y = 600;
	aff_pol("A qui de jouer :", 26, P, blanc);
	
	if (joueur==J1)
	{
		affiche_tour_joueur1(interface);
	}
	
	if (joueur==J2)
	{
		affiche_tour_joueur2(interface);
	}
}


void changer_joueur(JOUEUR * joueur)   
{
	if (* joueur == J1)
	{
		* joueur = J2;
	}
	
	else if (* joueur == J2)
	{
		* joueur = J1;
	}
}


void affiche_menu1()
{
	POINT P1,P2,P3;
	POINT centre;
	
	P1.x=300; P1.y=600;
	aff_pol("Bienvenue sur notre jeu dame", 40,P1,floralwhite);
	
	centre.x=260; centre.y=570;
	P1.x=centre.x; P1.y=centre.y-25;
	P2.x=centre.x-25; P2.y=centre.y;
	P3.x=centre.x+25; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,tomato);

	P1.x=centre.x;    P1.y=centre.y+25;
	P2.x=centre.x-25; P2.y=centre.y;
	P3.x=centre.x+25; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,lightsteelblue);
	
	centre.x=950; centre.y=570;
	P1.x=centre.x; P1.y=centre.y-25;
	P2.x=centre.x-25; P2.y=centre.y;
	P3.x=centre.x+25; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,lightsteelblue);

	P1.x=centre.x;    P1.y=centre.y+25;
	P2.x=centre.x-25; P2.y=centre.y;
	P3.x=centre.x+25; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,tomato);
	
	P1.x=400;   P1.y=380;
	P2.x =780;  P2.y =280;
	
	draw_fill_rectangle(P1,P2,lightsteelblue);
	
	P1.x = 405;  P1.y = 385;
	P2.x = 775; P2.y = 285;
	P3.x = P1.x; P3.y = P2.y;
	draw_fill_triangle(P2,P1,P3,bleumarine);

	P3.x = P2.x; P3.y = P1.y;
	draw_fill_triangle(P2,P1,P3,darkblue);

	P1.x = 525; P1.y = 368;
	aff_pol("JOUER",45,P1,azure);
	

	affiche_all();
}


void affiche_menu2()
{
	POINT P1,P2,P3;
	POINT centre;
		
	P1.x=450; P1.y=470;
	P2.x=750; P2.y=370;
	draw_fill_rectangle(P1,P2,lightsteelblue);
	
	P1.x = 455; P1.y = 475;
	P2.x = 745; P2.y = 375;
	P3.x = P1.x; P3.y = P2.y;
	draw_fill_triangle(P1,P2,P3,bleumarine);

	P3.x = P2.x; P3.y = P1.y;
	draw_fill_triangle(P2,P1,P3,darkblue);
	
	P3.x=600; P3.y=425;
	draw_fill_circle(P3,35,silver);
	draw_fill_circle(P3,30,floralwhite);
	draw_circle(P3,20,noir);	
	
	
	P1.x=450; P1.y=270;
	P2.x=750; P2.y=170;
	draw_fill_rectangle(P1,P2,lightsteelblue);
	
	P1.x = 455; P1.y = 275;
	P2.x = 745; P2.y = 175;
	P3.x = P1.x; P3.y = P2.y;
	draw_fill_triangle(P1,P2,P3,bleumarine);

	P3.x = P2.x; P3.y = P1.y;
	draw_fill_triangle(P2,P1,P3,darkblue);
	
	
	centre.x=600; centre.y=225;
	P1.x=centre.x; P1.y=centre.y-35;
	P2.x=centre.x-35; P2.y=centre.y;
	P3.x=centre.x+35; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,crimson);

	P1.x=centre.x;    P1.y=centre.y+35;
	P2.x=centre.x-35; P2.y=centre.y;
	P3.x=centre.x+35; P3.y=centre.y;
	draw_fill_triangle(P1,P2,P3,rouge);
	
	
	affiche_all();
}


INTERFACE affiche_et_recupere_choix_interface()
{
	POINT clicMenu1,clicMenu2;
	INTERFACE interface;

	affiche_menu1();
 	do               
	{
		clicMenu1 = wait_clic();
	}	
		
	while (!(((clicMenu1.x>=400) && (clicMenu1.x<=780))   
		
			&&
			
			((clicMenu1.y>=270) && (clicMenu1.y<=380))));
	
			init_graphics(1200,700);
			fill_screen(lightslategray);
			affiche_menu2();		
		   
	do
	{	
		clicMenu2 = wait_clic();
		
	}while (!((((clicMenu2.y >= 350) && (clicMenu2.y <= 500)) 
			||
			((clicMenu2.y>=150) && (clicMenu2.y<=300)))
			&&
			((clicMenu2.x >= 450) && (clicMenu2.x<= 780)))); 
		
		
	if ((clicMenu2.y >= 350) && (clicMenu2.y <= 500))
	{		
		init_graphics(1200,740);
		fill_screen(monVert);		
		interface=INTERFACE1;
		affiche_plateau1();
	}

						
	else 
	{
		init_graphics(1200,740);
		fill_screen(monVert);				
		interface=INTERFACE2;
		affiche_plateau2();
	}				
			
		affiche_all();
	return interface;
}		



 /*****************************************
 * Contrôleur
 ****************************************/
 
int main()
{
	numCase ncInitial,ncFinal,ncCoupMultiple;	
	POINT clicInitial,clicFinal,clicCoupMultiple;
	JOUEUR joueur;
	INTERFACE interface;
	
	
	init_graphics(1200,740);
	fill_screen(lightslategray);
	
	affiche_auto_off();

	joueur=J1;
	interface=affiche_et_recupere_choix_interface();
	init_plateau();
	affiche_tour(joueur,interface);
	

						
	while (interface == INTERFACE1)
	{
		ncInitial.l = 15;
		
		do        
		{
			clicInitial=wait_clic();
			
			coulorier_case(ncInitial,noir,joueur);
			
			ncInitial = transforme_clic_en_numCase(clicInitial,interface);
			
			coulorier_case(ncInitial,rouge,joueur);
											
		}while((!est_tour_valide(joueur,ncInitial)) || (est_piece_bloquee(ncInitial,joueur)) || (est_case_vide(ncInitial)));   
		
		coulorier_case(ncInitial,vert,joueur);
		
		do   
		{
			clicFinal = wait_clic();
			ncFinal = transforme_clic_en_numCase(clicFinal,interface);
								
		}while((!est_mouvement_valide(ncInitial,ncFinal,joueur)) || (!est_case_vide(ncFinal)));
		
		coulorier_case(ncInitial,noir,joueur);

	
		if (est_prise_valide(ncInitial,ncFinal,joueur))
		{
			affiche_coup(ncInitial,ncFinal,interface);
			
			appliquer_coup(ncInitial,ncFinal,joueur); 
			
			ncCoupMultiple.l = 15;
			
			if 	(est_prise_possible(ncFinal,joueur))
			{
				affiche_bouton_stop();
				
				do
				{
					clicCoupMultiple = wait_clic();
					ncCoupMultiple = transforme_clic_en_numCase(clicCoupMultiple,interface);
										
				}while (((!est_prise_valide(ncFinal,ncCoupMultiple,joueur)) 
						&& 
						(!est_bouton_stop_appuyee(clicCoupMultiple)))
						||
						 (!est_case_vide(ncCoupMultiple)));	
				
				efface_bouton_stop();
				if (est_prise_valide(ncFinal,ncCoupMultiple,joueur))
				{
			
					affiche_coup(ncFinal,ncCoupMultiple,interface);
			
					appliquer_coup(ncFinal,ncCoupMultiple,joueur);	
				}		
			}
			
		
		}
			
		else 
		
		{
			affiche_coup(ncInitial,ncFinal,interface);
			
			appliquer_coup(ncInitial,ncFinal,joueur);		
		}
		
		changer_joueur(& joueur);
		
		affiche_tour(joueur,interface);
							
	}

				
	wait_escape();
	return 0;
	
}



