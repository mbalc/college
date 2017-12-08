package pl.edu.mimuw.weather.event;

import java.util.ArrayList;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class WeatherPropertiesUpdateEvent extends WeatherStateEvent {

    public static final int PROPERTIES_COUNT = 6;

    private final Float temperature;
    private final Float windDirection;
    private final Float windForce;
    private final Float humidity;
    private final Float pressure;
    private final Float cloudiness;

    public WeatherPropertiesUpdateEvent (Float temperature, Float windDirection,
                                         Float windForce, Float humidity,
                                         Float pressure, Float cloudiness) {
        this.temperature = temperature;
        this.windDirection = windDirection;
        this.windForce = windForce;
        this.humidity = humidity;
        this.pressure = pressure;
        this.cloudiness = cloudiness;
    }

    public WeatherPropertiesUpdateEvent (Float tab[]) {
        assert(tab.length == PROPERTIES_COUNT);
        this.temperature = tab[0];
        this.windDirection = tab[1];
        this.windForce = tab[2];
        this.humidity = tab[3];
        this.pressure = tab[4];
        this.cloudiness = tab[5];
    }

    public Float getTemperature () {
        return temperature;
    }

    public Float getWindDirection () {
        return windDirection;
    }

    public Float getWindForce () {
        return windForce;
    }

    public Float getHumidity () {
        return humidity;
    }

    public Float getPressure () {
        return pressure;
    }

    public Float getCloudiness () {
        return cloudiness;
    }

    @Override
    public String toString () {
        return "WeatherPropertiesUpdateEvent{" +
                "temperature=" + temperature +
                ", windDirection=" + windDirection +
                ", windForce=" + windForce +
                ", humidity=" + humidity +
                ", pressure=" + pressure +
                ", cloudiness=" + cloudiness +
                '}';
    }
}
