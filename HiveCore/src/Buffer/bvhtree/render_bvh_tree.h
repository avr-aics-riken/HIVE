/*
 * LSGL - Large Scale Graphics Library
 *
 * Copyright (c) 2013 - 2015 Advanced Institute for Computational Science,
 *RIKEN.
 * All rights reserved.
 *
 * Copy from LSGL library. Depencence for SparseVolumeData
 */

#ifndef __LSGL_RENDER_BVH_TREE_H__
#define __LSGL_RENDER_BVH_TREE_H__

//#include "render_common.h"

// Use StackVector for optimization
#include "stack_container.h"

//
// Generic BVH tree class.
// This class is used for scene graph and sparse volume.
//

#include <ctime>
#include <vector>

namespace HIVE {
namespace render {

struct BVHNode {

  // local bbox
  double bmin[3];
  double bmax[3];

  // Child index offset
  int child[2]; ///< 0 or -1 = leaf.

  // Inner
  int axis; ///< Split axis

  // Leaf
  bool isLeaf;
  unsigned int nodeID;
};

struct BVHData {
  // local bbox
  double bmin[3];
  double bmax[3];

  unsigned int nodeID; ///< Node ID
};

struct BVHNodeIntersection {
  double tMin, tMax;   ///< Enter and exit distance.
  unsigned int nodeID; ///< BVH node id
  unsigned int rayID;
};

struct BVHNodeLocator {
  double pos[3];       ///< local position in BVH node.
  unsigned int nodeID; ///< BVH node id
  unsigned int rayID;
};

class BVHTree {
public:
  BVHTree() : isBuiltTree(false) {}
  ~BVHTree() {}

  // Add node to the list.
  void AddNode(BVHData &node) { m_nodes.push_back(node); }

  // Build toplevel BVH tree. Valid after adding nodes using AddNode().
  void BuildTree();

  // Trace ray into toplevel BVH tree and list up possible intersection list.
  bool Trace(StackVector<BVHNodeIntersection, 32> &isects, double rayorg[3],
             double raydir[3], double maxdist) const;

  // Find intersecting BVHNode for a given position.
  bool Locate(StackVector<BVHNodeLocator, 32> &locators,
              const double position[3]) const;

  std::vector<BVHNode> &GetNodesTree() { return m_nodesTree; }
  const std::vector<BVHNode> &GetNodesTree() const { return m_nodesTree; }

  void SetNodesTree(const std::vector<BVHNode> &nodesTree) {
    m_nodesTree = nodesTree;
  }

  // Get bounding box of this toplevel BVH tree.
  void BoundingBox(double bmin[3], double bmax[3]) const;

  void Clear() {
	  m_nodesTree.clear();
	  m_nodes.clear();
	  isBuiltTree = false;
  }

private:
  void ConstructTree(size_t indexRoot, size_t indexLeft, size_t indexRight);

  // Tree reprenstation of BVH
  std::vector<BVHNode> m_nodesTree;

  // Input BVH list.
  std::vector<BVHData> m_nodes;

  bool isBuiltTree;
};

} // namespace render
} // namespace lsgl

#endif // __LSGL_RENDER_BVH_TREE_H__
