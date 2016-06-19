using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace IJonesCS
{
    class InputData
    {
        public int Height { get; set; }
        public int Width { get; set; }
        public List<string> Words { get; set; }
    }

    class OutputData
    {
        public long CountOfWays { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string inputFileName = args.Length >= 2 ? args[0] : "ijones.in";
            string outputFileName = args.Length >= 2 ? args[1] : "ijones.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string inputFileName)
        {
            var inputData = new InputData();

            var allLines = File.ReadAllLines(inputFileName);
            inputData.Width = int.Parse(allLines.First().Split()[0]);
            inputData.Height = int.Parse(allLines.First().Split()[1]);

            inputData.Words = allLines.Skip(1).ToList();

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var width = inputData.Width;
            var height = inputData.Height;
            var words = inputData.Words;

            var ways = new int[inputData.Height][];
            for (var i = 0; i < inputData.Height; i++)
            {
                ways[i] = new int[inputData.Width];
            }
            var dict = new Dictionary<char, int>();

            foreach (var letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ".ToLower().ToCharArray())
            {
                dict.Add(letter, 0);
            }

            for (var i = 0; i < height; i++)
            {
                ways[i][0] = 1;
                dict[words[i][0]] += 1;
            }

            for (var j = 1; j < width; j++)
            {
                for (var i = 0; i < height; i++)
                {
                    if (words[i][j] == words[i][j - 1])
                    {
                        ways[i][j] += dict[words[i][j]];
                    }
                    else
                    {
                        ways[i][j] = ways[i][j - 1];
                        ways[i][j] += dict[words[i][j]];
                    }
                }

                for (var i = 0; i < height; i++)
                {
                    dict[words[i][j]] += ways[i][j];
                }
            }

            var countOfWays = ways[0][width - 1] + ways[height - 1][width - 1];
            return new OutputData { CountOfWays = countOfWays };
        }

        private static void WriteOutput(string outputFileName, OutputData outputData)
        {
            File.WriteAllText(outputFileName, outputData.CountOfWays.ToString());
        }
    }
}
