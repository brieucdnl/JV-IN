
#include <stdafx.h>
#include <OgreFramework/Dijkstra.h>

namespace OgreFramework{

std::vector<std::vector<int>> value_tab;

//normal
bool tri(Math::Vector2<int> i, Math::Vector2<int> j ) { 
	return value_tab[i[0]][i[1]] < value_tab[j[0]][j[1]];
}


void remplir_grille(GameElements::Map * map, Math::Vector2<int> & my_cell,Math::Vector2<int> & target_cell)	
{
	bool atteint = false;
	std::vector<Math::Vector2<int>> a_parcourir;
	a_parcourir.push_back(my_cell);
	value_tab[my_cell[0]][my_cell[1]] = 0;
	while(!atteint)
	{
		while(!a_parcourir.empty())
		{
			std::vector<Math::Vector2<int>>::iterator it = std::min_element(a_parcourir.begin(), a_parcourir.end(), tri);
						
			Math::Vector2<int> cell = *it;
			Math::Vector2<int> current_cell;
			current_cell[0] = cell[0];
			current_cell[1] = cell[1];
			Math::Vector2<int> back_cell = a_parcourir.back();
			a_parcourir.erase(it);
	
			int min_val = value_tab[current_cell[0]][current_cell[1]];
			min_val++;
			//calculer les index des cellules autour
			Math::Vector2<int> haut = current_cell;
			Math::Vector2<int> bas = current_cell;
			Math::Vector2<int> gauche = current_cell;
			Math::Vector2<int> droite = current_cell;
			if(bas[0] < map->height()-1) bas[0] ++; 
			if(haut[0] > 0) haut[0] --;
			if(gauche[1] > 0) gauche[1] --;
			if(droite[1] < map->width()-1) droite[1] ++;

			//verifier si on a atteint la cible
			if(haut[0] == target_cell[0] && haut[1] == target_cell[1]) {std::cout << "atteint 1" << std::endl;value_tab[haut[0]][haut[1]] = min_val;	atteint = true; break;}
			if(bas[0] == target_cell[0] && bas[1] == target_cell[1]) {std::cout << "atteint 2" << std::endl;value_tab[bas[0]][bas[1]] = min_val;	atteint = true; break;}
			if(gauche[0] == target_cell[0] && gauche[1] == target_cell[1]) {std::cout << "atteint 3" << std::endl;value_tab[gauche[0]][gauche[1]] = min_val;	atteint = true; break;}
			if(droite[0] == target_cell[0] && droite[1] == target_cell[1]) {std::cout << "atteint 4" << std::endl;value_tab[droite[0]][droite[1]] = min_val;	atteint = true; break;}
				
			//remplir les valeurs des cellules autour
			std::cout << "----------------------------- " << map->getCell(haut).m_colorCode<< std::endl;
			std::cout << "cell type " << map->getCell(haut).m_colorCode<< std::endl;
			std::cout << "cell type " << map->getCell(bas).m_colorCode<< std::endl;
			std::cout << "cell type " << map->getCell(gauche).m_colorCode<< std::endl;
			std::cout << "cell type " << map->getCell(droite).m_colorCode<< std::endl;
			
			if(haut[0] != current_cell[0])//si la cellule existe
			{
				if(map->getCell(haut).m_colorCode == 2) {std::cout << "obstacle" << std::endl;value_tab[haut[0]][haut[1]] += 1000;}//infranchissable ??

				else if( (value_tab[ haut[0] ][ haut[1] ] == 0 || value_tab[ haut[0] ][ haut[1] ] > min_val) && map->isValid(haut)) //si la position est valide -> ça plante, on accede a une zone non autorisee./
				{
					a_parcourir.push_back(haut);
					value_tab[haut[0]][haut[1]] = min_val;	
				}
			}
			if(bas[0] != current_cell[0])//si la cellule existe
			{
				if(map->getCell(bas).m_colorCode == 2){std::cout << " obstacle " << std::endl;value_tab[bas[0]][bas[1]] +=1000;} //infranchissable
				else if( (value_tab[bas[0]][bas[1]]==-1 || value_tab[bas[0]][bas[1]] > min_val) && map->isValid(bas)) 
				{
					a_parcourir.push_back(bas);
					value_tab[bas[0]][bas[1]] = min_val;	
				}
			}

			if(gauche[1] != current_cell[1])//si la cellule existe
			{
				if(map->getCell(gauche).m_colorCode == 2) {std::cout << " obstacle " << std::endl;value_tab[gauche[0]][gauche[1]] +=1000;}//infranchissable
				else if( (value_tab[gauche[0]][gauche[1]] ==-1 || value_tab[gauche[0]][gauche[1]] > min_val) && map->isValid(gauche)) 
				{
					a_parcourir.push_back(gauche);
					value_tab[gauche[0]][gauche[1]] = min_val;	
				}
			}

			if(droite[1] != current_cell[1])//si la cellule existe
			{
				if(map->getCell(droite).m_colorCode == 2){std::cout << " obstacle " << std::endl;value_tab[droite[0]][droite[1]] +=1000;} //infranchissable
				else if( (value_tab[droite[0]][droite[1]]==-1 || value_tab[droite[0]][droite[1]] > min_val) && map->isValid(droite)) 
				{
					a_parcourir.push_back(droite);
					value_tab[droite[0]][droite[1]] = min_val;	
				}
			}		
		}
	}
}

void trouver_chemin(GameElements::Map * map, Math::Vector2<int> & my_cell,Math::Vector2<int> & target_cell,std::vector<Math::Vector2<int>> & chemin)
{
	
	value_tab[my_cell[0]][my_cell[1]] =0;
	Math::Vector2<int> current_cell = target_cell;
	int val_h = 0,val_b = 0,val_d = 0,val_g = 0;
	while(!(current_cell[0] == my_cell[0] && current_cell[1] == my_cell[1]))
	{
		val_h =0;
		val_b =0;
		val_g =0;
		val_d =0;
		//calculer les index des cellules autour
		Math::Vector2<int> haut(current_cell);
		Math::Vector2<int> bas(current_cell);
		Math::Vector2<int> gauche(current_cell);
		Math::Vector2<int> droite(current_cell);
		Math::Vector2<int> choice(current_cell);

		if(bas[0] < map->height()-1) bas[0] ++;
		if(haut[0] > 0) haut[0] --;
		if(gauche[1] > 0) gauche[1] --;
		if(droite[1] < map->width()-1) droite[1] ++;

		int min_val = value_tab[current_cell[0]][current_cell[1]];
		if(haut[0] != current_cell[0]){val_h = value_tab[haut[0]][haut[1]];}
		if(gauche[1] != current_cell[1]){val_g = value_tab[gauche[0]][gauche[1]];}
		if(droite[1] != current_cell[1]){val_d = value_tab[droite[0]][droite[1]];}
		if(bas[0] != current_cell[0]){ val_b = value_tab[bas[0]][bas[1]] ;}
		
		//rechercher la cellule avec la valeur min la plus proche
		if(val_h == -1 && val_d == -1 && val_g == -1 && val_b == -1){std::cout << "CELLULE NON ATTEINTE !! " << std::endl;break;}

		else{
			if(val_h != -1)
				if(min_val > val_h)
				{
					min_val = val_h;					
					choice = haut;
				}
			

			if(val_g != -1)
				if(min_val > val_g)
				{
					min_val = val_g;
					choice = gauche;
				}

			if(val_d != -1)
				if(min_val > val_d)
				{
					min_val = val_d;
					choice = droite;
				}

			if(val_b != -1)
				if(min_val > val_b)
				{
					min_val = val_b;
					choice = bas;
				}
			current_cell = choice ;
			chemin.push_back(choice);
		}	
	}
}

void calc_chemin(GameElements::Map * map,Math::Vector2<Config::Real> const & my_position,Math::Vector2<Config::Real> const & target_position,std::vector<Math::Vector2<Config::Real>> & chemin)
{

	Math::Vector2<int> my_cell = map->toGridCoordinates(my_position);
	Math::Vector2<int> target_cell = map->toGridCoordinates(target_position);

	std::vector<Math::Vector2<int>> chemin_verlent;

	/*
		Un chemin = un vecteur de la forme ( [cell1], [cell2], [cell3]... )
		tab = ( chemin1, chemin2, chemin3, ...)
	*/


	//une grille est créee - on suppose que les valeurs des cellules sont toutes à 0
	value_tab = std::vector<std::vector<int>>(map->height(), std::vector<int>(map->width(), -1));

	remplir_grille(map, my_cell,target_cell);	

	//Normalement, a ce stade, la grille est remplie avec les valeurs minimales pour atteindre chaque cellule
	
	trouver_chemin(map, my_cell, target_cell, chemin_verlent);

	
	for(int i=0;i<chemin_verlent.size();i++)
	{
		chemin.push_back(map->toWorldCoordinates(chemin_verlent[i]));
	}
	/*
	TO-DO   -> verifier la valeur de m_type
	*/




}

}