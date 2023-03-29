#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Node {
    int32_t parent = 0;
    int32_t height1 = 0; // max0 height
    int32_t height2 = 0; // max1 height
    int32_t lengthToOutermostPeak = 0;
    std::vector<int32_t> children;
};

void SetHeight(int32_t nodeIndex, std::vector<Node>& tree) {
    for (int32_t iNodeIndex : tree[nodeIndex].children) {
        SetHeight(iNodeIndex, tree);
        if (tree[iNodeIndex].height1 >= tree[nodeIndex].height2) {
            tree[nodeIndex].height2 = tree[iNodeIndex].height1 + 1;
            if (tree[nodeIndex].height2 > tree[nodeIndex].height1)
                std::swap(tree[nodeIndex].height1, tree[nodeIndex].height2);
        }
    }
}

void SetLengthToFurthestPeak(int32_t nodeIndex, std::vector<Node>& tree) {
    if (tree[nodeIndex].height1 + 1 == tree[tree[nodeIndex].parent].height1)
        tree[nodeIndex].lengthToOutermostPeak = tree[tree[nodeIndex].parent].height2 + 1;
    else
        tree[nodeIndex].lengthToOutermostPeak = tree[tree[nodeIndex].parent].height1 + 1;
    tree[nodeIndex].lengthToOutermostPeak = std::max(tree[tree[nodeIndex].parent].lengthToOutermostPeak + 1, tree[nodeIndex].lengthToOutermostPeak);
    for (int32_t iNodeIndex : tree[nodeIndex].children) {
        SetLengthToFurthestPeak(iNodeIndex, tree);
        tree[iNodeIndex].lengthToOutermostPeak = std::max(tree[iNodeIndex].lengthToOutermostPeak, tree[iNodeIndex].height1);
    }
}

void SetFurthestPeakForAllPeaks(std::vector<Node>& tree) {
    SetHeight(0, tree);
    for (int32_t iNodeIndex : tree[0].children) {
        SetLengthToFurthestPeak(iNodeIndex, tree);
        tree[iNodeIndex].lengthToOutermostPeak = std::max(tree[iNodeIndex].lengthToOutermostPeak, tree[iNodeIndex].height1);
    }
    tree[0].lengthToOutermostPeak = tree[0].height1;
}

int main() {
    int32_t nodeCount = 0;
    std::cin >> nodeCount;
    std::vector<Node> tree(nodeCount);
    for (int32_t i = 0; i < nodeCount - 1; ++i) {
        int32_t node1Index = 0;
        int32_t node2Index = 0;
        std::cin >> node1Index >> node2Index;
        if (node1Index > node2Index)
            std::swap(node1Index, node2Index);
        tree[node2Index].parent = node1Index;
        tree[node1Index].children.push_back(node2Index);
    }
    SetFurthestPeakForAllPeaks(tree);
    for (Node iNode : tree)
        std::cout << iNode.lengthToOutermostPeak << std::endl;
    return 0;
}
