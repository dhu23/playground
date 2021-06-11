from collections import namedtuple


NodeItem = namedtuple('NodeItem', ['name', 'count'])


class _TopoState(object):

    def __init__(self, work_set=None):
        
        self.topo_order = []
        self.visited = set()
        self.work_set = work_set

    def update(self, node_item):
        name = node_item.name
        if name in self.visited:
            return

        if self.work_set is None:
            self.visited.add(name)
            self.topo_order.append(node_item)
        else:
            if name in self.work_set:
                self.work_set.discard(name)
                self.visited.add(name)
                self.topo_order.append(node_item)

    def is_done(self):
        return self.work_set is not None and not self.work_set # empty set

    def is_visited(self, node_name):
        return node_name in self.visited



def _post_order_traversal(dag, adj_func, nodes, state):
    '''iterative depth first traversal, without loop detection'''
    
    StackItem = namedtuple('StackItem', ['node', 'ready'])

    # using an iterative method to traverse the graph
    # dft_stack simulates recursive function calls
    dft_stack = []

    for node_name in nodes:
        if state.is_visited(node_name):
            continue

        if node_name not in dag: # if n not having adjacent list, it is a leaf
            state.update(NodeItem(name=node_name, count=0))
            if state.is_done():
                return state
            continue

        dft_stack.append(
            StackItem(node=NodeItem(node_name, None), ready=False))

        while dft_stack:
            top_stack_node = dft_stack.pop()

            if top_stack_node.ready:
                state.update(top_stack_node.node)
                if state.is_done():
                    return state
            else:
                adj_list = adj_func(dag, top_stack_node.node.name)
                dft_stack.append(
                    StackItem(
                        node=NodeItem(
                            name=top_stack_node.node.name, 
                            count=len(adj_list) if adj_list else 0
                        ),
                        ready=True)
                )
                if adj_list:
                    dft_stack.extend(
                        StackItem(
                            node=NodeItem(name=adj, count=None), 
                            ready=False
                        ) for adj in adj_list
                    )
    
    return state


def topological_order(
        dag, adj_func, nodes, source_first=False, include_connection=False):
    '''
    For a given graph, topologically sort all given nodes. 
    -----------
    paramenters:
    dag - directed acyclic graph represented as adjacent lists
    adj_func - func that takes the dag and a node and gives adjacent list
    nodes - names of vertices to be ordered
    source_first - the order of the topological sort
    include_connection - include connected nodes that are not in input nodes
    '''

    topo_state = _TopoState(None if include_connection else set(nodes))
    topo = _post_order_traversal(dag, adj_func, nodes, topo_state)
    
    ret = topo.topo_order

    if source_first:
        ret.reverse()
    return ret


def verify_topological_order(dag, adj_func, nodes, source_first_topo_order):
    
    for n in nodes:
        
        try:
            n_index = source_first_topo_order.index(n)
        except:
            # n is the node to check, has to be in the topo order
            return False

        adj_list = adj_func(dag, n)
        if adj_list:
            for adj in adj_list:
                # so n should be before adj
                
                try:
                    adj_index = source_first_topo_order.index(adj)
                    if n_index >= adj_index:
                        return False
                except:
                    # if adj vertex not found, it is okay
                    pass

    return True


def graph_connection(
        dag, topo_order, adj_func, inclusive=True):
    '''calculate subgroup values for each node through topological order'''

    sub_group_accum_map = {} # node to value
    for node_name, count in topo_order:
        adj_list = adj_func(dag, node_name)

        if adj_list:
            assert count == len(adj_list)
        else:
            assert count == 0
            adj_list = []
    
        # for a tree structure with Node N, and subgraphs S1, S2, ...
        # in inclusive case:
        # f(N) = union(N, union(f(Si))), f(N) includes all connected nodes
        # in non-inclusive case:
        # f'(N) = union(union(Si), f'(Si))
        sub_group_accum_map[node_name] = set()
        for adj in adj_list:
            sub_group_accum_map[node_name].update(sub_group_accum_map[adj])

        if inclusive:
            sub_group_accum_map[node_name].add(node_name)
        else:
            for adj in adj_list:
                sub_group_accum_map[node_name].add(adj)

    # align sub_group_accum_map with topological order
    return [sub_group_accum_map[node_name] for node_name, _ in topo_order]


