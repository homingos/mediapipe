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

#include "mediapipe/framework/deps/topologicalsorter.h"

#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {

TEST(TopologicalSorterTest, NoConnection) {
  TopologicalSorter sorter(3);
  std::vector<int> expected_result({0, 1, 2});

  int visited = 0;
  int node_index;
  bool cyclic;
  std::vector<int> cycle_nodes;
  while (sorter.GetNext(&node_index, &cyclic, &cycle_nodes)) {
    EXPECT_EQ(expected_result[visited], node_index);
    ++visited;
  }
  ASSERT_FALSE(cyclic);
  EXPECT_EQ(3, visited);
}

TEST(TopologicalSorterTest, SimpleDAG) {
  TopologicalSorter sorter(5);
  sorter.AddEdge(4, 0);
  sorter.AddEdge(4, 1);
  sorter.AddEdge(4, 2);
  sorter.AddEdge(0, 3);
  sorter.AddEdge(1, 3);
  sorter.AddEdge(3, 2);
  std::vector<int> expected_result({4, 0, 1, 3, 2});

  int visited = 0;
  int node_index;
  bool cyclic;
  std::vector<int> cycle_nodes;
  while (sorter.GetNext(&node_index, &cyclic, &cycle_nodes)) {
    EXPECT_EQ(expected_result[visited], node_index);
    ++visited;
  }
  ASSERT_FALSE(cyclic);
  EXPECT_EQ(5, visited);
}

TEST(TopologicalSorterTest, DuplicatedEdges) {
  TopologicalSorter sorter(5);
  sorter.AddEdge(3, 2);
  sorter.AddEdge(4, 0);
  sorter.AddEdge(4, 2);
  sorter.AddEdge(4, 1);
  sorter.AddEdge(3, 2);
  sorter.AddEdge(4, 2);
  sorter.AddEdge(1, 3);
  sorter.AddEdge(0, 3);
  sorter.AddEdge(1, 3);
  sorter.AddEdge(3, 2);
  std::vector<int> expected_result({4, 0, 1, 3, 2});

  int visited = 0;
  int node_index;
  bool cyclic;
  std::vector<int> cycle_nodes;
  while (sorter.GetNext(&node_index, &cyclic, &cycle_nodes)) {
    EXPECT_EQ(expected_result[visited], node_index);
    ++visited;
  }
  ASSERT_FALSE(cyclic);
  EXPECT_EQ(5, visited);
}

TEST(TopologicalSorterTest, Cycle) {
  // Cycle: 1->3->2->1
  TopologicalSorter sorter(5);
  sorter.AddEdge(4, 0);
  sorter.AddEdge(4, 1);
  sorter.AddEdge(4, 2);
  sorter.AddEdge(0, 3);
  sorter.AddEdge(1, 3);
  sorter.AddEdge(3, 2);
  sorter.AddEdge(2, 1);

  int node_index;
  bool cyclic;
  std::vector<int> cycle_nodes;
  while (sorter.GetNext(&node_index, &cyclic, &cycle_nodes)) {
  }

  EXPECT_TRUE(cyclic);
  std::vector<int> expected_cycle({1, 3, 2});
  ASSERT_EQ(3, cycle_nodes.size());
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(expected_cycle[i], cycle_nodes[i]);
  }
}

}  // namespace mediapipe
