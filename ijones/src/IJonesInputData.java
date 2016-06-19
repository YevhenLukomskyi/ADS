public class IJonesInputData {
    private int width;
    private int height;
    private String[] words;

    public IJonesInputData(int width, int height, String[] words) {
        this.width = width;
        this.height = height;
        this.words = words;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public String[] getWords() {
        return words;
    }
}
