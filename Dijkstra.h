#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <climits>

using namespace std;
typedef long vertex_t;
typedef long weight_t;

const weight_t max_weight = LONG_MAX;



struct neighbor
{
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};
typedef std::vector<std::vector<neighbor> > adjacency_list_t;

void DijkstraComputePaths(vertex_t source,
    const adjacency_list_t &adjacency_list,
        std::vector<weight_t> &min_distance,
            std::vector<vertex_t> &previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
        vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor> &neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
        neighbor_iter != neighbors.end();
        neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;

            if (distance_through_u < min_distance[v])
            {
                vertex_queue.erase(std::make_pair(min_distance[v], v));
                min_distance[v] = distance_through_u;
                previous[v] = u;

                vertex_queue.insert(std::make_pair(min_distance[v], v));

            }
        }
    }
    adjacency_list_t adjacency_listeast(100);

    adjacency_listeast[0].push_back(neighbor(39, 3));
    adjacency_listeast[1].push_back(neighbor(0, 2));
    adjacency_listeast[2].push_back(neighbor(4, 9));
    adjacency_listeast[3].push_back(neighbor(1, 1));
    adjacency_listeast[4].push_back(neighbor(72, 9));
    adjacency_listeast[5].push_back(neighbor(3, 9));
    adjacency_listeast[7].push_back(neighbor(5, 8));
    adjacency_listeast[8].push_back(neighbor(7, 7));
    adjacency_listeast[9].push_back(neighbor(10, 9));
    adjacency_listeast[10].push_back(neighbor(11, 9));
    adjacency_listeast[11].push_back(neighbor(13, 9));
    adjacency_listeast[14].push_back(neighbor(8, 6));
    adjacency_listeast[15].push_back(neighbor(14, 5));
    adjacency_listeast[26].push_back(neighbor(51, 4));
    adjacency_listeast[28].push_back(neighbor(26, 3));
    adjacency_listeast[29].push_back(neighbor(28, 2));
    adjacency_listeast[30].push_back(neighbor(29, 1));
    adjacency_listeast[31].push_back(neighbor(70, 9));
    adjacency_listeast[31].push_back(neighbor(30, 8));
    adjacency_listeast[32].push_back(neighbor(31, 7));
    adjacency_listeast[33].push_back(neighbor(32, 6));
    adjacency_listeast[35].push_back(neighbor(33, 5));
    adjacency_listeast[36].push_back(neighbor(35, 4));
    adjacency_listeast[36].push_back(neighbor(68, 9));
    adjacency_listeast[38].push_back(neighbor(36, 5));
    adjacency_listeast[39].push_back(neighbor(38, 4));
    adjacency_listeast[40].push_back(neighbor(15, 4));
    adjacency_listeast[41].push_back(neighbor(40, 3));
    adjacency_listeast[42].push_back(neighbor(41, 2));
    adjacency_listeast[43].push_back(neighbor(42, 1));
    adjacency_listeast[44].push_back(neighbor(42, 9));
    adjacency_listeast[51].push_back(neighbor(52, 3));
    adjacency_listeast[52].push_back(neighbor(53, 4));
    adjacency_listeast[52].push_back(neighbor(54, 9));
    adjacency_listeast[51].push_back(neighbor(55, 9));
    adjacency_listeast[68].push_back(neighbor(69, 9));
    adjacency_listeast[70].push_back(neighbor(71, 9));
    adjacency_listeast[71].push_back(neighbor(2, 9));
    adjacency_listeast[72].push_back(neighbor(73, 9));
    adjacency_listeast[73].push_back(neighbor(9, 9));
}

std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{
    std::list<vertex_t> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}



#endif // DIJKSTRA_H
