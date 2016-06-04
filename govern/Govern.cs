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
        public List<Vertex> Vertices { get; set; }
        public List<Edge> Edges { get; set; }

        public Graph()
        {
            Vertices = new List<Vertex>();
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

    class TarjanAlgorithm
    {
        public static List<Vertex> GetTopologicalOrder(Graph graph)
        {
            var topologicalOrder = new List<Vertex>();
            var unvisitedVertices = new HashSet<string>(graph.Vertices.Select(v => v.Label));
            var visitedStatus = graph.Vertices.ToDictionary(k => k.Label, v => VertexStatus.NotVisited);

            while (unvisitedVertices.Any())
            {
                var unvisitedVertex = graph.Vertices.Where(v => v.Label == unvisitedVertices.First()).Single();
                DfsRecurse(unvisitedVertex, unvisitedVertices, visitedStatus, topologicalOrder);
            }

            return topologicalOrder;
        }

        private static void DfsRecurse(Vertex vertex, HashSet<string> unvisitedVertices, Dictionary<string, VertexStatus> visitedStatus, List<Vertex> topologicalOrder)
        {
            if(visitedStatus[vertex.Label] == VertexStatus.Visited)
            {
                throw new NotDirectedAcyclicGraphException();
            }

            if(visitedStatus[vertex.Label] == VertexStatus.NotVisited)
            {
                unvisitedVertices.Remove(vertex.Label);
                visitedStatus[vertex.Label] = VertexStatus.Visited;

                var neighbors = vertex.OutboundEdges.Select(e => e.EndVertex).ToList();

                foreach(var neighbor in neighbors)
                {
                    DfsRecurse(neighbor, unvisitedVertices, visitedStatus, topologicalOrder);
                }

                visitedStatus[vertex.Label] = VertexStatus.VisitedAndResolved;
                topologicalOrder.Add(vertex);
            }
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
            string inputFileName = args.Length >= 2 ? args[0] : "govern.in";
            string outputFileName = args.Length >= 2 ? args[1] : "govern.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string fileName)
        {
            var inputData = new InputData();

            foreach(var line in File.ReadLines(fileName))
            {
                var splittedLine = line.Split();
                var startVertexLabel = splittedLine[0];
                var endVertexLabel = splittedLine[1];

                var startVertex = inputData.Graph.Vertices.SingleOrDefault(v => v.Label == startVertexLabel);
                var endVertex = inputData.Graph.Vertices.SingleOrDefault(v => v.Label == endVertexLabel);

                if(startVertex == null)
                {
                    startVertex = new Vertex(startVertexLabel, new List<Edge>());
                    inputData.Graph.Vertices.Add(startVertex);
                }

                if(endVertex == null)
                {
                    endVertex = new Vertex(endVertexLabel, new List<Edge>());
                    inputData.Graph.Vertices.Add(endVertex);
                }

                var edge = new Edge(startVertex, endVertex);
                startVertex.OutboundEdges.Add(edge);
                inputData.Graph.Edges.Add(edge);
            }

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var topologicalOrder =  TarjanAlgorithm.GetTopologicalOrder(inputData.Graph);
            return new OutputData { TopologicalOrder = topologicalOrder };
        }

        private static void WriteOutput(string fileName, OutputData outputData)
        {
            File.WriteAllLines(fileName, outputData.TopologicalOrder.Select(v => v.Label));
        }
    }
}
