import java.util.Scanner;

class Floyd {

    private int min (int a, int b) {
        if (a > b) return b;
        return a;
    }

    private void algorithm () {

        Scanner in = new Scanner (System.in);
        int nodeCount = in.nextInt();
        int[][] shortestPaths = new int[nodeCount][nodeCount];

        for (int i = 0; i < nodeCount; ++i) {
            for (int j = 0; j < nodeCount; ++j) {
                shortestPaths[i][j] = in.nextInt();
                if (shortestPaths[i][j] == 0) shortestPaths[i][j] = Integer.MAX_VALUE / 4;
            }
        }

        for (int i = 0; i < nodeCount; ++i) shortestPaths[i][i] = 0;

        for (int k = 0; k < nodeCount; ++k) {
            for (int i = 0; i < nodeCount; ++i) {
                for (int j = 0; j < nodeCount; ++j)
                    shortestPaths[i][j] = min(shortestPaths[i][j], shortestPaths[i][k] + shortestPaths[k][j]);
            }
        }
    }

    public static void main (String[] args) {
        Floyd f = new Floyd();
        f.algorithm ();
    }
}
