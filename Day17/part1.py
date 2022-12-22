from copy import deepcopy
from typing import Optional

part1 = False
DEBUG_PRINT = False

with open("./input") as f:
    input = f.read()

nof_pieces = 2022 if part1 else 1000000000000


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, o):
        return Point(self.x + o.x, self.y + o.y)

    def __iter__(self):
        yield self.x
        yield self.y


LEFT = Point(-1, 0)
RIGHT = Point(1, 0)
DOWN = Point(0, -1)

CAVE_WIDTH = 7
EMPTY = "."
ROCK = "#"


class Piece:
    def __init__(self, block_list):
        self.blist = [Point(p[0], p[1]) for p in block_list]
        self.bl_corner = Point(
            min([x for x, _ in self.blist]),
            min([y for _, y in self.blist]),
        )
        self.height = abs(max([y for [_, y] in self.blist]) - self.bl_corner.y) + 1


class Cave:
    def __init__(self):
        self.impl = []
        self.curr_p: Optional[Piece] = None
        self.curr_ppos: Optional[Point] = None
        self.relative_height = 0
        self.erased_lines = 0
        self.cycle_detected = False
        self.cycle_map = dict()
        self.fallen_pieces = 0
        self.PIECE_LIST = (
            Piece([(0, 0), (1, 0), (2, 0), (3, 0)]),
            Piece([(0, 1), (1, 0), (1, 1), (1, 2), (2, 1)]),
            Piece([(0, 0), (1, 0), (2, 0), (2, 1), (2, 2)]),
            Piece([(0, 0), (0, 1), (0, 2), (0, 3)]),
            Piece([(0, 0), (0, 1), (1, 0), (1, 1)]),
        )

    def get_piece(self):
        return self.PIECE_LIST[self.fallen_pieces % len(self.PIECE_LIST)]

    @property
    def height(self):
        return self.relative_height + self.erased_lines

    def __getitem__(self, k):
        return self.impl[k.y][k.x]

    def __setitem__(self, k, v):
        self.impl[k.y][k.x] = v

    def extend_to(self, N):
        if len(self.impl) < N:
            for _ in range(N - len(self.impl)):
                self.impl.extend(deepcopy([[EMPTY] * CAVE_WIDTH]))

    def is_valid_pos(self, P):
        x, y = P
        return x >= 0 and x < CAVE_WIDTH and y >= 0 and self[P] == EMPTY

    def get_float_points(self):
        assert self.curr_p
        return [p + self.curr_ppos for p in self.curr_p.blist]

    def spawn(self):
        self.curr_p = self.get_piece()
        self.curr_ppos = Point(2, self.relative_height + 3) + self.curr_p.bl_corner
        self.extend_to(self.relative_height + self.curr_p.height + 3)

    def settle(self):
        assert self.curr_ppos is not None
        assert self.curr_p is not None
        for p in self.curr_p.blist:
            self[self.curr_ppos + p] = ROCK
        self.relative_height = max(
            self.curr_ppos.y + self.curr_p.height, self.relative_height
        )
        self.curr_p = None
        self.curr_ppos = None

        flood_detector = [-1] * CAVE_WIDTH
        for j in range(CAVE_WIDTH):
            for i in reversed(range(len(self.impl))):
                if self.impl[i][j] == ROCK:
                    flood_detector[j] = i
                    break
        keep_from = min(flood_detector)
        if keep_from >= 0:
            del self.impl[:keep_from]
            self.erased_lines += keep_from
            self.relative_height -= keep_from
            if not self.cycle_detected:
                flood_detector_tuple = (
                    tuple([x - keep_from for x in flood_detector]),
                    get_next_dir.index,
                    self.fallen_pieces % len(self.PIECE_LIST),
                )
                if flood_detector_tuple in self.cycle_map:
                    self.cycle_detected = True
                    cycle_piece_idx, cycle_height = self.cycle_map[flood_detector_tuple]
                    piece_cycle_size = self.fallen_pieces - cycle_piece_idx
                    nof_cycles = (
                        nof_pieces - (self.fallen_pieces + 1)
                    ) // piece_cycle_size
                    self.fallen_pieces += nof_cycles * piece_cycle_size
                    self.erased_lines += (self.height - cycle_height) * nof_cycles
                else:
                    self.cycle_map[flood_detector_tuple] = (
                        self.fallen_pieces,
                        self.height,
                    )
        self.fallen_pieces += 1

    def move(self, dir):
        assert self.curr_ppos is not None
        assert self.curr_p is not None
        if all(self.is_valid_pos(p + self.curr_ppos + dir) for p in self.curr_p.blist):
            self.curr_ppos += dir
        elif dir is DOWN:
            self.settle()
            return False
        return True

    def print(self):
        if not DEBUG_PRINT:
            return
        print()
        for i in reversed(range(0, len(self.impl))):
            pstr = "|"
            for j in range(0, CAVE_WIDTH):
                if (
                    cave.curr_ppos is not None
                    and cave.curr_p is not None
                    and any(i == y and j == x for x, y in self.get_float_points())
                ):
                    pstr += "@"
                else:
                    pstr += self.impl[i][j]
            pstr += "|"
            print(pstr)
        print("+" + "-" * CAVE_WIDTH + "+")


def get_next_dir():
    ret = LEFT if input[get_next_dir.index] == "<" else RIGHT
    get_next_dir.tot_index += 1
    get_next_dir.index = (get_next_dir.index + 1) % len(input)
    return ret


get_next_dir.index = 0
get_next_dir.tot_index = 0


cave = Cave()


while cave.fallen_pieces < nof_pieces:
    cave.spawn()
    while True:
        cave.move(get_next_dir())
        if not cave.move(DOWN):
            break

print(cave.height)
