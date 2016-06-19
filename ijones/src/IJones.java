import java.io.*;
import java.math.BigInteger;
import java.util.HashMap;
import java.util.Map;

public class IJones {

    public static void main(String[] args) throws IOException {
        String inputFileName = args.length >= 2 ? args[0] : "ijones.in";
        String outputFileName = args.length >= 2 ? args[1] : "ijones.out";

        IJonesInputData inputData = readInput(inputFileName);
        IJonesOutputData outputData = solve(inputData);
        writeOutput(outputFileName, outputData);
    }

    private static IJonesInputData readInput(String inputFileName) throws IOException {
        File inputFile = new File(inputFileName);
        try (FileReader inputFileReader = new FileReader(inputFile)) {
            try (BufferedReader bufferedReader = new BufferedReader(inputFileReader)) {
                String[] theFirstLine = bufferedReader.readLine().split("\\s+");
                int width = Integer.valueOf(theFirstLine[0]);
                int height = Integer.valueOf(theFirstLine[1]);

                String[] words = new String[height];
                for (int i = 0; i < height; i++) {
                    words[i] = bufferedReader.readLine();
                }

                return new IJonesInputData(width, height, words);
            }
        }
    }

    private static IJonesOutputData solve(IJonesInputData inputData) {

        int width = inputData.getWidth();
        int height = inputData.getHeight();
        String[] words = inputData.getWords();

        BigInteger[] waysPrev = new BigInteger[height];
        BigInteger[] waysCurrent = new BigInteger[height];
        Map<Character, BigInteger> map = new HashMap<>();

        for(Character ch : "abcdefghijklmnopqrstuvwxyz".toCharArray()){
            map.put(ch, BigInteger.ZERO);
        }

        for(int i = 0; i < height; i++){
            waysPrev[i] = waysCurrent[i] = BigInteger.ZERO;
        }

        for(int i = 0; i < height; i++){
            waysPrev[i] = BigInteger.ONE;
            Character key  = words[i].charAt(0);
            map.put(key, map.get(key).add(BigInteger.ONE));
        }

        for(int j = 1; j < width; j++){
            for(int i = 0; i < height; i++){

                for(int k = 0; k < height; k++){
                    waysCurrent[i] = BigInteger.ZERO;
                }

                if(words[i].charAt(j) != words[i].charAt(j - 1)){
                    waysCurrent[i] = waysPrev[i];
                }

                waysCurrent[i] = waysCurrent[i].add(map.get(words[i].charAt(j)));
            }

            for(int i = 0; i < height; i++){
                Character key  = words[i].charAt(j);
                map.put(key, map.get(key).add(waysCurrent[i]));
            }

            for(int i = 0; i < height; i++){
                waysPrev[i] = waysCurrent[i];
            }
        }

        BigInteger countOfWays = height - 1 == 0? waysPrev[0]: waysPrev[0].add(waysPrev[height - 1]);
        return new IJonesOutputData(countOfWays);
    }

    private static void writeOutput(String outputFileName, IJonesOutputData outputData) throws IOException {
        try (Writer outputFileWriter = new FileWriter(outputFileName)) {
            outputFileWriter.write(String.valueOf(outputData.getCountOfWays()));
        }
    }
}
