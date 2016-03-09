
#include <DataStructure/Grid.h>
#include <Math/Vector2.h>
#include <GameElements/Map.h>

namespace OgreFramework{

	bool tri(Math::Vector2<int> i, Math::Vector2<int> j);

	void remplir_grille(GameElements::Map * map, Math::Vector2<int> & my_cell,Math::Vector2<int> & target_cell);

	void trouver_chemin(GameElements::Map * map, Math::Vector2<int> & my_cell,Math::Vector2<int> & target_cell,std::vector<Math::Vector2<int>> & chemin);

	void calc_chemin(GameElements::Map * map,Math::Vector2<Config::Real> const & my_position,Math::Vector2<Config::Real> const & target_position,std::vector<Math::Vector2<Config::Real>> & chemin);

}