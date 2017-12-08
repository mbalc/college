package pl.edu.mimuw.weather.event;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class WeatherAirQualityUpdateEvent extends WeatherStateEvent {

    private final Float pm025;
    private final Float pm100;

    public WeatherAirQualityUpdateEvent (float pm025, float pm100) {
        this.pm025 = pm025;
        this.pm100 = pm100;
    }

    public Float getPm025 () {
        return pm025;
    }

    public Float getPm100 () {
        return pm100;
    }

    @Override
    public String toString () {
        return "WeatherAirQualityUpdateEvent{" +
                "pm025=" + pm025 +
                ", pm100=" + pm100 +
                '}';
    }
}
