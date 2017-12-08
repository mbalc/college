package chmura.przykłady.ProducenciKonsumenci;

class Opcje {
    static final int bufor = 2;
    static final int iluProducentow = 8;
    static final int iluKonsumentow = 8;

    // czas w milisekundach, jednorodne prawdopodobieństwo dla czasów z zakresu
    static final int[] zakresCzasuProd = new int[]{2000, 3000};
    static final int[] zakresCzasuKons = new int[]{4000, 5000};
    static final int[] zakresCzasuUmieszczania = new int[]{0, 0}; // tylko do testów - równoznaczne ze zwiększeniem zakresCzasuProd
    static final int[] zakresCzasuPobierania = new int[]{0, 0}; // tylko do testów - równoznaczne ze zwiększeniem zakresCzasuKons
    static final double mnożnikCzasu = 1.0;
}
