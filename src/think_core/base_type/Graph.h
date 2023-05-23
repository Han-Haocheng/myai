//
// Created by HanHaocheng on 2023/05/15.
//

#ifndef THINKSYSTEM_GRAPH_H
#define THINKSYSTEM_GRAPH_H

#include <memory>
#include <vector>

class GraphVertex;

class Edge
{
  GraphVertex *i, *j;
};

class GraphVertex
{
  std::vector<Edge> m_next_;
};

class Graph
{
};

#endif//THINKSYSTEM_GRAPH_H
