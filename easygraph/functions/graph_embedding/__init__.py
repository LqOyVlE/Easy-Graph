from .deepwalk import *
from .NOBE import *
from .node2vec import *


try:
    from .line import *
    from .sdne import *
except:
    print(
        "Warning raise in module:graph_embedding. Please install packages pytorch"
        "before you use functions related to graph_embedding"
    )


# from .sdne import SDNE
