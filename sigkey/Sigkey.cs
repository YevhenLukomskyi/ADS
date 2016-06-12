using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SigkeyCS
{
    class InputData
    {
        public int KeysCount { get; set; }
        public List<string> Keys { get; set; }
    }

    class OutputData
    {
        public int KeyPairCount { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string inputFileName = args.Length >= 2 ? args[0] : "sigkey.in";
            string outputFileName = args.Length >= 2 ? args[1] : "sigkey.out";

            InputData inputData = ReadInput(inputFileName);
            OutputData outputData = Solve(inputData);
            WriteOutput(outputFileName, outputData);
        }

        private static InputData ReadInput(string inputFileName)
        {
            var inputData = new InputData();

            var allLines = File.ReadAllLines(inputFileName);
            inputData.KeysCount = int.Parse(allLines.First());
            inputData.Keys = allLines.Skip(1).ToList();

            return inputData;
        }

        private static OutputData Solve(InputData inputData)
        {
            var keyMasks = new Dictionary<int, int>(inputData.KeysCount);

            foreach (var key in inputData.Keys)
            {
                var mostSignificantByteIndex = 0;
                var mask = 0;

                foreach (var keyChar in key.ToCharArray())
                {
                    var keyCharBitIndex = keyChar - 'a';
                    mask |= 1 << keyCharBitIndex;

                    if (keyCharBitIndex > mostSignificantByteIndex)
                    {
                        mostSignificantByteIndex = keyCharBitIndex;
                    }
                }

                keyMasks.Add(mask, ~mask & (1 << mostSignificantByteIndex + 1) - 1);
            }

            var keyPairCount = 0;

            foreach (var key in keyMasks.Keys)
            {
                var invMask = keyMasks[key];
                if (keyMasks.ContainsKey(invMask))
                {
                    keyPairCount++;
                }
            }

            return new OutputData { KeyPairCount = keyPairCount };
        }

        private static void WriteOutput(string outputFileName, OutputData outputData)
        {
            File.WriteAllText(outputFileName, outputData.KeyPairCount.ToString());
        }
    }
}
