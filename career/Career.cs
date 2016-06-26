using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Career
{
    class InputData
    {
        public int CountOfLevels { get; set; }
        public List<List<int>> Levels { get; set; }
    }

    class OutputData
    {
        public int MaxPossibleExperience { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string inputFileName = args.Length >= 2 ? args[0] : "career.in";
            string outputFileName = args.Length >= 2 ? args[1] : "career.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string inputFileName)
        {
            var inputData = new InputData();

            var allLines = File.ReadAllLines(inputFileName);
            inputData.CountOfLevels = int.Parse(allLines.First());

            inputData.Levels = new List<List<int>>();

            foreach (var line in allLines.Skip(1).ToList())
            {
                var levelValues = line.Split().Select(int.Parse).ToList();
                inputData.Levels.Add(levelValues);
            }

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var levels = inputData.Levels;
            var lastLevel = levels.Last();

            var solutions = new List<int>();
            solutions.AddRange(lastLevel);

            for (var i = levels.Count - 2; i >= 0; i--)
            {
                for (var j = 0; j < levels[i].Count; j++)
                {
                    solutions[j] = Math.Max(solutions[j], solutions[j + 1]) + levels[i][j];
                }
            }

            var maxExperience = solutions[0];
            return new OutputData {MaxPossibleExperience = maxExperience};
        }

        private static void WriteOutput(string outputFileName, OutputData outputData)
        {
            File.WriteAllText(outputFileName, outputData.MaxPossibleExperience.ToString());
        }
    }
}
