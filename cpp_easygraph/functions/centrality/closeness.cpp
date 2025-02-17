#include "centrality.h"
#include "../../classes/graph.h"
#include "../../common/utils.h"
#include "../../classes/linkgraph.h"
#include "../../classes/segment_tree.cpp"

double closeness_dijkstra(const Graph_L& G_l, const int &S, int cutoff, Segment_tree_zkw& segment_tree_zkw){
    const int dis_inf = 0x3f3f3f3f;
    int N = G_l.n;
    segment_tree_zkw.init(N);
    std::vector<int> dis(N+1, INT_MAX);
    const std::vector<LinkEdge>& E = G_l.edges;
    const std::vector<int>& head = G_l.head;
    int number_connected = 0;
    long long sum_dis = 0;
    dis[S] = 0; 
    segment_tree_zkw.change(S, 0);
    while(segment_tree_zkw.t[1] != dis_inf) {
        int u = segment_tree_zkw.num[1];
        if(u == 0) break;
        segment_tree_zkw.change(u, dis_inf);
        if (cutoff >= 0 && dis[u] > cutoff){
            continue;
        } 
        number_connected += 1;
        sum_dis += dis[u];
        for(register int p = head[u]; p != -1; p = E[p].next) {
            int v = E[p].to;
            if(cutoff >= 0 && (dis[u] + E[p].w) > cutoff){
                continue;
            }
            if (dis[v] > dis[u] + E[p].w) {
                dis[v] = dis[u] + E[p].w;
                segment_tree_zkw.change(v, dis[v]);  
            }
        }
    }
    if (number_connected == 1)
        return 0.0;
    else
        return 1.0 * (number_connected - 1) * (number_connected - 1) / ((N - 1) * sum_dis);
    
}

py::object closeness_centrality(py::object G, py::object weight, py::object cutoff, py::object sources) {
    Graph& G_ = G.cast<Graph&>();
    int N = G_.node.size();
    bool is_directed = G.attr("is_directed")().cast<bool>();
    std::string weight_key = weight_to_string(weight);
    const Graph_L& G_l = graph_to_linkgraph(G_, is_directed, weight_key, false, true);
    int cutoff_ = -1;
    if (!cutoff.is_none()){
        cutoff_ = cutoff.cast<int>();
    }
    Segment_tree_zkw segment_tree_zkw(N);
    py::list total_res_lst = py::list();
    if(!sources.is_none()){
        py::list sources_list = py::list(sources);
        int sources_list_len = py::len(sources_list);
        for(register int i = 0; i < sources_list_len; i++){
            if(G_.node_to_id.attr("get")(sources_list[i],py::none()).is_none()){
                printf("The node should exist in the graph!");
                return py::none();
            }
            py::list res_lst = py::list();
            node_t source_id = G_.node_to_id.attr("get")(sources_list[i]).cast<node_t>();
            float res = closeness_dijkstra(G_l, source_id, cutoff_,segment_tree_zkw);
            res_lst.append(py::cast(res));
            total_res_lst.append(res);
        }
    }
    else{
        for(register int i = 1; i <= N; i++){
            float res = closeness_dijkstra(G_l, i, cutoff_,segment_tree_zkw);
            total_res_lst.append(py::cast(res));
        }
    }
    return total_res_lst;
    
}