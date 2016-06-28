using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Lngpok
{
    class InputData
    {
        public List<int> Cards { get; set; }
    }

    class OutputData
    {
        public int MaxSequenceLength { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string inputFileName = args.Length >= 2 ? args[0] : "lngpok.in";
            string outputFileName = args.Length >= 2 ? args[1] : "lngpok.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string inputFileName)
        {
            var inputData = new InputData();
            var allLines = File.ReadAllLines(inputFileName);

            foreach (var line in allLines)
            {
                inputData.Cards = line.Split().Select(int.Parse).ToList();
            }

            return inputData;
        }

        private static bool CanFillGapsWithJokers(List<int> sortedCards, int jokerCount, int left, int right)
        {
            var jCount = 0;
            for (var i = left; i < right; i++)
            {
                var diff = sortedCards[i + 1] - sortedCards[i];
                if (diff > 1)
                {
                    jCount += (diff - 1);
                }
            }
            return jCount <= jokerCount;
        }

        private static OutputData Solve(InputData inputData)
        {
            var jokerCount = inputData.Cards.Count(v => v == 0);

            var hashSet = new HashSet<int>();
            inputData.Cards.ForEach(v => hashSet.Add(v));

            var sortedCardsWithoutJokers = new List<int>();

            for (var i = 1; i <= 1000000; i++)
            {
                if (hashSet.Contains(i))
                {
                    sortedCardsWithoutJokers.Add(i);
                }
            }

            var maxSequenceLength = jokerCount;

            for (var sequenceBegin = 0; sequenceBegin < sortedCardsWithoutJokers.Count; sequenceBegin++)
            {
                var left = sequenceBegin;
                var right = sortedCardsWithoutJokers.Count - 1;

                while (left < right)
                {
                    var candidateSequenceEnd = (left + right + 1) / 2;

                    if (!CanFillGapsWithJokers(sortedCardsWithoutJokers, jokerCount, sequenceBegin, candidateSequenceEnd))
                    {
                        right = candidateSequenceEnd - 1;
                    }
                    else
                    {
                        left = candidateSequenceEnd;
                    }
                }

                var sequenceEnd = right;
                maxSequenceLength = Math.Max(maxSequenceLength, (sequenceEnd - sequenceBegin + 1) + jokerCount);
            }

            return new OutputData {MaxSequenceLength = maxSequenceLength};
        }

        private static void WriteOutput(string outputFileName, OutputData outputData)
        {
            File.WriteAllText(outputFileName, outputData.MaxSequenceLength.ToString());
        }
    }
}