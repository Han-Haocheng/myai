//
// Created by HanHaocheng on 2023/05/15.
//

#ifndef THINKSYSTEM_GRAPH_H
#define THINKSYSTEM_GRAPH_H

#include <vector>
#include <memory>

/* 图 Graph G(V,E)
 * 由至少一个顶点的集合和边的集合组成的结构
 *
 * 稀疏图 - 有 很少 的边/弧组成的图(e < n * log n)。
 * 稠密图 - 有 较多 的边/弧组成的图。
 * 网 - 边/弧带有 权重 的图。
 *
 * 邻接 -
 *    有边/弧相连的 两个顶点 之间的 关系，
 *    存在(Vi,Vj),则称Vi和Vj互为邻接点，
 *    存在<Vi,Vj>,则称Vi邻接到Vj，Vj邻接于Vi
 * 关联(依附) -
 *    边/弧与顶点之间的关系。
 *    存在(Vi,Vj)/<Vi,Vj>，则称该边关联于Vi和Vj
 * 顶点的度 -
 *    与该顶点相关联的边的数目
 *    在有向图中，顶点的度等于该顶点的入度与出度之和。
 *
 * 路径 - 接续的边构成的 顶点序列
 * 路径长度 - 路径上边或弧的数目/权重之和。
 * 回路(环) - 第一个顶点和最后一个顶点相同的路径
 * 简单路径 - 除路径起点和终点可以相同外，其余顶点均不相同的路径。
 * 简单回路(简单环) - 除路径起点和终点相同外，其余顶点均不相同的路径。
 *
 * 联通图 - 在无向图G(V,{E})中, 若对任何两个顶点V和U都存在V到U的路径, 则称G是连通图。
 * 强连通图 - 在有向图G(V,{E})中, 若对任何两个顶点V和U都存在V到U的路径, 则称G是强连通图。
 * 权和网 -
 *    图中边/弧具有的的相关数称为权重, 表明从一个顶点到另一个顶点的距离或耗费
 *    带有全的图是网
 * 子图 -
 *    设有两个图，G=(V,{E})，G1=(V1,{E1})。若V1属于V, E1属于E，则称G1是G的子图。
 *
 * */
class GraphVertex;

class Edge{
  GraphVertex*i,*j;
};

class GraphVertex
{

  std::vector<Edge>m_next_;
};

class Graph
{
};

#endif//THINKSYSTEM_GRAPH_H
