class Region:
    def __init__(self, color, count):
        self.color = color
        self.count = count

    def is_greater(self, other):
        return self.count > other.count or (self.count == other.count and self.color <= other.color)

# Given a 2d array representing a NxN grid whose elements are colored pixels,
# we define a region R as a subset of the grid which satisfies the following:
# 1. All the pixels of R have the same color;
# 2. Every pair of pixels of R is connected by a path consisting of zero or more
# horizontal and/or vertical moves from a pixel to a neighboring pixel.
# This function finds the largest region of a grid, i.e. whose number of pixels
# is maximal, and returns this size along with the color of its pixels.
# If we have found more than one largest region, we return the "darkest" one, i.e.
# the region whose color value is minimal.
# The most obvious approach to solve this problem is to use a slightly modified
# version of any flooding algorithm used in computer graphics. It requires O(NxN)
# memory and, at each iteration, we have to look for a pixel which is not painted
# yet.
# The proposed solution below is an incremental algorithm whose time complexity
# is O(NxN) and space complexity is O(N) given a NxN grid. It scans the lines
# of the grid from the top to the bottom.
def find_most_adjacent(grid):
    regions = {}
    next_index = 0

    region_index = [[0] * len(grid), [0] * len(grid)]
    cur = 0
    prev = 1

    max_region = None

    # First row.
    for col in range(0, len(grid)):
        if col >= 1 and grid[0][col] == grid[0][col - 1]:
            region_index[cur][col] = region_index[cur][col - 1]
            regions[region_index[cur][col]].count += 1
        else:
            region_index[cur][col] = next_index
            regions[next_index] = Region(grid[0][col], 1)
            next_index += 1

    for row in range(1, len(grid)):
        cur, prev = prev, cur
        region_index[cur] = [None] * len(grid)

        # Look for connected pixels from the left side or the above line.
        for col in range(0, len(grid)):
            if grid[row][col] == grid[row - 1][col]:
                region_index[cur][col] = region_index[prev][col]
                regions[region_index[cur][col]].count += 1
            elif (col >= 1 and region_index[cur][col] is None and 
                region_index[cur][col - 1] is not None and grid[row][col] == grid[row][col -1]):
                region_index[cur][col] = region_index[cur][col - 1]
                regions[region_index[cur][col]].count += 1
    
        # Look for connected pixels from the right side.
        for col in range(len(grid) - 2, -1, -1):
            if (region_index[cur][col] is None and region_index[cur][col + 1] is not None and
                grid[row][col] == grid[row][col + 1]):
                region_index[cur][col] = region_index[cur][col + 1]
                regions[region_index[cur][col]].count += 1

        # Create new regions for unconnected pixels.
        for col in range(0, len(grid)):
            if region_index[cur][col] is None:
                if col == 0 or grid[row][col] != grid[row][col - 1]:
                    region_index[cur][col] = next_index
                    regions[next_index] = Region(grid[row][col], 1)
                    next_index += 1
                else:
                    region_index[cur][col] = region_index[cur][col - 1]
                    regions[region_index[cur][col]].count += 1

        # Merge connected regions having the same color.
        merge_table = {}
        for col in range(0, len(grid) - 1):
            if (region_index[cur][col] != region_index[cur][col + 1] and 
                regions[region_index[cur][col]].color == regions[region_index[cur][col + 1]].color):
                dest = merge_table.get(region_index[cur][col])
                if dest is None:
                    merge_table[region_index[cur][col + 1]] = region_index[cur][col]
                elif region_index[cur][col + 1] != dest:
                    merge_table[region_index[cur][col + 1]] = dest

        if len(merge_table) != 0:
            for src, dest in merge_table.items():
                regions.get(dest).count += regions.get(src).count
                del regions[src]
            for col in range(0, len(grid)):
                dest = merge_table.get(region_index[cur][col])
                if dest is not None:
                    region_index[cur][col] = dest

        # Remove regions that are not connected to the current row.
        # Determine the most adjacent region among them.
        to_delete = set(regions.keys())
        for col in range(0, len(grid)):
            to_delete.discard(region_index[cur][col])
        for index in to_delete:
            if max_region is None or regions[index].is_greater(max_region):
                max_region = regions[index]
            del regions[index]

    # Determine the most adjacent region.
    for _, region in regions.items():
        if max_region is None or region.is_greater(max_region):
            max_region = region

    return (max_region.color, max_region.count)
