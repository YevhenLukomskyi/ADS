using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace WChain
{
    class InputData
    {
        public int Count { get; set; }
        public List<string> Words { get; set; }
    }

    class OutputData
    {
        public int MaxSequenceLength { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string inputFileName = args.Length >= 2 ? args[0] : "wchain.in";
            string outputFileName = args.Length >= 2 ? args[1] : "wchain.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string inputFileName)
        {
            var inputData = new InputData();

            var allLines = File.ReadAllLines(inputFileName);
            inputData.Count = int.Parse(allLines.First());
            inputData.Words = allLines.Skip(1).ToList();

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var words = inputData.Words;
            words.Sort((x, y) => x.Length.CompareTo(y.Length));

            var solutions = words.TakeWhile(v => v.Length == 1).ToDictionary(word => word, word => 1);

            foreach (var word in words.SkipWhile(v => v.Length == 1))
            {
                var maxPossibleSequenceLength = 0;
                var charsToRemove = word.ToCharArray();

                foreach (var charToRemove in charsToRemove)
                {
                    var testWord = word.Replace(charToRemove.ToString(), string.Empty);
                    var testWordPossibleSequenceLength = 0;
                    if (solutions.TryGetValue(testWord, out testWordPossibleSequenceLength))
                    {
                        maxPossibleSequenceLength = Math.Max(testWordPossibleSequenceLength, maxPossibleSequenceLength);
                    }
                }

                maxPossibleSequenceLength++;
                solutions.Add(word, maxPossibleSequenceLength);
            }

            return new OutputData
            {
                MaxSequenceLength = solutions.Values.Max()
            };
        }

        private static void WriteOutput(string outputFileName, OutputData outputData)
        {
            File.WriteAllText(outputFileName, outputData.MaxSequenceLength.ToString());
        }
    }
}
