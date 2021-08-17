import networkx as nx
from karateclub import EdMot
import pandas as pd


def generate_graph(graph: nx.Graph, graph_input_file: str):

    with open(graph_input_file, 'r') as f:
        nodes_size = int(f.readline())

        for _id in range(nodes_size):
            graph.add_node(_id)

        while True:
            edge_line = f.readline()
            if not edge_line:
                break
            edge_attrs = edge_line.split(' ')
            u, v, w = int(edge_attrs[0]), int(edge_attrs[1]), float(edge_attrs[2])
            graph.add_edge(u, v, weight=w)

    return nodes_size


input_graph_files = ['2020-10-15.txt.graph']
input_unique_hashtags_files = ['2020-10-15.txt.unique_hashtags']

for m in range(len(input_graph_files)):

    Graph = nx.Graph()
    all_nodes = generate_graph(Graph, input_graph_files[m])

    with open(input_unique_hashtags_files[m], 'r') as f:
        unique_hashtags = f.readlines()
        unique_hashtags_size = int(unique_hashtags.pop(0))

    tweets_size = all_nodes - unique_hashtags_size

    print("generate_graph ... done")

    S = [Graph.subgraph(c).copy() for c in nx.connected_components(Graph) if len(c) > 500]
    print("number of main Component : ", len(S))
    for i in S:
        print("number of nodes : ", len(i))

    # running community detection for every Components
    part = -1
    num_old_communities = 0
    for gg in S:
        part = part + 1
        print("community detection on part ", str(part), end=" ... ")

        main_nodes = list(gg.nodes())
        idx = 0
        new_gg = nx.Graph()
        names_dict = dict()
        for name in main_nodes:
            names_dict[name] = idx
            new_gg.add_node(idx)
            idx = idx + 1

        for node1, node2 in list(gg.edges()):
            new_gg.add_edge(names_dict[node1], names_dict[node2])

        communitymodel = EdMot()

        communitymodel.fit(new_gg)
        print("done")
        # print("extract community index ", end=" ... ")
        # ------------
        idx_community = communitymodel.get_memberships()
        d1 = dict()
        for i in range(len(new_gg.nodes())):
            d1[i] = (i, idx_community[i] + num_old_communities)

        ss = list()
        for i in idx_community:
            ss.append(idx_community[i] + num_old_communities)
        numberofcommunity = len(list(set(ss)))
        print("number of community : ", numberofcommunity)

        num_old_communities = num_old_communities + numberofcommunity

        print("save data into files ", end=" ... ")
        # --------------
        df_hashtags = pd.DataFrame()
        df_tweets = pd.DataFrame()
        hashtag_lst, community_hashtag_lst = list(), list()
        tweet_lst, community_tweet_lst = list(), list()
        for i in d1:
            if i >= tweets_size:
                hashtag_lst.append(unique_hashtags[i - tweets_size])
                community_hashtag_lst.append(d1[i][1])
            else:
                tweet_lst.append(main_nodes[i])
                community_tweet_lst.append(d1[i][1])
        df_hashtags['hsahtags'] = hashtag_lst
        df_hashtags['community'] = community_hashtag_lst
        df_tweets['tweets_id'] = tweet_lst
        df_tweets['community'] = community_tweet_lst
        # --------------
        df_hashtags.to_csv(input_graph_files[m].split('.')[0] + '_community_hashtags_' + str(part) + ".csv",
                           index=False)
        df_tweets.to_csv(input_graph_files[m].split('.')[0] + '_community_tweets_' + str(part) + ".csv",
                         index=False)
        print("done")
