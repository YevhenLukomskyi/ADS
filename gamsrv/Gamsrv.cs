using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GamsrvCS
{
    #region Graph representation

    class Vertex
    {
        public string Label { get; set; }
        public List<Edge> OutboundEdges { get; set; }

        public Vertex(string label, List<Edge> incomingEdges, List<Edge> outboundEdges)
        {
            Label = label;
            OutboundEdges = outboundEdges;
        }

        public override string ToString()
        {
            return string.Format("Label: {0}, Edges: {1}",
                Label, string.Join(", ", OutboundEdges.Select(v => v.ToString()).ToList()));
        }
    }

    class Edge
    {
        public Vertex StartVertex { get; set; }
        public Vertex EndVertex { get; set; }
        public long Weight { get; set; }

        public Edge(Vertex startVertex, Vertex endVertex, int weight)
        {
            StartVertex = startVertex;
            EndVertex = endVertex;
            Weight = weight;
        }

        public override string ToString()
        {
            return string.Format("{0} -> {1}", StartVertex.Label, EndVertex.Label);
        }
    }

    class Graph
    {
        public Dictionary<string, Vertex> Vertices { get; set; }
        public List<Edge> Edges { get; set; }

        public Graph()
        {
            Vertices = new Dictionary<string, Vertex>();
            Edges = new List<Edge>();
        }
    }

    #endregion

    #region Input/Output data

    class InputData
    {
        public int VertexCount { get; set; }
        public int EdgeCount { get; set; }
        public HashSet<string> Clients { get; set; }
        public Graph Graph { get; set; }

        public InputData()
        {
            Clients = new HashSet<string>();
            Graph = new Graph();
        }
    }

    class OutputData
    {
        public long MaxLatency { get; set; }
    }

    #endregion

    #region Dijkstra algorithm

    class DistanceComparer : IComparer<Tuple<string, long>>
    {
        public int Compare(Tuple<string, long> x, Tuple<string, long> y)
        {
            return x.Item2.CompareTo(y.Item2);
        }
    }

    static class DijkstraAlgorithm
    {
        public static Dictionary<string, long> GetMinDistances(Vertex startVertex, Graph graph)
        {
            var distances = graph.Vertices.Keys.ToDictionary(k => k, v => long.MaxValue);
            distances[startVertex.Label] = 0;

            var sortedList = new List<Tuple<string, long>>();
            sortedList.Add(new Tuple<string, long>(startVertex.Label, 0));

            while (sortedList.Any())
            {
                var minDistVertex = graph.Vertices[sortedList[0].Item1];
                var minDist = sortedList[0].Item2;
                sortedList.RemoveAt(0);

                foreach(var edge in minDistVertex.OutboundEdges)
                {
                    var neighborVertex = edge.EndVertex;
                    var alternativeDistance = minDist + edge.Weight;

                    if (alternativeDistance < distances[neighborVertex.Label])
                    {
                        distances[neighborVertex.Label] = alternativeDistance;
                        sortedList.Add(new Tuple<string, long>(neighborVertex.Label, alternativeDistance));
                    }
                }

                sortedList.Sort(new DistanceComparer());
            }

            return distances;
        }
    }

    #endregion

    class Program
    {
        static void Main(string[] args)
        {
            string inputFileName = args.Length >= 2 ? args[0] : "gamsrv.in";
            string outputFileName = args.Length >= 2 ? args[1] : "gamsrv.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string fileName)
        {
            var inputData = new InputData();

            var lines = File.ReadLines(fileName).ToList();
            inputData.VertexCount = int.Parse(lines[0].Split()[0]);
            inputData.EdgeCount = int.Parse(lines[0].Split()[1]);
            inputData.Clients = new HashSet<string>(lines[1].Split());
            lines.RemoveRange(0, 2);

            foreach (var line in lines)
            {
                var splittedLine = line.Split();
                var startVertexLabel = splittedLine[0];
                var endVertexLabel = splittedLine[1];
                var weight = int.Parse(splittedLine[2]);

                Vertex startVertex;
                Vertex endVertex;

                if (!inputData.Graph.Vertices.TryGetValue(startVertexLabel, out startVertex))
                {
                    startVertex = new Vertex(startVertexLabel, new List<Edge>(), new List<Edge>());
                    inputData.Graph.Vertices.Add(startVertexLabel, startVertex);
                }

                if (!inputData.Graph.Vertices.TryGetValue(endVertexLabel, out endVertex))
                {
                    endVertex = new Vertex(endVertexLabel, new List<Edge>(), new List<Edge>());
                    inputData.Graph.Vertices.Add(endVertexLabel, endVertex);
                }

                var edge = new Edge(startVertex, endVertex, weight);
                startVertex.OutboundEdges.Add(edge);
                inputData.Graph.Edges.Add(edge);

                var reverseEdge = new Edge(endVertex, startVertex, weight);
                endVertex.OutboundEdges.Add(reverseEdge);
                inputData.Graph.Edges.Add(reverseEdge);
            }

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var latency = long.MaxValue;

            foreach (var vertex in inputData.Graph.Vertices.Values)
            {
                if (inputData.Clients.Contains(vertex.Label))
                {
                    continue;
                }
                var maxLatency = DijkstraAlgorithm.GetMinDistances(vertex, inputData.Graph).
                    Where(v=>inputData.Clients.Contains(v.Key)).Select(v=>v.Value).Max();
                if (latency > maxLatency)
                {
                    latency = maxLatency;
                }
            }

            return new OutputData { MaxLatency = latency };
        }

        private static void WriteOutput(string fileName, OutputData outputData)
        {
            File.WriteAllLines(fileName, new[] { outputData.MaxLatency.ToString() });
        }
    }
}
