from ast import literal_eval
from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from dataclasses import dataclass
from functools import reduce
from itertools import chain
from operator import mul
from typing import List, Dict, Tuple, Optional
import heapq
import math
import numpy as np
import sys
import tqdm

if len(sys.argv) == 1:
    filename = 'input.txt'
else:
    arg = sys.argv[1]
    filename = f'test_{arg}.txt'
print(filename)

with open(filename) as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]

debug = True

class Snail:
    def __init__(self, data, level, identity='l', parent=None):
        self.data = data
        self.val = None  # child if set
        self.type = None
        self.left = None
        self.right = None
        self.level = level
        self.parent = parent
        self.identity = identity
        self._traversal = None
        self._ll_traversal = None
        self.reduced = False

        # for traversal
        self.prev = None
        self.next = None

        if isinstance(data, int):
            self.val = data
            self.type = 'int'
        elif isinstance(data, list) and len(data) == 2:
            left, right = data
            if isinstance(left, int) and isinstance(right, int):
                self.val = data
                self.type = 'pair'
            else:
                self.left = Snail(left, level+1, 'l', parent=self)
                self.right = Snail(right, level+1, 'r', parent=self)
        else:
            raise ValueError(data)


    # includes leaves only
    def _inorder_traversal(self):
        if self._traversal:
            return self._traversal

        traversal = []
        if self.type:
            traversal.append(self)
            return traversal
        else:
            if self.left:
                traversal.extend(self.left._inorder_traversal())
            if self.right:
                traversal.extend(self.right._inorder_traversal())
        
        self._traversal = traversal
        return traversal


    def __repr__(self):
        if self.val is not None:
            return str(self.val)
            #return f"({str(self.val)}, {self.level})"
        else:
            return ""


    def ll(self):
        if self._ll_traversal:
            return self._ll_traversal

        traversal = self._inorder_traversal()
        head = traversal[0]
        curr = head
        for node in traversal[1:]:
            curr.next = node
            prev = curr
            curr = curr.next
            curr.prev = prev
        return head


    def explode(self, head):
        curr = head
        while curr:
            if curr.type == 'pair' and curr.level >= 4:
                lval, rval = curr.val
                if curr.prev is not None:
                    if curr.prev.type == 'pair':
                        curr.prev.val[1] += lval
                    elif curr.prev.type == 'int':
                        curr.prev.val += lval
                if curr.next is not None:
                    if curr.next.type == 'int':
                        curr.next.val += rval
                    elif curr.next.type == 'pair':
                        curr.next.val[0] += rval

                # reform self
                curr.val = 0
                curr.type = 'int'

                # merge?
                prev = curr.prev
                tail = curr.next
                if curr.identity == 'l' and curr.parent.right is not None and curr.parent.right.type == 'int':
                    curr.parent.val = [curr.val, curr.parent.right.val]
                    curr.parent.type = 'pair'
                    curr.parent.left = None
                    curr.parent.right = None
                    # merge with right
                    # [(6, 4), 0] becomes [0, 4]
                    tail = curr.next.next
                    curr = curr.parent
                elif curr.identity == 'r' and curr.parent.left is not None and curr.parent.left.type == 'int':
                    curr.parent.val = [curr.parent.left.val, curr.val]
                    curr.parent.type = 'pair'
                    curr.parent.left = None
                    curr.parent.right = None
                    # merge with left
                    # [0, (6, 4)] becomes [6, 0]
                    prev = curr.prev.prev
                    curr = curr.parent

                # fix traversal
                curr.prev = prev
                curr.next = tail

                if prev is not None:
                    prev.next = curr
                else:
                    head = curr
                if tail is not None:
                    tail.prev = curr
                if debug:
                    print(self.tree_to_list())
                    # import pdb; pdb.set_trace()
                
                return self.explode(head)
            else:
                curr = curr.next

        # traversed to end eithout exploding anything
        return head


    def split(self, head):
        curr = head
        while curr:
            if curr.type == 'pair' and curr.val[0] >= 10:
                lval, rval = curr.val[0] // 2, curr.val[0] // 2 + (curr.val[0] % 2)
                # treat self as updated right, insert left
                # [13, 0] -> [[6, 7], 0]
                grandpa = curr.parent
                parent = copy(grandpa)
                parent.level += 1
                parent.next = None
                parent.prev = None
                parent.val = None
                parent.type = None
                parent.parent = grandpa
                if curr.identity == 'l':
                    grandpa.left = parent
                    parent.identity = 'l'
                elif curr.identity == 'r':
                    grandpa.right = parent
                    parent.identity = 'r'
                # now update parent's children

                lchild = copy(curr)
                rchild = curr
                parent.left = lchild
                parent.right = rchild
                lchild.parent = parent
                rchild.parent = parent
                lchild.identity = 'l'
                rchild.identity = 'r'
                
                lchild.level += 1 
                rchild.level += 1
                lchild.next = rchild
                rchild.prev = lchild

                rchild.val = curr.val[1]
                rchild.type = 'int'
                lchild.val = [lval, rval]
                lchild.type = 'pair'

                # curr is rchild
                prev = curr.prev.prev
                tail = curr.next
                lchild.prev = prev
                if prev is not None:
                    prev.next = lchild
                else:
                    head = lchild
                rchild.next = tail
                if tail is not None:
                    tail.prev = rchild
                curr = lchild
            elif curr.type == 'pair' and curr.val[1] >= 10:
                lval, rval = curr.val[1] // 2, curr.val[1] // 2 + (curr.val[1] % 2)
                # treat self as updated left, insert right
                # [0, 13] -> [0, [6, 7]]

                grandpa = curr.parent
                parent = copy(grandpa)
                parent.level += 1
                parent.next = None
                parent.prev = None
                parent.val = None
                parent.type = None
                parent.parent = grandpa
                if curr.identity == 'l':
                    grandpa.left = parent
                    parent.identity = 'l'
                elif curr.identity == 'r':
                    grandpa.right = parent
                    parent.identity = 'r'
                # now update parent's children

                rchild = copy(curr)
                lchild = curr
                parent.left = lchild
                parent.right = rchild
                lchild.parent = parent
                rchild.parent = parent
                lchild.identity = 'l'
                rchild.identity = 'r'

                lchild.level += 1 
                rchild.level += 1
                lchild.next = rchild
                rchild.prev = lchild

                lchild.val = curr.val[0]
                lchild.type = 'int'
                rchild.val = [lval, rval]
                rchild.type = 'pair'

                prev = curr.prev
                tail = curr.next.next  # curr is lchild
                lchild.prev = prev
                if prev is not None:
                    prev.next = lchild
                else:
                    head = lchild
                rchild.next = tail
                if tail is not None:
                    tail.prev = rchild
                
                curr = rchild
            elif curr.type == 'int' and curr.val >= 10:
                # split
                lval, rval = curr.val // 2, curr.val // 2 + (curr.val % 2)
                curr.type = 'pair'
                curr.val = [lval, rval]                
            else:
                # no splits
                curr = curr.next
                continue

            if debug:
                print(self.tree_to_list())
                # import pdb; pdb.set_trace()

            # we split this iteration
            return self._reduce(head)
        
        # made it through without splits
        return head

    
    def _reduce(self, head):
        # explode loop
        head = self.explode(head)
        # split loop
        head = self.split(head)
        return head

    def reduce(self):
        head = self.ll()
        if debug:
            print(self.tree_to_list())

        head = self._reduce(head)
        self._ll_traversal = head
        self.data = self.tree_to_list()
        self.reduced = True

    def tree_to_list(self):
        if self.val is not None: return self.val
        res = []
        if self.left is not None:
            res.append(self.left.tree_to_list())
        if self.right is not None:
            res.append(self.right.tree_to_list())
        return res

    def __radd__(self, other):
        if isinstance(other, int):
            # sum starts with 0
            return self

        if not isinstance(other, Snail):
            return NotImplemented

        if not self.reduced:
            self.reduce()
        if not other.reduced:
            other.reduce()

        # both inputs are lists
        combined_data = [self.data, other.data]
        result = Snail(combined_data, 0)
        print('reducing result')
        result.reduce()
        
        return result

    __add__ = __radd__

    def magnitude(self):
        node = self
        if node.type == 'int':
            return node.val
        elif node.type == 'pair':
            return 3*node.val[0] + 2*node.val[1]
        else:
            ladd = 0 if not node.left else node.left.magnitude()
            radd = 0 if not node.right else node.right.magnitude()
            return 3*ladd + 2*radd


print('reducing p')
p = Snail([[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]], 0)
# p.reduce()
print(p.tree_to_list())

print('magnitude of m')
#m = Snail([[1,2],[[3,4],5]], 0)
#print(m.magnitude())

r = Snail([[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]], 0)
t = Snail([7,[[[3,7],[4,3]],[[6,3],[8,8]]]], 0)
print('r+t')
# rt = r+t
# rt.reduce()


snail_nums = []
for line in lines:
    snail_num = Snail(literal_eval(line), 0)
    snail_nums.append(snail_num)

final = 0
debug = True
print('final')
# for snail_num in snail_nums:
#     final += snail_num
#     print(final.tree_to_list())
#     # import pdb; pdb.set_trace()
# print(final.magnitude())
# import sys; sys.exit()

final = sum(snail_nums)
print(final.tree_to_list())
print(final.magnitude())

