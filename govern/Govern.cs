using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace GovernCS
{
    #region Graph representation

    class Vertex
    {
        public string Label { get; set; }
        public List<Edge> OutboundEdges { get; set; }

        public Vertex(string label, List<Edge> outboundEdges)
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

        public Edge(Vertex startVertex, Vertex endVertex)
        {
            StartVertex = startVertex;
            EndVertex = endVertex;
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
        public Graph Graph { get; set; }

        public InputData()
        {
            Graph = new Graph();
        }
    }

    class OutputData
    {
        public List<Vertex> TopologicalOrder { get; set; }

        public OutputData()
        {
            TopologicalOrder = new List<Vertex>();
        }
    }

    #endregion

    #region Topological sorting

    enum VertexStatus
    {
        NotVisited,
        Visited,
        VisitedAndResolved
    }

    interface ITopologicalOrderFindAlgorithm
    {
        List<Vertex> GetTopologicalOrder(Graph graph);
    }

    class TarjanAlgorithmRecursive : ITopologicalOrderFindAlgorithm
    {
        public List<Vertex> GetTopologicalOrder(Graph graph)
        {
            var topologicalOrder = new List<Vertex>();
            var unvisitedVertices = new HashSet<string>(graph.Vertices.Select(v => v.Key));
            var visitedStatus = graph.Vertices.ToDictionary(k => k.Key, v => VertexStatus.NotVisited);

            while (unvisitedVertices.Any())
            {
                var unvisitedVertex = graph.Vertices[unvisitedVertices.First()];
                DfsRecurse(unvisitedVertex, unvisitedVertices, visitedStatus, topologicalOrder);
            }

            return topologicalOrder;
        }

        private void DfsRecurse(Vertex vertex, HashSet<string> unvisitedVertices, Dictionary<string, VertexStatus> visitedStatus, List<Vertex> topologicalOrder)
        {
            if (visitedStatus[vertex.Label] == VertexStatus.Visited)
            {
                throw new NotDirectedAcyclicGraphException();
            }

            if (visitedStatus[vertex.Label] == VertexStatus.NotVisited)
            {
                unvisitedVertices.Remove(vertex.Label);
                visitedStatus[vertex.Label] = VertexStatus.Visited;

                var neighbors = vertex.OutboundEdges.Select(e => e.EndVertex).ToList();

                foreach (var neighbor in neighbors)
                {
                    DfsRecurse(neighbor, unvisitedVertices, visitedStatus, topologicalOrder);
                }

                visitedStatus[vertex.Label] = VertexStatus.VisitedAndResolved;
                topologicalOrder.Add(vertex);
            }
        }
    }

    class TarjsnAlgorithmStack : ITopologicalOrderFindAlgorithm
    {
        public List<Vertex> GetTopologicalOrder(Graph graph)
        {
            var toplogicalOrder = DfsStack(graph.Vertices.First().Value, graph);
            return toplogicalOrder;
        }

        private List<Vertex> DfsStack(Vertex startVertex, Graph graph)
        {
            var topologicalOrder = new List<Vertex>();
            var stack = new Stack<Vertex>();
            stack.Push(startVertex);

            var unvisitedVertices = new HashSet<string>(graph.Vertices.Select(v => v.Key));
            var visitedStatus = graph.Vertices.ToDictionary(k => k.Key, v => VertexStatus.NotVisited);

            while (stack.Any())
            {
                var vertex = stack.Pop();
                visitedStatus[vertex.Label] = VertexStatus.Visited;
                unvisitedVertices.Remove(vertex.Label);

                var unvisitedNeighbors = new List<Vertex>();
                var neighbors = vertex.OutboundEdges.Select(e => e.EndVertex).ToList();

                foreach (var neighbor in neighbors)
                {
                    if (visitedStatus[neighbor.Label] == VertexStatus.Visited)
                    {
                        throw new NotDirectedAcyclicGraphException();
                    }

                    if (visitedStatus[neighbor.Label] == VertexStatus.NotVisited)
                    {
                        unvisitedNeighbors.Add(neighbor);
                    }
                }

                if (!unvisitedNeighbors.Any())
                {
                    visitedStatus[vertex.Label] = VertexStatus.VisitedAndResolved;
                    topologicalOrder.Add(vertex);
                }
                else
                {
                    stack.Push(vertex);
                    unvisitedNeighbors.ForEach(v => stack.Push(v));
                }

            }

            return topologicalOrder;
        }


    }

    class NotDirectedAcyclicGraphException : Exception
    {
    }

    #endregion

    class Program
    {
        static void Main(string[] args)
        {
            var watch = System.Diagnostics.Stopwatch.StartNew();

            string inputFileName = args.Length >= 2 ? args[0] : "govern.in";
            string outputFileName = args.Length >= 2 ? args[1] : "govern.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);

            watch.Stop();

            var elapsedMs = watch.ElapsedMilliseconds;
            Console.WriteLine(elapsedMs);
            Console.ReadKey();
        }

        private static InputData ReadInput(string fileName)
        {
            var inputData = new InputData();

            foreach (var line in File.ReadLines(fileName))
            {
                var splittedLine = line.Split();
                var startVertexLabel = splittedLine[0];
                var endVertexLabel = splittedLine[1];

                Vertex startVertex;
                Vertex endVertex;

                if (!inputData.Graph.Vertices.TryGetValue(startVertexLabel, out startVertex))
                {
                    startVertex = new Vertex(startVertexLabel, new List<Edge>());
                    inputData.Graph.Vertices.Add(startVertexLabel, startVertex);
                }

                if (!inputData.Graph.Vertices.TryGetValue(endVertexLabel, out endVertex))
                {
                    endVertex = new Vertex(endVertexLabel, new List<Edge>());
                    inputData.Graph.Vertices.Add(endVertexLabel, endVertex);
                }

                var edge = new Edge(startVertex, endVertex);
                startVertex.OutboundEdges.Add(edge);
                inputData.Graph.Edges.Add(edge);
            }

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var algorithm = new TarjsnAlgorithmStack();
            var topologicalOrder = algorithm.GetTopologicalOrder(inputData.Graph);
            return new OutputData { TopologicalOrder = topologicalOrder };
        }

        private static void WriteOutput(string fileName, OutputData outputData)
        {
            File.WriteAllLines(fileName, outputData.TopologicalOrder.Select(v => v.Label));
        }
    }
}
