#include <unordered_map> // std::unordered_map, std::pair
#include <vector>        // std::vector
#include <queue>         // std::priority_queue

/**
 * \fn Dijkstra<DistanceType>(firstnode, lastnode, for_all_neighbors_of, return_route)
 * \brief Searches for the shortest route between nodes `firstnode` and `lastnode`.
 *
 * \tparam DistanceType Type of distances
 * \tparam NodeType     Type of node indexes
 * \param firstnode Node to start search from.
 * \param lastnode  Node to find route to.
 * \param for_all_neighbors_of Functor for enumerating neighbors and distances, see below.
 * \param return_route         Functor for receiving route information, see below.
 *
 * If `firstnode` = `lastnode`, calculates the shortest route from `firstnode` to every reachable node.
 *
 * `for_all_neighbors_of` must be a functor: void(NodeType a, Functor f)
 * which calls `f` with parameters (NodeType b, DistanceType d)
 *   for all neighbors of `a`, where `b` is a neighbor of `a`,
 *                              and `d` is the distance from `a` to `b`.
 *
 * `return_route` must be a functor: void(NodeType target, NodeType source, DistanceType length),
 *   that will be called several times after the search is complete.
 *   `length` is the shortest total distance from `firstnode` node to the `target` node.
 *   `source` is the node through which the shortest route to `target` goes.
 *
 *   If `firstnode` = `lastnode`, or a route to `lastnode` was not found,
 *     This function will be called for every target
 *     that is reachable from firstnode, in an unspecified order.
 *
 *   If `firstnode` is not `lastnode`, and a route to `lastnode` was found,
 *     The function calls comprise the reverse route from `firstnode` to `lastnode`,
 *       with first call having   `target`=`lastnode`,
 *       and the last call having `source`=`firstnode`.
 *     No calls to `return_route` will be made if route was not found.
 *
 *\returns Shortest total distance from `firstnode` to `lastnode`,
 *          or DistanceType() if no route was found or if `firstnode` = `lastnode`.
 */
template<
    typename DistanceType,
    typename NodeType,
    typename NodeIterationFunction,
    typename RouteIterationFunction
>
DistanceType Dijkstra(NodeType firstnode, NodeType lastnode,
                      NodeIterationFunction&& for_all_neighbors_of,
                      RouteIterationFunction&& return_route)
{
    struct NodeInfo { DistanceType distance; NodeType previous; bool visited; };

    std::unordered_map<NodeType, NodeInfo> lore{ {firstnode,NodeInfo{}} };

    using dp = std::pair<NodeType,DistanceType>;
    auto compare = [&](const dp& a, const dp& b) { return a.second > b.second; };
    std::priority_queue<dp, std::vector<dp>, decltype(compare)> queue(compare);

    // The priority queue elements must contain a copy of the distance,
    // because without the distance, modifying lore[].distance may break
    // the heap property of the priority queue.

    // Begin from firstnode with blank distance
    queue.emplace(firstnode, DistanceType());
    while(!queue.empty())
    {
        // Find the node with shortest distance
        auto Upair = queue.top(); queue.pop();
        NodeType U             = Upair.first;
        DistanceType Udistance = Upair.second;
        // U = node number, Uinfo.second = total distance from firstnode

        // If we're looking for a particular route,
        // terminate search as soon as the target node has been reached.
        // When that happens, there is no need to mark the node visited.
        if(firstnode != lastnode && U == lastnode) break;

        // Mark the node visited. Ignore the node if already visited before.
        auto Ulore = lore.find(U);
        if(Ulore->second.visited) continue;
        Ulore->second.visited = true;

        // Check all neighbors_of of U that have not yet been visited.
        for_all_neighbors_of(U, [=,&lore,&queue](NodeType V, DistanceType distance)
        {
            distance += Udistance;
            // If V is previously unknown, or if V has not yet been visited and
            // the new distance is shorter than what is previously known for V,
            // update records and make sure that this target is eventually visited.
            auto Vlore = lore.find(V);
            if(Vlore == lore.end()) // Previously unknown
            {
                lore.emplace(V,  NodeInfo{distance,U,false});
                queue.emplace(V, distance);
            }
            else if(!Vlore->second.visited && Vlore->second.distance > distance)
            {
                Vlore->second.distance = distance;
                Vlore->second.previous = U;
                queue.emplace(V, distance);
            }
        });
    }

    auto i = lore.find(lastnode);
    if(firstnode != lastnode && i != lore.cend())
    {
        // Report the route from lastnode to firstnode. The visited flag is not used.
        for(auto j = i; j->first != firstnode; j = lore.find(j->second.previous))
            return_route(j->first, j->second.previous, j->second.distance);
        return i->second.distance;
    }
    // Report all reachable routes.
    for(const auto& l: lore)
        if(l.second.visited)
            return_route(l.first, l.second.previous, l.second.distance);
    return {};
}
