// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#ifndef MEDIAPIPE_DEPS_TOPOLOGICALSORTER_H_
#define MEDIAPIPE_DEPS_TOPOLOGICALSORTER_H_

#include <functional>
#include <queue>
#include <vector>

namespace mediapipe {

// TopologicalSorter provides topologically sorted traversal of the nodes of a
// directed acyclic graph (DAG) with up to INT_MAX nodes. The sorter requires
// that all nodes and edges be added before traversing the nodes, otherwise it
// will die with a fatal error. If a cycle is detected during the traversal,
// the sorter will stop the traversal, and set the cycle_nodes vector.
//
// Sample usage:
//   TopologicalSorter sorter(num_nodes);
//   sorter.AddEdge(ObjToIndex(obj_a), ObjToIndex(obj_b));
//   sorter.AddEdge(ObjToIndex(obj_a), ObjToIndex(obj_c));
//   ...
//   sorter.AddEdge(ObjToIndex(obj_b), ObjToIndex(obj_c));
//   int idx;
//   bool cyclic = false;
//   std::vector<int> cycle_nodes;
//   while (sorter.GetNext(&idx, &cyclic, &cycle_nodes)) {
//     if (cyclic) {
//       PrintCycleNodes(cycle_nodes);
//     } else {
//       ABSL_LOG(INFO) << idx;
//     }
//   }
class TopologicalSorter {
 public:
  explicit TopologicalSorter(int num_nodes);
  TopologicalSorter(const TopologicalSorter&) = delete;
  TopologicalSorter& operator=(const TopologicalSorter&) = delete;

  // Adds a directed edge with the given endpoints to the graph.
  void AddEdge(int from, int to);

  // Visits the least node in topological order over the current set of
  // nodes and edges, and marks that node as visited.
  // The repeated calls to GetNext() will visit all nodes in order.  Writes the
  // newly visited node into *node_index and returns true with *cyclic set to
  // false (assuming the graph has not yet been discovered to be cyclic).
  // Returns false if all nodes have been visited, or if the graph is
  // discovered to be cyclic, in which case *cyclic is also set to true.
  bool GetNext(int* node_index, bool* cyclic,
               std::vector<int>* output_cycle_nodes);

 private:
  // Finds the cycle.
  void FindCycle(std::vector<int>* cycle_nodes);

  const int num_nodes_;
  // Outoging adjacency lists.
  std::vector<std::vector<int>> adjacency_lists_;

  // If true, no more AddEdge() can be called.
  bool traversal_started_ = false;
  int num_nodes_left_;
  std::priority_queue<int, std::vector<int>, std::greater<int>>
      nodes_with_zero_indegree_;
  std::vector<int> indegree_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_TOPOLOGICALSORTER_H_
